//*************************************************************************************************
// ����:	�ϳ�Pass
//---------------------------------------------------------
// ����:		
// ����:		2012-10-16
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_FINAL_PASS_H__
#define _SD_ENGINE_RENDER_SYSTEM_FINAL_PASS_H__
#include "sdRenderPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
class sdFinalPass : public sdRenderPass
{
public:
	sdFinalPass(sdRenderObjectAlloc* pkAlloc);
	virtual ~sdFinalPass();

	// �麯���̳�
	virtual void Draw(){};

	// ��ʼ��������
	bool Initialize();
	void Destroy();

	// ���ܲ���
	// @{
	// Copy
	void SetCopyInputTexture(NiTexture* spTexture);
	void Copy();

	// Copy-To-BackBuffer
	void SetCopyToBackBufferInputTexture_ColorTexture(NiTexture* spTexture);
	void CopyToBackBuffer();

	// Composite
	void SetCompositeInputtexture_ColorTexture(NiTexture* spTexture);
	void SetCompositeInputtexture_WeightTexture(NiTexture* spTexture);
	void SetCompositeInputtexture_DofTexture(NiTexture* spTexture);
	void SetCompositeInputtexture_GodRayTexture(NiTexture* spTexture);
	void SetCompositeInputtexture_BloomTexture(NiTexture* spTexture);
	void Composite();
	// @}
protected:
	// ����
	NiMaterialPtr	m_spCopyMaterial;
	NiMaterialPtr	m_spCopyToBackBufferMaterial;
	NiMaterialPtr	m_spCompositeMaterial;

	// ��Ⱦ����
	NiPropertyList	m_kCopyPropertyList;
	NiPropertyList	m_kCopyToBackBufferPropertyList;
	NiPropertyList	m_kCompositeList;

	// ��������
	NiTexturingPropertyPtr	m_spCopyTexProp;
	NiTexturingPropertyPtr	m_spCopyToBackBufferTexProp;
	NiTexturingPropertyPtr	m_spCompositeTexProp;

	// ��������
	NiTexturePtr	m_spRedAlertTexture;
};
NiSmartPointer(sdFinalPass);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif