//-------------------------------------------------------------------------------------------------
void sdResource::IgnoreResource()
{
	m_eStatus = E_RES_STATUS_IGNORED;
}
//-------------------------------------------------------------------------------------------------
bool sdResource::Loaded()
{
	return false;
}
//-------------------------------------------------------------------------------------------------
bool sdResource::Cached()
{
	return m_eStatus == E_RES_STATUS_READY;
}
//-------------------------------------------------------------------------------------------------
bool sdResource::Ignored()
{
	return m_eStatus == E_RES_STATUS_IGNORED;
}
//-------------------------------------------------------------------------------------------------
bool sdResource::Freed()
{
	return false;
}
//-------------------------------------------------------------------------------------------------
void sdResource::UpdatePrior(EResReqPrior ePrior, float fSqlDistance)
{
	m_ePrior = ePrior;
	m_fSqlDistance = fSqlDistance;
}
//-------------------------------------------------------------------------------------------------
sdResource::ResTicket sdResource::GetTicket() const
{
	return m_kResTicket;
}
//-------------------------------------------------------------------------------------------------
NiNode* sdResource::GetRootNode()
{
	return NULL;
}
//-------------------------------------------------------------------------------------------------
NiActorManager* sdResource::GetActorManager()
{
	return NULL;
}
//-------------------------------------------------------------------------------------------------
const std::string& sdResource::GetFileName() const
{
	return m_kFileName;
}
//-------------------------------------------------------------------------------------------------
void sdResource::SetTicket(sdResource::ResTicket kTicket)
{
	m_kResTicket = kTicket;
}
//-------------------------------------------------------------------------------------------------
EResUsageType sdResource::GetUsageType() const
{
	return m_eUsage;
}
//-------------------------------------------------------------------------------------------------
EResReqPrior sdResource::GetRequestPrior() const
{
	return m_ePrior;
}
//-------------------------------------------------------------------------------------------------