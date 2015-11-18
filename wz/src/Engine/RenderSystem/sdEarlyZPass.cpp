#include "sdRenderSystemPCH.h"
#include "sdEarlyZPass.h"
#include "sdFixedString.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
bool sdEarlyZPass::Comparator(sdRenderObject* lhs, sdRenderObject* rhs)
{
	NIASSERT(lhs);
	NIASSERT(rhs);
	NIASSERT(sdRenderObject::E_ROT_STATIC_MESH == lhs->GetType());
	NIASSERT(sdRenderObject::E_ROT_STATIC_MESH == rhs->GetType());

	// 键排序(材质)
	if (lhs->GetType() != rhs->GetType())
		return (uint)lhs->GetType() < (uint)rhs->GetType();
	
	// 距离排序(由近到远,也许并不必要)
	NiCamera kCamera;
	NiRenderer::GetRenderer()->GetCameraData(kCamera);
	const NiPoint3& kCamPos = kCamera.GetWorldLocation();
	const NiPoint3& kCamDir = kCamera.GetWorldDirection();
	const NiPoint3& lhsPos = lhs->GetMesh()->GetTranslate();
	const NiPoint3& rhsPos = rhs->GetMesh()->GetTranslate();
	return kCamDir.Dot(lhsPos - kCamPos) < kCamDir.Dot(rhsPos - kCamPos);
}
//-------------------------------------------------------------------------------------------------
sdEarlyZPass::sdEarlyZPass(sdRenderObjectAlloc* pkAlloc)
: sdRenderPass(pkAlloc)
, m_uiStaticMeshStencilID(0)
, m_uiStaticMeshStencilIDMask(0)
{
	
}
//-------------------------------------------------------------------------------------------------
sdEarlyZPass::~sdEarlyZPass()
{
	if (m_bInitialized)
	{
		Destroy();
		m_bInitialized = false;
	}
}
//-------------------------------------------------------------------------------------------------
bool sdEarlyZPass::Initialize(uint uiStaticMeshStencilID, uint uiStaticMeshStencilIDMask)
{
	NIASSERT(uiStaticMeshStencilID & uiStaticMeshStencilIDMask);

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

	// 初始化材质
	m_spSolidMeshZMaterial = pkRenderDevice->CreateMaterial("StaticMesh_Solid_EarlyZ");
	NIASSERT(m_spSolidMeshZMaterial);
	
	m_spNormalMapAlphaTestZMaterial = pkRenderDevice->CreateMaterial("StaticMesh_NormalMap_EarlyZ");
	NIASSERT(m_spNormalMapAlphaTestZMaterial);
	
	m_spDiffuseMapAlphaTestZMaterial = pkRenderDevice->CreateMaterial("StaticMesh_DiffuseMap_EarlyZ");
	NIASSERT(m_spDiffuseMapAlphaTestZMaterial);
	
	return (m_bInitialized = true);
}
//-------------------------------------------------------------------------------------------------
void sdEarlyZPass::Destroy()
{
	m_uiStaticMeshStencilID = 0;
	m_uiStaticMeshStencilIDMask = 0;
	
	m_spSolidMeshZMaterial = 0;
	m_spNormalMapAlphaTestZMaterial = 0;
	m_spDiffuseMapAlphaTestZMaterial = 0;

	m_bInitialized = false;
}
//-------------------------------------------------------------------------------------------------
void sdEarlyZPass::InsertStaticMesh(NiMesh* spMesh)
{
	NIASSERT(m_bInitialized);
	NIASSERT(spMesh);

	// 检查是否是骨骼动画
	if (spMesh->RequiresMaterialOption(NiRenderObjectMaterialOption::TRANSFORM_SKINNED()))
	{
		// RenderPath should not insert skinmesh to this pass
		//NIASSERT(0);
		return;
	}

	// 检查是否是实例化几何体
	if (spMesh->RequiresMaterialOption(NiRenderObjectMaterialOption::TRANSFORM_INSTANCED()))
	{
		//NIASSERT(0);
		return;
	}
	
	NiPropertyState* spPropertyState = spMesh->GetPropertyState();
	if (!spPropertyState)
	{
		NIASSERT(0);
		return;
	}
	
	NiAlphaProperty* spAlphaProp = spPropertyState->GetAlpha();
	if (!spAlphaProp || spAlphaProp->GetAlphaBlending())
	{
		// Alpha blending mesh should never be inserted to here
		//NIASSERT(0);
		return;	
	}

	NiWireframeProperty* spWireframeProp = spPropertyState->GetWireframe();
	if (!spWireframeProp || spWireframeProp->GetWireframe())
	{
		// Wireframe mesh should never be inserted to here
		//NIASSERT(0);
		return;	
	}
		
	NiTexturingProperty* spTexturingProp = spPropertyState->GetTexturing();
	if (!spTexturingProp)
	{
		//NIASSERT(0);
		return;	
	}

	// 加入渲染队列
	NiTexturingProperty::Map* spDifffuseMap = spTexturingProp->GetBaseMap();
	NiTexturingProperty::Map* spNormalMap = spTexturingProp->GetBaseMap();
	if (spAlphaProp->GetAlphaTesting())
	{
		if (spAlphaProp->GetTestMode() != NiAlphaProperty::TEST_GREATER &&
			spAlphaProp->GetTestMode() != NiAlphaProperty::TEST_GREATEREQUAL)
		{
			// Only support greater alpha test. in most case, weird tricks are not good
			//NIASSERT(0);
			return;		
		}

		static const sdFixedString szAlphaTestName("g_fAlphaTestRef", true);
		float fAlphaTest = spAlphaProp->GetTestRef() / 255.f + 0.001f;
		sdRenderObject::ShaderParamItem	kShaderParams[] = 
		{
			{&szAlphaTestName, sizeof(float), (void*)&fAlphaTest},
			{NULL, 0, NULL},
		};

		if (spNormalMap)
		{
			InsertStaticSceneObject(spMesh, m_spNormalMapAlphaTestZMaterial, NULL, NULL);
		}
		else if (spDifffuseMap)
		{
			InsertStaticSceneObject(spMesh, m_spDiffuseMapAlphaTestZMaterial, NULL, NULL);
		}
		else
		{
			//NIASSERT(0);
			return;	
		}
		 
	}
	else
	{
		InsertStaticSceneObject(spMesh, m_spSolidMeshZMaterial, NULL, NULL);
	}
}
//-------------------------------------------------------------------------------------------------
void sdEarlyZPass::Draw()
{
	if (!m_bInitialized || !m_bActived)
		return;

	D3DPERF_BeginEvent(0xff000000, L"EarlyZPass");

	IRenderDevice* pkRenderDevice = IRenderDevice::GetRenderDevice();
	NIASSERT(pkRenderDevice);

	// 清除数据绑定
	pkRenderDevice->ClearVertexBinding();
	pkRenderDevice->ClearTextureBinding();

	// Alpha
	pkRenderDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,	false);
	pkRenderDevice->SetRenderState(D3DRS_ALPHATESTENABLE,	false);

	// Z
	pkRenderDevice->SetRenderState(D3DRS_ZENABLE,			true);
	pkRenderDevice->SetRenderState(D3DRS_ZWRITEENABLE,		true);
	pkRenderDevice->SetRenderState(D3DRS_ZFUNC,				D3DCMP_LESSEQUAL);

	// Stencil
	if (m_uiStaticMeshStencilID)
	{
		pkRenderDevice->SetRenderState(D3DRS_STENCILENABLE,		true);
		pkRenderDevice->SetRenderState(D3DRS_STENCILFUNC,		D3DCMP_ALWAYS);
		pkRenderDevice->SetRenderState(D3DRS_STENCILFAIL,		D3DSTENCILOP_KEEP);
		pkRenderDevice->SetRenderState(D3DRS_STENCILZFAIL,		D3DSTENCILOP_KEEP);
		pkRenderDevice->SetRenderState(D3DRS_STENCILPASS,		D3DSTENCILOP_REPLACE);
		pkRenderDevice->SetRenderState(D3DRS_STENCILREF,		m_uiStaticMeshStencilID);
		pkRenderDevice->SetRenderState(D3DRS_STENCILWRITEMASK,	m_uiStaticMeshStencilIDMask);
	}
	else
	{
		pkRenderDevice->SetRenderState(D3DRS_STENCILENABLE,		false);
	}

	// 禁用第一个颜色缓存,以获取双倍输出
	// Then the most important one: Disable color output. Get double speed on ROP
	pkRenderDevice->SetRenderState(D3DRS_COLORWRITEENABLE,	0);

	// 绘制
	__super::Draw();

	// 恢复第一个颜色输出
	// Re-enable color output
	uint uiColorChannelMask = D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN |	D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA;
	pkRenderDevice->SetRenderState(D3DRS_COLORWRITEENABLE,	uiColorChannelMask);

	D3DPERF_EndEvent();
}
//-------------------------------------------------------------------------------------------------
sdRenderPass::RenderObjectComparator sdEarlyZPass::GetComparator()
{
	return sdEarlyZPass::Comparator;
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE