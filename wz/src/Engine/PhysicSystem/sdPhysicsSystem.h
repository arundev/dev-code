//*************************************************************************************************
// ����:	����ϵͳ
//---------------------------------------------------------
// ����:		
// ����:		2012-08-17
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_PHYSICS_SYSTEM_H__
#define _SD_ENGINE_PHYSICS_SYSTEM_H__
#include <sdSingleton.h>
#include <NxUserContactReport.h>

#include "sdPhysXCookUtility.h"

class sdMap;
class sdTerrain;

ENGINE_NAMESPACE_BEGIN_PHYSICSYSTEM
//-----------------------------------------------------------------------------
class sdPhysXTriggerReport : public NxUserTriggerReport, public NiMemObject
{
public:
	virtual void onTrigger(NxShape& triggerShape, NxShape& otherShape, NxTriggerFlag status);
};
//-----------------------------------------------------------------------------
class sdPhysXControllerHitReport : public NxUserControllerHitReport, public NiMemObject
{
public:
	virtual NxControllerAction onShapeHit(const NxControllerShapeHit& hit);
	virtual NxControllerAction onControllerHit(const NxControllersHit& hit);
};
//-----------------------------------------------------------------------------
// PhysX����ϵͳ������
//	1.���β���ʵʱװ�غ�����
//-----------------------------------------------------------------------------
class sdPhysicsSystem : public Base::sdTSingleton<sdPhysicsSystem> 
{
public:
	sdPhysicsSystem();
	~sdPhysicsSystem();

	// ϵͳ����
	bool	Initialize();
	void	Destroy();
	void	Update();

	// ����������
	//bool	CreateScene(uint uiSize);
	bool	LoadScene(sdMap* pkMap);
	//bool	SaveScene();
	void	DestroyScene();

	// ���߲�ѯ�ӿ�


protected:
	bool	CreateEmptyScene(uint uiSize);
	bool	CreateTerrain(sdTerrain* pkTerrain);
	bool	CreateTerrainBound(sdTerrain* pkTerrain);
	


protected:
	bool	m_bInitialized;
	bool	m_bSceneLoaded;

	NxPhysicsSDK*			m_pkPhysicsSDK;			///< SDK����
	NxUserAllocator*		m_pkAllocator;			///< ��������ڴ������
	NxUserOutputStream*		m_pkOutputStream;		///< ���Ƶ������		
	NxUserTriggerReport*	m_pkUserTriggerReport;
	NxUserControllerHitReport*	m_pkUserControllerHitReport;

	NxScene* m_pkScene;								///< ��ǰ������
	NxHeightField* m_pkHeightField;					///< ��ǰ�߶�ͼ(ÿ��Scene�����ж��)
	NxControllerManager* m_pkControllerManager;		///< ��ǰ���ƹ�����

	sdPhysXCookUtility m_kCookingUtility;			///< Cook����
};
//-----------------------------------------------------------------------------
ENGINE_NAMESPACE_END_PHYSICSYSTEM
#endif