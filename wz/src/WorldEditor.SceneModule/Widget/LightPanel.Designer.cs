namespace WorldEditor.SceneModule.Widget
{
    partial class LightPanel
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
            this.groupBoxAmbientLight = new System.Windows.Forms.GroupBox();
            this.trackerBarWidgetAmbientLightOcclusion = new WorldEditor.Common.CustomWidget.TrackerBarWidget2();
            this.trackerBarWidgetAmbientLightDimmer = new WorldEditor.Common.CustomWidget.TrackerBarWidget2();
            this.labelAmbientLightDiffuseColor = new System.Windows.Forms.Label();
            this.labelAmbientLightAmbientColor = new System.Windows.Forms.Label();
            this.labelAmbientLightDiffuse = new System.Windows.Forms.Label();
            this.labelAmbientAmbient = new System.Windows.Forms.Label();
            this.groupBoxMainLight = new System.Windows.Forms.GroupBox();
            this.trackerBarWidgetMainLightOcclusion = new WorldEditor.Common.CustomWidget.TrackerBarWidget2();
            this.trackerBarWidgetMainLightDimmer = new WorldEditor.Common.CustomWidget.TrackerBarWidget2();
            this.labelMainLightAmbientColor = new System.Windows.Forms.Label();
            this.labelMainLightDiffuseColor = new System.Windows.Forms.Label();
            this.labelMainLightAmbient = new System.Windows.Forms.Label();
            this.labelMainLightDiffuse = new System.Windows.Forms.Label();
            this.groupBoxAssistantLight = new System.Windows.Forms.GroupBox();
            this.trackerBarWidget21 = new WorldEditor.Common.CustomWidget.TrackerBarWidget2();
            this.trackerBarWidgetAssistantLightDimmer = new WorldEditor.Common.CustomWidget.TrackerBarWidget2();
            this.labelAssistantLightAmbientColor = new System.Windows.Forms.Label();
            this.labelAssistantLightDiffuseColor = new System.Windows.Forms.Label();
            this.labelAssistantLightAmbient = new System.Windows.Forms.Label();
            this.labelAssistantLightDiffuse = new System.Windows.Forms.Label();
            this.groupBoxAmbientLight.SuspendLayout();
            this.groupBoxMainLight.SuspendLayout();
            this.groupBoxAssistantLight.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBoxAmbientLight
            // 
            this.groupBoxAmbientLight.Controls.Add(this.trackerBarWidgetAmbientLightOcclusion);
            this.groupBoxAmbientLight.Controls.Add(this.trackerBarWidgetAmbientLightDimmer);
            this.groupBoxAmbientLight.Controls.Add(this.labelAmbientLightDiffuseColor);
            this.groupBoxAmbientLight.Controls.Add(this.labelAmbientLightAmbientColor);
            this.groupBoxAmbientLight.Controls.Add(this.labelAmbientLightDiffuse);
            this.groupBoxAmbientLight.Controls.Add(this.labelAmbientAmbient);
            this.groupBoxAmbientLight.Location = new System.Drawing.Point(4, 4);
            this.groupBoxAmbientLight.Name = "groupBoxAmbientLight";
            this.groupBoxAmbientLight.Size = new System.Drawing.Size(243, 143);
            this.groupBoxAmbientLight.TabIndex = 0;
            this.groupBoxAmbientLight.TabStop = false;
            this.groupBoxAmbientLight.Text = "环境光";
            // 
            // trackerBarWidgetAmbientLightOcclusion
            // 
            this.trackerBarWidgetAmbientLightOcclusion.DecimalPlaces = 2;
            this.trackerBarWidgetAmbientLightOcclusion.Enabled = false;
            this.trackerBarWidgetAmbientLightOcclusion.Increment = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.trackerBarWidgetAmbientLightOcclusion.InnerScale = new decimal(new int[] {
            100,
            0,
            0,
            0});
            this.trackerBarWidgetAmbientLightOcclusion.LabelCaption = "环境遮挡";
            this.trackerBarWidgetAmbientLightOcclusion.Location = new System.Drawing.Point(9, 114);
            this.trackerBarWidgetAmbientLightOcclusion.MaxValue = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.trackerBarWidgetAmbientLightOcclusion.MinValue = new decimal(new int[] {
            0,
            0,
            0,
            0});
            this.trackerBarWidgetAmbientLightOcclusion.Name = "trackerBarWidgetAmbientLightOcclusion";
            this.trackerBarWidgetAmbientLightOcclusion.Size = new System.Drawing.Size(228, 25);
            this.trackerBarWidgetAmbientLightOcclusion.TabIndex = 5;
            this.trackerBarWidgetAmbientLightOcclusion.Value = new decimal(new int[] {
            0,
            0,
            0,
            0});
            // 
            // trackerBarWidgetAmbientLightDimmer
            // 
            this.trackerBarWidgetAmbientLightDimmer.DecimalPlaces = 2;
            this.trackerBarWidgetAmbientLightDimmer.Increment = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.trackerBarWidgetAmbientLightDimmer.InnerScale = new decimal(new int[] {
            100,
            0,
            0,
            0});
            this.trackerBarWidgetAmbientLightDimmer.LabelCaption = "亮度 ";
            this.trackerBarWidgetAmbientLightDimmer.Location = new System.Drawing.Point(8, 85);
            this.trackerBarWidgetAmbientLightDimmer.MaxValue = new decimal(new int[] {
            64,
            0,
            0,
            0});
            this.trackerBarWidgetAmbientLightDimmer.MinValue = new decimal(new int[] {
            0,
            0,
            0,
            0});
            this.trackerBarWidgetAmbientLightDimmer.Name = "trackerBarWidgetAmbientLightDimmer";
            this.trackerBarWidgetAmbientLightDimmer.Size = new System.Drawing.Size(228, 25);
            this.trackerBarWidgetAmbientLightDimmer.TabIndex = 4;
            this.trackerBarWidgetAmbientLightDimmer.Value = new decimal(new int[] {
            0,
            0,
            0,
            0});
            this.trackerBarWidgetAmbientLightDimmer.OnValueChanged += new WorldEditor.Common.CustomWidget.OnValueChangedHandler(this.trackerBarWidgetAmbientLightDimmer_OnValueChanged);
            // 
            // labelAmbientLightDiffuseColor
            // 
            this.labelAmbientLightDiffuseColor.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.labelAmbientLightDiffuseColor.Location = new System.Drawing.Point(113, 52);
            this.labelAmbientLightDiffuseColor.Name = "labelAmbientLightDiffuseColor";
            this.labelAmbientLightDiffuseColor.Size = new System.Drawing.Size(123, 20);
            this.labelAmbientLightDiffuseColor.TabIndex = 3;
            this.labelAmbientLightDiffuseColor.Click += new System.EventHandler(this.labelAmbientLightDiffuseColor_Click);
            // 
            // labelAmbientLightAmbientColor
            // 
            this.labelAmbientLightAmbientColor.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.labelAmbientLightAmbientColor.Location = new System.Drawing.Point(113, 21);
            this.labelAmbientLightAmbientColor.Name = "labelAmbientLightAmbientColor";
            this.labelAmbientLightAmbientColor.Size = new System.Drawing.Size(123, 20);
            this.labelAmbientLightAmbientColor.TabIndex = 2;
            this.labelAmbientLightAmbientColor.Click += new System.EventHandler(this.labelAmbientLightAmbientColor_Click);
            // 
            // labelAmbientLightDiffuse
            // 
            this.labelAmbientLightDiffuse.AutoSize = true;
            this.labelAmbientLightDiffuse.Location = new System.Drawing.Point(6, 55);
            this.labelAmbientLightDiffuse.Name = "labelAmbientLightDiffuse";
            this.labelAmbientLightDiffuse.Size = new System.Drawing.Size(101, 12);
            this.labelAmbientLightDiffuse.TabIndex = 1;
            this.labelAmbientLightDiffuse.Text = "地面颜色(漫反射)";
            // 
            // labelAmbientAmbient
            // 
            this.labelAmbientAmbient.AutoSize = true;
            this.labelAmbientAmbient.Font = new System.Drawing.Font("SimSun", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.labelAmbientAmbient.Location = new System.Drawing.Point(6, 25);
            this.labelAmbientAmbient.Name = "labelAmbientAmbient";
            this.labelAmbientAmbient.Size = new System.Drawing.Size(101, 12);
            this.labelAmbientAmbient.TabIndex = 0;
            this.labelAmbientAmbient.Text = "天空颜色(环境光)";
            // 
            // groupBoxMainLight
            // 
            this.groupBoxMainLight.Controls.Add(this.trackerBarWidgetMainLightOcclusion);
            this.groupBoxMainLight.Controls.Add(this.trackerBarWidgetMainLightDimmer);
            this.groupBoxMainLight.Controls.Add(this.labelMainLightAmbientColor);
            this.groupBoxMainLight.Controls.Add(this.labelMainLightDiffuseColor);
            this.groupBoxMainLight.Controls.Add(this.labelMainLightAmbient);
            this.groupBoxMainLight.Controls.Add(this.labelMainLightDiffuse);
            this.groupBoxMainLight.Location = new System.Drawing.Point(4, 153);
            this.groupBoxMainLight.Name = "groupBoxMainLight";
            this.groupBoxMainLight.Size = new System.Drawing.Size(243, 196);
            this.groupBoxMainLight.TabIndex = 1;
            this.groupBoxMainLight.TabStop = false;
            this.groupBoxMainLight.Text = "主光源";
            // 
            // trackerBarWidgetMainLightOcclusion
            // 
            this.trackerBarWidgetMainLightOcclusion.DecimalPlaces = 2;
            this.trackerBarWidgetMainLightOcclusion.Enabled = false;
            this.trackerBarWidgetMainLightOcclusion.Increment = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.trackerBarWidgetMainLightOcclusion.InnerScale = new decimal(new int[] {
            100,
            0,
            0,
            0});
            this.trackerBarWidgetMainLightOcclusion.LabelCaption = "环境遮挡";
            this.trackerBarWidgetMainLightOcclusion.Location = new System.Drawing.Point(6, 105);
            this.trackerBarWidgetMainLightOcclusion.MaxValue = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.trackerBarWidgetMainLightOcclusion.MinValue = new decimal(new int[] {
            0,
            0,
            0,
            0});
            this.trackerBarWidgetMainLightOcclusion.Name = "trackerBarWidgetMainLightOcclusion";
            this.trackerBarWidgetMainLightOcclusion.Size = new System.Drawing.Size(231, 25);
            this.trackerBarWidgetMainLightOcclusion.TabIndex = 10;
            this.trackerBarWidgetMainLightOcclusion.Value = new decimal(new int[] {
            0,
            0,
            0,
            0});
            // 
            // trackerBarWidgetMainLightDimmer
            // 
            this.trackerBarWidgetMainLightDimmer.DecimalPlaces = 2;
            this.trackerBarWidgetMainLightDimmer.Increment = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.trackerBarWidgetMainLightDimmer.InnerScale = new decimal(new int[] {
            100,
            0,
            0,
            0});
            this.trackerBarWidgetMainLightDimmer.LabelCaption = "亮度";
            this.trackerBarWidgetMainLightDimmer.Location = new System.Drawing.Point(6, 78);
            this.trackerBarWidgetMainLightDimmer.MaxValue = new decimal(new int[] {
            64,
            0,
            0,
            0});
            this.trackerBarWidgetMainLightDimmer.MinValue = new decimal(new int[] {
            0,
            0,
            0,
            0});
            this.trackerBarWidgetMainLightDimmer.Name = "trackerBarWidgetMainLightDimmer";
            this.trackerBarWidgetMainLightDimmer.Size = new System.Drawing.Size(230, 21);
            this.trackerBarWidgetMainLightDimmer.TabIndex = 9;
            this.trackerBarWidgetMainLightDimmer.Value = new decimal(new int[] {
            0,
            0,
            0,
            0});
            this.trackerBarWidgetMainLightDimmer.OnValueChanged += new WorldEditor.Common.CustomWidget.OnValueChangedHandler(this.trackerBarWidgetMainLightDimmer_OnValueChanged);
            // 
            // labelMainLightAmbientColor
            // 
            this.labelMainLightAmbientColor.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.labelMainLightAmbientColor.Location = new System.Drawing.Point(113, 44);
            this.labelMainLightAmbientColor.Name = "labelMainLightAmbientColor";
            this.labelMainLightAmbientColor.Size = new System.Drawing.Size(123, 20);
            this.labelMainLightAmbientColor.TabIndex = 8;
            this.labelMainLightAmbientColor.Click += new System.EventHandler(this.labelMainLightAmbientColor_Click);
            // 
            // labelMainLightDiffuseColor
            // 
            this.labelMainLightDiffuseColor.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.labelMainLightDiffuseColor.Location = new System.Drawing.Point(113, 13);
            this.labelMainLightDiffuseColor.Name = "labelMainLightDiffuseColor";
            this.labelMainLightDiffuseColor.Size = new System.Drawing.Size(123, 20);
            this.labelMainLightDiffuseColor.TabIndex = 7;
            this.labelMainLightDiffuseColor.Click += new System.EventHandler(this.labelMainLightDiffuseColor_Click);
            // 
            // labelMainLightAmbient
            // 
            this.labelMainLightAmbient.AutoSize = true;
            this.labelMainLightAmbient.Location = new System.Drawing.Point(6, 47);
            this.labelMainLightAmbient.Name = "labelMainLightAmbient";
            this.labelMainLightAmbient.Size = new System.Drawing.Size(101, 12);
            this.labelMainLightAmbient.TabIndex = 6;
            this.labelMainLightAmbient.Tag = "";
            this.labelMainLightAmbient.Text = "阴影颜色(环境光)";
            // 
            // labelMainLightDiffuse
            // 
            this.labelMainLightDiffuse.AutoSize = true;
            this.labelMainLightDiffuse.Font = new System.Drawing.Font("SimSun", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.labelMainLightDiffuse.Location = new System.Drawing.Point(6, 17);
            this.labelMainLightDiffuse.Name = "labelMainLightDiffuse";
            this.labelMainLightDiffuse.Size = new System.Drawing.Size(101, 12);
            this.labelMainLightDiffuse.TabIndex = 5;
            this.labelMainLightDiffuse.Tag = "";
            this.labelMainLightDiffuse.Text = "灯光颜色(漫反射)";
            // 
            // groupBoxAssistantLight
            // 
            this.groupBoxAssistantLight.Controls.Add(this.trackerBarWidget21);
            this.groupBoxAssistantLight.Controls.Add(this.trackerBarWidgetAssistantLightDimmer);
            this.groupBoxAssistantLight.Controls.Add(this.labelAssistantLightAmbientColor);
            this.groupBoxAssistantLight.Controls.Add(this.labelAssistantLightDiffuseColor);
            this.groupBoxAssistantLight.Controls.Add(this.labelAssistantLightAmbient);
            this.groupBoxAssistantLight.Controls.Add(this.labelAssistantLightDiffuse);
            this.groupBoxAssistantLight.Location = new System.Drawing.Point(7, 355);
            this.groupBoxAssistantLight.Name = "groupBoxAssistantLight";
            this.groupBoxAssistantLight.Size = new System.Drawing.Size(243, 180);
            this.groupBoxAssistantLight.TabIndex = 1;
            this.groupBoxAssistantLight.TabStop = false;
            this.groupBoxAssistantLight.Text = "辅助光源";
            // 
            // trackerBarWidget21
            // 
            this.trackerBarWidget21.DecimalPlaces = 2;
            this.trackerBarWidget21.Enabled = false;
            this.trackerBarWidget21.Increment = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.trackerBarWidget21.InnerScale = new decimal(new int[] {
            100,
            0,
            0,
            0});
            this.trackerBarWidget21.LabelCaption = "环境遮挡";
            this.trackerBarWidget21.Location = new System.Drawing.Point(6, 109);
            this.trackerBarWidget21.MaxValue = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.trackerBarWidget21.MinValue = new decimal(new int[] {
            0,
            0,
            0,
            0});
            this.trackerBarWidget21.Name = "trackerBarWidget21";
            this.trackerBarWidget21.Size = new System.Drawing.Size(231, 25);
            this.trackerBarWidget21.TabIndex = 16;
            this.trackerBarWidget21.Value = new decimal(new int[] {
            0,
            0,
            0,
            0});
            // 
            // trackerBarWidgetAssistantLightDimmer
            // 
            this.trackerBarWidgetAssistantLightDimmer.DecimalPlaces = 2;
            this.trackerBarWidgetAssistantLightDimmer.Increment = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.trackerBarWidgetAssistantLightDimmer.InnerScale = new decimal(new int[] {
            100,
            0,
            0,
            0});
            this.trackerBarWidgetAssistantLightDimmer.LabelCaption = "亮度";
            this.trackerBarWidgetAssistantLightDimmer.Location = new System.Drawing.Point(6, 82);
            this.trackerBarWidgetAssistantLightDimmer.MaxValue = new decimal(new int[] {
            64,
            0,
            0,
            0});
            this.trackerBarWidgetAssistantLightDimmer.MinValue = new decimal(new int[] {
            0,
            0,
            0,
            0});
            this.trackerBarWidgetAssistantLightDimmer.Name = "trackerBarWidgetAssistantLightDimmer";
            this.trackerBarWidgetAssistantLightDimmer.Size = new System.Drawing.Size(230, 21);
            this.trackerBarWidgetAssistantLightDimmer.TabIndex = 15;
            this.trackerBarWidgetAssistantLightDimmer.Value = new decimal(new int[] {
            0,
            0,
            0,
            0});
            this.trackerBarWidgetAssistantLightDimmer.OnValueChanged += new WorldEditor.Common.CustomWidget.OnValueChangedHandler(this.trackerBarWidgetAssistantLightDimmer_OnValueChanged);
            // 
            // labelAssistantLightAmbientColor
            // 
            this.labelAssistantLightAmbientColor.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.labelAssistantLightAmbientColor.Location = new System.Drawing.Point(113, 48);
            this.labelAssistantLightAmbientColor.Name = "labelAssistantLightAmbientColor";
            this.labelAssistantLightAmbientColor.Size = new System.Drawing.Size(123, 20);
            this.labelAssistantLightAmbientColor.TabIndex = 14;
            this.labelAssistantLightAmbientColor.Click += new System.EventHandler(this.labelAssistantLightAmbientColor_Click);
            // 
            // labelAssistantLightDiffuseColor
            // 
            this.labelAssistantLightDiffuseColor.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.labelAssistantLightDiffuseColor.Location = new System.Drawing.Point(113, 17);
            this.labelAssistantLightDiffuseColor.Name = "labelAssistantLightDiffuseColor";
            this.labelAssistantLightDiffuseColor.Size = new System.Drawing.Size(123, 20);
            this.labelAssistantLightDiffuseColor.TabIndex = 13;
            // 
            // labelAssistantLightAmbient
            // 
            this.labelAssistantLightAmbient.AutoSize = true;
            this.labelAssistantLightAmbient.Location = new System.Drawing.Point(6, 51);
            this.labelAssistantLightAmbient.Name = "labelAssistantLightAmbient";
            this.labelAssistantLightAmbient.Size = new System.Drawing.Size(101, 12);
            this.labelAssistantLightAmbient.TabIndex = 12;
            this.labelAssistantLightAmbient.Tag = "";
            this.labelAssistantLightAmbient.Text = "阴影颜色(环境光)";
            // 
            // labelAssistantLightDiffuse
            // 
            this.labelAssistantLightDiffuse.AutoSize = true;
            this.labelAssistantLightDiffuse.Font = new System.Drawing.Font("SimSun", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.labelAssistantLightDiffuse.Location = new System.Drawing.Point(6, 21);
            this.labelAssistantLightDiffuse.Name = "labelAssistantLightDiffuse";
            this.labelAssistantLightDiffuse.Size = new System.Drawing.Size(101, 12);
            this.labelAssistantLightDiffuse.TabIndex = 11;
            this.labelAssistantLightDiffuse.Tag = "";
            this.labelAssistantLightDiffuse.Text = "灯光颜色(漫反射)";
            // 
            // LightPanel
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBoxAssistantLight);
            this.Controls.Add(this.groupBoxMainLight);
            this.Controls.Add(this.groupBoxAmbientLight);
            this.Name = "LightPanel";
            this.Size = new System.Drawing.Size(250, 599);
            this.groupBoxAmbientLight.ResumeLayout(false);
            this.groupBoxAmbientLight.PerformLayout();
            this.groupBoxMainLight.ResumeLayout(false);
            this.groupBoxMainLight.PerformLayout();
            this.groupBoxAssistantLight.ResumeLayout(false);
            this.groupBoxAssistantLight.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBoxAmbientLight;
        private System.Windows.Forms.GroupBox groupBoxMainLight;
        private System.Windows.Forms.GroupBox groupBoxAssistantLight;
        private System.Windows.Forms.Label labelAmbientLightDiffuse;
        private System.Windows.Forms.Label labelAmbientAmbient;
        private System.Windows.Forms.Label labelAmbientLightDiffuseColor;
        private System.Windows.Forms.Label labelAmbientLightAmbientColor;
        private WorldEditor.Common.CustomWidget.TrackerBarWidget2 trackerBarWidgetAmbientLightDimmer;
        private WorldEditor.Common.CustomWidget.TrackerBarWidget2 trackerBarWidgetAmbientLightOcclusion;
        private System.Windows.Forms.Label labelMainLightAmbientColor;
        private System.Windows.Forms.Label labelMainLightDiffuseColor;
        private System.Windows.Forms.Label labelMainLightAmbient;
        private System.Windows.Forms.Label labelMainLightDiffuse;
        private WorldEditor.Common.CustomWidget.TrackerBarWidget2 trackerBarWidgetMainLightDimmer;
        private WorldEditor.Common.CustomWidget.TrackerBarWidget2 trackerBarWidgetMainLightOcclusion;
        private WorldEditor.Common.CustomWidget.TrackerBarWidget2 trackerBarWidget21;
        private WorldEditor.Common.CustomWidget.TrackerBarWidget2 trackerBarWidgetAssistantLightDimmer;
        private System.Windows.Forms.Label labelAssistantLightAmbientColor;
        private System.Windows.Forms.Label labelAssistantLightDiffuseColor;
        private System.Windows.Forms.Label labelAssistantLightAmbient;
        private System.Windows.Forms.Label labelAssistantLightDiffuse;
    }
}
