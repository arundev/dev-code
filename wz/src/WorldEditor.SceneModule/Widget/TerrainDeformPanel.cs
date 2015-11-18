using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using WorldEditorFramework;

namespace WorldEditor.SceneModule.Widget
{
    public partial class TerrainDeformPanel : UserControl
    {
        // 操作接口
        private MEditTerrainHelper mEditTerrainHelper;

        public TerrainDeformPanel()
        {
            InitializeComponent();
        }

        // @{
        //*********************************************************************
        // 面板,用于处理操作接口与显示数据
        //*********************************************************************
        public void OnSceneLoad()
        {
            mEditTerrainHelper = new MEditTerrainHelper();
        }

        public void OnSceneClose()
        {

        }

        public void OnOpenPanel()
        {

        }

        public void OnClosePanel()
        {

        }
        // @}

        # region 笔刷(消息响应/UI回调函数)
        // @{
        private void trackerBarWidgetOutterRadius_OnValueChanged(object sender, EventArgs e)
        {
            mEditTerrainHelper.DeformBrushOutterRadius = (float)trackerBarWidgetOutterRadius.Value;
            if (trackerBarWidgetInnerRadius.Value > trackerBarWidgetOutterRadius.Value)
            {
                mEditTerrainHelper.DeformBrushInnerRadius = mEditTerrainHelper.DeformBrushOutterRadius;
                trackerBarWidgetInnerRadius.Value = trackerBarWidgetOutterRadius.Value;
            }
        }

        private void trackerBarWidgetInnerRadius_OnValueChanged(object sender, EventArgs e)
        {
            mEditTerrainHelper.DeformBrushInnerRadius = (float)trackerBarWidgetInnerRadius.Value;
            if (trackerBarWidgetInnerRadius.Value > trackerBarWidgetOutterRadius.Value)
            {
                mEditTerrainHelper.DeformBrushOutterRadius = mEditTerrainHelper.DeformBrushInnerRadius;
                trackerBarWidgetOutterRadius.Value = trackerBarWidgetInnerRadius.Value;
            }
        }

        private void trackerBarWidgetBrushPower_OnValueChanged(object sender, EventArgs e)
        {
            mEditTerrainHelper.DeformBrushHardness = (float)trackerBarWidgetBrushPower.Value;
        }

        private void trackerBarWidgetHeight_OnValueChanged(object sender, EventArgs e)
        {
            mEditTerrainHelper.DeformBrushHeight = (float)trackerBarWidgetHeight.Value;
        }

        private void toolBarBrushType_ButtonClick(object sender, ToolBarButtonClickEventArgs e)
        {
            // 设置Button按下状态
            foreach (ToolBarButton button in toolBarBrushType.Buttons)
            {
                button.Pushed = (button == e.Button);
            }

            // 检测按钮按下状态,处理之
            if (toolBarButtonPull.Pushed)
            {
                mEditTerrainHelper.DeformBrushType = 1;
            }
            else if (toolBarButtonSmooth.Pushed)
            {
                mEditTerrainHelper.DeformBrushType = 2;
            }
            else if (toolBarButtonNoise.Pushed)
            {
                mEditTerrainHelper.DeformBrushType = 3;
            }
            else if (toolBarButtonAverage.Pushed)
            {
                mEditTerrainHelper.DeformBrushType = 4;
            }
            else if (toolBarButtonFlat.Pushed)
            {
                mEditTerrainHelper.DeformBrushType = 5;
            }

            // 更新UI Panel
            trackerBarWidgetOutterRadius.Value = Convert.ToDecimal(mEditTerrainHelper.DeformBrushOutterRadius);
            trackerBarWidgetInnerRadius.Value = Convert.ToDecimal(mEditTerrainHelper.DeformBrushInnerRadius);
            trackerBarWidgetBrushPower.Value = Convert.ToDecimal(mEditTerrainHelper.DeformBrushHardness);
            trackerBarWidgetHeight.Value = Convert.ToDecimal(mEditTerrainHelper.DeformBrushHeight);
        }
        // @}
        #endregion
    }
}
