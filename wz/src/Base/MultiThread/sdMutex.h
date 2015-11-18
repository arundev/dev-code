//*************************************************************************************************
// ����:	������
//---------------------------------------------------------
// ����:		
// ����:		2013-02-27
// ����޸�:	2013-02-27
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_THREADING_MUTEX_H__
#define _SD_BASE_THREADING_MUTEX_H__

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_THREADING
//-----------------------------------------------------------------------------
// Win32������(�ο���Cry)
//-----------------------------------------------------------------------------
class sdMutex
{
public:
	sdMutex()
	{
#if defined(SD_MULTITHREADED)
		m_hMutex = ::CreateMutex(NULL, FALSE, NULL);
#endif
	}

	~sdMutex()
	{
#if defined(SD_MULTITHREADED)
		::CloseHandle(m_hMutex);
#endif
	}

	void Lock()
	{
#if defined(SD_MULTITHREADED)
		::WaitForSingleObject(m_hMutex, INFINITE);
#endif
	}

	void Unlock()
	{
#if defined(SD_MULTITHREADED)
		::ReleaseMutex(m_hMutex);
#endif
	}

	bool TryLock()
	{
#if defined(SD_MULTITHREADED)
		return ::WaitForSingleObject(m_hMutex, 0) != WAIT_TIMEOUT;
#else
		return true;
#endif
	}

protected:
#if defined(SD_MULTITHREADED)
	HANDLE m_hMutex;
#endif
};
//-----------------------------------------------------------------------------
BASE_NAMESPACE_END_THREADING
BASE_NAMESPACE_END_BASE
#endif