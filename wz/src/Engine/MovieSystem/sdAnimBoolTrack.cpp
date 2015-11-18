#include "sdMovieSystemPCH.h"
#include "sdAnimBoolTrack.h"

ENGINE_NAMESPACE_BEGIN_MOVIESYSTEM
//-------------------------------------------------------------------------------------------------
void sdAnimBoolTrack::GetValue(float fTime, bool& bValue) 
{
	if (GetNumKeys() == 0)
	{
		bValue = false;
		return;
	}
	else if (GetNumKeys() == 1)
	{
		bValue = GetKeyTime(0) <= fTime ? m_kTracks[0].m_bValue : false;
		return;
	}
	else
	{
		for (int i = 1; i < GetNumKeys(); ++i)
		{
			if (fTime >= GetKeyTime(i-1) && fTime < GetKeyTime(i))
			{
				bValue = m_kTracks[i-1].m_bValue;
				return;
			}
		}

		bValue = m_kTracks[GetNumKeys() - 1].m_bValue;
	}
}
//-------------------------------------------------------------------------------------------------
void sdAnimBoolTrack::GetValue(int iIndex, bool& bValue) 
{
	if (iIndex < 0 || iIndex >= GetNumKeys())
	{
		bValue = false;
		return;
	}
	else
	{
		bValue = m_kTracks[iIndex].m_bValue;
		return;
	}
}
//-------------------------------------------------------------------------------------------------
void sdAnimBoolTrack::SetValue(float fTime, bool bValue) 
{
	sdBoolKey kKey(fTime, IKey::E_KEY_FLAG_CONSTANT, bValue);
	int iIndex = FindKey(fTime);
	if (iIndex == -1)
	{
		int iKeyNum = GetNumKeys();
		SetNumKeys(iKeyNum + 1);
		SetKey(iKeyNum, &kKey);
		SortKeys();
	}
	else
	{
		SetKey(iIndex, &kKey);
	}
}
//-------------------------------------------------------------------------------------------------
void sdAnimBoolTrack::SetValue(int iIndex, bool bValue) 
{
	if (iIndex > 0 && iIndex < GetNumKeys())
	{
		m_kTracks[iIndex].m_bValue = bValue;
	}
}
//-------------------------------------------------------------------------------------------------
bool sdAnimBoolTrack::Load(Engine::Util::sdLuaReadUtil& kLuaStream)
{
	if (!kLuaStream.ReadData("TrackFlag", m_iFlags))
	{
		m_iFlags = IKey::E_KEY_FLAG_CONSTANT;
	}

	kLuaStream.BeginReadNode("Value");
	if (kLuaStream.IsNodeValid())
	{
		int t = kLuaStream.LoopReset();
		while (kLuaStream.LoopNext(t))
		{
			sdBoolKey kKey;
			kLuaStream.ReadData("Time",  kKey.m_fTime);
			kLuaStream.ReadData("Flags", kKey.m_iFlags);
			kLuaStream.ReadData("Bool",  kKey.m_bValue);
		}
	}
	kLuaStream.EndReadNode();

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdAnimBoolTrack::Save(Engine::Util::sdLuaWriteUtil& kLuaStream)
{
	const int iKeyLen = strlen("                ");

	kLuaStream.WriteNodeBegin("Track");
	kLuaStream.WriteData("TrackType", "ATRACK_BOOL", iKeyLen);
	{
		kLuaStream.WriteData("TrackFlag", m_iFlags, iKeyLen);
		kLuaStream.WriteNodeBegin("Value");

		for (int i = 0; i < GetNumKeys(); ++i)
		{
			sdBoolKey kKey;
			GetKey(i, &kKey);

			kLuaStream.LoopInnerBegin();
			kLuaStream.WriteData("Time",  kKey.m_fTime, iKeyLen);
			kLuaStream.WriteData("Flags", kKey.m_iFlags,iKeyLen);
			kLuaStream.WriteData("Bool",  kKey.m_bValue,iKeyLen);

			int iIndex = CreateKey(kKey.m_fTime);
			SetKey(iIndex, &kKey);

			kLuaStream.LoopInnerEnd();
		}

		kLuaStream.WriteNodeEnd();
	}
	kLuaStream.WriteNodeEnd();

	return true;
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_MOVIESYSTEM