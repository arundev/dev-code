//*************************************************************************************************
// 内容:	较为精确的性能分析工具(目前用于ResourceSystem处理资源加载计时)
//---------------------------------------------------------
// 作者:		
// 创建:		2013-02-04
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_DIAGNOSTICS_PROFILE_OUTPUT_H__
#define _SD_BASE_DIAGNOSTICS_PROFILE_OUTPUT_H__

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_DIAGNOSTICS

class sdProfileOutput
{
public:
	sdProfileOutput()
	{
		Reset();
	}

	void Begin()
	{
		::QueryPerformanceCounter(&m_liProfCountStart);
	}

	void End()
	{
		::QueryPerformanceCounter(&m_liProfCountEnd);
	}

	void Reset()
	{
		m_dProfDt = 0.0;
		m_dProfTotalDt = 0.0;
		m_llProfKFreq;

		::QueryPerformanceFrequency(&m_liProfFreq);
		m_llProfKFreq = m_liProfFreq.QuadPart / 1000;
	}

	//void Output(const std::string& strInfoPrex, double dMinNoOutput = 0.0)
	//{
	//	strInfoPrex;
	//	dMinNoOutput;
	//
	//	m_dProfDt = (double)(m_liProfCountEnd.QuadPart - m_liProfCountStart.QuadPart) / (double)(m_llProfKFreq);
	//	if (m_dProfDt > dMinNoOutput)
	//		sdOutDebugString("%s %.3f ms \n", strInfoPrex.c_str(), m_dProfDt);
	//}

	bool IsTimeOver(double dReference)
	{
		End();

		m_dProfDt = (double)(m_liProfCountEnd.QuadPart - m_liProfCountStart.QuadPart) / (double)(m_llProfKFreq);
		return m_dProfDt >= dReference;
	}

protected:
	LARGE_INTEGER m_liProfFreq;			///< 计时器频率(单位:count/second)
	LARGE_INTEGER m_liProfCountStart;	///< 起始计时(单位:Count)
	LARGE_INTEGER m_liProfCountEnd;		///< 终止计时(单位:Count)
	double m_dProfDt;		///< 计时(单位:ms)
	double m_dProfTotalDt;
	LONGLONG m_llProfKFreq;
};

BASE_NAMESPACE_END_DIAGNOSTICS
BASE_NAMESPACE_END_BASE
#endif