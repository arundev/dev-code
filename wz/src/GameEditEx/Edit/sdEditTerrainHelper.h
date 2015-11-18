//*************************************************************************************************
// 内容:	地形编辑对外接口
//---------------------------------------------------------
// 作者:		
// 创建:		
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMEEDITEX_EDITTERRAINHELPER_H__
#define _SD_GAMEEDITEX_EDITTERRAINHELPER_H__
#include "sdTerrainDeformMode.h"
#include "sdTerrainSurfaceMode.h"

#include <sdTerrain.h>

class sdMap;
class sdTerrain;

SD_GAMEEDITEX_NAMESPACE_BEGIN_GAMEEDITEX
class sdTerrainDeformMode;
class sdTerrainSurfaceMode;
SD_GAMEEDITEX_NAMESPACE_END_GAMEEDITEX

SD_GAMEEDITEX_NAMESPACE_BEGIN_GAMEEDITEX
//-------------------------------------------------------------------------------------------------
// 地形编辑对外接口,相当于一个Facade,避免上层逻辑直接调用FSM
//-------------------------------------------------------------------------------------------------
class sdEditTerrainHelper : public NiRefObject
{
public:
	sdEditTerrainHelper(sdTerrainDeformMode* pkTerrainDeformMode, 
		sdTerrainSurfaceMode* pkTerrainSurfaceMode);
	virtual ~sdEditTerrainHelper();

	// 地形高度编辑
	// @{
	uint	GetDeformBrushType();
	void	SetActiveDeformBrush(uint uiBrush);

	void	SetDeformBrushVisible(bool bVisible);

	float	GetDeformBrushOutterRadius();
	void	SetDeformBrushOutterRadius(float fRadius);

	float	GetDeformBrushInnerRadius();
	void	SetDeformBrushInnerRadius(float fRadius);

	float	GetDeformBrushHardness();
	void	SetDeformBrushHardness(float fHardness);

	float	GetDeformBrushHeight();
	void	SetDeformBrushHeight(float fHeight);
	// @}

	// 地形纹理编辑
	// @{
	void	SetSurfaceBrushVisible(bool bVisible);

	float	GetSurfaceBrushOutterRadius();
	void	SetSurfaceBrushOutterRadius(float fRadius);

	float	GetSurfaceBrushInnerRadius();
	void	SetSurfaceBrushInnerRadius(float fRadius);

	float	GetSurfaceBrushHardness();
	void	SetSurfaceBrushHardness(float fHardness);
	
	uint	GetLayerMapCount() const { return m_pkTerrain->GetLayerNum();}

	uint	GetActiveLayerMap() const { return m_pkTerrainSurfaceMode->GetActiveLayerMap();}
	void	SetActiveLayerMap(uint uiIndex) { m_pkTerrainSurfaceMode->SetActiveLayerMap(uiIndex);}

	bool AppendLayerMap(const char* szName, const char* szDiffuseMapName, const char* szNormalMapName, float fUVRepeat, float fMipmapBias);
	bool RemoveLayerMap(uint uiIndex);
	bool SwapLayerMap(uint uiLIndex, uint uiRIndex);

	const char* GetLayerMapName(uint uiIndex) const;
	void	SetLayerMapName(uint uiIndex, const char* szName);

	const char* GetLayerMapDiffuseMap(uint uiIndex) const;
	void SetLayerMapDiffuseMap(uint uiIndex, const char* szName);

	const char* GetLayerMapNormalMap(uint uiIndex) const;
	void SetLayerMapNormalMap(uint uiIndex, const char* szName);

	//const NiPoint3& GetLayerMapTranslate(uint uiIndex) const;
	//void SetLayerMapTranslate(uint uiIndex, NiPoint3& kTranslate);

	//const NiMatrix3& GetLayerMapRotate(uint uiIndex) const;
	//void SetLayerMapRotate(uint uiIndex, NiMatrix3& kRotate);

	//float GetLayerMapScale(uint uiIndex) const;
	//void SetLayerMapScale(uint uiIndex, float fScale);

	float GetLayerMapRepeatSize(uint uiIndex) const;
	void SetLayerMapRepeatSize(uint uiIndex, float fRepeat);

	float GetLayerMapMipmapBias(uint uiIndex) const;
	void SetLayerMapMipmapBias(uint uiIndex, float fBias);
	// @}

	// 地形纹理调试
	// @{
	bool	GetShowTileGrid() const { return m_pkTerrain->GetShowTileGrid();}
	void	SetShowTileGrid(bool bShow) { m_pkTerrain->SetShowTileGrid(bShow);}

	bool	GetShowCurrentLayer() const { return m_pkTerrain->GetShowCurrentLayer();}
	void	SetShowCurrentLayer(bool bShow) { m_pkTerrain->SetShowCurrentLayer(bShow);}
	void	SetCurrentLayerIndex(uint uiIndex){ m_pkTerrain->SetCurrentLayerIndex(uiIndex);}

	bool	GetShowInversibleLayers() const { return m_pkTerrain->GetShowInversibleLayers();}
	void	SetShowInversibleLayers(bool bShow) { m_pkTerrain->SetShowInversibleLayers(bShow);}
	// @}

	// 地形材质编辑
	// @{
	const Base::Math::sdVector3& GetAmbientMaterial() const;
	void SetAmbientMaterial(const Base::Math::sdVector3& kAmbientMaterial);

	const Base::Math::sdVector3& GetDiffuseMaterial() const;
	void SetDiffuseMaterial(const Base::Math::sdVector3& kDiffuseMaterial);

	const Base::Math::sdVector3& GetSpecularMaterial() const;
	void SetSpecularMaterial(const Base::Math::sdVector3& kSpecularMaterial);

	float GetShiness() const;
	void SetShiness(float fShiness);
	// @}

protected:
	sdMap*		m_pkMap;
	sdTerrain*	m_pkTerrain;
	sdTerrainDeformMode*	m_pkTerrainDeformMode;
	sdTerrainSurfaceMode*	m_pkTerrainSurfaceMode;
};
NiSmartPointer(sdEditTerrainHelper);
#include "sdEditTerrainHelper.inl"
//-------------------------------------------------------------------------------------------------
SD_GAMEEDITEX_NAMESPACE_END_GAMEEDITEX
#endif