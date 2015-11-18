#include "sdRenderSystemPCH.h"
#include "sdPostFogPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
sdPostFogPass::sdPostFogPass(sdRenderObjectAlloc* pkAlloc)
	: sdRenderPass(pkAlloc)
{

}
//-------------------------------------------------------------------------------------------------
sdPostFogPass::~sdPostFogPass()
{
	if (m_bInitialized)
	{
		Destroy();
		m_bInitialized = false;
	}
}
//-------------------------------------------------------------------------------------------------
bool sdPostFogPass::Initialize(NiTexture* spGeometryTexture)
{
	NIASSERT(spGeometryTexture);

	if (m_bInitialized)
	{
		NIASSERT(0);
		return false;
	}

	//
	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	// 初始化材质
	m_spFogMaterial = pkRenderDevice->CreateMaterial("PostEffect_Fog");
	NIASSERT(m_spFogMaterial);

	m_spCurveFogMaterial = pkRenderDevice->CreateMaterial("PostEffect_CurveFog");
	NIASSERT(m_spCurveFogMaterial);

	m_spAOAndFogMaterial = pkRenderDevice->CreateMaterial("PostEffect_CurveFogAndAO");
	NIASSERT(m_spAOAndFogMaterial);

	//m_spAODebugMaterial = pkRenderDevice->CreateMaterial("PostEffect_AODebug");
	//NIASSERT(m_spAODebugMaterial);

	// 初始化渲染属性
	m_spTexturingProp  = NiNew NiTexturingProperty;
	NIASSERT(m_spTexturingProp);
	m_spTexturingProp->SetShaderMap(0, NiNew NiTexturingProperty::ShaderMap(spGeometryTexture, 0));
	m_kPropertyList.AddTail((NiTexturingProperty*)m_spTexturingProp);

	return (m_bInitialized = true);
}
//-------------------------------------------------------------------------------------------------
void sdPostFogPass::Destroy()
{
	m_spFogMaterial = 0;
	m_spCurveFogMaterial = 0;
	m_spAOAndFogMaterial = 0;
	m_spAODebugMaterial = 0;

	m_kPropertyList.RemoveAll();
	m_spTexturingProp = 0;
}
//-------------------------------------------------------------------------------------------------
void sdPostFogPass::Draw()
{
	D3DPERF_BeginEvent(0xff000000, L"PostEffect_PostFog");



	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE