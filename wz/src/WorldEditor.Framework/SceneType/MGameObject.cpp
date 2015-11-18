#include "WorldEditor.FrameworkPCH.h"
#include "MGameObject.h"
#include "../BaseType/MPoint2.h"
#include "../BaseType/MPoint3.h"
#include "../BaseType/MColor.h"

#include <sdGameObject.h>

using namespace System;
using namespace System::Collections;
using namespace System::ComponentModel;
using namespace System::Drawing::Design;

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
//-----------------------------------------------------------------------------
MGameObject::MGameObject()
: m_pkGameObject(NULL)
, m_kColorEditor(nullptr)
{

}
//-----------------------------------------------------------------------------
MGameObject::MGameObject(sdGameObject* pkGameObject)
: m_pkGameObject(pkGameObject)
, m_kColorEditor(nullptr)
{

}
//-----------------------------------------------------------------------------
MGameObject::MGameObject(MNativePtWrap^ pkNativePointer)
: m_pkGameObject((sdGameObject*)pkNativePointer->GetPointer())
, m_kColorEditor(nullptr)
{

}
//-----------------------------------------------------------------------------
AttributeCollection^ MGameObject::GetAttributes()
{
	return TypeDescriptor::GetAttributes(this, true);
}
//-----------------------------------------------------------------------------
System::String^	MGameObject::GetClassName()
{
	return TypeDescriptor::GetClassName(this, true);
}
//-----------------------------------------------------------------------------
System::String^ MGameObject::GetComponentName()
{
	return TypeDescriptor::GetComponentName(this, true);
}
//-----------------------------------------------------------------------------
TypeConverter^ MGameObject::GetConverter()
{
	return TypeDescriptor::GetConverter(this, true);
}
//-----------------------------------------------------------------------------
EventDescriptor^ MGameObject::GetDefaultEvent()
{
	return TypeDescriptor::GetDefaultEvent(this, true);
}
//-----------------------------------------------------------------------------
PropertyDescriptor^ MGameObject::GetDefaultProperty()
{
	return nullptr;
}
//-----------------------------------------------------------------------------
Object^ MGameObject::GetEditor(System::Type^ pkEditorBaseType)
{
	return TypeDescriptor::GetEditor(this, pkEditorBaseType, true);
}
//-----------------------------------------------------------------------------
EventDescriptorCollection^ MGameObject::GetEvents()
{
	return TypeDescriptor::GetEvents(this, true);
}
//-----------------------------------------------------------------------------
EventDescriptorCollection^ MGameObject::GetEvents(array<System::Attribute^>^ akAttributes)
{
	return TypeDescriptor::GetEvents(this, akAttributes, true);
}
//-----------------------------------------------------------------------------
PropertyDescriptorCollection^ MGameObject::GetProperties()
{
	return TypeDescriptor::GetProperties(this, true);
}
//-----------------------------------------------------------------------------
PropertyDescriptorCollection^ MGameObject::GetProperties(array<System::Attribute^>^ akAttributes)
{
	// 非常重要的重载函数,决定PropertyGrid如何显示C++对象

	// CachePropDescs();

	// 取得对象的所有属性
	PropertyDescriptorCollection^ kBasePropDesc = TypeDescriptor::GetProperties(this, akAttributes, true);

	// 遍历CLI注册属性(在托管层附加的属性优先级最高)
	ArrayList^ kPropDesc = gcnew ArrayList();
	IEnumerator^ kBasePropDescEnumerator = kBasePropDesc->GetEnumerator();
	while (kBasePropDescEnumerator->MoveNext())
	{
		PropertyDescriptor^ kDesc = static_cast<PropertyDescriptor^>(kBasePropDescEnumerator->Current);
		if (kDesc->IsBrowsable)
			kPropDesc->Add(kBasePropDescEnumerator->Current);
	}

	//
	if (m_kColorEditor == nullptr)
	{

	}

	// 遍历C++对象注册属性
	sdGameObject* pkGameObject = m_pkGameObject;
	if (pkGameObject)
	{
		std::vector<PropFuncEntry> kProperties;
		pkGameObject->GetPropertyEntries(kProperties);

		std::vector<PropFuncEntry>::iterator itr = kProperties.begin();
		std::vector<PropFuncEntry>::iterator itr_end = kProperties.end();
		for (; itr != itr_end; ++itr)
		{
			const PropFuncEntry& kPropFuncEntry = *itr;

			if ((kPropFuncEntry.flag & PROP_ATTR_READ) == 0)
				continue;

			bool bReadOnly = false;
			if ((kPropFuncEntry.flag & PROP_ATTR_WRITE) == 0)
				bReadOnly = true;

			ArrayList^ kPropAttributes = gcnew ArrayList();
			UITypeEditor^ kEditor = nullptr;
			//MPoint3Converter^ kConverter = nullptr;
			
			//String^ kPropName = MStringToCharPointer(kPropFuncEntry->func_name.c_str());
			//PropAttrAttribute
		}
	}

	array<PropertyDescriptor^>^ akPropertyDescArray = dynamic_cast<array<PropertyDescriptor^>^>(kPropDesc->ToArray());
	return gcnew PropertyDescriptorCollection(akPropertyDescArray);
}
//-----------------------------------------------------------------------------
Object^ MGameObject::GetPropertyOwner(PropertyDescriptor^ pkPropertyDescriptor)
{
	return this;
}
//-----------------------------------------------------------------------------
Object^ MGameObject::GetPropertyData(String^ kPropName)
{
	sdGameObject* pkGameObject = m_pkGameObject;
	if (pkGameObject == NULL)
		return nullptr;

	const char* szPropName = MStringToCharPointer(kPropName);
	uchar ucType = pkGameObject->GetPropTypeByName(szPropName);
	switch(ucType)
	{
	case PROP_BOOL:
		{
			bool val = false;
			if (pkGameObject->GetPropValueByName(szPropName, val))
				return val;
		}
		break;
	case PROP_CHAR:
		{
			char val = 0;
			if (pkGameObject->GetPropValueByName(szPropName, val))
				return val;
		}
		break;
	case PROP_UCHAR:
		{
			uchar val = 0;
			if (pkGameObject->GetPropValueByName(szPropName, val))
				return val;
		}
		break;
	case PROP_INT:
		{
			int val = 0;
			if (pkGameObject->GetPropValueByName(szPropName, val))
				return val;
		}
		break;
	case PROP_UINT:
		{
			uint val = 0;
			if (pkGameObject->GetPropValueByName(szPropName, val))
				return val;
		}
		break;
	case PROP_FLOAT:
		{
			float val = 0.f;
			if (pkGameObject->GetPropValueByName(szPropName, val))
				return val;
		}
		break;
	case PROP_NIPT2:
		{
			NiPoint2 val = NiPoint2::ZERO;
			if (pkGameObject->GetPropValueByName(szPropName, val))
				return gcnew MPoint2(val);
		}
		break;
	case PROP_NIPT3:
		{
			NiPoint3 val = NiPoint3::ZERO;
			if (pkGameObject->GetPropValueByName(szPropName, val))
				return gcnew MPoint3(val);
		}
		break;
	case PROP_NICOLOR:
		{
			NiColor val = NiColor::BLACK;
			if (pkGameObject->GetPropValueByName(szPropName, val))
				return gcnew MColor(val.r, val.g, val.b);
		}
		break;
	case PROP_NISTRING:
		{
			NiFixedString val;
			if (pkGameObject->GetPropValueByName(szPropName, val))
				return gcnew String((const char*)val);
		}
		break;
	case PROP_SDOBJECT:
		{
			sdGameObject* val = NULL;
			if (pkGameObject->GetPropValueByName(szPropName, val))
				return gcnew MGameObject(val);
		}
		break;
	}

	return nullptr;
}
//-----------------------------------------------------------------------------
void MGameObject::SetPropertyData(System::String^ kPropName, Object^ kData, bool bUndoable)
{
	sdGameObject* pkGameObject = m_pkGameObject;
	if (pkGameObject == NULL)
		return;

	const char* szPropName = MStringToCharPointer(kPropName);
	uchar ucType = pkGameObject->GetPropTypeByName(szPropName);
//	switch(ucType)
//	{
//	case PROP_BOOL:
//		{
//			bool* pValue = dynamic_cast<bool*>(kData);
//			bool val = *pValue;
//			pkGameObject->SetPropValueByName(szPropName, val);
//		}
//		break;
//1	case PROP_CHAR:
//		{
//			char val = 0;
//			if (pkGameObject->GetPropValueByName(szPropName, val))
//				return val;
//		}
//		break;
//	case PROP_UCHAR:
//		{
//			uchar val = 0;
//			if (pkGameObject->GetPropValueByName(szPropName, val))
//				return val;
//		}
//		break;
//	case PROP_INT:
//		{
//			int val = 0;
//			if (pkGameObject->GetPropValueByName(szPropName, val))
//				return val;
//		}
//		break;
//	case PROP_UINT:
//		{
//			uint val = 0;
//			if (pkGameObject->GetPropValueByName(szPropName, val))
//				return val;
//		}
//		break;
//	case PROP_FLOAT:
//		{
//			float val = 0.f;
//			if (pkGameObject->GetPropValueByName(szPropName, val))
//				return val;
//		}
//		break;
//	case PROP_NIPT2:
//		{
//			NiPoint2 val = NiPoint2::ZERO;
//			if (pkGameObject->GetPropValueByName(szPropName, val))
//				return gcnew MPoint2(val);
//		}
//		break;
//	case PROP_NIPT3:
//		{
//			NiPoint3 val = NiPoint3::ZERO;
//			if (pkGameObject->GetPropValueByName(szPropName, val))
//				return gcnew MPoint3(val);
//		}
//		break;
//	case PROP_NICOLOR:
//		{
//			NiColor val = NiColor::BLACK;
//			if (pkGameObject->GetPropValueByName(szPropName, val))
//				return gcnew MColor(val);
//		}
//		break;
//	case PROP_NISTRING:
//		{
//			NiFixedString val;
//			if (pkGameObject->GetPropValueByName(szPropName, val))
//				return gcnew String((const char*)val);
//		}
//		break;
//	case PROP_SDGAMEOBJECT:
//		{
//			sdGameObject* val = NULL;
//			if (pkGameObject->GetPropValueByName(szPropName, val))
//				return gcnew MGameObject(val);
//		}
//		break;
//	}
}
//-----------------------------------------------------------------------------
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK