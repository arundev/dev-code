#include "sdRenderSystemPCH.h"
#include "sdDebugGizmoPass.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
sdDebugGizmoPass::sdDebugGizmoPass(sdRenderObjectAlloc* pkAlloc)
: sdRenderPass(pkAlloc)
{
	
}
//-------------------------------------------------------------------------------------------------
sdDebugGizmoPass::~sdDebugGizmoPass()
{
	if (m_bInitialized)
	{
		Destroy();
		m_bInitialized = false;
	}
}
//-------------------------------------------------------------------------------------------------
bool sdDebugGizmoPass::Initialize()
{
	if (m_bInitialized)
	{
		NIASSERT(0);
		return false;
	}

	return (m_bInitialized = true);
}
//-------------------------------------------------------------------------------------------------
void sdDebugGizmoPass::Destroy()
{
	m_bInitialized = false;
}
//-------------------------------------------------------------------------------------------------
void sdDebugGizmoPass::InsertStaticMesh(NiMesh* spMesh)
{
	NIASSERT(m_bInitialized);
	NIASSERT(spMesh);

	// 加入渲染队列
	InsertStaticSceneObject(spMesh, NULL, NULL, NULL);
}
//-------------------------------------------------------------------------------------------------
void sdDebugGizmoPass::Draw()
{
	if (!m_bInitialized || !m_bActived)
		return;


	D3DPERF_BeginEvent(0xff000000, L"DebugGizmoPass");

	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	// 清除数据绑定
	pkRenderDevice->ClearVertexBinding();
	//pkRenderDevice->ClearTextureBinding();

	// 绘制
	__super::Draw();

	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE