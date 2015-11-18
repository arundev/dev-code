//*************************************************************************************************
// 内容:	基于DX9的Gamebryo的延迟渲染路径
//---------------------------------------------------------
// 作者:		
// 创建:		2012-07-05
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_RENDER_PATH_DX9_DEFERRED_H__
#define _SD_ENGINE_RENDER_SYSTEM_RENDER_PATH_DX9_DEFERRED_H__
#include "sdRenderPath.h"
#include "sdRenderObjectAlloc.h"

#include "sdEarlyZPass.h"

#include "sdMRTGeometryPass.h"
#include "sdMRTShadingPass.h"

#include "sdTerrainDepthPass.h"
#include "sdTerrainAtlasGeometryPass.h"
#include "sdTerrainAtlasShadingPass.h"

#include "sdPreLightPass.h"
#include "sdProjectorPass.h"

#include "sdAOPass.h"
#include "sdMLAAPass.h"
#include "sdColorGradingPass.h"
#include "sdDOFPass.h"
#include "sdGodRayPass.h"
#include "sdHDRPass.h"
#include "sdFinalPass.h"

#include "sdEdgePass.h"
#include "sdEdgeEnhancementPass.h"

#include "sdDebugGizmoPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
// 使用Gamebryo的DX9实现的延迟着色渲染路径(内部具体的渲染由各个RenderPass来实现)
//-----------------------------------------------------------------------------
// Mesh Early-Z Pass	:	Draw opaque static/skinned meshes, write z, disable color output for 
//							double ROP speed.
//
// Terrain Depth Pass	:	Draw terrain mesh without material, write z, output linear depth to 
//							color render target.
//
// Terrain Geometry Pass:	Draw full-screen quad,disable z write, read linear depth, convert to 
//							uv, output terrain's linear depth and normal.
//
// Mesh Geometry Pass	:	Draw opaque static/skinned meshes, disable z write, output linear
//							depth and normal
//
// Pre-Light Pass		:	Draw light volumes, disable z write, use stencil test, read depth and
//							normal, calculate diffuse light color and speular light's luminance
//
// Terrain Shading Pass	:	Draw full-screen quad, disable z write, read linear terrain depth and 
//							normal, read light buffer, calculate uv, blend color, calculate full 
//							range light and light buffer light, output terrain's final color.
//							
// Shading Pass			:	Draw opaque static/skinned meshes, , disable z write, read light buffer, 
//							read ifself's normal/diffuse texture, calculate full range light and 
//							light buffer light, output opaque mesh's final color.
//
//-----------------------------------------------------------------------------
// Vertex shader loading:
//		
// Pixel shader loading:
//
// Texture bandwidth loading:
//
// ROP loading:
//-------------------------------------------------------------------------------------------------
class sdRenderPath_DX9	: public sdRenderPath
{
public:
	// 各种类型Mesh的StencilID,用于标定Geometry-Buffer的对象类型
	enum eMeshStencilID
	{
		E_SID_SKY			= 0,	///< 0000 0000
		E_SID_TERRAIN		= 1,	///< 0000 0001
		E_SID_STATIC_MESH	= 2,	///< 0000 0010
		E_SID_SKINNED_MESH	= 3,	///< 0000 0011
		E_SID_DECAL			= 4,	///< 0000 0100
		E_SID_LEAF			= 5,	///< 0000 0101
		E_SID_GRASS			= 6,	///< 0000 0110
		E_SID_WATER			= 7,	///< 0000 0111

		E_SID_MASK			= 0x07,	///< 0000 0111 
	};

	// 一些特殊掩码位(与普通Stencil组合使用)
	enum ePixelStencilBit
	{
		E_PSB_RECEIVE_DECAL_SHIFT		= 0x3,	///< 用于标记可贴花区域的Bit位置, 0000 1000
		E_PSB_DISTANCE_TERRAIN_SHIFT	= 0x4,	///< 用于标记远处地形的Bit位置,   0001 0000
		E_PSB_DEFERED_LIGHT_SHIFT		= 0x5,	///< 用于标记延迟光照的Bit位置,   0010 0000
		E_PSB_EDGE_SHIFT				= 0x6,	///< 用于标记勾边的Bit位置,		  0100 0000
	};


public:
	sdRenderPath_DX9();
	virtual ~sdRenderPath_DX9();

	// 虚函数继承(继承自IRenderPath)
	virtual bool Initialize();
	virtual void Destroy();

	virtual void RenderScene(sdMap* pkMap, NiCamera* pkCamera, NiRenderTargetGroup* pkRenderTarget);

	virtual void UpdateRenderParams(const sdRenderParams& kRenderParams);
	virtual void UpdateEnvironmentParams(const sdEnvironmentParams& kEnvParams);
	virtual void UpdatePostProcessParams(const sdPostProcessParams& kPostProcessParams);
	virtual void UpdateTerrainParams(const sdTerrainParams& kTerrainParams);

	virtual bool RenderStaticMesh(NiMesh* pkMesh);
	virtual bool RenderSkinnedMesh(NiMesh* pkMesh);
	virtual bool RenderTerrainMesh(NiMesh* pkMesh);
	virtual bool RenderLight(Engine::sdLightSceneObject* pkLight);
	virtual bool RenderProjector(sdProjectorSceneObject* pkProjector);
	virtual bool RenderMeshEdge(NiMesh* pkMesh);
	virtual bool RenderMeshEdgeEnhancement(const NiBound& kBound, const NiColor& kColor, uint uiWidth);

protected:
	// 创建渲染资源集
	void CreateAllRenderedTexture();
	void ReleaseAllRenderTexture();

	void CreateAllRenderTarget();
	void ReleaseAllRenderTarget();

	void BeginRenderScene(sdMap* pkMap, NiCamera* pkCamera, NiRenderTargetGroup* pkRenderTarget);
	void EndRenderScene();

	// 从NiNode树收集NiMesh对象(目前仅支持普通Mesh)
	void CollectMeshes(NiNode* pkNode, std::vector<NiMesh*>& kMeshVec);

	//*******************************************************
	// 资源准备与更新
	// @{
	void Update(sdMap* pkMap, NiCamera* pkCamera, NiRenderTargetGroup* pkRenderTarget);
	void UpdateRenderTargets(NiRenderTargetGroup* pkRenderTarget);
	//void UpdateCameras();
	//void UpdateReflectionCamera();
	// @}

	//*******************************************************
	// 从Map渲染RenderObject到RenderPass(裁剪与分发)
	// @{
	void Render();
	//void RenderShadow();
	//void RenderReflection();
	void RenderMainView();

	void OctreeCull();			///< 八叉树裁剪静态物件
	void UmbraCull();			///< Umbra裁剪
	void RenderActor();			///< 裁剪动态物件
	void RenderTerrain();		///< 裁剪地形
	void RenderDebugGizmo();
	// @}

	//*******************************************************
	// 从RenderPass渲染RenderObject到RenderTarget
	// @{
	void Draw();

	void DrawEarlyZPass();
	void DrawTerrainDepthPass();
	void DrawGeometryPasses();
	void DrawPreLightPass();
	void DrawShadingPasses();
	void DrawPostProcessPasses();
	void DrawEdge();
	// Transparence
	// HUD
	// UI

	// 设置适合本RenderPath的RenderDevice初始的渲染状态
	// (RenderState有缓存,所以只管设置不管复位)
	void PrepareRenderStates();

	// 设置着色器常量
	void PrepareShaderConstants();
	// @}

protected:
	// 渲染对象分配器
	sdRenderObjectAllocPtr	m_spRenderObjectAlloc;

	// 临时场景对象指针
	sdMap*					m_pkCurMap;
	NiCamera*				m_pkCurCam;
	NiRenderTargetGroup*	m_pkCurFinalRenderTarget;

	// @{
	// 渲染通道
	sdEarlyZPassPtr				m_spEarlyZPass;

	sdMRTGeometryPassPtr		m_spMRTGeometryPass;
	sdMRTShadingPassPtr			m_spMRTShadingPass;

	sdPreLightPassPtr			m_spPreLightPass;
	sdProjectorPassPtr			m_spProjectorPass;

	sdTerrainDepthPassPtr			m_spTerrainDepthPass;			///< Terrain specific depth pass
	sdTerrainAtlasGeometryPassPtr	m_spTerrainAtlasGeometryPass;
	sdTerrainAtlasShadingPassPtr	m_spTerrainAtlasShadingPass;

	sdMLAAPassPtr				m_spMLAAPass;				///< MLAA
	sdColorGradingPassPtr		m_spColorGradingPass;
	sdDOFPassPtr				m_spDofPass;
	sdGodRayPassPtr				m_spGodRayPass;
	sdHDRPassPtr				m_spHDRPass;
	sdFinalPassPtr				m_spFinalPass;

	sdEdgePassPtr				m_spEdgePass;				///< 勾边
	sdEdgeEnhancementPassPtr	m_spEdgeEnhancementPass;	///< 勾边增强

	sdDebugGizmoPassPtr			m_spDebugGizmoPass;			///< 调试绘制
	// }@


	// 渲染目标与渲染纹理
	// @{
	// Terrain-Depth-Buffer(使用m_spLightTexture)
	NiRenderTargetGroupPtr	m_spTerrainDepthTarget;

	// Geometry-Buffer
	//   |----depth88, normal88
	//	 |----diffuse_tex*diffuse_color888, lightmap8
	//	 |----specular_tex*specular_color888, shiness8
	NiRenderTargetGroupPtr	m_spGeometryAlbedoMRT;

	NiRenderTargetGroupPtr	m_spGeometryTarget;
	NiRenderedTexturePtr	m_spGeometryTexture;

	NiRenderTargetGroupPtr	m_spAlbedoTarget;
	NiRenderedTexturePtr	m_spAlbedoTexture;

	NiRenderTargetGroupPtr	m_spGlossTarget;
	NiRenderedTexturePtr	m_spGlossTexture;
	
	// Light-Buffer
	NiRenderTargetGroupPtr	m_spLightTarget;
	NiRenderedTexturePtr	m_spLightTexture;

	// Shading-Buffer
	NiRenderTargetGroupPtr	m_spShadingTarget;
	NiRenderedTexturePtr	m_spShadingTexture;

	// MLAA-Edge
	NiRenderTargetGroupPtr	m_spMLAAEdgeDetectTarget;
	NiRenderedTexturePtr	m_spMLAAEdgeDetectTexture;

	// DOF
	NiRenderTargetGroupPtr	m_spDofBlurXTarget;
	NiRenderTargetGroupPtr	m_spDofBlurYTarget;
	NiRenderedTexturePtr	m_spDofBlurXTexture;
	NiRenderedTexturePtr	m_spDofBlurYTexture;

	// GodRay
	NiRenderTargetGroupPtr	m_spQuaterSizeTarget[3];
	NiRenderedTexturePtr	m_spQuaterSizeTexture[3];

	// HDR Scaled
	NiRenderTargetGroupPtr	m_spHDRHalfSizeScaleTarget;
	NiRenderedTexturePtr	m_spHDRHalfSizeScaleTexture;

	NiRenderTargetGroupPtr	m_spHDRQuaterSizeScaleTarget;
	NiRenderedTexturePtr	m_spHDRQuaterSizeScaleTexture;

	NiRenderTargetGroupPtr	m_spHDROctantSizeScaleTarget;
	NiRenderedTexturePtr	m_spHDROctantSizeScaleTexture;

	// HDR Luminance
	NiRenderTargetGroupPtr	m_spHDRCurLuminanceTarget;
	NiRenderedTexturePtr	m_spHDRCurLuminanceTexture;

	NiRenderTargetGroupPtr	m_spHDRLastLuminanceTarget;
	NiRenderedTexturePtr	m_spHDRLastLuminanceTexture;

	NiRenderTargetGroupPtr	m_spHDRCurAdaptedLuminanceTarget;
	NiRenderedTexturePtr	m_spHDRCurAdaptedLuminanceTexture;

	std::vector<NiRenderTargetGroupPtr>	m_kHDRIntermediateLuminanceTarget;
	std::vector<NiRenderedTexturePtr>	m_kHDRIntermediateLuminanceTexture;

	// HDR Bloom
	NiRenderTargetGroupPtr	m_spHDRBloomThresholdTarget;
	NiRenderedTexturePtr	m_spHDRBloomThresholdTexture;

	NiRenderTargetGroupPtr	m_spHDRBloomBlurXTarget;
	NiRenderedTexturePtr	m_spHDRBloomBlurXTexture;

	NiRenderTargetGroupPtr	m_spHDRBloomBlurYTarget;
	NiRenderedTexturePtr	m_spHDRBloomBlurYTexture;

	// Back-Buffer
	NiRenderTargetGroupPtr	m_spHalfSizeTarget;
	NiRenderedTexturePtr	m_spHalfSizeTexture;

	// Water-Reflection-Buffer
	//NiRenderedTexturePtr	m_spWaterReflectionTexture;
	// }@


	// 渲染参数
	// @{
	sdRenderParams			m_kRenderParams;
	sdEnvironmentParams		m_kEnvironmentParams;
	sdPostProcessParams		m_kPostProcessParams;
	sdTerrainParams			m_kTerrainParams;
	// }@


	// 渲染设定
	// @{
	bool	m_bEnableRendering;
	bool	m_bEnableStaticMeshRendering;
	bool	m_bEnableSkinnedMeshRendering;
	bool	m_bUseLocalLight;
	bool	m_bUseEarlyZ;
	bool	m_bUseMRT;						
	// }@
};
NiSmartPointer(sdRenderPath_DX9);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif