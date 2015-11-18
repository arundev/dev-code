//*************************************************************************************************
// 内容:	渲染路径接口
//---------------------------------------------------------
// 作者:		
// 创建:		2012-07-04
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_RENDER_PATH_H__
#define _SD_ENGINE_RENDER_SYSTEM_RENDER_PATH_H__
#include <IRenderPath.h>
#include "sdRenderPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
class sdRenderPath : public IRenderPath
{
public:
	sdRenderPath();
	virtual ~sdRenderPath();

	// 虚函数继承
	//virtual bool Initialize() {};
	//virtual void Destroy() {};
	virtual bool IsInitialized(){ return m_bInitialized;};

	//virtual void RenderScene(sdMap* pkMap, NiCamera* spCamera, NiRenderTargetGroup* spRenderTarget) = 0;

	//virtual void UpdateRenderParams(const sdRenderParams& kRenderParams) = 0;
	//virtual void UpdateEnvironmentParams(const sdEnvironmentParams& kEnvParams) = 0;
	//virtual void UpdatePostProcessParams(const sdPostProcessParams& kPostProcessParams) = 0;
	//virtual void UpdateTerrainParams(const sdTerrainParams& kTerrainParams) = 0;

	//virtual bool RenderStaticMesh(NiMesh* spMesh, ushort usMeshId) = 0;
	//virtual bool RenderSkinnedMesh(NiMesh* spMesh, ushort usMeshId) = 0;
	//virtual bool RenderTerrainMesh(NiMesh* spMesh, ushort usMeshId) = 0;
	//virtual bool RenderTerrainHole(NiMesh* spMesh, ushort usMeshId) = 0;
	//virtual bool RenderLight(NiLight* spLight, ushort usMeshId) = 0;
	//virtual bool RenderProjector(sdProjectorSceneObject* pkProjector, ushort usMeshId) = 0;
	//virtual bool RenderDecal()
	//virtual bool RenderRoad()

protected:
	bool m_bInitialized;

	typedef std::vector<sdRenderPass*> RenderPassVec;
	typedef std::vector<sdRenderPass*>::iterator RenderPassVecItr;
	RenderPassVec m_kRenderPassVec;
};
NiSmartPointer(sdRenderPath);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif