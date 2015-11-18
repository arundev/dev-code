//*************************************************************************************************
// ����:	�����ڴ��,�̲߳���ȫ 
//---------------------------------------------------------
// ����:		
// ����:		2012-12-07
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMECORE_DYNAMIC_POOL_H__
#define _SD_GAMECORE_DYNAMIC_POOL_H__

SD_GAMECORE_NAMESPACE_BEGIN_GAMECORE

template <class T>
class TDynamicPool
{
public:
	explicit TDynamicPool()
		: m_uiInitCount(0)
		, m_uiGrowCount(0)
		, m_uiCurCount(0)
	{
	}

	virtual ~TDynamicPool()
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
			NiDelete [] *itr;
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
		T* pkT = NiNew T[uiCount];
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

SD_GAMECORE_NAMESPACE_END_GAMECORE
#endif