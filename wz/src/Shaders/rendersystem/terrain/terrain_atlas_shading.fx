//*************************************************************************************************
// 内容:	地形着色
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-08
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
	float3	vUVFarClipWorldPos	: TEXCOORD1;	// 当前点对应远裁剪面上的点的世界坐标
	float3	vUVFarClipViewPos	: TEXCOORD2;	// 当前点对应远裁剪面上的点的观察坐标
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
// 
//---------------------------------------------------------------------------------------
float4 PS_Main_Simple(VS_OUTPUT kInput) : COLOR0
{
	// 获取地形深度
	float4 vGeoTex = tex2D(sdGeomSampler, kInput.vUVSetScreenTex);
	float fDepth = UnpackDepth(vGeoTex.xy);
	
	// 反算世界坐标(根据线性深度,对相机位置和远平面对应点位置进行插值)
	float3 vWorldPos = lerp(g_vViewPos, kInput.vUVFarClipWorldPos, fDepth);
	
	// 计算当前点的地形相对UV(注意,这里没有偏移)
	float2 vUVSet = vWorldPos.xy * g_vRecipTerrainSize;
	
	
	// BaseNormalMap
	// @{
	// 根据UV采样BaseNormalMap
	float4 vBaseNormalTex = tex2D(sdBaseNormalSampler, vUVSet);
	
	// 解出倾斜情况,超过一定角度的不进行着色
	//float3 vPlanarWeight;
	//vPlanarWeight.xy 	= vBaseNormalTex.zw;
	//vPlanarWeight.z 	= saturate(1.f - vPlanarWeight.x - vPlanarWeight.y);

	//clip(1.5f - dot(sign(vPlanarWeight), 1.f));
	
	// 解出世界空间法线
	float3 vWorldNormal;
	vWorldNormal.xy	= vBaseNormalTex.xy * 2.f - 1.f;
	vWorldNormal.z 	= sqrt(dot(float3(1.f, vWorldNormal.xy), float3(1.f, -vWorldNormal.xy)));
	// @}
	
	
	// TileMap
	// @{
	// 根据UV采样TileMap(这里从[0,1]恢复到[0,255]的图层索引区间)
	float3 vIndices = tex2D(sdTileSampler, vUVSet).xyz * 255.f;
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
	float4 vBlendTex = tex2D(sdBlendSampler, vUVSet2);
	
	// 归一化权重
	float fTotalWeight = dot(vBlendTex.xyz, 1.f);
	vBlendTex.rgb /= fTotalWeight;
	
#ifdef _SD_EDITOR
	vBlendTex.a = max(vBlendTex.a, g_fLightMapMask);
#endif
	// @}
	
	
	// 贴图混合
	// @{
	// 计算新的地形UV(简单的使用地形位置,y取负值是因为地形的Y轴与贴图的Y轴相反)
	float2 vUVSet3 = float2(vWorldPos.x, -vWorldPos.y);
	//float4 vPlanarVec = texCUBE(sdPlanarTableSampler, vWorldNormal.xzy) * 255 - 1; 
	//float2 vUVSet3 = float2(dot(vWorldPos.xy, vPlanarVec.xy), dot(vWorldPos.yz, vPlanarVec.zw));

	// 计算当前像素到观察点的矢量
	float3 vWorldViewVector = normalize(g_vViewPos - kInput.vUVFarClipWorldPos);
	
	// 计算当前像素应取LOD(先计算的是当前Pixel对应的世界空间Pixel的尺寸,然后对2取对数得到LOD)
	//	g_vFarPixelSize				像素在垂直远平面上的对应尺寸(近似尺寸,真实远平面是一个椭球面的一部分)
	//	g_vFarPixelSize * fDepth	像素在当前距离下的垂直平面上的对应尺寸
	//	dot(vWorldViewVector, vWorldNormal)	当前像素相对投影方位的角度,即与投影方向的夹角余弦值
	// 目前没有进行LOD层级之间的过滤，所以LOD之间过渡很不自然
	float2 vLodLevel = log2(g_vFarPixelSize * fDepth / max(dot(vWorldViewVector, vWorldNormal), 0.25f));
	
	// 计算图集UV
	//	.xyz 分别是3个Layer的id计算得到的纹理U坐标 	LayerId * (1.0f / uiWidth) + (0.5f / uiWidth)
	//	.w	 是像素的lod信息计算得到的纹理V坐标	 	PixelLod * (1.0f / uiHeight) + (-iMinLodFactor / (float)uiHeight)
	float4 vUVSetTable;
	vUVSetTable.xyz = saturate(vIndices.bgr * g_fDiffuseAtlasIdScale + g_fDiffuseAtlasIdOffset);
	vUVSetTable.w	= saturate(max(vLodLevel.x, vLodLevel.y) * g_fDiffuseAtlasLevelScale + g_fDiffuseAtlasLevelOffset);
	
#ifdef _SD_EDITOR
	if (g_fTerrainShowCurrentLayer == 1.f)
	{
		vBlendTex.b *= (vIndices.b == g_fTerrainCurrentLayerIndex) ? 1.f : 0.f;
		vBlendTex.g *= (vIndices.g == g_fTerrainCurrentLayerIndex) ? 1.f : 0.f;
		vBlendTex.r *= (vIndices.r == g_fTerrainCurrentLayerIndex) ? 1.f : 0.f;
	}
#endif
	
	// 贴图混合
	float4 vDiffuseGloss = SamplerAtlasMap(sdDiffuseAtlasSampler, sdAtlasTableSampler, vUVSetTable.xw, vUVSet3) * vBlendTex.b +
		SamplerAtlasMap(sdDiffuseAtlasSampler, sdAtlasTableSampler, vUVSetTable.yw, vUVSet3) * vBlendTex.g +
		SamplerAtlasMap(sdDiffuseAtlasSampler, sdAtlasTableSampler, vUVSetTable.zw, vUVSet3) * vBlendTex.r;
	
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
	AccumLightingOnlyDiffuse(vViewNormal, g_fTerrainUseLightMap > 0.f ? vBlendTex.z : 1.f, vLightTex, vDiffuseLight);

	// 合成最终颜色
	float3 vColor = vDiffuseLight  * vDiffuseGloss.rgb * g_vTerrainDiffuseMaterial;
	// @}
	
#ifdef _SD_EDITOR
	vColor = lerp(vColor, float3(1.f, 0.f, 0.f), saturate(1.f - fTotalWeight) * g_fTerrainShowInvisibleLayers);
	vColor = lerp(vColor, float3(0.f, 1.f, 1.f), any(saturate(abs(tileCenterOffset) - 0.49f)) * g_fTerrainShowTileGrid);
#endif

	return float4(vColor, 0.f);
};

//---------------------------------------------------------------------------------------
// 平坦地区着色
float4 PS_Main_Planar(VS_OUTPUT kInput) : COLOR0
{
	// 获取地形深度
	float4 vGeoTex = tex2D(sdGeomSampler, kInput.vUVSetScreenTex);
	float fDepth = UnpackDepth(vGeoTex.xy);
	
	// 反算世界坐标(根据线性深度,对相机位置和远平面对应点位置进行插值)
	float3 vWorldPos = lerp(g_vViewPos, kInput.vUVFarClipWorldPos, fDepth);
	
	// 计算当前点的地形相对UV(注意,这里没有偏移)
	float2 vUVSet = vWorldPos.xy * g_vRecipTerrainSize;
	
	
	// BaseNormalMap
	// @{
	// 根据UV采样BaseNormalMap(注意,这里没有偏移半像素,因为BaseNormalMap是Linear采样的)
	float4 vBaseNormalTex = tex2D(sdBaseNormalSampler, vUVSet);
	
	// 解出倾斜情况,超过一定角度的不进行着色
	float3 vPlanarWeight;
	vPlanarWeight.xy 	= vBaseNormalTex.zw;
	vPlanarWeight.z 	= saturate(1.f - vPlanarWeight.x - vPlanarWeight.y);

	clip(1.5f - dot(sign(vPlanarWeight), 1.f));
	
	// 解出世界空间法线
	float3 vWorldNormal;
	vWorldNormal.xy	= vBaseNormalTex.xy * 2.f - 1.f;
	vWorldNormal.z 	= sqrt(dot(float3(1.f, vWorldNormal.xy), float3(1.f, -vWorldNormal.xy)));
	// @}
	
	
	// TileMap
	// @{
	// 根据UV采样TileMap(这里从[0,1]恢复到[0,255]的图层索引区间)
	float3 vIndices = tex2D(sdTileSampler, vUVSet).xyz * 255.f;
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
	float4 vBlendTex = tex2D(sdBlendSampler, vUVSet2);
	
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
	//	1.GB坐标系是X向右Y向前Z向上
	//	2.D3D坐标系是X向右Y向上Z向前
	//float2 vUVSet3 = float2(vWorldPos.x, -vWorldPos.y);
	float4 vPlanarVec = texCUBE(sdPlanarTableSampler, vWorldNormal.xzy) * 255.f - 1.f; 
	
	// 计算新的地形UV(??)
	float2 vUVSet3 = float2(dot(vWorldPos.xy, vPlanarVec.xy), dot(vWorldPos.yz, vPlanarVec.zw));

	// 计算当前像素到观察点的矢量
	float3 vWorldViewVector = normalize(g_vViewPos - kInput.vUVFarClipWorldPos);
	
	//
	// 计算当前像素应取LOD(先计算的是当前Pixel对应的世界空间Pixel的尺寸,然后对2取对数得到LOD)
	//	g_vFarPixelSize				像素在垂直远平面上的对应尺寸(近似尺寸,真实远平面是一个椭球面的一部分)
	//	g_vFarPixelSize * fDepth	像素在当前距离下的垂直平面上的对应尺寸
	//	dot(vWorldViewVector, vWorldNormal)	当前像素相对投影方位的角度,即与投影方向的夹角余弦值
	// 目前没有进行LOD层级之间的过滤，所以LOD之间过渡很不自然
	//
	// 另一种计算mipmap的方式：
	//	float2 vTexSubTileSize = float2(512.f, 512.f);		///< 需要计算mipmap的纹理的尺寸
	//	float2 vTexUVSet = vUVSet3;							///< 相对于纹理的采样纹理坐标
	//	float2 dx = ddx(vTexUVSet * vTexSubTileSize);		///< 计算纹理坐标差异	
	//	float2 dy = ddy(vTexUVSet * vTexSubTileSize);
	//	float d = max(dot(dx, dx), dot(dy, dy));   			///< 
	//	float mipLevel = 0.5f * log2(d);					///< 取底数
	//	mipLevel = floor(mipLevel);   						///< 向下取整[0, N]
	//	float2 vLodLevel = float2(-mipLevel, -mipLevel);
	//
	// 参考：
	//		http://www.ogre3d.org/forums/viewtopic.php?f=5&t=67846
	//		GPU Pro2, Large-Scale Terrain Rendering for Outdoor Games
	//
	float2 vLodLevel = log2(g_vFarPixelSize * fDepth / max(dot(vWorldViewVector, vWorldNormal), 0.25f));
	
	// 计算图集UV
	//	.xyz 分别是3个Layer的id计算得到的纹理U坐标 	LayerId * (1.0f / uiWidth) + (0.5f / uiWidth)
	//	.w	 是像素的lod信息计算得到的纹理V坐标	 	PixelLod * (1.0f / uiHeight) + (-iMinLodFactor / (float)uiHeight)
	float4 vUVSetTable;
	vUVSetTable.xyz = saturate(vIndices.bgr * g_fDiffuseAtlasIdScale + g_fDiffuseAtlasIdOffset);
	vUVSetTable.w	= saturate(max(vLodLevel.x, vLodLevel.y) * g_fDiffuseAtlasLevelScale + g_fDiffuseAtlasLevelOffset);
	
#ifdef _SD_EDITOR
	if (g_fTerrainShowCurrentLayer == 1.f)
	{
		// WZ原版不需要round函数就可以，暂时不知道原因
		vBlendTex.b *= (round(vIndices.b) == g_fTerrainCurrentLayerIndex) ? 1.f : 0.f;
		vBlendTex.g *= (round(vIndices.g) == g_fTerrainCurrentLayerIndex) ? 1.f : 0.f;
		vBlendTex.r *= (round(vIndices.r) == g_fTerrainCurrentLayerIndex) ? 1.f : 0.f;
		//vBlendTex.b *= (vIndices.b == g_fTerrainCurrentLayerIndex) ? 1.f : 0.f;
		//vBlendTex.g *= (vIndices.g == g_fTerrainCurrentLayerIndex) ? 1.f : 0.f;
		//vBlendTex.r *= (vIndices.r == g_fTerrainCurrentLayerIndex) ? 1.f : 0.f;
	}
#endif
	
	// 贴图混合
	float4 vDiffuseGloss = SamplerAtlasMap(sdDiffuseAtlasSampler, sdAtlasTableSampler, vUVSetTable.xw, vUVSet3) * vBlendTex.b +
		SamplerAtlasMap(sdDiffuseAtlasSampler, sdAtlasTableSampler, vUVSetTable.yw, vUVSet3) * vBlendTex.g +
		SamplerAtlasMap(sdDiffuseAtlasSampler, sdAtlasTableSampler, vUVSetTable.zw, vUVSet3) * vBlendTex.r;
	
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

	return float4(vColor, 0.f);

	//*************************
	//return float4(vDiffuseLight.rgb, 0);
	//return float4(vDiffuseGloss.rgb, 0.f);
	//return float4(g_vTerrainDiffuseMaterial.rgb, 0);
	//return float4(vDiffuseLight.rgb * vDiffuseGloss.rgb, 0.f);

	//return float4(vGeoTex.zw, 0, 0);
	
	// 测试平坦与陡峭地区的分割
	//float3 vMonoColor = lerp(float3(0.5,0.5,0.5), float3(0.f, 1.f, 1.f), 
	//	any(saturate(abs(tileCenterOffset) - 0.49f)) * g_fTerrainShowTileGrid);
	//return float4(vMonoColor,0);
	
	// 测试屏幕纹理坐标
	//return float4(kInput.vUVSetScreenTex, 0, 0);
	
	// 测试地形纹理坐标
	//return float4(vUVSet.xy, 0, 0);
	
	// 测试地形深度坐标
	//return float4(fDepth,fDepth,fDepth,0);
	
	// 测试法线
	//return float4(vBaseNormalTex.xy,1,0);
	//return float4((vWorldNormal + 1.f) * 0.5f,0);
	//return float4((vViewNormal + 1.f) * 0.5f,0);
	
	//if (vWorldNormal.z > 0.99)
	//return float4(1,0,0,0);
	//else
	//return float4(0,1,0,0);
	
	// 测试混合贴图
	//return tex2D(sdBlendSampler, vWorldPos.xy * g_vRecipTerrainSize/* + 0.5 * g_vRecipBlendMapSize*/);
	
	// 测试索引贴图

	// 测试光照1
	//float NL = dot(vWorldNormal, -g_vMainLightDir);
	//float NL = dot(mul(vWorldNormal, g_mView), -mul(g_vMainLightDir, g_mView));
	//float NL = dot(mul(vWorldNormal, g_mView), -g_vMainLightDir);
	//float NL = dot(mul(vViewNormal, g_mView), -g_vMainLightDir);
	//float NL = dot(vViewNormal, -g_vMainLightDir);
	//float NL = dot(mul(float3(0,0,1), g_mView), -g_vMainLightDir);
	//return float4(NL, NL, NL,0);

	// 测试光照2
	//float3 vDiffuseLightTemp;
	//float3 vSpeculatLightTemp;
	//AccumLighting(vViewVector, vViewNormal, 0, 1, float4(0,0,0,0), vDiffuseLightTemp, vSpeculatLightTemp);
	//return float4(vDiffuseLightTemp.xyz, 0);
	
	// 测试LOD选取情况
	//if (max(vLodLevel.x, vLodLevel.y) > -1.f)
	//	return float4(0,0,0,0);
	//else if (max(vLodLevel.x, vLodLevel.y) > -2.f)
	//	return float4(0.5,0.5,0.5,0);
	//else if (max(vLodLevel.x, vLodLevel.y) > -3.f)
	//	return float4(1,1,0,0);	
	//else if (max(vLodLevel.x, vLodLevel.y) > -4.f)
	//	return float4(1,0,1,0);	
	//else if (max(vLodLevel.x, vLodLevel.y) > -5.f)
	//	return float4(0,1,1,0);	
	//else if (max(vLodLevel.x, vLodLevel.y) > -6.f)
	//	return float4(0,0,1,0);	
	//else if (max(vLodLevel.x, vLodLevel.y) > -7.f)
	//	return float4(0,1,0,0);	
	//else if (max(vLodLevel.x, vLodLevel.y) > -8.f)
	//	return float4(1,0,0,0);	
	//else
	//	return float4(1,1,1,0);	
	
	// 测试从LOD到V坐标转换情况
	//if (vUVSetTable.w < 1.0 / 32.0)
	//	return float4(1,1,1,0);
	//else if (vUVSetTable.w < 2.0 / 32.0)
	//	return float4(1,0,0,0);
	//else if (vUVSetTable.w < 3.0 / 32.0)
	//	return float4(0,1,0,0);	
	//else if (vUVSetTable.w < 4.0 / 32.0)
	//	return float4(0,0,1,0);	
	//else if (vUVSetTable.w < 5.0 / 32.0)
	//	return float4(0.7,0.7,0.7,0);	
	//else if (vUVSetTable.w < 6.0 / 32.0)
	//	return float4(0.4,0.4,0.4,0);	
	//else
	//	return float4(0,0,0,0);	
	//*************************
};

//---------------------------------------------------------------------------------------
// 陡峭地区着色
float4 PS_Main_Seam(VS_OUTPUT kInput) : COLOR0
{
	// 获取地形深度
	float4 vGeoTex = tex2D(sdGeomSampler, kInput.vUVSetScreenTex);
	float fDepth = UnpackDepth(vGeoTex.xy);
	
	// 反算世界坐标(根据线性深度,对相机位置和远平面对应点位置进行插值)
	float3 vWorldPos = lerp(g_vViewPos, kInput.vUVFarClipWorldPos, fDepth);
	
	// 计算当前点的地形相对UV(注意,这里没有偏移)
	float2 vUVSet = vWorldPos.xy * g_vRecipTerrainSize;
	
	
	// BaseNormalMap
	// @{
	// 根据UV采样BaseNormalMap(注意,这里没有偏移半像素,因为BaseNormalMap是Linear采样的)
	float4 vBaseNormalTex = tex2D(sdBaseNormalSampler, vUVSet);
	
	// 解出倾斜情况,小于一定角度的不进行着色
	float3 vPlanarWeight;
	vPlanarWeight.xy 	= vBaseNormalTex.zw;
	vPlanarWeight.z 	= saturate(1.f - vPlanarWeight.x - vPlanarWeight.y);
	
	clip(dot(sign(vPlanarWeight), 1.f) - 1.5f);
	
	// 解出世界空间法线(x,y,z)
	float3 vWorldNormal;
	vWorldNormal.xy	= vBaseNormalTex.xy * 2.f - 1.f;
	vWorldNormal.z 	= sqrt(dot(float3(1.f, vWorldNormal.xy), float3(1.f, -vWorldNormal.xy)));
	// @}
	
	
	// TileMap
	// @{
	// 根据UV采样TileMap(这里从[0,1]恢复到[0,255]的图层索引区间)
	float3 vIndices = tex2D(sdTileSampler, vUVSet).xyz * 255.f;
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
	float4 vBlendTex = tex2D(sdBlendSampler, vUVSet2);
	
	// 归一化权重
	float fTotalWeight = dot(vBlendTex.xyz, 1.f);
	vBlendTex.rgb /= fTotalWeight;
	
#ifdef _SD_EDITOR
	vBlendTex.a = max(vBlendTex.a, g_fLightMapMask);
#endif
	// @}
	
	
	// 贴图混合
	// @{
	// 计算新的地形UV
	//	1.XY平面坐标,Y取负是因为贴图的Y轴是反的
	//	2.YZ平面坐标,Z取负是因为贴图的Y轴是反的，取法线X轴分量符号
	//	3.XZ平面坐标,Z取负是因为贴图的Y轴是反的，X取负是因为XY此时的增长方向不同
	//	4.不明白为啥减去0.5
	//float4 vPlanarVec = texCUBE(sdPlanarTableSampler, vWorldNormal.xzy) * 255.f - 1.f;
	//float2 vUVSet3 = float2(dot(vWorldPos.xy, vPlanarVec.xy), dot(vWorldPos.yz, vPlanarVec.zw));
	float2 vTerrainUV_XY = float2(vWorldPos.x, -vWorldPos.y);
	float2 vTerrainUV_YZ = float2(vWorldPos.y * sign(vWorldNormal.x - 0.5f), -vWorldPos.z);
	float2 vTerrainUV_XZ = float2(-vWorldPos.x * sign(vWorldNormal.y - 0.5f), -vWorldPos.z);
	
	// 计算当前像素到观察点矢量
	float3 vWorldViewVector = normalize(g_vViewPos - kInput.vUVFarClipWorldPos);
	
	// 计算当前像素应取LOD(先计算的是当前Pixel对应的世界空间Pixel的尺寸,然后对2取对数得到LOD)
	//	g_vFarPixelSize				像素在垂直远平面上的对应尺寸(近似尺寸,真实远平面是一个椭球面的一部分)
	//	g_vFarPixelSize * fDepth	像素在当前距离下的垂直平面上的对应尺寸
	//	dot(vWorldViewVector, vWorldNormal)	当前像素相对投影方位的角度,即与投影方向的夹角余弦值
	// 目前没有进行LOD层级之间的过滤，所以LOD之间过渡很不自然
	float2 vLodLevel = log2(g_vFarPixelSize * fDepth / max(dot(vWorldViewVector, vWorldNormal), 0.25f));
	
	// 计算图集UV
	//	.xyz 分别是3个Layer的id计算得到的纹理U坐标 	LayerId * (1.0f / uiWidth) + (0.5f / uiWidth)
	//	.w	 是像素的lod信息计算得到的纹理V坐标	 	PixelLod * (1.0f / uiHeight) + (-iMinLodFactor / (float)uiHeight)
	float4 vUVSetTable;
	vUVSetTable.xyz = saturate(vIndices.bgr * g_fDiffuseAtlasIdScale + g_fDiffuseAtlasIdOffset);
	vUVSetTable.w	= saturate(max(vLodLevel.x, vLodLevel.y) * g_fDiffuseAtlasLevelScale + g_fDiffuseAtlasLevelOffset);
	
#ifdef _SD_EDITOR
	if (g_fTerrainShowCurrentLayer == 1.f)
	{
		vBlendTex.b *= (vIndices.b == g_fTerrainCurrentLayerIndex) ? 1.f : 0.f;
		vBlendTex.g *= (vIndices.g == g_fTerrainCurrentLayerIndex) ? 1.f : 0.f;
		vBlendTex.r *= (vIndices.r == g_fTerrainCurrentLayerIndex) ? 1.f : 0.f;
	}
#endif
	
	// 贴图混合
	float4 vDiffuseGloss = SamplerAtlasMap_Planar(sdDiffuseAtlasSampler, sdAtlasTableSampler, vUVSetTable.xw, vTerrainUV_XY, vTerrainUV_YZ, vTerrainUV_XZ, vPlanarWeight) * vBlendTex.b +
		SamplerAtlasMap_Planar(sdDiffuseAtlasSampler, sdAtlasTableSampler, vUVSetTable.yw, vTerrainUV_XY, vTerrainUV_YZ, vTerrainUV_XZ, vPlanarWeight) * vBlendTex.g +
		SamplerAtlasMap_Planar(sdDiffuseAtlasSampler, sdAtlasTableSampler, vUVSetTable.zw, vTerrainUV_XY, vTerrainUV_YZ, vTerrainUV_XZ, vPlanarWeight) * vBlendTex.r;
	
#ifdef _SD_EDITOR
	vDiffuseGloss = max(vDiffuseGloss, float4(g_vDiffuseMapMask,g_fGlossMapMask));
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
	
	return float4(vColor, 0.f);
	
	//*************************
	// 测试平坦与陡峭地区的分割
	//return float4(0.5,0.5,0.5,0);
	
	// 测试法线
	//return float4(vBaseNormalTex.xy, 1.f, 0);
	//return float4((vWorldNormal + 1.f) * 0.5f,0);
	//return float4((vViewNormal + 1.f) * 0.5f,0);
	
	// 测试LOD选取情况
	//if (vLodLevel.x > -1.f)
	//	return float4(0,0,0,0);
	//else if (vLodLevel.x > -2.f)
	//	return float4(0.5,0.5,0.5,0);
	//else if (vLodLevel.x > -4.f)
	//	return float4(0,0,1,0);	
	//else if (vLodLevel.x > -6.f)
	//	return float4(0,1,0,0);	
	//else if (vLodLevel.x > -8.f)
	//	return float4(1,0,0,0);	
	//else
	//	return float4(1,1,1,0);	
	//*************************
}
//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique Terrain_AtlasShading_Simple
<
	string Description = "Terrain_AtlasShading_Simple";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_Simple ();
	}
}
//---------------------------------------------------------------------------------------
technique Terrain_AtlasShading_Planar
<
	string Description = "Terrain_AtlasShading_Planar";
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
technique Terrain_AtlasShading_Seam
<
	string Description = "Terrain_AtlasShading_Seam";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_Seam ();
	}
}
//---------------------------------------------------------------------------------------