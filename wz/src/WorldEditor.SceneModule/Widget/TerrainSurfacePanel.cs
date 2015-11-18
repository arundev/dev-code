using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using WorldEditorFramework;
using WorldEditor.SceneModule.Dialog;

namespace WorldEditor.SceneModule.Widget
{
    public partial class TerrainSurfacePanel : UserControl
    {
        // 操作接口
        MEditTerrainHelper mEditTerrainHelper;

        public TerrainSurfacePanel()
        {
            InitializeComponent();
        }

        # region 用于处理操作接口与显示数据
        // @{
        public void OnSceneLoad()
        {
            // 操作接口
            mEditTerrainHelper = new MEditTerrainHelper();
        }

        public void OnSceneClose()
        {
           
        }

        public void OnOpenPanel()
        {
            // 刷新图层列表
            RefreshLayerMapList();

            // 默认选中最顶部
            SetSelectLayerMap(listBoxLayer.Items.Count - 1);
        }

        public void OnClosePanel()
        {

        }
        // @}
        #endregion

        # region 笔刷(消息响应/UI回调函数)
        // @{
        private void trackerBarWidgetOutterRadius_OnValueChanged(object sender, EventArgs e)
        {
            mEditTerrainHelper.SurfaceBrushOutterRadius = (float)trackerBarWidgetOutterRadius.Value;
            if (trackerBarWidgetInnerRadius.Value > trackerBarWidgetOutterRadius.Value)
            {
                mEditTerrainHelper.SurfaceBrushInnerRadius = mEditTerrainHelper.SurfaceBrushOutterRadius;
                trackerBarWidgetInnerRadius.Value = trackerBarWidgetOutterRadius.Value;
            }
        }

        private void trackerBarWidgetInnerRadius_OnValueChanged(object sender, EventArgs e)
        {
            mEditTerrainHelper.SurfaceBrushInnerRadius = (float)trackerBarWidgetInnerRadius.Value;
            if (trackerBarWidgetInnerRadius.Value > trackerBarWidgetOutterRadius.Value)
            {
                mEditTerrainHelper.SurfaceBrushOutterRadius = mEditTerrainHelper.SurfaceBrushInnerRadius;
                trackerBarWidgetOutterRadius.Value = trackerBarWidgetInnerRadius.Value;
            }
        }

        private void trackerBarWidgetBrushPower_OnValueChanged(object sender, EventArgs e)
        {
            mEditTerrainHelper.SurfaceBrushHardness = (float)trackerBarWidgetBrushPower.Value;
        }

        private void listBoxLayer_SelectedIndexChanged(object sender, EventArgs e)
        {
            int iIndex = GetSelectedLayerMap();
            mEditTerrainHelper.SetActiveLayerMap((uint)iIndex);

            // 更新图层信息UI
            BaseMap = mEditTerrainHelper.GetLayerMapDiffuseMap((uint)iIndex);
            NormalMap = mEditTerrainHelper.GetLayerMapNormalMap((uint)iIndex);

            float fRepeatSize = mEditTerrainHelper.GetLayerMapRepeatSize((uint)iIndex);
            trackerBarWidgetUVScale.Value = Convert.ToDecimal(fRepeatSize);

            float fMipmapBias = mEditTerrainHelper.GetLayerMapMipmapBias((uint)iIndex);
            trackerBarWidgetMipmapBias.Value = Convert.ToDecimal(fMipmapBias);

            bool bShowTileGrid = mEditTerrainHelper.GetShowTileGrid();
            checkBoxShowTexTileGrid.Checked = bShowTileGrid;

            bool bShowCurrentlayer = mEditTerrainHelper.GetShowCurrentLayer();
            checkBoxShowCurrentLayer.Checked = bShowCurrentlayer;

            bool bShowInversibleLayers = mEditTerrainHelper.GetShowInversibleLayers();
            checkBoxShowInvisiableLayer.Checked = bShowInversibleLayers;
        }
        // @}
        #endregion

        # region 图层(消息响应/UI回调函数)
        // @{
        private void RefreshLayerMapList()
        {
            listBoxLayer.Items.Clear();
            int iNum = (int)mEditTerrainHelper.GetLayerMapCount();
            for (int i = iNum-1; i >= 0; --i)
            {
                listBoxLayer.Items.Add(mEditTerrainHelper.GetLayerMapName((uint)i));  ///< 倒序显示
            }
        }

        private void SetSelectLayerMap(int iIndex)
        {
            if (iIndex >= 0 && iIndex < listBoxLayer.Items.Count)
            {
                int iSelectedIndex = listBoxLayer.Items.Count - iIndex - 1;
                listBoxLayer.SelectedIndex = iSelectedIndex;
            }
        }

        private int GetSelectedLayerMap()
        {
            int iSelectedIndex = listBoxLayer.SelectedIndex;
            if (iSelectedIndex < 0)
                return iSelectedIndex;
            return listBoxLayer.Items.Count - iSelectedIndex - 1;
        }

        private void buttonLayerAdd_Click(object sender, EventArgs e)
        {
            // 自动产生一个可用的图层名
            String strNewLayerName = null;
            int iTestNum = 0;
            bool bSuccess = true;
            do
            {
                strNewLayerName = String.Format("Layer{0:d}", iTestNum++);

                bSuccess = true;
                uint uiLayerCount = mEditTerrainHelper.GetLayerMapCount();
                for (uint uiLayer = 0; uiLayer < uiLayerCount; ++uiLayer)
                {
                    String strLayerName = mEditTerrainHelper.GetLayerMapName(uiLayer);
                    if (strNewLayerName.Equals(strLayerName))
                    {
                        bSuccess = false;
                        break;
                    }
                }
            }while(!bSuccess);

            // 打开对话框
            NewLayerDlg kNewLayerDlg = new NewLayerDlg();
            kNewLayerDlg.LayerName = strNewLayerName;
            if (kNewLayerDlg.ShowDialog() == DialogResult.OK)
            {
                mEditTerrainHelper.AppendLayerMap(kNewLayerDlg.LayerName, 
                    kNewLayerDlg.BaseMap, kNewLayerDlg.NormalMap, 
                    kNewLayerDlg.UVRepeat, kNewLayerDlg.MipmapBias);

                RefreshLayerMapList();
                if (listBoxLayer.Items.Count > 0)
                    SetSelectLayerMap(listBoxLayer.Items.Count - 1);
            }
        }

        private void buttonLayerRemove_Click(object sender, EventArgs e)
        {
            int iSelectedIndex = GetSelectedLayerMap();
            if (iSelectedIndex < 0)
                return;

            bool bSuccess = mEditTerrainHelper.RemoveLayerMap((uint)iSelectedIndex);
            if (bSuccess)
            {
                // 刷新图层列表
                RefreshLayerMapList();

                // 默认选中第一个
                if (listBoxLayer.Items.Count > 0)
                    SetSelectLayerMap(listBoxLayer.Items.Count - 1);
                else
                    listBoxLayer.SelectedIndex = -1;
            }
        }

        private void buttonLayerUp_Click(object sender, EventArgs e)
        {
            int iIndex = (int)mEditTerrainHelper.GetActiveLayerMap();
            if (iIndex < 0)
                return;

            bool bSuccess = mEditTerrainHelper.SwapLayerMap((uint)iIndex, (uint)(iIndex+1));
            if (bSuccess)
            {
                // 刷新图层列表
                RefreshLayerMapList();

                // 默认选中当前选中的
                mEditTerrainHelper.SetActiveLayerMap((uint)(iIndex + 1));
                SetSelectLayerMap(iIndex + 1);
            }
        }

        private void buttonLayerDown_Click(object sender, EventArgs e)
        {
            int iIndex = (int)mEditTerrainHelper.GetActiveLayerMap();
            if (iIndex < 0)
                return;

            bool bSuccess = mEditTerrainHelper.SwapLayerMap((uint)iIndex, (uint)(iIndex - 1));
            if (bSuccess)
            {
                // 刷新图层列表
                RefreshLayerMapList();

                // 默认选中当前选中的
                mEditTerrainHelper.SetActiveLayerMap((uint)(iIndex - 1));
                SetSelectLayerMap(iIndex - 1);
            }
        }

        private string mBaseMap = null;
        public string BaseMap
        {
            get { return mBaseMap; }
            set 
            {
                mBaseMap = value;

                IntPtr hBitmap = MTextureHelper.LoadImageHBMP(value);
                if (hBitmap == IntPtr.Zero)
                {
                    // ILoggingService

                    pictureBoxDiffuseMap.Image = null;
                }
                else
                {
                    pictureBoxDiffuseMap.Image = Bitmap.FromHbitmap(hBitmap);
                    MTextureHelper.ReleaseImageHBMP(hBitmap);
                }
            }
        }

        private string mNormalMap = null;
        public string NormalMap
        {
            get { return mNormalMap; }
            set
            {
                mNormalMap = value;

                IntPtr hBitmap = MTextureHelper.LoadImageHBMP(value);
                if (hBitmap == IntPtr.Zero)
                {
                    // ILoggingService
                    pictureBoxNormalMap.Image = null;
                }
                else
                {
                    pictureBoxNormalMap.Image = Bitmap.FromHbitmap(hBitmap);
                    MTextureHelper.ReleaseImageHBMP(hBitmap);
                }
            }
        }


        private void pictureBoxDiffuseMap_Click(object sender, EventArgs e)
        {

        }

        private void pictureBoxNormalMap_Click(object sender, EventArgs e)
        {

        }

        private void trackerBarWidgetUVScale_OnValueChanged(object sender, EventArgs e)
        {
            int iSelectedIndex = GetSelectedLayerMap();
            if (iSelectedIndex < 0)
                return;

            float fUVRepeat = (float)Convert.ToDouble(trackerBarWidgetUVScale.Value);
            mEditTerrainHelper.SetLayerMapRepeatSize((uint)iSelectedIndex, fUVRepeat);
        }

        private void trackerBarWidgetMipmapBias_OnValueChanged(object sender, EventArgs e)
        {
            int iSelectedIndex = GetSelectedLayerMap();
            if (iSelectedIndex < 0)
                return;

            float fMipmapBias = (float)Convert.ToDouble(trackerBarWidgetMipmapBias.Value);
            mEditTerrainHelper.SetLayerMapMipmapBias((uint)iSelectedIndex, fMipmapBias);
        }

        private void checkBoxShowTexTileGrid_CheckedChanged(object sender, EventArgs e)
        {
            mEditTerrainHelper.SetShowTileGrid(checkBoxShowTexTileGrid.Checked);
        }

        private void checkBoxShowInvisiableLayer_CheckedChanged(object sender, EventArgs e)
        {
            mEditTerrainHelper.SetShowInversibleLayers(checkBoxShowInvisiableLayer.Checked);
        }

        private void checkBoxShowCurrentLayer_CheckedChanged(object sender, EventArgs e)
        {
            mEditTerrainHelper.SetShowCurrentLayer(checkBoxShowCurrentLayer.Checked);
        }
        // @}
        #endregion
    }
}
