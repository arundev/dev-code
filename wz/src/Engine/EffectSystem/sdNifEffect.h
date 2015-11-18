//*************************************************************************************************
// 内容:	Nif特效
//---------------------------------------------------------
// 作者:		
// 创建:		2013-01-05
// 最后修改:
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

	// 虚函数继承
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
	std::string		m_strFileName;	///< NIF资源路径
	NiAVObjectPtr	m_spNifRoot;	///< NIF资源根节点
};
NiSmartPointer(sdNifEffect);

ENGINE_NAMESPACE_END_EFFECTSYSTEM
#endif