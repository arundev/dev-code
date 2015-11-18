//*************************************************************************************************
// ����:	��Ч�ӿ�
//---------------------------------------------------------
// ����:		
// ����:		2012-12-28
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_EFFECT_SYSTEM_EFFECT_INTERFACE_H__
#define _SD_ENGINE_EFFECT_SYSTEM_EFFECT_INTERFACE_H__

#include <sdFactory.h>

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_UTIL
class sdLuaReadUtil;
class sdLuaWriteUtil;
ENGINE_NAMESPACE_END_UTIL
ENGINE_NAMESPACE_END_ENGINE

ENGINE_NAMESPACE_BEGIN_EFFECTSYSTEM
//-----------------------------------------------------------------------------
// EEffectType
//-----------------------------------------------------------------------------
enum EEffectType
{
	E_EFFECT_INVALID,
	E_EFFECT_NIF,		///< NIF�ļ���Ч
	E_EFFECT_TAIL,
	E_EFFECT_LIGHTING,
	E_EFFECT_CERATE,	///< �Խ���Ч(sdMovie������)
	E_EFFECT_DECAL,		///< ����
	E_EFFECT_PARABOLA,	
	E_EFFECT_BLADE,		///< ����

	NUM_EFFECTS,
};
//-----------------------------------------------------------------------------
// IEffect
//-----------------------------------------------------------------------------
class IEffect : public NiRefObject
{
public:
	virtual ~IEffect() {};

	// ��ȡ����
	virtual EEffectType GetType() = 0;

	// �����뱣�����
	virtual bool Load(Engine::Util::sdLuaReadUtil& kLuaStream) = 0;
	virtual bool Save(Engine::Util::sdLuaWriteUtil& kLuaStream) = 0;

	// ��ʼ��������
	virtual bool Initialize() = 0;
	virtual void Destroy() = 0;

	// ����
	virtual void UpdateFrame(float fAccumTime, float fFrameTime) = 0;

	// ����
	virtual void Start() = 0;
	virtual void Stop() = 0;
	virtual void Pause() = 0;
	virtual void Resume() = 0;

	// ��̬����
	//virtual void SetPosition(const NiPoint3& kPosition) = 0;
	//virtual const NiPoint3& GetPosition() = 0;

	//virtual void SetRotation(const NiMatrix3& kRotation) = 0;
	//virtual const NiMatrix3& GetRotation() = 0;

	//virtual void SetScale(const float fScale) = 0;
	//virtual float GetScale() = 0;

	//virtual void SetVisiable(bool bVisiable) = 0;
	//virtual bool GetVisiable() = 0;

	// ί��ʵ��(Ŀǰ�õ���GB��Node)
	virtual NiAVObject* GetEntity() = 0;

	// ��ȡ�����ò���
	virtual void SetParam(const std::string& key, const std::string& value) = 0;
	virtual void GetParam(const std::string& key, std::string& value) = 0;

	// ShimmerЧ������
	//static void SetupShimmer(NiAVObject* spObject);
};
NiSmartPointer(sdEffect);

//-----------------------------------------------------------------------------
//IEffectFactory
//-----------------------------------------------------------------------------
class IEffectFactory : public Base::Misc::IFactory
{
public:
	virtual ~IEffectFactory() = 0;

	// ��ʼ��������
	virtual bool Initialize() = 0;
	virtual void Destroy() = 0;

	// ����/����Effect
	virtual IEffect* CreateEffect(EEffectType eEffectType) = 0;
	virtual void DestroyEffect(IEffect* pkEffect) = 0;

	// ��ȡ�����ӿ�
	static IEffectFactory* GetFactory(){ return (IEffectFactory*)InstancePtr();}
};
//-----------------------------------------------------------------------------
ENGINE_NAMESPACE_END_EFFECTSYSTEM
#endif