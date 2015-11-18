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

	// Ԥ������¶���ֵ(65���춥��,�����Ǹ�����׶)
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

	// ������Ч���·�Χ
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

	// ����Ŀ������
	sdDynamicTexture::sdLockedRect kLockedRect = m_pkDynamicNormalTexture->LockRegion(uiStartX, uiStartY, uiWidth, uiHeight);
	if (!kLockedRect.pBits)
		return;

	// ��ȡ���߲����
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
			// ��ȡ����
			m_pkHeightMap->GetNormal(uiCol, uiRow, fNX, fNY, fNZ);

			// ѹ������([-1,1]��[0,255])
			ucNX = (uchar)((fNX * 0.5f + 0.5f) * 255.0f);
			ucNY = (uchar)((fNY * 0.5f + 0.5f) * 255.0f);

			// ������бȨֵ(����Ϊ����������)
			fWX = max(pow((fabs(fNX) - m_kPlanarThreshold.x) * 7.0f, 3.0f), 0);
			fWY = max(pow((fabs(fNY) - m_kPlanarThreshold.y) * 7.0f, 3.0f), 0);
			fWZ = max(pow((fabs(fNZ) - m_kPlanarThreshold.z) * 7.0f, 3.0f), 0);

			// ��һ����бȨֵ
			float fTotalW = fWX + fWY + fWZ;
			fWX /= fTotalW;
			fWY /= fTotalW;

			// ѹ����бȨֵ([0, 1]��[0, 255])
			fWX = floor(fWX * 255.f + 0.5f);
			fWY = floor(fWY * 255.f + 0.5f);
			if (fWX + fWY > 255.f)	///< Ӧ�ò��ᱻִ�е���ǰ���Ѿ�����һ����
			{
				// WZ������(fWX/fWY��������255,���Բ���������,���ǲ�֪����ô����ԭ��) 
				//fWY = 255.f - fWX;
				//
				// �ҵ�����(����������)
				float fTotal = fWX + fWY;
				fWX *= 255.f / fTotal;
				fWY *= 255.f / fTotal;
			}

			ucWX = (uchar)fWX;
			ucWY = (uchar)fWY;
		
			// ��䵽����(ע�������ʽ��ARGB)
			//					R,X			G,Y			B,Y			A,W
			*puiCurData = (ucNX << 16) | (ucNY << 8) | (ucWX) | (ucWY << 24);

			// ��һ��
			++puiCurData;
		}

		// ��һ��
		puiCurLine += uiPitch;
	}

	// ����Ŀ������
	m_pkDynamicNormalTexture->UnlockRegion();
}
//-------------------------------------------------------------------------------------------------