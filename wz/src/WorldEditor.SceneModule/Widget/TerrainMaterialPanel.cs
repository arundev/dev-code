using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using WorldEditor.Common.CustomForm;
using WorldEditorFramework;

namespace WorldEditor.SceneModule.Widget
{
    public partial class TerrainMaterialPanel : UserControl
    {
        // 操作接口
        MEditTerrainHelper mEditTerrainHelper;

        public TerrainMaterialPanel()
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
            mEditTerrainHelper = new MEditTerrainHelper();

            // 初始化界面
            MColor ambientColor = mEditTerrainHelper.GetAmbientMaterial();
            MColor diffuseColor = mEditTerrainHelper.GetDiffuseMaterial();
            MColor specularColor = mEditTerrainHelper.GetSpecularMaterial();
            float shiness = mEditTerrainHelper.GetShiness();

            int iR = (int)(ambientColor.R * 255.0f);
            int iG = (int)(ambientColor.G * 255.0f);
            int iB = (int)(ambientColor.B * 255.0f);
            labelTerrainAmbientColor.BackColor = Color.FromArgb(iR, iG, iB);

            iR = (int)(diffuseColor.R * 255.0f);
            iG = (int)(diffuseColor.G * 255.0f);
            iB = (int)(diffuseColor.B * 255.0f);
            labelTerrainDiffuseColor.BackColor = Color.FromArgb(iR, iG, iB);

            iR = (int)(specularColor.R * 255.0f);
            iG = (int)(specularColor.G * 255.0f);
            iB = (int)(specularColor.B * 255.0f);
            labelTerrainSpecularColor.BackColor = Color.FromArgb(iR, iG, iB);

            trackerBarWidgetTerrainShiness.Value = Convert.ToDecimal(shiness);
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
        private void labelTerrainAmbientColor_Click(object sender, EventArgs e)
        {
            ColorPickForm colorPicker = new ColorPickForm();
            
            MColor oldColor = mEditTerrainHelper.GetAmbientMaterial();
            int iR = (int)(oldColor.R * 255.0);
            int iG = (int)(oldColor.G * 255.0);
            int iB = (int)(oldColor.B * 255.0);
            colorPicker.ColorPicked = Color.FromArgb(iR, iG, iB);
            colorPicker.OnColorChanged += UITerrainAmbientColorNotify;

            if (colorPicker.ShowDialog() == DialogResult.OK)
            {
                float fR = colorPicker.ColorPicked.R / 255.0f;
                float fG = colorPicker.ColorPicked.G / 255.0f;
                float fB = colorPicker.ColorPicked.B / 255.0f;

                MColor mColor = new MColor(fR, fG, fB);
                mEditTerrainHelper.SetAmbientMaterial(mColor);                  ///< 更新材质颜色
                labelTerrainAmbientColor.BackColor = colorPicker.ColorPicked;   ///< 更新UI颜色
            }
            else
            {
                mEditTerrainHelper.SetAmbientMaterial(oldColor);                    ///< 恢复材质颜色
                labelTerrainAmbientColor.BackColor = Color.FromArgb(iR, iG, iB);    ///< 恢复UI颜色
            }
        }

        private void labelTerrainDiffuseColor_Click(object sender, EventArgs e)
        {
            ColorPickForm colorPicker = new ColorPickForm();

            MColor oldColor = mEditTerrainHelper.GetDiffuseMaterial();
            int iR = (int)(oldColor.R * 255.0);
            int iG = (int)(oldColor.G * 255.0);
            int iB = (int)(oldColor.B * 255.0);
            colorPicker.ColorPicked = Color.FromArgb(iR, iG, iB);
            colorPicker.OnColorChanged += UITerrainDiffuseColorNotify;

            if (colorPicker.ShowDialog() == DialogResult.OK)
            {
                float fR = colorPicker.ColorPicked.R / 255.0f;
                float fG = colorPicker.ColorPicked.G / 255.0f;
                float fB = colorPicker.ColorPicked.B / 255.0f;

                MColor mColor = new MColor(fR, fG, fB);
                mEditTerrainHelper.SetDiffuseMaterial(mColor);                  ///< 更新材质颜色
                labelTerrainDiffuseColor.BackColor = colorPicker.ColorPicked;   ///< 更新UI颜色
            }
            else
            {
                mEditTerrainHelper.SetDiffuseMaterial(oldColor);                ///< 恢复材质颜色
                labelTerrainDiffuseColor.BackColor = Color.FromArgb(iR, iG, iB);///< 恢复UI颜色
            }
        }

        private void labelTerrainSpecularColor_Click(object sender, EventArgs e)
        {
            ColorPickForm colorPicker = new ColorPickForm();

            MColor oldColor = mEditTerrainHelper.GetSpecularMaterial();
            int iR = (int)(oldColor.R * 255.0);
            int iG = (int)(oldColor.G * 255.0);
            int iB = (int)(oldColor.B * 255.0);
            colorPicker.ColorPicked = Color.FromArgb(iR, iG, iB);
            colorPicker.OnColorChanged += UITerrainSpecularColorNotify;

            if (colorPicker.ShowDialog() == DialogResult.OK)
            {
                float fR = colorPicker.ColorPicked.R / 255.0f;
                float fG = colorPicker.ColorPicked.G / 255.0f;
                float fB = colorPicker.ColorPicked.B / 255.0f;

                MColor mColor = new MColor(fR, fG, fB);
                mEditTerrainHelper.SetSpecularMaterial(mColor);                 ///< 更新材质颜色
                labelTerrainSpecularColor.BackColor = colorPicker.ColorPicked;   ///< 更新UI颜色
            }
            else
            {
                mEditTerrainHelper.SetSpecularMaterial(oldColor);               ///< 恢复材质颜色
                labelTerrainSpecularColor.BackColor = Color.FromArgb(iR, iG, iB);///< 恢复UI颜色
            }
        }

        private void trackerBarWidgetTerrainShiness_Scroll(object sender, ScrollEventArgs e)
        {
            float shinessValue = (float)(trackerBarWidgetTerrainShiness.Value);
            mEditTerrainHelper.SetShiness(shinessValue);
        }

        private void UITerrainAmbientColorNotify(object sender, EventArgs e)
        {
            ColorPickForm colorPicker = (ColorPickForm)sender;

            float fR = colorPicker.ColorPicked.R / 255.0f;
            float fG = colorPicker.ColorPicked.G / 255.0f;
            float fB = colorPicker.ColorPicked.B / 255.0f;

            MColor mColor = new MColor(fR, fG, fB);
            mEditTerrainHelper.SetAmbientMaterial(mColor);
        }

        private void UITerrainDiffuseColorNotify(object sender, EventArgs e)
        {
            ColorPickForm colorPicker = (ColorPickForm)sender;

            float fR = colorPicker.ColorPicked.R / 255.0f;
            float fG = colorPicker.ColorPicked.G / 255.0f;
            float fB = colorPicker.ColorPicked.B / 255.0f;

            MColor mColor = new MColor(fR, fG, fB);
            mEditTerrainHelper.SetDiffuseMaterial(mColor);
        }

        private void UITerrainSpecularColorNotify(object sender, EventArgs e)
        {
            ColorPickForm colorPicker = (ColorPickForm)sender;

            float fR = colorPicker.ColorPicked.R / 255.0f;
            float fG = colorPicker.ColorPicked.G / 255.0f;
            float fB = colorPicker.ColorPicked.B / 255.0f;

            MColor mColor = new MColor(fR, fG, fB);
            mEditTerrainHelper.SetSpecularMaterial(mColor);
        }
        // @}
    }
}
