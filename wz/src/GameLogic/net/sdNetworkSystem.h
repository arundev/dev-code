//*************************************************************************************************
// 内容:	网络设备
//---------------------------------------------------------
// 作者:		
// 创建:		
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _GAME_LOGIC_NETWORK_SYSTEM_
#define _GAME_LOGIC_NETWORK_SYSTEM_
#include "sdGameLogic.h"

// Base
#include <sdSingleton.h>

namespace GameLogic
{
	// 网络中心
	class sdNetworkSystem : public Base::sdTSingleton<sdNetworkSystem>
	{
	public:
		sdNetworkSystem();
		virtual ~sdNetworkSystem();


	};

}
#endif