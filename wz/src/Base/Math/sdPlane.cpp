#include "sdBasePCH.h"
#include "sdPlane.h"

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_MATH
//-------------------------------------------------------------------------------------------------
// ������ұ�(���ڼ�����ײ���)
const static uint g_uiVertexIndex[8] = 
{
	1,	///< 000,X-Y-Z-,��Сֵ������3,4,5
	4,	///< 001
	7,	///< 010
	2,	///< 011
	3,	///< 100
	6,	///< 101
	5,	///< 110
	0,	///< 111,X+Y+Z+,��Сֵ������1,2,3
};	

// ֵ������(���ڼ�����ײ���)
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

// ȡ���������������ķ��ţ����������������
uint GetNormalCode(const sdVector3& kNormal)
{
	const uint* px = reinterpret_cast<const uint*>(&kNormal.m_fX);
	const uint* py = reinterpret_cast<const uint*>(&kNormal.m_fY);
	const uint* pz = reinterpret_cast<const uint*>(&kNormal.m_fZ);

	//
	// IEEE32�ķ���Ϊ��31λ���Ӷ���
	//	(sign(*px) << 2) | (sign(*py) << 1) | (sign(*pz))
	//	==>	(((*px) >> 31 & 0x1) << 2) | (((*py) >> 31 & 0x1) << 1) | ((*pz) >> 31 & 0x1)
	//	==>	((*px) >> 29 & 0x4)		   | ((*py) >> 30 & 0x2)        | ((*pz) >> 31 & 0x1)
	//
	// �ο�:
	//	https://en.wikipedia.org/wiki/IEEE_floating_point
	//	https://en.wikipedia.org/wiki/Single_precision_floating-point_format
	//
	return ((*px) >> 29 & 0x4) | ((*py) >> 30 & 0x2)  | ((*pz) >> 31 & 0x1);
}
//-------------------------------------------------------------------------------------------------
sdPlane::Side sdPlane::GetSide(const float* pfVextex) const
{
	// �ж�AABB�Ƿ���ȫλ�ڱ���
	//	1.����ƽ�淨��,�����������
	//	2.�ҳ���ƽ�������AABB����
	//	3.���㶥�㵽ƽ��ľ���
	//	4.������ƽ������������AABB�����ж��㶼�����棬�ʲ��ཻ
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


	// �ж�AABB�Ƿ���ȫλ������
	// @{
	uiCode = (~uiCode) & 0x07;	///< ��3λȡ��
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