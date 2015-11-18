//*************************************************************************************************
// 内容:	键盘输入
//---------------------------------------------------------
// 作者:		
// 创建:		2012-07-23
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMECORE_INPUT_KEYBOARD_H__
#define _SD_GAMECORE_INPUT_KEYBOARD_H__

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

#include <dinput.h>

SD_GAMECORE_NAMESPACE_BEGIN_GAMECORE

class sdKeyboard : public NiRefObject
{
public:
	sdKeyboard();
	~sdKeyboard();

	// 设备初始化/更新/销毁
	bool	Initialize(LPDIRECTINPUT8 lpInputDevice, HWND hWnd);
	void	Update();
	void	Destroy();

	// 输入状态检测
	unsigned char	IsAnyKeyDown() const;
	unsigned char	IsAnyKeyPressed() const;
	bool			IsKeyDown(unsigned char uiKey) const;
	bool			IsKeyPressed(unsigned char uiKey) const;
	bool			IsKeyReleased(unsigned char uiKey) const;
	
private:
	LPDIRECTINPUTDEVICE8	m_lpKeyboardDevice;
	unsigned char			m_ucKeyState[256];
	unsigned char			m_ucLastKeyState[256];
};

SD_GAMECORE_NAMESPACE_END_GAMECORE
#endif