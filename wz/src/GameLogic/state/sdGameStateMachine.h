//*************************************************************************************************
// ����:	��Ϸ״̬��
//---------------------------------------------------------
// ����:		
// ����:		
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _GAME_LOGIC_GAME_STATE_MACHINE_H__
#define _GAME_LOGIC_GAME_STATE_MACHINE_H__
#include <sdFSM.h>

namespace GameLogic
{
	// ��Ϸ״̬��
	class sdGameStateMachine : public Base::sdTSingleton<sdEditFSM>, public Base::sdFSM 
	{
	public:
		sdGameStateMachine(){};
		virtual ~sdGameStateMachine(){};

		// �麯���̳�
		virtual void UpdateState(){};

		// 
		virtual void Render(){};
	};
}