//*************************************************************************************************
// 内容:	场景对象工厂
//---------------------------------------------------------
// 作者:		
// 创建:		
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMECORE_FACTORY_H__
#define _SD_GAMECORE_FACTORY_H__
#include "sdDynamicPool.h"
#include <sdSingleton.h>

SD_GAMECORE_NAMESPACE_BEGIN_GAMECORE

class sdSceneItem;

// 对象工厂
//	1.非线程安全
//	2.支持内存池的类必须有Initialize/Destory接口
class sdFactory : public Base::sdTSingleton<sdFactory>
{
public:
	sdFactory(){};
	virtual ~sdFactory(){};

	virtual void Initialize();
	virtual void Destroy();

	///< 对象分配与归还
	///< Fetch/Release, Alloc/Dealloc, New/Delete
	///< @{
	//sdSceneItem*	AllocSceneItem();
	//void			DeallocSceneItem(sdSceneItem* pkSceneItem);
	/// @}

	///< 属性访问(用于内存与性能分析和调试)
	///< @{
	//inline const TDynamicPool<sdSceneItem>& GetSceneItemPool() const;
	///< @}

protected:
	//TDynamicPool<sdSceneItem>	m_kSceneItemPool;
};

#include "sdFactory.inl"
SD_GAMECORE_NAMESPACE_END_GAMECORE
#endif