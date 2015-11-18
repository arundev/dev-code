//*************************************************************************************************
// 内容:	Terrain到Geometry-Buffer的渲染
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-06
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_TERRAIN_ATLAS_GEOMETRY_PASS_H__
#define _SD_ENGINE_RENDER_SYSTEM_TERRAIN_ATLAS_GEOMETRY_PASS_H__
#include "sdRenderPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
// 地形渲染到几何缓存
//-------------------------------------------------------------------------------------------------
class sdTerrainAtlasGeometryPass : public sdRenderPass
{
public:
	sdTerrainAtlasGeometryPass(sdRenderObjectAlloc* pkAlloc);
	virtual ~sdTerrainAtlasGeometryPass();

	// 虚函数继承
	virtual void Draw();

	// 初始化与销毁
	bool Initialize(
		uint uiTerrainStencilID, 
		uint uiTerrainStencilIDMask,
		uint uiFarTerrainStencilIDShift,
		NiRenderedTexture* spDepthTexture);
	void Destroy();

	// 设置地形渲染信息(用于更新内部的一些贴图信息)
	void SetTerrainParams(const sdTerrainParams& kTerrainParams);

protected:
	// 模版参数
	uint	m_uiTerrainStencilID;
	uint	m_uiTerrainStencilIDMask;
	uint	m_uiFarTerrainStencilIDShift;

	// 只渲染基本法线材质
	NiMaterialPtr	m_spBaseNormalOnlyMaterial;

	// 透视投影相机材质:
	//	1.近处渲染基本法线和法线贴图的材质
	//	2.远处渲染基本法线的材质
	NiMaterialPtr	m_spFarBaseNormalMaterial;
	NiMaterialPtr	m_spNearNormalMapMaterial;

	// 正射投影相机材质:
	//	1.近处渲染基本法线和法线贴图的材质
	//	2.远处渲染基本法线的材质
	NiMaterialPtr	m_spOrthoFarBaseNormalMaterial;
	NiMaterialPtr	m_spOrthoNearNormalMapMaterial;

	// 地形深度纹理
	NiRenderedTexturePtr	m_spDepthTexture;

	// 立方体贴图,用于确定地表贴图投影方向
	NiSourceCubeMapPtr		m_spUVTableCubeMap;

	// 地形渲染属性
	NiPropertyList			m_kPropertyList;
	NiTexturingPropertyPtr	m_spTexturingProp;
};
NiSmartPointer(sdTerrainAtlasGeometryPass);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif