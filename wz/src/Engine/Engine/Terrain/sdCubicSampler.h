//*************************************************************************************************
// 内容:	地形高度采样器，Catmull–Rom spline Interpolation
//
// 参考网址:
//		https://en.wikipedia.org/wiki/Cubic_Hermite_spline
//---------------------------------------------------------
// 作者:		
// 创建:		2013-05-05
// 最后修改:	2013-05-06
//*************************************************************************************************
#pragma once
#ifndef _TERRAIN_CUBIC_SAMPLER_H__
#define _TERRAIN_CUBIC_SAMPLER_H__

// WZ中表示最大的iSapcing数值(即两个高度点之间的最大距离)
#define MAX_TERRAIN_TESSELATION 16

//-----------------------------------------------------------------------------
// 需要进行Cubic采样的二维数据核
//-----------------------------------------------------------------------------
class sdCubicSamplerKernel : public NiMemObject
{
public:
	float m_akKernel[4][4];
};
//-----------------------------------------------------------------------------
// 对地形高度进行采样的运算器
//	1.这里使用Catmull–Rom Spline，已知四个邻近点来进行插值
//	2.计算出最大MAX_TERRAIN_TESSELATION下的各组系数。由于iSpacing的数值只能为
//	  1/2/4/8/16，故WZ中的最终采样点必然落在某一组系数上，从而进行预计算是有效的
//-----------------------------------------------------------------------------
class sdCubicSampler : public NiMemObject
{
public:
	sdCubicSampler(uint uiMaxTesselation = MAX_TERRAIN_TESSELATION);

	// 获取插值高度
	// Catmull曲线经过所有控制点,这里依次在两个维度进行一维插值
	//
	//	@param[in]	kKernel	采样数据集
	//	@param[in]	uiX/uiY	归一化到[0, MAX_TERRAIN_TESSELATION]的索引
	//	return				差值后的高度值
	//
	float Sample(const sdCubicSamplerKernel& kKernel, uint uiX, uint uiY) const;

	// 获取梯度
	//
	//	@param[in]	kKernel	采样数据集
	//	@param[in]	uiX/uiY	归一化到[0, MAX_TERRAIN_TESSELATION]的索引
	//	return				梯度值
	float SampleDerivX(const sdCubicSamplerKernel& kKernel, uint uiX, uint uiY) const;
	float SampleDerivY(const sdCubicSamplerKernel& kKernel, uint uiX, uint uiY) const;

protected:
	// 进行一维采样
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