#include "sdRenderSystemPCH.h"
#include "sdRenderSystem_DX9.h"
#include "sdRenderDevice_DX9.h"
#include "sdRenderPath_DX9.h"

#include <sdFileUtil.h>
#include <sdMap.h>
#include <sdTerrainSystem.h>

#include <NiMeshCullingProcess.h>

using namespace std;
using namespace Base;
using namespace Engine::ResourceSystem;

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
sdRenderSystem_DX9::sdRenderSystem_DX9()
: m_bInitialized(false)
, m_pkRenderDevice(NULL)
, m_pkRenderPath(NULL)
, m_pkMap(NULL) 
{

}
//-------------------------------------------------------------------------------------------------
sdRenderSystem_DX9::~sdRenderSystem_DX9()
{
	if (m_bInitialized)
	{
		Destroy();
		m_bInitialized = false;
	}
}
//-------------------------------------------------------------------------------------------------
bool sdRenderSystem_DX9::Initialize()
{
	// �Ѿ���ʼ����ֱ�ӷ���
	if (m_bInitialized) 
	{
		NIASSERT(0);
		return false;
	}

	// GB��Ⱦ�豸û�г�ʼ����ֱ�ӷ���
	m_spDX9Renderer = NiDX9Renderer::GetRenderer();
	NIASSERT(m_spDX9Renderer);
		
	// ����sd��Ⱦ�豸,����ʼ��
	m_pkRenderDevice = new sdRenderDevice_DX9;
	if (!m_pkRenderDevice || !m_pkRenderDevice->Initialize())
	{
		NIASSERT(0);
		Destroy();
		return false;
	}

	//
	IResourceSystem* pkResourceSystem = IResourceSystem::GetResourceSystem();
	NIASSERT(pkResourceSystem);

	// ��ʼ����ɫ��
	// @{
	// Shader��Ԥ�����
#ifdef _DEBUG
	m_spDX9Renderer->SetShaderCreationFlags("fx", D3DXSHADER_DEBUG);
	m_spDX9Renderer->AddGlobalMacro("_DEBUG", "1");
#else
	m_spDX9Renderer->SetShaderCreationFlags("fx", D3DXSHADER_OPTIMIZATION_LEVEL3);
	m_spDX9Renderer->AddGlobalMacro("_RELEASE", "1");
#endif

	//m_spDX9Renderer->AddGlobalMacro("_SD_GAME", "1");
	m_spDX9Renderer->AddGlobalMacro("_SD_EDITOR", "1");

	char szModulePath[MAX_PATH_LEN];
	Base::Util::sdFileUtil::GetModulePath(szModulePath);

	string szProgramPath = string(szModulePath) + "\\..\\dependence\\engine\\shader\\DX9";
	string szShaderPath = string(szModulePath) + "\\..\\dependence\\engine\\shader";
	const char* apcProgramDirectories[1] = { szProgramPath.c_str()};
	const char* apcShaderDirectories[1] = { szShaderPath.c_str()};

	NiD3DXEffectFactory::SetD3DXEffectDirectory(apcProgramDirectories[0]);
	m_pkRenderDevice->SetupShaderSystem(apcProgramDirectories, 1, apcShaderDirectories, 1);
	// @}


	// ����Ĭ����Դ 
	// @{
	// ����Ĭ�ϼ�����
	m_spScreenQuadMesh = CreateScreenQuadMesh();
	m_spUnitCubeMesh = CreateUnitCubeMesh();
	m_spUnitSphereMesh = CreateUnitSphereMesh();
	m_spUnitConeMesh = CreateUnitConeMesh(8);
	m_spUnitPyramidMesh = CreatePyramidMesh();

	// ����Ĭ�ϲ���
	char* szMaterialNames[NUM_DEFAULT_MATERIALS] = 
	{ 
		"Misc_StaticMesh_FlatShading", 
		"Misc_StaticMesh_WireframeShading",
		"Misc_SkinnedMesh_FlatShading",
		"Misc_SkinnedMesh_WireframeShading",
	};

	for (int i = 0; i < NUM_DEFAULT_MATERIALS; ++i)
	{
		m_spDefaultMaterials[i] = m_pkRenderDevice->CreateMaterial(szMaterialNames[i]);
	}

	// ����Ĭ������
	char* szTextureNames[NUM_DEFAULT_TEXTURES] =
	{
		"dependence\\engine\\texture\\error.dds",
		"dependence\\engine\\texture\\black.dds",
		"dependence\\engine\\texture\\gray75.dds",
		"dependence\\engine\\texture\\white.dds",
		"dependence\\engine\\texture\\chessboard.dds",
		"dependence\\engine\\texture\\up_normal.dds",
		"dependence\\engine\\texture\\random_normal.dds",
	};

	for (int i = 0; i < NUM_DEFAULT_TEXTURES; ++i)
	{
		m_spDefaultTextures[i] = pkResourceSystem->LoadTexture(szTextureNames[i]);
	}
	// }@


	// ������Ⱦ·��
	m_pkRenderPath = NiNew sdRenderPath_DX9;
	NIASSERT(m_pkRenderPath);
	m_pkRenderPath->Initialize();
	
	// ��ȡĬ����ȾĿ��
	m_spRenderTargetGroup = m_pkRenderDevice->GetDefaultRenderTargetGroup();
	NIASSERT(m_spRenderTargetGroup);

	// ����Ĭ�����


	// ����ֱ�ӻ��Ƶķ�������
	// @{
	m_spDirectCullingProcess = NiNew NiMeshCullingProcess(&m_kDirectVisibleArray, NULL);
	NIASSERT(m_spDirectCullingProcess);

	m_spDirectAccumulator = NiNew NiAlphaAccumulator;
	NIASSERT(m_spDirectAccumulator);

	//m_spAlphaSortProcessor = NiNew NiAlphaSortProcessor;
	//NIASSERT(m_spAlphaSortProcessor);
	// @}

	return (m_bInitialized = true);
}
//-------------------------------------------------------------------------------------------------
void sdRenderSystem_DX9::Destroy()
{
	// �Ѿ���������ֱ�ӷ���
	if (!m_bInitialized)
		return;

	//
	
	m_bInitialized = false;
}
//-------------------------------------------------------------------------------------------------
bool sdRenderSystem_DX9::IsInitialized()
{
	return m_bInitialized;
}
//-------------------------------------------------------------------------------------------------
void sdRenderSystem_DX9::UpdateFrame(float fAccumTime, float fFrameTime)
{

}
//-------------------------------------------------------------------------------------------------
void sdRenderSystem_DX9::RenderFrame()
{
	NIASSERT(m_spDX9Renderer);

	//sdPF_Func_Auto;

	//sdEntity::TickSysRenderFrameId();

	// ����豸��ʧ
	if (m_spDX9Renderer->IsDeviceLost())
	{
		return;
	}

	// ���浱ǰ��ȾĿ��
	//NiRenderTargetGroupPtr spCurRenderTarget = spDX9Renderer->GetCurrentRenderTargetGroup();

	// ��ʼ��Ⱦ֡
	if (!m_spDX9Renderer->GetInsideFrameState())
		m_spDX9Renderer->BeginFrame();

	// ��ʼ����Ⱦ֡
	D3DPERF_BeginEvent(0xffff0000, L"MainFrame");
	DrawOpaqueUI();			///< ���Ʋ�͸��UI
	DrawScene();			///< ���Ƴ���
	DrawTranslucentUI();	///< ����͸��UI
	D3DPERF_EndEvent();		

	// ������Ⱦ֡
	if (m_spDX9Renderer->GetInsideFrameState())
		m_spDX9Renderer->EndFrame();

	// �ָ�֮ǰ��ȾĿ��
	//if (spCurRenderTarget)
	//{
	//	if (spDX9Renderer->GetCurrentRenderTargetGroup())
	//		spDX9Renderer->EndUsingRenderTargetGroup;
	//	spDX9Renderer->BeginUsingRenderTargetGroup(spCurRenderTarget, NiRenderer::CLEAR_NONE);
	//}
}
//-------------------------------------------------------------------------------------------------
void sdRenderSystem_DX9::RenderFrame_Begin()
{
	NIASSERT(m_spDX9Renderer);
	if (!m_spDX9Renderer->GetInsideFrameState())
		m_spDX9Renderer->BeginFrame();
}
//-------------------------------------------------------------------------------------------------
void sdRenderSystem_DX9::RenderFrame_Internal()
{
	NIASSERT(m_spDX9Renderer);

	D3DPERF_BeginEvent(0xffff0000, L"MainFrame");
	DrawOpaqueUI();			///< ���Ʋ�͸��UI
	DrawScene();			///< ���Ƴ���
	DrawTranslucentUI();	///< ����͸��UI
	D3DPERF_EndEvent();		
}
//-------------------------------------------------------------------------------------------------
void sdRenderSystem_DX9::RenderFrame_End()
{
	NIASSERT(m_spDX9Renderer);
	if (m_spDX9Renderer->GetInsideFrameState())
		m_spDX9Renderer->EndFrame();
}
//-------------------------------------------------------------------------------------------------
void sdRenderSystem_DX9::DisplayFrame()
{
	NIASSERT(m_spDX9Renderer);
	m_spDX9Renderer->DisplayFrame();	///< ��ʾ
}
//-------------------------------------------------------------------------------------------------
void sdRenderSystem_DX9::DrawOpaqueUI()
{

}
//-------------------------------------------------------------------------------------------------
void sdRenderSystem_DX9::DrawTranslucentUI()
{ 
	
}
//-------------------------------------------------------------------------------------------------
void sdRenderSystem_DX9::DrawScene()
{
	//sdPF_Func_Auto

	// ��鳡��/���/��ȾĿ��/��Ⱦ�豸/��Ⱦ·����׼�����
	if (!m_pkMap)		return;
	if (!m_spCamera)	return;
	if (!m_spRenderTargetGroup)	return;
	if (!m_pkRenderDevice || !m_pkRenderDevice->IsInitialized())	return;
	if (!m_pkRenderPath || !m_pkRenderPath->IsInitialized())		return;

	// �ӳ�����ȡ��Ⱦ����,�����µ���Ⱦ��
	// @{
	const sdRenderParams& kRenderParams = m_pkMap->GetRenderParams();
	m_pkRenderPath->UpdateRenderParams(kRenderParams);

	const sdEnvironmentParams& kEnvironmentParams = m_pkMap->GetEnvironmentParams();
	m_pkRenderPath->UpdateEnvironmentParams(kEnvironmentParams);

	const sdPostProcessParams& kPostProcessParams = m_pkMap->GetPostProcessParams();
	m_pkRenderPath->UpdatePostProcessParams(kPostProcessParams);

	sdTerrain* pkTerrainSystem = m_pkMap->GetTerrain();
	if (pkTerrainSystem && pkTerrainSystem->IsVisible())
	{
		const sdTerrainParams& kTerrainParams = pkTerrainSystem->GetTerrainParams();
		m_pkRenderPath->UpdateTerrainParams(kTerrainParams);
	}
	// @}

	// ��Ⱦ
	m_pkRenderPath->RenderScene(m_pkMap, m_spCamera, m_spRenderTargetGroup);
}
//-------------------------------------------------------------------------------------------------
void sdRenderSystem_DX9::BeginDirectDraw(NiCamera* pkCamera)
{
	// �������
	m_spDirectCamera = 0;
	if (pkCamera)
		m_spDirectCamera = pkCamera;
	NIASSERT(m_spDirectCamera);

	// ��ʼ��Ⱦ֡
	if (!m_spDX9Renderer->GetInsideFrameState())
		m_spDX9Renderer->BeginFrame();

	// ������Ⱦ��
	m_spDX9Renderer->SetCameraData(m_spDirectCamera);		///< �����������Ⱦ��
	m_spDX9Renderer->SetSorter(m_spDirectAccumulator);		///< ��������������Ⱦ��
	m_spDX9Renderer->GetSorter()->StartAccumulating(m_spDirectCamera);

	m_kDirectVisibleArray.RemoveAll();
}
//-------------------------------------------------------------------------------------------------
void sdRenderSystem_DX9::DirectDraw(NiAVObject* pkObject)
{
	NIASSERT(pkObject);
	m_spDirectCullingProcess->Cull(m_spDirectCamera, pkObject, &m_kDirectVisibleArray);
}
//-------------------------------------------------------------------------------------------------
void sdRenderSystem_DX9::DirectDrawImmediate(NiAVObject* pkObject)
{
	NIASSERT(pkObject);
	m_spDirectCullingProcess->Cull(m_spDirectCamera, pkObject, &m_kDirectVisibleArray);
	NiDrawVisibleArrayAppend(m_kDirectVisibleArray);
	m_kDirectVisibleArray.RemoveAll();
}
//-------------------------------------------------------------------------------------------------
void sdRenderSystem_DX9::EndDirectDraw()
{
	// ��Ⱦ
	NiDrawVisibleArrayAppend(m_kDirectVisibleArray);
	m_kDirectVisibleArray.RemoveAll();
	m_spDX9Renderer->GetSorter()->FinishAccumulating();

	// ������Ⱦ֡
	if (m_spDX9Renderer->GetInsideFrameState())
		m_spDX9Renderer->EndFrame();
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE