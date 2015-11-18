//*****************************************************************************
// 内容:	(参考自WZEditor和FXEditor)
//---------------------------------------------------------
// 作者:		
// 创建:		2013-06-02
// 最后修改:
//*****************************************************************************
#pragma once
#include "MGameObject.h"

class sdEntity;

using namespace System::ComponentModel;

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
//-----------------------------------------------------------------------------
[PropAttrAttribute("TR", L"位置", L"基础", L"表示游戏对象的世界坐标", true, 0)]
[PropAttrAttribute("RT", L"旋转", L"基础", L"表示游戏对象的XYZ旋转角度", true, 0)]
[PropAttrAttribute("SC", L"缩放", L"基础", L"表示游戏对象的缩放比例", true, 0)]
public ref class MEntity : public MGameObject
{
public:
	MEntity();
	MEntity(sdEntity* pkEntity);
	MEntity(MNativePtWrap^ pkNativePointer);

	//
	virtual void Update(float fTime);
	virtual void Render();

protected:

};
//-----------------------------------------------------------------------------
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK