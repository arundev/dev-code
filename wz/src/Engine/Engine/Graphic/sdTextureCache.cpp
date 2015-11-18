#include "sdEnginePCH.h"
#include "sdTextureCache.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
//-------------------------------------------------------------------------------------------------
uint sdTextureCache::GenerateTextureFormatID(NiPersistentSrcTextureRendererData* spPersistentData)
{
	NIASSERT(spPersistentData);

	D3DRESOURCETYPE d3dType = D3DRTYPE_TEXTURE;
	uint uiLodLevel = spPersistentData->GetNumMipmapLevels();
	uint uiWidth = spPersistentData->GetWidth();
	uint uiHeight = spPersistentData->GetHeight();

	const NiPixelFormat kPixelFormat = spPersistentData->GetPixelFormat();
	D3DFORMAT d3dFmt = NiDX9PixelFormat::DetermineD3DFormat(kPixelFormat);

	return GenerateTextureFormatID(d3dType, uiLodLevel, d3dFmt, uiWidth,uiHeight);
}
//-------------------------------------------------------------------------------------------------
uint sdTextureCache::GenerateTextureFormatID(NiPixelData* spPixelData)
{
	NIASSERT(spPixelData);

	D3DRESOURCETYPE d3dType = D3DRTYPE_TEXTURE;
	uint uiLodLevel = spPixelData->GetNumMipmapLevels();
	uint uiWidth = spPixelData->GetWidth();
	uint uiHeight = spPixelData->GetHeight();

	const NiPixelFormat kPixelFormat = spPixelData->GetPixelFormat();
	D3DFORMAT d3dFmt = NiDX9PixelFormat::DetermineD3DFormat(kPixelFormat);

	return GenerateTextureFormatID(d3dType, uiLodLevel, d3dFmt, uiWidth,uiHeight);
}
//-------------------------------------------------------------------------------------------------
uint sdTextureCache::GenerateTextureFormatID(IDirect3DBaseTexture9* d3dTexture)
{
	NIASSERT(d3dTexture);

	D3DRESOURCETYPE d3dType = d3dTexture->GetType();
	uint uiLodLevel = d3dTexture->GetLevelCount();

	D3DSURFACE_DESC d3dDesc;
	((IDirect3DTexture9*)d3dTexture)->GetLevelDesc(0, &d3dDesc);

	return GenerateTextureFormatID(d3dType, uiLodLevel, d3dDesc.Format, d3dDesc.Width, d3dDesc.Height);
}
//-------------------------------------------------------------------------------------------------
uint sdTextureCache::GenerateTextureFormatID(D3DRESOURCETYPE d3dType, uint uiLodLevel, D3DFORMAT d3dFormat, uint uiWidth, uint uiHeight)
{
	NIASSERT(d3dType == D3DRTYPE_TEXTURE);	///< 目前支持持普通纹理
	NIASSERT(uiLodLevel != 0);				///< Mipmap层数不能为0
	NIASSERT(NiIsPowerOf2(uiWidth));		///< 长宽必须为2的幂次
	NIASSERT(NiIsPowerOf2(uiHeight));		

	uint uiID = 0;
	
	switch (d3dType)
	{
	case D3DRTYPE_TEXTURE:
		uiID |= 1;
		break;
	case D3DRTYPE_VOLUMETEXTURE:
	case D3DRTYPE_CUBETEXTURE:
	default:
		return 0;
	};
	
	switch (d3dFormat)
	{
	case D3DFMT_DXT1:
		uiID |= 1 << 1;
		break;
	case D3DFMT_DXT3:
		uiID |= 2 << 2;
		break;
	case D3DFMT_DXT5:
		uiID |= 3 << 2;
		break;
	default:
		return 0;
	};

	uiID |= (NiFastLog(uiWidth) + 1) << 4;
	uiID |= (NiFastLog(uiWidth) + 1) << 8;
	uiID |= uiLodLevel << 4;

	return uiID;
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE