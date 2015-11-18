#include "sdRenderSystemPCH.h"
#include "sdTerrainAtlasShadingPass.h"

using namespace Engine::ResourceSystem;

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
sdTerrainAtlasShadingPass::sdTerrainAtlasShadingPass(sdRenderObjectAlloc* pkAlloc)
: sdRenderPass(pkAlloc)
, m_uiStencilRef(0)
, m_uiStencilMask(0)
{

}
//-------------------------------------------------------------------------------------------------
sdTerrainAtlasShadingPass::~sdTerrainAtlasShadingPass()
{
	if (m_bInitialized)
	{
		Destroy();
		m_bInitialized = false;
	}
}
//-------------------------------------------------------------------------------------------------
bool sdTerrainAtlasShadingPass::Initialize(uint uiStencilRef, uint uiStencilMask, NiRenderedTexture* spLightTexture, NiRenderedTexture* spGeomTexture)
{
	NIASSERT(uiStencilRef & uiStencilMask);
	NIASSERT(spLightTexture);
	NIASSERT(spGeomTexture);

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

	// 设置模版参数
	m_uiStencilRef = uiStencilRef;
	m_uiStencilMask = uiStencilMask;

	// 设置输入纹理
	m_spLightTexture = spLightTexture;
	m_spGeomTexture = spGeomTexture;

	m_spUVTableCubeMap = pkResourceSystem->LoadCubeTexture("dependence\\engine\\texture\\terrain\\terrain_uvtable.dds");
	NIASSERT(m_spUVTableCubeMap);

	// 初始化材质
	m_spPlanarShadingMaterial = pkRenderDevice->CreateMaterial("Terrain_AtlasShading_Planar");
	NIASSERT(m_spPlanarShadingMaterial);

	m_spSeamShadingMaterial = pkRenderDevice->CreateMaterial("Terrain_AtlasShading_Seam");
	NIASSERT(m_spSeamShadingMaterial);

	// 初始化纹理属性
	m_spTexturingProp  = NiNew NiTexturingProperty;
	NIASSERT(m_spTexturingProp);
	m_spTexturingProp->SetShaderMap(0, NiNew NiTexturingProperty::ShaderMap(m_spUVTableCubeMap, 0));
	m_spTexturingProp->SetShaderMap(1, NiNew NiTexturingProperty::ShaderMap(spGeomTexture, 0));
	m_spTexturingProp->SetShaderMap(2, NiNew NiTexturingProperty::ShaderMap(spLightTexture, 0));
	m_spTexturingProp->SetShaderMap(3, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_spTexturingProp->SetShaderMap(4, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_spTexturingProp->SetShaderMap(5, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_spTexturingProp->SetShaderMap(6, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_spTexturingProp->SetShaderMap(7, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_kPropertyList.AddTail((NiTexturingProperty*)m_spTexturingProp);

	return (m_bInitialized = true);
}
//-------------------------------------------------------------------------------------------------
void sdTerrainAtlasShadingPass::Destroy()
{
	m_spTexturingProp = 0;

	m_uiStencilRef = 0;
	m_uiStencilMask = 0;

	m_spLightTexture = 0;
	m_spGeomTexture = 0;
	m_spUVTableCubeMap = 0;

	m_spSimpleShadingMaterial = 0;
	m_spPlanarShadingMaterial = 0;
	m_spSeamShadingMaterial = 0;
	m_spOrthoShadingMaterial = 0;

	m_bInitialized = false;
}
//-------------------------------------------------------------------------------------------------
void sdTerrainAtlasShadingPass::SetTerrainParams(const sdTerrainParams& kTerrainParams)
{
	// 地表normalMap
	NiTexturingProperty::ShaderMap* spShaderMap = m_spTexturingProp->GetShaderMap(3);
	NIASSERT(spShaderMap);
	if (spShaderMap->GetTexture() != kTerrainParams.baseNormalMap)
	{
		spShaderMap->SetTexture(kTerrainParams.baseNormalMap);
	}

	// 地表TileMap
	spShaderMap = m_spTexturingProp->GetShaderMap(4);
	NIASSERT(spShaderMap);
	if (spShaderMap->GetTexture() != kTerrainParams.tileMap)
	{
		spShaderMap->SetTexture(kTerrainParams.tileMap);
	}

	// 地表BlendeMap
	spShaderMap = m_spTexturingProp->GetShaderMap(5);
	NIASSERT(spShaderMap);
	if (spShaderMap->GetTexture() != kTerrainParams.blendMap)
	{
		spShaderMap->SetTexture(kTerrainParams.blendMap);
	}

	// 地表DiffuseMap
	spShaderMap = m_spTexturingProp->GetShaderMap(6);
	NIASSERT(spShaderMap);
	if (spShaderMap->GetTexture() != kTerrainParams.diffuseAtlasTableMap)
	{
		spShaderMap->SetTexture(kTerrainParams.diffuseAtlasTableMap);
	}

	spShaderMap = m_spTexturingProp->GetShaderMap(7);
	NIASSERT(spShaderMap);
	if (spShaderMap->GetTexture() != kTerrainParams.diffuseAtlasMap)
	{
		spShaderMap->SetTexture(kTerrainParams.diffuseAtlasMap);
	}
}
//-------------------------------------------------------------------------------------------------
void sdTerrainAtlasShadingPass::Draw()
{
	D3DPERF_BeginEvent(0xff000000, L"TerrainAtlasShadingPass");

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
	// CullMode
	pkRenderDevice->SetRenderState(D3DRS_CULLMODE,			D3DCULL_NONE);

	// Alpha 
	pkRenderDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,	false);
	pkRenderDevice->SetRenderState(D3DRS_ALPHATESTENABLE,	false);

	// Z
	pkRenderDevice->SetRenderState(D3DRS_ZENABLE,			false);
	pkRenderDevice->SetRenderState(D3DRS_ZWRITEENABLE,		false);

	// Stencil
	pkRenderDevice->SetRenderState(D3DRS_STENCILENABLE,		true);
	pkRenderDevice->SetRenderState(D3DRS_STENCILFUNC,		D3DCMP_EQUAL);
	pkRenderDevice->SetRenderState(D3DRS_STENCILFAIL,		D3DSTENCILOP_KEEP);
	pkRenderDevice->SetRenderState(D3DRS_STENCILZFAIL,		D3DSTENCILOP_KEEP);
	pkRenderDevice->SetRenderState(D3DRS_STENCILPASS,		D3DSTENCILOP_KEEP);
	pkRenderDevice->SetRenderState(D3DRS_STENCILREF,		m_uiStencilRef);
	pkRenderDevice->SetRenderState(D3DRS_STENCILMASK,		m_uiStencilMask);
	// @}

	// 绘制
	// @{
	NiMesh* spScreenMesh = pkRenderSystem->GetDefaultMesh(IRenderSystem::E_DM_SCREEN_QUAD);
	NIASSERT(spScreenMesh);

	NiPropertyState* spPropState = spScreenMesh->GetPropertyState();
	NIASSERT(spPropState);
	spPropState->SwapProperties(m_kPropertyList);
	
	// 平坦区域着色
	spScreenMesh->ApplyAndSetActiveMaterial(m_spPlanarShadingMaterial);
	spScreenMesh->RenderImmediate(spRenderer);

	// 陡峭区域着色
	spScreenMesh->ApplyAndSetActiveMaterial(m_spSeamShadingMaterial);
	spScreenMesh->RenderImmediate(spRenderer);

	spScreenMesh->SetActiveMaterial(NULL);
	spPropState->SwapProperties(m_kPropertyList);
	// @}


	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE