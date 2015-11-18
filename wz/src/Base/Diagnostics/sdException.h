//*************************************************************************************************
// 内容:	错误句柄处理(参考WZClient::Global/WZServers::common/SGDP::sdu)
//
//
// 参考资料：
//
//	http://msdn.microsoft.com/en-us/library/windows/desktop/ee663269(v=vs.85).aspx	Diagnostics
//	http://msdn.microsoft.com/en-us/library/windows/desktop/ms680657(v=vs.85).aspx	Structured Exception Handling
//
//	http://msdn.microsoft.com/en-us/library/x057540h.aspx	Exception Handling in Visual C++
//	http://msdn.microsoft.com/en-us/library/4t3saedz.aspx	C++ Exception Handling
//	http://msdn.microsoft.com/en-us/library/swezty51.aspx	Structured Exception Handling (C/C++)
//
//	http://msdn.microsoft.com/en-us/library/6d85y967.aspx	Exception Handling Routines
//	http://msdn.microsoft.com/en-us/library/969zc1ac.aspx	Error Handling (CRT)
//	
//	http://blog.csdn.net/jirongzi_cs2011/article/details/9564395
//	http://blog.csdn.net/chen_jint/article/details/8090266
//	http://www.cnblogs.com/yangchaobj/archive/2013/02/23/2923416.html
//	http://www.cnblogs.com/yangchaobj/archive/2013/02/23/2923479.html
//
//	http://sunxiunan.com/?p=1142
//	https://code.google.com/hosting/search?q=label:google
//	https://code.google.com/hosting/search?q=label:minidump
//	https://code.google.com/p/google-breakpad/		An open-source multi-platform crash reporting system
//
//
//	C异常处理:
//		1.无条件终止:exit()/abort()
//		2.有条件终止:assert()
//		3.错误码:errno
//		4.非局部跳转:setjmp/longjmp
//		5.信号:signal/raise
//
//	C++异常处理:
//		1.异常处理语法:try...catch...finally, throw
//		2.标准异常:sdexception
//		3.异常处理函数：
//			_set_se_translator
//			set_terminate
//			set_unexpected
//			terminate
//			unexpected
//
//	Windows异常处理(Structured Exception Handling,SEH):
//		1.__try...__except
//
//	Windows Dump转储生成：
//
//	
//---------------------------------------------------------
// 作者:	
// 创建:		2014-01-09
// 最后修改:	
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_DIAGNOSTICS_EXCEPTION_H__
#define _SD_BASE_DIAGNOSTICS_EXCEPTION_H__
#include "sdSingleton.h"

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_DIAGNOSTICS
//-------------------------------------------------------------------------------------------------
// SEH异常的C++转换对象
//-------------------------------------------------------------------------------------------------
class sdSEHException : public std::exception
{
public:
	sdSEHException() : m_lpException(NULL){}
	sdSEHException(LPEXCEPTION_POINTERS lpException) : m_lpException(lpException){}

	LPEXCEPTION_POINTERS GetSEHInfo() const { return m_lpException;}
	const char* what();

protected:
	LPEXCEPTION_POINTERS m_lpException;
};
//-------------------------------------------------------------------------------------------------
// SEH异常转C++异常回调函数，将SEH异常翻译为C++异常
//-------------------------------------------------------------------------------------------------
void SDSEHExceptionTranslate(UINT dwErrorCode, LPEXCEPTION_POINTERS lpException)
{
	(dwErrorCode);
	throw sdSEHException(lpException);
}
//-------------------------------------------------------------------------------------------------
// SEH异常转C++异常的安装与卸载(线程独立)
//-------------------------------------------------------------------------------------------------
class sdSEHToExceptionHandler : public sdTSingleton<sdSEHToExceptionHandler>
{
public:
	void InstalllTranslateFunc(){ m_pfOldFilter = ::_set_se_translator(SDSEHExceptionTranslate);}
	void UninstallTranslateFunc() { if (m_pfOldFilter) ::_set_se_translator(m_pfOldFilter);}

protected:
	_se_translator_function m_pfOldFilter; 
};
//-------------------------------------------------------------------------------------------------
void SDInvalidParameterHandler(const wchar_t*, const wchar_t*, const wchar_t*, unsigned int, uintptr_t);
void SDPureCallHandler();
void SDMakeDump();
//-------------------------------------------------------------------------------------------------
// 异常处理安装与卸载
//-------------------------------------------------------------------------------------------------
class sdSetExceptionHandler : public sdTSingleton<sdSetExceptionHandler> 
{
public:
	sdSetExceptionHandler();
	~sdSetExceptionHandler();
};
//-------------------------------------------------------------------------------------------------
// Dump生成函数
//-------------------------------------------------------------------------------------------------
void SDCatchDumpInfo(LPEXCEPTION_POINTERS lpExceptionPtr);
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_DIAGNOSTICS
BASE_NAMESPACE_END_BASE
#endif