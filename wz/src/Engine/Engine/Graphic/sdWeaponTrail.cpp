#include "sdEnginePCH.h"
#include "sdWeaponTrail.h"

ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
NiImplementRTTI(sdWeaponTrail, NiNode);
//-------------------------------------------------------------------------------------------------
void sdWeaponTrail::UpdateFrame(float fAccumTime, float fFrameTime)
{
	if (!m_bStarted)
		return;

	// 更新速率过高会导致刀光变短,这里暂时限制为60fps
	// (\TODO: 考虑改进为基于时间插值)
	m_fAccumFrameTime += fFrameTime;
	const static float fLimitFrameTime = 1.f / 60.f;
	if (m_fAccumFrameTime < fLimitFrameTime)
		return;		

	// 收尾阶段逐步降低控制点数量,直到数量为0时停止更新
	if (m_bStoping)
	{
		m_iCtrlPtNum -= ceil(m_iCtrlPtNum * m_fAccumFrameTime);
		if (m_iCtrlPtNum < 0)
			m_iCtrlPtNum = 0;

		if (m_iCtrlPtNum == 0)
		{
			m_spMesh->SetAppCulled(true);
			m_bStarted = false;
			m_bStoping = false;

			return;
		}
	}

	// 更新累计时间
	m_fAccumFrameTime = fmodf(m_fAccumFrameTime, fLimitFrameTime);

	// 计算新加入的控制点位置
	NiPoint3 kPos0 = m_kDummyPosition + m_kDummyZDirection * (m_fBias + m_fWidth * 0.5f);
	NiPoint3 kPos1 = m_kDummyPosition + m_kDummyZDirection * (m_fBias - m_fWidth * 0.5f);

	// 计算新加控制点与上一组控制点的最大距离
	int iCtrlPtSize = m_kCtrlPtVec0.size();
	int iLastCtrlPtIndex = m_iStartCtrlPtIndex + m_iCtrlPtNum - 1;
	if (iLastCtrlPtIndex >= iCtrlPtSize)
		iLastCtrlPtIndex -= iCtrlPtSize;

	NiPoint3 kVec0 = kPos0 - m_kCtrlPtVec0[iLastCtrlPtIndex];
	NiPoint3 kVec1 = kPos1 - m_kCtrlPtVec1[iLastCtrlPtIndex];
	float fLength0 = kVec0.Length();
	float fLength1 = kVec1.Length();
	if (fLength0 < fLength1)
		fLength0 = fLength1;

	// 控制点加入缓存
	if (m_iCtrlPtNum < iCtrlPtSize && !m_bStoping)
	{
		// 控制点缓存没有满,继续向末尾加入控制点
		++m_iCtrlPtNum;

		int iLastCtrlPtIndex = m_iStartCtrlPtIndex + m_iCtrlPtNum - 1;
		if (iLastCtrlPtIndex >= iCtrlPtSize)
			iLastCtrlPtIndex -= iCtrlPtSize;

		m_kCtrlPtVec0[iLastCtrlPtIndex] = kPos0;
		m_kCtrlPtVec1[iLastCtrlPtIndex] = kPos1;
		//m_kCtrlSegmentLength[iLastCtrlPtIndex] = fLength0;
	}
	else
	{
		// 控制点缓存已满,抛弃最老数据,向末尾加入控制点
		++m_iStartCtrlPtIndex;
		if (m_iStartCtrlPtIndex >= iCtrlPtSize)
			m_iStartCtrlPtIndex -= iCtrlPtSize;

		int iLastCtrlPtIndex = m_iStartCtrlPtIndex + m_iCtrlPtNum - 1;
		if (iLastCtrlPtIndex >= iCtrlPtSize)
			iLastCtrlPtIndex -= iCtrlPtSize;

		m_kCtrlPtVec0[iLastCtrlPtIndex] = kPos0;
		m_kCtrlPtVec1[iLastCtrlPtIndex] = kPos1;
		//m_kCtrlSegmentLength[iLastCtrlPtIndex] = fLength0;
	}

	// 
}
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM