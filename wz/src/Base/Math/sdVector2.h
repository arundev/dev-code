//*************************************************************************************************
// 内容:	参考自CCryEngine3/Ogre/Gamebryo/WorldZero
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-02
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_MATH_VECTOR2_H__
#define _SD_BASE_MATH_VECTOR2_H__

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_MATH
//-------------------------------------------------------------------------------------------------
// Vec2的模版实现(参考自CryEngine3)
//-------------------------------------------------------------------------------------------------
template <typename T> struct sdVector2T
{
	T m_kX;
	T m_kY;

	sdVector2T() : m_kX(0), m_kY(0){};
	sdVector2T(T kX, T kY) : m_kX(kX), m_kY(kY){};
};
//-------------------------------------------------------------------------------------------------
// 基础型别的实现
typedef sdVector2T<int>				sdVector2i;
typedef sdVector2T<unsigned int>	sdVector2ui;

typedef sdVector2T<short>			sdVector2s;
typedef sdVector2T<unsigned short>	sdVector2us;

typedef sdVector2T<char>			sdVector2c;
typedef sdVector2T<unsigned char>	sdVector2uc;

//typedef sdVector2T<float>			sdVector2f;
//typedef sdVector2T<double>		sdVector2d;
//-------------------------------------------------------------------------------------------------
// Vector2的float版本特别实现(参考自Ogre)
//-------------------------------------------------------------------------------------------------
class sdVector2 : public sdMemObject 
{
public:
	union
	{
		struct 
		{
			float m_fX;		///< 变量访问
			float m_fY;		///<
		};
		float m_afValue[2];	///< 数组访问	
	};

public:
	// 特殊值
	static const sdVector2 ZERO;
	static const sdVector2 UNIT_X;
	static const sdVector2 UNIT_Y;
	static const sdVector2 NEGATIVE_UNIT_X;
	static const sdVector2 NEGATIVE_UNIT_Y;
	static const sdVector2 UNIT_SCALE;

public:
	SD_INLINE sdVector2() {}
	SD_INLINE sdVector2(const float fX, const float fY) : m_fX(fX), m_fY(fY){}
	SD_INLINE sdVector2(const float fScaler) : m_fX(fScaler), m_fY(fScaler){}
	SD_INLINE sdVector2(const float* pfValue) : m_fX(pfValue[0]), m_fY(pfValue[1]){}
	SD_INLINE sdVector2(const sdVector2& rkVector): m_fX(rkVector.m_fX), m_fY(rkVector.m_fY){}

	// 运算符重载
	// @{
	SD_INLINE float operator [] (const size_t i) const
	{
		SDASSERT(i < 2);
		return m_afValue[i];
	}

	SD_INLINE float& operator [] (const size_t i)
	{
		SDASSERT(i < 2);
		return m_afValue[i];
	}

	SD_INLINE sdVector2& operator = (const sdVector2& rkVector)
	{
		m_fX = rkVector.m_fX;
		m_fY = rkVector.m_fY;

		return *this;
	}

	SD_INLINE sdVector2& operator = (const float fScaler)
	{
		m_fX = fScaler;
		m_fY = fScaler;

		return *this;
	}

	SD_INLINE bool operator == (const sdVector2& rkVector) const
	{
		return (m_fX == rkVector.m_fX && m_fY == rkVector.m_fY);
	}

	SD_INLINE bool operator != (const sdVector2& rkVector) const
	{
		return (m_fX != rkVector.m_fX || m_fY != rkVector.m_fY);
	}

//	SD_INLINE sdVector2 operator + (const sdVector2& rkVector) const
//	{
//		return sdVector2(m_fX + rkVector.m_fX, m_fY + rkVector.m_fY);
//	}
//
//	SD_INLINE sdVector2 operator - (const sdVector2& rkVector) const
//	{
//		return sdVector2(m_fX - rkVector.m_fX, m_fY - rkVector.m_fY);
//	}
//
//	SD_INLINE sdVector2 operator * (const float fScalar) const
//	{
//		return sdVector2(m_fX * fScalar, m_fY * fScalar);
//	}
//
//	SD_INLINE sdVector2 operator * (const sdVector2& rhs) const
//	{
//		return sdVector2(m_fX * rhs.m_fX, m_fY * rhs.m_fY);
//	}
//
//	SD_INLINE sdVector2 operator / (const float fScalar) const
//	{
//		SDASSERT(fScalar != 0.0);
//		float fInv = 1.0f / fScalar;
//		return sdVector2(m_fX * fInv, m_fY * fInv);
//	}
//
//	SD_INLINE sdVector2 operator / (const sdVector2& rhs) const
//	{
//		return sdVector2(m_fX / rhs.m_fX, m_fY / rhs.m_fY);
//	}

	SD_INLINE const sdVector2& operator + () const
	{
		return (*this);
	}

	SD_INLINE sdVector2 operator - () const
	{
		return sdVector2(-m_fX, -m_fY);
	}

	// overloaded operators to help sdVector2
	SD_INLINE friend sdVector2 operator + (const sdVector2& lkVector, const float rfScalar)
	{
		return sdVector2(lkVector.m_fX + rfScalar, lkVector.m_fY + rfScalar);
	}

	SD_INLINE friend sdVector2 operator + (const float lfScalar, const sdVector2& rkVector)
	{
		return sdVector2(lfScalar + rkVector.m_fX, lfScalar + rkVector.m_fY);
	}

	SD_INLINE friend sdVector2 operator + (const sdVector2& lkVector, const sdVector2& rkVector)
	{
		return sdVector2(lkVector.m_fX + rkVector.m_fX, lkVector.m_fY + rkVector.m_fY);
	}

	SD_INLINE friend sdVector2 operator - (const sdVector2& lkVector, const float rfScalar)
	{
		return sdVector2(lkVector.m_fX - rfScalar, lkVector.m_fY - rfScalar);
	}

	SD_INLINE friend sdVector2 operator - (const float lfScalar, const sdVector2& rkVector)
	{
		return sdVector2(lfScalar - rkVector.m_fX, lfScalar - rkVector.m_fY);
	}

	SD_INLINE friend sdVector2 operator - (const sdVector2& lkVector, const sdVector2& rkVector)
	{
		return sdVector2(lkVector.m_fX - rkVector.m_fX, lkVector.m_fY - rkVector.m_fY);
	}

	SD_INLINE friend sdVector2 operator * (const sdVector2& rkVector, const float rfScalar)
	{
		return sdVector2(rkVector.m_fX * rfScalar, rkVector.m_fY * rfScalar);
	}

	SD_INLINE friend sdVector2 operator * (const float lfScalar, const sdVector2& rkVector)
	{
		return sdVector2(lfScalar * rkVector.m_fX, lfScalar * rkVector.m_fY);
	}

	SD_INLINE friend sdVector2 operator * (const sdVector2& lkVector, const sdVector2& rkVector)
	{
		return sdVector2(lkVector.m_fX * rkVector.m_fX, lkVector.m_fY * rkVector.m_fY);
	}

	SD_INLINE friend sdVector2 operator / (const sdVector2& lkVector, const float rfScalar)
	{
		return sdVector2(lkVector.m_fX / rfScalar, lkVector.m_fY / rfScalar);
	}

	SD_INLINE friend sdVector2 operator / (const float lfScalar, const sdVector2& rkVector)
	{
		return sdVector2(lfScalar / rkVector.m_fX, lfScalar / rkVector.m_fY);
	}

	SD_INLINE friend sdVector2 operator / (const sdVector2& lkVector, const sdVector2& rkVector)
	{
		return sdVector2(lkVector.m_fX / rkVector.m_fX, lkVector.m_fY / rkVector.m_fY);
	}

	// arithmetic updates
	SD_INLINE sdVector2& operator += (const sdVector2& rkVector)
	{
		m_fX += rkVector.m_fX;
		m_fY += rkVector.m_fY;

		return *this;
	}

	SD_INLINE sdVector2& operator += (const float fScalar)
	{
		m_fX += fScalar;
		m_fY += fScalar;

		return *this;
	}

	SD_INLINE sdVector2& operator -= (const sdVector2& rkVector)
	{
		m_fX -= rkVector.m_fX;
		m_fY -= rkVector.m_fY;

		return *this;
	}

	SD_INLINE sdVector2& operator -= (const float fScalar)
	{
		m_fX -= fScalar;
		m_fY -= fScalar;

		return *this;
	}

	SD_INLINE sdVector2& operator *= (const float fScalar)
	{
		m_fX *= fScalar;
		m_fY *= fScalar;

		return *this;
	}

	SD_INLINE sdVector2& operator *= (const sdVector2& rkVector)
	{
		m_fX *= rkVector.m_fX;
		m_fY *= rkVector.m_fY;

		return *this;
	}

	SD_INLINE sdVector2& operator /= (const float fScalar)
	{
		SDASSERT(fScalar != 0.0);

		float fInv = 1.0f / fScalar;

		m_fX *= fInv;
		m_fY *= fInv;

		return *this;
	}

	SD_INLINE sdVector2& operator /= (const sdVector2& rkVector)
	{
		m_fX /= rkVector.m_fX;
		m_fY /= rkVector.m_fY;

		return *this;
	}

	SD_INLINE bool operator < (const sdVector2& rhs) const
	{
		if (m_fX < rhs.m_fX && m_fY < rhs.m_fY)
			return true;
		return false;
	}

	SD_INLINE bool operator > (const sdVector2& rhs) const
	{
		if (m_fX > rhs.m_fX && m_fY > rhs.m_fY)
			return true;
		return false;
	}
	// @}	


	// 常用
	// @{
	SD_INLINE float Length() const
	{
		return sqrt(m_fX * m_fX + m_fY * m_fY);
	}

	SD_INLINE float SquaredLength() const
	{
		return m_fX * m_fX + m_fY * m_fY;
	}

	SD_INLINE float DotProduct(const sdVector2& kVec) const
	{
		return m_fX * kVec.m_fX + m_fY * kVec.m_fY;
	}

	SD_INLINE float Normalise()
	{	
		float fInvLength = 0.f;
		float fLength = sdMath::Sqrt(m_fX * m_fX + m_fY * m_fY);
		if (fLength > sdMath::MATH_EPS)
			fInvLength = 1.0f / fLength;

		m_fX *= fInvLength;
		m_fY *= fInvLength;

		return fLength;
	}
	// @}
};
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_MATH
BASE_NAMESPACE_END_BASE
#endif