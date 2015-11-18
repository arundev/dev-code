#include "sdEnginePCH.h"
//#include "sdIOBuffPool.h"
//
//BASE_NAMESPACE_BEGIN_BASE
//BASE_NAMESPACE_BEGIN_IO
////-----------------------------------------------------------------------------------------
//sdIOBuff::sdIOBuff()
//	: m_uiSize(0), m_pcBuff(NULL), m_pkBuffPool(NULL)
//{
//
//}
////-----------------------------------------------------------------------------------------
//sdIOBuff::~sdIOBuff()
//{
//	m_uiSize = 0;
//	if (m_pcBuff)
//	{
//		DeallocBuff();
//		m_pcBuff = NULL;
//	}
//}
////-----------------------------------------------------------------------------------------
//void sdIOBuff::AllocBuff(uint uiSize)
//{
//	m_pkBuffPool->Lock();
//	AllocBuffNoLock(uiSize);
//	m_pkBuffPool->Unlock();
//}
////-----------------------------------------------------------------------------------------
//void sdIOBuff::DeallocBuff()
//{
//	m_pkBuffPool->Lock();
//	DeallocBuffNoLock();
//	m_pkBuffPool->Unlock();	
//}
////-----------------------------------------------------------------------------------------
//void sdIOBuff::AllocBuffNoLock(uint uiSize)
//{
//	if (m_pcBuff != NULL)
//		m_pkBuffPool->DeallocBuff(m_pcBuff);
//	m_pcBuff = m_pkBuffPool->AllocBuff(uiSize);
//}
////-----------------------------------------------------------------------------------------
//void sdIOBuff::DeallocBuffNoLock()
//{
//	if (m_pcBuff != NULL)
//	{
//		m_pkBuffPool->DeallocBuff(m_pcBuff);
//		m_pcBuff = NULL;
//	}
//}
////-----------------------------------------------------------------------------------------
//sdIOBuffPool::sdIOBuffPool()
//	: m_pkFreeBuff(NULL)
//{
//	
//}
////-----------------------------------------------------------------------------------------
//sdIOBuffPool::~sdIOBuffPool()
//{
//	// 释放所有Buff Pool Memory
//	while(m_pkFreeBuff)
//	{
//		stBuffHeader* pkNextBuff = m_pkFreeBuff->next_ptr;
//		delete [] m_pkFreeBuff;
//		m_pkFreeBuff = pkNextBuff;
//	}
//}
////-----------------------------------------------------------------------------------------
//sdIOBuff* sdIOBuffPool::AllocObject()
//{
//	sdIOBuff* pkBuff = sdTDynamicPool<sdIOBuff>::Alloc();
//	pkBuff->SetPool(this);
//	return pkBuff;
//}
////-----------------------------------------------------------------------------------------
//void sdIOBuffPool::DeallocObject(sdIOBuff* pkBuff)
//{
//	if (pkBuff)
//	{
//		if (pkBuff->GetPool() == this)
//		{
//			pkBuff->SetPool(NULL);
//			Dealloc(pkBuff);
//		}
//	}
//}
////-----------------------------------------------------------------------------------------
//char* sdIOBuffPool::AllocBuff(size_t uiSize)
//{
//	return NULL;
//}
////-----------------------------------------------------------------------------------------
//void sdIOBuffPool::DeallocBuff(void* pkData)
//{
//
//}
////-----------------------------------------------------------------------------------------
//sdIOBuffPoolGroup::sdIOBuffPoolGroup()
//{
//	m_kIOBuffPool[0] = new sdIOBuffPool();
//	m_kIOBuffPool[0]->Initialize(2, 1);
//
//	m_kIOBuffPool[1] = new sdIOBuffPool();
//	m_kIOBuffPool[1]->Initialize(2, 1);
//}
////-----------------------------------------------------------------------------------------
//sdIOBuffPoolGroup::~sdIOBuffPoolGroup()
//{
//	m_kIOBuffPool[0]->Destroy();
//	delete m_kIOBuffPool[0];
//	m_kIOBuffPool[0] = NULL;
//
//	m_kIOBuffPool[1]->Destroy();
//	delete m_kIOBuffPool[1];
//	m_kIOBuffPool[1] = NULL;
//}
////-----------------------------------------------------------------------------------------
//sdIOBuff* sdIOBuffPoolGroup::Alloc(uint uiSize)
//{
//	for (int i = 0; ; i = (i+1)%2)
//	{
//		sdIOBuffPool* pkPool = m_kIOBuffPool[i];
//		if (pkPool->TryLock())
//		{
//			sdIOBuff* pkBuff = pkPool->Alloc();	// 分配IOBuff对象
//			pkBuff->AllocBuffNoLock(uiSize);	// 分配IOBuff Memory区域
//			pkPool->Unlock();
//			return pkBuff;
//		}
//	}
//}
////-----------------------------------------------------------------------------------------
//void sdIOBuffPoolGroup::Dealloc(sdIOBuff* pkBuff)
//{
//	if (pkBuff == NULL)
//		return;
//
//	sdIOBuffPool* pkPool = pkBuff->GetPool();
//	if (pkPool == NULL)
//		return;
//
//	pkPool->Lock();
//	pkBuff->DeallocBuffNoLock();	// 归还IOBuff Memory区域
//	pkPool->DeallocObject(pkBuff);	// 归还IOBuff对象
//	pkPool->Unlock();
//}
////-----------------------------------------------------------------------------------------
//BASE_NAMESPACE_END_BASE
//BASE_NAMESPACE_END_IO