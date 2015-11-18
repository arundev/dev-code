//*************************************************************************************************
// ����:	��־ϵͳ
//---------------------------------------------------------
// ����:		
// ����:		2012-07-16
// ����޸�:	2013-02-04
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_DIAGNOSTICS_LOGGER_H__
#define _SD_BASE_DIAGNOSTICS_LOGGER_H__
#include "sdSingleton.h"
#include "sdCriticalSection.h"

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_DIAGNOSTICS

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4275)
#pragma warning(disable:4251)
#endif

//-----------------------------------------------------------------------------
// ��־�¼�����,���ڹ�����־
//-----------------------------------------------------------------------------
enum ELoggingLevel
{	
	E_LL_ERROR	= 0,
	E_LL_WARNNING,
	E_LL_INFORMATIVE,
};
//-----------------------------------------------------------------------------
// һ���򵥵ĵ���־�ļ�����־ϵͳ
// (���Ǽ��뻺��,�ֿ�д�����Ż�IO)
//-----------------------------------------------------------------------------
class sdLogger : public sdTSingleton<sdLogger>
{
public:
	// ��־�¼��ص�����
	typedef void (*LogEventHandler)(const std::string& message, ELoggingLevel level);

public:
	sdLogger();
	virtual ~sdLogger();

	// ��־
	void Log(const char* message, ...);
	void LogWarning(const char* message, ...);
	void LogError(const char* message, ...);
	void LogEvent(ELoggingLevel level, const char* message, ...);
	void LogEvent(const std::string& message, ELoggingLevel level = E_LL_INFORMATIVE);

	// �����̨��������ַ���(ʹ�þ�̬����,�Ӷ���֧�ֶ��߳�)
	void OutputDebugString(const char* message, ...);
	void OutputDebugStringW(const wchar_t* message, ...);

	// �����̨��������ַ���(ʹ�ö�ջ����,�Ӷ�֧�ֶ��߳�)
	void OutputDebugString(const char* file, const size_t line, const char* message, ...);

	// ������־�ļ���
	void SetLogFileName(const char* filename, bool append = false);

	// ������־�¼��ص�����,���ؾɵĻص�����
	LogEventHandler SetLogHandle(LogEventHandler pfn_logger);

	// ��ȡ��־����
	ELoggingLevel GetLoggingLevel() const;

protected:
	// ��־����
	ELoggingLevel		m_eLoggingLevel;

	// ��־������
	std::ofstream		m_kLogStream;		///< ��־������
	std::ostringstream	m_kLogWorkStream;	///< ��־��ʽ��������		

	// ��־����
	typedef std::vector<std::pair<std::string, ELoggingLevel> > LogCache;
	typedef std::vector<std::pair<std::string, ELoggingLevel> >::iterator LogCacheItr;
	LogCache	m_kLogCache;	///< ��־����
	bool		m_bLogCache;	///< �Ƿ�ʹ����־����

	// ��־�ص�����
	LogEventHandler m_pfnLogHandler;

	// �ٽ���
	Threading::sdCriticalSection m_kCriticalSection;	
};

#define LOG_TEXT		Logger::Instance().Log
#define LOG_WARNING		Logger::Instance().LogWarning
#define LOG_ERROR		Logger::Instance().LogError

#if defined(_MSC_VER)
#pragma warning(pop)
#endif

BASE_NAMESPACE_END_DIAGNOSTICS
BASE_NAMESPACE_END_BASE
#endif