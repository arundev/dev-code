//-------------------------------------------------------------------------------------------------
NiFixedString sdGameObject::GetNM() const
{
	return GetObjectName();
}
//-------------------------------------------------------------------------------------------------
void sdGameObject::SetNM(const NiFixedString& value)
{
	return SetObjectName(value);
}
//-------------------------------------------------------------------------------------------------
NiFixedString sdGameObject::GetCD() const
{
	struct
	{
		uint code;
		char c;
	} tmp = {GetHashCode(), 0};
	return (char*)(&tmp);
}
//-------------------------------------------------------------------------------------------------
void sdGameObject::SetCD(const NiFixedString& value)
{
	if (value.GetLength() == 4)
	{
		m_uiHashCode = *(const uint*)(const char*)value;
		//NIASSERT(GetCD().Length() == 4);
	}
	else
	{
		m_uiHashCode = 0;
	}
}
//-------------------------------------------------------------------------------------------------
NiFixedString sdGameObject::GetTP() const
{
	return GetClassType();
}
//-------------------------------------------------------------------------------------------------
void sdGameObject::SetTP(const NiFixedString& value)
{
}
//-------------------------------------------------------------------------------------------------
NiFixedString sdGameObject::GetObjectName() const
{
	return GetName();
}
//-------------------------------------------------------------------------------------------------
void sdGameObject::SetObjectName(const NiFixedString& value)
{
	SetName(value);
}
//-------------------------------------------------------------------------------------------------
NiFixedString sdGameObject::GetClassType() const
{
	return GetTypeClass()->className.c_str();
}
//-------------------------------------------------------------------------------------------------
void sdGameObject::SetClassType(const NiFixedString& value)
{
}
//-------------------------------------------------------------------------------------------------
uint sdGameObject::GetHashCode() const
{
	return m_uiHashCode;
}
//-------------------------------------------------------------------------------------------------
void sdGameObject::SetHashCode(const uint& uiHashCode)
{
	m_uiHashCode = uiHashCode;
	bool bExist = ms_kHashCodeGen.AddCode(uiHashCode);
	NIASSERT(bExist);
	//if (uiHashCode)
	//{
	//	NIASSERT(GetCD.GetLength() == 4);
	//}
}
//-------------------------------------------------------------------------------------------------