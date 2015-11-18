//*************************************************************************************************
// 内容:	物理场景加载器,提取出PhysX场景的加载功能
//---------------------------------------------------------
// 作者:		
// 创建:		2013-07-05
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_PHYSICS_SYSTEM_PHYSX_LOAD_MGR_H__
#define _SD_ENGINE_PHYSICS_SYSTEM_PHYSX_LOAD_MGR_H__

ENGINE_NAMESPACE_BEGIN_PHYSICSYSTEM
//-----------------------------------------------------------------------------
class sdPhysXLoadMgr : public NiRefObject
{
public:
	sdPhysXLoadMgr();
	~sdPhysXLoadMgr();

	//bool	LoadScene(sdMap* pkMap);
	//void	DestroyScene();

};
//-----------------------------------------------------------------------------
ENGINE_NAMESPACE_END_PHYSICSYSTEM
#endif