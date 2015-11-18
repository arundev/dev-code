//*************************************************************************************************
// 内容:	场景地图
//---------------------------------------------------------
// 作者:		
// 创建:		2012-06-29
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_MAP_H__
#define _SD_ENGINE_MAP_H__
#include "sdGameObject.h"
#include "sdTerrain.h"
#include "sdLight.h"

#include "sdFlare.h"
#include "sdSun.h"
#include "sdSkyBox.h"
#include "sdBGScroll.h"
#include "sdCloud.h"

#include "IRenderSystem.h"

#include "sdSceneLoadMgr.h"

// 最大地图名长度
#define MAX_MAP_NAME_LEN 64

//-------------------------------------------------------------------------------------------------
// 场景地图
//-------------------------------------------------------------------------------------------------
class sdMap : public sdGameObject
{
	///< 只能暂时共存
	NiDeclareRTTI;
	NiDeclareStream;

	///< 新动态系统
	DECLARE_PROPFUNC_MAP()
	DECLARE_DYNAMIC(sdMap)

public:
	sdMap();
	~sdMap();

	// @breif 地图裁剪
	// @param[in] kCamera		裁剪相机
	// @param[in] kEntityVec	
	void Cull(const NiCamera& kCamera, EntityVec& kEntityVec);

	// @breif 地图更新
	void UpdateGame();

	// Entity操作
	bool AddEntity(sdEntity* pkEntity);
	bool RemoveEntity(sdEntity* pkEntity);

	// 属性访问
	// @{
	// 
	sdTerrain*	GetTerrain() { return m_pkTerrain;};

	//
	NiNode*	GetDebugNode() const { return m_spDebugNode;}
//	NiNode* GetLightNode() const { return m_spLightNode;}
//	NiNode* GetDecalNode() const { return m_spDecalNode;}

	// 灯光
	sdLight* GetAmbientLight() const;
	sdLight* GetMainLight() const;
	sdLight* GetAssistantLight() const;

	// 环境效果
	sdFlare*	GetFlare() const;
	sdSun*		GetSun() const;
	sdSkyBox*	GetSkyBox() const;
	sdBGScroll*	GetBGScroll() const;
	sdCloud*	GetCloudDome() const;

	// 渲染参数
	Engine::RenderSystem::sdRenderParams& GetRenderParams() { return m_kRenderParams;};
	Engine::RenderSystem::sdEnvironmentParams& GetEnvironmentParams() { return m_kEnvironmentParams;};
	Engine::RenderSystem::sdPostProcessParams& GetPostProcessParams() { return m_kPostProcessParams;};
	// @}

	// 流化属性接口
	// @{
	SD_INLINE sdGameObject* GetTerrainProp() const;
	SD_INLINE void SetTerrainProp(const sdGameObject*&);

//	SD_INLINE sdGameObject* GetSkyProp();
//	SD_INLINE void SetSkyProp(const sdGameObject*&);

	SD_INLINE uint GetLight(sdGameObject*& kValue, uint uiIndex) const;
	SD_INLINE uint SetLight(const sdGameObject*& kValue, uint uiIndex);
	// @}

protected:

protected:
	// 地形系统
	sdTerrain*	m_pkTerrain;

	// 场景灯光
	sdLightPtr	m_pkAmbientLight;
	sdLightPtr	m_pkMainLight;
	sdLightPtr	m_pkAssistantLight;

	// 环境效果
	sdFlarePtr	m_pkFlare;
	sdSunPtr	m_pkSun;
	sdSkyBoxPtr	m_pkSkyBox;
	sdBGScrollPtr m_pkBGScroll;
	sdCloudPtr	m_pkCloudDome;

	// 天气效果
//	sdRain;
//	sdSnow;
//	sdDynamicWheather;

	// 场景普通对象
	NiNodePtr	m_spDebugNode;		///< 调试对象根节点(sdDynamicNode)
	NiNodePtr	m_spLightNode;		///< 灯光对象根节点(sdDynamicNode)
	NiNodePtr	m_spDecalNode;		///< 贴花对象根节点(sdDynamicNode)

	NiNodePtr	m_spBuildingNode;	///< 建筑对象根节点(sdNode)

	//NiNodePtr	m_spTrigger;
	//NiNodePtr	m_spThing;
	//NiNodePtr	m_spWater;
	//NiNodePtr	m_spRoad;
	
	EntityVec m_kStaticObjectVec;	///< 静态物件
	EntityVec m_kActor;				///< 动态角色
	EntityVec m_kEffect;			///< 特效对象
	EntityVec m_kArea;				///< 区域
	EntityVec m_kSliceCollider;		///< 阻挡

	// 加载管理器(负责对象加载与卸载)
	sdSceneLoadMgr m_kSceneLoadMgr;

	// 场景绘制参数(用于提供给渲染系统使用,内部不应该被分配内存和保存资源)
	Engine::RenderSystem::sdRenderParams		m_kRenderParams;
	Engine::RenderSystem::sdEnvironmentParams	m_kEnvironmentParams;
	Engine::RenderSystem::sdPostProcessParams	m_kPostProcessParams;
};
NiSmartPointer(sdMap);
//-------------------------------------------------------------------------------------------------
#include "sdMap.inl"
#endif