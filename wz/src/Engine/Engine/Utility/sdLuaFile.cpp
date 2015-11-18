#include "sdEnginePCH.h"
#include "sdLuaFile.h"

#include <sdLogger.h>

using namespace Base::Diagnostics;
using namespace Base::Math;

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_UTIL
//-------------------------------------------------------------------------------------------------
extern "C"
{
#ifdef SD_LUA_ALLOCATOR
void* sd_lua_alloc(void *ud, void *ptr, size_t osize, size_t nsize)
{
	(void)ud;
	(void)osize;
	if (nsize == 0)
	{
		FREE_BYTE(ptr);
		return NULL;
	}
	else
	{
		return REALLOC_BYTE(ptr, nsize);
	}
}

int sd_lua_panic(lua_State *L)
{
	(void)L;	///< toavoid warning
	fprintf(stderr, "PANIC: unprotected error in call to Lua APU (%s)\n", lua_tostring(L, -1));
	return 0;
}

lua_State* sd_lua_open()
{
	lua_State* L = lua_newstate(sd_lua_alloc, NULL);
	if (L) 
		lua_atpanic(L, &sd_lua_panic);
	return L;
}
#endif
}
//-------------------------------------------------------------------------------------------------
bool sdLuaReadUtil::ReadData(lua_State* pkLuaState, const char *key, time_t &value)
{
	NIASSERT(pkLuaState);

	bool bRet = false;

	CScriptCleaner kScriptCleaner(pkLuaState, 1);
	lua_getfield(pkLuaState, -1, key);
	if (lua_isstring(pkLuaState, -1))
	{
		std::string strValue = lua_tostring(pkLuaState, -1);

		struct tm stInfo;
		memset((void*)&stInfo, 0, sizeof(stInfo));
		sscanf_s(strValue.c_str(), "%d-%d-%d %d:%d:%d", 
			&stInfo.tm_year, &stInfo.tm_mon, &stInfo.tm_mday, 
			&stInfo.tm_hour, &stInfo.tm_min, &stInfo.tm_sec);
		stInfo.tm_year -= 1900;
		stInfo.tm_mon -= 1;
		value = mktime(&stInfo);

		bRet = true;
	}
	else
	{
		bRet = false;
	}

	return bRet;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaReadUtil::ReadData(lua_State* pkLuaState, const char *key, bool &value)
{
	NIASSERT(pkLuaState);

	bool bRet = false;

	CScriptCleaner kScriptCleaner(pkLuaState, 1);
	lua_getfield(pkLuaState, -1, key);
	if (lua_isstring(pkLuaState, -1))
	{
		std::string strValue = lua_tostring(pkLuaState, -1);

		transform(strValue.begin(), strValue.end(), strValue.begin(), ::tolower);
		if (strValue == "true")
			value = true;
		else
			value = false;

		bRet = true;
	}
	else
	{
		bRet = false;
	}

	return bRet;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaReadUtil::ReadData(lua_State* pkLuaState, const char* key, int& value)
{
	NIASSERT(pkLuaState);

	bool bRet = false;

	CScriptCleaner kScriptCleaner(pkLuaState, 1);
	lua_getfield(pkLuaState, -1, key);
	if (lua_isstring(pkLuaState, -1))
	{
		std::string strValue = lua_tostring(pkLuaState, -1);
		value = atoi(strValue.c_str());

		bRet = true;
	}
	else
	{
		bRet = false;
	}

	return bRet;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaReadUtil::ReadData(lua_State* pkLuaState, const char* key, uint& value)
{
	NIASSERT(pkLuaState);

	bool bRet = false;

	CScriptCleaner kScriptCleaner(pkLuaState, 1);
	lua_getfield(pkLuaState, -1, key);
	if (lua_isstring(pkLuaState, -1))
	{
		std::string strValue = lua_tostring(pkLuaState, -1);
		sscanf_s(strValue.c_str(), "%u", &value);

		bRet = true;
	}
	else
	{
		bRet = false;
	}

	return bRet;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaReadUtil::ReadData(lua_State* pkLuaState, const char* key, float& value)
{
	NIASSERT(pkLuaState);

	bool bRet = false;

	CScriptCleaner kScriptCleaner(pkLuaState, 1);
	lua_getfield(pkLuaState, -1, key);
	if (lua_isstring(pkLuaState, -1))
	{
		std::string strValue = lua_tostring(pkLuaState, -1);
		value = (float)atof(strValue.c_str());

		bRet = true;
	}
	else
	{
		bRet = false;
	}

	return bRet;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaReadUtil::ReadData(lua_State* pkLuaState, const char *key, NiPoint2 &value)
{
	NIASSERT(pkLuaState);

	bool bRet = false;

	CScriptCleaner kScriptCleaner(pkLuaState, 1);
	lua_getfield(pkLuaState, -1, key);
	if (lua_isstring(pkLuaState, -1))
	{
		std::string strValue = lua_tostring(pkLuaState, -1);
		sscanf_s(strValue.c_str(), "%f,%f", &value.x, &value.y);

		bRet = true;
	}
	else
	{
		bRet = false;
	}

	return bRet;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaReadUtil::ReadData(lua_State* pkLuaState, const char *key, NiPoint3 &value)
{
	NIASSERT(pkLuaState);

	bool bRet = false;

	CScriptCleaner kScriptCleaner(pkLuaState, 1);
	lua_getfield(pkLuaState, -1, key);
	if (lua_isstring(pkLuaState, -1))
	{
		std::string strValue = lua_tostring(pkLuaState, -1);
		sscanf_s(strValue.c_str(), "%f,%f,%f", &value.x, &value.y, &value.z);

		bRet = true;
	}
	else
	{
		bRet = false;
	}

	return bRet;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaReadUtil::ReadData(lua_State* pkLuaState, const char *key, NiPoint4 &value)
{
	NIASSERT(pkLuaState);

	bool bRet = false;

	CScriptCleaner kScriptCleaner(pkLuaState, 1);
	lua_getfield(pkLuaState, -1, key);
	if (lua_isstring(pkLuaState, -1))
	{
		std::string strValue = lua_tostring(pkLuaState, -1);
		sscanf_s(strValue.c_str(), "%f,%f,%f,%f", &value.m_afPt[0], &value.m_afPt[1], &value.m_afPt[2],  &value.m_afPt[3]);

		bRet = true;
	}
	else
	{
		bRet = false;
	}

	return bRet;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaReadUtil::ReadData(lua_State* pkLuaState, const char *key, NiQuaternion &value)
{
	NIASSERT(pkLuaState);

	bool bRet = false;

	CScriptCleaner kScriptCleaner(pkLuaState, 1);
	lua_getfield(pkLuaState, -1, key);
	if (lua_isstring(pkLuaState, -1))
	{
		std::string strValue = lua_tostring(pkLuaState, -1);
		sscanf_s(strValue.c_str(), "%f,%f,%f,%f", &value.m_fX, &value.m_fY, &value.m_fZ, &value.m_fW);

		bRet = true;
	}
	else
	{
		bRet = false;
	}

	return bRet;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaReadUtil::ReadData(lua_State* pkLuaState, const char *key, NiColor &value)
{
	NIASSERT(pkLuaState);

	bool bRet = false;

	CScriptCleaner kScriptCleaner(pkLuaState, 1);
	lua_getfield(pkLuaState, -1, key);
	if (lua_isstring(pkLuaState, -1))
	{
		std::string strValue = lua_tostring(pkLuaState, -1);
		sscanf_s(strValue.c_str(), "%f,%f,%f", &value.r, &value.g, &value.b);

		bRet = true;
	}
	else
	{
		bRet = false;
	}

	return bRet;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaReadUtil::ReadData(lua_State* pkLuaState, const char *key, std::string &value)
{
	NIASSERT(pkLuaState);

	bool bRet = false;

	if (strcmp(key, "") == 0)
	{
		value = lua_tostring(pkLuaState, -1);
		bRet = true;
	}
	else
	{
		CScriptCleaner kScriptCleaner(pkLuaState, 1);
		lua_getfield(pkLuaState, -1, key);
		if (lua_isstring(pkLuaState, -1))
		{
			value = lua_tostring(pkLuaState, -1);
			bRet = true;
		}
		else
		{
			bRet = false;
		}
	}

	return bRet;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaReadUtil::ReadData(lua_State* pkLuaState, const char* key, char* value, uint len)
{
	NIASSERT(pkLuaState);
	//NIASSERT(key);
	//NIASSERT(value);

	bool bRet = false;

	if (strcmp(key, "") == 0)
	{
		strncpy(value, lua_tostring(pkLuaState, -1), len);
		bRet = true;
	}
	else
	{
		CScriptCleaner kScriptCleaner(pkLuaState, 1);
		lua_getfield(pkLuaState, -1, key);
		if (lua_isstring(pkLuaState, -1))
		{
			strncpy(value, lua_tostring(pkLuaState, -1), len);
			bRet = true;
		}
		else
		{
			bRet = false;
		}
	}

	return bRet;
}
//-------------------------------------------------------------------------------------------------
sdLuaReadUtil::sdLuaReadUtil()
{
#ifdef SD_LUA_ALLOCATOR
	m_pkLuaState = sd_lua_open();
#else
	m_pkLuaState = lua_open();
#endif

	luaL_openlibs(m_pkLuaState);
}
//-------------------------------------------------------------------------------------------------
sdLuaReadUtil::~sdLuaReadUtil()
{
	lua_close(m_pkLuaState);
	m_pkLuaState = NULL;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaReadUtil::LoadFile(const char* szFileName)
{
	// Load file to memory buffer
	NiFile* spFile = NiFile::GetFile(szFileName, NiFile::READ_ONLY);
	if ((bool)(*spFile) == false)	///< 不解
	{
		//Log("Can't open file: %s\n", szFileName);
		NiDelete spFile;
		spFile = NULL;

		return false;
	}

	int iSize = spFile->GetFileSize();
	uchar* pucBuffer = (uchar*) NiAlloc(unsigned char, iSize);
	NIASSERT(pucBuffer);

	spFile->Read(pucBuffer, iSize);

	NiDelete spFile;
	spFile = NULL;

	// Compile lua file
	int nStatus = luaL_loadbuffer(m_pkLuaState, (char*)pucBuffer, iSize, "");
	if (nStatus)
	{
		Report(nStatus);

		NiFree(pucBuffer);
		pucBuffer = NULL;

		return false;
	}

	// Set pushing all return values into stack
	nStatus = lua_pcall(m_pkLuaState, 0, LUA_MULTRET, 0);
	if (nStatus)
	{
		Report(nStatus);
		return false;
	}

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaReadUtil::ReadData(const char *key, time_t &value)
{
	bool bRet = false;

	CScriptCleaner kScriptCleaner(m_pkLuaState, 1);
	lua_getfield(m_pkLuaState, -1, key);
	if (lua_isstring(m_pkLuaState, -1))
	{
		std::string strValue = lua_tostring(m_pkLuaState, -1);
		
		struct tm stInfo;
		memset((void*)&stInfo, 0, sizeof(stInfo));
		sscanf_s(strValue.c_str(), "%d-%d-%d %d:%d:%d", 
			&stInfo.tm_year, &stInfo.tm_mon, &stInfo.tm_mday, 
			&stInfo.tm_hour, &stInfo.tm_min, &stInfo.tm_sec);
		stInfo.tm_year -= 1900;
		stInfo.tm_mon -= 1;
		value = mktime(&stInfo);

		bRet = true;
	}
	else
	{
		bRet = false;
	}

	return bRet;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaReadUtil::ReadData(const char *key, bool &value)
{
	bool bRet = false;

	CScriptCleaner kScriptCleaner(m_pkLuaState, 1);
	lua_getfield(m_pkLuaState, -1, key);
	if (lua_isstring(m_pkLuaState, -1))
	{
		std::string strValue = lua_tostring(m_pkLuaState, -1);

		transform(strValue.begin(), strValue.end(), strValue.begin(), ::tolower);
		if (strValue == "true")
			value = true;
		else
			value = false;

		bRet = true;
	}
	else
	{
		bRet = false;
	}

	return bRet;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaReadUtil::ReadData(const char* key, int& value)
{
	bool bRet = false;

	CScriptCleaner kScriptCleaner(m_pkLuaState, 1);
	lua_getfield(m_pkLuaState, -1, key);
	if (lua_isstring(m_pkLuaState, -1))
	{
		std::string strValue = lua_tostring(m_pkLuaState, -1);
		value = atoi(strValue.c_str());

		bRet = true;
	}
	else
	{
		bRet = false;
	}

	return bRet;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaReadUtil::ReadData(const char* key, uint& value)
{
	bool bRet = false;

	CScriptCleaner kScriptCleaner(m_pkLuaState, 1);
	lua_getfield(m_pkLuaState, -1, key);
	if (lua_isstring(m_pkLuaState, -1))
	{
		std::string strValue = lua_tostring(m_pkLuaState, -1);
		sscanf_s(strValue.c_str(), "%u", &value);

		bRet = true;
	}
	else
	{
		bRet = false;
	}

	return bRet;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaReadUtil::ReadData(const char* key, float& value)
{
	bool bRet = false;

	CScriptCleaner kScriptCleaner(m_pkLuaState, 1);
	lua_getfield(m_pkLuaState, -1, key);
	if (lua_isstring(m_pkLuaState, -1))
	{
		std::string strValue = lua_tostring(m_pkLuaState, -1);
		value = (float)atof(strValue.c_str());

		bRet = true;
	}
	else
	{
		bRet = false;
	}

	return bRet;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaReadUtil::ReadData(const char *key, NiPoint2 &value)
{
	bool bRet = false;

	CScriptCleaner kScriptCleaner(m_pkLuaState, 1);
	lua_getfield(m_pkLuaState, -1, key);
	if (lua_isstring(m_pkLuaState, -1))
	{
		std::string strValue = lua_tostring(m_pkLuaState, -1);
		sscanf_s(strValue.c_str(), "%f,%f", &value.x, &value.y);

		bRet = true;
	}
	else
	{
		bRet = false;
	}

	return bRet;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaReadUtil::ReadData(const char *key, NiPoint3 &value)
{
	bool bRet = false;

	CScriptCleaner kScriptCleaner(m_pkLuaState, 1);
	lua_getfield(m_pkLuaState, -1, key);
	if (lua_isstring(m_pkLuaState, -1))
	{
		std::string strValue = lua_tostring(m_pkLuaState, -1);
		sscanf_s(strValue.c_str(), "%f,%f,%f", &value.x, &value.y, &value.z);

		bRet = true;
	}
	else
	{
		bRet = false;
	}

	return bRet;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaReadUtil::ReadData(const char *key, sdVector2 &value)
{
	bool bRet = false;

	CScriptCleaner kScriptCleaner(m_pkLuaState, 1);
	lua_getfield(m_pkLuaState, -1, key);
	if (lua_isstring(m_pkLuaState, -1))
	{
		std::string strValue = lua_tostring(m_pkLuaState, -1);
		sscanf_s(strValue.c_str(), "%f,%f", &value.m_fX, &value.m_fY);

		bRet = true;
	}
	else
	{
		bRet = false;
	}

	return bRet;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaReadUtil::ReadData(const char *key, sdVector3 &value)
{
	bool bRet = false;

	CScriptCleaner kScriptCleaner(m_pkLuaState, 1);
	lua_getfield(m_pkLuaState, -1, key);
	if (lua_isstring(m_pkLuaState, -1))
	{
		std::string strValue = lua_tostring(m_pkLuaState, -1);
		sscanf_s(strValue.c_str(), "%f,%f,%f", &value.m_fX, &value.m_fY, &value.m_fZ);

		bRet = true;
	}
	else
	{
		bRet = false;
	}

	return bRet;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaReadUtil::ReadData(const char *key, NiPoint4 &value)
{
	bool bRet = false;

	CScriptCleaner kScriptCleaner(m_pkLuaState, 1);
	lua_getfield(m_pkLuaState, -1, key);
	if (lua_isstring(m_pkLuaState, -1))
	{
		std::string strValue = lua_tostring(m_pkLuaState, -1);
		sscanf_s(strValue.c_str(), "%f,%f,%f,%f", &value.m_afPt[0], &value.m_afPt[1], &value.m_afPt[2],  &value.m_afPt[3]);

		bRet = true;
	}
	else
	{
		bRet = false;
	}

	return bRet;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaReadUtil::ReadData(const char *key, NiQuaternion &value)
{
	bool bRet = false;

	CScriptCleaner kScriptCleaner(m_pkLuaState, 1);
	lua_getfield(m_pkLuaState, -1, key);
	if (lua_isstring(m_pkLuaState, -1))
	{
		std::string strValue = lua_tostring(m_pkLuaState, -1);
		sscanf_s(strValue.c_str(), "%f,%f,%f,%f", &value.m_fX, &value.m_fY, &value.m_fZ, &value.m_fW);

		bRet = true;
	}
	else
	{
		bRet = false;
	}

	return bRet;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaReadUtil::ReadData(const char *key, NiColor &value)
{
	bool bRet = false;

	CScriptCleaner kScriptCleaner(m_pkLuaState, 1);
	lua_getfield(m_pkLuaState, -1, key);
	if (lua_isstring(m_pkLuaState, -1))
	{
		std::string strValue = lua_tostring(m_pkLuaState, -1);
		sscanf_s(strValue.c_str(), "%f,%f,%f", &value.r, &value.g, &value.b);

		bRet = true;
	}
	else
	{
		bRet = false;
	}

	return bRet;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaReadUtil::ReadData(const char *key, std::string &value)
{
	bool bRet = false;

	if (strcmp(key, "") == 0)
	{
		value = lua_tostring(m_pkLuaState, -1);
		bRet = true;
	}
	else
	{
		CScriptCleaner kScriptCleaner(m_pkLuaState, 1);
		lua_getfield(m_pkLuaState, -1, key);
		if (lua_isstring(m_pkLuaState, -1))
		{
			value = lua_tostring(m_pkLuaState, -1);
			bRet = true;
		}
		else
		{
			bRet = false;
		}
	}

	return bRet;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaReadUtil::ReadData(const char* key, char* value, uint len)
{
	//NIASSERT(key);
	//NIASSERT(value);

	bool bRet = false;

	if (strcmp(key, "") == 0)
	{
		strncpy(value, lua_tostring(m_pkLuaState, -1), len);
		bRet = true;
	}
	else
	{
		CScriptCleaner kScriptCleaner(m_pkLuaState, 1);
		lua_getfield(m_pkLuaState, -1, key);
		if (lua_isstring(m_pkLuaState, -1))
		{
			strncpy(value, lua_tostring(m_pkLuaState, -1), len);
			bRet = true;
		}
		else
		{
			bRet = false;
		}
	}

	return bRet;
}
//-------------------------------------------------------------------------------------------------
void sdLuaReadUtil::Report(int nStatus)
{
	if (nStatus && !lua_isnil(m_pkLuaState, -1))
	{
		const char* szMsg = lua_tostring(m_pkLuaState, -1);
		if (!szMsg)
			szMsg = "(lua: error object is not a string)";

		sdLogger::Instance().OutputDebugString(szMsg);
		sdLogger::Instance().OutputDebugString("\n");

		lua_pop(m_pkLuaState, 1);
	}
}
//-------------------------------------------------------------------------------------------------
sdLuaWriteUtil::sdLuaWriteUtil()
{
	m_kStream << "";
}
//-------------------------------------------------------------------------------------------------
sdLuaWriteUtil::~sdLuaWriteUtil()
{
	CloseFile();
}
//-------------------------------------------------------------------------------------------------
bool sdLuaWriteUtil::OpenFile(const char* szFileName)
{
	NIASSERT(szFileName);

	std::locale::global(std::locale(""));
	m_kStream.open(szFileName, std::ios_base::out | std::ios_base::trunc);
	if (!m_kStream)
		return false;

	return true;
}
//-------------------------------------------------------------------------------------------------
void sdLuaWriteUtil::CloseFile()
{
	if (m_kStream.is_open())
		m_kStream.close();
}
//-------------------------------------------------------------------------------------------------
void sdLuaWriteUtil::WriteRootNodeBegin(const char *key)
{
	char szBuf[256];
	sprintf_s(szBuf, 256, "%s=\n", key);

	(*this) << szBuf;
	(*this) << "{\n";

	m_nPreTabCnt = 0;
}
//-------------------------------------------------------------------------------------------------
void sdLuaWriteUtil::WriteRootNodeEnd()
{
	(*this) << "}\n\n";

	m_nPreTabCnt = 0;
}
//-------------------------------------------------------------------------------------------------
void sdLuaWriteUtil::WriteNodeBegin(const char *key)
{
	const char* szTab = "    ";
	const int iKeyLen = strlen("               ");

	m_nPreTabCnt += 1;

	int iSpaceNum = iKeyLen - strlen(key);
	WriteLoopString(m_nPreTabCnt, szTab);
	(*this) << key;
	WriteLoopString(iSpaceNum, " ");
	(*this) << "=\n";
	WriteLoopString(m_nPreTabCnt, szTab);
	(*this) << "{\n";
}
//-------------------------------------------------------------------------------------------------
void sdLuaWriteUtil::WriteNodeEnd()
{
	const char* szTab = "    ";
	WriteLoopString(m_nPreTabCnt, szTab);
	(*this) << "},\n";

	m_nPreTabCnt -= 1;
}
//-------------------------------------------------------------------------------------------------
void sdLuaWriteUtil::LoopInnerBegin()
{
	const char* szTab = "    ";

	m_nPreTabCnt += 1;

	(*this).WriteLoopString(m_nPreTabCnt, szTab);
	(*this) << "{\n";
}
//-------------------------------------------------------------------------------------------------
void sdLuaWriteUtil::LoopInnerEnd()
{
	const char* szTab = "    ";

	(*this).WriteLoopString(m_nPreTabCnt, szTab);
	(*this) << "},\n";

	m_nPreTabCnt -= 1;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaWriteUtil::WriteData(const char* key, bool value, int len)
{
	const char* szTab = "    ";
	WriteLoopString(m_nPreTabCnt+1, szTab);
	
	WriteKeyValue(key, value ? "\"true\"" : "\"false\"", len);
	m_kStream << "," << std::endl;

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaWriteUtil::WriteData(const char* key, int value, int len)
{
	const char* szTab = "    ";
	WriteLoopString(m_nPreTabCnt+1, szTab);

	char buff[64];
	sprintf_s(buff, 64, "%d", value);

	WriteKeyValue(key, buff, len);
	m_kStream << "," << std::endl;

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaWriteUtil::WriteData(const char* key, uint value, int len)
{
	const char* szTab = "    ";
	WriteLoopString(m_nPreTabCnt+1, szTab);

	char buff[64];
	sprintf_s(buff, 64, "%u", value);

	WriteKeyValue(key, buff, len);
	m_kStream << "," << std::endl;

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaWriteUtil::WriteData(const char* key, float value, int len)
{
	const char* szTab = "    ";
	WriteLoopString(m_nPreTabCnt+1, szTab);

	char buff[64];
	sprintf_s(buff, 64, "%f", value);

	WriteKeyValue(key, buff, len);
	m_kStream << "," << std::endl;

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaWriteUtil::WriteData(const char* key, time_t value, int len)
{
	const char* szTab = "    ";
	WriteLoopString(m_nPreTabCnt+1, szTab);

	char buff[64];
	struct tm ti;
	_localtime64_s(&ti, &value);
	sprintf_s(buff, 64, "\"%02d-%02d-%02d %02d:%02d:%02d\"", 
		ti.tm_year + 1900, ti.tm_mon + 1, ti.tm_mday,
		ti.tm_hour, ti.tm_min, ti.tm_sec);

	WriteKeyValue(key, buff, len);
	m_kStream << "," << std::endl;

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaWriteUtil::WriteData(const char* key, const NiPoint2& value, int len)
{
	const char* szTab = "    ";
	WriteLoopString(m_nPreTabCnt+1, szTab);

	char buff[64];
	sprintf_s(buff, 64, "\"%f,%f\"", value.x, value.y);

	WriteKeyValue(key, buff, len);
	m_kStream << "," << std::endl;

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaWriteUtil::WriteData(const char* key, const NiPoint3& value, int len)
{
	const char* szTab = "    ";
	WriteLoopString(m_nPreTabCnt+1, szTab);

	char buff[64];
	sprintf_s(buff, 64, "\"%f,%f,%f\"", value.x, value.y, value.z);

	WriteKeyValue(key, buff, len);
	m_kStream << "," << std::endl;

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaWriteUtil::WriteData(const char* key, const NiPoint4& value, int len)
{
	const char* szTab = "    ";
	WriteLoopString(m_nPreTabCnt+1, szTab);

	char buff[64];
	sprintf_s(buff, 64, "\"%f,%f,%f,%f\"", value.m_afPt[0], value.m_afPt[1], value.m_afPt[2], value.m_afPt[3]);

	WriteKeyValue(key, buff, len);
	m_kStream << "," << std::endl;

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaWriteUtil::WriteData(const char* key, const NiQuaternion& value, int len)
{
	const char* szTab = "    ";
	WriteLoopString(m_nPreTabCnt+1, szTab);

	char buff[64];
	sprintf_s(buff, 64, "\"%f,%f,%f,%f\"", value.m_fX, value.m_fY, value.m_fZ, value.m_fW);

	WriteKeyValue(key, buff, len);
	m_kStream << "," << std::endl;

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaWriteUtil::WriteData(const char* key, const NiColor& value, int len)
{
	const char* szTab = "    ";
	WriteLoopString(m_nPreTabCnt+1, szTab);

	char buff[64];
	sprintf_s(buff, 64, "\"%f,%f,%f\"", value.r, value.g, value.b);

	WriteKeyValue(key, buff, len);
	m_kStream << "," << std::endl;

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaWriteUtil::WriteData(const char* key, const sdVector2& value, int len)
{
	const char* szTab = "    ";
	WriteLoopString(m_nPreTabCnt+1, szTab);

	char buff[64];
	sprintf_s(buff, 64, "\"%f,%f\"", value.m_fX, value.m_fY);

	WriteKeyValue(key, buff, len);
	m_kStream << "," << std::endl;

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaWriteUtil::WriteData(const char* key, const sdVector3& value, int len)
{
	const char* szTab = "    ";
	WriteLoopString(m_nPreTabCnt+1, szTab);

	char buff[64];
	sprintf_s(buff, 64, "\"%f,%f,%f\"", value.m_fX, value.m_fY, value.m_fZ);

	WriteKeyValue(key, buff, len);
	m_kStream << "," << std::endl;

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaWriteUtil::WriteData(const char* key, const char* value, int len)
{
	const char* szTab = "    ";
	WriteLoopString(m_nPreTabCnt+1, szTab);

	char buff[1024];
	sprintf_s(buff, 1024, "\"%s\"", value);

	WriteKeyValue(key, buff, len);
	m_kStream << "," << std::endl;

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaWriteUtil::WriteData(const char* key, const std::string& value, int len)
{
	const char* szTab = "    ";
	WriteLoopString(m_nPreTabCnt+1, szTab);

	char buff[1024];
	sprintf_s(buff, 1024, "\"%s\"", value.c_str());

	WriteKeyValue(key, buff, len);
	m_kStream << "," << std::endl;

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaWriteUtil::WriteData(const char* key, const char* value, int len, bool path)
{
	const char* szTab = "    ";
	WriteLoopString(m_nPreTabCnt+1, szTab);


	char buff[1024];
	if (path)
	{
		// 替换掉字符串中的"\\"为"/"
		// (没有检查溢出)
		const char* ptr_src = value;
		char* ptr_dst = buff;
		*ptr_dst++ = '\"';
		for (; *ptr_src != '\0'; ++ptr_src, ++ptr_dst)
		{
			if (*ptr_src == '\\')
				*ptr_dst = '/';
			else
				*ptr_dst = *ptr_src;
		}
		*ptr_dst++ = '\"';
		*ptr_dst = '\0';
	}
	else
	{
		sprintf_s(buff, 1024, "\"%s\"", value);
	}

	WriteKeyValue(key, buff, len);
	m_kStream << "," << std::endl;

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaWriteUtil::WriteKeyValue(const char* key, const char* value, int len)
{
	m_kStream << key;

	int iSpaceNum = len - strlen(key);
	for (int i = 0; i < iSpaceNum; ++i)	///< 考虑用查找表加速
		m_kStream << " ";

	m_kStream << "= " << value;

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdLuaWriteUtil::WriteLoopString(int count, const char *value)
{
	for (int i = 0; i < count; ++i)
		m_kStream << value;	

	return true;
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE
ENGINE_NAMESPACE_END_UTIL