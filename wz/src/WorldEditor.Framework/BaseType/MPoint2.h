//*************************************************************************************************
// 内容:	(参考自WZEditor.Framework和FXEditor.Framework)
//---------------------------------------------------------
// 作者:		
// 创建:		2013-05-29
// 最后修改:	2014-05-28
//*************************************************************************************************
#pragma once
#include "MPoint2Converter.h"

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
//-------------------------------------------------------------------------------------------------
[System::SerializableAttribute]
[System::ComponentModel::TypeConverterAttribute(MPoint2Converter::typeid)]
public ref class MPoint2
{
public:
	MPoint2();
	MPoint2(float fX, float m_fY);
	MPoint2(const NiPoint2& kPoint2);
	MPoint2(MPoint2^ rhs);
	MPoint2(MPoint2% rhs);

	[System::ComponentModel::RefreshPropertiesAttribute(System::ComponentModel::RefreshProperties::All)]
	property float X{ void set(float fX);float get();}

	[System::ComponentModel::RefreshPropertiesAttribute(System::ComponentModel::RefreshProperties::All)]
	property float Y{ void set(float fY);float get();}

	bool Assign(Object^ obj);

	virtual bool Equals(Object^ obj) override;
	virtual System::String^ ToString() override;

private:
	float m_fX;
	float m_fY;
};
//-------------------------------------------------------------------------------------------------
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK