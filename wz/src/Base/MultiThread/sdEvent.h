//*************************************************************************************************
// 内容:	事件
//---------------------------------------------------------
// 作者:		
// 创建:		2013-02-27
// 最后修改:	2013-02-27
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_THREADING_EVENT_H__
#define _SD_BASE_THREADING_EVENT_H__

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_THREADING
//-----------------------------------------------------------------------------
// Win32事件(参考自Cry)
//-----------------------------------------------------------------------------
class sdEvent
{
public:
	sdEvent()
	{
#if defined(SD_MULTITHREADED)
		m_hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
#endif
	}

	~sdEvent()
	{
#if defined(SD_MULTITHREADED)
		::CloseHandle(m_hEvent);
#endif
	}

	// Reset the event to the unsignalled state.
	void Reset()
	{
#if defined(SD_MULTITHREADED)
		::ResetEvent(m_hEvent);
#endif
	}

	// Set the event to the signalled state.
	void Set()
	{
#if defined(SD_MULTITHREADED)
		::SetEvent(m_hEvent);
#endif
	}

	// Wait indefinitely for the object to become signalled.
	void Wait() const
	{
#if defined(SD_MULTITHREADED)
		::WaitForSingleObject(m_hEvent, INFINITE);
#endif
	}

	// Wait, with a time limit, for the object to become signalled. 
	bool Wait(const uint uiTimeOut) const
	{
#if defined(SD_MULTITHREADED)
		DWORD dwCode = ::WaitForSingleObject(m_hEvent, uiTimeOut);
		return uiTimeOut != WAIT_TIMEOUT;
#else
		return true
#endif
	}

protected:
#if defined(SD_MULTITHREADED)
	HANDLE m_hEvent;
#endif
};
//-----------------------------------------------------------------------------
BASE_NAMESPACE_END_THREADING
BASE_NAMESPACE_END_BASE
#endif