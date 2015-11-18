//*************************************************************************************************
// ����:	
//---------------------------------------------------------
// ����:		
// ����:		2013-06-02
// ����޸�:
//*************************************************************************************************
#pragma once
#include "MEntity.h"
#include "MColor.h"

class sdLight;

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
//-------------------------------------------------------------------------------------------------
//[PropAttrAttribute("LightType", L"�ƹ�����", L"�ƹ�", L"��ʾ�ƹ���������", true, 0)]
//[PropAttrAttribute("AmbientColor", L"��������", L"�ƹ�", L"��������", true, 0)]
//[PropAttrAttribute("DiffuseColor", L"�������", L"�ƹ�", L"�������", true, 0)]
//[PropAttrAttribute("SpecularColor", L"�߹����", L"�ƹ�", L"�߹����", true, 0)]
//[PropAttrAttribute("Dimmer", L"����", L"�ƹ�", L"����", true, 0)]
public ref class MLight : public MEntity
{
public:
	MLight();
	MLight(sdLight* pkLight);
	MLight(MNativePtWrap^ pkNativePointer);

	[System::ComponentModel::CategoryAttribute::Category(L"�ƹ�")]
	[System::ComponentModel::DisplayNameAttribute::DisplayName(L"��������")]
	property MColor^ AmbientColor{ void set(MColor^ val);MColor^ get();}

	[System::ComponentModel::CategoryAttribute::Category(L"�ƹ�")]
	[System::ComponentModel::DisplayNameAttribute::DisplayName(L"�������")]
	property MColor^ DiffuseColor{ void set(MColor^ val);MColor^ get();}

	[System::ComponentModel::CategoryAttribute::Category(L"�ƹ�")]
	[System::ComponentModel::DisplayNameAttribute::DisplayName(L"�߹����")]
	property MColor^ SpecularColor{ void set(MColor^ val);MColor^ get();}

	[System::ComponentModel::CategoryAttribute::Category(L"�ƹ�")]
	[System::ComponentModel::DisplayNameAttribute::DisplayName(L"ǿ��")]
	property float Dimmer{ void set(float val);float get();}

	//
	virtual void Update(float fTime) override;
	virtual void Render() override;

protected:

};
//-------------------------------------------------------------------------------------------------
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK