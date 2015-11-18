#include "sdGameCorePCH.h"
#include "sdCameraState.h"
#include "sdCameraFSM.h"

SD_GAMECORE_NAMESPACE_BEGIN_GAMECORE
//-------------------------------------------------------------------------------------------------
int sdCameraMovieState::Update()
{
	sdCameraFSM* pkCameraFSM = sdCameraFSM::InstancePtr();
	NIASSERT(pkCameraFSM);

	NiCamera* spCamera = pkCameraFSM->GetCamera();
	spCamera->Update(0.f);

	return 0;
}
//-------------------------------------------------------------------------------------------------
SD_GAMECORE_NAMESPACE_END_GAMECORE