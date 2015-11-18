#include "sdBasePCH.h"
#include "sdLogger.h"
#include "sdFileUtil.h"
#include "sdStrUtil.h"

// sdk
#include <strsafe.h>

using namespace Base::Util;

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_DIAGNOSTICS
//-----------------------------------------------------------------------------------------------
static char buff[1024];		///< 内部临时缓存,用于格式化字符串之类的
static wchar_t wbuff[1024];	///< 内部临时缓存
//-----------------------------------------------------------------------------------------------
#define SD_CHECK_MSG(message)	\
	if (!message)				\
	{							\
		return;					\
	}

#define SD_GET_FMT_MSG				\
	{								\
		va_list args;				\
		va_start(args, message);	\
		::StringCchVPrintf(buff, 1024, message, args);	\
		buff[1023] = '\0';			\
		va_end(args);				\
	}

#define SD_GET_FMT_MSGW			\
{								\
	va_list args;				\
	va_start(args, message);	\
	::StringCchVPrintfW(wbuff, 1024, message, args);	\
	buff[1023] = L'\0';			\
	va_end(args);				\
}

//-----------------------------------------------------------------------------------------------
sdLogger::sdLogger()
: m_eLoggingLevel(E_LL_INFORMATIVE)
, m_bLogCache(true)
, m_pfnLogHandler(NULL)
{
	std::string szName = sdFileUtil::GetModuleName();
	char szData[16] = { 0 };
	char szTime[16] = { 0 };
	_strdate_s(szData);
	_strtime_s(szTime);
	szData[2] = '-';
	szData[5] = '-';
	szTime[2] = '-';
	szTime[5] = '-';

	SetLogFileName(sdStrUtil::Format("log\\%s-%s-%s.log", szName.c_str(), szData, szTime).c_str());

	LogEvent("Logger Singleton Initialized!");
}
//-----------------------------------------------------------------------------------------------
sdLogger::~sdLogger()
{
	if (m_kLogStream.is_open())
	{
		LogEvent("Logger Singleton Destroyed!");
		m_kLogStream.close();
	}
}
//-----------------------------------------------------------------------------------------------
void sdLogger::Log(const char* message, ...)
{
	m_kCriticalSection.Lock();
	SD_CHECK_MSG(message);
	SD_GET_FMT_MSG(message);

	// 输出到调试控制台
	::OutputDebugString(buff);
	::OutputDebugString("\n");

	// 输出到日志
	LogEvent(buff, E_LL_INFORMATIVE);

	m_kCriticalSection.Unlock();
}
//-----------------------------------------------------------------------------------------------
void sdLogger::LogWarning(const char* message, ...)
{
	m_kCriticalSection.Lock();
	SD_CHECK_MSG(message);
	SD_GET_FMT_MSG(message);

	// 输出到调试控制台
	::OutputDebugString(buff);
	::OutputDebugString("\n");

	// 输出到日志
	LogEvent(buff, E_LL_WARNNING);

	m_kCriticalSection.Unlock();
}
//-----------------------------------------------------------------------------------------------
void sdLogger::LogError(const char* message, ...)
{
	m_kCriticalSection.Lock();
	SD_CHECK_MSG(message);
	SD_GET_FMT_MSG(message);

	// 输出到调试控制台
	::OutputDebugString(buff);
	::OutputDebugString("\n");

	// 输出到日志
	LogEvent(buff, E_LL_ERROR);

	m_kCriticalSection.Unlock();
}
//-----------------------------------------------------------------------------------------------
void sdLogger::LogEvent(ELoggingLevel level, const char* message, ...)
{
	// 内部函数,调用前已经加锁
	SD_CHECK_MSG(message);
	SD_GET_FMT_MSG(message);

	LogEvent(buff, level);
}
//-----------------------------------------------------------------------------------------------
void sdLogger::LogEvent(const std::string& message, ELoggingLevel level /* = E_INFORMATIVE */)
{
	using namespace std;

	// 抛弃掉低级别日志
	if (level > m_eLoggingLevel)
		return;

	// 调用日志回调
	if (m_pfnLogHandler)
		m_pfnLogHandler(message, level);

	//
	time_t et;
	time(&et);
	tm* etm = localtime(&et);
	if (etm)
	{
		// Clear string stream
		m_kLogWorkStream.str("");

		// Write Date
		m_kLogWorkStream << setfill('0') << setw(2) << etm->tm_mday << '/'
						 << setfill('0') << setw(2) << 1 + etm->tm_mon << '/'
						 << setw(4) << (1900 + etm + etm->tm_year) << ' ';

		// Write Time
		m_kLogWorkStream << setfill('0') << setw(2) << etm->tm_hour << ':'
						 << setfill('0') << setw(2) << etm->tm_min  << ':'
						 << setfill('0') << setw(2) << etm->tm_sec  << ' ';

		// Write event type code
		switch(level)
		{
		case E_LL_ERROR:
			m_kLogWorkStream << "(Error\t\t)";
			break;

		case E_LL_WARNNING:
			m_kLogWorkStream << "(Warnning)\t";
			break;

		case E_LL_INFORMATIVE:
			m_kLogWorkStream << "(Informative)";
			break;

		default:
			m_kLogWorkStream << "(Unkown)\t";
		}

		// Write message
		m_kLogWorkStream << message << endl;

		// 
		if (m_bLogCache)
		{
			m_kLogCache.push_back(std::make_pair(m_kLogWorkStream.str(), level));
		}
		else
		{
			m_kLogStream << m_kLogWorkStream.str();	///< write message
			m_kLogStream.flush();					///< ensure new event is written to the file, rather than just being buffered	
		}
	}
}
//-----------------------------------------------------------------------------------------------
void sdLogger::SetLogFileName(const char* filename, bool append)
{
	using namespace std;

	// 设置能够访问中文路径
	std::locale loc = std::locale::global(std::locale(""));

	// 创建日志文件目录
	char szFilePath[MAX_PATH];
	sdFileUtil::GetModulePath(szFilePath);

	string full_filename = std::string(szFilePath) + "\\" + filename;
	sdFileUtil::ConfimDir(sdFileUtil::ExtractPath(full_filename).c_str());

	// Close current log file(if any)
	if (m_kLogStream.is_open())
		m_kLogStream.close();

	// 如果文件存在,去掉只读属性
	if (sdFileUtil::IsFileExist(full_filename.c_str()))
		sdFileUtil::RemoveReadOnly(full_filename.c_str());

	//
	m_kLogStream.open(
		full_filename.c_str(),
		std::ios_base::out | (append ? std::ios_base::app : std::ios_base::trunc));
	if (!m_kLogStream.is_open())
	{
		OutputDebugString("Logger::SetLogFileName - Failed to open file. %s\n", full_filename);
		return;
	}

	// Initialize width for date & time alignment
	m_kLogStream.width(2);

	// Write out cached log strings
	if (m_bLogCache)
	{
		m_bLogCache = false;

		LogCacheItr itr = m_kLogCache.begin();
		LogCacheItr itr_end = m_kLogCache.end();
		for (; itr!=itr_end; ++itr)
		{
			if (itr->second <= m_eLoggingLevel)
			{
				m_kLogStream << itr->first;	///< Write message
				m_kLogStream.flush();		///< Ensure new event is written to the file, rather than just being buffered
			}
		}
		m_kLogCache.clear();
	}

	// 恢复设置
	std::locale::global(loc);
}
//-----------------------------------------------------------------------------------------------
ELoggingLevel sdLogger::GetLoggingLevel() const
{
	return m_eLoggingLevel;
}
//-----------------------------------------------------------------------------------------------
void sdLogger::OutputDebugString(const char* message, ...)
{
	SD_GET_FMT_MSG(message);
	::OutputDebugString(buff);	///< 输出到调试控制台
}
//-----------------------------------------------------------------------------------------------
void sdLogger::OutputDebugStringW(const wchar_t* message, ...)
{
	SD_GET_FMT_MSGW(message);
	::OutputDebugStringW(wbuff);	///< 输出到调试控制台
}
//-----------------------------------------------------------------------------------------------
void sdLogger::OutputDebugString(const char* file, const size_t line, const char* message, ...)
{
	if (!message || !*message)
		return;

	char buf[1024] = { 0 };
	va_list args;
	va_start(args, message);
	vsprintf_s(buf, message, args);
	va_end(args);

	char ret[1280] = { 0 };
	sprintf_s(ret, "%s(%d): %s\n", file, line, buf);

	::OutputDebugString(ret);
}
//-----------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_DIAGNOSTICS
BASE_NAMESPACE_END_BASE