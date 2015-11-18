#include "sdEnginePCH.h"
#include "sdCubicSampler.h"

using namespace Base::Math;
//-------------------------------------------------------------------------------------------------
sdCubicSampler::sdCubicSampler(uint uiMaxTesselation)
: m_uiMaxTesselation(uiMaxTesselation)
{
	for (uint I = 0; I <= m_uiMaxTesselation; ++I)
	{
		float T = (float)I / (float)m_uiMaxTesselation;
		float T2 = T * T;
		float T3 = T2 * T;

		// -0.5f * (T * T * T - 2.f * T * T + T);
		// (+2.f * T * T * T - 3.f * T * T + 1.f) - 0.5f * (T * T * T - T * T);
		// (-2.f * T * T * T + 3.f * T * T) + 0.5f * (T * T * T - 2.f * T * T + T);
		// +0.5f * (T * T * T - T * T);
		m_akCubicBasis[I][0] = -0.5f * (T3 - 2.f * T2 + T);
		m_akCubicBasis[I][1] = (+2.f * T3 - 3.f * T2 + 1.f) -0.5f * (T3 - T2);
		m_akCubicBasis[I][2] = (-2.f * T3 + 3.f * T2) + 0.5f * (T3 - 2.f * T2 + T);
		m_akCubicBasis[I][3] = +0.5f * (T3 - T2);

		// 0.5f * (-1.f + 4.f * T - 3.f * T * T);
		// -6.f * T + 6.f * T * T + 0.5f * (2.f * T - 3.f * T * T);
		// +6.f * T - 6.f * T * T + 0.5f * (1.f - 4.f * T + 3.f * T * T);
		// 0.5f * (-2.f * T + 3.f * T * T);
		m_akCubicBasisDeriv[I][0] = 0.5f * (-1.f + 4.f * T - 3.f * T2);
		m_akCubicBasisDeriv[I][1] = -6.f * T + 6.f * T2 + 0.5f * (2.f * T - 3.f * T2);
		m_akCubicBasisDeriv[I][2] = +6.f * T - 6.f * T2 + 0.5f * (1.f - 4.f * T + 3.f * T2);
		m_akCubicBasisDeriv[I][3] = 0.5f * (-2.f * T + 3.f * T2);
	}
}
//-------------------------------------------------------------------------------------------------
float sdCubicSampler::Sample(const sdCubicSamplerKernel& kKernel, uint uiX, uint uiY) const
{
	return Cubic(
		Cubic(kKernel.m_akKernel[0][0], kKernel.m_akKernel[0][1], kKernel.m_akKernel[0][2], kKernel.m_akKernel[0][3], uiX),
		Cubic(kKernel.m_akKernel[1][0], kKernel.m_akKernel[1][1], kKernel.m_akKernel[1][2], kKernel.m_akKernel[1][3], uiX),
		Cubic(kKernel.m_akKernel[2][0], kKernel.m_akKernel[2][1], kKernel.m_akKernel[2][2], kKernel.m_akKernel[2][3], uiX),
		Cubic(kKernel.m_akKernel[3][0], kKernel.m_akKernel[3][1], kKernel.m_akKernel[3][2], kKernel.m_akKernel[3][3], uiX),
		uiY);
}
//-------------------------------------------------------------------------------------------------
float sdCubicSampler::SampleDerivX(const sdCubicSamplerKernel& kKernel, uint uiX, uint uiY) const
{
	return sdMath::Lerp(
		sdMath::Lerp(kKernel.m_akKernel[1][2] - kKernel.m_akKernel[1][0], 
					kKernel.m_akKernel[1][3] - kKernel.m_akKernel[1][1], float(uiX) / (float)m_uiMaxTesselation),
		sdMath::Lerp(kKernel.m_akKernel[2][2] - kKernel.m_akKernel[2][0], 
					kKernel.m_akKernel[2][3] - kKernel.m_akKernel[2][1], float(uiX) / (float)m_uiMaxTesselation),
		float(uiY) / (float)m_uiMaxTesselation);
}
//-------------------------------------------------------------------------------------------------
float sdCubicSampler::SampleDerivY(const sdCubicSamplerKernel& kKernel, uint uiX, uint uiY) const
{
	return sdMath::Lerp(
		sdMath::Lerp(kKernel.m_akKernel[2][1] - kKernel.m_akKernel[0][1], 
				kKernel.m_akKernel[3][1] - kKernel.m_akKernel[1][1], float(uiY) / (float)m_uiMaxTesselation),
		sdMath::Lerp(kKernel.m_akKernel[2][2] - kKernel.m_akKernel[0][2], 
				kKernel.m_akKernel[3][2] - kKernel.m_akKernel[1][2], float(uiY) / (float)m_uiMaxTesselation),
		float(uiX) / (float)m_uiMaxTesselation);	
}
//-------------------------------------------------------------------------------------------------
float sdCubicSampler::Cubic(float P0, float P1, float P2 , float P3, uint I) const
{
	return P0 * m_akCubicBasis[I][0] +
		P1 * m_akCubicBasis[I][1] +
		P2 * m_akCubicBasis[I][2] +
		P3 * m_akCubicBasis[I][3];
}
//-------------------------------------------------------------------------------------------------
float sdCubicSampler::CubicDeriv(float P0, float P1, float P2 , float P3, uint I) const
{
	return P0 * m_akCubicBasisDeriv[I][0] +
		P1 * m_akCubicBasisDeriv[I][1] +
		P2 * m_akCubicBasisDeriv[I][2] +
		P3 * m_akCubicBasisDeriv[I][3];
}
//-------------------------------------------------------------------------------------------------