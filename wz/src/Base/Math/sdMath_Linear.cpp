#include "sdBasePCH.h"
#include "sdMath.h"
#include "sdVector2.h"
#include "sdVector3.h"
#include "sdVector4.h"
#include "sdMatrix2.h"
#include "sdMatrix3.h"
#include "sdMatrix4.h"

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_MATH
//-------------------------------------------------------------------------------------------------
sdVector2& sdMath::Add(sdVector2& lkVector, const sdVector2& rkVector)
{
	lkVector.m_fX += rkVector.m_fX;
	lkVector.m_fY += rkVector.m_fY;

	return lkVector;
}
//-------------------------------------------------------------------------------------------------
sdVector3& sdMath::Add(sdVector3& lkVector, const sdVector3& rkVector)
{
	lkVector.m_fX += rkVector.m_fX;
	lkVector.m_fY += rkVector.m_fY;
	lkVector.m_fZ += rkVector.m_fZ;

	return lkVector;
}
//-------------------------------------------------------------------------------------------------
sdVector4& sdMath::Add(sdVector4& lkVector, const sdVector4& rkVector)
{
	lkVector.m_fX += rkVector.m_fX;
	lkVector.m_fY += rkVector.m_fY;
	lkVector.m_fZ += rkVector.m_fZ;
	lkVector.m_fW += rkVector.m_fW;

	return lkVector;
}
//-------------------------------------------------------------------------------------------------
sdMatrix2& sdMath::Add(sdMatrix2& lkMatrix, const sdMatrix2& rkMatrix)
{
	lkMatrix._m00 += rkMatrix._m00;
	lkMatrix._m01 += rkMatrix._m01;

	lkMatrix._m10 += rkMatrix._m10;
	lkMatrix._m11 += rkMatrix._m11;

	return lkMatrix;
}
//-------------------------------------------------------------------------------------------------
sdMatrix3& sdMath::Add(sdMatrix3& lkMatrix, const sdMatrix3& rkMatrix)
{
	lkMatrix._m00 += rkMatrix._m00;
	lkMatrix._m01 += rkMatrix._m01;
	lkMatrix._m02 += rkMatrix._m02;

	lkMatrix._m10 += rkMatrix._m10;
	lkMatrix._m11 += rkMatrix._m11;
	lkMatrix._m12 += rkMatrix._m12;

	lkMatrix._m20 += rkMatrix._m20;
	lkMatrix._m21 += rkMatrix._m21;
	lkMatrix._m22 += rkMatrix._m22;

	return lkMatrix;
}
//-------------------------------------------------------------------------------------------------
sdMatrix4& sdMath::Add(sdMatrix4& lkMatrix, const sdMatrix4& rkMatrix)
{
	lkMatrix._m00 += rkMatrix._m00;
	lkMatrix._m01 += rkMatrix._m01;
	lkMatrix._m02 += rkMatrix._m02;
	lkMatrix._m03 += rkMatrix._m03;

	lkMatrix._m10 += rkMatrix._m10;
	lkMatrix._m11 += rkMatrix._m11;
	lkMatrix._m12 += rkMatrix._m12;
	lkMatrix._m13 += rkMatrix._m13;

	lkMatrix._m20 += rkMatrix._m20;
	lkMatrix._m21 += rkMatrix._m21;
	lkMatrix._m22 += rkMatrix._m22;
	lkMatrix._m23 += rkMatrix._m23;

	lkMatrix._m30 += rkMatrix._m30;
	lkMatrix._m31 += rkMatrix._m31;
	lkMatrix._m32 += rkMatrix._m32;
	lkMatrix._m33 += rkMatrix._m33;

	return lkMatrix;
}
//-------------------------------------------------------------------------------------------------
sdVector2& sdMath::Add(sdVector2& kValue, const sdVector2& lkVector, const sdVector2& rkVector)
{
	kValue.m_fX = lkVector.m_fX + rkVector.m_fX;
	kValue.m_fY = lkVector.m_fY + rkVector.m_fY;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdVector3& sdMath::Add(sdVector3& kValue, const sdVector3& lkVector, const sdVector3& rkVector)
{
	kValue.m_fX = lkVector.m_fX + rkVector.m_fX;
	kValue.m_fY = lkVector.m_fY + rkVector.m_fY;
	kValue.m_fZ = lkVector.m_fZ + rkVector.m_fZ;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdVector4& sdMath::Add(sdVector4& kValue, const sdVector4& lkVector, const sdVector4& rkVector)
{
	kValue.m_fX = lkVector.m_fX + rkVector.m_fX;
	kValue.m_fY = lkVector.m_fY + rkVector.m_fY;
	kValue.m_fZ = lkVector.m_fZ + rkVector.m_fZ;
	kValue.m_fW = lkVector.m_fW + rkVector.m_fW;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdMatrix2& sdMath::Add(sdMatrix2& kValue, const sdMatrix2& lkMatrix, const sdMatrix2& rkMatrix)
{
	kValue._m00 = lkMatrix._m00 + rkMatrix._m00;
	kValue._m01 = lkMatrix._m01 + rkMatrix._m01;

	kValue._m10 = lkMatrix._m10 + rkMatrix._m10;
	kValue._m11 = lkMatrix._m11 + rkMatrix._m11;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdMatrix3& sdMath::Add(sdMatrix3& kValue, const sdMatrix3& lkMatrix, const sdMatrix3& rkMatrix)
{
	kValue._m00 = lkMatrix._m00 + rkMatrix._m00;
	kValue._m01 = lkMatrix._m01 + rkMatrix._m01;
	kValue._m02 = lkMatrix._m02 + rkMatrix._m02;

	kValue._m10 = lkMatrix._m10 + rkMatrix._m10;
	kValue._m11 = lkMatrix._m11 + rkMatrix._m11;
	kValue._m12 = lkMatrix._m12 + rkMatrix._m12;

	kValue._m20 = lkMatrix._m20 + rkMatrix._m20;
	kValue._m21 = lkMatrix._m21 + rkMatrix._m21;
	kValue._m22 = lkMatrix._m22 + rkMatrix._m22;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdMatrix4& sdMath::Add(sdMatrix4& kValue, const sdMatrix4& lkMatrix, const sdMatrix4& rkMatrix)
{
	kValue._m00 = lkMatrix._m00 + rkMatrix._m00;
	kValue._m01 = lkMatrix._m01 + rkMatrix._m01;
	kValue._m02 = lkMatrix._m02 + rkMatrix._m02;
	kValue._m03 = lkMatrix._m03 + rkMatrix._m03;

	kValue._m10 = lkMatrix._m10 + rkMatrix._m10;
	kValue._m11 = lkMatrix._m11 + rkMatrix._m11;
	kValue._m12 = lkMatrix._m12 + rkMatrix._m12;
	kValue._m13 = lkMatrix._m13 + rkMatrix._m13;

	kValue._m20 = lkMatrix._m20 + rkMatrix._m20;
	kValue._m21 = lkMatrix._m21 + rkMatrix._m21;
	kValue._m22 = lkMatrix._m22 + rkMatrix._m22;
	kValue._m23 = lkMatrix._m23 + rkMatrix._m23;

	kValue._m30 = lkMatrix._m30 + rkMatrix._m30;
	kValue._m31 = lkMatrix._m31 + rkMatrix._m31;
	kValue._m32 = lkMatrix._m32 + rkMatrix._m32;
	kValue._m33 = lkMatrix._m33 + rkMatrix._m33;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdVector2& sdMath::Sub(sdVector2& lkVector, const sdVector2& rkVector)
{
	lkVector.m_fX -= rkVector.m_fX;
	lkVector.m_fY -= rkVector.m_fY;

	return lkVector;
}
//-------------------------------------------------------------------------------------------------
sdVector3& sdMath::Sub(sdVector3& lkVector, const sdVector3& rkVector)
{
	lkVector.m_fX -= rkVector.m_fX;
	lkVector.m_fY -= rkVector.m_fY;
	lkVector.m_fZ -= rkVector.m_fZ;

	return lkVector;
}
//-------------------------------------------------------------------------------------------------
sdVector4& sdMath::Sub(sdVector4& lkVector, const sdVector4& rkVector)
{
	lkVector.m_fX -= rkVector.m_fX;
	lkVector.m_fY -= rkVector.m_fY;
	lkVector.m_fZ -= rkVector.m_fZ;
	lkVector.m_fW -= rkVector.m_fW;

	return lkVector;
}
//-------------------------------------------------------------------------------------------------
sdMatrix2& sdMath::Sub(sdMatrix2& lkMatrix, const sdMatrix2& rkMatrix)
{
	lkMatrix._m00 -= rkMatrix._m00;
	lkMatrix._m01 -= rkMatrix._m01;

	lkMatrix._m10 -= rkMatrix._m10;
	lkMatrix._m11 -= rkMatrix._m11;

	return lkMatrix;
}
//-------------------------------------------------------------------------------------------------
sdMatrix3& sdMath::Sub(sdMatrix3& lkMatrix, const sdMatrix3& rkMatrix)
{
	lkMatrix._m00 -= rkMatrix._m00;
	lkMatrix._m01 -= rkMatrix._m01;
	lkMatrix._m02 -= rkMatrix._m02;

	lkMatrix._m10 -= rkMatrix._m10;
	lkMatrix._m11 -= rkMatrix._m11;
	lkMatrix._m12 -= rkMatrix._m12;

	lkMatrix._m20 -= rkMatrix._m20;
	lkMatrix._m21 -= rkMatrix._m21;
	lkMatrix._m22 -= rkMatrix._m22;

	return lkMatrix;
}
//-------------------------------------------------------------------------------------------------
sdMatrix4& sdMath::Sub(sdMatrix4& lkMatrix, const sdMatrix4& rkMatrix)
{
	lkMatrix._m00 -= rkMatrix._m00;
	lkMatrix._m01 -= rkMatrix._m01;
	lkMatrix._m02 -= rkMatrix._m02;
	lkMatrix._m03 -= rkMatrix._m03;

	lkMatrix._m10 -= rkMatrix._m10;
	lkMatrix._m11 -= rkMatrix._m11;
	lkMatrix._m12 -= rkMatrix._m12;
	lkMatrix._m13 -= rkMatrix._m13;

	lkMatrix._m20 -= rkMatrix._m20;
	lkMatrix._m21 -= rkMatrix._m21;
	lkMatrix._m22 -= rkMatrix._m22;
	lkMatrix._m23 -= rkMatrix._m23;

	lkMatrix._m30 -= rkMatrix._m30;
	lkMatrix._m31 -= rkMatrix._m31;
	lkMatrix._m32 -= rkMatrix._m32;
	lkMatrix._m33 -= rkMatrix._m33;

	return lkMatrix;
}
//-------------------------------------------------------------------------------------------------
sdVector2& sdMath::Sub(sdVector2& kValue, const sdVector2& lkVector, const sdVector2& rkVector)
{
	kValue.m_fX = lkVector.m_fX - rkVector.m_fX;
	kValue.m_fY = lkVector.m_fY - rkVector.m_fY;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdVector3& sdMath::Sub(sdVector3& kValue, const sdVector3& lkVector, const sdVector3& rkVector)
{
	kValue.m_fX = lkVector.m_fX - rkVector.m_fX;
	kValue.m_fY = lkVector.m_fY - rkVector.m_fY;
	kValue.m_fZ = lkVector.m_fZ - rkVector.m_fZ;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdVector4& sdMath::Sub(sdVector4& kValue, const sdVector4& lkVector, const sdVector4& rkVector)
{
	kValue.m_fX = lkVector.m_fX - rkVector.m_fX;
	kValue.m_fY = lkVector.m_fY - rkVector.m_fY;
	kValue.m_fZ = lkVector.m_fZ - rkVector.m_fZ;
	kValue.m_fW = lkVector.m_fW - rkVector.m_fW;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdMatrix2& sdMath::Sub(sdMatrix2& kValue, const sdMatrix2& lkMatrix, const sdMatrix2& rkMatrix)
{
	kValue._m00 = lkMatrix._m00 - rkMatrix._m00;
	kValue._m01 = lkMatrix._m01 - rkMatrix._m01;

	kValue._m10 = lkMatrix._m10 - rkMatrix._m10;
	kValue._m11 = lkMatrix._m11 - rkMatrix._m11;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdMatrix3& sdMath::Sub(sdMatrix3& kValue, const sdMatrix3& lkMatrix, const sdMatrix3& rkMatrix)
{
	kValue._m00 = lkMatrix._m00 - rkMatrix._m00;
	kValue._m01 = lkMatrix._m01 - rkMatrix._m01;
	kValue._m02 = lkMatrix._m02 - rkMatrix._m02;

	kValue._m10 = lkMatrix._m10 - rkMatrix._m10;
	kValue._m11 = lkMatrix._m11 - rkMatrix._m11;
	kValue._m12 = lkMatrix._m12 - rkMatrix._m12;

	kValue._m20 = lkMatrix._m20 - rkMatrix._m20;
	kValue._m21 = lkMatrix._m21 - rkMatrix._m21;
	kValue._m22 = lkMatrix._m22 - rkMatrix._m22;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdMatrix4& sdMath::Sub(sdMatrix4& kValue, const sdMatrix4& lkMatrix, const sdMatrix4& rkMatrix)
{
	kValue._m00 = lkMatrix._m00 - rkMatrix._m00;
	kValue._m01 = lkMatrix._m01 - rkMatrix._m01;
	kValue._m02 = lkMatrix._m02 - rkMatrix._m02;
	kValue._m03 = lkMatrix._m03 - rkMatrix._m03;

	kValue._m10 = lkMatrix._m10 - rkMatrix._m10;
	kValue._m11 = lkMatrix._m11 - rkMatrix._m11;
	kValue._m12 = lkMatrix._m12 - rkMatrix._m12;
	kValue._m13 = lkMatrix._m13 - rkMatrix._m13;

	kValue._m20 = lkMatrix._m20 - rkMatrix._m20;
	kValue._m21 = lkMatrix._m21 - rkMatrix._m21;
	kValue._m22 = lkMatrix._m22 - rkMatrix._m22;
	kValue._m23 = lkMatrix._m23 - rkMatrix._m23;

	kValue._m30 = lkMatrix._m30 - rkMatrix._m30;
	kValue._m31 = lkMatrix._m31 - rkMatrix._m31;
	kValue._m32 = lkMatrix._m32 - rkMatrix._m32;
	kValue._m33 = lkMatrix._m33 - rkMatrix._m33;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdVector2& sdMath::Mult(sdVector2& lkVector, const float fScalar)
{
	lkVector.m_fX *= fScalar;
	lkVector.m_fY *= fScalar;

	return lkVector;
}
//-------------------------------------------------------------------------------------------------
sdVector3& sdMath::Mult(sdVector3& lkVector, const float fScalar)
{
	lkVector.m_fX *= fScalar;
	lkVector.m_fY *= fScalar;
	lkVector.m_fZ *= fScalar;

	return lkVector;
}
//-------------------------------------------------------------------------------------------------
sdVector4& sdMath::Mult(sdVector4& lkVector, const float fScalar)
{
	lkVector.m_fX *= fScalar;
	lkVector.m_fY *= fScalar;
	lkVector.m_fZ *= fScalar;
	lkVector.m_fW *= fScalar;

	return lkVector;
}
//-------------------------------------------------------------------------------------------------
sdMatrix2& sdMath::Mult(sdMatrix2& lkMatrix, const float fScalar)
{
	lkMatrix._m00 *= fScalar;
	lkMatrix._m01 *= fScalar;

	lkMatrix._m10 *= fScalar;
	lkMatrix._m11 *= fScalar;

	return lkMatrix;
}
//-------------------------------------------------------------------------------------------------
sdMatrix3& sdMath::Mult(sdMatrix3& lkMatrix, const float fScalar)
{
	lkMatrix._m00 *= fScalar;
	lkMatrix._m01 *= fScalar;
	lkMatrix._m02 *= fScalar;

	lkMatrix._m10 *= fScalar;
	lkMatrix._m11 *= fScalar;
	lkMatrix._m12 *= fScalar;

	lkMatrix._m20 *= fScalar;
	lkMatrix._m21 *= fScalar;
	lkMatrix._m22 *= fScalar;

	return lkMatrix;
}
//-------------------------------------------------------------------------------------------------
sdMatrix4& sdMath::Mult(sdMatrix4& lkMatrix, const float fScalar)
{
	lkMatrix._m00 *= fScalar;
	lkMatrix._m01 *= fScalar;
	lkMatrix._m02 *= fScalar;
	lkMatrix._m03 *= fScalar;

	lkMatrix._m10 *= fScalar;
	lkMatrix._m11 *= fScalar;
	lkMatrix._m12 *= fScalar;
	lkMatrix._m13 *= fScalar;

	lkMatrix._m20 *= fScalar;
	lkMatrix._m21 *= fScalar;
	lkMatrix._m22 *= fScalar;
	lkMatrix._m23 *= fScalar;

	lkMatrix._m30 *= fScalar;
	lkMatrix._m31 *= fScalar;
	lkMatrix._m32 *= fScalar;
	lkMatrix._m33 *= fScalar;

	return lkMatrix;
}
//-------------------------------------------------------------------------------------------------
sdVector2& sdMath::Mult(sdVector2& kValue, const sdVector2& lkVector, const float fScalar)
{
	kValue.m_fX = lkVector.m_fX * fScalar;
	kValue.m_fY = lkVector.m_fY * fScalar;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdVector3& sdMath::Mult(sdVector3& kValue, const sdVector3& lkVector, const float fScalar)
{
	kValue.m_fX = lkVector.m_fX * fScalar;
	kValue.m_fY = lkVector.m_fY * fScalar;
	kValue.m_fZ = lkVector.m_fZ * fScalar;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdVector4& sdMath::Mult(sdVector4& kValue, const sdVector4& lkVector, const float fScalar)
{
	kValue.m_fX = lkVector.m_fX * fScalar;
	kValue.m_fY = lkVector.m_fY * fScalar;
	kValue.m_fZ = lkVector.m_fZ * fScalar;
	kValue.m_fW = lkVector.m_fW * fScalar;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdMatrix2& sdMath::Mult(sdMatrix2& kValue, const sdMatrix2& lkMatrix, const float fScalar)
{
	kValue._m00 = lkMatrix._m00 * fScalar;
	kValue._m01 = lkMatrix._m01 * fScalar;

	kValue._m10 = lkMatrix._m10 * fScalar;
	kValue._m11 = lkMatrix._m11 * fScalar;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdMatrix3& sdMath::Mult(sdMatrix3& kValue, const sdMatrix3& lkMatrix, const float fScalar)
{
	kValue._m00 = lkMatrix._m00 * fScalar;
	kValue._m01 = lkMatrix._m01 * fScalar;
	kValue._m02 = lkMatrix._m02 * fScalar;

	kValue._m10 = lkMatrix._m10 * fScalar;
	kValue._m11 = lkMatrix._m11 * fScalar;
	kValue._m12 = lkMatrix._m12 * fScalar;

	kValue._m20 = lkMatrix._m20 * fScalar;
	kValue._m21 = lkMatrix._m21 * fScalar;
	kValue._m22 = lkMatrix._m22 * fScalar;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdMatrix4& sdMath::Mult(sdMatrix4& kValue, const sdMatrix4& lkMatrix, const float fScalar)
{
	kValue._m00 = lkMatrix._m00 * fScalar;
	kValue._m01 = lkMatrix._m01 * fScalar;
	kValue._m02 = lkMatrix._m02 * fScalar;
	kValue._m03 = lkMatrix._m03 * fScalar;

	kValue._m10 = lkMatrix._m10 * fScalar;
	kValue._m11 = lkMatrix._m11 * fScalar;
	kValue._m12 = lkMatrix._m12 * fScalar;
	kValue._m13 = lkMatrix._m13 * fScalar;

	kValue._m20 = lkMatrix._m20 * fScalar;
	kValue._m21 = lkMatrix._m21 * fScalar;
	kValue._m22 = lkMatrix._m22 * fScalar;
	kValue._m23 = lkMatrix._m23 * fScalar;

	kValue._m30 = lkMatrix._m20 * fScalar;
	kValue._m31 = lkMatrix._m21 * fScalar;
	kValue._m32 = lkMatrix._m22 * fScalar;
	kValue._m33 = lkMatrix._m23 * fScalar;


	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdVector3& sdMath::Mult(sdVector3& kValue, const sdVector3& lkVector, const sdVector3& rkVector)
{
	kValue.m_fX = lkVector.m_fX * rkVector.m_fX;
	kValue.m_fY = lkVector.m_fY * rkVector.m_fY;
	kValue.m_fZ = lkVector.m_fZ * rkVector.m_fZ;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdVector3& sdMath::Mult(sdVector3& kValue, const sdVector3& lkVector, const sdMatrix3& rkMatrix)
{
	kValue.m_fX = lkVector.m_fX * rkMatrix._m00 + lkVector.m_fY * rkMatrix._m10 + lkVector.m_fZ * rkMatrix._m20;
	kValue.m_fY = lkVector.m_fX * rkMatrix._m01 + lkVector.m_fY * rkMatrix._m11 + lkVector.m_fZ * rkMatrix._m21;
	kValue.m_fZ = lkVector.m_fX * rkMatrix._m02 + lkVector.m_fY * rkMatrix._m12 + lkVector.m_fZ * rkMatrix._m22;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdVector3& sdMath::Mult(sdVector3& kValue, const sdMatrix3& lkMatrix, const sdVector3& rkVector)
{
	kValue.m_fX = lkMatrix._m00 * rkVector.m_fX + lkMatrix._m01 * rkVector.m_fY + lkMatrix._m02 * rkVector.m_fZ;
	kValue.m_fY = lkMatrix._m10 * rkVector.m_fX + lkMatrix._m11 * rkVector.m_fY + lkMatrix._m12 * rkVector.m_fZ;
	kValue.m_fZ = lkMatrix._m20 * rkVector.m_fX + lkMatrix._m21 * rkVector.m_fY + lkMatrix._m22 * rkVector.m_fZ;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdVector4& sdMath::Mult(sdVector4& kValue, const sdVector4& lkVector, const sdMatrix4& rkMatrix)
{
	kValue.m_fX = lkVector.m_fX * rkMatrix._m00 + lkVector.m_fY * rkMatrix._m10 + lkVector.m_fZ * rkMatrix._m20 + lkVector.m_fW * rkMatrix._m30;
	kValue.m_fY = lkVector.m_fX * rkMatrix._m01 + lkVector.m_fY * rkMatrix._m11 + lkVector.m_fZ * rkMatrix._m21 + lkVector.m_fW * rkMatrix._m31;
	kValue.m_fZ = lkVector.m_fX * rkMatrix._m02 + lkVector.m_fY * rkMatrix._m12 + lkVector.m_fZ * rkMatrix._m22 + lkVector.m_fW * rkMatrix._m32;
	kValue.m_fW = lkVector.m_fX * rkMatrix._m03 + lkVector.m_fY * rkMatrix._m13 + lkVector.m_fZ * rkMatrix._m23 + lkVector.m_fW * rkMatrix._m33;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdVector4& sdMath::Mult(sdVector4& kValue, const sdMatrix4& lkMatrix, const sdVector4& rkVector)
{
	kValue.m_fX = lkMatrix._m00 * rkVector.m_fX + lkMatrix._m01 * rkVector.m_fY + lkMatrix._m02 * rkVector.m_fZ + lkMatrix._m03 * rkVector.m_fW;
	kValue.m_fY = lkMatrix._m10 * rkVector.m_fX + lkMatrix._m11 * rkVector.m_fY + lkMatrix._m12 * rkVector.m_fZ + lkMatrix._m13 * rkVector.m_fW;
	kValue.m_fZ = lkMatrix._m20 * rkVector.m_fX + lkMatrix._m21 * rkVector.m_fY + lkMatrix._m22 * rkVector.m_fZ + lkMatrix._m23 * rkVector.m_fW;
	kValue.m_fW = lkMatrix._m30 * rkVector.m_fX + lkMatrix._m31 * rkVector.m_fY + lkMatrix._m32 * rkVector.m_fZ + lkMatrix._m33 * rkVector.m_fW;
	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdMatrix3& sdMath::Mult(sdMatrix3& kValue, const sdMatrix3& lkMatrix, const sdMatrix3& rkMatrix)
{
	kValue._m00 = lkMatrix._m00 * rkMatrix._m00 + lkMatrix._m01 * rkMatrix._m10 + lkMatrix._m02 * rkMatrix._m20;
	kValue._m01 = lkMatrix._m00 * rkMatrix._m01 + lkMatrix._m01 * rkMatrix._m11 + lkMatrix._m02 * rkMatrix._m21;
	kValue._m02 = lkMatrix._m00 * rkMatrix._m02 + lkMatrix._m01 * rkMatrix._m12 + lkMatrix._m02 * rkMatrix._m22;

	kValue._m10 = lkMatrix._m10 * rkMatrix._m00 + lkMatrix._m11 * rkMatrix._m10 + lkMatrix._m12 * rkMatrix._m20;
	kValue._m11 = lkMatrix._m10 * rkMatrix._m01 + lkMatrix._m11 * rkMatrix._m11 + lkMatrix._m12 * rkMatrix._m21;
	kValue._m12 = lkMatrix._m10 * rkMatrix._m02 + lkMatrix._m11 * rkMatrix._m12 + lkMatrix._m12 * rkMatrix._m22;

	kValue._m20 = lkMatrix._m20 * rkMatrix._m00 + lkMatrix._m21 * rkMatrix._m10 + lkMatrix._m22 * rkMatrix._m20;
	kValue._m21 = lkMatrix._m20 * rkMatrix._m01 + lkMatrix._m21 * rkMatrix._m11 + lkMatrix._m22 * rkMatrix._m21;
	kValue._m22 = lkMatrix._m20 * rkMatrix._m02 + lkMatrix._m21 * rkMatrix._m12 + lkMatrix._m22 * rkMatrix._m22;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdMatrix4& sdMath::Mult(sdMatrix4& kValue, const sdMatrix4& lkMatrix, const sdMatrix4& rkMatrix)
{
	kValue._m00 = lkMatrix._m00 * rkMatrix._m00 + lkMatrix._m01 * rkMatrix._m10 + lkMatrix._m02 * rkMatrix._m20 + lkMatrix._m03 * rkMatrix._m30;
	kValue._m01 = lkMatrix._m00 * rkMatrix._m01 + lkMatrix._m01 * rkMatrix._m11 + lkMatrix._m02 * rkMatrix._m21 + lkMatrix._m03 * rkMatrix._m31;
	kValue._m02 = lkMatrix._m00 * rkMatrix._m02 + lkMatrix._m01 * rkMatrix._m12 + lkMatrix._m02 * rkMatrix._m22 + lkMatrix._m03 * rkMatrix._m32;
	kValue._m03 = lkMatrix._m00 * rkMatrix._m03 + lkMatrix._m01 * rkMatrix._m13 + lkMatrix._m02 * rkMatrix._m23 + lkMatrix._m03 * rkMatrix._m33;

	kValue._m10 = lkMatrix._m10 * rkMatrix._m00 + lkMatrix._m11 * rkMatrix._m10 + lkMatrix._m12 * rkMatrix._m20 + lkMatrix._m13 * rkMatrix._m30;
	kValue._m11 = lkMatrix._m10 * rkMatrix._m01 + lkMatrix._m11 * rkMatrix._m11 + lkMatrix._m12 * rkMatrix._m21 + lkMatrix._m13 * rkMatrix._m31;
	kValue._m12 = lkMatrix._m10 * rkMatrix._m02 + lkMatrix._m11 * rkMatrix._m12 + lkMatrix._m12 * rkMatrix._m22 + lkMatrix._m13 * rkMatrix._m32;
	kValue._m13 = lkMatrix._m10 * rkMatrix._m03 + lkMatrix._m11 * rkMatrix._m13 + lkMatrix._m12 * rkMatrix._m23 + lkMatrix._m13 * rkMatrix._m33;

	kValue._m20 = lkMatrix._m20 * rkMatrix._m00 + lkMatrix._m21 * rkMatrix._m10 + lkMatrix._m22 * rkMatrix._m20 + lkMatrix._m23 * rkMatrix._m30;
	kValue._m21 = lkMatrix._m20 * rkMatrix._m01 + lkMatrix._m21 * rkMatrix._m11 + lkMatrix._m22 * rkMatrix._m21 + lkMatrix._m23 * rkMatrix._m31;
	kValue._m22 = lkMatrix._m20 * rkMatrix._m02 + lkMatrix._m21 * rkMatrix._m12 + lkMatrix._m22 * rkMatrix._m22 + lkMatrix._m23 * rkMatrix._m32;
	kValue._m23 = lkMatrix._m20 * rkMatrix._m03 + lkMatrix._m21 * rkMatrix._m13 + lkMatrix._m22 * rkMatrix._m23 + lkMatrix._m23 * rkMatrix._m33;

	kValue._m30 = lkMatrix._m30 * rkMatrix._m00 + lkMatrix._m31 * rkMatrix._m10 + lkMatrix._m32 * rkMatrix._m20 + lkMatrix._m33 * rkMatrix._m30;
	kValue._m31 = lkMatrix._m30 * rkMatrix._m01 + lkMatrix._m31 * rkMatrix._m11 + lkMatrix._m32 * rkMatrix._m21 + lkMatrix._m33 * rkMatrix._m31;
	kValue._m32 = lkMatrix._m30 * rkMatrix._m02 + lkMatrix._m31 * rkMatrix._m12 + lkMatrix._m32 * rkMatrix._m22 + lkMatrix._m33 * rkMatrix._m32;
	kValue._m33 = lkMatrix._m30 * rkMatrix._m03 + lkMatrix._m31 * rkMatrix._m13 + lkMatrix._m32 * rkMatrix._m23 + lkMatrix._m33 * rkMatrix._m33;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdVector3& sdMath::Madd(sdVector3& kValue, const sdVector3& lkVector, const float fScalar)
{
	kValue.m_fX += lkVector.m_fX * fScalar;
	kValue.m_fY += lkVector.m_fY * fScalar;
	kValue.m_fZ += lkVector.m_fZ * fScalar;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
float sdMath::Dot(const sdVector3& lkVector, const sdVector3& rkVector)
{
	return lkVector.m_fX * rkVector.m_fX + lkVector.m_fY * rkVector.m_fY + lkVector.m_fZ * rkVector.m_fZ;
}
//-------------------------------------------------------------------------------------------------
float sdMath::Dot(const sdVector4& lkVector, const sdVector4& rkVector)
{
	return lkVector.m_fX * rkVector.m_fX + lkVector.m_fY * rkVector.m_fY + lkVector.m_fZ * rkVector.m_fZ + lkVector.m_fW * rkVector.m_fW;
}
//-------------------------------------------------------------------------------------------------
sdVector3& sdMath::Cross(sdVector3& kValue, const sdVector3& lkVector, const sdVector3& rkVector)
{
	kValue.m_fX = lkVector.m_fY * rkVector.m_fZ - lkVector.m_fZ * rkVector.m_fY;
	kValue.m_fY = lkVector.m_fZ * rkVector.m_fX - lkVector.m_fX * rkVector.m_fZ;
	kValue.m_fZ = lkVector.m_fX * rkVector.m_fY - lkVector.m_fY * rkVector.m_fX;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdVector3& sdMath::Reflect(sdVector3& kValue, const sdVector3& kLight, const sdVector3& kNormal)
{
//	float fNL = 2.f * kNormal.DotProduct(kLight);
//	kValue = kNormal * fNL - kLight; 

	float fNL = Dot(kNormal, kLight);
	Negative(kValue, kLight);
	Mult(kValue, kNormal, fNL * 2.f);

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdVector2& sdMath::Assignment(sdVector2& kValue, const sdVector2& kVector)
{
	kValue.m_fX = kVector.m_fX;
	kValue.m_fY = kVector.m_fY;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdVector3& sdMath::Assignment(sdVector3& kValue, const sdVector3& kVector)
{
	kValue.m_fX = kVector.m_fX;
	kValue.m_fY = kVector.m_fY;
	kValue.m_fZ = kVector.m_fZ;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdVector4& sdMath::Assignment(sdVector4& kValue, const sdVector4& kVector)
{
	kValue.m_fX = kVector.m_fX;
	kValue.m_fY = kVector.m_fY;
	kValue.m_fZ = kVector.m_fZ;
	kValue.m_fW = kVector.m_fW;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdVector2& sdMath::Negative(sdVector2& kVector)
{
	kVector.m_fX = -kVector.m_fX;
	kVector.m_fY = -kVector.m_fY;

	return kVector;
}
//-------------------------------------------------------------------------------------------------
sdVector3& sdMath::Negative(sdVector3& kVector)
{
	kVector.m_fX = -kVector.m_fX;
	kVector.m_fY = -kVector.m_fY;
	kVector.m_fZ = -kVector.m_fZ;

	return kVector;
}
//-------------------------------------------------------------------------------------------------
sdVector4& sdMath::Negative(sdVector4& kVector)
{
	kVector.m_fX = -kVector.m_fX;
	kVector.m_fY = -kVector.m_fY;
	kVector.m_fZ = -kVector.m_fZ;
	kVector.m_fW = -kVector.m_fW;

	return kVector;
}
//-------------------------------------------------------------------------------------------------
sdVector2& sdMath::Negative(sdVector2& kValue, const sdVector2& kVector)
{
	kValue.m_fX = -kVector.m_fX;
	kValue.m_fY = -kVector.m_fY;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdVector3& sdMath::Negative(sdVector3& kValue, const sdVector3& kVector)
{
	kValue.m_fX = -kVector.m_fX;
	kValue.m_fY = -kVector.m_fY;
	kValue.m_fZ = -kVector.m_fZ;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdVector4& sdMath::Negative(sdVector4& kValue, const sdVector4& kVector)
{
	kValue.m_fX = -kVector.m_fX;
	kValue.m_fY = -kVector.m_fY;
	kValue.m_fZ = -kVector.m_fZ;
	kValue.m_fW = -kVector.m_fW;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
float sdMath::Normalize(sdVector2& kVector)
{
	float fInvLength = 0.f;
	float fLength = Sqrt(kVector.m_fX * kVector.m_fX + kVector.m_fY * kVector.m_fY);
	if (fLength > MATH_EPS)
		fInvLength = 1.0f / fLength;

	kVector.m_fX *= fInvLength;
	kVector.m_fY *= fInvLength;

	return fLength;
}
//-------------------------------------------------------------------------------------------------
float sdMath::Normalize(sdVector3& kVector)
{
	float fInvLength = 0.f;
	float fLength = Sqrt(kVector.m_fX * kVector.m_fX + kVector.m_fY * kVector.m_fY + kVector.m_fZ * kVector.m_fZ);
	if (fLength > MATH_EPS)
		fInvLength = 1.0f / fLength;

	kVector.m_fX *= fInvLength;
	kVector.m_fY *= fInvLength;
	kVector.m_fZ *= fInvLength;

	return fLength;
}
//-------------------------------------------------------------------------------------------------
float sdMath::Normalize(sdVector4& kVector)
{
	float fInvLength = 0.f;
	float fLength = Sqrt(kVector.m_fX * kVector.m_fX + kVector.m_fY * kVector.m_fY + kVector.m_fZ * kVector.m_fZ + kVector.m_fW * kVector.m_fW);
	if (fLength > MATH_EPS)
		fInvLength = 1.0f / fLength;

	kVector.m_fX *= fInvLength;
	kVector.m_fY *= fInvLength;
	kVector.m_fZ *= fInvLength;
	kVector.m_fW *= fInvLength;

	return fLength;
}
//-------------------------------------------------------------------------------------------------
float sdMath::Normalize(sdQuaternion& kQuaternion)
{
	float fInvLength = 0.f;
	float fLength = Sqrt(kQuaternion.m_fX * kQuaternion.m_fX + kQuaternion.m_fY * kQuaternion.m_fY + kQuaternion.m_fZ * kQuaternion.m_fZ + kQuaternion.m_fW * kQuaternion.m_fW);
	if (fLength > MATH_EPS)
		fInvLength = 1.0f / fLength;

	kQuaternion.m_fX *= fInvLength;
	kQuaternion.m_fY *= fInvLength;
	kQuaternion.m_fZ *= fInvLength;
	kQuaternion.m_fW *= fInvLength;

	return fLength;
}
//-------------------------------------------------------------------------------------------------
float sdMath::Determinet(sdMatrix3& kMatrix)
{
	return Det(kMatrix._m00, kMatrix._m01, kMatrix._m02,
			   kMatrix._m10, kMatrix._m11, kMatrix._m12,
			   kMatrix._m20, kMatrix._m21, kMatrix._m22);
}
//-------------------------------------------------------------------------------------------------
sdMatrix3& sdMath::Transpose(sdMatrix3& kMatrix)
{
	float fTmp = 0.f;

	fTmp = kMatrix._m01;
	kMatrix._m01 = kMatrix._m10;
	kMatrix._m10 = fTmp;

	fTmp = kMatrix._m02;
	kMatrix._m02 = kMatrix._m20;
	kMatrix._m20 = fTmp;

	fTmp = kMatrix._m12;
	kMatrix._m12 = kMatrix._m21;
	kMatrix._m21 = fTmp;

	return kMatrix;
}
//-------------------------------------------------------------------------------------------------
sdMatrix3& sdMath::Transpose(sdMatrix3& kValue, sdMatrix3& kMatrix)
{
	kValue._m00 = kMatrix._m00;
	kValue._m01 = kMatrix._m10;
	kValue._m02 = kMatrix._m20;

	kValue._m10 = kMatrix._m01;
	kValue._m11 = kMatrix._m11;
	kValue._m12 = kMatrix._m21;

	kValue._m20 = kMatrix._m02;
	kValue._m21 = kMatrix._m12;
	kValue._m22 = kMatrix._m22;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdMatrix4& sdMath::Transpose(sdMatrix4& kMatrix)
{
	float fTmp = 0.f;

	fTmp = kMatrix._m01;
	kMatrix._m01 = kMatrix._m10;
	kMatrix._m10 = fTmp;

	fTmp = kMatrix._m02;
	kMatrix._m02 = kMatrix._m20;
	kMatrix._m20 = fTmp;

	fTmp = kMatrix._m03;
	kMatrix._m03 = kMatrix._m30;
	kMatrix._m30 = fTmp;

	fTmp = kMatrix._m12;
	kMatrix._m12 = kMatrix._m21;
	kMatrix._m21 = fTmp;

	fTmp = kMatrix._m13;
	kMatrix._m13 = kMatrix._m31;
	kMatrix._m31 = fTmp;

	fTmp = kMatrix._m23;
	kMatrix._m23 = kMatrix._m32;
	kMatrix._m32 = fTmp;

	return kMatrix;
}
//-------------------------------------------------------------------------------------------------
sdMatrix4& sdMath::Transpose(sdMatrix4& kValue, const sdMatrix4& kMatrix)
{
	kValue._m00 = kMatrix._m00;
	kValue._m01 = kMatrix._m10;
	kValue._m02 = kMatrix._m20;
	kValue._m03 = kMatrix._m30;

	kValue._m10 = kMatrix._m01;
	kValue._m11 = kMatrix._m11;
	kValue._m12 = kMatrix._m21;
	kValue._m13 = kMatrix._m31;

	kValue._m20 = kMatrix._m02;
	kValue._m21 = kMatrix._m12;
	kValue._m22 = kMatrix._m22;
	kValue._m23 = kMatrix._m32;

	kValue._m30 = kMatrix._m03;
	kValue._m31 = kMatrix._m13;
	kValue._m32 = kMatrix._m23;
	kValue._m33 = kMatrix._m33;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdMatrix3& sdMath::Invert(sdMatrix3& kValue, const sdMatrix3& kMatrix)
{
	//	_m00, _m01, _m02,
	//	_m10, _m11, _m12,
	//	_m20, _m21, _m22,

	kValue._m00 = Det(kMatrix._m11, kMatrix._m12, kMatrix._m21, kMatrix._m22);
	kValue._m10 =-Det(kMatrix._m10, kMatrix._m12, kMatrix._m20, kMatrix._m22);
	kValue._m20 = Det(kMatrix._m10, kMatrix._m11, kMatrix._m20, kMatrix._m21);

	kValue._m01 =-Det(kMatrix._m01, kMatrix._m02, kMatrix._m21, kMatrix._m22);
	kValue._m11 = Det(kMatrix._m00, kMatrix._m02, kMatrix._m20, kMatrix._m22);
	kValue._m21 =-Det(kMatrix._m00, kMatrix._m01, kMatrix._m20, kMatrix._m21);

	kValue._m02 = Det(kMatrix._m01, kMatrix._m02, kMatrix._m11, kMatrix._m12);
	kValue._m12 =-Det(kMatrix._m00, kMatrix._m02, kMatrix._m10, kMatrix._m12);
	kValue._m22 = Det(kMatrix._m00, kMatrix._m01, kMatrix._m10, kMatrix._m11);

	float fDet = (kMatrix._m00 * kValue._m00) +  (kMatrix._m01 * kValue._m01) +  (kMatrix._m02 * kValue._m02);

	// the following divions goes unchecked for division by zero, we should 
	// consider throwing an exception if det < eps
	float fOODet = 1.f / fDet;	

	//
	Mult(kValue, fOODet);

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdMatrix4& sdMath::Invert(sdMatrix4& kValue, const sdMatrix4& kMatrix)
{
	//	_m00, _m01, _m02, _m03,
	//	_m10, _m11, _m12, _m13,
	//	_m20, _m21, _m22, _m23,
	//	_m30, _m31, _m32, _m33,

	kValue._m00 = Det(kMatrix._m11, kMatrix._m21, kMatrix._m31, kMatrix._m12, kMatrix._m22, kMatrix._m32, kMatrix._m13, kMatrix._m23, kMatrix._m33);
	kValue._m10 =-Det(kMatrix._m10, kMatrix._m20, kMatrix._m30, kMatrix._m12, kMatrix._m22, kMatrix._m32, kMatrix._m13, kMatrix._m23, kMatrix._m33);
	kValue._m20 = Det(kMatrix._m10, kMatrix._m20, kMatrix._m30, kMatrix._m11, kMatrix._m21, kMatrix._m31, kMatrix._m13, kMatrix._m23, kMatrix._m33);
	kValue._m30 =-Det(kMatrix._m10, kMatrix._m20, kMatrix._m30, kMatrix._m11, kMatrix._m21, kMatrix._m31, kMatrix._m12, kMatrix._m22, kMatrix._m32);
	
	kValue._m01 =-Det(kMatrix._m01, kMatrix._m21, kMatrix._m31, kMatrix._m02, kMatrix._m22, kMatrix._m32, kMatrix._m03, kMatrix._m23, kMatrix._m33);
	kValue._m11 = Det(kMatrix._m00, kMatrix._m20, kMatrix._m30, kMatrix._m02, kMatrix._m22, kMatrix._m32, kMatrix._m03, kMatrix._m23, kMatrix._m33);
	kValue._m21 =-Det(kMatrix._m00, kMatrix._m20, kMatrix._m30, kMatrix._m01, kMatrix._m21, kMatrix._m31, kMatrix._m03, kMatrix._m23, kMatrix._m33);
	kValue._m31 = Det(kMatrix._m00, kMatrix._m20, kMatrix._m30, kMatrix._m01, kMatrix._m21, kMatrix._m31, kMatrix._m02, kMatrix._m22, kMatrix._m32);
	
	kValue._m02 = Det(kMatrix._m01, kMatrix._m11, kMatrix._m31, kMatrix._m02, kMatrix._m12, kMatrix._m32, kMatrix._m03, kMatrix._m13, kMatrix._m33);
	kValue._m12 =-Det(kMatrix._m00, kMatrix._m10, kMatrix._m30, kMatrix._m02, kMatrix._m12, kMatrix._m32, kMatrix._m03, kMatrix._m13, kMatrix._m33);
	kValue._m22 = Det(kMatrix._m00, kMatrix._m10, kMatrix._m30, kMatrix._m01, kMatrix._m11, kMatrix._m31, kMatrix._m03, kMatrix._m13, kMatrix._m33);
	kValue._m32 =-Det(kMatrix._m00, kMatrix._m10, kMatrix._m30, kMatrix._m01, kMatrix._m11, kMatrix._m31, kMatrix._m02, kMatrix._m12, kMatrix._m32);
	
	kValue._m03 =-Det(kMatrix._m01, kMatrix._m11, kMatrix._m21, kMatrix._m02, kMatrix._m12, kMatrix._m22, kMatrix._m03, kMatrix._m13, kMatrix._m23);
	kValue._m13 = Det(kMatrix._m00, kMatrix._m10, kMatrix._m20, kMatrix._m02, kMatrix._m12, kMatrix._m22, kMatrix._m03, kMatrix._m13, kMatrix._m23);
	kValue._m23 =-Det(kMatrix._m00, kMatrix._m10, kMatrix._m20, kMatrix._m01, kMatrix._m11, kMatrix._m21, kMatrix._m03, kMatrix._m13, kMatrix._m23);
	kValue._m33 = Det(kMatrix._m00, kMatrix._m10, kMatrix._m20, kMatrix._m01, kMatrix._m11, kMatrix._m21, kMatrix._m02, kMatrix._m12, kMatrix._m22);

	float fDet = (kMatrix._m00 * kValue._m00) +  (kMatrix._m01 * kValue._m10) +  (kMatrix._m02 * kValue._m20) +  (kMatrix._m03 * kValue._m30);
	
	// the following divions goes unchecked for division by zero, we should 
	// consider throwing an exception if det < eps
	float fOODet = 1.f / fDet;	

	//
	Mult(kValue, fOODet);
	
	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdVector3& sdMath::TransformPosition(sdVector3& kValue, const sdVector3& kPosition, const sdMatrix4& kMatrix)
{
	float fOODet = 1.f;
	float fDivider = kPosition.m_fX * kMatrix._m03 + kPosition.m_fY * kMatrix._m13 + kPosition.m_fZ * kMatrix._m23 + kMatrix._m33;
	if (fDivider > MATH_EPS || fDivider < -MATH_EPS)
		fOODet = 1.f / fDivider;

	kValue.m_fX = (kMatrix._m00 * kPosition.m_fX + kMatrix._m10 * kPosition.m_fY + kMatrix._m20 * kPosition.m_fZ + kMatrix._m30) * fOODet;
	kValue.m_fY = (kMatrix._m01 * kPosition.m_fX + kMatrix._m11 * kPosition.m_fY + kMatrix._m21 * kPosition.m_fZ + kMatrix._m31) * fOODet;
	kValue.m_fZ = (kMatrix._m02 * kPosition.m_fX + kMatrix._m12 * kPosition.m_fY + kMatrix._m22 * kPosition.m_fZ + kMatrix._m32) * fOODet;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdVector3& sdMath::TransformPosition(sdVector3& kValue, const sdMatrix4& kMatrix, const sdVector3& kPosition)
{
	float fOODet = 1.f;
	float fDivider = kPosition.m_fX * kMatrix._m30 + kPosition.m_fY * kMatrix._m31 + kPosition.m_fZ * kMatrix._m32 + kMatrix._m33;
	if (fDivider > MATH_EPS || fDivider < -MATH_EPS)
		fOODet = 1.f / fDivider;

	kValue.m_fX = (kPosition.m_fX * kMatrix._m00 + kPosition.m_fY * kMatrix._m01 + kPosition.m_fZ * kMatrix._m02 + kMatrix._m03) * fOODet;
	kValue.m_fY = (kPosition.m_fX * kMatrix._m10 + kPosition.m_fY * kMatrix._m11 + kPosition.m_fZ * kMatrix._m12 + kMatrix._m13) * fOODet;
	kValue.m_fZ = (kPosition.m_fX * kMatrix._m20 + kPosition.m_fY * kMatrix._m21 + kPosition.m_fZ * kMatrix._m22 + kMatrix._m23) * fOODet;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdVector3& sdMath::TransformDirection(sdVector3& kValue, const sdVector3& kDirection, const sdMatrix4& kMatrix)
{
	kValue.m_fX = kDirection.m_fX * kMatrix._m00 + kDirection.m_fY * kMatrix._m10 + kDirection.m_fZ * kMatrix._m20;
	kValue.m_fY = kDirection.m_fX * kMatrix._m01 + kDirection.m_fY * kMatrix._m11 + kDirection.m_fZ * kMatrix._m21;
	kValue.m_fZ = kDirection.m_fX * kMatrix._m02 + kDirection.m_fY * kMatrix._m12 + kDirection.m_fZ * kMatrix._m22;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdVector3& sdMath::TransformDirection(sdVector3& kValue, const sdMatrix4& kMatrix, const sdVector3& kDirection)
{
	kValue.m_fX = kMatrix._m00 * kDirection.m_fX + kMatrix._m01 * kDirection.m_fY + kMatrix._m02 * kDirection.m_fZ;
	kValue.m_fY = kMatrix._m10 * kDirection.m_fX + kMatrix._m11 * kDirection.m_fY + kMatrix._m12 * kDirection.m_fZ;
	kValue.m_fZ = kMatrix._m20 * kDirection.m_fX + kMatrix._m21 * kDirection.m_fY + kMatrix._m22 * kDirection.m_fZ;

	return kValue;	
}
//-------------------------------------------------------------------------------------------------
sdMatrix4& sdMath::LookAtRHToRH(sdMatrix4& kValue, const sdVector3& kEye, const sdVector3& kTarget, const sdVector3& kUp)
{
	// Z(前向:从目标点指向观察点)
	sdVector3 kZ;
	Sub(kZ, kEye, kTarget);
	Normalize(kZ);

	// X(右向:从观察点向右)
	// X = Y(Up) cross Z
	sdVector3 kX;
	Cross(kX, kUp, kZ);
	Normalize(kX);

	// Y(上向:从观察点向上)
	// Y = Z cross X
	sdVector3 kY;
	Cross(kY, kZ, kX);
	Normalize(kY);

	// kMatrix
	kValue._m00 = kX.m_fX;
	kValue._m10 = kX.m_fY;
	kValue._m20 = kX.m_fZ;
	kValue._m30 = -Dot(kEye, kX);

	kValue._m01 = kY.m_fX;
	kValue._m11 = kY.m_fY;
	kValue._m21 = kY.m_fZ;
	kValue._m31 = -Dot(kEye, kY);

	kValue._m02 = kZ.m_fX;
	kValue._m12 = kZ.m_fY;
	kValue._m22 = kZ.m_fZ;
	kValue._m32 = -Dot(kEye, kZ);
	
	kValue._m03 = 0.f;
	kValue._m13 = 0.f;
	kValue._m23 = 0.f;
	kValue._m33 = 1.f;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdMatrix4& sdMath::LookAtRHToLH(sdMatrix4& kValue, const sdVector3& kEye, const sdVector3& kTarget, const sdVector3& kUp)
{
	// Z(前向:从目标点指向观察点)
	sdVector3 kZ;
	Sub(kZ, kTarget, kEye);
	Normalize(kZ);

	// X(右向:从观察点向右)
	// X = -(Y(Up) cross Z)
	sdVector3 kX;
	Cross(kX, kUp, kZ);
	Negative(kX);
	Normalize(kX);
	
	// Y(上向:从观察点向上)
	// Y =  -(X cross Z)
	sdVector3 kY;
	Cross(kY, kX, kZ);
	Negative(kY);
	Normalize(kY);

	// kMatrix
	kValue._m00 = kX.m_fX;
	kValue._m10 = kX.m_fY;
	kValue._m20 = kX.m_fZ;
	kValue._m30 = -Dot(kEye, kX);

	kValue._m01 = kY.m_fX;
	kValue._m11 = kY.m_fY;
	kValue._m21 = kY.m_fZ;
	kValue._m31 = -Dot(kEye, kY);

	kValue._m02 = kZ.m_fX;
	kValue._m12 = kZ.m_fY;
	kValue._m22 = kZ.m_fZ;
	kValue._m32 = -Dot(kEye, kZ);

	kValue._m03 = 0.f;
	kValue._m13 = 0.f;
	kValue._m23 = 0.f;
	kValue._m33 = 1.f;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdMatrix4& sdMath::LookAtLHToRH(sdMatrix4& kValue, const sdVector3& kEye, const sdVector3& kTarget, const sdVector3& kUp)
{
	// Z(前向:从目标点指向观察点)
	sdVector3 kZ;
	Sub(kZ, kEye, kTarget);
	Normalize(kZ);

	// X(右向:从观察点向右)
	// X = -(Y(Up) cross Z)
	sdVector3 kX;
	Cross(kX, kUp, kZ);
	Negative(kX);
	Normalize(kX);

	// Y(上向:从观察点向上)
	// Y = -(Z cross X)
	sdVector3 kY;
	Cross(kY, kZ, kX);
	Negative(kY);
	Normalize(kY);

	// kMatrix
	kValue._m00 = kX.m_fX;
	kValue._m10 = kX.m_fY;
	kValue._m20 = kX.m_fZ;
	kValue._m30 = -Dot(kEye, kX);

	kValue._m01 = kY.m_fX;
	kValue._m11 = kY.m_fY;
	kValue._m21 = kY.m_fZ;
	kValue._m31 = -Dot(kEye, kY);

	kValue._m02 = kZ.m_fX;
	kValue._m12 = kZ.m_fY;
	kValue._m22 = kZ.m_fZ;
	kValue._m32 = -Dot(kEye, kZ);

	kValue._m03 = 0.f;
	kValue._m13 = 0.f;
	kValue._m23 = 0.f;
	kValue._m33 = 1.f;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdMatrix4& sdMath::LookAtLHToLH(sdMatrix4& kValue, const sdVector3& kEye, const sdVector3& kTarget, const sdVector3& kUp)
{
	// Z(前向:从观察点指向目标点)
	sdVector3 kZ;
	Sub(kZ, kTarget, kEye);
	Normalize(kZ);

	// X(右向:从观察点向右)
	// X = Y(Up) cross Z
	sdVector3 kX;
	Cross(kX, kUp, kZ);
	Normalize(kX);

	// Y(上向:从观察点向上)
	// Y = Z cross X
	sdVector3 kY;
	Cross(kY, kZ, kX);
	Normalize(kY);

	// kMatrix
	kValue._m00 = kX.m_fX;
	kValue._m10 = kX.m_fY;
	kValue._m20 = kX.m_fZ;
	kValue._m30 = -Dot(kEye, kX);

	kValue._m01 = kY.m_fX;
	kValue._m11 = kY.m_fY;
	kValue._m21 = kY.m_fZ;
	kValue._m31 = -Dot(kEye, kY);

	kValue._m02 = kZ.m_fX;
	kValue._m12 = kZ.m_fY;
	kValue._m22 = kZ.m_fZ;
	kValue._m32 = -Dot(kEye, kZ);

	kValue._m03 = 0.f;
	kValue._m13 = 0.f;
	kValue._m23 = 0.f;
	kValue._m33 = 1.f;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdMatrix4& sdMath::Perspective(sdMatrix4& kValue, const float fLeft, const float fRight, const float fBottom, const float fTop, const float fNear, const float fFar)
{
	//
	//	0.其中(l,b,n)(r,t,f)是视锥在观察坐标系的最小点和最大点
	//	1.观察坐标系为X向右，Y向上，Z向里(内)
	//	2.观察立方体为观察坐标系的[-1,1][-1,1][-1,1]区间
	//	3.观察立方体中心为观察坐标系的原点
	//
	// 变换矩阵:
	//	|2n/(r-l)		0				0				0 |
	//	|0				2n/(t-b)		0				0 |
	//	|-(r+l)/(r-l)	-(t+b)/(t-b)	(f+n)/(f-n)		1 |
	//	|0				0				-2fn/(f-n)		0 |
	//

	kValue._m00 = (2.f * fNear) / (fRight - fLeft);
	kValue._m10 = 0.f;
	kValue._m20 = -(fRight + fLeft) / (fRight - fLeft);
	kValue._m30 = 0.f;

	kValue._m01 = 0.f;
	kValue._m11 = (2.f * fNear) / (fTop - fBottom);
	kValue._m21 = -(fTop + fBottom) / (fTop - fBottom);
	kValue._m31 = 0.f;

	kValue._m02 = 0.f;
	kValue._m12 = 0.f;
	kValue._m22 = (fFar + fNear) / (fFar - fNear);
	kValue._m32 = -(2.f * fFar * fNear) / (fFar - fNear);

	kValue._m03 = 0.f;
	kValue._m13 = 0.f;
	kValue._m23 = 1.f;	
	kValue._m33 = 0.f;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdMatrix4& sdMath::PerspectiveGL(sdMatrix4& kValue, const float fLeft, const float fRight, const float fBottom, const float fTop, const float fNear, const float fFar)
{
	//
	//	|2n/(r-l)		0				0				0 |
	//	|0				2n/(t-b)		0				0 |
	//	|(r+l)/(r-l)	(t+b)/(t-b)		-(f+n)/(f-n)   -1 |
	//	|0				0				-2fn/(f-n)		0 |
	//

	kValue._m00 = (2.f * fNear) / (fRight - fLeft);
	kValue._m10 = 0.f;
	kValue._m20 = (fRight + fLeft) / (fRight - fLeft);
	kValue._m30 = 0.f;

	kValue._m01 = 0.f;
	kValue._m11 = (2.f * fNear) / (fTop - fBottom);
	kValue._m21 = (fTop + fBottom) / (fTop - fBottom);
	kValue._m31 = 0.f;

	kValue._m02 = 0.f;
	kValue._m12 = 0.f;
	kValue._m22 = -(fFar + fNear) / (fFar - fNear);
	kValue._m32 = -(2.f * fFar * fNear) / (fFar - fNear);;

	kValue._m03 = 0.f;
	kValue._m13 = 0.f;
	kValue._m23 = -1.f;	
	kValue._m33 = 0.f;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdMatrix4& sdMath::PerspectiveD3DLH(sdMatrix4& kValue, const float fLeft, const float fRight, const float fBottom, const float fTop, const float fNear, const float fFar)
{
	//
	//	|2n/(r-l)		0				0				0 |
	//	|0				2n/(t-b)		0				0 |
	//	|-(r+l)/(r-l)	-(t+b)/(t-b)	f/(f-n)			1 |
	//	|0				0				-fn/(f-n)		0 |
	//

	kValue._m00 = (2.f * fNear) / (fRight - fLeft);
	kValue._m10 = 0.f;
	kValue._m20 = (fRight + fLeft) / (fRight - fLeft);
	kValue._m30 = 0.f;

	kValue._m01 = 0.f;
	kValue._m11 = (2.f * fNear) / (fTop - fBottom);
	kValue._m21 = (fTop + fBottom) / (fTop - fBottom);
	kValue._m31 = 0.f;

	kValue._m02 = 0.f;
	kValue._m12 = 0.f;
	kValue._m22 = fFar / (fFar - fNear);
	kValue._m32 = -(fFar * fNear) / (fFar - fNear);

	kValue._m03 = 0.f;
	kValue._m13 = 0.f;
	kValue._m23 = 1.f;
	kValue._m33 = 0.f;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdMatrix4& sdMath::PerspectiveD3DRH(sdMatrix4& kValue, const float fLeft, const float fRight, const float fBottom, const float fTop, const float fNear, const float fFar)
{
	//
	//	|2n/(r-l)		0				0				0 |
	//	|0				2n/(t-b)		0				0 |
	//	|(r+l)/(r-l)	(t+b)/(t-b)		-f/(f-n)	   -1 |
	//	|0				0				-fn/(f-n)		0 |
	//

	kValue._m00 = (2.f * fNear) / (fRight - fLeft);
	kValue._m10 = 0.f;
	kValue._m20 = (fRight + fLeft) / (fRight - fLeft);
	kValue._m30 = 0.f;

	kValue._m01 = 0.f;
	kValue._m11 = (2.f * fNear) / (fTop - fBottom);
	kValue._m21 = (fTop + fBottom) / (fTop - fBottom);
	kValue._m31 = 0.f;

	kValue._m02 = 0.f;
	kValue._m12 = 0.f;
	kValue._m22 = -fFar / (fFar - fNear);
	kValue._m32 = -(fFar * fNear) / (fFar - fNear);

	kValue._m03 = 0.f;
	kValue._m13 = 0.f;
	kValue._m23 = -1.f;
	kValue._m33 = 0.f;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdMatrix4& sdMath::PerspectiveFov(sdMatrix4& kValue, const float fFovY, const float fAspect, const float fNear, const float fFar)
{
	float fYMax = fNear * tanf(fFovY * 0.5f);	
	float fYMin = -fYMax;

	float fXMax = fYMax * fAspect;
	float fXMin = fYMin * fAspect;

	return Perspective(kValue, fXMin, fXMax, fYMin, fYMax, -fNear, -fFar);
}
//-------------------------------------------------------------------------------------------------
sdMatrix4& sdMath::Ortho(sdMatrix4& kValue, const float fLeft, const float fRight, const float fBottom, const float fTop, const float fNear, const float fFar)
{
	//
	//	0.其中l\r\t\b\n\f都是与观察坐标系坐标轴平行的面
	//	1.观察坐标系为X向右，Y向上，Z向里(内)
	//	2.观察立方体为观察坐标系的[-1,1][-1,1][-1,1]区间
	//	3.观察立方体中心为观察坐标系的原点
	//
	// 平移矩阵：
	//	|1				0				0				0 |
	//	|0				1				0				0 |
	//	|0				0				1				0 |
	//	|-(r+l)/2		-(t+b)/2		-(f+n)/2		1 |
	//
	// 缩放矩阵：
	//	|2/(r-l)		0				0				0 |
	//	|0				2/(t-b)			0				0 |
	//	|0				0				2/(f-n)			0 |
	//	|0				0				0				1 |
	//
	// 变换矩阵 = 平移矩阵 * 缩放矩阵:
	//	|2/(r-l)		0				0				0 |
	//	|0				2/(t-b)			0				0 |
	//	|0				0				2/(f-n)			0 |
	//	|-(r+l)/(r-l)	-(t+b)/(t-b)	-(f+n)/(f-n)	1 |
	//

	kValue._m00 = 2.f / (fRight - fLeft);
	kValue._m10 = 0.f;
	kValue._m20 = 0.f;
	kValue._m30 = -(fRight + fLeft) / (fRight - fLeft);	

	kValue._m01 = 0.f;
	kValue._m11 = 2.f / (fTop - fBottom);
	kValue._m21 = 0.f;
	kValue._m31 = -(fTop + fBottom) / (fTop - fBottom);

	kValue._m02 = 0.f;
	kValue._m12 = 0.f;
	kValue._m22 = 2.f / (fFar - fNear);
	kValue._m32 = -(fFar + fNear) / (fFar - fNear);

	kValue._m03 = 0.f;
	kValue._m13 = 0.f;
	kValue._m23 = 0.f;
	kValue._m33 = 1.f;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdMatrix4& sdMath::OrthoGL(sdMatrix4& kValue, const float fLeft, const float fRight, const float fBottom, const float fTop, const float fNear, const float fFar)
{
	//
	//	|2/(r-l)		0				0				0 |
	//	|0				2/(t-b)			0				0 |
	//	|0				0				-2/(f-n)		0 |
	//	|-(r+l)/(r-l)	-(t+b)/(t-b)	-(f+n)/(f-n)	1 |
	//

	kValue._m00 = 2.f / (fRight - fLeft);
	kValue._m10 = 0.f;
	kValue._m20 = 0.f;
	kValue._m30 = -(fRight + fLeft) / (fRight - fLeft);	

	kValue._m01 = 0.f;
	kValue._m11 = 2.f / (fTop - fBottom);
	kValue._m21 = 0.f;
	kValue._m31 = -(fTop + fBottom) / (fTop - fBottom);

	kValue._m02 = 0.f;
	kValue._m12 = 0.f;
	kValue._m22 = -2.f / (fFar - fNear);
	kValue._m32 = -(fFar + fNear) / (fFar - fNear);

	kValue._m03 = 0.f;
	kValue._m13 = 0.f;
	kValue._m23 = 0.f;
	kValue._m33 = 1.f;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdMatrix4& sdMath::OrthoD3DRH(sdMatrix4& kValue, const float fLeft, const float fRight, const float fBottom, const float fTop, const float fNear, const float fFar)
{
	//
	//	|2/(r-l)		0				0				0 |
	//	|0				2/(t-b)			0				0 |
	//	|0				0				-1/(f-n)		0 |
	//	|-(r+l)/(r-l)	-(t+b)/(t-b)	-n/(f-n)		1 |
	//

	kValue._m00 = 2.f / (fRight - fLeft);
	kValue._m10 = 0.f;
	kValue._m20 = 0.f;
	kValue._m30 = -(fRight + fLeft) / (fRight - fLeft);	

	kValue._m01 = 0.f;
	kValue._m11 = 2.f / (fTop - fBottom);
	kValue._m21 = 0.f;
	kValue._m31 = -(fTop + fBottom) / (fTop - fBottom);

	kValue._m02 = 0.f;
	kValue._m12 = 0.f;
	kValue._m22 = -1.f / (fFar - fNear);
	kValue._m32 = -fNear / (fFar - fNear);

	kValue._m03 = 0.f;
	kValue._m13 = 0.f;
	kValue._m23 = 0.f;
	kValue._m33 = 1.f;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdMatrix4& sdMath::OrthoD3DLH(sdMatrix4& kValue, const float fLeft, const float fRight, const float fBottom, const float fTop, const float fNear, const float fFar)
{
	//
	//	|2/(r-l)		0				0				0 |
	//	|0				2/(t-b)			0				0 |
	//	|0				0				1/(f-n)			0 |
	//	|-(r+l)/(r-l)	-(t+b)/(t-b)	-n/(f-n)		1 |
	//

	kValue._m00 = 2.f / (fRight - fLeft);
	kValue._m10 = 0.f;
	kValue._m20 = 0.f;
	kValue._m30 = -(fRight + fLeft) / (fRight - fLeft);	

	kValue._m01 = 0.f;
	kValue._m11 = 2.f / (fTop - fBottom);
	kValue._m21 = 0.f;
	kValue._m31 = -(fTop + fBottom) / (fTop - fBottom);

	kValue._m02 = 0.f;
	kValue._m12 = 0.f;
	kValue._m22 = 1.f / (fFar - fNear);
	kValue._m32 = -fNear / (fFar - fNear);

	kValue._m03 = 0.f;
	kValue._m13 = 0.f;
	kValue._m23 = 0.f;
	kValue._m33 = 1.f;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_MATH
BASE_NAMESPACE_END_BASE