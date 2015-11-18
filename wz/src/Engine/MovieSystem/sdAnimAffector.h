//*************************************************************************************************
// 内容:	
//---------------------------------------------------------
// 作者:		
// 创建:		2013-01-08
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_MOVIE_SYSTEM_ANIM_AFFECTOR_H__
#define _SD_ENGINE_MOVIE_SYSTEM_ANIM_AFFECTOR_H__

#include "IAnimTrack.h"

ENGINE_NAMESPACE_BEGIN_MOVIESYSTEM
//-------------------------------------------------------------------------------------------------
class sdAnimAffector : public IAnimAffector
{
public:
	sdAnimAffector();
	virtual ~sdAnimAffector();

	// 虚函数继承
	virtual EAnimAffectorType GetType() { return E_ANIM_AFFECTOR_DEFAULT;}

	virtual bool Load(Engine::Util::sdLuaReadUtil& kLuaStream);
	virtual bool Save(Engine::Util::sdLuaWriteUtil& kLuaStream);
	virtual void Clear(){};

	virtual void UpdateFrame(float fAccumTime, float fFrameTime, IAnimNode* pkAnimNode){};
	virtual void Reset();

	virtual void SetAnimTrack(IAnimTrack* pkAnimTrack) { m_pkAnimTrack = pkAnimTrack;}
	virtual IAnimTrack* GetAnimTrack() { return m_pkAnimTrack;}

	virtual IAnimAffectorControl* GetAnimTrackControl() { return m_pkAnimAffectorControl;}

	// 用于数据流化
	static const std::string& GetStringFromType(EAnimAffectorType eType);
	static EAnimAffectorType  GetTypeFromString(const std::string& strType);

	static IAnimAffector* CreateAffector(EAnimAffectorType eType);

protected:
	// 检查当权是否有一帧位于指定时间间隙
	static bool IsChangingKey(IAnimTrack* pkAnimTrack, float fStartTime, float fEndTime);

protected:
	bool m_bLastKeyUpdated;	///< 最后一帧是否已经更新
	float m_fStartTime;		///< 起始时间
	float m_fEndTime;		///< 终止时间
	IAnimAffectorControlPtr m_pkAnimAffectorControl;	///< 所有AnimTrackControl
	IAnimTrackPtr m_pkAnimTrack;						///< 所有AnimTrack

protected:
	static std::string ms_kEffectTypeStringTable[NUM_ANIM_AFFECTOR];	///< AnimAffector类型字符串表
};
NiSmartPointer(sdAnimAffector);
//-------------------------------------------------------------------------------------------------
class sdAnimPositionAffector : public sdAnimAffector
{
public:
	sdAnimPositionAffector();

	// 虚函数继承
	virtual EAnimAffectorType GetType() { return E_ANIM_AFFECTOR_POSITION;}
	virtual void UpdateFrame(float fAccumTime, float fFrameTime, IAnimNode* pkAnimNode);
};
NiSmartPointer(sdAnimPositionAffector);
//-------------------------------------------------------------------------------------------------
class sdAnimRotationAffector : public sdAnimAffector
{
public:
	sdAnimRotationAffector();

	// 虚函数继承
	virtual EAnimAffectorType GetType() { return E_ANIM_AFFECTOR_ROTATION;}
	virtual void UpdateFrame(float fAccumTime, float fFrameTime, IAnimNode* pkAnimNode);
};
NiSmartPointer(sdAnimRotationAffector);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_MOVIESYSTEM
#endif