//*************************************************************************************************
// 内容:	系统时间
//---------------------------------------------------------
// 作者:		
// 创建:		2012-07-16
// 最后修改:	2013-02-04
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_MISC_TIME_MGR_H__
#define _SD_BASE_MISC_TIME_MGR_H__
#include "sdSingleton.h"

BASE_NAMESPACE_BEGIN_BASE

// 简单的时间管理
// ::GetTickCount()					获取操作系统启动到现在所经过(elapsed)的毫秒数
// ::QueryPerformanceCounter();		获取高精度计时器时间(单位:Count)
// ::QueryPerformanceFrequency();	获取高精度计时器频率(单位:Count/Second)
class sdTimeMgr : public sdTSingleton<sdTimeMgr>
{
public:
	sdTimeMgr();
	~sdTimeMgr();

	// 更新
	void Update();

	//
	float	GetAccumTime(){	return m_fAccumTime;}
	float	GetFrameTime(){	return m_fFrameTime;}

	void	SetSCDelayTime(float fDelayTime){ m_fSCDelayTime = fDelayTime;}
	float	GetSCDelayTime()  { return m_fSCDelayTime;}

protected:
	// 高精度时钟Tick
	LARGE_INTEGER	m_liStartTime;	// 程序启动时钟计数(Ticks)
	LARGE_INTEGER	m_liFrequency;	// 高精度时钟频率(Ticks/s)
	DWORD			m_uiStartTick;	// 系统时间(ms)

	// 保存更新时间用于调整高精度时钟时使用
	LONGLONG	m_llLastTime;

	// 计时信息
	float	m_fFrameTime;		// 每次Tick之间时间间隔
	float	m_fAccumTime;		// 程序运行累计计时
	float	m_fSCDelayTime;	

	// 进程信息
	HANDLE	m_hProc;
	HANDLE	m_hThreading;	
	DWORD	m_uiProcMask;
	DWORD	m_uiSysMask;
};

BASE_NAMESPACE_END_BASE
#endif