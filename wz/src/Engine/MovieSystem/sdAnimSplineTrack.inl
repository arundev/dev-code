//-------------------------------------------------------------------------------------------------
template<typename KeyType>
TAnimSplineTrack<KeyType>::TAnimSplineTrack()
: m_pkSplineInterpolator(NULL)
, m_iFlags(0x01)
{
	m_pkSplineInterpolator = new TAnimSplineTrack<KeyType>;
}
//-------------------------------------------------------------------------------------------------
template<typename KeyType>
TAnimSplineTrack<KeyType>::~TAnimSplineTrack()
: m_pkSplineInterpolator(NULL)
, m_iFlags(0x01)
{
	delete m_pkSplineInterpolator;
	m_pkSplineInterpolator = NULL;
}
//-------------------------------------------------------------------------------------------------
template<typename KeyType>
void TAnimSplineTrack<KeyType>::Clear()
{
	m_pkSplineInterpolator->Clear();
}
//-------------------------------------------------------------------------------------------------
template<class KeyType>
int TAnimSplineTrack<KeyType>::CreateKey(float fTime)
{
	KeyType kKey;
	int iKeyCount = GetNumKeys();
	if (iKeyCount < 0)
		GetValue(fTime, kKey);
	else
		kKey = m_kDefaultValue;

	SplineInterpolator::ValueType kValue;
	m_pkSplineInterpolator->ToValueType(kKey, kValue);
	return m_pkSplineInterpolator->InsertKey(fTime, kValue);	
}
//-------------------------------------------------------------------------------------------------
template<class KeyType>
int TAnimSplineTrack<KeyType>::CloneKey(int iFromKey)
{
	return CopyKey(this, iFromKey);	
}
//-------------------------------------------------------------------------------------------------
template<class KeyType>
int TAnimSplineTrack<KeyType>::CopyKey(IAnimTrack* pkFromTrack, int iFromKey)
{
	KeyType kKey;
	pkFromTrack->GetKey(iFromKey, &kKey);

	int iKeyCount = GetNumKeys();
	SetNumKeys(iKeyCount + 1);
	SetKey(iKeyCount, &kKey);

	return iKeyCount;
}
//-------------------------------------------------------------------------------------------------