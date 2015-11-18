//*************************************************************************************************
// 内容:	
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-28
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMECORE_EVENTSET_H__
#define _SD_GAMECORE_EVENTSET_H__
#include "sdEvent.h"

SD_GAMECORE_NAMESPACE_BEGIN_GAMECORE

// 用于保存本地订阅信息,要订阅事件的类可以从这里继承
class sdEventSet
{
public:
	sdEventSet();
	virtual ~sdEventSet();

	// 事件订阅与取消订阅
	void	SubscribeEvent(eEventID eID, sdEventHandle* pkSubscriber);
	void	UnsubscribeEvent(eEventID eID, sdEventHandle* pkSubscriber);
	void	UnsubscribeAllEvents();

protected:
	// 事件列表
	typedef std::map<eEventID, sdEventHandle*> EventMap;
	typedef std::map<eEventID, sdEventHandle*>::iterator EventMapItr;
	EventMap m_kEventMap;
};

SD_GAMECORE_NAMESPACE_END_GAMECORE
#endif