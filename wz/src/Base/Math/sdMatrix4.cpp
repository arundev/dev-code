#include "sdBasePCH.h"
#include "sdMatrix4.h"
#include "sdVector4.h"

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_MATH
//-------------------------------------------------------------------------------------------------
const sdMatrix4 sdMatrix4::ZERO(
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0);

const sdMatrix4 sdMatrix4::ZEROAFFINE(
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 1);

const sdMatrix4 sdMatrix4::IDENTITY(
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1);
//-------------------------------------------------------------------------------------------------
void sdMatrix4::GetRow(int iRow, sdVector4& kRow) const
{
	kRow.m_fX = m[iRow][0];
	kRow.m_fY = m[iRow][1];
	kRow.m_fZ = m[iRow][2];
	kRow.m_fW = m[iRow][3];
}
//-------------------------------------------------------------------------------------------------
void sdMatrix4::SetRow(int iRow, const sdVector4& kRow)
{
	m[iRow][0] = kRow.m_fX;
	m[iRow][1] = kRow.m_fY;
	m[iRow][2] = kRow.m_fZ;
	m[iRow][3] = kRow.m_fW;
}
//-------------------------------------------------------------------------------------------------
void sdMatrix4::GetCol(int iCol, sdVector4& kCol) const
{
	kCol.m_fX = m[0][iCol];
	kCol.m_fY = m[1][iCol];
	kCol.m_fZ = m[2][iCol];
	kCol.m_fW = m[3][iCol];
}
//-------------------------------------------------------------------------------------------------
void sdMatrix4::SetCol(int iCol, const sdVector4& kCol)
{
	m[0][iCol] = kCol.m_fX;
	m[1][iCol] = kCol.m_fY;
	m[2][iCol] = kCol.m_fZ;
	m[3][iCol] = kCol.m_fW;
}
//-------------------------------------------------------------------------------------------------
sdVector4 operator * (const sdVector4& lkVector, const sdMatrix4& rkMatrix)
{
	sdVector4 kValue;
	kValue.m_fX = lkVector.m_fX * rkMatrix._m00 + lkVector.m_fY * rkMatrix._m10 + lkVector.m_fZ * rkMatrix._m20 + lkVector.m_fW * rkMatrix._m30;
	kValue.m_fY = lkVector.m_fX * rkMatrix._m01 + lkVector.m_fY * rkMatrix._m11 + lkVector.m_fZ * rkMatrix._m21 + lkVector.m_fW * rkMatrix._m31;
	kValue.m_fZ = lkVector.m_fX * rkMatrix._m02 + lkVector.m_fY * rkMatrix._m12 + lkVector.m_fZ * rkMatrix._m22 + lkVector.m_fW * rkMatrix._m32;
	kValue.m_fW = lkVector.m_fX * rkMatrix._m03 + lkVector.m_fY * rkMatrix._m13 + lkVector.m_fZ * rkMatrix._m23 + lkVector.m_fW * rkMatrix._m33;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
sdVector4 operator * (const sdMatrix4& lkMatrix, const sdVector4& rkVector)
{
	sdVector4 kValue;
	kValue.m_fX = lkMatrix._m00 * rkVector.m_fX + lkMatrix._m01 * rkVector.m_fY + lkMatrix._m02 * rkVector.m_fZ + lkMatrix._m03 * rkVector.m_fW;
	kValue.m_fY = lkMatrix._m10 * rkVector.m_fX + lkMatrix._m11 * rkVector.m_fY + lkMatrix._m12 * rkVector.m_fZ + lkMatrix._m13 * rkVector.m_fW;
	kValue.m_fZ = lkMatrix._m20 * rkVector.m_fX + lkMatrix._m21 * rkVector.m_fY + lkMatrix._m22 * rkVector.m_fZ + lkMatrix._m23 * rkVector.m_fW;
	kValue.m_fW = lkMatrix._m30 * rkVector.m_fX + lkMatrix._m31 * rkVector.m_fY + lkMatrix._m32 * rkVector.m_fZ + lkMatrix._m33 * rkVector.m_fW;

	return kValue;
}
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_MATH
BASE_NAMESPACE_END_BASE