#include "WorldEditor.FrameworkPCH.h"
#include "MEditTerrainHelper.h"
#include "sdWorldEditor.h"
#include "MMacros.h"

//
#include <sdWorldEditFSM.h>
#include <sdEditTerrainHelper.h>

//
using namespace GameEditEx;

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
//-------------------------------------------------------------------------------------------------
MEditTerrainHelper::MEditTerrainHelper()
{
	sdWorldEditFSM* pkWorldEditFSM = sdWorldEditor::Instance().GetWorldEditFSM();
	m_pkEditTerrainHelper = pkWorldEditFSM->GetEditTerrainHelper();
}
//-------------------------------------------------------------------------------------------------
int MEditTerrainHelper::DeformBrushType::get()
{
	return m_pkEditTerrainHelper->GetDeformBrushType();
}
//-------------------------------------------------------------------------------------------------
void MEditTerrainHelper::DeformBrushType::set(int iType)
{
	m_pkEditTerrainHelper->SetActiveDeformBrush(iType);
}
//-------------------------------------------------------------------------------------------------
float MEditTerrainHelper::DeformBrushOutterRadius::get()
{
	return m_pkEditTerrainHelper->GetDeformBrushOutterRadius();
}
//-------------------------------------------------------------------------------------------------
void MEditTerrainHelper::DeformBrushOutterRadius::set(float fRadius)
{
	return m_pkEditTerrainHelper->SetDeformBrushOutterRadius(fRadius);
}
//-------------------------------------------------------------------------------------------------
float MEditTerrainHelper::DeformBrushInnerRadius::get()
{
	return m_pkEditTerrainHelper->GetDeformBrushInnerRadius();
}
//-------------------------------------------------------------------------------------------------
void MEditTerrainHelper::DeformBrushInnerRadius::set(float fRadius)
{
	return m_pkEditTerrainHelper->SetDeformBrushInnerRadius(fRadius);
}
//-------------------------------------------------------------------------------------------------
float MEditTerrainHelper::DeformBrushHardness::get()
{
	return m_pkEditTerrainHelper->GetDeformBrushHardness();
}
//-------------------------------------------------------------------------------------------------
void MEditTerrainHelper::DeformBrushHardness::set(float fHardness)
{
	return m_pkEditTerrainHelper->SetDeformBrushHardness(fHardness);
}
//-------------------------------------------------------------------------------------------------
float MEditTerrainHelper::DeformBrushHeight::get()
{
	return m_pkEditTerrainHelper->GetDeformBrushHeight();
}
//-------------------------------------------------------------------------------------------------
void MEditTerrainHelper::DeformBrushHeight::set(float fHeight)
{
	return m_pkEditTerrainHelper->SetDeformBrushHeight(fHeight);
}
//-------------------------------------------------------------------------------------------------
float MEditTerrainHelper::SurfaceBrushOutterRadius::get()
{
	return m_pkEditTerrainHelper->GetDeformBrushOutterRadius();
}
//-------------------------------------------------------------------------------------------------
void MEditTerrainHelper::SurfaceBrushOutterRadius::set(float fRadius)
{
	return m_pkEditTerrainHelper->SetSurfaceBrushOutterRadius(fRadius);
}
//-------------------------------------------------------------------------------------------------
float MEditTerrainHelper::SurfaceBrushInnerRadius::get()
{
	return m_pkEditTerrainHelper->GetSurfaceBrushInnerRadius();
}
//-------------------------------------------------------------------------------------------------
void MEditTerrainHelper::SurfaceBrushInnerRadius::set(float fRadius)
{
	return m_pkEditTerrainHelper->SetSurfaceBrushInnerRadius(fRadius);
}
//-------------------------------------------------------------------------------------------------
float MEditTerrainHelper::SurfaceBrushHardness::get()
{
	return m_pkEditTerrainHelper->GetSurfaceBrushHardness();
}
//-------------------------------------------------------------------------------------------------
void MEditTerrainHelper::SurfaceBrushHardness::set(float fHardness)
{
	return m_pkEditTerrainHelper->SetSurfaceBrushHardness(fHardness);
}
//-------------------------------------------------------------------------------------------------
unsigned int MEditTerrainHelper::GetLayerMapCount()
{
	return m_pkEditTerrainHelper->GetLayerMapCount();
}
//-------------------------------------------------------------------------------------------------
unsigned int MEditTerrainHelper::GetActiveLayerMap()
{
	return m_pkEditTerrainHelper->GetActiveLayerMap();
}
//-------------------------------------------------------------------------------------------------
void MEditTerrainHelper::SetActiveLayerMap(unsigned int uiIndex)
{
	m_pkEditTerrainHelper->SetActiveLayerMap(uiIndex);
	m_pkEditTerrainHelper->SetCurrentLayerIndex(uiIndex);
}
//-------------------------------------------------------------------------------------------------
bool MEditTerrainHelper::AppendLayerMap(System::String^ strName, System::String^ strDiffuseMapName, System::String^ strNormalMapName, float fUVRepeat, float fMipmapBias)
{
	const char* szName = MStringToCharPointer(strName);
	const char* szDiffuseMapName = MStringToCharPointer(strDiffuseMapName);
	const char* szNormalMapName = MStringToCharPointer(strNormalMapName);
	bool bSuccess =m_pkEditTerrainHelper->AppendLayerMap(szName, szDiffuseMapName, szNormalMapName, fUVRepeat, fMipmapBias);
	MFreeCharPointer(szName);
	MFreeCharPointer(szDiffuseMapName);
	MFreeCharPointer(szNormalMapName);
	return bSuccess;
}
//-------------------------------------------------------------------------------------------------
bool MEditTerrainHelper::RemoveLayerMap(uint uiIndex)
{
	return m_pkEditTerrainHelper->RemoveLayerMap(uiIndex);
}
//-------------------------------------------------------------------------------------------------
bool MEditTerrainHelper::SwapLayerMap(uint uiLIndex, uint uiRIndex)
{
	return m_pkEditTerrainHelper->SwapLayerMap(uiLIndex, uiRIndex);
}
//-------------------------------------------------------------------------------------------------
System::String^ MEditTerrainHelper::GetLayerMapName(unsigned int uiIndex)
{
	const char* szName = m_pkEditTerrainHelper->GetLayerMapName(uiIndex);
	return (szName == 0) ? "" : gcnew System::String(szName);
}
//-------------------------------------------------------------------------------------------------
void MEditTerrainHelper::SetLayerMapName(unsigned int uiIndex, System::String^ strName)
{
	const char* szName = MStringToCharPointer(strName);
	m_pkEditTerrainHelper->SetLayerMapName(uiIndex, szName);
	MFreeCharPointer(szName);
}
//-------------------------------------------------------------------------------------------------
System::String^ MEditTerrainHelper::GetLayerMapDiffuseMap(unsigned int uiIndex)
{
	const char* szName = m_pkEditTerrainHelper->GetLayerMapDiffuseMap(uiIndex);
	return (szName == 0) ? "" : gcnew System::String(szName);
}
//-------------------------------------------------------------------------------------------------
void MEditTerrainHelper::SetLayerMapDiffuseMap(unsigned int uiIndex, System::String^ strName)
{
	const char* szName = MStringToCharPointer(strName);
	m_pkEditTerrainHelper->SetLayerMapDiffuseMap(uiIndex, szName);
	MFreeCharPointer(szName);
}
//-------------------------------------------------------------------------------------------------
System::String^ MEditTerrainHelper::GetLayerMapNormalMap(unsigned int uiIndex)
{
	const char* szName = m_pkEditTerrainHelper->GetLayerMapNormalMap(uiIndex);
	return (szName == 0) ? "" : gcnew System::String(szName);
}
//-------------------------------------------------------------------------------------------------
void MEditTerrainHelper::SetLayerMapNormalMap(unsigned int uiIndex, System::String^ strName)
{
	const char* szName = MStringToCharPointer(strName);
	m_pkEditTerrainHelper->SetLayerMapNormalMap(uiIndex, szName);
	MFreeCharPointer(szName);
}
//-------------------------------------------------------------------------------------------------
float MEditTerrainHelper::GetLayerMapRepeatSize(unsigned int uiIndex)
{
	return m_pkEditTerrainHelper->GetLayerMapRepeatSize(uiIndex);
}
//-------------------------------------------------------------------------------------------------
void MEditTerrainHelper::SetLayerMapRepeatSize(unsigned int uiIndex, float fRepeat)
{
	m_pkEditTerrainHelper->SetLayerMapRepeatSize(uiIndex, fRepeat);
}
//-------------------------------------------------------------------------------------------------
float MEditTerrainHelper::GetLayerMapMipmapBias(unsigned int uiIndex)
{
	return m_pkEditTerrainHelper->GetLayerMapMipmapBias(uiIndex);
}
//-------------------------------------------------------------------------------------------------
void MEditTerrainHelper::SetLayerMapMipmapBias(unsigned int uiIndex, float fBias)
{
	m_pkEditTerrainHelper->SetLayerMapMipmapBias(uiIndex, fBias);
}
//-------------------------------------------------------------------------------------------------
bool MEditTerrainHelper::GetShowTileGrid()
{ 
	return m_pkEditTerrainHelper->GetShowTileGrid();
}
//-------------------------------------------------------------------------------------------------
void MEditTerrainHelper::SetShowTileGrid(bool bShow) 
{ 
	m_pkEditTerrainHelper->SetShowTileGrid(bShow);
}
//-------------------------------------------------------------------------------------------------
bool MEditTerrainHelper::GetShowCurrentLayer()
{ 
	return m_pkEditTerrainHelper->GetShowCurrentLayer();
}
//-------------------------------------------------------------------------------------------------
void MEditTerrainHelper::SetShowCurrentLayer(bool bShow) 
{ 
	m_pkEditTerrainHelper->SetShowCurrentLayer(bShow);
}
//-------------------------------------------------------------------------------------------------
void MEditTerrainHelper::SetCurrentLayerIndex(uint uiIndex)
{ 
	m_pkEditTerrainHelper->SetCurrentLayerIndex(uiIndex);
}
//-------------------------------------------------------------------------------------------------
bool MEditTerrainHelper::GetShowInversibleLayers()
{ 
	return m_pkEditTerrainHelper->GetShowInversibleLayers();
}
//-------------------------------------------------------------------------------------------------
void MEditTerrainHelper::SetShowInversibleLayers(bool bShow) 
{ 
	m_pkEditTerrainHelper->SetShowInversibleLayers(bShow);
}
//-------------------------------------------------------------------------------------------------
MColor^ MEditTerrainHelper::GetAmbientMaterial()
{
	Base::Math::sdVector3 kColor = m_pkEditTerrainHelper->GetAmbientMaterial();
	return gcnew MColor(kColor.m_fX, kColor.m_fY, kColor.m_fZ);
}
//-------------------------------------------------------------------------------------------------
void MEditTerrainHelper::SetAmbientMaterial(MColor^ mColor)
{
	Base::Math::sdVector3 kColor(mColor->R, mColor->G, mColor->B);
	m_pkEditTerrainHelper->SetAmbientMaterial(kColor);
}
//-------------------------------------------------------------------------------------------------
MColor^ MEditTerrainHelper::GetDiffuseMaterial()
{
	Base::Math::sdVector3 kColor = m_pkEditTerrainHelper->GetDiffuseMaterial();
	return gcnew MColor(kColor.m_fX, kColor.m_fY, kColor.m_fZ);
}
//-------------------------------------------------------------------------------------------------
void MEditTerrainHelper::SetDiffuseMaterial(MColor^ mColor)
{
	Base::Math::sdVector3 kColor(mColor->R, mColor->G, mColor->B);
	m_pkEditTerrainHelper->SetDiffuseMaterial(kColor);
}
//-------------------------------------------------------------------------------------------------
MColor^ MEditTerrainHelper::GetSpecularMaterial()
{
	Base::Math::sdVector3 kColor = m_pkEditTerrainHelper->GetSpecularMaterial();
	return gcnew MColor(kColor.m_fX, kColor.m_fY, kColor.m_fZ);
}
//-------------------------------------------------------------------------------------------------
void MEditTerrainHelper::SetSpecularMaterial(MColor^ mColor)
{
	Base::Math::sdVector3 kColor(mColor->R, mColor->G, mColor->B);
	m_pkEditTerrainHelper->SetSpecularMaterial(kColor);
}
//-------------------------------------------------------------------------------------------------
float MEditTerrainHelper::GetShiness()
{
	return m_pkEditTerrainHelper->GetShiness();
}
//-------------------------------------------------------------------------------------------------
void MEditTerrainHelper::SetShiness(float fShiness)
{
	m_pkEditTerrainHelper->SetShiness(fShiness);
}
//-------------------------------------------------------------------------------------------------
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK