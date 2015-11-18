#include "sdResourceSystemPCH.h"
#include "sdResource.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RESOURCESYSTEM
//-------------------------------------------------------------------------------------------------
sdResource::sdResource(const std::string& kFileName, bool bReleaseOnGC, EResUsageType eUsageType)
: m_kFileName(kFileName)
, m_bReleaseOnGC(bReleaseOnGC)
, m_eUsage(eUsageType)
, m_ePrior(E_RES_PRIOR_LOW)
, m_eStatus(E_RES_STATUS_WAITING)
{
	
}
//-------------------------------------------------------------------------------------------------
sdResource::~sdResource()
{

}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE