#include "sdEnginePCH.h"
#include "sdGameObject.h"
#include "sdStreamMacros.h"

///< 只能暂时共存
NiImplementRTTI(sdGameObject, NiNode);
sdImplementDefaultStream(sdGameObject);

sdHashCode sdGameObject::ms_kHashCodeGen;
//-------------------------------------------------------------------------------------------------
void RegisterPropFunction(std::vector<PropFuncEntry>& entries, 
	const NiFixedString& funcName, uchar type, uchar flag, PropFunc getter, PropFunc setter)
{
	///< 暂时没有检查属性重复问题
	struct PropFuncEntry entry = { getter, setter, (const char*)funcName, type, flag};
	entries.push_back(entry);
}
//-------------------------------------------------------------------------------------------------
void UnregisterPropFunction(std::vector<PropFuncEntry>& entries, const NiFixedString& funcName)
{
	std::vector<PropFuncEntry>::iterator itr = entries.begin();
	std::vector<PropFuncEntry>::iterator itr_end = entries.end();
	for (; itr!=itr_end; ++itr)
	{
		if (itr->func_name == (const char*)funcName)
		{
			entries.erase(itr);
			return;
		}
	}
}
//-------------------------------------------------------------------------------------------------
TypeClass* TypeClass::firstClass = NULL;
//-------------------------------------------------------------------------------------------------
// 关于属性尺寸
//-------------------------------------------------------------------------------------------------
uint sdGameObject::ms_akTypeSizeTable[PROP_TOTAL] =
{
	0,
	sizeof(bool),
	sizeof(char),
	sizeof(uchar),
	sizeof(int),
	sizeof(uint),
	sizeof(float),
	sizeof(NiPoint2),
	sizeof(NiPoint3),
	sizeof(NiColor),
	sizeof(NiFixedString),
	sizeof(sdGameObject*),
	sizeof(NiPoint3),		///< 设置为元素大小
	sizeof(NiFixedString),
	sizeof(sdGameObject*),
};
//-------------------------------------------------------------------------------------------------
uint sdGameObject::GetTypeSize(uchar type)
{
	return ms_akTypeSizeTable[(uint)type];
}
//-------------------------------------------------------------------------------------------------
// 关于RTTI
//-------------------------------------------------------------------------------------------------
TypeClass sdGameObject::ms_sdGameObjectRTTI =
{
	"sdGameObject",
	sizeof(sdGameObject),
	sdGameObject::CreateGameObject,
	NULL,
	NULL,
};
static _LINK_DYNAMIC ms_sdGameObjectLink(&sdGameObject::ms_sdGameObjectRTTI);
//-------------------------------------------------------------------------------------------------
TypeClass* sdGameObject::GetTypeClass() const
{
	return &sdGameObject::ms_sdGameObjectRTTI;
}
//-------------------------------------------------------------------------------------------------
sdGameObject* sdGameObject::CreateGameObject()
{
	return NiNew sdGameObject;
}
//-------------------------------------------------------------------------------------------------
sdGameObject* sdGameObject::FactoryCreateObject(const NiFixedString& className)
{
	sdGameObject* pkObject = NULL;
	TypeClass* pkType = TypeClass::firstClass;
	while (pkType != NULL)
	{
		if (pkType->className == (const char*)className)
		{
			pkObject = pkType->pfnCreateObject();
			return pkObject;
		}
		pkType = pkType->nextClass;
	}

	return NULL;
}
//-------------------------------------------------------------------------------------------------
// 关于Stream
//-------------------------------------------------------------------------------------------------
std::vector<PropFuncEntry> sdGameObject::ms_akPropFuncEntries;
const PropFuncMap sdGameObject::ms_kPropFuncMap = 
{
	NULL, 
	&sdGameObject::ms_akPropFuncEntries,
};
//-------------------------------------------------------------------------------------------------
const PropFuncMap* sdGameObject::GetPropFuncMap() const
{
	return &sdGameObject::ms_kPropFuncMap;
}
//-------------------------------------------------------------------------------------------------
uint sdGameObject::GetPropertyEntries(std::vector<PropFuncEntry>& kVecProps)
{
	// 从派生类到根类,提取操作接口
	std::vector<const PropFuncMap*> kPropFuncMapVec;
	const PropFuncMap* pkMap = GetPropFuncMap();
	while (pkMap)
	{
		kPropFuncMapVec.push_back(pkMap);
		pkMap = pkMap->basePropFuncMap;
	}

	// 逆序提取
	typedef std::vector<const PropFuncMap*>::reverse_iterator PropFuncMapVecRItr;
	PropFuncMapVecRItr itr_map = kPropFuncMapVec.rbegin();
	PropFuncMapVecRItr itr_map_end = kPropFuncMapVec.rend();
	for (; itr_map != itr_map_end; ++itr_map)
	{
		const PropFuncMap* pkPropFuncMap = (*itr_map);
		if (!pkPropFuncMap->propFuncEntries->empty())
		{
			typedef std::vector<PropFuncEntry>::iterator PropFuncEntryVecItr;
			PropFuncEntryVecItr itr = pkPropFuncMap->propFuncEntries->begin();
			PropFuncEntryVecItr itr_end = pkPropFuncMap->propFuncEntries->end();
			for (; itr!=itr_end; ++itr)
			{
				kVecProps.push_back(*itr);
			}
		}
	}

	return kVecProps.size();
}
//-------------------------------------------------------------------------------------------------
sdGameObject* sdGameObject::CloneWithoutCreate()
{
	sdGameObject* pkNewObject = FactoryCreateObject(GetClassType());
	if (pkNewObject)
	{
		std::vector<PropFuncEntry> kPropFuncEntryVec;
		union PropResult		unResult;
		NiFixedString			kStringResult;
		NiColor					kColorResult;
		NiPoint2				kPoint2Result;
		NiPoint3				kPoint3Result;
		union PropGetterFunc	pfnPropGetter;
		union PropSetterFunc	pfnPropSetter;
		uint					uiElementNum;
		
		GetPropertyEntries(kPropFuncEntryVec);
		std::vector<PropFuncEntry>::iterator itr = kPropFuncEntryVec.begin();
		std::vector<PropFuncEntry>::iterator itr_end = kPropFuncEntryVec.end();
		for (; itr != itr_end; ++itr)
		{
			pfnPropGetter.pfn = (*itr).getter;
			pfnPropSetter.pfn = (*itr).setter;

			switch ((*itr).type) 
			{
			case PROP_BOOL :
				unResult.bResult = (this->*pfnPropGetter.pfn_b)();
				(pkNewObject->*pfnPropSetter.pfn_b)(unResult.bResult);
				break;
			case PROP_CHAR :
				unResult.cResult = (this->*pfnPropGetter.pfn_c)();
				(pkNewObject->*pfnPropSetter.pfn_c)(unResult.cResult);
				break;
			case PROP_UCHAR :
				unResult.ucResult = (this->*pfnPropGetter.pfn_uc)();
				(pkNewObject->*pfnPropSetter.pfn_uc)(unResult.ucResult);
				break;
			case PROP_INT :
				unResult.iResult = (this->*pfnPropGetter.pfn_i)();
				(pkNewObject->*pfnPropSetter.pfn_i)(unResult.iResult);
				break;
			case PROP_UINT :
				unResult.uiResult = (this->*pfnPropGetter.pfn_ui)();
				(pkNewObject->*pfnPropSetter.pfn_ui)(unResult.uiResult);
				break;
			case PROP_FLOAT :
				unResult.fResult = (this->*pfnPropGetter.pfn_f)();
				(pkNewObject->*pfnPropSetter.pfn_f)(unResult.fResult);
				break;
			case PROP_NIPT2 :
				kPoint2Result = (this->*pfnPropGetter.pfn_nipt2)();
				(pkNewObject->*pfnPropSetter.pfn_nipt2)(kPoint2Result);
				break;
			case PROP_NIPT3 :
				kPoint3Result = (this->*pfnPropGetter.pfn_nipt3)();
				(pkNewObject->*pfnPropSetter.pfn_nipt3)(kPoint3Result);
				break;
			case PROP_NICOLOR :
				kColorResult = (this->*pfnPropGetter.pfn_nicolor)();
				(pkNewObject->*pfnPropSetter.pfn_nicolor)(kColorResult);
				break;
			case PROP_NISTRING :
				kStringResult = (this->*pfnPropGetter.pfn_nistring)();
				(pkNewObject->*pfnPropSetter.pfn_nistring)(kStringResult);
				break;
			case PROP_SDOBJECT :
				unResult.pkObjectResult = (this->*pfnPropGetter.pfn_sdobject)();
				(pkNewObject->*pfnPropSetter.pfn_sdobject)((const sdGameObject *&)unResult.pkObjectResult);
				break;
			case PROP_NIPT3_C :
				uiElementNum = (this->*pfnPropGetter.pfn_nipt3c)(kPoint3Result, 0);
				for (uint ui = 0; ui < uiElementNum; ++ui)
				{
					(this->*pfnPropGetter.pfn_nipt3c)(kPoint3Result, ui);
					(pkNewObject->*pfnPropSetter.pfn_nipt3c)(kPoint3Result, ui);
				}	
				break;
			case PROP_NISTRING_C :
				uiElementNum = (this->*pfnPropGetter.pfn_nistringc)(kStringResult, 0);
				for (uint ui = 0; ui < uiElementNum; ++ui)
				{
					(this->*pfnPropGetter.pfn_nistringc)(kStringResult, ui);
					(pkNewObject->*pfnPropSetter.pfn_nistringc)(kStringResult, ui);
				}	
				break;
			case PROP_SDOBJECT_C :
				uiElementNum = (this->*pfnPropGetter.pfn_sdobjectc)(unResult.pkObjectResult, 0);
				for (uint ui = 0; ui < uiElementNum; ++ui)
				{
					(this->*pfnPropGetter.pfn_sdobjectc)(unResult.pkObjectResult, ui);
					(pkNewObject->*pfnPropSetter.pfn_sdobjectc)((const sdGameObject *&)unResult.pkObjectResult, ui);
				}	
				break;
			default:
				NIASSERT("Unknown data type property!");
				break;
			}
		}
	}

	return pkNewObject;
}
//-------------------------------------------------------------------------------------------------
bool sdGameObject::SetPropValueByName(const char* name, uint index, void* inData, uint dataSize)
{
	uint valueSize = GetTypeSize((GetPropTypeByName(name)));
	if (dataSize < valueSize)
		return false;

	std::vector<PropFuncEntry> kPropFuncEntryVec;
	GetPropertyEntries(kPropFuncEntryVec);

	uint uiElementNum = 0;
	union PropSetterFunc	pfnPropSetter;
	std::vector<PropFuncEntry>::iterator itr = kPropFuncEntryVec.begin();
	std::vector<PropFuncEntry>::iterator itr_end = kPropFuncEntryVec.end();
	for (; itr != itr_end; ++itr)
	{
		if ((*itr).func_name == name)
		{
			pfnPropSetter.pfn = (*itr).setter;

			switch ((*itr).type) 
			{
			case PROP_BOOL :
				(this->*pfnPropSetter.pfn_b)(*((bool*)inData));
				break;
			case PROP_CHAR :
				(this->*pfnPropSetter.pfn_c)(*((char*)inData));
				break;
			case PROP_UCHAR :
				(this->*pfnPropSetter.pfn_uc)(*((uchar*)inData));
				break;
			case PROP_INT :
				(this->*pfnPropSetter.pfn_i)(*((int*)inData));
				break;
			case PROP_UINT :
				(this->*pfnPropSetter.pfn_ui)(*((uint*)inData));
				break;
			case PROP_FLOAT :
				(this->*pfnPropSetter.pfn_f)(*((float*)inData));
				break;
			case PROP_NIPT2 :
				(this->*pfnPropSetter.pfn_nipt2)(*((NiPoint2*)inData));
				break;
			case PROP_NIPT3 :
				(this->*pfnPropSetter.pfn_nipt3)(*((NiPoint3*)inData));
				break;
			case PROP_NICOLOR :
				(this->*pfnPropSetter.pfn_nicolor)(*((NiColor*)inData));
				break;
			case PROP_NISTRING :
				(this->*pfnPropSetter.pfn_nistring)(*((const char**)inData));
				break;
			//case PROP_SDOBJECT :
			//	(*((sdGameObject**)outData)) = (this->*pfnPropGetter.pfn_sdobject)();
			//	break;
			//case PROP_NIPT3_C :	///< 这里没有判断溢出
			//	uiElementNum = (this->*pfnPropGetter.pfn_nipt3c)(*((NiPoint3*)outData), index);	
			//	break;
			//case PROP_NISTRING_C :
			//	uiElementNum = (this->*pfnPropGetter.pfn_nistringc)(*((NiFixedString*)outData), index);	
			//	break;
			//case PROP_SDOBJECT_C :
			//	uiElementNum = (this->*pfnPropGetter.pfn_sdobjectc)(*((sdGameObject**)outData), index);		
			//	break;
			default:
				NIASSERT("Unknown data type property!");
				break;
			}
		}
	}
	return true;
}
//-------------------------------------------------------------------------------------------------
uint sdGameObject::GetPropValueByName(const char* name, uint index, void* outData, uint dataSize, uint& retDataSize)
{
	std::vector<PropFuncEntry> kPropFuncEntryVec;
	GetPropertyEntries(kPropFuncEntryVec);

	uint uiElementNum = 0;
	union PropGetterFunc	pfnPropGetter;
	std::vector<PropFuncEntry>::iterator itr = kPropFuncEntryVec.begin();
	std::vector<PropFuncEntry>::iterator itr_end = kPropFuncEntryVec.end();
	for (; itr != itr_end; ++itr)
	{
		if ((*itr).func_name == name)
		{
			///< 防止数据溢出
			uint typeSize = GetTypeSize((*itr).type);
			if (typeSize > dataSize)
			{
				retDataSize = 0;
				return 0;
			}

			pfnPropGetter.pfn = (*itr).getter;

			switch ((*itr).type) 
			{
			case PROP_BOOL :
				(*((bool*)outData)) = (this->*pfnPropGetter.pfn_b)();
				break;
			case PROP_CHAR :
				(*((char*)outData)) = (this->*pfnPropGetter.pfn_c)();
				break;
			case PROP_UCHAR :
				(*((uchar*)outData)) = (this->*pfnPropGetter.pfn_uc)();
				break;
			case PROP_INT :
				(*((int*)outData)) = (this->*pfnPropGetter.pfn_i)();
				break;
			case PROP_UINT :
				(*((uint*)outData)) = (this->*pfnPropGetter.pfn_ui)();
				break;
			case PROP_FLOAT :
				(*((float*)outData)) = (this->*pfnPropGetter.pfn_f)();
				break;
			case PROP_NIPT2 :
				(*((NiPoint2*)outData)) = (this->*pfnPropGetter.pfn_nipt2)();
				break;
			case PROP_NIPT3 :
				(*((NiPoint3*)outData)) = (this->*pfnPropGetter.pfn_nipt3)();
				break;
			case PROP_NICOLOR :
				(*((NiColor*)outData)) = (this->*pfnPropGetter.pfn_nicolor)();
				break;
			case PROP_NISTRING :
				(*((NiFixedString*)outData)) = (this->*pfnPropGetter.pfn_nistring)();
				break;
			//case PROP_SDOBJECT :
			//	(*((sdGameObject**)outData)) = (this->*pfnPropGetter.pfn_sdobject)();
			//	break;
			case PROP_NIPT3_C :	///< 这里没有判断溢出
				uiElementNum = (this->*pfnPropGetter.pfn_nipt3c)(*((NiPoint3*)outData), index);	
				break;
			case PROP_NISTRING_C :
				uiElementNum = (this->*pfnPropGetter.pfn_nistringc)(*((NiFixedString*)outData), index);	
				break;
			//case PROP_SDOBJECT_C :
			//	uiElementNum = (this->*pfnPropGetter.pfn_sdobjectc)(*((sdGameObject**)outData), index);		
			//	break;
			default:
				NIASSERT("Unknown data type property!");
				break;
			}

			retDataSize = typeSize;
			return uiElementNum;
		}
	}

	retDataSize = 0;
	return 0;
}
//-------------------------------------------------------------------------------------------------
uchar sdGameObject::GetPropTypeByName(const char* name)
{
	std::vector<PropFuncEntry> kPropFuncEntryVec;
	GetPropertyEntries(kPropFuncEntryVec);

	std::vector<PropFuncEntry>::iterator itr = kPropFuncEntryVec.begin();
	std::vector<PropFuncEntry>::iterator itr_end = kPropFuncEntryVec.end();
	for (; itr != itr_end; ++itr)
	{
		if ((*itr).func_name == name)
			return (*itr).type;
	}

	return PROP_UNKNOWN;
}
//-------------------------------------------------------------------------------------------------
uchar sdGameObject::GetPropAttrByName(const char* name)
{
	std::vector<PropFuncEntry> kPropFuncEntryVec;
	GetPropertyEntries(kPropFuncEntryVec);

	std::vector<PropFuncEntry>::iterator itr = kPropFuncEntryVec.begin();
	std::vector<PropFuncEntry>::iterator itr_end = kPropFuncEntryVec.end();
	for (; itr != itr_end; ++itr)
	{
		if ((*itr).func_name == name)
			return (*itr).flag;
	}

	return PROP_ATTR_NONE;
}
//-------------------------------------------------------------------------------------------------
sdGameObject::sdGameObject() : m_uiHashCode(0)
{
	// 建议将此定义在函数外面,但必须在ms_akPropFuncEntries之后(类似MFC?)
	BEGIN_PROPFUNC_MAP(sdGameObject)
		REGISTER_PROPFUNC(PROP_NISTRING, PROP_ATTR_READ | PROP_ATTR_STREAM, sdGameObject, TP)	///< Type
		REGISTER_PROPFUNC(PROP_NISTRING, PROP_ATTR_READ | PROP_ATTR_STREAM, sdGameObject, CD)	///< HashCode
		REGISTER_PROPFUNC(PROP_NISTRING, PROP_ATTR_DEFAULT, sdGameObject, NM)					///< Name
	END_PROPFUNC_MAP(sdGameObject)
}
//-------------------------------------------------------------------------------------------------
sdGameObject::~sdGameObject()
{

}
//-------------------------------------------------------------------------------------------------
//void* sdGameObject::operator new(size_t size)
//{
//	return g_pkEntityPool->CreateGameObject(ms_sdGameObjectRTTI.className);
//}
//-------------------------------------------------------------------------------------------------
//void sdGameObject::operator delete(void* p, size_t size)
//{
//	g_pkEntityPool->DeleteGameObject(p);
//}
//-------------------------------------------------------------------------------------------------