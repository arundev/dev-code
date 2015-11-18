//*************************************************************************************************
// 内容:	资源缓存池接口
//---------------------------------------------------------
// 作者:		
// 创建:		2013-01-31
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RESOURCE_SYSTEM_RESOURCE_POOL_INTERFACE_H__
#define _SD_ENGINE_RESOURCE_SYSTEM_RESOURCE_POOL_INTERFACE_H__
#include <sdCriticalSection.h>

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RESOURCESYSTEM
//-------------------------------------------------------------------------------------------------
class IResourcePool : public NiRefObject
{
public:
	virtual ~IResourcePool() {};

	// (主线程使用)
	virtual void RemoveAllInstance() = 0;	
};
NiSmartPointer(IResourcePool);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif