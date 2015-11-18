//*************************************************************************************************
// ����:	ϵͳʱ��
//---------------------------------------------------------
// ����:		
// ����:		2012-07-16
// ����޸�:	2013-02-04
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_MISC_TIME_MGR_H__
#define _SD_BASE_MISC_TIME_MGR_H__
#include "sdSingleton.h"

BASE_NAMESPACE_BEGIN_BASE

// �򵥵�ʱ�����
// ::GetTickCount()					��ȡ����ϵͳ����������������(elapsed)�ĺ�����
// ::QueryPerformanceCounter();		��ȡ�߾��ȼ�ʱ��ʱ��(��λ:Count)
// ::QueryPerformanceFrequency();	��ȡ�߾��ȼ�ʱ��Ƶ��(��λ:Count/Second)
class sdTimeMgr : public sdTSingleton<sdTimeMgr>
{
public:
	sdTimeMgr();
	~sdTimeMgr();

	// ����
	void Update();

	//
	float	GetAccumTime(){	return m_fAccumTime;}
	float	GetFrameTime(){	return m_fFrameTime;}

	void	SetSCDelayTime(float fDelayTime){ m_fSCDelayTime = fDelayTime;}
	float	GetSCDelayTime()  { return m_fSCDelayTime;}

protected:
	// �߾���ʱ��Tick
	LARGE_INTEGER	m_liStartTime;	// ��������ʱ�Ӽ���(Ticks)
	LARGE_INTEGER	m_liFrequency;	// �߾���ʱ��Ƶ��(Ticks/s)
	DWORD			m_uiStartTick;	// ϵͳʱ��(ms)

	// �������ʱ�����ڵ����߾���ʱ��ʱʹ��
	LONGLONG	m_llLastTime;

	// ��ʱ��Ϣ
	float	m_fFrameTime;		// ÿ��Tick֮��ʱ����
	float	m_fAccumTime;		// ���������ۼƼ�ʱ
	float	m_fSCDelayTime;	

	// ������Ϣ
	HANDLE	m_hProc;
	HANDLE	m_hThreading;	
	DWORD	m_uiProcMask;
	DWORD	m_uiSysMask;
};

BASE_NAMESPACE_END_BASE
#endif