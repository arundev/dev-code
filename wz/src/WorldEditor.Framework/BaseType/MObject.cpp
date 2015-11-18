#include "WorldEditor.FrameworkPCH.h"
#include "MObject.h"

using namespace System;

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
//-----------------------------------------------------------------------------
MObject::MObject() : m_spGBObject(NULL)
{

}
//-----------------------------------------------------------------------------
MObject::MObject(NiObject* spGBObject) : m_spGBObject(spGBObject)
{
	MInitRefObject(m_spGBObject);
}
//-----------------------------------------------------------------------------
MObject::~MObject()
{
	MDisposeRefObject(m_spGBObject);
}
//-----------------------------------------------------------------------------
NiObject* MObject::GetGBObject()
{
	return m_spGBObject;
}
//-----------------------------------------------------------------------------
void MObject::Do_Dispose(bool bDisposing)
{
	MDisposeRefObject(m_spGBObject);
}
//-----------------------------------------------------------------------------
MObjectNET::MObjectNET()
{

}
//-----------------------------------------------------------------------------
MObjectNET::MObjectNET(NiObjectNET* spGBObjectNET) : MObject(spGBObjectNET)
{

}
//-----------------------------------------------------------------------------
MObjectNET::~MObjectNET()
{

}
//-----------------------------------------------------------------------------
NiObjectNET* MObjectNET::GetGBObjectNET()
{
	return (NiObjectNET*)m_spGBObject;
}
//-----------------------------------------------------------------------------
void MObjectNET::Name::set(String^ kName)
{
	
}
//-----------------------------------------------------------------------------
String^ MObjectNET::Name::get()
{
	return nullptr;
}
//-----------------------------------------------------------------------------
MAVObject::MAVObject()
{

}
//-----------------------------------------------------------------------------
MAVObject::MAVObject(NiAVObject* spGBAVObject) : MObjectNET(spGBAVObject)
{

}
//-----------------------------------------------------------------------------
MAVObject::~MAVObject()
{

}
//-----------------------------------------------------------------------------
NiAVObject* MAVObject::GetGBAVObject()
{
	return (NiAVObject*)m_spGBObject;
}
//-----------------------------------------------------------------------------
MNode::MNode()
{

}
//-----------------------------------------------------------------------------
MNode::MNode(NiNode* spGBAVObject) : MAVObject(spGBAVObject)
{

}
//-----------------------------------------------------------------------------
MNode::~MNode()
{

}
//-----------------------------------------------------------------------------
NiNode* MNode::GetGBNode()
{
	return (NiNode*)m_spGBObject;
}
//-----------------------------------------------------------------------------
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK