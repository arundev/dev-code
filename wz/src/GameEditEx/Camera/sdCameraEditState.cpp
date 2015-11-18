#include "sdGameEditExPCH.h"
#include "sdCameraEditState.h"

#include <sdTimeMgr.h>

#include <sdCameraFSM.h>

using namespace Base;
using namespace GameCore;

SD_GAMEEDITEX_NAMESPACE_BEGIN_GAMEEDITEX
//-------------------------------------------------------------------------------------------------
sdEditFreeCameraState::sdEditFreeCameraState()
: m_bMoveForward(false)
, m_bMoveBack(false)
, m_bMoveLeft(false)
, m_bMoveRight(false)
, m_bRightButtonDown(false)
, m_iX(0)
, m_iY(0)
, m_iDeltaZ(0)
, m_iLastX(0)
, m_iLastY(0)
, m_fKeyToFreeCameraSpeed(10.0f)
, m_fMouseToFreeCameraSpeed(1.0f)
, m_fMouseToFreeCameraRotate(0.001f)
{
	SetStateID(E_EDIT_CAMERA_FREE);
}
//-------------------------------------------------------------------------------------------------
void sdEditFreeCameraState::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
	if ('W' == wParam)
	{
		m_bMoveForward = true;
		m_bMoveBack = false;
	}
	else if ('S' == wParam)
	{
		m_bMoveForward = false;
		m_bMoveBack = true;
	}
	else if ('A' == wParam)
	{
		m_bMoveLeft = true;
		m_bMoveRight = false;
	}
	else if ('D' == wParam)
	{
		m_bMoveLeft = false;
		m_bMoveRight = true;
	}
}
//-------------------------------------------------------------------------------------------------
void sdEditFreeCameraState::OnKeyUp(WPARAM wParam, LPARAM lParam)
{
	if ('W' == wParam)
	{
		m_bMoveForward = false;
	}
	else if ('S' == wParam)
	{
		m_bMoveBack = false;
	}
	else if ('A' == wParam)
	{
		m_bMoveLeft = false;
	}
	else if ('D' == wParam)
	{
		m_bMoveRight = false;
	}
}
//-------------------------------------------------------------------------------------------------
void sdEditFreeCameraState::OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
	UINT uiLines;
	SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &uiLines, 0);
	m_iDeltaZ += (int)(short)HIWORD(wParam) / WHEEL_DELTA * uiLines;
}
//-------------------------------------------------------------------------------------------------
void sdEditFreeCameraState::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	m_iLastX = m_iX;
	m_iLastY = m_iY;
	m_iX = (int)(short)LOWORD(lParam);
	m_iY = (int)(short)HIWORD(lParam);
}
//-------------------------------------------------------------------------------------------------
void sdEditFreeCameraState::OnRightButtonDown(WPARAM wParam, LPARAM lParam)
{
	m_bRightButtonDown = true;
}
//-------------------------------------------------------------------------------------------------
void sdEditFreeCameraState::OnRightButtonUp(WPARAM wParam, LPARAM lParam)
{
	m_bRightButtonDown = false;
}
//-------------------------------------------------------------------------------------------------
int sdEditFreeCameraState::Update()
{
	sdCameraFSM* pkCameraFSM = sdCameraFSM::InstancePtr();
	NIASSERT(pkCameraFSM);

	Base::sdTimeMgr* pkTimeMgr = Base::sdTimeMgr::InstancePtr();
	NIASSERT(pkTimeMgr);

	NiCamera* spCamera = pkCameraFSM->GetCamera();
	NIASSERT(spCamera);

	// 关于平移
	// @{
	//
	NiPoint3 kCamPosition = spCamera->GetTranslate();

	// 计算平移
	float fFrameTime = pkTimeMgr->GetFrameTime();
	NiPoint3& kCameraWorldDirection = spCamera->GetWorldDirection();
	NiPoint3& kCameraRightDirection = spCamera->GetWorldRightVector();
	if (m_bMoveForward)		kCamPosition += kCameraWorldDirection * fFrameTime * m_fKeyToFreeCameraSpeed;
	if (m_bMoveBack)		kCamPosition -= kCameraWorldDirection * fFrameTime * m_fKeyToFreeCameraSpeed;	
	if (m_bMoveLeft)		kCamPosition -= kCameraRightDirection * fFrameTime * m_fKeyToFreeCameraSpeed;
	if (m_bMoveRight)		kCamPosition += kCameraRightDirection * fFrameTime * m_fKeyToFreeCameraSpeed;
	
	kCamPosition += kCameraWorldDirection * (float)m_iDeltaZ * m_fMouseToFreeCameraSpeed;
	m_iDeltaZ = 0;

	// 应用平移
	spCamera->SetTranslate(kCamPosition);
	// @}


	// 关于旋转
	// @{
	if (m_bRightButtonDown)
	{
		// 计算旋转
		NiMatrix3 kRotation = NiViewMath::Look(
			(m_iX - m_iLastX) * m_fMouseToFreeCameraRotate, 
			(m_iY - m_iLastY) * m_fMouseToFreeCameraRotate, 
			spCamera->GetRotate(), 
			NiPoint3::UNIT_Z);

		// 应用旋转
		spCamera->SetRotate(kRotation);

		//
		m_iLastX = m_iX;
		m_iLastY = m_iY;
	}
	// @}

	//
	spCamera->Update(0.0f);

	return 0;
}
//-------------------------------------------------------------------------------------------------
SD_GAMEEDITEX_NAMESPACE_END_GAMEEDITEX