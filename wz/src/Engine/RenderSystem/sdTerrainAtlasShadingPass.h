//*************************************************************************************************
// 内容:	Terrain着色
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-09
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_TERRAIN_ATLAS_SHADING_PASS_H__
#define _SD_ENGINE_RENDER_SYSTEM_TERRAIN_ATLAS_SHADING_PASS_H__
#include "sdRenderPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
// 地形渲染到几何缓存
//-------------------------------------------------------------------------------------------------
class sdTerrainAtlasShadingPass : public sdRenderPass
{
public:
	sdTerrainAtlasShadingPass(sdRenderObjectAlloc* pkAlloc);
	virtual ~sdTerrainAtlasShadingPass();

	// 虚函数继承
	virtual void Draw();

	// 初始化与销毁
	bool Initialize(
		uint uiStencilRef, 
		uint uiStencilMask,
		NiRenderedTexture* spLightTexture, 
		NiRenderedTexture* spGeomTexture);
	void Destroy();

	// 设置地形渲染信息(用于更新内部的一些贴图信息)
	void SetTerrainParams(const sdTerrainParams& kTerrainParams);

protected:
	// 模版参数
	uint	m_uiStencilRef;
	uint	m_uiStencilMask;

	// 材质
	NiMaterialPtr	m_spSimpleShadingMaterial;	
	NiMaterialPtr	m_spPlanarShadingMaterial;
	NiMaterialPtr	m_spSeamShadingMaterial;
	NiMaterialPtr	m_spOrthoShadingMaterial;	// 正射投影时的着色材质

	//NiMaterialPtr	m_spDebugSimpleShadingMaterial;	
	//NiMaterialPtr	m_spDebugPlanarShadingMaterial;
	//NiMaterialPtr	m_spDebugSeamShadingMaterial;
	//NiMaterialPtr	m_spDebugOrthoShadingMaterial;

	// 着色输入纹理
	NiRenderedTexturePtr	m_spLightTexture;
	NiRenderedTexturePtr	m_spGeomTexture;

	// 立方体贴图,用于确定地表贴图投影方向
	NiSourceCubeMapPtr		m_spUVTableCubeMap;

	// 地形渲染属性
	NiPropertyList			m_kPropertyList;
	NiTexturingPropertyPtr	m_spTexturingProp;
};
NiSmartPointer(sdTerrainAtlasShadingPass);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif