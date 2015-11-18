//*************************************************************************************************
// 内容:	事件中心,自定义的事件机制
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-27
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMECORE_EVENTMGR_H__
#define _SD_GAMECORE_EVENTMGR_H__
#include "sdEvent.h"
#include <sdSingleton.h>

SD_GAMECORE_NAMESPACE_BEGIN_GAMECORE

// 事件中心,维护一组事件和订阅回调函数
class sdEventMgr : public Base::sdTSingleton<sdEventMgr> 
{
public:
	sdEventMgr();
	virtual ~sdEventMgr();

	// 用最大事件数量初始化
	virtual bool Initialize(uint uiMaxNum);

	// 事件订阅与取消订阅
	void	SubscribeEvent(eEventID eID, sdEventHandle* pkSubscriber);
	void	UnsubscribeEvent(eEventID eID, sdEventHandle* pkSubscriber);
	void	FireEvent(eEventID eID, const stEventArg& kArg);	

protected:
	// 事件列表
	typedef std::vector<sdEvent> EventVec;
	typedef std::vector<sdEvent>::iterator EventVecItr;
	EventVec m_kEventVec;
	uint m_uiMaxNum;
};

SD_GAMECORE_NAMESPACE_END_GAMECORE
#endif