#include "WorldEditor.FrameworkPCH.h"
#include "MLight.h"

#include <sdLight.h>

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
//-----------------------------------------------------------------------------
MLight::MLight()
: MEntity(/*NiNew sdLight*/)
{

}
//-----------------------------------------------------------------------------
MLight::MLight(sdLight* pkLight)
: MEntity(pkLight)
{

}
//-----------------------------------------------------------------------------
MLight::MLight(MNativePtWrap^ pkNativePointer)
: MEntity(pkNativePointer)
{

}
//-----------------------------------------------------------------------------
void MLight::AmbientColor::set(MColor^ val)
{
	NiColor kColor(val->R, val->G, val->B);
	((sdLight*)m_pkGameObject)->SetAmbientColor(kColor);
}
//-----------------------------------------------------------------------------
MColor^ MLight::AmbientColor::get()
{
	const NiColor& kColor = ((sdLight*)m_pkGameObject)->GetAmbientColor();
	return gcnew MColor(kColor.m_r, kColor.m_g, kColor.m_b);
}
//-----------------------------------------------------------------------------
void MLight::DiffuseColor::set(MColor^ val)
{
	NiColor kColor(val->R, val->G, val->B);
	((sdLight*)m_pkGameObject)->SetDiffuseColor(kColor);
}
//-----------------------------------------------------------------------------
MColor^ MLight::DiffuseColor::get()
{
	const NiColor& kColor = ((sdLight*)m_pkGameObject)->GetDiffuseColor();
	return gcnew MColor(kColor.m_r, kColor.m_g, kColor.m_b);
}
//-----------------------------------------------------------------------------
void MLight::SpecularColor::set(MColor^ val)
{
	NiColor kColor(val->R, val->G, val->B);
	((sdLight*)m_pkGameObject)->SetSpecularColor(kColor);
}
//-----------------------------------------------------------------------------
MColor^ MLight::SpecularColor::get()
{
	const NiColor& kColor = ((sdLight*)m_pkGameObject)->GetSpecularColor();
	return gcnew MColor(kColor.m_r, kColor.m_g, kColor.m_b);
}
//-----------------------------------------------------------------------------
void MLight::Dimmer::set(float val)
{
	((sdLight*)m_pkGameObject)->SetDimmer(val);
}
//-----------------------------------------------------------------------------
float MLight::Dimmer::get()
{
	return ((sdLight*)m_pkGameObject)->GetDimmer();
}
//-----------------------------------------------------------------------------
void MLight::Update(float fTime)
{

}
//-----------------------------------------------------------------------------
void MLight::Render()
{

}
//-----------------------------------------------------------------------------
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK