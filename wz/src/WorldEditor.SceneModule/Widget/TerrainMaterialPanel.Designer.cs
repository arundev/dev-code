namespace WorldEditor.SceneModule.Widget
{
    partial class TerrainMaterialPanel
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
            this.groupBoxTerrainMaterial = new System.Windows.Forms.GroupBox();
            this.labelTerrainAmbientColor = new System.Windows.Forms.Label();
            this.labelTerrainAmbient = new System.Windows.Forms.Label();
            this.groupBoxTerrainEffect = new System.Windows.Forms.GroupBox();
            this.labelTerrainDiffuse = new System.Windows.Forms.Label();
            this.labelTerrainSpecular = new System.Windows.Forms.Label();
            this.labelTerrainDiffuseColor = new System.Windows.Forms.Label();
            this.labelTerrainSpecularColor = new System.Windows.Forms.Label();
            this.trackerBarWidgetTerrainShiness = new WorldEditor.Common.CustomWidget.TrackerBarWidget2();
            this.groupBoxTerrainMaterial.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBoxTerrainMaterial
            // 
            this.groupBoxTerrainMaterial.Controls.Add(this.trackerBarWidgetTerrainShiness);
            this.groupBoxTerrainMaterial.Controls.Add(this.labelTerrainSpecularColor);
            this.groupBoxTerrainMaterial.Controls.Add(this.labelTerrainDiffuseColor);
            this.groupBoxTerrainMaterial.Controls.Add(this.labelTerrainSpecular);
            this.groupBoxTerrainMaterial.Controls.Add(this.labelTerrainDiffuse);
            this.groupBoxTerrainMaterial.Controls.Add(this.labelTerrainAmbientColor);
            this.groupBoxTerrainMaterial.Controls.Add(this.labelTerrainAmbient);
            this.groupBoxTerrainMaterial.Location = new System.Drawing.Point(4, 4);
            this.groupBoxTerrainMaterial.Name = "groupBoxTerrainMaterial";
            this.groupBoxTerrainMaterial.Size = new System.Drawing.Size(243, 167);
            this.groupBoxTerrainMaterial.TabIndex = 0;
            this.groupBoxTerrainMaterial.TabStop = false;
            this.groupBoxTerrainMaterial.Text = "地形材质";
            // 
            // labelTerrainAmbientColor
            // 
            this.labelTerrainAmbientColor.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.labelTerrainAmbientColor.Location = new System.Drawing.Point(84, 21);
            this.labelTerrainAmbientColor.Name = "labelTerrainAmbientColor";
            this.labelTerrainAmbientColor.Size = new System.Drawing.Size(153, 20);
            this.labelTerrainAmbientColor.TabIndex = 4;
            this.labelTerrainAmbientColor.Click += new System.EventHandler(this.labelTerrainAmbientColor_Click);
            // 
            // labelTerrainAmbient
            // 
            this.labelTerrainAmbient.AutoSize = true;
            this.labelTerrainAmbient.Font = new System.Drawing.Font("SimSun", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.labelTerrainAmbient.Location = new System.Drawing.Point(6, 25);
            this.labelTerrainAmbient.Name = "labelTerrainAmbient";
            this.labelTerrainAmbient.Size = new System.Drawing.Size(59, 12);
            this.labelTerrainAmbient.TabIndex = 3;
            this.labelTerrainAmbient.Text = "环境材质:";
            // 
            // groupBoxTerrainEffect
            // 
            this.groupBoxTerrainEffect.Location = new System.Drawing.Point(4, 177);
            this.groupBoxTerrainEffect.Name = "groupBoxTerrainEffect";
            this.groupBoxTerrainEffect.Size = new System.Drawing.Size(243, 174);
            this.groupBoxTerrainEffect.TabIndex = 1;
            this.groupBoxTerrainEffect.TabStop = false;
            this.groupBoxTerrainEffect.Text = "地形效果";
            // 
            // labelTerrainDiffuse
            // 
            this.labelTerrainDiffuse.AutoSize = true;
            this.labelTerrainDiffuse.Font = new System.Drawing.Font("SimSun", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.labelTerrainDiffuse.Location = new System.Drawing.Point(6, 54);
            this.labelTerrainDiffuse.Name = "labelTerrainDiffuse";
            this.labelTerrainDiffuse.Size = new System.Drawing.Size(71, 12);
            this.labelTerrainDiffuse.TabIndex = 5;
            this.labelTerrainDiffuse.Text = "漫反射材质:";
            // 
            // labelTerrainSpecular
            // 
            this.labelTerrainSpecular.AutoSize = true;
            this.labelTerrainSpecular.Font = new System.Drawing.Font("SimSun", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.labelTerrainSpecular.Location = new System.Drawing.Point(6, 82);
            this.labelTerrainSpecular.Name = "labelTerrainSpecular";
            this.labelTerrainSpecular.Size = new System.Drawing.Size(59, 12);
            this.labelTerrainSpecular.TabIndex = 6;
            this.labelTerrainSpecular.Text = "高光材质:";
            // 
            // labelTerrainDiffuseColor
            // 
            this.labelTerrainDiffuseColor.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.labelTerrainDiffuseColor.Location = new System.Drawing.Point(84, 49);
            this.labelTerrainDiffuseColor.Name = "labelTerrainDiffuseColor";
            this.labelTerrainDiffuseColor.Size = new System.Drawing.Size(153, 20);
            this.labelTerrainDiffuseColor.TabIndex = 7;
            this.labelTerrainDiffuseColor.Click += new System.EventHandler(this.labelTerrainDiffuseColor_Click);
            // 
            // labelTerrainSpecularColor
            // 
            this.labelTerrainSpecularColor.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.labelTerrainSpecularColor.Location = new System.Drawing.Point(84, 77);
            this.labelTerrainSpecularColor.Name = "labelTerrainSpecularColor";
            this.labelTerrainSpecularColor.Size = new System.Drawing.Size(153, 20);
            this.labelTerrainSpecularColor.TabIndex = 8;
            this.labelTerrainSpecularColor.Click += new System.EventHandler(this.labelTerrainSpecularColor_Click);
            // 
            // trackerBarWidgetTerrainShiness
            // 
            this.trackerBarWidgetTerrainShiness.DecimalPlaces = 2;
            this.trackerBarWidgetTerrainShiness.Increment = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.trackerBarWidgetTerrainShiness.InnerScale = new decimal(new int[] {
            100,
            0,
            0,
            0});
            this.trackerBarWidgetTerrainShiness.LabelCaption = "高光范围";
            this.trackerBarWidgetTerrainShiness.Location = new System.Drawing.Point(6, 110);
            this.trackerBarWidgetTerrainShiness.MaxValue = new decimal(new int[] {
            100,
            0,
            0,
            0});
            this.trackerBarWidgetTerrainShiness.MinValue = new decimal(new int[] {
            0,
            0,
            0,
            0});
            this.trackerBarWidgetTerrainShiness.Name = "trackerBarWidgetTerrainShiness";
            this.trackerBarWidgetTerrainShiness.Size = new System.Drawing.Size(229, 21);
            this.trackerBarWidgetTerrainShiness.TabIndex = 10;
            this.trackerBarWidgetTerrainShiness.Value = new decimal(new int[] {
            0,
            0,
            0,
            0});
            this.trackerBarWidgetTerrainShiness.Scroll += new System.Windows.Forms.ScrollEventHandler(this.trackerBarWidgetTerrainShiness_Scroll);
            // 
            // TerrainMaterialPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBoxTerrainEffect);
            this.Controls.Add(this.groupBoxTerrainMaterial);
            this.Name = "TerrainMaterialPanel";
            this.Size = new System.Drawing.Size(250, 357);
            this.groupBoxTerrainMaterial.ResumeLayout(false);
            this.groupBoxTerrainMaterial.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBoxTerrainMaterial;
        private System.Windows.Forms.GroupBox groupBoxTerrainEffect;
        private System.Windows.Forms.Label labelTerrainAmbientColor;
        private System.Windows.Forms.Label labelTerrainAmbient;
        private System.Windows.Forms.Label labelTerrainSpecularColor;
        private System.Windows.Forms.Label labelTerrainDiffuseColor;
        private System.Windows.Forms.Label labelTerrainSpecular;
        private System.Windows.Forms.Label labelTerrainDiffuse;
        private WorldEditor.Common.CustomWidget.TrackerBarWidget2 trackerBarWidgetTerrainShiness;
    }
}
