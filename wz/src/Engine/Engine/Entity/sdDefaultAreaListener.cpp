#include "sdEnginePCH.h"
#include "sdDefaultAreaListener.h"

//-------------------------------------------------------------------------------------------------
sdDefaultAreaListener::sdDefaultAreaListener()
{

}
//-------------------------------------------------------------------------------------------------
sdDefaultAreaListener::~sdDefaultAreaListener()
{

}
//-------------------------------------------------------------------------------------------------
const NiPoint3& sdDefaultAreaListener::GetListenerPos() const
{
	return NiPoint3::ZERO;
}
//-------------------------------------------------------------------------------------------------
void sdDefaultAreaListener::Update()
{

}
//-------------------------------------------------------------------------------------------------
void sdDefaultAreaListener::OnEnterArea(sdArea* pkArea)
{
	if (pkArea)	return;

	//OnEnter2DAudioArea(pkArea);
	//OnEnter3DAmbientAudioArea(pkArea);
	//OnEnterReverAudioArea(pkArea);
	//OnEnterEnvironmentArea(pkArea);
}
//-------------------------------------------------------------------------------------------------
void sdDefaultAreaListener::OnStayArea(sdArea* pkArea)
{
	if (pkArea)	return;

}
//-------------------------------------------------------------------------------------------------
void sdDefaultAreaListener::OnLeaveArea(sdArea* pkArea)
{
	if (pkArea)	return;

	//OnLeave2DAudioArea(pkArea);
	//OnLeave3DAmbientAudioArea(pkArea);
	//OnLeaveReverAudioArea(pkArea);
	//OnLeaveEnvironmentArea(pkArea);
}
//-------------------------------------------------------------------------------------------------