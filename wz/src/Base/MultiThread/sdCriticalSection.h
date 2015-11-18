//*************************************************************************************************
// 内容:		临界区(参考自Gamebryo::NiCriticalSection)
//
// 参考资料：
//	http://www.cnblogs.com/yangchaobj/archive/2013/02/08/2909459.html
//
//---------------------------------------------------------
// 作者:		
// 创建:		2012-07-16
// 最后修改:	2012-02-04
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_THREADING_CRITICAL_SECTION_H__
#define _SD_BASE_THREADING_CRITICAL_SECTION_H__

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_THREADING
//-----------------------------------------------------------------------------
// Win32临界区(参考自GB)
//-----------------------------------------------------------------------------
class sdCriticalSection
{
public:
	sdCriticalSection()
	{
#if defined(SD_MULTITHREADED)
		m_ulThreadOwner = 0;
		m_uiLockCount = 0;
		::InitializeCriticalSection(&m_kCriticalSection);
#endif
	}

	~sdCriticalSection()
	{
#if defined(SD_MULTITHREADED)
		::DeleteCriticalSection(&m_kCriticalSection);
#endif
	}

	void Lock()
	{
#if defined(SD_MULTITHREADED)
		::EnterCriticalSection(&m_kCriticalSection);
		m_ulThreadOwner = ::GetCurrentThreadId();
		m_uiLockCount++;
#endif
	}

	void Unlock()
	{
#if defined(SD_MULTITHREADED)
		assert(m_uiLockCount > 0);
		assert(m_ulThreadOwner == ::GetCurrentThreadId());
		m_uiLockCount--;
		if (m_uiLockCount == 0)
			m_ulThreadOwner = 0;
		::LeaveCriticalSection(&m_kCriticalSection);
#endif
	}

	bool TryLock()
	{
#if defined(SD_MULTITHREADED)
		if (::TryEnterCriticalSection(&m_kCriticalSection) == TRUE)
		{
			m_ulThreadOwner = ::GetCurrentThreadId();
			m_uiLockCount++;

			return true;
		}

		return false;
#else
		return true;
#endif
	}

	ulong GetOwningThreadID() const
	{
#if defined(SD_MULTITHREADED)
		return m_ulThreadOwner;
#else
		return 0;
#endif
	}

	uint GetCurrentLockCount() const
	{
#if defined(SD_MULTITHREADED)
		return m_uiLockCount;
#else
		return 0;
#endif
	}

protected:
#if defined(SD_MULTITHREADED)
	CRITICAL_SECTION	m_kCriticalSection;
	ulong				m_ulThreadOwner;	///< The ID of the last thread to lock the critical section
	uint				m_uiLockCount;		///< The count of how many times the critical section has been locked by the same thread
#endif

};
//-----------------------------------------------------------------------------
// Win32临界区(参考自GB)
//-----------------------------------------------------------------------------
class sdFastCriticalSection
{
public:
	sdFastCriticalSection()
	{
#if defined(SD_MULTITHREADED)
		::InitializeCriticalSection(&m_kCriticalSection);

#if defined(_DEBUG)
		m_bLocked = false;
#endif

#endif
	}

	~sdFastCriticalSection()
	{
#if defined(SD_MULTITHREADED)
		::DeleteCriticalSection(&m_kCriticalSection);
#endif
	}

	void Lock()
	{
#if defined(SD_MULTITHREADED)
		::EnterCriticalSection(&m_kCriticalSection);

#if defined _DEBUG
		assert(m_bLocked == false);
		m_bLocked = true;
#endif

#endif
	}

	void Unlock()
	{
#if defined(SD_MULTITHREADED)
#if defined _DEBUG
		assert(m_bLocked == true);
		m_bLocked = false;
#endif

		::LeaveCriticalSection(&m_kCriticalSection);
#endif
	}

	bool TryLock()
	{
#if defined(SD_MULTITHREADED)
		if (::TryEnterCriticalSection(&m_kCriticalSection) == TRUE)
		{
#if defined _DEBUG
			assert(m_bLocked == false);
			m_bLocked = true;
#endif
			return true;
		}

		return false;
#else
		return true;
#endif
	}

protected:
#if defined(SD_MULTITHREADED)
	CRITICAL_SECTION m_kCriticalSection;

#if defined _DEBUG
	bool m_bLocked;	///< 当前是否锁定
#endif	

#endif
};
//-----------------------------------------------------------------------------
// Win32临界区使用(参考自WZ)
//-----------------------------------------------------------------------------
//class sdAutoCriticalSection
//{
//public:
//	sdAutoCriticalSection(CRITICAL_SECTION* pkCriticalSection) 
//		: m_pkCriticalSection(pkCriticalSection)
//	{
//		::InitializeCriticalSection(m_pkCriticalSection);
//	}
//
//	~sdAutoCriticalSection()
//	{
//		::DeleteCriticalSection(m_pkCriticalSection);
//	}
//
//protected:
//	CRITICAL_SECTION*	m_pkCriticalSection;
//};
//-----------------------------------------------------------------------------
BASE_NAMESPACE_END_THREADING
BASE_NAMESPACE_END_BASE
#endif