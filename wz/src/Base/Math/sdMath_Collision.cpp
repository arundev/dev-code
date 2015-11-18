#include "sdBasePCH.h"
#include "sdMath.h"
#include "sdRay.h"
#include "sdAxisAlignedBox.h"

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_MATH
//-------------------------------------------------------------------------------------------------
bool sdMath::Intersects(const sdRay& kRay, const sdPlane& kPlane, float& fIntersect, bool bBack)
{	
	// �ж�������ʼ�㵽ƽ�����
	//	1.Ϊ����ʾ������ʼ��ƽ������
	//	2.Ϊ����ʾ������ʼ��ƽ�渺��
	float fRayDistance = kPlane.GetDistance(kRay.GetOrigin());

	// ���������뷨�߼н�
	//	1.Ϊ����ʾ��ƽ�淨��ͬ��
	//	2.Ϊ����ʾ��ƽ�淨������
	float fCosAngle = kPlane.m_kNormal.DotProduct(kRay.GetDirection());

	// ���㽻�㵽��ʼ�����
	//	1.���ߴӸ������������
	//	2.���ߴ�������������
	if (fRayDistance < 0.f && fCosAngle > 0.f && bBack)
	{
		fIntersect = fRayDistance / fCosAngle;	///< ������,���ָ�ֵ
		return true;
	}

	if (fRayDistance > 0.f && fCosAngle < 0.f)	
	{
		fIntersect = -fRayDistance / fCosAngle;	///< ����Ϊ��ֵ
		return true;
	}

	return false;
}
//-------------------------------------------------------------------------------------------------
bool Intersects(const sdRay& kRay, const sdVector3& kV1, const sdVector3& kV2, const sdVector3& kV3, float& fIntersect, bool bBack)
{


	return false;
}
//-------------------------------------------------------------------------------------------------
bool sdMath::Intersects(const sdRay& kRay, const sdAxisAlignedBox& kAABB, float& fStart)
{
	// ���������Χ��
	if (kAABB.IsNull()) 
		return false;

	if (kAABB.IsInfinite()) 
	{
		fStart = 0.0f;
		return true;
	}

	// Ray��ʼ��λ��AABB�ڲ�,ֱ�ӷ���
	const sdVector3& kRayOrigin = kRay.GetOrigin();
	const sdVector3& kRayDir = kRay.GetDirection();
	const sdVector3& kBoxMin = kAABB.GetMinimum();
	const sdVector3& kBoxMax = kAABB.GetMaximum();
	if (kRayOrigin > kBoxMin && kRayOrigin < kBoxMax)
	{
		fStart = 0.0f;
		return true;
	}

	// Check each face in turn, only check closest 3
	// ���μ��AABB�ĸ�����, 
	// @{
	float fMinDistance = 0.0f;	// ��¼��ǰ������С����
	float fDistance = 0.0f;		// ��¼��ǰ�������
	bool bIntersect = false;	// ��¼��ǰ�Ƿ��н���
	sdVector3 kIntersect;		// ��¼��ǰ����

	// Min x
	if (kRayOrigin.m_fX <= kBoxMin.m_fX && kRayDir.m_fX > 0)
	{
		fDistance = (kBoxMin.m_fX - kRayOrigin.m_fX) / kRayDir.m_fX;
		if (fDistance >= 0)
		{
			// ���㽻��,��齻�㷶Χ
			kIntersect = kRayOrigin + kRayDir * fDistance;
			if (kIntersect.m_fY >= kBoxMin.m_fY && kIntersect.m_fY <= kBoxMax.m_fY &&
				kIntersect.m_fZ >= kBoxMin.m_fZ && kIntersect.m_fZ <= kBoxMax.m_fZ &&
				(!bIntersect || fDistance < fMinDistance))
			{
				bIntersect = true;
				fMinDistance = fDistance;
			}
		}
	}

	// Max x
	if (kRayOrigin.m_fX >= kBoxMax.m_fX && kRayDir.m_fX < 0)
	{
		fDistance = (kBoxMax.m_fX - kRayOrigin.m_fX) / kRayDir.m_fX;
		if (fDistance >= 0)
		{
			// Substitute fDistance back into ray and check bounds and dist
			kIntersect = kRayOrigin + kRayDir * fDistance;
			if (kIntersect.m_fY >= kBoxMin.m_fY && kIntersect.m_fY <= kBoxMax.m_fY &&
				kIntersect.m_fZ >= kBoxMin.m_fZ && kIntersect.m_fZ <= kBoxMax.m_fZ &&
				(!bIntersect || fDistance < fMinDistance))
			{
				bIntersect = true;
				fMinDistance = fDistance;
			}
		}
	}

	// Min y
	if (kRayOrigin.m_fY <= kBoxMin.m_fY && kRayDir.m_fY > 0)
	{
		fDistance = (kBoxMin.m_fY - kRayOrigin.m_fY) / kRayDir.m_fY;
		if (fDistance >= 0)
		{
			// Substitute fDistance back into ray and check bounds and dist
			kIntersect = kRayOrigin + kRayDir * fDistance;
			if (kIntersect.m_fX >= kBoxMin.m_fX && kIntersect.m_fX <= kBoxMax.m_fX &&
				kIntersect.m_fZ >= kBoxMin.m_fZ && kIntersect.m_fZ <= kBoxMax.m_fZ &&
				(!bIntersect || fDistance < fMinDistance))
			{
				bIntersect = true;
				fMinDistance = fDistance;
			}
		}
	}

	// Max y
	if (kRayOrigin.m_fY >= kBoxMax.m_fY && kRayDir.m_fY < 0)
	{
		fDistance = (kBoxMax.m_fY - kRayOrigin.m_fY) / kRayDir.m_fY;
		if (fDistance >= 0)
		{
			// Substitute fDistance back into ray and check bounds and dist
			kIntersect = kRayOrigin + kRayDir * fDistance;
			if (kIntersect.m_fX >= kBoxMin.m_fX && kIntersect.m_fX <= kBoxMax.m_fX &&
				kIntersect.m_fZ >= kBoxMin.m_fZ && kIntersect.m_fZ <= kBoxMax.m_fZ &&
				(!bIntersect || fDistance < fMinDistance))
			{
				bIntersect = true;
				fMinDistance = fDistance;
			}
		}
	}

	// Min z
	if (kRayOrigin.m_fZ <= kBoxMin.m_fZ && kRayDir.m_fZ > 0)
	{
		fDistance = (kBoxMin.m_fZ - kRayOrigin.m_fZ) / kRayDir.m_fZ;
		if (fDistance >= 0)
		{
			// Substitute fDistance back into ray and check bounds and dist
			kIntersect = kRayOrigin + kRayDir * fDistance;
			if (kIntersect.m_fX >= kBoxMin.m_fX && kIntersect.m_fX <= kBoxMax.m_fX &&
				kIntersect.m_fY >= kBoxMin.m_fY && kIntersect.m_fY <= kBoxMax.m_fY &&
				(!bIntersect || fDistance < fMinDistance))
			{
				bIntersect = true;
				fMinDistance = fDistance;
			}
		}
	}

	// Max z
	if (kRayOrigin.m_fZ >= kBoxMax.m_fZ && kRayDir.m_fZ < 0)
	{
		fDistance = (kBoxMax.m_fZ - kRayOrigin.m_fZ) / kRayDir.m_fZ;
		if (fDistance >= 0)
		{
			// Substitute fDistance back into ray and check bounds and dist
			kIntersect = kRayOrigin + kRayDir * fDistance;
			if (kIntersect.m_fX >= kBoxMin.m_fX && kIntersect.m_fX <= kBoxMax.m_fX &&
				kIntersect.m_fY >= kBoxMin.m_fY && kIntersect.m_fY <= kBoxMax.m_fY &&
				(!bIntersect || fDistance < fMinDistance))
			{
				bIntersect = true;
				fMinDistance = fDistance;
			}
		}
	}
	// @}

	fStart = fMinDistance;
	return bIntersect;
} 
//-----------------------------------------------------------------------------------------
bool sdMath::Intersects(const sdRay& kRay, const sdAxisAlignedBox& kAABB, float& fStart, float& fEnd)
{
	// ���������Χ��
	if (kAABB.IsNull())
		return false;

	if (kAABB.IsInfinite())
	{
		fStart = 0;
		fEnd = POS_INFINITY;
		return true;
	}

	//
	const sdVector3& kRayOrigin = kRay.GetOrigin();
	const sdVector3& kRayDir = kRay.GetDirection();
	const sdVector3& kBoxMin = kAABB.GetMinimum();
	const sdVector3& kBoxMax = kAABB.GetMaximum();

	// ��Ray�����������շ�������ֵ��С��������,
	sdVector3 kAbsDir;
	kAbsDir[0] = sdMath::Abs(kRayDir[0]);
	kAbsDir[1] = sdMath::Abs(kRayDir[1]);
	kAbsDir[2] = sdMath::Abs(kRayDir[2]);

	int iMax = 0, iMid = 1, iMin = 2;
	if (kAbsDir[0] < kAbsDir[2])
	{
		iMax = 2;
		iMin = 0;
	}
	if (kAbsDir[1] < kAbsDir[iMin])
	{
		iMid = iMin;
		iMin = 1;
	}
	else if (kAbsDir[1] > kAbsDir[iMax])
	{
		iMid = iMax;
		iMax = 1;
	}

	// ����Ray����������ǿ�ȴ������С���д���
	// @{
	fStart = 0;
	fEnd = sdMath::POS_INFINITY;

#define _CALC_AXIS(i)										\
do {														\
float fDenom = 1 / kRayDir[i];								\
float fNewStart = (kBoxMin[i] - kRayOrigin[i]) * fDenom;	\
float fNewEnd = (kBoxMax[i] - kRayOrigin[i]) * fDenom;      \
if (fNewStart > fNewEnd) std::swap(fNewStart, fNewEnd);		\
if (fNewStart > fEnd || fNewEnd < fStart) return false;		\
if (fNewStart > fStart) fStart = fNewStart;					\
if (fNewEnd < fEnd) fEnd = fNewEnd;							\
} while(0)

	// �����������
	_CALC_AXIS(iMax);

	if (kAbsDir[iMid] < std::numeric_limits<float>::epsilon())
	{
		// ����ƽ���ڴδ������С��
		if (kRayOrigin[iMid] < kBoxMin[iMid] || kRayOrigin[iMid] > kBoxMax[iMid] ||
			kRayOrigin[iMin] < kBoxMin[iMin] || kRayOrigin[iMin] > kBoxMax[iMin])
			return false;
	}
	else
	{
		// ����δ�����
		_CALC_AXIS(iMid);

		if (kAbsDir[iMin] < std::numeric_limits<float>::epsilon())
		{
			// ����ƽ������С��
			if (kRayOrigin[iMin] < kBoxMin[iMin] || kRayOrigin[iMin] > kBoxMax[iMin])
				return false;
		}
		else
		{
			// ������С����
			_CALC_AXIS(iMin);
		}
	}
#undef _CALC_AXIS
	// @}

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdMath::Intersects_V2(const sdRay& kRay, const sdAxisAlignedBox& kAABB, float& fStart, float& fEnd)
{
	// ���������Χ��
	if (kAABB.IsNull())
		return false;

	if (kAABB.IsInfinite())
	{
		fStart = 0.f;
		fEnd = POS_INFINITY;
		return true;
	}

	// ȡ��AABB����������ı���
	sdPlane akPlane[6];
	akPlane[0] = sdPlane(sdVector3::UNIT_X, sdVector3(kAABB.GetMaximum().m_fX, 0.f, 0.f));
	akPlane[1] = sdPlane(sdVector3::UNIT_Y, sdVector3(0.f, kAABB.GetMaximum().m_fY, 0.f));
	akPlane[2] = sdPlane(sdVector3::UNIT_Z, sdVector3(0.f, 0.f, kAABB.GetMaximum().m_fZ));
	akPlane[3] = sdPlane(-sdVector3::UNIT_X, sdVector3(kAABB.GetMinimum().m_fX, 0.f, 0.f));
	akPlane[4] = sdPlane(-sdVector3::UNIT_Y, sdVector3(0.f, kAABB.GetMinimum().m_fY, 0.f));
	akPlane[5] = sdPlane(-sdVector3::UNIT_Z, sdVector3(0.f, 0.f, kAABB.GetMinimum().m_fZ));

	// ������������ֱ���
	bool bInsideBox = true;
	bool abIntersect[6] = { false };		///< �ཻ���(�Ƿ��ཻ)
	bool abIntersectStatus[6] = { false };	///< �ཻ���(���仹�����)
	float afDistance[6] = { 0.f };			///< �ཻ����
	for (int i = 0; i < 6; ++i)
	{
		sdPlane& kPlane = akPlane[i];

		// �ж�������ʼ���Ƿ�λ��ƽ�渺��
		// (ֻҪ������ʼ��������һ��ƽ�������,��ô��Ȼ����AABB�ڲ�)
		float fRayDistance = kPlane.GetDistance(kRay.GetOrigin());
		if (fRayDistance > 0.f)
			bInsideBox = false;	

		// ���������뷨�߼н�
		float fCosAngle = kPlane.m_kNormal.DotProduct(kRay.GetDirection());

		// ���㽻�㵽��ʼ�����
		//	1.���ߴӸ������������
		//	2.���ߴ�������������
		if (fRayDistance < 0.f && fCosAngle > 0.f)
		{
			abIntersect[i] = true;
			abIntersectStatus[i] = false;
			afDistance[i] = -fRayDistance / fCosAngle;	///< ����Ϊ��ֵ
		}
		
		if (fRayDistance > 0.f && fCosAngle < 0.f)	
		{
			abIntersect[i] = true;
			abIntersectStatus[i] = true;
			afDistance[i] = -fRayDistance / fCosAngle;	///< ����Ϊ��ֵ
		}
	}

	// ��������������Զ����
	if (bInsideBox)
	{
		fStart = 0.f;
		fEnd = FLT_MAX;		///< ȡ������������
		for (int i = 0; i < 6; ++i)
		{
			if (!abIntersect[i])
				continue;

			if (!abIntersectStatus[i] && afDistance[i] < fEnd)
			{
				fEnd = afDistance[i];
			}
		}

		return true;
	}
	else
	{
		fStart = -FLT_MAX;	///< ȡ�������Զ����
		fEnd = FLT_MAX;		///< ȡ������������
		for (int i = 0; i < 6; ++i)
		{
			if (!abIntersect[i])
				continue;

			if (abIntersectStatus[i])
			{
				if (afDistance[i] > fStart)
					fStart = afDistance[i];
			}
			else
			{
				if (afDistance[i] < fEnd)
					fEnd = afDistance[i];	
			}
		}

		// û�н���
		if (fStart == -FLT_MAX || fEnd == FLT_MAX)
			return false;

		return true;
	}
}
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_MATH
BASE_NAMESPACE_END_BASE