//*************************************************************************************************
// ����:	�ο���Ogre::Quaternion/WorldZero
//---------------------------------------------------------
// ����:		
// ����:		2014-03-26
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_MATH_QUATERNION_H__
#define _SD_BASE_MATH_QUATERNION_H__

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_MATH
//-------------------------------------------------------------------------------------------------
class sdQuaternion : public sdMemObject
{
public:
	union
	{
		struct 
		{
			float m_fX;		///< ��������
			float m_fY;		///<
			float m_fZ;		///<
			float m_fW;		///<
		};
		float m_afValue[4];	///< �������	
	};

public:
	static const sdQuaternion IDENTITY;

public:
	SD_INLINE sdQuaternion() 
	{
	}

	SD_INLINE sdQuaternion(const float fX, const float fY, const float fZ, const float fW) 
		: m_fX(fX), m_fY(fY), m_fZ(fZ), m_fW(fW)
	{

	};

	SD_INLINE sdQuaternion(const sdQuaternion& kQuaternion)
		: m_fX(kQuaternion.m_fX), m_fY(kQuaternion.m_fY), m_fZ(kQuaternion.m_fZ), m_fW(kQuaternion.m_fW)
	{

	}

	// ��ָ������תָ���Ƕ�
	SD_INLINE sdQuaternion(const sdVector3& kAxis, float fAngle)
	{
		float fLength = kAxis.Length();
		if (fLength > 0)
		{
			float fInvLength = 1.f / fLength;
			float fHalfAngle = fAngle * 0.5f;
			float fScale = sinf(fHalfAngle) * fInvLength;
			m_fX = fScale * kAxis.m_fX;
			m_fY = fScale * kAxis.m_fY;
			m_fZ = fScale * kAxis.m_fZ;
			m_fW = cosf(fHalfAngle);
		}
	}


	// ����
	// @{
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