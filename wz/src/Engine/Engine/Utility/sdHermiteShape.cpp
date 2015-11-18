#include "sdEnginePCH.h"
#include "sdHermiteShape.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_UTIL
//---------------------------------------------------------------------------------------------
sdHermiteShape::sdHermiteShape()
: m_fExtendWidth(6.f)	///< 不理解这个值
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
	// 清理插值生成点
	m_kInterPointVec.clear();

	// 检查控制点数
	int iCtrlNum = m_kCtrlPointVec.size();
	if (iCtrlNum < 2)
		return false;

	// 起始点压入
	m_kInterPointVec.push_back(m_kCtrlPointVec[0]);

	// 循环处理,在每两个控制点之间进行插入处理(注意两端)
	//	1.计算两端切线
	//	2.使用Cubic Hermite Spline进行插值处理
	NiPoint3* spPtCur0 = &(m_kCtrlPointVec[0]);
	NiPoint3* spPtCur1 = &(m_kCtrlPointVec[1]);
	NiPoint3 kPtCurV0, kPtCurV1;
	for (int i = 0; i < iCtrlNum - 1; ++i)
	{
		// 1
		NiPoint3 kVPrev;
		NiPoint3 kV = *spPtCur1 - *spPtCur0;
		float fV = kV.Unitize();	///< 计算区间长度,用于缩放切线到[V0,V1]区间
		if (i - 1 < 0)
		{
			kVPrev = kV;
		}
		else
		{
			kVPrev = *spPtCur0 - *(spPtCur0 - 1);
			kVPrev.Unitize();
		}
		kPtCurV0 = (kVPrev + kV) * fV / 1.3f;	///< 1.3为曲率因子(经验值)

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
		kPtCurV1 = (kV + kVNext) * fV / 1.3f;	///< 1.3为曲率因子(经验值)

		// 2
		int iInterNum = m_iMaxInterNum;
		if (iInterNum < 0)
			iInterNum = (int)(fV / 6.f);	///< (不解???)
		
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
	// 清理扩展生成点
	m_kExtendPointVec.clear();

	// 检查插值点数
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