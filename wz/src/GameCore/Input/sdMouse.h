//*************************************************************************************************
// 内容:	鼠标输入
//---------------------------------------------------------
// 作者:		
// 创建:		2012-07-23
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMECORE_INPUT_MOUSE_H__
#define _SD_GAMECORE_INPUT_MOUSE_H__

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

#include <dinput.h>

SD_GAMECORE_NAMESPACE_BEGIN_GAMECORE

// 鼠标按键
enum eMouseButton
{
	E_MOUSE_LBUTTON,
	E_MOUSE_RBUTTON,
	E_MOUSE_MBUTTON,
};

class sdMouse : public NiRefObject
{
public:
	sdMouse();
	~sdMouse();

	// 设备初始化/更新/销毁
	bool	Initialize(LPDIRECTINPUT8 lpInputDevice, HWND hWnd);
	void	Update();
	void	Destroy();

	// 输入状态检测
	bool	IsButtonDown(eMouseButton eButton) const;
	bool	IsButtonPressed(eMouseButton eButton) const;
	bool	IsButtonReleased(eMouseButton eButton) const;

	void	SetPositionDelta(int iX, int iY, int iZ);
	void	GetPositionDelta(int& iX, int& iY, int& iZ);
	
private:
	LPDIRECTINPUTDEVICE8	m_lpMouseDevice;
	DIMOUSESTATE			m_eState;
	DIMOUSESTATE			m_eLastState;
};

SD_GAMECORE_NAMESPACE_END_GAMECORE
#endif