//*************************************************************************************************
// 内容:	世界
//---------------------------------------------------------
// 作者:		
// 创建:		2013-05-13
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_SCENE_WORLD_H__
#define _SD_ENGINE_SCENE_WORLD_H__
#include <sdSingleton.h>

// 最大世界名长度
#define MAX_WORLD_NAME_LEN 32

// 加载回调函数(用于显示加载进度,考虑增加回传文字信息)
// @param[in]	iMax		最大进度
// @param[in]	iCurrent	当前进度
// @param[in]	szMsg		当前消息
typedef void (*LoadCallback)(int iMax, int iCurrent, const char* szMsg);

class sdMap;
//-----------------------------------------------------------------------------
// 世界
//-----------------------------------------------------------------------------
class sdWorld : public NiRefObject
{
public:
	sdWorld(const char* szName, const char* szPath);
	~sdWorld();

	const char* GetName() const { return m_szName;}
	void		SetName(const char* szName) { strncpy(m_szName, szName, MAX_WORLD_NAME_LEN);}

	const char* GetPath() const { return m_szPath;}
	void		SetPath(const char* szPath) { strncpy(m_szPath, szPath, MAX_WORLD_NAME_LEN);}

protected:
	char m_szName[MAX_WORLD_NAME_LEN];	///< 世界名称
	char m_szPath[MAX_WORLD_NAME_LEN];	///< 世界资源文件夹名称
};
NiSmartPointer(sdWorld);
//-----------------------------------------------------------------------------
// 世界管理器,主要用于路径转换
//-----------------------------------------------------------------------------
class sdWorldManager : public Base::sdTSingleton<sdWorldManager>
{
public:
	sdWorldManager();
	~sdWorldManager();

	sdWorld*	GetCurrentWorld() { return m_pkWorld;}
	void		SetCurrentWorld(sdWorld* pkWorld) { m_pkWorld = pkWorld;}

	const char* GetGameRootPath() const { return m_szGameRootPath;}

	// 场景
	// @{
	//bool	CreateScene(const char* szMapName);
	//void	DestroyScene();
	sdMap*	LoadScene(const char* szMapName, LoadCallback pfnCallback);
	bool	SaveScene(sdMap* pkMap, const char* szMapName, LoadCallback pfnCallback);
	// @}

	//
	// 转换路径到资源路径(不可以是同一个目录)
	//
	// 目前路径有以下几种:
	//	1.物理路径(路径中含有":\\"或":/")					x:\
	//	2.相对路径(以"\\",".\\","..\\","/","./","../"开始)	\relativepath
	//	3.资源路径(路径中含有":")							worldname:respath
	//	4.复合路径											worldname:respath\relativepath
	//
	// 注意：
	//	1.左右斜杠通用
	//	2.保存到lua一般用'/'
	//	3.字符串大小写
	//
	bool		Convert2ResPath(const char* szPath, char* szNewPath);

protected:
	bool	IsPhyPath(const char* szPath);
	bool	IsResPath(const char* szPath);
	bool	IsRelPath(const char* szPath);
	bool	GetWorldName(const char* szPath, char* szWorldName);
	bool	GetRelativePath(const char* szPath, char* szRelPath);

protected:
	sdWorldPtr	m_pkWorld;						///< 当前世界
	char		m_szGameRootPath[MAX_PATH_LEN];	///< 游戏根目录
};
//-----------------------------------------------------------------------------
#endif