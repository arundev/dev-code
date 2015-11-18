#include "WorldEditor.FrameworkPCH.h"
#include "MPoint3Converter.h"
#include "MPoint3.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Globalization;
using namespace System::Collections;

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
//-------------------------------------------------------------------------------------------------
PropertyDescriptorCollection^ MPoint3Converter::GetProperties(ITypeDescriptorContext^ context, Object^ value, array<Attribute^>^ attributes)
{
	if (context->GetType() == MPoint3::typeid && context->PropertyDescriptor)
	{
		MPoint3^ pkSrcValue = static_cast<MPoint3^>(value);

		ArrayList^ pakPropDesc = gcnew ArrayList();
		pakPropDesc->Add(gcnew MElementDescriptor(pkSrcValue, 0, context->PropertyDescriptor, "x", attributes));
		pakPropDesc->Add(gcnew MElementDescriptor(pkSrcValue, 1, context->PropertyDescriptor, "y", attributes));
		pakPropDesc->Add(gcnew MElementDescriptor(pkSrcValue, 3, context->PropertyDescriptor, "z", attributes));

		array<PropertyDescriptor^>^ pakPropDescArray = dynamic_cast<array<PropertyDescriptor^>^ >(attributes);
		return gcnew PropertyDescriptorCollection(pakPropDescArray);
	}

	return TypeDescriptor::GetProperties(value, attributes);
}
//-------------------------------------------------------------------------------------------------
bool MPoint3Converter::GetPropertiesSupported(ITypeDescriptorContext^ context)
{
	return true;
}
//-------------------------------------------------------------------------------------------------
bool MPoint3Converter::CanConvertTo(ITypeDescriptorContext^ context, Type^ destinationType)
{
	if (context->GetType() == MPoint3::typeid)
		return true;

	return TypeConverter::CanConvertTo(context, destinationType);
}
//-------------------------------------------------------------------------------------------------
Object^ MPoint3Converter::ConvertTo(ITypeDescriptorContext^ context, CultureInfo^ culture, Object^ value, Type^ destinationType)
{
	if (value->GetType() == MPoint3::typeid && destinationType == String::typeid)
	{
		// build the string as (x,y,z)
		MPoint3^ pkValue = static_cast<MPoint3^>(value);
		return String::Format("({0},{1},{2})", pkValue->X, pkValue->Y, pkValue->Z);	
	}

	return TypeConverter::ConvertTo(context, culture, value, destinationType);
}
//-------------------------------------------------------------------------------------------------
bool MPoint3Converter::CanConvertFrom(ITypeDescriptorContext^ context, Type^ sourceType)
{
	if (sourceType == String::typeid)
		return true;

	return TypeConverter::CanConvertFrom(context, sourceType);
}
//-------------------------------------------------------------------------------------------------
Object^ MPoint3Converter::ConvertFrom(ITypeDescriptorContext^ context, CultureInfo^ culture, Object^ value)
{
	if (value->GetType() == String::typeid)
	{
		try
		{
			String^ pkSrcValue = static_cast<String^>(value);

			// Parse the format (x,y,z)
			String^ pkDelim = ",";
			array<Char>^ pkDelimIter = pkDelim->ToCharArray();
			array<String^>^ pkSplit = pkSrcValue->Split(pkDelimIter, 3);

			MPoint3^ pkDstValue = gcnew MPoint3;
			pkDstValue->X = Convert::ToSingle(pkSplit[0]);
			pkDstValue->Y = Convert::ToSingle(pkSplit[1]);
			pkDstValue->Z = Convert::ToSingle(pkSplit[2]);

			return pkDstValue;

		}
		catch (...)
		{
			throw gcnew ArgumentException(String::Format("Cannot convert '{0}' to type MPoint3", value));
		}
	}

	return TypeConverter::ConvertFrom(context, culture, value);
}
//-------------------------------------------------------------------------------------------------
MPoint3Converter::MElementDescriptor::MElementDescriptor(Object^ pkValue, uint uiElement, PropertyDescriptor^ pkParentDescriptor, String^ kName, 
														 array<System::Attribute^>^ pakAttribute)
														 : PropertyDescriptor(kName, pakAttribute)
														 , m_pkValue(pkValue)
														 , m_uiElement(uiElement)
														 , m_pkParentDescriptor(pkParentDescriptor)
{
	System::Diagnostics::Debug::Assert(pkValue != nullptr);
	System::Diagnostics::Debug::Assert(uiElement < 4);
	System::Diagnostics::Debug::Assert(pkParentDescriptor != nullptr);
}
//-------------------------------------------------------------------------------------------------
System::Type^ MPoint3Converter::MElementDescriptor::ComponentType::get()
{
	return m_pkValue->GetType();
}
//-------------------------------------------------------------------------------------------------
System::Type^ MPoint3Converter::MElementDescriptor::PropertyType::get()
{
	return Element->GetType();
}
//-------------------------------------------------------------------------------------------------
bool MPoint3Converter::MElementDescriptor::IsReadOnly::get()
{
	if (m_pkParentDescriptor != nullptr)
		return m_pkParentDescriptor->IsReadOnly;
	else
		return true;
}
//-------------------------------------------------------------------------------------------------
System::Object^ MPoint3Converter::MElementDescriptor::GetValue(System::Object^ pkComponent)
{
	return Element;
}
//-------------------------------------------------------------------------------------------------
void MPoint3Converter::MElementDescriptor::SetValue(System::Object^ pkComponent, System::Object^ pkValue)
{
	Element = pkValue;

	if (m_pkParentDescriptor != nullptr)
		m_pkParentDescriptor->SetValue(pkComponent, pkValue);
}
//-------------------------------------------------------------------------------------------------
void MPoint3Converter::MElementDescriptor::ResetValue(System::Object^ pkComponent)
{
	if (m_pkParentDescriptor != nullptr)
		m_pkParentDescriptor->ResetValue(pkComponent);
}
//-------------------------------------------------------------------------------------------------
bool MPoint3Converter::MElementDescriptor::CanResetValue(System::Object^ pkComponent)
{
	if (m_pkParentDescriptor != nullptr)
		return m_pkParentDescriptor->CanResetValue(pkComponent);

	return false;
}
//-------------------------------------------------------------------------------------------------
bool MPoint3Converter::MElementDescriptor::ShouldSerializeValue(System::Object^ pkComponent)
{
	if (m_pkParentDescriptor != nullptr)
		return m_pkParentDescriptor->ShouldSerializeValue(pkComponent);

	return false;
}
//-------------------------------------------------------------------------------------------------
System::Object^ MPoint3Converter::MElementDescriptor::Element::get()
{
	if (m_uiElement == 0)
		return static_cast<MPoint3^>(m_pkValue)->X;
	else if (m_uiElement == 1)
		return static_cast<MPoint3^>(m_pkValue)->Y;
	else if (m_uiElement == 2)
		return static_cast<MPoint3^>(m_pkValue)->Z;

	return nullptr;
}
//-------------------------------------------------------------------------------------------------
void MPoint3Converter::MElementDescriptor::Element::set(System::Object^ value)
{
	float^ pfValue = static_cast<float^>(value);

	if (m_uiElement == 0)
		return static_cast<MPoint3^>(m_pkValue)->X = *pfValue;
	else if (m_uiElement == 1)
		return static_cast<MPoint3^>(m_pkValue)->Y = *pfValue;
	else if (m_uiElement == 2)
		return static_cast<MPoint3^>(m_pkValue)->Z = *pfValue;
}
//-------------------------------------------------------------------------------------------------
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK