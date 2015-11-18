#include "WorldEditor.FrameworkPCH.h"
#include "sdWorldEditor.h"

//
#include <NiDX9Renderer.h>
#include <NiDX9SystemDesc.h>

//
#include <sdFile.h>
#include <sdFileUtil.h>

//
#include <sdRenderSystem_DX9.h>
#include <sdResourceSystem.h>
#include <sdWpfFileSystem.h>

#include <sdCameraEditState.h>
#include <sdEditMode.h>
#include <sdEditEventType.h>

using namespace Base;
using namespace Engine::RenderSystem;
using namespace Engine::ResourceSystem;
using namespace Engine::FileSystem;
using namespace GameCore;
using namespace GameEditEx;
//-------------------------------------------------------------------------------------------------
sdWorldEditor::sdWorldEditor()
{
	m_bInitialized = false;
	m_hWnd = NULL;
	m_spRenderer = NULL;
}
//-------------------------------------------------------------------------------------------------
sdWorldEditor::~sdWorldEditor()
{

}
//-------------------------------------------------------------------------------------------------
bool sdWorldEditor::Initialize(HWND hWnd)
{
	// 设置窗口句柄
	m_hWnd = hWnd;

	// 初始化Gamebryo
	NiInitOptions* spInitOptions = NiExternalNew NiInitOptions;
	spInitOptions->SetFloodgateParallelExecution(false);
	NiInit(spInitOptions);

	// 设置文件系统
	Engine::sdFile::StaticInitialize();
	m_pkFileSystem = new sdWpfFileSystem;
	m_pkFileSystem->Initialize();

	// 设置动态Shader的默认生成目录
	char szWorkingPath[MAX_PATH_LEN];
	Base::Util::sdFileUtil::GetModulePath(szWorkingPath);

	std::string szWorkingDir = std::string(szWorkingPath) + "\\dependence\\Shader\\Generated";
	NiMaterial::SetDefaultWorkingDirectory(szWorkingDir.c_str());

	// 查找PerHUD设备,存在则使用之
	const NiDX9SystemDesc* spDesc = NiDX9Renderer::GetSystemDesc();
	NiDX9Renderer::DeviceDesc eDevType = NiDX9Renderer::DEVDESC_PURE;
	unsigned int uiAdapter = D3DADAPTER_DEFAULT;
	unsigned int uiAdapterCount = spDesc->GetAdapterCount();
	for (unsigned int i = 0; i < uiAdapterCount; ++i)
	{
		const NiDX9AdapterDesc* spAdapter = spDesc->GetAdapter(i);
		const char* pcAdapterName = spAdapter->GetStringDescription();
		if (strstr(pcAdapterName, "NVIDIA PerfHUD"))
		{
			uiAdapter = i;
			eDevType = NiDX9Renderer::DEVDESC_REF_HWVERTEX;
			break;
		}
	}

	// 获取窗口信息
	RECT kRect;
	::GetClientRect(m_hWnd, &kRect);
	uint uiWidth = kRect.right - kRect.left;
	uint uiHeight = kRect.bottom - kRect.top;

	// 创建Gamebryo设备
	NIASSERT(!m_spRenderer);
	m_spRenderer = NiDX9Renderer::Create(
		uiWidth, uiHeight,
		NiDX9Renderer::USE_MULTITHREADED,
		m_hWnd, m_hWnd,
		uiAdapter,
		eDevType,
		NiDX9Renderer::FBFMT_X8R8G8B8,
		NiDX9Renderer::DSFMT_D24S8,
		NiDX9Renderer::PRESENT_INTERVAL_IMMEDIATE,
		NiDX9Renderer::SWAPEFFECT_DEFAULT,
		NiDX9Renderer::FBMODE_DEFAULT);
	NIASSERT(m_spRenderer);
	m_spRenderer->IncRefCount();

	// 初始化资源系统
	m_pkResourceSystem = NiNew sdResourceSystem;
	m_pkResourceSystem->Initialize();

	// 初始化渲染系统
	m_pkRenderSystem = new sdRenderSystem_DX9;
	m_pkRenderSystem->Initialize();

	// 初始化相机系统
	// @{
	m_kCameraFSM.Initialize();

	sdEditCameraState* pkEditFreeCamState = new sdEditFreeCameraState;
	m_kCameraFSM.AddState(pkEditFreeCamState);

	m_kCameraFSM.SetState(sdEditCameraState::E_EDIT_CAMERA_FREE);
	m_pkRenderSystem->SetCamera(m_kCameraFSM.GetCamera());
	// @}

	// 初始化消息中心
	m_kEventMgr.Initialize(NUM_EDITEVENTS);


	//*************************************************
	// 临时代码
	sdWorld* pkWorld = NiNew sdWorld("world0", "edata\\world0\\");
	NIASSERT(pkWorld);
	m_kWorldManager.SetCurrentWorld(pkWorld);

	m_pkMap = m_kWorldManager.LoadScene("xinshoucun", NULL);
	NIASSERT(m_pkMap);

	m_pkRenderSystem->SetMap((sdMap*)m_pkMap);
	//*************************************************


	// 初始化编辑状态机
	m_kWorldEditFSM.Initialize();

	return (m_bInitialized = true);
}
//-------------------------------------------------------------------------------------------------
void sdWorldEditor::Destroy()
{
	m_bInitialized = false;
}
//-------------------------------------------------------------------------------------------------
void sdWorldEditor::Update()
{
	if (m_bInitialized)
	{
		// 更新时间
		m_kTimeMgr.Update();

		// 资源
		m_pkResourceSystem->Update();

		// 更新相机
		m_kCameraFSM.UpdateState();

		// 更新地图
		m_pkMap->UpdateGame();

		// 更新编辑状态机
		m_kWorldEditFSM.UpdateState();

		// 渲染
		m_pkRenderSystem->RenderFrame_Begin();
		m_pkRenderSystem->RenderFrame_Internal();
		m_kWorldEditFSM.GetEditGizmoHelper()->RenderGizmo();	///< 临时解决
		m_pkRenderSystem->RenderFrame_End();

		// 显示
		m_pkRenderSystem->DisplayFrame();
	}
}
//-------------------------------------------------------------------------------------------------
bool sdWorldEditor::SaveScene()
{
	return sdWorldManager::Instance().SaveScene(m_pkMap, "xinshoucun", NULL);
}
//-------------------------------------------------------------------------------------------------
bool sdWorldEditor::OpenScene()
{
	//return m_pkMap->GetTerrain()->LoadScene("E:\\project_game\\edata\\world0\\scene\\xinshoucun");
	return false;
}
//-------------------------------------------------------------------------------------------------