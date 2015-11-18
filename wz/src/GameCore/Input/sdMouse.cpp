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

	// ��������豸
	HRESULT hr = lpInputDevice->CreateDevice(GUID_SysMouse, &m_lpMouseDevice, NULL);
	if (FAILED(hr))	return false;

	// �������ݸ�ʽΪͨ�����
	hr = m_lpMouseDevice->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr))	return false;

	// ����Э��ģʽ
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

	// ��ȡ�豸
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

	// ����״̬
	m_eLastState = m_eState;

	// ״̬��ȡ
	m_lpMouseDevice->Poll();
	HRESULT hr = m_lpMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &m_eState);
	if (FAILED(hr))
	{
		memset(&m_eState, 0, sizeof(m_eState));

		// ������(�豸��ʧ����/�豸��ȡ����)
		if (hr == DIERR_INPUTLOST || hr == DIERR_NOTACQUIRED)
		{
			// �������»�ȡ�豸
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