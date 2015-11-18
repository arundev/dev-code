using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using WorldEditor.Common.Common;
using WorldEditor.Common.CustomWidget;

namespace WorldEditor.Common.CustomForm
{
    // 调色板
    public partial class ColorPickForm : Form
    {
        // 调色板颜色改变事件
        public event EventHandler OnColorChanged;

        // 显示方式
        public enum EDrawStyle
        {
            Hue,
            Saturation,
            Luminance,
            Red,
            Green,
            Blue,
        };

        // 当前选中的颜色
        private ColorSpaceConverter.HSL mColorHSL;
        private ColorSpaceConverter.CMYK mColorCMYK;
        private Color mColorPicked;
        public Color ColorPicked
        {
            get { return mColorPicked; }
            set
            {
                mColorPicked = value;
                mColorHSL = Common.ColorSpaceConverter.RGB2HSL(mColorPicked);
                mColorCMYK = Common.ColorSpaceConverter.RGB2CMYK(mColorPicked);

                colorPaletteUserControl.RGB = mColorPicked;         ///< 更新主控件
                colorVerticalSliderUserControl.RGB = mColorPicked;  ///< 更新辅助控件 

                UpdateTextBoxes();  ///< 更新文本框
            }
        }

        // 当前调色板样式
        public EDrawStyle DrawStyle
        {
            get
            {
                if (radioButtonHSL_H.Checked) 
                    return EDrawStyle.Hue;
                else if (radioButtonHSL_S.Checked) 
                    return EDrawStyle.Saturation;
                else if (radioButtonHSL_L.Checked) 
                    return EDrawStyle.Luminance;
                else if (radioButtonRGB_R.Checked) 
                    return EDrawStyle.Red;
                else if (radioButtonRGB_G.Checked) 
                    return EDrawStyle.Green;
                else if (radioButtonRGB_B.Checked) 
                    return EDrawStyle.Blue;
                else 
                    return EDrawStyle.Hue;
            }
            set
            {
                switch (value)
                {
                    case EDrawStyle.Hue :
                        radioButtonHSL_H.Checked = true;
                        break;
                    case EDrawStyle.Saturation:
                        radioButtonHSL_S.Checked = true;
                        break;
                    case EDrawStyle.Luminance:
                        radioButtonHSL_L.Checked = true;
                        break;
                    case EDrawStyle.Red:
                        radioButtonRGB_R.Checked = true;
                        break;
                    case EDrawStyle.Green:
                        radioButtonRGB_G.Checked = true;
                        break;
                    case EDrawStyle.Blue:
                        radioButtonRGB_B.Checked = true;
                        break;
                }
            }
        }

        public ColorPickForm()
        {
            InitializeComponent();

            mColorPicked = new Color();
            mColorHSL = new ColorSpaceConverter.HSL();
            mColorCMYK = new ColorSpaceConverter.CMYK();

            radioButtonHSL_H.Checked = true;    ///< 默认模式

            // 注册事件响应
            colorPaletteUserControl.Scroll += new System.EventHandler(this.colorPaletteUserControl_Scroll);
            colorVerticalSliderUserControl.Scroll += new System.EventHandler(this.colorVerticalSliderUserControl_Scroll);
        

            // 设置黑色为默认颜色
            ColorPicked = Color.FromArgb(0, 0, 0);
        }

        #region UI Callback Function
        //*********************************************************************
        // 消息响应/UI回调函数
        //*********************************************************************

        #region 
        private void colorPaletteUserControl_Scroll(object sender, System.EventArgs e)
        {
            // 从主颜色控件获取当前颜色
            mColorPicked = colorPaletteUserControl.RGB;
            mColorHSL = colorPaletteUserControl.HSL;
            mColorCMYK = Common.ColorSpaceConverter.RGB2CMYK(colorPaletteUserControl.RGB);
            
            // 更新主界面
            UpdateTextBoxes();

            // 用HSL值更新辅助控件(数值精度比较高)
            colorVerticalSliderUserControl.HSL = mColorHSL;

            // 发送改变消息
            if (OnColorChanged != null)
                OnColorChanged(this, e);
        }

        private void colorVerticalSliderUserControl_Scroll(object sender, System.EventArgs e)
        {
            // 从辅助颜色控件获取当前颜色
            mColorPicked = colorVerticalSliderUserControl.RGB;
            mColorHSL =colorVerticalSliderUserControl.HSL;
            mColorCMYK = Common.ColorSpaceConverter.RGB2CMYK(colorVerticalSliderUserControl.RGB);

            // 更新界面
            UpdateTextBoxes();

            // 用HSL值更新主控件(数值精度比较高)
            colorPaletteUserControl.HSL = mColorHSL;

            // 发送改变消息
            if (OnColorChanged != null)
                OnColorChanged(this, e);
        }
        #endregion

        #region Radio Button
        private void radioButtonHSL_H_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButtonHSL_H.Checked)
            {
                colorPaletteUserControl.DrawStyle = ColorPaletteUserControl.EDrawStyle.Hue;
                colorVerticalSliderUserControl.DrawStyle = ColorVerticalSliderUserControl.EDrawStyle.Hue;
            }
        }

        private void radioButtonHSL_S_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButtonHSL_S.Checked)
            {
                colorPaletteUserControl.DrawStyle = ColorPaletteUserControl.EDrawStyle.Saturation;
                colorVerticalSliderUserControl.DrawStyle = ColorVerticalSliderUserControl.EDrawStyle.Saturation;
            }
        }

        private void radioButtonHSL_L_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButtonHSL_L.Checked)
            {
                colorPaletteUserControl.DrawStyle = ColorPaletteUserControl.EDrawStyle.Luminance;
                colorVerticalSliderUserControl.DrawStyle = ColorVerticalSliderUserControl.EDrawStyle.Luminance;
            }
        }

        private void radioButtonRGB_R_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButtonRGB_R.Checked)
            {
                colorPaletteUserControl.DrawStyle = ColorPaletteUserControl.EDrawStyle.Red;
                colorVerticalSliderUserControl.DrawStyle = ColorVerticalSliderUserControl.EDrawStyle.Red;
            }
        }

        private void radioButtonRGB_G_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButtonRGB_G.Checked)
            {
                colorPaletteUserControl.DrawStyle = ColorPaletteUserControl.EDrawStyle.Green;
                colorVerticalSliderUserControl.DrawStyle = ColorVerticalSliderUserControl.EDrawStyle.Green;
            }
        }

        private void radioButtonRGB_B_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButtonRGB_B.Checked)
            {
                colorPaletteUserControl.DrawStyle = ColorPaletteUserControl.EDrawStyle.Blue;
                colorVerticalSliderUserControl.DrawStyle = ColorVerticalSliderUserControl.EDrawStyle.Blue;
            }
        }
        #endregion

        #region Text Box
        private void textBoxRGB_R_Leave(object sender, EventArgs e)
        {
            string colorText = textBoxRGB_R.Text;

            // 非法字符检测
            bool hasIllegalChar = false;
            if (colorText.Length <= 0)
            {
                hasIllegalChar = true;
            }
            else
            {
                foreach (char letter in colorText)
                {
                    if (!char.IsNumber(letter))
                    {
                        hasIllegalChar = true;
                        break;
                    }
                }
            }

            // 计算新的颜色值
            if (hasIllegalChar)
            {
                MessageBox.Show("Red must be a number value between 0 and 255!");
                UpdateTextBoxes();  ///< 更新文本框
            }
            else
            {
                int colorValue = int.Parse(colorText);
                if (colorValue < 0)
                {
                    MessageBox.Show("Red must be a number value between 0 and 255, Closese value inserted!");
                    textBoxRGB_R.Text = "0";
                    mColorPicked = Color.FromArgb(0, mColorPicked.G, mColorPicked.B);
                }
                else if (colorValue > 255)
                {
                    MessageBox.Show("Red must be a number value between 0 and 255, Closese value inserted!");
                    textBoxRGB_R.Text = "255";
                    mColorPicked = Color.FromArgb(255, mColorPicked.G, mColorPicked.B);
                }
                else
                {
                    mColorPicked = Color.FromArgb(colorValue, mColorPicked.G, mColorPicked.B);
                }

                mColorHSL = Common.ColorSpaceConverter.RGB2HSL(mColorPicked);
                mColorCMYK = Common.ColorSpaceConverter.RGB2CMYK(mColorPicked);

                colorPaletteUserControl.RGB = mColorPicked;         ///< 用RGB值更新主控件
                colorVerticalSliderUserControl.RGB = mColorPicked;  ///< 用RGB值更新辅助控件 

                UpdateTextBoxes();  ///< 更新文本框
            }   
        }

        private void textBoxRGB_G_Leave(object sender, EventArgs e)
        {
            string colorText = textBoxRGB_G.Text;

            // 非法字符检测
            bool hasIllegalChar = false;
            if (colorText.Length <= 0)
            {
                hasIllegalChar = true;
            }
            else
            {
                foreach (char letter in colorText)
                {
                    if (!char.IsNumber(letter))
                    {
                        hasIllegalChar = true;
                        break;
                    }
                }
            }

            // 计算新的颜色值
            if (hasIllegalChar)
            {
                MessageBox.Show("Green must be a number value between 0 and 255!");
                UpdateTextBoxes();  ///< 更新文本框
            }
            else
            {
                int colorValue = int.Parse(colorText);
                if (colorValue < 0)
                {
                    MessageBox.Show("Green must be a number value between 0 and 255, Closese value inserted!");
                    textBoxRGB_G.Text = "0";
                    mColorPicked = Color.FromArgb(mColorPicked.R, 0, mColorPicked.B);
                }
                else if (colorValue > 255)
                {
                    MessageBox.Show("Green must be a number value between 0 and 255, Closese value inserted!");
                    textBoxRGB_G.Text = "255";
                    mColorPicked = Color.FromArgb(mColorPicked.R, 255, mColorPicked.B);
                }
                else
                {
                    mColorPicked = Color.FromArgb(mColorPicked.R, colorValue, mColorPicked.B);
                }

                mColorHSL = Common.ColorSpaceConverter.RGB2HSL(mColorPicked);
                mColorCMYK = Common.ColorSpaceConverter.RGB2CMYK(mColorPicked);

                colorPaletteUserControl.RGB = mColorPicked;         ///< 用RGB值更新主控件
                colorVerticalSliderUserControl.RGB = mColorPicked;  ///< 用RGB值更新辅助控件 

                UpdateTextBoxes();  ///< 更新文本框
            }   
        }

        private void textBoxRGB_B_Leave(object sender, EventArgs e)
        {
            string colorText = textBoxRGB_B.Text;

            // 非法字符检测
            bool hasIllegalChar = false;
            if (colorText.Length <= 0)
            {
                hasIllegalChar = true;
            }
            else
            {
                foreach (char letter in colorText)
                {
                    if (!char.IsNumber(letter))
                    {
                        hasIllegalChar = true;
                        break;
                    }
                }
            }

            // 计算新的颜色值
            if (hasIllegalChar)
            {
                MessageBox.Show("Red must be a number value between 0 and 255!");
                UpdateTextBoxes();  ///< 更新文本框
            }
            else
            {
                int colorValue = int.Parse(colorText);
                if (colorValue < 0)
                {
                    MessageBox.Show("Red must be a number value between 0 and 255, Closese value inserted!");
                    textBoxRGB_B.Text = "0";
                    mColorPicked = Color.FromArgb(mColorPicked.R, mColorPicked.G, 0);
                }
                else if (colorValue > 255)
                {
                    MessageBox.Show("Red must be a number value between 0 and 255, Closese value inserted!");
                    textBoxRGB_B.Text = "255";
                    mColorPicked = Color.FromArgb(mColorPicked.R, mColorPicked.G, 255);
                }
                else
                {
                    mColorPicked = Color.FromArgb(mColorPicked.R, mColorPicked.G, colorValue);
                }

                mColorHSL = Common.ColorSpaceConverter.RGB2HSL(mColorPicked);
                mColorCMYK = Common.ColorSpaceConverter.RGB2CMYK(mColorPicked);

                colorPaletteUserControl.RGB = mColorPicked;         ///< 用RGB值更新主控件
                colorVerticalSliderUserControl.RGB = mColorPicked;  ///< 用RGB值更新辅助控件 

                UpdateTextBoxes();  ///< 更新文本框
            }   
        }
        #endregion

        #region Button
        private void buttonOK_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        private void buttonCancel_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }
        #endregion

        #endregion

        private void UpdateTextBoxes()
        {
            // 更新界面
            labelColor.BackColor = mColorPicked;

            textBoxRGB_R.Text = mColorPicked.R.ToString();
            textBoxRGB_G.Text = mColorPicked.G.ToString();
            textBoxRGB_B.Text = mColorPicked.B.ToString();

            textBoxHSL_H.Text = Round(mColorHSL.H * 360).ToString();
            textBoxHSL_S.Text = Round(mColorHSL.S * 100).ToString();
            textBoxHSL_L.Text = Round(mColorHSL.L * 100).ToString();

            textBoxCMYK_C.Text = Round(mColorCMYK.C * 100).ToString();
            textBoxCMYK_M.Text = Round(mColorCMYK.M * 100).ToString();
            textBoxCMYK_Y.Text = Round(mColorCMYK.Y * 100).ToString();
            textBoxCMYK_K.Text = Round(mColorCMYK.K * 100).ToString();

            textBoxHex.Text = Common.ColorSpaceConverter.RGB2HexString(mColorPicked);

            labelColor.Update();
            textBoxRGB_R.Update();
            textBoxRGB_G.Update();
            textBoxRGB_B.Update();
            textBoxHSL_H.Update();
            textBoxHSL_S.Update();
            textBoxHSL_L.Update();
            textBoxCMYK_C.Update();
            textBoxCMYK_M.Update();
            textBoxCMYK_Y.Update();
            textBoxCMYK_K.Update();
            textBoxHex.Update();
        }

        // 四舍五入取整
        private static int Round(double val)
        {
            int ret = (int)val;
            int tmp = (int)(val * 100);
            if ((tmp % 100) >= 50)
                ret += 1;

            return ret;
        }
    }
}
