//*************************************************************************************************
// ����:	PhysX��Cook���ܼ򵥷�װ
//---------------------------------------------------------
// ����:		
// ����:		2013-07-08
// ����޸�:
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
	// ��ʼ��Cooking�ӿ�
	//	@param[in]	pkAllocator
	//	@param[in]	pkOutputStream
	static bool InitCooking(NxUserAllocator* pkAllocator, NxUserOutputStream* pkOutputStream);

	// �ر�Cooking�ӿ�
	static void CloseCooking();

	// ����PhysX Map
	//	@param[out]	kpMap	
	//	@param[in]	kMesh	����������
	//	@param[in]	nxDensity
	//	@param[in]	pkOutputStream
	static bool CreatePMap(NxPMap& kPMap, const NxTriangleMesh& kTriMesh, NxU32 nxDensity, NxUserOutputStream* pkOutputStream);

	// �ͷ�PhysX Map
	static bool ReleasePMap(NxPMap& kPMap);

	// ����Cooking����
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