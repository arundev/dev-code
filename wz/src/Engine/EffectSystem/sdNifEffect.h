//*************************************************************************************************
// ����:	Nif��Ч
//---------------------------------------------------------
// ����:		
// ����:		2013-01-05
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _EFFECT_SYSTEM_NIF_EFFECT_H__
#define _EFFECT_SYSTEM_NIF_EFFECT_H__

#include <IEffect.h>

ENGINE_NAMESPACE_BEGIN_EFFECTSYSTEM

class sdNifEffect : public IEffect
{
public:
	sdNifEffect();
	virtual ~sdNifEffect();

	// �麯���̳�
	virtual inline EEffectType GetType();

	virtual bool Load(Engine::Util::sdLuaReadUtil& kLuaStream);
	virtual bool Save(Engine::Util::sdLuaWriteUtil& kLuaStream);
	
	virtual bool Initialize();
	virtual void Destroy();

	virtual void UpdateFrame(float fAccumTime, float fFrameTime);

	virtual void Start();
	virtual void Stop();
	virtual void Pause();
	virtual void Resume();

	virtual inline NiAVObject* GetEntity();

	virtual void SetParam(const std::string& key, const std::string& value);
	virtual void GetParam(const std::string& key, std::string& value);

protected:
	std::string		m_strFileName;	///< NIF��Դ·��
	NiAVObjectPtr	m_spNifRoot;	///< NIF��Դ���ڵ�
};
NiSmartPointer(sdNifEffect);

ENGINE_NAMESPACE_END_EFFECTSYSTEM
#endif