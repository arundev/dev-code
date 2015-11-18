//-------------------------------------------------------------------------------------------------
EAnimNodeType sdAnimNode::GetType()
{
	return E_ANIM_NODE_INVALID;
}
//-------------------------------------------------------------------------------------------------
void sdAnimNode::ClearValue(const sdAnimProperty* pkProp)
{
	AnimValueMapItr itr = GetAnimValueMap().find(pkProp);
	AnimValueMapItr itr_end = GetAnimValueMap().end();
	if (itr != itr_end)
	{
		NiDelete itr->second;
		GetAnimValueMap().erase(itr);
	}
}
//-------------------------------------------------------------------------------------------------
void sdAnimNode::ClearAllValues()
{
	AnimValueMapItr itr = GetAnimValueMap().begin();
	AnimValueMapItr itr_end = GetAnimValueMap().end();
	for (; itr != itr_end; ++itr)
	{
		NiDelete itr->second;
		itr->second = NULL;
	}
	GetAnimValueMap().clear();
}
//-------------------------------------------------------------------------------------------------