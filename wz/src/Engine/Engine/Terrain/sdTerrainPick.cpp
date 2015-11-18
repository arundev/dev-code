#include "sdEnginePCH.h"
#include "sdTerrainPick.h"

//
#include <sdMath.h>
#include <sdRay.h>
#include <sdAxisAlignedBox.h>

using namespace Base::Math;
//-------------------------------------------------------------------------------------------------
sdTerrainPick::sdTerrainPick()
: m_bCullBack(true)
, m_fMaxDistance(FLT_MAX)
{

}
//-------------------------------------------------------------------------------------------------
sdTerrainPick::sdTerrainPick(sdHeightMap* pkHeightMap)
: m_pkHeightMap(pkHeightMap)
, m_bCullBack(true)
, m_fMaxDistance(FLT_MAX)
{
	NIASSERT(pkHeightMap);
}
//-------------------------------------------------------------------------------------------------
bool sdTerrainPick::Pick(const sdRay& kRay, sdVector3& kIntersect)
{
	// �����Χ��
	uint uiSize = m_pkHeightMap->GetSize();
	sdAxisAlignedBox kAABB;
	kAABB.SetMinimum(0, 0, m_pkHeightMap->GetMinHeight());
	kAABB.SetMaximum((float)uiSize, (float)uiSize, m_pkHeightMap->GetMaxHeight());

	// ����Ray��HeightMap��AABB�Ľ���
	float fStart = 0.0f;
	float fEnd = 0.0f;
	bool bIntersect = sdMath::Intersects(kRay, kAABB, fStart, fEnd);
	//bool bIntersect = sdMath::Intersects_V2(kRay, kAABB, fStart, fEnd);
	if (!bIntersect)
		return false;

	// ǯλ��Զ����
	if (fEnd > m_fMaxDistance)
		fEnd = m_fMaxDistance;

	// ����ʼ�㵽��ֹ����й̶�����΢���Ա������о����ĸ��ӽ���������
	// (ԭʼֵ��0.5f,���˾���Ӧ����0.99999f)
	float fSampleDelta = 0.99999f;
	sdVector3 kDeltaPos(fSampleDelta * kRay.GetDirection());					///< ����ֵ
	sdVector3 kDeltaStartPos(kRay.GetOrigin() + fStart * kRay.GetDirection());	///< ��ʼλ��

	uint uiLastX = UINT_MAX;
	uint uiLastY = UINT_MAX;
	uint uiX = UINT_MAX;
	uint uiY = UINT_MAX;
	for (bool bFirst = true;fStart <= fEnd; fStart += fSampleDelta, bFirst = false)
	{
		// ���뵽դ��
		uiX = (uint)kDeltaStartPos.m_fX;
		uiY = (uint)kDeltaStartPos.m_fY;
		
		// ֻ�е�ǰ������һ�㲻λ��ͬһ���߶ȸ����ڲŽ��д���
		//   +---+---+
		//   |   |   |
		//	 |   |   |
		//	 +---+---+
		//   |   |   |
		//	 |   |   |
		//	 +---+---+
		// (�ܸо����ﲻ����,ò��Ӧ��������һ��)
		if (uiX != uiLastX || uiY != uiLastY)
		{
			// ��ǰ������һ��λ��б�Խǵ���������,�������������Խǵĸ���
			// (�����ж���������Խ��������ж����ĸ������ཻ)
			// (���ܲ�©)
			if (uiX != uiLastX && uiY != uiLastY && bFirst == false)
			{
				if (Intersect(uiX, uiLastY, kRay, kIntersect))
					return true;

				if (Intersect(uiLastX, uiY, kRay, kIntersect))
					return true;
			}

			// ����ǰ����
			if (Intersect(uiX, uiY, kRay, kIntersect))
				return true;

			//
			uiLastX = uiX;
			uiLastY = uiY;
		}

		// ��һ����
		kDeltaStartPos += kDeltaPos;
	}

	return false;
}
//-------------------------------------------------------------------------------------------------
bool sdTerrainPick::Intersect(uint uiX, uint uiY, const sdRay& kRay, sdVector3& kIntersect)
{
	if (uiX >= m_pkHeightMap->GetSize() || uiY >= m_pkHeightMap->GetSize())
		return false;

	// ��������֯��ʽ
	// (����û������,�п�������Ƶ���֯��ʽ��һ��)
	//	1--3
	//	|\ |
	//  | \|
	//  0--2
	//
	// ��ȡ�ĸ���
	sdVector3 kPt0((float)uiX,		(float)uiY,		m_pkHeightMap->GetRawHeight(uiX, uiY));
	sdVector3 kPt1((float)uiX,		(float)(uiY+1),	m_pkHeightMap->GetRawHeight(uiX, uiY+1));
	sdVector3 kPt2((float)(uiX+1),	(float)uiY,		m_pkHeightMap->GetRawHeight(uiX+1, uiY));
	sdVector3 kPt3((float)(uiX+1),	(float)(uiY+1),	m_pkHeightMap->GetRawHeight(uiX+1, uiY+1));

	// �ֱ���0-1-2��2-1-3��
	sdVector3 kIntersectLeft, kIntersectRight;
	bool bLeft = Intersect(kPt0, kPt1, kPt2, kRay, kIntersectLeft);
	bool bRight = Intersect(kPt2, kPt1, kPt3, kRay, kIntersectRight);

	// �жϽ��
	// (������Ը���Ray���ж����ཻ��������,�Ӷ����ܼ�������Intersect)
	if (bLeft && bRight)
	{
		// ���������ཻ��ȡ���ֵ
		float fLeft = (kIntersectLeft - kRay.GetOrigin()).SquaredLength();
		float fRight = (kIntersectLeft - kRay.GetOrigin()).SquaredLength();
		kIntersect = fLeft < fRight ? kIntersectLeft : kIntersectRight;
		return true;
	}
	else if (bLeft || bRight)
	{
		// ֻ��һ���ཻȡ�ֵཻ
		kIntersect = bLeft ? kIntersectLeft : kIntersectRight;
		return true;
	}
	else
	{
		// ���ཻ
		return false;
	}
}
//-------------------------------------------------------------------------------------------------
bool sdTerrainPick::Intersect(const sdVector3& kV1, const sdVector3& kV2, const sdVector3& kV3, const sdRay& kRay, sdVector3& kIntersect)
{
	//
	// �ж��ַ���:
	//	1.����������ƽ�潻��,Ȼ��������ͶӰ��һ��ƽ��(XY/YZ/ZX),�ж�2ά���Ƿ���2ά�������ڲ�
	//	2.�ο�DX��Pick���Ӻ͡�ʵʱ���㼰ͼ��ѧ����P322
	// ������WZ���㷨��ʹ���˵�2�ֵ�һ������
	//
	// ����Triangle��ʸ���ͷ���(������������������濴˳ʱ��˳����)
	sdVector3 kEdge1 = kV2 - kV1;
	sdVector3 kEdge2 = kV3 - kV1;
	
	// ����
	const sdVector3& kOrigin = kRay.GetOrigin();
	const sdVector3& kDir = kRay.GetDirection();

	sdVector3 kP = kDir.CrossProduct(kEdge2);
	float fDet = kEdge1.DotProduct(kP);
	float fR, fS, fT;
	float fTolerance = 1e-05f;
	if (fDet <= -fTolerance)
	{
		// ����̴�����������ƽ��
		sdVector3 kS = kOrigin - kV1;
		fS = kS.DotProduct(kP);
		if (fS > 0.0f || fS < fDet)
			return false;

		sdVector3 kQ = kS.CrossProduct(kEdge1);
		fT = kDir.DotProduct(kQ);
		if (fT > 0.0f || fS + fT < fDet)
			return false;

		fR = kEdge2.DotProduct(kQ);
		if (fR > 0.0f)
			return false;
	}
	else if (fDet >= fTolerance)
	{
		// ����̴�����������ƽ��
		//
		// �����б�����
		if (m_bCullBack)
			return false;

		sdVector3 kS = kOrigin - kV1;
		fS = kS.DotProduct(kP);
		if (fS < 0.0f || fS > fDet)
			return false;

		sdVector3 kQ = kS.CrossProduct(kEdge1);
		fT = kDir.DotProduct(kQ);
		if (fT < 0.0f || fS + fT > fDet)
			return false;

		fR = kEdge2.DotProduct(kQ);
		if (fR < 0.0f)
			return false;
	}
	else
	{
		// ����������������ƽ��ƽ��
		return false;
	}

	// ���㽻��
	float fInvDet = 1.f / fDet;
	fS *= fInvDet;
	fT *= fInvDet;
	fR *= fInvDet;

	kIntersect = kOrigin + fR * kDir;
	
	return true;
}
//-------------------------------------------------------------------------------------------------