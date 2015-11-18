//*************************************************************************************************
// 内容:	地形数据流加载与保存
//---------------------------------------------------------
// 作者:		
// 创建:		2012-09-05
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_TERRAIN_STREAM_H__
#define _SD_ENGINE_TERRAIN_STREAM_H__
#include "sdHeightMap.h"
#include "sdNormalMap.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
class sdDynamicTexture;
class sdTextureAtlas;
ENGINE_NAMESPACE_END_ENGINE

class sdTerrain;
//-------------------------------------------------------------------------------------------------
// 用于独立的保存与加载地形数据
//  \TODO	考虑改进为GB的Stream形式
//	\TODO	考虑改进为对象的形式而不是静态函数集合
//	\TODO	手动存储为DDS格式
//	\TODO	中间数据压缩为DDS格式(DetailAlpha/LightMap)
//	\TODO	动态重建AtlasMap
//-------------------------------------------------------------------------------------------------
class sdTerrainStream : public NiRefObject
{
public:
	sdTerrainStream(const char* szSceneFullPath);

	// 加载与保存配置信息
	bool SaveConfig(sdTerrain* pkTerrain);
	bool LoadConfig(sdTerrain* pkTerrain);

	// 加载与保存高度图
	bool SaveHeightMapConfig(sdHeightMap* pkHeightMap);
	sdHeightMapPtr LoadHeightMapConfig();

	bool SaveHeightMap(sdHeightMap* pkHeightMap);
	bool LoadHeightMap(sdHeightMap* pkHeightMap);

	// 加载与保存法线贴图
	bool SaveNormalMap(sdNormalMap* pkNormalMap);
	sdNormalMapPtr LoadNormalMap();

	// 加载和保存图层文件
	bool SaveLayerMapConfig(sdTerrain* pkTerrain);
	bool LoadLayerMapConfig(sdTerrain* pkTerrain);

	bool SaveLayerMap(sdTerrain* pkTerrain);
	bool LoadLayerMap(sdTerrain* pkTerrain);

	// 加载和保存混合贴图
	// (这里使用D3DX来处理,WZ原始方式是自己手工处理)
	bool SaveBlendMap(Engine::sdDynamicTexture* pkDynamicBlendMap);
	NiTexturePtr LoadBlendMap();

	// 加载和保存混合索引贴图
	// (这里使用D3DX来处理,WZ原始方式是自己手工处理)
	bool SaveTileMap(Engine::sdDynamicTexture* pkDynamicTileMap);
	NiTexturePtr LoadTileMap();

	// 加载与保存漫反射贴图集与法线贴图集参数
	bool SaveAtlasConfig(Engine::sdTextureAtlas* pkDiffuseAtlas, Engine::sdTextureAtlas* pkNormalAtlas);
	bool LoadAtlasConfig(sdTerrain* pkTerrain);

	// 加载和保存漫反射贴图集与法线贴图集索引
	bool SaveDiffuseAtlasMap(NiTexture* spDiffuseMap);
	bool SaveDiffuseAtlasTableMap(NiTexture* spDiffuseTableMap);
	NiTexturePtr LoadDiffuseAtlasMap();
	NiTexturePtr LoadDiffuseAtlasTableMap();

	// 加载和保存混合贴图与混合贴图索引
	bool SaveNormalAtlasMap(NiTexture* spNormalMap);
	bool SaveNormalAtlasTableMap(NiTexture* spNormalTableMap);
	NiTexturePtr LoadNormalAtlasMap();
	NiTexturePtr LoadNormalAtlasTableMap();

protected:
	char m_szSceneFullPath[MAX_PATH];	///< 场景全路径
	bool m_bCompactHeightMap;			///< 是否使用压缩高度图(内部传递变量)
};
NiSmartPointer(sdTerrainStream);
//-------------------------------------------------------------------------------------------------
#endif