//*************************************************************************************************
// 内容:	游戏状态基类
//---------------------------------------------------------
// 作者:		
// 创建:		
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _GAME_LOGIC_GAME_STATE_H__
#define _GAME_LOGIC_GAME_STATE_H__
#include <sdFSM.h>

namespace GameLogic
{
	// 游戏状态基类
	class sdGameState : public Base::sdState
	{
	public:
		// 游戏状态
		enum eGameState
		{
			E_GAMESTATE_LOGO,
			E_GAMESTATE_LOGIN,
			E_GAMESTATE_SERVER_LIST,
			E_GAMESTATE_CHARACTER,
			E_GAMESTATE_CREATE_CHARACTER,
			E_GAMESTATE_SCENE,
			NUM_GAMESTATES,
		};

	public:
		sdGameState(){};
		virtual sdGameState(){};

		// 虚函数继承
		virtual void Enter() {};
		virtual	void Leave() {};
		virtual int	 Update() { return 0;};


//		virtual void UIEventHandler(){};
	};
}