//*************************************************************************************************
// 内容:	
//---------------------------------------------------------
// 作者:		
// 创建:		2012-02-26
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_TEXTURE_CACHE_H__
#define _SD_ENGINE_TEXTURE_CACHE_H__

ENGINE_NAMESPACE_BEGIN_ENGINE
//-------------------------------------------------------------------------------------------------
class sdTextureCache : public NiMemObject
{
public:
	sdTextureCache();
	~sdTextureCache();

	//


protected:
	

protected:
	// 贴图格式的ID组成:
	//	1.长宽各12种值(1 2 4 8 16 32 64 128 256 512 1024 2048),使用12bit
	//	2.mipmap层级,使用4bit
	//	3.纹理格式format(DXT1\DXT3\DXT5,不支持Alpha预乘的DXT2和DXT4),使用2bit
	//	4.资源类型type(D3DRTYPE_TEXTURE = 0, D3DRTYPE_VOLUMETEXTURE = 1, D3DRTYPE_CUBETEXTURE = 2),使用2bit
	// 共16bit, msb* mipmap-h-w-fmt-type *lsb,任务不属于此范畴的贴图将不予加载
	static uint GenerateTextureFormatID(NiPersistentSrcTextureRendererData* spPersistentData);
	static uint GenerateTextureFormatID(NiPixelData* spPixelData);
	static uint GenerateTextureFormatID(IDirect3DBaseTexture9* d3dTexture);
	static uint GenerateTextureFormatID(D3DRESOURCETYPE d3dType, uint uiLodLevel, D3DFORMAT d3dFormat, uint uiWidth, uint uiHeight);
};
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE
#endif