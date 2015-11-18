//*************************************************************************************************
// 内容:		资源
//---------------------------------------------------------
// 作者:		
// 创建:		2013-01-31
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RESOURCE_SYSTEM_RESOURCE_GROUP_H__
#define _SD_ENGINE_RESOURCE_SYSTEM_RESOURCE_GROUP_H__
#include "sdResource.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RESOURCESYSTEM
//-------------------------------------------------------------------------------------------------
// sdResourceGroup
//
// \TODO: 改进Ticket生成与检索
//-------------------------------------------------------------------------------------------------
class sdResourceGroup : public NiRefObject
{
	friend class sdResourceSystem;
public:
	typedef stdext::hash_map<std::string, sdResource::ResTicket> NameToResTicketMap;
	typedef stdext::hash_map<std::string, sdResource::ResTicket>::iterator NameToResTicketMapItr;

	typedef std::map<sdResource::ResTicket, sdResourcePtr> ResourceMap;
	typedef std::map<sdResource::ResTicket, sdResourcePtr>::iterator ResourceMapItr;

public:
	sdResourceGroup(EResType eResType);
	virtual ~sdResourceGroup();

	// 查找资源
	inline sdResource* FindResource(sdResource::ResTicket kResTicket);
	inline sdResource* FindResource(const char* szFileName);

	// 添加资源
	sdResource::ResTicket AddResource(sdResource* pkResource, bool bGenerateKey);

	// 删除资源
	sdResourcePtr RemoveResource(sdResource::ResTicket kResTicket);

	// 清除所有未使用资源
	void RemoveAllUnusedResource();

protected:
	// 垃圾收集
	void GarbageCollect(ResourceList kResourceList);

protected:
	EResType			m_eResType;
	NameToResTicketMap	m_kNameToResTicketMap;	///< Name到Ticket映射
	ResourceMap			m_kResourceMap;			///< Ticket到Res映射
	uint				m_uiKeyGenerator;		///< ResTicketID生成器
};
NiSmartPointer(sdResourceGroup);
//-------------------------------------------------------------------------------------------------
#include "sdResourceGroup.inl"
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif