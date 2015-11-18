#include "sdEnginePCH.h"
#include "sdSurfaceAtlas.h"
#include "sdTextureUtility.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
//-------------------------------------------------------------------------------------------------
sdSurfaceAtlas::sdSurfaceAtlas(uint uiWidth, uint uiHeight, D3DFORMAT eFormat)
: m_uiWidth(uiWidth)
, m_uiHeight(uiHeight)
, m_eFormat(eFormat)
, m_d3dTargetSurface(NULL)
{
	// ����Ƿ���Ȼ���ĸ�ʽ
	NIASSERT(!sdTextureUtility::IsZStencilFormat(eFormat));

	// ����Ƿ�DDS��ʽ
	m_bIsDDSFormat = sdTextureUtility::IsDDSFormat(eFormat);
}
//-------------------------------------------------------------------------------------------------
sdSurfaceAtlas::~sdSurfaceAtlas()
{
	DeallocAllTiles();
	UnbindTargetSurface();
}
//-------------------------------------------------------------------------------------------------
uint sdSurfaceAtlas::AllocTile(uint uiLeft, uint uiTop, uint uiWidth, uint uiHeight)
{
	// ��������Tile�ߴ��Ƿ�Ϊ��
	if (uiWidth < 1 || uiHeight < 1)	
		return -1;

	// �����ǰ��DDS��ʽ,��ô�����ʼλ�úͳߴ��Ƿ���power(4,n)
	if (m_bIsDDSFormat)
	{
		if (uiLeft & 0x03 || uiTop & 0x03 || uiWidth & 0x03 || uiHeight & 0x03)
			return -1;
	}

	// ����ϱ߽��Ƿ�Խ��
	uint uiRight = uiLeft + uiWidth;
	uint uiBottom = uiTop + uiHeight;
	if (uiRight > m_uiWidth || uiBottom > m_uiHeight)
		return -1;

	// ����Ƿ�������Tile��������
	AtlasTileVecItr itr = m_kAtlasTileVec.begin();
	AtlasTileVecItr itr_end = m_kAtlasTileVec.end();
	for (; itr != itr_end; ++itr)
	{
		const stAtlasTile& kAtlasTile = *itr;
		if (kAtlasTile.uiLeft >= uiRight || (kAtlasTile.uiLeft + kAtlasTile.uiWidth) <= uiLeft ||
			kAtlasTile.uiTop >= uiBottom || (kAtlasTile.uiTop + kAtlasTile.uiHeight) <= uiTop)
		{
			continue;
		}

		break;
	}

	if (itr != itr_end)
		return -1;

	// ����¼����Tile���б�
	m_kAtlasTileVec.push_back(stAtlasTile(uiLeft, uiTop, uiWidth, uiHeight));

	return m_kAtlasTileVec.size()-1;
}
//-------------------------------------------------------------------------------------------------
bool sdSurfaceAtlas::DeallocTile(uint uiIndex)
{
	// ���������Χ
	if (uiIndex >= m_kAtlasTileVec.size())
		return false;

	// ��Surface��Tile
	UnbindSourceSurfaceFromTile(uiIndex);

	// ��Tile�б�ɾ��
	m_kAtlasTileVec.erase(m_kAtlasTileVec.begin() + uiIndex);

	return true;
}
//-------------------------------------------------------------------------------------------------
void sdSurfaceAtlas::DeallocAllTiles()
{
	AtlasTileVecItr itr = m_kAtlasTileVec.begin();
	AtlasTileVecItr itr_end = m_kAtlasTileVec.end();
	for (uint uiIndex = 0; itr != itr_end; ++itr, ++uiIndex)
	{
		UnbindSourceSurfaceFromTile(uiIndex);
	}

	m_kAtlasTileVec.clear();
}
//-------------------------------------------------------------------------------------------------
bool sdSurfaceAtlas::BindSourceSurfaceToTile(uint uiIndex, IDirect3DSurface9* d3dSurface, uint uiLeft, uint uiTop)
{
	// �������
	// @{
	// ���������Χ
	if (uiIndex >= m_kAtlasTileVec.size())
		return false;

	// ���ԴSurface��Ч��
	if(NULL == d3dSurface)
		return false;

	// ���Surface������(�������ڴ�����,������ָ����ʽ��ͬ)
	D3DSURFACE_DESC kDesc;
	HRESULT hr = d3dSurface->GetDesc(&kDesc);
	if (D3D_OK != hr || D3DPOOL_SYSTEMMEM != kDesc.Pool || m_eFormat != kDesc.Format)
		return false;
	
	// �����ǰ��DDS��ʽ,��ô�����ʼλ���Ƿ���power(4,n)
	if (m_bIsDDSFormat)
	{
		if (uiLeft & 0x03 || uiTop & 0x03)
			return false;
	} 

	// �������Surface�ϱ߽��Ƿ�Խ��
	stAtlasTile& kAtlasTile = m_kAtlasTileVec[uiIndex];
	uint uiRight = uiLeft + kAtlasTile.uiWidth - 1;
	uint uiBottom = uiTop + kAtlasTile.uiHeight - 1;
	if (uiRight >= kDesc.Width || uiBottom >= kDesc.Height)
		return false;
	// @}

	// ����Tile������Source�뵱ǰ������ͬ
	stAtlasTileSurface& kOldAtlasTileSource = kAtlasTile.kSource;
	if (kOldAtlasTileSource.d3dSurface == d3dSurface && kOldAtlasTileSource.uiLeft == uiLeft &&	kOldAtlasTileSource.uiTop == uiTop)
		return true;

	// ���µ�Surface����ǰTile
	// @{
	if (kOldAtlasTileSource.d3dSurface)
		kOldAtlasTileSource.d3dSurface->Release();	///< �ͷžɵ�Surface

	if (d3dSurface)
		d3dSurface->AddRef();						///< �����µ�Surface

	kAtlasTile.kSource.d3dSurface = d3dSurface;
	kAtlasTile.kSource.uiLeft = uiLeft;
	kAtlasTile.kSource.uiTop = uiTop;

	kAtlasTile.bDirty = true;
	// @}

	return true;
}
//-------------------------------------------------------------------------------------------------
void sdSurfaceAtlas::UnbindSourceSurfaceFromTile(uint uiIndex)
{
	// �������
	// @{
	// ���������Χ
	if (uiIndex >= m_kAtlasTileVec.size())
		return;
	// @}

	// ���
	// @{
	stAtlasTile& kAtlasTile = m_kAtlasTileVec[uiIndex];
	stAtlasTileSurface& kAtlasTileSource = kAtlasTile.kSource;
	if (kAtlasTileSource.d3dSurface)
		kAtlasTileSource.d3dSurface->Release();
	kAtlasTileSource.Reset();

	kAtlasTile.bDirty = true;
	// @}
}
//-------------------------------------------------------------------------------------------------
bool sdSurfaceAtlas::BindTargetSurface(IDirect3DSurface9* d3dSurface)
{
	// �������
	// @{
	// ���TargetSurface��Ч��
	if(NULL == d3dSurface)
		return false;

	// ���TargetSurface������(�������ڴ�����,������ָ����ʽ��ͬ,������ָ����С��ͬ)
	D3DSURFACE_DESC kDesc;
	HRESULT hr = d3dSurface->GetDesc(&kDesc);
	if (D3D_OK != hr || m_eFormat != kDesc.Format || kDesc.Width != m_uiWidth || kDesc.Height != m_uiHeight)
		return false;
	// @}

	// �󶨵�TargetSurface
	// @{
	if (m_d3dTargetSurface)
	{
		m_d3dTargetSurface->Release();	///< �ͷžɵ�Surface
		m_d3dTargetSurface = NULL;
	}

	m_d3dTargetSurface = d3dSurface;		
	m_d3dTargetSurface->AddRef();		///< �����µ�Surface

	AtlasTileVecItr itr = m_kAtlasTileVec.begin();
	AtlasTileVecItr itr_end = m_kAtlasTileVec.end();
	for (; itr != itr_end; ++itr)
	{
		stAtlasTile& kAtlasTile = (*itr);
		stAtlasTileSurface& kAtlasTileSurface = kAtlasTile.kSource;
		if (NULL == kAtlasTileSurface.d3dSurface)
			continue;

		kAtlasTile.bDirty = true;
	}
	// @}

	return true;
}
//-------------------------------------------------------------------------------------------------
void sdSurfaceAtlas::UnbindTargetSurface()
{
	if (m_d3dTargetSurface)
	{
		m_d3dTargetSurface->Release();
		m_d3dTargetSurface = NULL;
	}
}
//-------------------------------------------------------------------------------------------------
bool sdSurfaceAtlas::UpdateTargetSurface()
{
	// ȷ��TargetSurface�ǿ�
	if (NULL == m_d3dTargetSurface)
		return false;

	// ����TargetSurface
	D3DLOCKED_RECT kLockedDstRect;
	HRESULT hr = m_d3dTargetSurface->LockRect(&kLockedDstRect, NULL, 0);
	if (FAILED(hr))
		return false;

	// ����Tile
	AtlasTileVecItr itr = m_kAtlasTileVec.begin();
	AtlasTileVecItr itr_end = m_kAtlasTileVec.end();
	for (; itr != itr_end; ++itr)
	{
		stAtlasTile& kAtlasTile = (*itr);
		stAtlasTileSurface& kAtlasTileSurface = kAtlasTile.kSource;

		if (NULL == kAtlasTileSurface.d3dSurface) continue;
		if (!kAtlasTile.bDirty)	continue;

		// ԴSurface����
		RECT kSrcRect;
		kSrcRect.left	= kAtlasTileSurface.uiLeft;
		kSrcRect.top	= kAtlasTileSurface.uiTop;
		kSrcRect.right	= kAtlasTileSurface.uiLeft + kAtlasTile.uiWidth;
		kSrcRect.bottom	= kAtlasTileSurface.uiTop + kAtlasTile.uiHeight;

		// Ŀ��Surface����
		RECT kDstRect;
		kDstRect.left	= kAtlasTile.uiLeft;
		kDstRect.top	= kAtlasTile.uiTop;
		kDstRect.right	= kAtlasTile.uiLeft + kAtlasTile.uiWidth;
		kDstRect.bottom	= kAtlasTile.uiTop + kAtlasTile.uiHeight;
		
		// ����Դ����
		D3DLOCKED_RECT kLockedSrcRect;
		HRESULT hr = kAtlasTileSurface.d3dSurface->LockRect(&kLockedSrcRect, &kSrcRect, D3DLOCK_READONLY);
		if (FAILED(hr))
			continue;

		// ���ݿ���
		// (����ʮ�ֲ�����Ϊʲô��4x4��Ŀ���,�ѵ��ض���dds)
		// (����:ddsÿ��4x4�����������һ��)
		uchar* pucSrcData = (uchar*)kLockedSrcRect.pBits;
		uchar* pucDstData = (uchar*)kLockedDstRect.pBits;
		pucDstData += kDstRect.top / 4 * kLockedDstRect.Pitch + kDstRect.left / 4 * (4 * 4);
		for (uint y = 0; y < kAtlasTile.uiHeight / 4; ++y)
		{
			// ÿ�ο���4�еĿ�����
			memcpy(pucDstData , pucSrcData, kAtlasTile.uiWidth / 4 * (4 * 4));

			// ��һ��
			pucSrcData += kLockedSrcRect.Pitch;
			pucDstData += kLockedDstRect.Pitch;
		}

		// ����Դ����
		kAtlasTileSurface.d3dSurface->UnlockRect();

		kAtlasTile.bDirty = false;
	}

	// ����TargetSurface
	m_d3dTargetSurface->UnlockRect();

	return true;
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE