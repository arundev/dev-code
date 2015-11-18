#include "sdResourceSystemPCH.h"
#include "sdNifResource.h"
#include "sdNifResourcePool.h"
#include "sdResourceSystem.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RESOURCESYSTEM
//-------------------------------------------------------------------------------------------------
sdNifResource::sdNifResource(const std::string& kFileName, bool bReleaseOnGC, EResUsageType eUsageType)
: sdResource(kFileName, bReleaseOnGC, eUsageType)
{

}
//-------------------------------------------------------------------------------------------------
sdNifResource::sdNifResource(NiNode* spNode)
: sdResource(std::string(), true, E_RES_USAGE_NIF_COMMON)
, m_spRootNode(spNode)
{
	
}
//-------------------------------------------------------------------------------------------------
sdNifResource::~sdNifResource()
{

}
//-------------------------------------------------------------------------------------------------
bool sdNifResource::LoadResource(bool bMainThreadLoad /* = false */)
{
	//__VLD_THREAD_TRACE_AUTO2(sdNifResourcePool);

	m_eStatus = E_RES_STATUS_LOADED;
	
	sdResourceSystem* pkResourceSystem = (sdResourceSystem*)sdResourceSystem::GetResourceSystem();
	NIASSERT(pkResourceSystem);

	sdNifResourcePool* pkNifResourcePool = pkResourceSystem->GetNifResourcePool();
	NIASSERT(pkNifResourcePool);
	
	m_spRootNode = pkNifResourcePool->LoadNifFile(m_kFileName, m_eUsage, bMainThreadLoad);
	if (!m_spRootNode)
		return false;

	return true;
}
//-------------------------------------------------------------------------------------------------
void sdNifResource::FreeResource(bool bNotify /* = true */)
{
	if (m_spRootNode)
	{
		m_spRootNode = 0;

		sdResourceSystem* pkResourceSystem = (sdResourceSystem*)sdResourceSystem::GetResourceSystem();
		NIASSERT(pkResourceSystem);

		sdNifResourcePool* pkNifResourcePool = pkResourceSystem->GetNifResourcePool();
		NIASSERT(pkNifResourcePool);
		pkNifResourcePool->FreeNifFile(m_kFileName);
	}
}
//-------------------------------------------------------------------------------------------------
void sdNifResource::CacheResource()
{

}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE