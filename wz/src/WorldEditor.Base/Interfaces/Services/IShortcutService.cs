//*************************************************************************************************
// 内容:	(参考自WZEditor.Base)
//---------------------------------------------------------
// 作者:		
// 创建:		2014-06-01
// 最后修改:    
//*************************************************************************************************
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WorldEditor.Base.Interfaces.Services
{
    // 快捷键服务接口
    public interface IShortcutService : IService
    {
        // 注册与反注册快捷键
        void RegisterShortcut(Keys key, UICommands.UICommand command);
        void UnregisterShortkey(Keys key);

        // 查找快捷键
        Keys GetKey(UICommands.UICommand command);

        // 处理快捷键
        bool HandleKey(Keys key);
    }
}
