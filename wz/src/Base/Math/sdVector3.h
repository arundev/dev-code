//*************************************************************************************************
// 内容:	参考自CryEngine3/Ogre/Gamebryo/WorldZero
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-02
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_MATH_VECTOR3_H__
#define _SD_BASE_MATH_VECTOR3_H__

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_MATH
//-------------------------------------------------------------------------------------------------
// Vec3的模版实现(参考自CryEngine3)
//-------------------------------------------------------------------------------------------------
template <typename T> struct sdVector3T
{
	T m_kX;
	T m_kY;
	T m_kZ;

	sdVector3T() : m_kX(0), m_kY(0), m_kZ(0) {};
	sdVector3T(T kX, T kY, T kZ) : m_kX(kX), m_kY(kY), m_kZ(kZ){};
};
//-------------------------------------------------------------------------------------------------
// 基础型别的实现
//-------------------------------------------------------------------------------------------------
typedef sdVector3T<int>					sdVector3i;
typedef sdVector3T<unsigned int>		sdVector3ui;

typedef sdVector3T<short>				sdVector3s;
typedef sdVector3T<unsigned short>		sdVector3us;

typedef sdVector3T<char>				sdVector3c;
typedef sdVector3T<unsigned char>		sdVector3uc;

//typedef sdVector3T<float>				sdVector3f;
//typedef sdVector3T<double>			sdVector3d;
//-------------------------------------------------------------------------------------------------
// Vector3的float版本特别实现(参考自Ogre)
//-------------------------------------------------------------------------------------------------
class sdVector3 : public sdMemObject
{
public:
	union
	{
		struct 
		{
			float m_fX;		///< 变量访问
			float m_fY;		///<
			float m_fZ;		///<
		};
		float m_afValue[3];	///< 数组访问	
	};

public:
	// 特殊值
	static const sdVector3 ZERO;
	static const sdVector3 UNIT_X;
	static const sdVector3 UNIT_Y;
	static const sdVector3 UNIT_Z;
	static const sdVector3 NEGATIVE_UNIT_X;
	static const sdVector3 NEGATIVE_UNIT_Y;
	static const sdVector3 NEGATIVE_UNIT_Z;
	static const sdVector3 UNIT_SCALE;

public:
	SD_INLINE sdVector3() {}
	SD_INLINE sdVector3(const float fX, const float fY, const float fZ) : m_fX(fX), m_fY(fY), m_fZ(fZ){}
	SD_INLINE sdVector3(const float fScaler) : m_fX(fScaler), m_fY(fScaler), m_fZ(fScaler){}
	SD_INLINE sdVector3(const float* pfValue) : m_fX(pfValue[0]), m_fY(pfValue[1]), m_fZ(pfValue[2]){}
	SD_INLINE sdVector3(const sdVector3& rkVector): m_fX(rkVector.m_fX), m_fY(rkVector.m_fY), m_fZ(rkVector.m_fZ){}
	
	// 运算符重载
	// @{
	SD_INLINE float operator [] (const size_t i) const
	{
		SDASSERT(i < 3);
		return m_afValue[i];
	}

	SD_INLINE float& operator [] (const size_t i)
	{
		SDASSERT(i < 3);
		return m_afValue[i];
	}

	SD_INLINE sdVector3& operator = (const sdVector3& rkVector)
	{
		m_fX = rkVector.m_fX;
		m_fY = rkVector.m_fY;
		m_fZ = rkVector.m_fZ;

		return *this;
	}

	SD_INLINE sdVector3& operator = (const float fScaler)
	{
		m_fX = fScaler;
		m_fY = fScaler;
		m_fZ = fScaler;

		return *this;
	}

	SD_INLINE bool operator == (const sdVector3& rkVector) const
	{
		return (m_fX == rkVector.m_fX && m_fY == rkVector.m_fY && m_fZ == rkVector.m_fZ);
	}

	SD_INLINE bool operator != (const sdVector3& rkVector) const
	{
		return (m_fX != rkVector.m_fX || m_fY != rkVector.m_fY || m_fZ != rkVector.m_fZ);
	}

//	SD_INLINE sdVector3 operator + (const sdVector3& rkVector) const
//	{
//		return sdVector3(
//			m_fX + rkVector.m_fX,
//			m_fY + rkVector.m_fY,
//			m_fZ + rkVector.m_fZ);
//	}
//
//	SD_INLINE sdVector3 operator - (const sdVector3& rkVector) const
//	{
//		return sdVector3(
//			m_fX - rkVector.m_fX,
//			m_fY - rkVector.m_fY,
//			m_fZ - rkVector.m_fZ);
//	}
//
//	SD_INLINE sdVector3 operator * (const float fScalar) const
//	{
//		return sdVector3(
//			m_fX * fScalar,
//			m_fY * fScalar,
//			m_fZ * fScalar);
//	}
//
//	SD_INLINE sdVector3 operator * (const sdVector3& rhs) const
//	{
//		return sdVector3(
//			m_fX * rhs.m_fX,
//			m_fY * rhs.m_fY,
//			m_fZ * rhs.m_fZ);
//	}
//
//	SD_INLINE sdVector3 operator / (const float fScalar) const
//	{
//		assert(fScalar != 0.0);
//
//		float fInv = 1.0f / fScalar;
//
//		return sdVector3(
//			m_fX * fInv,
//			m_fY * fInv,
//			m_fZ * fInv);
//	}
//
//	SD_INLINE sdVector3 operator / (const sdVector3& rhs) const
//	{
//		return sdVector3(
//			m_fX / rhs.m_fX,
//			m_fY / rhs.m_fY,
//			m_fZ / rhs.m_fZ);
//	}

	SD_INLINE const sdVector3& operator + () const
	{
		return *this;
	}

	SD_INLINE sdVector3 operator - () const
	{
		return sdVector3(-m_fX, -m_fY, -m_fZ);
	}

	// overloaded operators to help sdVector3
	SD_INLINE friend sdVector3 operator + (const sdVector3& lkVector, const float rfScalar)
	{
		return sdVector3(
			lkVector.m_fX + rfScalar,
			lkVector.m_fY + rfScalar,
			lkVector.m_fZ + rfScalar);
	}

	SD_INLINE friend sdVector3 operator + (const float lfScalar, const sdVector3& rkVector)
	{
		return sdVector3(
			lfScalar + rkVector.m_fX,
			lfScalar + rkVector.m_fY,
			lfScalar + rkVector.m_fZ);
	}

	SD_INLINE friend sdVector3 operator + (const sdVector3& lkVector, const sdVector3& rkVector)
	{
		return sdVector3(
			lkVector.m_fX + rkVector.m_fX,
			lkVector.m_fY + rkVector.m_fY,
			lkVector.m_fZ + rkVector.m_fZ);
	}

	SD_INLINE friend sdVector3 operator - (const sdVector3& lkVector, const float rfScalar)
	{
		return sdVector3(
			lkVector.m_fX - rfScalar,
			lkVector.m_fY - rfScalar,
			lkVector.m_fZ - rfScalar);
	}

	SD_INLINE friend sdVector3 operator - (const float lfScalar, const sdVector3& rkVector)
	{
		return sdVector3(
			lfScalar - rkVector.m_fX,
			lfScalar - rkVector.m_fY,
			lfScalar - rkVector.m_fZ);
	}

	SD_INLINE friend sdVector3 operator - (const sdVector3& lkVector, const sdVector3& rkVector)
	{
		return sdVector3(
			lkVector.m_fX - rkVector.m_fX,
			lkVector.m_fY - rkVector.m_fY,
			lkVector.m_fZ - rkVector.m_fZ);
	}

	SD_INLINE friend sdVector3 operator * (const sdVector3& lkVector, const float rfScalar)
	{
		return sdVector3(
			lkVector.m_fX * rfScalar,
			lkVector.m_fY * rfScalar,
			lkVector.m_fZ * rfScalar);
	}

	SD_INLINE friend sdVector3 operator * (const float fScalar, const sdVector3& rkVector)
	{
		return sdVector3(
			fScalar * rkVector.m_fX,
			fScalar * rkVector.m_fY,
			fScalar * rkVector.m_fZ);
	}

	SD_INLINE friend sdVector3 operator * (const sdVector3& lkVector, const sdVector3& rkVector)
	{
		return sdVector3(
			lkVector.m_fX * rkVector.m_fX,
			lkVector.m_fY * rkVector.m_fY,
			lkVector.m_fZ * rkVector.m_fZ);
	}

	SD_INLINE friend sdVector3 operator / (const sdVector3& lkVector, const float rfScalar)
	{
		return sdVector3(
			lkVector.m_fX / rfScalar,
			lkVector.m_fY / rfScalar,
			lkVector.m_fZ / rfScalar);
	}

	SD_INLINE friend sdVector3 operator / (const float fScalar, const sdVector3& rkVector)
	{
		return sdVector3(
			fScalar / rkVector.m_fX,
			fScalar / rkVector.m_fY,
			fScalar / rkVector.m_fZ);
	}

	SD_INLINE friend sdVector3 operator / (const sdVector3& lkVector, const sdVector3& rkVector)
	{
		return sdVector3(
			lkVector.m_fX / rkVector.m_fX,
			lkVector.m_fY / rkVector.m_fY,
			lkVector.m_fZ / rkVector.m_fZ);
	}

	SD_INLINE friend sdVector3 operator ^ (const sdVector3& lkVector, const sdVector3& rkVector)
	{
		return sdVector3(
			lkVector.m_fY * rkVector.m_fZ - lkVector.m_fZ * rkVector.m_fY,
			lkVector.m_fZ * rkVector.m_fX - lkVector.m_fX * rkVector.m_fZ,
			lkVector.m_fX * rkVector.m_fY - lkVector.m_fY * rkVector.m_fX);
	}

	// arithmetic updates
	SD_INLINE sdVector3& operator += (const sdVector3& rkVector)
	{
		m_fX += rkVector.m_fX;
		m_fY += rkVector.m_fY;
		m_fZ += rkVector.m_fZ;

		return *this;
	}

	SD_INLINE sdVector3& operator += (const float fScalar)
	{
		m_fX += fScalar;
		m_fY += fScalar;
		m_fZ += fScalar;

		return *this;
	}

	SD_INLINE sdVector3& operator -= (const sdVector3& rkVector)
	{
		m_fX -= rkVector.m_fX;
		m_fY -= rkVector.m_fY;
		m_fZ -= rkVector.m_fZ;

		return *this;
	}

	SD_INLINE sdVector3& operator -= (const float fScalar)
	{
		m_fX -= fScalar;
		m_fY -= fScalar;
		m_fZ -= fScalar;

		return *this;
	}

	SD_INLINE sdVector3& operator *= (const float fScalar)
	{
		m_fX *= fScalar;
		m_fY *= fScalar;
		m_fZ *= fScalar;

		return *this;
	}

	SD_INLINE sdVector3& operator *= (const sdVector3& rkVector)
	{
		m_fX *= rkVector.m_fX;
		m_fY *= rkVector.m_fY;
		m_fZ *= rkVector.m_fZ;

		return *this;
	}

	SD_INLINE sdVector3& operator /= (const float fScalar)
	{
		assert(fScalar != 0.0);

		float fInv = 1.0f / fScalar;

		m_fX *= fInv;
		m_fY *= fInv;
		m_fZ *= fInv;

		return *this;
	}

	SD_INLINE sdVector3& operator /= (const sdVector3& rkVector)
	{
		m_fX /= rkVector.m_fX;
		m_fY /= rkVector.m_fY;
		m_fZ /= rkVector.m_fZ;

		return *this;
	}

	SD_INLINE bool operator < (const sdVector3& rhs) const
	{
		if (m_fX < rhs.m_fX && m_fY < rhs.m_fY && m_fZ < rhs.m_fZ )
			return true;
		return false;
	}

	SD_INLINE bool operator > (const sdVector3& rhs) const
	{
		if (m_fX > rhs.m_fX && m_fY > rhs.m_fY && m_fZ > rhs.m_fZ )
			return true;
		return false;
	}
	// @}


	// 常用
	// @{
	SD_INLINE float Length() const
	{
		return sqrt(m_fX * m_fX + m_fY * m_fY + m_fZ * m_fZ);
	}

	SD_INLINE float SquaredLength() const
	{
		return m_fX * m_fX + m_fY * m_fY + m_fZ * m_fZ;
	}

	SD_INLINE float DotProduct(const sdVector3& kVec) const
	{
		return m_fX * kVec.m_fX + m_fY * kVec.m_fY + m_fZ * kVec.m_fZ;
	}

	SD_INLINE float AbsDotProduct(const sdVector3& kVec) const
	{
		return sdMath::Abs(m_fX * kVec.m_fX) + sdMath::Abs(m_fY * kVec.m_fY) + sdMath::Abs(m_fZ * kVec.m_fZ);
	}

	SD_INLINE sdVector3 CrossProduct(const sdVector3& kVector) const
	{
		return sdVector3(
			m_fY * kVector.m_fZ - m_fZ * kVector.m_fY,
			m_fZ * kVector.m_fX - m_fX * kVector.m_fZ,
			m_fX * kVector.m_fY - m_fY * kVector.m_fX);
	}

	SD_INLINE sdVector3& CrossProduct(const sdVector3& rkVector, sdVector3& kValue) const
	{
		kValue.m_fX = m_fY * rkVector.m_fZ - m_fZ * rkVector.m_fY;
		kValue.m_fY = m_fZ * rkVector.m_fX - m_fX * rkVector.m_fZ;
		kValue.m_fZ = m_fX * rkVector.m_fY - m_fY * rkVector.m_fX;

		return kValue;
	}

	SD_INLINE float Normalise()
	{	
		float fInvLength = 0.f;
		float fLength = sdMath::Sqrt(m_fX * m_fX + m_fY * m_fY + m_fZ * m_fZ);
		if (fLength > sdMath::MATH_EPS)
			fInvLength = 1.0f / fLength;

		m_fX *= fInvLength;
		m_fY *= fInvLength;
		m_fZ *= fInvLength;

		return fLength;
	}

	// Determine the orthogonal projection of this on kVector:
	// dot(kVector, this) * kVector and subtract it from this 
	// resulting in the orthogonalized this
	// (come from WorldZero)
	SD_INLINE void Orthogonalize(const sdVector3& kVector)
	{
		sdVector3 kVectorRes = DotProduct(kVector) * kVector;
		m_fX -= kVectorRes.m_fX;
		m_fY -= kVectorRes.m_fY;
		m_fZ -= kVectorRes.m_fZ;
	}

	// 取两个Vector每个分量的较小值
	SD_INLINE void MakeMinimum(const sdVector3& kVector)
	{
		if(kVector.m_fX < m_fX)	m_fX = kVector.m_fX;
		if(kVector.m_fY < m_fY) m_fY = kVector.m_fY;
		if(kVector.m_fZ < m_fZ) m_fZ = kVector.m_fZ;
	}

	// 取两个Vector每个分量的较大值 
	SD_INLINE void MakeMaximum(const sdVector3& kVector)
	{
		if(kVector.m_fX > m_fX)	m_fX = kVector.m_fX;
		if(kVector.m_fY > m_fY) m_fY = kVector.m_fY;
		if(kVector.m_fZ > m_fZ) m_fZ = kVector.m_fZ;
	}
	// @}
};
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_MATH
BASE_NAMESPACE_END_BASE
#endif