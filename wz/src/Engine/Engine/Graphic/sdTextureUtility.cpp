#include "sdEnginePCH.h"
#include "sdTextureUtility.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
//-------------------------------------------------------------------------------------------------
bool sdTextureUtility::Compress(void* pSrcData, uint uiWidth, uint uiHeight, uint uiBytesPerPixel, eCompressFMT eFormat, stBuffer& kDstBuffer)
{
	// Init destination buffer
	kDstBuffer.pBits = NULL;
	kDstBuffer.uiSize = 0;

	// 获取设备
	NiD3DRenderer* spDX9Render = NiD3DRenderer::GetRenderer();
	NIASSERT(spDX9Render);

	LPDIRECT3DDEVICE9 d3dDevice = spDX9Render->GetD3DDevice();
	
	// 获取源数据格式和目标数据格式
	D3DFORMAT d3dDstFMT = (D3DFORMAT)ConvertCompressFMT2D3DFMT(eFormat);
	D3DFORMAT d3dSrcFMT = (D3DFORMAT)ConvertNumBytes2D3DFMT(uiBytesPerPixel);

	//
	IDirect3DTexture9* d3dSrcTexture = NULL;
	HRESULT hr = D3DXCreateTexture(d3dDevice, uiWidth, uiHeight, 1, 0, d3dSrcFMT, D3DPOOL_MANAGED, &d3dSrcTexture);
	if (FAILED(hr))
	{
		NIASSERT(0);
		return false;
	}

	IDirect3DTexture9* d3dDstTexture = NULL;
	hr = D3DXCreateTexture(d3dDevice, uiWidth, uiHeight, 1, 0, d3dSrcFMT, D3DPOOL_MANAGED, &d3dDstTexture);
	if (FAILED(hr))
	{
		NIASSERT(0);
		return false;
	}

	//
	IDirect3DSurface9* d3dSrcSurface = NULL;
	hr = d3dSrcTexture->GetSurfaceLevel(0, &d3dSrcSurface);
	if (FAILED(hr))
	{
		NIASSERT(0);
		return false;
	}
	
	IDirect3DSurface9* d3dDstSurface = NULL;
	hr = d3dDstTexture->GetSurfaceLevel(0, &d3dDstSurface);
	if (FAILED(hr))
	{
		NIASSERT(0);
		return false;
	}

	//
	D3DLOCKED_RECT d3dDstData;
	hr = d3dSrcTexture->LockRect(0, &d3dDstData, NULL, 0);
	if (FAILED(hr))
	{
		NIASSERT(0);
		return false;
	}

	memcpy_s(d3dDstData.pBits, d3dDstData.Pitch * uiWidth, pSrcData, uiWidth * uiHeight * uiBytesPerPixel);

	d3dSrcTexture->UnlockRect(0);

	//
	hr = D3DXLoadSurfaceFromSurface(d3dDstSurface, NULL, NULL, d3dSrcSurface, NULL, NULL, D3DX_DEFAULT, 0);
	if (FAILED(hr))
	{
		NIASSERT(0);
		return false;
	}

	//
	ID3DXBuffer* d3dBuffer = NULL;
	hr = D3DXSaveTextureToFileInMemory(&d3dBuffer, D3DXIFF_DDS, d3dDstTexture, NULL);
	if (FAILED(hr))
	{
		NIASSERT(0);
		return false;
	}

	//
	kDstBuffer.uiSize = d3dBuffer->GetBufferSize();
	kDstBuffer.pBits = NiAlloc2(uchar, kDstBuffer.uiSize, NiMemHint::TEXTURE);
	memcpy_s(kDstBuffer.pBits, kDstBuffer.uiSize, d3dBuffer->GetBufferPointer(), kDstBuffer.uiSize);

	//
	d3dSrcTexture->Release();
	d3dDstTexture->Release();
	d3dSrcSurface->Release();
	d3dDstSurface->Release();
	d3dBuffer->Release();

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdTextureUtility::Uncompress(stBuffer& kSrcBuffer, uint uiBytesPerPixel, stBuffer& kDstBuffer)
{
	// Init destination buffer
	kDstBuffer.pBits = NULL;
	kDstBuffer.uiSize = 0;

	// 获取设备
	NiD3DRenderer* spDX9Render = NiD3DRenderer::GetRenderer();
	NIASSERT(spDX9Render);

	LPDIRECT3DDEVICE9 d3dDevice = spDX9Render->GetD3DDevice();

	// 获取源数据格式
	D3DFORMAT d3dDstFMT = (D3DFORMAT)ConvertNumBytes2D3DFMT(uiBytesPerPixel);

	//
	IDirect3DTexture9* d3dDstTexture = NULL;
	D3DXIMAGE_INFO d3dSrcImageInfo;
	HRESULT hr = D3DXCreateTextureFromFileInMemoryEx(d3dDevice, kSrcBuffer.pBits, kSrcBuffer.uiSize,
		0, 0, 1, 0, d3dDstFMT, D3DPOOL_SYSTEMMEM, D3DX_DEFAULT, D3DX_DEFAULT, 0, 
		&d3dSrcImageInfo, NULL, &d3dDstTexture);
	if (FAILED(hr))
	{
		NIASSERT(0);
		return false;
	}

	//
	UINT uiWidth = d3dSrcImageInfo.Width;
	UINT uiHeight = d3dSrcImageInfo.Height;
	D3DLOCKED_RECT d3dSrcData;
	hr = d3dDstTexture->LockRect(0, &d3dSrcData, NULL, 0);
	if (FAILED(hr))
	{
		NIASSERT(0);
		return false;
	}

	kDstBuffer.uiSize = d3dSrcData.Pitch * uiHeight;
	kDstBuffer.pBits = NiAlloc2(uchar, uiWidth * uiHeight * uiBytesPerPixel, NiMemHint::TEXTURE);
	memcpy_s(kDstBuffer.pBits, d3dSrcData.Pitch * uiHeight, d3dSrcData.pBits, d3dSrcData.Pitch * uiHeight);

	d3dDstTexture->UnlockRect(0);

	//
	d3dDstTexture->Release();

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdTextureUtility::IsDDSFormat(D3DFORMAT eFormat)
{
	static uint32 uiKey		= MAKEFOURCC('D', 'X', 'T', 0);
	static uint32 uiMask	= MAKEFOURCC(0xff, 0xff, 0xff, 0x00);

	return (eFormat & uiMask) == uiKey;
}
//-------------------------------------------------------------------------------------------------
bool sdTextureUtility::IsZStencilFormat(D3DFORMAT eFormat)
{
	// Depth-Stencil缓存格式列表(来自d3dtypes.h)
	static D3DFORMAT eZStencilFormatTable[] = {
		D3DFMT_D16_LOCKABLE,
		D3DFMT_D32,
		D3DFMT_D15S1,
		D3DFMT_D24S8,
		D3DFMT_D24X8,
		D3DFMT_D24X4S4,
		D3DFMT_D16,
		D3DFMT_D32F_LOCKABLE,
		D3DFMT_D24FS8 ,
	#if !defined(D3D_DISABLE_9EX)
		D3DFMT_D32_LOCKABLE,
		D3DFMT_S8_LOCKABLE,
	#endif
		};
	
	static uint uiNumZStencilFormat = sizeof(eZStencilFormatTable) / sizeof (eZStencilFormatTable[0]);

	for (uint i = 0; i < uiNumZStencilFormat; ++i)
	{
		if (eFormat == eZStencilFormatTable[i])
			return true;
	}

	return false;
}
//-------------------------------------------------------------------------------------------------
DWORD sdTextureUtility::ConvertCompressFMT2D3DFMT(eCompressFMT eFormat)
{
	D3DFORMAT d3dFMT = D3DFMT_UNKNOWN;
	switch(eFormat)
	{
	case DXT1:
		d3dFMT = D3DFMT_DXT1;
		break;
	case DXT2:
		d3dFMT = D3DFMT_DXT2;
		break;
	case DXT3:
		d3dFMT = D3DFMT_DXT3;
		break;
	case DXT4:
		d3dFMT = D3DFMT_DXT4;
		break;
	case DXT5:
		d3dFMT = D3DFMT_DXT5;
		break;
	default:
		NIASSERT(0);
		break;
	}
	return d3dFMT;
}
//-------------------------------------------------------------------------------------------------
DWORD sdTextureUtility::ConvertNumBytes2D3DFMT(uint uiBytesPerPixel)
{
	D3DFORMAT d3dFMT = D3DFMT_UNKNOWN;
	switch(uiBytesPerPixel)
	{
	case 3:
		d3dFMT = D3DFMT_R8G8B8;
		break;
	case 4:
		d3dFMT = D3DFMT_A8R8G8B8;
		break;
	default:
		NIASSERT(0);
		break;
	}
	return d3dFMT;
}
//-------------------------------------------------------------------------------------------------
IDirect3DTexture9* sdTextureUtility::ConvertDXT5ToDXT1(IDirect3DTexture9* d3dDXT5Tex)
{
	if (!d3dDXT5Tex)
		return NULL;

	// 获取D3D设备
	NiD3DRenderer* spDX9Render = NiD3DRenderer::GetRenderer();
	NIASSERT(spDX9Render);

	LPDIRECT3DDEVICE9 d3dDevice = spDX9Render->GetD3DDevice();
	NIASSERT(d3dDevice);

	// 检查DXT5纹理
	IDirect3DSurface9* d3dDXT5Surface = NULL;
	HRESULT hr = d3dDXT5Tex->GetSurfaceLevel(0, &d3dDXT5Surface);
	if (hr != D3D_OK)
		return NULL;

	D3DSURFACE_DESC d3dDXT5SurfaceDesc;
	hr = d3dDXT5Surface->GetDesc(&d3dDXT5SurfaceDesc);
	{
		d3dDXT5Surface->Release();
		return NULL;
	}

	if (d3dDXT5SurfaceDesc.Format != D3DFMT_DXT5 || d3dDXT5SurfaceDesc.Width != d3dDXT5SurfaceDesc.Height || !NiIsPowerOf2(d3dDXT5SurfaceDesc.Width))
	{
		d3dDXT5Surface->Release();
		return NULL;
	}	

	// 计算Mipmap数量
	uint uiLevelCount = 1;
	while ((d3dDXT5SurfaceDesc.Width >> uiLevelCount) > 0)
		++uiLevelCount;

	// 创建DXT1纹理
	IDirect3DTexture9* d3dDXT1Tex = NULL;
	hr = d3dDevice->CreateTexture(d3dDXT5SurfaceDesc.Width, d3dDXT5SurfaceDesc.Height, uiLevelCount, 0, D3DFMT_DXT1, D3DPOOL_SYSTEMMEM, &d3dDXT1Tex, 0);
	if (hr != D3D_OK)
	{
		d3dDXT5Surface->Release();
		return NULL;
	}

	//  
	IDirect3DSurface9* d3dDXT1Surface = NULL;
	hr = d3dDXT1Tex->GetSurfaceLevel(0, &d3dDXT1Surface);
	if (hr != D3D_OK)
	{
		d3dDXT5Surface->Release();
		d3dDXT1Tex->Release();
		return NULL;
	}

	// 拷贝数据
	D3DLOCKED_RECT d3dDXT5LockRect;
	hr = d3dDXT5Surface->LockRect(&d3dDXT5LockRect, NULL, D3DLOCK_READONLY);
	if (hr != D3D_OK)
	{
		d3dDXT5Surface->Release();
		d3dDXT1Surface->Release();
		d3dDXT1Tex->Release();
		return NULL;
	}

	D3DLOCKED_RECT d3dDXT1LockRect;
	hr = d3dDXT1Surface->LockRect(&d3dDXT1LockRect, NULL, D3DLOCK_READONLY);
	if (hr != D3D_OK)
	{
		d3dDXT5Surface->UnlockRect();
		d3dDXT5Surface->Release();
		d3dDXT1Surface->Release();
		d3dDXT1Tex->Release();
		return NULL;
	}	

	uchar* ucSrcData = static_cast<uchar*>(d3dDXT5LockRect.pBits);
	uchar* ucDstData = static_cast<uchar*>(d3dDXT1LockRect.pBits);
	uint uiWidth = d3dDXT5SurfaceDesc.Width;
	uint uiHeight = d3dDXT5SurfaceDesc.Height;
	for (uint uiH = 0; uiH < uiHeight/4; ++uiH)
	{
		for (uint uiW = 0; uiW < uiWidth/4; ++uiW)
		{
			// DXT1 8 byte
			for (uint ui = 0; ui < 9; ++ui)
				ucDstData[uiW * 8 + ui] = ucSrcData[uiW * 16 + 8 + ui];
		}

		ucSrcData += d3dDXT5LockRect.Pitch;
		ucDstData += d3dDXT1LockRect.Pitch;
	}

	d3dDXT5Surface->UnlockRect();
	d3dDXT1Surface->UnlockRect();

	//
	d3dDXT5Surface->Release();
	d3dDXT1Surface->Release();

	//
	IDirect3DSurface9* d3dBigSurface = NULL;
	IDirect3DSurface9* d3dSmallSurface = NULL;
	for (uint uiLevel = 0; uiLevel < uiLevelCount-1; ++uiLevel)
	{
		if (FAILED(d3dDXT1Tex->GetSurfaceLevel(uiLevel, &d3dBigSurface)))
			continue;

		if (FAILED(d3dDXT1Tex->GetSurfaceLevel(uiLevel+1, &d3dSmallSurface)))
		{
			d3dBigSurface->Release();
			continue;
		}

		hr = D3DXLoadSurfaceFromSurface(d3dSmallSurface, NULL, NULL, d3dBigSurface, NULL, NULL, D3DX_FILTER_LINEAR, 0);

		d3dBigSurface->Release();
		d3dSmallSurface->Release();
	}

	return d3dDXT1Tex;
}
//-------------------------------------------------------------------------------------------------
DWORD SDBitsPerPixel(D3DFORMAT d3dFmt)
{
	DWORD dwFmt = (DWORD)d3dFmt;
	switch(dwFmt)
	{
	case D3DFMT_A32B32G32R32F:
		return 128;

	case D3DFMT_A16B16G16R16:
	case D3DFMT_Q16W16V16U16:
	case D3DFMT_A16B16G16R16F:
	case D3DFMT_G32R32F:
		return 64;

	case D3DFMT_A8R8G8B8:
	case D3DFMT_A8B8G8R8:
	case D3DFMT_X8R8G8B8:
	case D3DFMT_X8B8G8R8:
	case D3DFMT_A2R10G10B10:
	case D3DFMT_A2B10G10R10:
	case D3DFMT_G16R16:
	case D3DFMT_Q8W8V8U8:
	case D3DFMT_V16U16:
	case D3DFMT_X8L8V8U8:
	case D3DFMT_A2W10V10U10:
	case D3DFMT_D32:
	case D3DFMT_D24S8:
	case D3DFMT_D24X8:
	case D3DFMT_D24X4S4:
	case D3DFMT_D24FS8:
	case D3DFMT_D32F_LOCKABLE:
	case D3DFMT_INDEX32:
	case D3DFMT_G16R16F:
	case D3DFMT_R32F:
		return 32;

	case D3DFMT_R8G8B8:
		return 24;

	case D3DFMT_A4R4G4B4:
	case D3DFMT_X4R4G4B4:
	case D3DFMT_R5G6B5:
	case D3DFMT_L16:
	case D3DFMT_A8L8:
	case D3DFMT_X1R5G5B5:
	case D3DFMT_A1R5G5B5:
	case D3DFMT_A8R3G3B2:
	case D3DFMT_V8U8:
	case D3DFMT_CxV8U8:
	case D3DFMT_L6V5U5:
	case D3DFMT_G8R8_G8B8:
	case D3DFMT_R8G8_B8G8:
	case D3DFMT_D16:
	case D3DFMT_D16_LOCKABLE:
	case D3DFMT_D15S1:
	case D3DFMT_INDEX16:
	case D3DFMT_R16F:
	case D3DFMT_YUY2:
		return 16;

	case D3DFMT_R3G3B2:
	case D3DFMT_A8:
	case D3DFMT_A8P8:
	case D3DFMT_P8:
	case D3DFMT_L8:
	case D3DFMT_A4L4:
		return 8;

	case D3DFMT_DXT1:
		return 4;

	case D3DFMT_DXT2:
	case D3DFMT_DXT3:
	case D3DFMT_DXT4:
	case D3DFMT_DXT5:
		return 8;

	case D3DFMT_UYVY:
		return 16;

	case MAKEFOURCC('A', 'I', '4', '4'):
	case MAKEFOURCC('I', 'A', '4', '4'):
		return 8;

	case MAKEFOURCC('Y', 'V', '1', '2'):
		return 12;

#if !defined(D3D_DISABLE_9EX)
	case D3DFMT_D32_LOCKABLE:
		return 32;

	case D3DFMT_S8_LOCKABLE:
		return 8;

	case D3DFMT_A1:
		return 1;
#endif ///<! D3D_DISABLE_9EX

	default:
		return 0;

	}
}
//-------------------------------------------------------------------------------------------------
DWORD SDCalcD3DTextureSize(IDirect3DTexture9* d3dTexture)
{
	// 关于mipmap BC压缩算法可查看Block Compression, 详细说明了如何padding的问题

	DWORD dwLevelCount = d3dTexture->GetLevelCount();
	if (!dwLevelCount)
		return 0;

	D3DRESOURCETYPE d3dType = d3dTexture->GetType();
	if (d3dType != D3DRTYPE_TEXTURE)
		return 0;

	D3DSURFACE_DESC d3dDesc;
	((IDirect3DTexture9*)d3dTexture)->GetLevelDesc(0, &d3dDesc);

	DWORD dwBytesPerBlock = 0;
	if (d3dDesc.Format == D3DFMT_DXT1)
		dwBytesPerBlock = 8;
	else if (d3dDesc.Format == D3DFMT_A8R8G8B8 || d3dDesc.Format == D3DFMT_X8R8G8B8)
		dwBytesPerBlock = 64;
	else if (d3dDesc.Format == D3DFMT_R5G6B5 || d3dDesc.Format == D3DFMT_X1R5G5B5)
		dwBytesPerBlock = 32;
	else if (d3dDesc.Format == D3DFMT_DXT5 || d3dDesc.Format == D3DFMT_DXT3 || d3dDesc.Format == D3DFMT_A8)
		dwBytesPerBlock = 16;
	else 
		dwBytesPerBlock = 2 * SDBitsPerPixel(d3dDesc.Format);

	DWORD dwFileSize = 0;
	DWORD dwWidth = d3dDesc.Width;
	DWORD dwHeight = d3dDesc.Height;
	IDirect3DSurface9* d3dSurface = NULL;
	for (DWORD dwLevel = 0; dwLevel < dwLevelCount; ++dwLevel)
	{
		dwWidth = (dwLevel != 0) ? dwWidth : 1;
		dwHeight = (dwHeight != 0) ? dwHeight : 1;

		// 宽高都必须取4的整数倍(向上round),再计算出总像素,
		// 每个Block有16个像素,所以必须先除以16(右移4)，再乘以BytesPerBlock
		dwFileSize += ((((dwWidth + 3) & ~0x3) * ((dwHeight + 3) & ~0x3)) >> 4) * dwBytesPerBlock;
		
		dwWidth >> 1;
		dwHeight >> 1;
	}

	return dwFileSize;
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE