//*************************************************************************************************
// 内容:	编辑状态的相机状态(此时输入方式不同)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-27
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMEEDITEX_EDITCAMERA_H__
#define _SD_GAMEEDITEX_EDITCAMERA_H__
#include <sdCameraState.h>

SD_GAMEEDITEX_NAMESPACE_BEGIN_GAMEEDITEX
//-------------------------------------------------------------------------------------------------
// 编辑状态下的相机
//-------------------------------------------------------------------------------------------------
class sdEditCameraState : public GameCore::sdCameraState
{
public:
	// 相机操作模式(编辑状态)
	enum eEditCameraMode
	{
		E_EDIT_CAMERA_FREE = NUM_CAMERA_MODE,	///< 自由视角模式
		E_EDIT_CAMERA_CENTER,					///< 围绕指定点旋转模式
		NUM_EDIT_CAMERA_MODE,
	};

public:
	sdEditCameraState() {};
	virtual ~sdEditCameraState() {};

	// 虚函数继承
	virtual void Enter() {};
	virtual	void Leave() {};
	virtual int	 Update() { return 0;};

	// 输入消息
	virtual void OnKeyDown(WPARAM wParam, LPARAM lParam){};
	virtual void OnKeyUp(WPARAM wParam, LPARAM lParam){};
	virtual void OnMouseWheel(WPARAM wParam, LPARAM lParam){};
	virtual void OnMouseMove(WPARAM wParam, LPARAM lParam){};
	virtual void OnLeftButtonDown(WPARAM wParam, LPARAM lParam){};
	virtual void OnLeftButtonUp(WPARAM wParam, LPARAM lParam){};
	virtual void OnRightButtonDown(WPARAM wParam, LPARAM lParam){};
	virtual void OnRightButtonUp(WPARAM wParam, LPARAM lParam){};	
};
//-------------------------------------------------------------------------------------------------
// 编辑状态下的自由相机
//-------------------------------------------------------------------------------------------------
class sdEditFreeCameraState : public sdEditCameraState
{
public:
	sdEditFreeCameraState();

	// 虚函数继承
	virtual int Update();

	virtual void OnKeyDown(WPARAM wParam, LPARAM lParam);
	virtual void OnKeyUp(WPARAM wParam, LPARAM lParam);
	virtual void OnMouseWheel(WPARAM wParam, LPARAM lParam);
	virtual void OnMouseMove(WPARAM wParam, LPARAM lParam);
	virtual void OnRightButtonDown(WPARAM wParam, LPARAM lParam);
	virtual void OnRightButtonUp(WPARAM wParam, LPARAM lParam);

protected:
	bool m_bMoveForward;
	bool m_bMoveBack;
	bool m_bMoveLeft;
	bool m_bMoveRight;
	bool m_bRightButtonDown;
	int m_iX, m_iY, m_iLastX, m_iLastY, m_iDeltaZ;

	float m_fKeyToFreeCameraSpeed;
	float m_fMouseToFreeCameraSpeed;
	float m_fMouseToFreeCameraRotate;
};
//-------------------------------------------------------------------------------------------------
SD_GAMEEDITEX_NAMESPACE_END_GAMEEDITEX
#endif