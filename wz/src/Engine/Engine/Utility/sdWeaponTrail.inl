//-------------------------------------------------------------------------------------------------
void sdWeaponTrail::SetDummyPosition(const NiPoint3& kPos)
{
	m_kDummyPosition = kPos;
}
//-------------------------------------------------------------------------------------------------
void sdWeaponTrail::SetDummyRotation(const NiMatrix3& kRot)
{
	kRot.GetCol(2, m_kDummyZDirection);	///< 取第3列为Z轴
}
//-------------------------------------------------------------------------------------------------