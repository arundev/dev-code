//-------------------------------------------------------------------------------------------------
NiFixedString sdBuilding::GetNIFFile() const
{
	return m_kNIFPath;
}
//-------------------------------------------------------------------------------------------------
void sdBuilding::SetNIFFile(const NiFixedString& kNIFPath)
{
	if (m_kNIFPath.EqualsNoCase(kNIFPath))
		return;

	if (m_spNIFRoot)
	{
		DetachChild(m_spNIFRoot);
		m_spNIFRoot = 0;
	}

	m_kNIFPath = kNIFPath;
}
//-------------------------------------------------------------------------------------------------