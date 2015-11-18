//*************************************************************************************************
// ����:		��Դ�ص��¼�(ģ�淽ʽ)
//			0.C�ص�������C++����ľ�̬��Ա����,���������ָ��,ʹ����Ԫ
//			1.�麯����ʽ��ʽ��:
//				a.��¶һ��ILoadCallback�ӿ�,������һ��ص��ӿ�
//				b.�����첽������̳���ILoadCallback
//				c.�¼�����ָ���첽���ض����ָ��
//			2.ģ�淽ʽ��ֱ��ȡ��ָ��
//			3.Hack��ʽ,ʹ�û��ָ���thisָ�뵽ecx
//---------------------------------------------------------
// ����:		
// ����:		2013-02-02
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RESOURCE_SYSTEM_RESOURCE_EVENT_H__
#define _SD_ENGINE_RESOURCE_SYSTEM_RESOURCE_EVENT_H__
#include <IResourceSystem.h>

#include "sdResource.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RESOURCESYSTEM
//-------------------------------------------------------------------------------------------------
// sdResourceEvent, ��Template��ʽ��ȡ�����������Ϣ
//-------------------------------------------------------------------------------------------------
//class sdResourceEvent : public NiRefObject
//{
//public:
//	class sdInner {};
//	typedef void (sdInner::* INNERHANDLER)(IResource* pkResource);	///< ��Ա����ָ��
//	typedef void (* OUTERHANDLER)(IResource* pkResource);			///< ����ָ��
//
//public:
//	sdResourceEvent()
//		: m_pkObject(NULL), m_kTicket(sdResource::INVALID_RES_TICKET)
//	{
//		m_ukHandler.pfnOuterHandler = NULL;
//	}
//
//	// C������C++��ľ�̬��Ա������Ϊ�ص�����
//	sdResourceEvent(sdResourceEvent::OUTERHANDLER pfnHandler)
//		: m_pkObject(NULL), m_kTicket(sdResource::INVALID_RES_TICKET)
//	{
//		m_ukHandler.pfnOuterHandler = pfnHandler;
//	}
//
//	// C++�����ͨ��Ա������Ϊ�ص�����
//	template <class T> 
//	sdResourceEvent(T* pkObject, void (T::* pfnHandler)(sdResource*))
//		: m_pkObject((sdInner*)pkObject), m_kTicket(sdResource::INVALID_RES_TICKET)
//	{
//		union
//		{
//			INNERHANDLER pfnInnerHandler;
//			void (T::* pfnHandler)(sdResource*);
//		}kTempUnion;
//		kTempUnion.pfnHandler = pfnHandler;
//		m_ukHandler.pfnInnerHandler = kTempUnion.pfnInnerHandler;
//	}
//
//	~sdResourceEvent(){};
//
//	inline void operator()(sdResource* pkResource)
//	{
//		if (m_pkObject)
//			(m_pkObject->*(m_ukHandler.pfnInnerHandler))(pkResource);
//		else if (m_ukHandler.pfnOuterHandler)
//			m_ukHandler.pfnOuterHandler(pkResource);	
//	}
//
//	inline const void* GetObject() const;
//
//	// ��ԴID
//	inline sdResource::ResTicket GetTicket() const;
//	inline void SetTicket(sdResource::ResTicket kResTicket);
//
//protected:
//	// ����ָ��
//	sdInner* m_pkObject;
//
//	// ����ָ��/�����Ա����ָ��
//	union
//	{
//		INNERHANDLER pfnInnerHandler;
//		OUTERHANDLER pfnOuterHandler;
//	} m_ukHandler;
//
//	// ��ԴID
//	sdResource::ResTicket m_kTicket;	
//};
//NiSmartPointer(sdResourceEvent);
//-------------------------------------------------------------------------------------------------
// sdResourceEvent, ��virtual��ʽ��ȡ���ֶ����������Ϣ
//-------------------------------------------------------------------------------------------------
class sdResourceEvent : public NiRefObject
{
public:
	sdResourceEvent()
		: m_pkObject(NULL), m_kTicket(sdResource::INVALID_RES_TICKET)
	{
	}

	sdResourceEvent(IResourceEventListener* pkObject)
		: m_pkObject(pkObject), m_kTicket(sdResource::INVALID_RES_TICKET)
	{
	}

	~sdResourceEvent()
	{
	};

	inline void operator()(sdResource* pkResource)
	{
		if (m_pkObject)
			m_pkObject->OnLoaded(pkResource);
	}

	// �ص�����
	inline const IResourceEventListener* GetObject() const;

	// ��ԴID
	inline sdResource::ResTicket GetTicket() const;
	inline void SetTicket(sdResource::ResTicket kResTicket);

protected:
	IResourceEventListener* m_pkObject;	///< ������Դ���صĶ���
	sdResource::ResTicket m_kTicket;	///< ��ԴID
};
NiSmartPointer(sdResourceEvent);
//-------------------------------------------------------------------------------------------------
// sdResourceEventList
//-------------------------------------------------------------------------------------------------
class sdResourceEventList : public NiRefObject
{
public:
	~sdResourceEventList(){ m_kEventList.RemoveAll();}

	inline sdResourceEventList& AddEvent(const sdResourceEventPtr& pkEvent);
	inline sdResourceEventList& RemoveEvent(const sdResourceEventPtr& pkEvent);

	inline void operator()(sdResource* pkResource);

	inline uint GetSize() const;
	inline const NiTPointerList<sdResourceEventPtr>& GetEvents() const;

protected:
	NiTPointerList<sdResourceEventPtr> m_kEventList;
};
NiSmartPointer(sdResourceEventList);
//-------------------------------------------------------------------------------------------------
#include "sdResourceEvent.inl"
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif