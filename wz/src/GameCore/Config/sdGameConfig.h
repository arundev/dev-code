//*************************************************************************************************
// ����:	��Ϸ���ù���
//---------------------------------------------------------
// ����:		
// ����:		2012-07-09
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMECORE_GAME_CONFIG_H__
#define _SD_GAMECORE_GAME_CONFIG_H__
#include <sdSingleton.h>
#include "sdVideoConfig.h"

SD_GAMECORE_NAMESPACE_BEGIN_GAMECORE

// ��ȡ��Ϸ����ini,��ȡ��ǰ��������Ϣ
class sdGameConfig : public Base::sdTSingleton<sdGameConfig>
{
public:



public:
	sdVideoConfig	m_kVideoConfig;

	
};

SD_GAMECORE_NAMESPACE_END_GAMECORE
#endif