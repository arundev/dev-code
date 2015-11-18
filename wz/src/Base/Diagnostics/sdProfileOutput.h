//*************************************************************************************************
// ����:	��Ϊ��ȷ�����ܷ�������(Ŀǰ����ResourceSystem������Դ���ؼ�ʱ)
//---------------------------------------------------------
// ����:		
// ����:		2013-02-04
// ����޸�:
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
	LARGE_INTEGER m_liProfFreq;			///< ��ʱ��Ƶ��(��λ:count/second)
	LARGE_INTEGER m_liProfCountStart;	///< ��ʼ��ʱ(��λ:Count)
	LARGE_INTEGER m_liProfCountEnd;		///< ��ֹ��ʱ(��λ:Count)
	double m_dProfDt;		///< ��ʱ(��λ:ms)
	double m_dProfTotalDt;
	LONGLONG m_llProfKFreq;
};

BASE_NAMESPACE_END_DIAGNOSTICS
BASE_NAMESPACE_END_BASE
#endif