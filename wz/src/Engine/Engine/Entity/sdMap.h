//*************************************************************************************************
// ����:	������ͼ
//---------------------------------------------------------
// ����:		
// ����:		2012-06-29
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_MAP_H__
#define _SD_ENGINE_MAP_H__
#include "sdGameObject.h"
#include "sdTerrain.h"
#include "sdLight.h"

#include "sdFlare.h"
#include "sdSun.h"
#include "sdSkyBox.h"
#include "sdBGScroll.h"
#include "sdCloud.h"

#include "IRenderSystem.h"

#include "sdSceneLoadMgr.h"

// ����ͼ������
#define MAX_MAP_NAME_LEN 64

//-------------------------------------------------------------------------------------------------
// ������ͼ
//-------------------------------------------------------------------------------------------------
class sdMap : public sdGameObject
{
	///< ֻ����ʱ����
	NiDeclareRTTI;
	NiDeclareStream;

	///< �¶�̬ϵͳ
	DECLARE_PROPFUNC_MAP()
	DECLARE_DYNAMIC(sdMap)

public:
	sdMap();
	~sdMap();

	// @breif ��ͼ�ü�
	// @param[in] kCamera		�ü����
	// @param[in] kEntityVec	
	void Cull(const NiCamera& kCamera, EntityVec& kEntityVec);

	// @breif ��ͼ����
	void UpdateGame();

	// Entity����
	bool AddEntity(sdEntity* pkEntity);
	bool RemoveEntity(sdEntity* pkEntity);

	// ���Է���
	// @{
	// 
	sdTerrain*	GetTerrain() { return m_pkTerrain;};

	//
	NiNode*	GetDebugNode() const { return m_spDebugNode;}
//	NiNode* GetLightNode() const { return m_spLightNode;}
//	NiNode* GetDecalNode() const { return m_spDecalNode;}

	// �ƹ�
	sdLight* GetAmbientLight() const;
	sdLight* GetMainLight() const;
	sdLight* GetAssistantLight() const;

	// ����Ч��
	sdFlare*	GetFlare() const;
	sdSun*		GetSun() const;
	sdSkyBox*	GetSkyBox() const;
	sdBGScroll*	GetBGScroll() const;
	sdCloud*	GetCloudDome() const;

	// ��Ⱦ����
	Engine::RenderSystem::sdRenderParams& GetRenderParams() { return m_kRenderParams;};
	Engine::RenderSystem::sdEnvironmentParams& GetEnvironmentParams() { return m_kEnvironmentParams;};
	Engine::RenderSystem::sdPostProcessParams& GetPostProcessParams() { return m_kPostProcessParams;};
	// @}

	// �������Խӿ�
	// @{
	SD_INLINE sdGameObject* GetTerrainProp() const;
	SD_INLINE void SetTerrainProp(const sdGameObject*&);

//	SD_INLINE sdGameObject* GetSkyProp();
//	SD_INLINE void SetSkyProp(const sdGameObject*&);

	SD_INLINE uint GetLight(sdGameObject*& kValue, uint uiIndex) const;
	SD_INLINE uint SetLight(const sdGameObject*& kValue, uint uiIndex);
	// @}

protected:

protected:
	// ����ϵͳ
	sdTerrain*	m_pkTerrain;

	// �����ƹ�
	sdLightPtr	m_pkAmbientLight;
	sdLightPtr	m_pkMainLight;
	sdLightPtr	m_pkAssistantLight;

	// ����Ч��
	sdFlarePtr	m_pkFlare;
	sdSunPtr	m_pkSun;
	sdSkyBoxPtr	m_pkSkyBox;
	sdBGScrollPtr m_pkBGScroll;
	sdCloudPtr	m_pkCloudDome;

	// ����Ч��
//	sdRain;
//	sdSnow;
//	sdDynamicWheather;

	// ������ͨ����
	NiNodePtr	m_spDebugNode;		///< ���Զ�����ڵ�(sdDynamicNode)
	NiNodePtr	m_spLightNode;		///< �ƹ������ڵ�(sdDynamicNode)
	NiNodePtr	m_spDecalNode;		///< ����������ڵ�(sdDynamicNode)

	NiNodePtr	m_spBuildingNode;	///< ����������ڵ�(sdNode)

	//NiNodePtr	m_spTrigger;
	//NiNodePtr	m_spThing;
	//NiNodePtr	m_spWater;
	//NiNodePtr	m_spRoad;
	
	EntityVec m_kStaticObjectVec;	///< ��̬���
	EntityVec m_kActor;				///< ��̬��ɫ
	EntityVec m_kEffect;			///< ��Ч����
	EntityVec m_kArea;				///< ����
	EntityVec m_kSliceCollider;		///< �赲

	// ���ع�����(������������ж��)
	sdSceneLoadMgr m_kSceneLoadMgr;

	// �������Ʋ���(�����ṩ����Ⱦϵͳʹ��,�ڲ���Ӧ�ñ������ڴ�ͱ�����Դ)
	Engine::RenderSystem::sdRenderParams		m_kRenderParams;
	Engine::RenderSystem::sdEnvironmentParams	m_kEnvironmentParams;
	Engine::RenderSystem::sdPostProcessParams	m_kPostProcessParams;
};
NiSmartPointer(sdMap);
//-------------------------------------------------------------------------------------------------
#include "sdMap.inl"
#endif