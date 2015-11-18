//*************************************************************************************************
// ����:	�ڴ���󻺴��
//---------------------------------------------------------
// ����:		
// ����:		2012-10-15
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_MEMORY_DYNAMIC_POOL_H__
#define _SD_BASE_MEMORY_DYNAMIC_POOL_H__

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_MEMORY

// �ڴ�����,�̲߳���ȫ 
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

	// �Ӷ���ط������
	T* Alloc()
	{
		T* pkT = NULL;

		// ��Pool���ù�,���Դ�Stack�������,����Pool
		if (m_kFreeList.empty() && !_Alloc(m_uiGrowCount))
		{
			return NULL;
		}

		// ��Pool�������
		pkT = m_kFreeList.front();
		m_kFreeList.pop_front();

		return pkT;
	}

	// �黹����������
	void Dealloc(T* pkT)
	{
		m_kFreeList.push_back(pkT);
	}

	//
	size_t GetFreeCount() const { return m_kFreeList.size();}
	size_t GetAllCount() const { return m_uiCurCount;}

protected:
	// �Ӷѷ���ָ����Ŀ�����������
	virtual bool _Alloc(uint uiCount)
	{
		if (!m_kFreeList.empty())
		{
			return true;
		}

		// ����һ�����
		T* pkT = new T[uiCount];
		if (pkT == NULL)
		{
			return false;
		}

		// �������
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