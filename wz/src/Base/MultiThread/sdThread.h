//*************************************************************************************************
// ����:	���̶߳���
//---------------------------------------------------------
// ����:		
// ����:		2013-02-27
// ����޸�:	
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_THREADING_THREAD_H__
#define _SD_BASE_THREADING_THREAD_H__

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_THREADING
//-----------------------------------------------------------------------------
// �߳����ȼ�
//-----------------------------------------------------------------------------
enum EThreadPriority
{
	E_TP_IDLE = 0,
	E_TP_LOWEST,
	E_TP_BLOW_NORMAL,
	E_TP_NORMAL,
	E_TP_ABOVE_NORMAL,
	E_TP_HIGHEST,

	NUM_TP,
};
//-----------------------------------------------------------------------------
// �߳�����״̬
//-----------------------------------------------------------------------------
enum EThreadStatus
{
	E_TS_RUNNING = 0,
	E_TS_SUSPENDED,
	E_TS_COMPLETE,
};
//-----------------------------------------------------------------------------
// �߳�ִ�ж���(�ο���Cry/UE/GB)
//-----------------------------------------------------------------------------
class sdThreadProcedure 
{
public:
	//sdThreadProcedure(){}
	//virtual ~sdThreadProcedure() {}

	//virtual bool Initialize() = 0;
	//virtual bool Run() = 0;
	//virtual void Stop() = 0;
	//virtual void Exit() = 0;
};
//-----------------------------------------------------------------------------
// �̶߳���(�ο���Cry/UE/GB)
//-----------------------------------------------------------------------------
class sdThread
{
public:
	//sdThread();
	//virtual ~sdThread();

	//// �߳����ȼ�
	//virtual void SetThreadPriority(EThreadPriority eThreadPriority);
	//virtual EThreadPriority GetThreadPriority() const;

	//// �������׺���
	//virtual void SetProcessorAffinity(DWORD ProcessorNum);

};
//-----------------------------------------------------------------------------
BASE_NAMESPACE_END_THREADING
BASE_NAMESPACE_END_BASE
#endif