//*************************************************************************************************
// ����:	���״̬
//---------------------------------------------------------
// ����:		
// ����:		2012-07-30
// ����޸�:
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
	// �������ģʽ
	enum eCameraMode
	{
		E_CAMERA_FOLLOW = 0,	///< �������ģʽ
		E_CAMERA_MOVIE,			///< ��������ģʽ
		E_CAMERA_FREE,			///< �����ӽ�ģʽ

		NUM_CAMERA_MODE,
	};

public:
	// �麯���̳�
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

	// �麯���̳�
	virtual int Update();

protected:
	float m_fFreeCameraSpeed;
	float m_fFreeCameraRotate;
};
//-----------------------------------------------------------------------------
class sdCameraMovieState : public sdCameraState
{
public:
	// �麯���̳�
	virtual int Update();
};
//-----------------------------------------------------------------------------
SD_GAMECORE_NAMESPACE_END_GAMECORE
#endif