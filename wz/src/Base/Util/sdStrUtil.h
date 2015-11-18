//*************************************************************************************************
// ����:	�ַ����ĸ�������һЩ����
//---------------------------------------------------------
// ����:		
// ����:		2012-07-09
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_UTIL_STR_UTIL_H__
#define _SD_BASE_UTIL_STR_UTIL_H__

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_UTIL

class sdStrUtil
{
public:
		

	// ��ʽ���ַ����������string
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

	// �ַ���ת��Ϊ��д
	//static std::string MakeUpper(const std::string& strValue)
	//{
	//	std::string strTmpString = strValue;
	//	transform(strTmpString.begin(), strTmpString.end(), strValue.begin(), toupper);
	//	return strTmpString;
	//}

	// �ַ���ת��ΪСд
	//static std::string MakeLower(const std::string& strValue)
	//{
	//	std::string strTmpString = strValue;
	//	transform(strTmpString.begin(), strTmpString.end(), strValue.begin(), tolower);
	//	return strTmpString;
	//}

	// ����ǰ��ո�
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