namespace WorldEditor.SceneModule.Widget
{
    partial class TerrainSurfacePanel
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

        #region Component Designer generated code

        /// <summary> 
        /// Required method for Designer support - do not modify 
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.listBoxLayer = new System.Windows.Forms.ListBox();
            this.buttonLayerAdd = new System.Windows.Forms.Button();
            this.buttonLayerRemove = new System.Windows.Forms.Button();
            this.buttonLayerUp = new System.Windows.Forms.Button();
            this.buttonLayerDown = new System.Windows.Forms.Button();
            this.groupBoxLayer = new System.Windows.Forms.GroupBox();
            this.groupBoxLayerDiffuse = new System.Windows.Forms.GroupBox();
            this.pictureBoxDiffuseMap = new System.Windows.Forms.PictureBox();
            this.groupBoxNormal = new System.Windows.Forms.GroupBox();
            this.pictureBoxNormalMap = new System.Windows.Forms.PictureBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.trackerBarWidgetMipmapBias = new WorldEditor.Common.CustomWidget.TrackerBarWidget();
            this.trackerBarWidgetUVScale = new WorldEditor.Common.CustomWidget.TrackerBarWidget();
            this.groupBoxCurLayer = new System.Windows.Forms.GroupBox();
            this.listBoxCurLayer = new System.Windows.Forms.ListBox();
            this.trackerBarWidgetBrushPower = new WorldEditor.Common.CustomWidget.TrackerBarWidget();
            this.trackerBarWidgetInnerRadius = new WorldEditor.Common.CustomWidget.TrackerBarWidget();
            this.trackerBarWidgetOutterRadius = new WorldEditor.Common.CustomWidget.TrackerBarWidget();
            this.checkBoxShowTexTileGrid = new System.Windows.Forms.CheckBox();
            this.checkBoxShowInvisiableLayer = new System.Windows.Forms.CheckBox();
            this.checkBoxShowCurrentLayer = new System.Windows.Forms.CheckBox();
            this.groupBoxLayer.SuspendLayout();
            this.groupBoxLayerDiffuse.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxDiffuseMap)).BeginInit();
            this.groupBoxNormal.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxNormalMap)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.groupBoxCurLayer.SuspendLayout();
            this.SuspendLayout();
            // 
            // listBoxLayer
            // 
            this.listBoxLayer.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.listBoxLayer.FormattingEnabled = true;
            this.listBoxLayer.ItemHeight = 12;
            this.listBoxLayer.Location = new System.Drawing.Point(7, 21);
            this.listBoxLayer.Name = "listBoxLayer";
            this.listBoxLayer.Size = new System.Drawing.Size(187, 148);
            this.listBoxLayer.TabIndex = 0;
            this.listBoxLayer.SelectedIndexChanged += new System.EventHandler(this.listBoxLayer_SelectedIndexChanged);
            // 
            // buttonLayerAdd
            // 
            this.buttonLayerAdd.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonLayerAdd.Location = new System.Drawing.Point(200, 21);
            this.buttonLayerAdd.Name = "buttonLayerAdd";
            this.buttonLayerAdd.Size = new System.Drawing.Size(32, 32);
            this.buttonLayerAdd.TabIndex = 1;
            this.buttonLayerAdd.Text = "+";
            this.buttonLayerAdd.UseVisualStyleBackColor = true;
            this.buttonLayerAdd.Click += new System.EventHandler(this.buttonLayerAdd_Click);
            // 
            // buttonLayerRemove
            // 
            this.buttonLayerRemove.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonLayerRemove.Location = new System.Drawing.Point(200, 59);
            this.buttonLayerRemove.Name = "buttonLayerRemove";
            this.buttonLayerRemove.Size = new System.Drawing.Size(32, 32);
            this.buttonLayerRemove.TabIndex = 2;
            this.buttonLayerRemove.Text = "-";
            this.buttonLayerRemove.UseVisualStyleBackColor = true;
            this.buttonLayerRemove.Click += new System.EventHandler(this.buttonLayerRemove_Click);
            // 
            // buttonLayerUp
            // 
            this.buttonLayerUp.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonLayerUp.Location = new System.Drawing.Point(200, 97);
            this.buttonLayerUp.Name = "buttonLayerUp";
            this.buttonLayerUp.Size = new System.Drawing.Size(32, 32);
            this.buttonLayerUp.TabIndex = 3;
            this.buttonLayerUp.Text = "上";
            this.buttonLayerUp.UseVisualStyleBackColor = true;
            this.buttonLayerUp.Click += new System.EventHandler(this.buttonLayerUp_Click);
            // 
            // buttonLayerDown
            // 
            this.buttonLayerDown.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonLayerDown.Location = new System.Drawing.Point(200, 135);
            this.buttonLayerDown.Name = "buttonLayerDown";
            this.buttonLayerDown.Size = new System.Drawing.Size(32, 32);
            this.buttonLayerDown.TabIndex = 4;
            this.buttonLayerDown.Text = "下";
            this.buttonLayerDown.UseVisualStyleBackColor = true;
            this.buttonLayerDown.Click += new System.EventHandler(this.buttonLayerDown_Click);
            // 
            // groupBoxLayer
            // 
            this.groupBoxLayer.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBoxLayer.Controls.Add(this.buttonLayerDown);
            this.groupBoxLayer.Controls.Add(this.buttonLayerUp);
            this.groupBoxLayer.Controls.Add(this.buttonLayerRemove);
            this.groupBoxLayer.Controls.Add(this.buttonLayerAdd);
            this.groupBoxLayer.Controls.Add(this.listBoxLayer);
            this.groupBoxLayer.Location = new System.Drawing.Point(6, 187);
            this.groupBoxLayer.Name = "groupBoxLayer";
            this.groupBoxLayer.Size = new System.Drawing.Size(238, 182);
            this.groupBoxLayer.TabIndex = 8;
            this.groupBoxLayer.TabStop = false;
            this.groupBoxLayer.Text = "图层";
            // 
            // groupBoxLayerDiffuse
            // 
            this.groupBoxLayerDiffuse.Controls.Add(this.pictureBoxDiffuseMap);
            this.groupBoxLayerDiffuse.Location = new System.Drawing.Point(7, 20);
            this.groupBoxLayerDiffuse.Name = "groupBoxLayerDiffuse";
            this.groupBoxLayerDiffuse.Size = new System.Drawing.Size(110, 124);
            this.groupBoxLayerDiffuse.TabIndex = 0;
            this.groupBoxLayerDiffuse.TabStop = false;
            this.groupBoxLayerDiffuse.Text = "漫反射贴图";
            // 
            // pictureBoxDiffuseMap
            // 
            this.pictureBoxDiffuseMap.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pictureBoxDiffuseMap.Location = new System.Drawing.Point(3, 17);
            this.pictureBoxDiffuseMap.Name = "pictureBoxDiffuseMap";
            this.pictureBoxDiffuseMap.Size = new System.Drawing.Size(104, 104);
            this.pictureBoxDiffuseMap.TabIndex = 0;
            this.pictureBoxDiffuseMap.TabStop = false;
            this.pictureBoxDiffuseMap.Click += new System.EventHandler(this.pictureBoxDiffuseMap_Click);
            // 
            // groupBoxNormal
            // 
            this.groupBoxNormal.Controls.Add(this.pictureBoxNormalMap);
            this.groupBoxNormal.Location = new System.Drawing.Point(122, 20);
            this.groupBoxNormal.Name = "groupBoxNormal";
            this.groupBoxNormal.Size = new System.Drawing.Size(110, 124);
            this.groupBoxNormal.TabIndex = 1;
            this.groupBoxNormal.TabStop = false;
            this.groupBoxNormal.Text = "法线贴图";
            // 
            // pictureBoxNormalMap
            // 
            this.pictureBoxNormalMap.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pictureBoxNormalMap.Location = new System.Drawing.Point(3, 17);
            this.pictureBoxNormalMap.Name = "pictureBoxNormalMap";
            this.pictureBoxNormalMap.Size = new System.Drawing.Size(104, 104);
            this.pictureBoxNormalMap.TabIndex = 0;
            this.pictureBoxNormalMap.TabStop = false;
            this.pictureBoxNormalMap.Click += new System.EventHandler(this.pictureBoxNormalMap_Click);
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.trackerBarWidgetMipmapBias);
            this.groupBox1.Controls.Add(this.trackerBarWidgetUVScale);
            this.groupBox1.Controls.Add(this.groupBoxNormal);
            this.groupBox1.Controls.Add(this.groupBoxLayerDiffuse);
            this.groupBox1.Location = new System.Drawing.Point(6, 375);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(238, 271);
            this.groupBox1.TabIndex = 9;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "当前图层";
            // 
            // trackerBarWidgetMipmapBias
            // 
            this.trackerBarWidgetMipmapBias.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.trackerBarWidgetMipmapBias.DecimalPlaces = 1;
            this.trackerBarWidgetMipmapBias.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.trackerBarWidgetMipmapBias.InnerScale = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.trackerBarWidgetMipmapBias.LabelCaption = "Mipmap-Bias";
            this.trackerBarWidgetMipmapBias.Location = new System.Drawing.Point(7, 207);
            this.trackerBarWidgetMipmapBias.MaxValue = new decimal(new int[] {
            2,
            0,
            0,
            0});
            this.trackerBarWidgetMipmapBias.MinValue = new decimal(new int[] {
            2,
            0,
            0,
            -2147483648});
            this.trackerBarWidgetMipmapBias.Name = "trackerBarWidgetMipmapBias";
            this.trackerBarWidgetMipmapBias.Size = new System.Drawing.Size(225, 55);
            this.trackerBarWidgetMipmapBias.TabIndex = 9;
            this.trackerBarWidgetMipmapBias.Value = new decimal(new int[] {
            0,
            0,
            0,
            0});
            this.trackerBarWidgetMipmapBias.OnValueChanged += new WorldEditor.Common.CustomWidget.OnValueChangedHandler(this.trackerBarWidgetMipmapBias_OnValueChanged);
            // 
            // trackerBarWidgetUVScale
            // 
            this.trackerBarWidgetUVScale.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.trackerBarWidgetUVScale.DecimalPlaces = 2;
            this.trackerBarWidgetUVScale.Increment = new decimal(new int[] {
            1,
            0,
            0,
            131072});
            this.trackerBarWidgetUVScale.InnerScale = new decimal(new int[] {
            100,
            0,
            0,
            0});
            this.trackerBarWidgetUVScale.LabelCaption = "UV-Scale";
            this.trackerBarWidgetUVScale.Location = new System.Drawing.Point(7, 147);
            this.trackerBarWidgetUVScale.MaxValue = new decimal(new int[] {
            256,
            0,
            0,
            0});
            this.trackerBarWidgetUVScale.MinValue = new decimal(new int[] {
            0,
            0,
            0,
            0});
            this.trackerBarWidgetUVScale.Name = "trackerBarWidgetUVScale";
            this.trackerBarWidgetUVScale.Size = new System.Drawing.Size(225, 54);
            this.trackerBarWidgetUVScale.TabIndex = 8;
            this.trackerBarWidgetUVScale.Value = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.trackerBarWidgetUVScale.OnValueChanged += new WorldEditor.Common.CustomWidget.OnValueChangedHandler(this.trackerBarWidgetUVScale_OnValueChanged);
            // 
            // groupBoxCurLayer
            // 
            this.groupBoxCurLayer.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBoxCurLayer.Controls.Add(this.listBoxCurLayer);
            this.groupBoxCurLayer.Location = new System.Drawing.Point(6, 716);
            this.groupBoxCurLayer.Name = "groupBoxCurLayer";
            this.groupBoxCurLayer.Size = new System.Drawing.Size(238, 120);
            this.groupBoxCurLayer.TabIndex = 12;
            this.groupBoxCurLayer.TabStop = false;
            this.groupBoxCurLayer.Text = "当前使用层";
            // 
            // listBoxCurLayer
            // 
            this.listBoxCurLayer.Dock = System.Windows.Forms.DockStyle.Fill;
            this.listBoxCurLayer.FormattingEnabled = true;
            this.listBoxCurLayer.ItemHeight = 12;
            this.listBoxCurLayer.Location = new System.Drawing.Point(3, 17);
            this.listBoxCurLayer.Name = "listBoxCurLayer";
            this.listBoxCurLayer.Size = new System.Drawing.Size(232, 100);
            this.listBoxCurLayer.TabIndex = 0;
            // 
            // trackerBarWidgetBrushPower
            // 
            this.trackerBarWidgetBrushPower.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.trackerBarWidgetBrushPower.DecimalPlaces = 2;
            this.trackerBarWidgetBrushPower.Increment = new decimal(new int[] {
            1,
            0,
            0,
            131072});
            this.trackerBarWidgetBrushPower.InnerScale = new decimal(new int[] {
            100,
            0,
            0,
            0});
            this.trackerBarWidgetBrushPower.LabelCaption = "强度";
            this.trackerBarWidgetBrushPower.Location = new System.Drawing.Point(6, 125);
            this.trackerBarWidgetBrushPower.MaxValue = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.trackerBarWidgetBrushPower.MinValue = new decimal(new int[] {
            1,
            0,
            0,
            -2147483648});
            this.trackerBarWidgetBrushPower.Name = "trackerBarWidgetBrushPower";
            this.trackerBarWidgetBrushPower.Size = new System.Drawing.Size(238, 55);
            this.trackerBarWidgetBrushPower.TabIndex = 7;
            this.trackerBarWidgetBrushPower.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.trackerBarWidgetBrushPower.OnValueChanged += new WorldEditor.Common.CustomWidget.OnValueChangedHandler(this.trackerBarWidgetBrushPower_OnValueChanged);
            // 
            // trackerBarWidgetInnerRadius
            // 
            this.trackerBarWidgetInnerRadius.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.trackerBarWidgetInnerRadius.DecimalPlaces = 2;
            this.trackerBarWidgetInnerRadius.Increment = new decimal(new int[] {
            1,
            0,
            0,
            131072});
            this.trackerBarWidgetInnerRadius.InnerScale = new decimal(new int[] {
            100,
            0,
            0,
            0});
            this.trackerBarWidgetInnerRadius.LabelCaption = "内圈半径";
            this.trackerBarWidgetInnerRadius.Location = new System.Drawing.Point(3, 64);
            this.trackerBarWidgetInnerRadius.MaxValue = new decimal(new int[] {
            20,
            0,
            0,
            0});
            this.trackerBarWidgetInnerRadius.MinValue = new decimal(new int[] {
            0,
            0,
            0,
            0});
            this.trackerBarWidgetInnerRadius.Name = "trackerBarWidgetInnerRadius";
            this.trackerBarWidgetInnerRadius.Size = new System.Drawing.Size(241, 55);
            this.trackerBarWidgetInnerRadius.TabIndex = 6;
            this.trackerBarWidgetInnerRadius.Value = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.trackerBarWidgetInnerRadius.OnValueChanged += new WorldEditor.Common.CustomWidget.OnValueChangedHandler(this.trackerBarWidgetInnerRadius_OnValueChanged);
            // 
            // trackerBarWidgetOutterRadius
            // 
            this.trackerBarWidgetOutterRadius.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.trackerBarWidgetOutterRadius.DecimalPlaces = 2;
            this.trackerBarWidgetOutterRadius.Increment = new decimal(new int[] {
            1,
            0,
            0,
            131072});
            this.trackerBarWidgetOutterRadius.InnerScale = new decimal(new int[] {
            100,
            0,
            0,
            0});
            this.trackerBarWidgetOutterRadius.LabelCaption = "外圈半径";
            this.trackerBarWidgetOutterRadius.Location = new System.Drawing.Point(3, 3);
            this.trackerBarWidgetOutterRadius.MaxValue = new decimal(new int[] {
            20,
            0,
            0,
            0});
            this.trackerBarWidgetOutterRadius.MinValue = new decimal(new int[] {
            0,
            0,
            0,
            0});
            this.trackerBarWidgetOutterRadius.Name = "trackerBarWidgetOutterRadius";
            this.trackerBarWidgetOutterRadius.Size = new System.Drawing.Size(241, 55);
            this.trackerBarWidgetOutterRadius.TabIndex = 5;
            this.trackerBarWidgetOutterRadius.Value = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.trackerBarWidgetOutterRadius.OnValueChanged += new WorldEditor.Common.CustomWidget.OnValueChangedHandler(this.trackerBarWidgetOutterRadius_OnValueChanged);
            // 
            // checkBoxShowTexTileGrid
            // 
            this.checkBoxShowTexTileGrid.AutoSize = true;
            this.checkBoxShowTexTileGrid.Location = new System.Drawing.Point(9, 653);
            this.checkBoxShowTexTileGrid.Name = "checkBoxShowTexTileGrid";
            this.checkBoxShowTexTileGrid.Size = new System.Drawing.Size(108, 16);
            this.checkBoxShowTexTileGrid.TabIndex = 14;
            this.checkBoxShowTexTileGrid.Text = "显示材质块范围";
            this.checkBoxShowTexTileGrid.UseVisualStyleBackColor = true;
            this.checkBoxShowTexTileGrid.CheckedChanged += new System.EventHandler(this.checkBoxShowTexTileGrid_CheckedChanged);
            // 
            // checkBoxShowInvisiableLayer
            // 
            this.checkBoxShowInvisiableLayer.AutoSize = true;
            this.checkBoxShowInvisiableLayer.Location = new System.Drawing.Point(9, 672);
            this.checkBoxShowInvisiableLayer.Name = "checkBoxShowInvisiableLayer";
            this.checkBoxShowInvisiableLayer.Size = new System.Drawing.Size(84, 16);
            this.checkBoxShowInvisiableLayer.TabIndex = 15;
            this.checkBoxShowInvisiableLayer.Text = "显示超出层";
            this.checkBoxShowInvisiableLayer.UseVisualStyleBackColor = true;
            this.checkBoxShowInvisiableLayer.CheckedChanged += new System.EventHandler(this.checkBoxShowInvisiableLayer_CheckedChanged);
            // 
            // checkBoxShowCurrentLayer
            // 
            this.checkBoxShowCurrentLayer.AutoSize = true;
            this.checkBoxShowCurrentLayer.Location = new System.Drawing.Point(9, 691);
            this.checkBoxShowCurrentLayer.Name = "checkBoxShowCurrentLayer";
            this.checkBoxShowCurrentLayer.Size = new System.Drawing.Size(96, 16);
            this.checkBoxShowCurrentLayer.TabIndex = 16;
            this.checkBoxShowCurrentLayer.Text = "只显示当前层";
            this.checkBoxShowCurrentLayer.UseVisualStyleBackColor = true;
            this.checkBoxShowCurrentLayer.CheckedChanged += new System.EventHandler(this.checkBoxShowCurrentLayer_CheckedChanged);
            // 
            // TerrainSurfacePanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.checkBoxShowCurrentLayer);
            this.Controls.Add(this.checkBoxShowInvisiableLayer);
            this.Controls.Add(this.checkBoxShowTexTileGrid);
            this.Controls.Add(this.groupBoxCurLayer);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.groupBoxLayer);
            this.Controls.Add(this.trackerBarWidgetBrushPower);
            this.Controls.Add(this.trackerBarWidgetInnerRadius);
            this.Controls.Add(this.trackerBarWidgetOutterRadius);
            this.Name = "TerrainSurfacePanel";
            this.Size = new System.Drawing.Size(250, 855);
            this.groupBoxLayer.ResumeLayout(false);
            this.groupBoxLayerDiffuse.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxDiffuseMap)).EndInit();
            this.groupBoxNormal.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxNormalMap)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBoxCurLayer.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private WorldEditor.Common.CustomWidget.TrackerBarWidget trackerBarWidgetInnerRadius;
        private WorldEditor.Common.CustomWidget.TrackerBarWidget trackerBarWidgetOutterRadius;
        private WorldEditor.Common.CustomWidget.TrackerBarWidget trackerBarWidgetBrushPower;
        private System.Windows.Forms.ListBox listBoxLayer;
        private System.Windows.Forms.Button buttonLayerAdd;
        private System.Windows.Forms.Button buttonLayerRemove;
        private System.Windows.Forms.Button buttonLayerUp;
        private System.Windows.Forms.Button buttonLayerDown;
        private System.Windows.Forms.GroupBox groupBoxLayer;
        private System.Windows.Forms.GroupBox groupBoxLayerDiffuse;
        private System.Windows.Forms.PictureBox pictureBoxDiffuseMap;
        private System.Windows.Forms.GroupBox groupBoxNormal;
        private System.Windows.Forms.PictureBox pictureBoxNormalMap;
        private WorldEditor.Common.CustomWidget.TrackerBarWidget trackerBarWidgetUVScale;
        private WorldEditor.Common.CustomWidget.TrackerBarWidget trackerBarWidgetMipmapBias;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBoxCurLayer;
        private System.Windows.Forms.ListBox listBoxCurLayer;
        private System.Windows.Forms.CheckBox checkBoxShowTexTileGrid;
        private System.Windows.Forms.CheckBox checkBoxShowInvisiableLayer;
        private System.Windows.Forms.CheckBox checkBoxShowCurrentLayer;
    }
}
