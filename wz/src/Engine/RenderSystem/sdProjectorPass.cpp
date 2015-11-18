#include "sdRenderSystemPCH.h"
#include "sdProjectorPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
sdProjectorPass::sdProjectorPass(sdRenderObjectAlloc* pkAlloc)
: sdRenderPass(pkAlloc)
, m_uiProjectorMeshStencilID(0)
, m_uiProjectorMeshStencilIDMask(0)
{

}
//-------------------------------------------------------------------------------------------------
sdProjectorPass::~sdProjectorPass()
{
	if (m_bInitialized)
	{
		Destroy();
		m_bInitialized = false;
	}
}
//-------------------------------------------------------------------------------------------------
bool sdProjectorPass::Initialize(uint uiProjectorMeshStencilID, uint uiProjectorMeshStencilIDMask, 
	NiRenderedTexture* pkGeometryTexture, NiRenderedTexture* pkLightTexture)
{
	NIASSERT(uiProjectorMeshStencilID & uiProjectorMeshStencilIDMask);
	NIASSERT(pkGeometryTexture);
	NIASSERT(pkLightTexture);

	if (m_bInitialized)
	{
		NIASSERT(0);
		return false;
	}

	// ����ģ�����
	m_uiProjectorMeshStencilID = uiProjectorMeshStencilID;
	m_uiProjectorMeshStencilIDMask = uiProjectorMeshStencilIDMask;

	// ��ʼ������
	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	char szMaterialName[512];
	uint uiNum = (uint)pow(2.0f, sdRenderParams::NUM_MAP_CHANNELS);
	m_kMaterialVec.resize(uiNum);
	for (uint i = 0; i < uiNum; ++i)
	{
		sprintf(szMaterialName, "Decal_Projector0x%08x", i);
		m_kMaterialVec[i] = pkRenderDevice->CreateMaterial(szMaterialName);
	}

	// ��ʼ������
	m_spGeometryTexture = pkGeometryTexture;
	m_spLightTexture = pkLightTexture;

	// ��ʼ����������
	m_spTexturingProp  = NiNew NiTexturingProperty;
	NIASSERT(m_spTexturingProp);
	m_spTexturingProp->SetBaseMap(NiNew NiTexturingProperty::Map(NULL, 0));		///< һ���Է������ͼ���
	m_spTexturingProp->SetNormalMap(NiNew NiTexturingProperty::Map(NULL, 0));	///< 
	m_spTexturingProp->SetGlowMap( NiNew NiTexturingProperty::Map(NULL, 0));	///<
	m_spTexturingProp->SetDarkMap( NiNew NiTexturingProperty::Map(NULL, 0));	///<
	m_spTexturingProp->SetShaderMap(0, NiNew NiTexturingProperty::ShaderMap(m_spGeometryTexture, 0));
	m_spTexturingProp->SetShaderMap(1, NiNew NiTexturingProperty::ShaderMap(m_spLightTexture, 0));
	m_kPropertyList.AddTail((NiTexturingProperty*)m_spTexturingProp);

	return (m_bInitialized = true);
}
//-------------------------------------------------------------------------------------------------
void sdProjectorPass::Destroy()
{
	m_uiProjectorMeshStencilID = 0;
	m_uiProjectorMeshStencilIDMask = 0;

	m_kMaterialVec.clear();

	m_spGeometryTexture = 0;
	m_spLightTexture = 0;

	m_spTexturingProp = 0;
	m_kPropertyList.RemoveAll();

	m_bInitialized = false;
}
//-------------------------------------------------------------------------------------------------
void sdProjectorPass::InsertProjector(sdProjectorSceneObject* pkProjector)
{
	typedef sdRenderObject::ShaderParamItem	ShaderParamItem;

	NIASSERT(m_bInitialized);
	NIASSERT(pkProjector);

	// ����Ƿ�ɼ�
	if (!pkProjector->GetVisible())
		return;

	// ����Ƿ��������
	//	1.�Դ�����,��ֱ�Ӳ���
	//	2.���Դ�����,����Ҳ��ʲ�����
	NiMaterial* pkCustomMaterial = pkProjector->GetMaterial();
	if (pkCustomMaterial)
	{
		InsertProjectorSceneObject(pkProjector, pkCustomMaterial, &m_kPropertyList, NULL);
	}
	else
	{
		uint uiChannelFlag = 0;

		if (pkProjector->GetReceiveLighting() && pkProjector->GetDiffuseMap())
			uiChannelFlag |= (1 << sdRenderParams::E_DIFFUSEMAP);

		if (pkProjector->GetReceiveLighting() && pkProjector->GetNormalMap())
			uiChannelFlag |= (1 << sdRenderParams::E_NORMALMAP);

		if (pkProjector->GetFilterMap())
			uiChannelFlag |= (1 << sdRenderParams::E_FILTERMAP);

		if (!pkProjector->GetReceiveLighting() && pkProjector->GetDiffuseMap())
			uiChannelFlag |= (1 << sdRenderParams::E_GLOWMAP);

		uiChannelFlag &= m_pkRenderParams->buildingChannelMask;

		NiMaterial* pkMaterial = m_kMaterialVec[uiChannelFlag];
		if (pkMaterial)
			InsertProjectorSceneObject(pkProjector, pkMaterial, &m_kPropertyList, NULL);
	}
}
//-------------------------------------------------------------------------------------------------
void sdProjectorPass::Draw()
{
	D3DPERF_BeginEvent(0xff000000, L"ProjectorPass");

	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	// ������ݰ�
	pkRenderDevice->ClearVertexBinding();
	//pkRenderDevice->ClearTextureBinding();

	// ������Ⱦ״̬
	// (������������,��Ͷ���帲�ǵĵط���ʹ��Ͷ�����)
	// @{
	pkRenderDevice->SetRenderState(D3DRS_CULLMODE,			D3DCULL_CCW);
	pkRenderDevice->SetRenderState(D3DRS_FILLMODE,			D3DFILL_SOLID);

	pkRenderDevice->SetRenderState(D3DRS_ZENABLE,			true);
	pkRenderDevice->SetRenderState(D3DRS_ZWRITEENABLE,		false);
	pkRenderDevice->SetRenderState(D3DRS_ZFUNC,				D3DCMP_GREATER);

	pkRenderDevice->SetRenderState(D3DRS_STENCILENABLE,		true);
	pkRenderDevice->SetRenderState(D3DRS_STENCILFUNC,		D3DCMP_EQUAL);
	pkRenderDevice->SetRenderState(D3DRS_STENCILFAIL,		D3DSTENCILOP_KEEP);
	pkRenderDevice->SetRenderState(D3DRS_STENCILZFAIL,		D3DSTENCILOP_KEEP);
	pkRenderDevice->SetRenderState(D3DRS_STENCILPASS,		D3DSTENCILOP_KEEP);
	pkRenderDevice->SetRenderState(D3DRS_STENCILREF,		m_uiProjectorMeshStencilID);
	pkRenderDevice->SetRenderState(D3DRS_STENCILMASK,		m_uiProjectorMeshStencilIDMask);
	pkRenderDevice->SetRenderState(D3DRS_STENCILWRITEMASK,	0xFFFFFFFF);

	pkRenderDevice->SetRenderState(D3DRS_ALPHATESTENABLE,	false);
	pkRenderDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,	true);
	pkRenderDevice->SetRenderState(D3DRS_BLENDOP,			D3DBLENDOP_ADD);
	pkRenderDevice->SetRenderState(D3DRS_SRCBLEND,			D3DBLEND_ONE);
	pkRenderDevice->SetRenderState(D3DRS_DESTBLEND,			D3DBLEND_ONE);

	pkRenderDevice->SetRenderState(D3DRS_COLORWRITEENABLE,	D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
	// @}

	// ����
	__super::Draw();

	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE