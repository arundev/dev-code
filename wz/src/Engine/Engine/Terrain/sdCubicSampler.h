//*************************************************************************************************
// ����:	���θ߶Ȳ�������Catmull�CRom spline Interpolation
//
// �ο���ַ:
//		https://en.wikipedia.org/wiki/Cubic_Hermite_spline
//---------------------------------------------------------
// ����:		
// ����:		2013-05-05
// ����޸�:	2013-05-06
//*************************************************************************************************
#pragma once
#ifndef _TERRAIN_CUBIC_SAMPLER_H__
#define _TERRAIN_CUBIC_SAMPLER_H__

// WZ�б�ʾ����iSapcing��ֵ(�������߶ȵ�֮���������)
#define MAX_TERRAIN_TESSELATION 16

//-----------------------------------------------------------------------------
// ��Ҫ����Cubic�����Ķ�ά���ݺ�
//-----------------------------------------------------------------------------
class sdCubicSamplerKernel : public NiMemObject
{
public:
	float m_akKernel[4][4];
};
//-----------------------------------------------------------------------------
// �Ե��θ߶Ƚ��в�����������
//	1.����ʹ��Catmull�CRom Spline����֪�ĸ��ڽ��������в�ֵ
//	2.��������MAX_TERRAIN_TESSELATION�µĸ���ϵ��������iSpacing����ֵֻ��Ϊ
//	  1/2/4/8/16����WZ�е����ղ������Ȼ����ĳһ��ϵ���ϣ��Ӷ�����Ԥ��������Ч��
//-----------------------------------------------------------------------------
class sdCubicSampler : public NiMemObject
{
public:
	sdCubicSampler(uint uiMaxTesselation = MAX_TERRAIN_TESSELATION);

	// ��ȡ��ֵ�߶�
	// Catmull���߾������п��Ƶ�,��������������ά�Ƚ���һά��ֵ
	//
	//	@param[in]	kKernel	�������ݼ�
	//	@param[in]	uiX/uiY	��һ����[0, MAX_TERRAIN_TESSELATION]������
	//	return				��ֵ��ĸ߶�ֵ
	//
	float Sample(const sdCubicSamplerKernel& kKernel, uint uiX, uint uiY) const;

	// ��ȡ�ݶ�
	//
	//	@param[in]	kKernel	�������ݼ�
	//	@param[in]	uiX/uiY	��һ����[0, MAX_TERRAIN_TESSELATION]������
	//	return				�ݶ�ֵ
	float SampleDerivX(const sdCubicSamplerKernel& kKernel, uint uiX, uint uiY) const;
	float SampleDerivY(const sdCubicSamplerKernel& kKernel, uint uiX, uint uiY) const;

protected:
	// ����һά����
	float Cubic(float P0, float P1, float P2 , float P3	, uint I) const;
	float CubicDeriv(float P0, float P1, float P2 , float P3, uint I) const;

protected:
	uint	m_uiMaxTesselation;
	float	m_akCubicBasis[MAX_TERRAIN_TESSELATION + 1][4];
	float	m_akCubicBasisDeriv[MAX_TERRAIN_TESSELATION + 1][4];
};
//-----------------------------------------------------------------------------
NiSmartPointer(sdHeightMap);
#endif