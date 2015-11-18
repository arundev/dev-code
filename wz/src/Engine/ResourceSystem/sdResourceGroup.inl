//-------------------------------------------------------------------------------------------------
sdResource* sdResourceGroup::FindResource(sdResource::ResTicket kResTicket)
{
	sdResource* pkResource = NULL;
	ResourceMapItr itr = m_kResourceMap.find(kResTicket);
	ResourceMapItr itr_end = m_kResourceMap.end();
	if (itr != itr_end)
		pkResource = itr->second;

	return pkResource;
}
//-------------------------------------------------------------------------------------------------
sdResource* sdResourceGroup::FindResource(const char* szFileName)
{
	sdResource* pkResource = NULL;
	NameToResTicketMapItr itr_name_tick = m_kNameToResTicketMap.find(szFileName);
	NameToResTicketMapItr itr_name_tick_end = m_kNameToResTicketMap.end();
	if (itr_name_tick != itr_name_tick_end)
	{
		pkResource = FindResource(itr_name_tick->second);
		if (!pkResource)
			m_kNameToResTicketMap.erase(itr_name_tick);
	}


	return pkResource;
}
//-------------------------------------------------------------------------------------------------