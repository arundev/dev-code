//*************************************************************************************************
// 内容:	Win32 Cursor的一些功能封装
//---------------------------------------------------------
// 作者:		
// 创建:		2012-07-13
// 最后修改:	2012-12-31
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_UTIL_CURSOR_UTIL_H__
#define _SD_BASE_UTIL_CURSOR_UTIL_H__

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_UTIL

// 用来封装Windows对Cursor的一些简单操作
class sdCursorUtil
{
public:
	// 加载系统光标
	static HCURSOR LoadCursor(HINSTANCE hHandle, LPCSTR lpCursorName)
	{
		return ::LoadCursor(NULL, lpCursorName);
	}

	// 从文件加载光标
	static HCURSOR LoadCursor(const char* szFileName)
	{
		return ::LoadCursorFromFile(szFileName);
	}

	// 销毁光标
	static void DestroyCursor(HCURSOR hCursor)
	{
		::DestroyCursor(hCursor);
	}

	// 设置当前光标
	static void SetCursor(HCURSOR hCursor)
	{
		::SetCursor(hCursor);
	}

	// 设置光标是否显示
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