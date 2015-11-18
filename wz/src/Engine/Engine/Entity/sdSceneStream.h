//*************************************************************************************************
// 内容:	场景数据流加载与保存
//---------------------------------------------------------
// 作者:		
// 创建:		2014-03-13
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_SCENE_STREAM_H__
#define _SD_ENGINE_SCENE_STREAM_H__
#include <sdWorld.h>

// Lua头文件
extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

// Lua分配器订制
extern "C"
{
#ifdef SD_LUA_ALLOCATOR
	extern lua_State *sdLua_open();
#endif
}

class sdGameObject;
//-------------------------------------------------------------------------------------------------
// 用于独立的保存与加载场景数据
// (因为有特殊数据，所以重用了sdLuaWriteUtil和sdLuaReadUtil部分代码)
//-------------------------------------------------------------------------------------------------
class sdSceneStream : public NiRefObject
{
public:
	sdSceneStream(const char* szScenePath);
	virtual ~sdSceneStream();

	sdMap*	LoadScene(const char* szMapName, LoadCallback pfnCallback);
	bool	SaveScene(const sdMap* pkMap, LoadCallback pfnCallback);

protected:
	void SaveHeader();
	void SaveObject(sdGameObject* pkGameObject, int iLevel, const char* szObjectName);
	void SaveObjectCollection(sdGameObject* pkGameObject, 
		union PropGetterFunc kPropGetterFunc, 
		int iLevel, 
		const char* szObjectName);

	void LoadHeader();
	sdGameObject*  LoadObject(const char* szObjectName);
	uint LoadObjectCollection(sdGameObject* pkGameObject, union PropSetterFunc kPropSetterFunc);

protected:
	char m_szScenePath[MAX_PATH];	///< 场景路径

	lua_State* m_pkLuaState;		///< 文件读入所需的lua解释器
	std::ofstream m_kOutStream;		///< 文件写出数据流
	char m_acBufLine[MAX_PATH];		///< 行缓存
};
NiSmartPointer(sdTerrainStream);
//-------------------------------------------------------------------------------------------------
#endif