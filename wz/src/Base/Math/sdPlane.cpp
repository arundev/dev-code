#include "sdBasePCH.h"
#include "sdPlane.h"

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_MATH
//-------------------------------------------------------------------------------------------------
// 顶点查找表(用于加速碰撞检测)
const static uint g_uiVertexIndex[8] = 
{
	1,	///< 000,X-Y-Z-,最小值索引是3,4,5
	4,	///< 001
	7,	///< 010
	2,	///< 011
	3,	///< 100
	6,	///< 101
	5,	///< 110
	0,	///< 111,X+Y+Z+,最小值索引是1,2,3
};	

// 值索引表(用于加速碰撞检测)
const static uint g_uiValueIndex[8][3] =
{
	{0, 1, 2},
	{3, 4, 5},
	{3, 1, 2},
	{0, 4, 5},
	{3, 4, 2},
	{0, 1, 5},
	{0, 4, 2},
	{3, 1, 5},
};

// 取出法线三个分量的符号，计算最近顶点索引
uint GetNormalCode(const sdVector3& kNormal)
{
	const uint* px = reinterpret_cast<const uint*>(&kNormal.m_fX);
	const uint* py = reinterpret_cast<const uint*>(&kNormal.m_fY);
	const uint* pz = reinterpret_cast<const uint*>(&kNormal.m_fZ);

	//
	// IEEE32的符号为第31位，从而：
	//	(sign(*px) << 2) | (sign(*py) << 1) | (sign(*pz))
	//	==>	(((*px) >> 31 & 0x1) << 2) | (((*py) >> 31 & 0x1) << 1) | ((*pz) >> 31 & 0x1)
	//	==>	((*px) >> 29 & 0x4)		   | ((*py) >> 30 & 0x2)        | ((*pz) >> 31 & 0x1)
	//
	// 参考:
	//	https://en.wikipedia.org/wiki/IEEE_floating_point
	//	https://en.wikipedia.org/wiki/Single_precision_floating-point_format
	//
	return ((*px) >> 29 & 0x4) | ((*py) >> 30 & 0x2)  | ((*pz) >> 31 & 0x1);
}
//-------------------------------------------------------------------------------------------------
sdPlane::Side sdPlane::GetSide(const float* pfVextex) const
{
	// 判断AABB是否完全位于背面
	//	1.根据平面法线,计算查找索引
	//	2.找出离平面最近的AABB顶点
	//	3.计算顶点到平面的距离
	//	4.顶点在平面外面则整个AABB的所有顶点都在外面，故不相交
	// @{
	uint uiCode = GetNormalCode(m_kNormal);
	uint uiIndex = g_uiVertexIndex[uiCode];

	sdVector3 kNearVertex;
	kNearVertex.m_fX = m_kNormal.m_fX * (*(pfVextex + g_uiValueIndex[uiIndex][0]));
	kNearVertex.m_fY = m_kNormal.m_fY * (*(pfVextex + g_uiValueIndex[uiIndex][1]));
	kNearVertex.m_fZ = m_kNormal.m_fY * (*(pfVextex + g_uiValueIndex[uiIndex][2]));

	float fDistance = m_kNormal.DotProduct(kNearVertex) + m_fDistance;
	if (fDistance < 0.f)
		return E_NEGATIVE_SIDE;
	// @}


	// 判断AABB是否完全位于正面
	// @{
	uiCode = (~uiCode) & 0x07;	///< 低3位取反
	uiIndex = g_uiVertexIndex[uiCode];

	kNearVertex.m_fX = m_kNormal.m_fX * (*(pfVextex + g_uiValueIndex[uiIndex][0]));
	kNearVertex.m_fY = m_kNormal.m_fY * (*(pfVextex + g_uiValueIndex[uiIndex][1]));
	kNearVertex.m_fZ = m_kNormal.m_fY * (*(pfVextex + g_uiValueIndex[uiIndex][2]));

	fDistance = m_kNormal.DotProduct(kNearVertex) + m_fDistance;
	if (fDistance > 0.f)
		return E_NEGATIVE_SIDE;
	// @}

	return sdPlane::E_BOTH_SIDE;
}
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_MATH
BASE_NAMESPACE_END_BASE