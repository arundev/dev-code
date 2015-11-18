//*************************************************************************************************
// ����:	�ڴ�����
//---------------------------------------------------------
// ����:		
// ����:		2012-08-11
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_MEMORY_TEXTURE_H__
#define _SD_ENGINE_MEMORY_TEXTURE_H__

ENGINE_NAMESPACE_BEGIN_ENGINE
//-------------------------------------------------------------------------------------------------
// ֱ�ӷ�װD3D�ڴ�����
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

	// ���Է���
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

	// ����
	NiTexturePtr		m_spTexture;	// ������װD3D�����GB�������
	LPDIRECT3DTEXTURE9	m_spD3DTexture;	// D3D�ڴ�����
};
NiSmartPointer(sdMemoryTexture);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE
#endif