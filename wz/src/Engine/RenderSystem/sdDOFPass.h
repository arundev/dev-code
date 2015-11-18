//*************************************************************************************************
// 内容:	Depth of Field
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-20
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_DOF_PASS_H__
#define _SD_ENGINE_RENDER_SYSTEM_DOF_PASS_H__
#include "sdRenderPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
class sdDOFPass : public sdRenderPass
{
public:
	sdDOFPass(sdRenderObjectAlloc* pkAlloc);
	virtual ~sdDOFPass();

	// 虚函数继承
	virtual void Draw(){};

	// 初始化与销毁
	bool Initialize(sdPostProcessParams& kParams);
	void Destroy();

	// 功能步骤
	// @{
	// Down Sample
	void SetDownSampleInputTexture(NiTexture* spTexture);
	void DownSample();

	// BlurX
	void SetBlurXInputTexture(NiTexture* spTexture);
	void BlurX();

	// BlurY
	void SetBlurYInputTexture(NiTexture* spTexture);
	void BlurY();
	// @}
protected:
	// 材质
	NiMaterialPtr	m_spDownSampleMaterial;		// 1/2向下采样
	NiMaterialPtr	m_spSeparableBlurMaterial;	// 一维过滤

	// 渲染属性
	NiPropertyList	m_kPropertyList;

	// 纹理属性
	NiTexturingPropertyPtr	m_spTexProp;

	// 后期特效参数
	sdPostProcessParams*	m_pkPostProcessParams;
};
NiSmartPointer(sdDOFPass);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif