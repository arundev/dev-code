#include "sdEffectSystemPCH.h"
#include "sdNifEffect.h"

ENGINE_NAMESPACE_BEGIN_EFFECTSYSTEM
//-------------------------------------------------------------------------------------------------
sdNifEffect::sdNifEffect()
{

}
//-------------------------------------------------------------------------------------------------
sdNifEffect::~sdNifEffect()
{
	Destroy();
}
//-------------------------------------------------------------------------------------------------
void sdNifEffect::UpdateFrame(float fAccumTime, float fFrameTime)
{
	if (m_spNifRoot)
		m_spNifRoot->Update(fAccumTime);
}
//-------------------------------------------------------------------------------------------------
bool sdNifEffect::Load(Engine::Util::sdLuaReadUtil& kLuaStream)
{
	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdNifEffect::Save(Engine::Util::sdLuaWriteUtil& kLuaStream)
{
	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdNifEffect::Initialize()
{
	return true;
}
//-------------------------------------------------------------------------------------------------
void sdNifEffect::Destroy()
{

}
//-------------------------------------------------------------------------------------------------
void sdNifEffect::Start()
{

}
//-------------------------------------------------------------------------------------------------
void sdNifEffect::Stop()
{

}
//-------------------------------------------------------------------------------------------------
void sdNifEffect::Pause()
{

}
//-------------------------------------------------------------------------------------------------
void sdNifEffect::Resume()
{

}
//-------------------------------------------------------------------------------------------------
void sdNifEffect::SetParam(const std::string& key, const std::string& value)
{

}
//-------------------------------------------------------------------------------------------------
void sdNifEffect::GetParam(const std::string& key, std::string& value)
{

}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_EFFECTSYSTEM