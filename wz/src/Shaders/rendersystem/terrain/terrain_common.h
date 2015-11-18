//*************************************************************************************************
// ����:	������ɫ���Ĺ�������
//---------------------------------------------------------
// ����:		
// ����:		2012-08-04
// ����޸�:	2013-05-02
//*************************************************************************************************
#ifndef _TERRAIN_COMMON_H__
#define _TERRAIN_COMMON_H__

//
#include "../common.h"

//---------------------------------------------------------------------------------------
// ��ر���
//---------------------------------------------------------------------------------------
// ����Tile������Ϣ
// [origin_x][origin_y][scale][morph_factor]
float4	a_vTerrainQuadParam 	: ATTRIBUTE;
#define	a_vQuadOrigin			float2(a_vTerrainQuadParam[0], a_vTerrainQuadParam[1])
#define	a_fQuadScale			a_vTerrainQuadParam[2]
#define	a_fQuadMorphFactor		a_vTerrainQuadParam[3]

// ����һЩ��ͼ�ĳߴ�(���ڼ���UV)
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

// ���θ߶ȷ�Χ(����ѹ������)
float2  g_vTerrainHeightRange	: GLOBAL;
#define g_fTerrainMinHeight		g_vTerrainHeightRange.x
#define g_fTerrainMaxHeight		g_vTerrainHeightRange.y

// ���ν���Զƽ��ķֽ���
float	g_fTerrainFarStart		: GLOBAL;

// Զƽ�洦���ش�С(���ڼ�������LOD)
float2 	g_vFarPixelSize			: GLOBAL;

// ���β�����Ϣ
float3 	g_vTerrainDiffuseMaterial	: GLOBAL;
float3 	g_vTerrainSpecularMaterial	: GLOBAL;
float	g_fTerrainShiness			: GLOBAL;

// Diffuse��ͼ����Ϣ
float4	g_vDiffuseAtlasTableParam	: GLOBAL;
#define g_fDiffuseAtlasIdScale		g_vDiffuseAtlasTableParam.x
#define g_fDiffuseAtlasIdOffset		g_vDiffuseAtlasTableParam.y
#define g_fDiffuseAtlasLevelScale	g_vDiffuseAtlasTableParam.z
#define g_fDiffuseAtlasLevelOffset	g_vDiffuseAtlasTableParam.w

// Normal��ͼ����Ϣ
float4	g_vNormalAtlasTableParam	: GLOBAL;
#define g_fNormalAtlasIdScale		g_vNormalAtlasTableParam.x
#define g_fNormalAtlasIdOffset		g_vNormalAtlasTableParam.y
#define g_fNormalAtlasLevelScale	g_vNormalAtlasTableParam.z
#define g_fNormalAtlasLevelOffset	g_vNormalAtlasTableParam.w

// Ч������
float3	g_vTerrainLODControl		: GLOBAL;
#define	g_fTerrainUseLightMap		g_vTerrainLODControl.x
#define	g_fTerrainUseSpecular		g_vTerrainLODControl.y

// ���Բ���
float4 	g_vTerrainDebugParams		: GLOBAL;
#define g_fTerrainShowInvisibleLayers	g_vTerrainDebugParams.x		///<
#define g_fTerrainShowTileGrid			g_vTerrainDebugParams.y		///< ��ʾTexTile�߽�
#define g_fTerrainShowCurrentLayer		g_vTerrainDebugParams.z		///< ��ʾ��ǰͼ��
#define g_fTerrainCurrentLayerIndex		g_vTerrainDebugParams.w		///< ��ʾ��ǰͼ������ָ�ĵ�ǰͼ������	

//---------------------------------------------------------------------------------------
// ͼ������(����Diffuseͼ����Normalͼ��)
//---------------------------------------------------------------------------------------
float4 SamplerAtlasMap(sampler kAtlasMap, sampler kAtlasTable, float2 vTableUV, float2 vTerrainUV)
{
	// AtlasTable��������Ϣ:
	//	.x	LayerMap����ڵ��ε�����
	//	.y 	(�����)
	//	.zw	�ֱ���XY����LayerMap��PackedMap�����λ��(�Ѿ�����������ƫ��)
	//
	// ����ͼ�����ұ�
	float4 vTileInfo = tex2D(kAtlasTable, vTableUV);
	
	// ����ͼ����������
	float2 vAtlasTavleUV = frac(vTerrainUV * vTileInfo.x) * vTileInfo.y + vTileInfo.zw;
	
	// ������ͼ����ֵ
	return tex2D(kAtlasMap, vAtlasTavleUV);
}
//---------------------------------------------------------------------------------------
float4 SamplerAtlasMap_Planar(sampler kAtlasMap, sampler kAtlasTable, float2 vTableUV,
	float2 vTerrainUV_XY, float2 vTerrainUV_YZ,  float2 vTerrainUV_XZ, float3 vWeight)
{
	// AtlasTable��������Ϣ:
	//	.x	LayerMap����ڵ��ε�����
	//	.y 	(�����)
	//	.zw	�ֱ���XY����LayerMap��PackedMap�����λ��(�Ѿ�����������ƫ��)
	//
	// ����ͼ�����ұ�
	float4 vTileInfo = tex2D(kAtlasTable, vTableUV);
	
	// ����ͼ����������
	float2 vAtlasTavleUV_XY = frac(vTerrainUV_XY * vTileInfo.x) * vTileInfo.y + vTileInfo.zw;
	float2 vAtlasTavleUV_YZ = frac(vTerrainUV_YZ * vTileInfo.x) * vTileInfo.y + vTileInfo.zw;
	float2 vAtlasTavleUV_XZ = frac(vTerrainUV_XZ * vTileInfo.x) * vTileInfo.y + vTileInfo.zw;
	
	// ������ͼ����ֵ
	return tex2D(kAtlasMap, vAtlasTavleUV_XY) * vWeight.z + 
		   tex2D(kAtlasMap, vAtlasTavleUV_YZ) * vWeight.x +
		   tex2D(kAtlasMap, vAtlasTavleUV_XZ) * vWeight.y;
}
//---------------------------------------------------------------------------------------
#endif