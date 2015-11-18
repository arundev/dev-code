//*************************************************************************************************
// ����:	��Ⱦ·���ӿ�
//---------------------------------------------------------
// ����:		
// ����:		2012-07-04
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_RENDER_PATH_INTERFACE_H__
#define _SD_ENGINE_RENDER_SYSTEM_RENDER_PATH_INTERFACE_H__
#include "IRenderSystem.h"
#include "sdSceneObject.h"

class sdMap;

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
// ��Ⱦ·���ӿ�
// ��Ⱦ·������Ⱦ���Ե�һ�ַ�װ,�������������Ⱦһ��ָ����Map��ָ����RenderTarget
//
// RenderPath����������Ⱦ,
//	1.����RenderPass���к������֯�͵���,�Լ�����һ�����޵���Դ������
//	2.�ü�Map�õ�Mesh����,
//	3.���Mesh��RenderObject
//	4.���RenderObject����Ӧ��RenderPass
//
// RenderPass����һ���ض����м䲽��,����RenderObject��Ⱦ��RenderTarget
//-------------------------------------------------------------------------------------------------
// \class sdRenderPath
//
// sdRenderPath is implemented to encapsulate the rendering strategy.
// It defines how to render a specified scene to specified render target.
// Its caller should create and initialize the render target
//
// It only create necessary resources from the resource cache(not implement yet)
// which used to implement its rendering strategy. For example: Off-screen
// render targets used to store its rendering intermediat result.
//
// Usually, the sdRenderPath will split its rendering strategy to several sdRenderPass
// which define the strategy of how to render a intermedat/final result of the
// whole rendering process.
//
// Each sdRenderPath's derived class should know what its sub sdRenderPass will do.
// It should properly provide and initialize render targets for its sub sdRenderPass.
//
// Each sdRenderPass only responses to properly render meshes to current render target.
//-------------------------------------------------------------------------------------------------
class IRenderPath : public NiRefObject
{
public:
	virtual ~IRenderPath() {};

	// Initialize/Release video memory resource
	// ��ʼ��������
	virtual bool Initialize() = 0;
	virtual void Destroy() = 0;
	virtual bool IsInitialized() = 0;

	// �豸�ؽ��ص�
	//virtual bool Recreate() = 0;

	// ��Ⱦ�ܽӿ�,����ü�(Entity������Mesh����)����Ⱦ
	virtual void RenderScene(sdMap* pkMap, NiCamera* spCamera, NiRenderTargetGroup* spRenderTarget) = 0;

	// ������Ⱦ����
	// @{
	virtual void UpdateRenderParams(const sdRenderParams& kRenderParams) = 0;
	virtual void UpdateEnvironmentParams(const sdEnvironmentParams& kEnvParams) = 0;
	virtual void UpdatePostProcessParams(const sdPostProcessParams& kPostProcessParams) = 0;
	virtual void UpdateTerrainParams(const sdTerrainParams& kTerrainParams) = 0;
	// @}

	// @brief ��Ӿ�̬Mesh����Ⱦ����
	// @param[in]	pkMesh
	// @return
	virtual bool RenderStaticMesh(NiMesh* pkMesh) = 0;

	// @brief ��Ӵ�������Mesh����Ⱦ����
	// @param[in]	pkMesh
	// @return
	virtual bool RenderSkinnedMesh(NiMesh* pkMesh) = 0;

	// @brief ��ӵ��ε�Mesh����Ⱦ����
	// @param[in]	pkMesh
	// @return 
	virtual bool RenderTerrainMesh(NiMesh* pkMesh) = 0;

//	// @brief ��ӵ��ε�Mesh����Ⱦ����
//	// @param[in]	pkMesh
//	// @return 
//	virtual bool RenderTerrainHole(NiMesh* pkMesh) = 0;

	// @brief ��ӵƹⳡ��������Ⱦ����
	// @param[in]	pkLight
	// @return 
	virtual bool RenderLight(Engine::sdLightSceneObject* pkLight) = 0;

	// @brief ���ͶӰ����������Ⱦ����
	// @param[in]	pkProjector
	// @return 
	virtual bool RenderProjector(Engine::sdProjectorSceneObject* pkProjector) = 0;

//	// @brief �������������Ⱦ����
//	// @param[in]	
//	// @return 
//	virtual bool RenderDecal()

//	// @brief ��ӵ�·������Ⱦ����
//	// @param[in]	
//	// @return 
//	virtual bool RenderRoad()

	// @brief �����Ҫ���ߵĶ�����Ⱦ����
	// @param[in]	
	// @return 
	virtual bool RenderMeshEdge(NiMesh* pkMesh) = 0;
	virtual bool RenderMeshEdgeEnhancement(const NiBound& kBound, const NiColor& kColor, uint uiWidth) = 0;
	// @}
};
NiSmartPointer(sdRenderPath);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif