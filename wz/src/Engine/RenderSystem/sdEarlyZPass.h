//*************************************************************************************************
// 内容:	EarlyZ的渲染
//---------------------------------------------------------
// 作者:		
// 创建:		2012-07-10
// 最后修改:	2012-12-12
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_EARLYZ_PASS_H__
#define _SD_ENGINE_RENDER_SYSTEM_EARLYZ_PASS_H__
#include "sdRenderPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
// 仅仅写入Depth-Stencil Buffer,可以获取双倍写入速度
// 
// \class sdEarlyZPass
// 
// The Early-Z Pass only writes depth-stencil buffer of the specified render target group.
//-------------------------------------------------------------------------------------------------
class sdEarlyZPass : public sdRenderPass
{
public:
	sdEarlyZPass(sdRenderObjectAlloc* pkAlloc);
	virtual ~sdEarlyZPass();

	// 虚函数继承
	virtual void Draw();

	// 初始化与销毁
	bool Initialize(uint uiStaticMeshStencilID, uint uiStaticMeshStencilIDWriteMask);
	void Destroy();

	// 添加绘制对象
	void InsertStaticMesh(NiMesh* spMesh);

protected:
	//@{
	// 虚函数继承
	virtual RenderObjectComparator GetComparator();

	// 定制的排序比较器
	static bool	Comparator(sdRenderObject* lhs, sdRenderObject* rhs);
	//@}

protected:
	// 模版参数
	uint	m_uiStaticMeshStencilID;
	uint	m_uiStaticMeshStencilIDMask;

	// 材质
	NiMaterialPtr	m_spSolidMeshZMaterial;
	NiMaterialPtr	m_spNormalMapAlphaTestZMaterial;
	NiMaterialPtr	m_spDiffuseMapAlphaTestZMaterial;	
};
NiSmartPointer(sdEarlyZPass);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif