#include "sdGameCorePCH.h"
#include "sdCameraState.h"
#include "sdCameraFSM.h"
#include "sdInputSystem.h"
#include <sdTimeMgr.h>

SD_GAMECORE_NAMESPACE_BEGIN_GAMECORE
//-------------------------------------------------------------------------------------------------
sdCameraFreeState::sdCameraFreeState()
: m_fFreeCameraSpeed(50.0f)
, m_fFreeCameraRotate(0.001f)
{
	SetStateID(E_CAMERA_FREE);
}
//-------------------------------------------------------------------------------------------------
sdCameraFreeState::~sdCameraFreeState()
{
	
}
//-------------------------------------------------------------------------------------------------
int sdCameraFreeState::Update()
{
	sdCameraFSM* pkCameraFSM = sdCameraFSM::InstancePtr();
	NIASSERT(pkCameraFSM);

	sdInputSystem* pkInputSystem = sdInputSystem::InstancePtr();
	NIASSERT(pkInputSystem);

	Base::sdTimeMgr* pkTimeMgr = Base::sdTimeMgr::InstancePtr();
	NIASSERT(pkTimeMgr);

	// ����û�б�������ֱ�ӷ���
	if (!pkInputSystem->GetActive())
		return 0;

	// ����ƽ��
	// @{
	// �����̷����
	bool bUp	  = pkInputSystem->IsInputKeyDown(E_INPUTKEY_UP);
	bool bDown	  = pkInputSystem->IsInputKeyDown(E_INPUTKEY_DOWN);
	bool bLeft	  = pkInputSystem->IsInputKeyDown(E_INPUTKEY_TURNLEFT);
	bool bRight   = pkInputSystem->IsInputKeyDown(E_INPUTKEY_TURNRIGHT);
	bool bAscend  = pkInputSystem->IsInputKeyDown(E_INPUTKEY_ASCEND);
	bool bDescend = pkInputSystem->IsInputKeyDown(E_INPUTKEY_DESCEND);
	
	NiCamera* spCamera = pkCameraFSM->GetCamera();
	NiPoint3 kCamPosition = spCamera->GetWorldTranslate();

	// ����ƽ��
	float fFrameTime = pkTimeMgr->GetFrameTime();
	if (bUp)		kCamPosition += spCamera->GetWorldDirection() * fFrameTime * m_fFreeCameraSpeed;
	if (bDown)		kCamPosition -= spCamera->GetWorldDirection() * fFrameTime * m_fFreeCameraSpeed;	
	if (bLeft)		kCamPosition -= spCamera->GetWorldRightVector() * fFrameTime * m_fFreeCameraSpeed;
	if (bRight)		kCamPosition += spCamera->GetWorldRightVector() * fFrameTime * m_fFreeCameraSpeed;
	if (bAscend)	kCamPosition += spCamera->GetWorldUpVector() * fFrameTime * m_fFreeCameraSpeed;
	if (bDescend)	kCamPosition -= spCamera->GetWorldUpVector() * fFrameTime * m_fFreeCameraSpeed;

	// Ӧ��ƽ��
	spCamera->SetWorldTranslate(kCamPosition);
	// }@


	// ������ת
	// @{
	bool bRightButtonDown = pkInputSystem->IsButtonDown(E_MOUSE_RBUTTON);	
	if (bRightButtonDown)
	{
		// ������
		int iX = 0, iY = 0, iZ = 0;
		pkInputSystem->GetPositionDelta(iX, iY, iZ);

		// ������ת
		NiMatrix3 kRotation = NiViewMath::Look(iX * m_fFreeCameraRotate, 
			iY * m_fFreeCameraRotate, 
			spCamera->GetWorldRotate(), 
			NiPoint3::UNIT_Z);

		// Ӧ����ת
		spCamera->SetWorldRotate(kRotation);
	}
	// @}

	return 0;
}
//-------------------------------------------------------------------------------------------------
SD_GAMECORE_NAMESPACE_END_GAMECORE