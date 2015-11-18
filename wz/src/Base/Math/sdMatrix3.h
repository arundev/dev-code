//*************************************************************************************************
// 内容:	参考自Ogre/WorldZero
//---------------------------------------------------------
// 作者:		
// 创建:		2012-09-10
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_MATH_MATRIX3_H__
#define _SD_BASE_MATH_MATRIX3_H__

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_MATH
//-------------------------------------------------------------------------------------------------
class sdMatrix3 : public sdMemObject
{
public:
	union 
	{
		float m[3][3];				///< 数组访问
		float _m[9];				///< 数组访问
		struct  
		{
			float _m00, _m01, _m02;	///< 变量访问
			float _m10, _m11, _m12;	///< 变量访问
			float _m20, _m21, _m22;	///< 变量访问
		};
		struct  
		{
			float _00, _01, _02;	///< 变量访问
			float _10, _11, _12;	///< 变量访问
			float _20, _21, _22;	///< 变量访问
		};
	};

public:
	static const sdMatrix3 ZERO;
	static const sdMatrix3 IDENTITY;

public:
	SD_INLINE sdMatrix3()
	{
	}

	SD_INLINE sdMatrix3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22)
		: _m00(m00), _m01(m01),_m02(m02)
		, _m10(m10), _m11(m11),_m12(m12)
		, _m20(m20), _m21(m21),_m22(m22)
	{
	}

	SD_INLINE sdMatrix3(const float* pfValue) 
	{
		::memcpy(m, pfValue, sizeof(m));
	}

	SD_INLINE sdMatrix3(const sdMatrix3& rkMatrix) 
	{
		::memcpy(m, &rkMatrix, sizeof(rkMatrix));
	}

	// 运算符重载
	// @{
	SD_INLINE sdMatrix3 concatenate(const sdMatrix3 &kRight) const
	{
		sdMatrix3 kMatrix;
		kMatrix.m[0][0] = m[0][0] * kRight.m[0][0] + m[0][1] * kRight.m[1][0] + m[0][2] * kRight.m[2][0];
		kMatrix.m[0][1] = m[0][0] * kRight.m[0][1] + m[0][1] * kRight.m[1][1] + m[0][2] * kRight.m[2][1];
		kMatrix.m[0][2] = m[0][0] * kRight.m[0][2] + m[0][1] * kRight.m[1][2] + m[0][2] * kRight.m[2][2];

		kMatrix.m[1][0] = m[1][0] * kRight.m[0][0] + m[1][1] * kRight.m[1][0] + m[1][2] * kRight.m[2][0];
		kMatrix.m[1][1] = m[1][0] * kRight.m[0][1] + m[1][1] * kRight.m[1][1] + m[1][2] * kRight.m[2][1];
		kMatrix.m[1][2] = m[1][0] * kRight.m[0][2] + m[1][1] * kRight.m[1][2] + m[1][2] * kRight.m[2][2];

		kMatrix.m[2][0] = m[2][0] * kRight.m[0][0] + m[2][1] * kRight.m[1][0] + m[2][2] * kRight.m[2][0];
		kMatrix.m[2][1] = m[2][0] * kRight.m[0][1] + m[2][1] * kRight.m[1][1] + m[2][2] * kRight.m[2][1];
		kMatrix.m[2][2] = m[2][0] * kRight.m[0][2] + m[2][1] * kRight.m[1][2] + m[2][2] * kRight.m[2][2];
		return kMatrix;
	}

	SD_INLINE sdMatrix3 operator * (const sdMatrix3 &kRight) const
	{
		return concatenate(kRight);
	}

	SD_INLINE sdMatrix3& operator += (const float& fScaler)
	{
		for (int i = 0; i < 9; ++i)
			_m[i] += fScaler;
		return *this;
	}

	SD_INLINE sdMatrix3& operator -= (const float& fScaler)
	{
		for (int i = 0; i < 9; ++i)
			_m[i] -= fScaler;
		return *this;
	}

	SD_INLINE sdMatrix3& operator *= (const float& fScaler)
	{
		for (int i = 0; i < 9; ++i)
			_m[i] *= fScaler;
		return *this;
	}

	SD_INLINE sdMatrix3& operator /= (const float& fScaler)
	{
		for (int i = 0; i < 9; ++i)
			_m[i] /= fScaler;
		return *this;
	}
	// @}

	// Util
	// @{
	SD_INLINE void Transpose()
	{
		float fTmp = 0.f;

		fTmp = _m01;
		_m01 = _m10;
		_m10 = fTmp;

		fTmp = _m02;
		_m02 = _m20;
		_m20 = fTmp;

		fTmp = _m12;
		_m12 = _m21;
		_m21 = fTmp;
	}

	SD_INLINE void Transpose(sdMatrix3& kMatrix) const
	{
		kMatrix._m00 = _m00;
		kMatrix._m01 = _m10;
		kMatrix._m02 = _m20;

		kMatrix._m10 = _m01;
		kMatrix._m11 = _m11;
		kMatrix._m12 = _m21;

		kMatrix._m20 = _m02;
		kMatrix._m21 = _m12;
		kMatrix._m22 = _m22;
	}
	// @}
};
//-------------------------------------------------------------------------------------------------
SD_INLINE sdVector3 operator * (const sdVector3& lkVector, const sdMatrix3& rkMatrix);
SD_INLINE sdVector3 operator * (const sdMatrix3& lkMatrix, const sdVector3& rkVector);
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_MATH
BASE_NAMESPACE_END_BASE
#endif