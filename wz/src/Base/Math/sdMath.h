//*************************************************************************************************
// ����:	��ѧ��(��ȡ��Ogre/OgreMath.h)
//---------------------------------------------------------
// ����:		
// ����:		2012-06-30
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_MATH_MATH_H__
#define _SD_BASE_MATH_MATH_H__

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_MATH
class sdVector2;
class sdVector3;
class sdVector4;
class sdMatrix2;
class sdMatrix3;
class sdMatrix4;
class sdQuaternion;
class sdRay;
class sdPlane;
class sdAxisAlignedBox;
BASE_NAMESPACE_END_MATH
BASE_NAMESPACE_END_BASE

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_MATH
//-------------------------------------------------------------------------------------------------
// һЩ����(����wz,��ʱ����)
//-------------------------------------------------------------------------------------------------
//#define	SD_MATH_PI			3.14159265358979323846264338327950288419716939937510582f
//#define	SD_MATH_E			2.71828182845904523536028747135266249775724709369995957f
//#define	SD_MATH_DEG_TO_RAD	SD_MATH_PI / 180.0f
//#define	SD_MATH_RAD_TO_DEG	180.0f  / SD_MATH_PI
//#define	SD_MATH_EPS			10e-6f
//#define	SD_MATH_BIG_EPS		10e-2f
//#define	SD_MATH_SMALL_EPS	10e-6f
//#define	SD_MATH_SQRTHALF	0.7071067811865475244f
//-------------------------------------------------------------------------------------------------
// ������ѧ�����뺯��(��ȡ��Ogre)
//-------------------------------------------------------------------------------------------------
class sdMath : public sdMemObject
{
public:
	// ����ֵ
	SD_STATIC_INLINE float Abs(float fValue){ return (float)fabs(fValue);}

	// 
	SD_STATIC_INLINE float Bias(const float fValue, const float fBias)
	{
		return powf(fValue, logf(fBias) / logf(0.5f));
	}

	//
	SD_STATIC_INLINE float Gain(const float fValue, const float fGain)
	{
		if (fValue < 0.5f)
			return Bias(1.f - fGain, 2.f * fValue) * 0.5f;
		return 1.f - Bias(1.f - fGain, 2.f - 2.f * fValue) * 0.5f;
	}

	// 
	SD_STATIC_INLINE bool FloatEqual(float fX, float fY, float fTolerance)
	{
		return (fX - fY) <= fTolerance && (fY - fX) >= -fTolerance;
	}

	// ƽ��/ƽ����
	// @{
	SD_STATIC_INLINE float Sqr(float fValue) { return fValue*fValue;}
	SD_STATIC_INLINE float Sqrt(float fValue) { return (float)sqrt(fValue);}

	SD_STATIC_INLINE float FastSqrt(float fValue)
	{
		const float fZero = 0.0f;
		const float fThree = 3.0f;
		const float fOneHalf = 0.5f;
		float fTemp;

		__asm
		{
			movss	xmm1, [fValue]
			rsqrtss	xmm0, xmm1		// 1/sqrt estimate(12bits)

			// Newton-Raphson iteration(X1 = 0.5 * x0 * (3 - (y*x0) * x0))
			movss	xmm3, [fThree]
			movss	xmm2, xmm0
			mulss	xmm0, xmm1			// y * x0
			mulss	xmm0, xmm2			// y * x0 * x0
			mulss	xmm2, [fOneHalf];	// 0.5 * x0
			subss	xmm3, xmm0			// 3 - (y*x0) * x0)
			mulss	xmm3, xmm2			// 0.5 * x0 * (3 - (y*x0) * x0))

			movss	xmm4, [fZero];
			cmpss	xmm4, xmm1, 4		// not equal
			mulss	xmm3, xmm1			// sqrt(f) = f * 1/sqrt(f);
			andps	xmm3, xmm4			// seet result to zero if input is zero
			movss	[fTemp], xmm3
		}

		return fTemp;
	}

	SD_STATIC_INLINE float FastInvSqrt(float fValue)
	{
		const float fThree = 3.0f;
		const float fOneHalf = 0.5f;
		float fTemp;

		__asm
		{
			movss	xmm1, [fValue]
			rsqrtss	xmm0, xmm1		// 1/sqrt estimate(12bits)

			// Newton-Raphson iteration(X1 = 0.5 * x0 * (3 - (y*x0) * x0))
			movss	xmm3, [fThree]
			movss	xmm2, xmm0
				mulss	xmm0, xmm1			// y * x0
				mulss	xmm0, xmm2			// y * x0 * x0
				mulss	xmm2, [fOneHalf];	// 0.5 * x0
			subss	xmm3, xmm0			// 3 - (y*x0) * x0)
				mulss	xmm3, xmm2			// 0.5 * x0 * (3 - (y*x0) * x0))
				movss	[fTemp], xmm3
		}

		return fTemp;	
	}
	// @}

	// ȡ����
	// @{
	SD_STATIC_INLINE int Ceil (float fValue) { return int(ceil(fValue)); }
	SD_STATIC_INLINE int Floor (float fValue) { return int(floor(fValue)); }
	
	SD_STATIC_INLINE int FastTrunc(float fValue)
	{
		__asm cvttss2si	eax, [fValue];
		// return value in eax.
	}

	SD_STATIC_INLINE int FastRound(float fValue)
	{
		__asm cvtss2si	eax, [fValue];
		// return value in eax.
	}

	SD_STATIC_INLINE int FastFloor(float fValue)
	{
		const uint mxcsr_floor = 0x00003f80;
		const uint mxcsr_default = 0x00001f80;

		__asm ldmxcsr	[mxcsr_floor];		// Round toward -infinity.
		__asm cvtss2si	eax, [fValue];
		__asm ldmxcsr	[mxcsr_default];	// Round to nearest
		
	}
	// @}

	// �����
	// @{
	SD_STATIC_INLINE int Rand() { return rand();}
	SD_STATIC_INLINE float Rand(float fMaximum) { return rand() * (fMaximum / RAND_MAX);}
	SD_STATIC_INLINE float Frand() { return rand() / (float)RAND_MAX;}
	// @}

	//  
	SD_STATIC_INLINE bool IsPowerOf2(uint uiNum)
	{
		return 0 == (uiNum & (uiNum -1));
	}

	// 
	SD_STATIC_INLINE uint Power2_Floor(uint uiNum)
	{
		while (!IsPowerOf2(uiNum))	uiNum &= uiNum-1;
		return uiNum;
	}

	//
	SD_STATIC_INLINE uint Power2_Ceil(uint uiNum)
	{
		uint uiFloor = Power2_Floor(uiNum);
		if (uiFloor == uiNum)
			return uiFloor;
		else
			return uiFloor << 1;
	}

	// ������Ƕ�ת��
	// @{
	SD_STATIC_INLINE float Degree2Radian(float fDegree) { return fDegree * DEG2RAD;}
	SD_STATIC_INLINE float Radian2Degree(float fRadian) { return fRadian * RAD2DEG;}
	// @}

	// ���㷨
	// @{
	// ������ƽ����(�ο���Ogre)
	//	@param[in]	kRay		����
	//	@param[in]	kPlane		ƽ��
	//	@param[out]	fIntersect	���㵽������ʼ��ľ���
	//	@param[in]	bBack		�Ƿ�ʹ�ñ�����
	static bool Intersects(const sdRay& kRay, const sdPlane& kPlane, float& fIntersect, bool bBack = true);

	// �������������ཻ(�ο��ԡ�ʵʱ���㼰ͼ��ѧ����P322)
	//	@param[in]	kRay		����
	//	@param[in]	kV1,kV2,kV3	������
	//	@param[out]	fIntersect	���㵽������ʼ��ľ���
	//	@param[in]	bBack		�Ƿ�ʹ�ñ�����
	static bool Intersects(const sdRay& kRay, const sdVector3& kV1, const sdVector3& kV2, const sdVector3& kV3, float& fIntersect, bool bBack = true);

	// �������Χ����(�ο���Ogre)
	//	@param[in]	kRay	����
	//	@param[in]	kAABB	��Χ��
	//	@param[out]	fStart	���㵽������ʼ��ľ���
	static bool Intersects(const sdRay& kRay, const sdAxisAlignedBox& kAABB, float& fStart);

	// �������Χ����(�ο���Ogre,���Բο���ʵʱ�����ͼ��ѧ��P319)
	//	@param[in]	kRay	����
	//	@param[in]	kAABB	��Χ��
	//	@param[out]	fStart	�����㵽������ʼ��ľ���
	//	@param[out]	fEnd	Զ���㵽������ʼ��ľ���
	static bool Intersects(const sdRay& kRay, const sdAxisAlignedBox& kAABB, float& fStart, float& fEnd);

	// �������Χ����(�ο���WZ)
	//	@param[in]	kRay	����
	//	@param[in]	kAABB	��Χ��
	//	@param[out]	fStart	�����㵽������ʼ��ľ���
	//	@param[out]	fEnd	Զ���㵽������ʼ��ľ���
	static bool Intersects_V2(const sdRay& kRay, const sdAxisAlignedBox& kAABB, float& fStart, float& fEnd);
	// @}

	// ǯλ
	template <typename T>
	SD_STATIC_INLINE T Clamp(const T& kValue, const T& kMinValue, const T& kMaxValue)
	{
		return kValue < kMinValue ? kMinValue : (kValue > kMaxValue ? kMaxValue : kValue);
	}

	// ȡģ
	template <typename T>
	SD_STATIC_INLINE T Modulo(const T& kValue, const T& kModus)
	{
		kValue -= kModus * int(kValue / kModus);
		if (kValue < 0)
			kValue += kModus;
		return kValue;
	}

	// ��ֵ
	// @{
	// ���Բ�ֵ
	template <typename T>
	SD_STATIC_INLINE T Lerp(const T& kStartValue, const T& kEndValue, float fWeight)
	{
		return kStartValue + static_cast<T>((kEndValue - kStartValue) * fWeight);
	}

	// ˫���Բ�ֵ
	template <typename T>
	SD_STATIC_INLINE T Bilerp(const T& kU, const T& kV, const T& kQ, const T& kR, float fS, float fT)
	{
		return Lerp(Lerp(kU, kV, fS), Lerp(kQ, kR, fS), fT);
	}

	// �������ֵ
	template <typename T>
	SD_STATIC_INLINE T Cubic_Interpolate(const T& kV0, const T& kV1, const T& kV2, const T& kV3, float fT)
	{
		const T kP = kV3 - kV2 - kV0 + kV1;
		const T kQ = kV0 - kV1 - P;
		const T kR = kV2 - kV0;
		return (((kP * fT) + kQ) * fT + kR) * fT + kV1;
	}

	// ���ǲ�ֵ
	template <typename T>
	SD_STATIC_INLINE T Cos_Interpolate(const T& kV0, const T& kV1, float fT)
	{
		const float fV = (1.f - cosf(fT * PI) * 0.5f);
		return kV0 * (1.f - fV) + kV1 * fV;
	}
	// @}

	// ���Դ�������
	// @{
	// @brief calculate the determinet of 2x2 matrix in the form:	
	//				| a1 a2 |					
	//				| b1 b2 |	
	SD_STATIC_INLINE float Det(float fA1, float fA2, float fB1, float fB2)
	{
		return fA1 * fB2 - fB1 * fA2;
	}

	// @brief calculate the determinet of 2x2 matrix in the form:	
	//				| a1 a2 a3 |
	//				| b1 b2 b3 |
	//				| c1 c2 c3 |					
	SD_STATIC_INLINE float Det(float fA1, float fA2, float fA3, float fB1, float fB2, float fB3, float fC1, float fC2, float fC3)
	{
		return fA1 * Det(fB2, fB3, fC2, fC3) - fB1 * Det(fA2, fA3, fC2, fC3) + fC1 * Det(fA2, fA3, fB2, fB3);
	}

	// �ӷ�(ԭ��)
	SD_STATIC sdVector2& Add(sdVector2& lkVector, const sdVector2& rkVector);
	SD_STATIC sdVector3& Add(sdVector3& lkVector, const sdVector3& rkVector);
	SD_STATIC sdVector4& Add(sdVector4& lkVector, const sdVector4& rkVector);

	SD_STATIC sdMatrix2& Add(sdMatrix2& lkMatrix, const sdMatrix2& rkMatrix);
	SD_STATIC sdMatrix3& Add(sdMatrix3& lkMatrix, const sdMatrix3& rkMatrix);
	SD_STATIC sdMatrix4& Add(sdMatrix4& lkMatrix, const sdMatrix4& rkMatrix);

	// �ӷ�
	SD_STATIC sdVector2& Add(sdVector2& kValue, const sdVector2& lkVector, const sdVector2& rkVector);
	SD_STATIC sdVector3& Add(sdVector3& kValue, const sdVector3& lkVector, const sdVector3& rkVector);
	SD_STATIC sdVector4& Add(sdVector4& kValue, const sdVector4& lkVector, const sdVector4& rkVector);

	SD_STATIC sdMatrix2& Add(sdMatrix2& kValue, const sdMatrix2& lkMatrix, const sdMatrix2& rkMatrix);
	SD_STATIC sdMatrix3& Add(sdMatrix3& kValue, const sdMatrix3& lkMatrix, const sdMatrix3& rkMatrix);
	SD_STATIC sdMatrix4& Add(sdMatrix4& kValue, const sdMatrix4& lkMatrix, const sdMatrix4& rkMatrix);

	// ����(ԭ��)
	SD_STATIC sdVector2& Sub(sdVector2& lkVector, const sdVector2& rkVector);
	SD_STATIC sdVector3& Sub(sdVector3& lkVector, const sdVector3& rkVector);
	SD_STATIC sdVector4& Sub(sdVector4& lkVector, const sdVector4& rkVector);

	SD_STATIC sdMatrix2& Sub(sdMatrix2& lkMatrix, const sdMatrix2& rkMatrix);
	SD_STATIC sdMatrix3& Sub(sdMatrix3& lkMatrix, const sdMatrix3& rkMatrix);
	SD_STATIC sdMatrix4& Sub(sdMatrix4& lkMatrix, const sdMatrix4& rkMatrix);

	// ����
	SD_STATIC sdVector2& Sub(sdVector2& kValue, const sdVector2& lkVector, const sdVector2& rkVector);
	SD_STATIC sdVector3& Sub(sdVector3& kValue, const sdVector3& lkVector, const sdVector3& rkVector);
	SD_STATIC sdVector4& Sub(sdVector4& kValue, const sdVector4& lkVector, const sdVector4& rkVector);

	SD_STATIC sdMatrix2& Sub(sdMatrix2& kValue, const sdMatrix2& lkMatrix, const sdMatrix2& rkMatrix);
	SD_STATIC sdMatrix3& Sub(sdMatrix3& kValue, const sdMatrix3& lkMatrix, const sdMatrix3& rkMatrix);
	SD_STATIC sdMatrix4& Sub(sdMatrix4& kValue, const sdMatrix4& lkMatrix, const sdMatrix4& rkMatrix);

	// �˷�(ԭ��)
	SD_STATIC sdVector2& Mult(sdVector2& lkVector, const float fScalar);
	SD_STATIC sdVector3& Mult(sdVector3& lkVector, const float fScalar);
	SD_STATIC sdVector4& Mult(sdVector4& lkVector, const float fScalar);

	SD_STATIC sdMatrix2& Mult(sdMatrix2& lkMatrix, const float fScalar);
	SD_STATIC sdMatrix3& Mult(sdMatrix3& lkMatrix, const float fScalar);
	SD_STATIC sdMatrix4& Mult(sdMatrix4& lkMatrix, const float fScalar);

	// �˷�
	SD_STATIC sdVector2& Mult(sdVector2& kValue, const sdVector2& lkVector, const float fScalar);
	SD_STATIC sdVector3& Mult(sdVector3& kValue, const sdVector3& lkVector, const float fScalar);
	SD_STATIC sdVector4& Mult(sdVector4& kValue, const sdVector4& lkVector, const float fScalar);

	SD_STATIC sdMatrix2& Mult(sdMatrix2& kValue, const sdMatrix2& lkMatrix, const float fScalar);
	SD_STATIC sdMatrix3& Mult(sdMatrix3& kValue, const sdMatrix3& lkMatrix, const float fScalar);
	SD_STATIC sdMatrix4& Mult(sdMatrix4& kValue, const sdMatrix4& lkMatrix, const float fScalar);	

	SD_STATIC sdVector3& Mult(sdVector3& kValue, const sdVector3& lkVector, const sdVector3& rkVector);

	SD_STATIC sdVector3& Mult(sdVector3& kValue, const sdVector3& lkVector, const sdMatrix3& rkMatrix);
	SD_STATIC sdVector3& Mult(sdVector3& kValue, const sdMatrix3& lkMatrix, const sdVector3& rkVector);

	SD_STATIC sdVector4& Mult(sdVector4& kValue, const sdVector4& lkVector, const sdMatrix4& rkMatrix);
	SD_STATIC sdVector4& Mult(sdVector4& kValue, const sdMatrix4& lkMatrix, const sdVector4& rkVector);

	SD_STATIC sdMatrix3& Mult(sdMatrix3& kValue, const sdMatrix3& lkMatrix, const sdMatrix3& rkMatrix);
	SD_STATIC sdMatrix4& Mult(sdMatrix4& kValue, const sdMatrix4& lkMatrix, const sdMatrix4& rkMatrix);
	
	// �ӳ�
	SD_STATIC sdVector3& Madd(sdVector3& kValue, const sdVector3& lkVector, const float fScalar);

	// ��ֵ
	SD_STATIC sdVector2& Assignment(sdVector2& kValue, const sdVector2& kVector);
	SD_STATIC sdVector3& Assignment(sdVector3& kValue, const sdVector3& kVector);
	SD_STATIC sdVector4& Assignment(sdVector4& kValue, const sdVector4& kVector);	

	// ȡ��
	SD_STATIC sdVector2& Negative(sdVector2& kVector);
	SD_STATIC sdVector3& Negative(sdVector3& kVector);
	SD_STATIC sdVector4& Negative(sdVector4& kVector);

	SD_STATIC sdVector2& Negative(sdVector2& kValue, const sdVector2& kVector);
	SD_STATIC sdVector3& Negative(sdVector3& kValue, const sdVector3& kVector);
	SD_STATIC sdVector4& Negative(sdVector4& kValue, const sdVector4& kVector);

	// ��һ��
	SD_STATIC float Normalize(sdVector2& kVector);
	SD_STATIC float Normalize(sdVector3& kVector);
	SD_STATIC float Normalize(sdVector4& kVector);
	SD_STATIC float Normalize(sdQuaternion& kQuaternion);
	
	// ���
	SD_STATIC float Dot(const sdVector3& lkVector, const sdVector3& rkVector);
	SD_STATIC float Dot(const sdVector4& lkVector, const sdVector4& rkVector);
	
	// ���
	SD_STATIC sdVector3& Cross(sdVector3& kValue, const sdVector3& lkVector, const sdVector3& rkVector);

	// ����(�ο���WZ)
	SD_STATIC sdVector3& Reflect(sdVector3& kValue, const sdVector3& kLight, const sdVector3& kNormal);

	// @brief ����ʽ��ֵ
	// @param[in]	kMatrix	����ʽ
	// @return				ֵ
	SD_STATIC float Determinet(sdMatrix3& kMatrix);

	// @brief ����ת��(ԭ��ת��)
	// @param[in]	kMatrix	��ת�þ���
	// @return				ת�ý��
	SD_STATIC sdMatrix3& Transpose(sdMatrix3& kMatrix);
	SD_STATIC sdMatrix4& Transpose(sdMatrix4& kMatrix);

	// @brief ����ת��
	// @param[out]	kValue	ת�ý��
	// @param[in]	kMatrix	��ת�þ���
	// @return				ת�ý��
	SD_STATIC sdMatrix3& Transpose(sdMatrix3& kValue, sdMatrix3& kMatrix);
	SD_STATIC sdMatrix4& Transpose(sdMatrix4& kValue, const sdMatrix4& kMatrix);

	// @brief ��������(�ο���WZ)
	// @param[out]	kValue	������
	// @param[in]	kMatrix	���������
	// @return				������
	SD_STATIC sdMatrix3& Invert(sdMatrix3& kValue, const sdMatrix3& kMatrix);
	SD_STATIC sdMatrix4& Invert(sdMatrix4& kValue, const sdMatrix4& kMatrix);

	// @brief �����ı任(�ο���WZ)
	// @param[out]	kValue
	// @param[in]	kPosition	���任�ĵ�
	// @param[in]	kMatrix		�任����
	// @return
	SD_STATIC sdVector3& TransformPosition(sdVector3& kValue, const sdVector3& kPosition, const sdMatrix4& kMatrix);
	SD_STATIC sdVector3& TransformPosition(sdVector3& kValue, const sdMatrix4& kMatrix, const sdVector3& kPosition);

	// @brief ���㷽��ı任(�ο���WZ)
	// @note  ��ѧ����Ҫ�任������T��������ת�þ���,���������������������
	// @param[out]	kValue
	// @param[in]	kPosition	���任��ʸ��
	// @param[in]	kMatrix		�任����
	// @return
	SD_STATIC sdVector3& TransformDirection(sdVector3& kValue, const sdVector3& kDirection, const sdMatrix4& kMatrix);
	SD_STATIC sdVector3& TransformDirection(sdVector3& kValue, const sdMatrix4& kMatrix, const sdVector3& kDirection);

	// @brief �����ĳ�㿴��Ŀ���ı任����(���ֲο���WZ��	
	//			LookAtRHToRH
	//				����ռ�Ϊ��������ϵ
	//				�۲�ռ�Ϊ��������ϵ:�۲��Ϊԭ��,����ΪX,����ΪY,����ΪZ
	//			LookAtRHToLH
	//				����ռ�Ϊ��������ϵ
	//				�۲�ռ�Ϊ��������ϵ:�۲��Ϊԭ��,����ΪX,����ΪY,����ΪZ
	//			LookAtLHToRH
	//				����ռ�Ϊ��������ϵ
	//				�۲�ռ�Ϊ��������ϵ:�۲��Ϊԭ��,����ΪX,����ΪY,����ΪZ
	//			LookAtLHToLH
	//				����ռ�Ϊ��������ϵ
	//				�۲�ռ�Ϊ��������ϵ:�۲��Ϊԭ��,����ΪX,����ΪY,����ΪZ
	// @note �ο�D3DSDK�ĵ�D3DXMatrixLookAtLH��D3DXMatrixLookAtRH����
	// @param[out]	kValue
	// @param[in]	kEye	�ӵ�
	// @param[in]	kTarget	Ŀ���
	// @param[in]	kUp		��������ϵ������������
	// @return
	SD_STATIC sdMatrix4& LookAtRHToRH(
		sdMatrix4& kValue, 
		const sdVector3& kEye, 
		const sdVector3& kTarget, 
		const sdVector3& kUp);

	SD_STATIC sdMatrix4& LookAtRHToLH(
		sdMatrix4& kValue, 
		const sdVector3& kEye, 
		const sdVector3& kTarget, 
		const sdVector3& kUp);

	SD_STATIC sdMatrix4& LookAtLHToRH(
		sdMatrix4& kValue, 
		const sdVector3& kEye, 
		const sdVector3& kTarget, 
		const sdVector3& kUp);

	SD_STATIC sdMatrix4& LookAtLHToLH(
		sdMatrix4& kValue, 
		const sdVector3& kEye, 
		const sdVector3& kTarget, 
		const sdVector3& kUp);

	// @brief ����͸�ӱ任ʱ�۲�ռ䵽ͶӰ�ռ�ı任����(�ο���WZ,������)
	//			Perspective
	//				�۲�ռ���׶�壺(l,b,n)(r,t,f)
	//				ͶӰ�ռ�ü������壺[-1,1][-1,1][-1,1]
	//				�۲�ռ�Ϊ��������ϵ�汾������ΪX,����ΪY,����ΪZ
	//				ͶӰ�ռ�Ϊ��������ϵ�汾������ΪX,����ΪY,����ΪZ
	//			PerspectiveGL
	//				�۲�ռ���׶�壺(l,b,n)(r,t,f)
	//				ͶӰ�ռ�ü������壺[-1,1][-1,1][-1,1]
	//				�۲�ռ�Ϊ��������ϵ�汾������ΪX,����ΪY,����ΪZ
	//				ͶӰ�ռ�Ϊ��������ϵ�汾������ΪX,����ΪY,����ΪZ(���ֹ���豸����ϵ)
	//			PerspectiveD3DLH
	//				����Direct3D�Ĺ������ͶӰת��
	//				�۲�ռ���׶�壺(l,b,n)(r,t,f)
	//				ͶӰ�ռ�ü������壺[-1,1][-1,1][0,1]
	//				�۲�ռ�Ϊ��������ϵ�汾������ΪX,����ΪY,����ΪZ
	//				ͶӰ�ռ�Ϊ��������ϵ�汾������ΪX,����ΪY,����ΪZ(���ֹ���豸����ϵ)
	//			PerspectiveD3DRH
	//				����Direct3D�Ĺ������ͶӰת��
	//				�۲�ռ���׶�壺[l,r][b,t][n,f]
	//				ͶӰ�ռ�ü������壺[-1,1][-1,1][0,1]
	//				�۲�ռ�Ϊ��������ϵ�汾������ΪX,����ΪY,����ΪZ
	//				ͶӰ�ռ�Ϊ��������ϵ�汾������ΪX,����ΪY,����ΪZ				
	// @note  �ο���ʵʱ�����ͼ��ѧ��P34-39
	//		  �ο�OpenGL�ĵ�glFrustum����
	//		  �ο�Direct3D�ĵ�D3DXMatrixPerspectiveLH\D3DXMatrixPerspectiveRH\
	//			D3DXMatrixPerspectiveOffCenterLH\D3DXMatrixPerspectiveOffCenterRH����
	// @param[out]	kValue
	// @param[in]	fLeft\fRight\fBottom\fTop\fNear\fFar	ͶӰ�ռ�Ĳ���ֵ
	// @return
	SD_STATIC sdMatrix4& Perspective(
		sdMatrix4& kValue, 
		const float fLeft,
		const float fRight,
		const float fBottom,
		const float fTop,
		const float fNear,
		const float fFar);

	SD_STATIC sdMatrix4& PerspectiveGL(
		sdMatrix4& kValue, 
		const float fLeft,
		const float fRight,
		const float fBottom,
		const float fTop,
		const float fNear,
		const float fFar);

	SD_STATIC sdMatrix4& PerspectiveD3DLH(
		sdMatrix4& kValue, 
		const float fLeft,
		const float fRight,
		const float fBottom,
		const float fTop,
		const float fNear,
		const float fFar);

	SD_STATIC sdMatrix4& PerspectiveD3DRH(
		sdMatrix4& kValue, 
		const float fLeft,
		const float fRight,
		const float fBottom,
		const float fTop,
		const float fNear,
		const float fFar);

	// @brief ����͸�ӱ任ʱ�۲�ռ䵽ͶӰ�ռ�ı任����(�ο���WZ,������)
	// @param[in]	fFovY	��ֱ�ӽ�(��λ:����)
	// @param[in]	fAspect	��߱�
	// @param[in]	fNear	�����ü���ľ���(��ֵ)
	// @param[in]	fFar	��Զ�ü���ľ���(��ֵ)
	SD_STATIC sdMatrix4& PerspectiveFov(
		sdMatrix4& kValue,
		const float fFovY,
		const float fAspect,
		const float fNear,
		const float fFar);

	// @brief ���������任ʱ�۲�ռ䵽ͶӰ�ռ�ı任����(�ο���WZ,������)
	//			Ortho
	//				�۲�ռ���׶�壺[l,r][b,t][n,f]
	//				ͶӰ�ռ�ü������壺[-1,1][-1,1][-1,1]
	//				�۲�ռ�Ϊ��������ϵ�汾������ΪX,����ΪY,����ΪZ
	//				ͶӰ�ռ�Ϊ��������ϵ�汾������ΪX,����ΪY,����ΪZ
	//			OrthoGL
	//				����OpenGL�Ĺ������ͶӰת��
	//				�۲�ռ���׶�壺[l,r][b,t][n,f]
	//				ͶӰ�ռ�ü������壺[-1,1][-1,1][-1,1]
	//				�۲�ռ�Ϊ��������ϵ�汾������ΪX,����ΪY,����ΪZ
	//				ͶӰ�ռ�Ϊ��������ϵ�汾������ΪX,����ΪY,����ΪZ(���ֹ���豸����ϵ)
	//			OrthoD3DLH
	//				����Direct3D�Ĺ������ͶӰת��
	//				�۲�ռ���׶�壺[l,r][b,t][n,f]
	//				ͶӰ�ռ�ü������壺[-1,1][-1,1][0,1]
	//				�۲�ռ�Ϊ��������ϵ�汾������ΪX,����ΪY,����ΪZ
	//				ͶӰ�ռ�Ϊ��������ϵ�汾������ΪX,����ΪY,����ΪZ(���ֹ���豸����ϵ)
	//			OrthoD3DRH
	//				����Direct3D�Ĺ������ͶӰת��
	//				�۲�ռ���׶�壺[l,r][b,t][n,f]
	//				ͶӰ�ռ�ü������壺[-1,1][-1,1][0,1]
	//				�۲�ռ�Ϊ��������ϵ�汾������ΪX,����ΪY,����ΪZ
	//				ͶӰ�ռ�Ϊ��������ϵ�汾������ΪX,����ΪY,����ΪZ
	// @note  �ο���ʵʱ�����ͼ��ѧ��P34-39
	//		  �ο�OpenGL�ĵ�glOrtho����
	//		  �ο�Direct3D�ĵ�D3DXMatrixOrthoLH\D3DXMatrixOrthoRH\
	//			D3DXMatrixOrthoOffCenterLH\D3DXMatrixOrthoOffCenterRH����
	// @param[out]	kValue
	// @param[in]	fLeft\fRight\fBottom\fTop\fNear\fFar	ͶӰ�ռ�Ĳ���ֵ
	// @return
	SD_STATIC sdMatrix4& Ortho(
		sdMatrix4& kValue, 
		const float fLeft,
		const float fRight,
		const float fBottom,
		const float fTop,
		const float fNear,
		const float fFar);

	SD_STATIC sdMatrix4& OrthoGL(
		sdMatrix4& kValue, 
		const float fLeft,
		const float fRight,
		const float fBottom,
		const float fTop,
		const float fNear,
		const float fFar);

	SD_STATIC sdMatrix4& OrthoD3DLH(
		sdMatrix4& kValue, 
		const float fLeft,
		const float fRight,
		const float fBottom,
		const float fTop,
		const float fNear,
		const float fFar);

	SD_STATIC sdMatrix4& OrthoD3DRH(
		sdMatrix4& kValue, 
		const float fLeft,
		const float fRight,
		const float fBottom,
		const float fTop,
		const float fNear,
		const float fFar);
	// @}


public:
	static const float	POS_INFINITY;	///< float�ܱ�ʾ����󸡵���
	static const float	NEG_INFINITY;	///< float�ܱ�ʾ����С������

	static const float	MATH_EPS;		///< ���ֵ(���ڴ��������)
	static const float	MATH_BIG_EPS;	///< ���ֵ
	static const float	MATH_SMALL_EPS;	///< ���ֵ

	static const float	E;				///< ��Ȼ�����ĵ�
	static const float	PI;				///< Բ����
	static const float	TWO_PI;
	static const float	HALF_PI;
	static const float	DEG2RAD;		///< ����ת����
	static const float	RAD2DEG;		///< ����ת����
};
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_MATH
BASE_NAMESPACE_END_BASE
#endif