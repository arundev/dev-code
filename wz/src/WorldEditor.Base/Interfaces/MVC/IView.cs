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
    // 视图接口
    public interface IView
    {
        // 视图所属文档
        IDocument Document
        { 
            get; 
        }

        // 视图适配器
        IViewAdapter ViewAdapter 
        { 
            get; 
            set; 
        }

        // 视图ID
        string ID 
        { 
            get; 
        }

        // 视图名
        string Name 
        { 
            get; 
        }

        void NotifyDocument(IDocument kDoc);
        void UpdateDocument();
    }
}
