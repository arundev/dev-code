//-------------------------------------------------------------------------------------------------
NiNode* sdNifResource::GetRootNode()
{
	return m_spRootNode;
}
//-------------------------------------------------------------------------------------------------
bool sdNifResource::Loaded()
{
	return m_spRootNode != 0;
}
//-------------------------------------------------------------------------------------------------
bool sdNifResource::Freed()
{
	if (!m_spRootNode)
		return true;

	return m_bReleaseOnGC;
}
//-------------------------------------------------------------------------------------------------