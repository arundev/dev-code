//-------------------------------------------------------------------------------------------------
inline sdLight* sdMap::GetAmbientLight() const
{
	return m_pkAmbientLight;
}
//-------------------------------------------------------------------------------------------------
inline sdLight* sdMap::GetMainLight() const
{
	return m_pkMainLight;
}
//-------------------------------------------------------------------------------------------------
inline sdLight* sdMap::GetAssistantLight() const
{
	return m_pkAssistantLight;
}
//-------------------------------------------------------------------------------------------------
inline sdFlare* sdMap::GetFlare() const
{
	return m_pkFlare;
}
//-------------------------------------------------------------------------------------------------
inline sdSun* sdMap::GetSun() const
{
	return m_pkSun;
}
//-------------------------------------------------------------------------------------------------
inline sdSkyBox* sdMap::GetSkyBox() const
{
	return m_pkSkyBox;
}
//-------------------------------------------------------------------------------------------------
inline sdBGScroll* sdMap::GetBGScroll() const
{
	return m_pkBGScroll;
}
//-------------------------------------------------------------------------------------------------
inline sdCloud* sdMap::GetCloudDome() const
{
	return m_pkCloudDome;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE sdGameObject* sdMap::GetTerrainProp() const
{
	return m_pkTerrain;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdMap::SetTerrainProp(const sdGameObject*& val)
{
	AddEntity((sdEntity*)val);
}
//-------------------------------------------------------------------------------------------------
SD_INLINE uint sdMap::GetLight(sdGameObject*& kValue, uint uiIndex) const
{
	uint uiNum = m_spLightNode->GetChildCount();
	uint uiSize = m_spLightNode->GetArrayCount();
	for (uint ui = 0, uiValidIndex = 0; ui < uiSize; ++ui)
	{
		sdGameObject* pkGameObject = NiDynamicCast(sdGameObject, m_spLightNode->GetAt(ui));
		if (pkGameObject && uiValidIndex == uiIndex)
		{
			kValue = pkGameObject;
			break;
		}

		if (pkGameObject != NULL)
			++uiValidIndex;
	}

	return uiNum;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE uint sdMap::SetLight(const sdGameObject*& pkGameObject, uint uiIndex)
{
	AddEntity((sdEntity*)pkGameObject);
	return m_spLightNode->GetChildCount();
}
//-------------------------------------------------------------------------------------------------