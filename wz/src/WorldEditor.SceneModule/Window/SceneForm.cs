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
    public partial class SceneForm : WeifenLuo.WinFormsUI.Docking.DockContent
    {
        // 子面板
        private LightPanel mLightSettingPanel;


        public SceneForm()
        {
            InitializeComponent();

            // 灯光设置面板
            mLightSettingPanel = new LightPanel();
            mLightSettingPanel.Dock = DockStyle.Fill;
            mLightSettingPanel.Location = new Point(5, 13);
            mLightSettingPanel.Visible = false;
            this.groupBoxProperty.Controls.Add(mLightSettingPanel);
        }

        // 初始化
        public void Initialize()
        {

        }

        // @{
        //*********************************************************************
        // 面板,用于处理操作接口与显示数据
        //*********************************************************************
        // 初始化
        public void OnSceneLoad()
        {
            mLightSettingPanel.Visible = true;
            mLightSettingPanel.OnSceneLoad();
        }
        // @}


        #region 控件消息响应
        // @{
        private void buttonLight_Click(object sender, EventArgs e)
        {
            this.groupBoxProperty.Size = new Size(8, 100) + mLightSettingPanel.Size;
        }

        private void buttonSkyDome_Click(object sender, EventArgs e)
        {

        }
        // @}
        #endregion
    }
}
