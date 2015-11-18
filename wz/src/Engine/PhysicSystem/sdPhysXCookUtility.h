//*************************************************************************************************
// 内容:	PhysX的Cook功能简单封装
//---------------------------------------------------------
// 作者:		
// 创建:		2013-07-08
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_PHYSICS_SYSTEM_PHYSX_COOK_UTILITY_H__
#define _SD_ENGINE_PHYSICS_SYSTEM_PHYSX_COOK_UTILITY_H__
#include <sdSingleton.h>

// Only if on the Windows, Linux or PS3 platorm and 2.5.0 or higher, use the versioned Cooking
// interface via PhysXLoader
#if defined(WIN32) || defined(LINUX) || defined(__CELLOS_LV2__) || defined(__PPCGKKO__)
#if NX_SDK_VERSION_NUMBER >= 250
	#define COOKING_INTERFACE 1
#endif
#endif

ENGINE_NAMESPACE_BEGIN_PHYSICSYSTEM
//-----------------------------------------------------------------------------
class sdPhysXCookUtility : public Base::sdTSingleton<sdPhysXCookUtility> 
{
public:
	// 初始化Cooking接口
	//	@param[in]	pkAllocator
	//	@param[in]	pkOutputStream
	static bool InitCooking(NxUserAllocator* pkAllocator, NxUserOutputStream* pkOutputStream);

	// 关闭Cooking接口
	static void CloseCooking();

	// 创建PhysX Map
	//	@param[out]	kpMap	
	//	@param[in]	kMesh	输入三角形
	//	@param[in]	nxDensity
	//	@param[in]	pkOutputStream
	static bool CreatePMap(NxPMap& kPMap, const NxTriangleMesh& kTriMesh, NxU32 nxDensity, NxUserOutputStream* pkOutputStream);

	// 释放PhysX Map
	static bool ReleasePMap(NxPMap& kPMap);

	// 进行Cooking处理
	static bool CookConvexMesh(const NxConvexMeshDesc& kDesc, NxStream& kStream);
	static bool CookClothMesh(const NxClothMeshDesc& kDesc, NxStream& kStream);
	static bool CookTriangleMesh(const NxTriangleMeshDesc& kDesc, NxStream& kStream);
	static bool CookSoftBodyMesh(const NxSoftBodyMeshDesc& kDesc, NxStream& kStream);

protected:
#ifdef COOKING_INTERFACE
	static NxCookingInterface* ms_pkCooking;
#endif
};
//-----------------------------------------------------------------------------
ENGINE_NAMESPACE_END_PHYSICSYSTEM
#endif