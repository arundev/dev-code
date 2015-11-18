#include "sdEnginePCH.h"
#include "sdSceneStream.h"
#include "sdMap.h"

#include "sdStrValueUtil.h"
#include "sdLuaFile.h"
#include "sdFile.h"

#include <sdFileUtil.h>

#define MAP_FILE_NAME	"map.lua"	///< ����ͼ�ļ�

using namespace Base::Diagnostics;
using namespace Base::Util;
using namespace Engine;
using namespace Engine::Util;
//-------------------------------------------------------------------------------------------------
sdSceneStream::sdSceneStream(const char* szScenePath)
{
	NIASSERT(szScenePath);
	::strncpy(m_szScenePath, szScenePath, MAX_PATH);

	// ��ʼ��lua������
#ifdef SD_LUA_ALLOCATOR
	m_pkLuaState = sd_lua_open();
#else
	m_pkLuaState = lua_open();
#endif

	luaL_openlibs(m_pkLuaState);
}
//-------------------------------------------------------------------------------------------------
sdSceneStream::~sdSceneStream()
{
	// ����lua������
	lua_close(m_pkLuaState);
	m_pkLuaState = NULL;
}
//-------------------------------------------------------------------------------------------------
bool sdSceneStream::SaveScene(const sdMap* pkMap, LoadCallback pfnCallback)
{
	NIASSERT(pkMap);

	// �����ļ���·��
	char szBuff[MAX_PATH];
	strcpy(szBuff, m_szScenePath);

	// ȷ���ļ���·������
	sdFileUtil::ConfimDir(szBuff);

	// �����ļ�·��
	char szFileFullPath[MAX_PATH];
	strcpy(szFileFullPath, szBuff);
	strcat(szFileFullPath, "\\");
	strcat(szFileFullPath, MAP_FILE_NAME);

	// ����ļ�ֻ������
	DWORD dwAttr = ::GetFileAttributes(szFileFullPath);
	if ((dwAttr != INVALID_FILE_ATTRIBUTES) && (dwAttr & FILE_ATTRIBUTE_READONLY))
	{
		DWORD dwWriteableAttr = dwAttr & ~FILE_ATTRIBUTE_READONLY;
		::SetFileAttributes(szFileFullPath, dwWriteableAttr);
	}

	// д���ļ�
	m_kOutStream.open(szFileFullPath, std::ios_base::out | std::ios_base::trunc);
	if (m_kOutStream)
	{
		SaveHeader();										///< д���ļ�ͷ
		SaveObject(const_cast<sdMap*>(pkMap), 0, "Map");	///< д���ļ�����

		m_kOutStream.close();
	}

	// �ָ��ļ�����
	if ((dwAttr != INVALID_FILE_ATTRIBUTES) && (dwAttr & FILE_ATTRIBUTE_READONLY))
	{
		::SetFileAttributes(szFileFullPath, dwAttr);
	}

	return true;
}
//-------------------------------------------------------------------------------------------------
void sdSceneStream::SaveHeader()
{
	m_kOutStream << "Header=" << std::endl;
	m_kOutStream << "{" << std::endl;

	char* pcStart = m_acBufLine + 4;			///< �����ַ���ʼλ��
	::memset(m_acBufLine, 0x20, 4);				///< ���ǰ�ÿո�

	sprintf(pcStart, "MajorVersion=%d,", 1);		///< ���汾��
	m_kOutStream << m_acBufLine << std::endl;	

	sprintf(pcStart, "MinorVersion=%d,", 0);		///< �ΰ汾��
	m_kOutStream << m_acBufLine << std::endl;	

	m_kOutStream << "}" << std::endl;
}
//-------------------------------------------------------------------------------------------------
void sdSceneStream::SaveObject(sdGameObject* pkGameObject, int iLevel, const char* szObjectName)
{
	NIASSERT(pkGameObject);

	// д���������("Light=")
	if (szObjectName && szObjectName[0] != '\0')
	{
		char* pcStart = m_acBufLine + iLevel*4;		///< �����ַ���ʼλ��
		::memset(m_acBufLine, 0x20, iLevel*4);		///< ���ǰ�ÿո�
		sprintf(pcStart, "%s=", szObjectName);
		m_kOutStream << m_acBufLine << std::endl;	///< д�뵽������
	}

	// д��ǰ������("{")
	{
		char* pcStart = m_acBufLine + iLevel*4;		///< �����ַ���ʼλ��
		::memset(m_acBufLine, 0x20, iLevel*4);		///< ���ǰ�ÿո�
		sprintf(pcStart, "{"); 
		m_kOutStream << m_acBufLine << std::endl;	///< д�뵽������
	}

	// д�����Լ�
	typedef std::vector<PropFuncEntry> PropFuncEntryVec;
	typedef std::vector<PropFuncEntry>::iterator PropFuncEntryVecItr;
	PropFuncEntryVec kPropFuncEntryVec;

	char* pcStart = m_acBufLine + (iLevel+1)*4;		///< �����ַ���ʼλ��

	pkGameObject->GetPropertyEntries(kPropFuncEntryVec);
	PropFuncEntryVecItr kIter = kPropFuncEntryVec.begin();
	PropFuncEntryVecItr kIterEnd = kPropFuncEntryVec.end();
	for (; kIter != kIterEnd; ++kIter)
	{
		PropFuncEntry& kPropFuncEntry = *kIter;

		// ��������Ҫ����������
		if (!(kPropFuncEntry.flag & PROP_ATTR_STREAM))
			continue;

		// ��ȡ���Զ�ȡָ��
		union PropGetterFunc kPropGetterFunc;
		kPropGetterFunc.pfn = kPropFuncEntry.getter;

		// ���ǰ�ÿո�(ÿ��ѭ����Ҫ��䣬��ֹ�ݹ����ʱ�ĸ������)
		::memset(m_acBufLine, 0x20, (iLevel+1)*4);		

		// д������
		switch (kPropFuncEntry.type)
		{
		case PROP_BOOL:
			sprintf(pcStart, "%s=%d,", 
				kPropFuncEntry.func_name.c_str(), 
				(pkGameObject->*kPropGetterFunc.pfn_b)());
			m_kOutStream << m_acBufLine << std::endl;
			break;

		case PROP_CHAR:
			sprintf(pcStart, "%s=%c,", 
				kPropFuncEntry.func_name.c_str(), 
				(pkGameObject->*kPropGetterFunc.pfn_c)());
			m_kOutStream << m_acBufLine << std::endl;
			break;

		case PROP_UCHAR:
			sprintf(pcStart, "%s=%u,", 
				kPropFuncEntry.func_name.c_str(), 
				(pkGameObject->*kPropGetterFunc.pfn_uc)());
			m_kOutStream << m_acBufLine << std::endl;
			break;

		case PROP_INT:
			sprintf(pcStart, "%s=%d,", 
				kPropFuncEntry.func_name.c_str(), 
				(pkGameObject->*kPropGetterFunc.pfn_i)());
			m_kOutStream << m_acBufLine << std::endl;
			break;

		case PROP_UINT:
			sprintf(pcStart, "%s=%u,", 
				kPropFuncEntry.func_name.c_str(), 
				(pkGameObject->*kPropGetterFunc.pfn_ui)());
			m_kOutStream << m_acBufLine << std::endl;
			break;

		case PROP_FLOAT:
			sprintf(pcStart, "%s=%.3f,", 
				kPropFuncEntry.func_name.c_str(), 
				(pkGameObject->*kPropGetterFunc.pfn_f)());
			m_kOutStream << m_acBufLine << std::endl;
			break;

		case PROP_NIPT2:
			sprintf(pcStart, "%s=\"%s\",", 
				kPropFuncEntry.func_name.c_str(),
				Value2Str((pkGameObject->*kPropGetterFunc.pfn_nipt2)()));
			m_kOutStream << m_acBufLine << std::endl;
			break;

		case PROP_NIPT3:
			sprintf(pcStart, "%s=\"%s\",", 
				kPropFuncEntry.func_name.c_str(),
				Value2Str((pkGameObject->*kPropGetterFunc.pfn_nipt3)()));
			m_kOutStream << m_acBufLine << std::endl;
			break;

		case PROP_NICOLOR:
			sprintf(pcStart, "%s=\"%s\",", 
				kPropFuncEntry.func_name.c_str(),
				Value2Str((pkGameObject->*kPropGetterFunc.pfn_nicolor)()));
			m_kOutStream << m_acBufLine << std::endl;
			break;

		case PROP_NISTRING:
			sprintf(pcStart, "%s=\"%s\",", 
				kPropFuncEntry.func_name.c_str(),
				Value2Str((pkGameObject->*kPropGetterFunc.pfn_nistring)()));
			m_kOutStream << m_acBufLine << std::endl;
			break;

		case PROP_SDOBJECT:
			SaveObject((pkGameObject->*kPropGetterFunc.pfn_sdobject)(), 
				iLevel+1, 
				kPropFuncEntry.func_name.c_str());
			m_kOutStream << "," << std::endl;
			break;

		case PROP_NIPT3_C:
		case PROP_NISTRING_C:
			SDDEBUGBREAKPROC();
			break;

		case PROP_SDOBJECT_C:
			SaveObjectCollection(pkGameObject, 
				kPropGetterFunc, 
				iLevel+1, 
				kPropFuncEntry.func_name.c_str());
			break;

		default:
			SDDEBUGBREAKPROC();
			break;
		}
	}

	// д���������("}")
	{
		char* pcStart = m_acBufLine + iLevel*4;		///< �����ַ���ʼλ��
		::memset(m_acBufLine, 0x20, iLevel*4);		///< ���ǰ�ÿո�
		sprintf(pcStart, "}"); 
		m_kOutStream << m_acBufLine;				///< д�뵽������
	}
}
//-------------------------------------------------------------------------------------------------
void sdSceneStream::SaveObjectCollection(sdGameObject* pkGameObject, 
	union PropGetterFunc kPropGetterFunc, int iLevel, const char* szObjectName)
{
	NIASSERT(pkGameObject);

	sdGameObject* pkResult = NULL;
	uint uiElement = (pkGameObject->*kPropGetterFunc.pfn_sdobjectc)((sdGameObject*&)pkResult, 0);

	// д�븸��������("Light=")
	if (szObjectName && szObjectName[0] != '\0')
	{
		char* pcStart = m_acBufLine + iLevel*4;		///< �����ַ���ʼλ��
		::memset(m_acBufLine, 0x20, iLevel*4);		///< ���ǰ�ÿո�
		sprintf(pcStart, "%s=", szObjectName);
		m_kOutStream << m_acBufLine << std::endl;	///< д�뵽������
	}

	// д��ǰ������("{")
	{
		char* pcStart = m_acBufLine + iLevel*4;		///< �����ַ���ʼλ��
		::memset(m_acBufLine, 0x20, iLevel*4);		///< ���ǰ�ÿո�
		sprintf(pcStart, "{"); 
		m_kOutStream << m_acBufLine << std::endl;	///< д�뵽������
	}

	// д�����
	{
		char* pcStart = m_acBufLine + (iLevel+1)*4;	///< �����ַ���ʼλ��
		::memset(m_acBufLine, 0x20, (iLevel+1)*4);	///< ���ǰ�ÿո�

		sprintf(pcStart, "TP=\"Collect\",");			///< TP="Collect"
		m_kOutStream << m_acBufLine << std::endl;	///< д�뵽������

		sprintf(pcStart, "Count=%d,", uiElement);	///< Count=x
		m_kOutStream << m_acBufLine << std::endl;	///< д�뵽������

		if (uiElement > 0)
		{
			sprintf(pcStart, "ElementType=\"%s\",", (const char*)pkResult->GetTP());
			m_kOutStream << m_acBufLine << std::endl;

			sprintf(pcStart, "Element=");
			m_kOutStream << m_acBufLine << std::endl;

			sprintf(pcStart, "{");
			m_kOutStream << m_acBufLine << std::endl;

			for (uint ui = 0; ui < uiElement; ++ui)
			{
				(pkGameObject->*kPropGetterFunc.pfn_sdobjectc)((sdGameObject*&)pkResult, ui);
				if (pkResult)
				{
					SaveObject(pkResult, iLevel+2, NULL);					
					m_kOutStream << "," << std::endl;	///< ���붺��
				}
			}

			sprintf(pcStart, "}");
			m_kOutStream << m_acBufLine << std::endl;
		}
	}

	// д���������("}")
	{
		char* pcStart = m_acBufLine + iLevel*4;		///< �����ַ���ʼλ��
		::memset(m_acBufLine, 0x20, iLevel*4);		///< ���ǰ�ÿո�
		sprintf(pcStart, "}"); 
		m_kOutStream << m_acBufLine << std::endl;	///< д�뵽������
	}
}
//-------------------------------------------------------------------------------------------------
sdMap* sdSceneStream::LoadScene(const char* szMapName, LoadCallback pfnCallback)
{
	// �����ļ���·��
	char szBuff[MAX_PATH];
	strcpy(szBuff, m_szScenePath);

	// �����ļ�·��
	char szFileFullPath[MAX_PATH];
	strcpy(szFileFullPath, szBuff);
	strcat(szFileFullPath, "\\");
	strcat(szFileFullPath, MAP_FILE_NAME);

	// ����ļ��Ƿ����
	if (!sdFileUtil::IsFileExist(szFileFullPath))
	{
		if (pfnCallback)
			pfnCallback(100, 2, "������map.lua�ļ�������!!!");
		return 0;
	}

	// �򿪳����ļ�
	sdFile* pkFile = (sdFile*)NiFile::GetFile(szFileFullPath,(NiFile::OpenMode)(sdFile::SD_READ_ONLY));
	if (!pkFile)
	{
		if (pfnCallback)
			pfnCallback(100, 2, "������map.lua�ļ���ʧ��!!!");
		return 0;
	}

	// ���س����ļ���lua������
	int iStatus = 1;
	int iFileSize = pkFile->GetFileSize();
	const char* pkFileData = pkFile->GetBuff();
	if (iFileSize > 0)
		iStatus = luaL_loadbuffer(m_pkLuaState, pkFileData, iFileSize, "SceneRoot");

	NiDelete pkFile;	///< ɾ���ļ�����
	pkFile = NULL;		///< �ÿ��ļ�ָ��

	if (iFileSize <= 0 || iStatus)
	{
		if (pfnCallback)
			pfnCallback(100, 2, "������map.lua����ʧ��!!!");
		return 0;
	}

	// ���Խ��������ļ�
	if (lua_pcall(m_pkLuaState, 0, LUA_MULTRET, 0))
	{
		if (pfnCallback)
			pfnCallback(100, 2, "������map.lua����ʧ��!!!");
		return 0;
	}

	// ��ȡ�����ļ�
	LoadHeader();

	lua_getglobal(m_pkLuaState, "Map");				///< �����ʵ��
	sdGameObject* pkGameObject = LoadObject(NULL);

	return (sdMap*)pkGameObject;
}
//-------------------------------------------------------------------------------------------------
void sdSceneStream::LoadHeader()
{
	int iMajorVersion = 0;
	int iMinorVersion = 0;
	lua_getglobal(m_pkLuaState, "Header");
	if (lua_istable(m_pkLuaState, -1))
	{
		sdLuaReadUtil::ReadData(m_pkLuaState, "MajorVersion", iMajorVersion);
		sdLuaReadUtil::ReadData(m_pkLuaState, "MinorVersion", iMinorVersion);
	}
}
//-------------------------------------------------------------------------------------------------
sdGameObject* sdSceneStream::LoadObject(const char* szObjectName)
{
	sdGameObject* pkGameObject = NULL;

	// ���Ը������ʹ�������
	if (szObjectName == NULL || szObjectName[0] == '\0')
	{
		bool bSuccess = sdLuaReadUtil::ReadData(m_pkLuaState, "TP", m_acBufLine, MAX_PATH);
		if (!bSuccess)
			return NULL;

		pkGameObject = sdGameObject::FactoryCreateObject(m_acBufLine);
		NIASSERT(pkGameObject);
	}
	else
	{
		pkGameObject = sdGameObject::FactoryCreateObject(szObjectName);
		NIASSERT(pkGameObject);
	}

	// 
	typedef std::vector<PropFuncEntry> PropFuncEntryVec;
	typedef std::vector<PropFuncEntry>::iterator PropFuncEntryVecItr;
	PropFuncEntryVec kPropFuncEntryVec;

	pkGameObject->GetPropertyEntries(kPropFuncEntryVec);
	PropFuncEntryVecItr kIter = kPropFuncEntryVec.begin();
	PropFuncEntryVecItr kIterEnd = kPropFuncEntryVec.end();
	for (; kIter != kIterEnd; ++kIter)
	{
		PropFuncEntry& kPropFuncEntry = *kIter;

		// ��ȡ����д��ָ��
		union PropSetterFunc kPropSetterFunc;
		kPropSetterFunc.pfn = kPropFuncEntry.setter;
	
		// ��������Ҫ����������
		if (!(kPropFuncEntry.flag & PROP_ATTR_STREAM))
			continue;

		union PropResult unPropResult;
		NiPoint2 kPoint2;
		NiPoint3 kPoint3;
		NiColor kColor;

		// д������
		bool bResult = false;
		switch (kPropFuncEntry.type)
		{
		case PROP_BOOL:
			bResult = sdLuaReadUtil::ReadData(m_pkLuaState, kPropFuncEntry.func_name.c_str(), unPropResult.bResult);
			if (bResult)
				(pkGameObject->*kPropSetterFunc.pfn_b)(unPropResult.bResult);
			break;

		case PROP_CHAR:
			bResult = sdLuaReadUtil::ReadData(m_pkLuaState, kPropFuncEntry.func_name.c_str(), unPropResult.cResult);
			if (bResult)
				(pkGameObject->*kPropSetterFunc.pfn_c)(unPropResult.cResult);
			break;

		case PROP_UCHAR:
			bResult = sdLuaReadUtil::ReadData(m_pkLuaState, kPropFuncEntry.func_name.c_str(), unPropResult.ucResult);
			if (bResult)
				(pkGameObject->*kPropSetterFunc.pfn_uc)(unPropResult.ucResult);
			break;

		case PROP_INT:
			bResult = sdLuaReadUtil::ReadData(m_pkLuaState, kPropFuncEntry.func_name.c_str(), unPropResult.iResult);
			if (bResult)
				(pkGameObject->*kPropSetterFunc.pfn_i)(unPropResult.iResult);
			break;

		case PROP_UINT:
			bResult = sdLuaReadUtil::ReadData(m_pkLuaState, kPropFuncEntry.func_name.c_str(), unPropResult.uiResult);
			if (bResult)
				(pkGameObject->*kPropSetterFunc.pfn_ui)(unPropResult.uiResult);
			break;

		case PROP_FLOAT:
			bResult = sdLuaReadUtil::ReadData(m_pkLuaState, kPropFuncEntry.func_name.c_str(), unPropResult.fResult);
			if (bResult)
				(pkGameObject->*kPropSetterFunc.pfn_f)(unPropResult.fResult);
			break;

		case PROP_NIPT2:
			bResult = sdLuaReadUtil::ReadData(m_pkLuaState, kPropFuncEntry.func_name.c_str(), kPoint2);
			if (bResult)
				(pkGameObject->*kPropSetterFunc.pfn_nipt2)(kPoint2);
			break;

		case PROP_NIPT3:
			bResult = sdLuaReadUtil::ReadData(m_pkLuaState, kPropFuncEntry.func_name.c_str(), kPoint3);
			if (bResult)
				(pkGameObject->*kPropSetterFunc.pfn_nipt3)(kPoint3);
			break;

		case PROP_NICOLOR:
			bResult = sdLuaReadUtil::ReadData(m_pkLuaState, kPropFuncEntry.func_name.c_str(), kColor);
			if (bResult)
				(pkGameObject->*kPropSetterFunc.pfn_nicolor)(kColor);
			break;

		case PROP_NISTRING:
			bResult = sdLuaReadUtil::ReadData(m_pkLuaState, kPropFuncEntry.func_name.c_str(), m_acBufLine, MAX_PATH);
			if (bResult)
				(pkGameObject->*kPropSetterFunc.pfn_nistring)(m_acBufLine);
			break;

		case PROP_SDOBJECT:
			lua_getfield(m_pkLuaState, -1, kPropFuncEntry.func_name.c_str());	///< ��Object��ջ
			bResult = lua_isnil(m_pkLuaState, -1);
			if (!bResult)
			{
				unPropResult.pkObjectResult = LoadObject(NULL);
				if (unPropResult.pkObjectResult)
					(pkGameObject->*kPropSetterFunc.pfn_sdobject)((const sdGameObject*&)unPropResult.pkObjectResult);
			}
			lua_pop(m_pkLuaState, 1);
			break;

		case PROP_NIPT3_C:
		case PROP_NISTRING_C:
			SDDEBUGBREAKPROC();
			break;

		case PROP_SDOBJECT_C:
			lua_getfield(m_pkLuaState, -1, kPropFuncEntry.func_name.c_str());	///< ��Object��ջ
			bResult = lua_isnil(m_pkLuaState, -1);
			if (!bResult)
				LoadObjectCollection(pkGameObject, kPropSetterFunc);
			lua_pop(m_pkLuaState, 1);
			break;

		default:
			SDDEBUGBREAKPROC();
			break;
		}
	}

	return pkGameObject;
}
//-------------------------------------------------------------------------------------------------
uint sdSceneStream::LoadObjectCollection(sdGameObject* pkGameObject, union PropSetterFunc kPropSetterFunc)
{
	NIASSERT(pkGameObject);

	char pcBuf[MAX_PATH];
	uint uiElementCount = 0;
	uint uiCount = 0;

	sdLuaReadUtil::ReadData(m_pkLuaState, "TP", pcBuf, MAX_PATH);
	sdLuaReadUtil::ReadData(m_pkLuaState, "Count", uiElementCount);
	if (uiElementCount == 0)
		return uiElementCount;

	sdLuaReadUtil::ReadData(m_pkLuaState, "ElementType", pcBuf, MAX_PATH);

	lua_getfield(m_pkLuaState, -1, "Element");
	lua_pushnil(m_pkLuaState);
	while (lua_next(m_pkLuaState, -2))
	{
		sdGameObject* pkChildObject = LoadObject(pcBuf);
		if (pkChildObject)
		{
			(pkGameObject->*kPropSetterFunc.pfn_sdobjectc)((const sdGameObject*&)pkChildObject, uiElementCount);
			++uiCount;
		}
		lua_pop(m_pkLuaState, 1);
	}
	lua_pop(m_pkLuaState, 1);
	
	return uiCount;
}
//-------------------------------------------------------------------------------------------------