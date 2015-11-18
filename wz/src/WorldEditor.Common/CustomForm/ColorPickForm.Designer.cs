namespace WorldEditor.Common.CustomForm
{
    partial class ColorPickForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            WorldEditor.Common.Common.ColorSpaceConverter.HSL hsl1 = new WorldEditor.Common.Common.ColorSpaceConverter.HSL();
            WorldEditor.Common.Common.ColorSpaceConverter.HSL hsl2 = new WorldEditor.Common.Common.ColorSpaceConverter.HSL();
            this.labelColor = new System.Windows.Forms.Label();
            this.buttonOK = new System.Windows.Forms.Button();
            this.buttonCancel = new System.Windows.Forms.Button();
            this.radioButtonHSL_H = new System.Windows.Forms.RadioButton();
            this.radioButtonHSL_S = new System.Windows.Forms.RadioButton();
            this.radioButtonHSL_L = new System.Windows.Forms.RadioButton();
            this.textBoxHSL_H = new System.Windows.Forms.TextBox();
            this.textBoxHSL_S = new System.Windows.Forms.TextBox();
            this.textBoxHSL_L = new System.Windows.Forms.TextBox();
            this.textBoxRGB_B = new System.Windows.Forms.TextBox();
            this.textBoxRGB_G = new System.Windows.Forms.TextBox();
            this.textBoxRGB_R = new System.Windows.Forms.TextBox();
            this.radioButtonRGB_B = new System.Windows.Forms.RadioButton();
            this.radioButtonRGB_G = new System.Windows.Forms.RadioButton();
            this.radioButtonRGB_R = new System.Windows.Forms.RadioButton();
            this.label = new System.Windows.Forms.Label();
            this.textBoxHex = new System.Windows.Forms.TextBox();
            this.textBoxLab_b = new System.Windows.Forms.TextBox();
            this.textBoxLab_a = new System.Windows.Forms.TextBox();
            this.textBoxLab_L = new System.Windows.Forms.TextBox();
            this.radioButtonLab_b = new System.Windows.Forms.RadioButton();
            this.radioButtonLab_a = new System.Windows.Forms.RadioButton();
            this.radioButtonLab_L = new System.Windows.Forms.RadioButton();
            this.textBoxCMYK_Y = new System.Windows.Forms.TextBox();
            this.textBoxCMYK_M = new System.Windows.Forms.TextBox();
            this.textBoxCMYK_C = new System.Windows.Forms.TextBox();
            this.radioButtonCMYK_Y = new System.Windows.Forms.RadioButton();
            this.radioButtonCMYK_M = new System.Windows.Forms.RadioButton();
            this.radioButtonCMYK_C = new System.Windows.Forms.RadioButton();
            this.textBoxCMYK_K = new System.Windows.Forms.TextBox();
            this.radioButtonCMYK_K = new System.Windows.Forms.RadioButton();
            this.colorVerticalSliderUserControl = new WorldEditor.Common.CustomWidget.ColorVerticalSliderUserControl();
            this.colorPaletteUserControl = new WorldEditor.Common.CustomWidget.ColorPaletteUserControl();
            this.SuspendLayout();
            // 
            // labelColor
            // 
            this.labelColor.BackColor = System.Drawing.SystemColors.ControlText;
            this.labelColor.Location = new System.Drawing.Point(374, 13);
            this.labelColor.Name = "labelColor";
            this.labelColor.Size = new System.Drawing.Size(72, 72);
            this.labelColor.TabIndex = 2;
            // 
            // buttonOK
            // 
            this.buttonOK.Location = new System.Drawing.Point(511, 12);
            this.buttonOK.Name = "buttonOK";
            this.buttonOK.Size = new System.Drawing.Size(75, 23);
            this.buttonOK.TabIndex = 3;
            this.buttonOK.Text = "确定";
            this.buttonOK.UseVisualStyleBackColor = true;
            this.buttonOK.Click += new System.EventHandler(this.buttonOK_Click);
            // 
            // buttonCancel
            // 
            this.buttonCancel.Location = new System.Drawing.Point(511, 42);
            this.buttonCancel.Name = "buttonCancel";
            this.buttonCancel.Size = new System.Drawing.Size(75, 23);
            this.buttonCancel.TabIndex = 4;
            this.buttonCancel.Text = "取消";
            this.buttonCancel.UseVisualStyleBackColor = true;
            this.buttonCancel.Click += new System.EventHandler(this.buttonCancel_Click);
            // 
            // radioButtonHSL_H
            // 
            this.radioButtonHSL_H.AutoSize = true;
            this.radioButtonHSL_H.Location = new System.Drawing.Point(386, 125);
            this.radioButtonHSL_H.Name = "radioButtonHSL_H";
            this.radioButtonHSL_H.Size = new System.Drawing.Size(35, 16);
            this.radioButtonHSL_H.TabIndex = 5;
            this.radioButtonHSL_H.Text = "H:";
            this.radioButtonHSL_H.UseVisualStyleBackColor = true;
            this.radioButtonHSL_H.CheckedChanged += new System.EventHandler(this.radioButtonHSL_H_CheckedChanged);
            // 
            // radioButtonHSL_S
            // 
            this.radioButtonHSL_S.AutoSize = true;
            this.radioButtonHSL_S.Location = new System.Drawing.Point(386, 152);
            this.radioButtonHSL_S.Name = "radioButtonHSL_S";
            this.radioButtonHSL_S.Size = new System.Drawing.Size(35, 16);
            this.radioButtonHSL_S.TabIndex = 6;
            this.radioButtonHSL_S.Text = "S:";
            this.radioButtonHSL_S.UseVisualStyleBackColor = true;
            this.radioButtonHSL_S.CheckedChanged += new System.EventHandler(this.radioButtonHSL_S_CheckedChanged);
            // 
            // radioButtonHSL_L
            // 
            this.radioButtonHSL_L.AutoSize = true;
            this.radioButtonHSL_L.Location = new System.Drawing.Point(386, 179);
            this.radioButtonHSL_L.Name = "radioButtonHSL_L";
            this.radioButtonHSL_L.Size = new System.Drawing.Size(35, 16);
            this.radioButtonHSL_L.TabIndex = 7;
            this.radioButtonHSL_L.Text = "L:";
            this.radioButtonHSL_L.UseVisualStyleBackColor = true;
            this.radioButtonHSL_L.CheckedChanged += new System.EventHandler(this.radioButtonHSL_L_CheckedChanged);
            // 
            // textBoxHSL_H
            // 
            this.textBoxHSL_H.Enabled = false;
            this.textBoxHSL_H.Location = new System.Drawing.Point(418, 122);
            this.textBoxHSL_H.Name = "textBoxHSL_H";
            this.textBoxHSL_H.Size = new System.Drawing.Size(52, 21);
            this.textBoxHSL_H.TabIndex = 8;
            // 
            // textBoxHSL_S
            // 
            this.textBoxHSL_S.Enabled = false;
            this.textBoxHSL_S.Location = new System.Drawing.Point(418, 149);
            this.textBoxHSL_S.Name = "textBoxHSL_S";
            this.textBoxHSL_S.Size = new System.Drawing.Size(52, 21);
            this.textBoxHSL_S.TabIndex = 9;
            // 
            // textBoxHSL_L
            // 
            this.textBoxHSL_L.Enabled = false;
            this.textBoxHSL_L.Location = new System.Drawing.Point(418, 176);
            this.textBoxHSL_L.Name = "textBoxHSL_L";
            this.textBoxHSL_L.Size = new System.Drawing.Size(52, 21);
            this.textBoxHSL_L.TabIndex = 10;
            // 
            // textBoxRGB_B
            // 
            this.textBoxRGB_B.Location = new System.Drawing.Point(418, 264);
            this.textBoxRGB_B.Name = "textBoxRGB_B";
            this.textBoxRGB_B.Size = new System.Drawing.Size(52, 21);
            this.textBoxRGB_B.TabIndex = 16;
            this.textBoxRGB_B.Leave += new System.EventHandler(this.textBoxRGB_B_Leave);
            // 
            // textBoxRGB_G
            // 
            this.textBoxRGB_G.Location = new System.Drawing.Point(418, 237);
            this.textBoxRGB_G.Name = "textBoxRGB_G";
            this.textBoxRGB_G.Size = new System.Drawing.Size(52, 21);
            this.textBoxRGB_G.TabIndex = 15;
            this.textBoxRGB_G.Leave += new System.EventHandler(this.textBoxRGB_G_Leave);
            // 
            // textBoxRGB_R
            // 
            this.textBoxRGB_R.Location = new System.Drawing.Point(418, 210);
            this.textBoxRGB_R.Name = "textBoxRGB_R";
            this.textBoxRGB_R.Size = new System.Drawing.Size(52, 21);
            this.textBoxRGB_R.TabIndex = 14;
            this.textBoxRGB_R.Leave += new System.EventHandler(this.textBoxRGB_R_Leave);
            // 
            // radioButtonRGB_B
            // 
            this.radioButtonRGB_B.AutoSize = true;
            this.radioButtonRGB_B.Location = new System.Drawing.Point(386, 267);
            this.radioButtonRGB_B.Name = "radioButtonRGB_B";
            this.radioButtonRGB_B.Size = new System.Drawing.Size(35, 16);
            this.radioButtonRGB_B.TabIndex = 13;
            this.radioButtonRGB_B.Text = "B:";
            this.radioButtonRGB_B.UseVisualStyleBackColor = true;
            this.radioButtonRGB_B.CheckedChanged += new System.EventHandler(this.radioButtonRGB_B_CheckedChanged);
            // 
            // radioButtonRGB_G
            // 
            this.radioButtonRGB_G.AutoSize = true;
            this.radioButtonRGB_G.Location = new System.Drawing.Point(386, 240);
            this.radioButtonRGB_G.Name = "radioButtonRGB_G";
            this.radioButtonRGB_G.Size = new System.Drawing.Size(35, 16);
            this.radioButtonRGB_G.TabIndex = 12;
            this.radioButtonRGB_G.Text = "G:";
            this.radioButtonRGB_G.UseVisualStyleBackColor = true;
            this.radioButtonRGB_G.CheckedChanged += new System.EventHandler(this.radioButtonRGB_G_CheckedChanged);
            // 
            // radioButtonRGB_R
            // 
            this.radioButtonRGB_R.AutoSize = true;
            this.radioButtonRGB_R.Location = new System.Drawing.Point(386, 213);
            this.radioButtonRGB_R.Name = "radioButtonRGB_R";
            this.radioButtonRGB_R.Size = new System.Drawing.Size(35, 16);
            this.radioButtonRGB_R.TabIndex = 11;
            this.radioButtonRGB_R.Text = "R:";
            this.radioButtonRGB_R.UseVisualStyleBackColor = true;
            this.radioButtonRGB_R.CheckedChanged += new System.EventHandler(this.radioButtonRGB_R_CheckedChanged);
            // 
            // label
            // 
            this.label.AutoSize = true;
            this.label.Location = new System.Drawing.Point(386, 298);
            this.label.Name = "label";
            this.label.Size = new System.Drawing.Size(11, 12);
            this.label.TabIndex = 17;
            this.label.Text = "#";
            // 
            // textBoxHex
            // 
            this.textBoxHex.Location = new System.Drawing.Point(403, 291);
            this.textBoxHex.Name = "textBoxHex";
            this.textBoxHex.Size = new System.Drawing.Size(67, 21);
            this.textBoxHex.TabIndex = 18;
            // 
            // textBoxLab_b
            // 
            this.textBoxLab_b.Enabled = false;
            this.textBoxLab_b.Location = new System.Drawing.Point(522, 176);
            this.textBoxLab_b.Name = "textBoxLab_b";
            this.textBoxLab_b.Size = new System.Drawing.Size(52, 21);
            this.textBoxLab_b.TabIndex = 24;
            // 
            // textBoxLab_a
            // 
            this.textBoxLab_a.Enabled = false;
            this.textBoxLab_a.Location = new System.Drawing.Point(522, 149);
            this.textBoxLab_a.Name = "textBoxLab_a";
            this.textBoxLab_a.Size = new System.Drawing.Size(52, 21);
            this.textBoxLab_a.TabIndex = 23;
            // 
            // textBoxLab_L
            // 
            this.textBoxLab_L.Enabled = false;
            this.textBoxLab_L.Location = new System.Drawing.Point(522, 122);
            this.textBoxLab_L.Name = "textBoxLab_L";
            this.textBoxLab_L.Size = new System.Drawing.Size(52, 21);
            this.textBoxLab_L.TabIndex = 22;
            // 
            // radioButtonLab_b
            // 
            this.radioButtonLab_b.AutoSize = true;
            this.radioButtonLab_b.Enabled = false;
            this.radioButtonLab_b.Location = new System.Drawing.Point(490, 179);
            this.radioButtonLab_b.Name = "radioButtonLab_b";
            this.radioButtonLab_b.Size = new System.Drawing.Size(35, 16);
            this.radioButtonLab_b.TabIndex = 21;
            this.radioButtonLab_b.Text = "b:";
            this.radioButtonLab_b.UseVisualStyleBackColor = true;
            // 
            // radioButtonLab_a
            // 
            this.radioButtonLab_a.AutoSize = true;
            this.radioButtonLab_a.Enabled = false;
            this.radioButtonLab_a.Location = new System.Drawing.Point(490, 152);
            this.radioButtonLab_a.Name = "radioButtonLab_a";
            this.radioButtonLab_a.Size = new System.Drawing.Size(35, 16);
            this.radioButtonLab_a.TabIndex = 20;
            this.radioButtonLab_a.Text = "a:";
            this.radioButtonLab_a.UseVisualStyleBackColor = true;
            // 
            // radioButtonLab_L
            // 
            this.radioButtonLab_L.AutoSize = true;
            this.radioButtonLab_L.Enabled = false;
            this.radioButtonLab_L.Location = new System.Drawing.Point(490, 125);
            this.radioButtonLab_L.Name = "radioButtonLab_L";
            this.radioButtonLab_L.Size = new System.Drawing.Size(35, 16);
            this.radioButtonLab_L.TabIndex = 19;
            this.radioButtonLab_L.Text = "L:";
            this.radioButtonLab_L.UseVisualStyleBackColor = true;
            // 
            // textBoxCMYK_Y
            // 
            this.textBoxCMYK_Y.Enabled = false;
            this.textBoxCMYK_Y.Location = new System.Drawing.Point(522, 262);
            this.textBoxCMYK_Y.Name = "textBoxCMYK_Y";
            this.textBoxCMYK_Y.Size = new System.Drawing.Size(52, 21);
            this.textBoxCMYK_Y.TabIndex = 30;
            // 
            // textBoxCMYK_M
            // 
            this.textBoxCMYK_M.Enabled = false;
            this.textBoxCMYK_M.Location = new System.Drawing.Point(522, 235);
            this.textBoxCMYK_M.Name = "textBoxCMYK_M";
            this.textBoxCMYK_M.Size = new System.Drawing.Size(52, 21);
            this.textBoxCMYK_M.TabIndex = 29;
            // 
            // textBoxCMYK_C
            // 
            this.textBoxCMYK_C.Enabled = false;
            this.textBoxCMYK_C.Location = new System.Drawing.Point(522, 208);
            this.textBoxCMYK_C.Name = "textBoxCMYK_C";
            this.textBoxCMYK_C.Size = new System.Drawing.Size(52, 21);
            this.textBoxCMYK_C.TabIndex = 28;
            // 
            // radioButtonCMYK_Y
            // 
            this.radioButtonCMYK_Y.AutoSize = true;
            this.radioButtonCMYK_Y.Enabled = false;
            this.radioButtonCMYK_Y.Location = new System.Drawing.Point(490, 265);
            this.radioButtonCMYK_Y.Name = "radioButtonCMYK_Y";
            this.radioButtonCMYK_Y.Size = new System.Drawing.Size(35, 16);
            this.radioButtonCMYK_Y.TabIndex = 27;
            this.radioButtonCMYK_Y.Text = "Y:";
            this.radioButtonCMYK_Y.UseVisualStyleBackColor = true;
            // 
            // radioButtonCMYK_M
            // 
            this.radioButtonCMYK_M.AutoSize = true;
            this.radioButtonCMYK_M.Enabled = false;
            this.radioButtonCMYK_M.Location = new System.Drawing.Point(490, 238);
            this.radioButtonCMYK_M.Name = "radioButtonCMYK_M";
            this.radioButtonCMYK_M.Size = new System.Drawing.Size(35, 16);
            this.radioButtonCMYK_M.TabIndex = 26;
            this.radioButtonCMYK_M.Text = "M:";
            this.radioButtonCMYK_M.UseVisualStyleBackColor = true;
            // 
            // radioButtonCMYK_C
            // 
            this.radioButtonCMYK_C.AutoSize = true;
            this.radioButtonCMYK_C.Enabled = false;
            this.radioButtonCMYK_C.Location = new System.Drawing.Point(490, 211);
            this.radioButtonCMYK_C.Name = "radioButtonCMYK_C";
            this.radioButtonCMYK_C.Size = new System.Drawing.Size(35, 16);
            this.radioButtonCMYK_C.TabIndex = 25;
            this.radioButtonCMYK_C.Text = "C:";
            this.radioButtonCMYK_C.UseVisualStyleBackColor = true;
            // 
            // textBoxCMYK_K
            // 
            this.textBoxCMYK_K.Enabled = false;
            this.textBoxCMYK_K.Location = new System.Drawing.Point(522, 293);
            this.textBoxCMYK_K.Name = "textBoxCMYK_K";
            this.textBoxCMYK_K.Size = new System.Drawing.Size(52, 21);
            this.textBoxCMYK_K.TabIndex = 32;
            // 
            // radioButtonCMYK_K
            // 
            this.radioButtonCMYK_K.AutoSize = true;
            this.radioButtonCMYK_K.Enabled = false;
            this.radioButtonCMYK_K.Location = new System.Drawing.Point(490, 296);
            this.radioButtonCMYK_K.Name = "radioButtonCMYK_K";
            this.radioButtonCMYK_K.Size = new System.Drawing.Size(35, 16);
            this.radioButtonCMYK_K.TabIndex = 31;
            this.radioButtonCMYK_K.Text = "K:";
            this.radioButtonCMYK_K.UseVisualStyleBackColor = true;
            // 
            // colorVerticalSliderUserControl
            // 
            this.colorVerticalSliderUserControl.DrawStyle = WorldEditor.Common.CustomWidget.ColorVerticalSliderUserControl.EDrawStyle.Hue;
            this.colorVerticalSliderUserControl.ForeColor = System.Drawing.SystemColors.ControlText;
            hsl1.H = 0;
            hsl1.L = 1;
            hsl1.S = 1;
            this.colorVerticalSliderUserControl.HSL = hsl1;
            this.colorVerticalSliderUserControl.Location = new System.Drawing.Point(328, 13);
            this.colorVerticalSliderUserControl.Name = "colorVerticalSliderUserControl";
            this.colorVerticalSliderUserControl.RGB = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            this.colorVerticalSliderUserControl.Size = new System.Drawing.Size(40, 308);
            this.colorVerticalSliderUserControl.TabIndex = 1;
            // 
            // colorPaletteUserControl
            // 
            this.colorPaletteUserControl.DrawStyle = WorldEditor.Common.CustomWidget.ColorPaletteUserControl.EDrawStyle.Hue;
            hsl2.H = 0;
            hsl2.L = 1;
            hsl2.S = 1;
            this.colorPaletteUserControl.HSL = hsl2;
            this.colorPaletteUserControl.Location = new System.Drawing.Point(12, 12);
            this.colorPaletteUserControl.Name = "colorPaletteUserControl";
            this.colorPaletteUserControl.RGB = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            this.colorPaletteUserControl.Size = new System.Drawing.Size(309, 309);
            this.colorPaletteUserControl.TabIndex = 0;
            // 
            // ColorPickForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(598, 334);
            this.Controls.Add(this.textBoxCMYK_K);
            this.Controls.Add(this.radioButtonCMYK_K);
            this.Controls.Add(this.textBoxCMYK_Y);
            this.Controls.Add(this.textBoxCMYK_M);
            this.Controls.Add(this.textBoxCMYK_C);
            this.Controls.Add(this.radioButtonCMYK_Y);
            this.Controls.Add(this.radioButtonCMYK_M);
            this.Controls.Add(this.radioButtonCMYK_C);
            this.Controls.Add(this.textBoxLab_b);
            this.Controls.Add(this.textBoxLab_a);
            this.Controls.Add(this.textBoxLab_L);
            this.Controls.Add(this.radioButtonLab_b);
            this.Controls.Add(this.radioButtonLab_a);
            this.Controls.Add(this.radioButtonLab_L);
            this.Controls.Add(this.textBoxHex);
            this.Controls.Add(this.label);
            this.Controls.Add(this.textBoxRGB_B);
            this.Controls.Add(this.textBoxRGB_G);
            this.Controls.Add(this.textBoxRGB_R);
            this.Controls.Add(this.radioButtonRGB_B);
            this.Controls.Add(this.radioButtonRGB_G);
            this.Controls.Add(this.radioButtonRGB_R);
            this.Controls.Add(this.textBoxHSL_L);
            this.Controls.Add(this.textBoxHSL_S);
            this.Controls.Add(this.textBoxHSL_H);
            this.Controls.Add(this.radioButtonHSL_L);
            this.Controls.Add(this.radioButtonHSL_S);
            this.Controls.Add(this.radioButtonHSL_H);
            this.Controls.Add(this.buttonCancel);
            this.Controls.Add(this.buttonOK);
            this.Controls.Add(this.labelColor);
            this.Controls.Add(this.colorVerticalSliderUserControl);
            this.Controls.Add(this.colorPaletteUserControl);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "ColorPickForm";
            this.ShowInTaskbar = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "ColorPickForm";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private WorldEditor.Common.CustomWidget.ColorPaletteUserControl colorPaletteUserControl;
        private WorldEditor.Common.CustomWidget.ColorVerticalSliderUserControl colorVerticalSliderUserControl;
        private System.Windows.Forms.Label labelColor;
        private System.Windows.Forms.Button buttonOK;
        private System.Windows.Forms.Button buttonCancel;
        private System.Windows.Forms.RadioButton radioButtonHSL_H;
        private System.Windows.Forms.RadioButton radioButtonHSL_S;
        private System.Windows.Forms.RadioButton radioButtonHSL_L;
        private System.Windows.Forms.TextBox textBoxHSL_H;
        private System.Windows.Forms.TextBox textBoxHSL_S;
        private System.Windows.Forms.TextBox textBoxHSL_L;
        private System.Windows.Forms.TextBox textBoxRGB_B;
        private System.Windows.Forms.TextBox textBoxRGB_G;
        private System.Windows.Forms.TextBox textBoxRGB_R;
        private System.Windows.Forms.RadioButton radioButtonRGB_B;
        private System.Windows.Forms.RadioButton radioButtonRGB_G;
        private System.Windows.Forms.RadioButton radioButtonRGB_R;
        private System.Windows.Forms.Label label;
        private System.Windows.Forms.TextBox textBoxHex;
        private System.Windows.Forms.TextBox textBoxLab_b;
        private System.Windows.Forms.TextBox textBoxLab_a;
        private System.Windows.Forms.TextBox textBoxLab_L;
        private System.Windows.Forms.RadioButton radioButtonLab_b;
        private System.Windows.Forms.RadioButton radioButtonLab_a;
        private System.Windows.Forms.RadioButton radioButtonLab_L;
        private System.Windows.Forms.TextBox textBoxCMYK_Y;
        private System.Windows.Forms.TextBox textBoxCMYK_M;
        private System.Windows.Forms.TextBox textBoxCMYK_C;
        private System.Windows.Forms.RadioButton radioButtonCMYK_Y;
        private System.Windows.Forms.RadioButton radioButtonCMYK_M;
        private System.Windows.Forms.RadioButton radioButtonCMYK_C;
        private System.Windows.Forms.TextBox textBoxCMYK_K;
        private System.Windows.Forms.RadioButton radioButtonCMYK_K;
     }
}