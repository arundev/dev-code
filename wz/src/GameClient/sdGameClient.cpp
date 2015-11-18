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
	// 默认消息处理
	sdGameClient* pkGameClient = sdGameClient::InstancePtr();
	if (!pkGameClient || !pkGameClient->IsInitialized())
	{
		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	// 自定义的特殊消息处理
	LRESULT lRresult = 0;
	switch (uMsg)
	{
		//case WM_INPUTLANGCHANGEREQUEST: 
		//	break;

		//case WM_SETCURSOR:
		//	break;

		// 窗口关闭时,三个消息顺序是WM_CLOSE,WM_DESTROY,WM_QUIT
		//case WM_CLOSE:
		//	break;

		// 窗口销毁,发送WM_QUIT消息
		case WM_DESTROY:
			::PostQuitMessage(0);
			break;

		// 窗口激活
		case WM_ACTIVATE:
		{
			// (这段不解)
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

		// 最大化与最小化消息
		case WM_SYSCOMMAND:
		{
			if (wParam == SC_MINIMIZE)
			{
				// 最小化
			}
			else if (wParam == SC_MAXIMIZE)
			{
				// 最大化
			}
		}

		// 系统按键消息
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
	// 模块句柄
	m_hInstance = hInstance;

	// 创建窗口
	if (!CreateGameWindow())
	{
		::MessageBox(NULL, "创建窗口失败", "错误", MB_OK);
		return false;
	}

	// 初始化Gamebryo
	//sdEngineAllocator kEngineAllocator;
	//NiInitOptions kInitOptions(kEngineAllocator);
	//NiInit(spInitOptions);

	//NiInitOptions* spInitOptions = NiExternalNew NiInitOptions;
	//spInitOptions->SetFloodgateParallelExecution(false);
	//NiInit(spInitOptions);

	NiInit();


	// 创建Gamebryo渲染器
	if (!CreateGameRenderer())
	{
		::MessageBox(NULL, "创建渲染器失败", "错误", MB_OK);
		return false;
	}

	// 初始化WZ资源系统
	m_kResourceSystem.Initialize();

	// 初始化WZ渲染系统
	m_kRenderSystem.Initialize();


	//*************************************************
	// 临时代码
	sdWorld* pkWorld = NiNew sdWorld("world0", "world0");
	NIASSERT(pkWorld);
	m_kWorldManager.SetCurrentWorld(pkWorld);

	m_pkMap = NiNew sdMap;
	NIASSERT(m_pkMap);
	m_pkMap->LoadScene("xinshoucun");

	m_kRenderSystem.SetMap((sdMap*)m_pkMap);
	//*************************************************


	// 初始化输入设备
	m_kInputSystem.Initialize(m_hInstance, m_hWnd);

	// 初始化相机系统
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

	// 销毁相机系统
	m_kCameraFSM.Destroy();

	// 销毁输入设备
	m_kInputSystem.Destroy();

	// 销毁WZ渲染系统
	m_kRenderSystem.Destroy();

	// 销毁WZ资源系统
	m_kResourceSystem.Destroy();

	// 销毁Gamebryo渲染器
	m_spRenderer = 0;

	// 销毁Gamebryo
	NiShutdown();

	// 销毁窗口
	::DestroyWindow(m_hWnd);
}
//-------------------------------------------------------------------------------------------------
bool sdGameClient::Run()
{
	// 窗口消息分发
	MSG kMsg;
	if (::PeekMessage(&kMsg, NULL, 0, 0, PM_REMOVE))
	{
		// 退出消息,则设置退出标记
		if (kMsg.message == WM_QUIT)
			return (m_bRunning = false);

		// 普通消息则进行分发
		if (!::TranslateAccelerator(m_hWnd, NULL, &kMsg))
		{
			::TranslateMessage(&kMsg);
			::DispatchMessage(&kMsg);
		}
	}

	// 限帧

	// 计时与更新
	{
		//
		//MeasureTime();

		// 逻辑更新(始终是每秒不高于30FPS)
		UpdateFrame();

		// 渲染与显示
		RenderFrame();

		//
		//MeasureTime();
	}

	return m_bRunning;
}
//-------------------------------------------------------------------------------------------------
bool sdGameClient::CreateGameWindow()
{
	// 游戏分辨率
	sdGameConfig* pkGameConfig = sdGameConfig::InstancePtr();
	sdVideoConfig* pkVideoConfig = &(pkGameConfig->m_kVideoConfig);
	unsigned int uiWidth = pkVideoConfig->m_uiWidth;
	unsigned int uiHeight = pkVideoConfig->m_uiHeight;
	bool bFullScreen = pkVideoConfig->m_bFullScreen;

	// 屏幕分辨率
	HDC hDC = ::GetDC(NULL);
	unsigned int uiScreenWidth = ::GetDeviceCaps(hDC, HORZRES);
	unsigned int uiScreenHeight = ::GetDeviceCaps(hDC, VERTRES);

	// 程序句柄
	HINSTANCE hInstance = (HINSTANCE)::GetCurrentThread();

	// 窗口标题
	static const char* lpszName = "WorldZero";

	// 注册窗口类
	WNDCLASS wc;
	::ZeroMemory(&wc, sizeof(WNDCLASS));
	wc.style			= CS_HREDRAW | CS_HREDRAW | CS_DBLCLKS;	// 窗口样式
	wc.lpfnWndProc		= sdGameClient::WndProc;				// 窗口回调函数
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= hInstance;
	wc.hIcon			= ::LoadIcon(NULL, IDI_ERROR);
	wc.hCursor			= ::LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)::GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName		= 0;
	wc.lpszClassName	= lpszName;
	::RegisterClass(&wc);

	// 根据客户区尺寸,窗口样式计算窗口尺寸,位置
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
		// 窗口状态下要减去标题栏和任务栏高度
		if (uiWindowWidth > uiScreenWidth)		uiWindowWidth = uiScreenWidth;
		if (uiWindowHeight > uiScreenHeight)	uiWindowHeight = uiScreenHeight;
	}

	unsigned int uiX = (uiScreenWidth - uiWindowWidth) / 2;
	unsigned int uiY = (uiScreenHeight - uiWindowHeight) / 2;

	m_uiWindowWidth = uiWindowWidth;
	m_uiWindowHeight = uiWindowHeight;

	// 创建窗口
	m_hWnd = ::CreateWindow(lpszName, lpszName,
		dwStyle,
		uiX, uiY,
		uiWindowWidth, uiWindowHeight,
		NULL, NULL,
		hInstance,
		NULL);
	if (!m_hWnd)
		return false;

	// 隐藏窗口
	::ShowWindow(m_hWnd, SW_HIDE);

	// 更新窗口
	::UpdateWindow(m_hWnd);

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdGameClient::CreateGameRenderer()
{
	sdGameConfig* pkGameConfig = sdGameConfig::InstancePtr();
	sdVideoConfig* pkVideoConfig = &(pkGameConfig->m_kVideoConfig);
	bool bVSync = pkVideoConfig->m_bVSync;

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

	// 获取渲染窗口客户区尺寸
	RECT rect;
	::GetClientRect(m_hWnd, &rect);
	unsigned int uiWidth = rect.right - rect.left;
	unsigned int uiHeight = rect.bottom - rect.top;

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
		bVSync ? NiDX9Renderer::PRESENT_INTERVAL_ONE : NiDX9Renderer::PRESENT_INTERVAL_IMMEDIATE,
		NiDX9Renderer::SWAPEFFECT_DEFAULT,
		NiDX9Renderer::FBMODE_DEFAULT);

	if (!m_spRenderer)
		return false;

	// 设置设备回调函数
	m_spRenderer->AddLostDeviceNotificationFunc(OnLostDevice, 0);
	m_spRenderer->AddResetNotificationFunc(OnResetDevice, 0);
	m_spRenderer->SetMipmapSkipLevel(pkVideoConfig->m_uiMipmapSkipLevel);

	return true;
}
//-------------------------------------------------------------------------------------------------
void sdGameClient::UpdateFrame()
{
	// sdPF-Func_Auto;

	// 更新时间
	m_kTimeMgr.Update();

	// 更新输入
	m_kInputSystem.SetActive(m_bActive);
	m_kInputSystem.Update();

	// 更新相机
	m_kCameraFSM.UpdateState();
}
//-------------------------------------------------------------------------------------------------
void sdGameClient::RenderFrame()
{
	//sdPF_Func_Auto;

	// 貌似意思是在非窗口最小化时才渲染
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

	// 失去与获得焦点的处理(例如音频)
}
//-------------------------------------------------------------------------------------------------