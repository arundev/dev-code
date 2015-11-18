//*************************************************************************************************
// 内容:	GodRay
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-22
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_GODRAY_H__
#define _SD_ENGINE_RENDER_SYSTEM_GODRAY_H__
#include "sdRenderPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
class sdGodRayPass : public sdRenderPass
{
public:
	sdGodRayPass(sdRenderObjectAlloc* pkAlloc);
	virtual ~sdGodRayPass();

	// 虚函数继承
	virtual void Draw(){};

	// 初始化与销毁
	bool Initialize();
	void Destroy();

	// 功能步骤
	// @{
	// Radial Blur
	void SetRadialBlurInputTexture(NiTexture* spTexture);
	void RadialBlur();
	// @}

protected:
	// 材质
	NiMaterialPtr	m_spRadialBlurMaterial;

	// 渲染属性
	NiPropertyList	m_kPropertyList;

	// 纹理属性
	NiTexturingPropertyPtr	m_spTexProp;
};
NiSmartPointer(sdGodRayPass);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif