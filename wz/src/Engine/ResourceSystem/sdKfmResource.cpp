#include "sdResourceSystemPCH.h"
#include "sdKfmResource.h"
#include "sdNifResourcePool.h"
#include "sdKfmResourcePool.h"
#include "sdResourceSystem.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RESOURCESYSTEM
//-------------------------------------------------------------------------------------------------
sdKfmResource::sdKfmResource(const std::string& kFileName,	bool bReleaseOnGC /* = true */, 
	EResUsageType eUsageType /* = E_RES_USAGE_KFM_COMMON */, 
	const std::string& kChangeNifFileName /* = std::string() */,
	const std::string& kModelRootName /* = std::string() */)
: sdResource(kFileName, bReleaseOnGC, eUsageType)
, m_kModelRootName(kModelRootName)
, m_kChangeNifFileName(kChangeNifFileName)
{
	
}
//-------------------------------------------------------------------------------------------------
sdKfmResource::~sdKfmResource()
{
	if (m_spActorManager)
	{
		NIASSERT(0);
		::OutputDebugString("RsourceMamager - Memory Leak Detected");
	}
}
//-------------------------------------------------------------------------------------------------
bool sdKfmResource::LoadResource(bool bMainThreadLoad /* = false */)
{
	//__VLD_THREAD_TRACE_AUTO2(sdKfmResource);

	m_eStatus = E_RES_STATUS_LOADED;

	sdResourceSystem* pkResourceSystem = (sdResourceSystem*)sdResourceSystem::GetResourceSystem();
	NIASSERT(pkResourceSystem);

	sdKfmResourcePool* pkKfmResourcePool = pkResourceSystem->GetKfmResourcePool();
	NIASSERT(pkKfmResourcePool);

	m_spActorManager = pkKfmResourcePool->LoadKfmFile(m_kFileName, m_eUsage, m_bReleaseOnGC, bMainThreadLoad);
	if (!m_spActorManager)
		return false;
	
	if (!m_kChangeNifFileName.empty() && !m_kModelRootName.empty())
	{
		sdNifResourcePool* pkNifResourcePool = pkResourceSystem->GetNifResourcePool();
		NIASSERT(pkNifResourcePool);

		NiNodePtr spNewNifRoot = pkNifResourcePool->LoadNifFile(m_kChangeNifFileName, E_RES_USAGE_NIF_COMMON, bMainThreadLoad);
		if (spNewNifRoot)
		{
			if (m_spActorManager->GetNIFRoot())
			{
				// ¼ì²é
				NiAVObject* pkModelRoot = m_spActorManager->GetNIFRoot()->GetObjectByName(m_kModelRootName.c_str());
				if (pkModelRoot)
				{
					NiKFMTool* spKFMTool = m_spActorManager->GetKFMTool();
					if (spKFMTool)
						spKFMTool->SetModelRoot(m_kModelRootName.c_str());
				}
			}

			m_spActorManager->ChangeNIFRoot((NiNode*)spNewNifRoot);
		}
	}

	return true;
}
//-------------------------------------------------------------------------------------------------
void sdKfmResource::FreeResource(bool bNotify /* = true */)
{
	if (m_spActorManager)
	{
		sdResourceSystem* pkResourceSystem = (sdResourceSystem*)sdResourceSystem::GetResourceSystem();
		NIASSERT(pkResourceSystem);

		sdNifResourcePool* pkNifResourcePool = pkResourceSystem->GetNifResourcePool();
		NIASSERT(pkNifResourcePool);
		if (!m_kChangeNifFileName.empty() && !m_kModelRootName.empty())
			pkNifResourcePool->FreeNifFile(m_kChangeNifFileName);

		m_spActorManager = 0;

		sdKfmResourcePool* pkKfmResourcePool = pkResourceSystem->GetKfmResourcePool();
		NIASSERT(pkKfmResourcePool);
		pkKfmResourcePool->FreeKFMFile(m_kFileName);
	}
}
//-------------------------------------------------------------------------------------------------
void sdKfmResource::CacheResource()
{

}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE