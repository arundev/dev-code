//*************************************************************************************************
// 内容:		资源缓存池
//---------------------------------------------------------
// 作者:		
// 创建:		2013-01-31
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RESOURCE_SYSTEM_RESOURCE_POOL_H__
#define _SD_ENGINE_RESOURCE_SYSTEM_RESOURCE_POOL_H__
#include <IResourcePool.h>

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RESOURCESYSTEM
//-------------------------------------------------------------------------------------------------
class sdResourcePool : public IResourcePool
{
protected:
	sdResourcePool();
	virtual ~sdResourcePool();

protected:
	Base::Threading::sdCriticalSection m_kLock;
	NiCloningProcess m_kCloingProcess;
};
NiSmartPointer(IResourcePool);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif