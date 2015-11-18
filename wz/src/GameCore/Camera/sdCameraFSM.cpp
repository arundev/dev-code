#include "sdGameCorePCH.h"
#include "sdCameraFSM.h"
#include "sdCameraState.h"

SD_GAMECORE_NAMESPACE_BEGIN_GAMECORE
//-------------------------------------------------------------------------------------------------
sdCameraFSM::sdCameraFSM()
{

}
//-------------------------------------------------------------------------------------------------
sdCameraFSM::~sdCameraFSM()
{

}
//-------------------------------------------------------------------------------------------------
bool sdCameraFSM::Initialize()
{
	// 创建主相机
	// @{
	NiRenderer* spRender = NiRenderer::GetRenderer();
	NIASSERT(spRender);

	m_spCamera = NiNew NiCamera;
	NIASSERT(m_spCamera);

	// 视形体
	Ni2DBuffer* spBackBuffer = spRender->GetDefaultBackBuffer();
	NIASSERT(spBackBuffer);

	float fAspectRadio = spBackBuffer->GetWidth() / (float)spBackBuffer->GetHeight();
	float fVerticalFieldOfViewDegree = 20.0f;
	float fVerticalFieldOfViewRad = NI_PI / 180.0f * fVerticalFieldOfViewDegree;
	float fViewPlaneHalfHeight = tanf(fVerticalFieldOfViewRad * 0.5f);
	float fViewPlaneHalfWidth = fViewPlaneHalfHeight * fAspectRadio;

	NiFrustum kFrustum(-fViewPlaneHalfWidth, fViewPlaneHalfWidth,
		fViewPlaneHalfHeight, -fViewPlaneHalfHeight, 
		1.0f, 1000.0f);
	m_spCamera->SetViewFrustum(kFrustum);

	// 视口
	NiRect<float> kViewport(0.0f, 1.0f, 1.0f, 0.0f);
	m_spCamera->SetViewPort(kViewport);

	// 位置
	m_spCamera->SetTranslate(0, 0, 10);

	// 旋转
	NiMatrix3 kRotation = NiViewMath::Look(0, 0, NiMatrix3::IDENTITY, NiPoint3::UNIT_Z);
	m_spCamera->SetRotate(kRotation);

	// 更新
	m_spCamera->Update(0.0f);
	// @}

	return true;
}
//-------------------------------------------------------------------------------------------------
void sdCameraFSM::Destroy()
{
	m_spCamera = 0;
}
//-------------------------------------------------------------------------------------------------
int sdCameraFSM::UpdateState()
{
	int iResult = __super::UpdateState();
	return iResult;
}
//-------------------------------------------------------------------------------------------------
SD_GAMECORE_NAMESPACE_END_GAMECORE