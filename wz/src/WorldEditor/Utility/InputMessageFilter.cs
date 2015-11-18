using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using WorldEditorFramework;

namespace WorldEditor
{
    //*****************************************************************************************
    // 应用程序输入消息过滤器
    //
    // C# 中消息的工作流程(中间还有一些次要的流程PreviewKeyDown之类的)：
    //  1.C#中的消息被Application类从应用程序消息队列中取出
    //  2.Application类的IMessageFilter进行预过滤,然后分发到消息对应的窗体
    //      Application.FilterMessage
    //      IMessageFilter
    //  2.窗体对象的第一个响应函数是对象中的 
    //      protected override void WndProc(ref System.Windows.Forms.Message e)
    //      protected override bool ProcessCmdKey(ref Message msg, Keys keyData,Control control)
    //  3.它再根据消息的类型调用默认的消息响应函数,如
    //      protected override void OnMouseDown(MouseEventArgs e)
    //  4.默认的响应函数然后根据对象的事件字段(如this.MouseDown )中的函数指针列表，
    //    调用用户所加入的响应函数(如Form1_MouseDown1和Form1_MouseDown2)，
    //    而且调用顺序和用户添加顺序一致。 
    //
    //
    // Win32消息:
    //  1.PostMessage执行后马上返回
    //  2.SendMessage必须等到消息被处理后才会返回
    //  3.PostThreadMessage
    //  4.GetMessage
    //  5.PeekMessage
    //
    //
    // 输入
    //  1.Win32输入
    //  2.C#输入事件/输入消息
    //
    //*****************************************************************************************
    class InputMessageFilter : IMessageFilter
    {
        // 场景编辑器模块
        SceneModule.Module.SceneModule mSceneModule;

        public InputMessageFilter(SceneModule.Module.SceneModule sceneModule)
        {
            mSceneModule = sceneModule;
        }

        public bool PreFilterMessage(ref Message m)
        {
            // 消息拦截
            // mSceneModule.WndProc(ref m);

            // 处理热键

            return false;
        }
    }
}
