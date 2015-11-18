#include "sdBasePCH.h"
#include "sdTimeMgr.h"

BASE_NAMESPACE_BEGIN_BASE
//-------------------------------------------------------------------------------------------------
sdTimeMgr::sdTimeMgr()
{
	// 获取当前进程与线程句柄
	m_hProc = ::GetCurrentProcess();
	m_hThreading = ::GetCurrentThread();

	// 获取进程的CPU亲缘信息,强制绑定到CPU0,获取时间
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
	// 获取当前精确时间计数(tick)
	LARGE_INTEGER liCurTime;
	::SetThreadAffinityMask(m_hThreading, 0x1);
	::QueryPerformanceCounter(&liCurTime);
	::SetThreadAffinityMask(m_hThreading, m_uiProcMask);

	// 计算程序启动的Eclapse时间
	LONGLONG llNewTime = liCurTime.QuadPart - m_liStartTime.QuadPart;
	unsigned long ulNewTicks = (unsigned long)(1000 * llNewTime / m_liFrequency.QuadPart);

	// 补偿计时误差,貌似代码来自Ogre::OgreTimerImp.h
	//
	// (来自http://blog.csdn.net/hunter8777/article/details/6204719)
	// 通过查询Microsoft KB: Q274323（http://support.microsoft.com/kb/274323/en-us）,
	// 由于芯片组的设计缺陷，QueryPerformanceCounter函数返回的结果会意外的产生跳跃的错误时间。
	// 这也就是不能单独使用高精度计时器的原因，必须有另一个计时方法来进行补偿调整。
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

	// 更新计时
	m_fFrameTime = ulNewTicks / 1000.0f - m_fAccumTime;
	m_fAccumTime += m_fFrameTime;
	
	// 更新当前精确时间计数(tick)
	m_liStartTime.QuadPart = liCurTime.QuadPart;
}
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_BASE