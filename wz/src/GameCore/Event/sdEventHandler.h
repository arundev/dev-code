//*************************************************************************************************
// 内容:	事件类型
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-27
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMECORE_EVENTHANDLE_H__
#define _SD_GAMECORE_EVENTHANDLE_H__
#include "sdEventArg.h"

SD_GAMECORE_NAMESPACE_BEGIN_GAMECORE
//---------------------------------------------------------------------------------------
// 事件处理函数接口
//---------------------------------------------------------------------------------------
class sdEventHandle
{
public:
	virtual ~sdEventHandle() {}
	virtual bool operator()(const stEventArg& kEventArg) = 0;
};
//---------------------------------------------------------------------------------------
// 成员函数事件处理者
//---------------------------------------------------------------------------------------
template<typename T>
class sdTMemberFunctionHandle : public sdEventHandle
{
public:
	typedef bool (T::*MemberFunctionType)(const stEventArg&);

public:
	sdTMemberFunctionHandle(T* pkObject, MemberFunctionType pfnFunc): m_pkObject(pkObject), m_pfnFunction(pfnFunc){}
	virtual ~sdTMemberFunctionHandle(){}

	virtual bool operator()(const stEventArg& kEventArg)
	{
		return (m_pkObject->*m_pfnFunction)(kEventArg);
	}

protected:
	T*					m_pkObject;		// 绑定对象
	MemberFunctionType	m_pfnFunction;	// 绑定成员函数
};
//---------------------------------------------------------------------------------------
#define BEGIN_EVENT(className)			{ typedef className TempClass;	
#define SUBSCRIBE_EVENT(event, func)	{ SubscribeEvent(event, new sdTMemberFunctionHandle<TempClass>(this, &TempClass::func)); }
#define SUBSCRIBE_EVENT2(event, func)	{ sdEventMgr::Instance().SubscribeEvent(event, new sdTMemberFunctionHandle<TempClass>(this, &TempClass::func)); }
#define END_EVENT()						}
//---------------------------------------------------------------------------------------
SD_GAMECORE_NAMESPACE_END_GAMECORE
#endif