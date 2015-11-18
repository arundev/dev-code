//*************************************************************************************************
// 内容:	(参考自WZEditor.Base)
//---------------------------------------------------------
// 作者:		
// 创建:		2014-05-29
// 最后修改:    2014-05-29
//*************************************************************************************************
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WorldEditor.Base.Interfaces.MVC
{
    public interface IShortCutView : IView
    {
        // 是否能够剪切
        bool CanCut 
        { 
            get;
        }

        // 是否能够拷贝
        bool CanCopy 
        { 
            get; 
        }

        // 是否能够粘贴
        bool CanPaste 
        { 
            get;
        }

        // 是否能够撤销
        bool CanUndo 
        { 
            get; 
        }

        // 是否能够重做
        bool CanRedo 
        { 
            get; 
        }

        // 剪切
        bool Cut();

        // 拷贝
        bool Copy();

        // 粘贴
        bool Paste();

        // 撤销
        bool Undo();

        // 重做
        bool Redo();
    }
}
