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
	// ����һ:����Error��LODת��ϵ��(���ǲο������������)
	// @{
	float fScreenWidth	= 1024.f;						///< Ĭ�Ϸֱ���
	float fScreenHeight = 768.f;						///< Ĭ�Ϸֱ���
	float fFOV			= sdMath::Degree2Radian(60.f);	///< Ĭ��FOV
	float fMaxLodError	= 100.f;						///< WZΪʲô��100(ò���Ǹ�����ֵ)
	m_fError2Distance = fScreenHeight / (fabsf(tanf(fFOV / 2.f)) * 2.f * fMaxLodError);
	// @}

	// ������:�����������Ļ�ֱ��ʼ���LOD����ת��ϵ��
	//	1.����Զ�ü��浥λ���ض�Ӧ�����糤��
	//	2.����ת��ϵ��
	//	3.����ת��ϵ��
	// @{
	//uint uiHeight = 768;								///< Ĭ�Ϸֱ���
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

	// ���߶�ͼ�ߴ�,�����ͼ�ߴ�
	//
	//
	m_uiTerrainSize		= uiTerrainSize;
	m_uiBlendTexSize	= uiBlendTexSize;
	m_uiTileMapSize		= uiBlendTexSize / m_uiTexTileSize;
	m_fMeterPerPixel	= uiTerrainSize / (float)uiBlendTexSize;
	
	// ��ʼ����̬��Ա
	sdQuadMesh::StaticInitialize(GetTileSize());

	// �����߶�ͼ
	m_pkHeightMap = NiNew sdHeightMap(uiTerrainSize);
	NIASSERT(m_pkHeightMap);

	// ����ʰȡ
	m_pkPick = NiNew sdTerrainPick(m_pkHeightMap);
	NIASSERT(m_pkPick);

	// ����������ͼ
	m_pkNormalMap = NiNew sdNormalMap(uiTerrainSize, m_pkHeightMap);
	NIASSERT(m_pkNormalMap);
	m_pkNormalMap->Update();

	if (m_bEnableEditMaterial)
	{
		// ���������ͼ
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

		// ���������ͼ��Ƭ��ͼ(���ڱ���ÿ����Ƭ�ڵ��Ĳ�����ͼ��Ϣ)
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

		// ����ƴ����
		// @{
		m_pkDiffuseAtlas = NiNew sdTextureAtlas(D3DFMT_DXT5);
		NIASSERT(m_pkDiffuseAtlas);

		m_pkNormalAtlas = NiNew sdTextureAtlas(D3DFMT_DXT5);
		NIASSERT(m_pkNormalAtlas);
		// @}

		//
		// ��ʼ��Tile��Ϲ�����������Ϊ4x4��Tile��ÿ��������6x6��һ�����������,
		// Ԥ����Ĳ�����Ͳ���Ȩ�ء�
		//	1.uiIdx����ÿ���������ڲ��������ϵĲ�������(һά)
		//	2.fWeight����ÿ��������Ĳ���Ȩ��
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

	// �����Ĳ���(���ݸ߶�ͼ)
	m_pkQuadRoot = NiNew sdQuadNode(this);
	m_pkQuadRoot->Build(sdVector4ui(0, 0, uiTerrainSize, uiTerrainSize), NULL);

	// ��ʼ��������Ⱦ��Ϣ
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

	// ����
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

	// ������Ⱦ����
	m_kRenderParams.Reset();

	// ��ȡ�����ļ�
	sdTerrainStream kTerrainStream(szSceneFullPath);
	if (!kTerrainStream.LoadConfig(this))
	{
		NIASSERT(0);
		return false;
	}

	m_uiTileMapSize = m_uiBlendTexSize / m_uiTexTileSize;
	m_fMeterPerPixel = m_uiTerrainSize / (float)m_uiBlendTexSize;

	// ��ʼ����̬��Ա
	sdQuadMesh::StaticInitialize(GetTileSize());

	// �����߶�ͼ
	m_pkHeightMap = kTerrainStream.LoadHeightMapConfig();
	NIASSERT(m_pkHeightMap);

	kTerrainStream.LoadHeightMap(m_pkHeightMap);

	// ����ʰȡ
	m_pkPick = NiNew sdTerrainPick(m_pkHeightMap);
	NIASSERT(m_pkPick);

	// ����������ͼ
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

	// ������ͼ
	if (m_bEnableEditMaterial)
	{
		// ���������ͼ
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

		// ���������ͼ��Ƭ��ͼ(���ڱ���ÿ����Ƭ�ڵ��Ĳ�����ͼ��Ϣ)
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

		// ����ƴ����
		// @{
		m_pkDiffuseAtlas = NiNew sdTextureAtlas(D3DFMT_DXT5);
		NIASSERT(m_pkDiffuseAtlas);

		m_pkNormalAtlas = NiNew sdTextureAtlas(D3DFMT_DXT5);
		NIASSERT(m_pkNormalAtlas);
		// @}

		//
		// ��ʼ��Tile��Ϲ�����������Ϊ4x4��Tile��ÿ��������6x6��һ�����������,
		// Ԥ����Ĳ�����Ͳ���Ȩ�ء�
		//	1.uiIdx����ÿ���������ڲ��������ϵĲ�������(һά)
		//	2.fWeight����ÿ��������Ĳ���Ȩ��
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

		// ����ͼ����Ȩ��
		kTerrainStream.LoadLayerMapConfig(this);
		kTerrainStream.LoadLayerMap(this);
	}
	else
	{
		// ���ػ����ͼ
		m_spBlendMap = kTerrainStream.LoadBlendMap();
		NIASSERT(m_spBlendMap);

		// ���ػ����ͼ��Ƭ��ͼ
		m_spTileMap = kTerrainStream.LoadTileMap();
		NIASSERT(m_spTileMap);

//		//
//		// ����һ��ֱ������ϳɺõĵ�ͼ
//		// �����������ݱ������Ϣ��̬����
//		//
//		// ������ͼ�����ò���
//		kTerrainStream.LoadAtlasConfig();
//
//		// ���صر���ͼ
//		m_spDiffuseAtlasMap = kTerrainStream.LoadDiffuseAtlasMap();
//		m_spDiffuseAtlasTable = kTerrainStream.LoadDiffuseAtlasTableMap();
//
//		m_spNormalAtlasMap = kTerrainStream.LoadNormalAtlasMap();
//		m_spNormalAtlasTable = kTerrainStream.LoadNormalAtlasTableMap();
	}

	// �����Ĳ���(���ݸ߶�ͼ)
	m_pkQuadRoot = NiNew sdQuadNode(this);
	m_pkQuadRoot->Build(sdVector4ui(0, 0, m_uiTerrainSize, m_uiTerrainSize), NULL);

	// ��ʼ��������Ⱦ��Ϣ
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
	// ����GeometryMesh
	m_pkQuadRoot->UpdateGeometry(fCenterX, fCenterY, fRadius);

	// ����NormalMap
	// @{
	// ���㷶Χ(��������)
	float fStartX	= fCenterX - fRadius;
	float fStartY	= fCenterY - fRadius;
	float fEndX	= fCenterX + fRadius;
	float fEndY = fCenterY + fRadius;

	// ���㷶Χ(�߶�ͼ/����ͼ����)
	float fTotalScale = m_fScale * m_fMeterPerGrid;
	float fNStartX = (fStartX - m_kOrigin.x) / fTotalScale;
	float fNStartY = (fStartY - m_kOrigin.y) / fTotalScale;
	float fNEndX = (fEndX - m_kOrigin.x) / fTotalScale;
	float fNEndY = (fEndY - m_kOrigin.y) / fTotalScale;

	// ǯλ����Χ������
	if (fNStartX < 0)	fNStartX = 0.0f;
	if (fNStartY < 0)	fNStartY = 0.0f;
	if (fNEndX > (float)m_uiTerrainSize)	fNEndX = (float)m_uiTerrainSize;
	if (fNEndY > (float)m_uiTerrainSize)	fNEndY = (float)m_uiTerrainSize;

	uint uiNStartX = (uint)fNStartX;
	uint uiNStartY = (uint)fNStartY;
	uint uiNEndX = (uint)(fNEndX);
	uint uiNEndY = (uint)(fNEndY);

	// ���·���ͼ
	m_pkNormalMap->Update(uiNStartX, uiNStartY, uiNEndX - uiNStartX, uiNEndY - uiNStartY);
	// @}

	// 
	m_kRenderParams.minHeight = m_pkHeightMap->GetMinHeight();
	m_kRenderParams.maxHeight = m_pkHeightMap->GetMaxHeight();
}
//-------------------------------------------------------------------------------------------------
void sdTerrain::Cull(const NiCamera& kCamera, std::vector<NiMesh*>& kMeshVec)
{
	// ����ü�ƽ��
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
	// ת����HeightMap����ϵ(�����в������)
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