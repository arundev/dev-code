//*************************************************************************************************
// 内容:		KFM资源缓存池
//---------------------------------------------------------
// 作者:		
// 创建:		2013-02-01
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RESOURCE_SYSTEM_KFM_RESOURCE_POOL_H__
#define _SD_ENGINE_RESOURCE_SYSTEM_KFM_RESOURCE_POOL_H__
#include "sdResourcePool.h"
#include <IResource.h>

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RESOURCESYSTEM
class sdResourceSystem;
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RESOURCESYSTEM
//-------------------------------------------------------------------------------------------------
class sdKfmResourcePool : public sdResourcePool
{
	friend class sdResourceSystem;
public:
	// 虚函数继承
	virtual void RemoveAllInstance();	

	// 加载Kfm文件
	NiActorManagerPtr LoadKfmFile(const std::string& kFileName, EResUsageType eUsageType, bool bReleaseOnGC = true, bool bMainThreadLoad = false);

	// 释放Kfm文件
	void FreeKFMFile(const std::string& kFileName);

	// 清除
	void RemoveGCInstance(const std::set<std::string>& kNoReleaseMap);
	
protected:
	sdKfmResourcePool();
	virtual ~sdKfmResourcePool();

	// 从硬盘加载资源
	NiActorManagerPtr LoadFromDisk(const std::string& kFileName, bool bMainThreadLoad = false);

protected:
	class CacheKfmInfo : public NiMemObject
	{
	public:
		NiActorManagerPtr m_spActorMgr;
		int m_iCloneCount;		///< 被克隆的次数
		bool m_bReleaseOnGC;

	public:
		CacheKfmInfo() : m_iCloneCount(0), m_bReleaseOnGC(true){}
	};

protected:
	typedef std::map<std::string, CacheKfmInfo> CacheKfmInfoMap;
	typedef std::map<std::string, CacheKfmInfo>::iterator CacheKfmInfoMapItr;
	CacheKfmInfoMap m_kCacheKfmInfoMap;
};
NiSmartPointer(sdKfmResourcePool);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif