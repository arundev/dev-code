//-------------------------------------------------------------------------------------------------
// sdAnimValue
//-------------------------------------------------------------------------------------------------
void sdAnimValue::Set(const bool& bValue)
{
	NIASSERT(m_eType == E_ANIM_VALUE_BOOL);
	m_bValue = bValue;
}
//-------------------------------------------------------------------------------------------------
void sdAnimValue::Set(const int& iValue)
{
	NIASSERT(m_eType == E_ANIM_VALUE_INT);
	m_iValue = iValue;
}
//-------------------------------------------------------------------------------------------------
void sdAnimValue::Set(const uint& uiValue)
{
	NIASSERT(m_eType == E_ANIM_VALUE_UINT);
	m_uiValue = uiValue;
}
//-------------------------------------------------------------------------------------------------
void sdAnimValue::Set(const float& fValue)
{
	NIASSERT(m_eType == E_ANIM_VALUE_FLOAT);
	m_fValue = fValue;
}
//-------------------------------------------------------------------------------------------------
void sdAnimValue::Set(const NiPoint2& kValue)
{
	NIASSERT(m_eType == E_ANIM_VALUE_NIPOINT2);
	m_kValuePt2[0] = kValue.x;
	m_kValuePt2[1] = kValue.y;
}
//-------------------------------------------------------------------------------------------------
void sdAnimValue::Set(const NiPoint3& kValue)
{
	NIASSERT(m_eType == E_ANIM_VALUE_NIPOINT3);
	m_kValuePt3[0] = kValue.x;
	m_kValuePt3[1] = kValue.y;
	m_kValuePt3[2] = kValue.z;
}
//-------------------------------------------------------------------------------------------------
void sdAnimValue::Set(const NiQuaternion& kValue)
{
	NIASSERT(m_eType == E_ANIM_VALUE_NIQUATERNION);
	m_kValueQuaternion[0] = kValue.m_fX;
	m_kValueQuaternion[1] = kValue.m_fY;
	m_kValueQuaternion[2] = kValue.m_fZ;
	m_kValueQuaternion[3] = kValue.m_fW;
}
//-------------------------------------------------------------------------------------------------
void sdAnimValue::Set(const NiColor& kValue)
{
	NIASSERT(m_eType == E_ANIM_VALUE_NIPOINT3);
	m_kValueColor[0] = kValue.r;
	m_kValueColor[1] = kValue.g;
	m_kValueColor[2] = kValue.b;
}
//-------------------------------------------------------------------------------------------------
void sdAnimValue::Set(const std::string& strValue)
{
	NIASSERT(m_eType == E_ANIM_VALUE_STRING);
	*m_pkValueString = strValue;
}
//-------------------------------------------------------------------------------------------------
void sdAnimValue::Set(const char* szValue)
{
	NIASSERT(m_eType == E_ANIM_VALUE_STRING);
	*m_pkValueString = szValue;
}
//-------------------------------------------------------------------------------------------------
void sdAnimValue::Get(bool& bValue) const
{
	NIASSERT(m_eType == E_ANIM_VALUE_BOOL);
	bValue = m_bValue;
}
//-------------------------------------------------------------------------------------------------
void sdAnimValue::Get(int& iValue) const
{
	NIASSERT(m_eType == E_ANIM_VALUE_INT);
	iValue = m_iValue;
}
//-------------------------------------------------------------------------------------------------
void sdAnimValue::Get(uint& uiValue) const
{
	NIASSERT(m_eType == E_ANIM_VALUE_UINT);
	uiValue = m_uiValue;
}
//-------------------------------------------------------------------------------------------------
void sdAnimValue::Get(float& fValue) const
{
	NIASSERT(m_eType == E_ANIM_VALUE_FLOAT);
	fValue = m_fValue;
}
//-------------------------------------------------------------------------------------------------
void sdAnimValue::Get(NiPoint2& kValue) const
{
	NIASSERT(m_eType == E_ANIM_VALUE_NIPOINT2);
	kValue.x = m_kValuePt2[0];
	kValue.y = m_kValuePt2[1];
}
//-------------------------------------------------------------------------------------------------
void sdAnimValue::Get(NiPoint3& kValue) const
{
	NIASSERT(m_eType == E_ANIM_VALUE_NIPOINT3);
	kValue.x = m_kValuePt3[0];
	kValue.y = m_kValuePt3[1];
	kValue.z = m_kValuePt3[2];
}
//-------------------------------------------------------------------------------------------------
void sdAnimValue::Get(NiQuaternion& kValue) const
{
	NIASSERT(m_eType == E_ANIM_VALUE_NIQUATERNION);
	kValue.m_fX = m_kValueQuaternion[0];
	kValue.m_fY = m_kValueQuaternion[1];
	kValue.m_fZ = m_kValueQuaternion[2];
	kValue.m_fW = m_kValueQuaternion[3];
}
//-------------------------------------------------------------------------------------------------
void sdAnimValue::Get(NiColor& kValue) const
{
	NIASSERT(m_eType == E_ANIM_VALUE_NIPOINT3);
	kValue.r = m_kValueColor[0];
	kValue.g = m_kValueColor[1];
	kValue.b = m_kValueColor[2];
}
//-------------------------------------------------------------------------------------------------
void sdAnimValue::Get(std::string& strValue) const
{
	NIASSERT(m_eType == E_ANIM_VALUE_STRING);
	strValue = *m_pkValueString;
}
//-------------------------------------------------------------------------------------------------
void sdAnimValue::SetIsTrackValue(bool bTrackValue)
{
	m_bTrackBalue = bTrackValue;
}
//-------------------------------------------------------------------------------------------------
bool sdAnimValue::GetIsTrackValue() const
{
	return m_bTrackBalue;
}
//-------------------------------------------------------------------------------------------------
// sdAnimProperty
//-------------------------------------------------------------------------------------------------
const std::string& sdAnimProperty::GetName() const
{
	return m_strName;
}
//-------------------------------------------------------------------------------------------------
const bool sdAnimProperty::GetHerits() const
{
	return m_bInHerits;
}
//-------------------------------------------------------------------------------------------------
const sdAnimValue* sdAnimProperty::GetDefaultValue() const
{
	return m_pkDefaultValue;
}
//-------------------------------------------------------------------------------------------------