//*************************************************************************************************
// 内容:		NIF资源
//---------------------------------------------------------
// 作者:		
// 创建:		2013-01-31
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RESOURCE_SYSTEM_NIF_RESOURCE_H__
#define _SD_ENGINE_RESOURCE_SYSTEM_NIF_RESOURCE_H__
#include "sdResource.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RESOURCESYSTEM
//-------------------------------------------------------------------------------------------------
class sdNifResource : public sdResource
{
public:
	sdNifResource(const std::string& kFileName, bool bReleaseOnGC, EResUsageType eUsageType = E_RES_USAGE_NIF_COMMON);
	sdNifResource(NiNode* spNode);
	virtual ~sdNifResource();

	// 虚函数继承
	virtual bool LoadResource(bool bMainThreadLoad = false);
	virtual void FreeResource(bool bNotify = true);
	virtual void CacheResource();

	inline virtual bool Loaded();
	inline virtual bool Freed();

	inline virtual NiNode* GetRootNode();

protected:
	NiNodePtr m_spRootNode;
};
NiSmartPointer(sdNifResource);
//-------------------------------------------------------------------------------------------------
#include "sdNifResource.inl"
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif