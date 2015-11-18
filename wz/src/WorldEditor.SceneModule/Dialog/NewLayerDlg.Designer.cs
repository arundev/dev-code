namespace WorldEditor.SceneModule.Dialog
{
    partial class NewLayerDlg
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
            this.labelLayerName = new System.Windows.Forms.Label();
            this.textBoxLayerName = new System.Windows.Forms.TextBox();
            this.trackerBarWidgetLayerMipmapBias = new WorldEditor.Common.CustomWidget.TrackerBarWidget();
            this.trackerBarWidgetLayerUVRepeat = new WorldEditor.Common.CustomWidget.TrackerBarWidget();
            this.groupBoxNormal = new System.Windows.Forms.GroupBox();
            this.pictureBoxLayerNormalMap = new System.Windows.Forms.PictureBox();
            this.groupBoxDiffuseLayer = new System.Windows.Forms.GroupBox();
            this.pictureBoxLayerDiffuseMap = new System.Windows.Forms.PictureBox();
            this.buttonOK = new System.Windows.Forms.Button();
            this.buttonCancel = new System.Windows.Forms.Button();
            this.textBoxLayerDiffuseMap = new System.Windows.Forms.TextBox();
            this.textBoxLayerNormalMap = new System.Windows.Forms.TextBox();
            this.checkBoxLayerAlpha = new System.Windows.Forms.CheckBox();
            this.groupBoxNormal.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxLayerNormalMap)).BeginInit();
            this.groupBoxDiffuseLayer.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxLayerDiffuseMap)).BeginInit();
            this.SuspendLayout();
            // 
            // labelLayerName
            // 
            this.labelLayerName.AutoSize = true;
            this.labelLayerName.Location = new System.Drawing.Point(12, 12);
            this.labelLayerName.Name = "labelLayerName";
            this.labelLayerName.Size = new System.Drawing.Size(53, 12);
            this.labelLayerName.TabIndex = 0;
            this.labelLayerName.Text = "图层名称";
            // 
            // textBoxLayerName
            // 
            this.textBoxLayerName.Location = new System.Drawing.Point(71, 9);
            this.textBoxLayerName.Name = "textBoxLayerName";
            this.textBoxLayerName.Size = new System.Drawing.Size(190, 21);
            this.textBoxLayerName.TabIndex = 1;
            // 
            // trackerBarWidgetLayerMipmapBias
            // 
            this.trackerBarWidgetLayerMipmapBias.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.trackerBarWidgetLayerMipmapBias.DecimalPlaces = 2;
            this.trackerBarWidgetLayerMipmapBias.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.trackerBarWidgetLayerMipmapBias.InnerScale = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.trackerBarWidgetLayerMipmapBias.LabelCaption = "Mipmap-Bias";
            this.trackerBarWidgetLayerMipmapBias.Location = new System.Drawing.Point(270, 320);
            this.trackerBarWidgetLayerMipmapBias.MaxValue = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.trackerBarWidgetLayerMipmapBias.MinValue = new decimal(new int[] {
            0,
            0,
            0,
            0});
            this.trackerBarWidgetLayerMipmapBias.Name = "trackerBarWidgetLayerMipmapBias";
            this.trackerBarWidgetLayerMipmapBias.Size = new System.Drawing.Size(250, 55);
            this.trackerBarWidgetLayerMipmapBias.TabIndex = 13;
            this.trackerBarWidgetLayerMipmapBias.Value = new decimal(new int[] {
            0,
            0,
            0,
            0});
            // 
            // trackerBarWidgetLayerUVRepeat
            // 
            this.trackerBarWidgetLayerUVRepeat.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.trackerBarWidgetLayerUVRepeat.DecimalPlaces = 2;
            this.trackerBarWidgetLayerUVRepeat.Increment = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.trackerBarWidgetLayerUVRepeat.InnerScale = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.trackerBarWidgetLayerUVRepeat.LabelCaption = "UV-Repeat";
            this.trackerBarWidgetLayerUVRepeat.Location = new System.Drawing.Point(14, 320);
            this.trackerBarWidgetLayerUVRepeat.MaxValue = new decimal(new int[] {
            256,
            0,
            0,
            0});
            this.trackerBarWidgetLayerUVRepeat.MinValue = new decimal(new int[] {
            0,
            0,
            0,
            0});
            this.trackerBarWidgetLayerUVRepeat.Name = "trackerBarWidgetLayerUVRepeat";
            this.trackerBarWidgetLayerUVRepeat.Size = new System.Drawing.Size(247, 54);
            this.trackerBarWidgetLayerUVRepeat.TabIndex = 12;
            this.trackerBarWidgetLayerUVRepeat.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // groupBoxNormal
            // 
            this.groupBoxNormal.Controls.Add(this.pictureBoxLayerNormalMap);
            this.groupBoxNormal.Location = new System.Drawing.Point(270, 37);
            this.groupBoxNormal.Name = "groupBoxNormal";
            this.groupBoxNormal.Size = new System.Drawing.Size(250, 250);
            this.groupBoxNormal.TabIndex = 11;
            this.groupBoxNormal.TabStop = false;
            this.groupBoxNormal.Text = "法线贴图";
            // 
            // pictureBoxLayerNormalMap
            // 
            this.pictureBoxLayerNormalMap.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pictureBoxLayerNormalMap.Location = new System.Drawing.Point(3, 17);
            this.pictureBoxLayerNormalMap.Name = "pictureBoxLayerNormalMap";
            this.pictureBoxLayerNormalMap.Size = new System.Drawing.Size(244, 230);
            this.pictureBoxLayerNormalMap.TabIndex = 0;
            this.pictureBoxLayerNormalMap.TabStop = false;
            this.pictureBoxLayerNormalMap.Click += new System.EventHandler(this.pictureBoxLayerNormalMap_Click);
            // 
            // groupBoxDiffuseLayer
            // 
            this.groupBoxDiffuseLayer.Controls.Add(this.pictureBoxLayerDiffuseMap);
            this.groupBoxDiffuseLayer.Location = new System.Drawing.Point(14, 36);
            this.groupBoxDiffuseLayer.Name = "groupBoxDiffuseLayer";
            this.groupBoxDiffuseLayer.Size = new System.Drawing.Size(250, 250);
            this.groupBoxDiffuseLayer.TabIndex = 10;
            this.groupBoxDiffuseLayer.TabStop = false;
            this.groupBoxDiffuseLayer.Text = "漫反射贴图";
            // 
            // pictureBoxLayerDiffuseMap
            // 
            this.pictureBoxLayerDiffuseMap.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pictureBoxLayerDiffuseMap.Location = new System.Drawing.Point(3, 17);
            this.pictureBoxLayerDiffuseMap.Name = "pictureBoxLayerDiffuseMap";
            this.pictureBoxLayerDiffuseMap.Size = new System.Drawing.Size(244, 230);
            this.pictureBoxLayerDiffuseMap.TabIndex = 0;
            this.pictureBoxLayerDiffuseMap.TabStop = false;
            this.pictureBoxLayerDiffuseMap.Click += new System.EventHandler(this.pictureBoxLayerDiffuseMap_Click);
            // 
            // buttonOK
            // 
            this.buttonOK.Location = new System.Drawing.Point(361, 384);
            this.buttonOK.Name = "buttonOK";
            this.buttonOK.Size = new System.Drawing.Size(75, 23);
            this.buttonOK.TabIndex = 14;
            this.buttonOK.Text = "确认";
            this.buttonOK.UseVisualStyleBackColor = true;
            this.buttonOK.Click += new System.EventHandler(this.buttonOK_Click);
            // 
            // buttonCancel
            // 
            this.buttonCancel.Location = new System.Drawing.Point(442, 384);
            this.buttonCancel.Name = "buttonCancel";
            this.buttonCancel.Size = new System.Drawing.Size(75, 23);
            this.buttonCancel.TabIndex = 15;
            this.buttonCancel.Text = "取消";
            this.buttonCancel.UseVisualStyleBackColor = true;
            this.buttonCancel.Click += new System.EventHandler(this.buttonCancel_Click);
            // 
            // textBoxLayerDiffuseMap
            // 
            this.textBoxLayerDiffuseMap.BackColor = System.Drawing.SystemColors.Control;
            this.textBoxLayerDiffuseMap.Location = new System.Drawing.Point(12, 293);
            this.textBoxLayerDiffuseMap.Name = "textBoxLayerDiffuseMap";
            this.textBoxLayerDiffuseMap.Size = new System.Drawing.Size(252, 21);
            this.textBoxLayerDiffuseMap.TabIndex = 16;
            // 
            // textBoxLayerNormalMap
            // 
            this.textBoxLayerNormalMap.BackColor = System.Drawing.SystemColors.Control;
            this.textBoxLayerNormalMap.Location = new System.Drawing.Point(270, 293);
            this.textBoxLayerNormalMap.Name = "textBoxLayerNormalMap";
            this.textBoxLayerNormalMap.Size = new System.Drawing.Size(250, 21);
            this.textBoxLayerNormalMap.TabIndex = 17;
            // 
            // checkBoxLayerAlpha
            // 
            this.checkBoxLayerAlpha.AutoSize = true;
            this.checkBoxLayerAlpha.Enabled = false;
            this.checkBoxLayerAlpha.Location = new System.Drawing.Point(270, 11);
            this.checkBoxLayerAlpha.Name = "checkBoxLayerAlpha";
            this.checkBoxLayerAlpha.Size = new System.Drawing.Size(48, 16);
            this.checkBoxLayerAlpha.TabIndex = 18;
            this.checkBoxLayerAlpha.Text = "透明";
            this.checkBoxLayerAlpha.UseVisualStyleBackColor = true;
            // 
            // NewLayerDlg
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(531, 415);
            this.Controls.Add(this.checkBoxLayerAlpha);
            this.Controls.Add(this.textBoxLayerNormalMap);
            this.Controls.Add(this.textBoxLayerDiffuseMap);
            this.Controls.Add(this.buttonCancel);
            this.Controls.Add(this.buttonOK);
            this.Controls.Add(this.trackerBarWidgetLayerMipmapBias);
            this.Controls.Add(this.trackerBarWidgetLayerUVRepeat);
            this.Controls.Add(this.groupBoxNormal);
            this.Controls.Add(this.groupBoxDiffuseLayer);
            this.Controls.Add(this.textBoxLayerName);
            this.Controls.Add(this.labelLayerName);
            this.Name = "NewLayerDlg";
            this.Text = "新图层";
            this.groupBoxNormal.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxLayerNormalMap)).EndInit();
            this.groupBoxDiffuseLayer.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxLayerDiffuseMap)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label labelLayerName;
        private System.Windows.Forms.TextBox textBoxLayerName;
        private WorldEditor.Common.CustomWidget.TrackerBarWidget trackerBarWidgetLayerMipmapBias;
        private WorldEditor.Common.CustomWidget.TrackerBarWidget trackerBarWidgetLayerUVRepeat;
        private System.Windows.Forms.GroupBox groupBoxNormal;
        private System.Windows.Forms.PictureBox pictureBoxLayerNormalMap;
        private System.Windows.Forms.GroupBox groupBoxDiffuseLayer;
        private System.Windows.Forms.PictureBox pictureBoxLayerDiffuseMap;
        private System.Windows.Forms.Button buttonOK;
        private System.Windows.Forms.Button buttonCancel;
        private System.Windows.Forms.TextBox textBoxLayerDiffuseMap;
        private System.Windows.Forms.TextBox textBoxLayerNormalMap;
        private System.Windows.Forms.CheckBox checkBoxLayerAlpha;
    }
}