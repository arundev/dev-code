#include "sdEnginePCH.h"
#include "sdActor.h"

using namespace Engine::RenderSystem;

NiImplementRTTI(sdActor, sdEntity);
sdImplementDefaultStream(sdActor);

IMPL_PROPFUNC_MAP(sdActor, sdEntity)
IMPL_DYNAMIC(sdActor, sdEntity)

//-------------------------------------------------------------------------------------------------
sdActor::sdActor()
: m_bEnableShadow(true)
, m_bEnableRimlight(false)
{
#ifdef SDDEBUG
	SetName("Actor");
#endif
}
//-------------------------------------------------------------------------------------------------
sdActor::~sdActor()
{

}
//-------------------------------------------------------------------------------------------------
void sdActor::Cull(const NiCamera& kCamera, IRenderPath& kRenderPath)
{
	if (GetAppCulled())
		return;

	__super::Cull(kCamera, kRenderPath);


}
//-------------------------------------------------------------------------------------------------