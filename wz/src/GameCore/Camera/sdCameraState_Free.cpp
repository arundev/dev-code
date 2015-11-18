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

	// 窗口没有被激活则直接返回
	if (!pkInputSystem->GetActive())
		return 0;

	// 关于平移
	// @{
	// 检测键盘方向键
	bool bUp	  = pkInputSystem->IsInputKeyDown(E_INPUTKEY_UP);
	bool bDown	  = pkInputSystem->IsInputKeyDown(E_INPUTKEY_DOWN);
	bool bLeft	  = pkInputSystem->IsInputKeyDown(E_INPUTKEY_TURNLEFT);
	bool bRight   = pkInputSystem->IsInputKeyDown(E_INPUTKEY_TURNRIGHT);
	bool bAscend  = pkInputSystem->IsInputKeyDown(E_INPUTKEY_ASCEND);
	bool bDescend = pkInputSystem->IsInputKeyDown(E_INPUTKEY_DESCEND);
	
	NiCamera* spCamera = pkCameraFSM->GetCamera();
	NiPoint3 kCamPosition = spCamera->GetWorldTranslate();

	// 计算平移
	float fFrameTime = pkTimeMgr->GetFrameTime();
	if (bUp)		kCamPosition += spCamera->GetWorldDirection() * fFrameTime * m_fFreeCameraSpeed;
	if (bDown)		kCamPosition -= spCamera->GetWorldDirection() * fFrameTime * m_fFreeCameraSpeed;	
	if (bLeft)		kCamPosition -= spCamera->GetWorldRightVector() * fFrameTime * m_fFreeCameraSpeed;
	if (bRight)		kCamPosition += spCamera->GetWorldRightVector() * fFrameTime * m_fFreeCameraSpeed;
	if (bAscend)	kCamPosition += spCamera->GetWorldUpVector() * fFrameTime * m_fFreeCameraSpeed;
	if (bDescend)	kCamPosition -= spCamera->GetWorldUpVector() * fFrameTime * m_fFreeCameraSpeed;

	// 应用平移
	spCamera->SetWorldTranslate(kCamPosition);
	// }@


	// 关于旋转
	// @{
	bool bRightButtonDown = pkInputSystem->IsButtonDown(E_MOUSE_RBUTTON);	
	if (bRightButtonDown)
	{
		// 检测鼠标
		int iX = 0, iY = 0, iZ = 0;
		pkInputSystem->GetPositionDelta(iX, iY, iZ);

		// 计算旋转
		NiMatrix3 kRotation = NiViewMath::Look(iX * m_fFreeCameraRotate, 
			iY * m_fFreeCameraRotate, 
			spCamera->GetWorldRotate(), 
			NiPoint3::UNIT_Z);

		// 应用旋转
		spCamera->SetWorldRotate(kRotation);
	}
	// @}

	return 0;
}
//-------------------------------------------------------------------------------------------------
SD_GAMECORE_NAMESPACE_END_GAMECORE