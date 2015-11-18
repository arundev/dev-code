//*************************************************************************************************
// 内容:	lua文件加载与保存,通过Lua解释器来执行
//---------------------------------------------------------
// 作者:		
// 创建:		2012-09-05
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_UTIL_LUA_FILE_H__
#define _SD_ENGINE_UTIL_LUA_FILE_H__

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

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_UTIL
//---------------------------------------------------------------------------------------
// 用于从lua文件读取数据表到内存
class sdLuaReadUtil : public NiMemObject
{
public:
	sdLuaReadUtil();
	~sdLuaReadUtil();

	// 从路径加载所有lua文件到lua解释器(递归)
	//bool LoadPath(const char* szPath);

	// 从文件加载lua文件到lua解释器
	bool LoadFile(const char* szFileName);

	// 读取Lua表格
	inline void ReadRootNode(const char* szRootNodeName);
	inline void BeginReadNode(const char* szNodeName);
	inline void EndReadNode();
	inline bool IsNodeValid();

	inline int	LoopReset();
	inline int	LoopNext(int t);
	inline bool LoopInnerEnd();
	inline int	LoopCount();
	
	bool ReadData(const char* key, bool& value);
	bool ReadData(const char* key, int& value);
	bool ReadData(const char* key, uint& value);
	bool ReadData(const char* key, float& value);
	bool ReadData(const char* key, time_t& value);

	bool ReadData(const char* key, NiPoint2& value);
	bool ReadData(const char* key, NiPoint3& value);
	bool ReadData(const char* key, NiPoint4& value);
	bool ReadData(const char* key, NiQuaternion& value);
	bool ReadData(const char* key, NiColor& value);

	bool ReadData(const char* key, Base::Math::sdVector2& value);
	bool ReadData(const char* key, Base::Math::sdVector3& value);

	bool ReadData(const char* key, std::string& value);
	bool ReadData(const char* key, char* value, uint len);

	template<typename T>
	bool ReadData(const char* key, T& value)
	{
		bool bRet = false;

		CScriptCleaner kScriptCleaner(m_pkLuaState, 1);
		lua_getfield(m_pkLuaState, -1, key);
		if (lua_isnumber(m_pkLuaState, -1))
		{
			value = static_cast<T>(lua_tonumber(m_pkLuaState, -1));
			bRet = true;
		}
		else
		{
			bRet = false;
		}

		return bRet;
	}

public:
	// lua读取辅助函数
	// @{
	static bool ReadData(lua_State* pkLuaState, const char* key, bool& value);
	static bool ReadData(lua_State* pkLuaState, const char* key, int& value);
	static bool ReadData(lua_State* pkLuaState, const char* key, uint& value);
	static bool ReadData(lua_State* pkLuaState, const char* key, float& value);
	static bool ReadData(lua_State* pkLuaState, const char* key, time_t& value);
	static bool ReadData(lua_State* pkLuaState, const char* key, NiPoint2& value);
	static bool ReadData(lua_State* pkLuaState, const char* key, NiPoint3& value);
	static bool ReadData(lua_State* pkLuaState, const char* key, NiPoint4& value);
	static bool ReadData(lua_State* pkLuaState, const char* key, NiQuaternion& value);
	static bool ReadData(lua_State* pkLuaState, const char* key, NiColor& value);
	static bool ReadData(lua_State* pkLuaState, const char* key, std::string& value);
	static bool ReadData(lua_State* pkLuaState, const char* key, char* value, uint len);

	template<typename T>
	static bool ReadData(lua_State* pkLuaState, const char* key, T& value)
	{
		NIASSERT(pkLuaState);

		bool bRet = false;

		CScriptCleaner kScriptCleaner(pkLuaState, 1);
		lua_getfield(pkLuaState, -1, key);
		if (lua_isnumber(pkLuaState, -1))
		{
			value = static_cast<T>(lua_tonumber(pkLuaState, -1));
			bRet = true;
		}
		else
		{
			bRet = false;
		}

		return bRet;
	}
	// @}

protected:
	// lua解释器错误返回处理
	void Report(int nStatus);

protected:
	// 内嵌类,用于简化lua读取
	class CScriptCleaner : public NiMemObject
	{
	public:
		explicit CScriptCleaner(lua_State* pkLuaState) : m_pkLuaState(pkLuaState), m_nNum(-1) {}
		explicit CScriptCleaner(lua_State* pkLuaState, int nNum) : m_pkLuaState(pkLuaState), m_nNum(nNum) {}
		~CScriptCleaner() { NIASSERT(m_pkLuaState); lua_pop(m_pkLuaState, m_nNum); }

	protected:
		lua_State* m_pkLuaState;
		INT32 m_nNum;
	};

protected:
	lua_State* m_pkLuaState;	///< lua解释器
	int m_iTop;
};
//---------------------------------------------------------------------------------------
// 用于写入数据表到Lua文件
class sdLuaWriteUtil : public NiMemObject
{
public:
	sdLuaWriteUtil();
	~sdLuaWriteUtil();

	// 目标文件打开与关闭
	bool OpenFile(const char* szFileName);
	void CloseFile();

	// 写入lua数据表格
	void WriteRootNodeBegin(const char* key);
	void WriteRootNodeEnd();

	void WriteNodeBegin(const char* key);
	void WriteNodeEnd();

	void LoopInnerBegin();
	void LoopInnerEnd();

	bool WriteData(const char* key, bool value, int len);
	bool WriteData(const char* key, int value, int len);
	bool WriteData(const char* key, uint value, int len);
	bool WriteData(const char* key, float value, int len);
	bool WriteData(const char* key, time_t value, int len);

	bool WriteData(const char* key, const NiPoint2& value, int len);
	bool WriteData(const char* key, const NiPoint3& value, int len);
	bool WriteData(const char* key, const NiPoint4& value, int len);
	bool WriteData(const char* key, const NiQuaternion& value, int len);
	bool WriteData(const char* key, const NiColor& value, int len);

	bool WriteData(const char* key, const Base::Math::sdVector2& value, int len);
	bool WriteData(const char* key, const Base::Math::sdVector3& value, int len);

	bool WriteData(const char* key, const char* value, int len);
	bool WriteData(const char* key, const std::string& value, int len);
	bool WriteData(const char* key, const char* value, int len, bool path);	

protected:
	// 流写入
	inline sdLuaWriteUtil& operator << (const char* value);

	// 写入键值
	bool WriteKeyValue(const char* key, const char* value, int len);
	bool WriteLoopString(int count, const char* value);

protected:
	std::ofstream m_kStream;	///< 写入数据流
	int m_nPreTabCnt;			///< 当前行前缩进量
};
//---------------------------------------------------------------------------------------
#include "sdLuaFile.inl"

ENGINE_NAMESPACE_END_ENGINE
ENGINE_NAMESPACE_END_UTIL
#endif