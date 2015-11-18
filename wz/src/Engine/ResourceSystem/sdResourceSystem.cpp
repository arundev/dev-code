#include "sdResourceSystemPCH.h"
#include "sdResourceSystem.h"
#include "sdResourceStream.h"

#include <sdWorld.h>

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RESOURCESYSTEM
//-------------------------------------------------------------------------------------------------
sdResourceSystem::sdResourceSystem()
: m_spProcessThread(NULL)
, m_spSyncStream(NULL)
, m_spAsyncStream(NULL)
, m_uiMaxRasingCount(999)
, m_dMaxProcessTime(5.f)
, m_bDisposing(false)
{
#ifdef _DEBUG
	m_dMaxProcessTime = 100.0;
#else
	m_dMaxProcessTime = 5.0;
#endif
}
//-------------------------------------------------------------------------------------------------
sdResourceSystem::~sdResourceSystem()
{

}
//-------------------------------------------------------------------------------------------------
bool sdResourceSystem::Initialize()
{
	// ��ʼ��PNG�����ȡ
	NiDevImageConverter* pkDevImageConverter = NiNew NiDevImageConverter;
	NIASSERT(pkDevImageConverter);
	pkDevImageConverter->AddReader(NiNew NiPNGReader);
	NiDevImageConverter::SetImageConverter(pkDevImageConverter);

	// ��ʼ����Դ��
	m_kResourceGroupVec.resize(NUM_RES);
	m_kResourceGroupVec[E_RES_TYPE_NIF] = NiNew sdResourceGroup(E_RES_TYPE_NIF);
	m_kResourceGroupVec[E_RES_TYPE_KFM] = NiNew sdResourceGroup(E_RES_TYPE_KFM);

	m_uiResourceGroupCount = NUM_RES;

	// ��ʼ����Դ�����
	m_pkNifResourcePool = NiNew sdNifResourcePool;
	NIASSERT(m_pkNifResourcePool);

	m_pkKfmResourcePool = NiNew sdKfmResourcePool;
	NIASSERT(m_pkKfmResourcePool);

	m_pkKfResourcePool = NiNew sdKfResourcePool;
	NIASSERT(m_pkKfResourcePool);

	// ��ʼ����Դ�����߳�
	if (!m_spProcessThread)
	{
		m_bActive = true;

		m_spProcessThread = NiThread::Create(this);
		NIASSERT(m_spProcessThread);
		m_spProcessThread->Resume();
	}

	// ��ʼ��������
	m_spSyncStream = NiNew sdResourceStream;
	NIASSERT(m_spSyncStream);

	m_spAsyncStream = NiNew sdResourceStream;
	NIASSERT(m_spAsyncStream);

	return true;
}
//-------------------------------------------------------------------------------------------------
void sdResourceSystem::Destroy()
{
	// �����ռ�
	GarbageCollect();
	m_bDisposing = true;

	// �ύ������Դ�ͷ�����ProcessList
	m_kProcessListLocker.Lock();

	if (!m_kRequestList.empty())
	{
		sdRequestInfoListItr itr = m_kRequestList.begin();
		sdRequestInfoListItr itr_end = m_kRequestList.end();
		for (; itr != itr_end; ++itr)
		{
			sdRequestInfo& kRequestInfo = *itr;
			if (kRequestInfo.m_eRequest == sdRequestInfo::E_REQ_FREE)
				m_kProcessList.push_back(kRequestInfo);
		}
		m_kRequestList.clear();
	}

	m_kProcessListLocker.Unlock();

	// ������Դ�����߳�
	m_bActive = false;

	if (m_spProcessThread)
	{
		m_spProcessThread->WaitForCompletion();

		NiDelete m_spProcessThread;
		m_spProcessThread = NULL;
	}

	// �ͷ�ProcessList�е���Դ
	if (!m_kProcessList.empty())
	{
		sdRequestInfoListItr itr = m_kProcessList.begin();
		sdRequestInfoListItr itr_end = m_kProcessList.end();
		for (; itr != itr_end; ++itr)
		{
			sdRequestInfo& kRequestInfo = *itr;
			kRequestInfo.m_spResource->FreeResource(false);
			kRequestInfo.m_spResource = 0;
		}

		m_kProcessList.clear();
	}	

	// �ͷ�FinishedList�е���Դ
	if (!m_kFinishedList.empty())
	{
		sdRequestInfoListItr itr = m_kFinishedList.begin();
		sdRequestInfoListItr itr_end = m_kFinishedList.end();
		for (; itr != itr_end; ++itr)
		{
			sdRequestInfo& kRequestInfo = *itr;
			kRequestInfo.m_spResource->FreeResource(false);
			kRequestInfo.m_spResource = 0;
		}

		m_kFinishedList.clear();
	}	

	// �ͷ�CachedList�е���Դ
	if (!m_kCachedList.empty())
	{
		sdRequestInfoListItr itr = m_kCachedList.begin();
		sdRequestInfoListItr itr_end = m_kCachedList.end();
		for (; itr != itr_end; ++itr)
		{
			sdRequestInfo& kRequestInfo = *itr;
			kRequestInfo.m_spResource->FreeResource(false);
			kRequestInfo.m_spResource = 0;
		}

		m_kFinishedList.clear();
	}	

	// ������Դ�����
	m_pkNifResourcePool = 0;
	m_pkKfmResourcePool = 0;
	m_pkKfResourcePool = 0;
}
//-------------------------------------------------------------------------------------------------
void sdResourceSystem::Update()
{
	// ������ʱ��
	m_kProfileOutput.Begin();

	// ����m_kCachedList,ִ�лص�֪ͨ
	uint uiRasedCount = 0;
	sdRequestInfoListItr itr = m_kCachedList.begin();
	sdRequestInfoListItr itr_end = m_kCachedList.end();
	for (; itr != itr_end; ++itr)
	{
		RaiseEvent((*itr).m_spResource);

		itr = m_kCachedList.erase(itr);

		++uiRasedCount;
		if (uiRasedCount > m_uiMaxRasingCount)
			break;

		if (m_kProfileOutput.IsTimeOver(m_dMaxProcessTime))
			break;
	}

	// ����m_kFinishedList
	//	E_REQ_LOAD, ֱ��ִ����Դ�ص�֪ͨ����ӵ������б�m_kCachedList
	//	E_REQ_FREE, ֱ��ɾ��
	// (ʹ��TryLock�ķ�ʽ��ֹ���̶߳���)
	if (m_kFinishedListLocker.TryLock())
	{
		if (!m_kFinishedList.empty())
		{
			sdRequestInfoListItr itr = m_kFinishedList.begin();
			sdRequestInfoListItr itr_end = m_kFinishedList.end();
			for (; itr != itr_end; ++itr)
			{
				sdRequestInfo& kRequestInfo = *itr;
				if (kRequestInfo.m_eRequest == sdRequestInfo::E_REQ_LOAD)
				{
					if (kRequestInfo.m_spResource)
						kRequestInfo.m_spResource->CacheResource();

					if (uiRasedCount < m_uiMaxRasingCount && !m_kProfileOutput.IsTimeOver(m_dMaxProcessTime))
					{
						RaiseEvent(kRequestInfo.m_spResource);
						++uiRasedCount;
					}
					else
					{
						m_kCachedList.push_back(kRequestInfo);
					}
				}
				else
				{
					kRequestInfo.m_spResource = 0;
				}
			}

			m_kFinishedList.clear();
		}

		m_kFinishedListLocker.Unlock();
	}

	// ����m_kRequestList,
	//	E_REQ_LOAD && Cached	ֱ��ִ����Դ�ص�֪ͨ����ӵ������б�m_kCachedList
	//	Other					��ӵ�m_kProcessList
	// (ʹ��TryLock�ķ�ʽ��ֹ���̶߳���)
	if (m_kProcessListLocker.TryLock())
	{
		if (!m_kRequestList.empty())
		{
			sdRequestInfoListItr itr = m_kRequestList.begin();
			sdRequestInfoListItr itr_end = m_kRequestList.end();
			for (; itr != itr_end; ++itr)
			{
				sdRequestInfo& kRequestInfo = *itr;
				if (kRequestInfo.m_eRequest == sdRequestInfo::E_REQ_LOAD && kRequestInfo.m_spResource->Cached())
				{
					if (uiRasedCount < m_uiMaxRasingCount && !m_kProfileOutput.IsTimeOver(m_dMaxProcessTime))
					{
						RaiseEvent(kRequestInfo.m_spResource);
					}
					else
					{
						m_kCachedList.push_back(kRequestInfo);
					}
				}
				else
				{
					m_kProcessList.push_back(kRequestInfo);
				}
			}

			m_kRequestList.clear();
		}

		if (m_kProcessList.size() > 1)
			m_kProcessList.sort(stRequestProcessListOrderFunctor());

		m_kProcessListLocker.Unlock();	
	}
}
//-------------------------------------------------------------------------------------------------
uint sdResourceSystem::ThreadProcedure(NiThread* spThread)
{
	//__VLD_THREAD_NAME;

	while (m_bActive)
	{
		m_kProcessListLocker.Lock();

		if (m_kProcessList.empty())
		{
			m_kProcessListLocker.Unlock();
			::Sleep(10);
		}
		else
		{
			sdRequestInfo kRequestInfo = m_kProcessList.front();
			m_kProcessList.pop_front();

			m_kProcessListLocker.Unlock();

			if (kRequestInfo.m_spResource)
			{
				if (!kRequestInfo.m_spResource->Ignored() && kRequestInfo.m_eRequest == sdRequestInfo::E_REQ_LOAD)
				{
					if (!m_bDisposing)
						kRequestInfo.m_spResource->LoadResource();

					m_kFinishedListLocker.Lock();
					m_kFinishedList.push_back(kRequestInfo);
					m_kFinishedListLocker.Unlock();
				}
				else
				{
					kRequestInfo.m_spResource->FreeResource();
				}
			}
		}
	}

	return 0;
}
//-------------------------------------------------------------------------------------------------
void sdResourceSystem::RegisterEvent(sdResourceEvent* pkEvent)
{
	NIASSERT(pkEvent);

	// Event����TicketEventMap
	sdResourceEventPtr pkResEvent = pkEvent;
	sdResourceEventList* pkResEventList = NULL;
	if (m_kTicketEventMap.GetAt(pkResEvent->GetTicket(), pkResEventList))
	{
		pkResEventList->AddEvent(pkResEvent);
	}
	else
	{
		pkResEventList = NiNew sdResourceEventList;
		pkResEventList->AddEvent(pkResEvent);
		m_kTicketEventMap.SetAt(pkResEvent->GetTicket(), pkResEventList);
	}

	// Event����ObjectEventMap
	if (m_kObjectEventMap.GetAt(pkResEvent->GetObject(), pkResEventList))
	{
		pkResEventList->AddEvent(pkResEvent);
	}
	else
	{
		pkResEventList = NiNew sdResourceEventList;
		pkResEventList->AddEvent(pkResEvent);
		m_kObjectEventMap.SetAt(pkResEvent->GetObject(), pkResEventList);
	}
}
//-------------------------------------------------------------------------------------------------
void sdResourceSystem::UnregisterEvent(const void* pkObject)
{
	NIASSERT(pkObject);

	sdResourceEventList* pkObjectEventList = NULL;
	if (m_kObjectEventMap.GetAt(pkObject, pkObjectEventList))
	{
		const NiTPointerList<sdResourceEventPtr>& kResEvents = pkObjectEventList->GetEvents();
		NiTListIterator kIter = kResEvents.GetHeadPos();
		while (kIter)
		{
			sdResourceEvent* pkResEvent = kResEvents.GetNext(kIter);
			NIASSERT(pkResEvent);

			sdResource::ResTicket kResTicket = pkResEvent->GetTicket();

			// ȡ������
			sdResourceGroup* pkResourceGroup = m_kResourceGroupVec[kResTicket >> 28];
			if (pkResourceGroup)
			{
				sdResourcePtr pkResource = pkResourceGroup->RemoveResource(kResTicket);
				if (pkResource)
				{
					pkResource->IgnoreResource();
					pkResource = 0;
				}
			}

			// EventList��TicketEventMap�Ƴ�
			sdResourceEventList* pkTicketEventList = NULL;
			if (m_kTicketEventMap.GetAt(kResTicket, pkTicketEventList))
			{
				if (!pkTicketEventList->RemoveEvent(pkResEvent).GetSize())
				{
					NiDelete pkTicketEventList;
					pkTicketEventList = NULL;
					m_kTicketEventMap.RemoveAt(kResTicket);
				}
			}
		}

		// EventList��ObjectEventMap�Ƴ�
		NiDelete pkObjectEventList;
		pkObjectEventList = NULL;
		m_kObjectEventMap.RemoveAt(pkObject);
	}
}
//-------------------------------------------------------------------------------------------------
void sdResourceSystem::ClearAllEvents()
{
	// ����TicketEventMap
	NiTMapIterator kIter = m_kTicketEventMap.GetFirstPos();
	while (kIter)
	{
		sdResource::ResTicket kResTick;
		sdResourceEventList* pkResEventList = NULL;
		m_kTicketEventMap.GetNext(kIter, kResTick, pkResEventList);
		if (pkResEventList)
		{
			NiDelete pkResEventList;
			pkResEventList = NULL;
		}
	}

	// ����ObjectEventMap
	kIter = m_kObjectEventMap.GetFirstPos();
	while (kIter)
	{
		const void* pkObject = NULL;
		sdResourceEventList* pkResEventList = NULL;
		m_kObjectEventMap.GetNext(kIter, pkObject, pkResEventList);
		if (pkResEventList)
		{
			NiDelete pkResEventList;
			pkResEventList = NULL;
		}
	}
}
//-------------------------------------------------------------------------------------------------
void sdResourceSystem::RaiseEvent(ResourceSystem::sdResource *pkResource)
{
	if (!pkResource)
		return;

	sdResourceEventList* pkTicketEventList = NULL;
	if (m_kTicketEventMap.GetAt(pkResource->GetTicket(), pkTicketEventList))
	{
		const NiTPointerList<sdResourceEventPtr>& kResEvents = pkTicketEventList->GetEvents();
		NiTListIterator kIter = kResEvents.GetHeadPos();
		while (kIter)
		{
			sdResourceEvent* pkResEvent = kResEvents.GetNext(kIter);
			NIASSERT(pkResEvent);

			sdResourceEventList* pkObjectEventList = NULL;
			if (m_kObjectEventMap.GetAt(pkResEvent->GetObject(), pkObjectEventList))
			{
				if (!pkObjectEventList->RemoveEvent(pkResEvent).GetSize())
				{
					NiDelete pkObjectEventList;
					pkObjectEventList = NULL;
					m_kObjectEventMap.RemoveAt(pkResEvent->GetObject());
				}
			}

			if (!pkResource->Ignored())
				(*pkResEvent)(pkResource);	///< Call bacl after remove
		}

		NiDelete pkTicketEventList;
		pkTicketEventList = NULL;
		m_kTicketEventMap.RemoveAt(pkResource->GetTicket());
	}
}
//-------------------------------------------------------------------------------------------------
void sdResourceSystem::GarbageCollect()
{
	sdRequestInfoListItr itr = m_kCachedList.begin();
	sdRequestInfoListItr itr_end = m_kCachedList.end();
	for (; itr != itr_end; ++itr)
	{
		sdRequestInfo& kRequestInfo = *itr;
		RaiseEvent(kRequestInfo.m_spResource);
	}
	m_kCachedList.clear();

	itr = m_kRequestList.begin();
	itr_end = m_kRequestList.end();
	for (; itr != itr_end;)
	{
		sdRequestInfo& kRequestInfo = *itr;

		if (kRequestInfo.m_eRequest == sdRequestInfo::E_REQ_LOAD && kRequestInfo.m_spResource->Cached())
		{
			RaiseEvent(kRequestInfo.m_spResource);
			itr = m_kRequestList.erase(itr);
		}
		else
		{
			++itr;
		}
	}
	m_kRequestList.clear();

	ResourceGroupVecItr itr_res_gp = m_kResourceGroupVec.begin();
	ResourceGroupVecItr itr_res_gp_end = m_kResourceGroupVec.end();
	for (; itr_res_gp != itr_res_gp_end; ++itr_res_gp)
	{
		sdResourceGroup* pkResourceGroup = *itr_res_gp;
		if (!pkResourceGroup)
			continue;

		ResourceList kResourceList;
		pkResourceGroup->GarbageCollect(kResourceList);

		ResourceListItr itr_res_list = kResourceList.begin();
		ResourceListItr itr_res_list_end = kResourceList.end();
		for (; itr_res_list != itr_res_list_end; ++itr_res_list)
		{
			// ��̨�߳������Դ
			//RequestFree(*itr_res_list);

			// ���߳������Դ
			sdResourcePtr pkResource = *itr_res_list;
			pkResource->FreeResource(false);
			pkResource = 0;
		}
	}
}
//-------------------------------------------------------------------------------------------------
bool sdResourceSystem::Convert2ResPath(const char* szPath, char* szNewPath)
{
	NIASSERT(szPath);
	NIASSERT(szNewPath);
	return sdWorldManager::Instance().Convert2ResPath(szPath, szNewPath);
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE