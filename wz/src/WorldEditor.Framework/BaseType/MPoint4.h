//*************************************************************************************************
// 内容:	(参考自WZEditor.Framework和FXEditor.Framework)
//---------------------------------------------------------
// 作者:		
// 创建:		2013-05-29
// 最后修改:	2014-05-28
//*************************************************************************************************
#pragma once

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
//-------------------------------------------------------------------------------------------------
[System::SerializableAttribute]
[System::ComponentModel::TypeConverterAttribute(System::ComponentModel::ExpandableObjectConverter::typeid)]
public ref class MPoint4
{
public:
	MPoint4();
	MPoint4(float fX, float m_fY, float fZ, float fW);
	MPoint4(MPoint4^ rhs);
	MPoint4(MPoint4% rhs);

	[System::ComponentModel::RefreshPropertiesAttribute(System::ComponentModel::RefreshProperties::All)]
	property float X{ void set(float fX);float get();}

	[System::ComponentModel::RefreshPropertiesAttribute(System::ComponentModel::RefreshProperties::All)]
	property float Y{ void set(float fY);float get();}

	[System::ComponentModel::RefreshPropertiesAttribute(System::ComponentModel::RefreshProperties::All)]
	property float Z{ void set(float fZ);float get();}

	[System::ComponentModel::RefreshPropertiesAttribute(System::ComponentModel::RefreshProperties::All)]
	property float W{ void set(float fW);float get();}

	bool Assign(Object^ obj);

	virtual bool Equals(Object^ obj) override;
	virtual System::String^ ToString() override;

private:
	float m_fX;
	float m_fY;
	float m_fZ;
	float m_fW;
};
//-------------------------------------------------------------------------------------------------
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK