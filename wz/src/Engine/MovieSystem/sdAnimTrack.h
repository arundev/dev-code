//*************************************************************************************************
// 内容:	AnimKey与AnimTrack
//---------------------------------------------------------
// 作者:		
// 创建:		2013-01-05
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _MOVIE_SYSTEM_ANIM_TRACK_H__
#define _MOVIE_SYSTEM_ANIM_TRACK_H__

#include <IAnimTrack.h>
#include <sdLuaFile.h>

ENGINE_NAMESPACE_BEGIN_MOVIESYSTEM
//-----------------------------------------------------------------------------
class sdBoolKey : public IKey
{
public:
	bool m_bValue;

public:
	sdBoolKey() : m_bValue(false) {}
	sdBoolKey(float fTime, int iFlag, bool bValue) : IKey(fTime, iFlag), m_bValue(bValue) {} 
};
//-----------------------------------------------------------------------------
class sdUIntKey : public IKey
{
public:
	uint m_uiValue;

public:
	sdUIntKey() : m_uiValue(0) {}
	sdUIntKey(float fTime, int iFlag, uint uiValue) : IKey(fTime, iFlag), m_uiValue(uiValue) {} 
};
//-----------------------------------------------------------------------------
class sdFloatKey : public IKey
{
public:
	float m_fValue;

public:
	sdFloatKey() : m_fValue(0.f) {}
	sdFloatKey(float fTime, int iFlag, float fValue) : IKey(fTime, iFlag), m_fValue(fValue) {} 
};
//-----------------------------------------------------------------------------
//class sdPosKey : public IKey
//{
//public:
//	NiPoint3 m_kValue;
//
//public:
//	sdPosKey(){}
//};
//-----------------------------------------------------------------------------
//class sdRotKey : public IKey
//{
//public:
//	NiQuaternion m_kValue;
//
//public:
//	sdRotKey(){}
//};
//-----------------------------------------------------------------------------
//class sdColorKey : public IKey
//{
//public:
//	NiColor m_kValue;
//
//public:
//	sdColorKey(){}
//};
//-----------------------------------------------------------------------------
class sdStringKey : public IKey
{
public:
	std::string m_kValue;

public:
	sdStringKey(){}
};

//-----------------------------------------------------------------------------
// General templated track for event type keys.
// KeyType class must be derived from IKey
//-----------------------------------------------------------------------------
template<typename KeyType>
class TAnimTrack : public IAnimTrack
{
public:
	TAnimTrack() : m_iCurrentIndex(0), m_iFlags(0x01), m_bModified(false){}
	virtual ~TAnimTrack(){}

	// 虚函数继承
	virtual inline EAnimTrackType GetType(){ return E_ANIM_TRACK_DEFAULT;}
	
	virtual bool Load(Engine::Util::sdLuaReadUtil& kLuaStream){	return true; }
	virtual bool Save(Engine::Util::sdLuaWriteUtil& kLuaStream){ return true; }
	virtual void Clear(){ m_kTracks.clear();}
	
	virtual inline int CreateKey(float fTime);
	virtual inline int CloneKey(int iFromKey);
	virtual inline int CopyKey(IAnimTrack* pkFromTrack, int iFromKey);

	virtual inline void RemoveKey(int iIndex);

	virtual inline void GetKey(int iIndex, IKey* pkKey);
	virtual inline void SetKey(int iIndex, IKey* pkKey);

	virtual inline float GetKeyTime(int iIndex) const;
	virtual inline void  SetKeyTime(int iIndex, float fTime);

	virtual inline int FindKey(float fTime);

	virtual inline void SortKeys();

	virtual inline int GetNumKeys() const { return m_kTracks.size();}
	virtual inline void SetNumKeys(int iSize) { m_kTracks.resize(iSize);}

	virtual void GetValue(float fTime, bool& bValue) { };
	virtual void GetValue(float fTime, uint& uiValue) { };
	virtual void GetValue(float fTime, float& fValue) { };
	virtual void GetValue(float fTime, NiPoint2& kValue) { };
	virtual void GetValue(float fTime, NiPoint3& kValue) { };
	virtual void GetValue(float fTime, NiQuaternion& kValue) { };
	virtual void GetValue(float fTime, NiColor& kValue) { };
	virtual void GetValue(float fTime, std::string& strValue) { };

	virtual void GetValue(int iIndex, bool& bValue) { };
	virtual void GetValue(int iIndex, uint& uiValue) { };
	virtual void GetValue(int iIndex, float& fValue) { };
	virtual void GetValue(int iIndex, NiPoint2& kValue) { };
	virtual void GetValue(int iIndex, NiPoint3& kValue) { };
	virtual void GetValue(int iIndex, NiQuaternion& kValue) { };
	virtual void GetValue(int iIndex, NiColor& kValue) { };
	virtual void GetValue(int iIndex, std::string& strValue) { };

	virtual void SetValue(float fTime, bool bValue) { };
	virtual void SetValue(float fTime, uint uiValue) { };
	virtual void SetValue(float fTime, float fValue) { };
	virtual void SetValue(float fTime, const NiPoint2& kValue) { };
	virtual void SetValue(float fTime, const NiPoint3& kValue) { };
	virtual void SetValue(float fTime, const NiQuaternion& kValue) { };
	virtual void SetValue(float fTime, const NiColor& kValue) { };
	virtual void SetValue(float fTime, const std::string& strValue) { };

	virtual void SetValue(int iIndex, bool bValue) { };
	virtual void SetValue(int iIndex, uint uiValue) { };
	virtual void SetValue(int iIndex, float fValue) { };
	virtual void SetValue(int iIndex, const NiPoint2& kValue) { };
	virtual void SetValue(int iIndex, const NiPoint3& kValue) { };
	virtual void SetValue(int iIndex, const NiQuaternion& kValue) { };
	virtual void SetValue(int iIndex, const NiColor& kValue) { };
	virtual void SetValue(int iIndex, const std::string& strValue) { };
	
protected:
	void Invalidate() { m_bModified = true;};

protected:
	//// AnimTrack内部标记
	//enum EAnimTrackFlag
	//{
	//	E_ANIM_TRACK_FLAG_CONSTANT	= 0x0001,
	//	E_ANIM_TRACK_FLAG_CYCLE		= 0x0002,
	//	E_ANIM_TRACK_FLAG_LOOP		= 0x0003,
	//	E_ANIM_TRACK_FLAG_LINEAR	= 0x0005,
	//	E_ANIM_TRACK_FLAG_RELATIVE_REPEAT = 0x0007,
	//};

protected:
	std::vector<KeyType> m_kTracks;
	int m_iCurrentIndex;
	int m_iFlags;
	bool m_bModified;
};
//-----------------------------------------------------------------------------
#include "sdAnimTrack.inl"
ENGINE_NAMESPACE_END_MOVIESYSTEM
#endif