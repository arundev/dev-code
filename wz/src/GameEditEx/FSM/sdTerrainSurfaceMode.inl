//-------------------------------------------------------------------------------------------------
inline void sdTerrainSurfaceMode::SetShowBrush(bool bShow)
{
	sdBrushShape* pkBrushShape = m_pkEditBrush->GetBrushShape();
	NIASSERT(pkBrushShape);
	pkBrushShape->SetAppCulled(!bShow);
}
//-------------------------------------------------------------------------------------------------
inline float sdTerrainSurfaceMode::GetOutterRadius()
{
	sdEditShape* pkEditShape = m_pkEditBrush->GetEditShape();
	NIASSERT(pkEditShape);

	sdDualCircleShape* pkDualCircleShape = (sdDualCircleShape*)pkEditShape;
	return pkDualCircleShape->GetOuterRadius();
}
//-------------------------------------------------------------------------------------------------
inline void sdTerrainSurfaceMode::SetOutterRadius(float fRadius)
{
	sdEditShape* pkEditShape = m_pkEditBrush->GetEditShape();
	NIASSERT(pkEditShape);

	sdDualCircleShape* pkDualCircleShape = (sdDualCircleShape*)pkEditShape;
	pkDualCircleShape->SetOuterRadius(fRadius);

	sdBrushShape* pkBrushShape = m_pkEditBrush->GetBrushShape();
	NIASSERT(pkBrushShape);

	sdDualCircleMesh* pkDualCircleMesh = (sdDualCircleMesh*)pkBrushShape;
	pkDualCircleMesh->SetOuterRadius(fRadius);
}
//-------------------------------------------------------------------------------------------------
inline float sdTerrainSurfaceMode::GetInnerRadius()
{
	sdEditShape* pkEditShape = m_pkEditBrush->GetEditShape();
	NIASSERT(pkEditShape);

	sdDualCircleShape* pkDualCircleShape = (sdDualCircleShape*)pkEditShape;
	return pkDualCircleShape->GetInnerRadius();
}
//-------------------------------------------------------------------------------------------------
inline void sdTerrainSurfaceMode::SetInnerRadius(float fRadius)
{
	sdEditShape* pkEditShape = m_pkEditBrush->GetEditShape();
	NIASSERT(pkEditShape);

	sdDualCircleShape* pkDualCircleShape = (sdDualCircleShape*)pkEditShape;
	pkDualCircleShape->SetInnerRadius(fRadius);

	sdBrushShape* pkBrushShape = m_pkEditBrush->GetBrushShape();
	NIASSERT(pkBrushShape);

	sdDualCircleMesh* pkDualCircleMesh = (sdDualCircleMesh*)pkBrushShape;
	pkDualCircleMesh->SetInnerRadius(fRadius);
}
//-------------------------------------------------------------------------------------------------
inline float sdTerrainSurfaceMode::GetHardness()
{
	sdEditShape* pkEditShape = m_pkEditBrush->GetEditShape();
	NIASSERT(pkEditShape);

	sdDualCircleShape* pkDualCircleShape = (sdDualCircleShape*)pkEditShape;
	return pkDualCircleShape->GetHardness();
}
//-------------------------------------------------------------------------------------------------
inline void sdTerrainSurfaceMode::SetHardness(float fHardness)
{
	sdEditShape* pkEditShape = m_pkEditBrush->GetEditShape();
	NIASSERT(pkEditShape);

	sdDualCircleShape* pkDualCircleShape = (sdDualCircleShape*)pkEditShape;
	pkDualCircleShape->SetHardness(fHardness);
}
//-------------------------------------------------------------------------------------------------