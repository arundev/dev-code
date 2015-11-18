//*************************************************************************************************
// ����:	
//---------------------------------------------------------
// ����:		
// ����:		2012-02-26
// ����޸�:
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
	// ��ͼ��ʽ��ID���:
	//	1.�����12��ֵ(1 2 4 8 16 32 64 128 256 512 1024 2048),ʹ��12bit
	//	2.mipmap�㼶,ʹ��4bit
	//	3.�����ʽformat(DXT1\DXT3\DXT5,��֧��AlphaԤ�˵�DXT2��DXT4),ʹ��2bit
	//	4.��Դ����type(D3DRTYPE_TEXTURE = 0, D3DRTYPE_VOLUMETEXTURE = 1, D3DRTYPE_CUBETEXTURE = 2),ʹ��2bit
	// ��16bit, msb* mipmap-h-w-fmt-type *lsb,�������ڴ˷������ͼ���������
	static uint GenerateTextureFormatID(NiPersistentSrcTextureRendererData* spPersistentData);
	static uint GenerateTextureFormatID(NiPixelData* spPixelData);
	static uint GenerateTextureFormatID(IDirect3DBaseTexture9* d3dTexture);
	static uint GenerateTextureFormatID(D3DRESOURCETYPE d3dType, uint uiLodLevel, D3DFORMAT d3dFormat, uint uiWidth, uint uiHeight);
};
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE
#endif