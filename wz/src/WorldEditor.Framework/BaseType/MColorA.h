//*************************************************************************************************
// ����:	(�ο���WZEditor.Framework��FXEditor.Framework)
//---------------------------------------------------------
// ����:		
// ����:		2013-05-29
// ����޸�:	2014-05-28
//*************************************************************************************************
#pragma once

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
//-------------------------------------------------------------------------------------------------
[System::SerializableAttribute]
[System::ComponentModel::TypeConverterAttribute(System::ComponentModel::ExpandableObjectConverter::typeid)]
public ref class MColorA
{
public:
	MColorA();
	MColorA(float fR, float fG, float fB, float fA);
	MColorA(MColorA^ rhs);
	MColorA(MColorA% rhs);

	[System::ComponentModel::RefreshPropertiesAttribute(System::ComponentModel::RefreshProperties::All)]
	property float R{ void set(float fR);float get();}

	[System::ComponentModel::RefreshPropertiesAttribute(System::ComponentModel::RefreshProperties::All)]
	property float G{ void set(float fG);float get();}

	[System::ComponentModel::RefreshPropertiesAttribute(System::ComponentModel::RefreshProperties::All)]
	property float B{ void set(float fB);float get();}

	[System::ComponentModel::RefreshPropertiesAttribute(System::ComponentModel::RefreshProperties::All)]
	property float A{ void set(float fA);float get();}

	bool Assign(Object^ obj);

	virtual bool Equals(Object^ obj) override;
	virtual System::String^ ToString() override;

private:
	float m_fR;
	float m_fG;
	float m_fB;
	float m_fA;
};
//-------------------------------------------------------------------------------------------------
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK