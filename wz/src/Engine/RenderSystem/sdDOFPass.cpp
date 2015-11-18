#include "sdRenderSystemPCH.h"
#include "sdDOFPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
sdDOFPass::sdDOFPass(sdRenderObjectAlloc* pkAlloc)
	: sdRenderPass(pkAlloc), m_pkPostProcessParams(NULL)
{

}
//-------------------------------------------------------------------------------------------------
sdDOFPass::~sdDOFPass()
{
	if (m_bInitialized)
	{
		Destroy();
		m_bInitialized = false;
	}
}
//-------------------------------------------------------------------------------------------------
bool sdDOFPass::Initialize(sdPostProcessParams& kParams)
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
	m_spDownSampleMaterial = pkRenderDevice->CreateMaterial("PostEffect_DOF_DownSample");
	NIASSERT(m_spDownSampleMaterial);

	m_spSeparableBlurMaterial = pkRenderDevice->CreateMaterial("PostEffect_DOF_SeparableBlur");
	NIASSERT(m_spSeparableBlurMaterial);

	// 初始化渲染属性
	m_spTexProp  = NiNew NiTexturingProperty;
	NIASSERT(m_spTexProp);
	m_spTexProp->SetShaderMap(0, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_kPropertyList.AddTail((NiTexturingProperty*)m_spTexProp);

	return (m_bInitialized = true);
}
//-------------------------------------------------------------------------------------------------
void sdDOFPass::Destroy()
{
	m_spDownSampleMaterial = 0;
	m_spSeparableBlurMaterial = 0;

	m_kPropertyList.RemoveAll();
	m_spTexProp = 0;

	m_pkPostProcessParams = NULL;

	m_bInitialized = false;
}
//-------------------------------------------------------------------------------------------------
void sdDOFPass::SetDownSampleInputTexture(NiTexture* spTexture)
{
	if (m_spTexProp->GetShaderMap(0)->GetTexture() != spTexture)
	{
		NiTexturingProperty::ShaderMap* spShaderMap = m_spTexProp->GetShaderMap(0);
		NIASSERT(spShaderMap);
		spShaderMap->SetTexture(spTexture);
	}
}
//-------------------------------------------------------------------------------------------------
void sdDOFPass::DownSample()
{
	D3DPERF_BeginEvent(0xff000000, L"PostEffect_DOF_DownSample");

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
	spScreenMesh->ApplyAndSetActiveMaterial(m_spDownSampleMaterial);
	spScreenMesh->RenderImmediate(spRenderer);
	spScreenMesh->SetActiveMaterial(NULL);
	spPropState->SwapProperties(m_kPropertyList);
	// @}

	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
void sdDOFPass::SetBlurXInputTexture(NiTexture* spTexture)
{
	if (m_spTexProp->GetShaderMap(0)->GetTexture() != spTexture)
	{
		NiTexturingProperty::ShaderMap* spShaderMap = m_spTexProp->GetShaderMap(0);
		NIASSERT(spShaderMap);
		spShaderMap->SetTexture(spTexture);
	}
}
//-------------------------------------------------------------------------------------------------
void sdDOFPass::BlurX()
{
	D3DPERF_BeginEvent(0xff000000, L"PostEffect_DOF_BlurX");

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

	// 设置Guass过滤核
	// @{
	static const int iNumSamples = 9;
	float akTexSampleCoord[iNumSamples * 2];
	float akTexCoordWeight[iNumSamples];

	NiTexture* spTexture = m_spTexProp->GetShaderMap(0)->GetTexture();
	float fTexUnit = 1.f / spTexture->GetWidth();

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

	pkRenderDevice->SetGlobalShaderConstant("g_akDOFSamplerOffsets", sizeof(akTexSampleCoord), &akTexSampleCoord[0]);
	pkRenderDevice->SetGlobalShaderConstant("g_akDOFSampleWeights", sizeof(akTexCoordWeight), &akTexCoordWeight[0]);
	// @}

	// 绘制
	// @{
	NiMesh* spScreenMesh = pkRenderSystem->GetDefaultMesh(IRenderSystem::E_DM_SCREEN_QUAD);
	NIASSERT(spScreenMesh);

	NiPropertyState* spPropState = spScreenMesh->GetPropertyState();
	NIASSERT(spPropState);
	spPropState->SwapProperties(m_kPropertyList);
	spScreenMesh->ApplyAndSetActiveMaterial(m_spDownSampleMaterial);
	spScreenMesh->RenderImmediate(spRenderer);
	spScreenMesh->SetActiveMaterial(NULL);
	spPropState->SwapProperties(m_kPropertyList);
	// @}

	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
void sdDOFPass::SetBlurYInputTexture(NiTexture* spTexture)
{
	if (m_spTexProp->GetShaderMap(0)->GetTexture() != spTexture)
	{
		NiTexturingProperty::ShaderMap* spShaderMap = m_spTexProp->GetShaderMap(0);
		NIASSERT(spShaderMap);
		spShaderMap->SetTexture(spTexture);
	}
}
//-------------------------------------------------------------------------------------------------
void sdDOFPass::BlurY()
{
	D3DPERF_BeginEvent(0xff000000, L"PostEffect_DOF_BlurY");

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

	// 设置Guass过滤核
	// @{
	static const int iNumSamples = 9;
	float akTexSampleCoord[iNumSamples * 2];
	float akTexCoordWeight[iNumSamples];

	NiTexture* spTexture = m_spTexProp->GetShaderMap(0)->GetTexture();
	float fTexUnit = 1.f / spTexture->GetHeight();

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

	pkRenderDevice->SetGlobalShaderConstant("g_akDOFSamplerOffsets", sizeof(akTexSampleCoord), &akTexSampleCoord[0]);
	pkRenderDevice->SetGlobalShaderConstant("g_akDOFSampleWeights", sizeof(akTexCoordWeight), &akTexCoordWeight[0]);
	// @}

	// 绘制
	// @{
	NiMesh* spScreenMesh = pkRenderSystem->GetDefaultMesh(IRenderSystem::E_DM_SCREEN_QUAD);
	NIASSERT(spScreenMesh);

	NiPropertyState* spPropState = spScreenMesh->GetPropertyState();
	NIASSERT(spPropState);
	spPropState->SwapProperties(m_kPropertyList);
	spScreenMesh->ApplyAndSetActiveMaterial(m_spDownSampleMaterial);
	spScreenMesh->RenderImmediate(spRenderer);
	spScreenMesh->SetActiveMaterial(NULL);
	spPropState->SwapProperties(m_kPropertyList);
	// @}

	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE