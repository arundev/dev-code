//*************************************************************************************************
// 内容:	Terrain深度预渲染到ZBuffer
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-04
// 最后修改:	2012-12-12
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_TERRAIN_DEPTH_PASS_H__
#define _SD_ENGINE_RENDER_SYSTEM_TERRAIN_DEPTH_PASS_H__
#include "sdRenderPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
// 1.渲染地形深度到一张纹理
// 2.渲染地形到模板缓存
//-------------------------------------------------------------------------------------------------
class sdTerrainDepthPass : public sdRenderPass
{
public:
	sdTerrainDepthPass(sdRenderObjectAlloc* pkAlloc);
	virtual ~sdTerrainDepthPass();

	// 虚函数继承
	virtual void Draw();

	// 初始化与销毁
	bool Initialize(uint uiTerraiStencilID, uint uiTerrainStencilIDWriteMask);
	void Destroy();

	// 添加绘制对象
	void InsertTerrainMesh(NiMesh* spMesh);

protected:
	//@{
	// 虚函数继承
	virtual RenderObjectComparator GetComparator();

	// 定制的排序比较器
	static bool	Comparator(sdRenderObject* lhs, sdRenderObject* rhs);
	//@}

protected:
	// 模版参数
	uint	m_uiTerrainStencilID;
	uint	m_uiTerrainStencilIDWriteMask;

	// 材质
	NiMaterialPtr	m_spTerrainMaterial;
};
NiSmartPointer(sdTerrainDepthPass);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif