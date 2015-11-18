#include "sdRenderSystemPCH.h"
#include "sdMLAAPass.h"

using namespace Engine::ResourceSystem;

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
sdMLAAPass::sdMLAAPass(sdRenderObjectAlloc* pkAlloc)
: sdRenderPass(pkAlloc)
{
	
}
//-------------------------------------------------------------------------------------------------
sdMLAAPass::~sdMLAAPass()
{
	if (m_bInitialized)
	{
		Destroy();
		m_bInitialized = false;
	}
}
//-------------------------------------------------------------------------------------------------
bool sdMLAAPass::Initialize()
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
	m_spAreaTexture = pkResourceSystem->LoadTextureByD3D("dependence\\engine\\texture\\postprocess\\areamap9.dds");
	NIASSERT(m_spAreaTexture);

	// ³õÊ¼»¯²ÄÖÊ
	m_spColorEdgeDetectMaterial = pkRenderDevice->CreateMaterial("ColorEdgeDetection");
	NIASSERT(m_spColorEdgeDetectMaterial);

	m_spDepthEdgeDetectMaterial = pkRenderDevice->CreateMaterial("DepthEdgeDetection");
	NIASSERT(m_spDepthEdgeDetectMaterial);

	m_spWeightCalcMaterial = pkRenderDevice->CreateMaterial("BlendWeightCalculation");
	NIASSERT(m_spWeightCalcMaterial);

	m_spNeighborBlendMaterial = pkRenderDevice->CreateMaterial("NeighborhoodBlending");
	NIASSERT(m_spNeighborBlendMaterial);

	m_spPostWeightCalculationMaterial = pkRenderDevice->CreateMaterial("PostEffect_PostWeightCalculation");
	NIASSERT(m_spPostWeightCalculationMaterial);

	m_spPostWeightBlendingMaterial = pkRenderDevice->CreateMaterial("PostEffect_PostWeightBlending");
	NIASSERT(m_spPostWeightBlendingMaterial);

	// ³õÊ¼»¯äÖÈ¾ÊôÐÔ
	m_spColorEdgeDetectTexProp = NiNew NiTexturingProperty();
	NIASSERT(m_spColorEdgeDetectTexProp);
	m_spColorEdgeDetectTexProp->SetShaderMap(0, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_kColorEdgeDetectPropertyList.AddTail((NiTexturingProperty*)m_spColorEdgeDetectTexProp);

	m_spDepthEdgeDetectTexProp = NiNew NiTexturingProperty();
	NIASSERT(m_spDepthEdgeDetectTexProp);
	m_spDepthEdgeDetectTexProp->SetShaderMap(0, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_kDepthEdgeDetectPropertyList.AddTail((NiTexturingProperty*)m_spDepthEdgeDetectTexProp);

	m_spWeightCalcTexProp = NiNew NiTexturingProperty();
	NIASSERT(m_spWeightCalcTexProp);
	m_spWeightCalcTexProp->SetShaderMap(0, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_spWeightCalcTexProp->SetShaderMap(1, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_spWeightCalcTexProp->SetShaderMap(2, NiNew NiTexturingProperty::ShaderMap(m_spAreaTexture, 0));
	m_kWeightCalcPropertyList.AddTail((NiTexturingProperty*)m_spWeightCalcTexProp);

	m_spNeighborBlendTexProp = NiNew NiTexturingProperty();
	NIASSERT(m_spNeighborBlendTexProp);
	m_spNeighborBlendTexProp->SetShaderMap(0, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_spNeighborBlendTexProp->SetShaderMap(1, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_kNeighborBlendPropertyList.AddTail((NiTexturingProperty*)m_spNeighborBlendTexProp);

	m_spPostWeightCalculationTexProp = NiNew NiTexturingProperty();
	NIASSERT(m_spPostWeightCalculationTexProp);
	m_spPostWeightCalculationTexProp->SetShaderMap(0, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_kPostWeightCalculationPropertyList.AddTail((NiTexturingProperty*)m_spPostWeightCalculationTexProp);

	m_spPostWeightBlendingTexProp = NiNew NiTexturingProperty();
	NIASSERT(m_spPostWeightBlendingTexProp);
	m_spPostWeightBlendingTexProp->SetShaderMap(0, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_spPostWeightBlendingTexProp->SetShaderMap(1, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_kPostWeightBlendingPropertyList.AddTail((NiTexturingProperty*)m_spPostWeightBlendingTexProp);	

	return (m_bInitialized = true);
}
//-------------------------------------------------------------------------------------------------
void sdMLAAPass::Destroy()
{
	m_spColorEdgeDetectMaterial = 0;
	m_spDepthEdgeDetectMaterial = 0;
	m_spWeightCalcMaterial = 0;
	m_spNeighborBlendMaterial = 0;
	m_spPostWeightCalculationMaterial = 0;
	m_spPostWeightBlendingMaterial = 0;

	m_kColorEdgeDetectPropertyList.RemoveAll();
	m_kDepthEdgeDetectPropertyList.RemoveAll();
	m_kWeightCalcPropertyList.RemoveAll();
	m_kNeighborBlendPropertyList.RemoveAll();
	m_kPostWeightCalculationPropertyList.RemoveAll();
	m_kPostWeightBlendingPropertyList.RemoveAll();

	m_spColorEdgeDetectTexProp = 0;
	m_spDepthEdgeDetectTexProp = 0;
	m_spWeightCalcTexProp = 0;
	m_spNeighborBlendTexProp = 0;
	m_spPostWeightCalculationTexProp = 0;
	m_spPostWeightBlendingTexProp = 0;

	m_spAreaTexture = 0;
}
//-------------------------------------------------------------------------------------------------
void sdMLAAPass::SetColorEdgeDetectInputTexture(NiTexture* spColorTexture)
{
	if (m_spColorEdgeDetectTexProp->GetShaderMap(0)->GetTexture() != spColorTexture)
	{
		NiTexturingProperty::ShaderMap* spShaderMap = m_spColorEdgeDetectTexProp->GetShaderMap(0);
		NIASSERT(spShaderMap);
		spShaderMap->SetTexture(spColorTexture);
	}
}
//-------------------------------------------------------------------------------------------------
void sdMLAAPass::ColorEdgeDetect()
{
	D3DPERF_BeginEvent(0xff000000, L"MLAAPass_ColorEdgeDetect");

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

	pkRenderDevice->SetRenderState(D3DRS_STENCILENABLE,		true);
	// @}

	// »æÖÆ
	// @{
	NiMesh* spScreenMesh = pkRenderSystem->GetDefaultMesh(IRenderSystem::E_DM_SCREEN_QUAD);
	NIASSERT(spScreenMesh);

	NiPropertyState* spPropState = spScreenMesh->GetPropertyState();
	NIASSERT(spPropState);
	spPropState->SwapProperties(m_kColorEdgeDetectPropertyList);
	spScreenMesh->ApplyAndSetActiveMaterial(m_spColorEdgeDetectMaterial);
	spScreenMesh->RenderImmediate(spRenderer);
	spScreenMesh->SetActiveMaterial(NULL);
	spPropState->SwapProperties(m_kColorEdgeDetectPropertyList);
	// @}

	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
void sdMLAAPass::SetDepthEdgeDetectInputTexture(NiTexture* spDepthTexture)
{
	if (m_spColorEdgeDetectTexProp->GetShaderMap(0)->GetTexture() != spDepthTexture)
	{
		NiTexturingProperty::ShaderMap* spShaderMap = m_spDepthEdgeDetectTexProp->GetShaderMap(0);
		NIASSERT(spShaderMap);
		spShaderMap->SetTexture(spDepthTexture);
	}
}
//-------------------------------------------------------------------------------------------------
void sdMLAAPass::DepthEdgeDetect()
{
	D3DPERF_BeginEvent(0xff000000, L"MLAAPass_DepthEdgeDetect");

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

	pkRenderDevice->SetRenderState(D3DRS_STENCILENABLE,		true);
	// @}

	// »æÖÆ
	// @{
	NiMesh* spScreenMesh = pkRenderSystem->GetDefaultMesh(IRenderSystem::E_DM_SCREEN_QUAD);
	NIASSERT(spScreenMesh);

	NiPropertyState* spPropState = spScreenMesh->GetPropertyState();
	NIASSERT(spPropState);
	spPropState->SwapProperties(m_kDepthEdgeDetectPropertyList);
	spScreenMesh->ApplyAndSetActiveMaterial(m_spDepthEdgeDetectMaterial);
	spScreenMesh->RenderImmediate(spRenderer);
	spScreenMesh->SetActiveMaterial(NULL);
	spPropState->SwapProperties(m_kDepthEdgeDetectPropertyList);
	// @}

	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
void sdMLAAPass::SetBlendWeightCalcInputTexture(NiTexture* spTexture)
{
	if (m_spWeightCalcTexProp->GetShaderMap(0)->GetTexture() != spTexture)
	{
		NiTexturingProperty::ShaderMap* spShaderMap = m_spWeightCalcTexProp->GetShaderMap(0);
		NIASSERT(spShaderMap);
		spShaderMap->SetTexture(spTexture);
	}

	if (m_spWeightCalcTexProp->GetShaderMap(1)->GetTexture() != spTexture)
	{
		NiTexturingProperty::ShaderMap* spShaderMap = m_spWeightCalcTexProp->GetShaderMap(1);
		NIASSERT(spShaderMap);
		spShaderMap->SetTexture(spTexture);
	}
}
//-------------------------------------------------------------------------------------------------
void sdMLAAPass::BlendWeightCalc()
{
	D3DPERF_BeginEvent(0xff000000, L"MLAAPass_BlendWeightCalc");

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

	pkRenderDevice->SetRenderState(D3DRS_STENCILENABLE,		true);
	// @}

	// »æÖÆ
	// @{
	NiMesh* spScreenMesh = pkRenderSystem->GetDefaultMesh(IRenderSystem::E_DM_SCREEN_QUAD);
	NIASSERT(spScreenMesh);

	NiPropertyState* spPropState = spScreenMesh->GetPropertyState();
	NIASSERT(spPropState);
	spPropState->SwapProperties(m_kWeightCalcPropertyList);
	spScreenMesh->ApplyAndSetActiveMaterial(m_spWeightCalcMaterial);
	spScreenMesh->RenderImmediate(spRenderer);
	spScreenMesh->SetActiveMaterial(NULL);
	spPropState->SwapProperties(m_kWeightCalcPropertyList);
	// @}

	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
void sdMLAAPass::SetNeighborhoodBlendInputTexture(NiTexture* spColorTexture, NiTexture* spBlendTexture)
{
	if (m_spNeighborBlendTexProp->GetShaderMap(0)->GetTexture() != spColorTexture)
	{
		NiTexturingProperty::ShaderMap* spShaderMap = m_spNeighborBlendTexProp->GetShaderMap(0);
		NIASSERT(spShaderMap);
		spShaderMap->SetTexture(spColorTexture);
	}

	if (m_spNeighborBlendTexProp->GetShaderMap(1)->GetTexture() != spBlendTexture)
	{
		NiTexturingProperty::ShaderMap* spShaderMap = m_spNeighborBlendTexProp->GetShaderMap(1);
		NIASSERT(spShaderMap);
		spShaderMap->SetTexture(spBlendTexture);
	}
}
//-------------------------------------------------------------------------------------------------
void sdMLAAPass::NeighborhoodBlend()
{
	D3DPERF_BeginEvent(0xff000000, L"MLAAPass_NeighborhoodBlend");

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

	pkRenderDevice->SetRenderState(D3DRS_STENCILENABLE,		true);
	// @}

	// »æÖÆ
	// @{
	NiMesh* spScreenMesh = pkRenderSystem->GetDefaultMesh(IRenderSystem::E_DM_SCREEN_QUAD);
	NIASSERT(spScreenMesh);

	NiPropertyState* spPropState = spScreenMesh->GetPropertyState();
	NIASSERT(spPropState);
	spPropState->SwapProperties(m_kNeighborBlendPropertyList);
	spScreenMesh->ApplyAndSetActiveMaterial(m_spNeighborBlendMaterial);
	spScreenMesh->RenderImmediate(spRenderer);
	spScreenMesh->SetActiveMaterial(NULL);
	spPropState->SwapProperties(m_kNeighborBlendPropertyList);
	// @}

	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
void sdMLAAPass::SetPostWeightCalculationInputTexture(NiTexture* spGeometryTexture)
{
	if (m_spPostWeightCalculationTexProp->GetShaderMap(0)->GetTexture() != spGeometryTexture)
	{
		NiTexturingProperty::ShaderMap* spShaderMap = m_spPostWeightCalculationTexProp->GetShaderMap(0);
		NIASSERT(spShaderMap);
		spShaderMap->SetTexture(spGeometryTexture);
	}
}
//-------------------------------------------------------------------------------------------------
void sdMLAAPass::PostWeightCalculation()
{
	D3DPERF_BeginEvent(0xff000000, L"MLAAPass_PostWeightCalc");

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
	spPropState->SwapProperties(m_kPostWeightCalculationPropertyList);
	spScreenMesh->ApplyAndSetActiveMaterial(m_spPostWeightCalculationMaterial);
	spScreenMesh->RenderImmediate(spRenderer);
	spScreenMesh->SetActiveMaterial(NULL);
	spPropState->SwapProperties(m_kPostWeightCalculationPropertyList);
	// @}

	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
//void sdMLAAPass::SetPostWeightBlendInputTexture(NiTexture* spColorTexture, NiTexture* spBlendTexture)
//{
//	if (m_spPostWeightBlendingTexProp->GetShaderMap(0)->GetTexture() != spColorTexture)
//	{
//		NiTexturingProperty::ShaderMap* spShaderMap = m_spPostWeightBlendingTexProp->GetShaderMap(0);
//		NIASSERT(spShaderMap);
//		spShaderMap->SetTexture(spColorTexture);
//	}
//
//	if (m_spPostWeightBlendingTexProp->GetShaderMap(1)->GetTexture() != spBlendTexture)
//	{
//		NiTexturingProperty::ShaderMap* spShaderMap = m_spPostWeightBlendingTexProp->GetShaderMap(1);
//		NIASSERT(spShaderMap);
//		spShaderMap->SetTexture(spBlendTexture);
//	}
//}
////-------------------------------------------------------------------------------------------------
//void sdMLAAPass::PostWeightBlend()
//{
//	D3DPERF_BeginEvent(0xff000000, L"MLAAPass_PostWeightBlend");
//
//	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
//	NIASSERT(pkRenderDevice);
//
//	IRenderSystem* pkRenderSystem = IRenderSystem::GetRenderSystem;
//	NIASSERT(pkRenderSystem);
//
//	NiRenderer* spRenderer = NiRenderer::GetRenderer();
//	NIASSERT(spRenderer);
//
//	// ÉèÖÃäÖÈ¾×´Ì¬
//	// @{
//	pkRenderDevice->SetRenderState(D3DRS_CULLMODE,			D3DCULL_NONE);
//
//	pkRenderDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,	false);
//	pkRenderDevice->SetRenderState(D3DRS_ALPHATESTENABLE,	false);
//
//	pkRenderDevice->SetRenderState(D3DRS_ZENABLE,			false);
//	pkRenderDevice->SetRenderState(D3DRS_ZWRITEENABLE,		false);
//
//	pkRenderDevice->SetRenderState(D3DRS_STENCILENABLE,		true);
//	// @}
//
//	// »æÖÆ
//	// @{
//	NiMesh* spScreenMesh = pkRenderSystem->GetDefaultMesh(IRenderSystem::E_DM_SCREEN_QUAD);
//	NIASSERT(spScreenMesh);
//
//	NiPropertyState* spPropState = spScreenMesh->GetPropertyState();
//	NIASSERT(spPropState);
//	spPropState->SwapProperties(m_kPostWeightBlendingPropertyList);
//	spScreenMesh->ApplyAndSetActiveMaterial(m_spPostWeightBlendingMaterial);
//	spScreenMesh->RenderImmediate(spRenderer);
//	spScreenMesh->SetActiveMaterial(NULL);
//	spPropState->SwapProperties(m_kPostWeightBlendingPropertyList);
//	// @}
//
//	D3DPERF_EndEvent();
//}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE