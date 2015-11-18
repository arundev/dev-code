//*************************************************************************************************
// ����:	���������(����֧Ԯ���˺�����)
//---------------------------------------------------------
// ����:		
// ����:		2012-08-27
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMEEDITEX_COMMAND_H__
#define _SD_GAMEEDITEX_COMMAND_H__
#include <sdSingleton.h>

namespace GameEditEx
{
	//*************************************************************************
	// ����ӿ�
	class sdICommand
	{
	public:
		// �༭����������
		enum EEditType
		{
			NUM_EDIT_MODE,
		};

	public:
		virtual ~sdICommand(){};

		virtual EEditType GetType() = 0;	// ����
		virtual bool Redo() = 0;			// ����
		virtual bool Undo() = 0;			// ����
	};

	//*************************************************************************
	// ���������
	class sdCommandMgr : public Base::sdTSingleton<sdCommandMgr>
	{
	public:
		sdCommandMgr();
		~sdCommandMgr();

	protected:
		typedef std::list<sdICommand*> CommandList;
		typedef std::list<sdICommand*>::iterator CommandListItr;
		CommandList m_kRedoList;
		CommandList m_kUndoList;
	};
}
#endif