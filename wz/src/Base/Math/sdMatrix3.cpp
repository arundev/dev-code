#include "sdBasePCH.h"
#include "sdMatrix3.h"
#include "sdVector3.h"

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_MATH
//-------------------------------------------------------------------------------------------------
const sdMatrix3 sdMatrix3::ZERO(
	0, 0, 0,
	0, 0, 0,
	0, 0, 0);

const sdMatrix3 sdMatrix3::IDENTITY(
	1, 0, 0,
	0, 1, 0,
	0, 0, 1);
//-------------------------------------------------------------------------------------------------
sdVector3 operator * (const sdVector3& lkVector, const sdMatrix3& rkMatrix)
{
	sdVector3 kValue;
	kValue.m_fX = lkVector.m_fX * rkMatrix._m00 + lkVector.m_fY * rkMatrix._m10 + lkVector.m_fZ * rkMatrix._m20;
	kValue.m_fY = lkVector.m_fX * rkMatrix._m01 + lkVector.m_fY * rkMatrix._m11 + lkVector.m_fZ * rkMatrix._m21;
	kValue.m_fZ = lkVector.m_fX * rkMatrix._m02 + lkVector.m_fY * rkMatrix._m12 + lkVector.m_fZ * rkMatrix._m22;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdVector3 operator * (const sdMatrix3& lkMatrix, const sdVector3& rkVector)
{
	sdVector3 kValue;
	kValue.m_fX = lkMatrix._m00 * rkVector.m_fX + lkMatrix._m01 * rkVector.m_fY + lkMatrix._m02 * rkVector.m_fZ;
	kValue.m_fY = lkMatrix._m10 * rkVector.m_fX + lkMatrix._m11 * rkVector.m_fY + lkMatrix._m12 * rkVector.m_fZ;
	kValue.m_fZ = lkMatrix._m20 * rkVector.m_fX + lkMatrix._m21 * rkVector.m_fY + lkMatrix._m22 * rkVector.m_fZ;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_MATH
BASE_NAMESPACE_END_BASE