#include "sdBasePCH.h"
#include "sdException.h"

#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_DIAGNOSTICS
//-------------------------------------------------------------------------------------------------
const char* sdSEHException::what()
{
	if (!m_lpException)
	{
		static char szSEHExpErr[] = {"SEH Exception"};
		return szSEHExpErr;
	}
	else
	{
		std::string strTemp("SEH Exception Error: ");
		EXCEPTION_RECORD* pkRec = m_lpException->ExceptionRecord;
		switch(pkRec->ExceptionCode)
		{
		case EXCEPTION_ACCESS_VIOLATION:
			strTemp += "EXCEPTION_ACCESS_VIOLATION";
			break;
		case EXCEPTION_DATATYPE_MISALIGNMENT:
			strTemp += "EXCEPTION_DATATYPE_MISALIGNMENT";
			break;
		case EXCEPTION_BREAKPOINT:
			strTemp += "EXCEPTION_BREAKPOINT";
			break;
		case EXCEPTION_SINGLE_STEP:
			strTemp += "EXCEPTION_SINGLE_STEP";
			break;
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
			strTemp += "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
			break;
		case EXCEPTION_FLT_DENORMAL_OPERAND:
			strTemp += "EXCEPTION_FLT_DENORMAL_OPERAND";
			break;
		case EXCEPTION_FLT_DIVIDE_BY_ZERO:
			strTemp += "EXCEPTION_FLT_DIVIDE_BY_ZERO";
			break;
		case EXCEPTION_FLT_INEXACT_RESULT:
			strTemp += "EXCEPTION_FLT_INEXACT_RESULT";
			break;
		case EXCEPTION_FLT_INVALID_OPERATION:
			strTemp += "EXCEPTION_FLT_INVALID_OPERATION";
			break;
		case EXCEPTION_FLT_OVERFLOW:
			strTemp += "EXCEPTION_FLT_OVERFLOW";
			break;
		case EXCEPTION_FLT_STACK_CHECK:
			strTemp += "EXCEPTION_FLT_STACK_CHECK";
			break;
		case EXCEPTION_FLT_UNDERFLOW:
			strTemp += "EXCEPTION_FLT_UNDERFLOW";
			break;
		case EXCEPTION_INT_DIVIDE_BY_ZERO:
			strTemp += "EXCEPTION_INT_DIVIDE_BY_ZERO";
			break;
		case EXCEPTION_INT_OVERFLOW:
			strTemp += "EXCEPTION_INT_OVERFLOW";
			break;
		case EXCEPTION_PRIV_INSTRUCTION:
			strTemp += "EXCEPTION_PRIV_INSTRUCTION";
			break;
		case EXCEPTION_IN_PAGE_ERROR:
			strTemp += "EXCEPTION_IN_PAGE_ERROR";
			break;
		case EXCEPTION_ILLEGAL_INSTRUCTION:
			strTemp += "EXCEPTION_ILLEGAL_INSTRUCTION";
			break;
		case EXCEPTION_NONCONTINUABLE_EXCEPTION:
			strTemp += "EXCEPTION_NONCONTINUABLE_EXCEPTION";
			break;
		case EXCEPTION_STACK_OVERFLOW:
			strTemp += "EXCEPTION_STACK_OVERFLOW";
			break;
		case EXCEPTION_INVALID_DISPOSITION:
			strTemp += "EXCEPTION_INVALID_DISPOSITION";
			break;
		case EXCEPTION_GUARD_PAGE:
			strTemp += "EXCEPTION_GUARD_PAGE";
			break;
		case EXCEPTION_INVALID_HANDLE:
			strTemp += "EXCEPTION_INVALID_HANDLE";
			break;
		}

		return strTemp.c_str();
	}
}
//-------------------------------------------------------------------------------------------------
void SDInvalidParameterHandler(const wchar_t*, const wchar_t*, const wchar_t*, unsigned int, uintptr_t)
{
	SDMakeDump();
	throw std::exception("Invalid Parameter!!!");
}
//-------------------------------------------------------------------------------------------------
void SDPureCallHandler()
{
	SDMakeDump();
	throw std::exception("Pure Call!!!");
}
//-------------------------------------------------------------------------------------------------
void SDMakeDump()
{
	// 抓取SEH上下文
	CONTEXT kContext;
	::RtlCaptureContext(&kContext);

	// 生成Dmup
	EXCEPTION_RECORD kExceptionRec;
	kExceptionRec.ExceptionCode = EXCEPTION_ACCESS_VIOLATION;
	kExceptionRec.ExceptionFlags = EXCEPTION_NONCONTINUABLE;
	kExceptionRec.ExceptionRecord = NULL;
	kExceptionRec.NumberParameters = 1;
	kExceptionRec.ExceptionInformation[0] = 0;

	EXCEPTION_POINTERS kExceptionPtr;
	kExceptionPtr.ContextRecord = &kContext;
	kExceptionPtr.ExceptionRecord = &kExceptionRec;

	SDCatchDumpInfo(&kExceptionPtr);
}
//-------------------------------------------------------------------------------------------------
sdSetExceptionHandler::sdSetExceptionHandler()
{
	::_set_invalid_parameter_handler(SDInvalidParameterHandler);	///< CRT非法参数错误回调
	::_set_purecall_handler(SDPureCallHandler);						///< 纯虚函数错误回调
	::set_terminate(SDMakeDump);									///< 终止函数回调

	sdSEHToExceptionHandler::Instance().InstalllTranslateFunc();
}
//-------------------------------------------------------------------------------------------------
sdSetExceptionHandler::~sdSetExceptionHandler()
{
	sdSEHToExceptionHandler::Instance().UninstallTranslateFunc();
}
//-------------------------------------------------------------------------------------------------
void SDCatchDumpInfo(LPEXCEPTION_POINTERS lpExceptionPtr)
{
	static int iCount = 0;
	if (++iCount > 50)
		return;

	// 创建dump文件夹
	::CreateDirectory(_T("./dump"), NULL);

	// 转储Dump
	SYSTEMTIME tm;
	::GetLocalTime(&tm);

	TCHAR tcDumpName[128] = { 0 };
	wsprintf(tcDumpName, _T("../dump//dumo_%04d_%02d_%02d_%02d_%02d_%02d_%03d.dmp")
		, tm.wYear
		, tm.wMonth
		, tm.wDay
		, tm.wHour
		, tm.wMinute
		, tm.wSecond
		, tm.wMilliseconds);

	HANDLE hDumpFile = ::CreateFile(tcDumpName,
		GENERIC_WRITE, 
		FILE_SHARE_READ,
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);

	if (hDumpFile != INVALID_HANDLE_VALUE)
	{
		MINIDUMP_EXCEPTION_INFORMATION kMiniDumpExceptionInfo;
		kMiniDumpExceptionInfo.ExceptionPointers = lpExceptionPtr;
		kMiniDumpExceptionInfo.ThreadId = ::GetCurrentThreadId();
		kMiniDumpExceptionInfo.ClientPointers = TRUE;

		BOOL bMiniDump =::MiniDumpWriteDump(
			::GetCurrentProcess(), 
			::GetCurrentProcessId(),
			hDumpFile,
			MiniDumpNormal,
			&kMiniDumpExceptionInfo,
			NULL,
			NULL);

		if (bMiniDump)
		{
			HRESULT hr = (HRESULT)::GetLastError();	
			void* pvMsgBuf;
			::FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				hr,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPTSTR)(&pvMsgBuf),
				0,
				NULL);
			::LocalFree(pvMsgBuf);
		}

		::CloseHandle(hDumpFile);
	}
	else
	{
		HRESULT hr = (HRESULT)::GetLastError();	
		void* pvMsgBuf;
		::FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			hr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)(&pvMsgBuf),
			0,
			NULL);
		::LocalFree(pvMsgBuf);
	}	
}
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_DIAGNOSTICS
BASE_NAMESPACE_END_BASE