//*************************************************************************************************
// 内容:	字符串与数值转换
//
//	1.内部使用的是单个静态缓存,线程不安全且不能同时使用两个
//	2.没有检查数据以提高执行效率
//---------------------------------------------------------
// 作者:		
// 创建:		
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_UTIL_STRVALUE_UTIL_H__
#define _SD_ENGINE_UTIL_STRVALUE_UTIL_H__

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_UTIL
//-----------------------------------------------------------------------------
const char*		Value2Str(bool value);
//const char*	Value2Str(char value);
//const char*	Value2Str(uchar value);
//const char*	Value2Str(short value);
//const char*	Value2Str(ushort value);
const char*		Value2Str(int value, bool bHex = false);
const char*		Value2Str(uint value, bool bHex = false);
const char*		Value2Str(llong value);
const char*		Value2Str(ullong value);
const char*		Value2Str(float value);

const char*		Value2Str(const char* value);
const char*		Value2Str(const std::string& value);

//inline const char*	Value2Str(BYTE value, bool bHex = false);
//inline const char*	Value2Str(WORD value, bool bHex = false);
const char*		Value2Str(DWORD value, bool bHex = false);

const char*		Value2Str(const NiPoint2& value);
const char*		Value2Str(const NiPoint3& value);
const char*		Value2Str(const NiQuaternion& value);
const char*		Value2Str(const NiColor& value);
const char*		Value2Str(const NiColorA& value);
const char*		Value2Str(const NiFixedString& value);

const char*		Value2Str(const std::vector<uint>& value);
//-----------------------------------------------------------------------------
bool			Str2Bool(const char* value);
//char			String2Char(const char* value);
//uchar			String2UChar(const char* value);
//short			String2Short(const char* value);
//ushort		String2UShort(const char* value);
int				Str2Int(const char* value);
uint			Str2UInt(const char* value);
llong			Str2LLong(const char* value);
ullong			Str2ULLong(const char* value);
float			Str2Float(const char* value);

DWORD			Str2DWORD(const char* value);
//POINT			Str2POINT(const char* value);
RECT			Str2RECT(const char* value);

NiPoint2		Str2NiPoint2(const char* value);
NiPoint3		Str2NiPoint3(const char* value);
NiQuaternion	Str2NiQuaternion(const char* value);
NiColor			Str2NiColor(const char* value);
NiColorA		Str2NiColorA(const char* value);
NiFixedString	Str2NiFixedString(const char* value);
//-----------------------------------------------------------------------------
inline bool				Str2Bool(const std::string& value) { return Str2Bool(value.c_str());}
//inline char			String2Char(const std::string& value);
//inline uchar			String2UChar(const std::string& value);
//inline short			String2Short(const std::string& value);
//inline ushort			String2UShort(const std::string& value);
inline int				Str2Int(const std::string& value) { return Str2Int(value.c_str());}
inline uint				Str2UInt(const std::string& value) { return Str2UInt(value.c_str());}
inline llong			Str2LLong(const std::string& value) { return Str2LLong(value.c_str());}
inline ullong			Str2ULLong(const std::string& value) { return Str2ULLong(value.c_str());}
inline float			Str2Float(const std::string& value) { return Str2Float(value.c_str());}

inline DWORD			Str2DWORD(const std::string& value) { return Str2DWORD(value.c_str());}
//inline POINT			Str2POINT(const std::string& value);
inline RECT				Str2RECT(const std::string& value) { return Str2RECT(value.c_str());}

inline NiPoint2			Str2NiPoint2(const std::string& value) { return Str2NiPoint2(value.c_str());}
inline NiPoint3			Str2NiPoint3(const std::string& value) { return Str2NiPoint3(value.c_str());}
inline NiQuaternion		Str2NiQuaternion(const std::string& value) { return Str2NiQuaternion(value.c_str());}
inline NiColor			Str2NiColor(const std::string& value) { return Str2NiColor(value.c_str());}
inline NiColorA			Str2NiColorA(const std::string& value)  { return Str2NiColorA(value.c_str());}
inline NiFixedString	Str2NiFixedString(const std::string& value) { return Str2NiFixedString(value.c_str());}
//-----------------------------------------------------------------------------
inline void Str2Value(const char* str, bool& value) { value = Str2Bool(str);}
inline void Str2Value(const char* str, char& value) { value = Str2Int(str);}
inline void Str2Value(const char* str, uchar& value) { value = Str2UInt(str);}
inline void Str2Value(const char* str, short& value) { value = Str2Int(str);}
inline void Str2Value(const char* str, ushort& value) { value = Str2UInt(str);}
inline void Str2Value(const char* str, int& value) { value = Str2Int(str);}
inline void Str2Value(const char* str, uint& value) { value = Str2UInt(str);}
inline void Str2Value(const char* str, llong& value) { value = Str2LLong(str);}
inline void Str2Value(const char* str, ullong& value) { value = Str2ULLong(str);}
inline void Str2Value(const char* str, float& value) { value = Str2Float(str);}
inline void Str2Value(const char* str, std::string& value) { value = str;}
inline void Str2Value(const char* str, DWORD& value) { value = Str2DWORD(str);}
inline void Str2Value(const char* str, RECT& value) { value = Str2RECT(str);}
inline void Str2Value(const char* str, NiPoint2& value) { value = Str2NiPoint2(str);}
inline void Str2Value(const char* str, NiPoint3& value) { value = Str2NiPoint3(str);}
inline void Str2Value(const char* str, NiQuaternion& value) { value = Str2NiQuaternion(str);}
inline void Str2Value(const char* str, NiColor& value) { value = Str2NiColor(str);}
inline void Str2Value(const char* str, NiColorA& value) { value = Str2NiColorA(str);}
inline void Str2Value(const char* str, NiFixedString& value) { value = str;}
//-----------------------------------------------------------------------------
inline void Str2Value(const std::string& str, bool& value) { value = Str2Bool(str);}
inline void Str2Value(const std::string& str, char& value) { value = Str2Int(str);}
inline void Str2Value(const std::string& str, uchar& value) { value = Str2UInt(str);}
inline void Str2Value(const std::string& str, short& value) { value = Str2Int(str);}
inline void Str2Value(const std::string& str, ushort& value) { value = Str2UInt(str);}
inline void Str2Value(const std::string& str, int& value) { value = Str2Int(str);}
inline void Str2Value(const std::string& str, uint& value) { value = Str2UInt(str);}
inline void Str2Value(const std::string& str, llong& value) { value = Str2LLong(str);}
inline void Str2Value(const std::string& str, ullong& value) { value = Str2ULLong(str);}
inline void Str2Value(const std::string& str, float& value) { value = Str2Float(str);}
inline void Str2Value(const std::string& str, std::string& value) { value = str;}
inline void Str2Value(const std::string& str, DWORD& value) { value = Str2DWORD(str);}
inline void Str2Value(const std::string& str, RECT& value) { value = Str2RECT(str);}
inline void Str2Value(const std::string& str, NiPoint2& value) { value = Str2NiPoint2(str);}
inline void Str2Value(const std::string& str, NiPoint3& value) { value = Str2NiPoint3(str);}
inline void Str2Value(const std::string& str, NiQuaternion& value) { value = Str2NiQuaternion(str);}
inline void Str2Value(const std::string& str, NiColor& value) { value = Str2NiColor(str);}
inline void Str2Value(const std::string& str, NiColorA& value) { value = Str2NiColorA(str);}
inline void Str2Value(const std::string& str, NiFixedString& value) { value = str.c_str();}
//-----------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE
ENGINE_NAMESPACE_END_UTIL
#endif