//*************************************************************************************************
// 内容:	信号量
//---------------------------------------------------------
// 作者:		
// 创建:		2013-02-04
// 最后修改:	2013-02-27
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_THREADING_SEMAPHORE_H__
#define _SD_BASE_THREADING_SEMAPHORE_H__

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_THREADING
//-----------------------------------------------------------------------------
// Win32信号量(参考自WZ)
//-----------------------------------------------------------------------------
class sdSemaphore
{
public:
	sdSemaphore(long lCount = 0)
	{
#if defined(SD_MULTITHREADED)
		m_hSemaphore = ::CreateSemaphore(0, lCount, 0x7fffffff, 0);
#endif
	}

	~sdSemaphore()
	{
#if defined(SD_MULTITHREADED)
		::CloseHandle(m_hSemaphore);
#endif
	}

	void Signal(long lCount = 1) const
	{
#if defined(SD_MULTITHREADED)
		::ReleaseSemaphore(m_hSemaphore, lCount, 0);
#endif
	}

	void Wait() const
	{
#if defined(SD_MULTITHREADED)
		::WaitForSingleObject(m_hSemaphore, INFINITE);
#endif
	}

	bool Wait(const uint uiTimeOut) const
	{
#if defined(SD_MULTITHREADED)
		DWORD dwCode = ::WaitForSingleObject(m_hSemaphore, uiTimeOut);
		return uiTimeOut != WAIT_TIMEOUT;
#else
		return true;
#endif
	}

protected:
	HANDLE m_hSemaphore;
};
//-----------------------------------------------------------------------------
// Win32信号量(参考自GB)
//-----------------------------------------------------------------------------
//class sdSemaphore
//{
//public:
//	sdSemaphore() 
//		: m_iCount(0), m_iMaxCount(1)
//	{
//		m_hSemaphore = ::CreateSemaphore(0, m_iCount, m_iMaxCount, 0);
//		assert(m_hSemaphore)
//	}
//
//	sdSemaphore(int iCount)
//		: m_iCount(iCount), m_iMaxCount(iCount + 1)
//	{
//		m_hSemaphore = ::CreateSemaphore(0, m_iCount, m_iMaxCount, 0);
//		assert(m_hSemaphore)
//	}
//
//	sdSemaphore(int iCount, int iMaxCount)
//		: m_iCount(iCount), m_iMaxCount(iMaxCount)
//	{
//		m_hSemaphore = ::CreateSemaphore(0, m_iCount, m_iMaxCount, 0);
//		assert(m_hSemaphore)
//	}
//
//	~sdSemaphore()
//	{
//		::CloseHandle(m_hSemaphore);
//	}
//
//	int Signal()
//	{
//		::ReleaseSemaphore(m_hSemaphore, 1, 0)
//		return ::InterlockedIncrement((LONG*)&m_iCount);
//	}
//
//	int Wait()
//	{
//		::WaitForSingleObject(m_hSemaphore, INFINITE);
//		return ::InterlockedDecrement((LONG*)&m_iCount);
//	}
//
//	int GetCount() const
//	{
//		return m_iCount;
//	}
//
//	int GetMaxCount() const
//	{
//		return m_iMaxCount;
//	}
//
//protected:
//	HANDLE			m_hSemaphore;
//	volatile int	m_iCount;
//	int				m_iMaxCount;
//};
//-----------------------------------------------------------------------------
BASE_NAMESPACE_END_THREADING
BASE_NAMESPACE_END_BASE
#endif