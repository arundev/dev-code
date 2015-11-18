//*************************************************************************************************
// 内容:	(参考自WZEditor和FXEditor)
//---------------------------------------------------------
// 作者:		
// 创建:		2013-06-02
// 最后修改:
//*************************************************************************************************
#pragma once
#include "MGameObject.h"
#include "MLight.h"

class sdMap;

using namespace System::ComponentModel;

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
//-----------------------------------------------------------------------------
public ref class MMap : public MGameObject
{
public:
	MMap();
	MMap(sdMap* pkMap);
	MMap(MNativePtWrap^ pkNativePointer);

	[CategoryAttribute::Category(L"基础")]
	[DisplayNameAttribute::DisplayName(L"名称")]
	property System::String^ Name{ void set(System::String^ val);System::String^ get();}

	[BrowsableAttribute(false)]
	property MLight^ AmbientLight { MLight^ get();}

	[BrowsableAttribute(false)]
	property MLight^ MainLight { MLight^ get();}

	[BrowsableAttribute(false)]
	property MLight^ AssistantLight { MLight^ get();}

	//
	virtual void Update(float fTime);
	virtual void Render();

protected:
	
};
//-----------------------------------------------------------------------------
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK