namespace WorldEditor.SceneModule.Window
{
    partial class PropertyForm
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
            this.groupBoxProp = new System.Windows.Forms.GroupBox();
            this.groupBoxModelViewer = new System.Windows.Forms.GroupBox();
            this.propertyGridProp = new System.Windows.Forms.PropertyGrid();
            this.groupBoxProp.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBoxProp
            // 
            this.groupBoxProp.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBoxProp.Controls.Add(this.propertyGridProp);
            this.groupBoxProp.Location = new System.Drawing.Point(2, 3);
            this.groupBoxProp.Name = "groupBoxProp";
            this.groupBoxProp.Size = new System.Drawing.Size(284, 274);
            this.groupBoxProp.TabIndex = 0;
            this.groupBoxProp.TabStop = false;
            this.groupBoxProp.Text = "属性";
            // 
            // groupBoxModelViewer
            // 
            this.groupBoxModelViewer.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBoxModelViewer.Location = new System.Drawing.Point(2, 283);
            this.groupBoxModelViewer.Name = "groupBoxModelViewer";
            this.groupBoxModelViewer.Size = new System.Drawing.Size(284, 209);
            this.groupBoxModelViewer.TabIndex = 1;
            this.groupBoxModelViewer.TabStop = false;
            this.groupBoxModelViewer.Text = "模型查看";
            // 
            // propertyGridProp
            // 
            this.propertyGridProp.Dock = System.Windows.Forms.DockStyle.Fill;
            this.propertyGridProp.Location = new System.Drawing.Point(3, 17);
            this.propertyGridProp.Name = "propertyGridProp";
            this.propertyGridProp.PropertySort = System.Windows.Forms.PropertySort.Categorized;
            this.propertyGridProp.Size = new System.Drawing.Size(278, 254);
            this.propertyGridProp.TabIndex = 0;
            this.propertyGridProp.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(this.propertyGridProp_PropertyValueChanged);
            // 
            // PropertyForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(292, 504);
            this.Controls.Add(this.groupBoxModelViewer);
            this.Controls.Add(this.groupBoxProp);
            this.Name = "PropertyForm";
            this.Text = "PropertyForm";
            this.groupBoxProp.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBoxProp;
        private System.Windows.Forms.GroupBox groupBoxModelViewer;
        private System.Windows.Forms.PropertyGrid propertyGridProp;
    }
}