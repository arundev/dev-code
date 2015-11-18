//*************************************************************************************************
// 内容:		NIF资源缓存池
//---------------------------------------------------------
// 作者:		
// 创建:		2013-01-31
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RESOURCE_SYSTEM_NIF_RESOURCE_POOL_H__
#define _SD_ENGINE_RESOURCE_SYSTEM_NIF_RESOURCE_POOL_H__
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
class sdNifResourcePool : public sdResourcePool
{
	friend class sdResourceSystem;
public:
	// 虚函数继承
	virtual void RemoveAllInstance();	

	// 加载Nif文件
	NiNodePtr LoadNifFile(const std::string& kFileName, EResUsageType eUsageType, bool bMainThreadLoad = false);
	NiNodePtr LoadNifFile(const std::string& kFileName, EResUsageType eUsageType, NiStream* spStream);

	// 释放Nif文件
	void FreeNifFile(const std::string& kFileName);
	void FreeKFMRoot(NiActorManager* spActorMgr);

	// 清除所有非KFMRoot的NIF Cache
	void RemoveGCInstance();
	
protected:
	sdNifResourcePool();
	virtual ~sdNifResourcePool();

	// 从硬盘加载资源
	NiNodePtr LoadFromDisk(const std::string& kFileName, EResUsageType eUsageType, NiStream* spStream);

protected:
	class CacheNifInfo : public NiMemObject
	{
	public:
		NiNodePtr m_spRootNode;
		int m_iCloneCount;	///< 被克隆的次数
		bool m_bKFMRoot;	///< 是否是KFM根节点

	public:
		CacheNifInfo() : m_iCloneCount(0), m_bKFMRoot(false){}
	};

protected:
	typedef std::map<std::string, CacheNifInfo> CacheNifMap;
	typedef std::map<std::string, CacheNifInfo>::iterator CacheNifMapItr;
	CacheNifMap m_kCacheNifMap;

};
NiSmartPointer(sdNifResourcePool);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif