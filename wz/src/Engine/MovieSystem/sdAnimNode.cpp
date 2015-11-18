#include "sdMovieSystemPCH.h"
#include "sdAnimNode.h"

ENGINE_NAMESPACE_BEGIN_MOVIESYSTEM
//-------------------------------------------------------------------------------------------------
const char* sdAnimNode::GetStringFromType(EAnimNodeType eType)
{
	return NULL;
}
//-------------------------------------------------------------------------------------------------
EAnimNodeType sdAnimNode::GetTypeFromString(const char* szName)
{
	return E_ANIM_NODE_INVALID;
}
//-------------------------------------------------------------------------------------------------
sdAnimNode::sdAnimNode()
{

}
//-------------------------------------------------------------------------------------------------
sdAnimNode::~sdAnimNode()
{
	Clear();
}
//-------------------------------------------------------------------------------------------------
bool sdAnimNode::Load(Engine::Util::sdLuaReadUtil& kLuaStream)
{
	
	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdAnimNode::Save(Engine::Util::sdLuaWriteUtil& kLuaStream)
{
	return true;
}
//-------------------------------------------------------------------------------------------------
void sdAnimNode::Clear()
{
	
}
//-------------------------------------------------------------------------------------------------
void sdAnimNode::UpdateFrame(float fAccumTime, float fFrameTime)
{

}
//-------------------------------------------------------------------------------------------------
void AddChild(IAnimNode* pkNode)
{

}
//-------------------------------------------------------------------------------------------------
const sdAnimProperty* sdAnimNode::FindProperty(const std::string& strName) const
{
	stAnimPropTable* pkAnimPropTable = GetAnimPropTable();
	while (pkAnimPropTable)
	{
		const sdAnimProperty* pkAnimProperty = pkAnimPropTable->propTable[0];
		for (; pkAnimProperty; ++pkAnimProperty)
		{
			if (pkAnimProperty->GetName() == strName)
				return pkAnimProperty;
		}

		pkAnimPropTable = pkAnimPropTable->baseTable;
	}

	return NULL;
}
//-------------------------------------------------------------------------------------------------
bool sdAnimNode::HasProperty(const std::string& strName) const
{
	stAnimPropTable* pkAnimPropTable = GetAnimPropTable();
	while (pkAnimPropTable)
	{
		const sdAnimProperty* pkAnimProperty = pkAnimPropTable->propTable[0];
		for (; pkAnimProperty; ++pkAnimProperty)
		{
			if (pkAnimProperty->GetName() == strName)
				return true;
		}

		pkAnimPropTable = pkAnimPropTable->baseTable;
	}

	return false;
}
//-------------------------------------------------------------------------------------------------
bool sdAnimNode::HasProperty(const sdAnimProperty* pkProp) const
{
	stAnimPropTable* pkAnimPropTable = GetAnimPropTable();
	while (pkAnimPropTable)
	{
		const sdAnimProperty* pkAnimProperty = pkAnimPropTable->propTable[0];
		for (; pkAnimProperty; ++pkAnimProperty)
		{
			if (pkAnimProperty == pkProp)
				return true;
		}

		pkAnimPropTable = pkAnimPropTable->baseTable;
	}

	return false;
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_MOVIESYSTEM