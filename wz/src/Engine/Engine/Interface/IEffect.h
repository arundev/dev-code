//*************************************************************************************************
// 内容:	特效接口
//---------------------------------------------------------
// 作者:		
// 创建:		2012-12-28
// 最后修改:
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
	E_EFFECT_NIF,		///< NIF文件特效
	E_EFFECT_TAIL,
	E_EFFECT_LIGHTING,
	E_EFFECT_CERATE,	///< 自建特效(sdMovie创建的)
	E_EFFECT_DECAL,		///< 贴花
	E_EFFECT_PARABOLA,	
	E_EFFECT_BLADE,		///< 刀光

	NUM_EFFECTS,
};
//-----------------------------------------------------------------------------
// IEffect
//-----------------------------------------------------------------------------
class IEffect : public NiRefObject
{
public:
	virtual ~IEffect() {};

	// 获取类型
	virtual EEffectType GetType() = 0;

	// 加载与保存参数
	virtual bool Load(Engine::Util::sdLuaReadUtil& kLuaStream) = 0;
	virtual bool Save(Engine::Util::sdLuaWriteUtil& kLuaStream) = 0;

	// 初始化与销毁
	virtual bool Initialize() = 0;
	virtual void Destroy() = 0;

	// 更新
	virtual void UpdateFrame(float fAccumTime, float fFrameTime) = 0;

	// 播放
	virtual void Start() = 0;
	virtual void Stop() = 0;
	virtual void Pause() = 0;
	virtual void Resume() = 0;

	// 姿态控制
	//virtual void SetPosition(const NiPoint3& kPosition) = 0;
	//virtual const NiPoint3& GetPosition() = 0;

	//virtual void SetRotation(const NiMatrix3& kRotation) = 0;
	//virtual const NiMatrix3& GetRotation() = 0;

	//virtual void SetScale(const float fScale) = 0;
	//virtual float GetScale() = 0;

	//virtual void SetVisiable(bool bVisiable) = 0;
	//virtual bool GetVisiable() = 0;

	// 委托实体(目前用的是GB的Node)
	virtual NiAVObject* GetEntity() = 0;

	// 获取与设置参数
	virtual void SetParam(const std::string& key, const std::string& value) = 0;
	virtual void GetParam(const std::string& key, std::string& value) = 0;

	// Shimmer效果设置
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

	// 初始化与销毁
	virtual bool Initialize() = 0;
	virtual void Destroy() = 0;

	// 创建/销毁Effect
	virtual IEffect* CreateEffect(EEffectType eEffectType) = 0;
	virtual void DestroyEffect(IEffect* pkEffect) = 0;

	// 获取工厂接口
	static IEffectFactory* GetFactory(){ return (IEffectFactory*)InstancePtr();}
};
//-----------------------------------------------------------------------------
ENGINE_NAMESPACE_END_EFFECTSYSTEM
#endif