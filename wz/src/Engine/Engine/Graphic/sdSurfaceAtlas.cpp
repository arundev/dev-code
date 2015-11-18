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
	// 检查是否深度缓存的格式
	NIASSERT(!sdTextureUtility::IsZStencilFormat(eFormat));

	// 检查是否DDS格式
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
	// 检查待分配Tile尺寸是否为空
	if (uiWidth < 1 || uiHeight < 1)	
		return -1;

	// 如果当前是DDS格式,那么检查起始位置和尺寸是否是power(4,n)
	if (m_bIsDDSFormat)
	{
		if (uiLeft & 0x03 || uiTop & 0x03 || uiWidth & 0x03 || uiHeight & 0x03)
			return -1;
	}

	// 检查上边界是否越界
	uint uiRight = uiLeft + uiWidth;
	uint uiBottom = uiTop + uiHeight;
	if (uiRight > m_uiWidth || uiBottom > m_uiHeight)
		return -1;

	// 检查是否与已有Tile发生覆盖
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

	// 添加新加入的Tile到列表
	m_kAtlasTileVec.push_back(stAtlasTile(uiLeft, uiTop, uiWidth, uiHeight));

	return m_kAtlasTileVec.size()-1;
}
//-------------------------------------------------------------------------------------------------
bool sdSurfaceAtlas::DeallocTile(uint uiIndex)
{
	// 检查索引范围
	if (uiIndex >= m_kAtlasTileVec.size())
		return false;

	// 绑定Surface到Tile
	UnbindSourceSurfaceFromTile(uiIndex);

	// 从Tile列表删除
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
	// 参数检查
	// @{
	// 检查索引范围
	if (uiIndex >= m_kAtlasTileVec.size())
		return false;

	// 检查源Surface有效性
	if(NULL == d3dSurface)
		return false;

	// 检查Surface的属性(必须是内存纹理,必须与指定格式相同)
	D3DSURFACE_DESC kDesc;
	HRESULT hr = d3dSurface->GetDesc(&kDesc);
	if (D3D_OK != hr || D3DPOOL_SYSTEMMEM != kDesc.Pool || m_eFormat != kDesc.Format)
		return false;
	
	// 如果当前是DDS格式,那么检查起始位置是否是power(4,n)
	if (m_bIsDDSFormat)
	{
		if (uiLeft & 0x03 || uiTop & 0x03)
			return false;
	} 

	// 检查输入Surface上边界是否越界
	stAtlasTile& kAtlasTile = m_kAtlasTileVec[uiIndex];
	uint uiRight = uiLeft + kAtlasTile.uiWidth - 1;
	uint uiBottom = uiTop + kAtlasTile.uiHeight - 1;
	if (uiRight >= kDesc.Width || uiBottom >= kDesc.Height)
		return false;
	// @}

	// 待绑定Tile上已有Source与当前输入相同
	stAtlasTileSurface& kOldAtlasTileSource = kAtlasTile.kSource;
	if (kOldAtlasTileSource.d3dSurface == d3dSurface && kOldAtlasTileSource.uiLeft == uiLeft &&	kOldAtlasTileSource.uiTop == uiTop)
		return true;

	// 绑定新的Surface到当前Tile
	// @{
	if (kOldAtlasTileSource.d3dSurface)
		kOldAtlasTileSource.d3dSurface->Release();	///< 释放旧的Surface

	if (d3dSurface)
		d3dSurface->AddRef();						///< 引用新的Surface

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
	// 参数检查
	// @{
	// 检查索引范围
	if (uiIndex >= m_kAtlasTileVec.size())
		return;
	// @}

	// 解绑定
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
	// 参数检查
	// @{
	// 检查TargetSurface有效性
	if(NULL == d3dSurface)
		return false;

	// 检查TargetSurface的属性(必须是内存纹理,必须与指定格式相同,必须与指定大小相同)
	D3DSURFACE_DESC kDesc;
	HRESULT hr = d3dSurface->GetDesc(&kDesc);
	if (D3D_OK != hr || m_eFormat != kDesc.Format || kDesc.Width != m_uiWidth || kDesc.Height != m_uiHeight)
		return false;
	// @}

	// 绑定到TargetSurface
	// @{
	if (m_d3dTargetSurface)
	{
		m_d3dTargetSurface->Release();	///< 释放旧的Surface
		m_d3dTargetSurface = NULL;
	}

	m_d3dTargetSurface = d3dSurface;		
	m_d3dTargetSurface->AddRef();		///< 引用新的Surface

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
	// 确定TargetSurface非空
	if (NULL == m_d3dTargetSurface)
		return false;

	// 锁定TargetSurface
	D3DLOCKED_RECT kLockedDstRect;
	HRESULT hr = m_d3dTargetSurface->LockRect(&kLockedDstRect, NULL, 0);
	if (FAILED(hr))
		return false;

	// 遍历Tile
	AtlasTileVecItr itr = m_kAtlasTileVec.begin();
	AtlasTileVecItr itr_end = m_kAtlasTileVec.end();
	for (; itr != itr_end; ++itr)
	{
		stAtlasTile& kAtlasTile = (*itr);
		stAtlasTileSurface& kAtlasTileSurface = kAtlasTile.kSource;

		if (NULL == kAtlasTileSurface.d3dSurface) continue;
		if (!kAtlasTile.bDirty)	continue;

		// 源Surface区域
		RECT kSrcRect;
		kSrcRect.left	= kAtlasTileSurface.uiLeft;
		kSrcRect.top	= kAtlasTileSurface.uiTop;
		kSrcRect.right	= kAtlasTileSurface.uiLeft + kAtlasTile.uiWidth;
		kSrcRect.bottom	= kAtlasTileSurface.uiTop + kAtlasTile.uiHeight;

		// 目标Surface区域
		RECT kDstRect;
		kDstRect.left	= kAtlasTile.uiLeft;
		kDstRect.top	= kAtlasTile.uiTop;
		kDstRect.right	= kAtlasTile.uiLeft + kAtlasTile.uiWidth;
		kDstRect.bottom	= kAtlasTile.uiTop + kAtlasTile.uiHeight;
		
		// 锁定源数据
		D3DLOCKED_RECT kLockedSrcRect;
		HRESULT hr = kAtlasTileSurface.d3dSurface->LockRect(&kLockedSrcRect, &kSrcRect, D3DLOCK_READONLY);
		if (FAILED(hr))
			continue;

		// 数据拷贝
		// (这里十分不明白为什么是4x4块的拷贝,难道必定是dds)
		// (猜想:dds每个4x4块的数据是在一起)
		uchar* pucSrcData = (uchar*)kLockedSrcRect.pBits;
		uchar* pucDstData = (uchar*)kLockedDstRect.pBits;
		pucDstData += kDstRect.top / 4 * kLockedDstRect.Pitch + kDstRect.left / 4 * (4 * 4);
		for (uint y = 0; y < kAtlasTile.uiHeight / 4; ++y)
		{
			// 每次拷贝4行的块数据
			memcpy(pucDstData , pucSrcData, kAtlasTile.uiWidth / 4 * (4 * 4));

			// 下一组
			pucSrcData += kLockedSrcRect.Pitch;
			pucDstData += kLockedDstRect.Pitch;
		}

		// 解锁源数据
		kAtlasTileSurface.d3dSurface->UnlockRect();

		kAtlasTile.bDirty = false;
	}

	// 解锁TargetSurface
	m_d3dTargetSurface->UnlockRect();

	return true;
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE