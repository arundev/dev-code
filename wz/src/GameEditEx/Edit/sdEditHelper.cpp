#include "sdGameEditExPCH.h"
#include "sdEditHelper.h"

#include <sdMap.h>
#include <IRenderSystem.h>

using namespace Engine::RenderSystem;

SD_GAMEEDITEX_NAMESPACE_BEGIN_GAMEEDITEX
//-------------------------------------------------------------------------------------------------
sdEditHelper::sdEditHelper()
{
	IRenderSystem* pkRenderSystem = IRenderSystem::GetRenderSystem();
	NIASSERT(pkRenderSystem);

	m_pkMap = pkRenderSystem->GetMap();
}
//-------------------------------------------------------------------------------------------------
void sdEditHelper::SetEnableMapChannel(int iChannel, bool bEnable)
{
	sdRenderParams& kRenderParams = m_pkMap->GetRenderParams();
	kRenderParams.EnableChannel(sdRenderParams::E_BUILDING, (sdRenderParams::eShadingChannel)iChannel, bEnable);
	kRenderParams.EnableChannel(sdRenderParams::E_ACTOR,	(sdRenderParams::eShadingChannel)iChannel, bEnable);
	kRenderParams.EnableChannel(sdRenderParams::E_TERRAIN,	(sdRenderParams::eShadingChannel)iChannel, bEnable);
}
//-------------------------------------------------------------------------------------------------
SD_GAMEEDITEX_NAMESPACE_END_GAMEEDITEX