//-------------------------------------------------------------------------------------------------
// TSpline
//-------------------------------------------------------------------------------------------------
template <typename KeyType,typename BasisType>
void TSpline::AddFlag(int iFlag) 
{ 
	m_uiFlags |= iFlag; 
};
//-------------------------------------------------------------------------------------------------
template <typename KeyType,typename BasisType>
void TSpline::RemoveFlag(int iFlag) 
{ 
	m_uiFlags &= ~iFlag; 
};
//-------------------------------------------------------------------------------------------------
template <typename KeyType,typename BasisType> 
int TSpline::GetFlag(int iFlag)  
{ 
	return m_uiFlags & iFlag; 
};
//-------------------------------------------------------------------------------------------------
template <typename KeyType,typename BasisType> 
void TSpline::SetORT( int ort ) 
{ 
	m_ORT = ort; 
}
//-------------------------------------------------------------------------------------------------
template <typename KeyType,typename BasisType> 
int  TSpline::GetORT() const 
{ 
	return m_ORT; 
}
//-------------------------------------------------------------------------------------------------
template <typename KeyType,typename BasisType> 
bool TSpline::IsORT(int ort) const 
{ 
	return (m_ORT == ort); 
};
//-------------------------------------------------------------------------------------------------
template <typename KeyType,typename BasisType> 
void TSpline::SetRange(float fStart,float fEnd) 
{ 
	m_fRangeStart = fStart; 
	m_fRangeEnd = fEnd; 
};
//-------------------------------------------------------------------------------------------------
template <typename KeyType,typename BasisType> 
float TSpline::GetRangeStart() const 
{ 
	return m_fRangeStart; 
};
//-------------------------------------------------------------------------------------------------
template <typename KeyType,typename BasisType> 
float TSpline::GetRangeEnd() const 
{ 
	return m_fRangeEnd; 
};
//-------------------------------------------------------------------------------------------------
template <typename KeyType,typename BasisType> 
void TSpline::ReserveKeys( int n ) 
{ 
	m_kKeyVec.reserve(n); 
};
//-------------------------------------------------------------------------------------------------
template <typename KeyType,typename BasisType> 
void TSpline::ClearKeys()               
{ 
	m_kKeyVec.clear(); 
};
//-------------------------------------------------------------------------------------------------
template <typename KeyType,typename BasisType> 
void TSpline::ResizeKeys(int num)     
{ 
	m_kKeyVec.resize(num); 
	SetModified(true); 
};
//-------------------------------------------------------------------------------------------------
template <typename KeyType, typename BasisType> 
bool TSpline::EmptyKeys() const         
{ 
	return m_kKeyVec.empty(); 
};
//-------------------------------------------------------------------------------------------------
template <typename KeyType, typename BasisType> 
int TSpline::NumKeys() const       
{ 
	return (int)m_kKeyVec.size(); 
};
//-------------------------------------------------------------------------------------------------
template <typename KeyType,typename BasisType> 
key_type& TSpline::GetKey(int n)     
{ 
	return m_kKeyVec[n]; 
};
//-------------------------------------------------------------------------------------------------
template <typename KeyType, typename BasisType> 
float& TSpline::GetKeyTime(int n) 
{ 
	return m_kKeyVec[n].time; 
};
//-------------------------------------------------------------------------------------------------
template <typename KeyType, typename BasisType> 
value_type&	TSpline::GetKeyValue(int n) 
{ 
	return m_kKeyVec[n].value; 
};
//-------------------------------------------------------------------------------------------------
template <typename KeyType, typename BasisType> 
value_type&	TSpline::GetKeyInTangent(int n)
{ 
	return m_kKeyVec[n].m_kInTangent; 
};
//-------------------------------------------------------------------------------------------------
template <typename KeyType,typename BasisType> 
value_type&	TSpline::GetKeyOutTangent(int n)    
{ 
	return m_kKeyVec[n].m_kOutTangent; 
};
//-------------------------------------------------------------------------------------------------
template <typename KeyType, typename BasisType> 
int& TSpline::GetKeyFlags(int n)        
{ 
	return m_kKeyVec[n].flags; 
};
//-------------------------------------------------------------------------------------------------
template <typename KeyType, typename BasisType> 
key_type& TSpline::GetKey(int n) const    
{ 
	return m_kKeyVec[n]; 
};
//-------------------------------------------------------------------------------------------------
template <typename KeyType, typename BasisType> 
float& TSpline::GetKeyTime(int n) const   
{ 
	return m_kKeyVec[n].time; 
};
//-------------------------------------------------------------------------------------------------
template <typename KeyType, typename BasisType> 
value_type&	TSpline::GetKeyValue(int n) const  
{ 
	return m_kKeyVec[n].value; 
};
//-------------------------------------------------------------------------------------------------
template <typename KeyType, typename BasisType> 
value_type&	TSpline::GetKeyInTangent(int n) const  
{ 
	return m_kKeyVec[n].m_kInTangent; 
};
//-------------------------------------------------------------------------------------------------
template <typename KeyType, typename BasisType> 
value_type&	TSpline::GetKeyOutTangent(int n) const     
{ 
	return m_kKeyVec[n].m_kOutTangent; 
};
//-------------------------------------------------------------------------------------------------
template <typename KeyType, typename BasisType> 
int& TSpline::GetKeyFlags(int n) const    
{ 
	return m_kKeyVec[n].flags; 
};
//-------------------------------------------------------------------------------------------------
template <typename KeyType, typename BasisType> 
int TSpline::GetInTangentType(int n) const  
{ 
	return (GetKeyFlags(n) & SPLINE_KEY_TANGENT_IN_MASK)  >> SPLINE_KEY_TANGENT_IN_SHIFT; 
}
//-------------------------------------------------------------------------------------------------
template <typename KeyType, typename BasisType> 
int TSpline::GetOutTangentType(int n) const 
{ 
	return (GetKeyFlags(n) & SPLINE_KEY_TANGENT_OUT_MASK) >> SPLINE_KEY_TANGENT_OUT_SHIFT; 
}
//-------------------------------------------------------------------------------------------------
template <typename KeyType, typename BasisType> 
void TSpline::EraseKey(int n)	
{ 
	m_kKeyVec.erase(m_kKeyVec.begin() + n); 
	SetModified(true); 
};
//-------------------------------------------------------------------------------------------------
inline void SortKeys()
{
	std::sort( m_kKeyVec.begin(),m_kKeyVec.end() );
	m_uiFlags &= ~E_FLAG_MUST_SORT;
}
//-------------------------------------------------------------------------------------------------
template <typename KeyType, typename BasisType> 
void TSpline::PushBackKey(const key_type &kKey)
{
	m_kKeyVec.push_back(kKey);
	SetModified(true);
};
//-------------------------------------------------------------------------------------------------
template <typename KeyType, typename BasisType> 
int TSpline::InsertKey(const key_type &kKey)
{
	int num = NumKeys();
	for (int i = 0; i < num; i++)
	{
		if (m_kKeyVec[i].time > kKey.time)
		{
			m_kKeyVec.insert( m_kKeyVec.begin()+i, kKey);
			SetModified(true);
			return i;
		}
	}

	m_kKeyVec.push_back(kKey);
	SetModified(true);

	return NumKeys() -1 ;
};
//-------------------------------------------------------------------------------------------------
template <typename KeyType, typename BasisType> 
int TSpline::InsertKey(float fTime, value_type const& kValue)
{
	key_type kKey;
	kKey.time = fTime;
	kKey.value = kValue;
	kKey.flags = 0;
	Zero(kKey.m_kInTangent);
	Zero(kKey.m_kOutTangent);

	return insert_key(kKey);
}
//-------------------------------------------------------------------------------------------------
template <typename KeyType, typename BasisType> 
void TSpline::find_keys_in_range(float fStartTime, float fEndTime, int& iFirstFoundKey, int& iNumFoundKeys) const
{
	int iCount = NumKeys();
	int iStart = 0;
	int iEnd = iCount;
	for (int i = 0; i < iCount; ++i)
	{
		float keyTime = m_kKeyVec[i].time;
		if (keyTime < fStartTime)
			iStart = i + 1;
		if (keyTime > fEndTime && iEnd > i)
			iEnd = i;
	}
	if (iStart < iEnd)
	{
		iFirstFoundKey = iStart;
		iNumFoundKeys = iEnd - iStart;
	}
	else
	{
		iFirstFoundKey = -1;
		iNumFoundKeys = 0;
	}
}
//-------------------------------------------------------------------------------------------------
template <typename KeyType, typename BasisType> 
void TSpline::RemoveKeysInRange(float fStartTime, float fEndTime)
{
	int iFirstFoundKey, iNumFoundKeys;
	find_keys_in_range(fStartTime, fEndTime, iFirstFoundKey, iNumFoundKeys);
	if (iNumFoundKeys)
	{
		m_kKeyVec.erase(m_kKeyVec.begin() + iFirstFoundKey, m_kKeyVec.begin() + iFirstFoundKey + iNumFoundKeys);
		SetModified(true);
	}
}
//-------------------------------------------------------------------------------------------------
template <typename KeyType, typename BasisType> 
void TSpline::SetModified(bool bOn, bool bSort /* = false */)
{
	if (bOn) 
		m_uiFlags |= E_FLAG_MODIFIED; 
	else 
		m_uiFlags &= ~(E_FLAG_MODIFIED);

	if (bSort)
		m_uiFlags |= E_FLAG_MUST_SORT;

	m_curr = 0;
}
//-------------------------------------------------------------------------------------------------
template <typename KeyType, typename BasisType> 
void TSpline::Update()
{
	if (m_uiFlags & E_FLAG_MODIFIED)
	{
		SortKeys();

		if (m_uiFlags & E_FLAG_MODIFIED)	
			Preompute();
	}
}
//-------------------------------------------------------------------------------------------------
template <typename KeyType, typename BasisType> 
bool TSpline::IsUpdated() const
{
	return (m_uiFlags & E_FLAG_MODIFIED) == 0;
}
//-------------------------------------------------------------------------------------------------
template <typename KeyType, typename BasisType> 
bool TSpline::Interpolate(float fTime, value_type& kValue)
{
	Update();

	if (EmptyKeys()) 
		return false;

	if (fTime < GetKeyTime(0))
	{
		kValue = GetKeyValue(0);
		return true;
	}

	AdjustTime(fTime);

	int iNum = NumKeys();
	int iCurIndex = SeekKey(fTime);
	if (iCurIndex < iNum-1)
	{
		assert(fTime >= GetKeyTime(iCurIndex));

		float u = (fTime - GetKeyTime(iCurIndex)) / (GetKeyTime(iCurIndex+1) - GetKeyTime(iCurIndex));
		InterpolateKeys(iCurIndex, iCurIndex+1, u, kValue);
	}	
	else
	{
		kValue = GetKeyValue(iNum-1);
	}
	return true;
}
//-------------------------------------------------------------------------------------------------
template <typename KeyType, typename BasisType> 
int TSpline::SeekKey(float fTime)
{
	NIASSERT(NumKeys() < (1<<15));

	int iNumKeys = NumKeys();
	if (m_curr >= iNumKeys || (GetKeyTime(m_curr) > fTime))
		m_curr = 0;		///< Search from begining.

	while ((m_curr < iNumKeys - 1) && (GetKeyTime(m_curr + 1) <= fTime)) 
		++m_curr;

	return m_curr;
}
//-------------------------------------------------------------------------------------------------
template <typename KeyType, typename BasisType> 
void TSpline::AdjustTime(float &fTime)
{
	if (IsORT(E_ORT_CYCLE) || IsORT(E_ORT_LOOP))
	{
		if (!EmptyKeys())
		{
			float fEndTime = GetKeyTime(NumKeys()-1);
			if (fTime > fEndTime)
			{
				fTime = fmod(fTime, fEndTime);	///< Warp time.
			}
		}
	}
}
//-------------------------------------------------------------------------------------------------
// TBezierSpline
//-------------------------------------------------------------------------------------------------
template <class T, class Key = TSplineKey<T> >
void TBezierSpline::Preompute()
{
	SetModified(true);

	int iNum = NumKeys();
	if (iNum > 1)
	{
		const float fOneThird = 1 / 3.0f;
		const int iLast = iNum - 1;

		// 初始化两侧切线
		if (GetInTangentType(0) != E_SPLINE_KEY_TANGENT_CUSTOM)
			Zero(GetKey(0).m_kInTangent);
		if (GetOutTangentType(0) != E_SPLINE_KEY_TANGENT_CUSTOM)
			GetKey(0).m_kOutTangent = fOneThird * (GetKeyValue(1) - GetKeyValue(0));

		if (GetInTangentType(iLast) != E_SPLINE_KEY_TANGENT_CUSTOM)
			GetKey(iLast).m_kInTangent = fOneThird * (GetKeyValue(iLast) - GetKeyValue(iLast-1));
		if (GetOutTangentType(iLast) != E_SPLINE_KEY_TANGENT_CUSTOM)
			Zero(GetKey(iLast).m_kOutTangent);

		for (int i = 1; i < iLast; ++i)
		{
			key_type& kKey = GetKey(i);
			T ds0 = kKey.m_kInTangent;
			T dd0 = kKey.m_kOutTangent;

			const float fDeltaTime = GetKeyTime(i+1) - GetKeyTime(i-1);
			if(fDeltaTime <= 0)
			{
				Zero(kKey.m_kInTangent);
				Zero(kKey.m_kOutTangent);
			}
			else
			{
				const float k = (GetKeyTime(i) - GetKeyTime(i-1)) / fDeltaTime;
				const value_type fDeltaValue = GetKeyValue(i+1) - GetKeyValue(i-1);
				kKey.m_kInTangent = fOneThird * fDeltaValue * k;
				kKey.m_kOutTangent = fOneThird * fDeltaValue * (1-k);
			}

			switch (this->GetInTangentType(i))
			{
			case E_SPLINE_KEY_TANGENT_STEP:
				Zero(kKey.m_kInTangent);
				break;
			case E_SPLINE_KEY_TANGENT_ZERO:
				Zero(kKey.m_kInTangent);
				break;
			case E_SPLINE_KEY_TANGENT_LINEAR:
				kKey.m_kInTangent = fOneThird * (GetKeyValue(i) - GetKeyValue(i-1));
				break;
			case E_SPLINE_KEY_TANGENT_CUSTOM:
				kKey.m_kInTangent = ds0;
				break;
			}

			switch (this->GetOutTangentType(i))
			{
			case E_SPLINE_KEY_TANGENT_STEP:
				Zero(kKey.m_kOutTangent);
				break;
			case E_SPLINE_KEY_TANGENT_ZERO:
				Zero(kKey.m_kOutTangent);
				break;
			case E_SPLINE_KEY_TANGENT_LINEAR:
				kKey.m_kOutTangent = fOneThird * (GetKeyValue(i+1) - GetKeyValue(i));
				break;
			case E_SPLINE_KEY_TANGENT_CUSTOM:
				kKey.m_kOutTangent = dd0;
				break;
			}
		}
	}
}
//-------------------------------------------------------------------------------------------------
template <class T, class Key = TSplineKey<T> >
void TBezierSpline::InterpolateKeys(int iKeyFrom, int iKeyTo, float u, value_type& kValue)
{
	if (GetOutTangentType(iKeyFrom) == E_SPLINE_KEY_TANGENT_STEP)
	{
		kValue = GetKeyValue(iKeyTo);
	}
	else if (GetInTangentType(iKeyTo) == E_SPLINE_KEY_TANGENT_STEP)
	{
		kValue = GetKeyValue(iKeyFrom);
	}
	else
	{
		typename TSpline<Key, sdBezierBasis>::basis_type basis(u);

		const T p0 = GetKeyValue(iKeyFrom);
		const T p3 = GetKeyValue(iKeyTo);
		const T p1 = p0 + GetKeyOutTangent(iKeyFrom);
		const T p2 = p3 - GetKeyInTangent(iKeyTo);

		kValue = (basis[0] * p0) + (basis[1] * p1) + (basis[2] * p2) + (basis[3] * p3);
	}
}
//-------------------------------------------------------------------------------------------------