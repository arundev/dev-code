#include "sdGameCorePCH.h"
#include "sdKeyboard.h"

SD_GAMECORE_NAMESPACE_BEGIN_GAMECORE
//-------------------------------------------------------------------------------------------------
sdKeyboard::sdKeyboard()
	: m_lpKeyboardDevice(NULL)
{
	memset(m_ucKeyState, 0, 256);
	memset(m_ucLastKeyState, 0, 256);
}

sdKeyboard::~sdKeyboard()
{
	Destroy();
}
//-------------------------------------------------------------------------------------------------
bool sdKeyboard::Initialize(LPDIRECTINPUT8 lpInputDevice, HWND hWnd)
{
	if (!lpInputDevice)		
		return false;

	// 创建键盘设备
	HRESULT hr = lpInputDevice->CreateDevice(GUID_SysKeyboard, &m_lpKeyboardDevice, NULL);
	if (FAILED(hr))	return false;

	// 设置数据格式为通用键盘
	hr = m_lpKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))	return false;

	// 设置协作模式
	//	DISCL_FOREGROUND	不捕捉后台数据(最好在调用时窗口已经显示出来)
	//	DISCL_BACKGROUND	
	//	DISCL_EXCLUSIVE		排他
	//	DISCL_NONEXCLUSIVE	
	DWORD dwFlags = DISCL_BACKGROUND | DISCL_NONEXCLUSIVE;
	hr = m_lpKeyboardDevice->SetCooperativeLevel(hWnd, dwFlags);
	if (FAILED(hr))	return false;

	// 
	DIPROPDWORD	dwBufferData;
	dwBufferData.diph.dwSize		= sizeof(DIPROPDWORD);
	dwBufferData.diph.dwHeaderSize	= sizeof(DIPROPHEADER);
	dwBufferData.diph.dwObj			= 0;
	dwBufferData.diph.dwHow			= DIPH_DEVICE;
	dwBufferData.dwData				= 256;

	hr = m_lpKeyboardDevice->SetProperty(DIPROP_BUFFERSIZE, &dwBufferData.diph);
	if (FAILED(hr))	return false;

	// 获取设备
	hr = m_lpKeyboardDevice->Acquire();
	if (FAILED(hr))	return false;

	return true;
}
//-------------------------------------------------------------------------------------------------
void sdKeyboard::Destroy()
{
	if (m_lpKeyboardDevice)
	{
		m_lpKeyboardDevice->Unacquire();
		m_lpKeyboardDevice->Release();
		m_lpKeyboardDevice = NULL;
	}
}
//-------------------------------------------------------------------------------------------------
void sdKeyboard::Update()
{
	if (!m_lpKeyboardDevice) return;

	// 保存状态
	memcpy(m_ucLastKeyState, m_ucKeyState, 256);

	// 状态获取
	HRESULT hr = m_lpKeyboardDevice->GetDeviceState(256, m_ucKeyState);
	if (FAILED(hr))
	{
		memset(m_ucKeyState, 0, 256);
		memset(m_ucLastKeyState, 0, 256);

		// 错误处理(设备丢失错误/设备获取错误)
		if ((hr == DIERR_INPUTLOST)		|| 
			(hr == DIERR_NOTACQUIRED)	||
			(hr == DIERR_OTHERAPPHASPRIO))
		{
			// 尝试重新获取设备
			m_lpKeyboardDevice->Acquire();
		}
	}
}
//-------------------------------------------------------------------------------------------------
unsigned char sdKeyboard::IsAnyKeyDown() const
{
	// 扫描
	for (unsigned char uiKey = 0; uiKey < 256; ++uiKey) 
	{
		if (m_ucKeyState[uiKey])
			return uiKey;
	};

	return 0x00;
}
//-------------------------------------------------------------------------------------------------
unsigned char sdKeyboard::IsAnyKeyPressed() const
{
	// 扫描
	for (unsigned char uiKey = 0; uiKey < 256; ++uiKey) 
	{
		if ((m_ucKeyState[uiKey] & 0x80) && !(m_ucLastKeyState[uiKey] & 0x80))
			return uiKey;
	};

	return 0x00;
}
//-------------------------------------------------------------------------------------------------
bool sdKeyboard::IsKeyDown(unsigned char uiKey) const
{
	if (uiKey >= 256)
		return false;

	if (m_ucKeyState[uiKey] & 0x80)
		return true;

	return false;
}
//-------------------------------------------------------------------------------------------------
bool sdKeyboard::IsKeyPressed(unsigned char uiKey) const
{
	if (uiKey >= 256)
		return false;

	if ((m_ucKeyState[uiKey] & 0x80) && !(m_ucLastKeyState[uiKey] & 0x80))
		return true;

	return false;
}
//-------------------------------------------------------------------------------------------------
bool sdKeyboard::IsKeyReleased(unsigned char uiKey) const
{
	if (uiKey >= 256)
		return false;

	if (!(m_ucKeyState[uiKey] & 0x80) && (m_ucLastKeyState[uiKey] & 0x80))
		return true;

	return false;
}
//-------------------------------------------------------------------------------------------------
SD_GAMECORE_NAMESPACE_END_GAMECORE