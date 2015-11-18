//*************************************************************************************************
// ����:	
//---------------------------------------------------------
// ����:		
// ����:		2013-01-05
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_MOVIE_SYSTEM_ANIMKEY_H__
#define _SD_ENGINE_MOVIE_SYSTEM_ANIMKEY_H__

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_UTIL
class sdLuaReadUtil;
class sdLuaWriteUtil;
ENGINE_NAMESPACE_END_UTIL
ENGINE_NAMESPACE_END_ENGINE

ENGINE_NAMESPACE_BEGIN_MOVIESYSTEM
//-----------------------------------------------------------------------------
// Interface to animation key
//-----------------------------------------------------------------------------
class IKey : public NiMemObject
{
public:
	// ���
	enum EKeyFlag
	{
		E_KEY_FLAG_NONE		= 0x00000000,
		E_KEY_FLAG_CONSTANT	= 0x00000001,
		E_KEY_FLAG_CYCLE	= 0x00000002,
		E_KEY_FLAG_LOOP		= 0x00000003,
		E_KEY_FLAG_LINEAR	= 0x00000005,
	};

public:
	float	m_fTime;
	int		m_iFlags;

public:
	IKey() : m_fTime(0.f), m_iFlags(0){}
	IKey(float fTime, int iFlag) : m_fTime(fTime), m_iFlags(iFlag){}

	// compare keys.
	bool operator<(const IKey &key) const { return m_fTime < key.m_fTime; }
	bool operator==(const IKey &key) const { return m_fTime == key.m_fTime; }
	bool operator>(const IKey &key) const { return m_fTime > key.m_fTime; }
	bool operator<=(const IKey &key) const { return m_fTime <= key.m_fTime; }
	bool operator>=(const IKey &key) const { return m_fTime >= key.m_fTime; }
	bool operator!=(const IKey &key) const { return m_fTime != key.m_fTime; }
};
//-----------------------------------------------------------------------------
// AnimTrack����
//-----------------------------------------------------------------------------
enum EAnimTrackType
{
	E_ANIM_TRACK_DEFAULT = 0,
	E_ANIM_TRACK_BOOL,
	E_ANIM_TRACK_UINT,
	E_ANIM_TRACK_FLOAT,
	E_ANIM_TRACK_NIPOINT2,
	E_ANIM_TRACK_NIPOINT3,
	E_ANIM_TRACK_QUATERNION,
	E_ANIM_TRACK_COLOR,
	E_ANIM_TRACK_STRING,
};
//-----------------------------------------------------------------------------
// Interface of Animation Track
// Track��һ��Key�ļ���
//-----------------------------------------------------------------------------
class IAnimTrack : public NiRefObject
{
public:
	virtual ~IAnimTrack() {};

	// ��ȡ����
	virtual EAnimTrackType GetType() = 0;

	// ����/����/����
	virtual bool Load(Engine::Util::sdLuaReadUtil& kLuaStream) = 0;
	virtual bool Save(Engine::Util::sdLuaWriteUtil& kLuaStream) = 0;
	virtual void Clear() = 0;
	
	// Key����
	virtual int CreateKey(float fTime) = 0;
	virtual int CloneKey(int iKey) = 0;
	virtual int CopyKey(IAnimTrack* pkFromTrack, int iFromKey) = 0;

	virtual void RemoveKey(int iIndex) = 0;
	
	virtual void GetKey(int iIndex, IKey* pkKey) = 0;
	virtual void SetKey(int iIndex, IKey* pkKey) = 0;
	
	virtual float GetKeyTime(int iIndex) const = 0;
	virtual void  SetKeyTime(int iIndex, float fTime) = 0;
	
	virtual int FindKey(float fTime) = 0;
	
	virtual void SortKeys() = 0;
	
	virtual int	 GetNumKeys() const = 0;
	virtual void SetNumKeys(int iKey) = 0;
	
	// Key-Value����
	virtual void GetValue(float fTime, bool& bValue) = 0;
	virtual void GetValue(float fTime, uint& uiValue) = 0;
	virtual void GetValue(float fTime, float& fValue) = 0;
	virtual void GetValue(float fTime, NiPoint2& kValue) = 0;
	virtual void GetValue(float fTime, NiPoint3& kValue) = 0;
	virtual void GetValue(float fTime, NiQuaternion& kValue) = 0;
	virtual void GetValue(float fTime, NiColor& kValue) = 0;
	virtual void GetValue(float fTime, std::string& strValue) = 0;
	
	virtual void GetValue(int iIndex, bool& bValue) = 0;
	virtual void GetValue(int iIndex, uint& uiValue) = 0;
	virtual void GetValue(int iIndex, float& fValue) = 0;
	virtual void GetValue(int iIndex, NiPoint2& kValue) = 0;
	virtual void GetValue(int iIndex, NiPoint3& kValue) = 0;
	virtual void GetValue(int iIndex, NiQuaternion& kValue) = 0;
	virtual void GetValue(int iIndex, NiColor& kValue) = 0;
	virtual void GetValue(int iIndex, std::string& strValue) = 0;
	
	virtual void SetValue(float fTime, bool bValue) = 0;
	virtual void SetValue(float fTime, uint uiValue) = 0;
	virtual void SetValue(float fTime, float fValue) = 0;
	virtual void SetValue(float fTime, const NiPoint2& kValue) = 0;
	virtual void SetValue(float fTime, const NiPoint3& kValue) = 0;
	virtual void SetValue(float fTime, const NiQuaternion& kValue) = 0;
	virtual void SetValue(float fTime, const NiColor& kValue) = 0;
	virtual void SetValue(float fTime, const std::string& strValue) = 0;
	
	virtual void SetValue(int iIndex, bool bValue) = 0;
	virtual void SetValue(int iIndex, uint uiValue) = 0;
	virtual void SetValue(int iIndex, float fValue) = 0;
	virtual void SetValue(int iIndex, const NiPoint2& kValue) = 0;
	virtual void SetValue(int iIndex, const NiPoint3& kValue) = 0;
	virtual void SetValue(int iIndex, const NiQuaternion& kValue) = 0;
	virtual void SetValue(int iIndex, const NiColor& kValue) = 0;
	virtual void SetValue(int iIndex, const std::string& strValue) = 0;
};
NiSmartPointer(IAnimTrack);
//-----------------------------------------------------------------------------
// AnimAffector����
//-----------------------------------------------------------------------------
enum EAnimAffectorType
{
	E_ANIM_AFFECTOR_DEFAULT,
	E_ANIM_AFFECTOR_POSITION,
	E_ANIM_AFFECTOR_ROTATION,
	E_ANIM_AFFECTOR_SCALE,
	E_ANIM_AFFECTOR_VISIBLE,
	E_ANIM_AFFECTOR_ACTION,
	E_ANIM_AFFECTOR_PICK,
	E_ANIM_AFFECTOR_SUBTITLE,
	E_ANIM_AFFECTOR_CAMERA,
	E_ANIM_AFFECTOR_FADE,
	E_ANIM_AFFECTOR_ENABLE,
	E_ANIM_AFFECTOR_DOF,
	E_ANIM_AFFECTOR_CLONE,
	E_ANIM_AFFECTOR_OFFSET,
	E_ANIM_AFFECTOR_LOOKAT,
	E_ANIM_AFFECTOR_ALPHA,
	E_ANIM_AFFECTOR_ATTACH,
	E_ANIM_AFFECTOR_BLENDACTION,
	E_ANIM_AFFECTOR_FLYSPEED,
	E_ANIM_AFFECTOR_PLAYSPEED,
	E_ANIM_AFFECTOR_COLOR,
	E_ANIM_AFFECTOR_SPEFFECT,
	E_ANIM_AFFECTOR_SPEFFECT_GHOST,
	E_ANIM_AFFECTOR_DECAL_SCALE,
	E_ANIM_AFFECTOR_DECAL_ROTATION,
	E_ANIM_AFFECTOR_VOLUME,

	NUM_ANIM_AFFECTOR,
};
//-----------------------------------------------------------------------------
// AnimAffectorָ��һ��AnimTrack
//-----------------------------------------------------------------------------
class IAnimAffectorControl;
class IAnimNode;
class IAnimAffector : public NiRefObject
{
public:
	virtual ~IAnimAffector() {};

	// ��ȡ����
	virtual EAnimAffectorType GetType() = 0;

	// ����/����/����
	virtual bool Load(Engine::Util::sdLuaReadUtil& kLuaStream) = 0;
	virtual bool Save(Engine::Util::sdLuaWriteUtil& kLuaStream) = 0;
	virtual void Clear() = 0;

	// ����
	virtual void UpdateFrame(float fAccumTime, float fFrameTime, IAnimNode* pkAnimNode) = 0;

	// �ڲ�ͬ����������(?)
	//virtual void UpdateFrame(float fAccumTime, float fFrameTime, NiPoint3& kPos) = 0;

	// ����
	virtual void Reset() = 0;

	// ��ӵ�е�AnimTrack
	virtual void SetAnimTrack(IAnimTrack* pkAnimTrack) = 0;
	virtual IAnimTrack* GetAnimTrack() = 0;
	
	// ��ӵ�е�AnimTrackControl
	virtual IAnimAffectorControl* GetAnimTrackControl() = 0;

};
NiSmartPointer(IAnimAffector);
//-----------------------------------------------------------------------------
// AnimAffectorControl��һ��AnimAffector�ļ���, ÿ��AnimAffectorֻ����һ��
//-----------------------------------------------------------------------------
class IAnimAffectorControl : public NiRefObject
{
public:
	virtual ~IAnimAffectorControl() {};

	// ����/����/����
	virtual bool Load(Engine::Util::sdLuaReadUtil& kLuaStream) = 0;
	virtual bool Save(Engine::Util::sdLuaWriteUtil& kLuaStream) = 0;
	virtual void Clear() = 0;

	// Affector����
	virtual int	 AddAffector(EAnimAffectorType eAnimAffectorType, int iTracktype) = 0;
	virtual void RemoveAffector(EAnimAffectorType eAnimAffectorType) = 0;

	virtual IAnimAffector* GetAffectorByType(EAnimAffectorType eAnimAffectorType) = 0;
	virtual IAnimAffector* GetAffectorByIndex(int iIndex) = 0;

	virtual int GetAffectorNum() = 0;
};
NiSmartPointer(IAnimAffectorControl);
//-----------------------------------------------------------------------------
ENGINE_NAMESPACE_END_MOVIESYSTEM
#endif
