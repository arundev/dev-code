//*************************************************************************************************
// 内容:	借鉴自CryEngine3/Ogre/WorldZero
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-03
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_MATH_VECTOR4_H__
#define _SD_BASE_MATH_VECTOR4_H__


BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_MATH
//-------------------------------------------------------------------------------------------------
// Vec4的模版实现(参考自CryEngine3)
//-------------------------------------------------------------------------------------------------
template <typename T> struct sdVector4T
{
	T m_kX;
	T m_kY;
	T m_kZ;
	T m_kW;

	sdVector4T() : m_kX(0), m_kY(0), m_kZ(0), m_kW(0) {};
	sdVector4T(T kX, T kY, T kZ, T kW) : m_kX(kX), m_kY(kY), m_kZ(kZ), m_kW(kW){};
};
//-------------------------------------------------------------------------------------------------
// 基础型别的实现
//-------------------------------------------------------------------------------------------------
typedef sdVector4T<int>					sdVector4i;
typedef sdVector4T<unsigned int>		sdVector4ui;

typedef sdVector4T<short>				sdVector4s;
typedef sdVector4T<unsigned short>		sdVector4us;

typedef sdVector4T<char>				sdVector4c;
typedef sdVector4T<unsigned char>		sdVector4uc;

//typedef sdVector4T<float>				sdVector4f;
//typedef sdVector4T<double>			sdVector4d;
//-------------------------------------------------------------------------------------------------
// Vector4的float版本特别实现(参考自Ogre)
//-------------------------------------------------------------------------------------------------
class sdVector4 : public sdMemObject 
{
public:
	union
	{
		struct 
		{
			float m_fX;		///< 变量访问
			float m_fY;		///<
			float m_fZ;		///<
			float m_fW;		///<
		};
		struct 
		{
			float m_fR;		///< 变量访问
			float m_fG;		///<
			float m_fB;		///<
			float m_fA;		///<
		};
		float m_afValue[4];	///< 数组访问	
	};

public:
	// 特殊值
	static const sdVector4 ZERO;
	static const sdVector4 UNIT_X;
	static const sdVector4 UNIT_Y;
	static const sdVector4 UNIT_Z;
	static const sdVector4 UNIT_W;
	static const sdVector4 NEGATIVE_UNIT_X;
	static const sdVector4 NEGATIVE_UNIT_Y;
	static const sdVector4 NEGATIVE_UNIT_Z;
	static const sdVector4 NEGATIVE_UNIT_W;
	static const sdVector4 UNIT_SCALE;

public:
	SD_INLINE sdVector4() {};
	SD_INLINE sdVector4(const float fX, const float fY, const float fZ, const float fW) : m_fX(fX), m_fY(fY), m_fZ(fZ), m_fW(fW){};
	SD_INLINE sdVector4(const float fScaler) : m_fX(fScaler), m_fY(fScaler), m_fZ(fScaler), m_fW(fScaler){};
	SD_INLINE sdVector4(const float* pfValue) : m_fX(pfValue[0]), m_fY(pfValue[1]), m_fZ(pfValue[2]), m_fW(pfValue[3]){}
	SD_INLINE sdVector4(const sdVector4& rkVector): m_fX(rkVector.m_fX), m_fY(rkVector.m_fY), m_fZ(rkVector.m_fZ), m_fW(rkVector.m_fW){}

	// 运算符重载
	// @{
	SD_INLINE float operator [] (const size_t i) const
	{
		SDASSERT(i < 4);
		return m_afValue[i];
	}

	SD_INLINE float& operator [] (const size_t i)
	{
		SDASSERT(i < 4);
		return m_afValue[i];
	}

	SD_INLINE sdVector4& operator = (const sdVector4& rkVector)
	{
		m_fX = rkVector.m_fX;
		m_fY = rkVector.m_fY;
		m_fZ = rkVector.m_fZ;
		m_fW = rkVector.m_fW;

		return *this;
	}

	SD_INLINE sdVector4& operator = (const float fScaler)
	{
		m_fX = fScaler;
		m_fY = fScaler;
		m_fZ = fScaler;
		m_fW = fScaler;

		return *this;
	}

	SD_INLINE bool operator == (const sdVector4& rkVector) const
	{
		return (m_fX == rkVector.m_fX && m_fY == rkVector.m_fY && m_fZ == rkVector.m_fZ && m_fW == rkVector.m_fW);
	}

	SD_INLINE bool operator != (const sdVector4& rkVector) const
	{
		return (m_fX != rkVector.m_fX || m_fY != rkVector.m_fY || m_fZ != rkVector.m_fZ || m_fW != rkVector.m_fW);
	}

//	SD_INLINE sdVector4 operator + (const sdVector4& rkVector) const
//	{
//		return sdVector4(
//			m_fX + rkVector.m_fX,
//			m_fY + rkVector.m_fY,
//			m_fZ + rkVector.m_fZ,
//			m_fW + rkVector.m_fW);
//	}
//
//	SD_INLINE sdVector4 operator - (const sdVector4& rkVector) const
//	{
//		return sdVector4(
//			m_fX - rkVector.m_fX,
//			m_fY - rkVector.m_fY,
//			m_fZ - rkVector.m_fZ,
//			m_fW - rkVector.m_fW);
//	}
//
//	SD_INLINE sdVector4 operator * (const float fScalar) const
//	{
//		return sdVector4(
//			m_fX * fScalar,
//			m_fY * fScalar,
//			m_fZ * fScalar,
//			m_fW * fScalar);
//	}
//
//	SD_INLINE sdVector4 operator * (const sdVector4& rhs) const
//	{
//		return sdVector4(
//			m_fX * rhs.m_fX,
//			m_fY * rhs.m_fY,
//			m_fZ * rhs.m_fZ,
//			m_fW * rhs.m_fW);
//	}
//
//	SD_INLINE sdVector4 operator / (const float fScalar) const
//	{
//		SDASSERT(fScalar != 0.0);
//
//		float fInv = 1.0f / fScalar;
//
//		return sdVector4(
//			m_fX * fInv,
//			m_fY * fInv,
//			m_fZ * fInv,
//			m_fW * fInv);
//	}
//
//	SD_INLINE sdVector4 operator / (const sdVector4& rhs) const
//	{
//		return sdVector4(
//			m_fX / rhs.m_fX,
//			m_fY / rhs.m_fY,
//			m_fZ / rhs.m_fZ
//			m_fW / rhs.m_fW);
//	}

	SD_INLINE const sdVector4& operator + () const
	{
		return *this;
	}

	SD_INLINE sdVector4 operator - () const
	{
		return sdVector4(-m_fX, -m_fY, -m_fZ, - m_fW);
	}

	// overloaded operators to help sdVector4
	SD_INLINE friend sdVector4 operator + (const sdVector4& lkVector, const float rfScalar)
	{
		return sdVector4(
			lkVector.m_fX + rfScalar,
			lkVector.m_fY + rfScalar,
			lkVector.m_fZ + rfScalar,
			lkVector.m_fW + rfScalar);
	}

	SD_INLINE friend sdVector4 operator + (const float lfScalar, const sdVector4& rkVector)
	{
		return sdVector4(
			lfScalar + rkVector.m_fX,
			lfScalar + rkVector.m_fY,
			lfScalar + rkVector.m_fZ,
			lfScalar + rkVector.m_fW);
	}

	SD_INLINE friend sdVector4 operator + (const sdVector4& lkVector, const sdVector4& rkVector)
	{
		return sdVector4(
			lkVector.m_fX + rkVector.m_fX,
			lkVector.m_fY + rkVector.m_fY,
			lkVector.m_fZ + rkVector.m_fZ,
			lkVector.m_fW + rkVector.m_fW);
	}

	SD_INLINE friend sdVector4 operator - (const sdVector4& lkVector, const float rfScalar)
	{
		return sdVector4(
			lkVector.m_fX - rfScalar,
			lkVector.m_fY - rfScalar,
			lkVector.m_fZ - rfScalar,
			lkVector.m_fW - rfScalar);
	}

	SD_INLINE friend sdVector4 operator - (const float lfScalar, const sdVector4& rkVector)
	{
		return sdVector4(
			lfScalar - rkVector.m_fX,
			lfScalar - rkVector.m_fY,
			lfScalar - rkVector.m_fZ,
			lfScalar - rkVector.m_fW);
	}

	SD_INLINE friend sdVector4 operator - (const sdVector4& lkVector, const sdVector4& rkVector)
	{
		return sdVector4(
			lkVector.m_fX - rkVector.m_fX,
			lkVector.m_fY - rkVector.m_fY,
			lkVector.m_fZ - rkVector.m_fZ,
			lkVector.m_fW - rkVector.m_fW);
	}

	SD_INLINE friend sdVector4 operator * (const sdVector4& lkVector, const float rfScalar)
	{
		return sdVector4(
			lkVector.m_fX * rfScalar,
			lkVector.m_fY * rfScalar,
			lkVector.m_fZ * rfScalar,
			lkVector.m_fW * rfScalar);
	}

	SD_INLINE friend sdVector4 operator * (const float fScalar, const sdVector4& rkVector)
	{
		return sdVector4(
			fScalar * rkVector.m_fX,
			fScalar * rkVector.m_fY,
			fScalar * rkVector.m_fZ,
			fScalar * rkVector.m_fW);
	}

	SD_INLINE friend sdVector4 operator * (const sdVector4& lkVector, const sdVector4& rkVector)
	{
		return sdVector4(
			lkVector.m_fX * rkVector.m_fX,
			lkVector.m_fY * rkVector.m_fY,
			lkVector.m_fZ * rkVector.m_fZ,
			lkVector.m_fW * rkVector.m_fW);
	}

	SD_INLINE friend sdVector4 operator / (const sdVector4& lkVector, const float rfScalar)
	{
		return sdVector4(
			lkVector.m_fX / rfScalar,
			lkVector.m_fY / rfScalar,
			lkVector.m_fZ / rfScalar,
			lkVector.m_fW / rfScalar);
	}

	SD_INLINE friend sdVector4 operator / (const float fScalar, const sdVector4& rkVector)
	{
		return sdVector4(
			fScalar / rkVector.m_fX,
			fScalar / rkVector.m_fY,
			fScalar / rkVector.m_fZ,
			fScalar / rkVector.m_fW);
	}

	SD_INLINE friend sdVector4 operator / (const sdVector4& lkVector, const sdVector4& rkVector)
	{
		return sdVector4(
			lkVector.m_fX / rkVector.m_fX,
			lkVector.m_fY / rkVector.m_fY,
			lkVector.m_fZ / rkVector.m_fZ,
			lkVector.m_fW / rkVector.m_fW);
	}

	// arithmetic updates
	SD_INLINE sdVector4& operator += (const sdVector4& rkVector)
	{
		m_fX += rkVector.m_fX;
		m_fY += rkVector.m_fY;
		m_fZ += rkVector.m_fZ;
		m_fW += rkVector.m_fW;

		return *this;
	}

	SD_INLINE sdVector4& operator += (const float fScalar)
	{
		m_fX += fScalar;
		m_fY += fScalar;
		m_fZ += fScalar;
		m_fW += fScalar;

		return *this;
	}

	SD_INLINE sdVector4& operator -= (const sdVector4& rkVector)
	{
		m_fX -= rkVector.m_fX;
		m_fY -= rkVector.m_fY;
		m_fZ -= rkVector.m_fZ;
		m_fW -= rkVector.m_fW;

		return *this;
	}

	SD_INLINE sdVector4& operator -= (const float fScalar)
	{
		m_fX -= fScalar;
		m_fY -= fScalar;
		m_fZ -= fScalar;
		m_fW -= fScalar;

		return *this;
	}

	SD_INLINE sdVector4& operator *= (const float fScalar)
	{
		m_fX *= fScalar;
		m_fY *= fScalar;
		m_fZ *= fScalar;
		m_fW *= fScalar;

		return *this;
	}

	SD_INLINE sdVector4& operator *= (const sdVector4& rkVector)
	{
		m_fX *= rkVector.m_fX;
		m_fY *= rkVector.m_fY;
		m_fZ *= rkVector.m_fZ;
		m_fW *= rkVector.m_fW;

		return *this;
	}

	SD_INLINE sdVector4& operator /= (const float fScalar)
	{
		SDASSERT(fScalar != 0.0);

		float fInv = 1.0f / fScalar;

		m_fX *= fInv;
		m_fY *= fInv;
		m_fZ *= fInv;
		m_fW *= fInv;

		return *this;
	}

	SD_INLINE sdVector4& operator /= (const sdVector4& rkVector)
	{
		m_fX /= rkVector.m_fX;
		m_fY /= rkVector.m_fY;
		m_fZ /= rkVector.m_fZ;
		m_fW /= rkVector.m_fW;

		return *this;
	}

	SD_INLINE bool operator < (const sdVector4& rhs) const
	{
		if (m_fX < rhs.m_fX && m_fY < rhs.m_fY && m_fZ < rhs.m_fZ && m_fW < rhs.m_fW)
			return true;
		return false;
	}

	SD_INLINE bool operator > (const sdVector4& rhs) const
	{
		if (m_fX > rhs.m_fX && m_fY > rhs.m_fY && m_fZ > rhs.m_fZ && m_fW > rhs.m_fW)
			return true;
		return false;
	}
	// @}


	// 常用
	// @{
	SD_INLINE float Length() const
	{
		return sqrt(m_fX * m_fX + m_fY * m_fY + m_fZ * m_fZ + m_fW * m_fW);
	}

	SD_INLINE float SquaredLength() const
	{
		return m_fX * m_fX + m_fY * m_fY + m_fZ * m_fZ + m_fW * m_fW;
	}

	SD_INLINE float Normalise()
	{	
		float fInvLength = 0.f;
		float fLength = sdMath::Sqrt(m_fX * m_fX + m_fY * m_fY + m_fZ * m_fZ + m_fW * m_fW);
		if (fLength > sdMath::MATH_EPS)
			fInvLength = 1.0f / fLength;

		m_fX *= fInvLength;
		m_fY *= fInvLength;
		m_fZ *= fInvLength;
		m_fW *= fInvLength;

		return fLength;
	}
	// @}
};
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_MATH
BASE_NAMESPACE_END_BASE
#endif