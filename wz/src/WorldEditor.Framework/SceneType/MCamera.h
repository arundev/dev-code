//*************************************************************************************************
// 内容:	(参考自WZEditor和FXEditor)
//---------------------------------------------------------
// 作者:		
// 创建:		2014-05-27
// 最后修改:
//*************************************************************************************************
#pragma once

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
//-------------------------------------------------------------------------------------------------
public ref class MFrustum
{
public:
	MFrustum();

	//[System::ComponentModel::RefreshPropertiesAttribute(System::ComponentModel::RefreshProperties::All)]
	//property bool Ortho{ void set(bool value);bool get();}

	//[System::ComponentModel::RefreshPropertiesAttribute(System::ComponentModel::RefreshProperties::All)]
	//property float Left{ void set(float value);float get();}

	//[System::ComponentModel::RefreshPropertiesAttribute(System::ComponentModel::RefreshProperties::All)]
	//property float Right{ void set(float value);float get();}

	//[System::ComponentModel::RefreshPropertiesAttribute(System::ComponentModel::RefreshProperties::All)]
	//property float Top{ void set(float value);float get();}

	//[System::ComponentModel::RefreshPropertiesAttribute(System::ComponentModel::RefreshProperties::All)]
	//property float Bottom{ void set(float value);float get();}

	//[System::ComponentModel::RefreshPropertiesAttribute(System::ComponentModel::RefreshProperties::All)]
	//property float Near{ void set(float value);float get();}

	//[System::ComponentModel::RefreshPropertiesAttribute(System::ComponentModel::RefreshProperties::All)]
	//property float Far{ void set(float value);float get();}

protected:
	bool m_bOrtho;
	float m_fLeft;
	float m_fRight;
	float m_fTop;
	float m_fBottom;
	float m_fNear;
	float m_fFar;
};
//-------------------------------------------------------------------------------------------------
public ref class MCamera
{

};
//-------------------------------------------------------------------------------------------------
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK