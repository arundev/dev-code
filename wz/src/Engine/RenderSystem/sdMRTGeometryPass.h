//*************************************************************************************************
// 内容:	物件Geometry-Buffer的渲染
//---------------------------------------------------------
// 作者:		
// 创建:		2012-07-14
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_MRT_GEOMETRY_PASS_H__
#define _SD_ENGINE_RENDER_SYSTEM_MRT_GEOMETRY_PASS_H__
#include "sdRenderPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
class sdMRTGeometryPass : public sdRenderPass
{
public:
	sdMRTGeometryPass(sdRenderObjectAlloc* pkAlloc);
	virtual ~sdMRTGeometryPass();

	// 虚函数继承
	virtual void Draw();

	// 初始化与销毁
	bool Initialize(uint uiStencilID, uint uiStencilIDWriteMask);
	void Destroy();

	// 添加绘制对象
	void InsertStaticMesh(NiMesh* spMesh);

protected:
	uint	m_uiStencilID;
	uint	m_uiStencilIDWriteMask;

	typedef std::vector<NiMaterialPtr> MaterialVec;
	typedef std::vector<NiMaterialPtr>::iterator MaterialVecItr;
	MaterialVec	m_kStaticGeometryMaterials;		///< 静态Mesh材质
	MaterialVec	m_kSkinnedGeometryMaterials;	///< 带骨骼Mesh材质

	NiPropertyList m_kPropertyList;
	NiTexturingPropertyPtr m_spTexturingProp;
};
NiSmartPointer(sdMRTGeometryPass);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif