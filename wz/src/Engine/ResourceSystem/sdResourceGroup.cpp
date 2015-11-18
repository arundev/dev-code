#include "sdResourceSystemPCH.h"
#include "sdResourceGroup.h"
#include "sdNifResourcePool.h"
#include "sdKfmResourcePool.h"
#include "sdResourceSystem.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RESOURCESYSTEM
//-------------------------------------------------------------------------------------------------
sdResourceGroup::sdResourceGroup(EResType eResType)
: m_eResType(eResType)
, m_uiKeyGenerator(1)
{

}
//-------------------------------------------------------------------------------------------------
sdResourceGroup::~sdResourceGroup()
{
	m_kNameToResTicketMap.clear();
	m_kResourceMap.clear();
}
//-------------------------------------------------------------------------------------------------
sdResource::ResTicket sdResourceGroup::AddResource(sdResource* pkResource, bool bGenerateKey)
{
	NIASSERT(pkResource);

	// 检查Ticket是否已经存在
	if (bGenerateKey)
	{
		const std::string&kFileName = pkResource->GetFileName();
		NameToResTicketMapItr itr = m_kNameToResTicketMap.find(kFileName);
		NameToResTicketMapItr itr_end = m_kNameToResTicketMap.end();
		if (itr != itr_end)
		{
			sdResource* pkResource = FindResource(itr->second);
			if (pkResource)
				return pkResource->GetTicket();
		}
	}

	// 加入资源表
	sdResource::ResTicket kResTicket = ((uint)m_eResType << 28) + m_uiKeyGenerator++;
	pkResource->SetTicket(kResTicket);
	m_kResourceMap[kResTicket] = pkResource;

	if (bGenerateKey)
	{
		const std::string&kFileName = pkResource->GetFileName();
		m_kNameToResTicketMap[kFileName] = kResTicket;
	}

	return kResTicket;
}
//-------------------------------------------------------------------------------------------------
sdResourcePtr sdResourceGroup::RemoveResource(sdResource::ResTicket kResTicket)
{
	sdResourcePtr pkResource = 0;
	ResourceMapItr itr = m_kResourceMap.find(kResTicket);
	ResourceMapItr itr_end = m_kResourceMap.end();
	if (itr != itr_end)
	{
		pkResource = itr->second;
		m_kResourceMap.erase(itr);
	}

	return pkResource;
}
//-------------------------------------------------------------------------------------------------
void sdResourceGroup::RemoveAllUnusedResource()
{
	ResourceMapItr itr_map = m_kResourceMap.begin();
	ResourceMapItr itr_map_end = m_kResourceMap.begin();
	for (; itr_map != itr_map_end; ++itr_map)
	{
		sdResourcePtr pkResource = itr_map->second;
		if (pkResource && pkResource->Cached() && pkResource->GetRefCount() == 2)
		{
			const std::string& kFileName = pkResource->GetFileName();
			NameToResTicketMapItr itr_name_tick = m_kNameToResTicketMap.find(kFileName);
			NameToResTicketMapItr itr_name_tick_end = m_kNameToResTicketMap.end();
			if (itr_name_tick != itr_name_tick_end)
				m_kNameToResTicketMap.erase(itr_name_tick);

			m_kResourceMap.erase(itr_map);
			pkResource = 0;
		}
	}
}
//-------------------------------------------------------------------------------------------------
void sdResourceGroup::GarbageCollect(ResourceList kResourceList)
{
	typedef std::set<std::string> StringSet;
	typedef std::set<std::string>::iterator StringSetItr;	
	StringSet kNoReleaseResNameMap;
	ResourceMap kNoReleaseResMap;
	ResourceMapItr itr_map = m_kResourceMap.begin();
	ResourceMapItr itr_map_end = m_kResourceMap.begin();
	for (; itr_map != itr_map_end; ++itr_map)
	{
		sdResourcePtr pkResource = itr_map->second;
		if (pkResource && pkResource->Cached() && pkResource->Freed())
		{
			const std::string& kFileName = pkResource->GetFileName();
			NameToResTicketMapItr itr_name_tick = m_kNameToResTicketMap.find(kFileName);
			NameToResTicketMapItr itr_name_tick_end = m_kNameToResTicketMap.end();
			if (itr_name_tick != itr_name_tick_end)
				m_kNameToResTicketMap.erase(itr_name_tick);

			kResourceList.push_back(pkResource);
			itr_map->second = 0;
		}
		else
		{
			kNoReleaseResMap.insert(*itr_map);
			kNoReleaseResNameMap.insert(pkResource->GetFileName());
		}
	}

	m_kResourceMap.clear();
	m_kResourceMap.insert(kNoReleaseResMap.begin(), kNoReleaseResMap.end());

	sdResourceSystem* pkResourceSystem = (sdResourceSystem*)sdResourceSystem::GetResourceSystem();
	NIASSERT(pkResourceSystem);

	if (m_eResType == E_RES_TYPE_NIF)
	{
		sdNifResourcePool* pkNifResourcePool = pkResourceSystem->GetNifResourcePool();
		NIASSERT(pkNifResourcePool);
		pkNifResourcePool->RemoveGCInstance();
	}
	else if (m_eResType == E_RES_TYPE_KFM)
	{
		sdKfmResourcePool* pkKfmResourcePool = pkResourceSystem->GetKfmResourcePool();
		NIASSERT(pkKfmResourcePool);
		pkKfmResourcePool->RemoveGCInstance(kNoReleaseResNameMap);
	}
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE