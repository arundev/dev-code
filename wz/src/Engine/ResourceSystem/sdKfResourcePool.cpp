#include "sdResourceSystemPCH.h"
#include "sdKfResourcePool.h"
#include "sdResourceSystem.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RESOURCESYSTEM
//-------------------------------------------------------------------------------------------------
sdKfResourcePool::sdKfResourcePool()
{

}
//-------------------------------------------------------------------------------------------------
sdKfResourcePool::~sdKfResourcePool()
{
	RemoveAllInstance();
}
//-------------------------------------------------------------------------------------------------
NiSequenceDataPtr sdKfResourcePool::LoadSequenceData(const std::string& kFileName, NiKFMTool::Sequence* spKFMSequence, NiStream* spStream)
{
	NiSequenceDataPtr spSequenceDataInstance = 0;

	// 在缓存查找
	m_kLock.Lock();
	CacheSequenceMapItr itr = m_kCacheSequenceMap.find(kFileName);
	CacheSequenceMapItr itr_end = m_kCacheSequenceMap.end();
	if (itr != itr_end)
	{
		CacheSequenceInfo& kCacheSequenceInfo = itr->second;
		spSequenceDataInstance = GetSequenceDataFromArray(spKFMSequence, kCacheSequenceInfo.m_kSequenceDataVec);
		if (spSequenceDataInstance)
			kCacheSequenceInfo.m_iCloneCount++;
	}
	m_kLock.Unlock();

	if (spSequenceDataInstance)
		return spSequenceDataInstance;

	CacheSequenceInfo kCacheSequenceInfo;
	kCacheSequenceInfo.m_iCloneCount = 0;

	// 新加载
	m_kLock.Lock();
	if (LoadFromDisk(kFileName, spStream, kCacheSequenceInfo.m_kSequenceDataVec))
	{
		spSequenceDataInstance = GetSequenceDataFromArray(spKFMSequence, kCacheSequenceInfo.m_kSequenceDataVec);
		if (spSequenceDataInstance)
			kCacheSequenceInfo.m_iCloneCount++;

		m_kCacheSequenceMap[kFileName] = kCacheSequenceInfo;
	}
	m_kLock.Unlock();

	return spSequenceDataInstance;
}
//-------------------------------------------------------------------------------------------------
NiSequenceDataPtr sdKfResourcePool::GetSequenceDataFromArray(NiKFMTool::Sequence* spKFMSequence, std::vector<NiSequenceDataPtr>& kSeqDataVec)
{
	NiSequenceDataPtr spSequenceDataInstance = 0;
	const NiFixedString& kSequenceName = spKFMSequence->GetSequenceName();
	if (kSequenceName.Exists())
	{
		std::vector<NiSequenceDataPtr>::iterator itr = kSeqDataVec.begin();
		std::vector<NiSequenceDataPtr>::iterator itr_end = kSeqDataVec.end();
		for (; itr != itr_end; ++itr)
		{
			NiSequenceData* spSequenceData = *itr;
			if (spSequenceData && spSequenceData->GetName() == kSequenceName)
			{
				spSequenceDataInstance = spSequenceData;
				break;
			}
		}
	}
	else
	{
		uint iAnimIndex = spKFMSequence->GetAnimIndex();
		if (iAnimIndex < kSeqDataVec.size() && iAnimIndex > 0)
			spSequenceDataInstance = kSeqDataVec[iAnimIndex];
	}

	return spSequenceDataInstance;
}
//-------------------------------------------------------------------------------------------------
void sdKfResourcePool::FreeSequenceData(NiActorManager* spActorMgr)
{
	if (!spActorMgr)
		return;

	NiKFMTool* spKFMTool = spActorMgr->GetKFMTool();
	if (!spKFMTool)
		return;

	NiActorManager::SequenceID* spSequenceID = NULL;
	uint uiNumIds = 0;
	spKFMTool->GetSequenceIDs(spSequenceID, uiNumIds);

	for (uint ui = 0; ui < uiNumIds; ++ui)
	{
		NiActorManager::SequenceID kSequenceID = spSequenceID[ui];
		NiFixedString kKfFileName = spKFMTool->GetFullKFFilename(kSequenceID);
		NiKFMTool::Sequence* spSequence = spKFMTool->GetSequence(kSequenceID);
		FreeSequenceData((const char*)kKfFileName, spSequence);
	}

	if (spSequenceID)
	{
		NiFree(spSequenceID);
		spSequenceID = NULL;
	}
}
//-------------------------------------------------------------------------------------------------
void sdKfResourcePool::FreeSequenceData(const std::string& kFileName, NiKFMTool::Sequence* spKFMSequence)
{
	if (spKFMSequence)
	{
		const NiFixedString& kSeqName = spKFMSequence->GetSequenceName();

		m_kLock.Lock();
		CacheSequenceMapItr itr = m_kCacheSequenceMap.find(kFileName);
		CacheSequenceMapItr itr_end = m_kCacheSequenceMap.end();
		if (itr != itr_end)
		{
			CacheSequenceInfo& kCacheSequenceInfo = itr->second;
			NiSequenceDataPtr spSequenceData = GetSequenceDataFromArray(spKFMSequence, kCacheSequenceInfo.m_kSequenceDataVec);
			if (spSequenceData)
				kCacheSequenceInfo.m_iCloneCount--;

			if (kCacheSequenceInfo.m_iCloneCount <= 0)
				m_kCacheSequenceMap.erase(itr);
		}
		m_kLock.Unlock();
	}
}
//-------------------------------------------------------------------------------------------------
bool sdKfResourcePool::LoadFromDisk(const std::string& kFileName, NiStream* spStream, std::vector<NiSequenceDataPtr>& kSeqDataVec)
{
	if (spStream && spStream->Load(kFileName.c_str()))
	{
		kSeqDataVec.clear();
		kSeqDataVec.resize(spStream->GetObjectCount());
		for (int i = 0; i < (int)spStream->GetObjectCount(); ++i)
			kSeqDataVec[i] = (NiSequenceData*)spStream->GetObjectAt(i);

		return true;
	}

	return false;
}
//-------------------------------------------------------------------------------------------------
void sdKfResourcePool::RemoveAllInstance()
{
	m_kLock.Lock();
	m_kCacheSequenceMap.clear();
	m_kLock.Unlock();
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE