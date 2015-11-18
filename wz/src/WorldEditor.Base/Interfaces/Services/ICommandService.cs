//*************************************************************************************************
// 内容:	(参考自WZEditor.Base)
//---------------------------------------------------------
// 作者:		
// 创建:		2014-05-29
// 最后修改:    2014-06-01
//*************************************************************************************************
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WorldEditor.Base.Interfaces.Services
{
    // 回退与重做事件通知
    public delegate void UndoRedoHandler(ICommand[] akCommands);

    // 编辑命令
    public interface ICommand
    {
        String Name
        {
            get;
        }

        void DoCommand(bool bInBatch, bool bUndoable);
        void UndoCommand(bool bInBatch);
    }

    // 编辑命令服务接口,用于支持回退与重做
    interface ICommandService : IService
    {
        event UndoRedoHandler CommandsAddedToUndoStack;
        event UndoRedoHandler CommandsRemoveFromUndoStack;
        event UndoRedoHandler CommandsAddedToRedoStack;
        event UndoRedoHandler CommandsRemoveFromRedoStack;

        uint UndoStackCount
        {
            get;
        }

        uint RedoStackCount
        {
            get;
        }

        void ExecuteCommand(ICommand kCommand, bool bAddToUndoStack);
        void UndoCommands(uint uiNumToUndo);
        void RedoCommands(uint uiNumToRedo);

//		bool BeginUndoFrame(String kName);
//		bool EdnUndoFrame(bool bAddToUndoStack);

//		ICommand[] GetUndoCommands();
//		ICommand[] GetRedoCommands();
    }
}
