#include "sdEnginePCH.h"
#include "sdTextureLodManager.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
//-------------------------------------------------------------------------------------------------
sdTextureLodManager::sdTextureLodManager()
: m_bInitialized(false)
, m_bEnable(true)
, m_cBaseSkipLevel(0)
, m_cDynamicLevel(0)
{
}
//-------------------------------------------------------------------------------------------------
sdTextureLodManager::~sdTextureLodManager()
{

}
//-------------------------------------------------------------------------------------------------
bool sdTextureLodManager::Initialize()
{
	//m_auiLevelArray[0] = 260 * 1024 * 1024;
	//m_auiLevelArray[1] = 330 * 1024 * 1024;
	//m_auiLevelArray[2] = 365 * 1024 * 1024;
	//m_auiLevelArray[3] = 380 * 1024 * 1024;

	m_auiLevelArray[0] = 470 * 1024 * 1024;
	m_auiLevelArray[1] = 570 * 1024 * 1024;
	m_auiLevelArray[2] = 620 * 1024 * 1024;
	m_auiLevelArray[3] = 650 * 1024 * 1024;

	return (m_bInitialized = true);
}
//-------------------------------------------------------------------------------------------------
void sdTextureLodManager::Destroy()
{
	m_bInitialized = false;
}
//-------------------------------------------------------------------------------------------------
void sdTextureLodManager::Update()
{
	//sdPF_Func_Auto;

	ITextureManager* pkTextureManager = ITextureManager::GetTextureManager();
	if (pkTextureManager && m_bEnable)
	{
		char cTempLevel = 0;
		int iDxt1, iDxt3, iDxt5, iSynTexSize;
		int iD3dTexSize = pkTextureManager->GetTextureSize(iDxt1, iDxt3, iDxt5, iSynTexSize);
		for (uint ui = 0; ui < TEX_LOD_LEVEL; ++ui)
		{
			if (iD3dTexSize + iSynTexSize > (int)m_auiLevelArray[ui])
				++cTempLevel;
			else
				break;
		}

		m_cDynamicLevel = cTempLevel;
	}
	else
	{
		m_cDynamicLevel = 0;
	}
}
//-------------------------------------------------------------------------------------------------
bool sdTextureLodManager::IsInitialized()
{
	return m_bInitialized;
}
//-------------------------------------------------------------------------------------------------
uchar sdTextureLodManager::GetBaseSkipLevel()
{
	return m_cBaseSkipLevel;
}
//-------------------------------------------------------------------------------------------------
void sdTextureLodManager::SetBaseSkipLevel(char cLevel)
{
	m_cBaseSkipLevel = cLevel;

	// 重新加载所有纹理
	ITextureManager* pkTextureManager = ITextureManager::GetTextureManager();
	if (pkTextureManager)
		pkTextureManager->ReloadCache(m_cBaseSkipLevel);
}
//-------------------------------------------------------------------------------------------------
uchar sdTextureLodManager::GetSkipLevel()
{
	return m_cBaseSkipLevel + m_cDynamicLevel;
}
//-------------------------------------------------------------------------------------------------
void sdTextureLodManager::SetEnableDynamicLevel(bool bEnable)
{
	m_bEnable = bEnable;
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_ENGINE