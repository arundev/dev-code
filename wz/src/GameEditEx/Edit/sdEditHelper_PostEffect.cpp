#include "sdGameEditExPCH.h"
#include "sdEditHelper.h"

#include <sdMap.h>
#include <IRenderSystem.h>

using namespace Engine::RenderSystem;

SD_GAMEEDITEX_NAMESPACE_BEGIN_GAMEEDITEX
//-------------------------------------------------------------------------------------------------
bool sdEditHelper::GetEnableMLAA()
{
	sdPostProcessParams& kParams = m_pkMap->GetPostProcessParams();
	return kParams.enableMLAA;
}
//-------------------------------------------------------------------------------------------------
void sdEditHelper::SetEnableMLAA(bool bEnable)
{
	sdPostProcessParams& kParams = m_pkMap->GetPostProcessParams();
	kParams.enableMLAA = bEnable;
}
//-------------------------------------------------------------------------------------------------
bool sdEditHelper::GetEnableColorGrading()
{
	sdPostProcessParams& kParams = m_pkMap->GetPostProcessParams();
	return kParams.enableColorGrading;
}
//-------------------------------------------------------------------------------------------------
void sdEditHelper::SetEnableColorGrading(bool bEnable)
{
	sdPostProcessParams& kParams = m_pkMap->GetPostProcessParams();
	kParams.enableColorGrading = bEnable;
}
//-------------------------------------------------------------------------------------------------
bool sdEditHelper::GetEnableDof()
{
	sdPostProcessParams& kParams = m_pkMap->GetPostProcessParams();
	return kParams.enableDof;
}
//-------------------------------------------------------------------------------------------------
void sdEditHelper::SetEnableDof(bool bEnable)
{
	sdPostProcessParams& kParams = m_pkMap->GetPostProcessParams();
	kParams.enableDof = bEnable;
}
//-------------------------------------------------------------------------------------------------
bool sdEditHelper::GetEnableGodRay()
{
	sdPostProcessParams& kParams = m_pkMap->GetPostProcessParams();
	return kParams.enableGodRay;
}
//-------------------------------------------------------------------------------------------------
void sdEditHelper::SetEnableGodRay(bool bEnable)
{
	sdPostProcessParams& kParams = m_pkMap->GetPostProcessParams();
	kParams.enableGodRay = bEnable;
}
//-------------------------------------------------------------------------------------------------
bool sdEditHelper::GetEnableHDR()
{
	sdPostProcessParams& kParams = m_pkMap->GetPostProcessParams();
	return kParams.enableHDR;
}
//-------------------------------------------------------------------------------------------------
void sdEditHelper::SetEnableHDR(bool bEnable)
{
	sdPostProcessParams& kParams = m_pkMap->GetPostProcessParams();
	kParams.enableHDR = bEnable;
}
//-------------------------------------------------------------------------------------------------
SD_GAMEEDITEX_NAMESPACE_END_GAMEEDITEX