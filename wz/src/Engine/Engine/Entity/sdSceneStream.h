//*************************************************************************************************
// ����:	���������������뱣��
//---------------------------------------------------------
// ����:		
// ����:		2014-03-13
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_SCENE_STREAM_H__
#define _SD_ENGINE_SCENE_STREAM_H__
#include <sdWorld.h>

// Luaͷ�ļ�
extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

// Lua����������
extern "C"
{
#ifdef SD_LUA_ALLOCATOR
	extern lua_State *sdLua_open();
#endif
}

class sdGameObject;
//-------------------------------------------------------------------------------------------------
// ���ڶ����ı�������س�������
// (��Ϊ���������ݣ�����������sdLuaWriteUtil��sdLuaReadUtil���ִ���)
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
	char m_szScenePath[MAX_PATH];	///< ����·��

	lua_State* m_pkLuaState;		///< �ļ����������lua������
	std::ofstream m_kOutStream;		///< �ļ�д��������
	char m_acBufLine[MAX_PATH];		///< �л���
};
NiSmartPointer(sdTerrainStream);
//-------------------------------------------------------------------------------------------------
#endif