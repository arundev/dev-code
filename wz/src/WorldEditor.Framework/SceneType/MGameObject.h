//*****************************************************************************
// 内容:	
//---------------------------------------------------------
// 作者:		
// 创建:		2013-05-31
// 最后修改:
//*****************************************************************************
#pragma once
#include "../MDisposable.h"

class sdGameObject;

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
//-----------------------------------------------------------------------------
// 编辑器可编辑游戏对象的对象反射系统简介：
//	具体知识涉及C#的PropertyGrid，PropertyDescriptor，ICustomTypeDescriptor接口
//	详细信息请查阅MSDN。编辑器游戏对象属性编辑的核心构件是PropertyGrid组件，其
//	实现了对象属性的显示与修改功能。
//	PropertyGrid组件能显示集成并实现了ICustomTypeDescriptor接口的对象属性结构。
//	PropertyGrid使用GameObject的ICustomTypeDescriptor::GetProperties()接口获得
//	GameObject所有属性的描述,这些描述实际上是一个PropertyDescriptor对象的集合,
//	PropertyDescriptor又包含了一切关于类型属性特征的描述。PropertyDescriptor中
//	最重要的两个方法是:
//		Object* GetValue(Object* SelectedObject)
//		void SetValue(Object* SelectedObject, Object* pmValue)
//	他们用来处理对象属性值的读取和写入。
//
// 具体实现思路：
//	MGameObject是所有编辑器代理类的基类,内含sdGameObject对象指针,用来处理C++对象
//	与C#的PropertyGrid之间的数据交互。具体的交互方式是根据C++对象的属性定义，组
//	织所有的属性，并添加属性的编辑约束条件，并根据这些新生成PropertyDescriptor
//	对象，从而实现PropertyGrid显示C++游戏对象的属性的功能。
//
// 代理类：
//	所有需要在C#或PropertyGrid中操作的C++对象，均需要编写一个CLI的托管代理类
//	(因为C#无法处理不带metadata的非托管数据)，所有编辑器上层所需功能，都在代理类
//	中转接或实现。
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
[PropAttrAttribute("TP", L"类型", L"基础", L"表示游戏对象的类型", true, 0)]
[PropAttrAttribute("CD", L"哈希", L"基础", L"表示游戏对象的唯一哈希值", true, 0)]
[PropAttrAttribute("NM", L"名称", L"基础", L"表示游戏对象的名称", true, 0)]
//[TypeConverterAttribute()]
public ref class MGameObject : public MDisposable, public System::ComponentModel::ICustomTypeDescriptor
{
public:
	MGameObject();
	MGameObject(sdGameObject* pkGameObject);
	MGameObject(MNativePtWrap^ pkNativePointer);

	// 重载自ICustomTypeDescriptor
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

	// 属性接口
	// @{
	Object^ GetPropertyData(System::String^ kPropName);
	void SetPropertyData(System::String^ kPropName, Object^ kData, bool bUndoable);
	// @}

protected:

protected:
	sdGameObject* m_pkGameObject;	///< C++对象
	System::Drawing::Design::UITypeEditor^ m_kColorEditor;	///< 颜色属性的编辑器
};
//-----------------------------------------------------------------------------
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK