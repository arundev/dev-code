//*************************************************************************************************
// 内容:	(参考自WZEditor.Framework和FXEditor.Framework)
//---------------------------------------------------------
// 作者:		
// 创建:		2013-05-30
// 最后修改:
//*************************************************************************************************
#pragma once
#include "../MDisposable.h"

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
//-----------------------------------------------------------------------------
public ref class MObject : public MDisposable
{
public:
	MObject(NiObject* spGBObject);
	~MObject();

	NiObject* GetGBObject();

protected:
	MObject();

	// 虚函数继承
	virtual void Do_Dispose(bool bDisposing) override; 

protected:
	NiObject* m_spGBObject;
};
//-----------------------------------------------------------------------------
public ref class MObjectNET : public MObject
{
public:
	MObjectNET(NiObjectNET* spGBObjectNET);
	~MObjectNET();

	NiObjectNET* GetGBObjectNET();

	property System::String^ Name{ void set(System::String^ kName);System::String^ get();}

protected:
	MObjectNET();
};
//-----------------------------------------------------------------------------
public ref class MAVObject : public MObjectNET
{
public:
	MAVObject(NiAVObject* spGBAVObject);
	~MAVObject();

	NiAVObject* GetGBAVObject();

	//property MPoint3^ Translate{ void set(MPoint3^ val);MPoint3 get();}
	//property MMatrix3^ Translate{ void set(MMatrix3^ val);MMatrix3 get();}
	//property float Scale{ void set(float val);float get();}

protected:
	MAVObject();
};
//-----------------------------------------------------------------------------
public ref class MNode : public MAVObject
{
public:
	MNode(NiNode* spGBNode);
	~MNode();

	NiNode* GetGBNode();

protected:
	MNode();
};
//-----------------------------------------------------------------------------
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK