//*************************************************************************************************
// 内容:	日志系统
//---------------------------------------------------------
// 作者:		
// 创建:		2012-07-16
// 最后修改:	2013-02-04
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
// 日志事件级别,用于过滤日志
//-----------------------------------------------------------------------------
enum ELoggingLevel
{	
	E_LL_ERROR	= 0,
	E_LL_WARNNING,
	E_LL_INFORMATIVE,
};
//-----------------------------------------------------------------------------
// 一个简单的单日志文件的日志系统
// (考虑加入缓存,分块写入以优化IO)
//-----------------------------------------------------------------------------
class sdLogger : public sdTSingleton<sdLogger>
{
public:
	// 日志事件回调函数
	typedef void (*LogEventHandler)(const std::string& message, ELoggingLevel level);

public:
	sdLogger();
	virtual ~sdLogger();

	// 日志
	void Log(const char* message, ...);
	void LogWarning(const char* message, ...);
	void LogError(const char* message, ...);
	void LogEvent(ELoggingLevel level, const char* message, ...);
	void LogEvent(const std::string& message, ELoggingLevel level = E_LL_INFORMATIVE);

	// 向控制台输出调试字符串(使用静态缓存,从而不支持多线程)
	void OutputDebugString(const char* message, ...);
	void OutputDebugStringW(const wchar_t* message, ...);

	// 向控制台输出调试字符串(使用堆栈缓存,从而支持多线程)
	void OutputDebugString(const char* file, const size_t line, const char* message, ...);

	// 设置日志文件名
	void SetLogFileName(const char* filename, bool append = false);

	// 设置日志事件回调函数,返回旧的回调函数
	LogEventHandler SetLogHandle(LogEventHandler pfn_logger);

	// 获取日志级别
	ELoggingLevel GetLoggingLevel() const;

protected:
	// 日志级别
	ELoggingLevel		m_eLoggingLevel;

	// 日志数据流
	std::ofstream		m_kLogStream;		///< 日志数据流
	std::ostringstream	m_kLogWorkStream;	///< 日志格式化数据流		

	// 日志缓存
	typedef std::vector<std::pair<std::string, ELoggingLevel> > LogCache;
	typedef std::vector<std::pair<std::string, ELoggingLevel> >::iterator LogCacheItr;
	LogCache	m_kLogCache;	///< 日志缓存
	bool		m_bLogCache;	///< 是否使用日志缓存

	// 日志回调函数
	LogEventHandler m_pfnLogHandler;

	// 临界区
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