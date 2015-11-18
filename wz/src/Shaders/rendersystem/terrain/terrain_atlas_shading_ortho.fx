//*************************************************************************************************
// 内容:	地形着色
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-23
// 最后修改:	2013-05-02
//*************************************************************************************************
#include "terrain_common.h"

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
SD_POINT_CLAMP_CUBESAMPLE(0, sdPlanarTableSampler, 	sdPlanarTableTex);				//
SD_POINT_CLAMP_SAMPLE(1, sdGeomSampler, 			sdGeomBuf, 			false);		// 屏幕深度与法线缓存
SD_POINT_CLAMP_SAMPLE(2, sdLightSampler, 			sdLightBuf, 		false);		// 屏幕局部光照缓存
SD_LINEAR_WRAP_SAMPLE(3, sdBaseNormalSampler, 		sdBaseNormalTex, 	false);		// 地形基本法线贴图
SD_POINT_CLAMP_SAMPLE(4, sdTileSampler,				sdTileTex,			false);		// 地形TileMap
SD_LINEAR_WRAP_SAMPLE(5, sdBlendSampler,			sdBlendTex,			false);		// 地形BlendMap
SD_POINT_CLAMP_SAMPLE(6, sdAtlasTableSampler,		sdAtlasTableTex,	false);		// 地形漫反射贴图查询表
SD_LINEAR_WRAP_SAMPLE(7, sdDiffuseAtlasSampler,		sdDiffuseAtlasTex,	false);		// 地形漫反射贴图图集

//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3	vPos			: POSITION0;	// 屏幕矩形顶点
	float2	vUVSet0			: TEXCOORD0;	// 屏幕矩形纹理坐标
};

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos			: POSITION;		// 投影坐标
	float2	vUVSetScreenTex		: TEXCOORD0;	// 纠正半像素偏移的屏幕纹理坐标
	float3	vUVNearClipWorldPos	: TEXCOORD1;	// 当前点对应近裁剪面上的点的世界坐标
	float3	vUVFarClipWorldPos	: TEXCOORD2;	// 当前点对应远裁剪面上的点的世界坐标
	float3	vUVFarClipViewPos	: TEXCOORD3;	// 当前点对应远裁剪面上的点的观察坐标
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;

	// 投影坐标
	kOutput.vProjPos = float4(kInput.vPos, 1.f);
	
	// 纠正半像素偏移的屏幕纹理坐标
	kOutput.vUVSetScreenTex = kInput.vUVSet0 + g_vHalfPixelOffset;
	
	// 当前点对应近裁剪面上的点的世界坐标
	kOutput.vUVNearClipWorldPos = mul(float4(kInput.vPos.xy, 0.f, 1.f), g_mDepthToWorld).xyz;
	
	// 当前点对应远裁剪面上的点的世界坐标
	float4 vUVFarClipProjPos  = float4(kInput.vPos.xy, 1.f, 1.f);
	float4 vUVFarClipWorldPos = mul(vUVFarClipProjPos, g_mDepthToWorld);
	kOutput.vUVFarClipWorldPos = vUVFarClipWorldPos.xyz;
	
	// 当前点对应远裁剪面上的点的观察坐标
	//	1.需要g_mView的平移变换,所以要恢复float4
	// 	2.替换掉w分量是避免计算误差累积
	kOutput.vUVFarClipViewPos  = mul(float4(vUVFarClipWorldPos.xyz, 1.f), g_mView).xyz;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
float4 PS_Main_Planar(VS_OUTPUT kInput) : COLOR0
{
	// 获取地形深度
	float4 vGeoTex = tex2D(sdGeomSampler, kInput.vUVSetScreenTex);
	float fDepth = UnpackDepth(vGeoTex.xy);
	
	// 反算世界坐标(根据线性深度,对相机位置和远平面对应点位置进行插值)
	float3 vWorldPos = lerp(kInput.vUVNearClipWorldPos, kInput.vUVFarClipWorldPos, fDepth);
	
	// 计算当前点的地形相对UV(注意,这里没有偏移)
	float2 vUVSet = vWorldPos.xy * g_vRecipTerrainSize;
	
	
	// BaseNormalMap
	// @{
	// 根据UV采样BaseNormalMap(注意,这里没有偏移半像素,因为BaseNormalMap是Linear采样的)
	float2 vBaseNormalTex = tex2D(sdBaseNormalSampler, vUVSet).xy;
	
	// 解出世界空间法线
	float3 vWorldNormal;
	vWorldNormal.xy	= vBaseNormalTex.xy * 2.f - 1.f;
	vWorldNormal.z 	= sqrt(dot(float3(1.f, vWorldNormal.xy), float3(1.f, -vWorldNormal.xy)));
	// @}
	
	
	// TileMap
	// @{
	// 根据UV采样TileMap(这里从[0,1]恢复到[0,255]的图层索引区间)
	float4 vIndices = tex2D(sdTileSampler, vUVSet) * 255.f;
	// @}
	
	
	// BlendMap
	// @{
	// 计算新的UV(将采样点从BlendTex的中心点向背离Tile中心方向偏移)
	//	1.计算当前Fragment相对TexTile中心点的偏移(单位:TexTile)
	//	2.将当前Fragment相对TexTile中心点的偏移转化为对BlendMapPixel中心偏移(单位:BlendMapPixel)
	//	3.从UVSet减去对BlendMapPixel中心偏移(注意UVSet本身没有处理半像素偏移)
	//
	// WZ原式:
	//	float2 tileCenterOffset = frac(vUVSet * (2048.0 / 4.0)) - 0.5;
	//	vUVSet -= tileCenterOffset * (1.0 / 2048.0);
	//
	// 本地实现:
	//	float2 tileCenterOffset = frac(vUVSet * g_vTileMapSize) - 0.5;
	//	float2 vUVSet2 = vUVSet - tileCenterOffset * g_vRecipBlendMapSize;
	//
	// 修订版(仅仅修正对BlendMap像素中心偏移)
	//	float2 vUVSet2 = vUVSet + g_vRecipBlendMapSize * 0.5f;
	//
	float2 tileCenterOffset = frac(vUVSet * g_fTileMapSize) - 0.5f;
	float2 vUVSet2 = vUVSet - tileCenterOffset * g_fRecipBlendMapSize;

	// 根据UV采样BlendMap
	float4 vBlendTex = tex2D(sdBlendSampler, vUVSet);
	
	// 归一化权重
	float fTotalWeight = dot(vBlendTex.xyz, 1.f);
	vBlendTex.rgb /= fTotalWeight;
	
#ifdef _SD_EDITOR
	vBlendTex.a = max(vBlendTex.a, g_fLightMapMask);
#endif
	// @}
	
	
	// 贴图混合
	// @{
	// 采样立方体纹理
	float4 vPlanarVec = texCUBE(sdPlanarTableSampler, vWorldNormal.xzy) * 255.f - 1.f;
	
	// 计算新的地形UV(??)
	float2 vUVSet3 = float2(dot(vWorldPos.xy, vPlanarVec.xy), dot(vWorldPos.yz, vPlanarVec.zw));
	
	// 计算当前像素到观察点矢量
	float3 vWorldViewVector = normalize(g_vViewPos - kInput.vUVFarClipWorldPos);
	
	// 计算当前像素应取LOD(这里不解,有待进一步关注)(靠,固定编码)
	float fLodLevel = log2(2.0 * length( kInput.vUVFarClipViewPos) / 768.f / max(sqrt(dot(vWorldViewVector, vWorldNormal)), 0.25f));
	
	// 计算图集UV
	float4 vUVSetTableU = saturate(vIndices.bgra * g_fNormalAtlasIdScale + g_fNormalAtlasIdOffset);
	float vUVSetTableV	= saturate(fLodLevel * g_fNormalAtlasLevelScale + g_fNormalAtlasLevelOffset);
	
	// 贴图混合
	float4 vDiffuseGloss = SamplerAtlasMap(sdDiffuseAtlasSampler, sdAtlasTableSampler, float2(vUVSetTableU.x, vUVSetTableV), vUVSet3) * vBlendTex.b +
						   SamplerAtlasMap(sdDiffuseAtlasSampler, sdAtlasTableSampler, float2(vUVSetTableU.y, vUVSetTableV), vUVSet3) * vBlendTex.g +
						   SamplerAtlasMap(sdDiffuseAtlasSampler, sdAtlasTableSampler, float2(vUVSetTableU.z, vUVSetTableV), vUVSet3) * vBlendTex.r;
	
#ifdef _SD_EDITOR
	vDiffuseGloss = max(vDiffuseGloss, float4(g_vDiffuseMapMask, g_fGlossMapMask));
#endif
	// @}
	
	
	// 计算最终颜色
	// @{
	// 采样LightBuffer
	float4 vLightTex = tex2D(sdLightSampler, kInput.vUVSetScreenTex);
	
	// 计算观察方向与法线
	float3 vViewVector = -normalize(kInput.vUVFarClipViewPos);
	float3 vViewNormal 	= UnpackNormal(vGeoTex.zw);
	
	// 合成光照
	float3 vDiffuseLight;
	float3 vSpeculatLight;
	AccumLighting(vViewVector, vViewNormal, g_fTerrainShiness, vBlendTex.a, vLightTex, vDiffuseLight, vSpeculatLight);
	
	// 合成最终颜色
	float3 vColor = vDiffuseLight  * vDiffuseGloss.rgb * g_vTerrainDiffuseMaterial +
					vSpeculatLight * vDiffuseGloss.a   * g_vTerrainSpecularMaterial;	
	// @}
	
#ifdef _SD_EDITOR
	vColor = lerp(vColor, float3(1.f, 0.f, 0.f), saturate(1.f - fTotalWeight) * g_fTerrainShowInvisibleLayers);
	vColor = lerp(vColor, float3(0.f, 1.f, 1.f), any(saturate(abs(tileCenterOffset) - 0.49f)) * g_fTerrainShowTileGrid);
#endif
	
	return float4(vColor, 0);
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
technique Terrain_AtlasShading_Planar_Ortho
<
	string Description = "Terrain_AtlasShading_Planar_Ortho";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_Planar ();
	}
}
//---------------------------------------------------------------------------------------