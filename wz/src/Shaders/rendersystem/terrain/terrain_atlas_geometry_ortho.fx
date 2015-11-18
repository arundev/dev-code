//*************************************************************************************************
// 内容:	地形Normal/Depth绘制到G-Buffer
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
SD_POINT_CLAMP_SAMPLE(1, sdDepthSampler, 			sdDepthBuf, 		false);		// 地形屏幕深度缓存
SD_LINEAR_WRAP_SAMPLE(2, sdBaseNormalSampler, 		sdBaseNormalTex, 	false);		// 地形基本法线贴图
SD_POINT_CLAMP_SAMPLE(3, sdTileSampler,				sdTileTex,			false);		// 地形TileMap
SD_LINEAR_WRAP_SAMPLE(4, sdBlendSampler,			sdBlendTex,			false);		// 地形BlendMap
SD_POINT_CLAMP_SAMPLE(5, sdAtlasTableSampler,		sdAtlasTableTex,	false);		// 地形法线贴图查询表
SD_LINEAR_WRAP_SAMPLE(6, sdNormalAtlasSampler,		sdNormalAtlasTex,	false);		// 地形法线贴图图集

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
	
	// 当前点对应近远裁剪面上的点的世界坐标
	kOutput.vUVNearClipWorldPos = mul(float4(kInput.vPos.xy, 0.f, 1.f), g_mDepthToWorld).xyz;
	kOutput.vUVFarClipWorldPos  = mul(float4(kInput.vPos.xy, 1.f, 1.f), g_mDepthToWorld).xyz;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
// 渲染远处,只渲染基础法线贴图
float4 PS_Main_Far_BaseNormal(VS_OUTPUT kInput) : COLOR0
{
	// 获取地形深度
	float2 vPackedDepth = tex2D(sdDepthSampler, kInput.vUVSetScreenTex).xy;
	float fDepth = UnpackDepth(vPackedDepth.xy);
	
	// 反算世界坐标(根据线性深度,对近远平面对应点位置进行插值)
	float3 vWorldPos = lerp(kInput.vUVNearClipWorldPos, kInput.vUVFarClipWorldPos, fDepth);
	
	// 裁剪掉指定近平面以内的像素
	clip(length(vWorldPos - kInput.vUVNearClipWorldPos) - g_fTerrainFarStart);
	
	// 计算当前点的地形相对UV(注意,这里没有偏移)
	float2 vUVSet = vWorldPos.xy * g_vRecipTerrainSize;
	
	// BaseNormalMap
	// @{
	// 根据UV采样BaseNormalMap(注意,这里没有偏移半像素,因为BaseNormalMap是Linear采样的)
	float4 vBaseNormalTex 	= tex2D(sdBaseNormalSampler, vUVSet);
	
	// 解出世界空间法线
	float3 vWorldNormal;
	vWorldNormal.xy	= vBaseNormalTex.xy * 2.f - 1.f;
	vWorldNormal.z 	= sqrt(dot(float3(1.f, vWorldNormal.xy), float3(1.f, -vWorldNormal.xy)));
	
	// 变换Normal到观察空间(详见terrain_atlas_geometry.fx)
	float3 vViewNormal = mul(float4(vWorldNormal, 0.f), g_mView).xyz;
	// @}
	
	// 输出打包的法线和深度
	return float4(vPackedDepth, PackNormal(vViewNormal));
}

//---------------------------------------------------------------------------------------
// 渲染近处,渲染基础法线和法线贴图
float4 PS_Main_Near_BaseNormalAndNormalMap(VS_OUTPUT kInput) : COLOR0
{
	// 获取地形深度
	float2 vPackedDepth = tex2D(sdDepthSampler, kInput.vUVSetScreenTex).xy;
	float fDepth = UnpackDepth(vPackedDepth.xy);
	
	// 反算世界坐标(根据线性深度,对近远平面对应点位置进行插值)
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
	// 计算新的UV(详见terrain_atlas_geometry.fx)
	float2 tileCenterOffset = frac(vUVSet * g_vTileMapSize) - 0.5f;
	float2 vUVSet2 = vUVSet - tileCenterOffset * g_vRecipBlendMapSize;
	
	// 根据UV采样BlendMap
	float3 vBlendTex = tex2D(sdBlendSampler, vUVSet2).rgb;
	
	// 归一化权重
	float fTotalWeight = dot(vBlendTex, 1.f);
	vBlendTex.rgb /= fTotalWeight;
	// @}
	
	// 计算切线空间
	// @{
	// 采样立方体纹理(详见terrain_atlas_geometry.fx)
	float4 vPlanarVec = texCUBE(sdPlanarTableSampler, vWorldNormal.xzy) * 255.f - 1.f;
	
	// 计算新的地形UV(详见terrain_atlas_geometry.fx)
	float2 vUVSet3 = float2(dot(vWorldPos.xy, vPlanarVec.xy), dot(vWorldPos.yz, vPlanarVec.zw));
	
	// 计算当前点的切线空间
	float3 vWorldBinormal 	= cross(float3(vPlanarVec.xy, 0.f), vWorldNormal);
	float3 vWorldTangent 	= cross(vWorldNormal, vWorldBinormal);
	// @}
	
	// 法线混合(NormalAtlasTable, NormalAtlas)
	// @{
	// 计算当前像素到观察点矢量
	float3 vWorldViewVector = kInput.vUVNearClipWorldPos - vWorldPos;
	float fWorldDistance = length(vWorldViewVector);
	vWorldViewVector /= fWorldDistance;
	
	// 计算当前像素应取LOD(这里不解,有待进一步关注)(靠,固定编码)
	float fLodLevel = log2(2.f * fWorldDistance / 768.f / max(sqrt(dot(vWorldViewVector, vWorldNormal)), 0.25f));
	
	// 计算图集UV(详见terrain_atlas_geometry.fx)
	float4 vUVSetTableU = saturate(vIndices.bgra * g_fNormalAtlasIdScale + g_fNormalAtlasIdOffset);
	float vUVSetTableV	= saturate(fLodLevel * g_fNormalAtlasLevelScale + g_fNormalAtlasLevelOffset);
	
	// 贴图混合
	float3 vNormal = SamplerAtlasMap(sdNormalAtlasSampler, sdAtlasTableSampler, float2(vUVSetTableU.x, vUVSetTableV), vUVSet3) * vBlendTex.b +
					 SamplerAtlasMap(sdNormalAtlasSampler, sdAtlasTableSampler, float2(vUVSetTableU.y, vUVSetTableV), vUVSet3) * vBlendTex.g +
					 SamplerAtlasMap(sdNormalAtlasSampler, sdAtlasTableSampler, float2(vUVSetTableU.z, vUVSetTableV), vUVSet3) * vBlendTex.r;
	
	// 变换DetailNormal(从[0,1]到[-1,1])
	vNormal = vNormal * 2.f - 1.f;
	
	// 缩放DetailNormal
	vNormal.xy *= g_fTerrainNormalScale;
	
	// 与远处法线区域做过渡(详见terrain_atlas_geometry.fx)
	float fNormalSmooth = saturate(5.f - 5.f * length(vWorldPos - kInput.vUVNearClipWorldPos) / g_fTerrainFarStart);
	vNormal.xy *= fNormalSmooth;
	
	// 归一化
	vNormal = normalize(vNormal);
	//vNormal.z = sqrt(dot(float3(1.f, vNormal.xy), float3(1.f, -vNormal.xy)));
	
	// DetailNormal从切线空间转换到世界坐标(详见terrain_atlas_geometry.fx)
	vNormal = vNormal.z * vWorldNormal + vNormal.y * vWorldBinormal + vNormal.x * vWorldTangent;
	
	// 转换到观察坐标系
	float3 vViewNormal = mul(float4(vNormal, 0.f), g_mView).xyz;
	// @}
	
	return float4(vPackedDepth, PackNormal(vViewNormal));
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique Terrain_AtlasGeometry_Far_BaseNormal_Ortho
<
	string Description = "Terrain_AtlasGeometry_Far_BaseNormal_Ortho";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_Far_BaseNormal ();
	}
}
//---------------------------------------------------------------------------------------
technique Terrain_AtlasGeometry_Near_BaseNormalAndNormalMap_Ortho
<
	string Description = "Terrain_AtlasGeometry_Near_BaseNormalAndNormalMap";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_Near_BaseNormalAndNormalMap ();
	}
}
//---------------------------------------------------------------------------------------