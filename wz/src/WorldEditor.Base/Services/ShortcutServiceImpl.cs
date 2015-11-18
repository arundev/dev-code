//*************************************************************************************************
// 内容:	事件(参考自WZEditor.Base)
//---------------------------------------------------------
// 作者:		
// 创建:		2014-06-01
// 最后修改:    
//*************************************************************************************************
using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WorldEditor.Base.Services
{
    // 快捷键服务实现
    class ShortcutServiceImpl : StandardServiceBaseImpl, Interfaces.Services.IShortcutService
    {
        // 快捷键绑定查询表
        private Hashtable mKeyBindings;

        public ShortcutServiceImpl()
        {
            mKeyBindings = new Hashtable();
        }

        public void RegisterShortcut(Keys key, UICommands.UICommand command)
        {
            if (command != null && key != Keys.None)
            {
                mKeyBindings[key] = command;
            }
        }

        public void UnregisterShortkey(Keys key)
        {
            mKeyBindings.Remove(key);
        }

        public Keys GetKey(UICommands.UICommand command)
        {
            foreach (DictionaryEntry bindings in mKeyBindings)
            {
                if (bindings.Value == command)
                    return (Keys)bindings.Key;
            }

            return Keys.None;
        }

        public bool HandleKey(Keys key)
        {
            if (mKeyBindings.Contains(key))
            {
                UICommands.UICommand command = (UICommands.UICommand)mKeyBindings[key];
                Debug.Assert(command != null, "Null Command!");


            }

            return false;
        }
    }
}
