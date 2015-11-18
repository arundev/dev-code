#include "sdEnginePCH.h"
#include "sdNormalMap.h"
#include "sdHeightMap.h"
#include "sdDynamicTexture.h"

using namespace Engine;
//-------------------------------------------------------------------------------------------------
sdNormalMap::sdNormalMap(uint uiSize, sdHeightMap* pkHeightMap)
{
	NIASSERT(pkHeightMap);
	m_pkHeightMap = pkHeightMap;

	m_pkDynamicNormalTexture = NiNew sdDynamicTexture(uiSize, uiSize, sdDynamicTexture::E_FORMAT_A8R8G8B8);
	NIASSERT(m_pkDynamicNormalTexture);

	// 预计算的坡度阈值(65度天顶角,大致是个四棱锥)
	// Only use vertical uv when normal angle is greater than 65 degree
	m_kPlanarThreshold.z = cos(65.f / 180.0f * NI_PI);
	m_kPlanarThreshold.x = sin(65.f / 180.0f * NI_PI) / sqrt(2.0f);
	m_kPlanarThreshold.y = m_kPlanarThreshold.x;
}
//-------------------------------------------------------------------------------------------------
sdNormalMap::sdNormalMap(NiTexture* spNormalTexture)
: m_pkHeightMap(NULL)
{
	NIASSERT(spNormalTexture);
	m_spNormalTexture = spNormalTexture;
}
//-------------------------------------------------------------------------------------------------
sdNormalMap::~sdNormalMap()
{
	m_pkHeightMap = NULL;
	m_spNormalTexture = 0;
	m_pkDynamicNormalTexture = 0;
}
//-------------------------------------------------------------------------------------------------
NiTexture* sdNormalMap::GetGBTexture() const
{
	if (m_spNormalTexture)				
		return m_spNormalTexture;
	else
		return m_pkDynamicNormalTexture->GetGBTexture();
}
//-------------------------------------------------------------------------------------------------
void sdNormalMap::Update()
{
	NIASSERT(m_pkDynamicNormalTexture);

	uint uiSize = m_pkDynamicNormalTexture->GetWidth();
	Update(0, 0, (int)uiSize, (int)uiSize);
}
//-------------------------------------------------------------------------------------------------
void sdNormalMap::Update(int iX, int iY, int iW, int iH)
{
	NIASSERT(m_pkHeightMap);
	NIASSERT(m_pkDynamicNormalTexture);

	// 计算有效更新范围
	uint uiSize = m_pkDynamicNormalTexture->GetWidth();
	if (iX < 0)	return;
	if (iY < 0)	return;
	if (iW < 0)	return;
	if (iH < 0)	return;
	if (iX >= (int)uiSize)	return;
	if (iY >= (int)uiSize)	return;

	uint uiStartX = (uint)iX;
	uint uiStartY = (uint)iY;
	uint uiWidth = (uint)iW;
	uint uiHeight = (uint)iH;
	uint uiEndX = min(uiSize, uiStartX + uiWidth);
	uint uiEndY = min(uiSize, uiStartY + uiHeight);

	// 锁定目标纹理
	sdDynamicTexture::sdLockedRect kLockedRect = m_pkDynamicNormalTexture->LockRegion(uiStartX, uiStartY, uiWidth, uiHeight);
	if (!kLockedRect.pBits)
		return;

	// 获取法线并填充
	float fNX = 0.0f, fNY = 0.0f, fNZ = 0.0f;
	float fWX = 0.0f, fWY = 0.0f, fWZ = 0.0f;
	uchar ucNX = 0, ucNY = 0;
	uchar ucWX = 0, ucWY = 0;
	uint  uiPitch = kLockedRect.uiPitch / 4;
	uint* puiCurLine = (uint*)kLockedRect.pBits;
	for (uint uiRow = uiStartY; uiRow < uiEndY; ++uiRow)
	{
		uint* puiCurData = puiCurLine;
		for (uint uiCol = uiStartX; uiCol < uiEndX; ++uiCol)
		{
			// 获取法线
			m_pkHeightMap->GetNormal(uiCol, uiRow, fNX, fNY, fNZ);

			// 压缩法线([-1,1]到[0,255])
			ucNX = (uchar)((fNX * 0.5f + 0.5f) * 255.0f);
			ucNY = (uchar)((fNY * 0.5f + 0.5f) * 255.0f);

			// 计算倾斜权值(不解为何这样计算)
			fWX = max(pow((fabs(fNX) - m_kPlanarThreshold.x) * 7.0f, 3.0f), 0);
			fWY = max(pow((fabs(fNY) - m_kPlanarThreshold.y) * 7.0f, 3.0f), 0);
			fWZ = max(pow((fabs(fNZ) - m_kPlanarThreshold.z) * 7.0f, 3.0f), 0);

			// 归一化倾斜权值
			float fTotalW = fWX + fWY + fWZ;
			fWX /= fTotalW;
			fWY /= fTotalW;

			// 压缩倾斜权值([0, 1]到[0, 255])
			fWX = floor(fWX * 255.f + 0.5f);
			fWY = floor(fWY * 255.f + 0.5f);
			if (fWX + fWY > 255.f)	///< 应该不会被执行到，前面已经被归一化了
			{
				// WZ做法是(fWX/fWY均不大于255,所以不会有问题,但是不知道这么做的原因) 
				//fWY = 255.f - fWX;
				//
				// 我的做法(按比例缩减)
				float fTotal = fWX + fWY;
				fWX *= 255.f / fTotal;
				fWY *= 255.f / fTotal;
			}

			ucWX = (uchar)fWX;
			ucWY = (uchar)fWY;
		
			// 填充到数据(注意纹理格式是ARGB)
			//					R,X			G,Y			B,Y			A,W
			*puiCurData = (ucNX << 16) | (ucNY << 8) | (ucWX) | (ucWY << 24);

			// 下一个
			++puiCurData;
		}

		// 下一行
		puiCurLine += uiPitch;
	}

	// 解锁目标纹理
	m_pkDynamicNormalTexture->UnlockRegion();
}
//-------------------------------------------------------------------------------------------------