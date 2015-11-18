using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using WorldEditor.SceneModule.Widget;

namespace WorldEditor.SceneModule.Window
{
    public partial class SettingForm : WeifenLuo.WinFormsUI.Docking.DockContent
    {
        // 子面板
        private SettingPanel mSettingPanel;

        public SettingForm()
        {
            InitializeComponent();

            mSettingPanel = new SettingPanel();
            mSettingPanel.Dock = DockStyle.Fill;
            mSettingPanel.Visible = false;
            this.Controls.Add(mSettingPanel);
        }

        // @{
        //*********************************************************************
        // 面板,用于处理操作接口与显示数据
        //*********************************************************************
        // 初始化
        public void OnSceneLoad()
        {
            mSettingPanel.Visible = true;
            mSettingPanel.OnSceneLoad();
        }
        // @}
    }
}
