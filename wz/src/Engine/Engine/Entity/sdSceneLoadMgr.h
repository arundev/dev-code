//*************************************************************************************************
// 内容:	场景地图加载管理器,提取出Map的加载功能
//---------------------------------------------------------
// 作者:		
// 创建:		2013-05-24
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_SCENE_SCENE_LOAD_MANAGER_H__
#define _SD_ENGINE_SCENE_SCENE_LOAD_MANAGER_H__
#include "sdEntity.h"
#include <sdProfileOutput.h>

//--------------------------------------------------------------------------------------
enum ESceneLoadMode
{
	E_SCENE_LOAD_SYNC,	///< 同步加载
	E_SCENE_LOAD_ASYNC,	///< 异步加载
};
//--------------------------------------------------------------------------------------
class sdSceneLoadMgr : public NiRefObject
{
public:
	sdSceneLoadMgr();
	~sdSceneLoadMgr();

	// 更新
	void Update();

	// 向加载管理器添加与移除对象
	void AddEntity(sdEntity* pkEntity);
	void RemoveEntity(sdEntity* pkEntity);

	// 场景加载模式
	ESceneLoadMode GetLoadMode() const { return m_eLoadMode;}
	void SetLoadMode(ESceneLoadMode eLoadMode) { m_eLoadMode = eLoadMode;}

	// 最小加载移动量
	float GetLoadRadius() const { return m_fMinLoadRadius;}
	void  SetLoadRadius(float fRadius) { m_fMinLoadRadius = fRadius;}

protected:
	
	// 同步加载对象
	void SyncLoad();

	// 异步加载对象
	void AsyncLoad();

protected:
	ESceneLoadMode m_eLoadMode;	///< 加载模式

	// 目前使用简单的线性列表而不是复杂的空间剖分结构进行管理
	EntityVec m_kEntityVec;			///< 对象列表
	EntityVec m_kProcessVec;		///< 当前帧待处理列表
	EntityVec m_kDelayUnloadingVec;	///< 延迟到下一帧处理的卸载列表

	// 计时器(限时加载)
	Base::Diagnostics::sdProfileOutput m_kProfileOutput;

	NiPoint3 m_kLastLoadPos;	///< 上一次进行加载时的相机位置
	float m_fMinLoadRadius;		///< 需要重新加载的相机最小移动距离
};
NiSmartPointer(sdSceneLoadMgr);
//--------------------------------------------------------------------------------------
#endif