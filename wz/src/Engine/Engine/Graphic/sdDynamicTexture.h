//*************************************************************************************************
// ����:	��̬����
//---------------------------------------------------------
// ����:		
// ����:		2012-08-07
// ����޸�:	2013-05-15
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_DYNAMIC_TEXTURE_H__
#define _SD_ENGINE_DYNAMIC_TEXTURE_H__
#include "sdTexture.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
//-------------------------------------------------------------------------------------------------
// ֱ�ӷ�װD3D����
//	1.�����GB��NiDynamicTexture����Ĳ��ܲ��ָ��µ�����
//	2.��ͬ����D3D�ڴ�����,��ͬ����D3D�Դ�����(ͬ��)
//	3.�豸��ʧ������������д���
//	4.�̳���NiTexture��֤�豸����ʱ�ᱻ֪ͨ
// 
//	(���������������ܱ���Ƕ�뵽NiDX9Renderer�ڲ�ȥ,��������)
//	WZ��һ���汾�����޸�NiSourceTexture,��NiPixelData������NiSourceTexture,
//	ͨ����̬����NiPixelData������
//		NiPixelDataPtr		m_spPixelData;
//		NiSourceTexturePtr	m_spTexture;
//-------------------------------------------------------------------------------------------------
class sdDynamicTexture : public sdTexture
{
public:
	// ��ǰ����֧���������ָ�ʽ
	enum eDynamicFormat
	{
		E_FORMAT_UNKNOWN = 0,	// Ĭ��ʹ��32λ��ʽ
		E_FORMAT_A8,
		E_FORMAT_A4R4G4B4,
		E_FORMAT_R8G8B8,
		E_FORMAT_A8R8G8B8,
		NUM_FORMATS,
	};

	// ��������ķ���ֵ(��D3D)
	// (D3D���صĲ���һ����������,����pBits��ʼ��һ������,���о�ΪuiPitch)
	struct sdLockedRect
	{
		uint	uiPitch;
		void*	pBits;
	};
	
public:
	sdDynamicTexture(uint uiWidth, uint uiHeight, eDynamicFormat eFormat = E_FORMAT_UNKNOWN);
	~sdDynamicTexture();

	// 
	sdLockedRect LockRegion(uint uiX, uint uiY, uint uiW, uint uiH);
	void UnlockRegion();

	//
	uint	GetWidth() const { return m_uiWidth;}
	uint	GetHeight() const { return m_uiHeight;}
	eDynamicFormat GetFormat() const { return m_eFormat;}
	NiTexture* GetGBTexture() const { return m_spTexture;}
	LPDIRECT3DTEXTURE9 GetSysMemTexture() const { return m_spD3DSysTexture;}
	LPDIRECT3DTEXTURE9 GetVidMemTexture() const { return m_spD3DVidTexture;}

protected:
	// �豸�ص�����
	virtual bool OnDeviceLost();
	virtual bool OnDeviceReset(bool bBefore);

protected:
	uint	m_uiWidth;
	uint	m_uiHeight;
	bool	m_uiLevel;
	eDynamicFormat m_eFormat;	///< ֧���޶���ʽ

	// ����
	NiTexturePtr		m_spTexture;		///< ������װD3D�����GB�������
	LPDIRECT3DTEXTURE9	m_spD3DSysTexture;	///< D3D�ڴ�����,ֱ�ӱ����µ�����
	LPDIRECT3DTEXTURE9	m_spD3DVidTexture;	///< D3D�Դ�����,���ձ����º�ʹ�õ�����

	// ��Ⱦ�豸
	NiDX9Renderer*		m_spRenderer;
	LPDIRECT3DDEVICE9	m_spD3DDevice;

	// D3D��ʽ���ұ��
	static D3DFORMAT ms_kD3DFormatTable[NUM_FORMATS];
};
NiSmartPointer(sdDynamicTexture);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE
#endif