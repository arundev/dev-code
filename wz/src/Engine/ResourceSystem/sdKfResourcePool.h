//*************************************************************************************************
// 内容:		KF资源缓存池
//---------------------------------------------------------
// 作者:		
// 创建:		2013-02-01
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RESOURCE_SYSTEM_KF_RESOURCE_POOL_H__
#define _SD_ENGINE_RESOURCE_SYSTEM_KF_RESOURCE_POOL_H__
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
class sdKfResourcePool : public sdResourcePool
{
	friend class sdResourceSystem;
public:
	// 虚函数继承
	virtual void RemoveAllInstance();	

	// 加载Kf文件
	NiSequenceDataPtr LoadSequenceData(const std::string& kFileName, NiKFMTool::Sequence* pkKFMSequence, NiStream* spStream);

	// 释放Kf文件
	void FreeSequenceData(NiActorManager* spActorMgr);
	
protected:
	sdKfResourcePool();
	virtual ~sdKfResourcePool();

	// 查找
	NiSequenceDataPtr GetSequenceDataFromArray(NiKFMTool::Sequence* spKFMSequence, std::vector<NiSequenceDataPtr>& kSeqDataVec);

	// 从硬盘加载资源
	bool LoadFromDisk(const std::string& kFileName, NiStream* spStream, std::vector<NiSequenceDataPtr>& kSeqDataVec);

	// 销毁
	void FreeSequenceData(const std::string& kFileName, NiKFMTool::Sequence* spKFMSequence);

protected:
	class CacheSequenceInfo : public NiMemObject
	{
	public:
		std::vector<NiSequenceDataPtr> m_kSequenceDataVec;
		int m_iCloneCount;	///< 被克隆的次数

	public:
		CacheSequenceInfo() : m_iCloneCount(0){}
	};

protected:
	typedef std::map<std::string, CacheSequenceInfo> CacheSequenceMap;
	typedef std::map<std::string, CacheSequenceInfo>::iterator CacheSequenceMapItr;
	CacheSequenceMap m_kCacheSequenceMap;
};
NiSmartPointer(sdKfResourcePool);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif