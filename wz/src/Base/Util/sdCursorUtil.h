//*************************************************************************************************
// ����:	Win32 Cursor��һЩ���ܷ�װ
//---------------------------------------------------------
// ����:		
// ����:		2012-07-13
// ����޸�:	2012-12-31
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_UTIL_CURSOR_UTIL_H__
#define _SD_BASE_UTIL_CURSOR_UTIL_H__

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_UTIL

// ������װWindows��Cursor��һЩ�򵥲���
class sdCursorUtil
{
public:
	// ����ϵͳ���
	static HCURSOR LoadCursor(HINSTANCE hHandle, LPCSTR lpCursorName)
	{
		return ::LoadCursor(NULL, lpCursorName);
	}

	// ���ļ����ع��
	static HCURSOR LoadCursor(const char* szFileName)
	{
		return ::LoadCursorFromFile(szFileName);
	}

	// ���ٹ��
	static void DestroyCursor(HCURSOR hCursor)
	{
		::DestroyCursor(hCursor);
	}

	// ���õ�ǰ���
	static void SetCursor(HCURSOR hCursor)
	{
		::SetCursor(hCursor);
	}

	// ���ù���Ƿ���ʾ
	static void SafeShowCursor(bool bShow)
	{
		if (bShow)
		{
			while (::ShowCursor(TRUE) < 0);
		}
		else
		{
			while (::ShowCursor(FALSE) >= 0);
		}
	}
};
BASE_NAMESPACE_END_UTIL
BASE_NAMESPACE_END_BASE
#endif