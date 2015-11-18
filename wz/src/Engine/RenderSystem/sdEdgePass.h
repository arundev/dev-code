//*************************************************************************************************
// 内容:	勾边
//---------------------------------------------------------
// 作者:		
// 创建:		2013-05-30
// 最后修改:	
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_EDGE_PASS_H__
#define _SD_ENGINE_RENDER_SYSTEM_EDGE_PASS_H__
#include "sdRenderPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
class sdEdgePass : public sdRenderPass
{
public:
	sdEdgePass(sdRenderObjectAlloc* pkAlloc);
	virtual ~sdEdgePass();

	// 虚函数继承
	virtual void Draw();

	// 初始化与销毁
	bool Initialize(uint uiEdgeStencilID, uint uiEdgeStencilIDMask, uint uiEdgeStencilIDWriteMask);
	void Destroy();

	// 添加绘制对象
	void InsertEdgeMesh(NiMesh* pkMesh);

protected:
	uint m_uiEdgeStencilID;				///< 勾边模板参考值
	uint m_uiEdgeStencilIDMask;			///< 勾边模板掩码
	uint m_uiEdgeStencilIDWriteMask;	///< 勾边模板写入掩码
	
	typedef std::vector<NiMaterialPtr> MaterialVec;
	typedef std::vector<NiMaterialPtr>::iterator MaterialVecItr;
	MaterialVec	m_kMaterialVec;			///< 材质
};
NiSmartPointer(sdEdgePass);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif