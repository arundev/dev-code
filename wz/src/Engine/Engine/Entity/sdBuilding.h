//*************************************************************************************************
// ����:	����,����,�ŵȾ�̬���
//---------------------------------------------------------
// ����:		
// ����:		2012-12-06
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_SCENE_BUILDING_H__
#define _SD_ENGINE_SCENE_BUILDING_H__
#include "sdEntity.h"
#include "IResource.h"
#include "IResourceSystem.h"

//-------------------------------------------------------------------------------------------------
// ��ͨ���
//	1.��̬����
//	2.������Ҷ(��Ҫ��͸������)
//-------------------------------------------------------------------------------------------------
class sdBuilding : public sdEntity, public Engine::ResourceSystem::IResourceEventListener
{
	NiDeclareRTTI;
	NiDeclareStream;

	DECLARE_PROPFUNC_MAP()
	DECLARE_DYNAMIC(sdBuilding)

public:
	sdBuilding();
	virtual ~sdBuilding();

	// �麯���̳�(�̳���sdEntity)
	virtual void Cull(const NiCamera& kCamera, Engine::RenderSystem::IRenderPath& kRenderPath);
	virtual void UpdateGame();

	// ���Է���
	// @{
	DEFINE_GAMEOBJECT_PROP(NiFixedString, NIF, NIFFile)
	inline NiFixedString	GetNIFFile() const;
	inline void				SetNIFFile(const NiFixedString&);

	// @}

	// NIF��Դ������ж�ؽӿ�(�ṩ��SceneLoadManager����)
	// @{
	void UpdateLoad();
	void UpdateUnload();
	void UpdateDelayUnload();

	// ͬ������NIF�ļ�(���ڱ༭��)
	void SyncLoadNIF();
	// @}

	// 
//	void LoadLightMap();
//	void LoadEnvironmentMap();
//	void LoadDetailNormalMap();

protected:
	// �麯���̳�(����IResourceEventListener)
	virtual void OnLoaded(Engine::ResourceSystem::IResource* pkResource); 

	// ��Դ������ж�ع��ܺ���
	// @{
	// ����Geometry��LOD

	// ����Texture��LOD

	// �첽����NIF�ļ�
	void AsyncLoadNIF();

	// �ҽ��뷴�ҽ�ͼ����Դ
	void AttachGraphic(NiNode* pkNode);
	void DetachGraphic();
	void GraphicLoadOver();

	void ProcessShimmer(NiNode* pkNode);	///< ͼ���ļ�����ʱ,Ԥ����Shimmer��Ϣ
	void ProcessTreeLeaf(NiNode* pkNode);	///< ͼ���ļ�����ʱ,Ԥ����TreeLeaf��Ϣ
	void ProcessLight(NiNode* pkNode);		///< ͼ���ļ�����ʱ,Ԥ����Light��Ϣ
	

	// ���¼������ȼ�(Ŀǰ�Ե�����ľ�����ΪȨ��)
	bool UpdatePrior(Engine::ResourceSystem::EResReqPrior ePrior, float fSqlDistance);
	// @}


protected:
	NiNodePtr		m_spNIFRoot;	///< NIF�ļ�����
	NiFixedString	m_kNIFPath;		///< NIF�ļ�·��

	// ��Դ�����ĵ��Դ
	typedef std::vector<NiPointLight*> PointLightVec;
	typedef std::vector<NiPointLight*>::iterator PointLightVecItr;
	PointLightVec m_kLights;

	// �������״̬
	ELoadingState m_eLightMapLoadingState;
	ELoadingState m_eDetaiNormalMapLoadingState;
	ELoadingState m_eEnvironmentMapLoadingState;
	
	// ���߳���Դ����
	Engine::ResourceSystem::IResourcePtr			m_pkResource;
	Engine::ResourceSystem::IResource::ResTicket	m_uiResourceTick;
};
NiSmartPointer(sdBuilding);
//-------------------------------------------------------------------------------------------------
#include "sdBuilding.inl"
#endif