#include "sdEnginePCH.h"
#include "sdDX9SourceTextureData.h"
#include "sdSourceTexture.h"

#include <NiFilename.h>

#include <ddraw.h>

#pragma warning(push)
#pragma warning(disable:4552)	// warning C4552: '>>' : operator has no effect

using namespace Engine::FileSystem;

ENGINE_NAMESPACE_BEGIN_ENGINE
//-------------------------------------------------------------------------------------------------
sdDX9SourceTextureData* sdDX9SourceTextureData::Create(sdSourceTexture* pkTexture, 
	NiDX9Renderer* pkRenderer, unsigned uiSkipLevels)
{
	// Using the new default texture formats.
	const NiDX9Renderer::TexUsage eTex = NiDX9Renderer::TEXUSE_TEX;
	NiPixelFormat** ppkPixFmts = pkRenderer->GetTextureFormatArray(eTex);

	// 创建纹理的渲染器对象
	sdDX9SourceTextureData* pkThis = NiNew sdDX9SourceTextureData(pkTexture, pkRenderer);	
	NIASSERT(pkThis);
	
	IDataStream* pkDataStream = pkTexture->GetDataStream();
	NiPixelData* pkSrcPixels = pkTexture->GetSourcePixelData();
	NiDX9PersistentSrcTextureRendererData* pkPersistentSrcRendererData = 
		(NiDX9PersistentSrcTextureRendererData*)pkTexture->GetPersistentSourceRendererData();
	
	// 如果PersistentData/PixelData/DataStream都不存在，则尝试从磁盘加载数据
	bool bFoundFile = false;
	bool bCanLoadFile = false;
	if (!pkPersistentSrcRendererData && !pkSrcPixels && !pkDataStream)
	{
		const char* pcFileName = pkTexture->GetPlatformSpecificFilename();
		if (pcFileName)
		{
			if (pkTexture->GetLoadDirectToRendererHint())
			{
				const NiTexture::FormatPrefs kPerfs = pkTexture->GetFormatPreferences();
				bool bSuccess = pkThis->LoadTextureFileEx(pcFileName, pkRenderer, kPerfs, uiSkipLevels);
				if (bSuccess)
				{
					pkThis->m_pkTexture->SetRendererData(pkThis);
					return pkThis;
				}
			}

			pkSrcPixels = NiImageConverter::GetImageConverter()->ReadImageFile(pcFileName, NULL);
			if (pkSrcPixels)
			{
				bFoundFile = true;
				bCanLoadFile = false;
			}
			else
			{
				char acStandardFilename[NI_MAX_PATH];
				NiStrcpy(acStandardFilename, NI_MAX_PATH, pcFileName);
				NiPath::Standardize(acStandardFilename);
				
				NiFile* pkIstr = NiFile::GetFile(acStandardFilename, NiFile::READ_ONLY);
				if ((pkIstr) && (*pkIstr))
					bFoundFile = true;
				NiDelete pkIstr;

				if (bFoundFile)
				{
					bCanLoadFile = NiImageConverter::GetImageConverter()->CanReadImageFile(pcFileName);
				}
			}
		}
	}

	NiPixelDataPtr spSrcPixels = pkSrcPixels;	///<　函数退出时指针减一

	const NiTexture::FormatPrefs& kPrefs = pkTexture->GetFormatPreferences();

	// 对PersistentData/PixelData/DataStream进行数据检查
	const NiPixelFormat* pkDstFormat = NULL;	///< 纹理GB格式
	D3DFORMAT eD3DFmt = D3DFMT_UNKNOWN;			///< 纹理D3D格式
	unsigned int uiWidth = 0;					///< 纹理宽度
	unsigned int uiHeight = 0;					///< 纹理高度
	unsigned int uiMipmapLvl = 0;				///< 纹理的Mipmap数量
	if (pkPersistentSrcRendererData)
	{
		const NiPixelFormat& kSrcFmt = pkPersistentSrcRendererData->GetPixelFormat();
		uiWidth = pkPersistentSrcRendererData->GetWidth();
		uiHeight = pkPersistentSrcRendererData->GetHeight();

		if ((pkPersistentSrcRendererData->GetPixels() == NULL) || (pkPersistentSrcRendererData->GetTargetPlatform() != NiPersistentSrcTextureRendererData::NI_DX9))
		{
			spSrcPixels = GetReplacementData(pkTexture, FAILEDCONVERT);
			pkThis->m_bReplacementData = true;

			NiD3DRenderer::Error("Texture %s contains persistent source texture"
				" data for a platform other than DX9, cannot convert\n", 
				pkTexture->GetPlatformSpecificFilename());
		}
		else if (!(NiIsPowerOf2(uiWidth) &&	NiIsPowerOf2(uiHeight)) && (pkRenderer->GetFlags() & NiRenderer::CAPS_NONPOW2_TEXT) == 0)
		{
			if (pkRenderer->GetFlags() & NiRenderer::CAPS_NONPOW2_TEXT)
			{
				pkDstFormat = pkThis->FindMatchingPixelFormat(kSrcFmt, kPrefs, ppkPixFmts);
				if (pkDstFormat == NULL)
				{
					spSrcPixels = GetReplacementData(pkTexture, FAILEDCONVERT);
					pkThis->m_bReplacementData = true;
				}
			}
			else
			{
				spSrcPixels = GetReplacementData(pkTexture, BADDIMENSIONS);
				pkThis->m_bReplacementData = true;
			}
		}
		else
		{
			pkDstFormat = pkThis->FindMatchingPixelFormat(kSrcFmt, kPrefs, ppkPixFmts);
			if (pkDstFormat == NULL)
			{
				spSrcPixels = GetReplacementData(pkTexture, FAILEDCONVERT);
				pkThis->m_bReplacementData = true;
			}
		}

		// Determine the D3D format from the pixel format.
		eD3DFmt = NiDX9PixelFormat::DetermineD3DFormat(kSrcFmt);
		if (eD3DFmt == D3DFMT_UNKNOWN)
		{
			spSrcPixels = GetReplacementData(pkTexture, FAILEDCONVERT);
			pkThis->m_bReplacementData = true;
		}

		// Determine if the D3D format is compatible with device.
		if (!CheckTextureFormat(eD3DFmt, pkRenderer))
		{
			spSrcPixels = GetReplacementData(pkTexture, FAILEDCONVERT);
			pkThis->m_bReplacementData = true;
		}

		// Check texture dimensions
		if (!CheckTextureDimensions(uiWidth, uiHeight, pkRenderer))
		{
			spSrcPixels = GetReplacementData(pkTexture, FAILEDCONVERT);
			pkThis->m_bReplacementData = true;
		}
	}
	else if (spSrcPixels)
	{
		const NiPixelFormat& kSrcFmt = pkSrcPixels->GetPixelFormat();
		uiWidth = pkSrcPixels->GetWidth();
		uiHeight = pkSrcPixels->GetHeight();

		if (!(NiIsPowerOf2(uiWidth) &&	NiIsPowerOf2(uiHeight)) && (pkRenderer->GetFlags() & NiRenderer::CAPS_NONPOW2_TEXT) == 0)
		{
			if (pkRenderer->GetFlags() & NiRenderer::CAPS_NONPOW2_TEXT)
			{
				if (kPrefs.m_ePixelLayout == NiTexture::FormatPrefs::COMPRESSED)
				{
					NiTexture::FormatPrefs kPrefsUncompressed = kPrefs;
					kPrefsUncompressed.m_ePixelLayout = NiTexture::FormatPrefs::TRUE_COLOR_32;

					pkDstFormat = pkThis->FindMatchingPixelFormat(kSrcFmt, kPrefsUncompressed, ppkPixFmts);
				}
				else
				{
					pkDstFormat = pkThis->FindMatchingPixelFormat(kSrcFmt, kPrefs, ppkPixFmts);
				}

				if (pkDstFormat == NULL)
				{
					spSrcPixels = GetReplacementData(pkTexture, FAILEDCONVERT);
					pkThis->m_bReplacementData = true;
				}
			}
			else
			{
				spSrcPixels = GetReplacementData(pkTexture, BADDIMENSIONS);
				pkThis->m_bReplacementData = true;
			}
		}
		else
		{
			pkDstFormat = pkThis->FindMatchingPixelFormat(kSrcFmt, kPrefs, ppkPixFmts);
			if (pkDstFormat == NULL)
			{
				spSrcPixels = GetReplacementData(pkTexture, FAILEDCONVERT);
				pkThis->m_bReplacementData = true;
			}
		}

		// Check texture dimensions
		if (!pkThis->CheckTextureDimensions(uiWidth, uiHeight, pkRenderer))
		{
			spSrcPixels = GetReplacementData(pkTexture, FAILEDCONVERT);
			pkThis->m_bReplacementData = true;
		}
	}
	else if (pkDataStream)
	{
		// 获取数据流数据
		const char* pcDataBuff = pkDataStream->GetBuff();
		unsigned int uiDataSize = pkDataStream->Size();
		NIASSERT(pcDataBuff);
		NIASSERT(uiDataSize);

		D3DXIMAGE_INFO d3dImageInfo;
		HRESULT hr = D3DXGetImageInfoFromFileInMemory((VOID*)pcDataBuff, uiDataSize, &d3dImageInfo);
		if (FAILED(hr))
		{
			spSrcPixels = GetReplacementData(pkTexture, FAILEDCONVERT);
			pkThis->m_bReplacementData = true;
		}
		else
		{
			uiWidth = d3dImageInfo.Width;
			uiHeight = d3dImageInfo.Height;
			uiMipmapLvl = d3dImageInfo.MipLevels;

			// 反算GB纹理格式
			NiDX9PixelFormat::InitFromD3DFormat(d3dImageInfo.Format, (NiPixelFormat&)pkDstFormat);

			// 检查纹理尺寸
			if (!CheckTextureDimensions(uiWidth, uiHeight, pkRenderer))
			{
				spSrcPixels = GetReplacementData(pkTexture, FAILEDCONVERT);
				pkThis->m_bReplacementData = true;
			}
		}
	}
	else
	{
		if (!bFoundFile)
		{
			spSrcPixels = GetReplacementData(pkTexture, FILENOTFOUND);
			NiD3DRenderer::Error("Failed to find texture %s\n", 
				pkTexture->GetPlatformSpecificFilename());
		}
		else if (!bCanLoadFile)
		{
			spSrcPixels = GetReplacementData(pkTexture, FILEFORMATNOTSUPPORTED);
			NiD3DRenderer::Error("Texture file %s found, but format is not supported\n", 
				pkTexture->GetPlatformSpecificFilename());
		}
		else
		{
			spSrcPixels = GetReplacementData(pkTexture, FILELOADFAILED);
			NiD3DRenderer::Error("Texture file %s found and supported, but could not be loaded\n", 
				pkTexture->GetPlatformSpecificFilename());
		}

		pkThis->m_bReplacementData = true;
	}
	
	//
	if (pkThis->m_bReplacementData)
	{
		pkDstFormat = ppkPixFmts[pkRenderer->GetReplacementDataFormat()];

		pkPersistentSrcRendererData = NULL;	///< Sanity checks (???)
		NIASSERT(pkSrcPixels);
		NIASSERT(pkDstFormat);
	}

	// 创建设备纹理并加载纹理数据
	if (pkPersistentSrcRendererData)
	{
		// Update texture attributes from the NiDX9SourceTextureData
		pkThis->m_kPixelFormat = pkPersistentSrcRendererData->GetPixelFormat();
		pkThis->m_uiWidth = pkPersistentSrcRendererData->GetWidth();
		pkThis->m_uiHeight = pkPersistentSrcRendererData->GetHeight();
		pkThis->m_usLevels = (unsigned short)pkPersistentSrcRendererData->GetNumMipmapLevels();
		pkThis->m_bMipmap = (pkThis->m_usLevels == 1) ? false : true;
		pkThis->m_bReplacementData = false;
		pkThis->m_pkRenderer = pkRenderer;
		pkThis->m_uiFormattedSize = (unsigned int)pkPersistentSrcRendererData->GetTotalSizeInBytes();

		// Attach palette if the surface is palettized
		pkThis->UpdatePalette(pkPersistentSrcRendererData->GetPalette());

		// Allocate texture and establish mipmap levels
		if (!pkThis->m_pkD3DTexture)
			pkThis->CreateSurfFromRendererDataEx(pkPersistentSrcRendererData, eD3DFmt, uiSkipLevels);

		// Copy to texture
		if (pkThis->m_pkD3DTexture)
			pkThis->CopyRendererDataToSurface(pkPersistentSrcRendererData);
	}
	else if (pkSrcPixels)
	{
		pkThis->m_uiSourceRevID = pkSrcPixels->GetRevisionID();
		pkThis->m_kPixelFormat = *pkDstFormat;
		pkThis->m_uiWidth = pkSrcPixels->GetWidth();
		pkThis->m_uiHeight = pkSrcPixels->GetHeight();

		pkThis->m_bMipmap = (!pkThis->m_bReplacementData && 
			((kPrefs.m_eMipMapped == NiTexture::FormatPrefs::YES) ||
			((kPrefs.m_eMipMapped == NiTexture::FormatPrefs::MIP_DEFAULT) &&
			NiTexture::GetMipmapByDefault()))) ? true : false;

		NiPixelData* pkFormatted = NULL;
		if (pkThis->m_bReplacementData)
		{
			pkFormatted = pkSrcPixels;	///< Do not convert replacement data.
		}
		else
		{
			// Convert data format
			pkFormatted = NiImageConverter::GetImageConverter()->ConvertPixelData(
				*pkSrcPixels, *pkDstFormat, pkSrcPixels, pkThis->m_bMipmap);
			if (pkFormatted == NULL)
				pkFormatted = pkSrcPixels;
		}
		NIASSERT(pkFormatted);

		// Allocate texture
		if (!pkThis->m_pkD3DTexture)
			pkThis->CreateSurfEx(*pkFormatted, uiSkipLevels);

		// Copy to texture
		if (pkThis->m_pkD3DTexture)
			pkThis->CopyDataToSurface(*pkFormatted);

		pkThis->m_usLevels = (unsigned short)pkFormatted->GetNumMipmapLevels();

		// Attach palette if the surface is palettized
		pkThis->UpdatePalette(pkSrcPixels->GetPalette());

		pkThis->m_uiFormattedSize = (unsigned int)pkFormatted->GetTotalSizeInBytes();

		//
		NiPixelDataPtr spDestructor = pkFormatted;
	}
	else if (pkDataStream)
	{
		// 更新纹理属性
		pkThis->m_kPixelFormat = *pkDstFormat;
		pkThis->m_uiWidth = uiWidth;
		pkThis->m_uiHeight = uiHeight;
		pkThis->m_usLevels = (unsigned short)uiMipmapLvl;
		pkThis->m_bMipmap = (pkThis->m_usLevels == 1) ? false : true;
		pkThis->m_bReplacementData = false;
		pkThis->m_pkRenderer = pkRenderer;
		pkThis->m_uiFormattedSize = (unsigned int)pkDataStream->Size();	///< ???

		// 更新调色板
		pkThis->UpdatePalette(NULL);

		// 创建D3D设备纹理
		if (!pkThis->m_pkD3DTexture)
			pkThis->CreateSurfFromDataStreamEx(pkDataStream,uiSkipLevels);

		// 拷贝DataStream数据到D3D设备纹理
		if (pkThis->m_pkD3DTexture)
			pkThis->CopyStreamDataToSurfaceEx(pkDataStream);
	}
	else
	{
		
	}

	// 将渲染数据设置给纹理
	pkThis->m_pkTexture->SetRendererData(pkThis);

	// 销毁内存数据
	if (NiSourceTexture::GetDestroyAppDataFlag())
		pkTexture->DestroyAppPixelData();

	return pkThis;
}
//-------------------------------------------------------------------------------------------------
//sdDX9SourceTextureData* sdDX9SourceTextureData::Create(sdSourceTexture* pkTexture, 
//	NiDX9Renderer* pkRenderer, IDirect3DTexture9* d3dTexture)
//{
//	NIASSERT(pkTexture);
//	NIASSERT(pkRenderer);
//	NIASSERT(d3dTexture);
//
//	sdDX9SourceTextureData* pkThis = NiNew sdDX9SourceTextureData(pkTexture, pkRenderer);
//	NIASSERT(pkThis);
//
//	bool bSuccess = pkThis->InitializeFromD3DTexture(d3dTexture);
//	if (!bSuccess)
//	{
//		NiDelete pkThis;
//		pkThis = NULL;
//
//		NiDX9Renderer::ReleaseTextureResource(d3dTexture);
//
//		return NULL;
//	}
//
//	pkThis->m_pkTexture->SetRendererData(pkThis);
//
//	return pkThis;
//}
//-------------------------------------------------------------------------------------------------
//sdDX9SourceTextureData* sdDX9SourceTextureData::Create(sdSourceTexture* pkTexture, 
//	NiDX9Renderer* pkRenderer, IDirect3DTexture9* d3dTexture,
//	NiPersistentSrcTextureRendererData* pkPersistentData, unsigned uiSkipLevels)
//{
//	NIASSERT(pkTexture);
//	NIASSERT(pkRenderer);
//	NIASSERT(pkPersistentData);
//	
//
//	///< 检查PersistentData数据
//	///< @{
//	if (pkPersistentData->GetPixels() == NULL)
//		return NULL;
//
//	if (pkPersistentData->GetTargetPlatform() != NiPersistentSrcTextureRendererData::NI_DX9)
//		return NULL;
//
//	// 检查纹理尺寸是否是2的幂次方
//	unsigned int uiWidth = pkPersistentData->GetWidth();
//	unsigned int uiHeight = pkPersistentData->GetHeight();
//	if (!(NiIsPowerOf2(uiWidth) && NiIsPowerOf2(uiHeight)) && (pkRenderer->GetFlags() & NiRenderer::CAPS_NONPOW2_TEXT))
//		return NULL;
//
//	// 检查纹理尺寸
//	if (!CheckTextureDimensions(uiWidth, uiHeight, (NiDX9Renderer*)pkRenderer))
//		return NULL;
//
//	// 确定数据格式(将GB格式映射为DX格式)
//	const NiPixelFormat& kSrcFmt = pkPersistentData->GetPixelFormat();
//	D3DFORMAT eD3DFmt = NiDX9PixelFormat::DetermineD3DFormat(kSrcFmt);
//	if (eD3DFmt == D3DFMT_UNKNOWN)
//		return NULL;
//
//	// 检查设备是否支持该DX格式纹理
//	if (!CheckTextureFormat(eD3DFmt, (NiDX9Renderer*)pkRenderer))
//		return NULL;
//	///< @}
//
//
//	// 创建渲染器纹理对象
//	sdDX9SourceTextureData* pkThis = NiNew sdDX9SourceTextureData(pkTexture, pkRenderer);
//	NIASSERT(pkThis);
//
//	// 更新NiDX9SourceTextureData
//	pkThis->m_kPixelFormat = pkPersistentData->GetPixelFormat();
//	pkThis->m_uiWidth = pkPersistentData->GetWidth();
//	pkThis->m_uiHeight = pkPersistentData->GetHeight();
//	pkThis->m_usLevels = (unsigned short)pkPersistentData->GetNumMipmapLevels();
//	pkThis->m_bMipmap = (pkThis->m_usLevels == 1) ? false : true;
//	pkThis->m_bReplacementData = false;
//	pkThis->m_pkRenderer = pkRenderer;
//	pkThis->m_uiFormattedSize = (unsigned int)pkPersistentData->GetTotalSizeInBytes();
//
//	// 创建D3D纹理
//	if (d3dTexture == NULL)
//		pkThis->CreateSurfFromRendererDataEx((NiDX9PersistentSrcTextureRendererData*)pkPersistentData, eD3DFmt, uiSkipLevels);
//	else
//		pkThis->InitializeFromD3DTexture(d3dTexture);
//
//	// 拷贝纹理数据
//	if (pkThis->m_pkD3DTexture)
//		pkThis->CopyRendererDataToSurface((NiDX9PersistentSrcTextureRendererData*)pkPersistentData);
//
//	// 如果纹理表面是调色板模式，则更新调色板参数
//	pkThis->UpdatePalette(pkPersistentData->GetPalette());
//
//	//
//	pkTexture->SetRendererData(pkThis);		///< 本渲染数据对象给纹理对象
//	pkTexture->SetPersistentSourceRendererData(NULL);
//	pkTexture->SetSourcePixelData(NULL);
//	pkTexture->SetDataStream(NULL);
//
//	return pkThis;
//}
//-------------------------------------------------------------------------------------------------
//sdDX9SourceTextureData* sdDX9SourceTextureData::Create(sdSourceTexture* pkTexture, 
//	NiDX9Renderer* pkRenderer, IDirect3DTexture9* d3dTexture,
//	NiPixelData* pkPixelData, unsigned uiSkipLevels)
//{
//	NIASSERT(pkTexture);
//	NIASSERT(pkRenderer);
//	NIASSERT(pkPixelData);
//	
//	// 创建渲染器数据
//	sdDX9SourceTextureData* pkThis = NiNew sdDX9SourceTextureData(pkTexture, pkRenderer);
//	NIASSERT(pkThis);
//
//	///< 检查PixelData数据
//	///< @{
//	// 检查纹理尺寸是否是2的幂次方
//	unsigned int uiWidth = pkPixelData->GetWidth();
//	unsigned int uiHeight = pkPixelData->GetHeight();
//	if (!(NiIsPowerOf2(uiWidth) && NiIsPowerOf2(uiHeight)) && (pkRenderer->GetFlags() & NiRenderer::CAPS_NONPOW2_TEXT))
//	{
//		NiDelete pkThis;
//		return NULL;
//	}
//
//	// 检查纹理尺寸
//	if (!CheckTextureDimensions(uiWidth, uiHeight, (NiDX9Renderer*)pkRenderer))
//	{
//		NiDelete pkThis;
//		return NULL;
//	}
//
//	// 确定数据格式
//	const NiDX9Renderer::TexUsage eTexUsg = NiDX9Renderer::TEXUSE_TEX;
//	NiPixelFormat** ppkPixelFmts = pkRenderer->GetTextureFormatArray(eTexUsg);
//
//	const NiTexture::FormatPrefs& kPrefs = pkTexture->GetFormatPreferences();
//	const NiPixelFormat& kSrcFmt = pkPixelData->GetPixelFormat();
//	const NiPixelFormat* pkDstFmt = pkThis->FindMatchingPixelFormat(kSrcFmt, kPrefs, ppkPixelFmts);
//	if (pkDstFmt == NULL)
//	{
//		NiDelete pkThis;
//		return NULL;
//	}
//	///< @}
//
//
//	// 更新NiDX9SourceTextureData
//	pkThis->m_bReplacementData = false;
//	pkThis->m_uiSourceRevID = pkPixelData->GetRevisionID();
//	pkThis->m_kPixelFormat = *pkDstFmt;
//	pkThis->m_uiWidth = pkPixelData->GetWidth();
//	pkThis->m_uiHeight = pkPixelData->GetHeight();
//	pkThis->m_usLevels = (unsigned short)pkPixelData->GetNumMipmapLevels();
//
//	pkThis->m_bMipmap = (!pkThis->m_bReplacementData && 
//		((kPrefs.m_eMipMapped == NiTexture::FormatPrefs::YES) ||
//		((kPrefs.m_eMipMapped == NiTexture::FormatPrefs::MIP_DEFAULT) &&
//		NiTexture::GetMipmapByDefault()))) ? true : false;
//
//	NiPixelData* pkFormatted = NULL;
//	pkFormatted = pkPixelData;		///< Do not convert replacement data.
//	
//	pkThis->m_pkRenderer = pkRenderer;
//	pkThis->m_uiFormattedSize = (unsigned int)pkFormatted->GetTotalSizeInBytes();
//
//	// 创建D3D纹理
//	if (d3dTexture == NULL)
//		pkThis->CreateSurfEx(*pkFormatted, uiSkipLevels);
//	else
//		pkThis->InitializeFromD3DTexture(d3dTexture);
//
//	// 拷贝纹理数据
//	if (pkThis->m_pkD3DTexture)
//		pkThis->CopyDataToSurface(*pkFormatted);
//
//	// 如果纹理表面是调色板模式，则更新调色板参数
//	pkThis->UpdatePalette(pkPixelData->GetPalette());
//
//	//
//	pkTexture->SetRendererData(pkThis);		///< 本渲染数据对象给纹理对象
//	pkTexture->SetPersistentSourceRendererData(NULL);
//	pkTexture->SetSourcePixelData(NULL);
//	pkTexture->SetDataStream(NULL);
//
//	return pkThis;
//}
//-------------------------------------------------------------------------------------------------
//sdDX9SourceTextureData* sdDX9SourceTextureData::Create(sdSourceTexture* pkTexture, 
//	NiDX9Renderer* pkRenderer, IDirect3DTexture9* d3dTexture,
//	Engine::FileSystem::IDataStream* pkDataStream, unsigned uiSkipLevels)
//{
//	NIASSERT(pkTexture);
//	NIASSERT(pkRenderer);
//	NIASSERT(pkDataStream);
//
//	NiDX9SourceTextureData* pkOldSourceTextureData = (NiDX9SourceTextureData*)pkTexture->GetRendererData();
//	IDirect3DTexture9* d3dManagedTexture = d3dTexture;
//	
//	// 纹理内存数据
//	unsigned int uiDataSize = pkDataStream->Size();
//	const char* pcDataBuff = pkDataStream->GetBuff();
//	if ((pkDataStream == NULL) && (uiDataSize == 0))
//		return false;
//
//	// 非DXT1/DXT3纹理
//	D3DXIMAGE_INFO d3dxImageInfo;
//	HRESULT hr = D3DXGetImageInfoFromFileInMemory((VOID*)pcDataBuff, uiDataSize, &d3dxImageInfo);
//	
//	if (!(d3dxImageInfo.Format == D3DFMT_DXT1 || d3dxImageInfo.Format == D3DFMT_DXT5))
//	{
//		if (pkOldSourceTextureData)
//		{
//			NiDelete pkOldSourceTextureData;
//			pkOldSourceTextureData = NULL;
//		}
//
//		LPDIRECT3DDEVICE9 d3dDevice9 = pkRenderer->GetD3DDevice();
//		HRESULT hr = D3DXCreateTextureFromFileInMemoryEx(
//			d3dDevice9,
//			pcDataBuff, uiDataSize,
//			d3dxImageInfo.Width, d3dxImageInfo.Height, d3dxImageInfo.MipLevels,
//			0, d3dxImageInfo.Format,
//			D3DPOOL_MANAGED,
//			D3DX_DEFAULT, D3DX_DEFAULT,	0,
//			&d3dxImageInfo,
//			NULL,
//			&d3dManagedTexture);
//
//		if (SUCCEEDED(hr))
//			return Create(pkTexture, pkRenderer, d3dManagedTexture);
//		else 
//			return NULL;
//	}
//
//	// 创建渲染器数据
//	sdDX9SourceTextureData* pkThis = NiNew sdDX9SourceTextureData(pkTexture, pkRenderer);
//	NIASSERT(pkThis);
//
//	// 创建设备纹理对象
//	if (d3dManagedTexture != NULL)
//	{
//		unsigned int uiMipmap = d3dManagedTexture->GetLevelCount();
//		if (uiMipmap == 0)
//		{
//			NiDelete pkThis;
//			return NULL;
//		}
//		
//		D3DSURFACE_DESC d3dSurfDesc;
//		d3dManagedTexture->GetLevelDesc(0, &d3dSurfDesc);
//
//		if ((d3dSurfDesc.Width != d3dxImageInfo.Width) ||
//			(d3dSurfDesc.Height != d3dxImageInfo.Height) ||
//			(d3dSurfDesc.Format != d3dxImageInfo.Format) ||
//			(uiMipmap != d3dxImageInfo.MipLevels))
//		{
//			NiDelete pkThis;
//			return NULL;
//		}
//	}
//	else
//	{
//		if (!(NiIsPowerOf2(d3dxImageInfo.Width) && (NiIsPowerOf2(d3dxImageInfo.Height))))
//		{
//			NiDelete pkThis;
//			return NULL;
//		}
//
//		pkThis->m_uiWidth = d3dxImageInfo.Width;
//		pkThis->m_uiHeight = d3dxImageInfo.Height;
//		pkThis->m_usLevels = (unsigned short)d3dxImageInfo.MipLevels;
//
//		unsigned short usSkipLevels = (unsigned short)((uiSkipLevels == 0) ? ms_uiSkipLevels : uiSkipLevels);
//		if (usSkipLevels >  (unsigned int)pkThis->m_usLevels - 1)
//			usSkipLevels =  (unsigned int)pkThis->m_usLevels - 1;
//
//		for (unsigned int ui = 0; ui < usSkipLevels; ++ui)
//		{
//			if (pkThis->m_uiWidth & 0x1)	///< if odd, add 1 so shifted value rounds up.
//				pkThis->m_uiWidth++;
//			pkThis->m_uiWidth >>= 1;
//
//			if (pkThis->m_uiHeight & 0x1)	///< if odd, add 1 so shifted value rounds up.
//				pkThis->m_uiHeight++;
//			pkThis->m_uiHeight >>= 1;
//		}
//
//		pkThis->m_uiLevelsSkipped = usSkipLevels;
//
//		LPDIRECT3DDEVICE9 d3dDevice9 = pkRenderer->GetD3DDevice();
//		HRESULT hr = d3dDevice9->CreateTexture(
//			pkThis->m_uiWidth,
//			pkThis->m_uiHeight,
//			pkThis->m_usLevels - pkThis->m_uiLevelsSkipped,
//			0,
//			d3dxImageInfo.Format,
//			D3DPOOL_MANAGED,
//			&d3dManagedTexture,
//			NULL);
//
//		if (FAILED(hr) || (d3dManagedTexture == NULL))
//		{
//			NiDelete pkThis;
//			return NULL;
//		}
//	}
//
//	// 使用设备纹理初始化渲染器对象
//	bool bSuccess = pkThis->InitializeFromD3DTexture(d3dManagedTexture);
//	if (!bSuccess)
//	{
//		NiDelete pkThis;
//		NiDX9Renderer::ReleaseTextureResource(d3dManagedTexture);
//		return NULL;
//	}
//
//	// 拷贝数据
//	unsigned int uiBitsPerBlock = 0;
//	if (d3dxImageInfo.Format == D3DFMT_DXT1)
//		uiBitsPerBlock = 8;
//	else if (d3dxImageInfo.Format == D3DFMT_DXT5)
//		uiBitsPerBlock = 16;
//
//	unsigned char* pcDataStart = ((unsigned char*)pcDataBuff) + sizeof(DWORD) + sizeof(DDSURFACEDESC2);
//	unsigned int uiWidth = d3dxImageInfo.Width;
//	unsigned int uiHeight = d3dxImageInfo.Height;
//
//	IDirect3DSurface9* d3dSurface = NULL;
//	D3DLOCKED_RECT d3dLockRect;
//
//	for (unsigned int ui = 0; ui < pkThis->m_uiLevelsSkipped; ++ui)
//	{
//		uiWidth = (uiWidth == 0) ? uiWidth : 1;
//		uiHeight = (uiHeight == 0) ? uiHeight : 1;
//
//		unsigned int uiMipmapSize = ((((uiWidth + 3) & ~0x3) * ((uiHeight + 3) & ~0x3)) >> 4) * uiBitsPerBlock;
//		pcDataStart += uiMipmapSize;
//
//		uiWidth >> 1;
//		uiHeight >> 1;
//	}
//
//	for (unsigned int ui = 0; ui < pkThis->m_usLevels - pkThis->m_uiLevelsSkipped; ++ui)
//	{
//		uiWidth = (uiWidth == 0) ? uiWidth : 1;
//		uiHeight = (uiHeight == 0) ? uiHeight : 1;
//
//		// 宽高都必须取4的整数倍(向上round),再计算出总像素,
//		// 每个Block有16个像素,所以必须先除以16(右移4)，再乘以BytesPerBlock
//		unsigned int uiMipmapSize = ((((uiWidth + 3) & ~0x3) * ((uiHeight + 3) & ~0x3)) >> 4) * uiBitsPerBlock;
//
//		HRESULT hr = ((IDirect3DTexture9*)(pkThis->m_pkD3DTexture))->GetSurfaceLevel(ui, &d3dSurface);
//		if (FAILED(hr))
//		{
//			NiDelete pkThis;
//			NiDX9Renderer::ReleaseTextureResource(d3dManagedTexture);
//			return NULL;
//		}
//
//		hr = d3dSurface->LockRect(&d3dLockRect, NULL, 0);
//		if (FAILED(hr))
//		{
//			NiDelete pkThis;
//			NiDX9Renderer::ReleaseTextureResource(d3dManagedTexture);
//			return NULL;
//		}
//
//		NiMemcpy((unsigned char*)d3dLockRect.pBits, (void*)pcDataStart, uiMipmapSize);
//
//		d3dSurface->UnlockRect();
//		NiDX9Renderer::ReleaseResource(d3dSurface);
//
//		pcDataStart += uiMipmapSize;
//		uiWidth >> 1;
//		uiHeight >> 1;
//	}
//
//	//
//	pkTexture->SetRendererData(pkThis);		///< 本渲染数据对象给纹理对象
//	pkTexture->SetPersistentSourceRendererData(NULL);
//	pkTexture->SetSourcePixelData(NULL);
//	pkTexture->SetDataStream(NULL);
//
//	if (pkOldSourceTextureData)
//	{
//		NiDelete pkOldSourceTextureData;
//		pkOldSourceTextureData = NULL;
//	}
//
//	return pkThis;
//}
//-------------------------------------------------------------------------------------------------
sdDX9SourceTextureData::sdDX9SourceTextureData(NiSourceTexture* pkTexture, NiDX9Renderer* pkRenderer)
: NiDX9SourceTextureData(pkTexture, pkRenderer)
//, m_d3dCreatePool(D3DPOOL_MANAGED)
//, m_d3dCreateFormat(D3DFMT_UNKNOWN)
{

}
//-------------------------------------------------------------------------------------------------
bool sdDX9SourceTextureData::CreateSurfEx(const NiPixelData& kPixels, unsigned int uiSkipLevel)
{
	D3DFORMAT eD3DFmt = NiDX9PixelFormat::DetermineD3DFormat(m_kPixelFormat);

	m_uiWidth = kPixels.GetWidth();
	m_uiHeight = kPixels.GetHeight();

	if (!(NiIsPowerOf2(m_uiWidth) && NiIsPowerOf2(m_uiHeight)) &&
		(m_pkRenderer->GetFlags() & NiRenderer::CAPS_NONPOW2_TEXT) == 0)
	{
		m_usLevels = 1;
	}
	else
	{
		m_usLevels = (unsigned short)kPixels.GetNumMipmapLevels();
	}

	m_bMipmap = ((m_usLevels == 1) ? false : true);

	unsigned short usSkipLevels = (unsigned short)((uiSkipLevel == 0) ? ms_uiSkipLevels : uiSkipLevel);
	if (usSkipLevels > m_usLevels - 1)
		usSkipLevels = m_usLevels - 1;

	// Adjust (possibly non-power-of-two) dimensions by uiSkipLevels.
	for (unsigned int i = 0; i < usSkipLevels; i++)
	{
		if (m_uiWidth & 0x1)    // If odd, add 1 so shifted value rounds up.
			m_uiWidth++;
		m_uiWidth >>= 1;
		if (m_uiHeight & 0x1)   // If odd, add 1 so shifted value rounds up.
			m_uiHeight++;
		m_uiHeight >>= 1;
	}

	m_uiLevelsSkipped = usSkipLevels;
	NIASSERT(ms_uiSkipLevels == 0 || m_usLevels != 0);

	LPDIRECT3DDEVICE9 pD3DDevice9 = m_pkRenderer->GetD3DDevice();
	LPDIRECT3DTEXTURE9 pkD3DTexture = NULL;
	HRESULT eD3DRet = pD3DDevice9->CreateTexture(
		m_uiWidth, 
		m_uiHeight,
		m_usLevels - m_uiLevelsSkipped, 
		0, 
		eD3DFmt, 
		D3DPOOL_MANAGED, 
		&pkD3DTexture, 
		NULL);
	if (FAILED(eD3DRet) || pkD3DTexture == NULL)
	{
		NiDX9Renderer::Warning("sdDX9SourceTextureData::CreateSurfEx> Failed"
			" CreateImageSurface - %s\n", 
			NiDX9ErrorString((unsigned int)eD3DRet));
		m_pkD3DTexture = NULL;
		return false;
	}

	m_pkD3DTexture = (D3DBaseTexturePtr)pkD3DTexture;

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdDX9SourceTextureData::CreateSurfFromRendererDataEx(
	const NiDX9PersistentSrcTextureRendererData* pkPersistentSrcRendererData,
	D3DFORMAT eD3DFmt, unsigned int uiSkipLevel)
{
	m_uiWidth = pkPersistentSrcRendererData->GetWidth();
	m_uiHeight = pkPersistentSrcRendererData->GetHeight();

	if (!(NiIsPowerOf2(m_uiWidth) && NiIsPowerOf2(m_uiHeight)) &&
		(m_pkRenderer->GetFlags() & NiRenderer::CAPS_NONPOW2_TEXT) == 0)
	{
		m_usLevels = 1;
	}
	else
	{
		m_usLevels = (unsigned short)pkPersistentSrcRendererData->GetNumMipmapLevels();
	}

	m_bMipmap = ((m_usLevels == 1) ? false : true);

	unsigned short usSkipLevels = (unsigned short)((uiSkipLevel == 0) ? ms_uiSkipLevels : uiSkipLevel);
	if (usSkipLevels > m_usLevels - 1)
		usSkipLevels = m_usLevels - 1;

	// Adjust (possibly non-power-of-two) dimensions by uiSkipLevels.
	for (unsigned int i = 0; i < usSkipLevels; i++)
	{
		if (m_uiWidth & 0x1)    // If odd, add 1 so shifted value rounds up.
			m_uiWidth++;
		m_uiWidth >>= 1;
		if (m_uiHeight & 0x1)   // If odd, add 1 so shifted value rounds up.
			m_uiHeight++;
		m_uiHeight >>= 1;
	}

	m_uiLevelsSkipped = usSkipLevels;
	NIASSERT(ms_uiSkipLevels == 0 || m_usLevels != 0);

	LPDIRECT3DDEVICE9 pkD3DDevice9 = m_pkRenderer->GetD3DDevice();
	D3DTexturePtr pkD3DTexture = NULL;
	HRESULT eD3DRet = pkD3DDevice9->CreateTexture(
		m_uiWidth,
		m_uiHeight,
		m_usLevels - m_uiLevelsSkipped, 
		0,
		eD3DFmt,
		D3DPOOL_MANAGED,
		&pkD3DTexture,
		NULL);
	if (FAILED(eD3DRet) || pkD3DTexture == NULL)
	{
		NiDX9Renderer::Warning(
			"sdDX9SourceTextureData::CreateSurfFromRendererDataEx> "
			"FAILED to create texture of format: 0x%x, D3D error: %s", 
			eD3DFmt, NiDX9ErrorString((unsigned int)eD3DRet));
		m_pkD3DTexture = NULL;
		return false;
	}

	m_pkD3DTexture = (D3DBaseTexturePtr)pkD3DTexture;

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdDX9SourceTextureData::CreateSurfFromDataStreamEx(
	const Engine::FileSystem::IDataStream* pkDataStream, unsigned int uiSkipLevel)
{
	// 获取数据流信息
	const char* pcDataBuff = pkDataStream->GetBuff();
	unsigned int uiDataSize = pkDataStream->Size();
	if ((pcDataBuff == NULL) || (uiDataSize == 0))
		return false;

	// 获取纹理信息
	D3DXIMAGE_INFO d3dxImageInfo;
	HRESULT eD3DRet = D3DXGetImageInfoFromFileInMemory((VOID*)pcDataBuff, uiDataSize, &d3dxImageInfo);
	if (FAILED(eD3DRet))
		return false;

	//
	m_uiWidth = d3dxImageInfo.Width;
	m_uiHeight = d3dxImageInfo.Height;

	if (!(NiIsPowerOf2(m_uiWidth) && NiIsPowerOf2(m_uiHeight)) &&
		(m_pkRenderer->GetFlags() & NiRenderer::CAPS_NONPOW2_TEXT) == 0)
	{
		m_usLevels = 1;
	}
	else
	{
		m_usLevels = (unsigned short)d3dxImageInfo.MipLevels;
	}

	m_bMipmap = ((m_usLevels == 1) ? false : true);

	unsigned short usSkipLevels = (unsigned short)((uiSkipLevel == 0) ? ms_uiSkipLevels : uiSkipLevel);
	if (usSkipLevels > m_usLevels - 1)
		usSkipLevels = m_usLevels - 1;

	// Adjust (possibly non-power-of-two) dimensions by uiSkipLevels.
	for (unsigned int i = 0; i < usSkipLevels; i++)
	{
		if (m_uiWidth & 0x1)    // If odd, add 1 so shifted value rounds up.
			m_uiWidth++;
		m_uiWidth >>= 1;
		if (m_uiHeight & 0x1)   // If odd, add 1 so shifted value rounds up.
			m_uiHeight++;
		m_uiHeight >>= 1;
	}

	m_uiLevelsSkipped = usSkipLevels;
	NIASSERT(ms_uiSkipLevels == 0 || m_usLevels != 0);

	LPDIRECT3DDEVICE9 pD3DDevice9 = m_pkRenderer->GetD3DDevice();
	LPDIRECT3DTEXTURE9 pkD3DTexture = NULL;
	eD3DRet = pD3DDevice9->CreateTexture(
		m_uiWidth, 
		m_uiHeight,
		m_usLevels - m_uiLevelsSkipped, 
		0, 
		d3dxImageInfo.Format, 
		D3DPOOL_MANAGED, 
		&pkD3DTexture, 
		NULL);
	if (FAILED(eD3DRet) || pkD3DTexture == NULL)
	{
		NiDX9Renderer::Warning("sdDX9SourceTextureData::CreateSurfFromDataStreamEx> Failed"
			" CreateImageSurface - %s\n", 
			NiDX9ErrorString((unsigned int)eD3DRet));
		m_pkD3DTexture = NULL;
		return false;
	}

	m_pkD3DTexture = (D3DBaseTexturePtr)pkD3DTexture;

	return true;
}
//-------------------------------------------------------------------------------------------------
void sdDX9SourceTextureData::CopyStreamDataToSurfaceEx(const Engine::FileSystem::IDataStream* pkDataStream)
{
	if (m_pkD3DTexture == NULL ||
		m_pkD3DTexture->GetType() == D3DRTYPE_CUBETEXTURE ||
		m_pkD3DTexture->GetType() == D3DRTYPE_VOLUMETEXTURE)
	{
		return;
	}

	NIASSERT(m_pkD3DTexture);

	// 获取数据流信息
	const char* pcDataBuff = pkDataStream->GetBuff();
	unsigned int uiDataSize = pkDataStream->Size();
	NIASSERT(pcDataBuff);
	NIASSERT(uiDataSize!=0);

	// 获取纹理信息
	D3DXIMAGE_INFO d3dSrcImageInfo;
	HRESULT eD3DRet = D3DXGetImageInfoFromFileInMemory((VOID*)pcDataBuff, uiDataSize, &d3dSrcImageInfo);
	if (FAILED(eD3DRet))
	{
		NiDX9Renderer::Error("sdDX9SourceTextureData::CopyStreamDataToSurfaceEx>"
			" Failed D3DXGetImageInfoFromFileInMemory %s",
			NiDX9ErrorString((unsigned int)eD3DRet));
		return;
	}

	NIASSERT((d3dSrcImageInfo.Format == D3DFMT_DXT1 || d3dSrcImageInfo.Format == D3DFMT_DXT3 || d3dSrcImageInfo.Format == D3DFMT_DXT5));

	//
	unsigned int uiBytesPerBlock = 0;
	if (d3dSrcImageInfo.Format == D3DFMT_DXT1)
		uiBytesPerBlock = 8;
	else if (d3dSrcImageInfo.Format == D3DFMT_DXT1)
		uiBytesPerBlock = 16;

	// 移动数据指针,跳过被跳过的MipmapLevel
	const char* pcDataBuffCur = pcDataBuff + sizeof(DWORD) + sizeof(DDSURFACEDESC2);
	unsigned int uiSrcWidth = d3dSrcImageInfo.Width;
	unsigned int uiSrcHeight = d3dSrcImageInfo.Height;
	for (unsigned int i = 0; i < m_uiLevelsSkipped; i++)
	{
		uiSrcWidth = (uiSrcWidth != 0) ? uiSrcWidth : 1;
		uiSrcHeight = (uiSrcHeight != 0) ? uiSrcHeight : 1;

		unsigned int uiAlignedWidth = (uiSrcWidth + 3) & ~0x3;		///< 长宽为4的倍数
		unsigned int uiAlignedHeight = (uiSrcHeight + 3) & ~0x3;	///< 
		unsigned int uiMipmapSize = ((uiAlignedWidth * uiAlignedHeight) >> 4) * uiBytesPerBlock;

		pcDataBuffCur += uiMipmapSize;

		uiSrcWidth >> 1;
		uiSrcHeight >> 1;
	}

	// 拷贝数据(目前只支持DXT1/DXT3/DXT5)
	D3DTexturePtr pkD3DTexture = (D3DTexturePtr)m_pkD3DTexture;
	for (unsigned int i = 0; i < m_usLevels - m_uiLevelsSkipped; i++)
	{
		// 获取Mipmap
		D3DSurfacePtr pkMipLevel;
		eD3DRet = pkD3DTexture->GetSurfaceLevel(i, &pkMipLevel);
		if (FAILED(eD3DRet))
		{
			NiDX9Renderer::Error("sdDX9SourceTextureData::CopyStreamDataToSurfaceEx>"
				" Failed GetSurfaceLevel (%d)- %s", i,
				NiDX9ErrorString((unsigned int)eD3DRet));
			return;
		}

		D3DSURFACE_DESC d3dDstSurfDesc;
		eD3DRet = pkMipLevel->GetDesc(&d3dDstSurfDesc);
		if (FAILED(eD3DRet))
		{
			NiDX9Renderer::Error("NiDX9SourceTextureData::CopyStreamDataToSurfaceEx>"
				" Failed GetDesc - %s", 
				NiDX9ErrorString((unsigned int)eD3DRet));
			NiD3DRenderer::ReleaseResource(pkMipLevel);
			return;
		}
		
		// 锁定目标区域
		D3DLOCKED_RECT d3dDstLockRect;
		eD3DRet = pkMipLevel->LockRect(&d3dDstLockRect, NULL, 0);
		if (FAILED(eD3DRet))
		{
			NiDX9Renderer::Error("NiDX9SourceTextureData::CopyDataToSurfaceLevel>"
				" Failed GetSurfaceLevel - %s", 
				NiDX9ErrorString((unsigned int)eD3DRet));
			NiD3DRenderer::ReleaseResource(pkMipLevel);
			return;
		}

		// 拷贝数据
		uiSrcWidth = (uiSrcWidth != 0) ? uiSrcWidth : 1;
		uiSrcHeight = (uiSrcHeight != 0) ? uiSrcHeight : 1;

		unsigned int uiAlignedWidth = (uiSrcWidth + 3) & ~0x3;		///< 长宽为4的倍数
		unsigned int uiAlignedHeight = (uiSrcHeight + 3) & ~0x3;	///< 
		unsigned int uiMipmapSize = ((uiAlignedWidth * uiAlignedHeight) >> 4) * uiBytesPerBlock;

		NiMemcpy(d3dDstLockRect.pBits, pcDataBuffCur, uiMipmapSize);

		pcDataBuffCur += uiMipmapSize;	///< 移动指针到下一级Mipmap

		uiSrcWidth >> 1;
		uiSrcHeight >> 1; 

		// 解锁目标区域
		pkMipLevel->UnlockRect();

		// 释放Mipmap
		NiD3DRenderer::ReleaseResource(pkMipLevel);
	}
}
//-------------------------------------------------------------------------------------------------
bool sdDX9SourceTextureData::LoadTextureFileEx(const char* pcFilename, 
	NiDX9Renderer* pkRenderer, const NiTexture::FormatPrefs& kPrefs, 
	unsigned int uiSkipLevel)
{
	if (pkRenderer == NULL)
		return false;

	LPDIRECT3DDEVICE9 pkD3DDevice = pkRenderer->GetD3DDevice();
	if (pkD3DDevice == NULL)
		return false;

	// 规整纹理路径
	char acStandardFilename[NI_MAX_PATH];
	NiStrcpy(acStandardFilename, NI_MAX_PATH, pcFilename);
	NiPath::Standardize(acStandardFilename);
	NiFilename kFileName(acStandardFilename);
	const char* pcExt = kFileName.GetExt();

	// 根据文件后缀名确定纹理格式
	D3DXIMAGE_FILEFORMAT eFormat = D3DXIFF_FORCE_DWORD;

	if (NiStricmp(pcExt, ".bmp") == 0)
		eFormat = D3DXIFF_BMP;
	else if (NiStricmp(pcExt, ".tga") == 0)
		eFormat = D3DXIFF_TGA;
	else if (NiStricmp(pcExt, ".dds") == 0)
		eFormat = D3DXIFF_DDS;

	if (eFormat == D3DXIFF_FORCE_DWORD)
		return false;

	// 读取文件数据到内存
	NiFile* pkIstr = NiFile::GetFile(acStandardFilename, NiFile::READ_ONLY);
	if ((!pkIstr) || (!(*pkIstr)))
	{
		NiDelete pkIstr;
		return false;
	}

	unsigned int uiBufferLength = pkIstr->GetFileSize();
	if (uiBufferLength == 0)
	{
		NiDelete pkIstr;
		return false;
	}

	unsigned char* pucBuffer = NiAlloc2(unsigned char, uiBufferLength, NiMemHint::TEXTURE);
	pkIstr->Read(pucBuffer, uiBufferLength);

	NiDelete pkIstr;

	// 获取纹理信息
	D3DXIMAGE_INFO kImageInfo;
	HRESULT hr = D3DXGetImageInfoFromFileInMemory((VOID*)pucBuffer, 
		uiBufferLength, &kImageInfo);

	if (FAILED(hr))
	{
		NiFree(pucBuffer);
		return false;
	}

	// Special case: any bump maps not in a signed format
	// must have data massaged in NiPixelData format
	if (kPrefs.m_ePixelLayout == NiTexture::FormatPrefs::BUMPMAP &&
		kImageInfo.Format != D3DFMT_V8U8 &&
		kImageInfo.Format != D3DFMT_Q8W8V8U8 &&
		kImageInfo.Format != D3DFMT_V16U16 &&
		kImageInfo.Format != D3DFMT_Q16W16V16U16 &&
		kImageInfo.Format != D3DFMT_CxV8U8 &&
		kImageInfo.Format != D3DFMT_L6V5U5 &&
		kImageInfo.Format != D3DFMT_X8L8V8U8 &&
		kImageInfo.Format != D3DFMT_A2W10V10U10)
	{
		NiFree(pucBuffer);
		return false;
	}

	// 创建设备纹理
	LPDIRECT3DBASETEXTURE9 pkD3DTexture = NULL;
	if (kImageInfo.ResourceType == D3DRTYPE_TEXTURE)
	{
		LPDIRECT3DTEXTURE9 pk2DTexture = NULL;

		///< ******************************************************************
		///< 修改用于Mipmap控制
		///< @{
		m_uiWidth = kImageInfo.Width;
		m_uiHeight = kImageInfo.Height;

		if (!(NiIsPowerOf2(m_uiWidth) && NiIsPowerOf2(m_uiHeight)) && 
			(m_pkRenderer->GetFlags() & NiRenderer::CAPS_NONPOW2_TEXT) == 0)
		{
			m_usLevels = 1;
		}
		else
		{
			m_usLevels = (unsigned short)kImageInfo.MipLevels;
		}

		if (kPrefs.m_eMipMapped == NiTexture::FormatPrefs::NO)
			m_usLevels = 1;

		unsigned short usSkipLevels = (unsigned short)((uiSkipLevel == 0) ? ms_uiSkipLevels : uiSkipLevel);
		if (usSkipLevels > m_usLevels - 1)
			usSkipLevels = m_usLevels - 1;

		for (unsigned int ui = 0; ui < usSkipLevels; ++ui)
		{
			if (m_uiWidth & 0x1)	///< if odd, add 1 so shifted value rounds up.
				m_uiWidth++;
			m_uiWidth >>= 1;

			if (m_uiHeight & 0x1)	///< if odd, add 1 so shifted value rounds up.
				m_uiHeight++;
			m_uiHeight >>= 1;
		}

		m_uiLevelsSkipped = usSkipLevels;

		//hr = D3DXCreateTextureFromFileInMemory(pkD3DDevice,
		//	(VOID*)pucBuffer, uiBufferLength, &pk2DTexture);

		hr = D3DXCreateTextureFromFileInMemoryEx(
			pkD3DDevice,
			(VOID*)pucBuffer, uiBufferLength,
			m_uiWidth, m_uiHeight, 
			m_usLevels - m_uiLevelsSkipped, 
			0, 
			kImageInfo.Format,
			D3DPOOL_MANAGED,
			D3DX_DEFAULT, D3DX_DEFAULT,
			0, NULL, NULL,
			&pk2DTexture);
		///< @}
		///< ******************************************************************

		pkD3DTexture = (LPDIRECT3DBASETEXTURE9)pk2DTexture;
	}
	else if (kImageInfo.ResourceType == D3DRTYPE_CUBETEXTURE)
	{
		NIASSERT(eFormat == D3DXIFF_DDS);
		LPDIRECT3DCUBETEXTURE9 pkCubeTexture = NULL;

		hr = D3DXCreateCubeTextureFromFileInMemory(pkD3DDevice,
			(VOID*)pucBuffer, uiBufferLength, &pkCubeTexture);

		//hr = D3DXCreateCubeTextureFromFileInMemoryEx(
		//	pkD3DDevice,
		//	(VOID*)pucBuffer, uiBufferLength,
		//	D3DX_DEFAULT, 
		//	kImageInfo.MipLevels, 
		//	0, 
		//	kImageInfo.Format,
		//	D3DPOOL_MANAGED,
		//	D3DX_DEFAULT, D3DX_DEFAULT,
		//	0, NULL, NULL,
		//	&pkCubeTexture);

		pkD3DTexture = (LPDIRECT3DBASETEXTURE9)pkCubeTexture;
	}
	else if (kImageInfo.ResourceType == D3DRTYPE_VOLUMETEXTURE)
	{
		NIASSERT(eFormat == D3DXIFF_DDS);
		LPDIRECT3DVOLUMETEXTURE9 pk3DTexture = NULL;

		hr = D3DXCreateVolumeTextureFromFileInMemory(pkD3DDevice,
			(VOID*)pucBuffer, uiBufferLength, &pk3DTexture);

		//hr = D3DXCreateVolumeTextureFromFileInMemoryEx(
		//	pkD3DDevice,
		//	(VOID*)pucBuffer, uiBufferLength,
		//	D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 
		//	kImageInfo.MipLevels, 
		//	0, 
		//	kImageInfo.Format,
		//	D3DPOOL_MANAGED,
		//	D3DX_DEFAULT, D3DX_DEFAULT,
		//	0, NULL, NULL,
		//	&pk3DTexture);

		pkD3DTexture = (LPDIRECT3DBASETEXTURE9)pk3DTexture;
	}
	else
	{
		hr = D3DERR_INVALIDCALL;
	}

	NiFree(pucBuffer);

	if (FAILED(hr) || pkD3DTexture == NULL)
		return false;

	// 使用设备对象初始化纹理对象信息
	bool bSuccess = InitializeFromD3DTexture(pkD3DTexture);
	if (bSuccess == false)
	{
		NiDX9Renderer::ReleaseTextureResource((D3DTexturePtr)pkD3DTexture);
	}

	return bSuccess;
}
//-------------------------------------------------------------------------------------------------
bool sdDX9SourceTextureData::CheckTextureDimensions(unsigned int uiWidth, 
	unsigned int uiHeight, NiDX9Renderer* pkDX9Renderer)
{
	if (uiWidth > pkDX9Renderer->GetMaxTextureWidth())
		return false;
	
	if (uiHeight > pkDX9Renderer->GetMaxTextureHeight())
		return false;

	unsigned int uiMaxAspectRatio = pkDX9Renderer->GetMaxTextureAspectRatio();
	if (uiMaxAspectRatio > 0)
	{
		if (uiWidth / uiHeight > uiMaxAspectRatio)
			return false;

		if (uiHeight / uiWidth > uiMaxAspectRatio)
			return false;
	}

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdDX9SourceTextureData::CheckTextureFormat(D3DFORMAT eD3DFmt, NiDX9Renderer* pkDX9Renderer)
{
	HRESULT hr = pkDX9Renderer->GetDirect3D()->CheckDeviceFormat(
		pkDX9Renderer->GetAdapter(),
		pkDX9Renderer->GetDevType(),
		pkDX9Renderer->GetAdapterFormat(),
		0,
		D3DRTYPE_CUBETEXTURE,
		eD3DFmt);

	if (SUCCEEDED(hr))
		return true;
	else
		return false;
}
//-------------------------------------------------------------------------------------------------
#pragma warning(pop)
ENGINE_NAMESPACE_END_ENGINE