//*************************************************************************************************
// ����:	�����ڵ�����
//---------------------------------------------------------
// ����:		
// ����:		2012-12-28
// ����޸�:	2012-01-09
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_MOVIE_SYSTEM_ANIMPROP_H__
#define _SD_ENGINE_MOVIE_SYSTEM_ANIMPROP_H__

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_UTIL
class sdLuaReadUtil;
class sdLuaWriteUtil;
ENGINE_NAMESPACE_END_UTIL
ENGINE_NAMESPACE_END_ENGINE

ENGINE_NAMESPACE_BEGIN_MOVIESYSTEM

//-------------------------------------------------------------------------------------------------
// sdAnimValue
//-------------------------------------------------------------------------------------------------
// �ڵ�����ֵ,���ڽű��ȷ�������
//	\TODO ����ʹ��ר�ŵĻ����������ڴ���Ƭ
class sdAnimValue : public NiMemObject
{
public:
	sdAnimValue(const bool& bValue);
	sdAnimValue(const int& iValue);
	sdAnimValue(const uint& uiValue);
	sdAnimValue(const float& fValue);
	sdAnimValue(const NiPoint2& kValue);
	sdAnimValue(const NiPoint3& kValue);
	sdAnimValue(const NiQuaternion& kValue);
	sdAnimValue(const NiColor& kColor);
	sdAnimValue(const std::string& strValue);
	sdAnimValue(const char* szValue);
	sdAnimValue(const sdAnimValue* pkAnimValue);
	sdAnimValue(const sdAnimValue& kAnimValue);
	~sdAnimValue();

	inline void Set(const bool& bValue);
	inline void Set(const int& iValue);
	inline void Set(const uint& uiValue);
	inline void Set(const float& fValue);
	inline void Set(const NiPoint2& kValue);
	inline void Set(const NiPoint3& kValue);
	inline void Set(const NiQuaternion& kValue);
	inline void Set(const NiColor& kValue);
	inline void Set(const std::string& strValue);
	inline void Set(const char* szValue);

	inline void Get(bool& bValue) const;
	inline void Get(int& iValue) const;
	inline void Get(uint& uiValue) const;
	inline void Get(float& fValue) const;
	inline void Get(NiPoint2& kValue) const;
	inline void Get(NiPoint3& kValue) const;
	inline void Get(NiQuaternion& kValue) const;
	inline void Get(NiColor& kColor) const;
	inline void Get(std::string& strValue) const;

	// ���ַ�������ת��
	bool FromString(const std::string& strValue);
	std::string ToString() const;

	// ��������
	inline void SetIsTrackValue(bool bTrackValue);
	inline bool GetIsTrackValue() const;

	// ��������(��ָ����ֵ��ȡ��д��ָ����ֵ)
	bool Save(Engine::Util::sdLuaWriteUtil& kLuaStream, const std::string& strKey);
	bool Load(Engine::Util::sdLuaReadUtil& kLuaStream, const std::string& strKey);

protected:
	// ��������
	enum EValueType
	{
		E_ANIM_VALUE_BOOL,
		E_ANIM_VALUE_INT,
		E_ANIM_VALUE_UINT,
		E_ANIM_VALUE_FLOAT,
		E_ANIM_VALUE_NIPOINT2,
		E_ANIM_VALUE_NIPOINT3,
		E_ANIM_VALUE_NIQUATERNION,
		E_ANIM_VALUE_NICOLOR,
		E_ANIM_VALUE_STRING,
	};

	// ��ֵ(\TODO �д�����)
	union 
	{
		bool			m_bValue;
		int				m_iValue;
		uint			m_uiValue;
		float			m_fValue;
		float			m_kValuePt2[2];
		float			m_kValuePt3[3];
		float			m_kValueColor[3];
		float			m_kValueQuaternion[4];
		std::string*	m_pkValueString;
	};

	// ��ֵ����
	EValueType m_eType;

	// ����Ƿ��ֶ����õ�ֵ
	bool m_bTrackBalue;
};

//-------------------------------------------------------------------------------------------------
// sdAnimProperty
//-------------------------------------------------------------------------------------------------
// �ڵ�����
class sdAnimProperty : public NiMemObject
{
public:
	sdAnimProperty(const std::string& strName, bool bInHerits, sdAnimValue* pkDefaultValue);
	~sdAnimProperty();

	// ���Է���
	inline const std::string&	GetName() const;
	inline const bool			GetHerits() const;
	inline const sdAnimValue*	GetDefaultValue() const;

	// ��������(��ָ����ֵ��ȡ��д��ָ����ֵ)
	bool Save(Engine::Util::sdLuaWriteUtil& kLuaStream, sdAnimValue* pkValue);
	sdAnimValue* Load(Engine::Util::sdLuaReadUtil& kLuaStream);

protected:
	std::string		m_strName;
	bool			m_bInHerits;
	sdAnimValue*	m_pkDefaultValue;
};

#include "sdAnimProperty.inl"

ENGINE_NAMESPACE_END_MOVIESYSTEM
#endif