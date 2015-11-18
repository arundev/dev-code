//*************************************************************************************************
// ����:	������״̬
//---------------------------------------------------------
// ����:		
// ����:		
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _GAME_LOGIC_GAME_SCENE_SUBSTATE_H__
#define _GAME_LOGIC_GAME_SCENE_SUBSTATE_H__
#include <sdFSM.h>

namespace GameLogic
{
	// ��Ϸ����״̬��״̬����
	class sdSceneState_SubState : public Base::sdState
	{	
	public:
		// ������״̬
		enum eSceneSubState
		{
			E_SCENESUBSTATE_LOADING,
			E_SCENESUBSTATE_GAME,
			E_SCENESUBSTATE_MOVIE,
			E_SCENESUBSTATE_COMA,
			E_SCENESUBSTATE_EDIT,
			E_SCENESUBSTATE_CG,
			E_SCENESUBSTATE_MINIGAME,
			E_SCENESUBSTATE_DEFLOWER,
			NUM_SCENESUBSTATES,
		};

	public:
		sdSceneState_SubState(){};
		virtual ~sdSceneState_SubState(){};

		// �麯���̳�
		virtual void Enter() {};
		virtual	void Leave() {};
		virtual int	 Update() { return 0;};
	};
}