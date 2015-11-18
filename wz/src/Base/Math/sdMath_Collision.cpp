#include "sdBasePCH.h"
#include "sdMath.h"
#include "sdRay.h"
#include "sdAxisAlignedBox.h"

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_MATH
//-------------------------------------------------------------------------------------------------
bool sdMath::Intersects(const sdRay& kRay, const sdPlane& kPlane, float& fIntersect, bool bBack)
{	
	// 判断射线起始点到平面距离
	//	1.为正表示射线起始于平面正面
	//	2.为负表示射线起始于平面负面
	float fRayDistance = kPlane.GetDistance(kRay.GetOrigin());

	// 计算射线与法线夹角
	//	1.为正表示与平面法线同向
	//	2.为负表示与平面法线异向
	float fCosAngle = kPlane.m_kNormal.DotProduct(kRay.GetDirection());

	// 计算交点到起始点距离
	//	1.射线从负面向正面射出
	//	2.射线从正面向负面射入
	if (fRayDistance < 0.f && fCosAngle > 0.f && bBack)
	{
		fIntersect = fRayDistance / fCosAngle;	///< 不纠正,保持负值
		return true;
	}

	if (fRayDistance > 0.f && fCosAngle < 0.f)	
	{
		fIntersect = -fRayDistance / fCosAngle;	///< 纠正为正值
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
	// 处理特殊包围盒
	if (kAABB.IsNull()) 
		return false;

	if (kAABB.IsInfinite()) 
	{
		fStart = 0.0f;
		return true;
	}

	// Ray起始点位于AABB内部,直接返回
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
	// 依次检查AABB的各个面, 
	// @{
	float fMinDistance = 0.0f;	// 记录当前交点最小距离
	float fDistance = 0.0f;		// 记录当前交点距离
	bool bIntersect = false;	// 记录当前是否有交点
	sdVector3 kIntersect;		// 记录当前交点

	// Min x
	if (kRayOrigin.m_fX <= kBoxMin.m_fX && kRayDir.m_fX > 0)
	{
		fDistance = (kBoxMin.m_fX - kRayOrigin.m_fX) / kRayDir.m_fX;
		if (fDistance >= 0)
		{
			// 计算交点,检查交点范围
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
	// 处理特殊包围盒
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

	// 对Ray的三个方向按照分量绝对值大小进行排序,
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

	// 按照Ray的三个方向强度从最大到最小进行处理
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

	// 计算最大轴向
	_CALC_AXIS(iMax);

	if (kAbsDir[iMid] < std::numeric_limits<float>::epsilon())
	{
		// 射线平行于次大轴和最小轴
		if (kRayOrigin[iMid] < kBoxMin[iMid] || kRayOrigin[iMid] > kBoxMax[iMid] ||
			kRayOrigin[iMin] < kBoxMin[iMin] || kRayOrigin[iMin] > kBoxMax[iMin])
			return false;
	}
	else
	{
		// 计算次大轴向
		_CALC_AXIS(iMid);

		if (kAbsDir[iMin] < std::numeric_limits<float>::epsilon())
		{
			// 射线平行于最小轴
			if (kRayOrigin[iMin] < kBoxMin[iMin] || kRayOrigin[iMin] > kBoxMax[iMin])
				return false;
		}
		else
		{
			// 计算最小轴向
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
	// 处理特殊包围盒
	if (kAABB.IsNull())
		return false;

	if (kAABB.IsInfinite())
	{
		fStart = 0.f;
		fEnd = POS_INFINITY;
		return true;
	}

	// 取出AABB的六个向外的表面
	sdPlane akPlane[6];
	akPlane[0] = sdPlane(sdVector3::UNIT_X, sdVector3(kAABB.GetMaximum().m_fX, 0.f, 0.f));
	akPlane[1] = sdPlane(sdVector3::UNIT_Y, sdVector3(0.f, kAABB.GetMaximum().m_fY, 0.f));
	akPlane[2] = sdPlane(sdVector3::UNIT_Z, sdVector3(0.f, 0.f, kAABB.GetMaximum().m_fZ));
	akPlane[3] = sdPlane(-sdVector3::UNIT_X, sdVector3(kAABB.GetMinimum().m_fX, 0.f, 0.f));
	akPlane[4] = sdPlane(-sdVector3::UNIT_Y, sdVector3(0.f, kAABB.GetMinimum().m_fY, 0.f));
	akPlane[5] = sdPlane(-sdVector3::UNIT_Z, sdVector3(0.f, 0.f, kAABB.GetMinimum().m_fZ));

	// 射线与六个面分别求交
	bool bInsideBox = true;
	bool abIntersect[6] = { false };		///< 相交情况(是否相交)
	bool abIntersectStatus[6] = { false };	///< 相交情况(入射还是射出)
	float afDistance[6] = { 0.f };			///< 相交距离
	for (int i = 0; i < 6; ++i)
	{
		sdPlane& kPlane = akPlane[i];

		// 判断射线起始点是否位于平面负面
		// (只要射线起始点在任意一个平面的正面,那么必然不在AABB内部)
		float fRayDistance = kPlane.GetDistance(kRay.GetOrigin());
		if (fRayDistance > 0.f)
			bInsideBox = false;	

		// 计算射线与法线夹角
		float fCosAngle = kPlane.m_kNormal.DotProduct(kRay.GetDirection());

		// 计算交点到起始点距离
		//	1.射线从负面向正面射出
		//	2.射线从正面向负面射入
		if (fRayDistance < 0.f && fCosAngle > 0.f)
		{
			abIntersect[i] = true;
			abIntersectStatus[i] = false;
			afDistance[i] = -fRayDistance / fCosAngle;	///< 纠正为正值
		}
		
		if (fRayDistance > 0.f && fCosAngle < 0.f)	
		{
			abIntersect[i] = true;
			abIntersectStatus[i] = true;
			afDistance[i] = -fRayDistance / fCosAngle;	///< 纠正为正值
		}
	}

	// 计算最近交点和最远交点
	if (bInsideBox)
	{
		fStart = 0.f;
		fEnd = FLT_MAX;		///< 取出射的最近交点
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
		fStart = -FLT_MAX;	///< 取入射的最远焦点
		fEnd = FLT_MAX;		///< 取出射的最近交点
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

		// 没有交点
		if (fStart == -FLT_MAX || fEnd == FLT_MAX)
			return false;

		return true;
	}
}
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_MATH
BASE_NAMESPACE_END_BASE