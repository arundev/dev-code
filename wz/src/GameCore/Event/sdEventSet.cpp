#include "sdGameCorePCH.h"
#include "sdEventSet.h"
#include "sdEventMgr.h"

SD_GAMECORE_NAMESPACE_BEGIN_GAMECORE
//-------------------------------------------------------------------------------------------------
sdEventSet::sdEventSet()
{

}
//-------------------------------------------------------------------------------------------------
sdEventSet::~sdEventSet()
{
	UnsubscribeAllEvents();
}
//-------------------------------------------------------------------------------------------------
void sdEventSet::SubscribeEvent(eEventID eID, sdEventHandle* pkSubscriber)
{
	sdEventMgr::Instance().SubscribeEvent(eID, pkSubscriber);
	m_kEventMap[eID] = pkSubscriber;
}
//-------------------------------------------------------------------------------------------------
void sdEventSet::UnsubscribeEvent(eEventID eID, sdEventHandle* pkSubscriber)
{
	sdEventMgr::Instance().UnsubscribeEvent(eID, pkSubscriber);
	EventMapItr itr = m_kEventMap.find(eID);
	if (itr != m_kEventMap.end())
		m_kEventMap.erase(itr);
}
//-------------------------------------------------------------------------------------------------
void sdEventSet::UnsubscribeAllEvents()
{
	EventMapItr itr = m_kEventMap.begin();
	EventMapItr itr_end = m_kEventMap.end();
	for (; itr!= itr_end; ++itr)
	{
		sdEventMgr::Instance().UnsubscribeEvent(itr->first, itr->second);
	}
	m_kEventMap.clear();
}
//-------------------------------------------------------------------------------------------------
SD_GAMECORE_NAMESPACE_END_GAMECORE