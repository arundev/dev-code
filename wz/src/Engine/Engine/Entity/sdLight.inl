//-------------------------------------------------------------------------------------------------
SD_INLINE uint sdLight::GetLightType() const
{
	return m_eLightType;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdLight::SetAmbientColor(const NiColor& kColor)
{
	m_kAmbientColor = kColor;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE NiColor sdLight::GetAmbientColor() const
{
	return m_kAmbientColor;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdLight::SetDiffuseColor(const NiColor& kColor)
{
	m_kDiffuseColor = kColor;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE NiColor sdLight::GetDiffuseColor() const
{
	return m_kDiffuseColor;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdLight::SetSpecularColor(const NiColor& kColor)
{
	m_kSpecularColor = kColor;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE NiColor sdLight::GetSpecularColor() const
{
	return m_kSpecularColor;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdLight::SetDimmer(const float& fDimmer)
{
	m_fDimmer = fDimmer;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE float sdLight::GetDimmer() const
{
	return m_fDimmer;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdLight::SetAttenuRange(const float& fAttenuRange)
{
	m_fAttenuRange = fAttenuRange;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE float sdLight::GetAttenuRange() const
{
	return m_fAttenuRange;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdLight::SetAttenuSoftness(const float& fAttenuSoftness)
{
	m_fAttenuSoftness = fAttenuSoftness;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE float sdLight::GetAttenuSoftness() const
{
	return m_fAttenuSoftness;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdLight::SetSpotFovX(const float& fFovX)
{
	m_fSpotFovX = fFovX;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE float sdLight::GetSpotFovX() const
{
	return m_fSpotFovX;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdLight::SetSpotFovY(const float& fFovY)
{
	m_fSpotFovY = fFovY;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE float sdLight::GetSpotFovY() const
{
	return m_fSpotFovY;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdLight::SetSpotSoftness(const float& fSoftness)
{
	m_fSpotSoftness = fSoftness;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE float sdLight::GetSpotSoftness() const
{
	return m_fSpotSoftness;
}
//-------------------------------------------------------------------------------------------------