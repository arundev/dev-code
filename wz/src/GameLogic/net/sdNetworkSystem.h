//*************************************************************************************************
// ����:	�����豸
//---------------------------------------------------------
// ����:		
// ����:		
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _GAME_LOGIC_NETWORK_SYSTEM_
#define _GAME_LOGIC_NETWORK_SYSTEM_
#include "sdGameLogic.h"

// Base
#include <sdSingleton.h>

namespace GameLogic
{
	// ��������
	class sdNetworkSystem : public Base::sdTSingleton<sdNetworkSystem>
	{
	public:
		sdNetworkSystem();
		virtual ~sdNetworkSystem();


	};

}
#endif