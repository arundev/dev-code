#include "sdEnginePCH.h"
#include "sdTerrain.h"
#include "sdTerrainStream.h"

NiImplementRTTI(sdTerrain, sdEntity);
sdImplementDefaultStream(sdTerrain);

IMPL_PROPFUNC_MAP(sdTerrain, sdEntity)
IMPL_DYNAMIC(sdTerrain, sdEntity)

using namespace Base;
using namespace Base::Math;
using namespace Engine;
using namespace Engine::RenderSystem;
//-------------------------------------------------------------------------------------------------
sdTerrain::sdTerrain()
: m_bInitialized(false)
, m_bIsVisible(true)
, m_bEnableEditHeight(false)
, m_bEnableEditMaterial(false)
, m_bEnableLOD(true)
, m_fMeterPerGrid(1.0f)
, m_uiTileSize(64)
, m_uiTexTileSize(4)
, m_fMeterPerPixel(0.5f)
, m_uiMeshLevel(3)
, m_fScale(1.0f)
, m_kOrigin(NiPoint3::ZERO)
, m_fError2Distance(0.0f)
{
	// 方案一:计算Error与LOD转换系数(考虑参考方案二来理解)
	// @{
	float fScreenWidth	= 1024.f;						///< 默认分辨率
	float fScreenHeight = 768.f;						///< 默认分辨率
	float fFOV			= sdMath::Degree2Radian(60.f);	///< 默认FOV
	float fMaxLodError	= 100.f;						///< WZ为什么是100(貌似是个调整值)
	m_fError2Distance = fScreenHeight / (fabsf(tanf(fFOV / 2.f)) * 2.f * fMaxLodError);
	// @}

	// 方案二:根据相机和屏幕分辨率计算LOD距离转换系数
	//	1.计算远裁剪面单位像素对应的世界长度
	//	2.计算转换系数
	//	3.调整转换系数
	// @{
	//uint uiHeight = 768;								///< 默认分辨率
	//const NiFrustum& kCamFrustum = kCamera.GetViewFrustum();
	//float fDeltaY = (kCamFrustum.m_fTop - kCamFrustum.m_fBottom) * kCamFrustum.m_fFar  / (float)uiHeight;
	//m_fError2Distance = kCamFrustum.m_fFar / fDeltaY;
	//m_fError2Distance /= 100.f;
	// @}
}
//-------------------------------------------------------------------------------------------------
sdTerrain::~sdTerrain()
{
	DestroyScene();
}
//-------------------------------------------------------------------------------------------------
bool sdTerrain::CreateScene(uint uiTerrainSize, uint uiBlendTexSize)
{
	if (m_bInitialized)
	{
		DestroyScene();
		m_bInitialized = false;
	}

	// 检查高度图尺寸,混合贴图尺寸
	//
	//
	m_uiTerrainSize		= uiTerrainSize;
	m_uiBlendTexSize	= uiBlendTexSize;
	m_uiTileMapSize		= uiBlendTexSize / m_uiTexTileSize;
	m_fMeterPerPixel	= uiTerrainSize / (float)uiBlendTexSize;
	
	// 初始化静态成员
	sdQuadMesh::StaticInitialize(GetTileSize());

	// 创建高度图
	m_pkHeightMap = NiNew sdHeightMap(uiTerrainSize);
	NIASSERT(m_pkHeightMap);

	// 地形拾取
	m_pkPick = NiNew sdTerrainPick(m_pkHeightMap);
	NIASSERT(m_pkPick);

	// 创建法线贴图
	m_pkNormalMap = NiNew sdNormalMap(uiTerrainSize, m_pkHeightMap);
	NIASSERT(m_pkNormalMap);
	m_pkNormalMap->Update();

	if (m_bEnableEditMaterial)
	{
		// 创建混合贴图
		// @{
		m_pkDynamicBlendMap = NiNew sdDynamicTexture(uiBlendTexSize, uiBlendTexSize, sdDynamicTexture::E_FORMAT_A4R4G4B4);
		NIASSERT(m_pkDynamicBlendMap);

		sdDynamicTexture::sdLockedRect kLockedRect = m_pkDynamicBlendMap->LockRegion(0, 0, uiBlendTexSize, uiBlendTexSize);
		NIASSERT(kLockedRect.pBits);
		::ZeroMemory(kLockedRect.pBits, kLockedRect.uiPitch * uiBlendTexSize);
		m_pkDynamicBlendMap->UnlockRegion();

		m_spBlendMap = m_pkDynamicBlendMap->GetGBTexture();
		NIASSERT(m_spBlendMap);
		// @}

		// 创建混合贴图瓦片贴图(用于保存每个瓦片内的四层混合贴图信息)
		// @{
		m_pkDynamicTileMap = NiNew sdDynamicTexture(m_uiTileMapSize, m_uiTileMapSize, sdDynamicTexture::E_FORMAT_A8R8G8B8);
		NIASSERT(m_pkDynamicTileMap);

		kLockedRect = m_pkDynamicTileMap->LockRegion(0, 0, m_uiTileMapSize, m_uiTileMapSize);
		NIASSERT(kLockedRect.pBits);
		::ZeroMemory(kLockedRect.pBits, kLockedRect.uiPitch * m_uiTileMapSize);
		m_pkDynamicTileMap->UnlockRegion();

		m_spTileMap = m_pkDynamicTileMap->GetGBTexture();
		NIASSERT(m_spTileMap);
		// @}

		// 创建拼接器
		// @{
		m_pkDiffuseAtlas = NiNew sdTextureAtlas(D3DFMT_DXT5);
		NIASSERT(m_pkDiffuseAtlas);

		m_pkNormalAtlas = NiNew sdTextureAtlas(D3DFMT_DXT5);
		NIASSERT(m_pkNormalAtlas);
		// @}

		//
		// 初始化Tile混合过滤器，这里为4x4的Tile的每个像素在6x6的一组采样区域上,
		// 预计算的采样点和采样权重。
		//	1.uiIdx保存每个采样点在采样区域上的采样索引(一维)
		//	2.fWeight保存每个采样点的采样权重
		//
		//  v50--v51--v52--v53--v54--v55
		//   |	  |    |    |    |    |
		//	v40--v41--v42--v43--v44--v45
		//   |	  |    |    |    |    |
		//	v30--v31--v32--v33--v34--v35
		//   |	  |    |    |    |    |
		//	v20--v21--v22--v23--v24--v25
		//   |	  |    |    |    |    |
		//	v10--v11--v12--v13--v14--v15
		//   |	  |    |    |    |    |
		//	v00--v01--v02--v03--v04--v05
		//
		m_kTileFilterMapVec.resize(m_uiTexTileSize * m_uiTexTileSize);
		float fOffset = 0.5f;
		float fScale = m_uiTexTileSize / (m_uiTexTileSize - 1.0f);
		for (uint uiY = 0; uiY < m_uiTexTileSize; ++uiY)
		{
			float fPosY = uiY * fScale + fOffset;
			uint uiTop = (uint)floor(fPosY);
			uint uiBottom = uiTop + 1;
			float fFracY = fPosY - uiTop;

			for (uint uiX = 0; uiX < m_uiTexTileSize; ++uiX)
			{
				float fPosX = uiX * fScale + fOffset;
				uint uiLeft = (uint)floor(fPosX);
				uint uiRight = uiLeft + 1;
				float fFracX = fPosX - uiLeft;

				stFilterMap& kMap = m_kTileFilterMapVec[uiY * m_uiTexTileSize + uiX];
				kMap.uiIdx[0] = uiTop * (m_uiTexTileSize + 2) + uiLeft;	
				kMap.uiIdx[1] = uiTop * (m_uiTexTileSize + 2) + uiLeft + 1;	
				kMap.uiIdx[2] = (uiTop + 1) * (m_uiTexTileSize + 2) + uiLeft;
				kMap.uiIdx[3] = (uiTop + 1) * (m_uiTexTileSize + 2) + uiLeft + 1;

				kMap.fWeight[0] = (1.0f - fFracX) * (1.0f - fFracY);
				kMap.fWeight[1] = fFracX * (1.0f - fFracY);
				kMap.fWeight[2] = (1.0f - fFracX) * fFracY;
				kMap.fWeight[3] = fFracX * fFracY;
			}
		}
	}

	// 构建四叉树(依据高度图)
	m_pkQuadRoot = NiNew sdQuadNode(this);
	m_pkQuadRoot->Build(sdVector4ui(0, 0, uiTerrainSize, uiTerrainSize), NULL);

	// 初始化地形渲染信息
	// @{
	m_kRenderParams.Reset();

	m_kRenderParams.terrainSize		= m_uiTerrainSize;
	m_kRenderParams.blendMapSize	= m_uiBlendTexSize;
	m_kRenderParams.tileMapSize		= m_uiTileMapSize;

	m_kRenderParams.minHeight = m_pkHeightMap->GetMinHeight();
	m_kRenderParams.maxHeight = m_pkHeightMap->GetMaxHeight();

	m_kRenderParams.ambientMaterial	 = sdVector3(0, 0, 0);
	m_kRenderParams.diffuseMaterial	 = sdVector3(1.f, 1.f, 1.f);
	m_kRenderParams.specularMaterial = sdVector3(1.f, 1.f, 1.);
	m_kRenderParams.shiness			 = 1.f;

	m_kRenderParams.baseNormalMap = m_pkNormalMap->GetGBTexture();
	m_kRenderParams.tileMap		= m_spTileMap;
	m_kRenderParams.blendMap	= m_spBlendMap;	

	m_kRenderParams.terrainFarStart = 64.f;
	// @}

	return (m_bInitialized = true);
}
//-------------------------------------------------------------------------------------------------
bool sdTerrain::SaveScene(const char* szSceneFullPath)
{
	NIASSERT(szSceneFullPath);

	if (!m_bInitialized)
		return false;

	// 保存
	sdTerrainStream kTerrainStream(szSceneFullPath);
	kTerrainStream.SaveConfig(this);
	kTerrainStream.SaveHeightMapConfig(m_pkHeightMap);
	kTerrainStream.SaveHeightMap(m_pkHeightMap);
	kTerrainStream.SaveNormalMap(m_pkNormalMap);
	kTerrainStream.SaveLayerMapConfig(this);
	kTerrainStream.SaveLayerMap(this);
	kTerrainStream.SaveBlendMap(m_pkDynamicBlendMap);
	kTerrainStream.SaveTileMap(m_pkDynamicTileMap);
	kTerrainStream.SaveAtlasConfig(m_pkDiffuseAtlas, m_pkNormalAtlas);
	kTerrainStream.SaveDiffuseAtlasMap(m_spDiffuseAtlasMap);
	kTerrainStream.SaveDiffuseAtlasTableMap(m_spDiffuseAtlasTable);
	kTerrainStream.SaveNormalAtlasMap(m_spNormalAtlasMap);
	kTerrainStream.SaveNormalAtlasTableMap(m_spNormalAtlasTable);

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdTerrain::LoadScene(const char* szSceneFullPath)
{
	NIASSERT(szSceneFullPath);

	if (m_bInitialized)
	{
		DestroyScene();
		m_bInitialized = false;
	}

	// 重置渲染参数
	m_kRenderParams.Reset();

	// 读取配置文件
	sdTerrainStream kTerrainStream(szSceneFullPath);
	if (!kTerrainStream.LoadConfig(this))
	{
		NIASSERT(0);
		return false;
	}

	m_uiTileMapSize = m_uiBlendTexSize / m_uiTexTileSize;
	m_fMeterPerPixel = m_uiTerrainSize / (float)m_uiBlendTexSize;

	// 初始化静态成员
	sdQuadMesh::StaticInitialize(GetTileSize());

	// 创建高度图
	m_pkHeightMap = kTerrainStream.LoadHeightMapConfig();
	NIASSERT(m_pkHeightMap);

	kTerrainStream.LoadHeightMap(m_pkHeightMap);

	// 地形拾取
	m_pkPick = NiNew sdTerrainPick(m_pkHeightMap);
	NIASSERT(m_pkPick);

	// 创建法线贴图
	if (m_bEnableEditHeight)
	{
		m_pkNormalMap = NiNew sdNormalMap(m_uiTerrainSize, m_pkHeightMap);
		NIASSERT(m_pkNormalMap);
		m_pkNormalMap->Update();
	}
	else
	{
		m_pkNormalMap = kTerrainStream.LoadNormalMap();
		NIASSERT(m_pkNormalMap);
	}

	// 创建贴图
	if (m_bEnableEditMaterial)
	{
		// 创建混合贴图
		// @{
		m_pkDynamicBlendMap = NiNew sdDynamicTexture(m_uiBlendTexSize, m_uiBlendTexSize, sdDynamicTexture::E_FORMAT_A4R4G4B4);
		NIASSERT(m_pkDynamicBlendMap);

		sdDynamicTexture::sdLockedRect kLockedRect = m_pkDynamicBlendMap->LockRegion(0, 0, m_uiBlendTexSize, m_uiBlendTexSize);
		NIASSERT(kLockedRect.pBits);
		::ZeroMemory(kLockedRect.pBits, kLockedRect.uiPitch * m_uiBlendTexSize);
		m_pkDynamicBlendMap->UnlockRegion();

		m_spBlendMap = m_pkDynamicBlendMap->GetGBTexture();
		NIASSERT(m_spBlendMap);
		// @}

		// 创建混合贴图瓦片贴图(用于保存每个瓦片内的四层混合贴图信息)
		// @{
		m_pkDynamicTileMap = NiNew sdDynamicTexture(m_uiTileMapSize, m_uiTileMapSize, sdDynamicTexture::E_FORMAT_A8R8G8B8);
		NIASSERT(m_pkDynamicTileMap);

		kLockedRect = m_pkDynamicTileMap->LockRegion(0, 0, m_uiTileMapSize, m_uiTileMapSize);
		NIASSERT(kLockedRect.pBits);
		::ZeroMemory(kLockedRect.pBits, kLockedRect.uiPitch * m_uiTileMapSize);
		m_pkDynamicTileMap->UnlockRegion();

		m_spTileMap = m_pkDynamicTileMap->GetGBTexture();
		NIASSERT(m_spTileMap);
		// @}

		// 创建拼接器
		// @{
		m_pkDiffuseAtlas = NiNew sdTextureAtlas(D3DFMT_DXT5);
		NIASSERT(m_pkDiffuseAtlas);

		m_pkNormalAtlas = NiNew sdTextureAtlas(D3DFMT_DXT5);
		NIASSERT(m_pkNormalAtlas);
		// @}

		//
		// 初始化Tile混合过滤器，这里为4x4的Tile的每个像素在6x6的一组采样区域上,
		// 预计算的采样点和采样权重。
		//	1.uiIdx保存每个采样点在采样区域上的采样索引(一维)
		//	2.fWeight保存每个采样点的采样权重
		//
		//  v50--v51--v52--v53--v54--v55
		//   |	  |    |    |    |    |
		//	v40--v41--v42--v43--v44--v45
		//   |	  |    |    |    |    |
		//	v30--v31--v32--v33--v34--v35
		//   |	  |    |    |    |    |
		//	v20--v21--v22--v23--v24--v25
		//   |	  |    |    |    |    |
		//	v10--v11--v12--v13--v14--v15
		//   |	  |    |    |    |    |
		//	v00--v01--v02--v03--v04--v05
		//
		m_kTileFilterMapVec.resize(m_uiTexTileSize * m_uiTexTileSize);
		float fOffset = 0.5f;
		float fScale = m_uiTexTileSize / (m_uiTexTileSize - 1.0f);
		for (uint uiY = 0; uiY < m_uiTexTileSize; ++uiY)
		{
			float fPosY = uiY * fScale + fOffset;
			uint uiTop = (uint)floor(fPosY);
			uint uiBottom = uiTop + 1;
			float fFracY = fPosY - uiTop;

			for (uint uiX = 0; uiX < m_uiTexTileSize; ++uiX)
			{
				float fPosX = uiX * fScale + fOffset;
				uint uiLeft = (uint)floor(fPosX);
				uint uiRight = uiLeft + 1;
				float fFracX = fPosX - uiLeft;

				stFilterMap& kMap = m_kTileFilterMapVec[uiY * m_uiTexTileSize + uiX];
				kMap.uiIdx[0] = uiTop * (m_uiTexTileSize + 2) + uiLeft;	
				kMap.uiIdx[1] = uiTop * (m_uiTexTileSize + 2) + uiLeft + 1;	
				kMap.uiIdx[2] = (uiTop + 1) * (m_uiTexTileSize + 2) + uiLeft;
				kMap.uiIdx[3] = (uiTop + 1) * (m_uiTexTileSize + 2) + uiLeft + 1;

				kMap.fWeight[0] = (1.0f - fFracX) * (1.0f - fFracY);
				kMap.fWeight[1] = fFracX * (1.0f - fFracY);
				kMap.fWeight[2] = (1.0f - fFracX) * fFracY;
				kMap.fWeight[3] = fFracX * fFracY;
			}
		}

		// 加载图层混合权重
		kTerrainStream.LoadLayerMapConfig(this);
		kTerrainStream.LoadLayerMap(this);
	}
	else
	{
		// 加载混合贴图
		m_spBlendMap = kTerrainStream.LoadBlendMap();
		NIASSERT(m_spBlendMap);

		// 加载混合贴图瓦片贴图
		m_spTileMap = kTerrainStream.LoadTileMap();
		NIASSERT(m_spTileMap);

//		//
//		// 方案一：直接载入合成好的地图
//		// 方案二：根据保存的信息动态载入
//		//
//		// 加载贴图集配置参数
//		kTerrainStream.LoadAtlasConfig();
//
//		// 加载地表贴图
//		m_spDiffuseAtlasMap = kTerrainStream.LoadDiffuseAtlasMap();
//		m_spDiffuseAtlasTable = kTerrainStream.LoadDiffuseAtlasTableMap();
//
//		m_spNormalAtlasMap = kTerrainStream.LoadNormalAtlasMap();
//		m_spNormalAtlasTable = kTerrainStream.LoadNormalAtlasTableMap();
	}

	// 构建四叉树(依据高度图)
	m_pkQuadRoot = NiNew sdQuadNode(this);
	m_pkQuadRoot->Build(sdVector4ui(0, 0, m_uiTerrainSize, m_uiTerrainSize), NULL);

	// 初始化地形渲染信息
	// @{
	m_kRenderParams.terrainSize		= m_uiTerrainSize;
	m_kRenderParams.blendMapSize	= m_uiBlendTexSize;
	m_kRenderParams.tileMapSize		= m_uiTileMapSize;

	m_kRenderParams.minHeight = m_pkHeightMap->GetMinHeight();
	m_kRenderParams.maxHeight = m_pkHeightMap->GetMaxHeight();

//	m_kRenderParams.ambientMaterial	 = sdVector3(0, 0, 0);
//	m_kRenderParams.diffuseMaterial	 = sdVector3(1.f, 1.f, 1.f);
//	m_kRenderParams.specularMaterial = sdVector3(0, 0, 0);
//	m_kRenderParams.shiness			 = 1.f;

	m_kRenderParams.baseNormalMap	= m_pkNormalMap->GetGBTexture();
	m_kRenderParams.tileMap			= m_spTileMap;
	m_kRenderParams.blendMap		= m_spBlendMap;	
	m_kRenderParams.diffuseAtlasMap = m_spDiffuseAtlasMap;
	m_kRenderParams.diffuseAtlasTableMap = m_spDiffuseAtlasTable;
	m_kRenderParams.normalAtlasMap	= m_spNormalAtlasMap;
	m_kRenderParams.normalAtlasTableMap	 = m_spNormalAtlasTable;

	m_kRenderParams.terrainFarStart = 64.f;
	// @}

	return (m_bInitialized = true);
}
//-------------------------------------------------------------------------------------------------
void sdTerrain::DestroyScene()
{
	if (m_bInitialized)
	{
		m_pkHeightMap = 0;

		m_pkQuadRoot->Destroy();
		m_pkQuadRoot = 0;

		sdQuadMesh::StaticDestroy();

		m_bInitialized = false;
	}
}
//-------------------------------------------------------------------------------------------------
void sdTerrain::UpdateGeometry(float fCenterX, float fCenterY, float fRadius)
{
	// 更行GeometryMesh
	m_pkQuadRoot->UpdateGeometry(fCenterX, fCenterY, fRadius);

	// 更新NormalMap
	// @{
	// 计算范围(世界坐标)
	float fStartX	= fCenterX - fRadius;
	float fStartY	= fCenterY - fRadius;
	float fEndX	= fCenterX + fRadius;
	float fEndY = fCenterY + fRadius;

	// 计算范围(高度图/法线图坐标)
	float fTotalScale = m_fScale * m_fMeterPerGrid;
	float fNStartX = (fStartX - m_kOrigin.x) / fTotalScale;
	float fNStartY = (fStartY - m_kOrigin.y) / fTotalScale;
	float fNEndX = (fEndX - m_kOrigin.x) / fTotalScale;
	float fNEndY = (fEndY - m_kOrigin.y) / fTotalScale;

	// 钳位到范围内整数
	if (fNStartX < 0)	fNStartX = 0.0f;
	if (fNStartY < 0)	fNStartY = 0.0f;
	if (fNEndX > (float)m_uiTerrainSize)	fNEndX = (float)m_uiTerrainSize;
	if (fNEndY > (float)m_uiTerrainSize)	fNEndY = (float)m_uiTerrainSize;

	uint uiNStartX = (uint)fNStartX;
	uint uiNStartY = (uint)fNStartY;
	uint uiNEndX = (uint)(fNEndX);
	uint uiNEndY = (uint)(fNEndY);

	// 更新法线图
	m_pkNormalMap->Update(uiNStartX, uiNStartY, uiNEndX - uiNStartX, uiNEndY - uiNStartY);
	// @}

	// 
	m_kRenderParams.minHeight = m_pkHeightMap->GetMinHeight();
	m_kRenderParams.maxHeight = m_pkHeightMap->GetMaxHeight();
}
//-------------------------------------------------------------------------------------------------
void sdTerrain::Cull(const NiCamera& kCamera, std::vector<NiMesh*>& kMeshVec)
{
	// 相机裁剪平面
	NiFrustumPlanes kFrustumPlanes;
	kFrustumPlanes.Set(kCamera.GetViewFrustum(), kCamera.GetWorldTransform());
	kFrustumPlanes.EnableAllPlanes();
	
	m_pkQuadRoot->Cull(kCamera, kFrustumPlanes, kMeshVec);
}
//-------------------------------------------------------------------------------------------------
float sdTerrain::GetRawHeight(uint uiX, uint uiY)
{
	return m_pkHeightMap->GetRawHeight(uiX, uiY);
}
//-------------------------------------------------------------------------------------------------
void sdTerrain::SetRawHeight(uint uiX, uint uiY, float fHeight)
{
	m_pkHeightMap->SetRawHeight(uiX, uiY, fHeight);
}
//-------------------------------------------------------------------------------------------------
float sdTerrain::GetHeight(float fX, float fY)
{
	// 转换到HeightMap坐标系(不进行参数检查)
	float fTotalScale = m_fScale * m_fMeterPerGrid;
	float fHX = (fX - m_kOrigin.x) / fTotalScale;
	float fHY = (fY - m_kOrigin.y) / fTotalScale;

	return m_pkHeightMap->GetHeight(fHX, fHY) * fTotalScale + m_kOrigin.z;
}
//-------------------------------------------------------------------------------------------------
bool sdTerrain::Pick(const sdRay& kRay, sdVector3& kIntersect, float fLimit)
{
	return m_pkPick->Pick(kRay, kIntersect);
}
//-------------------------------------------------------------------------------------------------