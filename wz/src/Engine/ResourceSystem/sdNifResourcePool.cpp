#include "sdResourceSystemPCH.h"
#include "sdNifResourcePool.h"
#include "sdResourceSystem.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RESOURCESYSTEM
//-------------------------------------------------------------------------------------------------
sdNifResourcePool::sdNifResourcePool()
{

}
//-------------------------------------------------------------------------------------------------
sdNifResourcePool::~sdNifResourcePool()
{
	RemoveAllInstance();
}
//-------------------------------------------------------------------------------------------------
NiNodePtr sdNifResourcePool::LoadNifFile(const std::string& kFileName, EResUsageType eUsageType, bool bMainThreadLoad /* = false */)
{
	sdResourceSystem* pkResourceSystem = (sdResourceSystem*)sdResourceSystem::GetResourceSystem();
	if (!pkResourceSystem)
		return 0;

	NiStream* spStream = bMainThreadLoad ? pkResourceSystem->GetSyncStream() : pkResourceSystem->GetAsyncStream();
	return LoadNifFile(kFileName, eUsageType, spStream);
}
//-------------------------------------------------------------------------------------------------
NiNodePtr sdNifResourcePool::LoadNifFile(const std::string& kFileName, EResUsageType eUsageType, NiStream* spStream)
{
	NiNodePtr spNifInstance = 0;

	m_kLock.Lock();
	spNifInstance = m_kCacheNifMap[kFileName].m_spRootNode;
	if (!spNifInstance)
	{
		m_kLock.Unlock();

		// 从磁盘加载资源
		spNifInstance = LoadFromDisk(kFileName, eUsageType, spStream);

		// 将资源加入缓存
		m_kLock.Lock();
		CacheNifInfo kCacheNifInfo;
		kCacheNifInfo.m_iCloneCount = 0;
		kCacheNifInfo.m_bKFMRoot = (eUsageType == E_RES_USAGE_NIF_KFMROOT);
		kCacheNifInfo.m_spRootNode = spNifInstance;
		m_kCacheNifMap[kFileName] = kCacheNifInfo;
	}

	CacheNifInfo& kCacheNifInfo = m_kCacheNifMap[kFileName];
	kCacheNifInfo.m_iCloneCount++;

	NiNodePtr spRootNode = 0;
	if (spNifInstance)
	{
		spRootNode = (NiNode*)spNifInstance->Clone(m_kCloingProcess);

		// Remove all cloned instance
		m_kCloingProcess.m_pkProcessMap->RemoveAll();
		m_kCloingProcess.m_pkCloneMap->RemoveAll();
	}
	
	m_kLock.Unlock();

	return spRootNode;
}
//-------------------------------------------------------------------------------------------------
NiNodePtr sdNifResourcePool::LoadFromDisk(const std::string& kFileName, EResUsageType eUsageType, NiStream* spStream)
{
	NiNodePtr spRootNode = 0;
	if (spStream && spStream->Load(kFileName.c_str()) && spStream->GetObjectCount())
	{
		spRootNode = (NiNode*)spStream->GetObjectAt(0);
		spStream->RemoveAllObjects();
	}

	return spRootNode;
}
//-------------------------------------------------------------------------------------------------
void sdNifResourcePool::FreeNifFile(const std::string& kFileName)
{
	m_kLock.Lock();
	CacheNifMapItr itr = m_kCacheNifMap.find(kFileName);
	CacheNifMapItr itr_end = m_kCacheNifMap.end();
	if (itr != itr_end)
	{
		CacheNifInfo& kCacheNifInfo = itr->second;
		kCacheNifInfo.m_iCloneCount--;
		if (kCacheNifInfo.m_iCloneCount <= 0)
			m_kCacheNifMap.erase(itr);
	}
	m_kLock.Unlock();
}
//-------------------------------------------------------------------------------------------------
void sdNifResourcePool::FreeKFMRoot(NiActorManager* spActorMgr)
{
	if (!spActorMgr)
		return;

	NiKFMTool* spKFMTool = spActorMgr->GetKFMTool();
	if (!spKFMTool)
		return;

	FreeNifFile((const char*)spKFMTool->GetFullModelPath());
}
//-------------------------------------------------------------------------------------------------
void sdNifResourcePool::RemoveAllInstance()
{
	//__VLD_THREAD_TRACE_AUTO2(sdNifResourcePool);

	m_kLock.Lock();
	m_kCacheNifMap.clear();
	m_kLock.Unlock();
}
//-------------------------------------------------------------------------------------------------
void sdNifResourcePool::RemoveGCInstance()
{
	m_kLock.Lock();
	CacheNifMapItr itr = m_kCacheNifMap.begin();
	CacheNifMapItr itr_end = m_kCacheNifMap.end();
	for (; itr != itr_end;)
	{
		CacheNifInfo& kCacheNifInfo = itr->second;
		kCacheNifInfo.m_iCloneCount--;
		if (kCacheNifInfo.m_bKFMRoot && kCacheNifInfo.m_iCloneCount <= 2)
			++itr;
		else
			itr = m_kCacheNifMap.erase(itr);
	}
	m_kLock.Unlock();
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE