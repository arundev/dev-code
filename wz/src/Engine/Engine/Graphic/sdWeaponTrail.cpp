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

	// �������ʹ��߻ᵼ�µ�����,������ʱ����Ϊ60fps
	// (\TODO: ���ǸĽ�Ϊ����ʱ���ֵ)
	m_fAccumFrameTime += fFrameTime;
	const static float fLimitFrameTime = 1.f / 60.f;
	if (m_fAccumFrameTime < fLimitFrameTime)
		return;		

	// ��β�׶��𲽽��Ϳ��Ƶ�����,ֱ������Ϊ0ʱֹͣ����
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

	// �����ۼ�ʱ��
	m_fAccumFrameTime = fmodf(m_fAccumFrameTime, fLimitFrameTime);

	// �����¼���Ŀ��Ƶ�λ��
	NiPoint3 kPos0 = m_kDummyPosition + m_kDummyZDirection * (m_fBias + m_fWidth * 0.5f);
	NiPoint3 kPos1 = m_kDummyPosition + m_kDummyZDirection * (m_fBias - m_fWidth * 0.5f);

	// �����¼ӿ��Ƶ�����һ����Ƶ��������
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

	// ���Ƶ���뻺��
	if (m_iCtrlPtNum < iCtrlPtSize && !m_bStoping)
	{
		// ���Ƶ㻺��û����,������ĩβ������Ƶ�
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
		// ���Ƶ㻺������,������������,��ĩβ������Ƶ�
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