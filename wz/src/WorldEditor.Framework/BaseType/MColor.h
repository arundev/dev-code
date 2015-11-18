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
public ref class MColor
{
public:
	MColor();
	MColor(float fR, float fG, float fB);
	MColor(MColor^ rhs);
	MColor(MColor% rhs);

	[System::ComponentModel::RefreshPropertiesAttribute(System::ComponentModel::RefreshProperties::All)]
	property float R{ void set(float fR);float get();}

	[System::ComponentModel::RefreshPropertiesAttribute(System::ComponentModel::RefreshProperties::All)]
	property float G{ void set(float fG);float get();}

	[System::ComponentModel::RefreshPropertiesAttribute(System::ComponentModel::RefreshProperties::All)]
	property float B{ void set(float fB);float get();}

	bool Assign(Object^ obj);

	virtual bool Equals(Object^ obj) override;
	virtual System::String^ ToString() override;

private:
	float m_fR;
	float m_fG;
	float m_fB;
};
//-------------------------------------------------------------------------------------------------
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK