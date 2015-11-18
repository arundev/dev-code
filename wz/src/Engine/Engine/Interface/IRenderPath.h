//*************************************************************************************************
// 内容:	渲染路径接口
//---------------------------------------------------------
// 作者:		
// 创建:		2012-07-04
// 最后修改:
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
// 渲染路径接口
// 渲染路径是渲染策略的一种封装,它定义了如何渲染一个指定的Map到指定的RenderTarget
//
// RenderPath处理整个渲染,
//	1.负责将RenderPass进行合理的组织和调用,以及创建一个有限的资源集保障
//	2.裁剪Map得到Mesh队列,
//	3.填充Mesh到RenderObject
//	4.填充RenderObject到对应的RenderPass
//
// RenderPass处理一个特定的中间步骤,负责将RenderObject渲染到RenderTarget
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
	// 初始化与销毁
	virtual bool Initialize() = 0;
	virtual void Destroy() = 0;
	virtual bool IsInitialized() = 0;

	// 设备重建回调
	//virtual bool Recreate() = 0;

	// 渲染总接口,处理裁剪(Entity级别与Mesh级别)与渲染
	virtual void RenderScene(sdMap* pkMap, NiCamera* spCamera, NiRenderTargetGroup* spRenderTarget) = 0;

	// 更新渲染参数
	// @{
	virtual void UpdateRenderParams(const sdRenderParams& kRenderParams) = 0;
	virtual void UpdateEnvironmentParams(const sdEnvironmentParams& kEnvParams) = 0;
	virtual void UpdatePostProcessParams(const sdPostProcessParams& kPostProcessParams) = 0;
	virtual void UpdateTerrainParams(const sdTerrainParams& kTerrainParams) = 0;
	// @}

	// @brief 添加静态Mesh到渲染队列
	// @param[in]	pkMesh
	// @return
	virtual bool RenderStaticMesh(NiMesh* pkMesh) = 0;

	// @brief 添加带骨骼的Mesh到渲染队列
	// @param[in]	pkMesh
	// @return
	virtual bool RenderSkinnedMesh(NiMesh* pkMesh) = 0;

	// @brief 添加地形的Mesh到渲染队列
	// @param[in]	pkMesh
	// @return 
	virtual bool RenderTerrainMesh(NiMesh* pkMesh) = 0;

//	// @brief 添加地形的Mesh到渲染队列
//	// @param[in]	pkMesh
//	// @return 
//	virtual bool RenderTerrainHole(NiMesh* pkMesh) = 0;

	// @brief 添加灯光场景对象到渲染队列
	// @param[in]	pkLight
	// @return 
	virtual bool RenderLight(Engine::sdLightSceneObject* pkLight) = 0;

	// @brief 添加投影场景对象到渲染队列
	// @param[in]	pkProjector
	// @return 
	virtual bool RenderProjector(Engine::sdProjectorSceneObject* pkProjector) = 0;

//	// @brief 添加贴花对象到渲染队列
//	// @param[in]	
//	// @return 
//	virtual bool RenderDecal()

//	// @brief 添加道路对象到渲染队列
//	// @param[in]	
//	// @return 
//	virtual bool RenderRoad()

	// @brief 添加需要勾边的对象到渲染队列
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