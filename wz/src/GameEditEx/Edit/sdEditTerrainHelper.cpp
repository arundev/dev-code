#include "sdGameEditExPCH.h"
#include "sdEditTerrainHelper.h"

#include <sdMap.h>
#include <IRenderSystem.h>

using namespace Engine::RenderSystem;

SD_GAMEEDITEX_NAMESPACE_BEGIN_GAMEEDITEX
//-------------------------------------------------------------------------------------------------
sdEditTerrainHelper::sdEditTerrainHelper(sdTerrainDeformMode* pkTerrainDeformMode, 
	sdTerrainSurfaceMode* pkTerrainSurfaceMode)
: m_pkTerrainDeformMode(pkTerrainDeformMode)
, m_pkTerrainSurfaceMode(pkTerrainSurfaceMode)
{
	NIASSERT(pkTerrainDeformMode);
	NIASSERT(pkTerrainSurfaceMode);

	//
	IRenderSystem* pkRenderSystem = IRenderSystem::GetRenderSystem();
	NIASSERT(pkRenderSystem);

	m_pkMap = pkRenderSystem->GetMap();
	NIASSERT(m_pkMap);

	m_pkTerrain = m_pkMap->GetTerrain();
	NIASSERT(m_pkTerrain);
}
//-------------------------------------------------------------------------------------------------
sdEditTerrainHelper::~sdEditTerrainHelper()
{

}
//-------------------------------------------------------------------------------------------------
bool sdEditTerrainHelper::AppendLayerMap(const char* szName, const char* szDiffuseMapName, const char* szNormalMapName, float fUVRepeat, float fMipmapBias)
{
	NIASSERT(szName);
	NIASSERT(szDiffuseMapName);
	NIASSERT(szNormalMapName);

	if (m_pkTerrain->GetLayerMap("szName"))
		return false;

	sdLayerMap* pkLayerMap = NiNew sdLayerMap(szName, szDiffuseMapName, szNormalMapName, fUVRepeat, fMipmapBias);
	NIASSERT(pkLayerMap);

	bool bSuccess = m_pkTerrain->AppendLayerMap(pkLayerMap);
	if (!bSuccess)
	{
		NiDelete pkLayerMap;
		pkLayerMap = NULL;
	}

	return bSuccess;
}
//-------------------------------------------------------------------------------------------------
bool sdEditTerrainHelper::RemoveLayerMap(uint uiIndex)
{
	bool bSuccess = m_pkTerrain->RemoveLayerMap(uiIndex);
	if (bSuccess)
	{
		m_pkTerrain->UpdateBlendMap(0.f, 0.f, m_pkTerrain->GetTerrainSize() * 1.42f);
		m_pkTerrain->RepackMaps();
	}
	return bSuccess;
}
//-------------------------------------------------------------------------------------------------
bool sdEditTerrainHelper::SwapLayerMap(uint uiLIndex, uint uiRIndex)
{
	bool bSuccess = m_pkTerrain->SwapLayerMap(uiLIndex, uiRIndex);
	if (bSuccess)
	{
		m_pkTerrain->UpdateBlendMap(0.f, 0.f, m_pkTerrain->GetTerrainSize() * 1.42f);
		m_pkTerrain->RepackMaps();
	}
	return bSuccess;
}
//-------------------------------------------------------------------------------------------------
const char* sdEditTerrainHelper::GetLayerMapName(uint uiIndex) const
{
	sdLayerMap* pkLayerMap = m_pkTerrain->GetLayerMap(uiIndex);
	if (pkLayerMap)
		return pkLayerMap->GetName();
	return NULL;
}
//-------------------------------------------------------------------------------------------------
void sdEditTerrainHelper::SetLayerMapName(uint uiIndex, const char* szName)
{
	sdLayerMap* pkLayerMap = m_pkTerrain->GetLayerMap(uiIndex);
	if (pkLayerMap)
		return pkLayerMap->SetName(szName);
}
//-------------------------------------------------------------------------------------------------
const char* sdEditTerrainHelper::GetLayerMapDiffuseMap(uint uiIndex) const
{
	sdLayerMap* pkLayerMap = m_pkTerrain->GetLayerMap(uiIndex);
	if (pkLayerMap)
		return pkLayerMap->GetDiffuseMapPath();
	return NULL;
}
//-------------------------------------------------------------------------------------------------
void sdEditTerrainHelper::SetLayerMapDiffuseMap(uint uiIndex, const char* szName)
{
	//sdLayerMap* pkLayerMap = m_pkTerrain->GetLayerMap(uiIndex);
	//if (pkLayerMap)
	//	pkLayerMap->SetDiffuseMapPath(szName);
}
//-------------------------------------------------------------------------------------------------
const char* sdEditTerrainHelper::GetLayerMapNormalMap(uint uiIndex) const
{
	sdLayerMap* pkLayerMap = m_pkTerrain->GetLayerMap(uiIndex);
	if (pkLayerMap)
		return pkLayerMap->GetNormalMapPath();
	return NULL;
}
//-------------------------------------------------------------------------------------------------
void sdEditTerrainHelper::SetLayerMapNormalMap(uint uiIndex, const char* szName)
{
	//sdLayerMap* pkLayerMap = m_pkTerrain->GetLayerMap(uiIndex);
	//if (pkLayerMap)
	//	pkLayerMap->SetNormalMapPath(szName);
}
//-------------------------------------------------------------------------------------------------
float sdEditTerrainHelper::GetLayerMapRepeatSize(uint uiIndex) const
{
	sdLayerMap* pkLayerMap = m_pkTerrain->GetLayerMap(uiIndex);
	if (pkLayerMap)
		return pkLayerMap->GetRepeatSize();
	return 1.f;
}
//-------------------------------------------------------------------------------------------------
void sdEditTerrainHelper::SetLayerMapRepeatSize(uint uiIndex, float fScale)
{
	sdLayerMap* pkLayerMap = m_pkTerrain->GetLayerMap(uiIndex);
	if (pkLayerMap)
	{
		pkLayerMap->SetRepeatSize(fScale);
		m_pkTerrain->RepackMaps();
	}
}
//-------------------------------------------------------------------------------------------------
float sdEditTerrainHelper::GetLayerMapMipmapBias(uint uiIndex) const
{
	sdLayerMap* pkLayerMap = m_pkTerrain->GetLayerMap(uiIndex);
	if (pkLayerMap)
		return pkLayerMap->GetMipmapBias();
	return 0.f;
}
//-------------------------------------------------------------------------------------------------
void sdEditTerrainHelper::SetLayerMapMipmapBias(uint uiIndex, float fBias)
{
	sdLayerMap* pkLayerMap = m_pkTerrain->GetLayerMap(uiIndex);
	if (pkLayerMap)
	{
		pkLayerMap->SetMipmapBias(fBias);
		m_pkTerrain->RepackMaps();
	}
}
//-------------------------------------------------------------------------------------------------
SD_GAMEEDITEX_NAMESPACE_END_GAMEEDITEX