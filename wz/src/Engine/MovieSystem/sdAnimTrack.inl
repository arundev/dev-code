//-------------------------------------------------------------------------------------------------
template<class KeyType>
int TAnimTrack<KeyType>::CreateKey(float fTime)
{
	KeyType kKey;
	kKey.m_fTime = fTime;

	int iKeyCount = GetNumKeys();
	SetNumKeys(iKeyCount + 1);
	SetKey(iKeyCount, &kKey);
	SortKeys();

	return iKeyCount;		
}
//-------------------------------------------------------------------------------------------------
template<class KeyType>
int TAnimTrack<KeyType>::CloneKey(int iFromKey)
{
	return CopyKey(this, iFromKey);			
}
//-------------------------------------------------------------------------------------------------
template<class KeyType>
int TAnimTrack<KeyType>::CopyKey(IAnimTrack* pkFromTrack, int iFromKey)
{
	KeyType kKey;
	pkFromTrack->GetKey(iFromKey, &kKey);

	int iKeyCount = GetNumKeys();
	SetNumKeys(iKeyCount + 1);
	SetKey(iKeyCount, &kKey);

	return iKeyCount;
}
//-------------------------------------------------------------------------------------------------
template<class KeyType>
void TAnimTrack<KeyType>::RemoveKey(int iIndex)
{
	NIASSERT(iIndex >= 0 && iIndex < (int)m_kTracks.size());
	m_kTracks.erase(m_kTracks.begin() + iIndex);
	Invalidate();
}
//-------------------------------------------------------------------------------------------------
template<class KeyType>
void TAnimTrack<KeyType>::GetKey(int iIndex, IKey* pkKey)
{
	NIASSERT(iIndex >= 0 && iIndex < (int)m_kTracks.size());
	NIASSERT(pkKey);
	*(KeyType*)pkKey = m_kTracks[iIndex];
}
//-------------------------------------------------------------------------------------------------
template<class KeyType>
void TAnimTrack<KeyType>::SetKey(int iIndex, IKey* pkKey)
{
	NIASSERT(iIndex >= 0 && iIndex < (int)m_kTracks.size());
	NIASSERT(pkKey);
	m_kTracks[iIndex] = *(KeyType*)pkKey;
	Invalidate();
}
//-------------------------------------------------------------------------------------------------
template<class KeyType>
float TAnimTrack<KeyType>::GetKeyTime(int iIndex) const
{
	NIASSERT(iIndex >= 0 && iIndex < (int)m_kTracks.size());
	return m_kTracks[iIndex].m_fTime;
}
//-------------------------------------------------------------------------------------------------
template<class KeyType>
void TAnimTrack<KeyType>::SetKeyTime(int iIndex, float fTime)
{
	NIASSERT(iIndex >= 0 && iIndex < (int)m_kTracks.size());
	m_kTracks[iIndex].m_fTime = fTime;
	Invalidate();
}
//-------------------------------------------------------------------------------------------------
template<class KeyType>
int TAnimTrack<KeyType>::FindKey(float fTime)
{
	for (int i = 0; i< (int)m_kTracks.size(); ++i)
	{
		if (m_kTracks[i].m_fTime == fTime)
			return i;
	}

	return -1;
}
//-------------------------------------------------------------------------------------------------
template<class KeyType>
void TAnimTrack<KeyType>::SortKeys()
{
	sort(m_kTracks.begin(), m_kTracks.end());
}
//-------------------------------------------------------------------------------------------------
//typedef TAnimTrack<sdBoolKey> sdBoolTrack;
////-------------------------------------------------------------------------------------------------
//template<>
//TAnimTrack<sdBoolKey>::TAnimTrack()
//{
//
//}
////-------------------------------------------------------------------------------------------------
//template<>
//IAnimTrack::EAnimTrackType TAnimTrack<sdBoolKey>::GetType()
//{
//	return E_ANIM_TRACK_BOOL;
//}
////-------------------------------------------------------------------------------------------------
//template<>
//void TAnimTrack<sdBoolKey>::GetValue(float fTime, bool& bValue) 
//{
//	if (GetNumKeys() == 0)
//	{
//		bValue = false;
//		return;
//	}
//	else if (GetNumKeys() == 1)
//	{
//		bValue = GetKeyTime(0) <= fTime ? m_kTracks[0].m_bValue : false;
//		return;
//	}
//	else
//	{
//		for (int i = 1; i < GetNumKeys(); ++i)
//		{
//			if (fTime >= GetKeyTime(i-1) && fTime < GetKeyTime(i))
//			{
//				bValue = m_kTracks[i-1].m_bValue;
//				return;
//			}
//		}
//
//		bValue = m_kTracks[GetNumKeys() - 1].m_bValue;
//	}
//}
////-------------------------------------------------------------------------------------------------
//template<>
//void TAnimTrack<sdBoolKey>::GetValue(int iIndex, bool& bValue) 
//{
//	if (iIndex < 0 || iIndex >= GetNumKeys())
//	{
//		bValue = false;
//		return;
//	}
//	else
//	{
//		bValue = m_kTracks[iIndex].m_bValue;
//		return;
//	}
//}
////-------------------------------------------------------------------------------------------------
//template<>
//void TAnimTrack<sdBoolKey>::SetValue(float fTime, bool bValue) 
//{
//	sdBoolKey kKey(fTime, IKey::E_KEY_FLAG_CONSTANT, bValue);
//	int iIndex = FindKey(fTime);
//	if (iIndex == -1)
//	{
//		int iKeyNum = GetNumKeys();
//		SetNumKeys(iKeyNum + 1);
//		SetKey(iKeyNum, &kKey);
//		SortKeys();
//	}
//	else
//	{
//		SetKey(iIndex, &kKey);
//	}
//}
////-------------------------------------------------------------------------------------------------
//template<>
//void TAnimTrack<sdBoolKey>::SetValue(int iIndex, bool bValue) 
//{
//	if (iIndex > 0 && iIndex < GetNumKeys())
//	{
//		m_kTracks[iIndex].m_bValue = bValue;
//	}
//}
////-------------------------------------------------------------------------------------------------
//template<>
//bool TAnimTrack<sdBoolKey>::Load(Engine::Util::sdLuaReadUtil& kLuaStream)
//{
//	if (!kLuaStream.ReadData("TrackFlag", m_iFlags))
//	{
//		m_iFlags = IKey::E_KEY_FLAG_CONSTANT;
//	}
//
//	kLuaStream.BeginReadNode("Value");
//	if (kLuaStream.IsNodeValid())
//	{
//		int t = kLuaStream.LoopReset();
//		while (kLuaStream.LoopNext(t))
//		{
//			sdBoolKey kKey;
//			kLuaStream.ReadData("Time",  kKey.m_fTime);
//			kLuaStream.ReadData("Flags", kKey.m_iFlags);
//			kLuaStream.ReadData("Bool",  kKey.m_bValue);
//		}
//	}
//	kLuaStream.EndReadNode();
//
//	return true;
//}
////-------------------------------------------------------------------------------------------------
//template<>
//bool TAnimTrack<sdBoolKey>::Save(Engine::Util::sdLuaWriteUtil& kLuaStream)
//{
//	const int iKeyLen = strlen("                ");
//
//	kLuaStream.WriteNodeBegin("Track");
//	kLuaStream.WriteData("TrackType", "ATRACK_BOOL", iKeyLen);
//	{
//		kLuaStream.WriteData("TrackFlag", m_iFlags, iKeyLen);
//		kLuaStream.WriteNodeBegin("Value");
//
//		for (int i = 0; i < GetNumKeys(); ++i)
//		{
//			sdBoolKey kKey;
//			GetKey(i, &kKey);
//
//			kLuaStream.LoopInnerBegin();
//			kLuaStream.WriteData("Time",  kKey.m_fTime, iKeyLen);
//			kLuaStream.WriteData("Flags", kKey.m_iFlags,iKeyLen);
//			kLuaStream.WriteData("Bool",  kKey.m_bValue,iKeyLen);
//			kLuaStream.LoopInnerEnd();
//		}
//
//		kLuaStream.WriteNodeEnd();
//	}
//	kLuaStream.WriteNodeEnd();
//
//	return true;
//}
////-------------------------------------------------------------------------------------------------