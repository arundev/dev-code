//*************************************************************************************************
// 内容:	地形着色器的公共部分
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-04
// 最后修改:	2013-05-02
//*************************************************************************************************
#ifndef _TERRAIN_COMMON_H__
#define _TERRAIN_COMMON_H__

//
#include "../common.h"

//---------------------------------------------------------------------------------------
// 相关变量
//---------------------------------------------------------------------------------------
// 地形Tile几何信息
// [origin_x][origin_y][scale][morph_factor]
float4	a_vTerrainQuadParam 	: ATTRIBUTE;
#define	a_vQuadOrigin			float2(a_vTerrainQuadParam[0], a_vTerrainQuadParam[1])
#define	a_fQuadScale			a_vTerrainQuadParam[2]
#define	a_fQuadMorphFactor		a_vTerrainQuadParam[3]

// 地形一些贴图的尺寸(用于计算UV)
// [terrain_size][blendmap_size][tilemap_size][unused]
// [recpi_terrain_size][recpi_blendmap_size][recpi_tilemap_size][unused]
float4	g_vTerrainMapSize		: GLOBAL;
#define g_fTerrainSize 			g_vTerrainMapSize.x
#define g_vTerrainSize 			g_vTerrainMapSize.xx
#define g_fBlendMapSize 		g_vTerrainMapSize.y
#define g_vBlendMapSize 		g_vTerrainMapSize.yy
#define g_fTileMapSize 			g_vTerrainMapSize.z
#define g_vTileMapSize 			g_vTerrainMapSize.zz

float4	g_vRecipTerrainMapSize	: GLOBAL;
#define g_fRecipTerrainSize 	g_vRecipTerrainMapSize.x
#define g_vRecipTerrainSize 	g_vRecipTerrainMapSize.xx
#define g_fRecipBlendMapSize 	g_vRecipTerrainMapSize.y
#define g_vRecipBlendMapSize 	g_vRecipTerrainMapSize.yy
#define g_fRecipTileMapSize 	g_vRecipTerrainMapSize.z
#define g_vRecipTileMapSize 	g_vRecipTerrainMapSize.zz

// 地形高度范围(用于压缩顶点)
float2  g_vTerrainHeightRange	: GLOBAL;
#define g_fTerrainMinHeight		g_vTerrainHeightRange.x
#define g_fTerrainMaxHeight		g_vTerrainHeightRange.y

// 地形近与远平面的分界面
float	g_fTerrainFarStart		: GLOBAL;

// 远平面处像素大小(用于计算纹理LOD)
float2 	g_vFarPixelSize			: GLOBAL;

// 地形材质信息
float3 	g_vTerrainDiffuseMaterial	: GLOBAL;
float3 	g_vTerrainSpecularMaterial	: GLOBAL;
float	g_fTerrainShiness			: GLOBAL;

// Diffuse贴图集信息
float4	g_vDiffuseAtlasTableParam	: GLOBAL;
#define g_fDiffuseAtlasIdScale		g_vDiffuseAtlasTableParam.x
#define g_fDiffuseAtlasIdOffset		g_vDiffuseAtlasTableParam.y
#define g_fDiffuseAtlasLevelScale	g_vDiffuseAtlasTableParam.z
#define g_fDiffuseAtlasLevelOffset	g_vDiffuseAtlasTableParam.w

// Normal贴图集信息
float4	g_vNormalAtlasTableParam	: GLOBAL;
#define g_fNormalAtlasIdScale		g_vNormalAtlasTableParam.x
#define g_fNormalAtlasIdOffset		g_vNormalAtlasTableParam.y
#define g_fNormalAtlasLevelScale	g_vNormalAtlasTableParam.z
#define g_fNormalAtlasLevelOffset	g_vNormalAtlasTableParam.w

// 效果控制
float3	g_vTerrainLODControl		: GLOBAL;
#define	g_fTerrainUseLightMap		g_vTerrainLODControl.x
#define	g_fTerrainUseSpecular		g_vTerrainLODControl.y

// 调试参数
float4 	g_vTerrainDebugParams		: GLOBAL;
#define g_fTerrainShowInvisibleLayers	g_vTerrainDebugParams.x		///<
#define g_fTerrainShowTileGrid			g_vTerrainDebugParams.y		///< 显示TexTile边界
#define g_fTerrainShowCurrentLayer		g_vTerrainDebugParams.z		///< 显示当前图层
#define g_fTerrainCurrentLayerIndex		g_vTerrainDebugParams.w		///< 显示当前图层中所指的当前图层索引	

//---------------------------------------------------------------------------------------
// 图集采样(用于Diffuse图集和Normal图集)
//---------------------------------------------------------------------------------------
float4 SamplerAtlasMap(sampler kAtlasMap, sampler kAtlasTable, float2 vTableUV, float2 vTerrainUV)
{
	// AtlasTable的像素信息:
	//	.x	LayerMap相对于地形的缩放
	//	.y 	(不清楚)
	//	.zw	分别是XY方向LayerMap对PackedMap的相对位置(已经修正半像素偏移)
	//
	// 采样图集查找表
	float4 vTileInfo = tex2D(kAtlasTable, vTableUV);
	
	// 计算图集采样坐标
	float2 vAtlasTavleUV = frac(vTerrainUV * vTileInfo.x) * vTileInfo.y + vTileInfo.zw;
	
	// 返回贴图采样值
	return tex2D(kAtlasMap, vAtlasTavleUV);
}
//---------------------------------------------------------------------------------------
float4 SamplerAtlasMap_Planar(sampler kAtlasMap, sampler kAtlasTable, float2 vTableUV,
	float2 vTerrainUV_XY, float2 vTerrainUV_YZ,  float2 vTerrainUV_XZ, float3 vWeight)
{
	// AtlasTable的像素信息:
	//	.x	LayerMap相对于地形的缩放
	//	.y 	(不清楚)
	//	.zw	分别是XY方向LayerMap对PackedMap的相对位置(已经修正半像素偏移)
	//
	// 采样图集查找表
	float4 vTileInfo = tex2D(kAtlasTable, vTableUV);
	
	// 计算图集采样坐标
	float2 vAtlasTavleUV_XY = frac(vTerrainUV_XY * vTileInfo.x) * vTileInfo.y + vTileInfo.zw;
	float2 vAtlasTavleUV_YZ = frac(vTerrainUV_YZ * vTileInfo.x) * vTileInfo.y + vTileInfo.zw;
	float2 vAtlasTavleUV_XZ = frac(vTerrainUV_XZ * vTileInfo.x) * vTileInfo.y + vTileInfo.zw;
	
	// 返回贴图采样值
	return tex2D(kAtlasMap, vAtlasTavleUV_XY) * vWeight.z + 
		   tex2D(kAtlasMap, vAtlasTavleUV_YZ) * vWeight.x +
		   tex2D(kAtlasMap, vAtlasTavleUV_XZ) * vWeight.y;
}
//---------------------------------------------------------------------------------------
#endif