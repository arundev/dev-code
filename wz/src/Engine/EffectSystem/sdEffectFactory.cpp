#include "sdEffectSystemPCH.h"
#include "sdEffectFactory.h"
#include "sdNifEffect.h"

ENGINE_NAMESPACE_BEGIN_EFFECTSYSTEM
//-------------------------------------------------------------------------------------------------
sdEffectFactory::sdEffectFactory()
{

}
//-------------------------------------------------------------------------------------------------
sdEffectFactory::~sdEffectFactory()
{
	Destroy();
}
//-------------------------------------------------------------------------------------------------
bool sdEffectFactory::Initialize()
{
	return true;
}
//-------------------------------------------------------------------------------------------------
void sdEffectFactory::Destroy()
{

}
//-------------------------------------------------------------------------------------------------
IEffect* sdEffectFactory::CreateEffect(EEffectType eEffectType)
{
	IEffect* pkEffect = NULL;
	switch(eEffectType)
	{
	case E_EFFECT_NIF :
		pkEffect = NiNew sdNifEffect();
		NIASSERT(pkEffect);
		break;

	};

	return pkEffect;
}
//-------------------------------------------------------------------------------------------------
void sdEffectFactory::DestroyEffect(IEffect* pkEffect)
{
	
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_EFFECTSYSTEM