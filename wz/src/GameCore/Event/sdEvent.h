//*************************************************************************************************
// 内容:	事件
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-27
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMECORE_EVENT_H__
#define _SD_GAMECORE_EVENT_H__
#include "sdEventType.h"
#include "sdEventArg.h"

SD_GAMECORE_NAMESPACE_BEGIN_GAMECORE
class sdEventHandle;
SD_GAMECORE_NAMESPACE_END_GAMECORE

SD_GAMECORE_NAMESPACE_BEGIN_GAMECORE
//-------------------------------------------------------------------------------------------------
class sdEvent
{
public:
	sdEvent(eEventID eID);
	virtual ~sdEvent();

	//
	eEventID GetEventID() const { return m_eEventID;}

	// 事件订阅与取消订阅
	void	Subscribe(sdEventHandle* pkSubscriber);
	void	Unsubscribe(sdEventHandle* pkSubscriber, bool bAutoDelete = true);
	void	UnsubscribeAll();

	// 执行回调
	void	operator()(const stEventArg& pkEventArg);

protected:
	// 事件
	eEventID	m_eEventID;

	// 订阅者列表
	typedef std::set<sdEventHandle*> SubscriberSet;
	typedef std::set<sdEventHandle*>::iterator SubscriberSetItr;
	SubscriberSet m_kSubscriberSet;
};
//-------------------------------------------------------------------------------------------------
SD_GAMECORE_NAMESPACE_END_GAMECORE
#endif