#include "sdGameCorePCH.h"
#include "sdVideoConfig.h"

SD_GAMECORE_NAMESPACE_BEGIN_GAMECORE
//-------------------------------------------------------------------------------------------------
sdVideoConfig::sdVideoConfig()
{
	m_uiWidth = 800;
	m_uiHeight = 600;
	m_bFullScreen = false;
	m_bVSync = false;

	m_uiMipmapSkipLevel = 0;
}
//-------------------------------------------------------------------------------------------------
sdVideoConfig::~sdVideoConfig()
{
	
}
//-------------------------------------------------------------------------------------------------
SD_GAMECORE_NAMESPACE_END_GAMECORE