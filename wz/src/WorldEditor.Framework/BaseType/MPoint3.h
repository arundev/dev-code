//*************************************************************************************************
// 内容:	(参考自WZEditor.Framework和FXEditor.Framework)
//---------------------------------------------------------
// 作者:		
// 创建:		2013-05-29
// 最后修改:	2014-05-28
//*************************************************************************************************
#pragma once
#include "MPoint3Converter.h"

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
//-------------------------------------------------------------------------------------------------
[System::SerializableAttribute]
[System::ComponentModel::TypeConverterAttribute(MPoint3Converter::typeid)]
public ref class MPoint3
{
public:
	MPoint3();
	MPoint3(float fX, float m_fY, float fZ);
	MPoint3(const NiPoint3& kPoint3);
	MPoint3(MPoint3^ rhs);
	MPoint3(MPoint3% rhs);

	[System::ComponentModel::RefreshPropertiesAttribute(System::ComponentModel::RefreshProperties::All)]
	property float X{ void set(float fX);float get();}

	[System::ComponentModel::RefreshPropertiesAttribute(System::ComponentModel::RefreshProperties::All)]
	property float Y{ void set(float fY);float get();}

	[System::ComponentModel::RefreshPropertiesAttribute(System::ComponentModel::RefreshProperties::All)]
	property float Z{ void set(float fZ);float get();}

	bool Assign(Object^ obj);

	virtual bool Equals(Object^ obj) override;
	virtual System::String^ ToString() override;

private:
	float m_fX;
	float m_fY;
	float m_fZ;
};
//-------------------------------------------------------------------------------------------------
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK