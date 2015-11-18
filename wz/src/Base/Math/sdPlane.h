//*************************************************************************************************
// 内容:	抽取自Ogre::OgrePlane.h
//---------------------------------------------------------
// 作者:		
// 创建:		2013-05-16
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_MATH_PLANE_H__
#define _SD_BASE_MATH_PLANE_H__
#include "sdMath.h"
#include "sdVector3.h"
#include "sdAxisAlignedBox.h"

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_MATH
//-------------------------------------------------------------------------------------------------
class sdPlane : public sdMemObject
{
public:
	sdVector3 m_kNormal;	///< 平面方程前三项,平面法线(要求归一化)
	float m_fDistance;		///< 平面方程常数项,平面到原点的距离

public:
	enum Side
	{
		E_NO_SIDE,			///< 平面上	
		E_POSITIVE_SIDE,	///< 正面
		E_NEGATIVE_SIDE,	///< 负面
		E_BOTH_SIDE,		///< 相交		
	};

public:
	inline sdPlane()
	{
		m_kNormal = sdVector3::ZERO;
		m_fDistance = 0.f;
	}
	
	inline sdPlane(const sdPlane& rhs)
	{
		m_kNormal = rhs.m_kNormal;
		m_fDistance = rhs.m_fDistance;
	}
	
	inline sdPlane(const sdVector3& kNormal, float fDistance)
	{
		m_kNormal = kNormal;
		m_fDistance = m_fDistance;
	}

	inline sdPlane(float a, float b, float c, float d)
		: m_kNormal(a, b, c), m_fDistance(d)
	{
	}
	
	inline sdPlane(const sdVector3& rkNormal, const sdVector3& rkPoint)
	{
		Redefine(rkNormal, rkPoint);
	}
	
	inline sdPlane(const sdVector3& kPoint0, const sdVector3& kPoint1, const sdVector3& kPoint2)
	{
		Redefine(kPoint0, kPoint1, kPoint2);
	}

	inline float GetDistance(const sdVector3& kPoint) const
	{
		return m_kNormal.DotProduct(kPoint) + m_fDistance;
	}
	
	sdPlane::Side GetSide(const sdVector3& kPoint) const
	{
		float fDistance = GetDistance(kPoint);
		if (fDistance < 0.f)
			return sdPlane::E_NEGATIVE_SIDE;
		if (fDistance > 0.f)
			return sdPlane::E_POSITIVE_SIDE;
		return sdPlane::E_NO_SIDE;
	}

	sdPlane::Side GetSide(const sdAxisAlignedBox& kAABB) const
	{
		if (kAABB.IsNull()) 
			return sdPlane::E_NO_SIDE;
		if (kAABB.IsInfinite())
			return sdPlane::E_BOTH_SIDE;

		sdVector3 kCenter;
		sdVector3 kHalfSize;
		kAABB.GetCenter(kCenter);
		kAABB.GetHalfSize(kHalfSize);

		return GetSide(kCenter, kHalfSize);
	}
	
	// 算法可参考《实时计算机图形学》(第二版)，P329
	sdPlane::Side GetSide(const sdVector3& kCenter, const sdVector3& kHalfSize) const
	{
		// Calculate the distance between box centre and the plane
		// 平面到包围盒中心距离
		float fDistance = GetDistance(kCenter);

		// Calculate the maximise allows absolute distance for
		// the distance between box centre and plane
		// 包围盒在平面法线方向投影的半长
		float fMaxAbsDistance = m_kNormal.AbsDotProduct(kHalfSize);

		if (fDistance < -fMaxAbsDistance)
			return sdPlane::E_NEGATIVE_SIDE;

		if (fDistance > +fMaxAbsDistance)
			return sdPlane::E_POSITIVE_SIDE;

		return sdPlane::E_BOTH_SIDE;
	}

	// 算法可参考《实时计算机图形学》(第二版)，P328
	sdPlane::Side sdPlane::GetSide(const float* pfVextex) const;
	
	// 三点式方程
	void Redefine(const sdVector3& kPoint0, const sdVector3& kPoint1, const sdVector3& kPoint2)
	{
		sdVector3 kEdge1 = kPoint1 - kPoint0;
		sdVector3 kEdge2 = kPoint2 - kPoint0;
		m_kNormal = kEdge1.CrossProduct(kEdge2);
		m_kNormal.Normalise();
		m_fDistance = -m_kNormal.DotProduct(kPoint0);
	}
	
	// 平面点法式方程
	//	A(x-x0) + B(y-y0) + C(z-z0) = 0
	//	(A, B, C)(x, y, z) - (A, B, C)(x0, y0, z0) = 0;
	void Redefine(const sdVector3& kNormal, const sdVector3& kPoint)
	{
		m_kNormal = kNormal;
		m_fDistance = -kNormal.DotProduct(kPoint);
	}
	
	float Normalise(void)
	{
		float fLength = m_kNormal.Length();

		// Will also work for zero-sized vectors, but will change nothing
		// We're not using epsilons because we don't need to.
		// Read http://www.ogre3d.org/forums/viewtopic.php?f=4&t=61259
		if (fLength > float(0.0f))
		{
			float fInvLength = 1.0f / fLength;
			m_kNormal *= fInvLength;
			m_fDistance *= fInvLength;
		}

		return fLength;
	}
};
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_MATH
BASE_NAMESPACE_END_BASE
#endif