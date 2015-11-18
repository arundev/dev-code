#include "sdMovieSystemPCH.h"
#include "sdAnimAffectorControl.h"

ENGINE_NAMESPACE_BEGIN_MOVIESYSTEM
//-------------------------------------------------------------------------------------------------
sdAnimAffectorControl::sdAnimAffectorControl()
{

}
//-------------------------------------------------------------------------------------------------
sdAnimAffectorControl::~sdAnimAffectorControl()
{
	Clear();
}
//-------------------------------------------------------------------------------------------------
bool sdAnimAffectorControl::Load(Engine::Util::sdLuaReadUtil& kLuaStream)
{
	//int t = kLuaStream.LoopReset();
	//while (kLuaStream.LoopNext(t))
	//{
	//	std::string strType;
	//	kLuaStream.ReadData("AffectorType", strType);

	//}

	return true;
}
//-------------------------------------------------------------------------------------------------
bool sdAnimAffectorControl::Save(Engine::Util::sdLuaWriteUtil& kLuaStream)
{


	return true;
}
//-------------------------------------------------------------------------------------------------
void sdAnimAffectorControl::Clear()
{
	AffectorDescVecItr itr = m_kAffectorDescVec.begin();
	AffectorDescVecItr itr_end = m_kAffectorDescVec.end();
	for (; itr != itr_end; ++itr)
	{
		itr->pkAffector = 0;
	}
	m_kAffectorDescVec.clear();
}
//-------------------------------------------------------------------------------------------------
int sdAnimAffectorControl::AddAffector(EAnimAffectorType eAnimAffectorType, int iTracktype)
{
	//sdAffectorDesc kDesc;
	//kDesc.eAnimAffectorType = eAnimAffectorType;

	//IAnimAffector* pkAffector = CreateAffector

	return -1;
}
//-------------------------------------------------------------------------------------------------
void sdAnimAffectorControl::RemoveAffector(EAnimAffectorType eAnimAffectorType)
{

}
//-------------------------------------------------------------------------------------------------
IAnimAffector* sdAnimAffectorControl::GetAffectorByType(EAnimAffectorType eAnimAffectorType)
{
	return NULL;
}
//-------------------------------------------------------------------------------------------------
IAnimAffector* sdAnimAffectorControl::GetAffectorByIndex(int iIndex)
{
	return NULL;
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_MOVIESYSTEM