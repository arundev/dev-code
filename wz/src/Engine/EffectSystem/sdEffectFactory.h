//*************************************************************************************************
// ����:	��Ч����
//---------------------------------------------------------
// ����:		
// ����:		2013-01-15
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _EFFECT_SYSTEM_EFFECT_FACTORY_H__
#define _EFFECT_SYSTEM_EFFECT_FACTORY_H__

#include <IEffect.h>

ENGINE_NAMESPACE_BEGIN_EFFECTSYSTEM

class sdEffectFactory : public IEffectFactory
{
public:
	sdEffectFactory();
	virtual ~sdEffectFactory();

	// �麯���̳�
	virtual bool Initialize();
	virtual void Destroy();

	virtual IEffect* CreateEffect(EEffectType eEffectType);
	virtual void DestroyEffect(IEffect* pkEffect);
};

ENGINE_NAMESPACE_END_EFFECTSYSTEM
#endif