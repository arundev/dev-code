//*************************************************************************************************
// ����:	�¼�����,�Զ�����¼�����
//---------------------------------------------------------
// ����:		
// ����:		2012-08-27
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMECORE_EVENTMGR_H__
#define _SD_GAMECORE_EVENTMGR_H__
#include "sdEvent.h"
#include <sdSingleton.h>

SD_GAMECORE_NAMESPACE_BEGIN_GAMECORE

// �¼�����,ά��һ���¼��Ͷ��Ļص�����
class sdEventMgr : public Base::sdTSingleton<sdEventMgr> 
{
public:
	sdEventMgr();
	virtual ~sdEventMgr();

	// ������¼�������ʼ��
	virtual bool Initialize(uint uiMaxNum);

	// �¼�������ȡ������
	void	SubscribeEvent(eEventID eID, sdEventHandle* pkSubscriber);
	void	UnsubscribeEvent(eEventID eID, sdEventHandle* pkSubscriber);
	void	FireEvent(eEventID eID, const stEventArg& kArg);	

protected:
	// �¼��б�
	typedef std::vector<sdEvent> EventVec;
	typedef std::vector<sdEvent>::iterator EventVecItr;
	EventVec m_kEventVec;
	uint m_uiMaxNum;
};

SD_GAMECORE_NAMESPACE_END_GAMECORE
#endif