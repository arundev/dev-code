//*************************************************************************************************
// ����:	����״̬
//---------------------------------------------------------
// ����:		
// ����:		
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _GAME_LOGIC_GAME_SCENE_STATE_H__
#define _GAME_LOGIC_GAME_SCENE_STATE_H__
#include <sdGameState.h>

namespace GameLogic
{
	// ��Ϸ����״̬
	class sdSceneState : public Base::sdFSM, public sdGameState
	{
	public:
		sdSceneState(){};
		virtual ~sdSceneState(){};
	};
}