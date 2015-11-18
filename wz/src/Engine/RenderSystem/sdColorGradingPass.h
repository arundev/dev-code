//*************************************************************************************************
// 内容:	Color Grading
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-13
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_COLORGRADING_PASS_H__
#define _SD_ENGINE_RENDER_SYSTEM_COLORGRADING_PASS_H__
#include "sdRenderPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
class sdColorGradingPass : public sdRenderPass
{
public:
	sdColorGradingPass(sdRenderObjectAlloc* pkAlloc);
	virtual ~sdColorGradingPass();

	// 虚函数继承
	virtual void Draw();

	// 初始化与销毁
	bool Initialize();
	void Destroy();

	// 渲染设置
	void	SetInputSceneTexture(NiTexture* spTexture);
	void	SetInputGeomTexture(NiTexture* spTexture);

protected:
	// 材质
	NiMaterialPtr			m_spColorGradingMaterial;	

	// 渲染属性
	NiPropertyList			m_kPropertyList;
	NiTexturingPropertyPtr	m_spTexturingProp;

	// 输入纹理
	NiTexturePtr	m_spSceneTexture;
	NiTexturePtr	m_spGeomTexture;
	NiTexturePtr	m_spBackgroundLUTTexture;	// 背景LUT查找纹理
	NiTexturePtr	m_spForegroundLUTTexture;	// 前景LUT查找纹理
};
NiSmartPointer(sdColorGradingPass);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif