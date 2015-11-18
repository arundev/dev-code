//-------------------------------------------------------------------------------------------------
inline sdEditBrush::EBrushType sdTerrainDeformMode::GetBrushType()
{
	if (m_pkActiveBrush)
		return m_pkActiveBrush->GetType();
	else
		return sdEditBrush::E_BRUSH_INVALID;
}
//-------------------------------------------------------------------------------------------------
inline void sdTerrainDeformMode::SetShowBrush(bool bShow)
{
	if (m_pkActiveBrush)
	{
		sdBrushShape* pkBrushShape = m_pkActiveBrush->GetBrushShape();
		NIASSERT(pkBrushShape);
		pkBrushShape->SetAppCulled(!bShow);
	}
}
//-------------------------------------------------------------------------------------------------
inline float sdTerrainDeformMode::GetOutterRadius()
{
	if (m_pkActiveBrush)
	{
		sdEditShape* pkEditShape = m_pkActiveBrush->GetEditShape();
		NIASSERT(pkEditShape);

		sdDualCircleShape* pkDualCircleShape = (sdDualCircleShape*)pkEditShape;
		return pkDualCircleShape->GetOuterRadius();
	}

	return 0.f;
}
//-------------------------------------------------------------------------------------------------
inline void sdTerrainDeformMode::SetOutterRadius(float fRadius)
{
	if (m_pkActiveBrush)
	{
		sdEditShape* pkEditShape = m_pkActiveBrush->GetEditShape();
		NIASSERT(pkEditShape);

		sdDualCircleShape* pkDualCircleShape = (sdDualCircleShape*)pkEditShape;
		pkDualCircleShape->SetOuterRadius(fRadius);

		sdBrushShape* pkBrushShape = m_pkActiveBrush->GetBrushShape();
		NIASSERT(pkBrushShape);

		sdDualCircleMesh* pkDualCircleMesh = (sdDualCircleMesh*)pkBrushShape;
		pkDualCircleMesh->SetOuterRadius(fRadius);
	}
}
//-------------------------------------------------------------------------------------------------
inline float sdTerrainDeformMode::GetInnerRadius()
{
	if (m_pkActiveBrush)
	{
		sdEditShape* pkEditShape = m_pkActiveBrush->GetEditShape();
		NIASSERT(pkEditShape);

		sdDualCircleShape* pkDualCircleShape = (sdDualCircleShape*)pkEditShape;
		return pkDualCircleShape->GetInnerRadius();
	}

	return 0.f;
}
//-------------------------------------------------------------------------------------------------
inline void sdTerrainDeformMode::SetInnerRadius(float fRadius)
{
	if (m_pkActiveBrush)
	{
		sdEditShape* pkEditShape = m_pkActiveBrush->GetEditShape();
		NIASSERT(pkEditShape);

		sdDualCircleShape* pkDualCircleShape = (sdDualCircleShape*)pkEditShape;
		pkDualCircleShape->SetInnerRadius(fRadius);

		sdBrushShape* pkBrushShape = m_pkActiveBrush->GetBrushShape();
		NIASSERT(pkBrushShape);

		sdDualCircleMesh* pkDualCircleMesh = (sdDualCircleMesh*)pkBrushShape;
		pkDualCircleMesh->SetInnerRadius(fRadius);
	}
}
//-------------------------------------------------------------------------------------------------
inline float sdTerrainDeformMode::GetHardness()
{
	if (m_pkActiveBrush)
	{
		sdEditShape* pkEditShape = m_pkActiveBrush->GetEditShape();
		NIASSERT(pkEditShape);

		sdDualCircleShape* pkDualCircleShape = (sdDualCircleShape*)pkEditShape;
		return pkDualCircleShape->GetHardness();
	}

	return 0.f;
}
//-------------------------------------------------------------------------------------------------
inline void sdTerrainDeformMode::SetHardness(float fHardness)
{
	if (m_pkActiveBrush)
	{
		sdEditShape* pkEditShape = m_pkActiveBrush->GetEditShape();
		NIASSERT(pkEditShape);

		sdDualCircleShape* pkDualCircleShape = (sdDualCircleShape*)pkEditShape;
		pkDualCircleShape->SetHardness(fHardness);
	}
}
//-------------------------------------------------------------------------------------------------
inline float sdTerrainDeformMode::GetHeight()
{
	if (m_pkActiveBrush)
	{
		if (m_pkActiveBrush->GetType() == sdEditBrush::E_BRUSH_PULL)
		{
			sdTerrainDeformPoolBrush* pkPullBrush = (sdTerrainDeformPoolBrush*)(sdEditBrush*)m_pkActiveBrush;
			return pkPullBrush->GetHeight();
		}
		else if (m_pkActiveBrush->GetType() == sdEditBrush::E_BRUSH_NOISE)
		{
			sdTerrainDeformNoiseBrush* pkNoiseBrush = (sdTerrainDeformNoiseBrush*)(sdEditBrush*)m_pkActiveBrush;
			return pkNoiseBrush->GetHeight();
		}
		else if (m_pkActiveBrush->GetType() == sdEditBrush::E_BRUSH_FLAT)
		{
			sdTerrainDeformFlatBrush* pkFlatBrush = (sdTerrainDeformFlatBrush*)(sdEditBrush*)m_pkActiveBrush;
			return pkFlatBrush->GetHeight();
		} 
	}

	return 0.f;
}
//-------------------------------------------------------------------------------------------------
inline void sdTerrainDeformMode::SetHeight(float fHeight)
{
	if (m_pkActiveBrush)
	{
		if (m_pkActiveBrush->GetType() == sdEditBrush::E_BRUSH_PULL)
		{
			sdTerrainDeformPoolBrush* pkPullBrush = (sdTerrainDeformPoolBrush*)(sdEditBrush*)m_pkActiveBrush;
			pkPullBrush->SetHeight(fHeight);
		}
		else if (m_pkActiveBrush->GetType() == sdEditBrush::E_BRUSH_NOISE)
		{
			sdTerrainDeformNoiseBrush* pkNoiseBrush = (sdTerrainDeformNoiseBrush*)(sdEditBrush*)m_pkActiveBrush;
			pkNoiseBrush->SetHeight(fHeight);
		}
		else if (m_pkActiveBrush->GetType() == sdEditBrush::E_BRUSH_FLAT)
		{
			sdTerrainDeformFlatBrush* pkFlatBrush = (sdTerrainDeformFlatBrush*)(sdEditBrush*)m_pkActiveBrush;
			return pkFlatBrush->SetHeight(fHeight);
		} 
	}
}
//-------------------------------------------------------------------------------------------------