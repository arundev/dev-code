#include "sdEnginePCH.h"
#include "sdStrValueUtil.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_UTIL
//-------------------------------------------------------------------------------------------------
const char* Value2Str(bool value)
{
	static char szBuff[MAX_PATH];
	sprintf_s(szBuff, "%s", value ? "true": "flase");
	return szBuff;
}
//-------------------------------------------------------------------------------------------------
const char* Value2Str(int value, bool bHex /* = false */)
{
	static char szBuff[MAX_PATH];
	sprintf_s(szBuff, bHex ? "0x%x" : "%d", value);
	return szBuff;
}
//-------------------------------------------------------------------------------------------------
const char* Value2Str(uint value, bool bHex /* = false */)
{
	static char szBuff[MAX_PATH];
	sprintf_s(szBuff, bHex ? "0x%x" : "%u", value);
	return szBuff;
}
//-------------------------------------------------------------------------------------------------
const char* Value2Str(llong value)
{
	static char szBuff[MAX_PATH];
	sprintf_s(szBuff, "%I64d", value);
	return szBuff;
}
//-------------------------------------------------------------------------------------------------
const char* Value2Str(ullong value)
{
	static char szBuff[MAX_PATH];
	sprintf_s(szBuff, "%I64u", value);
	return szBuff;
}
//-------------------------------------------------------------------------------------------------
const char* Value2Str(float value)
{
	static char szBuff[MAX_PATH];
	sprintf_s(szBuff, "%.3f", value);
	return szBuff;
}
//-------------------------------------------------------------------------------------------------
const char* Value2Str(const char* value)
{
	static char szBuff[MAX_PATH * 16];
	sprintf_s(szBuff, "%s", value);
	return szBuff;
}
//-------------------------------------------------------------------------------------------------
const char* Value2Str(const std::string& value)
{
	static char szBuff[MAX_PATH * 16];
	sprintf_s(szBuff, "%s", value.c_str());
	return szBuff;
}
//-------------------------------------------------------------------------------------------------
const char* Value2Str(DWORD value, bool bHex /* = false */)
{
	static char szBuff[MAX_PATH];
	sprintf_s(szBuff, bHex ? "0x%x" : "%u", value);
	return szBuff;
}
//-------------------------------------------------------------------------------------------------
const char* Value2Str(const NiPoint2& value)
{
	static char szBuff[MAX_PATH];
	sprintf_s(szBuff, "%.3f,%.3f", value.x, value.y);
	return szBuff;
}
//-------------------------------------------------------------------------------------------------
const char* Value2Str(const NiPoint3& value)
{
	static char szBuff[MAX_PATH];
	sprintf_s(szBuff, "%.3f,%.3f,%.3f", value.x, value.y, value.z);
	return szBuff;
}
//-------------------------------------------------------------------------------------------------
const char* Value2Str(const NiQuaternion& value)
{
	static char szBuff[MAX_PATH];
	sprintf_s(szBuff, "%.3f,%.3f,%.3f,%.3f", value.m_fX, value.m_fY, value.m_fZ, value.m_fW);
	return szBuff;
}
//-------------------------------------------------------------------------------------------------
const char* Value2Str(const NiColor& value)
{
	static char szBuff[MAX_PATH];
	sprintf_s(szBuff, "%.3f,%.3f,%.3f", value.r, value.g, value.b);
	return szBuff;
}
//-------------------------------------------------------------------------------------------------
const char* Value2Str(const NiColorA& value)
{
	static char szBuff[MAX_PATH];
	sprintf_s(szBuff, "%.3f,%.3f,%.3f,%.3f", value.r, value.g, value.b, value.a);
	return szBuff;
}
//-------------------------------------------------------------------------------------------------
const char* Value2Str(const NiFixedString& value)
{
	static char szBuff[MAX_PATH * 16];
	sprintf_s(szBuff, "%s", (const char*)value);
	return szBuff;
}
//-------------------------------------------------------------------------------------------------
const char* Value2Str(const std::vector<uint>& value)
{
	static char szBuff[MAX_PATH * 16];
	
	char* pcBuffer = szBuff;
	std::vector<uint>::const_iterator itr = value.begin();
	std::vector<uint>::const_iterator itr_end = value.end();
	for (; itr!=itr_end; ++itr)
	{
		char szInnerBuff[16];
		sprintf_s(szInnerBuff, "%d,", *itr);
		
		int iLength = strlen(szInnerBuff);
		memcpy(pcBuffer, szInnerBuff, iLength);
		pcBuffer += iLength;
	}

	return szBuff;	
}
//-------------------------------------------------------------------------------------------------
bool Str2Bool(const char* value)
{
	return !stricmp(value, "true");
}
//-------------------------------------------------------------------------------------------------
int Str2Int(const char* value)
{
	return atoi(value);
}
//-------------------------------------------------------------------------------------------------
uint Str2UInt(const char* value)
{
	return strtoul(value, 0, 10);
}
//-------------------------------------------------------------------------------------------------
llong Str2LLong(const char* value)
{
	llong ret = 0;
	sscanf_s(value, "%I64d", &ret);
	return ret;
}
//-------------------------------------------------------------------------------------------------
ullong Str2ULLong(const char* value)
{
	ullong ret = 0;
	sscanf_s(value, "%I64u", &ret);
	return ret;
}
//-------------------------------------------------------------------------------------------------
float Str2Float(const char* value)
{
	return (float)atof(value);
}
//-------------------------------------------------------------------------------------------------
DWORD Str2DWORD(const char* value)
{
	DWORD ret = 0;
	sscanf_s(value, "%u", &ret);
	return ret;
}
//-------------------------------------------------------------------------------------------------
RECT Str2RECT(const char* value)
{
	RECT ret;
	sscanf_s(value, "(%d, %d, %d, %d)", &ret.left, &ret.right, &ret.top, &ret.bottom);
	return ret;
}
//-------------------------------------------------------------------------------------------------
NiPoint2 Str2NiPoint2(const char* value)
{
	NiPoint2 ret;
	sscanf_s(value, "%f, %f", &ret.x, &ret.y);
	return ret;
}
//-------------------------------------------------------------------------------------------------
NiPoint3 Str2NiPoint3(const char* value)
{
	NiPoint3 ret;
	sscanf_s(value, "%f, %f, %f", &ret.x, &ret.y, &ret.z);
	return ret;
}
//-------------------------------------------------------------------------------------------------
NiQuaternion Str2NiQuaternion(const char* value)
{
	NiQuaternion ret;
	sscanf_s(value, "%f, %f, %f, %f", &ret.m_fX, &ret.m_fY, &ret.m_fZ, &ret.m_fW);
	return ret;
}
//-------------------------------------------------------------------------------------------------
NiColor Str2NiColor(const char* value)
{
	NiColor ret;
	sscanf_s(value, "%f, %f, %f", &ret.r, &ret.g, &ret.b);
	return ret;
}
//-------------------------------------------------------------------------------------------------
NiColorA Str2NiColorA(const char* value)
{
	NiColorA ret = 0;
	sscanf_s(value, "%f, %f, %f, %f", &ret.r, &ret.g, &ret.b, &ret.a);
	return ret;
}
//-------------------------------------------------------------------------------------------------
NiFixedString Str2NiFixedString(const char* value)
{
	return value;
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE
ENGINE_NAMESPACE_END_UTIL