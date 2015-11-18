//*************************************************************************************************
// ����:	��Դ����ؽӿ�
//---------------------------------------------------------
// ����:		
// ����:		2013-01-31
// ����޸�:
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

	// (���߳�ʹ��)
	virtual void RemoveAllInstance() = 0;	
};
NiSmartPointer(IResourcePool);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif