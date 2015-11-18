#include "sdEnginePCH.h"
#include "sdDoodad.h"

namespace Scene
{
//-------------------------------------------------------------------------------------------------
sdDoodad* sdDoodadsGroup::CreateDoodad()
{
	sdDoodad* pkDoodad = NiNew sdDoodad;
	NIASSERT(pkDoodad);

	m_kDoodadsVec.push_back(pkDoodad);

	return pkDoodad;
}
//-------------------------------------------------------------------------------------------------
void sdDoodadsGroup::DestroyDoodad(sdDoodad *pkDoodad)
{
	NIASSERT(pkDoodad);

	DoodadsVecItr itr = m_kDoodadsVec.begin();
	DoodadsVecItr itr_end = m_kDoodadsVec.end();
	for (; itr != itr_end; ++itr)
	{
		if (pkDoodad == *itr)
		{
			break;
		}
	}
}
//-------------------------------------------------------------------------------------------------
void sdDoodadsGroup::DestroyAllDoodads()
{
	DoodadsVecItr itr = m_kDoodadsVec.begin();
	DoodadsVecItr itr_end = m_kDoodadsVec.end();
	for (; itr != itr_end; ++itr)
	{
		NiDelete (*itr);
		*itr = NULL;
	}
	m_kDoodadsVec.clear();
}
//-------------------------------------------------------------------------------------------------
}