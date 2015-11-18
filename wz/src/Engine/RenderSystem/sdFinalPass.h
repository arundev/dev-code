//*************************************************************************************************
// 内容:	合成Pass
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-16
// 最后修改:
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

	// 虚函数继承
	virtual void Draw(){};

	// 初始化与销毁
	bool Initialize();
	void Destroy();

	// 功能步骤
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
	// 材质
	NiMaterialPtr	m_spCopyMaterial;
	NiMaterialPtr	m_spCopyToBackBufferMaterial;
	NiMaterialPtr	m_spCompositeMaterial;

	// 渲染属性
	NiPropertyList	m_kCopyPropertyList;
	NiPropertyList	m_kCopyToBackBufferPropertyList;
	NiPropertyList	m_kCompositeList;

	// 纹理属性
	NiTexturingPropertyPtr	m_spCopyTexProp;
	NiTexturingPropertyPtr	m_spCopyToBackBufferTexProp;
	NiTexturingPropertyPtr	m_spCompositeTexProp;

	// 输入纹理
	NiTexturePtr	m_spRedAlertTexture;
};
NiSmartPointer(sdFinalPass);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif