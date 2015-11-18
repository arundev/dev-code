//*************************************************************************************************
// 内容:	物理系统
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-17
// 最后修改:
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
// PhysX物理系统管理器
//	1.地形部分实时装载和生成
//-----------------------------------------------------------------------------
class sdPhysicsSystem : public Base::sdTSingleton<sdPhysicsSystem> 
{
public:
	sdPhysicsSystem();
	~sdPhysicsSystem();

	// 系统操作
	bool	Initialize();
	void	Destroy();
	void	Update();

	// 物理场景操作
	//bool	CreateScene(uint uiSize);
	bool	LoadScene(sdMap* pkMap);
	//bool	SaveScene();
	void	DestroyScene();

	// 射线查询接口


protected:
	bool	CreateEmptyScene(uint uiSize);
	bool	CreateTerrain(sdTerrain* pkTerrain);
	bool	CreateTerrainBound(sdTerrain* pkTerrain);
	


protected:
	bool	m_bInitialized;
	bool	m_bSceneLoaded;

	NxPhysicsSDK*			m_pkPhysicsSDK;			///< SDK对象
	NxUserAllocator*		m_pkAllocator;			///< 定制类堆内存分配器
	NxUserOutputStream*		m_pkOutputStream;		///< 定制调试输出		
	NxUserTriggerReport*	m_pkUserTriggerReport;
	NxUserControllerHitReport*	m_pkUserControllerHitReport;

	NxScene* m_pkScene;								///< 当前物理场景
	NxHeightField* m_pkHeightField;					///< 当前高度图(每个Scene可以有多个)
	NxControllerManager* m_pkControllerManager;		///< 当前控制管理器

	sdPhysXCookUtility m_kCookingUtility;			///< Cook工具
};
//-----------------------------------------------------------------------------
ENGINE_NAMESPACE_END_PHYSICSYSTEM
#endif