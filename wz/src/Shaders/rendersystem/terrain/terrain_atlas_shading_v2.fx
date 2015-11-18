//*************************************************************************************************
// 内容:	地形着色(V2,ruantianlong modifiy)
//---------------------------------------------------------
// 作者:		
// 创建:		2013-09-11
// 最后修改:	2013-09-12
//*************************************************************************************************
#include "terrain_common_v2.h"

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
	float3 vIndices = tex2D(sdTileSampler, vUVSet).bgr * 255.f;
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

//	// 计算当前像素到观察点的矢量
//	float3 vWorldViewVector = normalize(g_vViewPos - kInput.vUVFarClipWorldPos);
//	
//	// 计算当前像素应取LOD(先计算的是当前Pixel对应的世界空间Pixel的尺寸,然后对2取对数得到LOD)
//	//	g_vFarPixelSize				像素在垂直远平面上的对应尺寸(近似尺寸,真实远平面是一个椭球面的一部分)
//	//	g_vFarPixelSize * fDepth	像素在当前距离下的垂直平面上的对应尺寸
//	//	dot(vWorldViewVector, vWorldNormal)	当前像素相对投影方位的角度,即与投影方向的夹角余弦值
//	// 目前没有进行LOD层级之间的过滤，所以LOD之间过渡很不自然
//	float2 vLodLevel = log2(g_vFarPixelSize * fDepth / max(dot(vWorldViewVector, vWorldNormal), 0.25f));
//	
//	// 计算图集UV
//	//	.xyz 分别是3个Layer的id计算得到的纹理U坐标 	LayerId * (1.0f / uiWidth) + (0.5f / uiWidth)
//	//	.w	 是像素的lod信息计算得到的纹理V坐标	 	PixelLod * (1.0f / uiHeight) + (-iMinLodFactor / (float)uiHeight)
//	float4 vUVSetTable;
//	vUVSetTable.xyz = saturate(vIndices.bgr * g_fDiffuseAtlasIdScale + g_fDiffuseAtlasIdOffset);
//	vUVSetTable.w	= saturate(max(vLodLevel.x, vLodLevel.y) * g_fDiffuseAtlasLevelScale + g_fDiffuseAtlasLevelOffset);
//	
//#ifdef _SD_EDITOR
//	if (g_fTerrainShowCurrentLayer == 1.f)
//	{
//		vBlendTex.b *= (vIndices.b == g_fTerrainCurrentLayerIndex) ? 1.f : 0.f;
//		vBlendTex.g *= (vIndices.g == g_fTerrainCurrentLayerIndex) ? 1.f : 0.f;
//		vBlendTex.r *= (vIndices.r == g_fTerrainCurrentLayerIndex) ? 1.f : 0.f;
//	}
//#endif
//	
//	// 贴图混合
//	float4 vDiffuseGloss = SamplerAtlasMap(sdDiffuseAtlasSampler, sdAtlasTableSampler, vUVSetTable.xw, vUVSet3) * vBlendTex.b +
//		SamplerAtlasMap(sdDiffuseAtlasSampler, sdAtlasTableSampler, vUVSetTable.yw, vUVSet3) * vBlendTex.g +
//		SamplerAtlasMap(sdDiffuseAtlasSampler, sdAtlasTableSampler, vUVSetTable.zw, vUVSet3) * vBlendTex.r;

	float2 vTexSize = float2(512.f, 512.f);										///< 每张DiffuseMap的原始尺寸是512
	float2 vXDeriv = ddx(vUVSet3 * vTexSize * 2.8f * g_vTerrainSkipParam.y);	///< 其中2.8f是个固定偏置参数
	float2 vYDeriv = ddy(vUVSet3 * vTexSize * 2.8f * g_vTerrainSkipParam.y);	///< 其中2.8f是个固定偏置参数
	
	float4 vUVSetTable;
	vUVSetTable.xyz = saturate(vIndices.rgb * g_fDiffuseAtlasIdScale + g_fDiffuseAtlasIdOffset);
	
	float2 avUVSet1[3];		///< 三张贴图的UV坐标
	float3 vMipmapLevel;	///< 三张贴图的Mipmap等级
	float4 vTileInfo;

	vTileInfo = tex2D(sdAtlasTableSampler, float2(vUVSetTable.x, 0.f));
	avUVSet1[0] = float2(vUVSet3 * vTileInfo.x);
	vMipmapLevel.x = min(ComputeMipmapLevel_NODDXY(vXDeriv, vYDeriv, vTileInfo.x), 7.f);
	
	vTileInfo = tex2D(sdAtlasTableSampler, float2(vUVSetTable.y, 0.f));
	avUVSet1[1] = float2(vUVSet3 * vTileInfo.x);
	vMipmapLevel.y = min(ComputeMipmapLevel_NODDXY(vXDeriv, vYDeriv, vTileInfo.x), 7.f);
	
	vTileInfo = tex2D(sdAtlasTableSampler, float2(vUVSetTable.z, 0.f));
	avUVSet1[2] = float2(vUVSet3 * vTileInfo.x);
	vMipmapLevel.z = min(ComputeMipmapLevel_NODDXY(vXDeriv, vYDeriv, vTileInfo.x), 7.f);
	
	int2 iTexId[3];
	float fColCount = g_vTerrainAtlasSize.x / 512.f;						///< 每张DiffuseMap的原始尺寸是512
	for (int i = 0; i < 3; ++i)
	{
		iTexId[i].x = floor(floor(vIndices[i] + 0.5f) / fColCount);
		iTexId[i].y = floor(fmod(floor(vIndices[i] + 0.5f), fColCount));
	}

	float4 avUVSet2[4];
	float3 vMipmap = floor(vMipmapLevel);
	float3 vScaleTmp = pow(float3(0.5f, 0.5f, 0.5f), vMipmap);
	avUVSet2[0].xy = ComputeUV(vScaleTmp[0], avUVSet1[0], iTexId[0]);
	avUVSet2[1].xy = ComputeUV(vScaleTmp[1], avUVSet1[1], iTexId[1]);
	avUVSet2[2].xy = ComputeUV(vScaleTmp[2], avUVSet1[2], iTexId[2]);
	
	float3 vScaleTmp2 = vScaleTmp * 0.5f;
	avUVSet2[0].zw = ComputeUV(vScaleTmp[0], avUVSet1[0], iTexId[0]);
	avUVSet2[1].zw = ComputeUV(vScaleTmp[1], avUVSet1[1], iTexId[1]);
	avUVSet2[2].zw = ComputeUV(vScaleTmp[2], avUVSet1[2], iTexId[2]);

	float3 vWeight = frac(vMipmapLevel);
	float4 vDiffuseGloss = (tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2[0].xy, 0.f, vMipmap[0])) * (1.f - vWeight[0]) + tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2[0].zw, 0.f, vMipmap[0]+1)) * vWeight[0]) * vBlendTex.b +
					 (tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2[1].xy, 0.f, vMipmap[1])) * (1.f - vWeight[1]) + tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2[1].zw, 0.f, vMipmap[1]+1)) * vWeight[1]) * vBlendTex.g +
					 (tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2[2].xy, 0.f, vMipmap[2])) * (1.f - vWeight[2]) + tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2[2].zw, 0.f, vMipmap[2]+1)) * vWeight[2]) * vBlendTex.r;
	
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
	float3 vDiffuseLight, vSpecularLight;
//	AccumLightingOnlyDiffuse(vViewNormal, g_fTerrainUseLightMap > 0.f ? vBlendTex.z : 1.f, vLightTex, vDiffuseLight);
	AccumLighting(vViewVector, vViewNormal, g_fTerrainShiness, g_fTerrainUseLightMap > 0.f ? vBlendTex.z : 1.f, vLightTex, vDiffuseLight, vSpecularLight);

	// 合成最终颜色
//	float3 vColor = vDiffuseLight* vDiffuseGloss.rgb * g_vTerrainDiffuseMaterial;
	float3 vColor = vDiffuseLight * vDiffuseGloss.rgb * g_vTerrainDiffuseMaterial +
					vSpecularLight * vDiffuseGloss.a   * g_vTerrainSpecularMaterial;	
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
	float3 vIndices = tex2D(sdTileSampler, vUVSet).bgr * 255.f;
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

//	// 计算当前像素到观察点的矢量
//	float3 vWorldViewVector = normalize(g_vViewPos - kInput.vUVFarClipWorldPos);
//	
//	//
//	// 计算当前像素应取LOD(先计算的是当前Pixel对应的世界空间Pixel的尺寸,然后对2取对数得到LOD)
//	//	g_vFarPixelSize				像素在垂直远平面上的对应尺寸(近似尺寸,真实远平面是一个椭球面的一部分)
//	//	g_vFarPixelSize * fDepth	像素在当前距离下的垂直平面上的对应尺寸
//	//	dot(vWorldViewVector, vWorldNormal)	当前像素相对投影方位的角度,即与投影方向的夹角余弦值
//	// 目前没有进行LOD层级之间的过滤，所以LOD之间过渡很不自然
//	//
//	// 另一种计算mipmap的方式：
//	//	float2 vTexSubTileSize = float2(512.f, 512.f);		///< 需要计算mipmap的纹理的尺寸
//	//	float2 vTexUVSet = vUVSet3;							///< 相对于纹理的采样纹理坐标
//	//	float2 dx = ddx(vTexUVSet * vTexSubTileSize);		///< 计算纹理坐标差异	
//	//	float2 dy = ddy(vTexUVSet * vTexSubTileSize);
//	//	float d = max(dot(dx, dx), dot(dy, dy));   			///< 
//	//	float mipLevel = 0.5f * log2(d);					///< 取底数
//	//	mipLevel = floor(mipLevel);   						///< 向下取整[0, N]
//	//	float2 vLodLevel = float2(-mipLevel, -mipLevel);
//	//
//	// 参考：
//	//		http://www.ogre3d.org/forums/viewtopic.php?f=5&t=67846
//	//		GPU Pro2, Large-Scale Terrain Rendering for Outdoor Games
//	//
//	float2 vLodLevel = log2(g_vFarPixelSize * fDepth / max(dot(vWorldViewVector, vWorldNormal), 0.25f));
//	
//	// 计算图集UV
//	//	.xyz 分别是3个Layer的id计算得到的纹理U坐标 	LayerId * (1.0f / uiWidth) + (0.5f / uiWidth)
//	//	.w	 是像素的lod信息计算得到的纹理V坐标	 	PixelLod * (1.0f / uiHeight) + (-iMinLodFactor / (float)uiHeight)
//	float4 vUVSetTable;
//	vUVSetTable.xyz = saturate(vIndices.bgr * g_fDiffuseAtlasIdScale + g_fDiffuseAtlasIdOffset);
//	vUVSetTable.w	= saturate(max(vLodLevel.x, vLodLevel.y) * g_fDiffuseAtlasLevelScale + g_fDiffuseAtlasLevelOffset);
//	
//#ifdef _SD_EDITOR
//	if (g_fTerrainShowCurrentLayer == 1.f)
//	{
//		// WZ原版不需要round函数就可以，暂时不知道原因
//		vBlendTex.b *= (round(vIndices.b) == g_fTerrainCurrentLayerIndex) ? 1.f : 0.f;
//		vBlendTex.g *= (round(vIndices.g) == g_fTerrainCurrentLayerIndex) ? 1.f : 0.f;
//		vBlendTex.r *= (round(vIndices.r) == g_fTerrainCurrentLayerIndex) ? 1.f : 0.f;
//		//vBlendTex.b *= (vIndices.b == g_fTerrainCurrentLayerIndex) ? 1.f : 0.f;
//		//vBlendTex.g *= (vIndices.g == g_fTerrainCurrentLayerIndex) ? 1.f : 0.f;
//		//vBlendTex.r *= (vIndices.r == g_fTerrainCurrentLayerIndex) ? 1.f : 0.f;
//	}
//#endif
//	
//	// 贴图混合
//	float4 vDiffuseGloss = SamplerAtlasMap(sdDiffuseAtlasSampler, sdAtlasTableSampler, vUVSetTable.xw, vUVSet3) * vBlendTex.b +
//		SamplerAtlasMap(sdDiffuseAtlasSampler, sdAtlasTableSampler, vUVSetTable.yw, vUVSet3) * vBlendTex.g +
//		SamplerAtlasMap(sdDiffuseAtlasSampler, sdAtlasTableSampler, vUVSetTable.zw, vUVSet3) * vBlendTex.r;
	
	float2 vTexSize = float2(512.f, 512.f);										///< 每张DiffuseMap的原始尺寸是512
	float2 vXDeriv = ddx(vUVSet3 * vTexSize * 2.8f * g_vTerrainSkipParam.y);	///< 其中2.8f是个固定偏置参数
	float2 vYDeriv = ddy(vUVSet3 * vTexSize * 2.8f * g_vTerrainSkipParam.y);	///< 其中2.8f是个固定偏置参数
	
	float4 vUVSetTable;
	vUVSetTable.xyz = saturate(vIndices.rgb * g_fDiffuseAtlasIdScale + g_fDiffuseAtlasIdOffset);
	
	float2 avUVSet1[3];		///< 三张贴图的UV坐标
	float3 vMipmapLevel;	///< 三张贴图的Mipmap等级
	float4 vTileInfo;

	vTileInfo = tex2D(sdAtlasTableSampler, float2(vUVSetTable.x, 0.f));
	avUVSet1[0] = float2(vUVSet3 * vTileInfo.x);
	vMipmapLevel.x = min(ComputeMipmapLevel_NODDXY(vXDeriv, vYDeriv, vTileInfo.x), 7.f);
	
	vTileInfo = tex2D(sdAtlasTableSampler, float2(vUVSetTable.y, 0.f));
	avUVSet1[1] = float2(vUVSet3 * vTileInfo.x);
	vMipmapLevel.y = min(ComputeMipmapLevel_NODDXY(vXDeriv, vYDeriv, vTileInfo.x), 7.f);
	
	vTileInfo = tex2D(sdAtlasTableSampler, float2(vUVSetTable.z, 0.f));
	avUVSet1[2] = float2(vUVSet3 * vTileInfo.x);
	vMipmapLevel.z = min(ComputeMipmapLevel_NODDXY(vXDeriv, vYDeriv, vTileInfo.x), 7.f);
	
	int2 iTexId[3];
	float fColCount = g_vTerrainAtlasSize.x / 512.f;						///< 每张DiffuseMap的原始尺寸是512
	for (int i = 0; i < 3; ++i)
	{
		iTexId[i].x = floor(floor(vIndices[i] + 0.5f) / fColCount);
		iTexId[i].y = floor(fmod(floor(vIndices[i] + 0.5f), fColCount));
	}

	float4 avUVSet2[4];
	float3 vMipmap = floor(vMipmapLevel);
	float3 vScaleTmp = pow(float3(0.5f, 0.5f, 0.5f), vMipmap);
	avUVSet2[0].xy = ComputeUV(vScaleTmp[0], avUVSet1[0], iTexId[0]);
	avUVSet2[1].xy = ComputeUV(vScaleTmp[1], avUVSet1[1], iTexId[1]);
	avUVSet2[2].xy = ComputeUV(vScaleTmp[2], avUVSet1[2], iTexId[2]);
	
	float3 vScaleTmp2 = vScaleTmp * 0.5f;
	avUVSet2[0].zw = ComputeUV(vScaleTmp[0], avUVSet1[0], iTexId[0]);
	avUVSet2[1].zw = ComputeUV(vScaleTmp[1], avUVSet1[1], iTexId[1]);
	avUVSet2[2].zw = ComputeUV(vScaleTmp[2], avUVSet1[2], iTexId[2]);

	float3 vWeight = frac(vMipmapLevel);
	float4 vDiffuseGloss = (tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2[0].xy, 0.f, vMipmap[0])) * (1.f - vWeight[0]) + tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2[0].zw, 0.f, vMipmap[0]+1)) * vWeight[0]) * vBlendTex.b +
					 (tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2[1].xy, 0.f, vMipmap[1])) * (1.f - vWeight[1]) + tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2[1].zw, 0.f, vMipmap[1]+1)) * vWeight[1]) * vBlendTex.g +
					 (tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2[2].xy, 0.f, vMipmap[2])) * (1.f - vWeight[2]) + tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2[2].zw, 0.f, vMipmap[2]+1)) * vWeight[2]) * vBlendTex.r;
	
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
	float3 vIndices = tex2D(sdTileSampler, vUVSet).bgr * 255.f;
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
	
//	// 计算当前像素到观察点矢量
//	float3 vWorldViewVector = normalize(g_vViewPos - kInput.vUVFarClipWorldPos);
//	
//	// 计算当前像素应取LOD(先计算的是当前Pixel对应的世界空间Pixel的尺寸,然后对2取对数得到LOD)
//	//	g_vFarPixelSize				像素在垂直远平面上的对应尺寸(近似尺寸,真实远平面是一个椭球面的一部分)
//	//	g_vFarPixelSize * fDepth	像素在当前距离下的垂直平面上的对应尺寸
//	//	dot(vWorldViewVector, vWorldNormal)	当前像素相对投影方位的角度,即与投影方向的夹角余弦值
//	// 目前没有进行LOD层级之间的过滤，所以LOD之间过渡很不自然
//	float2 vLodLevel = log2(g_vFarPixelSize * fDepth / max(dot(vWorldViewVector, vWorldNormal), 0.25f));
//	
//	// 计算图集UV
//	//	.xyz 分别是3个Layer的id计算得到的纹理U坐标 	LayerId * (1.0f / uiWidth) + (0.5f / uiWidth)
//	//	.w	 是像素的lod信息计算得到的纹理V坐标	 	PixelLod * (1.0f / uiHeight) + (-iMinLodFactor / (float)uiHeight)
//	float4 vUVSetTable;
//	vUVSetTable.xyz = saturate(vIndices.bgr * g_fDiffuseAtlasIdScale + g_fDiffuseAtlasIdOffset);
//	vUVSetTable.w	= saturate(max(vLodLevel.x, vLodLevel.y) * g_fDiffuseAtlasLevelScale + g_fDiffuseAtlasLevelOffset);
//	
//#ifdef _SD_EDITOR
//	if (g_fTerrainShowCurrentLayer == 1.f)
//	{
//		vBlendTex.b *= (vIndices.b == g_fTerrainCurrentLayerIndex) ? 1.f : 0.f;
//		vBlendTex.g *= (vIndices.g == g_fTerrainCurrentLayerIndex) ? 1.f : 0.f;
//		vBlendTex.r *= (vIndices.r == g_fTerrainCurrentLayerIndex) ? 1.f : 0.f;
//	}
//#endif
//	
//	// 贴图混合
//	float4 vDiffuseGloss = SamplerAtlasMap_Planar(sdDiffuseAtlasSampler, sdAtlasTableSampler, vUVSetTable.xw, vTerrainUV_XY, vTerrainUV_YZ, vTerrainUV_XZ, vPlanarWeight) * vBlendTex.b +
//		SamplerAtlasMap_Planar(sdDiffuseAtlasSampler, sdAtlasTableSampler, vUVSetTable.yw, vTerrainUV_XY, vTerrainUV_YZ, vTerrainUV_XZ, vPlanarWeight) * vBlendTex.g +
//		SamplerAtlasMap_Planar(sdDiffuseAtlasSampler, sdAtlasTableSampler, vUVSetTable.zw, vTerrainUV_XY, vTerrainUV_YZ, vTerrainUV_XZ, vPlanarWeight) * vBlendTex.r;

	float2 vTexSize = float2(512.f, 512.f);											///< 每张DiffuseMap的原始尺寸是512
	float2 vXDeriv = ddx(vTerrainUV_XY * vTexSize * 2.8f * g_vTerrainSkipParam.y);	///< 其中2.8f是个固定偏置参数
	float2 vYDeriv = ddy(vTerrainUV_XY * vTexSize * 2.8f * g_vTerrainSkipParam.y);	///< 其中2.8f是个固定偏置参数
	
	float4 vUVSetTable;
	vUVSetTable.xyz = saturate(vIndices.rgb * g_fDiffuseAtlasIdScale + g_fDiffuseAtlasIdOffset);
	
	float2 avUVSet1_XY[3];		///< 三张贴图的UV坐标
	float2 avUVSet1_YZ[3];		///< 三张贴图的UV坐标
	float2 avUVSet1_XZ[3];		///< 三张贴图的UV坐标
	float3 vMipmapLevel;		///< 三张贴图的Mipmap等级
	float4 vTileInfo;

	vTileInfo = tex2D(sdAtlasTableSampler, float2(vUVSetTable.x, 0.f));
	avUVSet1_XY[0] = float2(vTerrainUV_XY * vTileInfo.x);
	avUVSet1_YZ[0] = float2(vTerrainUV_YZ * vTileInfo.x);
	avUVSet1_XZ[0] = float2(vTerrainUV_XZ * vTileInfo.x);
	vMipmapLevel.x = min(ComputeMipmapLevel_NODDXY(vXDeriv, vYDeriv, vTileInfo.x), 7.f);
	
	vTileInfo = tex2D(sdAtlasTableSampler, float2(vUVSetTable.y, 0.f));
	avUVSet1_XY[1] = float2(vTerrainUV_XY * vTileInfo.x);
	avUVSet1_YZ[1] = float2(vTerrainUV_YZ * vTileInfo.x);
	avUVSet1_XZ[1] = float2(vTerrainUV_XZ * vTileInfo.x);
	vMipmapLevel.y = min(ComputeMipmapLevel_NODDXY(vXDeriv, vYDeriv, vTileInfo.x), 7.f);
	
	vTileInfo = tex2D(sdAtlasTableSampler, float2(vUVSetTable.z, 0.f));
	avUVSet1_XY[2] = float2(vTerrainUV_XY * vTileInfo.x);
	avUVSet1_YZ[2] = float2(vTerrainUV_YZ * vTileInfo.x);
	avUVSet1_XZ[2] = float2(vTerrainUV_XZ * vTileInfo.x);
	vMipmapLevel.z = min(ComputeMipmapLevel_NODDXY(vXDeriv, vYDeriv, vTileInfo.x), 7.f);
	
	int2 iTexId[3];
	float fColCount = g_vTerrainAtlasSize.x / 512.f;						///< 每张DiffuseMap的原始尺寸是512
	for (int i = 0; i < 3; ++i)
	{
		iTexId[i].x = floor(floor(vIndices[i] + 0.5f) / fColCount);
		iTexId[i].y = floor(fmod(floor(vIndices[i] + 0.5f), fColCount));
	}

	float2 avUVSet2_XY[4];
	float2 avUVSet2_YZ[4];
	float2 avUVSet2_XZ[4];
	float3 vMipmap = floor(vMipmapLevel);
	float3 vScaleTmp = pow(float3(0.5f, 0.5f, 0.5f), vMipmap);
	avUVSet2_XY[0].xy = ComputeUV(vScaleTmp[0], avUVSet1_XY[0], iTexId[0]);
	avUVSet2_XY[1].xy = ComputeUV(vScaleTmp[1], avUVSet1_XY[1], iTexId[1]);
	avUVSet2_XY[2].xy = ComputeUV(vScaleTmp[2], avUVSet1_XY[2], iTexId[2]);
	
	avUVSet2_YZ[0].xy = ComputeUV(vScaleTmp[0], avUVSet1_YZ[0], iTexId[0]);
	avUVSet2_YZ[1].xy = ComputeUV(vScaleTmp[1], avUVSet1_YZ[1], iTexId[1]);
	avUVSet2_YZ[2].xy = ComputeUV(vScaleTmp[2], avUVSet1_YZ[2], iTexId[2]);
	
	avUVSet2_XZ[0].xy = ComputeUV(vScaleTmp[0], avUVSet1_XZ[0], iTexId[0]);
	avUVSet2_XZ[1].xy = ComputeUV(vScaleTmp[1], avUVSet1_XZ[1], iTexId[1]);
	avUVSet2_XZ[2].xy = ComputeUV(vScaleTmp[2], avUVSet1_XZ[2], iTexId[2]);

	float3 vWeight = frac(vMipmapLevel);
	float4 vDiffuseGloss = (tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2_XY[0].xy, 0.f, vMipmap[0])) * vPlanarWeight.z + tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2_YZ[0].xy, 0.f, vMipmap[0])) * vPlanarWeight.x + tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2_XZ[0].xy, 0.f, vMipmap[0])) * vPlanarWeight.y) * vBlendTex.b +
					 (tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2_XY[1].xy, 0.f, vMipmap[1])) * vPlanarWeight.z + tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2_YZ[1].xy, 0.f, vMipmap[1])) * vPlanarWeight.x + tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2_XZ[1].xy, 0.f, vMipmap[1])) * vPlanarWeight.y) * vBlendTex.g +
					 (tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2_XY[2].xy, 0.f, vMipmap[2])) * vPlanarWeight.z + tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2_YZ[2].xy, 0.f, vMipmap[2])) * vPlanarWeight.x + tex2Dlod(sdDiffuseAtlasSampler, float4(avUVSet2_XZ[2].xy, 0.f, vMipmap[2])) * vPlanarWeight.y) * vBlendTex.r;
	
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
}
//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique Terrain_AtlasShading_Simple_V2
<
	string Description = "Terrain_AtlasShading_Simple_V2";
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
technique Terrain_AtlasShading_Planar_V2
<
	string Description = "Terrain_AtlasShading_Planar_V2";
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
technique Terrain_AtlasShading_Seam_V2
<
	string Description = "Terrain_AtlasShading_Seam_V2";
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