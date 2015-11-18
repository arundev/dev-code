using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using WorldEditorFramework;
using WorldEditor.Common.CustomForm;

namespace WorldEditor.SceneModule.Widget
{
    public partial class LightPanel : UserControl
    {
        // 操作接口
        MEditHelper mEditHelper;

        public LightPanel()
        {
            InitializeComponent();
        }

        #region 用于处理操作接口与显示数据
        // @{
        public void OnSceneLoad()
        {
            // 操作接口
            mEditHelper = new MEditHelper();

            // 重设UI
            MMap map = mEditHelper.Map;
            if (map != null)
            {
                {
                    MLight ambientLight = map.AmbientLight;
                    MColor ambientAmbientColor = ambientLight.AmbientColor;
                    MColor ambientDiffuseColor = ambientLight.DiffuseColor;
                    float ambientDimmer = ambientLight.Dimmer;

                    int iR = (int)(ambientAmbientColor.R * 255.0f);
                    int iG = (int)(ambientAmbientColor.G * 255.0f);
                    int iB = (int)(ambientAmbientColor.B * 255.0f);
                    labelAmbientLightAmbientColor.BackColor = Color.FromArgb(iR, iG, iB);

                    iR = (int)(ambientDiffuseColor.R * 255.0f);
                    iG = (int)(ambientDiffuseColor.G * 255.0f);
                    iB = (int)(ambientDiffuseColor.B * 255.0f);
                    labelAmbientLightDiffuseColor.BackColor = Color.FromArgb(iR, iG, iB);

                    trackerBarWidgetAmbientLightDimmer.Value = Convert.ToDecimal(ambientDimmer);
                }

                {
                    MLight mainLight = map.MainLight;
                    MColor mainAmbientColor = mainLight.AmbientColor;
                    MColor mainDiffuseColor = mainLight.DiffuseColor;
                    float mainDimmer = mainLight.Dimmer;

                    int iR = (int)(mainAmbientColor.R * 255.0f);
                    int iG = (int)(mainAmbientColor.G * 255.0f);
                    int iB = (int)(mainAmbientColor.B * 255.0f);
                    labelMainLightAmbientColor.BackColor = Color.FromArgb(iR, iG, iB);

                    iR = (int)(mainDiffuseColor.R * 255.0f);
                    iG = (int)(mainDiffuseColor.G * 255.0f);
                    iB = (int)(mainDiffuseColor.B * 255.0f);
                    labelMainLightDiffuseColor.BackColor = Color.FromArgb(iR, iG, iB);

                    trackerBarWidgetMainLightDimmer.Value = Convert.ToDecimal(mainDimmer);
                }

                {
                    MLight assistantLight = map.AssistantLight;
                    MColor assistantAmbientColor = assistantLight.AmbientColor;
                    MColor assistantDiffuseColor = assistantLight.DiffuseColor;
                    float assistantDimmer = assistantLight.Dimmer;

                    int iR = (int)(assistantAmbientColor.R * 255.0f);
                    int iG = (int)(assistantAmbientColor.G * 255.0f);
                    int iB = (int)(assistantAmbientColor.B * 255.0f);
                    labelAssistantLightAmbientColor.BackColor = Color.FromArgb(iR, iG, iB);

                    iR = (int)(assistantDiffuseColor.R * 255.0f);
                    iG = (int)(assistantDiffuseColor.G * 255.0f);
                    iB = (int)(assistantDiffuseColor.B * 255.0f);
                    labelAssistantLightDiffuseColor.BackColor = Color.FromArgb(iR, iG, iB);

                    trackerBarWidgetAssistantLightDimmer.Value = Convert.ToDecimal(assistantDimmer);
                }
            }
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
        #endregion

        #region UI回调函数
        // @{
        private void labelAmbientLightAmbientColor_Click(object sender, EventArgs e)
        {
            MMap map = mEditHelper.Map;
            if (map == null)
                return;

            ColorPickForm colorPicker = new ColorPickForm();

            MLight ambientLight = map.AmbientLight;
            MColor oldColor = ambientLight.AmbientColor;
            int iR = (int)(oldColor.R * 255.0);
            int iG = (int)(oldColor.G * 255.0);
            int iB = (int)(oldColor.B * 255.0);
            colorPicker.ColorPicked = Color.FromArgb(iR, iG, iB);
            colorPicker.OnColorChanged += UIAmbientLightAmbientColorNotify;

            if (colorPicker.ShowDialog() == DialogResult.OK)
            {
                float fR = colorPicker.ColorPicked.R / 255.0f;
                float fG = colorPicker.ColorPicked.G / 255.0f;
                float fB = colorPicker.ColorPicked.B / 255.0f;

                MColor mColor = new MColor(fR, fG, fB);
                ambientLight.AmbientColor = mColor;                                ///< 更新灯光环境光颜色
                labelAmbientLightAmbientColor.BackColor = colorPicker.ColorPicked; ///< 更新UI颜色
            }
            else
            {
                ambientLight.AmbientColor = oldColor;                                  ///< 恢复灯光环境光颜色
                labelAmbientLightAmbientColor.BackColor = Color.FromArgb(iR, iG, iB);  ///< 恢复UI颜色
            }
        }

        private void labelAmbientLightDiffuseColor_Click(object sender, EventArgs e)
        {
            MMap map = mEditHelper.Map;
            if (map == null)
                return;

            ColorPickForm colorPicker = new ColorPickForm();

            MLight ambientLight = map.AmbientLight;
            MColor oldColor = ambientLight.DiffuseColor;
            int iR = (int)(oldColor.R * 255.0);
            int iG = (int)(oldColor.G * 255.0);
            int iB = (int)(oldColor.B * 255.0);
            colorPicker.ColorPicked = Color.FromArgb(iR, iG, iB);
            colorPicker.OnColorChanged += UIAmbientLightDiffuseColorNotify;

            if (colorPicker.ShowDialog() == DialogResult.OK)
            {
                float fR = colorPicker.ColorPicked.R / 255.0f;
                float fG = colorPicker.ColorPicked.G / 255.0f;
                float fB = colorPicker.ColorPicked.B / 255.0f;

                MColor mColor = new MColor(fR, fG, fB);
                ambientLight.DiffuseColor = mColor;                                ///< 更新灯光漫射光颜色
                labelAmbientLightDiffuseColor.BackColor = colorPicker.ColorPicked; ///< 更新UI颜色
            }
            else
            {
                ambientLight.DiffuseColor = oldColor;                                  ///< 恢复灯光漫射光颜色
                labelAmbientLightDiffuseColor.BackColor = Color.FromArgb(iR, iG, iB);  ///< 恢复UI颜色
            }
        }

        private void trackerBarWidgetAmbientLightDimmer_OnValueChanged(object sender, EventArgs e)
        {
            MMap map = mEditHelper.Map;
            if (map == null)
                return;

            MLight ambientLight = map.AmbientLight;
            ambientLight.Dimmer = (float)(trackerBarWidgetAmbientLightDimmer.Value);
        }

        private void labelMainLightAmbientColor_Click(object sender, EventArgs e)
        {
            MMap map = mEditHelper.Map;
            if (map == null)
                return;

            ColorPickForm colorPicker = new ColorPickForm();

            MLight mainLight = map.MainLight;
            MColor oldColor = mainLight.AmbientColor;
            int iR = (int)(oldColor.R * 255.0);
            int iG = (int)(oldColor.G * 255.0);
            int iB = (int)(oldColor.B * 255.0);
            colorPicker.ColorPicked = Color.FromArgb(iR, iG, iB);
            colorPicker.OnColorChanged += UIMainLightAmbientColorNotify;

            if (colorPicker.ShowDialog() == DialogResult.OK)
            {
                float fR = colorPicker.ColorPicked.R / 255.0f;
                float fG = colorPicker.ColorPicked.G / 255.0f;
                float fB = colorPicker.ColorPicked.B / 255.0f;

                MColor mColor = new MColor(fR, fG, fB);
                mainLight.AmbientColor = mColor;                                ///< 更新灯光环境光颜色
                labelMainLightAmbientColor.BackColor = colorPicker.ColorPicked; ///< 更新UI颜色
            }
            else
            {
                mainLight.AmbientColor = oldColor;                                  ///< 恢复灯光环境光颜色
                labelMainLightAmbientColor.BackColor = Color.FromArgb(iR, iG, iB);  ///< 恢复UI颜色
            }
        }

        private void labelMainLightDiffuseColor_Click(object sender, EventArgs e)
        {
            MMap map = mEditHelper.Map;
            if (map == null)
                return;

            ColorPickForm colorPicker = new ColorPickForm();

            MLight mainLight = map.MainLight;
            MColor oldColor = mainLight.DiffuseColor;
            int iR = (int)(oldColor.R * 255.0);
            int iG = (int)(oldColor.G * 255.0);
            int iB = (int)(oldColor.B * 255.0);
            colorPicker.ColorPicked = Color.FromArgb(iR, iG, iB);
            colorPicker.OnColorChanged += UIMainLightDiffuseColorNotify;

            if (colorPicker.ShowDialog() == DialogResult.OK)
            {
                float fR = colorPicker.ColorPicked.R / 255.0f;
                float fG = colorPicker.ColorPicked.G / 255.0f;
                float fB = colorPicker.ColorPicked.B / 255.0f;

                MColor mColor = new MColor(fR, fG, fB);
                mainLight.DiffuseColor = mColor;                                ///< 更新主灯光漫反射颜色
                labelMainLightDiffuseColor.BackColor = colorPicker.ColorPicked; ///< 更新UI颜色
            }
            else
            {
                mainLight.DiffuseColor = oldColor;                                  ///< 恢复主灯光漫反射颜色
                labelMainLightDiffuseColor.BackColor = Color.FromArgb(iR, iG, iB);  ///< 恢复UI颜色
            }
        }

        private void trackerBarWidgetMainLightDimmer_OnValueChanged(object sender, EventArgs e)
        {
            MMap map = mEditHelper.Map;
            if (map == null)
                return;

            MLight mainLight = map.MainLight;
            mainLight.Dimmer = (float)(trackerBarWidgetMainLightDimmer.Value);
        }

        private void labelAssistantLightDiffuseColor_Click(object sender, EventArgs e)
        {
            MMap map = mEditHelper.Map;
            if (map == null)
                return;

            ColorPickForm colorPicker = new ColorPickForm();

            MLight assistantLight = map.AssistantLight;
            MColor oldColor = assistantLight.AmbientColor;
            int iR = (int)(oldColor.R * 255.0);
            int iG = (int)(oldColor.G * 255.0);
            int iB = (int)(oldColor.B * 255.0);
            colorPicker.ColorPicked = Color.FromArgb(iR, iG, iB);
            colorPicker.OnColorChanged += UIAssistantLightAmbientColorNotify;

            if (colorPicker.ShowDialog() == DialogResult.OK)
            {
                float fR = colorPicker.ColorPicked.R / 255.0f;
                float fG = colorPicker.ColorPicked.G / 255.0f;
                float fB = colorPicker.ColorPicked.B / 255.0f;

                MColor mColor = new MColor(fR, fG, fB);
                assistantLight.AmbientColor = mColor;                                   ///< 更新灯光环境光颜色
                labelAssistantLightAmbientColor.BackColor = colorPicker.ColorPicked;    ///< 更新UI颜色
            }
            else
            {
                assistantLight.AmbientColor = oldColor;                                 ///< 恢复灯光环境光颜色
                labelAssistantLightAmbientColor.BackColor = Color.FromArgb(iR, iG, iB); ///< 恢复UI颜色
            }
        }

        private void labelAssistantLightAmbientColor_Click(object sender, EventArgs e)
        {
            MMap map = mEditHelper.Map;
            if (map == null)
                return;

            ColorPickForm colorPicker = new ColorPickForm();

            MLight assistantLight = map.AssistantLight;
            MColor oldColor = assistantLight.DiffuseColor;
            int iR = (int)(oldColor.R * 255.0);
            int iG = (int)(oldColor.G * 255.0);
            int iB = (int)(oldColor.B * 255.0);
            colorPicker.ColorPicked = Color.FromArgb(iR, iG, iB);
            colorPicker.OnColorChanged += UIAssistantLightDiffuseColorNotify;

            if (colorPicker.ShowDialog() == DialogResult.OK)
            {
                float fR = colorPicker.ColorPicked.R / 255.0f;
                float fG = colorPicker.ColorPicked.G / 255.0f;
                float fB = colorPicker.ColorPicked.B / 255.0f;

                MColor mColor = new MColor(fR, fG, fB);
                assistantLight.DiffuseColor = mColor;                                ///< 更新灯光漫反射颜色
                labelAssistantLightDiffuseColor.BackColor = colorPicker.ColorPicked; ///< 更新UI颜色
            }
            else
            {
                assistantLight.DiffuseColor = oldColor;                                  ///< 恢复灯光漫反射颜色
                labelAssistantLightDiffuseColor.BackColor = Color.FromArgb(iR, iG, iB);  ///< 恢复UI颜色
            }
        }

        private void trackerBarWidgetAssistantLightDimmer_OnValueChanged(object sender, EventArgs e)
        {
            MMap map = mEditHelper.Map;
            if (map == null)
                return;

            MLight assistantLight = map.AssistantLight;
            assistantLight.Dimmer = (float)(trackerBarWidgetAssistantLightDimmer.Value);
        }

        private void UIAmbientLightAmbientColorNotify(object sender, EventArgs e)
        {
            MMap map = mEditHelper.Map;
            if (map == null)
                return;

            ColorPickForm colorPicker = (ColorPickForm)sender;

            float fR = colorPicker.ColorPicked.R / 255.0f;
            float fG = colorPicker.ColorPicked.G / 255.0f;
            float fB = colorPicker.ColorPicked.B / 255.0f;

            MColor pickedColor = new MColor(fR, fG, fB);
            MLight ambientLight = map.AmbientLight;
            ambientLight.AmbientColor = pickedColor;
        }

        private void UIAmbientLightDiffuseColorNotify(object sender, EventArgs e)
        {
            MMap map = mEditHelper.Map;
            if (map == null)
                return;

            ColorPickForm colorPicker = (ColorPickForm)sender;

            float fR = colorPicker.ColorPicked.R / 255.0f;
            float fG = colorPicker.ColorPicked.G / 255.0f;
            float fB = colorPicker.ColorPicked.B / 255.0f;

            MColor pickedColor = new MColor(fR, fG, fB);
            MLight ambientLight = map.AmbientLight;
            ambientLight.DiffuseColor = pickedColor;
        }

        private void UIMainLightAmbientColorNotify(object sender, EventArgs e)
        {
            MMap map = mEditHelper.Map;
            if (map == null)
                return;

            ColorPickForm colorPicker = (ColorPickForm)sender;

            float fR = colorPicker.ColorPicked.R / 255.0f;
            float fG = colorPicker.ColorPicked.G / 255.0f;
            float fB = colorPicker.ColorPicked.B / 255.0f;

            MColor pickedColor = new MColor(fR, fG, fB);
            MLight mainLight = map.MainLight;
            mainLight.AmbientColor = pickedColor;
        }

        private void UIMainLightDiffuseColorNotify(object sender, EventArgs e)
        {
            MMap map = mEditHelper.Map;
            if (map == null)
                return;

            ColorPickForm colorPicker = (ColorPickForm)sender;

            float fR = colorPicker.ColorPicked.R / 255.0f;
            float fG = colorPicker.ColorPicked.G / 255.0f;
            float fB = colorPicker.ColorPicked.B / 255.0f;

            MColor pickedColor = new MColor(fR, fG, fB);
            MLight mainLight = map.MainLight;
            mainLight.DiffuseColor = pickedColor; 
        }

        private void UIAssistantLightAmbientColorNotify(object sender, EventArgs e)
        {
            MMap map = mEditHelper.Map;
            if (map == null)
                return;

            ColorPickForm colorPicker = (ColorPickForm)sender;

            float fR = colorPicker.ColorPicked.R / 255.0f;
            float fG = colorPicker.ColorPicked.G / 255.0f;
            float fB = colorPicker.ColorPicked.B / 255.0f;

            MColor pickedColor = new MColor(fR, fG, fB);
            MLight assistantLight = map.AssistantLight;
            assistantLight.AmbientColor = pickedColor;
        }

        private void UIAssistantLightDiffuseColorNotify(object sender, EventArgs e)
        {
            MMap map = mEditHelper.Map;
            if (map == null)
                return;

            ColorPickForm colorPicker = (ColorPickForm)sender;

            float fR = colorPicker.ColorPicked.R / 255.0f;
            float fG = colorPicker.ColorPicked.G / 255.0f;
            float fB = colorPicker.ColorPicked.B / 255.0f;

            MColor pickedColor = new MColor(fR, fG, fB);
            MLight assistantLight = map.AssistantLight;
            assistantLight.DiffuseColor = pickedColor;
        }
        // @}
        #endregion
    }
}
