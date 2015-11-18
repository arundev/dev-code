#include "sdBasePCH.h"
#include "sdIniFile.h"

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_IO
//-----------------------------------------------------------------------------------------
void sdIniFile::Open(const std::string &szFullPath)
{
	assert(!szFullPath.empty());
	m_szFullPath = szFullPath;
}
//-----------------------------------------------------------------------------------------
char* sdIniFile::ReadConfigString(const char* szSection, const char* szKey, const char* szDefault /* = "" */)
{
	static uint uiSize = 1024;
	static char szBuff[1024] = {0};

	assert(szSection);
	assert(szKey);
	assert(szDefault);

	::ZeroMemory(szBuff, uiSize);
	::GetPrivateProfileString(szSection, szKey, szDefault, szBuff, uiSize, m_szFullPath.c_str());

	return szBuff;
}
//-----------------------------------------------------------------------------------------
int sdIniFile::ReadConfigInt(const char* szSection, const char* szKey, int iDefault /* = 0 */)
{
	int iReturn = GetPrivateProfileInt(szSection, szKey, iDefault, m_szFullPath.c_str());
	return iReturn;
}
//-----------------------------------------------------------------------------------------
bool sdIniFile::ReadConfigBoolean(const char* szSection, const char* szKey, bool bDefault /* = false */)
{
	int iReturn = ReadConfigInt(szSection, szKey, bDefault ? 1: 0);
	return iReturn != 0;
}
//-----------------------------------------------------------------------------------------
float sdIniFile::ReadConfigFloat(const char* szSection, const char* szKey, float fDefault /* = 0 */)
{
	uint uiSize = 64;
	char szBuff[64] = {0};
	char szDefault[64] = {0};

	sprintf(szDefault, "%f", fDefault);
	::GetPrivateProfileString(szSection, szKey, szDefault, szBuff, uiSize, m_szFullPath.c_str());
	if (strlen(szBuff))
		return (float)atof(szBuff);
	else
		return fDefault;
}
//-----------------------------------------------------------------------------------------
void sdIniFile::WriteConfigString(const char *szSection, const char *szKey, const char *szValue)
{
	::WritePrivateProfileString(szSection, szKey, szValue, m_szFullPath.c_str());
}
//-----------------------------------------------------------------------------------------
void sdIniFile::WriteConfigInt(const char* szSection, const char* szKey, int iValue)
{
	uint uiSize = 64;
	char szBuff[64] = {0};

	sprintf(szBuff, "%d", iValue);
	::WritePrivateProfileString(szSection, szKey, szBuff, m_szFullPath.c_str());
}
//-----------------------------------------------------------------------------------------
void sdIniFile::WriteConfigBoolean(const char* szSection, const char* szKey, bool bValue)
{
	char* pszBuff = bValue ? "1": "0";
	::WritePrivateProfileString(szSection, szKey, pszBuff, m_szFullPath.c_str());
}
//-----------------------------------------------------------------------------------------
void sdIniFile::WriteConfigFloat(const char* szSection, const char* szKey, float fValue)
{
	uint uiSize = 64;
	char szBuff[64] = {0};

	if (fValue == static_cast<int>(fValue))
		sprintf(szBuff, "%i", fValue);
	else 
		sprintf(szBuff, "%.4f", fValue);
	::WritePrivateProfileString(szSection, szKey, szBuff, m_szFullPath.c_str());
}
//-----------------------------------------------------------------------------------------
BASE_NAMESPACE_END_BASE
BASE_NAMESPACE_END_IO