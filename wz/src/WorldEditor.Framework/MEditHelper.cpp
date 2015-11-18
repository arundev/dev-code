#include "WorldEditor.FrameworkPCH.h"
#include "MEditHelper.h"
#include "sdWorldEditor.h"
#include "MMacros.h"

//
#include <sdWorldEditFSM.h>
#include <sdEditHelper.h>

//
using namespace GameEditEx;

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
//-------------------------------------------------------------------------------------------------
MEditHelper::MEditHelper()
{
	sdWorldEditFSM* pkWorldEditFSM = sdWorldEditor::Instance().GetWorldEditFSM();
	m_pkEditHelper = pkWorldEditFSM->GetEditHelper();
}
//-------------------------------------------------------------------------------------------------
void MEditHelper::SetEnableMapChannel(int iChannel, bool bEnable)
{
	m_pkEditHelper->SetEnableMapChannel(iChannel, bEnable);
}
//-------------------------------------------------------------------------------------------------
MMap^ MEditHelper::Map::get()
{
	return gcnew MMap(m_pkEditHelper->GetMap());
}
//-------------------------------------------------------------------------------------------------
MLight^ MEditHelper::AmbientLight::get()
{
	return gcnew MLight(m_pkEditHelper->GetMap()->GetAmbientLight());
}
//-------------------------------------------------------------------------------------------------
MLight^ MEditHelper::MainLight::get()
{
	return gcnew MLight(m_pkEditHelper->GetMap()->GetMainLight());
}
//-------------------------------------------------------------------------------------------------
MLight^ MEditHelper::AssistantLight::get()
{
	return gcnew MLight(m_pkEditHelper->GetMap()->GetAssistantLight());
}
//-------------------------------------------------------------------------------------------------
void MEditHelper::MLAA::set(bool val)
{
	m_pkEditHelper->SetEnableMLAA(val);
}
//-------------------------------------------------------------------------------------------------
bool MEditHelper::MLAA::get()
{
	return m_pkEditHelper->GetEnableMLAA();
}
//-------------------------------------------------------------------------------------------------
void MEditHelper::ColorGrading::set(bool val)
{
	m_pkEditHelper->SetEnableColorGrading(val);
}
//-------------------------------------------------------------------------------------------------
bool MEditHelper::ColorGrading::get()
{
	return m_pkEditHelper->GetEnableColorGrading();
}
//-------------------------------------------------------------------------------------------------
void MEditHelper::Dof::set(bool val)
{
	m_pkEditHelper->SetEnableDof(val);
}
//-------------------------------------------------------------------------------------------------
bool MEditHelper::Dof::get()
{
	return m_pkEditHelper->GetEnableDof();
}
//-------------------------------------------------------------------------------------------------
void MEditHelper::GodRay::set(bool val)
{
	m_pkEditHelper->SetEnableGodRay(val);
}
//-------------------------------------------------------------------------------------------------
bool MEditHelper::GodRay::get()
{
	return m_pkEditHelper->GetEnableGodRay();
}
//-------------------------------------------------------------------------------------------------
void MEditHelper::HDR::set(bool val)
{
	m_pkEditHelper->SetEnableHDR(val);
}
//-------------------------------------------------------------------------------------------------
bool MEditHelper::HDR::get()
{
	return m_pkEditHelper->GetEnableHDR();
}
//-------------------------------------------------------------------------------------------------
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK