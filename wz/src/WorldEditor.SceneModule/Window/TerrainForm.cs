using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using WorldEditorFramework;
using WorldEditor.SceneModule.Widget;
using WorldEditor.SceneModule.Module;

namespace WorldEditor.SceneModule.Window
{
    public partial class TerrainForm : WeifenLuo.WinFormsUI.Docking.DockContent
    {
        // 操作接口
        MWorldEditorFSM mWorldEditorFSM;

        // 子面板
        private TerrainDeformPanel mTerrainDeformPanel;
        private TerrainSurfacePanel mTerrainSurfacePanel;
        private TerrainMaterialPanel mTerrainMaterialPanel;

        public TerrainForm()
        {
            InitializeComponent();

            // 地表高度面板
            mTerrainDeformPanel = new TerrainDeformPanel();
            //mTerrainDeformPanel.Dock = DockStyle.Fill;
            mTerrainDeformPanel.Location = new Point(5, 13);
            mTerrainDeformPanel.Visible = false;
            this.groupBoxProperty.Controls.Add(mTerrainDeformPanel);

            // 地表纹理面板
            mTerrainSurfacePanel = new TerrainSurfacePanel();
            //mTerrainSurfacePanel.Dock = DockStyle.Fill;
            mTerrainSurfacePanel.Location = new Point(5, 13);
            mTerrainSurfacePanel.Visible = false;
            this.groupBoxProperty.Controls.Add(mTerrainSurfacePanel);

            // 地表材质面板
            mTerrainMaterialPanel = new TerrainMaterialPanel();
            //mTerrainMaterialPanel.Dock = DockStyle.Fill;
            mTerrainMaterialPanel.Location = new Point(5, 13);
            mTerrainMaterialPanel.Visible = false;
            this.groupBoxProperty.Controls.Add(mTerrainMaterialPanel);
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
            // 操作接口
            mWorldEditorFSM = new MWorldEditorFSM();

            // 
            mTerrainSurfacePanel.OnSceneLoad();
            mTerrainDeformPanel.OnSceneLoad();
            mTerrainMaterialPanel.OnSceneLoad();
        }
        // @}

        // @{
        //*********************************************************************
        // 窗口消息响应
        //*********************************************************************
        private void TerrainForm_Enter(object sender, EventArgs e)
        {

        }

        private void TerrainForm_Leave(object sender, EventArgs e)
        {

        }
        // @}

        // @{
        //*********************************************************************
        // 控件消息响应
        //*********************************************************************
        private void buttonTerrainDeform_Click(object sender, EventArgs e)
        {
            mTerrainSurfacePanel.Visible = false;
            mTerrainSurfacePanel.OnClosePanel();

            mTerrainDeformPanel.Visible = true;
            mTerrainDeformPanel.OnOpenPanel();

            mTerrainMaterialPanel.Visible = false;
            mTerrainMaterialPanel.OnClosePanel();

            this.groupBoxProperty.Size = new Size(8, 100) + mTerrainDeformPanel.Size;

            mWorldEditorFSM.SetEditState(6);
        }

        private void buttonTerrainSurface_Click(object sender, EventArgs e)
        {
            mTerrainDeformPanel.Visible = false;
            mTerrainDeformPanel.OnClosePanel();

            mTerrainSurfacePanel.Visible = true;
            mTerrainSurfacePanel.OnOpenPanel();

            mTerrainMaterialPanel.Visible = false;
            mTerrainMaterialPanel.OnClosePanel();

            this.groupBoxProperty.Size = new Size(8, 100) + mTerrainSurfacePanel.Size;

            mWorldEditorFSM.SetEditState(7);
        }

        private void buttonTerrainMaterial_Click(object sender, EventArgs e)
        {
            mTerrainDeformPanel.Visible = false;
            mTerrainDeformPanel.OnClosePanel();

            mTerrainSurfacePanel.Visible = false;
            mTerrainSurfacePanel.OnClosePanel();

            mTerrainMaterialPanel.Visible = true;
            mTerrainMaterialPanel.OnOpenPanel();

            this.groupBoxProperty.Size = new Size(8, 100) + mTerrainMaterialPanel.Size;
        }
        // @}
    }
}
