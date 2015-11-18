//*************************************************************************************************
// 内容:	地形系统
//---------------------------------------------------------
// 作者:		
// 创建:		2012-07-31
// 最后修改:	2013-05-02
//*************************************************************************************************
#pragma once
#ifndef _TERRAIN_H__
#define _TERRAIN_H__
#include "sdEntity.h"
#include "sdHeightMap.h"
#include "sdNormalMap.h"
#include "sdLayerMap.h"
#include "sdQuadNode.h"
#include "sdTerrainPick.h"

// 渲染系统
#include "IRenderSystem.h"
#include "sdDynamicTexture.h"
#include "sdTextureAtlas.h"

//-------------------------------------------------------------------------------------------------
// 地形系统,所有对外接口均由此类暴露出去.
//	1.地形中所有内容同一分为三步:更新,裁减,渲染
//	2.地形分为几何部分和纹理部分分别处理
//
// 几何部分:
//	1.Tile管理: 四叉树管理,从中间节点到叶子节点均预生成Mesh,Node内部没有LOD
//			(可以考虑利用缓存,动态生成Mesh来改进效率)
//  2.Tile边缘接缝:下拉裙边的方式解决LOD边缘接缝,下拉长度是节点到下一级QuadNode的最大误差
//  3.Tile尺寸: 64*64格, 65*65点, 加上裙边67*67点 
//  3.Tile渲染时进行顶点压缩,以减少数据传输
//			XY存入两个UINT8
//			Z变换后存入两个UINT8
//  4.Tile渲染时使用TriangleStrip来排列顶点,使用Index, 
//		依据GPU顶点缓存默认大小为24来优化排列,网格按照Cache大小分成数栏
//		
// 纹理部分:
//	1.使用Texture Atlas技术技术进行Texture Pack处理
//
// \TODO 保存更多的Atlas信息,在游戏中实时生成DiffuseAtlas和NormalAtlas
// \TODO 在内存中使用DXT1格式来减小显存使用量
// \TODO 在游戏和编辑器使用不同的Atlas方式
// \TODO 在游戏中使用Cache来预生成纹理
//-------------------------------------------------------------------------------------------------
class sdTerrain : public sdEntity
{
	friend class sdQuadMesh;
	friend class sdQuadNode;
	friend class sdNormalMap;
	friend class sdTerrainStream;

	NiDeclareRTTI;
	NiDeclareStream;

	DECLARE_PROPFUNC_MAP()
	DECLARE_DYNAMIC(sdTerrain)

public:
	sdTerrain();
	virtual ~sdTerrain();

	// 
	bool	CreateScene(uint uiTerrainSize, uint uiBlendTextureSize);
	void	DestroyScene();
	bool	LoadScene(const char* szSceneFullPath);
	bool	SaveScene(const char* szSceneFullPath);
	bool	HasScene() { return m_bInitialized;};

	// 地形裁剪(世界坐标)
	//(wz封装了一层TerrainTileEntity,貌似没啥必要,这里直接返回Mesh)
	void	Cull(const NiCamera& kCamera, std::vector<NiMesh*>& kMeshVec);

	// 地形拾取(世界坐标)
	bool	Pick(const Base::Math::sdRay& kRay, Base::Math::sdVector3& kIntersect, float fLimit = FLT_MAX);	

	// 地形高度获取(世界坐标)
	float	GetHeight(float fX, float fY);

	// 高度编辑(函数均用于地形编辑器)
	// @{
	// 地形高度获取与设置(高度图坐标,直接从高度图取出和设置,没有进行变换)
	float	GetRawHeight(uint uiX, uint uiY);
	void	SetRawHeight(uint uiX, uint uiY, float fHeight);

	// 更新几何体(世界坐标)
	void	UpdateGeometry(float fCenterX, float fCenterY, float fRadius);
	// @}

	// 图层编辑(函数大部用于地形编辑器)
	// @{
	uint	GetLayerNum() const { return m_kLayerMapVec.size();}
	uint	GetLayerIndex(const char* szName) const;	///< 出错时返回-1(0xfffffffe)

	sdLayerMap*	GetLayerMap(uint uiIndex) const;
	sdLayerMap* GetLayerMap(const char* szName) const;

	bool	AppendLayerMap(sdLayerMap* pkLayerMap);
	bool	InsertLayerMap(sdLayerMap* pkLayerMap, uint uiIndex);

	bool	RemoveLayerMap();
	bool	RemoveLayerMap(uint uiIndex);

	bool	SwapLayerMap(uint uiLIndex, uint uiRIndex);

	// 图层像素点权值的获取与设置
	uchar	GetBlendMapData(uint uiX, uint uiY, uint uiLayer);
	void	SetBlendMapData(uint uiX, uint uiY, uint uiLayer, uchar uiWeight);

	// 更新混合贴图(世界坐标)
	//	@param[in]	fCenterX,fCenterY,rRadius	圆心位置与圆半径(世界坐标)
	void	UpdateBlendMap(float fCenterX, float fCenterY, float fRadius);

	// 对贴图进行重新打包
	void	RepackMaps();
	// @}

	// 常用属性访问
	// @{
	bool	IsVisible() const { return m_bIsVisible;}

	bool	GetEnableEditHeight() const { return m_bEnableEditHeight;}
	void	SetEnableEditHeight(bool bEnable) { m_bEnableEditHeight = bEnable;}

	bool	GetEnableEditMaterial() const { return m_bEnableEditMaterial;}
	void	SetEnableEditMaterial(bool bEnable) { m_bEnableEditMaterial = bEnable;}

	uint	GetTileSize() const { return m_uiTileSize;}
	uint	GetTerrainSize() const { return m_uiTerrainSize;}
	float	GetHeightMapScale() const { return m_fMeterPerGrid;}

	uint	GetTexTileSize() const { return m_uiTexTileSize;}
	uint	GetBlendMapSize() const { return m_uiBlendTexSize;}
	uint	GetTileMapSize() const { return m_uiTileMapSize;}
	float	GetBlendMapScale() const { return m_fMeterPerPixel;}

	uint	GetMeshLevel() const { return m_uiMeshLevel;}
	bool	GetEnableLOD() const { return m_bEnableLOD;}

	float	GetScale() const { return m_fScale;}
	const NiPoint3& GetOrigin() const { return m_kOrigin;}

	float	GetErrorToDistance() const { return m_fError2Distance;}

	const Engine::RenderSystem::sdTerrainParams& GetTerrainParams() const { return m_kRenderParams;};
	Engine::RenderSystem::sdTerrainParams& GetTerrainParams() { return m_kRenderParams;};
	// @}

	// 调试参数(用于编辑器)
	// @{
	bool	GetShowTileGrid() const { return m_kRenderParams.showTileGrid;}
	void	SetShowTileGrid(bool bShow) { m_kRenderParams.showTileGrid = bShow;}

	bool	GetShowCurrentLayer() const { return m_kRenderParams.showCurrentLayer;}
	void	SetShowCurrentLayer(bool bShow) { m_kRenderParams.showCurrentLayer = bShow;}
	void	SetCurrentLayerIndex(uint uiIndex){ m_kRenderParams.currentLauerIndex = uiIndex;}

	bool	GetShowInversibleLayers() const { return m_kRenderParams.showInvisiableLayers;}
	void	SetShowInversibleLayers(bool bShow) { m_kRenderParams.showInvisiableLayers = bShow;}
	// @}

	// 获取高度图(内部使用,编辑器使用)
	sdHeightMap*	GetHeightMap() { return m_pkHeightMap;};

protected:
	//
	// 获取指定像素点的各个图层权重列表（wz原始版本提取方式）
	//
	//	@param[in]	uiX,uiY		DetailMap的像素掩码位置
	//	@param[out]	pucWeights	对应像素掩码位置的所有图层的权重值
	//	return					有效的图层数
	//
	uint	GetWeights(uint uiX, uint uiY, uchar* pucWeights);

protected:
	bool	m_bInitialized;			// 是否初始化
	bool	m_bIsVisible;			// 是否可见(一般用于编辑器)	
	bool	m_bEnableEditHeight;	// 是否允许编辑地形高度
	bool	m_bEnableEditMaterial;	// 是否允许编辑地形材质

	// 地形世界偏移与缩放
	NiPoint3	m_kOrigin;	// 地形整体偏移(默认为(0,0,0))
	float		m_fScale;	// 地形整体缩放(默认1.0f)

	// 地表高度图
	sdHeightMapPtr	m_pkHeightMap;	

	// 高度拾取
	sdTerrainPickPtr m_pkPick;

	// 地表法线图
	sdNormalMapPtr	m_pkNormalMap;

	// 地表图层集合和图层混合信息集合
	typedef std::vector<sdLayerMapPtr> LayerMapVec;
	typedef std::vector<sdLayerMapPtr>::iterator LayerMapVecItr;
	typedef std::vector<sdLayerMapPtr>::const_iterator LayerMapVecConItr;
	LayerMapVec		m_kLayerMapVec;								///< 仅用于编辑器				

	typedef std::vector<uchar*> LayerAlphaVec;
	typedef std::vector<uchar*>::iterator LayerAlphaVecItr;
	LayerAlphaVec	m_kLayerAlphaVec;							///< 仅用于编辑器

	// 地表混合贴图与查找表
	Engine::sdDynamicTexturePtr	m_pkDynamicBlendMap;			///< 仅用于编辑器
	NiTexturePtr	m_spBlendMap;

	Engine::sdDynamicTexturePtr	m_pkDynamicTileMap;				///< 仅用于编辑器
	NiTexturePtr	m_spTileMap;	

	// 地表漫反射贴图集和查找表
	Engine::sdTextureAtlasPtr m_pkDiffuseAtlas;					///< 仅用于编辑器
	NiTexturePtr	m_spDiffuseAtlasMap;
	NiTexturePtr	m_spDiffuseAtlasTable;

	// 地表法线贴图集和查找表
	Engine::sdTextureAtlasPtr m_pkNormalAtlas;					///< 仅用于编辑器
	NiTexturePtr	m_spNormalAtlasMap;
	NiTexturePtr	m_spNormalAtlasTable;

	// 地形四叉树
	sdQuadNodePtr	m_pkQuadRoot;

	// 地形材质
	//NiColorA m_kAmbient;
	//NiColorA m_kDiffuse;
	//NiColorA m_kSpecular;
	//float m_fShiness;

	// 地形绘制参数(用于提供给渲染系统使用,内部不应该被分配内存和保存资源)
	Engine::RenderSystem::sdTerrainParams m_kRenderParams;

	uint	m_uiTerrainSize;	// Terrain的尺寸(256/512/1024/2048,单位Grid)
	float	m_fMeterPerGrid;	// 每单位大小(0.125/0.25/0.5/1/2/4/8,目前是1m)
	uint	m_uiTileSize;		// Terrain的Tile的尺寸(目前是64Unit)

	uint	m_uiBlendTexSize;	// BlendMap尺寸(256/512/1024/2048,单位Pixel)
	float	m_fMeterPerPixel;	// 每像素大小(0.125/0.25/0.5/1/2/4/8,目前是0.5f)
	uint	m_uiTexTileSize;	// BlendMap的TexTile尺寸(目前是4Pixel)
	uint	m_uiTileMapSize;	// TileMap的尺寸

	uint	m_uiMeshLevel;		// 四叉树中,只有小于一定层级的才会构建Mesh
	bool	m_bEnableLOD;		// 四叉树是否允许LOD显示
	bool	m_bEnableNormal;	// 材质是否允许法线贴图

	// 根据当前视锥体与分辨率计算出来的Error与Distance之间的转换率,用于控制LOD的切换
	// (多级切换,延迟切换,一般是一个数组,这里有待参考CryEngine)
	float	m_fError2Distance;	

protected:
	// TexTile进行权重混合时的过滤器(用于编辑器)
	// @{
	struct stFilterMap
	{
		uint	uiIdx[4];		///< 4个取样点的索引
		float	fWeight[4];		///< 4个取样点的权重
	};
	typedef std::vector<stFilterMap> FilterMapVec;;
	typedef std::vector<stFilterMap>::iterator FilterMapVecItr;
	FilterMapVec m_kTileFilterMapVec;
	// @}

protected:
	static uint	ms_uiMaxLayerCount;	///< 最大纹理层数


};
//-------------------------------------------------------------------------------------------------
#endif