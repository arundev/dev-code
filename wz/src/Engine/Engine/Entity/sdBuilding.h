//*************************************************************************************************
// 内容:	建筑,树干,桥等静态物件
//---------------------------------------------------------
// 作者:		
// 创建:		2012-12-06
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_SCENE_BUILDING_H__
#define _SD_ENGINE_SCENE_BUILDING_H__
#include "sdEntity.h"
#include "IResource.h"
#include "IResourceSystem.h"

//-------------------------------------------------------------------------------------------------
// 普通物件
//	1.静态建筑
//	2.树和树叶(需要半透明处理)
//-------------------------------------------------------------------------------------------------
class sdBuilding : public sdEntity, public Engine::ResourceSystem::IResourceEventListener
{
	NiDeclareRTTI;
	NiDeclareStream;

	DECLARE_PROPFUNC_MAP()
	DECLARE_DYNAMIC(sdBuilding)

public:
	sdBuilding();
	virtual ~sdBuilding();

	// 虚函数继承(继承自sdEntity)
	virtual void Cull(const NiCamera& kCamera, Engine::RenderSystem::IRenderPath& kRenderPath);
	virtual void UpdateGame();

	// 属性访问
	// @{
	DEFINE_GAMEOBJECT_PROP(NiFixedString, NIF, NIFFile)
	inline NiFixedString	GetNIFFile() const;
	inline void				SetNIFFile(const NiFixedString&);

	// @}

	// NIF资源加载与卸载接口(提供给SceneLoadManager调用)
	// @{
	void UpdateLoad();
	void UpdateUnload();
	void UpdateDelayUnload();

	// 同步加载NIF文件(用于编辑器)
	void SyncLoadNIF();
	// @}

	// 
//	void LoadLightMap();
//	void LoadEnvironmentMap();
//	void LoadDetailNormalMap();

protected:
	// 虚函数继承(来自IResourceEventListener)
	virtual void OnLoaded(Engine::ResourceSystem::IResource* pkResource); 

	// 资源加载与卸载功能函数
	// @{
	// 更新Geometry的LOD

	// 更新Texture的LOD

	// 异步加载NIF文件
	void AsyncLoadNIF();

	// 挂接与反挂接图形资源
	void AttachGraphic(NiNode* pkNode);
	void DetachGraphic();
	void GraphicLoadOver();

	void ProcessShimmer(NiNode* pkNode);	///< 图形文件加载时,预处理Shimmer信息
	void ProcessTreeLeaf(NiNode* pkNode);	///< 图形文件加载时,预处理TreeLeaf信息
	void ProcessLight(NiNode* pkNode);		///< 图形文件加载时,预处理Light信息
	

	// 更新加载优先级(目前以到相机的距离作为权重)
	bool UpdatePrior(Engine::ResourceSystem::EResReqPrior ePrior, float fSqlDistance);
	// @}


protected:
	NiNodePtr		m_spNIFRoot;	///< NIF文件对象
	NiFixedString	m_kNIFPath;		///< NIF文件路径

	// 资源附带的点光源
	typedef std::vector<NiPointLight*> PointLightVec;
	typedef std::vector<NiPointLight*>::iterator PointLightVecItr;
	PointLightVec m_kLights;

	// 纹理加载状态
	ELoadingState m_eLightMapLoadingState;
	ELoadingState m_eDetaiNormalMapLoadingState;
	ELoadingState m_eEnvironmentMapLoadingState;
	
	// 多线程资源加载
	Engine::ResourceSystem::IResourcePtr			m_pkResource;
	Engine::ResourceSystem::IResource::ResTicket	m_uiResourceTick;
};
NiSmartPointer(sdBuilding);
//-------------------------------------------------------------------------------------------------
#include "sdBuilding.inl"
#endif