//*************************************************************************************************
// 内容:	Win32 GDI的一些功能
//---------------------------------------------------------
// 作者:		
// 创建:		2012-07-09
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_GDI_UTIL_H__
#define _SD_BASE_GDI_UTIL_H__
#include "sdBase.h"

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_UTIL

class sdGDIUtil
{
public:
	// 获取屏幕尺寸
	// GetSystemMetrics()
	// GetDeviceCaps()
	// SystemParametersInfo()
	// EnumDisplaySettings()
	// GetWindowRect()
	static GetScreenInfo(unsigned int uiWidth, unsigned int uiHeight, bool bFullScreen)
	{
		HDC hDC = ::GetDC(NULL);
		uiWidth = ::GetDeviceCaps(hDC, HORZRES);
		uiHeight = ::GetDeviceCaps(hDC, VERTRES);
		::ReleaseDC(NULL, hDC);
	}
};

BASE_NAMESPACE_END_BASE
BASE_NAMESPACE_END_UTIL
#endif