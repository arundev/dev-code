//*************************************************************************************************
// 内容:	Post Fog
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-20
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_POSTFOG_PASS_H__
#define _SD_ENGINE_RENDER_SYSTEM_POSTFOG_PASS_H__
#include "sdRenderPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
class sdPostFogPass : public sdRenderPass
{
public:
	sdPostFogPass(sdRenderObjectAlloc* pkAlloc);
	virtual ~sdPostFogPass();

	// 虚函数继承
	virtual void Draw();

	// 初始化与销毁
	bool Initialize(NiTexture* spGeometryTexture);
	void Destroy();

protected:
	// 材质
	NiMaterialPtr	m_spFogMaterial;
	NiMaterialPtr	m_spCurveFogMaterial;
	NiMaterialPtr	m_spAOAndFogMaterial;
	NiMaterialPtr	m_spAODebugMaterial;

	// 渲染属性
	NiPropertyList			m_kPropertyList;
	NiTexturingPropertyPtr	m_spTexturingProp;
};
NiSmartPointer(sdPostFogPass);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif