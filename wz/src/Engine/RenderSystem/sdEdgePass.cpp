#include "sdRenderSystemPCH.h"
#include "sdEdgePass.h"
#include "sdFixedString.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
sdEdgePass::sdEdgePass(sdRenderObjectAlloc* pkAlloc)
: sdRenderPass(pkAlloc)
, m_uiEdgeStencilID(0)
, m_uiEdgeStencilIDMask(0)
, m_uiEdgeStencilIDWriteMask(0)
{

}
//-------------------------------------------------------------------------------------------------
sdEdgePass::~sdEdgePass()
{
	if (m_bInitialized)
	{
		Destroy();
		m_bInitialized = false;
	}
}
//-------------------------------------------------------------------------------------------------
bool sdEdgePass::Initialize(uint uiEdgeStencilID, uint uiEdgeStencilIDMask, uint uiEdgeStencilIDWriteMask)
{
	NIASSERT(uiEdgeStencilID & uiEdgeStencilIDMask);

	if (m_bInitialized)
	{
		NIASSERT(0);
		return false;
	}

	// 设置模版参数
	m_uiEdgeStencilID = uiEdgeStencilID;
	m_uiEdgeStencilIDMask = uiEdgeStencilIDMask;
	m_uiEdgeStencilIDWriteMask = uiEdgeStencilIDWriteMask;

	// 初始化材质
	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	char szMaterialName[512];
	uint uiNum = 8;
	m_kMaterialVec.resize(uiNum);
	for (uint i = 0; i < uiNum; ++i)
	{
		sprintf(szMaterialName, "Effect_Edge0x%08x", i);
		m_kMaterialVec[i] = pkRenderDevice->CreateMaterial(szMaterialName);
	}

	return (m_bInitialized = true);
}
//-------------------------------------------------------------------------------------------------
void sdEdgePass::Destroy()
{
	m_uiEdgeStencilID = 0;
	m_uiEdgeStencilIDMask = 0;
	m_uiEdgeStencilIDWriteMask = 0;

	m_kMaterialVec.clear();

	m_bInitialized = false;
}
//-------------------------------------------------------------------------------------------------
void sdEdgePass::InsertEdgeMesh(NiMesh* pkMesh)
{
	typedef sdRenderObject::ShaderParamItem	ShaderParamItem;

	NIASSERT(m_bInitialized);
	NIASSERT(pkMesh);

	// 构建ShaderParamItem数组,传入对象Material参数
	// @{
	static const sdFixedString s_szAlphaTestName("g_fAlphaTestRef",	true);

	float fAlphaTestRef = 0.f;
	NiPropertyState* pkPropState = pkMesh->GetPropertyState();
	if (pkPropState)
	{
		NiAlphaProperty* pkAlphaProp = pkPropState->GetAlpha();
		if (pkAlphaProp && pkAlphaProp->GetAlphaTesting())
		{
			uint uiAlphaTestRef = pkAlphaProp->GetTestRef();
			fAlphaTestRef = uiAlphaTestRef / 255.f + 0.001f;
		}
	}

	ShaderParamItem kShaderParams[] = 
	{
		{&s_szAlphaTestName,	sizeof(float),	(void*)&fAlphaTestRef},
		{NULL,					0,				NULL}
	};
	// @}
	
	// 根据Mesh情况,选择合适材质
	//#define VERTEX_COMPACTVERTEX_FLAG			0x00000001	///< 顶点压缩
	//#define VERTEX_EHARDWARESKIN_FLAG			0x00000002	///< 骨骼
	//#define VERTEX_LEAFANIM_FLAG				0x00000004	///< 树叶顶点动画
	// @{
	uint uiChannelFlag = 0;

	bool bCompactVertex = !pkMesh->FindStreamRef(NiCommonSemantics::BINORMAL()) && !pkMesh->FindStreamRef(NiCommonSemantics::TEXCOORD());
	if (bCompactVertex)
		uiChannelFlag |= (1 << 0);

	bool bSkinned = pkMesh->RequiresMaterialOption(NiRenderObjectMaterialOption::TRANSFORM_SKINNED());
	if (bSkinned)
		uiChannelFlag |= (1 << 1);

	bool bLeaf = pkMesh->GetName().Contains("WaveLeaf");
	if (bLeaf)
		uiChannelFlag |= (1 << 2);
	// @}

	InsertStaticSceneObject(pkMesh, m_kMaterialVec[uiChannelFlag], NULL, kShaderParams);
}
//-------------------------------------------------------------------------------------------------
void sdEdgePass::Draw()
{
	D3DPERF_BeginEvent(0xff000000, L"EdgePass");

	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	// 清除数据绑定
	pkRenderDevice->ClearVertexBinding();
	//pkRenderDevice->ClearTextureBinding();

	// 设置渲染状态
	// @{
	pkRenderDevice->SetRenderState(D3DRS_CULLMODE,			D3DCULL_CCW);
	pkRenderDevice->SetRenderState(D3DRS_FILLMODE,			D3DFILL_SOLID);

	pkRenderDevice->SetRenderState(D3DRS_ZENABLE,			false);
	pkRenderDevice->SetRenderState(D3DRS_ZWRITEENABLE,		false);
	pkRenderDevice->SetRenderState(D3DRS_ZFUNC,				D3DCMP_EQUAL);

	pkRenderDevice->SetRenderState(D3DRS_STENCILENABLE,		true);
	pkRenderDevice->SetRenderState(D3DRS_STENCILFUNC,		D3DCMP_ALWAYS);
	pkRenderDevice->SetRenderState(D3DRS_STENCILFAIL,		D3DSTENCILOP_KEEP);
	pkRenderDevice->SetRenderState(D3DRS_STENCILZFAIL,		D3DSTENCILOP_KEEP);
	pkRenderDevice->SetRenderState(D3DRS_STENCILPASS,		D3DSTENCILOP_REPLACE);
	pkRenderDevice->SetRenderState(D3DRS_STENCILREF,		m_uiEdgeStencilID);
	pkRenderDevice->SetRenderState(D3DRS_STENCILMASK,		m_uiEdgeStencilIDMask);
	pkRenderDevice->SetRenderState(D3DRS_STENCILWRITEMASK,	m_uiEdgeStencilIDWriteMask);

	pkRenderDevice->SetRenderState(D3DRS_ALPHATESTENABLE,	false);
	pkRenderDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,	false);

	pkRenderDevice->SetRenderState(D3DRS_COLORWRITEENABLE,	D3DCOLORWRITEENABLE_RED);
	// @}

	// 绘制
	__super::Draw();

	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE