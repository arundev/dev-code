//*************************************************************************************************
// ����:	����ϵͳ
//---------------------------------------------------------
// ����:		
// ����:		2012-07-31
// ����޸�:	2013-05-02
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

// ��Ⱦϵͳ
#include "IRenderSystem.h"
#include "sdDynamicTexture.h"
#include "sdTextureAtlas.h"

//-------------------------------------------------------------------------------------------------
// ����ϵͳ,���ж���ӿھ��ɴ��౩¶��ȥ.
//	1.��������������ͬһ��Ϊ����:����,�ü�,��Ⱦ
//	2.���η�Ϊ���β��ֺ������ֱַ���
//
// ���β���:
//	1.Tile����: �Ĳ�������,���м�ڵ㵽Ҷ�ӽڵ��Ԥ����Mesh,Node�ڲ�û��LOD
//			(���Կ������û���,��̬����Mesh���Ľ�Ч��)
//  2.Tile��Ե�ӷ�:����ȹ�ߵķ�ʽ���LOD��Ե�ӷ�,���������ǽڵ㵽��һ��QuadNode��������
//  3.Tile�ߴ�: 64*64��, 65*65��, ����ȹ��67*67�� 
//  3.Tile��Ⱦʱ���ж���ѹ��,�Լ������ݴ���
//			XY��������UINT8
//			Z�任���������UINT8
//  4.Tile��Ⱦʱʹ��TriangleStrip�����ж���,ʹ��Index, 
//		����GPU���㻺��Ĭ�ϴ�СΪ24���Ż�����,������Cache��С�ֳ�����
//		
// ������:
//	1.ʹ��Texture Atlas������������Texture Pack����
//
// \TODO ��������Atlas��Ϣ,����Ϸ��ʵʱ����DiffuseAtlas��NormalAtlas
// \TODO ���ڴ���ʹ��DXT1��ʽ����С�Դ�ʹ����
// \TODO ����Ϸ�ͱ༭��ʹ�ò�ͬ��Atlas��ʽ
// \TODO ����Ϸ��ʹ��Cache��Ԥ��������
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

	// ���βü�(��������)
	//(wz��װ��һ��TerrainTileEntity,ò��ûɶ��Ҫ,����ֱ�ӷ���Mesh)
	void	Cull(const NiCamera& kCamera, std::vector<NiMesh*>& kMeshVec);

	// ����ʰȡ(��������)
	bool	Pick(const Base::Math::sdRay& kRay, Base::Math::sdVector3& kIntersect, float fLimit = FLT_MAX);	

	// ���θ߶Ȼ�ȡ(��������)
	float	GetHeight(float fX, float fY);

	// �߶ȱ༭(���������ڵ��α༭��)
	// @{
	// ���θ߶Ȼ�ȡ������(�߶�ͼ����,ֱ�ӴӸ߶�ͼȡ��������,û�н��б任)
	float	GetRawHeight(uint uiX, uint uiY);
	void	SetRawHeight(uint uiX, uint uiY, float fHeight);

	// ���¼�����(��������)
	void	UpdateGeometry(float fCenterX, float fCenterY, float fRadius);
	// @}

	// ͼ��༭(���������ڵ��α༭��)
	// @{
	uint	GetLayerNum() const { return m_kLayerMapVec.size();}
	uint	GetLayerIndex(const char* szName) const;	///< ����ʱ����-1(0xfffffffe)

	sdLayerMap*	GetLayerMap(uint uiIndex) const;
	sdLayerMap* GetLayerMap(const char* szName) const;

	bool	AppendLayerMap(sdLayerMap* pkLayerMap);
	bool	InsertLayerMap(sdLayerMap* pkLayerMap, uint uiIndex);

	bool	RemoveLayerMap();
	bool	RemoveLayerMap(uint uiIndex);

	bool	SwapLayerMap(uint uiLIndex, uint uiRIndex);

	// ͼ�����ص�Ȩֵ�Ļ�ȡ������
	uchar	GetBlendMapData(uint uiX, uint uiY, uint uiLayer);
	void	SetBlendMapData(uint uiX, uint uiY, uint uiLayer, uchar uiWeight);

	// ���»����ͼ(��������)
	//	@param[in]	fCenterX,fCenterY,rRadius	Բ��λ����Բ�뾶(��������)
	void	UpdateBlendMap(float fCenterX, float fCenterY, float fRadius);

	// ����ͼ�������´��
	void	RepackMaps();
	// @}

	// �������Է���
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

	// ���Բ���(���ڱ༭��)
	// @{
	bool	GetShowTileGrid() const { return m_kRenderParams.showTileGrid;}
	void	SetShowTileGrid(bool bShow) { m_kRenderParams.showTileGrid = bShow;}

	bool	GetShowCurrentLayer() const { return m_kRenderParams.showCurrentLayer;}
	void	SetShowCurrentLayer(bool bShow) { m_kRenderParams.showCurrentLayer = bShow;}
	void	SetCurrentLayerIndex(uint uiIndex){ m_kRenderParams.currentLauerIndex = uiIndex;}

	bool	GetShowInversibleLayers() const { return m_kRenderParams.showInvisiableLayers;}
	void	SetShowInversibleLayers(bool bShow) { m_kRenderParams.showInvisiableLayers = bShow;}
	// @}

	// ��ȡ�߶�ͼ(�ڲ�ʹ��,�༭��ʹ��)
	sdHeightMap*	GetHeightMap() { return m_pkHeightMap;};

protected:
	//
	// ��ȡָ�����ص�ĸ���ͼ��Ȩ���б�wzԭʼ�汾��ȡ��ʽ��
	//
	//	@param[in]	uiX,uiY		DetailMap����������λ��
	//	@param[out]	pucWeights	��Ӧ��������λ�õ�����ͼ���Ȩ��ֵ
	//	return					��Ч��ͼ����
	//
	uint	GetWeights(uint uiX, uint uiY, uchar* pucWeights);

protected:
	bool	m_bInitialized;			// �Ƿ��ʼ��
	bool	m_bIsVisible;			// �Ƿ�ɼ�(һ�����ڱ༭��)	
	bool	m_bEnableEditHeight;	// �Ƿ�����༭���θ߶�
	bool	m_bEnableEditMaterial;	// �Ƿ�����༭���β���

	// ��������ƫ��������
	NiPoint3	m_kOrigin;	// ��������ƫ��(Ĭ��Ϊ(0,0,0))
	float		m_fScale;	// ������������(Ĭ��1.0f)

	// �ر�߶�ͼ
	sdHeightMapPtr	m_pkHeightMap;	

	// �߶�ʰȡ
	sdTerrainPickPtr m_pkPick;

	// �ر���ͼ
	sdNormalMapPtr	m_pkNormalMap;

	// �ر�ͼ�㼯�Ϻ�ͼ������Ϣ����
	typedef std::vector<sdLayerMapPtr> LayerMapVec;
	typedef std::vector<sdLayerMapPtr>::iterator LayerMapVecItr;
	typedef std::vector<sdLayerMapPtr>::const_iterator LayerMapVecConItr;
	LayerMapVec		m_kLayerMapVec;								///< �����ڱ༭��				

	typedef std::vector<uchar*> LayerAlphaVec;
	typedef std::vector<uchar*>::iterator LayerAlphaVecItr;
	LayerAlphaVec	m_kLayerAlphaVec;							///< �����ڱ༭��

	// �ر�����ͼ����ұ�
	Engine::sdDynamicTexturePtr	m_pkDynamicBlendMap;			///< �����ڱ༭��
	NiTexturePtr	m_spBlendMap;

	Engine::sdDynamicTexturePtr	m_pkDynamicTileMap;				///< �����ڱ༭��
	NiTexturePtr	m_spTileMap;	

	// �ر���������ͼ���Ͳ��ұ�
	Engine::sdTextureAtlasPtr m_pkDiffuseAtlas;					///< �����ڱ༭��
	NiTexturePtr	m_spDiffuseAtlasMap;
	NiTexturePtr	m_spDiffuseAtlasTable;

	// �ر�����ͼ���Ͳ��ұ�
	Engine::sdTextureAtlasPtr m_pkNormalAtlas;					///< �����ڱ༭��
	NiTexturePtr	m_spNormalAtlasMap;
	NiTexturePtr	m_spNormalAtlasTable;

	// �����Ĳ���
	sdQuadNodePtr	m_pkQuadRoot;

	// ���β���
	//NiColorA m_kAmbient;
	//NiColorA m_kDiffuse;
	//NiColorA m_kSpecular;
	//float m_fShiness;

	// ���λ��Ʋ���(�����ṩ����Ⱦϵͳʹ��,�ڲ���Ӧ�ñ������ڴ�ͱ�����Դ)
	Engine::RenderSystem::sdTerrainParams m_kRenderParams;

	uint	m_uiTerrainSize;	// Terrain�ĳߴ�(256/512/1024/2048,��λGrid)
	float	m_fMeterPerGrid;	// ÿ��λ��С(0.125/0.25/0.5/1/2/4/8,Ŀǰ��1m)
	uint	m_uiTileSize;		// Terrain��Tile�ĳߴ�(Ŀǰ��64Unit)

	uint	m_uiBlendTexSize;	// BlendMap�ߴ�(256/512/1024/2048,��λPixel)
	float	m_fMeterPerPixel;	// ÿ���ش�С(0.125/0.25/0.5/1/2/4/8,Ŀǰ��0.5f)
	uint	m_uiTexTileSize;	// BlendMap��TexTile�ߴ�(Ŀǰ��4Pixel)
	uint	m_uiTileMapSize;	// TileMap�ĳߴ�

	uint	m_uiMeshLevel;		// �Ĳ�����,ֻ��С��һ���㼶�ĲŻṹ��Mesh
	bool	m_bEnableLOD;		// �Ĳ����Ƿ�����LOD��ʾ
	bool	m_bEnableNormal;	// �����Ƿ���������ͼ

	// ���ݵ�ǰ��׶����ֱ��ʼ��������Error��Distance֮���ת����,���ڿ���LOD���л�
	// (�༶�л�,�ӳ��л�,һ����һ������,�����д��ο�CryEngine)
	float	m_fError2Distance;	

protected:
	// TexTile����Ȩ�ػ��ʱ�Ĺ�����(���ڱ༭��)
	// @{
	struct stFilterMap
	{
		uint	uiIdx[4];		///< 4��ȡ���������
		float	fWeight[4];		///< 4��ȡ�����Ȩ��
	};
	typedef std::vector<stFilterMap> FilterMapVec;;
	typedef std::vector<stFilterMap>::iterator FilterMapVecItr;
	FilterMapVec m_kTileFilterMapVec;
	// @}

protected:
	static uint	ms_uiMaxLayerCount;	///< ����������


};
//-------------------------------------------------------------------------------------------------
#endif