//*************************************************************************************************
// ����:	Win32 GDI��һЩ����
//---------------------------------------------------------
// ����:		
// ����:		2012-07-09
// ����޸�:
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
	// ��ȡ��Ļ�ߴ�
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