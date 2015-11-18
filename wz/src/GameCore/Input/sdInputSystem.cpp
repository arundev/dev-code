#include "sdGameCorePCH.h"
#include "sdInputSystem.h"
#include <sdStaticAssert.h>

using namespace Base::Diagnostics;

SD_GAMECORE_NAMESPACE_BEGIN_GAMECORE
//-------------------------------------------------------------------------------------------------
sdInputSystem::sdInputSystem()
: m_bInitialized(false)
, m_bActive(true)
, m_hWnd(0)
, m_lpInputDevice(NULL)
{

}

sdInputSystem::~sdInputSystem()
{
	if (m_bInitialized)
	{
		Destroy();
	}
}
//-------------------------------------------------------------------------------------------------
bool sdInputSystem::Initialize(HINSTANCE hInstance, HWND hWnd)
{
	if (m_bInitialized)
	{
		NIASSERT(0);
		return false;
	}

	// 检查NUM_INPUTKEYS(大于128造成编译错误)
	sdStaticAssertLess<NUM_INPUTKEYS, 128>();

	//
	m_hWnd = hWnd;

	// 创建D3D输入设备
	HRESULT hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_lpInputDevice, NULL);
	if (FAILED(hr))	return false;

	// 初始化鼠标与键盘
	m_kMouse.Initialize(m_lpInputDevice, hWnd);
	m_kKeyboard.Initialize(m_lpInputDevice, m_hWnd);

	//
	MakeDefaultKeyMapping();
	MakeDiskStringMapping();

	return (m_bInitialized = true);
}
//-------------------------------------------------------------------------------------------------
void sdInputSystem::Destroy()
{
	if (m_bInitialized)
	{
		m_kKeyboard.Destroy();
		m_kMouse.Destroy();

		if (m_lpInputDevice)
		{
			m_lpInputDevice->Release();
			m_lpInputDevice = NULL;
		}

		m_bInitialized = false;
	}
}
//-------------------------------------------------------------------------------------------------
void sdInputSystem::Update()
{
	if (m_bInitialized)
	{
		m_kMouse.Update();
		m_kKeyboard.Update();
	}
}
//-------------------------------------------------------------------------------------------------
void sdInputSystem::MakeDiskStringMapping()
{
	for (int i = 0; i < 256; ++i)
		m_kDIKString[i] = "";

	// @{
	//
	m_kDIKString[DIK_1]				= "1";
	m_kDIKString[DIK_2]				= "2";
	m_kDIKString[DIK_3]				= "3";
	m_kDIKString[DIK_4]				= "4";
	m_kDIKString[DIK_5]				= "5";
	m_kDIKString[DIK_6]				= "6";
	m_kDIKString[DIK_7]				= "7";
	m_kDIKString[DIK_8]				= "8";
	m_kDIKString[DIK_9]				= "9";
	m_kDIKString[DIK_0]				= "0";

	m_kDIKString[DIK_Q]				= "Q";
	m_kDIKString[DIK_W]				= "W";
	m_kDIKString[DIK_E]				= "E";
	m_kDIKString[DIK_R]				= "R";
	m_kDIKString[DIK_T]				= "T";
	m_kDIKString[DIK_Y]				= "Y";
	m_kDIKString[DIK_U]				= "U";
	m_kDIKString[DIK_I]				= "I";
	m_kDIKString[DIK_O]				= "O";
	m_kDIKString[DIK_P]				= "P";

	m_kDIKString[DIK_A]				= "A";
	m_kDIKString[DIK_S]				= "S";
	m_kDIKString[DIK_D]				= "D";
	m_kDIKString[DIK_F]				= "F";
	m_kDIKString[DIK_G]				= "G";
	m_kDIKString[DIK_H]				= "H";
	m_kDIKString[DIK_J]				= "J";
	m_kDIKString[DIK_K]				= "K";
	m_kDIKString[DIK_L]				= "L";

	m_kDIKString[DIK_Z]				= "Z";
	m_kDIKString[DIK_X]				= "X";
	m_kDIKString[DIK_C]				= "C";
	m_kDIKString[DIK_V]				= "V";
	m_kDIKString[DIK_B]				= "B";
	m_kDIKString[DIK_N]				= "N";
	m_kDIKString[DIK_M]				= "M";

	//
	m_kDIKString[DIK_GRAVE]			= "`";
	m_kDIKString[DIK_MINUS]			= "-";
	m_kDIKString[DIK_EQUALS]		= "=";
	m_kDIKString[DIK_BACK]			= "Backspace";
	m_kDIKString[DIK_TAB]			= "Tab";
	m_kDIKString[DIK_LBRACKET]		= "[";
	m_kDIKString[DIK_RBRACKET]		= "]";
	m_kDIKString[DIK_RETURN]		= "Enter";
	m_kDIKString[DIK_BACKSLASH]		= "\\";
	m_kDIKString[DIK_CAPITAL]		= "Caps";
	m_kDIKString[DIK_SEMICOLON]		= ";";
	m_kDIKString[DIK_APOSTROPHE]	= "'";
	m_kDIKString[DIK_COMMA]			= ",";
	m_kDIKString[DIK_PERIOD]		= ".";
	m_kDIKString[DIK_SLASH]			= "/";

	m_kDIKString[DIK_LCONTROL]		= "LCtrl";
	m_kDIKString[DIK_RCONTROL]		= "RCtrl";
	m_kDIKString[DIK_LSHIFT]		= "LShift";
	m_kDIKString[DIK_RSHIFT]		= "RShift";
	m_kDIKString[DIK_LALT]			= "LAlt";
	m_kDIKString[DIK_RALT]			= "RAlt";
	m_kDIKString[DIK_SPACE]			= "Space";
	m_kDIKString[DIK_LWIN]			= "LWin";
	m_kDIKString[DIK_RWIN]			= "RWin";

	//
	m_kDIKString[DIK_ESCAPE]		= "ESC";
	m_kDIKString[DIK_F1]			= "F1";
	m_kDIKString[DIK_F2]			= "F2";
	m_kDIKString[DIK_F3]			= "F3";
	m_kDIKString[DIK_F4]			= "F4";
	m_kDIKString[DIK_F5]			= "F5";
	m_kDIKString[DIK_F6]			= "F6";
	m_kDIKString[DIK_F7]			= "F7";
	m_kDIKString[DIK_F8]			= "F8";
	m_kDIKString[DIK_F9]			= "F9";
	m_kDIKString[DIK_F10]			= "F10";
	m_kDIKString[DIK_F11]			= "F11";
	m_kDIKString[DIK_F12]			= "F12";
	m_kDIKString[DIK_F13]			= "F13";
	m_kDIKString[DIK_F14]			= "F14";
	m_kDIKString[DIK_F15]			= "F15";

	//
	m_kDIKString[DIK_SYSRQ]			= "PrintScreen";
	m_kDIKString[DIK_SCROLL]		= "ScrollLock";
	m_kDIKString[DIK_PAUSE]			= "Pause";

	m_kDIKString[DIK_HOME]			= "Home";
	m_kDIKString[DIK_END]			= "End";
	m_kDIKString[DIK_PRIOR]			= "PageUp";
	m_kDIKString[DIK_NEXT]			= "PageDown";
	m_kDIKString[DIK_INSERT]		= "Insert";
	m_kDIKString[DIK_DELETE]		= "Delete";
	m_kDIKString[DIK_UP]			= "↑";
	m_kDIKString[DIK_DOWN]			= "↓";
	m_kDIKString[DIK_LEFT]			= "←";
	m_kDIKString[DIK_RIGHT]			= "→";

	//
	m_kDIKString[DIK_NUMLOCK]		= "NumLock";
	m_kDIKString[DIK_NUMPADENTER]	= "NUMEnter";
	m_kDIKString[DIK_NUMPAD0]		= "NUM0";
	m_kDIKString[DIK_NUMPAD1]		= "NUM1";
	m_kDIKString[DIK_NUMPAD2]		= "NUM2";
	m_kDIKString[DIK_NUMPAD3]		= "NUM3";
	m_kDIKString[DIK_NUMPAD4]		= "NUM4";
	m_kDIKString[DIK_NUMPAD5]		= "NUM5";
	m_kDIKString[DIK_NUMPAD6]		= "NUM6";
	m_kDIKString[DIK_NUMPAD7]		= "NUM7";
	m_kDIKString[DIK_NUMPAD8]		= "NUM8";
	m_kDIKString[DIK_NUMPAD9]		= "NUM9";
	m_kDIKString[DIK_DECIMAL]		= "NUM.";
	m_kDIKString[DIK_ADD]			= "NUM+";
	m_kDIKString[DIK_SUBTRACT]		= "NUM-";
	m_kDIKString[DIK_DIVIDE]		= "NUM/";
	m_kDIKString[DIK_MULTIPLY]		= "NUM*";

	//
	m_kDIKString[DIK_APPS]			= "Apps";
	m_kDIKString[DIK_POWER]			= "Power";
	m_kDIKString[DIK_SLEEP]			= "Sleep";
	m_kDIKString[DIK_WAKE]			= "Wake";

	m_kDIKString[DIK_MUTE]			= "Mute";
	m_kDIKString[DIK_CALCULATOR]	= "Calculator";
	m_kDIKString[DIK_PLAYPAUSE]		= "Play/Pause";
	m_kDIKString[DIK_STOP]			= "Stop";
	m_kDIKString[DIK_VOLUMEUP]		= "Volume+";
	m_kDIKString[DIK_VOLUMEDOWN]	= "Volume-";

	m_kDIKString[DIK_WEBHOME]		= "WebHome";
	m_kDIKString[DIK_WEBSEARCH]		= "WebSearch";
	m_kDIKString[DIK_WEBFAVORITES]	= "WebFavourites";
	m_kDIKString[DIK_WEBREFRESH]	= "WebApps";
	m_kDIKString[DIK_WEBSTOP]		= "WebStop";
	m_kDIKString[DIK_WEBFORWARD]	= "WebForward";
	m_kDIKString[DIK_WEBBACK]		= "WebBack";

	m_kDIKString[DIK_MYCOMPUTER]	= "MyComputer";

	m_kDIKString[DIK_MAIL]			= "Mail";
	m_kDIKString[DIK_MEDIASELECT]	= "MediaSelect";
	// @}

	// @{
	//
	m_sVK2DIKMapping['1']			= DIK_1;
	m_sVK2DIKMapping['2']			= DIK_2;
	m_sVK2DIKMapping['3']			= DIK_3;
	m_sVK2DIKMapping['4']			= DIK_4;
	m_sVK2DIKMapping['5']			= DIK_5;
	m_sVK2DIKMapping['6']			= DIK_6;
	m_sVK2DIKMapping['7']			= DIK_7;
	m_sVK2DIKMapping['8']			= DIK_8;
	m_sVK2DIKMapping['9']			= DIK_9;
	m_sVK2DIKMapping['0']			= DIK_0;

	m_sVK2DIKMapping['Q']			= DIK_Q;
	m_sVK2DIKMapping['W']			= DIK_W;
	m_sVK2DIKMapping['E']			= DIK_E;
	m_sVK2DIKMapping['R']			= DIK_R;
	m_sVK2DIKMapping['T']			= DIK_T;
	m_sVK2DIKMapping['Y']			= DIK_Y;
	m_sVK2DIKMapping['U']			= DIK_U;
	m_sVK2DIKMapping['I']			= DIK_I;
	m_sVK2DIKMapping['O']			= DIK_O;
	m_sVK2DIKMapping['P']			= DIK_P;
	m_sVK2DIKMapping['A']			= DIK_A;
	m_sVK2DIKMapping['S']			= DIK_S;
	m_sVK2DIKMapping['D']			= DIK_D;
	m_sVK2DIKMapping['F']			= DIK_F;
	m_sVK2DIKMapping['G']			= DIK_G;
	m_sVK2DIKMapping['H']			= DIK_H;
	m_sVK2DIKMapping['J']			= DIK_J;
	m_sVK2DIKMapping['K']			= DIK_K;
	m_sVK2DIKMapping['L']			= DIK_L;
	m_sVK2DIKMapping['Z']			= DIK_Z;
	m_sVK2DIKMapping['X']			= DIK_X;
	m_sVK2DIKMapping['C']			= DIK_C;
	m_sVK2DIKMapping['V']			= DIK_V;
	m_sVK2DIKMapping['B']			= DIK_B;
	m_sVK2DIKMapping['N']			= DIK_N;
	m_sVK2DIKMapping['M']			= DIK_M;

	//
	m_sVK2DIKMapping[VK_OEM_3]		= DIK_GRAVE;		// "~`"
	m_sVK2DIKMapping[VK_OEM_MINUS]	= DIK_MINUS;		// "-"
	m_sVK2DIKMapping[VK_OEM_PLUS]	= DIK_EQUALS;		// "+="
	m_sVK2DIKMapping[VK_BACK]		= DIK_BACK;			// "Backspace"
	m_sVK2DIKMapping[VK_TAB]		= DIK_TAB;			// "Tab"
	m_sVK2DIKMapping[VK_OEM_4]		= DIK_LBRACKET;		// "[{"
	m_sVK2DIKMapping[VK_OEM_6]		= DIK_RBRACKET;		// "]}"
	m_sVK2DIKMapping[VK_OEM_5]		= DIK_BACKSLASH;	// "|\"
	m_sVK2DIKMapping[VK_CAPITAL]	= DIK_CAPITAL;		// "Caps Lock"
	m_sVK2DIKMapping[VK_OEM_1]		= DIK_SEMICOLON;	// ":;"
	m_sVK2DIKMapping[VK_OEM_7]		= DIK_APOSTROPHE;	// ""'"
	m_sVK2DIKMapping[VK_RETURN]		= DIK_RETURN;		// "Enter"
	m_sVK2DIKMapping[VK_OEM_COMMA]	= DIK_COMMA;		// "<,"
	m_sVK2DIKMapping[VK_OEM_PERIOD]	= DIK_PERIOD;		// ">."
	m_sVK2DIKMapping[VK_OEM_2]		= DIK_SLASH;		// "?/"

	m_sVK2DIKMapping[VK_LCONTROL]	= DIK_LCONTROL;		// "Left Control"
	m_sVK2DIKMapping[VK_RCONTROL]	= DIK_RCONTROL;		// "Right Control"
	m_sVK2DIKMapping[VK_LSHIFT]		= DIK_LSHIFT;		// "Left Shift"
	m_sVK2DIKMapping[VK_RSHIFT]		= DIK_RSHIFT;		// "Right Shift"
	m_sVK2DIKMapping[VK_LMENU]		= DIK_LALT;			// "Left Alt"
	m_sVK2DIKMapping[VK_RMENU]		= DIK_RALT;			// "Right Alt"
	m_sVK2DIKMapping[VK_SPACE]		= DIK_SPACE;		// "Space"
	m_sVK2DIKMapping[VK_LWIN]		= DIK_LWIN;			// "Left Win"
	m_sVK2DIKMapping[VK_RWIN]		= DIK_RWIN;			// "Right Win"

	//
	m_sVK2DIKMapping[VK_ESCAPE]		= DIK_ESCAPE;		// "ESC"
	m_sVK2DIKMapping[VK_F1]			= DIK_F1;
	m_sVK2DIKMapping[VK_F2]			= DIK_F2;
	m_sVK2DIKMapping[VK_F3]			= DIK_F3;
	m_sVK2DIKMapping[VK_F4]			= DIK_F4;
	m_sVK2DIKMapping[VK_F5]			= DIK_F5;
	m_sVK2DIKMapping[VK_F6]			= DIK_F6;
	m_sVK2DIKMapping[VK_F7]			= DIK_F7;
	m_sVK2DIKMapping[VK_F8]			= DIK_F8;
	m_sVK2DIKMapping[VK_F9]			= DIK_F9;
	m_sVK2DIKMapping[VK_F10]		= DIK_F10;
	m_sVK2DIKMapping[VK_F11]		= DIK_F11;
	m_sVK2DIKMapping[VK_F12]		= DIK_F12;
	m_sVK2DIKMapping[VK_F13]		= DIK_F13;
	m_sVK2DIKMapping[VK_F14]		= DIK_F14;
	m_sVK2DIKMapping[VK_F15]		= DIK_F15;

	//
	//m_sVK2DIKMapping[]			= DIK_SYSRQ;		// "PrScrn/SysRq"
	m_sVK2DIKMapping[VK_SCROLL]		= DIK_SCROLL;		// "Scroll Lock"
	m_sVK2DIKMapping[VK_PAUSE]		= DIK_PAUSE;		// "Pause/Break"

	m_sVK2DIKMapping[VK_HOME]		= DIK_HOME;			// "Home"
	m_sVK2DIKMapping[VK_END]		= DIK_END;			// "End"
	m_sVK2DIKMapping[VK_PRIOR]		= DIK_PRIOR;		// "PageUp"
	m_sVK2DIKMapping[VK_NEXT]		= DIK_NEXT;			// "PageDown"
	m_sVK2DIKMapping[VK_INSERT]		= DIK_INSERT;		// "Insert"
	m_sVK2DIKMapping[VK_DELETE]		= DIK_DELETE;		// "Delete"
	m_sVK2DIKMapping[VK_UP]			= DIK_UP;			// "↑";
	m_sVK2DIKMapping[VK_DOWN]		= DIK_DOWN;			// "↓";
	m_sVK2DIKMapping[VK_LEFT]		= DIK_LEFT;			// "←";
	m_sVK2DIKMapping[VK_RIGHT]		= DIK_RIGHT;		// "→";

	//
	m_sVK2DIKMapping[VK_NUMLOCK]	= DIK_NUMLOCK;		// "NumLock";
	//m_sVK2DIKMapping[]			= DIK_NUMPADENTER;	// "NumEnter"
	m_sVK2DIKMapping[VK_NUMPAD0]	= DIK_NUMPAD0;		// "NUM0";
	m_sVK2DIKMapping[VK_NUMPAD1]	= DIK_NUMPAD1;		// "NUM1";
	m_sVK2DIKMapping[VK_NUMPAD2]	= DIK_NUMPAD2;		// "NUM2";
	m_sVK2DIKMapping[VK_NUMPAD3]	= DIK_NUMPAD3;		// "NUM3";
	m_sVK2DIKMapping[VK_NUMPAD4]	= DIK_NUMPAD4;		// "NUM4";
	m_sVK2DIKMapping[VK_NUMPAD5]	= DIK_NUMPAD5;		// "NUM5";
	m_sVK2DIKMapping[VK_NUMPAD6]	= DIK_NUMPAD6;		// "NUM6";
	m_sVK2DIKMapping[VK_NUMPAD7]	= DIK_NUMPAD7;		// "NUM7";
	m_sVK2DIKMapping[VK_NUMPAD8]	= DIK_NUMPAD8;		// "NUM8";
	m_sVK2DIKMapping[VK_NUMPAD9]	= DIK_NUMPAD9;		// "NUM9";
	m_sVK2DIKMapping[VK_DECIMAL]	= DIK_DECIMAL;		// "NUM."
	m_sVK2DIKMapping[VK_ADD]		= DIK_ADD;			// "NUM+"
	m_sVK2DIKMapping[VK_SUBTRACT]	= DIK_SUBTRACT;		// "NUM-"
	m_sVK2DIKMapping[VK_DIVIDE]		= DIK_DIVIDE;		// "NUM/"
	m_sVK2DIKMapping[VK_MULTIPLY]	= DIK_MULTIPLY;		// "NUM*"
	// @}
}
//-------------------------------------------------------------------------------------------------
void sdInputSystem::MakeDefaultKeyMapping()
{
	// 行动类
	AddInputKey("前进",		E_INPUTKEY_UP,			DIK_W,	DIK_UP,				true, E_KC_MOVE);
	AddInputKey("后退",		E_INPUTKEY_DOWN,		DIK_S,	DIK_DOWN,			true, E_KC_MOVE);
	AddInputKey("左平移",	E_INPUTKEY_LEFT,		DIK_Q,	sdInvalidCombDIK,	true, E_KC_MOVE);
	AddInputKey("右平移",	E_INPUTKEY_RIGHT,		DIK_E,	sdInvalidCombDIK,	true, E_KC_MOVE);
	AddInputKey("右转",		E_INPUTKEY_ASCEND,		DIK_R,	sdInvalidCombDIK,	true, E_KC_MOVE);
	AddInputKey("右转",		E_INPUTKEY_DESCEND,		DIK_F,	sdInvalidCombDIK,	true, E_KC_MOVE);
	AddInputKey("左转",		E_INPUTKEY_TURNLEFT,	DIK_A,	DIK_LEFT,			true, E_KC_MOVE);
	AddInputKey("右转",		E_INPUTKEY_TURNRIGHT,	DIK_D,	DIK_RIGHT,			true, E_KC_MOVE);
}
//-------------------------------------------------------------------------------------------------
void sdInputSystem::AddInputKey(const char* szDesc, sdEInputKey eInputKey, 
	unsigned short usMainDlk, unsigned short usAsstDlk, bool bFlag, sdEKeyCategory eCat)
{
	sdKeyInfo kKeyInfo;

	if (szDesc) strncpy(kKeyInfo.szDescInfo, szDesc, 64);
	kKeyInfo.eInputKey	= eInputKey;
	kKeyInfo.usMainDIK	= usMainDlk;
	kKeyInfo.usAsstDIK	= usAsstDlk;
	kKeyInfo.bCanModify	= bFlag;
	kKeyInfo.eKeyCat	= eCat;

	m_kDefaultMapping.insert(std::make_pair(eInputKey, kKeyInfo));
}
//-------------------------------------------------------------------------------------------------
bool sdInputSystem::IsButtonDown(eMouseButton eButton) const
{
	return m_kMouse.IsButtonDown(eButton);
}

bool sdInputSystem::IsButtonPressed(eMouseButton eButton) const
{
	return m_kMouse.IsButtonPressed(eButton);
}

bool sdInputSystem::IsButtonReleased(eMouseButton eButton) const
{
	return m_kMouse.IsButtonReleased(eButton);
}

void sdInputSystem::SetPositionDelta(int iX, int iY, int iZ)
{
	m_kMouse.SetPositionDelta(iX, iY, iZ);
}

void sdInputSystem::GetPositionDelta(int& iX, int& iY, int& iZ)
{
	m_kMouse.GetPositionDelta(iX, iY, iZ);
}
//-------------------------------------------------------------------------------------------------
bool sdInputSystem::IsInputKeyDown(sdEInputKey eInputKey)
{
	bool bRet = false;

	// 尝试检查主映射
	unsigned short	usMainDIKCombineKey = m_kDefaultMapping[eInputKey].usMainDIK;
	unsigned char	ucMainDIKKey = sdGetDIKKey(usMainDIKCombineKey);
	bRet = m_kKeyboard.IsKeyDown(ucMainDIKKey) && CheckAssistDIKKey(usMainDIKCombineKey);
	if (bRet)	return true;

	// 尝试检查辅助映射
	unsigned short	usAsstDIKCombineKey = m_kDefaultMapping[eInputKey].usAsstDIK;
	if (usAsstDIKCombineKey != sdInvalidCombDIK)
	{
		unsigned char	ucAsstDIKKey = sdGetDIKKey(usAsstDIKCombineKey);
		bRet = m_kKeyboard.IsKeyDown(ucAsstDIKKey) && CheckAssistDIKKey(usAsstDIKCombineKey);
	}

	return bRet;
} 

bool sdInputSystem::IsInputKeyPressed(sdEInputKey eInputKey)
{
	bool bRet = false;

	// 尝试检查主映射
	unsigned short	usMainDIKCombineKey = m_kDefaultMapping[eInputKey].usMainDIK;
	unsigned char	ucMainDIKKey = sdGetDIKKey(usMainDIKCombineKey);
	bRet = m_kKeyboard.IsKeyPressed(ucMainDIKKey) && CheckAssistDIKKey(usMainDIKCombineKey);
	if (bRet)	return true;

	// 尝试检查辅助映射
	unsigned short	usAsstDIKCombineKey = m_kDefaultMapping[eInputKey].usAsstDIK;
	if (usAsstDIKCombineKey != sdInvalidCombDIK)
	{
		unsigned char	ucAsstDIKKey = sdGetDIKKey(usAsstDIKCombineKey);
		bRet = m_kKeyboard.IsKeyPressed(ucAsstDIKKey) && CheckAssistDIKKey(usAsstDIKCombineKey);
	}

	return bRet;
}

bool sdInputSystem::IsInputKeyReleased(sdEInputKey eInputKey)
{
	bool bRet = false;

	// 尝试检查主映射
	unsigned short	usMainDIKCombineKey = m_kDefaultMapping[eInputKey].usMainDIK;
	unsigned char	ucMainDIKKey = sdGetDIKKey(usMainDIKCombineKey);
	bRet = m_kKeyboard.IsKeyReleased(ucMainDIKKey) && CheckAssistDIKKey(usMainDIKCombineKey);
	if (bRet)	return true;

	// 尝试检查辅助映射
	unsigned short	usAsstDIKCombineKey = m_kDefaultMapping[eInputKey].usAsstDIK;
	if (usAsstDIKCombineKey != sdInvalidCombDIK)
	{
		unsigned char	ucAsstDIKKey = sdGetDIKKey(usAsstDIKCombineKey);
		bRet = m_kKeyboard.IsKeyReleased(ucAsstDIKKey) && CheckAssistDIKKey(usAsstDIKCombineKey);
	}

	return bRet;
}
//-------------------------------------------------------------------------------------------------
bool sdInputSystem::IsShiftKeyDown() const
{
	return m_kKeyboard.IsKeyDown(DIK_LSHIFT) || m_kKeyboard.IsKeyDown(DIK_RSHIFT);
}

bool sdInputSystem::IsCtrlKeyDown() const
{
	return m_kKeyboard.IsKeyDown(DIK_LCONTROL) || m_kKeyboard.IsKeyDown(DIK_RCONTROL);
}

bool sdInputSystem::IsAltKeyDown() const
{
	return m_kKeyboard.IsKeyDown(DIK_LALT) || m_kKeyboard.IsKeyDown(DIK_RALT);
}
//-------------------------------------------------------------------------------------------------
unsigned char sdInputSystem::IsAnyDIKKeyDown() const
{
	return m_kKeyboard.IsAnyKeyDown();
}

unsigned char sdInputSystem::IsAnyDIKKeyPressed() const
{
	return m_kKeyboard.IsAnyKeyPressed();
}

bool sdInputSystem::IsDIKKeyDown(unsigned char ucDIKKey) const
{
	return m_kKeyboard.IsKeyDown(ucDIKKey);
}

bool sdInputSystem::IsDIKKeyPressed(unsigned char ucDIKKey) const
{
	return m_kKeyboard.IsKeyPressed(ucDIKKey);
}

bool sdInputSystem::IsDIKKeyReleased(unsigned char ucDIKKey) const
{
	return m_kKeyboard.IsKeyReleased(ucDIKKey);
}
//-------------------------------------------------------------------------------------------------
bool sdInputSystem::CheckAssistDIKKey(unsigned short usDIKCombineKey)
{
	if (sdHasDIKCtrlKey(usDIKCombineKey))
	{
		// 需要检测Ctrl,木有被按下
		if (!IsCtrlKeyDown())
			return false;
	}
	else
	{
		// 不需要检测Ctrl,被按下
		if (IsCtrlKeyDown())
			return false;
	}

	if (sdHasDIKAltKey(usDIKCombineKey))
	{
		if (!IsAltKeyDown())
			return false;
	}
	else
	{
		if (IsAltKeyDown())
			return false;
	}

	if (sdHasDIKShiftKey(usDIKCombineKey))
	{
		if (!IsShiftKeyDown())
			return false;
	}
	else
	{
		if (IsShiftKeyDown())
			return false;
	}

	return true;
}
//-------------------------------------------------------------------------------------------------
void sdInputSystem::SetActive(bool bActive)
{
	m_bActive = bActive;
}

bool sdInputSystem::GetActive()
{
	return m_bActive;
}
//-------------------------------------------------------------------------------------------------
SD_GAMECORE_NAMESPACE_END_GAMECORE