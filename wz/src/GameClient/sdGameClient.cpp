#include "Stdafx.h"
#include "sdGameClient.h"
#include <sdFileUtil.h>
#include <NiDX9SystemDesc.h>
#include <sdCameraState.h>

using namespace Base;
using namespace Base::Util;
using namespace RenderSystem;
using namespace GameCore;
//-------------------------------------------------------------------------------------------------
LRESULT CALLBACK sdGameClient::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Ĭ����Ϣ����
	sdGameClient* pkGameClient = sdGameClient::InstancePtr();
	if (!pkGameClient || !pkGameClient->IsInitialized())
	{
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	// �Զ����������Ϣ����
	LRESULT lRresult = 0;
	switch (uMsg)
	{
		//case WM_INPUTLANGCHANGEREQUEST: 
		//	break;

		//case WM_SETCURSOR:
		//	break;

		// ���ڹر�ʱ,������Ϣ˳����WM_CLOSE,WM_DESTROY,WM_QUIT
		//case WM_CLOSE:
		//	break;

		// ��������,����WM_QUIT��Ϣ
		case WM_DESTROY:
			::PostQuitMessage(0);
			break;

		// ���ڼ���
		case WM_ACTIVATE:
		{
			// (��β���)
			DWORD dwLastProcessId = 0;
			HWND hWndDeactivated = (HWND)lParam;
			::GetWindowThreadProcessId(hWndDeactivated, &dwLastProcessId);
			if (dwLastProcessId != ::GetCurrentProcessId())
			{
				//WA_ACTIVE
				//WA_CLICKACTIVE
				//WA_INACTIVE
				pkGameClient->SetActive(wParam == WA_ACTIVE || wParam == WA_CLICKACTIVE);
			}
		}


		//case WM_ACTIVATEAPP:

		// �������С����Ϣ
		case WM_SYSCOMMAND:
		{
			if (wParam == SC_MINIMIZE)
			{
				// ��С��
			}
			else if (wParam == SC_MAXIMIZE)
			{
				// ���
			}
		}

		// ϵͳ������Ϣ
		//case WM_SYSKEYDOWN:
		//	if (wParam == VK_MENU)
		//		return 0;
		//	else if (wParam == VK_F10)
		//		return 0;
		//	break;
	}

	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}
//-------------------------------------------------------------------------------------------------
bool sdGameClient::OnLostDevice(void* pData)
{
	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdGameClient::OnResetDevice(bool bBefore, void* pData)
{
	return true;
}
//-------------------------------------------------------------------------------------------------
sdGameClient::sdGameClient()
: m_hInstance(0)
, m_hWnd(0)
, m_bActive(true)
{
	m_bInitialize = false;
	m_bRunning = false;
}
//-------------------------------------------------------------------------------------------------
sdGameClient::~sdGameClient()
{

}
//-------------------------------------------------------------------------------------------------
bool sdGameClient::Initialize(HINSTANCE hInstance)
{
	// ģ����
	m_hInstance = hInstance;

	// ��������
	if (!CreateGameWindow())
	{
		::MessageBox(NULL, "��������ʧ��", "����", MB_OK);
		return false;
	}

	// ��ʼ��Gamebryo
	//sdEngineAllocator kEngineAllocator;
	//NiInitOptions kInitOptions(kEngineAllocator);
	//NiInit(spInitOptions);

	//NiInitOptions* spInitOptions = NiExternalNew NiInitOptions;
	//spInitOptions->SetFloodgateParallelExecution(false);
	//NiInit(spInitOptions);

	NiInit();


	// ����Gamebryo��Ⱦ��
	if (!CreateGameRenderer())
	{
		::MessageBox(NULL, "������Ⱦ��ʧ��", "����", MB_OK);
		return false;
	}

	// ��ʼ��WZ��Դϵͳ
	m_kResourceSystem.Initialize();

	// ��ʼ��WZ��Ⱦϵͳ
	m_kRenderSystem.Initialize();


	//*************************************************
	// ��ʱ����
	sdWorld* pkWorld = NiNew sdWorld("world0", "world0");
	NIASSERT(pkWorld);
	m_kWorldManager.SetCurrentWorld(pkWorld);

	m_pkMap = NiNew sdMap;
	NIASSERT(m_pkMap);
	m_pkMap->LoadScene("xinshoucun");

	m_kRenderSystem.SetMap((sdMap*)m_pkMap);
	//*************************************************


	// ��ʼ�������豸
	m_kInputSystem.Initialize(m_hInstance, m_hWnd);

	// ��ʼ�����ϵͳ
	// @{
	m_kCameraFSM.Initialize();

	sdCameraFreeState* pkCameraFreeState = new sdCameraFreeState;
	m_kCameraFSM.AddState(pkCameraFreeState);

	m_kCameraFSM.SetState(sdCameraState::E_CAMERA_FREE);
	m_kRenderSystem.SetCamera(m_kCameraFSM.GetCamera());
	// @}

	return (m_bInitialize = true), (m_bRunning = true);
}
//-------------------------------------------------------------------------------------------------
void sdGameClient::Destroy()
{
	using namespace RenderSystem;

	// �������ϵͳ
	m_kCameraFSM.Destroy();

	// ���������豸
	m_kInputSystem.Destroy();

	// ����WZ��Ⱦϵͳ
	m_kRenderSystem.Destroy();

	// ����WZ��Դϵͳ
	m_kResourceSystem.Destroy();

	// ����Gamebryo��Ⱦ��
	m_spRenderer = 0;

	// ����Gamebryo
	NiShutdown();

	// ���ٴ���
	::DestroyWindow(m_hWnd);
}
//-------------------------------------------------------------------------------------------------
bool sdGameClient::Run()
{
	// ������Ϣ�ַ�
	MSG kMsg;
	if (::PeekMessage(&kMsg, NULL, 0, 0, PM_REMOVE))
	{
		// �˳���Ϣ,�������˳����
		if (kMsg.message == WM_QUIT)
			return (m_bRunning = false);

		// ��ͨ��Ϣ����зַ�
		if (!::TranslateAccelerator(m_hWnd, NULL, &kMsg))
		{
			::TranslateMessage(&kMsg);
			::DispatchMessage(&kMsg);
		}
	}

	// ��֡

	// ��ʱ�����
	{
		//
		//MeasureTime();

		// �߼�����(ʼ����ÿ�벻����30FPS)
		UpdateFrame();

		// ��Ⱦ����ʾ
		RenderFrame();

		//
		//MeasureTime();
	}

	return m_bRunning;
}
//-------------------------------------------------------------------------------------------------
bool sdGameClient::CreateGameWindow()
{
	// ��Ϸ�ֱ���
	sdGameConfig* pkGameConfig = sdGameConfig::InstancePtr();
	sdVideoConfig* pkVideoConfig = &(pkGameConfig->m_kVideoConfig);
	unsigned int uiWidth = pkVideoConfig->m_uiWidth;
	unsigned int uiHeight = pkVideoConfig->m_uiHeight;
	bool bFullScreen = pkVideoConfig->m_bFullScreen;

	// ��Ļ�ֱ���
	HDC hDC = ::GetDC(NULL);
	unsigned int uiScreenWidth = ::GetDeviceCaps(hDC, HORZRES);
	unsigned int uiScreenHeight = ::GetDeviceCaps(hDC, VERTRES);

	// ������
	HINSTANCE hInstance = (HINSTANCE)::GetCurrentThread();

	// ���ڱ���
	static const char* lpszName = "WorldZero";

	// ע�ᴰ����
	WNDCLASS wc;
	::ZeroMemory(&wc, sizeof(WNDCLASS));
	wc.style			= CS_HREDRAW | CS_HREDRAW | CS_DBLCLKS;	// ������ʽ
	wc.lpfnWndProc		= sdGameClient::WndProc;				// ���ڻص�����
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	wc.hIcon			= ::LoadIcon(NULL, IDI_ERROR);
	wc.hCursor			= ::LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)::GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName		= 0;
	wc.lpszClassName	= lpszName;
	::RegisterClass(&wc);

	// ���ݿͻ����ߴ�,������ʽ���㴰�ڳߴ�,λ��
	RECT rect = {0, 0, uiWidth, uiHeight};
	DWORD dwStyle = 0;
	if (bFullScreen)
		dwStyle = WS_POPUP;
	else
		dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_SIZEBOX;
	::AdjustWindowRect(&rect, dwStyle, FALSE);

	unsigned int uiWindowWidth = rect.right - rect.left;
	unsigned int uiWindowHeight = rect.bottom - rect.top;

	if (!bFullScreen)
	{
		// ����״̬��Ҫ��ȥ���������������߶�
		if (uiWindowWidth > uiScreenWidth)		uiWindowWidth = uiScreenWidth;
		if (uiWindowHeight > uiScreenHeight)	uiWindowHeight = uiScreenHeight;
	}

	unsigned int uiX = (uiScreenWidth - uiWindowWidth) / 2;
	unsigned int uiY = (uiScreenHeight - uiWindowHeight) / 2;

	m_uiWindowWidth = uiWindowWidth;
	m_uiWindowHeight = uiWindowHeight;

	// ��������
	m_hWnd = ::CreateWindow(lpszName, lpszName,
		dwStyle,
		uiX, uiY,
		uiWindowWidth, uiWindowHeight,
		NULL, NULL,
		hInstance,
		NULL);
	if (!m_hWnd)
		return false;

	// ���ش���
	::ShowWindow(m_hWnd, SW_HIDE);

	// ���´���
	::UpdateWindow(m_hWnd);

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdGameClient::CreateGameRenderer()
{
	sdGameConfig* pkGameConfig = sdGameConfig::InstancePtr();
	sdVideoConfig* pkVideoConfig = &(pkGameConfig->m_kVideoConfig);
	bool bVSync = pkVideoConfig->m_bVSync;

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

	// ��ȡ��Ⱦ���ڿͻ����ߴ�
	RECT rect;
	::GetClientRect(m_hWnd, &rect);
	unsigned int uiWidth = rect.right - rect.left;
	unsigned int uiHeight = rect.bottom - rect.top;

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
		bVSync ? NiDX9Renderer::PRESENT_INTERVAL_ONE : NiDX9Renderer::PRESENT_INTERVAL_IMMEDIATE,
		NiDX9Renderer::SWAPEFFECT_DEFAULT,
		NiDX9Renderer::FBMODE_DEFAULT);

	if (!m_spRenderer)
		return false;

	// �����豸�ص�����
	m_spRenderer->AddLostDeviceNotificationFunc(OnLostDevice, 0);
	m_spRenderer->AddResetNotificationFunc(OnResetDevice, 0);
	m_spRenderer->SetMipmapSkipLevel(pkVideoConfig->m_uiMipmapSkipLevel);

	return true;
}
//-------------------------------------------------------------------------------------------------
void sdGameClient::UpdateFrame()
{
	// sdPF-Func_Auto;

	// ����ʱ��
	m_kTimeMgr.Update();

	// ��������
	m_kInputSystem.SetActive(m_bActive);
	m_kInputSystem.Update();

	// �������
	m_kCameraFSM.UpdateState();
}
//-------------------------------------------------------------------------------------------------
void sdGameClient::RenderFrame()
{
	//sdPF_Func_Auto;

	// ò����˼���ڷǴ�����С��ʱ����Ⱦ
	if (!::IsIconic(m_hWnd))
	{
		m_kRenderSystem.RenderFrame();
		m_kRenderSystem.DisplayFrame();
	}
}
//-------------------------------------------------------------------------------------------------
void sdGameClient::SetActive(bool bActive)
{
	m_bActive = bActive;

	// ʧȥ���ý���Ĵ���(������Ƶ)
}
//-------------------------------------------------------------------------------------------------