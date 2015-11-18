#include "sdResourceSystemPCH.h"
#include "sdResourcePool.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RESOURCESYSTEM
//-------------------------------------------------------------------------------------------------
sdResourcePool::sdResourcePool()
{
	m_kCloingProcess.m_eCopyType = NiObjectNET::COPY_EXACT;
	m_kCloingProcess.m_eAffectedNodeRelationBehavior = NiCloningProcess::CLONE_RELATION_ALL;
}
//-------------------------------------------------------------------------------------------------
sdResourcePool::~sdResourcePool()
{
	
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE