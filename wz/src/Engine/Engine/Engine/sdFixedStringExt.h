//*************************************************************************************************
// 内容:	Gamebryo的NiFixedString的扩展(参考自WZ::Engine)
//---------------------------------------------------------
// 作者:		
// 创建:		
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_UTIL_STRVALUE_UTIL_H__
#define _SD_ENGINE_UTIL_STRVALUE_UTIL_H__

ENGINE_NAMESPACE_BEGIN_ENGINE
//-------------------------------------------------------------------------------------------------
// Gamebryo的NiFixedString内部有统一的字符串管理功能
//-------------------------------------------------------------------------------------------------
struct FnNiFixedStringLess
{
public:
	bool operator()(const NiFixedString& lhs, const NiFixedString& rhs) const
	{
		const char* lhs_cstr = (const char*)lhs;
		const char* rhs_cstr = (const char*)rhs;

		if (lhs_cstr == rhs_cstr)
			return false;

		size_t iLLen = lhs.GetLength();
		size_t iRLen = rhs.GetLength();

		return (iLLen < iRLen) || ((iLLen == iRLen) && (std::memcmp(lhs_cstr, rhs_cstr, iLLen) < 0));
	}
};
//-------------------------------------------------------------------------------------------------
template <typename T>
class TFixedStringMap : public std::map<NiFixedString, T, FnNiFixedStringLess>
{

};
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE
#endif