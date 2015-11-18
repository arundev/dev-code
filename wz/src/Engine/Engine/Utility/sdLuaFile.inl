//-------------------------------------------------------------------------------------------------
void sdLuaReadUtil::ReadRootNode(const char* szRootNodeName)
{
	lua_getglobal(m_pkLuaState, szRootNodeName);
}
//-------------------------------------------------------------------------------------------------
void sdLuaReadUtil::BeginReadNode(const char* szNodeName)
{
	lua_getfield(m_pkLuaState, -1, szNodeName);
}
//-------------------------------------------------------------------------------------------------
void sdLuaReadUtil::EndReadNode()
{
	lua_pop(m_pkLuaState, 1);
}
//-------------------------------------------------------------------------------------------------
bool sdLuaReadUtil::IsNodeValid()
{
	return lua_istable(m_pkLuaState, -1);
}
//-------------------------------------------------------------------------------------------------
int sdLuaReadUtil::LoopReset()
{
	int t = lua_gettop(m_pkLuaState);
	lua_pushnil(m_pkLuaState);
	return t;
}
//-------------------------------------------------------------------------------------------------
int sdLuaReadUtil::LoopNext(int t)
{
	return lua_next(m_pkLuaState, t);
}
//-------------------------------------------------------------------------------------------------
bool sdLuaReadUtil::LoopInnerEnd()
{
	lua_pop(m_pkLuaState, 1);
	return true;
}
//-------------------------------------------------------------------------------------------------
int sdLuaReadUtil::LoopCount()
{
	int iRet = 0;
	if (lua_istable(m_pkLuaState, -1))
	{
		int t = lua_gettop(m_pkLuaState);
		lua_pushnil(m_pkLuaState);
		while (lua_next(m_pkLuaState, t))
		{
			++iRet;
			lua_pop(m_pkLuaState, 1);
		}
	}
	return iRet;
}
//-------------------------------------------------------------------------------------------------
sdLuaWriteUtil& sdLuaWriteUtil::operator << (const char* value)
{
	m_kStream << value;
	return (*this);
}
//-------------------------------------------------------------------------------------------------