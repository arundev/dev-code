////*************************************************************************************************
//// 内容:	文件缓存对象缓存池
////	
//// 1.根据Windows Platform SDK Debug Tool(gflags)测试, 游戏或编辑器在运行一段时间之后崩溃,
////	是因为大量的IO操作,且每次IO都会分配释放一大块连续内存,导致最后2G内存空间存在大量碎片,
////	最后触发C++内存分配异常返回空指针,导致不可预知行为
//// 2.sdIOBuff是基于单文件和NiFile的基础上的
//// 3.sdIOBuff只对IO Read做了处理,没有处理IO Write
////---------------------------------------------------------
//// 作者:		
//// 创建:		2012-10-15
//// 最后修改:
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
//	// 分配与释放Buff数据区
//	void	AllocBuff(uint uiSize);
//	void	DeallocBuff();
//
//	void	AllocBuffNoLock(uint uiSize);
//	void	DeallocBuffNoLock();
//
//	// 属性访问
//	char*	GetBuff() { return m_pcBuff; }
//
//	sdIOBuffPool* GetPool() { return m_pkBuffPool;}
//	void	SetPool(sdIOBuffPool* pkPool) { m_pkBuffPool = pkPool;}
//
//protected:
//	// Buff信息
//	uint	m_uiSize;
//	char*	m_pcBuff;
//
//	// Buff所属Pool
//	sdIOBuffPool*	m_pkBuffPool;
//};
//
////-----------------------------------------------------------------------------
//// IOBuff对象池
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
//// IOBuff对象池组
////-----------------------------------------------------------------------------
//class sdIOBuffPoolGroup : sdTSingleton<sdIOBuffPoolGroup>
//{
//public:
//	sdIOBuffPoolGroup();
//	~sdIOBuffPoolGroup();
//
//	// 分配与归还IOBuff
//	sdIOBuff*	Alloc(uint uiSize);
//	void		Dealloc(sdIOBuff* pkIOBuff);
//
//protected:
//	// 目前两个加载线程(Mesh,Texture),主线程也有IO
//	// 但是实际上,真正并发的线程跟CPU核心数量相关
//	sdIOBuffPool* m_kIOBuffPool[2];
//};
////-----------------------------------------------------------------------------
//BASE_NAMESPACE_END_BASE
//BASE_NAMESPACE_END_IO
//#endif