//*************************************************************************************************
// 内容:	场景逻辑对象
//---------------------------------------------------------
// 作者:		
// 创建:		
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_SCENE_ACTOR_H__
#define _SD_ENGINE_SCENE_ACTOR_H__
#include "sdEntity.h"

//-------------------------------------------------------------------------------------------------
class sdActor : public sdEntity
{
	NiDeclareRTTI;
	NiDeclareStream;

	DECLARE_PROPFUNC_MAP()
	DECLARE_DYNAMIC(sdActor)

public:
	// 阴影类型(DecalShadow)
	enum EShadowType
	{
		E_ST_REALTIME_SHADOW,	///< 实时阴影
		E_ST_ROUND_SHADOW,		///< 圆盘阴影
		E_ST_NO_SHADOW,			///< 无阴影
	};

public:
	sdActor();
	virtual ~sdActor();

	// 虚函数继承(继承自sdEntity)
	virtual void Cull(const NiCamera& kCamera, Engine::RenderSystem::IRenderPath& kRenderPath);

	// 轮廓光
	// @{
	

	// @}

protected:	
	// 轮廓光
	bool m_bEnableRimlight;

	// 阴影效果
	bool m_bEnableShadow;
	EShadowType m_eShadowType;

	// 离屏更新
	bool m_bOffScreenUpdate;			///< 是否离屏更新
	float m_fOffScreenUpdateDistance;	///< 离屏更新范围

	//m_eSkeletonQuility；	///< 影响每个顶点最大骨骼数量
};
NiSmartPointer(sdActor);
//-------------------------------------------------------------------------------------------------
#endif