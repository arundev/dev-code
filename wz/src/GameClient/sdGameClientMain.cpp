#include "Stdafx.h"
#include "sdGameClient.h"
#include "resource.h"

// 全局变量
sdGameClient* g_pkGameClient = NULL;

//-------------------------------------------------------------------------------------------------
// 异常处理
//-------------------------------------------------------------------------------------------------
class sdOtherException : public std::exception
{
};

inline void MakeDump()
{
	throw sdOtherException();
}

inline void InvalidParameterHandler(const wchar_t*, const wchar_t*, const wchar_t*, uint, uintptr_t)
{
	MakeDump();
}
//-------------------------------------------------------------------------------------------------
// 主程序入口
//-------------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	//
	// VS2005之后的版本,微软增加了一些新的异常机制,新机制在出现错误时默认不通知应用程序，
	// 这时程序就崩溃了.所以这种情况下,必须调用_set_invalid_parameter_handler、
	// _set_purecall_handler设置自己的异常处理函数。
	//
	// _set_invalid_parameter_handler	
	//	许多C运行时函数检查参数有效性传递给它们。如果无效参数传递，该功能可以设置errno错误号
	//	或返回错误代码。在这种情况下，参数无效处理程序调用。此功能允许该处理程序设置为另一个函数
	//
	// _set_purecall_handler
	//	Error-Handling Routines, Sets the handler for a pure virtual function call.
	//
	// set_terminate
	//	Exception-Handling Functions, Install your own termination routine to be called by terminate
	//
	_set_invalid_parameter_handler(InvalidParameterHandler);
	_set_purecall_handler(MakeDump);
	set_terminate(MakeDump);

	// Hook Windows内存分配函数
	//HookMemoryFunction();

	// vld
	//__VLD_THREEAD_NAME;
	//__VLD_THREAD_TRACE_AUTO;

	// profile统计注册
	//sdPF_Func_Register()

	// 游戏启动标志
	::CreateMutex(NULL, TRUE, "wzclient");

	// 硬件与软件检查
	// CheckSystem()

	// 注册CRT退出回调函数
	// atexit(ExitDump)

	// 加载多线程渲染库
	//HMODULE hMTDirectX9 = NULL;
	//if (true)
	//{
	//	std::vector<std::string> kCheckProcessVec;
	//	kCheckProcessVec.push_back("fraps.exe");	///< 运行fraps.exe时不开启多线程渲染
	//	if (!IsProcessRunning(sdUtil::IsProcessRunning(kCheckProcessVec)))
	//		hMTDirectX9 = ::LoadLibrary("MTDirectX9.dll");
	//}

	// 开启多线程角色更新

	// 加载进度条
	//g_pkLoadingDlg = new sdLoadingDlg;
	//g_pkLoadingDlg->Initialize();

	// 客户端初始化
	g_pkGameClient = new sdGameClient;
	assert(g_pkGameClient);
	g_pkGameClient->Initialize(hInstance);

	// 销毁进度条
	//g_pkLoadingDlg->Destroy();
	//delete g_pkLoadingDlg;
	//g_pkLoadingDlg = NULL;

	// 显示客户端窗口
	::ShowWindow(g_pkGameClient->GetWindowHandle(), SW_SHOWNORMAL);
	::UpdateWindow(g_pkGameClient->GetWindowHandle());

	// 客户端运行循环
	while(g_pkGameClient->Run());
	
	// 客户端销毁
	g_pkGameClient->Destroy();
	delete g_pkGameClient;
	g_pkGameClient = NULL;

	// 卸载多线程渲染库
	//if (hMTDirectX9 != NULL)
	//{
	//	::FreeLibrary(hMTDirectX9);
	//	hMTDirectX9 = NULL;
	//}

	// 客户端结束后调用浏览器之类的
	//::CreateProcess();

	return 0;
};
//-------------------------------------------------------------------------------------------------