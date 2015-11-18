#include "sdEnginePCH.h"
#include "sdRenderHelper.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
void sdRenderHelper::DisableZWrite(NiAVObject* pkAVObject)
{
	NIASSERT(pkAVObject);

	NiProperty* pkProperty = pkAVObject->GetProperty(NiZBufferProperty::GetType());
	if (pkProperty)
	{
		NiZBufferProperty* pkZBufferProperty = (NiZBufferProperty*)(pkProperty);
		NIASSERT(pkZBufferProperty);

		pkZBufferProperty->SetZBufferTest(true);
		pkZBufferProperty->SetZBufferWrite(false);
	}

	if (pkAVObject->IsNode())
	{
		NiNode* pkNode = (NiNode*)pkAVObject;
		NIASSERT(pkNode);

		for (uint ui = 0; ui < pkNode->GetArrayCount(); ++ui)
		{
			NiAVObject* pkChild = pkNode->GetAt(ui);
			if (pkChild)
				DisableZWrite(pkChild);
		}
	}
}
//-------------------------------------------------------------------------------------------------
void sdRenderHelper::DisableZTest(NiAVObject* pkAVObject)
{
	NIASSERT(pkAVObject);

	NiProperty* pkProperty = pkAVObject->GetProperty(NiZBufferProperty::GetType());
	if (pkProperty)
	{
		NiZBufferProperty* pkZBufferProperty = (NiZBufferProperty*)(pkProperty);
		NIASSERT(pkZBufferProperty);

		pkZBufferProperty->SetZBufferTest(false);
		pkZBufferProperty->SetZBufferWrite(true);
	}

	if (pkAVObject->IsNode())
	{
		NiNode* pkNode = (NiNode*)pkAVObject;
		NIASSERT(pkNode);

		for (uint ui = 0; ui < pkNode->GetArrayCount(); ++ui)
		{
			NiAVObject* pkChild = pkNode->GetAt(ui);
			if (pkChild)
				DisableZTest(pkChild);
		}
	}
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE