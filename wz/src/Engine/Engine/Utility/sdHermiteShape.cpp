#include "sdEnginePCH.h"
#include "sdHermiteShape.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_UTIL
//---------------------------------------------------------------------------------------------
sdHermiteShape::sdHermiteShape()
: m_fExtendWidth(6.f)	///< ��������ֵ
, m_iMaxInterNum(-1)
{

}
//---------------------------------------------------------------------------------------------
sdHermiteShape::~sdHermiteShape()
{

}
//---------------------------------------------------------------------------------------------
bool sdHermiteShape::Interpolation()
{
	// �����ֵ���ɵ�
	m_kInterPointVec.clear();

	// �����Ƶ���
	int iCtrlNum = m_kCtrlPointVec.size();
	if (iCtrlNum < 2)
		return false;

	// ��ʼ��ѹ��
	m_kInterPointVec.push_back(m_kCtrlPointVec[0]);

	// ѭ������,��ÿ�������Ƶ�֮����в��봦��(ע������)
	//	1.������������
	//	2.ʹ��Cubic Hermite Spline���в�ֵ����
	NiPoint3* spPtCur0 = &(m_kCtrlPointVec[0]);
	NiPoint3* spPtCur1 = &(m_kCtrlPointVec[1]);
	NiPoint3 kPtCurV0, kPtCurV1;
	for (int i = 0; i < iCtrlNum - 1; ++i)
	{
		// 1
		NiPoint3 kVPrev;
		NiPoint3 kV = *spPtCur1 - *spPtCur0;
		float fV = kV.Unitize();	///< �������䳤��,�����������ߵ�[V0,V1]����
		if (i - 1 < 0)
		{
			kVPrev = kV;
		}
		else
		{
			kVPrev = *spPtCur0 - *(spPtCur0 - 1);
			kVPrev.Unitize();
		}
		kPtCurV0 = (kVPrev + kV) * fV / 1.3f;	///< 1.3Ϊ��������(����ֵ)

		NiPoint3 kVNext;
		if (i + 2 >= iCtrlNum)
		{
			kVNext = kV;
		}
		else
		{
			kVNext = *(spPtCur1 + 1) - *spPtCur1;
			kVNext.Unitize();
		}
		kPtCurV1 = (kV + kVNext) * fV / 1.3f;	///< 1.3Ϊ��������(����ֵ)

		// 2
		int iInterNum = m_iMaxInterNum;
		if (iInterNum < 0)
			iInterNum = (int)(fV / 6.f);	///< (����???)
		
		float fDelta = 1.f / iInterNum;
		float fBias = fDelta / 5.f;
		for (float t = fDelta; t <=  1.f - fBias; t += fDelta)
		{
			float t2 = t * t;
			float t3 = t2 * t;

			float h00 = 2*t3 - 3*t2 + 1.f;	///< 2*t*t*t - 3*t*t + 1.f
			float h10 = t3 - 2*t2 + t;		///< t*t*t - 2*t*t + t
			float h01 = -2*t3 + 3*t2;		///< -2*t*t*t + 3*t*t
			float h11 = t3 - t2;			///< t*t*t - t*t

			NiPoint3 pos = h00 * (*spPtCur0) + h10 * kPtCurV0 + 
						   h01 * (*spPtCur1) + h11 * kPtCurV1;
			m_kInterPointVec.push_back(pos);
		}

		//
		m_kInterPointVec.push_back(*spPtCur1);

		//
		++spPtCur0;
		++spPtCur1;
	}

	return true;
}
//---------------------------------------------------------------------------------------------
bool sdHermiteShape::ExtentInterPoint()
{
	// ������չ���ɵ�
	m_kExtendPointVec.clear();

	// ����ֵ����
	int iInterNum = m_kInterPointVec.size();
	if (iInterNum < 2)
		return false;

	//
	float fHalfWidth = m_fExtendWidth / 2.f;	
	NiPoint3* spPtCur0 = &(m_kCtrlPointVec[0]);
	NiPoint3* spPtCur1 = &(m_kCtrlPointVec[1]);
	NiPoint3* spPtCur = spPtCur0;
	for (int i = 0; i < iInterNum; ++i)
	{
		NiPoint3 kV = ((*spPtCur1) - (*spPtCur0)).UnitCross(NiPoint3::UNIT_Z);
		m_kExtendPointVec.push_back((*spPtCur) + kV * fHalfWidth);
		m_kExtendPointVec.push_back((*spPtCur) - kV * fHalfWidth);

		++spPtCur0;
		++spPtCur1;
		++spPtCur;
	}

	return true;
}
//---------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE
ENGINE_NAMESPACE_END_UTIL