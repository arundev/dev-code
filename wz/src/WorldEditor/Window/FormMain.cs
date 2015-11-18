using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices; 
using WeifenLuo.WinFormsUI.Docking;
using WorldEditor.SceneModule;

namespace WorldEditor.Window
{
    public partial class FormMain : Form
    {
        // 场景编辑器模块
        SceneModule.Module.SceneModule mSceneModule;

        // 输出窗口
        //OutputForm mOutputForm;

        public FormMain()
        {
            InitializeComponent();

            //mOutputForm = new OutputForm();   ///< 输出窗口

            try
            {
                mSceneModule = SceneModule.Module.SceneModule.Instance;
            }
            catch (System.Exception ex)
            {
                System.Windows.Forms.MessageBox.Show(ex.ToString());
            }  
        }

        // 系统更新回调函数
        public void FormMain_Idle(object sender, EventArgs e)
        {
            bool bIdle = true;
            do
            {
                if (this.Visible && FormWindowState.Minimized != this.WindowState)
                {
                    // 窗口可见,且非最小化
                    //
                    // 更新操作接口
                    mSceneModule.Update();
                }

                // 如果消息队列为空,则循环
                NativeMethod.Message msg;
                bIdle = !NativeMethod.PeekMessage(out msg, IntPtr.Zero, 0, 0, 0);

                // 休眠
                System.Threading.Thread.Sleep(1);
            }
            while (bIdle);
        }

        // @{
        //*****************************************************************************************
        // 消息回调函数
        //*****************************************************************************************
        private void FormMain_Load(object sender, EventArgs e)
        {
            // 向应用程序注册更新回调函数
            //  
            // 实现自动Update有两种方式:
            //  1.Timer里面Update,但是Timer是一个独立的线程执行,可能引起问题
            //  2.OnIdle回调,但是OnIdle只会在每次执行完消息之后调用,
            //      所以需要一个Timer不停的发送消息,或者OnIdle给自己PeekMessage(异步)
            Application.Idle += new EventHandler(this.FormMain_Idle);

            // 向应用程序注册消息过滤器(用于捕捉输入消息)
            Application.AddMessageFilter(new InputMessageFilter(this.mSceneModule));

            // 附着输出框
            //mOutputForm.Show(dockPanel, DockState.DockBottom);

            // 初始化场景模块
            mSceneModule.Initialize(this.dockPanel);
        }

        private void FormMain_FormClosing(object sender, FormClosingEventArgs e)
        {
            // 销毁场景模块
            mSceneModule.Destroy(this.dockPanel);
        }
        //*****************************************************************************************
        // @}


        // @{
        //*****************************************************************************************
        // 菜单响应
        //*****************************************************************************************
        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
 
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (mSceneModule != null)
            {
                mSceneModule.WorldEditor.SaveScene();
            }
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (mSceneModule != null)
            {
                mSceneModule.WorldEditor.OpenScene();
            }
        }
        //*****************************************************************************************
        // @}
    }
}
