#include "sdMovieSystemPCH.h"
#include "sdAnimFloatTrack.h"

ENGINE_NAMESPACE_BEGIN_MOVIESYSTEM
//-------------------------------------------------------------------------------------------------
void sdAnimFloatTrack::GetValue(float fTime, float& fValue) 
{
	if (GetNumKeys() == 0)
	{
		fValue = -FLT_MAX;
		return;
	}
	else if (GetNumKeys() == 1)
	{
		fValue = GetKeyTime(0) <= fTime ? m_kTracks[0].m_fValue : -FLT_MAX;
		return;
	}
	else
	{
		for (int i = 1; i < GetNumKeys(); ++i)
		{
			if (fTime >= GetKeyTime(i-1) && fTime < GetKeyTime(i))
			{
				fValue = m_kTracks[i-1].m_fValue;
				return;
			}
		}

		fValue = m_kTracks[GetNumKeys() - 1].m_fValue;
	}
}
//-------------------------------------------------------------------------------------------------
void sdAnimFloatTrack::GetValue(int iIndex, float& fValue) 
{
	if (iIndex < 0 || iIndex >= GetNumKeys())
	{
		fValue = -FLT_MAX;
		return;
	}
	else
	{
		fValue = m_kTracks[iIndex].m_fValue;
		return;
	}
}
//-------------------------------------------------------------------------------------------------
void sdAnimFloatTrack::SetValue(float fTime, float fValue) 
{
	sdFloatKey kKey(fTime, IKey::E_KEY_FLAG_CONSTANT, fValue);
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
void sdAnimFloatTrack::SetValue(int iIndex, float fValue) 
{
	if (iIndex > 0 && iIndex < GetNumKeys())
	{
		m_kTracks[iIndex].m_fValue = fValue;
	}
}
//-------------------------------------------------------------------------------------------------
bool sdAnimFloatTrack::Load(Engine::Util::sdLuaReadUtil& kLuaStream)
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
			sdFloatKey kKey;
			kLuaStream.ReadData("Time",  kKey.m_fTime);
			kLuaStream.ReadData("Flags", kKey.m_iFlags);
			kLuaStream.ReadData("Float",  kKey.m_fValue);

			int iIndex = CreateKey(kKey.m_fTime);
			SetKey(iIndex, &kKey);

			kLuaStream.LoopInnerEnd();
		}
	}
	kLuaStream.EndReadNode();

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdAnimFloatTrack::Save(Engine::Util::sdLuaWriteUtil& kLuaStream)
{
	const int iKeyLen = strlen("                ");

	kLuaStream.WriteNodeBegin("Track");
	kLuaStream.WriteData("TrackType", "ATRACK_UINT", iKeyLen);
	{
		kLuaStream.WriteData("TrackFlag", m_iFlags, iKeyLen);
		kLuaStream.WriteNodeBegin("Value");

		for (int i = 0; i < GetNumKeys(); ++i)
		{
			sdFloatKey kKey;
			GetKey(i, &kKey);

			kLuaStream.LoopInnerBegin();
			kLuaStream.WriteData("Time",  kKey.m_fTime, iKeyLen);
			kLuaStream.WriteData("Flags", kKey.m_iFlags,iKeyLen);
			kLuaStream.WriteData("Float", kKey.m_fValue,iKeyLen);
			kLuaStream.LoopInnerEnd();
		}

		kLuaStream.WriteNodeEnd();
	}
	kLuaStream.WriteNodeEnd();

	return true;
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_MOVIESYSTEM