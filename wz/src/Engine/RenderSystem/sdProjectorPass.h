//*************************************************************************************************
// 内容:	Projector渲染
//---------------------------------------------------------
// 作者:		
// 创建:		2012-12-07
// 最后修改:	2014-04-20
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_PROJECTOR_PASS_H__
#define _SD_ENGINE_RENDER_SYSTEM_PROJECTOR_PASS_H__
#include "sdRenderPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
class sdProjectorPass : public sdRenderPass
{
public:
	sdProjectorPass(sdRenderObjectAlloc* pkAlloc);
	virtual ~sdProjectorPass();

	// 虚函数继承(继承自sdRenderPass)
	virtual void Draw();

	// 初始化与销毁
	bool Initialize(
		uint uiProjectorMeshStencilID, 
		uint uiProjectorMeshStencilIDMask,
		NiRenderedTexture* spGeometryTexture,
		NiRenderedTexture* spLightTexture);
	void Destroy();

	// 添加绘制对象
	void InsertProjector(sdProjectorSceneObject* pkProjector);

protected:
	uint m_uiProjectorMeshStencilID;			///< 投射体模型模板参考值
	uint m_uiProjectorMeshStencilIDMask;		///< 投射体模型模板掩码

	typedef std::vector<NiMaterialPtr> MaterialVec;
	typedef std::vector<NiMaterialPtr>::iterator MaterialVecItr;
	MaterialVec	m_kMaterialVec;					///< 材质

	NiRenderedTexturePtr m_spGeometryTexture;
	NiRenderedTexturePtr m_spLightTexture;

	NiPropertyList m_kPropertyList;
	NiTexturingPropertyPtr m_spTexturingProp;
};
NiSmartPointer(sdProjectorPass);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif