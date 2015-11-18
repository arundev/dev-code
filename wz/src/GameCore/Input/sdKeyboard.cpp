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

	// ���������豸
	HRESULT hr = lpInputDevice->CreateDevice(GUID_SysKeyboard, &m_lpKeyboardDevice, NULL);
	if (FAILED(hr))	return false;

	// �������ݸ�ʽΪͨ�ü���
	hr = m_lpKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr))	return false;

	// ����Э��ģʽ
	//	DISCL_FOREGROUND	����׽��̨����(����ڵ���ʱ�����Ѿ���ʾ����)
	//	DISCL_BACKGROUND	
	//	DISCL_EXCLUSIVE		����
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

	// ��ȡ�豸
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

	// ����״̬
	memcpy(m_ucLastKeyState, m_ucKeyState, 256);

	// ״̬��ȡ
	HRESULT hr = m_lpKeyboardDevice->GetDeviceState(256, m_ucKeyState);
	if (FAILED(hr))
	{
		memset(m_ucKeyState, 0, 256);
		memset(m_ucLastKeyState, 0, 256);

		// ������(�豸��ʧ����/�豸��ȡ����)
		if ((hr == DIERR_INPUTLOST)		|| 
			(hr == DIERR_NOTACQUIRED)	||
			(hr == DIERR_OTHERAPPHASPRIO))
		{
			// �������»�ȡ�豸
			m_lpKeyboardDevice->Acquire();
		}
	}
}
//-------------------------------------------------------------------------------------------------
unsigned char sdKeyboard::IsAnyKeyDown() const
{
	// ɨ��
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
	// ɨ��
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