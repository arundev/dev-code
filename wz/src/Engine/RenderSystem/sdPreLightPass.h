//*************************************************************************************************
// 内容:	延迟Light的渲染
//---------------------------------------------------------
// 作者:		
// 创建:		2012-07-20
// 最后修改:	2014-04-11
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_PRELIGHT_PASS_H__
#define _SD_ENGINE_RENDER_SYSTEM_PRELIGHT_PASS_H__
#include "sdRenderPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
class sdPreLightPass : public sdRenderPass
{
public:
	sdPreLightPass(sdRenderObjectAlloc* pkAlloc);
	virtual ~sdPreLightPass();

	// 虚函数继承(继承自sdRenderPass)
	virtual void Draw();

	// 初始化与销毁
	bool Initialize(
		uint uiLightVolumeStencilID, 
		uint uiLightVolumeStencilIDMask,
		uint uiLightVolumeStencilIDWriteMask,
		NiRenderedTexture* pkGeomTexture);
	void Destroy();

	// 添加绘制对象
	void InsertLight(sdLightSceneObject* pkLight);

protected:
	uint m_uiLightVolumeStencilID;				///< 灯光体模型模板参考值
	uint m_uiLightVolumeStencilIDMask;			///< 灯光体模型模板掩码
	uint m_uiLightVolumeStencilIDWriteMask;		///< 灯光体模型模板写入掩码

	NiMaterialPtr	m_spPointLightMaterial;		///< 点光源材质
	NiMaterialPtr	m_spSpotLightMaterial;		///< 聚光灯材质
	NiMaterialPtr	m_spRectSpotLightMaterial;	///< 聚光灯材质

	NiRenderedTexturePtr m_spGeometryTexture;

	NiPropertyList m_kPropertyList;
	NiTexturingPropertyPtr m_spTexturingProp;
};
NiSmartPointer(sdPreLightPass);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif