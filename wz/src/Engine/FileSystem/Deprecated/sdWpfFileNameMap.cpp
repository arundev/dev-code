#include "sdEnginePCH.h"
#include "sdWpfFileNameMap.h"

#include <sdFileUtil.h>

using namespace Base::Util;

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_FILESYSTEM
//-------------------------------------------------------------------------------------------------
// Wpf文件名(gdata目录下的)
const char* g_akWpfFileName[5] =
{
	"misc.wpf",
	"scene.wpf",
	"model.wpf",
	"texture1.wpf",
	"texture2.wpf",
};
//-------------------------------------------------------------------------------------------------
// Wpf文件包中的相对路径(gdata目录下的),用于根据路径名确定对应的Wpf文件
const char* g_aakWpfRelPath[5][5] =
{
	{
		"ui\\",
		"config\\",
		"build\\",
		"cursor\\",
		"sound\\",
	},
	{
		"scene\\",
	},
	{
		"model\\",
	},
	{
		"texture\\model\\scene\\",
	},
	{
		"texture\\",
	},
};
//-------------------------------------------------------------------------------------------------
sdWpfFileNameMap::sdWpfFileNameMap()
{
	// 获取游戏根目录
	sdFileUtil::GetModulePath(m_szGameRootPath);	///< 获取exe文件所在目录
	strncat_s(m_szGameRootPath, MAX_PATH_LEN, "\\..\\", MAX_PATH_LEN);			
	sdFileUtil::NormalizePath(m_szGameRootPath);	///< 获取上一层目录
	sdFileUtil::ConfimNoSlash(m_szGameRootPath);	///< 去掉路径末尾的斜杠
}
//-------------------------------------------------------------------------------------------------
bool sdWpfFileNameMap::GetWpfInterface(const char* szFileName, sdWpfFileNameMap::stWpfFileNameMapResult& kWpfFileNameMapResult)
{
	// 判断是否idata数据
	char szIDataPath[MAX_PATH_LEN];
	strncpy_s(szIDataPath, MAX_PATH_LEN, m_szGameRootPath, MAX_PATH_LEN);
	strncat_s(szIDataPath, MAX_PATH_LEN, "\\idata\\worldt", MAX_PATH_LEN);
	if (strstr(szFileName, szIDataPath))
	{
		strncpy_s(kWpfFileNameMapResult.szWorldName, MAX_WORLD_NAME_LEN, "world0", MAX_WORLD_NAME_LEN);
		strncpy_s(kWpfFileNameMapResult.szWpfName, MAX_PATH_LEN, m_szGameRootPath, MAX_PATH_LEN);
		strncat_s(kWpfFileNameMapResult.szWpfName, MAX_PATH_LEN, "\\idata\\worldt.wpf", MAX_PATH_LEN);
		strncpy_s(kWpfFileNameMapResult.szRelPath, MAX_PATH_LEN, szFileName + strlen(szIDataPath), MAX_PATH_LEN);
		kWpfFileNameMapResult.eWpfNameIndex = E_WPF_WORLDT;
		kWpfFileNameMapResult.pkWpfInterface = GetWpfInterface(kWpfFileNameMapResult.szWorldName, kWpfFileNameMapResult.eWpfNameIndex);

		return true;
	}

	// 判断是否gdata数据
	char szGDataPath[MAX_PATH_LEN];
	strncpy_s(szGDataPath, MAX_PATH_LEN, m_szGameRootPath, MAX_PATH_LEN);
	strncat_s(szGDataPath, MAX_PATH_LEN, "\\gdata\\", MAX_PATH_LEN);
	if (!strstr(szFileName, szGDataPath))
		return false;

	// 提取World名称
	int iGDataPathLen = strlen(szGDataPath);
	const char* pcWorldDirStartPos = szFileName + iGDataPathLen;
	const char* pcWorldDirEndPos = strstr(pcWorldDirStartPos, "\\");	///< "\\gdata\\"位置向后检索下一个"\\"
	if (!pcWorldDirEndPos)
		return false;

	int iWorkdDirNameLen = pcWorldDirEndPos - pcWorldDirStartPos;
	strncpy_s(kWpfFileNameMapResult.szWorldName, MAX_PATH_LEN, pcWorldDirStartPos, iWorkdDirNameLen);

	// 查找资源所在的Wpf文件
	bool bFind = false;
	const char* pcRelPathStartPos = pcWorldDirEndPos + 1;
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 5; ++j)
		{
			if (!g_aakWpfRelPath[i][j])
				break;

			if (!strstr(pcRelPathStartPos, g_aakWpfRelPath[i][j]))
				continue;
			
			strncpy_s(kWpfFileNameMapResult.szWpfName, MAX_PATH_LEN, szGDataPath, MAX_PATH_LEN);
			strncat_s(kWpfFileNameMapResult.szWpfName, MAX_PATH_LEN, kWpfFileNameMapResult.szWorldName, MAX_WORLD_NAME_LEN);
			strncat_s(kWpfFileNameMapResult.szWpfName, MAX_PATH_LEN, "\\", 1);
			strncat_s(kWpfFileNameMapResult.szWpfName, MAX_PATH_LEN, g_akWpfFileName[i], MAX_WPF_FILE_NAME_LEN);
			strncpy_s(kWpfFileNameMapResult.szRelPath, MAX_PATH_LEN, pcRelPathStartPos, MAX_PATH_LEN);
			kWpfFileNameMapResult.eWpfNameIndex = (EWpfNameIndex)i;
			kWpfFileNameMapResult.pkWpfInterface = GetWpfInterface(kWpfFileNameMapResult.szWorldName, kWpfFileNameMapResult.eWpfNameIndex);

			bFind = true;
			break;
		}

		if (bFind)
			break;
	}

	return bFind;
}
//-------------------------------------------------------------------------------------------------
void sdWpfFileNameMap::SetWpfInterface(const char* szWorldName, EWpfNameIndex eIndex, sdWpfLoader* pkWpfInterface)
{
	WpfMapItr itr = m_kWpfMap.find(szWorldName);
	WpfMapItr itr_end = m_kWpfMap.end();
	if (itr == itr_end)
	{
		stWpfFileNameMapEntry kEntry;
		kEntry.apkWpfInterface[eIndex] = pkWpfInterface;
		m_kWpfMap.insert(std::make_pair(std::string(szWorldName), kEntry));
	}
	else
	{
		itr->second.apkWpfInterface[eIndex] = pkWpfInterface;
	}
}
//-------------------------------------------------------------------------------------------------
void sdWpfFileNameMap::RemoveWpfInterface(const char* szWorldName)
{
	WpfMapItr itr = m_kWpfMap.find(szWorldName);
	WpfMapItr itr_end = m_kWpfMap.end();
	if (itr != itr_end)
	{
		for (int i = 0; i < NUM_WPFS; ++i)
		{
			//DestroyWpfInterface(itr->second.apkWpfInterface[i]);
		}
		m_kWpfMap.erase(itr);
	}
}
//-------------------------------------------------------------------------------------------------
bool sdWpfFileNameMap::ExistWpfNameMap(const char* szWorldName)
{
	WpfMapItr itr = m_kWpfMap.find(szWorldName);
	WpfMapItr itr_end = m_kWpfMap.end();
	return (itr != itr_end);
}
//-------------------------------------------------------------------------------------------------
sdWpfLoader* sdWpfFileNameMap::GetWpfInterface(const char* szWorldName, EWpfNameIndex eIndex)
{
	WpfMapItr itr = m_kWpfMap.find(szWorldName);
	WpfMapItr itr_end = m_kWpfMap.end();
	if (itr == itr_end)
		return NULL;
	return itr->second.apkWpfInterface[eIndex];
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_FILESYSTEM
ENGINE_NAMESPACE_END_ENGINE