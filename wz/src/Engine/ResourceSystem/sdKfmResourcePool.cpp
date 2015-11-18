#include "sdResourceSystemPCH.h"
#include "sdKfmResourcePool.h"
#include "sdResourceSystem.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RESOURCESYSTEM
//-------------------------------------------------------------------------------------------------
sdKfmResourcePool::sdKfmResourcePool()
{

}
//-------------------------------------------------------------------------------------------------
sdKfmResourcePool::~sdKfmResourcePool()
{
	RemoveAllInstance();
}
//-------------------------------------------------------------------------------------------------
NiActorManagerPtr sdKfmResourcePool::LoadKfmFile(const std::string& kFileName, EResUsageType eUsageType, bool bReleaseOnGC /* = true */, bool bMainThreadLoad /* = false */)
{
	NiActorManagerPtr spActorMgrInstance = 0;

	m_kLock.Lock();
	spActorMgrInstance = m_kCacheKfmInfoMap[kFileName].m_spActorMgr;
	if (!spActorMgrInstance)
	{
		m_kLock.Unlock();

		// 从磁盘加载资源
		spActorMgrInstance = LoadFromDisk(kFileName, bMainThreadLoad);

		// 将资源加入缓存
		m_kLock.Lock();
		CacheKfmInfo kCacheKfmInfo;
		kCacheKfmInfo.m_iCloneCount = 0;
		kCacheKfmInfo.m_bReleaseOnGC = bReleaseOnGC;
		kCacheKfmInfo.m_spActorMgr = spActorMgrInstance;
		m_kCacheKfmInfoMap[kFileName] = kCacheKfmInfo;
	}

	CacheKfmInfo& kCacheKfmInfo = m_kCacheKfmInfoMap[kFileName];
	kCacheKfmInfo.m_iCloneCount++;

	NiActorManagerPtr spActorMgr = 0;
	if (spActorMgrInstance)
	{
		//// 根据Usage类型决定追加文本
		//BYTE byAppend = 0;
		//switch(eUsageType)
		//{
		//case E_RES_USAGE_KFM_COMMON:
		//	byAppend = TEXT_KEY_NONE;
		//	break;
		//case E_RES_USAGE_KFM_SELF:
		//	byAppend = TEXT_KEY_SELF;
		//	break;
		//case E_RES_USAGE_KFM_PLAYER:
		//	byAppend = TEXT_KEY_PLAYER;
		//	break;
		//case E_RES_USAGE_KFM_NPC:
		//	byAppend = TEXT_KEY_NPC;
		//	break;
		//default:
		//	byAppend = TEXT_KEY_ALL;
		//}
		//
		//m_kCloingProcess.m_cAppendChar = GetTypeFromUsage(eUsageType);

		// 克隆
		spActorMgr = (NiActorManager*)spActorMgrInstance->Clone(&m_kCloingProcess);

		// Remove all cloned instance
		m_kCloingProcess.m_pkProcessMap->RemoveAll();
		m_kCloingProcess.m_pkCloneMap->RemoveAll();
	}

	m_kLock.Unlock();

	return spActorMgr;
}
//-------------------------------------------------------------------------------------------------
NiActorManagerPtr sdKfmResourcePool::LoadFromDisk(const std::string& kFileName, bool bMainThreadLoad /* = false */)
{
	sdResourceSystem* pkResourceSystem = (sdResourceSystem*)sdResourceSystem::GetResourceSystem();
	if (!pkResourceSystem)
		return 0;

	// 获取加载数据流
	NiStream* spStream = bMainThreadLoad ? pkResourceSystem->GetSyncStream() : pkResourceSystem->GetAsyncStream();
	if (!spStream)
		return 0;

	// 加载
	NiActorManagerPtr spActorMgr = 0;
	sdResourceSystem::ACTORMANAGERCREATEFUNC pfnActorManagerCreate = pkResourceSystem->GetActorManagerCreateFunc();
	if (pfnActorManagerCreate)
		spActorMgr = pfnActorManagerCreate(kFileName.c_str(), false, true, spStream, NULL);
	else
		NiActorManager::Create(kFileName.c_str(), false, true, spStream);

	return spActorMgr;
}
//-------------------------------------------------------------------------------------------------
void sdKfmResourcePool::FreeKFMFile(const std::string& kFileName)
{
	sdResourceSystem* pkResourceSystem = (sdResourceSystem*)sdResourceSystem::GetResourceSystem();
	if (!pkResourceSystem)
		return;

	m_kLock.Lock();
	CacheKfmInfoMapItr itr = m_kCacheKfmInfoMap.find(kFileName);
	CacheKfmInfoMapItr itr_end = m_kCacheKfmInfoMap.end();
	if (itr != itr_end)
	{
		CacheKfmInfo& kCacheKfmInfo = itr->second;
		if (kCacheKfmInfo.m_bReleaseOnGC)
		{
			kCacheKfmInfo.m_iCloneCount--;
			if (kCacheKfmInfo.m_iCloneCount <= 0)
			{
				pkResourceSystem->GetKfResourcePool()->FreeSequenceData(kCacheKfmInfo.m_spActorMgr);
				pkResourceSystem->GetNifResourcePool()->FreeKFMRoot(kCacheKfmInfo.m_spActorMgr);
				m_kCacheKfmInfoMap.erase(itr);
			}
		}
	}
	m_kLock.Unlock();
}
//-------------------------------------------------------------------------------------------------
void sdKfmResourcePool::RemoveAllInstance()
{
	//__VLD_THREAD_TRACE_AUTO2(sdKfmResourcePool);

	m_kLock.Lock();
	m_kCacheKfmInfoMap.clear();
	m_kLock.Unlock();
}
//-------------------------------------------------------------------------------------------------
void sdKfmResourcePool::RemoveGCInstance(const std::set<std::string>& kNoReleaseMap)
{
	sdResourceSystem* pkResourceSystem = (sdResourceSystem*)sdResourceSystem::GetResourceSystem();
	if (!pkResourceSystem)
		return;

	m_kLock.Lock();
	CacheKfmInfoMapItr itr = m_kCacheKfmInfoMap.begin();
	CacheKfmInfoMapItr itr_end = m_kCacheKfmInfoMap.end();
	for (; itr != itr_end;)
	{
		const std::string& kFileName = itr->first;
		if (kNoReleaseMap.find(kFileName) == kNoReleaseMap.end())
		{
			pkResourceSystem->GetKfResourcePool()->FreeSequenceData(itr->second.m_spActorMgr);
			pkResourceSystem->GetNifResourcePool()->FreeKFMRoot(itr->second.m_spActorMgr);
			itr = m_kCacheKfmInfoMap.erase(itr);
		}
		else
		{
			++itr;
		}
	}
	m_kLock.Unlock();
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE