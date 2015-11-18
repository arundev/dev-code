//*************************************************************************************************
// 内容:	Win32 File的一些功能
//---------------------------------------------------------
// 作者:		
// 创建:		2012-07-13
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_UTIL_FILE_UTIL_H__
#define _SD_BASE_UTIL_FILE_UTIL_H__

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_UTIL

// 用来处理文件目录路径等一些与操作系统相关的功能
class sdFileUtil
{
public:
	// 判断文件是否存在
	static bool IsFileExist(const char* szFileName)
	{
		WIN32_FIND_DATA kFindData = { 0 };
		HANDLE hFile = ::FindFirstFile(szFileName, &kFindData);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			::FindClose(hFile);
			if (kFindData.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
				return true;
		}
		return false;
	}

	// 判断目录是否存在
	static bool IsDirExist(const char* szDir)
	{
		DWORD uiAttribute = ::GetFileAttributes(szDir);
		return (uiAttribute != -1) && (uiAttribute & FILE_ATTRIBUTE_DIRECTORY);
	}

	// 确定目录是否存在,不存在则创建之
	static void ConfimDir(const char* szDir)
	{
		char szDOSPath[MAX_PATH];
		ConvertToDOSName(szDOSPath, szDir);	///< 转换为'\\'分割
		ConfimSlash(szDOSPath);				///< 确定以'\\'结尾

		if (IsDirExist(szDOSPath))
			return;
		
		char szDosCurPath[MAX_PATH];
		char* pcPos = strchr(szDOSPath, '\\');
		if (!pcPos) 
			return;
		strncpy(szDosCurPath, szDOSPath, pcPos-szDosCurPath);
		
		uint uiLength = strlen(szDir);
		for (;;)
		{
			char* pcLastPos = pcPos;
			pcPos = strchr(pcPos+1, '\\');	///< 当前位置往后查找
			if (!pcPos) 
				break;

			strncat(szDosCurPath, szDOSPath, pcPos-pcLastPos);
			::CreateDirectory(szDosCurPath, NULL);
			
			if (pcPos-pcLastPos == uiLength)		///< 到达文件结尾
				break;
		}
	}

	// 组合字符串
	//	@param[out]	szPath	输出缓存
	//	@param[in]	n		输入字符串数量
	//	@param[in]	...		输入字符串
	static void CombinePath(char* szPath, int n, ...)
	{
		if (n > 0)
		{
			va_list ap;
			va_start(ap, n);
			while (n--)
			{
				char* pcParam = va_arg(ap, char*);
				if (!pcParam)
					continue;

				strncpy(szPath, pcParam, MAX_PATH);
				if (n != 0)
					ConfimSlash(szPath);
			}
			va_end(ap);

			ConfimNoSlash(szPath);
		}
	}

	// 规整化路径
	static bool NormalizePath(char* szPath)
	{
		assert(szPath);
		char szTempPath[MAX_PATH];
		::GetFullPathName(szPath, MAX_PATH, szTempPath, NULL);	///< 从指定文件得到文件的路径
		_strlwr(szTempPath);
		strncpy(szPath, szTempPath, MAX_PATH);
		return true;
	}

	// 文件只读属性
	static void RemoveReadOnly(const char* szFileFullPath)
	{
		DWORD uiAttribute = ::GetFileAttributes(szFileFullPath);
		if ((uiAttribute != -1) && (uiAttribute & FILE_ATTRIBUTE_READONLY))
		{
			DWORD uiWriteableAttribute = uiAttribute & ~FILE_ATTRIBUTE_READONLY;
			::SetFileAttributes(szFileFullPath, uiWriteableAttribute);
		}
	}

	// 确认路径的最后以"\\"或"/"结束,不存在则添加之
	static void ConfimSlash(char* szPath)
	{
		uint uiLegnth = strlen(szPath);
		if (uiLegnth)
		{
			if (szPath[uiLegnth - 1] != '\\' && szPath[uiLegnth - 1] != '/')
				strncat(szPath, "\\", MAX_PATH);
		}
	}

	// 确认路径最后不以"//"或"/"结束,存在则去除
	static void ConfimNoSlash(char* szPath)
	{
		uint uiLegnth = strlen(szPath);
		if (uiLegnth)
		{
			if (szPath[uiLegnth - 1] == '\\' || szPath[uiLegnth - 1] == '/')
				szPath[uiLegnth - 1] = '\0';
		}
	}

	// 将路径转换为DOS样式(将'/'转换为'\\')
	static void ConvertToDOSName(char* pszDst, const char* pszSrc)
	{
		while (*pszSrc)
		{
			if (*pszSrc == '/')
				*pszDst = '\\';
			else
				*pszDst = *pszSrc;

			++pszSrc;
			++pszDst;
		}
		*pszDst = 0;
	}

	// 将路径转换为UNIX样式(将'\\''转换为'/)
	static void ConvertToUnixName(char* pszDst, const char* pszSrc)
	{
		while (*pszSrc)
		{
			if (*pszSrc == '\\')
				*pszDst = '/';
			else
				*pszDst = *pszSrc;

			++pszSrc;
			++pszDst;
		}
		*pszDst = 0;
	}

	// 从绝对路径文件名中解析路径(路径末尾不带斜杠)
	static std::string ExtractPath(const std::string& szPathFileName)
	{
		int iNum = (int)szPathFileName.length() - 1;
		while (iNum > 0)
		{
			if (szPathFileName[iNum] == '\\' || szPathFileName[iNum] == '/')
				return szPathFileName.substr(0, iNum);
			--iNum;
		}
		return "";
	};

	static bool ExtractPath(char* szPathFileName)
	{
		assert(szPathFileName);
		int iNum = (int)strlen(szPathFileName) - 1;
		while (iNum > 0)
		{
			if (szPathFileName[iNum] == '\\' || szPathFileName[iNum] == '/')
			{
				szPathFileName[iNum]= '\0';
				return true;
			}
			--iNum;
		}
		return false;
	}

	// 从绝对路径文件名中解析文件名
	static std::string ExtractName(const std::string& szPathFileName)
	{
		char szDosName[MAX_PATH + 1];
		ConvertToDOSName(szDosName, szPathFileName.c_str());
		return szPathFileName.substr(std::string(szDosName).rfind("\\") + 1);
	};

	// 取当前exe文件路径(末尾不带斜杠)
	static void GetModulePath(char* szPath)
	{
		char szTempPath[MAX_PATH + 1];
		::GetModuleFileName(NULL, szTempPath, MAX_PATH);
		ExtractPath(szTempPath);
		strncpy(szPath, szTempPath, MAX_PATH);
	}

	// 取当前exe/dll文件名(不带路径)
	static std::string GetModuleName()
	{
		char szPath[MAX_PATH + 1];
		::GetModuleFileName(NULL, szPath, MAX_PATH);
		return ExtractName(szPath);
	};

};

BASE_NAMESPACE_END_UTIL
BASE_NAMESPACE_END_BASE
#endif