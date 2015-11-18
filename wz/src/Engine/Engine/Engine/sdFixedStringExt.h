//*************************************************************************************************
// ����:	Gamebryo��NiFixedString����չ(�ο���WZ::Engine)
//---------------------------------------------------------
// ����:		
// ����:		
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_UTIL_STRVALUE_UTIL_H__
#define _SD_ENGINE_UTIL_STRVALUE_UTIL_H__

ENGINE_NAMESPACE_BEGIN_ENGINE
//-------------------------------------------------------------------------------------------------
// Gamebryo��NiFixedString�ڲ���ͳһ���ַ���������
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