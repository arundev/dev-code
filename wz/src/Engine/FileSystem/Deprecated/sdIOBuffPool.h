////*************************************************************************************************
//// ����:	�ļ�������󻺴��
////	
//// 1.����Windows Platform SDK Debug Tool(gflags)����, ��Ϸ��༭��������һ��ʱ��֮�����,
////	����Ϊ������IO����,��ÿ��IO��������ͷ�һ��������ڴ�,�������2G�ڴ�ռ���ڴ�����Ƭ,
////	��󴥷�C++�ڴ�����쳣���ؿ�ָ��,���²���Ԥ֪��Ϊ
//// 2.sdIOBuff�ǻ��ڵ��ļ���NiFile�Ļ����ϵ�
//// 3.sdIOBuffֻ��IO Read���˴���,û�д���IO Write
////---------------------------------------------------------
//// ����:		
//// ����:		2012-10-15
//// ����޸�:
////*************************************************************************************************
//#pragma once
//#ifndef _SD_BASE_IO_IO_BUFF_POOL_H__
//#define _SD_BASE_IO_IO_BUFF_POOL_H__
//#include "sdDynamicPool.h"
//#include "sdCriticalSection.h"
//#include "sdSingleton.h"
//
//BASE_NAMESPACE_BEGIN_BASE
//BASE_NAMESPACE_BEGIN_IO
//
//class sdIOBuff;
//class sdIOBuffPool;
//class sdIOBuffPoolGroup;
//
////-----------------------------------------------------------------------------
//// sdIOBuff
////-----------------------------------------------------------------------------
//class sdIOBuff
//{
//public:
//	sdIOBuff();
//	virtual ~sdIOBuff();
//
//	// �������ͷ�Buff������
//	void	AllocBuff(uint uiSize);
//	void	DeallocBuff();
//
//	void	AllocBuffNoLock(uint uiSize);
//	void	DeallocBuffNoLock();
//
//	// ���Է���
//	char*	GetBuff() { return m_pcBuff; }
//
//	sdIOBuffPool* GetPool() { return m_pkBuffPool;}
//	void	SetPool(sdIOBuffPool* pkPool) { m_pkBuffPool = pkPool;}
//
//protected:
//	// Buff��Ϣ
//	uint	m_uiSize;
//	char*	m_pcBuff;
//
//	// Buff����Pool
//	sdIOBuffPool*	m_pkBuffPool;
//};
//
////-----------------------------------------------------------------------------
//// IOBuff�����
////	- IOBuff Object Pool
////	- IOBuff Memory Manager
////-----------------------------------------------------------------------------
//class sdIOBuffPool : public Memory::sdTDynamicPool<sdIOBuff>
//{
//public:
//	sdIOBuffPool();
//	virtual ~sdIOBuffPool();
//
//	// 
//	void Lock(){ m_kCriticalSection.Lock();}
//	void Unlock(){ m_kCriticalSection.Unlock();}
//	bool TryLock(){ return m_kCriticalSection.TryLock();}
//
//	// 
//	sdIOBuff*	AllocObject();
//	void		DeallocObject(sdIOBuff* pkIOBuff);
//
//	//
//	char*	AllocBuff(size_t uiSize);
//	void	DeallocBuff(void* pkData);
//
//protected:
//	struct stBuffHeader
//	{
//		stBuffHeader*	prev_ptr;
//		stBuffHeader* next_ptr;
//		size_t	freeBytes;
//		size_t	totalBytes;
//		size_t	allocCount;
//		char*	data_ptr;
//		char	align[64 - sizeof(size_t) * 6];
//	};
//
//protected:
//	// IO Buff Memory Pool
//	stBuffHeader*	m_pkFreeBuff;
//
//	// 
//	Base::Threading::sdCriticalSection m_kCriticalSection;
//};
//
////-----------------------------------------------------------------------------
//// IOBuff�������
////-----------------------------------------------------------------------------
//class sdIOBuffPoolGroup : sdTSingleton<sdIOBuffPoolGroup>
//{
//public:
//	sdIOBuffPoolGroup();
//	~sdIOBuffPoolGroup();
//
//	// ������黹IOBuff
//	sdIOBuff*	Alloc(uint uiSize);
//	void		Dealloc(sdIOBuff* pkIOBuff);
//
//protected:
//	// Ŀǰ���������߳�(Mesh,Texture),���߳�Ҳ��IO
//	// ����ʵ����,�����������̸߳�CPU�����������
//	sdIOBuffPool* m_kIOBuffPool[2];
//};
////-----------------------------------------------------------------------------
//BASE_NAMESPACE_END_BASE
//BASE_NAMESPACE_END_IO
//#endif