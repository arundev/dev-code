//*************************************************************************************************
// 内容:	场景状态
//---------------------------------------------------------
// 作者:		
// 创建:		
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _GAME_LOGIC_GAME_SCENE_STATE_H__
#define _GAME_LOGIC_GAME_SCENE_STATE_H__
#include <sdGameState.h>

namespace GameLogic
{
	// 游戏场景状态
	class sdSceneState : public Base::sdFSM, public sdGameState
	{
	public:
		sdSceneState(){};
		virtual ~sdSceneState(){};
	};
}