#include "WorldEditor.FrameworkPCH.h"
#include "MPoint2Converter.h"
#include "MPoint2.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Globalization;
using namespace System::Collections;

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
//-------------------------------------------------------------------------------------------------
PropertyDescriptorCollection^ MPoint2Converter::GetProperties(ITypeDescriptorContext^ context, Object^ value, array<Attribute^>^ attributes)
{
	if (context->GetType() == MPoint2::typeid && context->PropertyDescriptor)
	{
		MPoint2^ pkSrcValue = static_cast<MPoint2^>(value);

		ArrayList^ pakPropDesc = gcnew ArrayList();
		pakPropDesc->Add(gcnew MElementDescriptor(pkSrcValue, 0, context->PropertyDescriptor, "x", attributes));
		pakPropDesc->Add(gcnew MElementDescriptor(pkSrcValue, 1, context->PropertyDescriptor, "y", attributes));

		array<PropertyDescriptor^>^ pakPropDescArray = dynamic_cast<array<PropertyDescriptor^>^ >(attributes);
		return gcnew PropertyDescriptorCollection(pakPropDescArray);
	}

	return TypeDescriptor::GetProperties(value, attributes);
}
//-------------------------------------------------------------------------------------------------
bool MPoint2Converter::GetPropertiesSupported(ITypeDescriptorContext^ context)
{
	return true;
}
//-------------------------------------------------------------------------------------------------
bool MPoint2Converter::CanConvertTo(ITypeDescriptorContext^ context, Type^ destinationType)
{
	if (context->GetType() == MPoint2::typeid)
		return true;

	return TypeConverter::CanConvertTo(context, destinationType);
}
//-------------------------------------------------------------------------------------------------
Object^ MPoint2Converter::ConvertTo(ITypeDescriptorContext^ context, CultureInfo^ culture, Object^ value, Type^ destinationType)
{
	if (value->GetType() == MPoint2::typeid && destinationType == String::typeid)
	{
		// build the string as (x,y)
		MPoint2^ pkValue = static_cast<MPoint2^>(value);
		return String::Format("({0},{1})", pkValue->X, pkValue->Y);
	}

	return TypeConverter::ConvertTo(context, culture, value, destinationType);
}
//-------------------------------------------------------------------------------------------------
bool MPoint2Converter::CanConvertFrom(ITypeDescriptorContext^ context, Type^ sourceType)
{
	if (sourceType == String::typeid)
		return true;

	return TypeConverter::CanConvertFrom(context, sourceType);
}
//-------------------------------------------------------------------------------------------------
Object^ MPoint2Converter::ConvertFrom(ITypeDescriptorContext^ context, CultureInfo^ culture, Object^ value)
{
	if (value->GetType() == String::typeid)
	{
		try
		{
			String^ pkSrcValue = static_cast<String^>(value);

			// Parse the format (x,y)
			int iOpenParen = pkSrcValue->IndexOf('(');
			int iComma = pkSrcValue->IndexOf(',', iOpenParen+1);
			int iCloseParen = pkSrcValue->IndexOf(')', iComma+1);
			if (iOpenParen != -1 && iCloseParen != -1 && iComma != -1)
			{
				MPoint2^ pkDstValue = gcnew MPoint2;

				String^ pkX = pkSrcValue->Substring(iOpenParen+1, iComma-(iOpenParen+1));
				pkDstValue->X = Single::Parse(pkX);

				String^ pkY = pkSrcValue->Substring(iComma+1, iCloseParen-(iComma+1));
				pkDstValue->Y = Single::Parse(pkY);

				return pkDstValue;
			}
		}
		catch (...)
		{
			throw gcnew ArgumentException(String::Format("Cannot convert '{0}' to type MPoint2", value));
		}
	}

	return TypeConverter::ConvertFrom(context, culture, value);
}
//-------------------------------------------------------------------------------------------------
MPoint2Converter::MElementDescriptor::MElementDescriptor(Object^ pkValue, uint uiElement, PropertyDescriptor^ pkParentDescriptor, String^ kName, 
	array<System::Attribute^>^ pakAttribute)
: PropertyDescriptor(kName, pakAttribute)
, m_pkValue(pkValue)
, m_uiElement(uiElement)
, m_pkParentDescriptor(pkParentDescriptor)
{
	System::Diagnostics::Debug::Assert(pkValue != nullptr);
	System::Diagnostics::Debug::Assert(uiElement < 3);
	System::Diagnostics::Debug::Assert(pkParentDescriptor != nullptr);
}
//-------------------------------------------------------------------------------------------------
System::Type^ MPoint2Converter::MElementDescriptor::ComponentType::get()
{
	return m_pkValue->GetType();
}
//-------------------------------------------------------------------------------------------------
System::Type^ MPoint2Converter::MElementDescriptor::PropertyType::get()
{
	return Element->GetType();
}
//-------------------------------------------------------------------------------------------------
bool MPoint2Converter::MElementDescriptor::IsReadOnly::get()
{
	if (m_pkParentDescriptor != nullptr)
		return m_pkParentDescriptor->IsReadOnly;
	
	return true;
}
//-------------------------------------------------------------------------------------------------
System::Object^ MPoint2Converter::MElementDescriptor::GetValue(System::Object^ pkComponent)
{
	return Element;
}
//-------------------------------------------------------------------------------------------------
void MPoint2Converter::MElementDescriptor::SetValue(System::Object^ pkComponent, System::Object^ pkValue)
{
	Element = pkValue;

	if (m_pkParentDescriptor != nullptr)
		m_pkParentDescriptor->SetValue(pkComponent, pkValue);
}
//-------------------------------------------------------------------------------------------------
void MPoint2Converter::MElementDescriptor::ResetValue(System::Object^ pkComponent)
{
	if (m_pkParentDescriptor != nullptr)
		m_pkParentDescriptor->ResetValue(pkComponent);
}
//-------------------------------------------------------------------------------------------------
bool MPoint2Converter::MElementDescriptor::CanResetValue(System::Object^ pkComponent)
{
	if (m_pkParentDescriptor != nullptr)
		return m_pkParentDescriptor->CanResetValue(pkComponent);

	return false;
}
//-------------------------------------------------------------------------------------------------
bool MPoint2Converter::MElementDescriptor::ShouldSerializeValue(System::Object^ pkComponent)
{
	if (m_pkParentDescriptor != nullptr)
		return m_pkParentDescriptor->ShouldSerializeValue(pkComponent);

	return false;
}
//-------------------------------------------------------------------------------------------------
System::Object^ MPoint2Converter::MElementDescriptor::Element::get()
{
	if (m_uiElement == 0)
		return static_cast<MPoint2^>(m_pkValue)->X;
	else if (m_uiElement == 1)
		return static_cast<MPoint2^>(m_pkValue)->Y;
	

	return nullptr;
}
//-------------------------------------------------------------------------------------------------
void MPoint2Converter::MElementDescriptor::Element::set(System::Object^ value)
{
	float^ pfValue = static_cast<float^>(value);

	if (m_uiElement == 0)
		return static_cast<MPoint2^>(m_pkValue)->X = *pfValue;
	else if (m_uiElement == 1)
		return static_cast<MPoint2^>(m_pkValue)->Y = *pfValue;
}
//-------------------------------------------------------------------------------------------------
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK