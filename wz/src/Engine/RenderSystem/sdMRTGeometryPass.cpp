#include "sdRenderSystemPCH.h"
#include "sdMRTGeometryPass.h"
#include "sdFixedString.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
sdMRTGeometryPass::sdMRTGeometryPass(sdRenderObjectAlloc* pkAlloc)
: sdRenderPass(pkAlloc)
, m_uiStencilID(0)
, m_uiStencilIDWriteMask(0)
{

}
//-------------------------------------------------------------------------------------------------
sdMRTGeometryPass::~sdMRTGeometryPass()
{
	if (m_bInitialized)
	{
		Destroy();
		m_bInitialized = false;
	}
}
//-------------------------------------------------------------------------------------------------
bool sdMRTGeometryPass::Initialize(uint uiStencilID, uint uiStencilIDWriteMask)
{
	if (m_bInitialized)
	{
		NIASSERT(0);
		return false;
	}

	// 设置模版参数
	m_uiStencilID = uiStencilID;
	m_uiStencilIDWriteMask = uiStencilIDWriteMask;

	// 初始化材质
	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	char szMaterialName[64];
	uint uiNum = 128;
	m_kStaticGeometryMaterials.resize(uiNum);
	m_kSkinnedGeometryMaterials.resize(uiNum);
	for (uint i = 0; i < uiNum; ++i)
	{
		sprintf(szMaterialName, "MRT3Geometry0x%08x", i);
		m_kStaticGeometryMaterials[i] = pkRenderDevice->CreateMaterial(szMaterialName);
		
		sprintf(szMaterialName, "MRT3SkinnedGeometry%08x", i);
		m_kSkinnedGeometryMaterials[i] = pkRenderDevice->CreateMaterial(szMaterialName);
	}

	// 初始化属性
	m_spTexturingProp = NiNew NiTexturingProperty;
	NIASSERT(m_spTexturingProp);
	m_spTexturingProp->SetShaderMap(0, NiNew NiTexturingProperty::ShaderMap(NULL, 0));
	m_spTexturingProp->SetShaderMap(1, NiNew NiTexturingProperty::ShaderMap(NULL, 0));
	m_kPropertyList.AddTail((NiTexturingProperty*)m_spTexturingProp);

	return (m_bInitialized = true);
}
//-------------------------------------------------------------------------------------------------
void sdMRTGeometryPass::Destroy()
{
	m_uiStencilID = 0;
	m_uiStencilIDWriteMask = 0;

	m_kStaticGeometryMaterials.clear();
	m_kSkinnedGeometryMaterials.clear();

	m_kPropertyList.RemoveAll();
	m_spTexturingProp = 0;

	m_bInitialized = false;
}
//-------------------------------------------------------------------------------------------------
void sdMRTGeometryPass::InsertStaticMesh(NiMesh* pkMesh)
{
	typedef sdRenderObject::ShaderParamItem	ShaderParamItem;

	NIASSERT(m_bInitialized);
	NIASSERT(pkMesh);

	IRenderSystem* pkRenderSystem = IRenderSystem::GetRenderSystem();
	NIASSERT(pkRenderSystem);

	NiPropertyState* pkPropState = pkMesh->GetPropertyState();
	if (!pkPropState)
	{
		NIASSERT(0);
		return;
	}

	NiWireframeProperty* pkWireframeProp = pkPropState->GetWireframe();
	if (!pkWireframeProp || pkWireframeProp->GetWireframe())
	{	
		NIASSERT(0);	///< Wireframe mesh should never be inserted to here
		return;	
	}

	NiTexturingProperty* pkTexProp = pkPropState->GetTexturing();
	NiMaterialProperty* pkMatProp = pkPropState->GetMaterial();
	NiAlphaProperty* pkAlphaProp = pkPropState->GetAlpha();
	if (!pkTexProp || !pkMatProp || !pkAlphaProp)
	{
		NIASSERT(0);
		return;
	}

	bool bSkinned = pkMesh->RequiresMaterialOption(NiRenderObjectMaterialOption::TRANSFORM_SKINNED());


	// 构建ShaderParamItem数组,传入对象Material参数
	// @{
	static const sdFixedString s_szDiffuseName("g_vDiffuseMaterial", true);
	static const sdFixedString s_szSpecularName("g_vSpecularMaterial", true);
	static const sdFixedString s_szEmissiveName("g_vEmissiveMaterial", true);
	static const sdFixedString s_szAlphaTestName("g_fAlphaTestRef",	true);
	static const sdFixedString s_szEdgeEnhancementName("g_fEdgeEnhancement", true);
 
	NiColor kDiffuseColor = pkMatProp->GetDiffuseColor();
	float fLocalLightFactor = 1.f;	// (uiOcclusionId & 0xff) / 255.f;
	NiPoint4 kDiffuse(kDiffuseColor.r, kDiffuseColor.g, kDiffuseColor.b, fLocalLightFactor);

	NiColor kSpecularColor = pkMatProp->GetSpecularColor();
	float fShiness = pkMatProp->GetShineness();
	NiPoint4 kSpecular(kSpecularColor.r, kSpecularColor.g, kSpecularColor.b, fShiness);
	
	NiColor kEmissiveColor = pkMatProp->GetEmittance();
	NiPoint4 kEmissive(kEmissiveColor.r, kEmissiveColor.g, kEmissiveColor.b, 1.0f);

	bool bEdgeEnhancement = false;//spMatProp->GetEdgeEnhancement();
	float fEdge = bEdgeEnhancement ? 1.f : 0.f;

	float fAlphaTestRef = 0.0f;
	if (pkAlphaProp->GetAlphaTesting())
	{
		fAlphaTestRef = 128.0f / 255.0f + 0.001f;	//spAlphaProp->GetTestRef()
	}

	ShaderParamItem kShaderParams[] = {
		{&s_szDiffuseName,			sizeof(float) * 4,		(void*)&kDiffuse},
		{&s_szSpecularName,			sizeof(float) * 4,		(void*)&kSpecular},
		{&s_szEmissiveName,			sizeof(float) * 4,		(void*)&kEmissive},
		{&s_szEdgeEnhancementName,	sizeof(float),			(void*)&fEdge},
		{&s_szAlphaTestName,		sizeof(float),			(void*)&fAlphaTestRef},
		{NULL,						0,						NULL}};
	// }@

	// 根据输入纹理情况,确定所使用的着色器掩码(掩码标记见static_mesh_common.h文件)
	//#define VERTEX_DIFFUSEMAP_CHANNEL			0x00000001	///< 漫反射通道
	//#define VERTEX_NORMALMAP_CHANNEL			0x00000002	///< 法线贴图通道
	//#define VERTEX_GLOSSMAP_CHANNEL			0x00000004	///< 高光贴图通道
	//#define VERTEX_LIGHTMAP_CHANNEL			0x00000008	///< 光照贴图通道
	//#define VERTEX_GLOWMAP_CHANNEL			0x00000010	///< 辉光贴图通道
	//#define VERTEX_DETAIL_NORMAL_CHANNEL		0x00000020	///< 细节贴图通道
	//#define VERTEX_ALPHATEST_FLAG				0x00000040	///< 透明测试
	//#define VERTEX_COMPACTVERTEX_FLAG			0x00000080	///< 顶点压缩
	//#define VERTEX_LEAFANIM_FLAG				0x00000100	///< 树叶顶点动画
	// @{
	uint uiChannelFlag = 0;

	if (pkTexProp->GetBaseMap())
		uiChannelFlag |= (1 << 0);

	if (pkTexProp->GetNormalMap())
		uiChannelFlag |= (1 << 1);
	
	if (pkTexProp->GetGlossMap())
		uiChannelFlag |= (1 << 2);

	if (bSkinned)
	{
		NiFloatExtraData* pkOcclusionExtraData = (NiFloatExtraData*)pkMesh->GetExtraData("a_fMainLightOcclusion");
		if (pkOcclusionExtraData)
			uiChannelFlag |= (1 << 3);
	}
	else
	{
		if (pkTexProp->GetDarkMap())		
			uiChannelFlag |= (1 << 3);
	}

	if (pkTexProp->GetGlowMap())
		uiChannelFlag |= (1 << 4);

//	if (NiAlphaProperty::TEST_GREATER == spAlphaProp->GetTestMode() ||
//		NiAlphaProperty::TEST_GREATEREQUAL == spAlphaProp->GetTestMode())
//	{
//		uiChannelFlag |= (1 << sdRenderParams::E_SHADER_ALPHA_TEST);
//	}
//	else
//	{
//		NIASSERT(0);
//		return;
//	}

//	if (bSkinned)
//	{
//		///< For rimlight
//		if (spMesh->FindStreamRef(NiCommonSemantics::COLOR())) 
//			uiChannelFlag |= (1 << sdRenderParams::E_SHADER_VERTEX_COLOR);
//	}
//	else
//	{
//		///< For detail normal
//		if (spTexProp->GetParallaxMap()) 
//			uiChannelFlag |= (1 << sdRenderParams::E_SHADER_VERTEX_COLOR);		
//	}
	// @}

	// 插入Mesh到绘制队列
	if (bSkinned)
	{
		uiChannelFlag &= m_pkRenderParams->actorChannelMask;
		InsertStaticSceneObject(pkMesh, m_kSkinnedGeometryMaterials[uiChannelFlag], &m_kPropertyList, kShaderParams);
	}
	else
	{
		uiChannelFlag &= m_pkRenderParams->buildingChannelMask;
		InsertStaticSceneObject(pkMesh, m_kStaticGeometryMaterials[uiChannelFlag], &m_kPropertyList, kShaderParams);
	}
}
//-------------------------------------------------------------------------------------------------
void sdMRTGeometryPass::Draw()
{
	D3DPERF_BeginEvent(0xff000000, L"MRTGeometryPass");

	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	// 清除数据绑定
	pkRenderDevice->ClearVertexBinding();
	pkRenderDevice->ClearTextureBinding();

	// CullMode
	pkRenderDevice->SetRenderState(D3DRS_CULLMODE,			D3DCULL_CW);

	// Alpha
	pkRenderDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,	false);
	pkRenderDevice->SetRenderState(D3DRS_ALPHATESTENABLE,	false);

	// Z
	pkRenderDevice->SetRenderState(D3DRS_ZENABLE,			true);
	pkRenderDevice->SetRenderState(D3DRS_ZWRITEENABLE,		true);
	pkRenderDevice->SetRenderState(D3DRS_ZFUNC,				D3DCMP_LESSEQUAL);

	// Stencil
	pkRenderDevice->SetRenderState(D3DRS_STENCILENABLE,		true);
	pkRenderDevice->SetRenderState(D3DRS_STENCILFUNC,		D3DCMP_ALWAYS);
	pkRenderDevice->SetRenderState(D3DRS_STENCILFAIL,		D3DSTENCILOP_KEEP);
	pkRenderDevice->SetRenderState(D3DRS_STENCILZFAIL,		D3DSTENCILOP_KEEP);
	pkRenderDevice->SetRenderState(D3DRS_STENCILPASS,		D3DSTENCILOP_REPLACE);
	pkRenderDevice->SetRenderState(D3DRS_STENCILREF,		m_uiStencilID);
	pkRenderDevice->SetRenderState(D3DRS_STENCILMASK,		0xffffffff);
	pkRenderDevice->SetRenderState(D3DRS_STENCILWRITEMASK,	m_uiStencilIDWriteMask);

	// 设置Texture Sample状态
	//NiDX9Renderer* spRenderer = NiDX9Renderer::GetRenderer();
	//NiDX9RenderState* spRenderState = spRenderer->GetRenderState();

	//bool bAnisotropicFilter = m_pkRenderParams->diffuseAnisotropicOffset > 0.f;
	//uint uiMaxAnisotropy = NiClamp(m_pkRenderParams->diffuseAnisotropicOffset, 1, 16);
	//float fLodBias = m_pkRenderParams->diffuseLodBiasOffset;

	//spRenderState->SetSamplerState(0, D3DSAMP_MINFILTER, bAnisotropicFilter ? D3DTEXF_ANISOTROPIC : D3DTEXF_LINEAR, true);
	//spRenderState->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, bAnisotropicFilter ? uiMaxAnisotropy : 1, true);
	//spRenderState->SetSamplerState(0, D3DSAMP_MIPMAPLODBIAS, *(DWORD*)&fLodBias, true)

	// 绘制
	__super::Draw();

	// 恢复Texture Sample状态
	//spRenderState->RestoreSamplerState(0, D3DSAMP_MINFILTER);
	//spRenderState->RestoreSamplerState(0, D3DSAMP_MAXANISOTROPY);
	//spRenderState->RestoreSamplerState(0, D3DSAMP_MIPMAPLODBIAS);

	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE