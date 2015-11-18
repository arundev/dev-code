#include "sdRenderSystemPCH.h"
#include "sdGodRayPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
sdGodRayPass::sdGodRayPass(sdRenderObjectAlloc* pkAlloc)
	: sdRenderPass(pkAlloc)
{

}
//-------------------------------------------------------------------------------------------------
sdGodRayPass::~sdGodRayPass()
{
	if (m_bInitialized)
	{
		Destroy();
		m_bInitialized = false;
	}
}
//-------------------------------------------------------------------------------------------------
bool sdGodRayPass::Initialize()
{
	if (m_bInitialized)
	{
		NIASSERT(0);
		return false;
	}

	//
	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	// 初始化材质
	m_spRadialBlurMaterial = pkRenderDevice->CreateMaterial("PostEffect_GodRay_RadialBlur");
	NIASSERT(m_spRadialBlurMaterial);

	// 初始化渲染属性
	m_spTexProp  = NiNew NiTexturingProperty;
	NIASSERT(m_spTexProp);
	m_spTexProp->SetShaderMap(0, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_kPropertyList.AddTail((NiTexturingProperty*)m_spTexProp);

	return (m_bInitialized = true);
}
//-------------------------------------------------------------------------------------------------
void sdGodRayPass::Destroy()
{
	m_spRadialBlurMaterial = 0;

	m_kPropertyList.RemoveAll();
	m_spTexProp = 0;

	m_bInitialized = false;
}
//-------------------------------------------------------------------------------------------------
void sdGodRayPass::SetRadialBlurInputTexture(NiTexture* spTexture)
{
	if (m_spTexProp->GetShaderMap(0)->GetTexture() != spTexture)
	{
		NiTexturingProperty::ShaderMap* spShaderMap = m_spTexProp->GetShaderMap(0);
		NIASSERT(spShaderMap);
		spShaderMap->SetTexture(spTexture);
	}
}
//-------------------------------------------------------------------------------------------------
void sdGodRayPass::RadialBlur()
{
	D3DPERF_BeginEvent(0xff000000, L"PostEffect_GodRay_RadialBlur");

	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	IRenderSystem* pkRenderSystem = IRenderSystem::GetRenderSystem();
	NIASSERT(pkRenderSystem);

	NiRenderer* spRenderer = NiRenderer::GetRenderer();
	NIASSERT(spRenderer);

	// 设置渲染状态
	// @{
	pkRenderDevice->SetRenderState(D3DRS_CULLMODE,			D3DCULL_NONE);
	pkRenderDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,	false);
	pkRenderDevice->SetRenderState(D3DRS_ALPHATESTENABLE,	false);
	pkRenderDevice->SetRenderState(D3DRS_ZENABLE,			false);
	pkRenderDevice->SetRenderState(D3DRS_ZWRITEENABLE,		false);
	pkRenderDevice->SetRenderState(D3DRS_STENCILENABLE,		false);
	// @}

	// 绘制
	// @{
	NiMesh* spScreenMesh = pkRenderSystem->GetDefaultMesh(IRenderSystem::E_DM_SCREEN_QUAD);
	NIASSERT(spScreenMesh);

	NiPropertyState* spPropState = spScreenMesh->GetPropertyState();
	NIASSERT(spPropState);
	spPropState->SwapProperties(m_kPropertyList);
	spScreenMesh->ApplyAndSetActiveMaterial(m_spRadialBlurMaterial);
	spScreenMesh->RenderImmediate(spRenderer);
	spScreenMesh->SetActiveMaterial(NULL);
	spPropState->SwapProperties(m_kPropertyList);
	// @}

	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE