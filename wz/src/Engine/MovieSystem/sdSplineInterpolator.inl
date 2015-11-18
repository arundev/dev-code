//-------------------------------------------------------------------------------------------------
// TBaseSplineInterpolator
//-------------------------------------------------------------------------------------------------
template <class value_type, class spline_type>
void TBaseSplineInterpolator::ToValueType(const value_type& kLocalValue, ValueType &kValue) 
{ 
	*(value_type*)kValue = kLocalValue; 
}
//-------------------------------------------------------------------------------------------------
template <class value_type, class spline_type>
void TBaseSplineInterpolator::FromValueType(ValueType kValue, value_type& kLocalValue) 
{ 
	kLocalValue = *(value_type*)kValue; 
}
//-------------------------------------------------------------------------------------------------
template <class value_type, class spline_type>
ESplineType TBaseSplineInterpolator::GetSplineType() 
{ 
	return E_SPLINE_BEZIER; 
}
//-------------------------------------------------------------------------------------------------
template <class value_type, class spline_type>
int TBaseSplineInterpolator::GetNumDimensions()
{
	assert(sizeof(value_type) % sizeof(ElemType) == 0);
	return DIM;
}
//-------------------------------------------------------------------------------------------------
template <class value_type, class spline_type>
int TBaseSplineInterpolator::InsertKey(float fTime, ValueType kValue)
{
	value_type value;
	FromValueType(kValue, value);
	return spline_type::insert_key(fTime, value);
}
//-------------------------------------------------------------------------------------------------
template <class value_type, class spline_type>
void TBaseSplineInterpolator::RemoveKey(int iKey)
{
	if (iKey >= 0 && iKey < NumKeys())
		spline_type::EraseKey(iKey);
}
//-------------------------------------------------------------------------------------------------
template <class value_type, class spline_type>
void TBaseSplineInterpolator::Clear()
{
	spline_type::Clear();
}
//-------------------------------------------------------------------------------------------------
template <class value_type, class spline_type>
void TBaseSplineInterpolator::FindKeysInRange(float fStartTime, float fEndTime, int& iFirstFoundKey, int& iNumFoundKeys)
{
	spline_type::FindKeysInRange(fStartTime, fEndTime, iFirstFoundKey, iNumFoundKeys);
}
//-------------------------------------------------------------------------------------------------
template <class value_type, class spline_type>
void TBaseSplineInterpolator::RemoveKeysInRange(float fStartTime, float fEndTime)
{
	spline_type::RemoveKeysInRange(fStartTime, fEndTime);
}
//-------------------------------------------------------------------------------------------------
template <class value_type, class spline_type>
int TBaseSplineInterpolator::GetKeyCount() 
{ 
	return spline_type::NumKeys(); 
};
//-------------------------------------------------------------------------------------------------
template <class value_type, class spline_type>
float TBaseSplineInterpolator::GetKeyTime(int iKey)
{
	if (iKey >= 0 && iKey < spline_type::NumKeys())
		return spline_type::GetKey(iKey).m_fTime;
	return 0;
}
//-------------------------------------------------------------------------------------------------
template <class value_type, class spline_type>
bool TBaseSplineInterpolator::GetKeyValue(int iKey,ValueType &kValue)
{
	if (iKey >= 0 && iKey < spline_type::NumKeys())
	{
		ToValueType(spline_type::GetKey(iKey).value, kValue);
		return true;
	}
	return false;
}
//-------------------------------------------------------------------------------------------------
template <class value_type, class spline_type>
void TBaseSplineInterpolator::SetKeyValue(int iKey,ValueType &kValue)
{
	if (iKey >= 0 && iKey < spline_type::NumKeys())
	{
		FromValueType(kValue, spline_type::GetKey(iKey).m_kValue);
		this->SetModified(true);
	}
}
//-------------------------------------------------------------------------------------------------
template <class value_type, class spline_type>
void TBaseSplineInterpolator::SetKeyTime(int iKey, float fTime )
{
	if (iKey >= 0 && iKey < spline_type::NumKeys())
	{
		spline_type::GetKey(iKey).m_fTime = fTime;
		spline_type::SetModified(true, true);
	}
}
//-------------------------------------------------------------------------------------------------
template <class value_type, class spline_type>
void  TBaseSplineInterpolator::SetKeyInTangent(int iKey, ValueType kTin)
{
	if (iKey >= 0 && iKey < spline_type::NumKeys())
	{
		FromValueType(kTin, spline_type::GetKey(iKey).m_kInTangent);
		spline_type::SetModified(true);
	}
}
//-------------------------------------------------------------------------------------------------
template <class value_type, class spline_type>
void  TBaseSplineInterpolator::SetKeyOutTangent(int iKey, ValueType kTout)
{
	if (iKey >= 0 && iKey < spline_type::NumKeys())
	{
		FromValueType(kTout, spline_type::GetKey(iKey).m_kOutTangent);
		spline_type::SetModified(true);
	}
}
//-------------------------------------------------------------------------------------------------
template <class value_type, class spline_type>
void TBaseSplineInterpolator::SetKeyTangents(int iKey, ValueType kTin, ValueType kTout)
{
	if (iKey >= 0 && iKey < spline_type::NumKeys())
	{
		FromValueType(kTin, spline_type::GetKey(iKey).m_kInTangent);
		FromValueType(kTout, spline_type::GetKey(iKey).m_kOutTangent);
		spline_type::SetModified(true);
	}
}
//-------------------------------------------------------------------------------------------------
template <class value_type, class spline_type>
bool TBaseSplineInterpolator::GetKeyTangents(int iKey, ValueType &kTin, ValueType &kTout)
{
	if (iKey >= 0 && iKey < spline_type::NumKeys())
	{
		ToValueType(GetKey(iKey).m_kInTangent, kTin);
		ToValueType(GetKey(iKey).m_kOutTangent, kTout);
		return true;
	}
	return false;
}
//-------------------------------------------------------------------------------------------------
template <class value_type, class spline_type>
void TBaseSplineInterpolator::SetKeyFlags(int iKey, int iFlags)
{
	if (iKey >= 0 && iKey < spline_type::NumKeys())
	{
		GetKey(iKey).m_iFlags = m_iFlags;
		spline_type::SetModified(true);
	}
}
//-------------------------------------------------------------------------------------------------
template <class value_type, class spline_type>
int TBaseSplineInterpolator::GetKeyFlags(int iKey)
{
	if (iKey >= 0 && iKey < spline_type::NumKeys())
	{
		return GetKey(iKey).m_iFlags;
	}
	return 0;
}
//-------------------------------------------------------------------------------------------------
template <class value_type, class spline_type>
void TBaseSplineInterpolator::Interpolate(float fTime, ValueType &kValue)
{
	value_type kLocalValue;
	spline_type::Interpolate(fTime, kLocalValue);
	ToValueType(kLocalValue, kValue);
}
//-------------------------------------------------------------------------------------------------