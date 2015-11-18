//*************************************************************************************************
// 内容:	相机控制状态机
//---------------------------------------------------------
// 作者:		
// 创建:		2012-07-30
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMECORE_CAMERA_FSM_H__
#define _SD_GAMECORE_CAMERA_FSM_H__
#include <sdSingleton.h>
#include <sdFSM.h>

SD_GAMECORE_NAMESPACE_BEGIN_GAMECORE

// 相机控制状态机,从输入设备提取状态,控制当前相机
class sdCameraFSM : public Base::sdTSingleton<sdCameraFSM>, public Base::sdFSM
{
public:
	sdCameraFSM();
	~sdCameraFSM();

	// 初始化与销毁
	bool Initialize();
	void Destroy();

	// 虚函数继承
	virtual int UpdateState();

	// 
	NiCamera*	GetCamera() { return m_spCamera;};

protected:
	// 这里直接使用相机,可以考虑挂接在NiNode上使其变换与世界坐标系一致
	NiCameraPtr	m_spCamera;
};

SD_GAMECORE_NAMESPACE_END_GAMECORE
#endif