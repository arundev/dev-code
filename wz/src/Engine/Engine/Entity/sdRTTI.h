//*************************************************************************************************
// ����:	RTTI & Dynamic Creation(�ο���WZ::Client::Engine::sdGameObject)
//---------------------------------------------------------
// ����:		
// ����:		2012-11-27
// ����޸�:	2014-03-14
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_SCENE_RTTI_H__
#define _SD_ENGINE_SCENE_RTTI_H__

class sdGameObject;
//-------------------------------------------------------------------------------------------------
struct TypeClass
{
	std::string		className;				///< ����
	uint			objectSize;				///< ������ڴ�ߴ�
	sdGameObject*	(*pfnCreateObject)();	///< �ഴ������
	TypeClass*		baseClass;				///< �����������ָ��
	TypeClass*		nextClass;				///< ���������ָ��
	
	static TypeClass* firstClass;			///< �������ָ��
};

//-------------------------------------------------------------------------------------------------
#define DECLARE_DYNAMIC(theClass)					\
	public:											\
		static TypeClass ms_##theClass##RTTI;		\
		virtual	TypeClass* GetTypeClass() const;	\
		static sdGameObject* CreateGameObject();
//-------------------------------------------------------------------------------------------------
struct _LINK_DYNAMIC
{
	_LINK_DYNAMIC(TypeClass* newClassType)
	{
		newClassType->nextClass = TypeClass::firstClass;
		TypeClass::firstClass = newClassType;
	}
};
//-------------------------------------------------------------------------------------------------
#define IMPL_DYNAMIC(theClass, baseClass)													\
	TypeClass theClass::ms_##theClass##RTTI = { 											\
		#theClass, sizeof(theClass), theClass::CreateGameObject, 							\
		&baseClass::ms_##baseClass##RTTI, NULL};											\
		static _LINK_DYNAMIC ms_##theClass##Link(&theClass::ms_##theClass##RTTI);			\
	TypeClass* theClass::GetTypeClass() const { return &theClass::ms_##theClass##RTTI;}		\
	sdGameObject* theClass::CreateGameObject() { return NiNew theClass;}	
//-------------------------------------------------------------------------------------------------
#endif