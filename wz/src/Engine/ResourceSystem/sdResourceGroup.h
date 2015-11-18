//*************************************************************************************************
// ����:		��Դ
//---------------------------------------------------------
// ����:		
// ����:		2013-01-31
// ����޸�:
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
// \TODO: �Ľ�Ticket���������
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

	// ������Դ
	inline sdResource* FindResource(sdResource::ResTicket kResTicket);
	inline sdResource* FindResource(const char* szFileName);

	// �����Դ
	sdResource::ResTicket AddResource(sdResource* pkResource, bool bGenerateKey);

	// ɾ����Դ
	sdResourcePtr RemoveResource(sdResource::ResTicket kResTicket);

	// �������δʹ����Դ
	void RemoveAllUnusedResource();

protected:
	// �����ռ�
	void GarbageCollect(ResourceList kResourceList);

protected:
	EResType			m_eResType;
	NameToResTicketMap	m_kNameToResTicketMap;	///< Name��Ticketӳ��
	ResourceMap			m_kResourceMap;			///< Ticket��Resӳ��
	uint				m_uiKeyGenerator;		///< ResTicketID������
};
NiSmartPointer(sdResourceGroup);
//-------------------------------------------------------------------------------------------------
#include "sdResourceGroup.inl"
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif