//*************************************************************************************************
// ����:	
//---------------------------------------------------------
// ����:		
// ����:		2012-08-28
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMECORE_EVENTSET_H__
#define _SD_GAMECORE_EVENTSET_H__
#include "sdEvent.h"

SD_GAMECORE_NAMESPACE_BEGIN_GAMECORE

// ���ڱ��汾�ض�����Ϣ,Ҫ�����¼�������Դ�����̳�
class sdEventSet
{
public:
	sdEventSet();
	virtual ~sdEventSet();

	// �¼�������ȡ������
	void	SubscribeEvent(eEventID eID, sdEventHandle* pkSubscriber);
	void	UnsubscribeEvent(eEventID eID, sdEventHandle* pkSubscriber);
	void	UnsubscribeAllEvents();

protected:
	// �¼��б�
	typedef std::map<eEventID, sdEventHandle*> EventMap;
	typedef std::map<eEventID, sdEventHandle*>::iterator EventMapItr;
	EventMap m_kEventMap;
};

SD_GAMECORE_NAMESPACE_END_GAMECORE
#endif