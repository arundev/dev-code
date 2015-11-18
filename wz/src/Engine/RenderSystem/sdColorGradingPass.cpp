#include "sdRenderSystemPCH.h"
#include "sdColorGradingPass.h"

using namespace Engine::ResourceSystem;

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
sdColorGradingPass::sdColorGradingPass(sdRenderObjectAlloc* pkAlloc)
	: sdRenderPass(pkAlloc)
{

}
//-------------------------------------------------------------------------------------------------
sdColorGradingPass::~sdColorGradingPass()
{
	if (m_bInitialized)
	{
		Destroy();
		m_bInitialized = false;
	}
}
//-------------------------------------------------------------------------------------------------
bool sdColorGradingPass::Initialize()
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

	// 设置输入纹理
	m_spBackgroundLUTTexture = pkResourceSystem->LoadTexture("dependence\\engine\\texture\\postprocess\\default_lut.png");
	NIASSERT(m_spBackgroundLUTTexture);

	m_spForegroundLUTTexture = m_spBackgroundLUTTexture;
	NIASSERT(m_spForegroundLUTTexture);

	// 初始化材质
	m_spColorGradingMaterial = pkRenderDevice->CreateMaterial("PostEffect_ColorGrading");
	NIASSERT(m_spColorGradingMaterial);

	// 初始化渲染属性
	m_spTexturingProp  = NiNew NiTexturingProperty;
	NIASSERT(m_spTexturingProp);
	m_spTexturingProp->SetShaderMap(0, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_spTexturingProp->SetShaderMap(1, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_spTexturingProp->SetShaderMap(2, NiNew NiTexturingProperty::ShaderMap(m_spForegroundLUTTexture, 0));
	m_spTexturingProp->SetShaderMap(3, NiNew NiTexturingProperty::ShaderMap(m_spBackgroundLUTTexture, 0));
	m_kPropertyList.AddTail((NiTexturingProperty*)m_spTexturingProp);

	return (m_bInitialized = true);
}
//-------------------------------------------------------------------------------------------------
void sdColorGradingPass::Destroy()
{
	m_spColorGradingMaterial = 0;

	m_kPropertyList.RemoveAll();
	m_spTexturingProp = 0;

	m_spSceneTexture = 0;
	m_spGeomTexture = 0;
	m_spBackgroundLUTTexture = 0;
	m_spForegroundLUTTexture = 0;

	m_bInitialized = false;
}
//-------------------------------------------------------------------------------------------------
void sdColorGradingPass::Draw()
{
	D3DPERF_BeginEvent(0xff000000, L"PostEffect_ColorGradingPass");

	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	IRenderSystem* pkRenderSystem = IRenderSystem::GetRenderSystem();
	NIASSERT(pkRenderSystem);

	NiRenderer* spRenderer = NiRenderer::GetRenderer();
	NIASSERT(spRenderer);

	// 清除数据绑定
	pkRenderDevice->ClearVertexBinding();
	//pkRenderDevice->ClearTextureBinding();

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
	spScreenMesh->ApplyAndSetActiveMaterial(m_spColorGradingMaterial);

	spScreenMesh->RenderImmediate(spRenderer);

	spScreenMesh->SetActiveMaterial(NULL);
	spPropState->SwapProperties(m_kPropertyList);
	// @}

	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
void sdColorGradingPass::SetInputSceneTexture(NiTexture* spTexture) 
{ 
	if (m_spSceneTexture != spTexture)
	{
		m_spSceneTexture = spTexture;

		NiTexturingProperty::ShaderMap* spShaderMap = m_spTexturingProp->GetShaderMap(0);
		NIASSERT(spShaderMap);
		spShaderMap->SetTexture(spTexture);
	}
}
//-------------------------------------------------------------------------------------------------
void sdColorGradingPass::SetInputGeomTexture(NiTexture* spTexture) 
{ 
	if (m_spGeomTexture != spTexture)
	{
		m_spGeomTexture = spTexture;

		NiTexturingProperty::ShaderMap* spShaderMap = m_spTexturingProp->GetShaderMap(1);
		NIASSERT(spShaderMap);
		spShaderMap->SetTexture(spTexture);
	}
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE