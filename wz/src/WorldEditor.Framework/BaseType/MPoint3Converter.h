//*************************************************************************************************
// 内容:	(参考自WZEditor.Framework)
//---------------------------------------------------------
// 作者:		
// 创建:		2014-05-27
// 最后修改:	2014-05-28
//*************************************************************************************************
#pragma once

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
ref class MPoint3;
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
//-------------------------------------------------------------------------------------------------
// 用于将字符串转换为MPoint3对象
//-------------------------------------------------------------------------------------------------
public ref class MPoint3Converter : public System::ComponentModel::TypeConverter
{
public:
	// 虚函数重载
	virtual System::ComponentModel::PropertyDescriptorCollection^ GetProperties(
		System::ComponentModel::ITypeDescriptorContext^ context,
		System::Object^ value, array<System::Attribute^>^ attributes) override;
	virtual bool GetPropertiesSupported(System::ComponentModel::ITypeDescriptorContext^ context) override;

	virtual bool CanConvertTo(System::ComponentModel::ITypeDescriptorContext^ context,
		System::Type^ destinationType) override;
	virtual System::Object^ ConvertTo(System::ComponentModel::ITypeDescriptorContext^ context, 
		System::Globalization::CultureInfo^ culture, System::Object^ value, System::Type^ destinationType) override;

	virtual bool CanConvertFrom(System::ComponentModel::ITypeDescriptorContext^ context,
		System::Type^ sourceType) override;
	virtual System::Object^ ConvertFrom(System::ComponentModel::ITypeDescriptorContext^ context, 
		System::Globalization::CultureInfo^ culture, System::Object^ value) override;

private:
	ref class MElementDescriptor : public System::ComponentModel::PropertyDescriptor
	{
	public:
		MElementDescriptor(System::Object^ pkValue, uint uiElement, 
			System::ComponentModel::PropertyDescriptor^ pkParentDescriptor, 
			System::String^ kName, 
			array<System::Attribute^>^ pakAttribute);
		
		// 虚函数重载
		property System::Type^ ComponentType{ virtual System::Type^ get() override;}
		property bool IsReadOnly{ virtual bool get() override;}
		property System::Type^ PropertyType{ virtual System::Type^ get() override;}

		virtual System::Object^ GetValue(System::Object^ pkComponent) override;
		virtual void SetValue(System::Object^ pkComponent, System::Object^ pkValue) override;
		virtual void ResetValue(System::Object^ pkComponent) override;
		virtual bool CanResetValue(System::Object^ pkComponent) override;
		virtual bool ShouldSerializeValue(System::Object^ pkComponent) override;

	private:
		property System::Object^ Element{ System::Object^ get(); void set(System::Object^);}

	private:
		Object^ m_pkValue;							//< 数据
		uint m_uiElement;							//< 分量
		PropertyDescriptor^ m_pkParentDescriptor;	//< 父对象描述符
	};
};
//-------------------------------------------------------------------------------------------------
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK