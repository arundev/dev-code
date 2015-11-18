#include "sdRenderSystemPCH.h"
#include "sdTerrainDepthPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
bool sdTerrainDepthPass::Comparator(sdRenderObject* lhs, sdRenderObject* rhs)
{
	NIASSERT(lhs);
	NIASSERT(rhs);
	NIASSERT(sdRenderObject::E_ROT_STATIC_MESH == lhs->GetType());
	NIASSERT(sdRenderObject::E_ROT_STATIC_MESH == rhs->GetType());

	// 距离排序(由近到远)
	// (这里貌似无用,有待改进,因为TileMesh没有被位移,位移在Shader里面进行)
	NiCamera kCamera;
	NiRenderer::GetRenderer()->GetCameraData(kCamera);
	const NiPoint3& kCamPos = kCamera.GetWorldLocation();
	const NiPoint3& kCamDir = kCamera.GetWorldDirection();
	const NiPoint3& lhsPos = lhs->GetMesh()->GetTranslate();
	const NiPoint3& rhsPos = rhs->GetMesh()->GetTranslate();
	return kCamDir.Dot(lhsPos - kCamPos) < kCamDir.Dot(rhsPos - kCamPos);
}
//-------------------------------------------------------------------------------------------------
sdTerrainDepthPass::sdTerrainDepthPass(sdRenderObjectAlloc* pkAlloc)
: sdRenderPass(pkAlloc)
, m_uiTerrainStencilID(0)
, m_uiTerrainStencilIDWriteMask(0)
{

}
//-------------------------------------------------------------------------------------------------
sdTerrainDepthPass::~sdTerrainDepthPass()
{
	if (m_bInitialized)
	{
		Destroy();
		m_bInitialized = false;
	}
}
//-------------------------------------------------------------------------------------------------
bool sdTerrainDepthPass::Initialize(uint uiTerrainStencilID, uint uiTerrainStencilIDWriteMask)
{
	NIASSERT(uiTerrainStencilID & uiTerrainStencilIDWriteMask);

	if (m_bInitialized)
	{
		NIASSERT(0);
		return false;
	}

	//
	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	// 设置模版参数
	m_uiTerrainStencilID = uiTerrainStencilID;
	m_uiTerrainStencilIDWriteMask = uiTerrainStencilIDWriteMask;

	// 初始化材质
	m_spTerrainMaterial = pkRenderDevice->CreateMaterial("Terrain_Depth");
	NIASSERT(m_spTerrainMaterial);

	return (m_bInitialized = true);
}
//-------------------------------------------------------------------------------------------------
void sdTerrainDepthPass::Destroy()
{
	m_uiTerrainStencilID = 0;
	m_uiTerrainStencilIDWriteMask = 0;

	m_spTerrainMaterial = 0;

	m_bInitialized = false;
}
//-------------------------------------------------------------------------------------------------
void sdTerrainDepthPass::InsertTerrainMesh(NiMesh* spMesh)
{
	NIASSERT(m_bInitialized);
	NIASSERT(spMesh);

	// 加入渲染队列
	InsertStaticSceneObject(spMesh, m_spTerrainMaterial, NULL, NULL);
}
//-------------------------------------------------------------------------------------------------
void sdTerrainDepthPass::Draw()
{
	if (!m_bInitialized || !m_bActived)
		return;

	D3DPERF_BeginEvent(0xff000000, L"TerrainDepthPass");

	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	// 清除数据绑定
	pkRenderDevice->ClearVertexBinding();
	pkRenderDevice->ClearTextureBinding();

	// CullMode
	pkRenderDevice->SetRenderState(D3DRS_CULLMODE,			D3DCULL_CCW);

	// FillMode
	pkRenderDevice->SetRenderState(D3DRS_FILLMODE,			D3DFILL_SOLID);

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
	pkRenderDevice->SetRenderState(D3DRS_STENCILREF,		m_uiTerrainStencilID);
	pkRenderDevice->SetRenderState(D3DRS_STENCILMASK,		0xffffffff);
	pkRenderDevice->SetRenderState(D3DRS_STENCILWRITEMASK,	m_uiTerrainStencilIDWriteMask);

	// 允许第一个颜色缓存输出
	uint uiColorChannelMask = D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN |	D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA;
	pkRenderDevice->SetRenderState(D3DRS_COLORWRITEENABLE,	uiColorChannelMask);

	// 绘制
	__super::Draw();

	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
sdRenderPass::RenderObjectComparator sdTerrainDepthPass::GetComparator()
{
	return sdTerrainDepthPass::Comparator;
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE