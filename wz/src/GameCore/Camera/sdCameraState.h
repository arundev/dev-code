//*************************************************************************************************
// 内容:	相机状态
//---------------------------------------------------------
// 作者:		
// 创建:		2012-07-30
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMECORE_CAMERA_STATE_H__
#define _SD_GAMECORE_CAMERA_STATE_H__
#include <sdFSM.h>

SD_GAMECORE_NAMESPACE_BEGIN_GAMECORE
//-----------------------------------------------------------------------------
class sdCameraState : public Base::sdState
{
public:
	// 相机操作模式
	enum eCameraMode
	{
		E_CAMERA_FOLLOW = 0,	///< 相机跟随模式
		E_CAMERA_MOVIE,			///< 过场动画模式
		E_CAMERA_FREE,			///< 自由视角模式

		NUM_CAMERA_MODE,
	};

public:
	// 虚函数继承
	virtual void Enter() {};
	virtual	void Leave() {};
	virtual int	 Update() { return 0;};

	virtual void Reset(){};
};
//-----------------------------------------------------------------------------
class sdCameraFreeState : public sdCameraState
{
public:
	sdCameraFreeState();
	~sdCameraFreeState();

	// 虚函数继承
	virtual int Update();

protected:
	float m_fFreeCameraSpeed;
	float m_fFreeCameraRotate;
};
//-----------------------------------------------------------------------------
class sdCameraMovieState : public sdCameraState
{
public:
	// 虚函数继承
	virtual int Update();
};
//-----------------------------------------------------------------------------
SD_GAMECORE_NAMESPACE_END_GAMECORE
#endif