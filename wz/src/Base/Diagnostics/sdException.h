//*************************************************************************************************
// ����:	����������(�ο�WZClient::Global/WZServers::common/SGDP::sdu)
//
//
// �ο����ϣ�
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
//	C�쳣����:
//		1.��������ֹ:exit()/abort()
//		2.��������ֹ:assert()
//		3.������:errno
//		4.�Ǿֲ���ת:setjmp/longjmp
//		5.�ź�:signal/raise
//
//	C++�쳣����:
//		1.�쳣�����﷨:try...catch...finally, throw
//		2.��׼�쳣:sdexception
//		3.�쳣��������
//			_set_se_translator
//			set_terminate
//			set_unexpected
//			terminate
//			unexpected
//
//	Windows�쳣����(Structured Exception Handling,SEH):
//		1.__try...__except
//
//	Windows Dumpת�����ɣ�
//
//	
//---------------------------------------------------------
// ����:	
// ����:		2014-01-09
// ����޸�:	
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_DIAGNOSTICS_EXCEPTION_H__
#define _SD_BASE_DIAGNOSTICS_EXCEPTION_H__
#include "sdSingleton.h"

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_DIAGNOSTICS
//-------------------------------------------------------------------------------------------------
// SEH�쳣��C++ת������
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
// SEH�쳣תC++�쳣�ص���������SEH�쳣����ΪC++�쳣
//-------------------------------------------------------------------------------------------------
void SDSEHExceptionTranslate(UINT dwErrorCode, LPEXCEPTION_POINTERS lpException)
{
	(dwErrorCode);
	throw sdSEHException(lpException);
}
//-------------------------------------------------------------------------------------------------
// SEH�쳣תC++�쳣�İ�װ��ж��(�̶߳���)
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
// �쳣����װ��ж��
//-------------------------------------------------------------------------------------------------
class sdSetExceptionHandler : public sdTSingleton<sdSetExceptionHandler> 
{
public:
	sdSetExceptionHandler();
	~sdSetExceptionHandler();
};
//-------------------------------------------------------------------------------------------------
// Dump���ɺ���
//-------------------------------------------------------------------------------------------------
void SDCatchDumpInfo(LPEXCEPTION_POINTERS lpExceptionPtr);
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_DIAGNOSTICS
BASE_NAMESPACE_END_BASE
#endif