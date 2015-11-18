//*************************************************************************************************
// 内容:		资源回调事件(模版方式)
//			0.C回调函数或C++的类的静态成员函数,并传入对象指针,使用友元
//			1.虚函数方式方式是:
//				a.暴露一个ILoadCallback接口,里面有一组回调接口
//				b.所有异步加载类继承自ILoadCallback
//				c.事件保存指向异步加载对象的指针
//			2.模版方式是直接取出指针
//			3.Hack方式,使用汇编指令传入this指针到ecx
//---------------------------------------------------------
// 作者:		
// 创建:		2013-02-02
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RESOURCE_SYSTEM_RESOURCE_EVENT_H__
#define _SD_ENGINE_RESOURCE_SYSTEM_RESOURCE_EVENT_H__
#include <IResourceSystem.h>

#include "sdResource.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RESOURCESYSTEM
//-------------------------------------------------------------------------------------------------
// sdResourceEvent, 以Template方式获取对象的类型信息
//-------------------------------------------------------------------------------------------------
//class sdResourceEvent : public NiRefObject
//{
//public:
//	class sdInner {};
//	typedef void (sdInner::* INNERHANDLER)(IResource* pkResource);	///< 成员函数指针
//	typedef void (* OUTERHANDLER)(IResource* pkResource);			///< 函数指针
//
//public:
//	sdResourceEvent()
//		: m_pkObject(NULL), m_kTicket(sdResource::INVALID_RES_TICKET)
//	{
//		m_ukHandler.pfnOuterHandler = NULL;
//	}
//
//	// C函数或C++类的静态成员函数作为回调函数
//	sdResourceEvent(sdResourceEvent::OUTERHANDLER pfnHandler)
//		: m_pkObject(NULL), m_kTicket(sdResource::INVALID_RES_TICKET)
//	{
//		m_ukHandler.pfnOuterHandler = pfnHandler;
//	}
//
//	// C++类的普通成员函数作为回调函数
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
//	// 资源ID
//	inline sdResource::ResTicket GetTicket() const;
//	inline void SetTicket(sdResource::ResTicket kResTicket);
//
//protected:
//	// 对象指针
//	sdInner* m_pkObject;
//
//	// 函数指针/对象成员函数指针
//	union
//	{
//		INNERHANDLER pfnInnerHandler;
//		OUTERHANDLER pfnOuterHandler;
//	} m_ukHandler;
//
//	// 资源ID
//	sdResource::ResTicket m_kTicket;	
//};
//NiSmartPointer(sdResourceEvent);
//-------------------------------------------------------------------------------------------------
// sdResourceEvent, 以virtual方式获取部分对象的类型信息
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

	// 回调对象
	inline const IResourceEventListener* GetObject() const;

	// 资源ID
	inline sdResource::ResTicket GetTicket() const;
	inline void SetTicket(sdResource::ResTicket kResTicket);

protected:
	IResourceEventListener* m_pkObject;	///< 请求资源加载的对象
	sdResource::ResTicket m_kTicket;	///< 资源ID
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