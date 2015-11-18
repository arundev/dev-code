#include "sdBasePCH.h"
#include "sdFSM.h"

BASE_NAMESPACE_BEGIN_BASE
//-------------------------------------------------------------------------------------------------
sdFSM::sdFSM()
: m_pkState(NULL)
, m_pkNextState(NULL)
, m_pkDelayNextState(NULL)
{

}
//-------------------------------------------------------------------------------------------------
sdFSM::~sdFSM()
{
	StateMapItr itr = m_kStateMap.begin();
	StateMapItr itr_end = m_kStateMap.end();
	for (; itr!=itr_end; ++itr)
	{
		sdState* pkState = itr->second;
		if (pkState)	
		{
			delete pkState;
			pkState = NULL;
		}
	}
	m_kStateMap.clear();
}
//-------------------------------------------------------------------------------------------------
void sdFSM::SetState(int iStateID)
{
	m_pkNextState = NULL;
	m_pkDelayNextState = NULL;

	StateMapItr itr = m_kStateMap.find(iStateID);
	if (itr != m_kStateMap.end())
	{
		sdState* pkState = itr->second;
		m_pkNextState = pkState;

		// 不同状态之间状态切换
		if (pkState && pkState != m_pkState)
		{
			if (m_pkState)
			{
				OnBeforeLeave(m_pkState->GetStateID());
				m_pkState->Leave();
			}

			m_pkState = pkState;

			{
				m_pkState->Enter();
				OnAfterEnter(m_pkState->GetStateID());
			}
		}
	}
}
//-------------------------------------------------------------------------------------------------
void sdFSM::DelaySetState(int iStateID)
{
	StateMapItr itr = m_kStateMap.find(iStateID);
	if (itr != m_kStateMap.end())
	{
		sdState* pkState = itr->second;

		// 不同状态之间状态切换
		if (pkState && pkState != m_pkState)
		{
			m_pkNextState = pkState;
			m_pkDelayNextState = pkState;
		}
	}
}
//-------------------------------------------------------------------------------------------------
int sdFSM::UpdateState()
{
	// 处理延迟状态切换
	if (m_pkDelayNextState &&  m_pkDelayNextState != m_pkState)
	{
		if (m_pkState)
		{
			OnBeforeLeave(m_pkState->GetStateID());
			m_pkState->Leave();
		}

		m_pkState = m_pkDelayNextState;

		{
			m_pkState->Enter();
			OnAfterEnter(m_pkState->GetStateID());
		}
	}

	// 更新当前状态
	if (m_pkState)
		return m_pkState->Update();

	return 0;
}
//-------------------------------------------------------------------------------------------------
sdState* sdFSM::GetCurrentState()
{
	return m_pkState;
}
//-------------------------------------------------------------------------------------------------
void sdFSM::AddState(sdState* pkState)
{
	assert(pkState);
	m_kStateMap[pkState->GetStateID()] = pkState;
}
//-------------------------------------------------------------------------------------------------
sdState* sdFSM::FindState(int iStateID)
{
	StateMapItr itr = m_kStateMap.find(iStateID);
	if (itr != m_kStateMap.end())
		return itr->second;

	return NULL;
}
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_BASE