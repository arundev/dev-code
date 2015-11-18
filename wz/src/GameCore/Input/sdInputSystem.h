//*************************************************************************************************
// 内容:	游戏输入系统
//			这里是使用的DirectInput来主动获取输入(考虑SDL封装的DInput)
//			(dinput.h,winuser.h)
//			(目前包括键盘,鼠标,以后可以扩展到游戏杆甚至于各种传感器与反馈器)
//				1.DirectInput直接与驱动进行交互,可以有直接和缓存两种输入方式
//				2.Windows消息系统只能用缓存方式
//
//			考虑添加被动消息注入状态以适应编辑器等
//---------------------------------------------------------
// 作者:		
// 创建:		2012-07-23
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMECORE_INPUT_SYSTEM_H__
#define _SD_GAMECORE_INPUT_SYSTEM_H__
#include <sdSingleton.h>
#include "sdMouse.h"
#include "sdKeyBoard.h"
#include "sdInputKey.h"

SD_GAMECORE_NAMESPACE_BEGIN_GAMECORE

// 键盘映射
struct sdKeyInfo
{
	sdKeyInfo()
	{
		eKeyCat		= E_KC_OTHER;
		eInputKey	= E_INPUTKEY_INVALID;
		usMainDIK	= sdInvalidCombDIK;
		usAsstDIK	= sdInvalidCombDIK;
		bCanModify	= false;
		szDescInfo[0]	= '\0';
	}

	sdEKeyCategory	eKeyCat;	// 逻辑键类型
	sdEInputKey		eInputKey;	// 逻辑键(物理键无关)
	unsigned short	usMainDIK;	// 低2位物理主键,高2位物理控制键(Ctrl/Shift/Alt)
	unsigned short	usAsstDIK;	// 低2位物理辅助键,高2位物理辅助控制键
	bool			bCanModify;	// 是否允许用户重定义
	char			szDescInfo[64];	// 逻辑键描述
};

// 输入系统,每帧主动获取设备状态并缓存,支持键盘映射
class sdInputSystem : public Base::sdTSingleton<sdInputSystem>
{
public:
	sdInputSystem();
	~sdInputSystem();

	// 设备初始化/更新/销毁
	bool	Initialize(HINSTANCE hInstance, HWND hWnd);
	void	Update();
	void	Destroy();

	// 激活状态
	void	SetActive(bool bActive);
	bool	GetActive();

	// 鼠标状态检测
	// @{
	bool	IsButtonDown(eMouseButton eButton) const;
	bool	IsButtonPressed(eMouseButton eButton) const;
	bool	IsButtonReleased(eMouseButton eButton) const;
	void	SetPositionDelta(int iX, int iY, int iZ);
	void	GetPositionDelta(int& iX, int& iY, int& iZ);
	// @}

	// 键盘状态检测
	// @{
	// 用户键检测
	bool	IsInputKeyDown(sdEInputKey eInputKey);
	bool	IsInputKeyPressed(sdEInputKey eInputKey);
	bool	IsInputKeyReleased(sdEInputKey eInputKey);

	// 物理键检测
	bool	IsShiftKeyDown() const;
	bool	IsCtrlKeyDown() const;
	bool	IsAltKeyDown() const;

	unsigned char	IsAnyDIKKeyDown() const;
	unsigned char	IsAnyDIKKeyPressed() const;

	bool	IsDIKKeyDown(unsigned char ucDIKKey) const;
	bool	IsDIKKeyPressed(unsigned char ucDIKKey) const;
	bool	IsDIKKeyReleased(unsigned char ucDIKKey) const;
	// @}

protected:
	// 添加缺省逻辑键-物理键映射
	void MakeDiskStringMapping();
	void MakeDefaultKeyMapping();

	// 添加用户键-物理键映射
	void AddInputKey(const char* szDesc, sdEInputKey eInputKey, 
		unsigned short usMainDlk, unsigned short usAsstDlk, bool bFlag, sdEKeyCategory eCat);

	// 检查输入键映射的物理组合键中的物理辅助键(Ctrl/Alt/Shift)是否按下
	bool CheckAssistDIKKey(unsigned short usDIKCombineKey);

protected:
	bool				m_bInitialized;		// 初始化
	bool				m_bActive;

	HWND				m_hWnd;				// 窗口句柄
	IDirectInput8*		m_lpInputDevice;	// D3D输入设备
	sdMouse				m_kMouse;
	sdKeyboard			m_kKeyboard;

	// 虚拟键-物理键映射
	short int	m_sVK2DIKMapping[256];
	char*		m_kDIKString[256];

	// 用户键-物理键映射(缺省映射和用户定义映射)
	typedef std::map<sdEInputKey, sdKeyInfo> KeyMap;
	typedef std::map<sdEInputKey, sdKeyInfo>::iterator	KeyMapItr;
	KeyMap	m_kDefaultMapping;
	KeyMap	m_kUserMapping;
};

SD_GAMECORE_NAMESPACE_END_GAMECORE
#endif