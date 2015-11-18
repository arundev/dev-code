//*************************************************************************************************
// 内容:	字符串的辅助处理一些功能
//---------------------------------------------------------
// 作者:		
// 创建:		2012-07-09
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_UTIL_UTIL_H__
#define _SD_BASE_UTIL_UTIL_H__

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_UTIL

class sdUtil
{
public:


	// 查找当前系统是否存在指定名称的进程,常用方法有:
	//	1.					ToolHelp API			CreateToolhelp32Snapshot/Process32First/Process32Next
	//	2. psapi.dll		Processing Status API	EnumProcesses/EnumProcessModules
	//	3. Wtsapi32.dll		Terminal Service API	WTSOpenServer/WTSEnumerateProcess
	//	3. ntdll.dll,		Native API				NtQuerySystemInformation
	//
	static bool IsProcessRunning(std::vector<std::string>& kProcessNameVec)
	{
		std::vector<std::string>::iterator itr = kProcessNameVec.begin();
		std::vector<std::string>::iterator itr_end = kProcessNameVec.end();
		for (; itr != itr_end; ++itr)
		{
			transform((*itr).begin(),(*itr).end(),(*itr).begin(),tolower);
		}

		HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (!hSnapshot)
			return false;

		PROCESSENTRY32 kProcessInfo;
		BOOL bStatus = ::Process32First(hSnapshot, &kProcessInfo);
		while (bStatus)
		{
			std::string kStrExeFile(kProcessInfo.szExeFile);
			transform(kStrExeFile.begin(),kStrExeFile.end(),kStrExeFile.begin(),tolower);
			
			std::vector<std::string>::iterator itr = kProcessNameVec.begin();
			std::vector<std::string>::iterator itr_end = kProcessNameVec.end();
			for (; itr != itr_end; ++itr)
			{
				if (!strcmp((*itr).c_str(), strExeFile.c_str()))
					return true;
			}

			bStatus = ::Process32Next(hSnapshot, &kProcessInfo);
		}

		::CloseHandle(hSnapshot);

		return false;
	};
};

BASE_NAMESPACE_END_BASE
BASE_NAMESPACE_END_UTIL
#endif