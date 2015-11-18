#include "WorldEditor.FrameworkPCH.h"
#include "MMap.h"

#include <sdMap.h>

using namespace System;

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
//-----------------------------------------------------------------------------
MMap::MMap()
: MGameObject(/*NiNew MMap*/)
{

}
//-----------------------------------------------------------------------------
MMap::MMap(sdMap* pkMap)
: MGameObject(pkMap)
{

}
//-----------------------------------------------------------------------------
MMap::MMap(MNativePtWrap^ pkNativePointer)
: MGameObject(pkNativePointer)
{

}
//-----------------------------------------------------------------------------
void MMap::Name::set(String^ val)
{
	((sdMap*)m_pkGameObject)->SetName(MStringToCharPointer(val));
}
//-----------------------------------------------------------------------------
String^ MMap::Name::get()
{
	return gcnew String((const char*)((sdMap*)m_pkGameObject)->GetName());
}
//-----------------------------------------------------------------------------
MLight^ MMap::AmbientLight::get()
{
	return gcnew MLight(((sdMap*)m_pkGameObject)->GetAmbientLight());
}
//-----------------------------------------------------------------------------
MLight^ MMap::MainLight::get()
{
	return gcnew MLight(((sdMap*)m_pkGameObject)->GetMainLight());
}
//-----------------------------------------------------------------------------
MLight^ MMap::AssistantLight::get()
{
	return gcnew MLight(((sdMap*)m_pkGameObject)->GetAssistantLight());
}
//-----------------------------------------------------------------------------
void MMap::Update(float fTime)
{

}
//-----------------------------------------------------------------------------
void MMap::Render()
{

}
//-----------------------------------------------------------------------------
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK