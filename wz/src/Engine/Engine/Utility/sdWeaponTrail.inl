//-------------------------------------------------------------------------------------------------
void sdWeaponTrail::SetDummyPosition(const NiPoint3& kPos)
{
	m_kDummyPosition = kPos;
}
//-------------------------------------------------------------------------------------------------
void sdWeaponTrail::SetDummyRotation(const NiMatrix3& kRot)
{
	kRot.GetCol(2, m_kDummyZDirection);	///< ȡ��3��ΪZ��
}
//-------------------------------------------------------------------------------------------------