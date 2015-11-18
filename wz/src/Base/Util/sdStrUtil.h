//*************************************************************************************************
// 内容:	字符串的辅助处理一些功能
//---------------------------------------------------------
// 作者:		
// 创建:		2012-07-09
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_UTIL_STR_UTIL_H__
#define _SD_BASE_UTIL_STR_UTIL_H__

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_UTIL

class sdStrUtil
{
public:
		

	// 格式化字符串并输出到string
	static std::string Format(const char* fmt, ...)
	{
		char buff[1024] = "";
		va_list ap;
		va_start(ap, fmt);
		vsprintf(buff, fmt, ap);	
		//vsnprintf(buff, 1024, fmt, ap);
		//::StringCchVPrintf(buff, 1024, fmt, ap);
		va_end(ap);

		return buff;
	};

	// 字符串转换为大写
	//static std::string MakeUpper(const std::string& strValue)
	//{
	//	std::string strTmpString = strValue;
	//	transform(strTmpString.begin(), strTmpString.end(), strValue.begin(), toupper);
	//	return strTmpString;
	//}

	// 字符串转换为小写
	//static std::string MakeLower(const std::string& strValue)
	//{
	//	std::string strTmpString = strValue;
	//	transform(strTmpString.begin(), strTmpString.end(), strValue.begin(), tolower);
	//	return strTmpString;
	//}

	// 消除前后空格
	static std::string Trim(const std::string& strValue)
	{
		static const std::string delims = " \t\r";
		std::string strTmpString = strValue;
		strTmpString.erase(strTmpString.find_last_not_of(delims) + 1);	///< Trim Right
		strTmpString.erase(0, strTmpString.find_first_not_of(delims));	///< Trim Right
		return strTmpString;
	}
};

BASE_NAMESPACE_END_BASE
BASE_NAMESPACE_END_UTIL
#endif