//*************************************************************************************************
// 内容:		KFM资源
//---------------------------------------------------------
// 作者:		
// 创建:		2013-02-01
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RESOURCE_SYSTEM_KFM_RESOURCE_H__
#define _SD_ENGINE_RESOURCE_SYSTEM_KFM_RESOURCE_H__
#include "sdResource.h"


ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RESOURCESYSTEM
//-------------------------------------------------------------------------------------------------
class sdKfmResource : public sdResource
{
public:
	// 用于资源加载
	sdKfmResource(const std::string& kFileName,					
		bool bReleaseOnGC = true, 
		EResUsageType eUsageType = E_RES_USAGE_KFM_COMMON, 
		const std::string& kChangeNifFileName = std::string(),
		const std::string& kModelRootName = std::string());

	// 用于资源释放
	sdKfmResource(NiNode* spNode);	
	virtual ~sdKfmResource();

	// 虚函数继承
	virtual bool LoadResource(bool bMainThreadLoad = false);
	virtual void FreeResource(bool bNotify = true);
	virtual void CacheResource();

	inline virtual bool Loaded();
	inline virtual bool Freed();

	inline virtual NiActorManager* GetActorManager();

protected:
	NiActorManagerPtr m_spActorManager;
	std::string m_kModelRootName;
	std::string m_kChangeNifFileName;
};
NiSmartPointer(sdKfmResource);
//-------------------------------------------------------------------------------------------------
#include "sdKfmResource.inl"
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif