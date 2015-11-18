#include "sdPhysicsSystemPCH.h"
#include "sdPhysXCookUtility.h"

ENGINE_NAMESPACE_BEGIN_PHYSICSYSTEM
//-------------------------------------------------------------------------------------------------
NxCookingInterface* sdPhysXCookUtility::ms_pkCooking = NULL;
//-------------------------------------------------------------------------------------------------
bool sdPhysXCookUtility::InitCooking(NxUserAllocator* pkAllocator, NxUserOutputStream* pkOutputStream)
{
#ifdef COOKING_INTERFACE
	if (!ms_pkCooking)
		ms_pkCooking = NxGetCookingLib(NX_PHYSICS_SDK_VERSION);

	if (!ms_pkCooking)
		return false;

	ms_pkCooking->NxInitCooking(pkAllocator, pkOutputStream);
	return true;
#else
	return NxInitCooking(pkAllocator, pkOutputStream);
#endif
}
//-------------------------------------------------------------------------------------------------
void sdPhysXCookUtility::CloseCooking()
{
#ifdef COOKING_INTERFACE
	if (ms_pkCooking)
		ms_pkCooking->NxCloseCooking();
#else
	return NxCloseCooking();
#endif
}
//-------------------------------------------------------------------------------------------------
bool sdPhysXCookUtility::CreatePMap(NxPMap& kPMap, const NxTriangleMesh& kTriMesh, NxU32 nxDensity, NxUserOutputStream* pkOutputStream)
{
#ifdef COOKING_INTERFACE
	return ms_pkCooking->NxCreatePMap(kPMap, kTriMesh, nxDensity, pkOutputStream);
#else
	return NxCreatePMap(kPMap, kTriMesh, nxDensity, pkOutputStream);
#endif
}
//-------------------------------------------------------------------------------------------------
bool sdPhysXCookUtility::ReleasePMap(NxPMap& kPMap)
{
#ifdef COOKING_INTERFACE
	return ms_pkCooking->NxReleasePMap(kPMap);
#else
	return NxReleasePMap(kPMap, kTriMesh, nxDensity, pkOutputStream);
#endif
}
//-------------------------------------------------------------------------------------------------
bool sdPhysXCookUtility::CookConvexMesh(const NxConvexMeshDesc& kDesc, NxStream& kStream)
{
#ifdef COOKING_INTERFACE
	return ms_pkCooking->NxCookConvexMesh(kDesc, kStream);
#else
	return NxCookConvexMesh(kDesc, kStream);
#endif
}
//-------------------------------------------------------------------------------------------------
bool sdPhysXCookUtility::CookClothMesh(const NxClothMeshDesc& kDesc, NxStream& kStream)
{
#ifdef COOKING_INTERFACE
	return ms_pkCooking->NxCookClothMesh(kDesc, kStream);
#else
	return NxCookClothMesh(kDesc, kStream);
#endif
}
//-------------------------------------------------------------------------------------------------
bool sdPhysXCookUtility::CookTriangleMesh(const NxTriangleMeshDesc& kDesc, NxStream& kStream)
{
#ifdef COOKING_INTERFACE
	return ms_pkCooking->NxCookTriangleMesh(kDesc, kStream);
#else
	return NxCookTriangleMesh(kDesc, kStream);
#endif
}
//-------------------------------------------------------------------------------------------------
bool sdPhysXCookUtility::CookSoftBodyMesh(const NxSoftBodyMeshDesc& kDesc, NxStream& kStream)
{
#ifdef COOKING_INTERFACE
	return ms_pkCooking->NxCookSoftBodyMesh(kDesc, kStream);
#else
	return NxCookSoftBodyMesh(kDesc, kStream);
#endif
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_PHYSICSYSTEM