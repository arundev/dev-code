//-------------------------------------------------------------------------------------------------
inline void sdLightSceneObject::SetLightType(ELightType eType)
{
	m_eLightType = eType;
}
//-------------------------------------------------------------------------------------------------
inline sdLightSceneObject::ELightType sdLightSceneObject::GetLightType() const
{
	return m_eLightType;
}
//-------------------------------------------------------------------------------------------------
inline void sdLightSceneObject::SetPosition(const NiPoint3& kPosition)
{
	m_kPosition = kPosition;
}
//-------------------------------------------------------------------------------------------------
inline const NiPoint3& sdLightSceneObject::GetPosition() const
{
	return m_kPosition;
}
//-------------------------------------------------------------------------------------------------
inline void sdLightSceneObject::SetRotate(const NiMatrix3& kRotate)
{
	m_kRotate = kRotate;
}
//-------------------------------------------------------------------------------------------------
inline const NiMatrix3& sdLightSceneObject::GetRotate() const
{
	return m_kRotate;
}
//-------------------------------------------------------------------------------------------------
inline void sdLightSceneObject::SetAmbientColor(const NiColor& kColor)
{
	m_kAmbientColor = kColor;
}
//-------------------------------------------------------------------------------------------------
inline NiColor sdLightSceneObject::GetAmbientColor() const
{
	return m_kAmbientColor;
}
//-------------------------------------------------------------------------------------------------
inline void sdLightSceneObject::SetDiffuseColor(const NiColor& kColor)
{
	m_kDiffuseColor = kColor;
}
//-------------------------------------------------------------------------------------------------
inline NiColor sdLightSceneObject::GetDiffuseColor() const
{
	return m_kDiffuseColor;
}
//-------------------------------------------------------------------------------------------------
inline void sdLightSceneObject::SetSpecularColor(const NiColor& kColor)
{
	m_kSpecularColor = kColor;
}
//-------------------------------------------------------------------------------------------------
inline NiColor sdLightSceneObject::GetSpecularColor() const
{
	return m_kSpecularColor;
}
//-------------------------------------------------------------------------------------------------
inline void sdLightSceneObject::SetDimmer(const float& fDimmer)
{
	m_fDimmer = fDimmer;
}
//-------------------------------------------------------------------------------------------------
inline float sdLightSceneObject::GetDimmer() const
{
	return m_fDimmer;
}
//-------------------------------------------------------------------------------------------------
inline void sdLightSceneObject::SetAttenuRange(float fAttenuRange)
{
	if (fAttenuRange >= 0.f)
		m_fAttenuRange = fAttenuRange;
}
//-------------------------------------------------------------------------------------------------
inline float sdLightSceneObject::GetAttenuRange() const
{
	return m_fAttenuRange;
}
//-------------------------------------------------------------------------------------------------
inline void sdLightSceneObject::SetAttenuSoftness(float fAttenuSoftness)
{
	if (fAttenuSoftness >= 0.f && fAttenuSoftness <= 1.f) 
		m_fAttenuSoftness = fAttenuSoftness;
}
//-------------------------------------------------------------------------------------------------
inline float sdLightSceneObject::GetAttenuSoftness() const
{
	return m_fAttenuSoftness;
}
//-------------------------------------------------------------------------------------------------
inline void sdLightSceneObject::SetSpotFovX(float fFovX)
{
	if (fFovX >= 0.f && fFovX <= 179.f)	
		m_fSpotFovX = fFovX;
}
//-------------------------------------------------------------------------------------------------
inline float sdLightSceneObject::GetSpotFovX() const
{
	return m_fSpotFovX;
}
//-------------------------------------------------------------------------------------------------
inline void sdLightSceneObject::SetSpotFovY(float fFovY)
{
	if (fFovY >= 0.f && fFovY <= 179.f)	
		m_fSpotFovY = fFovY;
}
//-------------------------------------------------------------------------------------------------
inline float sdLightSceneObject::GetSpotFovY() const
{
	return m_fSpotFovY;
}
//-------------------------------------------------------------------------------------------------
inline void sdLightSceneObject::SetSpotSoftness(float fSoftness)
{
	if (fSoftness >= 0.f && fSoftness <= 1.f) 
		m_fSpotSoftness = fSoftness;
}
//-------------------------------------------------------------------------------------------------
inline float sdLightSceneObject::GetSpotSoftness() const
{
	return m_fSpotSoftness;
}
//-------------------------------------------------------------------------------------------------
inline void sdLightSceneObject::SetFilterMap(NiTexture* spTexture)
{
	m_spFilterMap = spTexture;
}
//-------------------------------------------------------------------------------------------------
inline NiTexture* sdLightSceneObject::GetFilterMap() const
{
	return m_spFilterMap;
}
//-------------------------------------------------------------------------------------------------
inline void sdProjectorSceneObject::SetBlendType(EBlendType eBlendType)
{
	m_eBlendType = eBlendType;
}
//-------------------------------------------------------------------------------------------------
inline sdProjectorSceneObject::EBlendType sdProjectorSceneObject::GetBlendType() const
{
	return m_eBlendType;
}
//-------------------------------------------------------------------------------------------------
inline void sdProjectorSceneObject::SetReceiveLighting(bool bRecLighting)
{
	m_bReceiveLighting = bRecLighting;
}
//-------------------------------------------------------------------------------------------------
inline bool sdProjectorSceneObject::GetReceiveLighting() const
{
	return  m_bReceiveLighting;
}
//-------------------------------------------------------------------------------------------------
inline void sdProjectorSceneObject::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}
//-------------------------------------------------------------------------------------------------
inline bool sdProjectorSceneObject::GetVisible() const
{
	return m_bVisible;
}	
//-------------------------------------------------------------------------------------------------
inline void sdProjectorSceneObject::SetLayer(uint uiLayer)
{
	m_uiLayer = uiLayer;
}
//-------------------------------------------------------------------------------------------------
inline uint sdProjectorSceneObject::GetLaye() const
{
	return m_uiLayer;
}
//-------------------------------------------------------------------------------------------------
inline void sdProjectorSceneObject::SetPosition(const NiPoint3& kPosition)
{
	m_kPosition = kPosition;
}
//-------------------------------------------------------------------------------------------------
inline const NiPoint3& sdProjectorSceneObject::GetPosition() const
{
	return m_kPosition;
}
//-------------------------------------------------------------------------------------------------
inline void sdProjectorSceneObject::SetDirection(const NiPoint3& kDirection)
{
	m_kDirection = kDirection;
}
//-------------------------------------------------------------------------------------------------
inline const NiPoint3& sdProjectorSceneObject::GetDirection() const
{
	return m_kDirection;
}
//-------------------------------------------------------------------------------------------------
inline void sdProjectorSceneObject::SetUpDirection(const NiPoint3& kUp)
{
	m_kUp = kUp;
}
//-------------------------------------------------------------------------------------------------
inline const NiPoint3& sdProjectorSceneObject::GetUpDirection() const
{
	return m_kUp;
}
//-------------------------------------------------------------------------------------------------
inline void sdProjectorSceneObject::SetSize(const NiPoint3& kSize)
{
	m_kSize = kSize;
}
//-------------------------------------------------------------------------------------------------
inline const NiPoint3& sdProjectorSceneObject::GetSize() const
{
	return m_kSize;
}
//-------------------------------------------------------------------------------------------------
inline void sdProjectorSceneObject::SetLength(float fLength)
{
	m_kSize.z = fLength;
}
//-------------------------------------------------------------------------------------------------
inline float sdProjectorSceneObject::GetLength() const
{
	return m_kSize.z;
}
//-------------------------------------------------------------------------------------------------
inline void sdProjectorSceneObject::SetWidth(float fWidth)
{
	m_kSize.x = fWidth;
}
//-------------------------------------------------------------------------------------------------
inline float sdProjectorSceneObject::GetWidth() const
{
	return m_kSize.x;
}
//-------------------------------------------------------------------------------------------------
inline void sdProjectorSceneObject::SetHeight(float fHeight)
{
	m_kSize.y = fHeight;
}
//-------------------------------------------------------------------------------------------------
inline float sdProjectorSceneObject::GetHeight() const
{
	return m_kSize.y;
}
//-------------------------------------------------------------------------------------------------
inline void sdProjectorSceneObject::SetUVScale(const NiPoint2& kUVScale)
{
	m_kUVScale = kUVScale;
}
//-------------------------------------------------------------------------------------------------
inline const NiPoint2& sdProjectorSceneObject::GetUVScale() const
{
	return m_kUVScale;
}
//-------------------------------------------------------------------------------------------------
inline void sdProjectorSceneObject::SetUVOffset(const NiPoint2& kUVOffset)
{
	m_kUVOffset = kUVOffset;
}
//-------------------------------------------------------------------------------------------------
inline const NiPoint2& sdProjectorSceneObject::GetUVOffset() const
{
	return m_kUVOffset;
}
//-------------------------------------------------------------------------------------------------
inline void sdProjectorSceneObject::SetUVSpeed(const NiPoint2& kUVSpeed)
{
	m_kUVSpeed = kUVSpeed;
}
//-------------------------------------------------------------------------------------------------
inline const NiPoint2& sdProjectorSceneObject::GetUVSpeed() const
{
	return m_kUVSpeed;
}
//-------------------------------------------------------------------------------------------------
inline void sdProjectorSceneObject::SetAlpha(float fAlpha)
{
	m_fAlpha = fAlpha;
}
//-------------------------------------------------------------------------------------------------
inline float sdProjectorSceneObject::GetAlpha() const
{
	return m_fAlpha;
}
//-------------------------------------------------------------------------------------------------
inline void sdProjectorSceneObject::SetAlphaTest(float fAlphaTest)
{
	m_fAlphaTest = fAlphaTest;
}
//-------------------------------------------------------------------------------------------------
inline float sdProjectorSceneObject::GetAlphaTest() const
{
	return m_fAlphaTest;
}
//-------------------------------------------------------------------------------------------------
inline void sdProjectorSceneObject::SetDiffuseMaterial(const NiColor& kColor)
{
	m_kDiffuseMaterial = kColor;
}
//-------------------------------------------------------------------------------------------------
inline const NiColor& sdProjectorSceneObject::GetDiffuseMaterial() const
{
	return m_kDiffuseMaterial;
}
//-------------------------------------------------------------------------------------------------
inline void sdProjectorSceneObject::SetSpecularMaterial(const NiColor& kColor)
{
	m_kSpecularMaterial = kColor;
}
//-------------------------------------------------------------------------------------------------
inline const NiColor& sdProjectorSceneObject::GetSpecularMaterial() const
{
	return m_kSpecularMaterial;
}
//-------------------------------------------------------------------------------------------------
inline void sdProjectorSceneObject::SetShiness(float fShiness)
{
	m_fShiness = fShiness;
}
//-------------------------------------------------------------------------------------------------
inline float sdProjectorSceneObject::GetShiness() const
{
	return m_fShiness;
}
//-------------------------------------------------------------------------------------------------
inline void sdProjectorSceneObject::SetDiffuseMap(NiTexture* spTexture)
{
	m_spDiffuseMap = spTexture;
}
//-------------------------------------------------------------------------------------------------
inline NiTexture* sdProjectorSceneObject::GetDiffuseMap() const
{
	return m_spDiffuseMap;
}
//-------------------------------------------------------------------------------------------------
inline void sdProjectorSceneObject::SetNormalMap(NiTexture* spTexture)
{
	m_spNormalMap = spTexture;
}
//-------------------------------------------------------------------------------------------------
inline NiTexture* sdProjectorSceneObject::GetNormalMap() const
{
	return m_spNormalMap;
}
//-------------------------------------------------------------------------------------------------
inline void sdProjectorSceneObject::SetFilterMap(NiTexture* spTexture)
{
	m_spFilterMap = spTexture;
}
//-------------------------------------------------------------------------------------------------
inline NiTexture* sdProjectorSceneObject::GetFilterMap() const
{
	return m_spFilterMap;
}
//-------------------------------------------------------------------------------------------------
inline void sdProjectorSceneObject::SetMaterial(NiMaterial* spMaterial)
{
	m_spMaterial = spMaterial;
}
//-------------------------------------------------------------------------------------------------
inline NiMaterial* sdProjectorSceneObject::GetMaterial() const
{
	return m_spMaterial;
}
//-------------------------------------------------------------------------------------------------