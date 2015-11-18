#include "sdEnginePCH.h"
#include "sdPixelLoadTaskQueue.h"
#include "sdPixelLoader.h"
#include "sdSourceTexture.h"

#include <sdLock.h>

using namespace Base::Threading;

ENGINE_NAMESPACE_BEGIN_ENGINE
//-------------------------------------------------------------------------------------------------
sdPixelLoadTaskQueue::sdPixelLoadTaskQueue(sdTextureCache* pkTextureCache)
{
	m_pkPixelLoader = NiNew sdPixelLoader(this, pkTextureCache);
	NIASSERT(m_pkPixelLoader);

	::ZeroMemory(&m_kLoadIndicatorList, sizeof(m_kLoadIndicatorList));
}
//-------------------------------------------------------------------------------------------------
sdPixelLoadTaskQueue::~sdPixelLoadTaskQueue()
{
	NiDelete m_pkPixelLoader;
	m_pkPixelLoader = NULL;

	PixelLoadTaskListItr kItr = m_kLoadedList.begin();
	PixelLoadTaskListItr kItrEnd = m_kLoadedList.end();
	for (; kItr != kItrEnd; ++kItr)
	{
		PixelLoadTask& kTask = (*kItr);
		kTask.m_pkTexture = 0;
	}
	m_kLoadedList.clear();

	for (int i = 0; i < MAX_TEXTURE_PRIORITY+1; ++i)
	{
		PixelLoadTaskList& kTaskList = m_kPreloadListVec[i];

		kItr = m_kLoadedList.begin();
		kItrEnd = m_kLoadedList.end();
		for (; kItr != kItrEnd; ++kItr)
		{
			PixelLoadTask& kTask = (*kItr);
			kTask.m_pkTexture = 0;
		}

		kTaskList.clear();
	}

	::ZeroMemory(&m_kLoadIndicatorList, sizeof(m_kLoadIndicatorList));
}
//-------------------------------------------------------------------------------------------------
bool sdPixelLoadTaskQueue::AddLoadTask(sdSourceTexturePtr pkTexture, uchar ucSkipMipmapLvl)
{
	NiFixedString kFileName = pkTexture->GetPlatformSpecificFilename();
	if (!kFileName.Exists())
		return false;

	bool bResult = false;

	// 加入加载队列
	{
		SDTLock<sdCriticalSection> kLockImpl(m_kLock);

		SourceTextureMapItr kItr = m_kTaskDB.find(kFileName);
		SourceTextureMapItr kItrEnd = m_kTaskDB.end();
		if (kItr != kItrEnd)
		{
			if (pkTexture->GetTexStatus() == sdSourceTexture::E_TEX_PARAM_INITED ||
				pkTexture->GetTexStatus() == sdSourceTexture::E_TEX_PIX_PRE_LOAD ||
				pkTexture->GetTexStatus() == sdSourceTexture::E_TEX_IGNORED		 ||
				pkTexture->GetTexStatus() == sdSourceTexture::E_TEX_CREATED)
			{
				uchar ucPriority = pkTexture->GetPriority();
				NIASSERT(ucPriority <= MAX_TEXTURE_PRIORITY);

				PixelLoadTaskList& kPixelLoadTaskList = m_kPreloadListVec[ucPriority];

				PixelLoadTaskListItr kItr = kPixelLoadTaskList.begin();
				PixelLoadTaskListItr kItrEnd = kPixelLoadTaskList.end();
				for (; kItr != kItrEnd; ++kItr)
				{
					PixelLoadTask& kTask = (*kItr);
					if (kTask.m_pkTexture == pkTexture)
						break;
				}

				if (kItr == kItrEnd)
				{
					// 加入任务队列
					NiPixelData* pkPixelData = pkTexture->GetSourcePixelData();
					NiPersistentSrcTextureRendererData* pkPersisData = pkTexture->GetPersistentSourceRendererData();
					if (pkPixelData != NULL || pkPersisData != NULL)
					{
						PixelLoadTask kTask;
						kTask.m_cMipmap = 0;
						kTask.m_pkTexture = pkTexture;

						pkTexture->SetTexStatus(sdSourceTexture::E_TEX_PIX_LOADED);
						m_kLoadedList.push_back(kTask);	
					}
					else
					{
						kItr = kPixelLoadTaskList.begin();
						std::advance(kItr, m_kLoadIndicatorList[ucPriority].m_uiPreloadCount);

						PixelLoadTask kTask;
						kTask.m_cMipmap = 0;
						kTask.m_pkTexture = pkTexture;

						pkTexture->SetTexStatus(sdSourceTexture::E_TEX_PIX_PRE_LOAD);
						
						kPixelLoadTaskList.insert(kItr, kTask);
						m_kLoadIndicatorList[ucPriority].m_uiPreloadCount++;
					}

					// 加入任务数据库
					m_kTaskDB[kFileName] = (sdSourceTexture*)pkTexture;

					bResult = true;
				}
				else
				{
					NIASSERT(0);
				}
			}	
		}
		else
		{
			///< 已经在加载队列了不处理
		}
	}
	
	// 唤醒加载器
	m_pkPixelLoader->LoadWake();
	m_pkPixelLoader->DecreaseWake();

	return bResult;
}
//-------------------------------------------------------------------------------------------------
bool sdPixelLoadTaskQueue::AddDecreaseTask(sdSourceTexture* pkTexture, uchar ucSkipMipmapLvl)
{
	NiFixedString kFileName = pkTexture->GetPlatformSpecificFilename();
	if (!kFileName.Exists())
		return false;

	bool bResult = false;

	// 加入加载队列
	{
		SDTLock<sdCriticalSection> kLockImpl(m_kLock);

		SourceTextureMapItr kItr = m_kTaskDB.find(kFileName);
		SourceTextureMapItr kItrEnd = m_kTaskDB.end();
		if (kItr != kItrEnd)
		{
			PixelLoadTask kTask;
			kTask.m_pkTexture = pkTexture;
			kTask.m_cMipmap = ucSkipMipmapLvl;

			m_kLoadedList.push_back(kTask);		///< 加入任务队列
			m_kTaskDB[kFileName] = pkTexture;	///< 加入数据库

			bResult = true;
		}
	}

	// 唤醒加载器
	m_pkPixelLoader->DecreaseWake();

	return bResult;
}
//-------------------------------------------------------------------------------------------------
uint sdPixelLoadTaskQueue::GetRecentPreloadedTask(PixelLoadTask* pkTaskes, uint uiTaskWanted)
{
	SDTLock<sdCriticalSection> kLockImpl(m_kLock);

	PixelLoadTask kTask;
	kTask.m_pkTexture = 0;
	kTask.m_cMipmap = 0;

	uint uiTaskGet = 0;
	for (int i = 0; i < MAX_TEXTURE_PRIORITY+1; ++i)
	{
		PixelLoadTaskList& kTaskList = m_kPreloadListVec[i];
		if (kTaskList.empty() || kTaskList.front().m_pkTexture->GetTexStatus() != sdSourceTexture::E_TEX_PIX_PRE_LOAD)
		{
			continue;	///< 当前队列已经全部加载完成
		}
		else
		{
			uiTaskGet = m_kLoadIndicatorList[i].m_uiPreloadCount < uiTaskWanted ? 
				m_kLoadIndicatorList[i].m_uiPreloadCount : uiTaskWanted;

			PixelLoadTaskListItr kItr = kTaskList.begin();
			PixelLoadTaskListItr kItrBegin = kItr;
			for (uint j = 0; j < uiTaskGet; ++j, ++kItr)
			{
				NIASSERT(kItr->m_pkTexture->GetTexStatus() == sdSourceTexture::E_TEX_PIX_PRE_LOAD);
				pkTaskes[j].m_pkTexture = kItr->m_pkTexture;
				pkTaskes[j].m_pkTexture->SetTexStatus(sdSourceTexture::E_TEX_PIX_LOADING);			
			}

			kTaskList.erase(kItrBegin, kItr);
			m_kLoadIndicatorList[i].m_uiPreloadCount -= uiTaskGet;

			break;
		}
	}

	return uiTaskGet;
}
//-------------------------------------------------------------------------------------------------
uint sdPixelLoadTaskQueue::GetRecentLoadedTask(PixelLoadTask* pkTaskes, uint uiTaskWanted)
{
	SDTLock<sdCriticalSection> kLockImpl(m_kLock);

	uint uiTotalCount = m_kLoadedList.size();
	uint uiTaskGet = uiTotalCount < uiTaskWanted ? uiTotalCount : uiTaskWanted;

	PixelLoadTaskListItr kItr = m_kLoadedList.begin();
	PixelLoadTaskListItr kItrBegin = kItr;
	for (uint j = 0; j < uiTaskGet; ++j, ++kItr)
	{
		pkTaskes[j] = (*kItr);		
	}

	m_kLoadedList.erase(kItrBegin, kItr);

	return uiTaskGet;
}
//-------------------------------------------------------------------------------------------------
void sdPixelLoadTaskQueue::NotifyCompleteTask(PixelLoadTask* pkTaskes, uint uiTaskNum)
{
	SDTLock<sdCriticalSection> kLockImpl(m_kLock);

	for (uint ui = 0; ui < uiTaskNum; ++ui)
	{
		PixelLoadTask& kTask = pkTaskes[ui];
		NiFixedString kFileName = kTask.m_pkTexture->GetPlatformSpecificFilename();
		
		SourceTextureMapItr kMapItr = m_kTaskDB.find(kFileName);
		SourceTextureMapItr kMapItrEnd = m_kTaskDB.end();
		if (kMapItr != kMapItrEnd)
		{
			m_kTaskDB.erase(kMapItr);

			uint uiRef =  kTask.m_pkTexture->GetRefCount();
		////

		}
		else
		{
			NIASSERT(0);
		}
	}
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE