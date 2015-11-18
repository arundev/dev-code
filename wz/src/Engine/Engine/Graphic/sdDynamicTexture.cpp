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
	// ��ȡ��Ⱦ�豸
	m_spRenderer = NiDX9Renderer::GetRenderer();
	NIASSERT(m_spRenderer);

	m_spD3DDevice = m_spRenderer->GetD3DDevice();

	//
	m_uiWidth = uiWidth;
	m_uiHeight = uiHeight;
	m_uiLevel = 1;
	m_eFormat = eFormat;

	// ����D3D�ڴ�����
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

	// ����D3D�Դ�����
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

	// ����GB�������
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

	// �����ڴ�����
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
	// �����ڴ�����
	if (FAILED(m_spD3DSysTexture->UnlockRect(0)))
	{
		NIASSERT(0);
		return;
	}

	// �豸��ʧ
	if (m_spRenderer->IsDeviceLost())
		return;

	// ���µ���������
	if (FAILED(m_spD3DDevice->UpdateTexture(m_spD3DSysTexture, m_spD3DVidTexture)))
	{
		NIASSERT(0);
		return;
	}
}
//-------------------------------------------------------------------------------------------------
bool sdDynamicTexture::OnDeviceLost()
{
	// ����ɾ���Դ�����
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
	// ����ɾ��D3D�Դ�����
	if (m_spD3DVidTexture)
	{
		m_spD3DVidTexture->Release();
		m_spD3DVidTexture = 0;
	}

	//
	if (bBefore)
	{
		// �豸����ǰ
		//
		// ����GB������Դ��������
		m_spRenderer->PurgeTexture(m_spTexture);
		return true;
	}
	else
	{
		// �豸�Ѿ�����
		//
		// ����D3D�Դ�����
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
			// ��ʼ��GB�����Դ��������
			if (!NiDX9Direct3DTextureData::Create((NiDX9Direct3DTexture*)(NiTexture*)m_spTexture, m_spRenderer, m_spD3DVidTexture))
			{
				// ���¸����ڴ��������ݵ��Դ�����
				if (SUCCEEDED(m_spD3DSysTexture->AddDirtyRect(NULL)))
				{
					if (SUCCEEDED(m_spD3DDevice->UpdateTexture(m_spD3DSysTexture, m_spD3DVidTexture)))
					{
						return true;
					}
				}
			}
		}

		// ����
		NIASSERT(0);
		return false;
	}
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE