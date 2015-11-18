//-------------------------------------------------------------------------------------------------
NiStream* sdResourceSystem::GetSyncStream()
{
	return m_spSyncStream;
}
//-------------------------------------------------------------------------------------------------
NiStream* sdResourceSystem::GetAsyncStream()
{
	return m_spAsyncStream;
}
//-------------------------------------------------------------------------------------------------
sdNifResourcePool* sdResourceSystem::GetNifResourcePool()
{
	return m_pkNifResourcePool;
}
//-------------------------------------------------------------------------------------------------
sdKfResourcePool* sdResourceSystem::GetKfResourcePool()
{
	return m_pkKfResourcePool;
}
//-------------------------------------------------------------------------------------------------
sdKfmResourcePool* sdResourceSystem::GetKfmResourcePool()
{
	return m_pkKfmResourcePool;
}
//-------------------------------------------------------------------------------------------------
sdResourceSystem::ACTORMANAGERCREATEFUNC sdResourceSystem::GetActorManagerCreateFunc()
{
	return m_pfnActorManagerCreateFunc;
}
//-------------------------------------------------------------------------------------------------
//IResourcePtr sdResourceSystem::LoadResource(const sdResourceParam& kResParam, void (* pfnCallback)(IResource*))
//{
//	sdResourceEventPtr pkResourceEvent = NiNew sdResourceEvent(pfnCallback);
//	NIASSERT(pkResourceEvent);
//
//	sdResource::ResTicket kResTicket = 0;
//
//	return LoadResource(kResParam, kResTicket, pkResourceEvent);
//}
//-------------------------------------------------------------------------------------------------
//IResourcePtr sdResourceSystem::LoadResource(const sdResourceParam& kResParam, IResource::ResTicket& kResTicket, void (* pfnCallback)(IResource*))
//{
//	sdResourceEventPtr pkResourceEvent = NiNew sdResourceEvent(pfnCallback);
//	NIASSERT(pkResourceEvent);
//
//	return LoadResource(kResParam, kResTicket, pkResourceEvent);
//}
//-------------------------------------------------------------------------------------------------
//template <typename T>
//IResourcePtr sdResourceSystem::LoadResource(const sdResourceParam& kResParam, T* pkObject, void (* pfnCallback)(IResource*))
//{
//	sdResourceEventPtr pkResourceEvent = 0;
//	if (pkObject)
//	{
//		pkResourceEvent = NiNew sdResourceEvent(pkObject, pfnCallback);
//		NIASSERT(pkResourceEvent);
//	}
//
//	sdResource::ResTicket kResTicket = 0;
//
//	return LoadResource(kResParam, kResTicket, pkResourceEvent);
//}
//-------------------------------------------------------------------------------------------------
//template <typename T>
//IResourcePtr sdResourceSystem::LoadResource(const sdResourceParam& kResParam, IResource::ResTicket& kResTicket, T* pkObject, void (* pfnCallback)(IResource*))
//{
//	sdResourceEventPtr pkResourceEvent = 0;
//	if (pkObject)
//	{
//		pkResourceEvent = NiNew sdResourceEvent(pkObject, pfnCallback);
//		NIASSERT(pkResourceEvent);
//	}
//
//	return LoadResource(kResParam, kResTicket, pkResourceEvent);
//}
//-------------------------------------------------------------------------------------------------