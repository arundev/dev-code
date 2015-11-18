//*************************************************************************************************
// 内容:	数学库(抽取自Ogre/OgreMath.h)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-06-30
// 最后修改:
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
// 一些常用(来自wz,暂时保留)
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
// 常用数学常量与函数(抽取自Ogre)
//-------------------------------------------------------------------------------------------------
class sdMath : public sdMemObject
{
public:
	// 绝对值
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

	// 平方/平方根
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

	// 取整数
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

	// 随机数
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

	// 弧度与角度转换
	// @{
	SD_STATIC_INLINE float Degree2Radian(float fDegree) { return fDegree * DEG2RAD;}
	SD_STATIC_INLINE float Radian2Degree(float fRadian) { return fRadian * RAD2DEG;}
	// @}

	// 求交算法
	// @{
	// 射线与平面求交(参考自Ogre)
	//	@param[in]	kRay		射线
	//	@param[in]	kPlane		平面
	//	@param[out]	fIntersect	交点到射线起始点的距离
	//	@param[in]	bBack		是否使用背面求交
	static bool Intersects(const sdRay& kRay, const sdPlane& kPlane, float& fIntersect, bool bBack = true);

	// 射线与三角形相交(参考自《实时计算及图形学》的P322)
	//	@param[in]	kRay		射线
	//	@param[in]	kV1,kV2,kV3	三角形
	//	@param[out]	fIntersect	交点到射线起始点的距离
	//	@param[in]	bBack		是否使用背面求交
	static bool Intersects(const sdRay& kRay, const sdVector3& kV1, const sdVector3& kV2, const sdVector3& kV3, float& fIntersect, bool bBack = true);

	// 射线与包围盒求交(参考自Ogre)
	//	@param[in]	kRay	射线
	//	@param[in]	kAABB	包围盒
	//	@param[out]	fStart	交点到射线起始点的距离
	static bool Intersects(const sdRay& kRay, const sdAxisAlignedBox& kAABB, float& fStart);

	// 射线与包围盒求交(参考自Ogre,可以参考《实时计算机图形学》P319)
	//	@param[in]	kRay	射线
	//	@param[in]	kAABB	包围盒
	//	@param[out]	fStart	近交点到射线起始点的距离
	//	@param[out]	fEnd	远交点到射线起始点的距离
	static bool Intersects(const sdRay& kRay, const sdAxisAlignedBox& kAABB, float& fStart, float& fEnd);

	// 射线与包围盒求交(参考自WZ)
	//	@param[in]	kRay	射线
	//	@param[in]	kAABB	包围盒
	//	@param[out]	fStart	近交点到射线起始点的距离
	//	@param[out]	fEnd	远交点到射线起始点的距离
	static bool Intersects_V2(const sdRay& kRay, const sdAxisAlignedBox& kAABB, float& fStart, float& fEnd);
	// @}

	// 钳位
	template <typename T>
	SD_STATIC_INLINE T Clamp(const T& kValue, const T& kMinValue, const T& kMaxValue)
	{
		return kValue < kMinValue ? kMinValue : (kValue > kMaxValue ? kMaxValue : kValue);
	}

	// 取模
	template <typename T>
	SD_STATIC_INLINE T Modulo(const T& kValue, const T& kModus)
	{
		kValue -= kModus * int(kValue / kModus);
		if (kValue < 0)
			kValue += kModus;
		return kValue;
	}

	// 插值
	// @{
	// 线性插值
	template <typename T>
	SD_STATIC_INLINE T Lerp(const T& kStartValue, const T& kEndValue, float fWeight)
	{
		return kStartValue + static_cast<T>((kEndValue - kStartValue) * fWeight);
	}

	// 双线性插值
	template <typename T>
	SD_STATIC_INLINE T Bilerp(const T& kU, const T& kV, const T& kQ, const T& kR, float fS, float fT)
	{
		return Lerp(Lerp(kU, kV, fS), Lerp(kQ, kR, fS), fT);
	}

	// 立方体插值
	template <typename T>
	SD_STATIC_INLINE T Cubic_Interpolate(const T& kV0, const T& kV1, const T& kV2, const T& kV3, float fT)
	{
		const T kP = kV3 - kV2 - kV0 + kV1;
		const T kQ = kV0 - kV1 - P;
		const T kR = kV2 - kV0;
		return (((kP * fT) + kQ) * fT + kR) * fT + kV1;
	}

	// 三角插值
	template <typename T>
	SD_STATIC_INLINE T Cos_Interpolate(const T& kV0, const T& kV1, float fT)
	{
		const float fV = (1.f - cosf(fT * PI) * 0.5f);
		return kV0 * (1.f - fV) + kV1 * fV;
	}
	// @}

	// 线性代数计算
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

	// 加法(原地)
	SD_STATIC sdVector2& Add(sdVector2& lkVector, const sdVector2& rkVector);
	SD_STATIC sdVector3& Add(sdVector3& lkVector, const sdVector3& rkVector);
	SD_STATIC sdVector4& Add(sdVector4& lkVector, const sdVector4& rkVector);

	SD_STATIC sdMatrix2& Add(sdMatrix2& lkMatrix, const sdMatrix2& rkMatrix);
	SD_STATIC sdMatrix3& Add(sdMatrix3& lkMatrix, const sdMatrix3& rkMatrix);
	SD_STATIC sdMatrix4& Add(sdMatrix4& lkMatrix, const sdMatrix4& rkMatrix);

	// 加法
	SD_STATIC sdVector2& Add(sdVector2& kValue, const sdVector2& lkVector, const sdVector2& rkVector);
	SD_STATIC sdVector3& Add(sdVector3& kValue, const sdVector3& lkVector, const sdVector3& rkVector);
	SD_STATIC sdVector4& Add(sdVector4& kValue, const sdVector4& lkVector, const sdVector4& rkVector);

	SD_STATIC sdMatrix2& Add(sdMatrix2& kValue, const sdMatrix2& lkMatrix, const sdMatrix2& rkMatrix);
	SD_STATIC sdMatrix3& Add(sdMatrix3& kValue, const sdMatrix3& lkMatrix, const sdMatrix3& rkMatrix);
	SD_STATIC sdMatrix4& Add(sdMatrix4& kValue, const sdMatrix4& lkMatrix, const sdMatrix4& rkMatrix);

	// 减法(原地)
	SD_STATIC sdVector2& Sub(sdVector2& lkVector, const sdVector2& rkVector);
	SD_STATIC sdVector3& Sub(sdVector3& lkVector, const sdVector3& rkVector);
	SD_STATIC sdVector4& Sub(sdVector4& lkVector, const sdVector4& rkVector);

	SD_STATIC sdMatrix2& Sub(sdMatrix2& lkMatrix, const sdMatrix2& rkMatrix);
	SD_STATIC sdMatrix3& Sub(sdMatrix3& lkMatrix, const sdMatrix3& rkMatrix);
	SD_STATIC sdMatrix4& Sub(sdMatrix4& lkMatrix, const sdMatrix4& rkMatrix);

	// 减法
	SD_STATIC sdVector2& Sub(sdVector2& kValue, const sdVector2& lkVector, const sdVector2& rkVector);
	SD_STATIC sdVector3& Sub(sdVector3& kValue, const sdVector3& lkVector, const sdVector3& rkVector);
	SD_STATIC sdVector4& Sub(sdVector4& kValue, const sdVector4& lkVector, const sdVector4& rkVector);

	SD_STATIC sdMatrix2& Sub(sdMatrix2& kValue, const sdMatrix2& lkMatrix, const sdMatrix2& rkMatrix);
	SD_STATIC sdMatrix3& Sub(sdMatrix3& kValue, const sdMatrix3& lkMatrix, const sdMatrix3& rkMatrix);
	SD_STATIC sdMatrix4& Sub(sdMatrix4& kValue, const sdMatrix4& lkMatrix, const sdMatrix4& rkMatrix);

	// 乘法(原地)
	SD_STATIC sdVector2& Mult(sdVector2& lkVector, const float fScalar);
	SD_STATIC sdVector3& Mult(sdVector3& lkVector, const float fScalar);
	SD_STATIC sdVector4& Mult(sdVector4& lkVector, const float fScalar);

	SD_STATIC sdMatrix2& Mult(sdMatrix2& lkMatrix, const float fScalar);
	SD_STATIC sdMatrix3& Mult(sdMatrix3& lkMatrix, const float fScalar);
	SD_STATIC sdMatrix4& Mult(sdMatrix4& lkMatrix, const float fScalar);

	// 乘法
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
	
	// 加乘
	SD_STATIC sdVector3& Madd(sdVector3& kValue, const sdVector3& lkVector, const float fScalar);

	// 赋值
	SD_STATIC sdVector2& Assignment(sdVector2& kValue, const sdVector2& kVector);
	SD_STATIC sdVector3& Assignment(sdVector3& kValue, const sdVector3& kVector);
	SD_STATIC sdVector4& Assignment(sdVector4& kValue, const sdVector4& kVector);	

	// 取反
	SD_STATIC sdVector2& Negative(sdVector2& kVector);
	SD_STATIC sdVector3& Negative(sdVector3& kVector);
	SD_STATIC sdVector4& Negative(sdVector4& kVector);

	SD_STATIC sdVector2& Negative(sdVector2& kValue, const sdVector2& kVector);
	SD_STATIC sdVector3& Negative(sdVector3& kValue, const sdVector3& kVector);
	SD_STATIC sdVector4& Negative(sdVector4& kValue, const sdVector4& kVector);

	// 归一化
	SD_STATIC float Normalize(sdVector2& kVector);
	SD_STATIC float Normalize(sdVector3& kVector);
	SD_STATIC float Normalize(sdVector4& kVector);
	SD_STATIC float Normalize(sdQuaternion& kQuaternion);
	
	// 点积
	SD_STATIC float Dot(const sdVector3& lkVector, const sdVector3& rkVector);
	SD_STATIC float Dot(const sdVector4& lkVector, const sdVector4& rkVector);
	
	// 叉乘
	SD_STATIC sdVector3& Cross(sdVector3& kValue, const sdVector3& lkVector, const sdVector3& rkVector);

	// 反射(参考自WZ)
	SD_STATIC sdVector3& Reflect(sdVector3& kValue, const sdVector3& kLight, const sdVector3& kNormal);

	// @brief 行列式的值
	// @param[in]	kMatrix	行列式
	// @return				值
	SD_STATIC float Determinet(sdMatrix3& kMatrix);

	// @brief 矩阵转置(原地转置)
	// @param[in]	kMatrix	待转置矩阵
	// @return				转置结果
	SD_STATIC sdMatrix3& Transpose(sdMatrix3& kMatrix);
	SD_STATIC sdMatrix4& Transpose(sdMatrix4& kMatrix);

	// @brief 矩阵转置
	// @param[out]	kValue	转置结果
	// @param[in]	kMatrix	待转置矩阵
	// @return				转置结果
	SD_STATIC sdMatrix3& Transpose(sdMatrix3& kValue, sdMatrix3& kMatrix);
	SD_STATIC sdMatrix4& Transpose(sdMatrix4& kValue, const sdMatrix4& kMatrix);

	// @brief 矩阵求逆(参考自WZ)
	// @param[out]	kValue	求逆结果
	// @param[in]	kMatrix	待求逆矩阵
	// @return				求逆结果
	SD_STATIC sdMatrix3& Invert(sdMatrix3& kValue, const sdMatrix3& kMatrix);
	SD_STATIC sdMatrix4& Invert(sdMatrix4& kValue, const sdMatrix4& kMatrix);

	// @brief 计算点的变换(参考自WZ)
	// @param[out]	kValue
	// @param[in]	kPosition	待变换的点
	// @param[in]	kMatrix		变换矩阵
	// @return
	SD_STATIC sdVector3& TransformPosition(sdVector3& kValue, const sdVector3& kPosition, const sdMatrix4& kMatrix);
	SD_STATIC sdVector3& TransformPosition(sdVector3& kValue, const sdMatrix4& kMatrix, const sdVector3& kPosition);

	// @brief 计算方向的变换(参考自WZ)
	// @note  数学上需要变换矩阵是T的逆矩阵的转置矩阵,对正交矩阵而言是它本身
	// @param[out]	kValue
	// @param[in]	kPosition	待变换的矢量
	// @param[in]	kMatrix		变换矩阵
	// @return
	SD_STATIC sdVector3& TransformDirection(sdVector3& kValue, const sdVector3& kDirection, const sdMatrix4& kMatrix);
	SD_STATIC sdVector3& TransformDirection(sdVector3& kValue, const sdMatrix4& kMatrix, const sdVector3& kDirection);

	// @brief 计算从某点看向目标点的变换矩阵(部分参考自WZ）	
	//			LookAtRHToRH
	//				世界空间为右手坐标系
	//				观察空间为右手坐标系:观察点为原点,向右为X,向上为Y,向里为Z
	//			LookAtRHToLH
	//				世界空间为右手坐标系
	//				观察空间为左手坐标系:观察点为原点,向右为X,向上为Y,向外为Z
	//			LookAtLHToRH
	//				世界空间为左手坐标系
	//				观察空间为右手坐标系:观察点为原点,向右为X,向上为Y,向里为Z
	//			LookAtLHToLH
	//				世界空间为左手坐标系
	//				观察空间为左手坐标系:观察点为原点,向右为X,向上为Y,向外为Z
	// @note 参考D3DSDK文档D3DXMatrixLookAtLH和D3DXMatrixLookAtRH函数
	// @param[out]	kValue
	// @param[in]	kEye	视点
	// @param[in]	kTarget	目标点
	// @param[in]	kUp		世界坐标系的向上坐标轴
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

	// @brief 计算透视变换时观察空间到投影空间的变换矩阵(参考自WZ,经修正)
	//			Perspective
	//				观察空间视锥体：(l,b,n)(r,t,f)
	//				投影空间裁剪立方体：[-1,1][-1,1][-1,1]
	//				观察空间为右手坐标系版本：向右为X,向上为Y,向里为Z
	//				投影空间为右手坐标系版本：向右为X,向上为Y,向里为Z
	//			PerspectiveGL
	//				观察空间视锥体：(l,b,n)(r,t,f)
	//				投影空间裁剪立方体：[-1,1][-1,1][-1,1]
	//				观察空间为右手坐标系版本：向右为X,向上为Y,向里为Z
	//				投影空间为左手坐标系版本：向右为X,向上为Y,向外为Z(左手规格化设备坐标系)
	//			PerspectiveD3DLH
	//				按照Direct3D的规则进行投影转换
	//				观察空间视锥体：(l,b,n)(r,t,f)
	//				投影空间裁剪立方体：[-1,1][-1,1][0,1]
	//				观察空间为左手坐标系版本：向右为X,向上为Y,向外为Z
	//				投影空间为左手坐标系版本：向右为X,向上为Y,向外为Z(左手规格化设备坐标系)
	//			PerspectiveD3DRH
	//				按照Direct3D的规则进行投影转换
	//				观察空间视锥体：[l,r][b,t][n,f]
	//				投影空间裁剪立方体：[-1,1][-1,1][0,1]
	//				观察空间为左手坐标系版本：向右为X,向上为Y,向外为Z
	//				投影空间为右手坐标系版本：向右为X,向上为Y,向里为Z				
	// @note  参考《实时计算机图形学》P34-39
	//		  参考OpenGL文档glFrustum函数
	//		  参考Direct3D文档D3DXMatrixPerspectiveLH\D3DXMatrixPerspectiveRH\
	//			D3DXMatrixPerspectiveOffCenterLH\D3DXMatrixPerspectiveOffCenterRH函数
	// @param[out]	kValue
	// @param[in]	fLeft\fRight\fBottom\fTop\fNear\fFar	投影空间的参数值
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

	// @brief 计算透视变换时观察空间到投影空间的变换矩阵(参考自WZ,经修正)
	// @param[in]	fFovY	垂直视角(单位:弧度)
	// @param[in]	fAspect	宽高比
	// @param[in]	fNear	到近裁剪面的距离(正值)
	// @param[in]	fFar	到远裁剪面的距离(正值)
	SD_STATIC sdMatrix4& PerspectiveFov(
		sdMatrix4& kValue,
		const float fFovY,
		const float fAspect,
		const float fNear,
		const float fFar);

	// @brief 计算正交变换时观察空间到投影空间的变换矩阵(参考自WZ,经修正)
	//			Ortho
	//				观察空间视锥体：[l,r][b,t][n,f]
	//				投影空间裁剪立方体：[-1,1][-1,1][-1,1]
	//				观察空间为右手坐标系版本：向右为X,向上为Y,向里为Z
	//				投影空间为右手坐标系版本：向右为X,向上为Y,向里为Z
	//			OrthoGL
	//				按照OpenGL的规则进行投影转换
	//				观察空间视锥体：[l,r][b,t][n,f]
	//				投影空间裁剪立方体：[-1,1][-1,1][-1,1]
	//				观察空间为右手坐标系版本：向右为X,向上为Y,向里为Z
	//				投影空间为左手坐标系版本：向右为X,向上为Y,向外为Z(左手规格化设备坐标系)
	//			OrthoD3DLH
	//				按照Direct3D的规则进行投影转换
	//				观察空间视锥体：[l,r][b,t][n,f]
	//				投影空间裁剪立方体：[-1,1][-1,1][0,1]
	//				观察空间为左手坐标系版本：向右为X,向上为Y,向外为Z
	//				投影空间为左手坐标系版本：向右为X,向上为Y,向外为Z(左手规格化设备坐标系)
	//			OrthoD3DRH
	//				按照Direct3D的规则进行投影转换
	//				观察空间视锥体：[l,r][b,t][n,f]
	//				投影空间裁剪立方体：[-1,1][-1,1][0,1]
	//				观察空间为左手坐标系版本：向右为X,向上为Y,向外为Z
	//				投影空间为右手坐标系版本：向右为X,向上为Y,向里为Z
	// @note  参考《实时计算机图形学》P34-39
	//		  参考OpenGL文档glOrtho函数
	//		  参考Direct3D文档D3DXMatrixOrthoLH\D3DXMatrixOrthoRH\
	//			D3DXMatrixOrthoOffCenterLH\D3DXMatrixOrthoOffCenterRH函数
	// @param[out]	kValue
	// @param[in]	fLeft\fRight\fBottom\fTop\fNear\fFar	投影空间的参数值
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
	static const float	POS_INFINITY;	///< float能表示的最大浮点数
	static const float	NEG_INFINITY;	///< float能表示的最小浮点数

	static const float	MATH_EPS;		///< 误差值(低于此误差算零)
	static const float	MATH_BIG_EPS;	///< 误差值
	static const float	MATH_SMALL_EPS;	///< 误差值

	static const float	E;				///< 自然对数的底
	static const float	PI;				///< 圆周率
	static const float	TWO_PI;
	static const float	HALF_PI;
	static const float	DEG2RAD;		///< 度数转弧度
	static const float	RAD2DEG;		///< 弧度转度数
};
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_MATH
BASE_NAMESPACE_END_BASE
#endif