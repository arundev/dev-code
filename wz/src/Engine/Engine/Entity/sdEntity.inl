//-------------------------------------------------------------------------------------------------
SD_INLINE NiPoint3 sdEntity::GetTranslateProp() const
{
	return __super::GetTranslate();
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdEntity::SetTranslateProp(const NiPoint3& kTranslate)
{
	__super::SetTranslate(kTranslate);
}
//-------------------------------------------------------------------------------------------------
SD_INLINE NiPoint3 sdEntity::GetRotateProp() const
{
	NiPoint3 kRotate(NiPoint3::ZERO);
	const NiAVObject* pkObject = dynamic_cast<const NiAVObject*>(this);
	if (pkObject)
	{
		NiMatrix3 kRot = pkObject->GetRotate();
		kRot.ToEulerAnglesXYZ(kRotate.x, kRotate.y, kRotate.z);
		kRotate.x = Base::Math::sdMath::Radian2Degree(kRotate.x);
		kRotate.y = Base::Math::sdMath::Radian2Degree(kRotate.y);
		kRotate.z = Base::Math::sdMath::Radian2Degree(kRotate.z);
	}

	return kRotate;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdEntity::SetRotateProp(const NiPoint3& kRotate)
{
	NiAVObject* pkObject = dynamic_cast<NiAVObject*>(this);
	if (pkObject)
	{
		float fRotateX = Base::Math::sdMath::Degree2Radian(kRotate.x);
		float fRotateY = Base::Math::sdMath::Degree2Radian(kRotate.y);
		float fRotateZ = Base::Math::sdMath::Degree2Radian(kRotate.z);

		NiMatrix3 kRot;
		kRot.FromEulerAnglesXYZ(fRotateX, fRotateY, fRotateZ);

		pkObject->SetRotate(kRot);
	}
}
//-------------------------------------------------------------------------------------------------
SD_INLINE float sdEntity::GetScaleProp() const
{
	return __super::GetScale();
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdEntity::SetScaleProp(const float& fScale)
{
	__super::SetScale(fScale);
}
//-------------------------------------------------------------------------------------------------
SD_INLINE float sdEntity::GetViewDistance() const
{
	return m_fViewDistance;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdEntity::SetViewDistance(const float& fViewDistance)
{
	m_fViewDistance = fViewDistance;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdEntity::SetMap(sdMap* pkMap)
{
	m_pkMap = pkMap;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE sdMap* sdEntity::GetMap()
{
	return m_pkMap;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdEntity::SetType(EEntityType eType)
{
	m_eType = eType; 
}
//-------------------------------------------------------------------------------------------------
SD_INLINE EEntityType sdEntity::GetType()
{
	return m_eType;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdEntity::SetLoadingStatus(ELoadingState eStatus)
{
	m_eLoadingStatus = eStatus;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE ELoadingState sdEntity::GetLoadingStatus()
{
	return m_eLoadingStatus;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE bool sdEntity::GetShowEdge() const
{
	return m_bEdge;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdEntity::SetEdgeColor(const NiColor& kColor)
{
	m_kEdgeColor = kColor;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE const NiColor& sdEntity::GetEdgeColor() const
{
	return m_kEdgeColor;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE uint sdEntity::GetRenderedFrameId()
{
	return m_uiRenderedFrameId;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdEntity::TickRenderedFrameId()
{
	m_uiRenderedFrameId = ms_uiSystemRenderedFrameId;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE uint sdEntity::GetSystemRenderedFrameId()
{
	return ms_uiSystemRenderedFrameId;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdEntity::TickSystemRenderedFrameId()
{
	++ms_uiSystemRenderedFrameId;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE uint sdEntity::GetSystemVisiableFrameId()
{
	return ms_uiSystemVisiableFrameId;
}
//-------------------------------------------------------------------------------------------------
SD_INLINE void sdEntity::TickSystemVisiableFrameId()
{
	++ms_uiSystemVisiableFrameId;
}
//-------------------------------------------------------------------------------------------------