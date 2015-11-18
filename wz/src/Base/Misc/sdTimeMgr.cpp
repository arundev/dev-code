#include "sdBasePCH.h"
#include "sdTimeMgr.h"

BASE_NAMESPACE_BEGIN_BASE
//-------------------------------------------------------------------------------------------------
sdTimeMgr::sdTimeMgr()
{
	// ��ȡ��ǰ�������߳̾��
	m_hProc = ::GetCurrentProcess();
	m_hThreading = ::GetCurrentThread();

	// ��ȡ���̵�CPU��Ե��Ϣ,ǿ�ư󶨵�CPU0,��ȡʱ��
	::GetProcessAffinityMask(m_hProc, &m_uiProcMask, &m_uiSysMask);
	::SetThreadAffinityMask(m_hThreading, 0x1);
	::QueryPerformanceFrequency(&m_liFrequency);
	::QueryPerformanceCounter(&m_liStartTime);
	m_uiStartTick = ::GetTickCount();
	::SetThreadAffinityMask(m_hThreading, m_uiProcMask);

	m_fAccumTime = 0.0f;
	m_fFrameTime = 0.0f;
	m_fSCDelayTime = 0.0f;
	m_llLastTime = 0;
}
//-------------------------------------------------------------------------------------------------
sdTimeMgr::~sdTimeMgr()
{

}
//-------------------------------------------------------------------------------------------------
void sdTimeMgr::Update()
{
	// ��ȡ��ǰ��ȷʱ�����(tick)
	LARGE_INTEGER liCurTime;
	::SetThreadAffinityMask(m_hThreading, 0x1);
	::QueryPerformanceCounter(&liCurTime);
	::SetThreadAffinityMask(m_hThreading, m_uiProcMask);

	// �������������Eclapseʱ��
	LONGLONG llNewTime = liCurTime.QuadPart - m_liStartTime.QuadPart;
	unsigned long ulNewTicks = (unsigned long)(1000 * llNewTime / m_liFrequency.QuadPart);

	// ������ʱ���,ò�ƴ�������Ogre::OgreTimerImp.h
	//
	// (����http://blog.csdn.net/hunter8777/article/details/6204719)
	// ͨ����ѯMicrosoft KB: Q274323��http://support.microsoft.com/kb/274323/en-us��,
	// ����оƬ������ȱ�ݣ�QueryPerformanceCounter�������صĽ��������Ĳ�����Ծ�Ĵ���ʱ�䡣
	// ��Ҳ���ǲ��ܵ���ʹ�ø߾��ȼ�ʱ����ԭ�򣬱�������һ����ʱ���������в���������
	//
	// detect and compensate for performance counter leaps
	// (surprisingly common, see microsoft kb: Q274323)
	unsigned long uiCheckTick = ::GetTickCount() - m_uiStartTick;
	signed long slMescOffset = (signed long)(ulNewTicks - uiCheckTick);
	if (slMescOffset < -100 || slMescOffset > 100)
	{
		// we just kepp the timer running forward
		LONGLONG llAdjust = (std::min)(slMescOffset * m_liFrequency.QuadPart / 1000, llNewTime - m_llLastTime);
		m_liStartTime.QuadPart += llAdjust;
		llNewTime -= llAdjust;

		// re-calculate millseconds
		ulNewTicks = (unsigned long)(1000 * llNewTime / m_liFrequency.QuadPart);
	}
	m_llLastTime = llNewTime;

	// ���¼�ʱ
	m_fFrameTime = ulNewTicks / 1000.0f - m_fAccumTime;
	m_fAccumTime += m_fFrameTime;
	
	// ���µ�ǰ��ȷʱ�����(tick)
	m_liStartTime.QuadPart = liCurTime.QuadPart;
}
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_BASE