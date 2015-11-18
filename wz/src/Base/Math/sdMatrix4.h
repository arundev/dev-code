//*************************************************************************************************
// 内容:	抽取自Ogre/OgreMatrix4.h
//---------------------------------------------------------
// 作者:		
// 创建:		2012-09-10
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_MATH_MATRIX4_H__
#define _SD_BASE_MATH_MATRIX4_H__

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_MATH
//-------------------------------------------------------------------------------------------------
class sdMatrix4 : public sdMemObject
{
public:
	union 
	{
		float m[4][4];
		float _m[16];
		struct  
		{
			float _m00, _m01, _m02, _m03;
			float _m10, _m11, _m12, _m13;
			float _m20, _m21, _m22, _m23;
			float _m30, _m31, _m32, _m33;
		};
		struct  
		{
			float _00, _01, _02, _03;
			float _10, _11, _12, _13;
			float _20, _21, _22, _23;
			float _30, _31, _32, _33;
		};
	};

public:
	static const sdMatrix4 ZERO;
	static const sdMatrix4 ZEROAFFINE;
	static const sdMatrix4 IDENTITY;

public:
	SD_INLINE sdMatrix4()
	{
	}

	SD_INLINE sdMatrix4(float m00, float m01, float m02, float m03,float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,	float m30, float m31, float m32, float m33)
	{
		m[0][0] = m00;	m[0][1] = m01;	m[0][2] = m02;	m[0][3] = m03;
		m[1][0] = m10;	m[1][1] = m11;	m[1][2] = m12;	m[1][3] = m13;
		m[2][0] = m20;	m[2][1] = m21;	m[2][2] = m22;	m[2][3] = m23;
		m[3][0] = m30;	m[3][1] = m31;	m[3][2] = m32;	m[3][3] = m33;
	}

	SD_INLINE sdMatrix4(const float* pfValue) 
	{
		::memcpy(m, pfValue, sizeof(m));
	}

	SD_INLINE sdMatrix4(const sdMatrix4& rkMatrix) 
	{
		::memcpy(m, &rkMatrix, sizeof(rkMatrix));
	}

	void GetRow(int iRow, sdVector4& kRow) const;
	void SetRow(int iRow, const sdVector4& kRow);
	
	void GetCol(int iCol, sdVector4& kCol) const;
	void SetCol(int iCol, const sdVector4& kCol);

	// 运算符重载
	// @{
	SD_INLINE sdMatrix4 Concatenate(const sdMatrix4 &kRight) const
	{
		sdMatrix4 kMatrix;
		kMatrix.m[0][0] = m[0][0] * kRight.m[0][0] + m[0][1] * kRight.m[1][0] + m[0][2] * kRight.m[2][0] + m[0][3] * kRight.m[3][0];
		kMatrix.m[0][1] = m[0][0] * kRight.m[0][1] + m[0][1] * kRight.m[1][1] + m[0][2] * kRight.m[2][1] + m[0][3] * kRight.m[3][1];
		kMatrix.m[0][2] = m[0][0] * kRight.m[0][2] + m[0][1] * kRight.m[1][2] + m[0][2] * kRight.m[2][2] + m[0][3] * kRight.m[3][2];
		kMatrix.m[0][3] = m[0][0] * kRight.m[0][3] + m[0][1] * kRight.m[1][3] + m[0][2] * kRight.m[2][3] + m[0][3] * kRight.m[3][3];

		kMatrix.m[1][0] = m[1][0] * kRight.m[0][0] + m[1][1] * kRight.m[1][0] + m[1][2] * kRight.m[2][0] + m[1][3] * kRight.m[3][0];
		kMatrix.m[1][1] = m[1][0] * kRight.m[0][1] + m[1][1] * kRight.m[1][1] + m[1][2] * kRight.m[2][1] + m[1][3] * kRight.m[3][1];
		kMatrix.m[1][2] = m[1][0] * kRight.m[0][2] + m[1][1] * kRight.m[1][2] + m[1][2] * kRight.m[2][2] + m[1][3] * kRight.m[3][2];
		kMatrix.m[1][3] = m[1][0] * kRight.m[0][3] + m[1][1] * kRight.m[1][3] + m[1][2] * kRight.m[2][3] + m[1][3] * kRight.m[3][3];

		kMatrix.m[2][0] = m[2][0] * kRight.m[0][0] + m[2][1] * kRight.m[1][0] + m[2][2] * kRight.m[2][0] + m[2][3] * kRight.m[3][0];
		kMatrix.m[2][1] = m[2][0] * kRight.m[0][1] + m[2][1] * kRight.m[1][1] + m[2][2] * kRight.m[2][1] + m[2][3] * kRight.m[3][1];
		kMatrix.m[2][2] = m[2][0] * kRight.m[0][2] + m[2][1] * kRight.m[1][2] + m[2][2] * kRight.m[2][2] + m[2][3] * kRight.m[3][2];
		kMatrix.m[2][3] = m[2][0] * kRight.m[0][3] + m[2][1] * kRight.m[1][3] + m[2][2] * kRight.m[2][3] + m[2][3] * kRight.m[3][3];

		kMatrix.m[3][0] = m[3][0] * kRight.m[0][0] + m[3][1] * kRight.m[1][0] + m[3][2] * kRight.m[2][0] + m[3][3] * kRight.m[3][0];
		kMatrix.m[3][1] = m[3][0] * kRight.m[0][1] + m[3][1] * kRight.m[1][1] + m[3][2] * kRight.m[2][1] + m[3][3] * kRight.m[3][1];
		kMatrix.m[3][2] = m[3][0] * kRight.m[0][2] + m[3][1] * kRight.m[1][2] + m[3][2] * kRight.m[2][2] + m[3][3] * kRight.m[3][2];
		kMatrix.m[3][3] = m[3][0] * kRight.m[0][3] + m[3][1] * kRight.m[1][3] + m[3][2] * kRight.m[2][3] + m[3][3] * kRight.m[3][3];

		return kMatrix;
	}

	SD_INLINE sdMatrix4 operator * (const sdMatrix4 &kRight) const
	{
		return Concatenate(kRight);
	}

	SD_INLINE sdMatrix4& operator *= (const float& fScaler)
	{
		_m00 *= fScaler;
		_m01 *= fScaler;
		_m02 *= fScaler;
		_m03 *= fScaler;

		_m10 *= fScaler;
		_m11 *= fScaler;
		_m12 *= fScaler;
		_m13 *= fScaler;

		_m20 *= fScaler;
		_m21 *= fScaler;
		_m22 *= fScaler;
		_m23 *= fScaler;

		_m30 *= fScaler;
		_m31 *= fScaler;
		_m32 *= fScaler;
		_m33 *= fScaler;

		return *this;
	}

//	// overloaded operators to help sdMatrix3
//	SD_INLINE friend sdVector4 operator * (const sdVector4& lkVector, const sdMatrix4& rkMatrix)
//	{
//		sdVector4 kProd;
//		for (size_t iRow = 0; iRow < 4; iRow++)
//		{
//			kProd[iRow] =
//				lkVector[0] * rkMatrix.m[0][iRow] +
//				lkVector[1] * rkMatrix.m[1][iRow] +
//				lkVector[2] * rkMatrix.m[2][iRow] +
//				lkVector[3] * rkMatrix.m[3][iRow];
//		}
//		return kProd;
//	}
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

		fTmp = _m03;
		_m03 = _m30;
		_m30 = fTmp;

		fTmp = _m12;
		_m12 = _m21;
		_m21 = fTmp;

		fTmp = _m13;
		_m13 = _m31;
		_m31 = fTmp;

		fTmp = _m23;
		_m23 = _m32;
		_m32 = fTmp;
	}

	SD_INLINE void Transpose(sdMatrix4& kMatrix) const
	{
		kMatrix._m00 = _m00;
		kMatrix._m01 = _m10;
		kMatrix._m02 = _m20;
		kMatrix._m03 = _m30;

		kMatrix._m10 = _m01;
		kMatrix._m11 = _m11;
		kMatrix._m12 = _m21;
		kMatrix._m13 = _m31;

		kMatrix._m20 = _m02;
		kMatrix._m21 = _m12;
		kMatrix._m22 = _m22;
		kMatrix._m23 = _m32;

		kMatrix._m30 = _m03;
		kMatrix._m31 = _m13;
		kMatrix._m32 = _m23;
		kMatrix._m33 = _m33;
	}
	// @}
};
//-------------------------------------------------------------------------------------------------
SD_INLINE sdVector4 operator * (const sdVector4& lkVector, const sdMatrix4& rkMatrix);
SD_INLINE sdVector4 operator * (const sdMatrix4& lkMatrix, const sdVector4& rkVector);
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_MATH
BASE_NAMESPACE_END_BASE
#endif