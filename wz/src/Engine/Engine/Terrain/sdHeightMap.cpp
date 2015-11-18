#include "sdEnginePCH.h"
#include "sdHeightMap.h"

sdCubicSampler sdHeightMap::ms_kCubicSampler;
//-------------------------------------------------------------------------------------------------
sdHeightMap::sdHeightMap(uint uiSize)
: m_pfHeight(NULL)
, m_uiSpacing(1u)
, m_fSpacingDiv(1.f / 1u)
, m_uiSize(uiSize)
, m_uiAllocSize(uiSize + 1)
, m_fMinHeight(0.0f)
, m_fMaxHeight(0.0f)
, m_fAlignHeight(0.05f)
{
	// 检查尺寸
	NIASSERT(efd::IsPowerOf2(uiSize));

	// 创建高度图数据
	m_pfHeight = new float[m_uiAllocSize * m_uiAllocSize];
	NIASSERT(m_pfHeight);
	memset(m_pfHeight, 0, m_uiAllocSize * m_uiAllocSize * sizeof(float));
}
//-------------------------------------------------------------------------------------------------
sdHeightMap::sdHeightMap(uint uiSize, float fMinHeight, float fMaxHeight)
: m_pfHeight(NULL)
, m_uiSpacing(1u)
, m_fSpacingDiv(1.f / 1u)
, m_uiSize(uiSize)
, m_uiAllocSize(uiSize + 1)
, m_fMinHeight(fMinHeight)
, m_fMaxHeight(fMaxHeight)
, m_fAlignHeight(0.05f)
{
	// 检查尺寸
	NIASSERT(efd::IsPowerOf2(uiSize));

	// 检查最小最大高度
	NIASSERT(fMinHeight <= fMaxHeight);

	// 创建高度图数据
	m_pfHeight = new float[m_uiAllocSize * m_uiAllocSize];
	NIASSERT(m_pfHeight);
	memset(m_pfHeight, 0, m_uiAllocSize * m_uiAllocSize * sizeof(float));
}
//-------------------------------------------------------------------------------------------------
sdHeightMap::~sdHeightMap()
{
	if (m_pfHeight)
	{
		delete m_pfHeight;
		m_pfHeight = NULL;
	}
}
//-------------------------------------------------------------------------------------------------
void sdHeightMap::SetRawHeight(int iX, int iY, float fHeight)
{
	// 溢出则不设置
	if (iX < 0)	return;
	if (iY < 0) return;
	if (iX > (int)m_uiSize)	return;
	if (iY > (int)m_uiSize)	return;

	// 检查并更新高度范围
	const static float fMaxHeightRange = 10000.0f;
	if (fHeight < m_fMinHeight)
	{
		if (m_fMaxHeight - fHeight < fMaxHeightRange)	
			m_fMinHeight = fHeight;
		else 
			return;
	}
	else if (fHeight > m_fMaxHeight)
	{
		if (fHeight - m_fMinHeight < fMaxHeightRange)	
			m_fMaxHeight = fHeight;
		else 
			return;
	}

	// 高度对齐(四舍五入)
	NIASSERT(m_fAlignHeight > 0.0f);
	fHeight = floor(fHeight / m_fAlignHeight + 0.5f) * m_fAlignHeight;

	// 高度保存
	m_pfHeight[iY * m_uiAllocSize + iX] = fHeight;
}
//-------------------------------------------------------------------------------------------------
float sdHeightMap::GetRawHeight(int iX, int iY)
{
	// 溢出则返回对应边缘部分的值
	if (iX < 0)	iX = 0;
	if (iY < 0) iY = 0;
	if (iX > (int)m_uiSize)	iX = (int)m_uiSize;
	if (iY > (int)m_uiSize)	iY = (int)m_uiSize;
	return m_pfHeight[iY * m_uiAllocSize + iX];
}
//-------------------------------------------------------------------------------------------------
float sdHeightMap::GetHeight(int iX, int iY)
{
	//
	// v30---v31---v32---v33
	//	|     |     |     | 
	//	|     |     |	  |
	//	| 	  | 	|	  |
	// v20---v21---v22---v23
	//	|     |     |	  |
	//	|     |     |	  |
	//	| 	  |     |	  |
	// v10---v11---v12---v13
	//	|     |     |	  |
	//	|     |     |	  |
	//	| 	  |     |	  |
	// v00---v01---v02---v03
	//
	if (m_uiSpacing == 1)
		return GetRawHeight(iX, iY);

	// 限制范围到[0, m_uiSize * m_uiSpacing]
	int iRealSize = (int)(m_uiSize * m_uiSpacing);
	if (iX < 0)	iX = 0;
	if (iY < 0) iY = 0;
	if (iX > iRealSize)	iX = iRealSize;
	if (iY > iRealSize)	iY = iRealSize;

	// 计算所处采样点[0, m_uiSize]和余数[0, m_uiSpacing)
	int iGridX = (int)(iX * m_fSpacingDiv);
	int iGridY = (int)(iY * m_fSpacingDiv);
	int iGridDeltaX = iX - iGridX * (int)m_uiSpacing;
	int iGridDeltaY = iY - iGridY * (int)m_uiSpacing;

	// 采样指定点周围的4x4采样点 
	sdCubicSamplerKernel kKernel;
	kKernel.m_akKernel[0][0] = GetRawHeight(iGridX - 1	, iGridY - 1);
	kKernel.m_akKernel[0][1] = GetRawHeight(iGridX		, iGridY - 1);
	kKernel.m_akKernel[0][0] = GetRawHeight(iGridX + 2	, iGridY - 1);
	kKernel.m_akKernel[0][0] = GetRawHeight(iGridX + 3	, iGridY - 1);

	kKernel.m_akKernel[1][0] = GetRawHeight(iGridX - 1	, iGridY);
	kKernel.m_akKernel[1][1] = GetRawHeight(iGridX		, iGridY);
	kKernel.m_akKernel[1][0] = GetRawHeight(iGridX + 2	, iGridY);
	kKernel.m_akKernel[1][0] = GetRawHeight(iGridX + 3	, iGridY);

	kKernel.m_akKernel[2][0] = GetRawHeight(iGridX - 1	, iGridY + 1);
	kKernel.m_akKernel[2][1] = GetRawHeight(iGridX		, iGridY + 1);
	kKernel.m_akKernel[2][0] = GetRawHeight(iGridX + 2	, iGridY + 1);
	kKernel.m_akKernel[2][0] = GetRawHeight(iGridX + 3	, iGridY + 1);

	kKernel.m_akKernel[3][0] = GetRawHeight(iGridX - 1	, iGridY + 2);
	kKernel.m_akKernel[3][1] = GetRawHeight(iGridX		, iGridY + 2);
	kKernel.m_akKernel[3][0] = GetRawHeight(iGridX + 2	, iGridY + 2);
	kKernel.m_akKernel[3][0] = GetRawHeight(iGridX + 3	, iGridY + 2);

	// 归一化系数，[0, m_iSpacing)转换到[0, MAX_TERRAIN_TESSELATION)
	int iStep = MAX_TERRAIN_TESSELATION / m_uiSpacing;	

	// 取样高度
	float fHeight = ms_kCubicSampler.Sample(kKernel, iGridDeltaX * iStep, iGridDeltaY * iStep);

	return fHeight;
}
//-------------------------------------------------------------------------------------------------
float sdHeightMap::GetHeight(float fX, float fY)
{
	//
	// v10---v11
	//	| \   |
	//	|  \  |
	//	| 	\ |
	// v00---v01
	//
	// 如上图, 
	//	1.向右为X+，向上为Y+
	//	2.pt(fX,fY)落入矩形rect(v00-v01-v10-v11)内部
	//
	// 限制范围到[0, m_uiSize * m_uiSpacing]
	float fRealSize = (float)(m_uiSize * m_uiSpacing);
	if (fX < 0)	fX = 0;
	if (fY < 0) fY = 0;
	if (fX > fRealSize)	fX = fRealSize;
	if (fY > fRealSize)	fY = fRealSize;

	// 
	int iGridStartX = (int)floor(fX);
	int iGridStartY = (int)floor(fY);
	float fGridDeltaX = fX - iGridStartX;
	float fGridDeltaY = fY - iGridStartY;
	if (fGridDeltaX + fGridDeltaY <= 1.0f)
	{
		// 下三角tri(v00-v10-v11)
		//
		// 三角形三点高度
		float fV00 = GetHeight(iGridStartX,		iGridStartY);
		float fV01 = GetHeight(iGridStartX + 1,	iGridStartY);
		float fV10 = GetHeight(iGridStartX,		iGridStartY + 1);

		// Y方向插值
		float fH1 = fV00 + (fV10 - fV00) * fGridDeltaY;
		float fH2 = fV01 + (fV10 - fV01) * fGridDeltaY;

		// X方向插值
		return fH1 + (fH2 - fH1) * (fGridDeltaX / (1.0f - fGridDeltaY));
	}
	else
	{
		// 上三角tri(v01-v10-v11)
		//
		// 三角形三点高度
		float fV01 = GetHeight(iGridStartX + 1,	iGridStartY);
		float fV10 = GetHeight(iGridStartX,		iGridStartY + 1);
		float fV11 = GetHeight(iGridStartX + 1,	iGridStartY + 1);

		// Y方向插值
		float fH1 = fV01 + (fV10 - fV01) * fGridDeltaY;
		float fH2 = fV01 + (fV11 - fV01) * fGridDeltaY;

		// X方向插值
		// 原式:	(fGridDeltaX - (1.0f - fGridDeltaY)) / 1.0f - (1.0f - fGridDeltaY)
		// 简化:	(fGridDeltaX + fGridDeltaY - 1.0f) / fGridDeltaY
		return fH1 + (fH2 - fH1) * ((fGridDeltaX + fGridDeltaY - 1.0f) / fGridDeltaY);
	}
}
//-------------------------------------------------------------------------------------------------
float sdHeightMap::GetHeight_Bilinear(float fX, float fY)
{
	//
	// v10---v11
	//	|     |
	//	|     |
	//	| 	  |
	// v00---v01
	//
	// 如上图, 
	//	1.向右为X+，向上为Y+
	//	2.pt(fX,fY)落入矩形rect(v00-v01-v10-v11)内部
	//
	// 限制范围到[0, m_uiSize * m_uiSpacing]
	float fRealSize = (float)(m_uiSize * m_uiSpacing);
	if (fX < 0)	fX = 0;
	if (fY < 0) fY = 0;
	if (fX > fRealSize)	fX = fRealSize;
	if (fY > fRealSize)	fY = fRealSize;

	// 
	int iGridStartX = (int)floor(fX);
	int iGridStartY = (int)floor(fY);
	float fGridDeltaX = fX - iGridStartX;
	float fGridDeltaY = fY - iGridStartY;

	// 矩形四角高度
	float fV00 = GetHeight(iGridStartX,		iGridStartY);
	float fV01 = GetHeight(iGridStartX + 1,	iGridStartY);
	float fV10 = GetHeight(iGridStartX,		iGridStartY + 1);
	float fV11 = GetHeight(iGridStartX + 1,	iGridStartY + 1);

	// Y方向插值
	float fH1 = fV00 + (fV10 - fV00) * fGridDeltaY;
	float fH2 = fV01 + (fV11 - fV01) * fGridDeltaY;

	// X方向插值
	return fH1 + (fH2 - fH1) * fGridDeltaX;
}
//-------------------------------------------------------------------------------------------------
void sdHeightMap::GetNormal(int iX, int iY, float& fNX, float& fNY, float& fNZ)
{
	//
	//	v6----v7---v8
	//	| \   | \  |
	//	|  \  |  \ |
	//	| 	\ |   \|
	//	v4----v----v5
	//	| \	  | \  |
	//	|  \  |  \ |
	//	|   \ |   \|
	//	v1---v2----v3
	//
	// 如上图, 
	//	1.向右为X+，向上为Y+
	//	2.目前仅仅采用了4邻接点的法线的平均值,即
	//		tri(v-v2-v5), tri(v-v5-v7), tri(v-v7-v4), tri(v-v4-v2)
	//
	// WZ原式如下, 没有减去fVHeight,不知原因(也许是最终会抵消掉)
	//	NiPoint3 kV2( 0, -1,	GetRawHeight(iX, iY-1));
	//	NiPoint3 kV5( 1,  0,	GetRawHeight(uiX+1,	uiY));
	//	NiPoint3 kV7( 0,  1,	GetRawHeight(uiX, uiY+1));
	//	NiPoint3 kV4(-1,  0,	GetRawHeight(iX-1, uiY));
	//	NiPoint3 kV = kV2.UnitCross(kV5) + kV5.UnitCross(kV7) + 
	//			kV7.UnitCross(kV4) + kV4.UnitCross(kV2);
	//	kV.Unitize();
	//
	float fVHeight = GetHeight(iX, iY);
	NiPoint3 kV2( 0, -1,	GetHeight(iX	, iY-1)	- fVHeight);
	NiPoint3 kV5( 1,  0,	GetHeight(iX+1	, iY)	- fVHeight);
	NiPoint3 kV7( 0,  1,	GetHeight(iX	, iY+1)	- fVHeight);
	NiPoint3 kV4(-1,  0,	GetHeight(iX-1	, iY)	- fVHeight);

	NiPoint3 kV = kV2.UnitCross(kV5) + kV5.UnitCross(kV7) + kV7.UnitCross(kV4) + kV4.UnitCross(kV2);
	kV.Unitize();

	fNX = kV.x;
	fNY = kV.y;
	fNZ = kV.z;
}
//-------------------------------------------------------------------------------------------------
void sdHeightMap::GetGradients(int iX, int iY, float& fGX, float& fGY)
{
	// 限制范围到[0, m_uiSize * m_uiSpacing]
	int iRealSize = (int)(m_uiSize * m_uiSpacing);
	if (iX < 0)	iX = 0;
	if (iY < 0) iY = 0;
	if (iX > iRealSize)	iX = iRealSize;
	if (iY > iRealSize)	iY = iRealSize;

	// 计算所处采样点[0, m_uiSize]和余数[0, m_uiSpacing]
	int iGridX = (int)(iX * m_fSpacingDiv);
	int iGridY = (int)(iY * m_fSpacingDiv);
	int iGridDeltaX = iX - iGridX * (int)m_uiSpacing;
	int iGridDeltaY = iY - iGridY * (int)m_uiSpacing;

	// 采样指定点周围的4x4采样点 
	sdCubicSamplerKernel kKernel;
	kKernel.m_akKernel[0][0] = GetRawHeight(iGridX - 1	, iGridY - 1);
	kKernel.m_akKernel[0][1] = GetRawHeight(iGridX		, iGridY - 1);
	kKernel.m_akKernel[0][0] = GetRawHeight(iGridX + 2	, iGridY - 1);
	kKernel.m_akKernel[0][0] = GetRawHeight(iGridX + 3	, iGridY - 1);

	kKernel.m_akKernel[1][0] = GetRawHeight(iGridX - 1	, iGridY);
	kKernel.m_akKernel[1][1] = GetRawHeight(iGridX		, iGridY);
	kKernel.m_akKernel[1][0] = GetRawHeight(iGridX + 2	, iGridY);
	kKernel.m_akKernel[1][0] = GetRawHeight(iGridX + 3	, iGridY);

	kKernel.m_akKernel[2][0] = GetRawHeight(iGridX - 1	, iGridY + 1);
	kKernel.m_akKernel[2][1] = GetRawHeight(iGridX		, iGridY + 1);
	kKernel.m_akKernel[2][0] = GetRawHeight(iGridX + 2	, iGridY + 1);
	kKernel.m_akKernel[2][0] = GetRawHeight(iGridX + 3	, iGridY + 1);

	kKernel.m_akKernel[3][0] = GetRawHeight(iGridX - 1	, iGridY + 2);
	kKernel.m_akKernel[3][1] = GetRawHeight(iGridX		, iGridY + 2);
	kKernel.m_akKernel[3][0] = GetRawHeight(iGridX + 2	, iGridY + 2);
	kKernel.m_akKernel[3][0] = GetRawHeight(iGridX + 3	, iGridY + 2);

	// 归一化系数，[0, m_iSpacing)转换到[0, MAX_TERRAIN_TESSELATION)
	int iStep = MAX_TERRAIN_TESSELATION / m_uiSpacing;	

	// 计算坡度
	fGX = ms_kCubicSampler.SampleDerivX(kKernel, iGridDeltaX * iStep, iGridDeltaY * iStep);
	fGY = ms_kCubicSampler.SampleDerivY(kKernel, iGridDeltaX * iStep, iGridDeltaY * iStep);
}
//-------------------------------------------------------------------------------------------------
float sdHeightMap::GetMorphDelta(int iStartX, int iStartY, int iX, int iY, uint uiSpacing)
{
	//
	// LOD采样示意:
	//	1.tt点为下一级粗糙LOD的采样点
	//	2.tf/ft/ff点为下一级粗糙LOD对精细LOD产生误差的点
	// tf ff tf ff tf ff
	// tt ft tt ft tt ft
	// tf ff tf ff tf ff
	// tt ft tt ft tt ft
	// tf ff tf ff tf ff
	// tt ft tt ft tt ft
	//
	// 判定采样点是否位于非变形点上
	bool bXSample = !(iX & 0x1);
	bool bYSample = !(iY & 0x1);

	// 采样点转换到世界坐标(注意这里没有限定范围)
	int iWorldX = iStartX + iX * (int)uiSpacing;
	int iWorldY = iStartY + iY * (int)uiSpacing;

	// 获取粗级LOD的可能高度
	float fLODHeight = 0.0f;
	if (bXSample)
	{
		if (bYSample)
		{
			// 位于tt点
			// 此时两级LOD采样同一个点,没有误差
			return 0.0f;
		}
		else
		{
			// 位于tf点(采样上下两个tt点,这里不会产生下溢)
			fLODHeight += GetHeight(iWorldX, (int)(iWorldY - uiSpacing));
			fLODHeight += GetHeight(iWorldX, (int)(iWorldY + uiSpacing));
			fLODHeight *= 0.5f;
		}
	}
	else
	{
		if (bYSample)
		{
			// 位于ft点(采样左右两个tt点,这里不会产生下溢)
			fLODHeight += GetHeight((int)(iWorldX - uiSpacing), iWorldY);
			fLODHeight += GetHeight((int)(iWorldX + uiSpacing), iWorldY);
			fLODHeight *= 0.5f;
		}
		else
		{
			// 位于ff点(采样四角四个tt点,这里不会产生下溢)
			fLODHeight += GetHeight((int)(iWorldX - uiSpacing), (int)(iWorldY - uiSpacing));
			fLODHeight += GetHeight((int)(iWorldX + uiSpacing), (int)(iWorldY + uiSpacing));
			fLODHeight += GetHeight((int)(iWorldX - uiSpacing), (int)(iWorldY + uiSpacing));
			fLODHeight += GetHeight((int)(iWorldX + uiSpacing), (int)(iWorldY - uiSpacing));
			fLODHeight *= 0.25f;
		}
	}

	// 计算偏差
	return fLODHeight - GetHeight(iWorldX, iWorldY);
}
//-------------------------------------------------------------------------------------------------