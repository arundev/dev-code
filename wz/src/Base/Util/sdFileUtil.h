//*************************************************************************************************
// ����:	Win32 File��һЩ����
//---------------------------------------------------------
// ����:		
// ����:		2012-07-13
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_UTIL_FILE_UTIL_H__
#define _SD_BASE_UTIL_FILE_UTIL_H__

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_UTIL

// ���������ļ�Ŀ¼·����һЩ�����ϵͳ��صĹ���
class sdFileUtil
{
public:
	// �ж��ļ��Ƿ����
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

	// �ж�Ŀ¼�Ƿ����
	static bool IsDirExist(const char* szDir)
	{
		DWORD uiAttribute = ::GetFileAttributes(szDir);
		return (uiAttribute != -1) && (uiAttribute & FILE_ATTRIBUTE_DIRECTORY);
	}

	// ȷ��Ŀ¼�Ƿ����,�������򴴽�֮
	static void ConfimDir(const char* szDir)
	{
		char szDOSPath[MAX_PATH];
		ConvertToDOSName(szDOSPath, szDir);	///< ת��Ϊ'\\'�ָ�
		ConfimSlash(szDOSPath);				///< ȷ����'\\'��β

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
			pcPos = strchr(pcPos+1, '\\');	///< ��ǰλ���������
			if (!pcPos) 
				break;

			strncat(szDosCurPath, szDOSPath, pcPos-pcLastPos);
			::CreateDirectory(szDosCurPath, NULL);
			
			if (pcPos-pcLastPos == uiLength)		///< �����ļ���β
				break;
		}
	}

	// ����ַ���
	//	@param[out]	szPath	�������
	//	@param[in]	n		�����ַ�������
	//	@param[in]	...		�����ַ���
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

	// ������·��
	static bool NormalizePath(char* szPath)
	{
		assert(szPath);
		char szTempPath[MAX_PATH];
		::GetFullPathName(szPath, MAX_PATH, szTempPath, NULL);	///< ��ָ���ļ��õ��ļ���·��
		_strlwr(szTempPath);
		strncpy(szPath, szTempPath, MAX_PATH);
		return true;
	}

	// �ļ�ֻ������
	static void RemoveReadOnly(const char* szFileFullPath)
	{
		DWORD uiAttribute = ::GetFileAttributes(szFileFullPath);
		if ((uiAttribute != -1) && (uiAttribute & FILE_ATTRIBUTE_READONLY))
		{
			DWORD uiWriteableAttribute = uiAttribute & ~FILE_ATTRIBUTE_READONLY;
			::SetFileAttributes(szFileFullPath, uiWriteableAttribute);
		}
	}

	// ȷ��·���������"\\"��"/"����,�����������֮
	static void ConfimSlash(char* szPath)
	{
		uint uiLegnth = strlen(szPath);
		if (uiLegnth)
		{
			if (szPath[uiLegnth - 1] != '\\' && szPath[uiLegnth - 1] != '/')
				strncat(szPath, "\\", MAX_PATH);
		}
	}

	// ȷ��·�������"//"��"/"����,������ȥ��
	static void ConfimNoSlash(char* szPath)
	{
		uint uiLegnth = strlen(szPath);
		if (uiLegnth)
		{
			if (szPath[uiLegnth - 1] == '\\' || szPath[uiLegnth - 1] == '/')
				szPath[uiLegnth - 1] = '\0';
		}
	}

	// ��·��ת��ΪDOS��ʽ(��'/'ת��Ϊ'\\')
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

	// ��·��ת��ΪUNIX��ʽ(��'\\''ת��Ϊ'/)
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

	// �Ӿ���·���ļ����н���·��(·��ĩβ����б��)
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

	// �Ӿ���·���ļ����н����ļ���
	static std::string ExtractName(const std::string& szPathFileName)
	{
		char szDosName[MAX_PATH + 1];
		ConvertToDOSName(szDosName, szPathFileName.c_str());
		return szPathFileName.substr(std::string(szDosName).rfind("\\") + 1);
	};

	// ȡ��ǰexe�ļ�·��(ĩβ����б��)
	static void GetModulePath(char* szPath)
	{
		char szTempPath[MAX_PATH + 1];
		::GetModuleFileName(NULL, szTempPath, MAX_PATH);
		ExtractPath(szTempPath);
		strncpy(szPath, szTempPath, MAX_PATH);
	}

	// ȡ��ǰexe/dll�ļ���(����·��)
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