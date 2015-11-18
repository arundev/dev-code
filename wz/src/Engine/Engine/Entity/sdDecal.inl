//-------------------------------------------------------------------------------------------------
SD_INLINE void sdDecal::SetWidth(const float& fValue)
{
	m_fWidth = fValue;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE float sdDecal::GetWidth() const
{
	return m_fWidth;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdDecal::SetHeight(const float& fValue)
{
	m_fHeight = fValue;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE float sdDecal::GetHeight() const
{
	return m_fHeight;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdDecal::SetLength(const float& fValue)
{
	m_fLength = fValue;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE float sdDecal::GetLength() const
{
	return m_fLength;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdDecal::SetDiffuseMaterial(const NiColor& kValue)
{
	m_kDiffuseMaterial = kValue;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE NiColor sdDecal::GetDiffuseMaterial() const
{
	return m_kDiffuseMaterial;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdDecal::SetSpecularMaterial(const NiColor& kValue)
{
	m_kSpecularMaterial = kValue;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE NiColor sdDecal::GetSpecularMaterial() const
{
	return m_kSpecularMaterial;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdDecal::SetUVScale(const NiPoint2& kValue)
{
	m_kUVScale = kValue;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE NiPoint2 sdDecal::GetUVScale() const
{
	return m_kUVScale;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdDecal::SetUVOffset(const NiPoint2& kValue)
{
	m_kUVOffset = kValue;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE NiPoint2 sdDecal::GetUVOffset() const
{
	return m_kUVOffset;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdDecal::SetUVSpeed(const NiPoint2& kValue)
{
	m_kUVSpeed = kValue;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE NiPoint2 sdDecal::GetUVSpeed() const
{
	return m_kUVSpeed;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdDecal::SetDiffuseMap(const NiFixedString& kValue)
{
	if (m_kDiffuseMap != kValue)
	{
		m_kDiffuseMap = kValue;
		m_bDiffuseMapDirty = true;
	}
}
//-------------------------------------------------------------------------------------------------
SD_INLINE NiFixedString sdDecal::GetDiffuseMap() const
{
	return m_kDiffuseMap;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdDecal::SetNormalMap(const NiFixedString& kValue)
{
	if (m_kNormalMap != kValue)
	{
		m_kNormalMap = kValue;
		m_bNormalMapDirty = true;
	}
}
//-------------------------------------------------------------------------------------------------
SD_INLINE NiFixedString sdDecal::GetNormalMap() const
{
	return m_kNormalMap;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdDecal::SetFilterMap(const NiFixedString& kValue)
{
	if (m_kFilterMap != kValue)
	{
		m_kFilterMap = kValue;
		m_bFilterMapDirty = true;
	}
}
//-------------------------------------------------------------------------------------------------
SD_INLINE NiFixedString sdDecal::GetFilterMap() const
{
	return m_kFilterMap;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdDecal::SetRecvLighting(const bool& bValue)
{
	m_bReceiveLighting = bValue;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE bool sdDecal::GetRecvLighting() const
{
	return m_bReceiveLighting;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdDecal::SetAlpha(const float& fValue)
{
	m_fAlpha = fValue;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE float sdDecal::GetAlpha() const
{
	return m_fAlpha;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdDecal::SetBlendType(const int& iBlendType)
{
	m_iBlendType = iBlendType;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE int sdDecal::GetBlendType() const
{
	return m_iBlendType;
}
//-------------------------------------------------------------------------------------------------