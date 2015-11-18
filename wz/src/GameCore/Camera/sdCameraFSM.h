//*************************************************************************************************
// ����:	�������״̬��
//---------------------------------------------------------
// ����:		
// ����:		2012-07-30
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMECORE_CAMERA_FSM_H__
#define _SD_GAMECORE_CAMERA_FSM_H__
#include <sdSingleton.h>
#include <sdFSM.h>

SD_GAMECORE_NAMESPACE_BEGIN_GAMECORE

// �������״̬��,�������豸��ȡ״̬,���Ƶ�ǰ���
class sdCameraFSM : public Base::sdTSingleton<sdCameraFSM>, public Base::sdFSM
{
public:
	sdCameraFSM();
	~sdCameraFSM();

	// ��ʼ��������
	bool Initialize();
	void Destroy();

	// �麯���̳�
	virtual int UpdateState();

	// 
	NiCamera*	GetCamera() { return m_spCamera;};

protected:
	// ����ֱ��ʹ�����,���Կ��ǹҽ���NiNode��ʹ��任����������ϵһ��
	NiCameraPtr	m_spCamera;
};

SD_GAMECORE_NAMESPACE_END_GAMECORE
#endif