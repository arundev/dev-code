//*************************************************************************************************
// 内容:	字符串的封装,用于只需要初始化一次之后就再也不变更的字符串
//---------------------------------------------------------
// 作者:		
// 创建:		2012-06-30
// 最后修改:	2014-03-19
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_FIXED_STRING_H__
#define _SD_ENGINE_RENDER_SYSTEM_FIXED_STRING_H__
#include <NiFixedString.h>

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
class sdFixedString: public NiFixedString
{
public:
	sdFixedString();
	sdFixedString(const char* szString);
	sdFixedString(const char* szString, bool bNeverRelease);
	sdFixedString(sdFixedString& szFixedString);
	sdFixedString(NiFixedString& szFixedString);
	~sdFixedString();

	static void Shutdown();

protected:
	bool	m_bNeverRelease;

	typedef std::vector<sdFixedString*> FixedStringVec;
	typedef std::vector<sdFixedString*>::iterator FixedStringVecItr;
	static FixedStringVec ms_kDelayReleaseVec;
};
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif
