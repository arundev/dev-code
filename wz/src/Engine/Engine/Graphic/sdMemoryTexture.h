//*************************************************************************************************
// 内容:	内存纹理
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-11
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_MEMORY_TEXTURE_H__
#define _SD_ENGINE_MEMORY_TEXTURE_H__

ENGINE_NAMESPACE_BEGIN_ENGINE
//-------------------------------------------------------------------------------------------------
// 直接封装D3D内存纹理
//-------------------------------------------------------------------------------------------------
class sdMemoryTexture : public NiRefObject
{
public:
	sdMemoryTexture(uint uiWidth, uint uiHeight, uint uiLevel, D3DFORMAT eFormat);
	sdMemoryTexture(LPDIRECT3DTEXTURE9 spD3DTexture);
	~sdMemoryTexture();

	// 
	D3DLOCKED_RECT LockRegion(uint uiLevel, uint uiX, uint uiY, uint uiW, uint uiH);
	void UnlockRegion(uint uiLevel);

	// 属性访问
	uint	GetWidth() const { return m_uiWidth;}
	uint	GetHeight() const { return m_uiHeight;}
	uint	GetLevel() const { return m_uiLevel;}
	D3DFORMAT GetFormat() const { return m_eFormat;}
	NiTexture*	GetGBTexture() const { return m_spTexture;}

protected:
	uint		m_uiWidth;
	uint		m_uiHeight;
	uint		m_uiLevel;
	D3DFORMAT	m_eFormat;

	// 纹理
	NiTexturePtr		m_spTexture;	// 用来包装D3D纹理的GB纹理对象
	LPDIRECT3DTEXTURE9	m_spD3DTexture;	// D3D内存纹理
};
NiSmartPointer(sdMemoryTexture);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE
#endif