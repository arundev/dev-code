#include "sdRenderSystemPCH.h"
#include "sdTerrainAtlasGeometryPass.h"

using namespace Engine::ResourceSystem;

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
sdTerrainAtlasGeometryPass::sdTerrainAtlasGeometryPass(sdRenderObjectAlloc* pkAlloc)
: sdRenderPass(pkAlloc)
, m_uiTerrainStencilID(0)
, m_uiTerrainStencilIDMask(0)
, m_uiFarTerrainStencilIDShift(0)
{
	
}
//-------------------------------------------------------------------------------------------------
sdTerrainAtlasGeometryPass::~sdTerrainAtlasGeometryPass()
{
	if (m_bInitialized)
	{
		Destroy();
		m_bInitialized = false;
	}
}
//-------------------------------------------------------------------------------------------------
bool sdTerrainAtlasGeometryPass::Initialize(uint uiTerrainStencilID, uint uiTerrainStencilIDMask, 
	uint uiFarTerrainStencilIDShift, NiRenderedTexture* spDepthTexture)
{
	NIASSERT(uiTerrainStencilID & uiTerrainStencilIDMask);
	NIASSERT(uiFarTerrainStencilIDShift);
	NIASSERT(spDepthTexture);

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

	// ����ģ�����
	m_uiTerrainStencilID = uiTerrainStencilID;
	m_uiTerrainStencilIDMask = uiTerrainStencilIDMask;
	m_uiFarTerrainStencilIDShift = uiFarTerrainStencilIDShift;

	// ������������
	m_spDepthTexture = spDepthTexture;

	m_spUVTableCubeMap = pkResourceSystem->LoadCubeTexture("dependence\\engine\\texture\\terrain\\terrain_uvtable.dds");
	NIASSERT(m_spUVTableCubeMap);

	// ��ʼ������
	m_spBaseNormalOnlyMaterial = pkRenderDevice->CreateMaterial("Terrain_AtlasGeometry_BaseNormal");
	NIASSERT(m_spBaseNormalOnlyMaterial);

	m_spNearNormalMapMaterial = pkRenderDevice->CreateMaterial("Terrain_AtlasGeometry_Near_BaseNormalAndNormalMap");
	NIASSERT(m_spNearNormalMapMaterial);

	m_spFarBaseNormalMaterial = pkRenderDevice->CreateMaterial("Terrain_AtlasGeometry_Far_BaseNormal");
	NIASSERT(m_spFarBaseNormalMaterial);

	// ��ʼ����������
	m_spTexturingProp  = NiNew NiTexturingProperty;
	NIASSERT(m_spTexturingProp);
	m_spTexturingProp->SetShaderMap(0, NiNew NiTexturingProperty::ShaderMap(m_spUVTableCubeMap, 0));
	m_spTexturingProp->SetShaderMap(1, NiNew NiTexturingProperty::ShaderMap(spDepthTexture, 0));
	m_spTexturingProp->SetShaderMap(2, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_spTexturingProp->SetShaderMap(3, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_spTexturingProp->SetShaderMap(4, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_spTexturingProp->SetShaderMap(5, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_spTexturingProp->SetShaderMap(6, NiNew NiTexturingProperty::ShaderMap(0, 0));
	m_kPropertyList.AddTail((NiTexturingProperty*)m_spTexturingProp);

	return (m_bInitialized = true);
}
//-------------------------------------------------------------------------------------------------
void sdTerrainAtlasGeometryPass::Destroy()
{
	m_spTexturingProp = 0;

	m_spDepthTexture = 0;
	m_spUVTableCubeMap = 0;

	m_spBaseNormalOnlyMaterial = 0;
	m_spNearNormalMapMaterial = 0;
	m_spFarBaseNormalMaterial =0;

	m_uiTerrainStencilID = 0;
	m_uiTerrainStencilIDMask = 0;
	m_uiFarTerrainStencilIDShift = 0;

	m_bInitialized = false;
}
//-------------------------------------------------------------------------------------------------
void sdTerrainAtlasGeometryPass::SetTerrainParams(const sdTerrainParams& kTerrainParams)
{
	// �ر�normalMap
	NiTexturingProperty::ShaderMap* spShaderMap = m_spTexturingProp->GetShaderMap(2);
	NIASSERT(spShaderMap);
	if (spShaderMap->GetTexture() != kTerrainParams.baseNormalMap)
	{
		spShaderMap->SetTexture(kTerrainParams.baseNormalMap);
	}

	// �ر�TileMap
	spShaderMap = m_spTexturingProp->GetShaderMap(3);
	NIASSERT(spShaderMap);
	if (spShaderMap->GetTexture() != kTerrainParams.tileMap)
	{
		spShaderMap->SetTexture(kTerrainParams.tileMap);
	}

	// �ر�BlendeMap
	spShaderMap = m_spTexturingProp->GetShaderMap(4);
	NIASSERT(spShaderMap);
	if (spShaderMap->GetTexture() != kTerrainParams.blendMap)
	{
		spShaderMap->SetTexture(kTerrainParams.blendMap);
	}

	// �ر�NormalMap
	spShaderMap = m_spTexturingProp->GetShaderMap(5);
	NIASSERT(spShaderMap);
	if (spShaderMap->GetTexture() != kTerrainParams.normalAtlasTableMap)
	{
		spShaderMap->SetTexture(kTerrainParams.normalAtlasTableMap);
	}

	spShaderMap = m_spTexturingProp->GetShaderMap(6);
	NIASSERT(spShaderMap);
	if (spShaderMap->GetTexture() != kTerrainParams.normalAtlasMap)
	{
		spShaderMap->SetTexture(kTerrainParams.normalAtlasMap);
	}
}
//-------------------------------------------------------------------------------------------------
void sdTerrainAtlasGeometryPass::Draw()
{
	D3DPERF_BeginEvent(0xff000000, L"TerrainAtlasGeometryPass");

	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	IRenderSystem* pkRenderSystem = IRenderSystem::GetRenderSystem();
	NIASSERT(pkRenderSystem);

	NiRenderer* spRenderer = NiRenderer::GetRenderer();
	NIASSERT(spRenderer);

	// ������ݰ�
	pkRenderDevice->ClearVertexBinding();
	pkRenderDevice->ClearTextureBinding();

	// ���ݵ�ǰ������Ⱦ��NormalMapͨ�����ý��зֱ���
	if (m_pkRenderParams->IsEnableChannel(sdRenderParams::E_TERRAIN, sdRenderParams::E_NORMALMAP))
	{
		// ��������ʹ��NormalMapͨ��,�Ӷ�����ʹ��NormalMap
		//
		// ������Ⱦ״̬
		// @{
		// CullMode
		pkRenderDevice->SetRenderState(D3DRS_CULLMODE,			D3DCULL_NONE);

		// Alpha 
		//
		// WZԭ������,����(���޸�)
		//pkRenderDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,	true);
		//pkRenderDevice->SetRenderState(D3DRS_BLENDOP,				D3DBLENDOP_ADD);
		//pkRenderDevice->SetRenderState(D3DRS_SRCBLEND,			D3DBLEND_ONE);
		//pkRenderDevice->SetRenderState(D3DRS_DESTBLEND,			D3DBLEND_ONE);
		//
		pkRenderDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,	false);
		pkRenderDevice->SetRenderState(D3DRS_ALPHATESTENABLE,	false);

		// Z
		pkRenderDevice->SetRenderState(D3DRS_ZENABLE,			false);
		pkRenderDevice->SetRenderState(D3DRS_ZWRITEENABLE,		false);

		// Stencil
		// (���ｫ����Terrain�����滻ΪFraTerrain����)
		pkRenderDevice->SetRenderState(D3DRS_STENCILENABLE,		true);
		pkRenderDevice->SetRenderState(D3DRS_STENCILFUNC,		D3DCMP_EQUAL);
		pkRenderDevice->SetRenderState(D3DRS_STENCILFAIL,		D3DSTENCILOP_KEEP);
		pkRenderDevice->SetRenderState(D3DRS_STENCILZFAIL,		D3DSTENCILOP_KEEP);
		pkRenderDevice->SetRenderState(D3DRS_STENCILPASS,		D3DSTENCILOP_REPLACE);
		pkRenderDevice->SetRenderState(D3DRS_STENCILREF,		m_uiTerrainStencilID | (1 << m_uiFarTerrainStencilIDShift));
		pkRenderDevice->SetRenderState(D3DRS_STENCILMASK,		m_uiTerrainStencilIDMask);
		pkRenderDevice->SetRenderState(D3DRS_STENCILWRITEMASK,	1 << m_uiFarTerrainStencilIDShift);
		// @}

		// ����
		// @{
		NiMesh* spScreenMesh = pkRenderSystem->GetDefaultMesh(IRenderSystem::E_DM_SCREEN_QUAD);
		NIASSERT(spScreenMesh);

		NiPropertyState* spPropState = spScreenMesh->GetPropertyState();
		NIASSERT(spPropState);
		spPropState->SwapProperties(m_kPropertyList);

		// ����Զ��
		spScreenMesh->ApplyAndSetActiveMaterial(m_spFarBaseNormalMaterial);
		spScreenMesh->RenderImmediate(spRenderer);

		// ���ƽ���
		pkRenderDevice->SetRenderState(D3DRS_STENCILREF,		m_uiTerrainStencilID);
		pkRenderDevice->SetRenderState(D3DRS_STENCILMASK,		m_uiTerrainStencilIDMask | (1 << m_uiFarTerrainStencilIDShift));
		pkRenderDevice->SetRenderState(D3DRS_STENCILWRITEMASK,	1 << m_uiFarTerrainStencilIDShift);	

		spScreenMesh->ApplyAndSetActiveMaterial(m_spNearNormalMapMaterial);
		spScreenMesh->RenderImmediate(spRenderer);

		//
		spScreenMesh->SetActiveMaterial(NULL);
		spPropState->SwapProperties(m_kPropertyList);
		// @}
	}
	else
	{
		// ���ν���NormalMapͨ��,�Ӷ�ֻ��ʹ�û�������
		//
		// ������Ⱦ״̬
		// @{
		// CullMode
		pkRenderDevice->SetRenderState(D3DRS_CULLMODE,			D3DCULL_NONE);

		// Alpha 
		//
		// WZԭ������,����(���޸�)
		//pkRenderDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,	true);
		//pkRenderDevice->SetRenderState(D3DRS_BLENDOP,				D3DBLENDOP_ADD);
		//pkRenderDevice->SetRenderState(D3DRS_SRCBLEND,			D3DBLEND_ONE);
		//pkRenderDevice->SetRenderState(D3DRS_DESTBLEND,			D3DBLEND_ONE);
		//
		pkRenderDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,	false);
		pkRenderDevice->SetRenderState(D3DRS_ALPHATESTENABLE,	false);

		// Z
		pkRenderDevice->SetRenderState(D3DRS_ZENABLE,			false);
		pkRenderDevice->SetRenderState(D3DRS_ZWRITEENABLE,		false);

		// Stencil
		// (���ｫ����Terrain�����滻ΪFraTerrain����)
		pkRenderDevice->SetRenderState(D3DRS_STENCILENABLE,		true);
		pkRenderDevice->SetRenderState(D3DRS_STENCILFUNC,		D3DCMP_EQUAL);
		pkRenderDevice->SetRenderState(D3DRS_STENCILFAIL,		D3DSTENCILOP_KEEP);
		pkRenderDevice->SetRenderState(D3DRS_STENCILZFAIL,		D3DSTENCILOP_KEEP);
		pkRenderDevice->SetRenderState(D3DRS_STENCILPASS,		D3DSTENCILOP_REPLACE);
		pkRenderDevice->SetRenderState(D3DRS_STENCILREF,		m_uiTerrainStencilID | (1 << m_uiFarTerrainStencilIDShift));
		pkRenderDevice->SetRenderState(D3DRS_STENCILMASK,		m_uiTerrainStencilIDMask);
		pkRenderDevice->SetRenderState(D3DRS_STENCILWRITEMASK,	1 << m_uiFarTerrainStencilIDShift);
		// @}

		// ����
		// @{
		NiMesh* spScreenMesh = pkRenderSystem->GetDefaultMesh(IRenderSystem::E_DM_SCREEN_QUAD);
		NIASSERT(spScreenMesh);

		NiPropertyState* spPropState = spScreenMesh->GetPropertyState();
		NIASSERT(spPropState);
		spPropState->SwapProperties(m_kPropertyList);
		spScreenMesh->ApplyAndSetActiveMaterial(m_spBaseNormalOnlyMaterial);

		spScreenMesh->RenderImmediate(spRenderer);

		spScreenMesh->SetActiveMaterial(NULL);
		spPropState->SwapProperties(m_kPropertyList);
		// @}
	}

	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE