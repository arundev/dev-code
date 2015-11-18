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
    public partial class SettingPanel : UserControl
    {
        // 操作接口
        MEditHelper mEditHelper;

        public SettingPanel()
        {
            InitializeComponent();
        }

        // @{
        //*********************************************************************
        // 面板,用于处理操作接口与显示数据
        //*********************************************************************
        public void OnSceneLoad()
        {
            // 操作接口
            mEditHelper = new MEditHelper();

            // 重设UI
            checkBoxGlowMap.Checked = true;
            checkBoxDiffuseMap.Checked = true;
            checkBoxSpecularMap.Checked = true;
            checkBoxLightMap.Checked = true;
            checkBoxNormalMap.Checked = true;
            checkBoxDetailNormalMap.Checked = true;
            checkBoxAOMap.Checked = true;
            checkBoxEnvironmentMap.Checked = true;

            checkBoxMLAA.Checked = mEditHelper.MLAA;
            checkBoxColorGrading.Checked = mEditHelper.ColorGrading;
            checkBoxDof.Checked = mEditHelper.Dof;
            checkBoxGodRay.Checked = mEditHelper.GodRay;
            checkBoxHDR.Checked = mEditHelper.HDR;
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


        // @{
        //*********************************************************************
        // 消息响应/UI回调函数
        //*********************************************************************
        //
        // 贴图通道
        private void checkBoxGlowMap_CheckedChanged(object sender, EventArgs e)
        {
            mEditHelper.SetEnableMapChannel(5, checkBoxGlowMap.Checked);
        }

        private void checkBoxDiffuseMap_CheckedChanged(object sender, EventArgs e)
        {
            mEditHelper.SetEnableMapChannel(0, checkBoxDiffuseMap.Checked);
        }

        private void checkBoxSpecularMap_CheckedChanged(object sender, EventArgs e)
        {
            mEditHelper.SetEnableMapChannel(2, checkBoxSpecularMap.Checked);
        }

        private void checkBoxLightMap_CheckedChanged(object sender, EventArgs e)
        {
            mEditHelper.SetEnableMapChannel(3, checkBoxLightMap.Checked);
        }

        private void checkBoxNormal_CheckedChanged(object sender, EventArgs e)
        {
            mEditHelper.SetEnableMapChannel(1, checkBoxNormalMap.Checked);
        }

        private void checkBoxDetailNormalMap_CheckedChanged(object sender, EventArgs e)
        {
            mEditHelper.SetEnableMapChannel(6, checkBoxDetailNormalMap.Checked);
        }

        private void checkBoxAOMap_CheckedChanged(object sender, EventArgs e)
        {
            mEditHelper.SetEnableMapChannel(4, checkBoxAOMap.Checked);
        }

        private void checkBoxEnvironmentMap_CheckedChanged(object sender, EventArgs e)
        {
            mEditHelper.SetEnableMapChannel(6, checkBoxEnvironmentMap.Checked);
        }

        // 后期特效
        private void checkBoxMLAA_CheckedChanged(object sender, EventArgs e)
        {
            mEditHelper.MLAA = checkBoxMLAA.Checked;
        }

        private void checkBoxColorGrading_CheckedChanged(object sender, EventArgs e)
        {
            mEditHelper.ColorGrading = checkBoxColorGrading.Checked;
        }

        private void checkBoxDof_CheckedChanged(object sender, EventArgs e)
        {
            mEditHelper.Dof = checkBoxDof.Checked;
        }

        private void checkBoxGodRay_CheckedChanged(object sender, EventArgs e)
        {
            mEditHelper.GodRay = checkBoxGodRay.Checked;
        }

        private void checkBoxHDR_CheckedChanged(object sender, EventArgs e)
        {
            mEditHelper.HDR = checkBoxHDR.Checked;
        }


    }
}
