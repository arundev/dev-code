#include "sdGameCorePCH.h"
#include "sdMouse.h"

SD_GAMECORE_NAMESPACE_BEGIN_GAMECORE
//-------------------------------------------------------------------------------------------------
sdMouse::sdMouse()
: m_lpMouseDevice(NULL)
{
	memset(&m_eState, 0, sizeof(m_eState));
}

sdMouse::~sdMouse()
{
	Destroy();
}
//-------------------------------------------------------------------------------------------------
bool sdMouse::Initialize(LPDIRECTINPUT8 lpInputDevice, HWND hWnd)
{
	if (!lpInputDevice)		
		return false;

	// 创建鼠标设备
	HRESULT hr = lpInputDevice->CreateDevice(GUID_SysMouse, &m_lpMouseDevice, NULL);
	if (FAILED(hr))	return false;

	// 设置数据格式为通用鼠标
	hr = m_lpMouseDevice->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))	return false;

	// 设置协作模式
	DWORD dwFlags = DISCL_BACKGROUND | DISCL_NONEXCLUSIVE;
	hr = m_lpMouseDevice->SetCooperativeLevel(hWnd, dwFlags);
	if (FAILED(hr))	return false;

	// 
	DIPROPDWORD	dwBufferData;
	dwBufferData.diph.dwSize		= sizeof(DIPROPDWORD);
	dwBufferData.diph.dwHeaderSize	= sizeof(DIPROPHEADER);
	dwBufferData.diph.dwObj			= 0;
	dwBufferData.diph.dwHow			= DIPH_DEVICE;
	dwBufferData.dwData				= sizeof(DIMOUSESTATE);

	hr = m_lpMouseDevice->SetProperty(DIPROP_BUFFERSIZE, &dwBufferData.diph);
	if (FAILED(hr))	return false;

	// 获取设备
	hr = m_lpMouseDevice->Acquire();
	if (FAILED(hr))	return false;

	return true;
}
//-------------------------------------------------------------------------------------------------
void sdMouse::Destroy()
{
	if (m_lpMouseDevice)
	{
		m_lpMouseDevice->Unacquire();
		m_lpMouseDevice->Release();
		m_lpMouseDevice = NULL;
	}
}
//-------------------------------------------------------------------------------------------------
void sdMouse::Update()
{
	if (!m_lpMouseDevice) return;

	// 保存状态
	m_eLastState = m_eState;

	// 状态获取
	m_lpMouseDevice->Poll();
	HRESULT hr = m_lpMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_eState);
	if (FAILED(hr))
	{
		memset(&m_eState, 0, sizeof(m_eState));

		// 错误处理(设备丢失错误/设备获取错误)
		if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
		{
			// 尝试重新获取设备
			m_lpMouseDevice->Acquire();
		}
	}
}
//-------------------------------------------------------------------------------------------------
bool sdMouse::IsButtonDown(eMouseButton eButton) const
{
	return m_eState.rgbButtons[eButton] != FALSE;
}

bool sdMouse::IsButtonPressed(eMouseButton eButton) const
{
	return m_eState.rgbButtons[eButton] && !m_eLastState.rgbButtons[eButton];
}

bool sdMouse::IsButtonReleased(eMouseButton eButton) const
{
	return !m_eState.rgbButtons[eButton] && m_eLastState.rgbButtons[eButton];
}

void sdMouse::SetPositionDelta(int iX, int iY, int iZ)
{
	m_eState.lX = iX; m_eState.lY = iY; m_eState.lZ = iZ;
}

void sdMouse::GetPositionDelta(int& iX, int& iY, int& iZ)
{
	iX = m_eState.lX; iY = m_eState.lY; iZ = m_eState.lZ;
}
//-------------------------------------------------------------------------------------------------
SD_GAMECORE_NAMESPACE_END_GAMECORE