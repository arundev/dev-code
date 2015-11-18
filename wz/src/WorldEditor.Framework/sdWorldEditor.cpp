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
	// ���ô��ھ��
	m_hWnd = hWnd;

	// ��ʼ��Gamebryo
	NiInitOptions* spInitOptions = NiExternalNew NiInitOptions;
	spInitOptions->SetFloodgateParallelExecution(false);
	NiInit(spInitOptions);

	// �����ļ�ϵͳ
	Engine::sdFile::StaticInitialize();
	m_pkFileSystem = new sdWpfFileSystem;
	m_pkFileSystem->Initialize();

	// ���ö�̬Shader��Ĭ������Ŀ¼
	char szWorkingPath[MAX_PATH_LEN];
	Base::Util::sdFileUtil::GetModulePath(szWorkingPath);

	std::string szWorkingDir = std::string(szWorkingPath) + "\\dependence\\Shader\\Generated";
	NiMaterial::SetDefaultWorkingDirectory(szWorkingDir.c_str());

	// ����PerHUD�豸,������ʹ��֮
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

	// ��ȡ������Ϣ
	RECT kRect;
	::GetClientRect(m_hWnd, &kRect);
	uint uiWidth = kRect.right - kRect.left;
	uint uiHeight = kRect.bottom - kRect.top;

	// ����Gamebryo�豸
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

	// ��ʼ����Դϵͳ
	m_pkResourceSystem = NiNew sdResourceSystem;
	m_pkResourceSystem->Initialize();

	// ��ʼ����Ⱦϵͳ
	m_pkRenderSystem = new sdRenderSystem_DX9;
	m_pkRenderSystem->Initialize();

	// ��ʼ�����ϵͳ
	// @{
	m_kCameraFSM.Initialize();

	sdEditCameraState* pkEditFreeCamState = new sdEditFreeCameraState;
	m_kCameraFSM.AddState(pkEditFreeCamState);

	m_kCameraFSM.SetState(sdEditCameraState::E_EDIT_CAMERA_FREE);
	m_pkRenderSystem->SetCamera(m_kCameraFSM.GetCamera());
	// @}

	// ��ʼ����Ϣ����
	m_kEventMgr.Initialize(NUM_EDITEVENTS);


	//*************************************************
	// ��ʱ����
	sdWorld* pkWorld = NiNew sdWorld("world0", "edata\\world0\\");
	NIASSERT(pkWorld);
	m_kWorldManager.SetCurrentWorld(pkWorld);

	m_pkMap = m_kWorldManager.LoadScene("xinshoucun", NULL);
	NIASSERT(m_pkMap);

	m_pkRenderSystem->SetMap((sdMap*)m_pkMap);
	//*************************************************


	// ��ʼ���༭״̬��
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
		// ����ʱ��
		m_kTimeMgr.Update();

		// ��Դ
		m_pkResourceSystem->Update();

		// �������
		m_kCameraFSM.UpdateState();

		// ���µ�ͼ
		m_pkMap->UpdateGame();

		// ���±༭״̬��
		m_kWorldEditFSM.UpdateState();

		// ��Ⱦ
		m_pkRenderSystem->RenderFrame_Begin();
		m_pkRenderSystem->RenderFrame_Internal();
		m_kWorldEditFSM.GetEditGizmoHelper()->RenderGizmo();	///< ��ʱ���
		m_pkRenderSystem->RenderFrame_End();

		// ��ʾ
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