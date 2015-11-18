//*****************************************************************************
// ����:	
//---------------------------------------------------------
// ����:		
// ����:		2013-05-31
// ����޸�:
//*****************************************************************************
#pragma once
#include "../MDisposable.h"

class sdGameObject;

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
//-----------------------------------------------------------------------------
// �༭���ɱ༭��Ϸ����Ķ�����ϵͳ��飺
//	����֪ʶ�漰C#��PropertyGrid��PropertyDescriptor��ICustomTypeDescriptor�ӿ�
//	��ϸ��Ϣ�����MSDN���༭����Ϸ�������Ա༭�ĺ��Ĺ�����PropertyGrid�������
//	ʵ���˶������Ե���ʾ���޸Ĺ��ܡ�
//	PropertyGrid�������ʾ���ɲ�ʵ����ICustomTypeDescriptor�ӿڵĶ������Խṹ��
//	PropertyGridʹ��GameObject��ICustomTypeDescriptor::GetProperties()�ӿڻ��
//	GameObject�������Ե�����,��Щ����ʵ������һ��PropertyDescriptor����ļ���,
//	PropertyDescriptor�ְ�����һ�й�����������������������PropertyDescriptor��
//	����Ҫ������������:
//		Object* GetValue(Object* SelectedObject)
//		void SetValue(Object* SelectedObject, Object* pmValue)
//	�������������������ֵ�Ķ�ȡ��д�롣
//
// ����ʵ��˼·��
//	MGameObject�����б༭��������Ļ���,�ں�sdGameObject����ָ��,��������C++����
//	��C#��PropertyGrid֮������ݽ���������Ľ�����ʽ�Ǹ���C++��������Զ��壬��
//	֯���е����ԣ���������Եı༭Լ����������������Щ������PropertyDescriptor
//	���󣬴Ӷ�ʵ��PropertyGrid��ʾC++��Ϸ��������ԵĹ��ܡ�
//
// �����ࣺ
//	������Ҫ��C#��PropertyGrid�в�����C++���󣬾���Ҫ��дһ��CLI���йܴ�����
//	(��ΪC#�޷�������metadata�ķ��й�����)�����б༭���ϲ����蹦�ܣ����ڴ�����
//	��ת�ӻ�ʵ�֡�
//-----------------------------------------------------------------------------
[System::AttributeUsageAttribute(System::AttributeTargets::Class, Inherited = true, AllowMultiple = true)]
public ref class PropAttrAttribute : public System::Attribute
{
public:
	System::String^ m_kName;
	System::String^ m_kDisplayName;
	System::String^ m_kCategory;
	System::String^ m_kDesc;
	bool			m_bVisiable;
	int				m_iPriv;

public:
	PropAttrAttribute(System::String^ kName, System::String^ kDisplayName, System::String^ kCategory, System::String^ kDesc, bool bVisiable, int iPriv)
	{
		m_kName = kName;
		m_kDisplayName = kDisplayName;
		m_kCategory = kCategory;
		m_kDesc = kDesc;
		m_bVisiable = bVisiable;
		m_iPriv = iPriv;
	}
};
//-----------------------------------------------------------------------------
[PropAttrAttribute("TP", L"����", L"����", L"��ʾ��Ϸ���������", true, 0)]
[PropAttrAttribute("CD", L"��ϣ", L"����", L"��ʾ��Ϸ�����Ψһ��ϣֵ", true, 0)]
[PropAttrAttribute("NM", L"����", L"����", L"��ʾ��Ϸ���������", true, 0)]
//[TypeConverterAttribute()]
public ref class MGameObject : public MDisposable, public System::ComponentModel::ICustomTypeDescriptor
{
public:
	MGameObject();
	MGameObject(sdGameObject* pkGameObject);
	MGameObject(MNativePtWrap^ pkNativePointer);

	// ������ICustomTypeDescriptor
	// @{
	virtual System::ComponentModel::AttributeCollection^ GetAttributes();
	virtual System::String^ GetClassName();
	virtual System::String^ GetComponentName();
	virtual System::ComponentModel::TypeConverter^ GetConverter();
	virtual System::ComponentModel::EventDescriptor^ GetDefaultEvent();
	virtual System::ComponentModel::PropertyDescriptor^ GetDefaultProperty();
	virtual Object^ GetEditor(System::Type^ pkEditorBaseType);
	virtual System::ComponentModel::EventDescriptorCollection^ GetEvents();
	virtual System::ComponentModel::EventDescriptorCollection^ GetEvents(array<System::Attribute^>^ akAttributes);
	virtual System::ComponentModel::PropertyDescriptorCollection^ GetProperties();
	virtual System::ComponentModel::PropertyDescriptorCollection^ GetProperties(array<System::Attribute^>^ akAttributes);
	virtual Object^	GetPropertyOwner(System::ComponentModel::PropertyDescriptor^ pkPropertyDescriptor);
	// @}

	// ���Խӿ�
	// @{
	Object^ GetPropertyData(System::String^ kPropName);
	void SetPropertyData(System::String^ kPropName, Object^ kData, bool bUndoable);
	// @}

protected:

protected:
	sdGameObject* m_pkGameObject;	///< C++����
	System::Drawing::Design::UITypeEditor^ m_kColorEditor;	///< ��ɫ���Եı༭��
};
//-----------------------------------------------------------------------------
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK