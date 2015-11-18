#include "sdRenderSystemPCH.h"
#include "sdHDRPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
sdHDRPass::sdHDRPass(sdRenderObjectAlloc* pkAlloc)
	: sdRenderPass(pkAlloc), m_pkPostProcessParams(NULL)
{

}
//-------------------------------------------------------------------------------------------------
sdHDRPass::~sdHDRPass()
{
	if (m_bInitialized)
	{
		Destroy();
		m_bInitialized = false;
	}
}
//-------------------------------------------------------------------------------------------------
bool sdHDRPass::Initialize(sdPostProcessParams& kParams)
{
	if (m_bInitialized)
	{
		NIASSERT(0);
		return false;
	}

	//
	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	//
	m_pkPostProcessParams = &kParams;

	// 初始化材质
	// @{
	m_spDownScaleMaterial = pkRenderDevice->CreateMaterial("PostEffect_HDR_DownScale");
	NIASSERT(m_spDownScaleMaterial);

	m_spSceneLuminanceMaterial = pkRenderDevice->CreateMaterial("PostEffect_HDR_SceneLuminance");
	NIASSERT(m_spSceneLuminanceMaterial);

	m_spDownScaleLuminanceMaterial = pkRenderDevice->CreateMaterial("PostEffect_HDR_DownScaleLuminance");
	NIASSERT(m_spDownScaleLuminanceMaterial);

	m_spFinalDownScaleLuminanceMaterial = pkRenderDevice->CreateMaterial("PostEffect_HDR_FinalDownScaleLuminance");
	NIASSERT(m_spFinalDownScaleLuminanceMaterial);

	m_spInitLuminanceMaterial = pkRenderDevice->CreateMaterial("PostEffect_HDR_InitLuminance");
	NIASSERT(m_spInitLuminanceMaterial);

	m_spAdaptedLuminanceMaterial = pkRenderDevice->CreateMaterial("PostEffect_HDR_AdaptedLuminance");
	NIASSERT(m_spAdaptedLuminanceMaterial);

	m_spBloomThresholdMaterial = pkRenderDevice->CreateMaterial("PostEffect_HDR_BloomThreshold");
	NIASSERT(m_spBloomThresholdMaterial);

	m_spSeparableBlurMaterial = pkRenderDevice->CreateMaterial("PostEffect_HDR_SeparableBlur");
	NIASSERT(m_spSeparableBlurMaterial);

	m_spToneMapMaterial = pkRenderDevice->CreateMaterial("PostEffect_HDR_ToneMap");
	NIASSERT(m_spToneMapMaterial);
	// @}

	// 初始化渲染属性
	// @{
	m_spDownScaleTexProp = NiNew NiTexturingProperty;
	NIASSERT(m_spDownScaleTexProp);
	m_spDownScaleTexProp->SetShaderMap(0, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_kDownScalePropertyList.AddTail((NiTexturingProperty*)m_spDownScaleTexProp);

	m_spSceneLuminanceTexProp = NiNew NiTexturingProperty;
	NIASSERT(m_spSceneLuminanceTexProp);
	m_spSceneLuminanceTexProp->SetShaderMap(0, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_kSceneLuminancePropertyList.AddTail((NiTexturingProperty*)m_spSceneLuminanceTexProp);

	m_spDownScaleLuminanceTexProp = NiNew NiTexturingProperty;
	NIASSERT(m_spDownScaleLuminanceTexProp);
	m_spDownScaleLuminanceTexProp->SetShaderMap(0, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_kDownScaleLuminancePropertyList.AddTail((NiTexturingProperty*)m_spDownScaleLuminanceTexProp);

	m_spFinalDownScaleLuminanceTexProp = NiNew NiTexturingProperty;
	NIASSERT(m_spFinalDownScaleLuminanceTexProp);
	m_spFinalDownScaleLuminanceTexProp->SetShaderMap(0, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_kFinalDownScaleLuminancePropertyList.AddTail((NiTexturingProperty*)m_spFinalDownScaleLuminanceTexProp);

	m_spInitLuminanceTexProp = NiNew NiTexturingProperty;
	NIASSERT(m_spInitLuminanceTexProp);
	m_spInitLuminanceTexProp->SetShaderMap(2, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_kInitLuminancePropertyList.AddTail((NiTexturingProperty*)m_spInitLuminanceTexProp);

	m_spAdaptedLuminanceTexProp = NiNew NiTexturingProperty;
	NIASSERT(m_spAdaptedLuminanceTexProp);
	m_spAdaptedLuminanceTexProp->SetShaderMap(2, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_spAdaptedLuminanceTexProp->SetShaderMap(4, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_kAdaptedLuminancePropertyList.AddTail((NiTexturingProperty*)m_spAdaptedLuminanceTexProp);

	m_spBloomThresholdTexProp = NiNew NiTexturingProperty;
	NIASSERT(m_spBloomThresholdTexProp);
	m_spBloomThresholdTexProp->SetShaderMap(0, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_spBloomThresholdTexProp->SetShaderMap(3, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_kBloomThresholdPropertyList.AddTail((NiTexturingProperty*)m_spBloomThresholdTexProp);

	m_spSeparableBlurTexProp = NiNew NiTexturingProperty;
	NIASSERT(m_spSeparableBlurTexProp);
	m_spSeparableBlurTexProp->SetShaderMap(0, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_kSeparableBlurPropertyList.AddTail((NiTexturingProperty*)m_spSeparableBlurTexProp);

	m_spToneMapTexProp = NiNew NiTexturingProperty;
	NIASSERT(m_spToneMapTexProp);
	m_spToneMapTexProp->SetShaderMap(0, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_spToneMapTexProp->SetShaderMap(1, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_spToneMapTexProp->SetShaderMap(3, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_kToneMapPropertyList.AddTail((NiTexturingProperty*)m_spToneMapTexProp);
	// @}

	return (m_bInitialized = true);
}
//-------------------------------------------------------------------------------------------------
void sdHDRPass::Destroy()
{

}
//-------------------------------------------------------------------------------------------------
void sdHDRPass::SetDownScaleInputTexture(NiTexture* spTexture)
{
	if (m_spDownScaleTexProp->GetShaderMap(0)->GetTexture() != spTexture)
	{
		NiTexturingProperty::ShaderMap* spShaderMap = m_spDownScaleTexProp->GetShaderMap(0);
		NIASSERT(spShaderMap);
		spShaderMap->SetTexture(spTexture);
	}
}
//-------------------------------------------------------------------------------------------------
void sdHDRPass::DownScale()
{
	D3DPERF_BeginEvent(0xff000000, L"PostEffect_HDR_DownScale");

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

	// 设置
	NiRenderTargetGroup* spTargetGroup = pkRenderDevice->GetRenderTargetGroup();
	NIASSERT(spTargetGroup);

	uint uiWidth = spTargetGroup->GetBuffer(0)->GetWidth();
	uint uiHeight = spTargetGroup->GetBuffer(0)->GetHeight();

	NiPoint2 kHalfSizePixelOffset(0.5f / uiWidth, 0.5f / uiHeight);
	pkRenderDevice->SetGlobalShaderConstant("g_vHDRHalfPixelOffset", sizeof(NiPoint2), &kHalfSizePixelOffset);

	// 绘制
	// @{
	NiMesh* spScreenMesh = pkRenderSystem->GetDefaultMesh(IRenderSystem::E_DM_SCREEN_QUAD);
	NIASSERT(spScreenMesh);

	NiPropertyState* spPropState = spScreenMesh->GetPropertyState();
	NIASSERT(spPropState);
	spPropState->SwapProperties(m_kDownScalePropertyList);
	spScreenMesh->ApplyAndSetActiveMaterial(m_spDownScaleMaterial);
	spScreenMesh->RenderImmediate(spRenderer);
	spScreenMesh->SetActiveMaterial(NULL);
	spPropState->SwapProperties(m_kDownScalePropertyList);
	// @}

	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
void sdHDRPass::SetSceneLuminanceInputTexture(NiTexture* spTexture)
{
	if (m_spSceneLuminanceTexProp->GetShaderMap(0)->GetTexture() != spTexture)
	{
		NiTexturingProperty::ShaderMap* spShaderMap = m_spSceneLuminanceTexProp->GetShaderMap(0);
		NIASSERT(spShaderMap);
		spShaderMap->SetTexture(spTexture);
	}
}
//-------------------------------------------------------------------------------------------------
void sdHDRPass::SceneLuminance()
{
	D3DPERF_BeginEvent(0xff000000, L"PostEffect_HDR_SceneLuminance");

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

	// 设置
	NiRenderTargetGroup* spTargetGroup = pkRenderDevice->GetRenderTargetGroup();
	NIASSERT(spTargetGroup);

	uint uiWidth = spTargetGroup->GetBuffer(0)->GetWidth();
	uint uiHeight = spTargetGroup->GetBuffer(0)->GetHeight();

	NiPoint2 kHalfSizePixelOffset(0.5f / uiWidth, 0.5f / uiHeight);
	pkRenderDevice->SetGlobalShaderConstant("g_vHDRHalfPixelOffset", sizeof(NiPoint2), &kHalfSizePixelOffset);

	// 绘制
	// @{
	NiMesh* spScreenMesh = pkRenderSystem->GetDefaultMesh(IRenderSystem::E_DM_SCREEN_QUAD);
	NIASSERT(spScreenMesh);

	NiPropertyState* spPropState = spScreenMesh->GetPropertyState();
	NIASSERT(spPropState);
	spPropState->SwapProperties(m_kSceneLuminancePropertyList);
	spScreenMesh->ApplyAndSetActiveMaterial(m_spSceneLuminanceMaterial);
	spScreenMesh->RenderImmediate(spRenderer);
	spScreenMesh->SetActiveMaterial(NULL);
	spPropState->SwapProperties(m_kSceneLuminancePropertyList);
	// @}

	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
void sdHDRPass::SetDownScaleSceneLuminanceInputTexture(NiTexture* spTexture)
{
	if (m_spDownScaleLuminanceTexProp->GetShaderMap(0)->GetTexture() != spTexture)
	{
		NiTexturingProperty::ShaderMap* spShaderMap = m_spDownScaleLuminanceTexProp->GetShaderMap(0);
		NIASSERT(spShaderMap);
		spShaderMap->SetTexture(spTexture);
	}
}
//-------------------------------------------------------------------------------------------------
void sdHDRPass::DownScaleSceneLuminance()
{
	D3DPERF_BeginEvent(0xff000000, L"PostEffect_HDR_DownScaleSceneLuminance");

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

	// 设置
	NiTexture* spTexture = m_spDownScaleTexProp->GetShaderMap(0)->GetTexture();
	NIASSERT(spTexture);

	NiPoint2 kScreenMapSize((float)spTexture->GetWidth(), (float)spTexture->GetHeight());
	pkRenderDevice->SetGlobalShaderConstant("g_vHDRScreenMapSize", sizeof(NiPoint2), &kScreenMapSize);

	NiRenderTargetGroup* spTargetGroup = pkRenderDevice->GetRenderTargetGroup();
	NIASSERT(spTargetGroup);

	uint uiWidth = spTargetGroup->GetBuffer(0)->GetWidth();
	uint uiHeight = spTargetGroup->GetBuffer(0)->GetHeight();

	NiPoint2 kHalfSizePixelOffset(0.5f / uiWidth, 0.5f / uiHeight);
	pkRenderDevice->SetGlobalShaderConstant("g_vHDRHalfPixelOffset", sizeof(NiPoint2), &kHalfSizePixelOffset);

	// 绘制
	// @{
	NiMesh* spScreenMesh = pkRenderSystem->GetDefaultMesh(IRenderSystem::E_DM_SCREEN_QUAD);
	NIASSERT(spScreenMesh);

	NiPropertyState* spPropState = spScreenMesh->GetPropertyState();
	NIASSERT(spPropState);
	spPropState->SwapProperties(m_kDownScaleLuminancePropertyList);
	spScreenMesh->ApplyAndSetActiveMaterial(m_spDownScaleLuminanceMaterial);
	spScreenMesh->RenderImmediate(spRenderer);
	spScreenMesh->SetActiveMaterial(NULL);
	spPropState->SwapProperties(m_kDownScaleLuminancePropertyList);
	// @}

	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
void sdHDRPass::SetFinalDownScaleSceneLuminanceInputTexture(NiTexture* spTexture)
{
	if (m_spFinalDownScaleLuminanceTexProp->GetShaderMap(0)->GetTexture() != spTexture)
	{
		NiTexturingProperty::ShaderMap* spShaderMap = m_spFinalDownScaleLuminanceTexProp->GetShaderMap(0);
		NIASSERT(spShaderMap);
		spShaderMap->SetTexture(spTexture);
	}
}
//-------------------------------------------------------------------------------------------------
void sdHDRPass::FinalDownScaleSceneLuminance()
{
	D3DPERF_BeginEvent(0xff000000, L"PostEffect_HDR_FinalDownScaleSceneLuminance");

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

	// 设置
	NiTexture* spTexture = m_spDownScaleTexProp->GetShaderMap(0)->GetTexture();
	NIASSERT(spTexture);

	NiPoint2 kScreenMapSize((float)spTexture->GetWidth(), (float)spTexture->GetHeight());
	pkRenderDevice->SetGlobalShaderConstant("g_vHDRScreenMapSize", sizeof(NiPoint2), &kScreenMapSize);

	NiRenderTargetGroup* spTargetGroup = pkRenderDevice->GetRenderTargetGroup();
	NIASSERT(spTargetGroup);

	uint uiWidth = spTargetGroup->GetBuffer(0)->GetWidth();
	uint uiHeight = spTargetGroup->GetBuffer(0)->GetHeight();

	NiPoint2 kHalfSizePixelOffset(0.5f / uiWidth, 0.5f / uiHeight);
	pkRenderDevice->SetGlobalShaderConstant("g_vHDRHalfPixelOffset", sizeof(NiPoint2), &kHalfSizePixelOffset);

	// 绘制
	// @{
	NiMesh* spScreenMesh = pkRenderSystem->GetDefaultMesh(IRenderSystem::E_DM_SCREEN_QUAD);
	NIASSERT(spScreenMesh);

	NiPropertyState* spPropState = spScreenMesh->GetPropertyState();
	NIASSERT(spPropState);
	spPropState->SwapProperties(m_kFinalDownScaleLuminancePropertyList);
	spScreenMesh->ApplyAndSetActiveMaterial(m_spFinalDownScaleLuminanceMaterial);
	spScreenMesh->RenderImmediate(spRenderer);
	spScreenMesh->SetActiveMaterial(NULL);
	spPropState->SwapProperties(m_kFinalDownScaleLuminancePropertyList);
	// @}

	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
void sdHDRPass::SetInitLuminanceInputTexture(NiTexture* spTexture)
{
	if (m_spInitLuminanceTexProp->GetShaderMap(2)->GetTexture() != spTexture)
	{
		NiTexturingProperty::ShaderMap* spShaderMap = m_spInitLuminanceTexProp->GetShaderMap(2);
		NIASSERT(spShaderMap);
		spShaderMap->SetTexture(spTexture);
	}
}
//-------------------------------------------------------------------------------------------------
void sdHDRPass::InitLuminance()
{
	D3DPERF_BeginEvent(0xff000000, L"PostEffect_HDR_InitLuminance");

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

	// 设置
	NiRenderTargetGroup* spTargetGroup = pkRenderDevice->GetRenderTargetGroup();
	NIASSERT(spTargetGroup);

	uint uiWidth = spTargetGroup->GetBuffer(0)->GetWidth();
	uint uiHeight = spTargetGroup->GetBuffer(0)->GetHeight();

	NiPoint2 kHalfSizePixelOffset(0.5f / uiWidth, 0.5f / uiHeight);
	pkRenderDevice->SetGlobalShaderConstant("g_vHDRHalfPixelOffset", sizeof(NiPoint2), &kHalfSizePixelOffset);

	// 绘制
	// @{
	NiMesh* spScreenMesh = pkRenderSystem->GetDefaultMesh(IRenderSystem::E_DM_SCREEN_QUAD);
	NIASSERT(spScreenMesh);

	NiPropertyState* spPropState = spScreenMesh->GetPropertyState();
	NIASSERT(spPropState);
	spPropState->SwapProperties(m_kInitLuminancePropertyList);
	spScreenMesh->ApplyAndSetActiveMaterial(m_spInitLuminanceMaterial);
	spScreenMesh->RenderImmediate(spRenderer);
	spScreenMesh->SetActiveMaterial(NULL);
	spPropState->SwapProperties(m_kInitLuminancePropertyList);
	// @}

	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
void sdHDRPass::SetAdaptedLuminanceInputTexture_Cur(NiTexture* spTexture)
{
	if (m_spAdaptedLuminanceTexProp->GetShaderMap(2)->GetTexture() != spTexture)
	{
		NiTexturingProperty::ShaderMap* spShaderMap = m_spAdaptedLuminanceTexProp->GetShaderMap(2);
		NIASSERT(spShaderMap);
		spShaderMap->SetTexture(spTexture);
	}
}
//-------------------------------------------------------------------------------------------------
void sdHDRPass::SetAdaptedLuminanceInputTexture_Last(NiTexture* spTexture)
{
	if (m_spAdaptedLuminanceTexProp->GetShaderMap(4)->GetTexture() != spTexture)
	{
		NiTexturingProperty::ShaderMap* spShaderMap = m_spAdaptedLuminanceTexProp->GetShaderMap(4);
		NIASSERT(spShaderMap);
		spShaderMap->SetTexture(spTexture);
	}
}
//-------------------------------------------------------------------------------------------------
void sdHDRPass::AdaptedLuminance(float fFrameTime)
{
	D3DPERF_BeginEvent(0xff000000, L"PostEffect_HDR_AdaptedLuminance");

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

	// 设置着色器参数
	// @{
	NiRenderTargetGroup* spTargetGroup = pkRenderDevice->GetRenderTargetGroup();
	NIASSERT(spTargetGroup);

	uint uiWidth = spTargetGroup->GetBuffer(0)->GetWidth();
	uint uiHeight = spTargetGroup->GetBuffer(0)->GetHeight();

	NiPoint2 kHalfSizePixelOffset(0.5f / uiWidth, 0.5f / uiHeight);
	pkRenderDevice->SetGlobalShaderConstant("g_vHDRHalfPixelOffset", sizeof(NiPoint2), &kHalfSizePixelOffset);
	
	float fAdaptParam = 1.f - exp(-fFrameTime * m_pkPostProcessParams->hdrAdaptationRate);
	pkRenderDevice->SetGlobalShaderConstant("g_fAdaptedParam", sizeof(float), &fAdaptParam);
	// @}

	// 绘制
	// @{
	NiMesh* spScreenMesh = pkRenderSystem->GetDefaultMesh(IRenderSystem::E_DM_SCREEN_QUAD);
	NIASSERT(spScreenMesh);

	NiPropertyState* spPropState = spScreenMesh->GetPropertyState();
	NIASSERT(spPropState);
	spPropState->SwapProperties(m_kAdaptedLuminancePropertyList);
	spScreenMesh->ApplyAndSetActiveMaterial(m_spAdaptedLuminanceMaterial);
	spScreenMesh->RenderImmediate(spRenderer);
	spScreenMesh->SetActiveMaterial(NULL);
	spPropState->SwapProperties(m_kAdaptedLuminancePropertyList);
	// @}

	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
void sdHDRPass::SetBloomThresholdInputTexture_Color(NiTexture* spTexture)
{
	if (m_spBloomThresholdTexProp->GetShaderMap(0)->GetTexture() != spTexture)
	{
		NiTexturingProperty::ShaderMap* spShaderMap = m_spBloomThresholdTexProp->GetShaderMap(0);
		NIASSERT(spShaderMap);
		spShaderMap->SetTexture(spTexture);
	}
}
//-------------------------------------------------------------------------------------------------
void sdHDRPass::SetBloomThresholdInputTexture_Luminance(NiTexture* spTexture)
{
	if (m_spBloomThresholdTexProp->GetShaderMap(3)->GetTexture() != spTexture)
	{
		NiTexturingProperty::ShaderMap* spShaderMap = m_spBloomThresholdTexProp->GetShaderMap(3);
		NIASSERT(spShaderMap);
		spShaderMap->SetTexture(spTexture);
	}
}
//-------------------------------------------------------------------------------------------------
void sdHDRPass::BloomThreshold()
{
	D3DPERF_BeginEvent(0xff000000, L"PostEffect_HDR_BloomThreshold");

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

	// 设置着色器参数
	// @{
	NiRenderTargetGroup* spTargetGroup = pkRenderDevice->GetRenderTargetGroup();
	NIASSERT(spTargetGroup);

	uint uiWidth = spTargetGroup->GetBuffer(0)->GetWidth();
	uint uiHeight = spTargetGroup->GetBuffer(0)->GetHeight();

	NiPoint2 kHalfSizePixelOffset(0.5f / uiWidth, 0.5f / uiHeight);
	pkRenderDevice->SetGlobalShaderConstant("g_vHDRHalfPixelOffset", sizeof(NiPoint2), &kHalfSizePixelOffset);

	NiPoint4 kBloomParams;
	kBloomParams.m_afPt[0] = m_pkPostProcessParams->hdrMaxLuminance * m_pkPostProcessParams->hdrMaxLuminance;
	kBloomParams.m_afPt[1] = m_pkPostProcessParams->hdrToneMapKey;
	kBloomParams.m_afPt[2] = m_pkPostProcessParams->hdrBloomThreshold;
	kBloomParams.m_afPt[3] = m_pkPostProcessParams->hdrMinAverageLuminance;
	pkRenderDevice->SetGlobalShaderConstant("g_vBloomThresholdParams", sizeof(NiPoint4), &kBloomParams);
	// @}

	// 绘制
	// @{
	NiMesh* spScreenMesh = pkRenderSystem->GetDefaultMesh(IRenderSystem::E_DM_SCREEN_QUAD);
	NIASSERT(spScreenMesh);

	NiPropertyState* spPropState = spScreenMesh->GetPropertyState();
	NIASSERT(spPropState);
	spPropState->SwapProperties(m_kBloomThresholdPropertyList);
	spScreenMesh->ApplyAndSetActiveMaterial(m_spBloomThresholdMaterial);
	spScreenMesh->RenderImmediate(spRenderer);
	spScreenMesh->SetActiveMaterial(NULL);
	spPropState->SwapProperties(m_kBloomThresholdPropertyList);
	// @}

	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
void sdHDRPass::SetBloomBlurXInputTexture(NiTexture* spTexture)
{
	if (m_spSeparableBlurTexProp->GetShaderMap(0)->GetTexture() != spTexture)
	{
		NiTexturingProperty::ShaderMap* spShaderMap = m_spSeparableBlurTexProp->GetShaderMap(0);
		NIASSERT(spShaderMap);
		spShaderMap->SetTexture(spTexture);
	}
}
//-------------------------------------------------------------------------------------------------
void sdHDRPass::BloomBlurX()
{
	D3DPERF_BeginEvent(0xff000000, L"PostEffect_HDR_BloomBlurX");

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

	// 设置着色器参数
	// @{
	NiRenderTargetGroup* spTargetGroup = pkRenderDevice->GetRenderTargetGroup();
	NIASSERT(spTargetGroup);

	uint uiWidth = spTargetGroup->GetBuffer(0)->GetWidth();
	uint uiHeight = spTargetGroup->GetBuffer(0)->GetHeight();

	NiPoint2 kHalfSizePixelOffset(0.5f / uiWidth, 0.5f / uiHeight);
	pkRenderDevice->SetGlobalShaderConstant("g_vHDRHalfPixelOffset", sizeof(NiPoint2), &kHalfSizePixelOffset);
	// @}

	// 设置Guass过滤核
	// @{
	static const int iNumSamples = 9;
	float akTexSampleCoord[iNumSamples * 2];
	float akTexCoordWeight[iNumSamples];

	float fTexUnit = 1.f / uiWidth;

	float fBlurWidth = m_pkPostProcessParams->dofBlurWidth;

	float fTotalWeight = 0.f;
	int iHalfNumSamples = iNumSamples / 2;
	float fTerminateSigma = 3.f;
	for (int v = - iHalfNumSamples; v <= iHalfNumSamples; ++v)
	{
		float fSigma = v * fTerminateSigma / iHalfNumSamples;
		float fCoordinate = fBlurWidth * fSigma * fTexUnit;
		akTexSampleCoord[2 * (iHalfNumSamples + v)]		= fCoordinate;
		akTexSampleCoord[2 * (iHalfNumSamples + v) + 1]	= 0.f;

		float fWeight = 1.f / sqrtf(2.f * NI_PI);
		fWeight *= expf(-(fSigma * fSigma) / 2.f);
		akTexCoordWeight[iHalfNumSamples + v] = fWeight;

		fTotalWeight += fWeight;
	}

	// 归一化权值
	for (int v = - iHalfNumSamples; v <= iHalfNumSamples; ++v)
	{
		akTexCoordWeight[iHalfNumSamples + v] /= fTotalWeight;
	}

	pkRenderDevice->SetGlobalShaderConstant("g_akHDRSamplerOffsets", sizeof(akTexSampleCoord), &akTexSampleCoord[0]);
	pkRenderDevice->SetGlobalShaderConstant("g_akHDRSampleWeights", sizeof(akTexCoordWeight), &akTexCoordWeight[0]);
	// @}

	// 绘制
	// @{
	NiMesh* spScreenMesh = pkRenderSystem->GetDefaultMesh(IRenderSystem::E_DM_SCREEN_QUAD);
	NIASSERT(spScreenMesh);

	NiPropertyState* spPropState = spScreenMesh->GetPropertyState();
	NIASSERT(spPropState);
	spPropState->SwapProperties(m_kSeparableBlurPropertyList);
	spScreenMesh->ApplyAndSetActiveMaterial(m_spSeparableBlurMaterial);
	spScreenMesh->RenderImmediate(spRenderer);
	spScreenMesh->SetActiveMaterial(NULL);
	spPropState->SwapProperties(m_kSeparableBlurPropertyList);
	// @}

	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
void sdHDRPass::SetBloomBlurYInputTexture(NiTexture* spTexture)
{
	if (m_spSeparableBlurTexProp->GetShaderMap(0)->GetTexture() != spTexture)
	{
		NiTexturingProperty::ShaderMap* spShaderMap = m_spSeparableBlurTexProp->GetShaderMap(0);
		NIASSERT(spShaderMap);
		spShaderMap->SetTexture(spTexture);
	}
}
//-------------------------------------------------------------------------------------------------
void sdHDRPass::BloomBlurY()
{
	D3DPERF_BeginEvent(0xff000000, L"PostEffect_HDR_BloomBlurY");

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

	// 设置着色器参数
	// @{
	NiRenderTargetGroup* spTargetGroup = pkRenderDevice->GetRenderTargetGroup();
	NIASSERT(spTargetGroup);

	uint uiWidth = spTargetGroup->GetBuffer(0)->GetWidth();
	uint uiHeight = spTargetGroup->GetBuffer(0)->GetHeight();

	NiPoint2 kHalfSizePixelOffset(0.5f / uiWidth, 0.5f / uiHeight);
	pkRenderDevice->SetGlobalShaderConstant("g_vHDRHalfPixelOffset", sizeof(NiPoint2), &kHalfSizePixelOffset);
	// @}

	// 设置Guass过滤核
	// @{
	static const int iNumSamples = 9;
	float akTexSampleCoord[iNumSamples * 2];
	float akTexCoordWeight[iNumSamples];

	float fTexUnit = 1.f / uiHeight;

	float fBlurWidth = m_pkPostProcessParams->dofBlurWidth;

	float fTotalWeight = 0.f;
	int iHalfNumSamples = iNumSamples / 2;
	float fTerminateSigma = 3.f;
	for (int v = - iHalfNumSamples; v <= iHalfNumSamples; ++v)
	{
		float fSigma = v * fTerminateSigma / iHalfNumSamples;
		float fCoordinate = fBlurWidth * fSigma * fTexUnit;
		akTexSampleCoord[2 * (iHalfNumSamples + v)]		= 0.f;
		akTexSampleCoord[2 * (iHalfNumSamples + v) + 1]	= fCoordinate;

		float fWeight = 1.f / sqrtf(2.f * NI_PI);
		fWeight *= expf(-(fSigma * fSigma) / 2.f);
		akTexCoordWeight[iHalfNumSamples + v] = fWeight;

		fTotalWeight += fWeight;
	}

	// 归一化权值
	for (int v = - iHalfNumSamples; v <= iHalfNumSamples; ++v)
	{
		akTexCoordWeight[iHalfNumSamples + v] /= fTotalWeight;
	}

	pkRenderDevice->SetGlobalShaderConstant("g_akHDRSamplerOffsets", sizeof(akTexSampleCoord), &akTexSampleCoord[0]);
	pkRenderDevice->SetGlobalShaderConstant("g_akHDRSampleWeights", sizeof(akTexCoordWeight), &akTexCoordWeight[0]);
	// @}

	// 绘制
	// @{
	NiMesh* spScreenMesh = pkRenderSystem->GetDefaultMesh(IRenderSystem::E_DM_SCREEN_QUAD);
	NIASSERT(spScreenMesh);

	NiPropertyState* spPropState = spScreenMesh->GetPropertyState();
	NIASSERT(spPropState);
	spPropState->SwapProperties(m_kSeparableBlurPropertyList);
	spScreenMesh->ApplyAndSetActiveMaterial(m_spSeparableBlurMaterial);
	spScreenMesh->RenderImmediate(spRenderer);
	spScreenMesh->SetActiveMaterial(NULL);
	spPropState->SwapProperties(m_kSeparableBlurPropertyList);
	// @}

	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
void sdHDRPass::SetToneMappingInputTexture_Color(NiTexture* spTexture)
{
	if (m_spToneMapTexProp->GetShaderMap(0)->GetTexture() != spTexture)
	{
		NiTexturingProperty::ShaderMap* spShaderMap = m_spToneMapTexProp->GetShaderMap(0);
		NIASSERT(spShaderMap);
		spShaderMap->SetTexture(spTexture);
	}
}
//-------------------------------------------------------------------------------------------------
void sdHDRPass::SetToneMappingInputTexture_Bloom(NiTexture* spTexture)
{
	if (m_spToneMapTexProp->GetShaderMap(1)->GetTexture() != spTexture)
	{
		NiTexturingProperty::ShaderMap* spShaderMap = m_spToneMapTexProp->GetShaderMap(1);
		NIASSERT(spShaderMap);
		spShaderMap->SetTexture(spTexture);
	}
}
//-------------------------------------------------------------------------------------------------
void sdHDRPass::SetToneMappingInputTexture_Luminance(NiTexture* spTexture)
{
	if (m_spToneMapTexProp->GetShaderMap(3)->GetTexture() != spTexture)
	{
		NiTexturingProperty::ShaderMap* spShaderMap = m_spToneMapTexProp->GetShaderMap(3);
		NIASSERT(spShaderMap);
		spShaderMap->SetTexture(spTexture);
	}
}
//-------------------------------------------------------------------------------------------------
void sdHDRPass::ToneMapping()
{
	D3DPERF_BeginEvent(0xff000000, L"PostEffect_HDR_ToneMapping");

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

	// 设置着色器参数
	// @{
	NiRenderTargetGroup* spTargetGroup = pkRenderDevice->GetRenderTargetGroup();
	NIASSERT(spTargetGroup);

	uint uiWidth = spTargetGroup->GetBuffer(0)->GetWidth();
	uint uiHeight = spTargetGroup->GetBuffer(0)->GetHeight();

	NiPoint2 kHalfSizePixelOffset(0.5f / uiWidth, 0.5f / uiHeight);
	pkRenderDevice->SetGlobalShaderConstant("g_vHDRHalfPixelOffset", sizeof(NiPoint2), &kHalfSizePixelOffset);
	
	NiPoint4 kToneMappingParams;
	kToneMappingParams.m_afPt[0] = m_pkPostProcessParams->hdrMaxLuminance * m_pkPostProcessParams->hdrMaxLuminance;
	kToneMappingParams.m_afPt[1] = m_pkPostProcessParams->hdrToneMapKey;
	kToneMappingParams.m_afPt[2] = m_pkPostProcessParams->hdrBloomIntensity;
	kToneMappingParams.m_afPt[3] = m_pkPostProcessParams->hdrMinAverageLuminance;
	pkRenderDevice->SetGlobalShaderConstant("g_vToneMapParams", sizeof(NiPoint4), &kToneMappingParams);
	// @}

	// 绘制
	// @{
	NiMesh* spScreenMesh = pkRenderSystem->GetDefaultMesh(IRenderSystem::E_DM_SCREEN_QUAD);
	NIASSERT(spScreenMesh);

	NiPropertyState* spPropState = spScreenMesh->GetPropertyState();
	NIASSERT(spPropState);
	spPropState->SwapProperties(m_kToneMapPropertyList);
	spScreenMesh->ApplyAndSetActiveMaterial(m_spToneMapMaterial);
	spScreenMesh->RenderImmediate(spRenderer);
	spScreenMesh->SetActiveMaterial(NULL);
	spPropState->SwapProperties(m_kToneMapPropertyList);
	// @}

	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE