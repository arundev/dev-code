//*************************************************************************************************
// 内容:	游戏状态机
//---------------------------------------------------------
// 作者:		
// 创建:		
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _GAME_LOGIC_GAME_STATE_MACHINE_H__
#define _GAME_LOGIC_GAME_STATE_MACHINE_H__
#include <sdFSM.h>

namespace GameLogic
{
	// 游戏状态机
	class sdGameStateMachine : public Base::sdTSingleton<sdEditFSM>, public Base::sdFSM 
	{
	public:
		sdGameStateMachine(){};
		virtual ~sdGameStateMachine(){};

		// 虚函数继承
		virtual void UpdateState(){};

		// 
		virtual void Render(){};
	};
}