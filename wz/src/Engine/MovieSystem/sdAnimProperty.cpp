#include "sdMovieSystemPCH.h"
#include "sdAnimProperty.h"

#include <sdLuaFile.h>
#include <sdStrValueUtil.h>

using namespace Engine;
using namespace Engine::Util;

ENGINE_NAMESPACE_BEGIN_MOVIESYSTEM
//-------------------------------------------------------------------------------------------------
// sdAnimValue
//-------------------------------------------------------------------------------------------------
sdAnimValue::sdAnimValue(const bool& bValue) 
: m_eType(E_ANIM_VALUE_BOOL)
, m_bTrackBalue(false)
{
	Set(bValue);
}
//-------------------------------------------------------------------------------------------------
sdAnimValue::sdAnimValue(const int& iValue) 
: m_eType(E_ANIM_VALUE_INT)
, m_bTrackBalue(false)
{
	Set(iValue);
}
//-------------------------------------------------------------------------------------------------
sdAnimValue::sdAnimValue(const uint& uiValue) 
: m_eType(E_ANIM_VALUE_UINT)
, m_bTrackBalue(false)
{
	Set(uiValue);
}
//-------------------------------------------------------------------------------------------------
sdAnimValue::sdAnimValue(const float& fValue) 
: m_eType(E_ANIM_VALUE_FLOAT)
, m_bTrackBalue(false)
{
	Set(fValue);
}
//-------------------------------------------------------------------------------------------------
sdAnimValue::sdAnimValue(const NiPoint2& kValue) 
: m_eType(E_ANIM_VALUE_NIPOINT2)
, m_bTrackBalue(false)
{
	Set(kValue);
}
//-------------------------------------------------------------------------------------------------
sdAnimValue::sdAnimValue(const NiPoint3& kValue) 
: m_eType(E_ANIM_VALUE_NIPOINT3)
, m_bTrackBalue(false)
{
	Set(kValue);
}
//-------------------------------------------------------------------------------------------------
sdAnimValue::sdAnimValue(const NiColor& kValue) 
: m_eType(E_ANIM_VALUE_NICOLOR)
, m_bTrackBalue(false)
{
	Set(kValue);
}
//-------------------------------------------------------------------------------------------------
sdAnimValue::sdAnimValue(const NiQuaternion& kValue) 
: m_eType(E_ANIM_VALUE_NIQUATERNION)
, m_bTrackBalue(false)
{
	Set(kValue);
}
//-------------------------------------------------------------------------------------------------
sdAnimValue::sdAnimValue(const std::string& strValue) 
: m_eType(E_ANIM_VALUE_STRING)
, m_bTrackBalue(false)
{
	Set(strValue);
}
//-------------------------------------------------------------------------------------------------
sdAnimValue::sdAnimValue(const char* szValue) 
: m_eType(E_ANIM_VALUE_STRING)
, m_bTrackBalue(false)
{
	Set(szValue);
}
//-------------------------------------------------------------------------------------------------
sdAnimValue::sdAnimValue(const MovieSystem::sdAnimValue *pkAnimValue)
: m_eType(pkAnimValue->m_eType)
, m_bTrackBalue(false)
{
	NIASSERT(pkAnimValue);

	switch (m_eType)
	{
	case E_ANIM_VALUE_BOOL:
		m_bValue = pkAnimValue->m_bValue;
		break;

	case E_ANIM_VALUE_INT:
		m_iValue = pkAnimValue->m_iValue;
		break;

	case E_ANIM_VALUE_UINT:
		m_uiValue = pkAnimValue->m_uiValue;
		break;

	case E_ANIM_VALUE_NIPOINT2:
		memcpy_s(m_kValuePt2, 2,  pkAnimValue->m_kValuePt2, 2);
		break;

	case E_ANIM_VALUE_NIPOINT3:
		memcpy_s(m_kValuePt3, 3, pkAnimValue->m_kValuePt3, 3);
		break;

	case E_ANIM_VALUE_NICOLOR:
		memcpy_s(m_kValueColor, 3, pkAnimValue->m_kValueColor, 3);
		break;

	case E_ANIM_VALUE_NIQUATERNION:
		memcpy_s(m_kValueQuaternion, 4, pkAnimValue->m_kValueQuaternion, 4);
		break;

	case E_ANIM_VALUE_STRING:
		m_pkValueString = new std::string(*pkAnimValue->m_pkValueString);
		break;
	}
}
//-------------------------------------------------------------------------------------------------
sdAnimValue::sdAnimValue(const sdAnimValue& kAnimValue)
: m_eType(kAnimValue.m_eType)
, m_bTrackBalue(false)
{
	switch (m_eType)
	{
	case E_ANIM_VALUE_BOOL:
		m_bValue = kAnimValue.m_bValue;
		break;

	case E_ANIM_VALUE_INT:
		m_iValue = kAnimValue.m_iValue;
		break;

	case E_ANIM_VALUE_UINT:
		m_uiValue = kAnimValue.m_uiValue;
		break;

	case E_ANIM_VALUE_NIPOINT2:
		memcpy_s(m_kValuePt2, 2,  kAnimValue.m_kValuePt2, 2);
		break;

	case E_ANIM_VALUE_NIPOINT3:
		memcpy_s(m_kValuePt3, 3, kAnimValue.m_kValuePt3, 3);
		break;

	case E_ANIM_VALUE_NICOLOR:
		memcpy_s(m_kValueColor, 3, kAnimValue.m_kValueColor, 3);
		break;

	case E_ANIM_VALUE_NIQUATERNION:
		memcpy_s(m_kValueQuaternion, 4, kAnimValue.m_kValueQuaternion, 4);
		break;

	case E_ANIM_VALUE_STRING:
		m_pkValueString = new std::string(*kAnimValue.m_pkValueString);
		break;
	}
}
//-------------------------------------------------------------------------------------------------
sdAnimValue::~sdAnimValue()
{
	if (m_eType == E_ANIM_VALUE_STRING)
	{
		delete m_pkValueString;
		m_pkValueString = NULL;
	}
}
//-------------------------------------------------------------------------------------------------
bool sdAnimValue::FromString(const std::string& strValue)
{
	switch (m_eType)
	{
	case E_ANIM_VALUE_BOOL:
		Str2Value(strValue, m_bValue);
		break;

	case E_ANIM_VALUE_INT:
		Str2Value(strValue, m_iValue);
		break;

	case E_ANIM_VALUE_UINT:
		Str2Value(strValue, m_uiValue);
		break;

	case E_ANIM_VALUE_NIPOINT2:
		{
			NiPoint2 kValue;
			Str2Value(strValue, kValue);
			Set(kValue);
		}
		break;

	case E_ANIM_VALUE_NIPOINT3:
		{
			NiPoint3 kValue;
			Str2Value(strValue, kValue);
			Set(kValue);
		}
		break;

	case E_ANIM_VALUE_NICOLOR:
		{
			NiColor kValue;
			Str2Value(strValue, kValue);
			Set(kValue);
		}
		break;

	case E_ANIM_VALUE_NIQUATERNION:
		{
			NiQuaternion kValue;
			Str2Value(strValue, kValue);
			Set(kValue);
		}
		break;

	case E_ANIM_VALUE_STRING:
		*m_pkValueString = strValue;
		break;

	default:
		return false;
	}

	return true;
}
//-------------------------------------------------------------------------------------------------
std::string sdAnimValue::ToString() const
{
	switch (m_eType)
	{
	case E_ANIM_VALUE_BOOL:
		return Value2Str(m_bValue);

	case E_ANIM_VALUE_INT:
		return Value2Str(m_iValue);

	case E_ANIM_VALUE_UINT:
		return Value2Str(m_uiValue);

	case E_ANIM_VALUE_FLOAT:
		return Value2Str(m_fValue);

	case E_ANIM_VALUE_NIPOINT2:
		{
			NiPoint2 kValue;
			Get(kValue);
			return Value2Str(kValue);
		}

	case E_ANIM_VALUE_NIPOINT3:
		{
			NiPoint3 kValue;
			Get(kValue);
			return Value2Str(kValue);
		}

	case E_ANIM_VALUE_NICOLOR:
		{
			NiColor kValue;
			Get(kValue);
			return Value2Str(kValue);
		}

	case E_ANIM_VALUE_NIQUATERNION:
		{
			NiQuaternion kValue;
			Get(kValue);
			return Value2Str(kValue);
		}

	case E_ANIM_VALUE_STRING:
		return *m_pkValueString;
	}

	return "";
}
//-------------------------------------------------------------------------------------------------
bool sdAnimValue::Save(Engine::Util::sdLuaWriteUtil& kLuaStream, const std::string& strKey)
{
	switch (m_eType)
	{
	case E_ANIM_VALUE_BOOL:
		kLuaStream.WriteData(strKey.c_str(), m_bValue, 15);
		break;

	case E_ANIM_VALUE_INT:
		kLuaStream.WriteData(strKey.c_str(), m_iValue, 15);
		break;

	case E_ANIM_VALUE_UINT:
		kLuaStream.WriteData(strKey.c_str(), m_uiValue, 15);
		break;

	case E_ANIM_VALUE_FLOAT:
		kLuaStream.WriteData(strKey.c_str(), m_fValue, 15);
		break;

	case E_ANIM_VALUE_NIPOINT2:
		{
			NiPoint2 kValue;
			Get(kValue);
			kLuaStream.WriteData(strKey.c_str(), kValue, 15);
		}
		break;

	case E_ANIM_VALUE_NIPOINT3:
		{
			NiPoint3 kValue;
			Get(kValue);
			kLuaStream.WriteData(strKey.c_str(), kValue, 15);
		}
		break;

	case E_ANIM_VALUE_NICOLOR:
		{
			NiColor kValue;
			Get(kValue);
			kLuaStream.WriteData(strKey.c_str(), kValue, 15);
		}
		break;

	case E_ANIM_VALUE_NIQUATERNION:
		{
			NiQuaternion kValue;
			Get(kValue);
			kLuaStream.WriteData(strKey.c_str(), kValue, 15);
		}
		break;

	case E_ANIM_VALUE_STRING:
		kLuaStream.WriteData(strKey.c_str(), m_pkValueString->c_str(), 15);
		break;

	default:
		return false;
	};

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdAnimValue::Load(Engine::Util::sdLuaReadUtil& kLuaStream, const std::string& strKey)
{
	switch (m_eType)
	{
	case E_ANIM_VALUE_BOOL:
		if (!kLuaStream.ReadData(strKey.c_str(), m_bValue))
			return false;
		break;

	case E_ANIM_VALUE_INT:
		if (!kLuaStream.ReadData(strKey.c_str(), m_iValue))
			return false;
		break;

	case E_ANIM_VALUE_UINT:
		if (!kLuaStream.ReadData(strKey.c_str(), m_uiValue))
			return false;
		break;

	case E_ANIM_VALUE_FLOAT:
		if (!kLuaStream.ReadData(strKey.c_str(), m_fValue))
			return false;
		break;

	case E_ANIM_VALUE_NIPOINT2:
		{
			NiPoint2 kValue;
			if (!kLuaStream.ReadData(strKey.c_str(), kValue))
				return false;
			Set(kValue);
		}
		break;

	case E_ANIM_VALUE_NIPOINT3:
		{
			NiPoint3 kValue;
			if (!kLuaStream.ReadData(strKey.c_str(), kValue))
				return false;
			Set(kValue);
		}
		break;

	case E_ANIM_VALUE_NICOLOR:
		{
			NiColor kValue;
			if (!kLuaStream.ReadData(strKey.c_str(), kValue))
				return false;
			Set(kValue);
		}
		break;

	case E_ANIM_VALUE_NIQUATERNION:
		{
			NiQuaternion kValue;
			if (!kLuaStream.ReadData(strKey.c_str(), kValue))
				return false;
			Set(kValue);
		}
		break;

	case E_ANIM_VALUE_STRING:
		kLuaStream.ReadData(strKey.c_str(), *m_pkValueString);
		break;

	default:
		return false;
	};

	return true;
}
//-------------------------------------------------------------------------------------------------
// sdAnimProperty
//-------------------------------------------------------------------------------------------------
sdAnimProperty::sdAnimProperty(const std::string& strName, bool bInHerits, sdAnimValue* pkDefaultValue)
: m_strName(strName)
, m_bInHerits(bInHerits)
, m_pkDefaultValue(pkDefaultValue)
{

}
//-------------------------------------------------------------------------------------------------
sdAnimProperty::~sdAnimProperty()
{
	if (m_pkDefaultValue)
	{
		NiDelete m_pkDefaultValue;
		m_pkDefaultValue = NULL;
	}
}
//-------------------------------------------------------------------------------------------------
bool sdAnimProperty::Save(Engine::Util::sdLuaWriteUtil& kLuaStream, sdAnimValue* pkValue)
{
	if (pkValue)
		return pkValue->Save(kLuaStream, m_strName);
	return true;
}
//-------------------------------------------------------------------------------------------------
sdAnimValue* sdAnimProperty::Load(Engine::Util::sdLuaReadUtil& kLuaStream)
{
	if (!m_pkDefaultValue)
		return NULL;

	sdAnimValue* pkProxy = NiNew sdAnimValue(m_pkDefaultValue);
	if (!pkProxy->Load(kLuaStream, m_strName))
	{
		NiDelete pkProxy;
		pkProxy = NULL;
	}

	return pkProxy;
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_MOVIESYSTEM