#include "sdGameCorePCH.h"
#include "sdEvent.h"
#include "sdEventHandler.h"

SD_GAMECORE_NAMESPACE_BEGIN_GAMECORE
//-------------------------------------------------------------------------------------------------
sdEvent::sdEvent(eEventID eID)
: m_eEventID(eID)
{

}
//-------------------------------------------------------------------------------------------------
sdEvent::~sdEvent()
{

}
//-------------------------------------------------------------------------------------------------
void sdEvent::Subscribe(sdEventHandle* pkSubscriber)
{
	NIASSERT(pkSubscriber);
	m_kSubscriberSet.insert(pkSubscriber);
}
//-------------------------------------------------------------------------------------------------
void sdEvent::Unsubscribe(sdEventHandle* pkSubscriber, bool bAutoDelete)
{
	NIASSERT(pkSubscriber);
	
	if (bAutoDelete)
		delete pkSubscriber;

	SubscriberSetItr itr = m_kSubscriberSet.find(pkSubscriber);
	if (itr != m_kSubscriberSet.end())
		m_kSubscriberSet.erase(itr);
}
//-------------------------------------------------------------------------------------------------
void sdEvent::UnsubscribeAll()
{
	SubscriberSetItr itr = m_kSubscriberSet.begin();
	SubscriberSetItr itr_end = m_kSubscriberSet.end();
	for (; itr!=itr_end; ++itr)
	{
		NIASSERT(*itr);
		delete (*itr);
	}

	m_kSubscriberSet.clear();
}
//-------------------------------------------------------------------------------------------------
void sdEvent::operator ()(const stEventArg &kEventArg)
{
	SubscriberSetItr itr = m_kSubscriberSet.begin();
	SubscriberSetItr itr_end = m_kSubscriberSet.end();
	for (; itr!=itr_end; ++itr)
	{
		(*(*itr))(kEventArg);
	}
}
//-------------------------------------------------------------------------------------------------
SD_GAMECORE_NAMESPACE_END_GAMECORE