//-------------------------------------------------------------------------------------------------
SD_INLINE uint sdEditTerrainHelper::GetDeformBrushType()
{ 
	return m_pkTerrainDeformMode->GetBrushType();
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdEditTerrainHelper::SetActiveDeformBrush(uint uiBrush)
{ 
	m_pkTerrainDeformMode->SetActiveDeformBrush((sdEditBrush::EBrushType)uiBrush);
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdEditTerrainHelper::SetDeformBrushVisible(bool bVisible)
{
	m_pkTerrainDeformMode->SetShowBrush(bVisible);
}
//-------------------------------------------------------------------------------------------------
SD_INLINE float sdEditTerrainHelper::GetDeformBrushOutterRadius()
{ 
	return m_pkTerrainDeformMode->GetOutterRadius();
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdEditTerrainHelper::SetDeformBrushOutterRadius(float fRadius)
{
	m_pkTerrainDeformMode->SetOutterRadius(fRadius);
}
//-------------------------------------------------------------------------------------------------
SD_INLINE float sdEditTerrainHelper::GetDeformBrushInnerRadius()
{ 
	return m_pkTerrainDeformMode->GetInnerRadius();
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdEditTerrainHelper::SetDeformBrushInnerRadius(float fRadius)
{ 
	m_pkTerrainDeformMode->SetInnerRadius(fRadius);
}
//-------------------------------------------------------------------------------------------------
SD_INLINE float sdEditTerrainHelper::GetDeformBrushHardness()
{ 
	return m_pkTerrainDeformMode->GetHardness();
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdEditTerrainHelper::SetDeformBrushHardness(float fHardness)
{ 
	m_pkTerrainDeformMode->SetHardness(fHardness);
}
//-------------------------------------------------------------------------------------------------
SD_INLINE float sdEditTerrainHelper::GetDeformBrushHeight()
{ 
	return m_pkTerrainDeformMode->GetHeight();
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdEditTerrainHelper::SetDeformBrushHeight(float fHeight)
{
	m_pkTerrainDeformMode->SetHeight(fHeight);
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdEditTerrainHelper::SetSurfaceBrushVisible(bool bVisible)
{
	m_pkTerrainSurfaceMode->SetShowBrush(bVisible);
}
//-------------------------------------------------------------------------------------------------
SD_INLINE float sdEditTerrainHelper::GetSurfaceBrushOutterRadius()
{ 
	return m_pkTerrainSurfaceMode->GetOutterRadius();
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdEditTerrainHelper::SetSurfaceBrushOutterRadius(float fRadius)
{
	m_pkTerrainSurfaceMode->SetOutterRadius(fRadius);
}
//-------------------------------------------------------------------------------------------------
SD_INLINE float sdEditTerrainHelper::GetSurfaceBrushInnerRadius()
{ 
	return m_pkTerrainSurfaceMode->GetInnerRadius();
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdEditTerrainHelper::SetSurfaceBrushInnerRadius(float fRadius)
{ 
	m_pkTerrainSurfaceMode->SetInnerRadius(fRadius);
}
//-------------------------------------------------------------------------------------------------
SD_INLINE float sdEditTerrainHelper::GetSurfaceBrushHardness()
{ 
	return m_pkTerrainSurfaceMode->GetHardness();
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdEditTerrainHelper::SetSurfaceBrushHardness(float fHardness)
{ 
	m_pkTerrainSurfaceMode->SetHardness(fHardness);
}
//-------------------------------------------------------------------------------------------------
SD_INLINE const Base::Math::sdVector3& sdEditTerrainHelper::GetAmbientMaterial() const 
{ 
	return m_pkTerrain->GetTerrainParams().ambientMaterial;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdEditTerrainHelper::SetAmbientMaterial(const Base::Math::sdVector3& kAmbientMaterial) 
{ 
	m_pkTerrain->GetTerrainParams().ambientMaterial = kAmbientMaterial;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE const Base::Math::sdVector3& sdEditTerrainHelper::GetDiffuseMaterial() const 
{ 
	return  m_pkTerrain->GetTerrainParams().diffuseMaterial;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdEditTerrainHelper::SetDiffuseMaterial(const Base::Math::sdVector3& kDiffuseMaterial)
{
	m_pkTerrain->GetTerrainParams().diffuseMaterial = kDiffuseMaterial;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE const Base::Math::sdVector3& sdEditTerrainHelper::GetSpecularMaterial() const 
{ 
	return  m_pkTerrain->GetTerrainParams().specularMaterial;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdEditTerrainHelper::SetSpecularMaterial(const Base::Math::sdVector3& kSpecularMaterial) 
{ 
	m_pkTerrain->GetTerrainParams().specularMaterial = kSpecularMaterial;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE float sdEditTerrainHelper::GetShiness() const 
{ 
	return m_pkTerrain->GetTerrainParams().shiness;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdEditTerrainHelper::SetShiness(float fShiness) 
{ 
	m_pkTerrain->GetTerrainParams().shiness = fShiness;
}
//-------------------------------------------------------------------------------------------------