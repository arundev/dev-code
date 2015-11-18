//*************************************************************************************************
// 内容:	物件着色
//---------------------------------------------------------
// 作者:		
// 创建:		2012-07-14
// 最后修改:	2014-05-05
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_MRT_SHADING_PASS_H__
#define _SD_ENGINE_RENDER_SYSTEM_MRT_SHADING_PASS_H__
#include "sdRenderPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
class sdMRTShadingPass : public sdRenderPass
{
public:
	sdMRTShadingPass(sdRenderObjectAlloc* pkAlloc);
	virtual ~sdMRTShadingPass();

	// 虚函数继承
	virtual void Draw();

	// 初始化与销毁
	//	@param[in]	uiStaticMeshStencilID		待渲染对象模板ID
	//	@param[in]	uiStaticMeshStencilIDMask	待渲染对象模板ID掩码
	//	@param[in]	spLightTexture				屏幕局部光照缓存
	//	@param[in]	spGeomTexture				屏幕法线与深度缓存
	//	@param[in]	spAlbedoTexture				屏幕漫反射缓存
	//	@param[in]	spGlossTexture				屏幕高光缓存
	bool Initialize(uint uiStaticMeshStencilID, 
		uint uiStaticMeshStencilIDMask,
		NiRenderedTexture* pkLightTexture, 
		NiRenderedTexture* pkGeomTexture,
		NiRenderedTexture* pkAlbedoTexture,	
		NiRenderedTexture* pkGlossTexture, 
		NiRenderedTexture* pkGlowTexture);
	void Destroy();

protected:
	uint m_uiStaticMeshStencilID;
	uint m_uiStaticMeshStencilIDMask;

	typedef std::vector<NiMaterialPtr> MaterialVec;
	typedef std::vector<NiMaterialPtr>::iterator MaterialVecItr;
	MaterialVec	m_kStaticShadingMaterials;		///< 着色材质

	NiRenderedTexturePtr m_spLightTexture;
	NiRenderedTexturePtr m_spGeomTexture;
	NiRenderedTexturePtr m_spAlbedoTexture;
	NiRenderedTexturePtr m_spGlossTexture;
	NiRenderedTexturePtr m_spGlowTexture;

	NiPropertyList m_kPropertyList;
	NiTexturingPropertyPtr m_spTexturingProp;
};
NiSmartPointer(sdMRTShadingPass);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif