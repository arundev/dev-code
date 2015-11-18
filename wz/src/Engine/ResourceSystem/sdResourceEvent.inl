//-------------------------------------------------------------------------------------------------
// sdResourceEvent
//-------------------------------------------------------------------------------------------------
//const void* sdResourceEvent::GetObject() const 
//{ 
//	return m_pkObject;
//}
//-------------------------------------------------------------------------------------------------
//sdResource::ResTicket sdResourceEvent::GetTicket() const 
//{ 
//	return m_kTicket;
//}
//-------------------------------------------------------------------------------------------------
//void sdResourceEvent::SetTicket(sdResource::ResTicket kResTicket) 
//{
//	m_kTicket = kResTicket;
//}
//-------------------------------------------------------------------------------------------------
const IResourceEventListener* sdResourceEvent::GetObject() const 
{ 
	return m_pkObject;
}
//-------------------------------------------------------------------------------------------------
sdResource::ResTicket sdResourceEvent::GetTicket() const 
{ 
	return m_kTicket;
}
//-------------------------------------------------------------------------------------------------
void sdResourceEvent::SetTicket(sdResource::ResTicket kResTicket) 
{
	m_kTicket = kResTicket;
}
//-------------------------------------------------------------------------------------------------
// sdResourceEventList
//-------------------------------------------------------------------------------------------------
sdResourceEventList& sdResourceEventList::AddEvent(const sdResourceEventPtr& pkEvent)
{
	if (pkEvent) 
		m_kEventList.AddTail(pkEvent);

	return (*this);
}
//-------------------------------------------------------------------------------------------------
sdResourceEventList& sdResourceEventList::RemoveEvent(const sdResourceEventPtr& pkEvent)
{
	if (pkEvent) 
		m_kEventList.Remove(pkEvent);

	return (*this);
}
//-------------------------------------------------------------------------------------------------
void sdResourceEventList::operator()(sdResource* pkResource)
{
	NiTListIterator kIter = m_kEventList.GetHeadPos();
	while (kIter)
	{
		sdResourceEvent* pkResourceEvent = m_kEventList.GetNext(kIter);
		NIASSERT(pkResourceEvent);
		(*pkResourceEvent)(pkResource);
	}
}
//-------------------------------------------------------------------------------------------------
uint sdResourceEventList::GetSize() const
{
	return m_kEventList.GetSize();
}
//-------------------------------------------------------------------------------------------------
const NiTPointerList<sdResourceEventPtr>& sdResourceEventList::GetEvents() const
{
	return m_kEventList;
}
//-------------------------------------------------------------------------------------------------