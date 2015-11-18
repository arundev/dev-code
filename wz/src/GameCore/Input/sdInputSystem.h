//*************************************************************************************************
// ����:	��Ϸ����ϵͳ
//			������ʹ�õ�DirectInput��������ȡ����(����SDL��װ��DInput)
//			(dinput.h,winuser.h)
//			(Ŀǰ��������,���,�Ժ������չ����Ϸ�������ڸ��ִ������뷴����)
//				1.DirectInputֱ�����������н���,������ֱ�Ӻͻ����������뷽ʽ
//				2.Windows��Ϣϵͳֻ���û��淽ʽ
//
//			������ӱ�����Ϣע��״̬����Ӧ�༭����
//---------------------------------------------------------
// ����:		
// ����:		2012-07-23
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMECORE_INPUT_SYSTEM_H__
#define _SD_GAMECORE_INPUT_SYSTEM_H__
#include <sdSingleton.h>
#include "sdMouse.h"
#include "sdKeyBoard.h"
#include "sdInputKey.h"

SD_GAMECORE_NAMESPACE_BEGIN_GAMECORE

// ����ӳ��
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

	sdEKeyCategory	eKeyCat;	// �߼�������
	sdEInputKey		eInputKey;	// �߼���(������޹�)
	unsigned short	usMainDIK;	// ��2λ��������,��2λ������Ƽ�(Ctrl/Shift/Alt)
	unsigned short	usAsstDIK;	// ��2λ��������,��2λ���������Ƽ�
	bool			bCanModify;	// �Ƿ������û��ض���
	char			szDescInfo[64];	// �߼�������
};

// ����ϵͳ,ÿ֡������ȡ�豸״̬������,֧�ּ���ӳ��
class sdInputSystem : public Base::sdTSingleton<sdInputSystem>
{
public:
	sdInputSystem();
	~sdInputSystem();

	// �豸��ʼ��/����/����
	bool	Initialize(HINSTANCE hInstance, HWND hWnd);
	void	Update();
	void	Destroy();

	// ����״̬
	void	SetActive(bool bActive);
	bool	GetActive();

	// ���״̬���
	// @{
	bool	IsButtonDown(eMouseButton eButton) const;
	bool	IsButtonPressed(eMouseButton eButton) const;
	bool	IsButtonReleased(eMouseButton eButton) const;
	void	SetPositionDelta(int iX, int iY, int iZ);
	void	GetPositionDelta(int& iX, int& iY, int& iZ);
	// @}

	// ����״̬���
	// @{
	// �û������
	bool	IsInputKeyDown(sdEInputKey eInputKey);
	bool	IsInputKeyPressed(sdEInputKey eInputKey);
	bool	IsInputKeyReleased(sdEInputKey eInputKey);

	// ��������
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
	// ���ȱʡ�߼���-�����ӳ��
	void MakeDiskStringMapping();
	void MakeDefaultKeyMapping();

	// ����û���-�����ӳ��
	void AddInputKey(const char* szDesc, sdEInputKey eInputKey, 
		unsigned short usMainDlk, unsigned short usAsstDlk, bool bFlag, sdEKeyCategory eCat);

	// ��������ӳ���������ϼ��е���������(Ctrl/Alt/Shift)�Ƿ���
	bool CheckAssistDIKKey(unsigned short usDIKCombineKey);

protected:
	bool				m_bInitialized;		// ��ʼ��
	bool				m_bActive;

	HWND				m_hWnd;				// ���ھ��
	IDirectInput8*		m_lpInputDevice;	// D3D�����豸
	sdMouse				m_kMouse;
	sdKeyboard			m_kKeyboard;

	// �����-�����ӳ��
	short int	m_sVK2DIKMapping[256];
	char*		m_kDIKString[256];

	// �û���-�����ӳ��(ȱʡӳ����û�����ӳ��)
	typedef std::map<sdEInputKey, sdKeyInfo> KeyMap;
	typedef std::map<sdEInputKey, sdKeyInfo>::iterator	KeyMapItr;
	KeyMap	m_kDefaultMapping;
	KeyMap	m_kUserMapping;
};

SD_GAMECORE_NAMESPACE_END_GAMECORE
#endif