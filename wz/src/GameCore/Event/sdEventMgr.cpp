#include "sdGameCorePCH.h"
#include "sdEventMgr.h"

SD_GAMECORE_NAMESPACE_BEGIN_GAMECORE
//-------------------------------------------------------------------------------------------------
sdEventMgr::sdEventMgr()
: m_uiMaxNum(0)
{

}
//-------------------------------------------------------------------------------------------------
sdEventMgr::~sdEventMgr()
{
	EventVecItr itr = m_kEventVec.begin();
	EventVecItr itr_end = m_kEventVec.end();
	for (; itr!=itr_end; ++itr)
	{
		itr->UnsubscribeAll();
	}
	m_kEventVec.clear();
}
//-------------------------------------------------------------------------------------------------
bool sdEventMgr::Initialize(uint uiMaxNum)
{
	m_uiMaxNum = uiMaxNum;

	m_kEventVec.reserve(m_uiMaxNum);
	for (uint i = 0; i < m_uiMaxNum; ++i)
		m_kEventVec.push_back(sdEvent((eEventID)i));

	return true;
}
//-------------------------------------------------------------------------------------------------
void sdEventMgr::SubscribeEvent(eEventID eID, sdEventHandle* pkSubscriber)
{
	NIASSERT(eID >= 0 && (uint)eID < m_uiMaxNum)
	m_kEventVec[eID].Subscribe(pkSubscriber);
}
//-------------------------------------------------------------------------------------------------
void sdEventMgr::UnsubscribeEvent(eEventID eID, sdEventHandle* pkSubscriber)
{
	NIASSERT(eID >= 0 && (uint)eID < m_uiMaxNum)
	m_kEventVec[eID].Unsubscribe(pkSubscriber);
}
//-------------------------------------------------------------------------------------------------
void sdEventMgr::FireEvent(eEventID eID, const stEventArg& kArg)
{
	NIASSERT(eID >= 0 && (uint)eID < m_uiMaxNum)
	(*(&m_kEventVec[eID]))(kArg);
}
//-------------------------------------------------------------------------------------------------
SD_GAMECORE_NAMESPACE_END_GAMECORE