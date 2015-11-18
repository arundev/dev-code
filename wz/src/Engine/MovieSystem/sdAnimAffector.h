//*************************************************************************************************
// ����:	
//---------------------------------------------------------
// ����:		
// ����:		2013-01-08
// ����޸�:
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

	// �麯���̳�
	virtual EAnimAffectorType GetType() { return E_ANIM_AFFECTOR_DEFAULT;}

	virtual bool Load(Engine::Util::sdLuaReadUtil& kLuaStream);
	virtual bool Save(Engine::Util::sdLuaWriteUtil& kLuaStream);
	virtual void Clear(){};

	virtual void UpdateFrame(float fAccumTime, float fFrameTime, IAnimNode* pkAnimNode){};
	virtual void Reset();

	virtual void SetAnimTrack(IAnimTrack* pkAnimTrack) { m_pkAnimTrack = pkAnimTrack;}
	virtual IAnimTrack* GetAnimTrack() { return m_pkAnimTrack;}

	virtual IAnimAffectorControl* GetAnimTrackControl() { return m_pkAnimAffectorControl;}

	// ������������
	static const std::string& GetStringFromType(EAnimAffectorType eType);
	static EAnimAffectorType  GetTypeFromString(const std::string& strType);

	static IAnimAffector* CreateAffector(EAnimAffectorType eType);

protected:
	// ��鵱Ȩ�Ƿ���һ֡λ��ָ��ʱ���϶
	static bool IsChangingKey(IAnimTrack* pkAnimTrack, float fStartTime, float fEndTime);

protected:
	bool m_bLastKeyUpdated;	///< ���һ֡�Ƿ��Ѿ�����
	float m_fStartTime;		///< ��ʼʱ��
	float m_fEndTime;		///< ��ֹʱ��
	IAnimAffectorControlPtr m_pkAnimAffectorControl;	///< ����AnimTrackControl
	IAnimTrackPtr m_pkAnimTrack;						///< ����AnimTrack

protected:
	static std::string ms_kEffectTypeStringTable[NUM_ANIM_AFFECTOR];	///< AnimAffector�����ַ�����
};
NiSmartPointer(sdAnimAffector);
//-------------------------------------------------------------------------------------------------
class sdAnimPositionAffector : public sdAnimAffector
{
public:
	sdAnimPositionAffector();

	// �麯���̳�
	virtual EAnimAffectorType GetType() { return E_ANIM_AFFECTOR_POSITION;}
	virtual void UpdateFrame(float fAccumTime, float fFrameTime, IAnimNode* pkAnimNode);
};
NiSmartPointer(sdAnimPositionAffector);
//-------------------------------------------------------------------------------------------------
class sdAnimRotationAffector : public sdAnimAffector
{
public:
	sdAnimRotationAffector();

	// �麯���̳�
	virtual EAnimAffectorType GetType() { return E_ANIM_AFFECTOR_ROTATION;}
	virtual void UpdateFrame(float fAccumTime, float fFrameTime, IAnimNode* pkAnimNode);
};
NiSmartPointer(sdAnimRotationAffector);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_MOVIESYSTEM
#endif