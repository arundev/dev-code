//*************************************************************************************************
// 内容:	Depth of Field
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-20
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_AO_PASS_H__
#define _SD_ENGINE_RENDER_SYSTEM_AO_PASS_H__
#include "sdRenderPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
class sdAOPass : public sdRenderPass
{
public:
	sdAOPass(sdRenderObjectAlloc* pkAlloc);
	virtual ~sdAOPass();

	// 虚函数继承
	virtual void Draw(){};

	// 初始化与销毁
	bool Initialize(sdPostProcessParams& kParams);
	void Destroy();

	// 功能步骤
	// @{

	// @}
protected:
	// 材质
	NiMaterialPtr	m_spDownSampleMaterial;		
	NiMaterialPtr	m_spOcclusionMaterial;
	NiMaterialPtr	m_spBlurMaterial;
	NiMaterialPtr	m_spApplyMaterial;

	// 渲染属性
	NiPropertyList	m_kDownSamplePropertyList;
	NiPropertyList	m_kOcclusionPropertyList;
	NiPropertyList	m_kBlurPropertyList;
	NiPropertyList	m_kApplyPropertyList;

	// 纹理属性
	NiTexturingPropertyPtr	m_spDownSampleTexProp;
	NiTexturingPropertyPtr	m_spOcclusionTexProp;
	NiTexturingPropertyPtr	m_spBlurTexProp;
	NiTexturingPropertyPtr	m_spApplyTexProp;

	// 后期特效参数
	sdPostProcessParams*	m_pkPostProcessParams;
};
NiSmartPointer(sdAOPass);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif