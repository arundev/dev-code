//*************************************************************************************************
// 内容:	地形编辑帮助类
//---------------------------------------------------------
// 作者:		
// 创建:		2012-09-21
// 最后修改:
//*************************************************************************************************
#pragma once
#include "MColor.h"

SD_GAMEEDITEX_NAMESPACE_BEGIN_GAMEEDITEX
class sdEditTerrainHelper;
SD_GAMEEDITEX_NAMESPACE_END_GAMEEDITEX

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
public ref class MEditTerrainHelper
{
public:
	MEditTerrainHelper();

	// 地形高度编辑
	// @{
	property int DeformBrushType{void set(int iType);int get();}
	property float DeformBrushOutterRadius{	void set(float fRadius);float get();}
	property float DeformBrushInnerRadius{	void set(float fRadius);float get();}
	property float DeformBrushHardness{	void set(float fHardness);float get();}
	property float DeformBrushHeight{void set(float fHeight);float get();}
	// @}
	
	// 地形纹理编辑
	// @{
	property float SurfaceBrushOutterRadius{ void set(float fRadius);float get();}
	property float SurfaceBrushInnerRadius{	void set(float fRadius);float get();}
	property float SurfaceBrushHardness{ void set(float fHardness);float get();}	

	unsigned int GetLayerMapCount();

	unsigned int GetActiveLayerMap();
	void SetActiveLayerMap(unsigned int uiIndex);

	bool AppendLayerMap(System::String^ strName, System::String^ strDiffuseMapName, System::String^ strNormalMapName, float fUVRepeat, float fMipmapBias);
	bool RemoveLayerMap(uint uiIndex);
	bool SwapLayerMap(uint uiLIndex, uint uiRIndex);

	System::String^ GetLayerMapName(unsigned int uiIndex);
	void SetLayerMapName(unsigned int uiIndex, System::String^ strName);

	System::String^ GetLayerMapDiffuseMap(unsigned int uiIndex);
	void SetLayerMapDiffuseMap(unsigned int uiIndex, System::String^ strName);

	System::String^ GetLayerMapNormalMap(unsigned int uiIndex);
	void SetLayerMapNormalMap(unsigned int uiIndex, System::String^ strName);

	float GetLayerMapRepeatSize(unsigned int uiIndex);
	void SetLayerMapRepeatSize(unsigned int uiIndex, float fRepeat);

	float GetLayerMapMipmapBias(unsigned int uiIndex);
	void SetLayerMapMipmapBias(unsigned int uiIndex, float fBias);
	// @}

	// 地形纹理调试
	// @{
	bool	GetShowTileGrid();
	void	SetShowTileGrid(bool bShow);

	bool	GetShowCurrentLayer();
	void	SetShowCurrentLayer(bool bShow);
	void	SetCurrentLayerIndex(uint uiIndex);

	bool	GetShowInversibleLayers();
	void	SetShowInversibleLayers(bool bShow);
	// @}

	// 地形材质编辑
	// @{
	MColor^ GetAmbientMaterial();
	void SetAmbientMaterial(MColor^ mColor);

	MColor^ GetDiffuseMaterial();
	void SetDiffuseMaterial(MColor^ kDiffuseMaterial);

	MColor^ GetSpecularMaterial();
	void SetSpecularMaterial(MColor^ kSpecularMaterial);

	float GetShiness();
	void SetShiness(float kSpecularMaterial);
	// @} 

protected:
	GameEditEx::sdEditTerrainHelper* m_pkEditTerrainHelper;
};
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK