#include "sdEnginePCH.h"
#include "sdMemoryTexture.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
//-------------------------------------------------------------------------------------------------
sdMemoryTexture::sdMemoryTexture(uint uiWidth, uint uiHeight, uint uiLevel, D3DFORMAT eFormat)
{
	// ��ȡ��Ⱦ�豸
	NiDX9Renderer* spRenderer = NiDX9Renderer::GetRenderer();
	NIASSERT(spRenderer);

	LPDIRECT3DDEVICE9 spD3DDevice = spRenderer->GetD3DDevice();
	NIASSERT(spD3DDevice);

	//
	m_uiWidth = uiWidth;
	m_uiHeight = uiHeight;
	m_uiLevel = uiLevel;
	m_eFormat = eFormat;

	// ����D3D����
	HRESULT hr = D3DXCreateTexture(spD3DDevice, 
		m_uiWidth, m_uiHeight, uiLevel, 
		0,
		eFormat,
		D3DPOOL_SYSTEMMEM,
		&m_spD3DTexture);
	if (FAILED(hr))	
	{
		NIASSERT(0);
		return;
	}
	m_spD3DTexture->AddRef();

	// ����GB�������
	m_spTexture = spRenderer->CreateNiTextureFromD3DTexture(m_spD3DTexture);
	NIASSERT(m_spTexture);
}
//-------------------------------------------------------------------------------------------------
sdMemoryTexture::sdMemoryTexture(LPDIRECT3DTEXTURE9 spD3DTexture)
{
	NIASSERT(spD3DTexture);

	// ��ȡ��Ⱦ�豸
	NiDX9Renderer* spRenderer = NiDX9Renderer::GetRenderer();
	NIASSERT(spRenderer);

	// ��ȡ��ʽ��Ϣ
	D3DSURFACE_DESC kLevelDesc;
	spD3DTexture->GetLevelDesc(0, &kLevelDesc);

	m_uiWidth = kLevelDesc.Width;
	m_uiHeight = kLevelDesc.Height;
	m_uiLevel = spD3DTexture->GetLevelCount();
	m_eFormat = kLevelDesc.Format;

	//
	m_spD3DTexture = spD3DTexture;
	m_spD3DTexture->AddRef();

	// ����GB�������
	m_spTexture = spRenderer->CreateNiTextureFromD3DTexture(m_spD3DTexture);
	NIASSERT(m_spTexture);
}
//-------------------------------------------------------------------------------------------------
sdMemoryTexture::~sdMemoryTexture()
{
	//
	NiDX9Renderer* spRenderer = NiDX9Renderer::GetRenderer();
	NIASSERT(spRenderer);

	//
	if (m_spD3DTexture)
	{
		m_spD3DTexture->Release();
		m_spD3DTexture = NULL;
	}

	if (m_spTexture)
	{
		spRenderer->PurgeTexture(m_spTexture);
		m_spTexture = 0;
	}
}
//-------------------------------------------------------------------------------------------------
D3DLOCKED_RECT sdMemoryTexture::LockRegion(uint uiLevel, uint uiX, uint uiY, uint uiW, uint uiH)
{
	// �����ڴ�����
	D3DLOCKED_RECT kD3DLockRect;
	RECT kRect[] = {uiX, uiY, uiX + uiW, uiY + uiH};
	if (FAILED(m_spD3DTexture->LockRect(uiLevel, &kD3DLockRect, kRect, D3DLOCK_NO_DIRTY_UPDATE)))
	{
		NIASSERT(0);

		kD3DLockRect.pBits = 0;
		kD3DLockRect.Pitch = 0;
		return kD3DLockRect;
	}

	m_spD3DTexture->AddDirtyRect(kRect);

	return kD3DLockRect;
}
//-------------------------------------------------------------------------------------------------
void sdMemoryTexture::UnlockRegion(uint uiLevel)
{
	if (FAILED(m_spD3DTexture->UnlockRect(uiLevel)))
	{
		NIASSERT(0);
		return;
	}
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE