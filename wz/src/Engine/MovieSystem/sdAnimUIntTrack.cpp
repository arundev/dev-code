#include "sdMovieSystemPCH.h"
#include "sdAnimUIntTrack.h"

ENGINE_NAMESPACE_BEGIN_MOVIESYSTEM
//-------------------------------------------------------------------------------------------------
void sdAnimUIntTrack::GetValue(float fTime, uint& uiValue) 
{
	if (GetNumKeys() == 0)
	{
		uiValue = (uint)-1;
		return;
	}
	else if (GetNumKeys() == 1)
	{
		uiValue = GetKeyTime(0) <= fTime ? m_kTracks[0].m_uiValue : (uint)-1;
		return;
	}
	else
	{
		for (int i = 1; i < GetNumKeys(); ++i)
		{
			if (fTime >= GetKeyTime(i-1) && fTime < GetKeyTime(i))
			{
				uiValue = m_kTracks[i-1].m_uiValue;
				return;
			}
		}

		uiValue = m_kTracks[GetNumKeys() - 1].m_uiValue;
	}
}
//-------------------------------------------------------------------------------------------------
void sdAnimUIntTrack::GetValue(int iIndex, uint& uiValue) 
{
	if (iIndex < 0 || iIndex >= GetNumKeys())
	{
		uiValue = (uint)-1;
		return;
	}
	else
	{
		uiValue = m_kTracks[iIndex].m_uiValue;
		return;
	}
}
//-------------------------------------------------------------------------------------------------
void sdAnimUIntTrack::SetValue(float fTime, uint uiValue) 
{
	sdUIntKey kKey(fTime, IKey::E_KEY_FLAG_CONSTANT, uiValue);
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
void sdAnimUIntTrack::SetValue(int iIndex, uint uiValue) 
{
	if (iIndex > 0 && iIndex < GetNumKeys())
	{
		m_kTracks[iIndex].m_uiValue = uiValue;
	}
}
//-------------------------------------------------------------------------------------------------
bool sdAnimUIntTrack::Load(Engine::Util::sdLuaReadUtil& kLuaStream)
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
			sdUIntKey kKey;
			kLuaStream.ReadData("Time",  kKey.m_fTime);
			kLuaStream.ReadData("Flags", kKey.m_iFlags);
			kLuaStream.ReadData("UInt",  kKey.m_uiValue);

			int iIndex = CreateKey(kKey.m_fTime);
			SetKey(iIndex, &kKey);

			kLuaStream.LoopInnerEnd();
		}
	}
	kLuaStream.EndReadNode();

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdAnimUIntTrack::Save(Engine::Util::sdLuaWriteUtil& kLuaStream)
{
	const int iKeyLen = strlen("                ");

	kLuaStream.WriteNodeBegin("Track");
	kLuaStream.WriteData("TrackType", "ATRACK_UINT", iKeyLen);
	{
		kLuaStream.WriteData("TrackFlag", m_iFlags, iKeyLen);
		kLuaStream.WriteNodeBegin("Value");

		for (int i = 0; i < GetNumKeys(); ++i)
		{
			sdUIntKey kKey;
			GetKey(i, &kKey);

			kLuaStream.LoopInnerBegin();
			kLuaStream.WriteData("Time",  kKey.m_fTime, iKeyLen);
			kLuaStream.WriteData("Flags", kKey.m_iFlags,iKeyLen);
			kLuaStream.WriteData("UInt",  kKey.m_uiValue,iKeyLen);
			kLuaStream.LoopInnerEnd();
		}

		kLuaStream.WriteNodeEnd();
	}
	kLuaStream.WriteNodeEnd();

	return true;
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_MOVIESYSTEM