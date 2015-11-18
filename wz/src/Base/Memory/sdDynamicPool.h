//*************************************************************************************************
// 内容:	内存对象缓存池
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-15
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_MEMORY_DYNAMIC_POOL_H__
#define _SD_BASE_MEMORY_DYNAMIC_POOL_H__

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_MEMORY

// 内存对象池,线程不安全 
template <typename T>
class sdTDynamicPool
{
public:
	explicit sdTDynamicPool()
		: m_uiInitCount(0), m_uiGrowCount(0), m_uiCurCount(0)
	{
	}

	virtual ~sdTDynamicPool()
	{
	}

	bool Initialize(uint uiInitCount, uint uiGrowCount)
	{
		m_uiInitCount = uiInitCount;
		m_uiGrowCount = uiGrowCount;

		return _Alloc(uiInitCount);
	}

	void Destroy()
	{
		typename std::list<T*>::iterator itr = m_kFreeList.begin();
		typename std::list<T*>::iterator itr_end = m_kFreeList.end();
		for (; itr != itr_end; ++itr)
		{
			*itr = NULL;
		}
		m_kFreeList.clear();

		itr = m_kAllList.begin();
		itr_end = m_kAllList.end();
		for (; itr != itr_end; ++itr)
		{
			delete [] *itr;
			*itr = NULL;
		}
		m_kAllList.clear();
	}

	// 从对象池分配对象
	T* Alloc()
	{
		T* pkT = NULL;

		// 若Pool已用光,则尝试从Stack分配对象,加入Pool
		if (m_kFreeList.empty() && !_Alloc(m_uiGrowCount))
		{
			return NULL;
		}

		// 从Pool分配对象
		pkT = m_kFreeList.front();
		m_kFreeList.pop_front();

		return pkT;
	}

	// 归还对象给对象池
	void Dealloc(T* pkT)
	{
		m_kFreeList.push_back(pkT);
	}

	//
	size_t GetFreeCount() const { return m_kFreeList.size();}
	size_t GetAllCount() const { return m_uiCurCount;}

protected:
	// 从堆分配指定数目对象加入对象池
	virtual bool _Alloc(uint uiCount)
	{
		if (!m_kFreeList.empty())
		{
			return true;
		}

		// 分配一组对象
		T* pkT = new T[uiCount];
		if (pkT == NULL)
		{
			return false;
		}

		// 对象入池
		for (uint i = 0; i < uiCount; ++i)
		{
			m_kFreeList.push_back(&pkT[i]);
		}
		m_kAllList.push_back(pkT);

		m_uiCurCount += uiCount;

		return true;
	}

protected:
	std::list<T*> m_kFreeList;
	std::list<T*> m_kAllList;

	uint m_uiInitCount;
	uint m_uiGrowCount;
	uint m_uiCurCount;
};

BASE_NAMESPACE_END_MEMORY
BASE_NAMESPACE_END_BASE
#endif