//*****************************************************************************
// ����:	(�ο���WZEditor��FXEditor)
//---------------------------------------------------------
// ����:		
// ����:		2013-06-02
// ����޸�:
//*****************************************************************************
#pragma once
#include "MGameObject.h"

class sdEntity;

using namespace System::ComponentModel;

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
//-----------------------------------------------------------------------------
[PropAttrAttribute("TR", L"λ��", L"����", L"��ʾ��Ϸ�������������", true, 0)]
[PropAttrAttribute("RT", L"��ת", L"����", L"��ʾ��Ϸ�����XYZ��ת�Ƕ�", true, 0)]
[PropAttrAttribute("SC", L"����", L"����", L"��ʾ��Ϸ��������ű���", true, 0)]
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