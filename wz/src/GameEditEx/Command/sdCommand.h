//*************************************************************************************************
// 内容:	命令管理器(用于支援回退和重做)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-27
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMEEDITEX_COMMAND_H__
#define _SD_GAMEEDITEX_COMMAND_H__
#include <sdSingleton.h>

namespace GameEditEx
{
	//*************************************************************************
	// 命令接口
	class sdICommand
	{
	public:
		// 编辑器命令类型
		enum EEditType
		{
			NUM_EDIT_MODE,
		};

	public:
		virtual ~sdICommand(){};

		virtual EEditType GetType() = 0;	// 类型
		virtual bool Redo() = 0;			// 重做
		virtual bool Undo() = 0;			// 撤销
	};

	//*************************************************************************
	// 命令管理器
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