#include "Stdafx.h"
#include "sdGameClient.h"
#include "resource.h"

// ȫ�ֱ���
sdGameClient* g_pkGameClient = NULL;

//-------------------------------------------------------------------------------------------------
// �쳣����
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
// ���������
//-------------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	//
	// VS2005֮��İ汾,΢��������һЩ�µ��쳣����,�»����ڳ��ִ���ʱĬ�ϲ�֪ͨӦ�ó���
	// ��ʱ����ͱ�����.�������������,�������_set_invalid_parameter_handler��
	// _set_purecall_handler�����Լ����쳣��������
	//
	// _set_invalid_parameter_handler	
	//	���C����ʱ������������Ч�Դ��ݸ����ǡ������Ч�������ݣ��ù��ܿ�������errno�����
	//	�򷵻ش�����롣����������£�������Ч���������á��˹�������ô����������Ϊ��һ������
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

	// Hook Windows�ڴ���亯��
	//HookMemoryFunction();

	// vld
	//__VLD_THREEAD_NAME;
	//__VLD_THREAD_TRACE_AUTO;

	// profileͳ��ע��
	//sdPF_Func_Register()

	// ��Ϸ������־
	::CreateMutex(NULL, TRUE, "wzclient");

	// Ӳ����������
	// CheckSystem()

	// ע��CRT�˳��ص�����
	// atexit(ExitDump)

	// ���ض��߳���Ⱦ��
	//HMODULE hMTDirectX9 = NULL;
	//if (true)
	//{
	//	std::vector<std::string> kCheckProcessVec;
	//	kCheckProcessVec.push_back("fraps.exe");	///< ����fraps.exeʱ���������߳���Ⱦ
	//	if (!IsProcessRunning(sdUtil::IsProcessRunning(kCheckProcessVec)))
	//		hMTDirectX9 = ::LoadLibrary("MTDirectX9.dll");
	//}

	// �������߳̽�ɫ����

	// ���ؽ�����
	//g_pkLoadingDlg = new sdLoadingDlg;
	//g_pkLoadingDlg->Initialize();

	// �ͻ��˳�ʼ��
	g_pkGameClient = new sdGameClient;
	assert(g_pkGameClient);
	g_pkGameClient->Initialize(hInstance);

	// ���ٽ�����
	//g_pkLoadingDlg->Destroy();
	//delete g_pkLoadingDlg;
	//g_pkLoadingDlg = NULL;

	// ��ʾ�ͻ��˴���
	::ShowWindow(g_pkGameClient->GetWindowHandle(), SW_SHOWNORMAL);
	::UpdateWindow(g_pkGameClient->GetWindowHandle());

	// �ͻ�������ѭ��
	while(g_pkGameClient->Run());
	
	// �ͻ�������
	g_pkGameClient->Destroy();
	delete g_pkGameClient;
	g_pkGameClient = NULL;

	// ж�ض��߳���Ⱦ��
	//if (hMTDirectX9 != NULL)
	//{
	//	::FreeLibrary(hMTDirectX9);
	//	hMTDirectX9 = NULL;
	//}

	// �ͻ��˽�������������֮���
	//::CreateProcess();

	return 0;
};
//-------------------------------------------------------------------------------------------------