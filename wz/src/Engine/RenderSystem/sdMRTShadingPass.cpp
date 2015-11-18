#include "sdRenderSystemPCH.h"
#include "sdMRTShadingPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
sdMRTShadingPass::sdMRTShadingPass(sdRenderObjectAlloc* pkAlloc)
: sdRenderPass(pkAlloc)
, m_uiStaticMeshStencilID(0)
, m_uiStaticMeshStencilIDMask(0)
{

}
//-------------------------------------------------------------------------------------------------
sdMRTShadingPass::~sdMRTShadingPass()
{
	if (m_bInitialized)
	{
		Destroy();
		m_bInitialized = false;
	}
}
//-------------------------------------------------------------------------------------------------
bool sdMRTShadingPass::Initialize(uint uiStaticMeshStencilID, uint uiStaticMeshStencilIDMask, 
	NiRenderedTexture* pkLightTexture, NiRenderedTexture* pkGeomTexture, 
	NiRenderedTexture* pkAlbedoTexture, NiRenderedTexture* pkGlossTexture,
	NiRenderedTexture* pkGlowTexture)
{
	NIASSERT(uiStaticMeshStencilID);
	NIASSERT(uiStaticMeshStencilIDMask);

	NIASSERT(pkLightTexture);
	NIASSERT(pkGeomTexture);
	NIASSERT(pkAlbedoTexture);
	NIASSERT(pkGlossTexture);
	NIASSERT(pkGlowTexture)

	if (m_bInitialized)
	{
		NIASSERT(0);
		return false;
	}

	//
	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	// 设置模版参数
	m_uiStaticMeshStencilID = uiStaticMeshStencilID;
	m_uiStaticMeshStencilIDMask = uiStaticMeshStencilIDMask;

	// 设置输入纹理
	m_spGeomTexture = pkGeomTexture;
	m_spLightTexture = pkLightTexture;
	m_spAlbedoTexture = pkAlbedoTexture;
	m_spGlossTexture = pkGlossTexture;
	m_spGlowTexture = pkGlowTexture;

	// 初始化材质
	char szMaterialName[64];
	uint uiNum = 8;
	m_kStaticShadingMaterials.resize(uiNum);
	for (uint i = 0; i < uiNum; ++i)
	{
		sprintf(szMaterialName, "MRT3Shading0x%08x", i);
		m_kStaticShadingMaterials[i] = pkRenderDevice->CreateMaterial(szMaterialName);
	}

	// 初始化纹理属性
	m_spTexturingProp  = NiNew NiTexturingProperty;
	NIASSERT(m_spTexturingProp);
	m_spTexturingProp->SetShaderMap(0, NiNew NiTexturingProperty::ShaderMap(m_spLightTexture, 0));
	m_spTexturingProp->SetShaderMap(1, NiNew NiTexturingProperty::ShaderMap(m_spGeomTexture, 0));
	m_spTexturingProp->SetShaderMap(2, NiNew NiTexturingProperty::ShaderMap(m_spAlbedoTexture, 0));
	m_spTexturingProp->SetShaderMap(3, NiNew NiTexturingProperty::ShaderMap(m_spGlossTexture, 0));
	m_spTexturingProp->SetShaderMap(4, NiNew NiTexturingProperty::ShaderMap(m_spGlowTexture, 0));
	m_kPropertyList.AddTail((NiTexturingProperty*)m_spTexturingProp);

	return (m_bInitialized = true);
}
//-------------------------------------------------------------------------------------------------
void sdMRTShadingPass::Destroy()
{
	m_uiStaticMeshStencilID = 0;
	m_uiStaticMeshStencilIDMask = 0;

	m_kStaticShadingMaterials.clear();

	m_spGeomTexture = 0;
	m_spLightTexture = 0;
	m_spAlbedoTexture = 0;
	m_spGlossTexture = 0;
	m_spGlowTexture = 0;

	m_kPropertyList.RemoveAll();
	m_spTexturingProp = 0;

	m_bInitialized = false;
}
//-------------------------------------------------------------------------------------------------
void sdMRTShadingPass::Draw()
{
	D3DPERF_BeginEvent(0xff000000, L"MRTShadingPass");

	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	IRenderSystem* pkRenderSystem = IRenderSystem::GetRenderSystem();
	NIASSERT(pkRenderSystem);

	// 清除数据绑定
	pkRenderDevice->ClearVertexBinding();
	//pkRenderDevice->ClearTextureBinding();


	// 设置渲染状态
	// @{
	pkRenderDevice->SetRenderState(D3DRS_CULLMODE,			D3DCULL_NONE);
	pkRenderDevice->SetRenderState(D3DRS_FILLMODE,			D3DFILL_SOLID);

	pkRenderDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,	false);
	pkRenderDevice->SetRenderState(D3DRS_ALPHATESTENABLE,	false);

	pkRenderDevice->SetRenderState(D3DRS_ZENABLE,			false);
	pkRenderDevice->SetRenderState(D3DRS_ZWRITEENABLE,		false);

	pkRenderDevice->SetRenderState(D3DRS_STENCILENABLE,		true);
	pkRenderDevice->SetRenderState(D3DRS_STENCILFUNC,		D3DCMP_EQUAL);
	pkRenderDevice->SetRenderState(D3DRS_STENCILFAIL,		D3DSTENCILOP_KEEP);
	pkRenderDevice->SetRenderState(D3DRS_STENCILZFAIL,		D3DSTENCILOP_KEEP);
	pkRenderDevice->SetRenderState(D3DRS_STENCILPASS,		D3DSTENCILOP_KEEP);
	pkRenderDevice->SetRenderState(D3DRS_STENCILREF,		m_uiStaticMeshStencilID);
	pkRenderDevice->SetRenderState(D3DRS_STENCILMASK,		m_uiStaticMeshStencilIDMask);
	// @}

	
	// 选择材质(掩码标记见static_mesh_common.h文件)
	// #define	PIXEL_SPECULAR_LIGHTTING_CHANNEL	0x00000001	///< 高光
	// #define	PIXEL_RIM_LIGHTTING_CHANNEL			0x00000002	///< 轮廓光
	// #define	PIXEL_GLOW_LIGHTTING_CHANNEL		0x00000004	///< 辉光
	// @{
	uint uiChannelFlag = 0;

	if (true)
		uiChannelFlag |= (1 << 0);

	if (false)
		uiChannelFlag |= (1 << 1);

	if (false)
		uiChannelFlag |= (1 << 2);
	// @}


	// 绘制
	// @{
	NiMesh* pkScreenMesh = pkRenderSystem->GetDefaultMesh(IRenderSystem::E_DM_SCREEN_QUAD);
	NIASSERT(pkScreenMesh);

	NiPropertyState* pkPropState = pkScreenMesh->GetPropertyState();
	NIASSERT(pkPropState);
	pkPropState->SwapProperties(m_kPropertyList);

	pkScreenMesh->ApplyAndSetActiveMaterial(m_kStaticShadingMaterials[uiChannelFlag]);

	NiRenderer* pkRenderer = NiRenderer::GetRenderer();
	pkScreenMesh->RenderImmediate(pkRenderer);

	pkScreenMesh->SetActiveMaterial(NULL);
	pkPropState->SwapProperties(m_kPropertyList);
	// @}

	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE