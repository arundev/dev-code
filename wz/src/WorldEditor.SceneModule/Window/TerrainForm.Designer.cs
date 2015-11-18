namespace WorldEditor.SceneModule.Window
{
    partial class TerrainForm
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
            this.buttonTerrainSurface = new System.Windows.Forms.Button();
            this.groupBoxType = new System.Windows.Forms.GroupBox();
            this.button4 = new System.Windows.Forms.Button();
            this.buttonTerrainMaterial = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.buttonTerrainDeform = new System.Windows.Forms.Button();
            this.groupBoxProperty = new System.Windows.Forms.GroupBox();
            this.groupBoxType.SuspendLayout();
            this.SuspendLayout();
            // 
            // buttonTerrainSurface
            // 
            this.buttonTerrainSurface.Location = new System.Drawing.Point(116, 21);
            this.buttonTerrainSurface.Name = "buttonTerrainSurface";
            this.buttonTerrainSurface.Size = new System.Drawing.Size(99, 23);
            this.buttonTerrainSurface.TabIndex = 1;
            this.buttonTerrainSurface.Text = "地表纹理";
            this.buttonTerrainSurface.UseVisualStyleBackColor = true;
            this.buttonTerrainSurface.Click += new System.EventHandler(this.buttonTerrainSurface_Click);
            // 
            // groupBoxType
            // 
            this.groupBoxType.Controls.Add(this.button4);
            this.groupBoxType.Controls.Add(this.buttonTerrainMaterial);
            this.groupBoxType.Controls.Add(this.button2);
            this.groupBoxType.Controls.Add(this.button1);
            this.groupBoxType.Controls.Add(this.buttonTerrainSurface);
            this.groupBoxType.Controls.Add(this.buttonTerrainDeform);
            this.groupBoxType.Dock = System.Windows.Forms.DockStyle.Top;
            this.groupBoxType.Location = new System.Drawing.Point(2, 0);
            this.groupBoxType.Name = "groupBoxType";
            this.groupBoxType.Size = new System.Drawing.Size(288, 115);
            this.groupBoxType.TabIndex = 2;
            this.groupBoxType.TabStop = false;
            this.groupBoxType.Text = "类型";
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(116, 79);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(103, 23);
            this.button4.TabIndex = 5;
            this.button4.Text = "地表贴花";
            this.button4.UseVisualStyleBackColor = true;
            // 
            // buttonTerrainMaterial
            // 
            this.buttonTerrainMaterial.Location = new System.Drawing.Point(7, 50);
            this.buttonTerrainMaterial.Name = "buttonTerrainMaterial";
            this.buttonTerrainMaterial.Size = new System.Drawing.Size(103, 23);
            this.buttonTerrainMaterial.TabIndex = 4;
            this.buttonTerrainMaterial.Text = "地表材质";
            this.buttonTerrainMaterial.UseVisualStyleBackColor = true;
            this.buttonTerrainMaterial.Click += new System.EventHandler(this.buttonTerrainMaterial_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(7, 79);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(103, 23);
            this.button2.TabIndex = 3;
            this.button2.Text = "地表树木";
            this.button2.UseVisualStyleBackColor = true;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(116, 50);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(103, 23);
            this.button1.TabIndex = 2;
            this.button1.Text = "地表小物件";
            this.button1.UseVisualStyleBackColor = true;
            // 
            // buttonTerrainDeform
            // 
            this.buttonTerrainDeform.Location = new System.Drawing.Point(7, 21);
            this.buttonTerrainDeform.Name = "buttonTerrainDeform";
            this.buttonTerrainDeform.Size = new System.Drawing.Size(103, 23);
            this.buttonTerrainDeform.TabIndex = 0;
            this.buttonTerrainDeform.Text = "地表高度";
            this.buttonTerrainDeform.UseVisualStyleBackColor = true;
            this.buttonTerrainDeform.Click += new System.EventHandler(this.buttonTerrainDeform_Click);
            // 
            // groupBoxProperty
            // 
            this.groupBoxProperty.Dock = System.Windows.Forms.DockStyle.Top;
            this.groupBoxProperty.Location = new System.Drawing.Point(2, 115);
            this.groupBoxProperty.Name = "groupBoxProperty";
            this.groupBoxProperty.Size = new System.Drawing.Size(288, 345);
            this.groupBoxProperty.TabIndex = 3;
            this.groupBoxProperty.TabStop = false;
            this.groupBoxProperty.Text = "属性";
            // 
            // TerrainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.AutoScroll = true;
            this.ClientSize = new System.Drawing.Size(292, 463);
            this.Controls.Add(this.groupBoxProperty);
            this.Controls.Add(this.groupBoxType);
            this.Name = "TerrainForm";
            this.Padding = new System.Windows.Forms.Padding(2, 0, 2, 0);
            this.TabText = "TerrainForm";
            this.Text = "地形";
            this.Enter += new System.EventHandler(this.TerrainForm_Enter);
            this.Leave += new System.EventHandler(this.TerrainForm_Leave);
            this.groupBoxType.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button buttonTerrainSurface;
        private System.Windows.Forms.GroupBox groupBoxType;
        private System.Windows.Forms.Button buttonTerrainDeform;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Button buttonTerrainMaterial;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.GroupBox groupBoxProperty;
    }
}