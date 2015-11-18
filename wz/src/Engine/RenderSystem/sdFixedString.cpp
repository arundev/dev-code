#include "sdRenderSystemPCH.h"
#include "sdFixedString.h"

// 控制静态变量初始化顺序
#pragma warning(push) 
#pragma warning(disable:4073) 
#pragma init_seg(lib)
#pragma warning(pop) 

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
std::vector<sdFixedString*> sdFixedString::ms_kDelayReleaseVec;
//-------------------------------------------------------------------------------------------------
sdFixedString::sdFixedString()
: NiFixedString()
{
}
//-------------------------------------------------------------------------------------------------
sdFixedString::sdFixedString(const char* szString)
: NiFixedString(szString)
, m_bNeverRelease(false)
{
}
//-------------------------------------------------------------------------------------------------
sdFixedString::sdFixedString(const char* szString, bool bNeverRelease)
: NiFixedString(szString)
, m_bNeverRelease(bNeverRelease)
{
	if (m_bNeverRelease)
	{
		ms_kDelayReleaseVec.push_back(this);
	}
}
//-------------------------------------------------------------------------------------------------
sdFixedString::sdFixedString(sdFixedString& szFixedString)
: NiFixedString(szFixedString)
, m_bNeverRelease(szFixedString.m_bNeverRelease)
{
	if (m_bNeverRelease)
	{
		ms_kDelayReleaseVec.push_back(this);
	}
}
//-------------------------------------------------------------------------------------------------
sdFixedString::sdFixedString(NiFixedString& szFixedString)
: NiFixedString(szFixedString)
, m_bNeverRelease(false)
{
}
//-------------------------------------------------------------------------------------------------
sdFixedString::~sdFixedString()
{
	if (m_bNeverRelease)
	{
		m_handle = NiGlobalStringTable::NULL_STRING;
	}
}
//-------------------------------------------------------------------------------------------------
void sdFixedString::Shutdown()
{
	size_t nCount = ms_kDelayReleaseVec.size();
	for (size_t i = 0; i < nCount; ++i)
	{
		sdFixedString* pszFixedString = ms_kDelayReleaseVec[i];
		NiGlobalStringTable::DecRefCount(pszFixedString->m_handle);
	}
	ms_kDelayReleaseVec.clear();
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE