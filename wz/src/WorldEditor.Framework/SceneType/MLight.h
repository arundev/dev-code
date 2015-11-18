//*************************************************************************************************
// 内容:	
//---------------------------------------------------------
// 作者:		
// 创建:		2013-06-02
// 最后修改:
//*************************************************************************************************
#pragma once
#include "MEntity.h"
#include "MColor.h"

class sdLight;

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
//-------------------------------------------------------------------------------------------------
//[PropAttrAttribute("LightType", L"灯光类型", L"灯光", L"表示灯光对象的类型", true, 0)]
//[PropAttrAttribute("AmbientColor", L"环境分量", L"灯光", L"环境分量", true, 0)]
//[PropAttrAttribute("DiffuseColor", L"漫射分量", L"灯光", L"漫射分量", true, 0)]
//[PropAttrAttribute("SpecularColor", L"高光分量", L"灯光", L"高光分量", true, 0)]
//[PropAttrAttribute("Dimmer", L"亮度", L"灯光", L"亮度", true, 0)]
public ref class MLight : public MEntity
{
public:
	MLight();
	MLight(sdLight* pkLight);
	MLight(MNativePtWrap^ pkNativePointer);

	[System::ComponentModel::CategoryAttribute::Category(L"灯光")]
	[System::ComponentModel::DisplayNameAttribute::DisplayName(L"环境分量")]
	property MColor^ AmbientColor{ void set(MColor^ val);MColor^ get();}

	[System::ComponentModel::CategoryAttribute::Category(L"灯光")]
	[System::ComponentModel::DisplayNameAttribute::DisplayName(L"漫射分量")]
	property MColor^ DiffuseColor{ void set(MColor^ val);MColor^ get();}

	[System::ComponentModel::CategoryAttribute::Category(L"灯光")]
	[System::ComponentModel::DisplayNameAttribute::DisplayName(L"高光分量")]
	property MColor^ SpecularColor{ void set(MColor^ val);MColor^ get();}

	[System::ComponentModel::CategoryAttribute::Category(L"灯光")]
	[System::ComponentModel::DisplayNameAttribute::DisplayName(L"强度")]
	property float Dimmer{ void set(float val);float get();}

	//
	virtual void Update(float fTime) override;
	virtual void Render() override;

protected:

};
//-------------------------------------------------------------------------------------------------
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK