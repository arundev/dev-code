//*************************************************************************************************
// ����:	����DX9��Gamebryo���ӳ���Ⱦ·��
//---------------------------------------------------------
// ����:		
// ����:		2012-07-05
// ����޸�:
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
// ʹ��Gamebryo��DX9ʵ�ֵ��ӳ���ɫ��Ⱦ·��(�ڲ��������Ⱦ�ɸ���RenderPass��ʵ��)
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
	// ��������Mesh��StencilID,���ڱ궨Geometry-Buffer�Ķ�������
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

	// һЩ��������λ(����ͨStencil���ʹ��)
	enum ePixelStencilBit
	{
		E_PSB_RECEIVE_DECAL_SHIFT		= 0x3,	///< ���ڱ�ǿ����������Bitλ��, 0000 1000
		E_PSB_DISTANCE_TERRAIN_SHIFT	= 0x4,	///< ���ڱ��Զ�����ε�Bitλ��,   0001 0000
		E_PSB_DEFERED_LIGHT_SHIFT		= 0x5,	///< ���ڱ���ӳٹ��յ�Bitλ��,   0010 0000
		E_PSB_EDGE_SHIFT				= 0x6,	///< ���ڱ�ǹ��ߵ�Bitλ��,		  0100 0000
	};


public:
	sdRenderPath_DX9();
	virtual ~sdRenderPath_DX9();

	// �麯���̳�(�̳���IRenderPath)
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
	// ������Ⱦ��Դ��
	void CreateAllRenderedTexture();
	void ReleaseAllRenderTexture();

	void CreateAllRenderTarget();
	void ReleaseAllRenderTarget();

	void BeginRenderScene(sdMap* pkMap, NiCamera* pkCamera, NiRenderTargetGroup* pkRenderTarget);
	void EndRenderScene();

	// ��NiNode���ռ�NiMesh����(Ŀǰ��֧����ͨMesh)
	void CollectMeshes(NiNode* pkNode, std::vector<NiMesh*>& kMeshVec);

	//*******************************************************
	// ��Դ׼�������
	// @{
	void Update(sdMap* pkMap, NiCamera* pkCamera, NiRenderTargetGroup* pkRenderTarget);
	void UpdateRenderTargets(NiRenderTargetGroup* pkRenderTarget);
	//void UpdateCameras();
	//void UpdateReflectionCamera();
	// @}

	//*******************************************************
	// ��Map��ȾRenderObject��RenderPass(�ü���ַ�)
	// @{
	void Render();
	//void RenderShadow();
	//void RenderReflection();
	void RenderMainView();

	void OctreeCull();			///< �˲����ü���̬���
	void UmbraCull();			///< Umbra�ü�
	void RenderActor();			///< �ü���̬���
	void RenderTerrain();		///< �ü�����
	void RenderDebugGizmo();
	// @}

	//*******************************************************
	// ��RenderPass��ȾRenderObject��RenderTarget
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

	// �����ʺϱ�RenderPath��RenderDevice��ʼ����Ⱦ״̬
	// (RenderState�л���,����ֻ�����ò��ܸ�λ)
	void PrepareRenderStates();

	// ������ɫ������
	void PrepareShaderConstants();
	// @}

protected:
	// ��Ⱦ���������
	sdRenderObjectAllocPtr	m_spRenderObjectAlloc;

	// ��ʱ��������ָ��
	sdMap*					m_pkCurMap;
	NiCamera*				m_pkCurCam;
	NiRenderTargetGroup*	m_pkCurFinalRenderTarget;

	// @{
	// ��Ⱦͨ��
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

	sdEdgePassPtr				m_spEdgePass;				///< ����
	sdEdgeEnhancementPassPtr	m_spEdgeEnhancementPass;	///< ������ǿ

	sdDebugGizmoPassPtr			m_spDebugGizmoPass;			///< ���Ի���
	// }@


	// ��ȾĿ������Ⱦ����
	// @{
	// Terrain-Depth-Buffer(ʹ��m_spLightTexture)
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


	// ��Ⱦ����
	// @{
	sdRenderParams			m_kRenderParams;
	sdEnvironmentParams		m_kEnvironmentParams;
	sdPostProcessParams		m_kPostProcessParams;
	sdTerrainParams			m_kTerrainParams;
	// }@


	// ��Ⱦ�趨
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