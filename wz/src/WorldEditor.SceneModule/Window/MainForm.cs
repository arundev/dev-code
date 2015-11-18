using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using WorldEditor.SceneModule.Module;
using WorldEditor.Common;
using WorldEditor.Common.Common;
using WorldEditorFramework;

namespace WorldEditor.SceneModule.Window
{
    public partial class MainForm : WeifenLuo.WinFormsUI.Docking.DockContent
    {
        // FormView对下层引擎的View接口
        private MSceneRenderView mSceneRenderView;   

        public MainForm()
        {
            InitializeComponent();

            mSceneRenderView = new MSceneRenderView(this.Handle, MRenderViewType.E_SCENE_VIEW);
        }

        // @{
        //*****************************************************************************************
        // 窗口消息重载,主要是输入
        //*****************************************************************************************
        protected override void WndProc(ref System.Windows.Forms.Message e)
        {
            // 鼠标在主窗口单击时激活主窗口
            // (貌似是DockPanelSuite导致窗口不能获取焦点)
            if (e.Msg == (int)(Win32.WinMsg.WM_LBUTTONDOWN) || e.Msg == (int)(Win32.WinMsg.WM_RBUTTONDOWN))
            {
                this.Activate();
            }

            if (mSceneRenderView != null)
                mSceneRenderView.WndProc(ref e);
            
            base.WndProc(ref e);
        }
        // @}
    }
}
