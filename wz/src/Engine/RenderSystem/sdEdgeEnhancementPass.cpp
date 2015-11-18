#include "sdRenderSystemPCH.h"
#include "sdEdgeEnhancementPass.h"
#include "sdFixedString.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
sdEdgeEnhancementPass::sdEdgeEnhancementPass(sdRenderObjectAlloc* pkAlloc)
: sdRenderPass(pkAlloc)
, m_uiEdgeStencilID(0)
, m_uiEdgeStencilIDMask(0)
{

}
//-------------------------------------------------------------------------------------------------
sdEdgeEnhancementPass::~sdEdgeEnhancementPass()
{
	if (m_bInitialized)
	{
		Destroy();
		m_bInitialized = false;
	}
}
//-------------------------------------------------------------------------------------------------
bool sdEdgeEnhancementPass::Initialize(uint uiEdgeStencilID, uint uiEdgeStencilIDMask, NiRenderedTexture* pkEdgeTexture)
{
	NIASSERT(uiEdgeStencilID & uiEdgeStencilIDMask);
	NIASSERT(pkEdgeTexture);

	if (m_bInitialized)
	{
		NIASSERT(0);
		return false;
	}

	// 设置模版参数
	m_uiEdgeStencilID = uiEdgeStencilID;
	m_uiEdgeStencilIDMask = uiEdgeStencilIDMask;

	// 初始化纹理
	m_spEdgeTexture = pkEdgeTexture;

	// 初始化材质
	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	m_spEdgeMaterial = pkRenderDevice->CreateMaterial("Effect_EdgeEnhancement");

//	// 初始化渲染参数
//	// [center.x][center.y][center.z][radius]
//	// [edge_color.r][edge_color.g][edge_color.b][edge_wdith]
//	float afEdgeParam[8] = 
//	{ 
//		0.f, 0.f, 0.f, 1.f, 
//		1.f, 0.f, 0.f, 3.f, 
//	};
//
//	m_spEdgeParam = NiNew NiFloatsExtraData(8, afEdgeParam);
//	NIASSERT(m_spEdgeParam);

	// 初始化采样参数
	NiRenderTargetGroup* pkRenderTarget = pkRenderDevice->GetDefaultRenderTargetGroup();
	NIASSERT(pkRenderTarget);

	SetShaderParams(pkRenderTarget->GetWidth(0), pkRenderTarget->GetHeight(0));

//	m_spSampleOffsetsParam = NiNew NiFloatsExtraData(26, (float*)(&m_akSampleOffsets[0]));
//	NIASSERT(m_spSampleOffsetsParam);

//	m_spSampleWeightsParam = NiNew NiFloatsExtraData(13, m_afSampleWeights);
//	NIASSERT(m_spSampleWeightsParam);

	// 初始化Mesh
	CreateProxyMesh();

	return (m_bInitialized = true);
}
//-------------------------------------------------------------------------------------------------
void sdEdgeEnhancementPass::Destroy()
{
	m_uiEdgeStencilID = 0;
	m_uiEdgeStencilIDMask = 0;

	m_bInitialized = false;
}
//-------------------------------------------------------------------------------------------------
void sdEdgeEnhancementPass::InsertEdgeEnhancementArea(const NiBound& kBound, const NiColor& kColor, uint uiWidth)
{
	typedef sdRenderObject::ShaderParamItem	ShaderParamItem;

	NIASSERT(m_bInitialized);

	// 构建ShaderParamItem数组,传入对象Material参数
	// @{
	static const sdFixedString s_szEdgeParamName("a_akEdgeParam", true);

	// [center.x][center.y][center.z][radius]
	// [edge_color.r][edge_color.g][edge_color.b][edge_wdith]
	const NiPoint3& kCenter = kBound.GetCenter();
	float fRadius = kBound.GetRadius();

	float afEdgeParam[8] = 
	{ 
		kCenter.x, kCenter.y, kCenter.z, fRadius, 
		kColor.r, kColor.g, kColor.b, (float)uiWidth, 
	};

	ShaderParamItem kShaderParams[] = 
	{
		{&s_szEdgeParamName,	sizeof(afEdgeParam),	(void*)&afEdgeParam},
		{NULL,					0,						NULL}
	};
	// @}

	InsertStaticSceneObject(m_spProxyMesh, m_spEdgeMaterial, NULL, kShaderParams);
}
//-------------------------------------------------------------------------------------------------
void sdEdgeEnhancementPass::Draw()
{
	D3DPERF_BeginEvent(0xff000000, L"EdgeEnhancemengPass");

	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	// 清除数据绑定
	pkRenderDevice->ClearVertexBinding();
	//pkRenderDevice->ClearTextureBinding();

	// 设置渲染状态
	// @{
	pkRenderDevice->SetRenderState(D3DRS_CULLMODE,			D3DCULL_NONE);
	pkRenderDevice->SetRenderState(D3DRS_FILLMODE,			D3DFILL_SOLID);

	pkRenderDevice->SetRenderState(D3DRS_ZENABLE,			false);
	pkRenderDevice->SetRenderState(D3DRS_ZWRITEENABLE,		false);

	pkRenderDevice->SetRenderState(D3DRS_STENCILENABLE,		true);
	pkRenderDevice->SetRenderState(D3DRS_STENCILFUNC,		D3DCMP_NOTEQUAL);
	pkRenderDevice->SetRenderState(D3DRS_STENCILFAIL,		D3DSTENCILOP_KEEP);
	pkRenderDevice->SetRenderState(D3DRS_STENCILZFAIL,		D3DSTENCILOP_KEEP);
	pkRenderDevice->SetRenderState(D3DRS_STENCILPASS,		D3DSTENCILOP_KEEP);
	pkRenderDevice->SetRenderState(D3DRS_STENCILREF,		m_uiEdgeStencilID);
	pkRenderDevice->SetRenderState(D3DRS_STENCILMASK,		m_uiEdgeStencilIDMask);

	pkRenderDevice->SetRenderState(D3DRS_ALPHATESTENABLE,	false);
	pkRenderDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,	false);
	pkRenderDevice->SetRenderState(D3DRS_BLENDOP,			D3DBLENDOP_ADD);
	pkRenderDevice->SetRenderState(D3DRS_SRCBLEND,			D3DBLEND_SRCALPHA);
	pkRenderDevice->SetRenderState(D3DRS_DESTBLEND,			D3DBLEND_INVSRCALPHA);

	pkRenderDevice->SetRenderState(D3DRS_COLORWRITEENABLE,	D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
	// @}

	
	// 设置着色器变量
	// @{
	pkRenderDevice->SetGlobalShaderConstant(
		"a_akSampleOffsets",
		13 * sizeof(NiPoint2),
		m_akSampleOffsets);

	pkRenderDevice->SetGlobalShaderConstant(
		"a_afSampleWeights",
		13 * sizeof(float),
		m_afSampleWeights);
	// @}

	__super::Draw();


	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
void sdEdgeEnhancementPass::CreateProxyMesh()
{
	//
	//	0--1
	//	| /|
	//	|/ |
	//	2--3
	// 
	NiPoint3 akVertice[4];
	akVertice[0] = NiPoint3(-1.f, 1.f, 0.f);
	akVertice[1] = NiPoint3( 1.f, 1.f, 0.f);
	akVertice[2] = NiPoint3(-1.f,-1.f, 0.f);
	akVertice[3] = NiPoint3( 1.f,-1.f, 0.f);

	// 
	ushort ausIndice[6] =
	{
		0, 1, 2, 2, 1, 3
	};
	
	// 
	m_spProxyMesh = NiNew NiMesh();
	NIASSERT(m_spProxyMesh);

	m_spProxyMesh->SetPrimitiveType(NiPrimitiveType::PRIMITIVE_TRIANGLES);

	uint uiNumVertice = 4;
	NiDataStream* pkVerticesStream = NiDataStream::CreateSingleElementDataStream(
		NiDataStreamElement::F_FLOAT32_3,
		uiNumVertice,
		NiDataStream::ACCESS_CPU_WRITE_STATIC | NiDataStream::ACCESS_GPU_READ,
		NiDataStream::USAGE_VERTEX,
		akVertice);
	m_spProxyMesh->AddStreamRef(pkVerticesStream, NiCommonSemantics::POSITION(), 0);

	uint uiNumIndice = 6;
	NiDataStream* pkIndicesStream = NiDataStream::CreateSingleElementDataStream(
		NiDataStreamElement::F_UINT16_1,
		uiNumIndice,
		NiDataStream::ACCESS_CPU_WRITE_STATIC | NiDataStream::ACCESS_GPU_READ,
		NiDataStream::USAGE_VERTEX_INDEX,
		ausIndice);
	m_spProxyMesh->AddStreamRef(pkIndicesStream, NiCommonSemantics::INDEX(), 0);

	//
	NiTexturingPropertyPtr spTexProp = NiNew NiTexturingProperty();
	NIASSERT(spTexProp);
	spTexProp->SetShaderMap(0, NiNew NiTexturingProperty::ShaderMap(m_spEdgeTexture, 0));

	m_spProxyMesh->AttachProperty(spTexProp);
//	m_spProxyMesh->AddExtraData("a_akEdgeParam", m_spEdgeParam);
//	m_spProxyMesh->AddExtraData("a_akSampleOffsets", m_spSampleOffsetsParam);
//	m_spProxyMesh->AddExtraData("a_afSampleWeights", m_spSampleWeightsParam);
	m_spProxyMesh->ApplyAndSetActiveMaterial(m_spEdgeMaterial);

	m_spProxyMesh->UpdateWorldBound();
	m_spProxyMesh->UpdateProperties();
	m_spProxyMesh->UpdateEffects();
	m_spProxyMesh->Update(0.f);
}
//-------------------------------------------------------------------------------------------------
void sdEdgeEnhancementPass::SetShaderParams(uint uiScreenWidth, uint uiScreenHeight)
{
	//
	// 用13个点表示5x5高斯模糊，点阵如下(x是取样点,呈菱形分布)
	//	o o x o o
	//	o x x x o
	//	x x x x o
	//	o x x x o
	//	o o x o o
	//
	float fDivWidth = 1.f / uiScreenWidth;
	float fDivHeight = 1.f / uiScreenHeight;

	int iIndex = 0;
	float fTotalWeight = 0.f;
	for (int x = -2; x <= 2; ++x)
	{
		for (int y = -2; y <= 2; ++y)
		{
			if (abs(x) + abs(y) > 2)
				continue;

			m_akSampleOffsets[iIndex].x = x * fDivWidth;
			m_akSampleOffsets[iIndex].y = y * fDivHeight;

			m_afSampleWeights[iIndex] = GaussianDistribution((float)x, (float)y, 1.f);
			fTotalWeight += m_afSampleWeights[iIndex];

			++iIndex;
		}
	}

	float fMultiplier = 1.f;
	for (int i = 0; i < iIndex; ++i)
	{
		m_afSampleWeights[iIndex] /= fTotalWeight;
		m_afSampleWeights[iIndex] *= fMultiplier;
	}
}
//-------------------------------------------------------------------------------------------------
float sdEdgeEnhancementPass::GaussianDistribution(float x, float y, float rho)
{
	//
	// 正态分布
	// 参考： 
	//	http://en.wikipedia.org/wiki/Normal_distribution
	//	http://www.cplusplus.com/reference/random/normal_distribution/
	//
	float g = 1.f / (rho * sqrtf(2.f * NI_PI));
	g *= expf(-(x * x + y * y) / (2.f * rho * rho));

	return g;
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE