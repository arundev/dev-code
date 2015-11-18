#include "sdEnginePCH.h"
#include "sdDynamicTexture.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
//-------------------------------------------------------------------------------------------------
D3DFORMAT sdDynamicTexture::ms_kD3DFormatTable[NUM_FORMATS] = 
{
	D3DFMT_A8R8G8B8,
	D3DFMT_A8,
	D3DFMT_A4R4G4B4,
	D3DFMT_R8G8B8,
	D3DFMT_A8R8G8B8,
};
//-------------------------------------------------------------------------------------------------
sdDynamicTexture::sdDynamicTexture(uint uiWidth, uint uiHeight, eDynamicFormat eFormat)
: m_spRenderer(NULL)
, m_spD3DDevice(NULL)
, m_spD3DSysTexture(NULL)
, m_spD3DVidTexture(NULL)
{
	// 获取渲染设备
	m_spRenderer = NiDX9Renderer::GetRenderer();
	NIASSERT(m_spRenderer);

	m_spD3DDevice = m_spRenderer->GetD3DDevice();

	//
	m_uiWidth = uiWidth;
	m_uiHeight = uiHeight;
	m_uiLevel = 1;
	m_eFormat = eFormat;

	// 创建D3D内存纹理
	HRESULT hr = D3DXCreateTexture(m_spD3DDevice, 
		m_uiWidth, 
		m_uiHeight, 
		m_uiLevel, 
		0,
		ms_kD3DFormatTable[m_eFormat],
		D3DPOOL_SYSTEMMEM,
		&m_spD3DSysTexture);
	if (FAILED(hr))	
	{
		NIASSERT(0);
		return;
	}

	// 创建D3D显存纹理
	hr = D3DXCreateTexture(m_spD3DDevice, 
		m_uiWidth, 
		m_uiHeight, 
		m_uiLevel, 
		D3DUSAGE_DYNAMIC,
		ms_kD3DFormatTable[m_eFormat],
		D3DPOOL_DEFAULT,
		&m_spD3DVidTexture);
	if (FAILED(hr))	
	{
		NIASSERT(0);
		return;
	}

	// 创建GB纹理对象
	m_spTexture = m_spRenderer->CreateNiTextureFromD3DTexture(m_spD3DVidTexture);
	NIASSERT(m_spTexture);
}
//-------------------------------------------------------------------------------------------------
sdDynamicTexture::~sdDynamicTexture()
{
	if (m_spD3DVidTexture)
	{
		m_spD3DVidTexture->Release();
		m_spD3DVidTexture = NULL;
	}

	if (m_spD3DSysTexture)
	{
		m_spD3DSysTexture->Release();
		m_spD3DSysTexture = NULL;
	}

	if (m_spTexture)
	{
		m_spRenderer->PurgeTexture(m_spTexture);
		m_spTexture = 0;
	}

	m_pkPrev = NULL;
	m_pkNext = NULL;
}
//-------------------------------------------------------------------------------------------------
sdDynamicTexture::sdLockedRect sdDynamicTexture::LockRegion(uint uiX, uint uiY, uint uiW, uint uiH)
{
	sdLockedRect kLockedData;

	// 锁定内存纹理
	D3DLOCKED_RECT kD3DLockRect;
	RECT kRect[] = {uiX, uiY, uiX + uiW, uiY + uiH};
	if (FAILED(m_spD3DSysTexture->LockRect(0, &kD3DLockRect, kRect, D3DLOCK_NO_DIRTY_UPDATE)))
	{
		NIASSERT(0);

		kLockedData.pBits = 0;
		kLockedData.uiPitch = 0;
		return kLockedData;
	}

	m_spD3DSysTexture->AddDirtyRect(kRect);
	
	kLockedData.pBits = kD3DLockRect.pBits;
	kLockedData.uiPitch = kD3DLockRect.Pitch;

	return kLockedData;
}
//-------------------------------------------------------------------------------------------------
void sdDynamicTexture::UnlockRegion()
{
	// 解锁内存纹理
	if (FAILED(m_spD3DSysTexture->UnlockRect(0)))
	{
		NIASSERT(0);
		return;
	}

	// 设备丢失
	if (m_spRenderer->IsDeviceLost())
		return;

	// 更新到最终纹理
	if (FAILED(m_spD3DDevice->UpdateTexture(m_spD3DSysTexture, m_spD3DVidTexture)))
	{
		NIASSERT(0);
		return;
	}
}
//-------------------------------------------------------------------------------------------------
bool sdDynamicTexture::OnDeviceLost()
{
	// 尝试删除显存纹理
	if (m_spD3DVidTexture)
	{
		m_spD3DVidTexture->Release();
		m_spD3DVidTexture = 0;
	}

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdDynamicTexture::OnDeviceReset(bool bBefore)
{
	// 尝试删除D3D显存纹理
	if (m_spD3DVidTexture)
	{
		m_spD3DVidTexture->Release();
		m_spD3DVidTexture = 0;
	}

	//
	if (bBefore)
	{
		// 设备重设前
		//
		// 清理GB纹理的显存相关数据
		m_spRenderer->PurgeTexture(m_spTexture);
		return true;
	}
	else
	{
		// 设备已经重设
		//
		// 创建D3D显存纹理
		HRESULT hr = D3DXCreateTexture(m_spD3DDevice, 
			m_uiWidth, 
			m_uiHeight, 
			m_uiLevel, 
			D3DUSAGE_DYNAMIC,
			ms_kD3DFormatTable[m_eFormat],
			D3DPOOL_DEFAULT,
			&m_spD3DVidTexture);
		if (SUCCEEDED(hr))	
		{
			// 初始化GB纹理显存相关数据
			if (!NiDX9Direct3DTextureData::Create((NiDX9Direct3DTexture*)(NiTexture*)m_spTexture, m_spRenderer, m_spD3DVidTexture))
			{
				// 重新更新内存纹理数据到显存纹理
				if (SUCCEEDED(m_spD3DSysTexture->AddDirtyRect(NULL)))
				{
					if (SUCCEEDED(m_spD3DDevice->UpdateTexture(m_spD3DSysTexture, m_spD3DVidTexture)))
					{
						return true;
					}
				}
			}
		}

		// 出错
		NIASSERT(0);
		return false;
	}
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE