//*************************************************************************************************
// 内容:	根据文件全路径映射到对应的wpf文件
//---------------------------------------------------------
// 作者:		
// 创建:		2013-08-22
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_FILESYSTEM_WPF_FILE_NAME_MAP_H__
#define _SD_ENGINE_FILESYSTEM_WPF_FILE_NAME_MAP_H__

#define MAX_WORLD_NAME_LEN		32
#define MAX_WPF_FILE_NAME_LEN	32
//#define MAX_PATH_LEN			MAX_PATH

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_FILESYSTEM

class sdWpfLoader;
//-------------------------------------------------------------------------------------------------
// 根据文件路径映射到具体的Wpf文件
//-------------------------------------------------------------------------------------------------
class sdWpfFileNameMap
{
public:
	// Wpf文件类型
	enum EWpfNameIndex
	{
		E_WPF_MISC,
		E_WPF_SCENE,
		E_WPF_MODEL,
		E_WPF_TEXTURE1,
		E_WPF_TEXTURE2,
		E_WPF_WORLDT,

		NUM_WPFS,
	};

	// 查询返回值
	struct stWpfFileNameMapResult
	{
		sdWpfLoader*	pkWpfInterface;						///< Wpf文件打开接口
		char			szWorldName[MAX_WORLD_NAME_LEN];	///< 世界名称
		char			szWpfName[MAX_PATH_LEN];			///< Wpf文件全路径(包含文件名)
		char			szRelPath[MAX_PATH_LEN];			///< Wpf内部相对路径
		EWpfNameIndex	eWpfNameIndex;						///< Wpf文件类型
	};

public:
	sdWpfFileNameMap();

	// 根据文件路径获取其所在的Wpf文件接口
	//	@param[in]		szFileName				文件磁盘全路径(最好为全小写和左斜杠)
	//	@param[in,out]	kWpfFileNameMapResult	查找结果
	bool GetWpfInterface(const char* szFileName, stWpfFileNameMapResult& kWpfFileNameMapResult);

	// 
	void SetWpfInterface(const char* szWorldName, EWpfNameIndex eIndex, sdWpfLoader* pkWpfInterface);
	void RemoveWpfInterface(const char* szWorldName);
	bool ExistWpfNameMap(const char* szWorldName);

protected:
	// 根据世界名和Wpf类型获取对应Wpf文件接口
	sdWpfLoader* GetWpfInterface(const char* szWorldName, EWpfNameIndex eIndex);

protected:
	struct stWpfFileNameMapEntry
	{
		sdWpfLoader* apkWpfInterface[NUM_WPFS];	
		stWpfFileNameMapEntry() 
		{ 
			for (int i = 0; i < NUM_WPFS; ++i) 
				apkWpfInterface[i] = NULL;
		}
	};
	typedef stWpfFileNameMapEntry WpfFileNameMapEntry;
	typedef std::map<std::string, WpfFileNameMapEntry> WpfMap;
	typedef std::map<std::string, WpfFileNameMapEntry>::iterator WpfMapItr;
	WpfMap m_kWpfMap;						///< 所有world的所有wpf文件
	char m_szGameRootPath[MAX_PATH_LEN];	///< 游戏根目录
};
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_FILESYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif