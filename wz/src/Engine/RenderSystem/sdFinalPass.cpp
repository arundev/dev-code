#include "sdRenderSystemPCH.h"
#include "sdFinalPass.h"

using namespace Engine::ResourceSystem;

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
sdFinalPass::sdFinalPass(sdRenderObjectAlloc* pkAlloc)
	: sdRenderPass(pkAlloc)
{

}
//-------------------------------------------------------------------------------------------------
sdFinalPass::~sdFinalPass()
{
	if (m_bInitialized)
	{
		Destroy();
		m_bInitialized = false;
	}
}
//-------------------------------------------------------------------------------------------------
bool sdFinalPass::Initialize()
{
	if (m_bInitialized)
	{
		NIASSERT(0);
		return false;
	}

	//
	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	IResourceSystem* pkResourceSystem = IResourceSystem::GetResourceSystem();
	NIASSERT(pkResourceSystem);

	// ÉèÖÃÊäÈëÎÆÀí
	m_spRedAlertTexture = pkResourceSystem->LoadTexture("dependence\\engine\\texture\\postprocess\\poison.png");
	NIASSERT(m_spRedAlertTexture);

	// ³õÊ¼»¯²ÄÖÊ
	m_spCopyMaterial = pkRenderDevice->CreateMaterial("PostEffect_Copy");
	NIASSERT(m_spCopyMaterial);

	m_spCopyToBackBufferMaterial = pkRenderDevice->CreateMaterial("PostEffect_CopyToBackBuffer");
	NIASSERT(m_spCopyToBackBufferMaterial); 

	m_spCompositeMaterial = pkRenderDevice->CreateMaterial("PostEffect_Composite");
	NIASSERT(m_spCompositeMaterial);

	// ³õÊ¼»¯äÖÈ¾ÊôÐÔ
	m_spCopyTexProp  = NiNew NiTexturingProperty;
	NIASSERT(m_spCopyTexProp);
	m_spCopyTexProp->SetShaderMap(0, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_kCopyPropertyList.AddTail((NiTexturingProperty*)m_spCopyTexProp);

	m_spCopyToBackBufferTexProp  = NiNew NiTexturingProperty;
	NIASSERT(m_spCopyToBackBufferTexProp);
	m_spCopyToBackBufferTexProp->SetShaderMap(0, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_spCopyToBackBufferTexProp->SetShaderMap(1, NiNew NiTexturingProperty::ShaderMap(m_spRedAlertTexture, 0));
	m_kCopyToBackBufferPropertyList.AddTail((NiTexturingProperty*)m_spCopyToBackBufferTexProp);

	m_spCompositeTexProp  = NiNew NiTexturingProperty;
	NIASSERT(m_spCompositeTexProp);
	m_spCompositeTexProp->SetShaderMap(0, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_spCompositeTexProp->SetShaderMap(1, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_spCompositeTexProp->SetShaderMap(2, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_spCompositeTexProp->SetShaderMap(3, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_spCompositeTexProp->SetShaderMap(4, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_kCompositeList.AddTail((NiTexturingProperty*)m_spCompositeTexProp);

	return (m_bInitialized = true);
}
//-------------------------------------------------------------------------------------------------
void sdFinalPass::Destroy()
{
	m_spCopyMaterial = 0;
	m_spCopyToBackBufferMaterial = 0;
	m_spCompositeMaterial = 0;

	m_kCopyPropertyList.RemoveAll();
	m_kCopyToBackBufferPropertyList.RemoveAll();
	m_kCompositeList.RemoveAll();
	m_spCopyTexProp = 0;
	m_spCopyToBackBufferTexProp = 0;
	m_spCompositeTexProp = 0;

	m_bInitialized = false;
}
//-------------------------------------------------------------------------------------------------
void sdFinalPass::SetCopyInputTexture(NiTexture* spTexture) 
{ 
	if (m_spCopyTexProp->GetShaderMap(0)->GetTexture() != spTexture)
	{
		NiTexturingProperty::ShaderMap* spShaderMap = m_spCopyTexProp->GetShaderMap(0);
		NIASSERT(spShaderMap);
		spShaderMap->SetTexture(spTexture);
	}
}
//-------------------------------------------------------------------------------------------------
void sdFinalPass::Copy()
{
	D3DPERF_BeginEvent(0xff000000, L"PostEffect_Copy");

	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	IRenderSystem* pkRenderSystem = IRenderSystem::GetRenderSystem();
	NIASSERT(pkRenderSystem);

	NiRenderer* spRenderer = NiRenderer::GetRenderer();
	NIASSERT(spRenderer);

	// ÉèÖÃäÖÈ¾×´Ì¬
	// @{
	pkRenderDevice->SetRenderState(D3DRS_CULLMODE,			D3DCULL_NONE);
	pkRenderDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,	false);
	pkRenderDevice->SetRenderState(D3DRS_ALPHATESTENABLE,	false);
	pkRenderDevice->SetRenderState(D3DRS_ZENABLE,			false);
	pkRenderDevice->SetRenderState(D3DRS_ZWRITEENABLE,		false);
	pkRenderDevice->SetRenderState(D3DRS_STENCILENABLE,		false);
	// @}

	// »æÖÆ
	// @{
	NiMesh* spScreenMesh = pkRenderSystem->GetDefaultMesh(IRenderSystem::E_DM_SCREEN_QUAD);
	NIASSERT(spScreenMesh);

	NiPropertyState* spPropState = spScreenMesh->GetPropertyState();
	NIASSERT(spPropState);
	spPropState->SwapProperties(m_kCopyPropertyList);
	spScreenMesh->ApplyAndSetActiveMaterial(m_spCopyMaterial);
	spScreenMesh->RenderImmediate(spRenderer);
	spScreenMesh->SetActiveMaterial(NULL);
	spPropState->SwapProperties(m_kCopyPropertyList);
	// @}

	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
void sdFinalPass::SetCopyToBackBufferInputTexture_ColorTexture(NiTexture* spTexture)
{
	if (m_spCopyToBackBufferTexProp->GetShaderMap(0)->GetTexture() != spTexture)
	{
		NiTexturingProperty::ShaderMap* spShaderMap = m_spCopyToBackBufferTexProp->GetShaderMap(0);
		NIASSERT(spShaderMap);
		spShaderMap->SetTexture(spTexture);
	}
}
//-------------------------------------------------------------------------------------------------
void sdFinalPass::CopyToBackBuffer()
{
	D3DPERF_BeginEvent(0xff000000, L"PostEffect_CopyToBackBuffer");

	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	IRenderSystem* pkRenderSystem = IRenderSystem::GetRenderSystem();
	NIASSERT(pkRenderSystem);

	NiRenderer* spRenderer = NiRenderer::GetRenderer();
	NIASSERT(spRenderer);

	// ÉèÖÃäÖÈ¾×´Ì¬
	// @{
	pkRenderDevice->SetRenderState(D3DRS_CULLMODE,			D3DCULL_NONE);
	pkRenderDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,	false);
	pkRenderDevice->SetRenderState(D3DRS_ALPHATESTENABLE,	false);
	pkRenderDevice->SetRenderState(D3DRS_ZENABLE,			false);
	pkRenderDevice->SetRenderState(D3DRS_ZWRITEENABLE,		false);
	pkRenderDevice->SetRenderState(D3DRS_STENCILENABLE,		false);
	// @}

	// »æÖÆ
	// @{
	NiMesh* spScreenMesh = pkRenderSystem->GetDefaultMesh(IRenderSystem::E_DM_SCREEN_QUAD);
	NIASSERT(spScreenMesh);

	NiPropertyState* spPropState = spScreenMesh->GetPropertyState();
	NIASSERT(spPropState);
	spPropState->SwapProperties(m_kCopyToBackBufferPropertyList);
	spScreenMesh->ApplyAndSetActiveMaterial(m_spCopyToBackBufferMaterial);
	spScreenMesh->RenderImmediate(spRenderer);
	spScreenMesh->SetActiveMaterial(NULL);
	spPropState->SwapProperties(m_kCopyToBackBufferPropertyList);
	// @}

	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
void sdFinalPass::SetCompositeInputtexture_ColorTexture(NiTexture* spTexture)
{
	if (m_spCompositeTexProp->GetShaderMap(0)->GetTexture() != spTexture)
	{
		NiTexturingProperty::ShaderMap* spShaderMap = m_spCompositeTexProp->GetShaderMap(0);
		NIASSERT(spShaderMap);
		spShaderMap->SetTexture(spTexture);
	}
}
//-------------------------------------------------------------------------------------------------
void sdFinalPass::SetCompositeInputtexture_WeightTexture(NiTexture* spTexture)
{
	if (m_spCompositeTexProp->GetShaderMap(1)->GetTexture() != spTexture)
	{
		NiTexturingProperty::ShaderMap* spShaderMap = m_spCompositeTexProp->GetShaderMap(1);
		NIASSERT(spShaderMap);
		spShaderMap->SetTexture(spTexture);
	}
}
//-------------------------------------------------------------------------------------------------
void sdFinalPass::SetCompositeInputtexture_DofTexture(NiTexture* spTexture)
{
	if (m_spCompositeTexProp->GetShaderMap(2)->GetTexture() != spTexture)
	{
		NiTexturingProperty::ShaderMap* spShaderMap = m_spCompositeTexProp->GetShaderMap(2);
		NIASSERT(spShaderMap);
		spShaderMap->SetTexture(spTexture);
	}
}
//-------------------------------------------------------------------------------------------------
void sdFinalPass::SetCompositeInputtexture_GodRayTexture(NiTexture* spTexture)
{
	if (m_spCompositeTexProp->GetShaderMap(3)->GetTexture() != spTexture)
	{
		NiTexturingProperty::ShaderMap* spShaderMap = m_spCompositeTexProp->GetShaderMap(3);
		NIASSERT(spShaderMap);
		spShaderMap->SetTexture(spTexture);
	}
}
//-------------------------------------------------------------------------------------------------
void sdFinalPass::SetCompositeInputtexture_BloomTexture(NiTexture* spTexture)
{
	if (m_spCompositeTexProp->GetShaderMap(4)->GetTexture() != spTexture)
	{
		NiTexturingProperty::ShaderMap* spShaderMap = m_spCompositeTexProp->GetShaderMap(4);
		NIASSERT(spShaderMap);
		spShaderMap->SetTexture(spTexture);
	}
}
//-------------------------------------------------------------------------------------------------
void sdFinalPass::Composite()
{
	D3DPERF_BeginEvent(0xff000000, L"PostEffect_Composite");

	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	IRenderSystem* pkRenderSystem = IRenderSystem::GetRenderSystem();
	NIASSERT(pkRenderSystem);

	NiRenderer* spRenderer = NiRenderer::GetRenderer();
	NIASSERT(spRenderer);

	// ÉèÖÃäÖÈ¾×´Ì¬
	// @{
	pkRenderDevice->SetRenderState(D3DRS_CULLMODE,			D3DCULL_NONE);
	pkRenderDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,	false);
	pkRenderDevice->SetRenderState(D3DRS_ALPHATESTENABLE,	false);
	pkRenderDevice->SetRenderState(D3DRS_ZENABLE,			false);
	pkRenderDevice->SetRenderState(D3DRS_ZWRITEENABLE,		false);
	pkRenderDevice->SetRenderState(D3DRS_STENCILENABLE,		false);
	// @}

	// »æÖÆ
	// @{
	NiMesh* spScreenMesh = pkRenderSystem->GetDefaultMesh(IRenderSystem::E_DM_SCREEN_QUAD);
	NIASSERT(spScreenMesh);

	NiPropertyState* spPropState = spScreenMesh->GetPropertyState();
	NIASSERT(spPropState);
	spPropState->SwapProperties(m_kCompositeList);
	spScreenMesh->ApplyAndSetActiveMaterial(m_spCompositeMaterial);
	spScreenMesh->RenderImmediate(spRenderer);
	spScreenMesh->SetActiveMaterial(NULL);
	spPropState->SwapProperties(m_kCompositeList);
	// @}

	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE