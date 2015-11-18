#include "sdRenderSystemPCH.h"
#include "sdPreLightPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
sdPreLightPass::sdPreLightPass(sdRenderObjectAlloc* pkAlloc)
: sdRenderPass(pkAlloc)
{

}
//-------------------------------------------------------------------------------------------------
sdPreLightPass::~sdPreLightPass()
{
	if (m_bInitialized)
	{
		Destroy();
		m_bInitialized = false;
	}
}
//-------------------------------------------------------------------------------------------------
bool sdPreLightPass::Initialize(uint uiLightVolumeStencilID, uint uiLightVolumeStencilIDMask,
	uint uiLightVolumeStencilIDWriteMask, NiRenderedTexture* spGeometryTexture)
{
	NIASSERT(uiLightVolumeStencilID & uiLightVolumeStencilIDMask);
	NIASSERT(spGeometryTexture);

	if (m_bInitialized)
	{
		NIASSERT(0);
		return false;
	}

	// 设置模版参数
	m_uiLightVolumeStencilID = uiLightVolumeStencilID;
	m_uiLightVolumeStencilIDMask = uiLightVolumeStencilIDMask;
	m_uiLightVolumeStencilIDWriteMask = uiLightVolumeStencilIDWriteMask;

	// 初始化材质
	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	m_spPointLightMaterial = pkRenderDevice->CreateMaterial("Point_Lightting");
	NIASSERT(m_spPointLightMaterial);

	m_spSpotLightMaterial = pkRenderDevice->CreateMaterial("Spot_Lightting");
	NIASSERT(m_spSpotLightMaterial);

	m_spRectSpotLightMaterial = pkRenderDevice->CreateMaterial("RectSpot_Lightting");
	NIASSERT(m_spRectSpotLightMaterial);

	// 初始化纹理
	m_spGeometryTexture = spGeometryTexture;

	// 初始化属性
	m_spTexturingProp  = NiNew NiTexturingProperty;
	NIASSERT(m_spTexturingProp);
	m_spTexturingProp->SetShaderMap(0, NiNew NiTexturingProperty::ShaderMap(m_spGeometryTexture, 0));
	m_kPropertyList.AddTail((NiTexturingProperty*)m_spTexturingProp);

	return (m_bInitialized = true);
}
//-------------------------------------------------------------------------------------------------
void sdPreLightPass::Destroy()
{
	m_uiLightVolumeStencilID = 0;
	m_uiLightVolumeStencilIDMask = 0;
	m_uiLightVolumeStencilIDWriteMask = 0;

	m_spPointLightMaterial = 0;
	m_spSpotLightMaterial = 0;
	m_spRectSpotLightMaterial = 0;

	m_spGeometryTexture = 0;

	m_spTexturingProp = 0;
	m_kPropertyList.RemoveAll();

	m_bInitialized = false;
}
//-------------------------------------------------------------------------------------------------
void sdPreLightPass::InsertLight(sdLightSceneObject* pkLight)
{
	NIASSERT(m_bInitialized);
	NIASSERT(pkLight);

	if (pkLight->GetLightType() == sdLightSceneObject::E_LT_POINT)
		InsertLightSceneObject(pkLight, m_spPointLightMaterial, &m_kPropertyList, NULL);
	else if (pkLight->GetLightType() == sdLightSceneObject::E_LT_SPOT)
		InsertLightSceneObject(pkLight, m_spSpotLightMaterial, &m_kPropertyList, NULL);
	else if (pkLight->GetLightType() == sdLightSceneObject::E_LT_RECT_SPOT)
		InsertLightSceneObject(pkLight, m_spRectSpotLightMaterial, &m_kPropertyList, NULL);
}
//-------------------------------------------------------------------------------------------------
void sdPreLightPass::Draw()
{
	D3DPERF_BeginEvent(0xff000000, L"PreLightPass");

	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	IRenderSystem* pkRenderSystem = IRenderSystem::GetRenderSystem();
	NIASSERT(pkRenderSystem);

	// 清除数据绑定
	pkRenderDevice->ClearVertexBinding();
	pkRenderDevice->ClearTextureBinding();

//	// 设置渲染状态
//	pkRenderDevice->SetRenderState(D3DRS_STENCILREF,		m_uiLightVolumeStencilID);
//	pkRenderDevice->SetRenderState(D3DRS_STENCILMASK,		m_uiLightVolumeStencilIDMask);
//	pkRenderDevice->SetRenderState(D3DRS_STENCILWRITEMASK,	m_uiLightVolumeStencilIDWriteMask);

	// 渲染
	sdRenderPass::Draw();

	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE