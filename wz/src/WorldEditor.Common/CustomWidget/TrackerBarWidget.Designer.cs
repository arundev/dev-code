namespace WorldEditor.Common.CustomWidget
{
    partial class TrackerBarWidget
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
            this.groupBox = new System.Windows.Forms.GroupBox();
            this.numericUpDown = new System.Windows.Forms.NumericUpDown();
            this.trackBar = new WorldEditor.Common.CustomWidget.TrackerBarWidget.InnerTrackBar();
            this.groupBox.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar)).BeginInit();
            this.SuspendLayout();
            // 
            // groupBox
            // 
            this.groupBox.Controls.Add(this.numericUpDown);
            this.groupBox.Controls.Add(this.trackBar);
            this.groupBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.groupBox.Location = new System.Drawing.Point(0, 0);
            this.groupBox.Name = "groupBox";
            this.groupBox.Size = new System.Drawing.Size(200, 55);
            this.groupBox.TabIndex = 0;
            this.groupBox.TabStop = false;
            // 
            // numericUpDown
            // 
            this.numericUpDown.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.numericUpDown.Location = new System.Drawing.Point(146, 17);
            this.numericUpDown.Name = "numericUpDown";
            this.numericUpDown.Size = new System.Drawing.Size(51, 21);
            this.numericUpDown.TabIndex = 1;
            this.numericUpDown.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.numericUpDown.ValueChanged += new System.EventHandler(this.numericUpDown_ValueChanged);
            // 
            // trackBar
            // 
            this.trackBar.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.trackBar.AutoSize = false;
            this.trackBar.Location = new System.Drawing.Point(3, 17);
            this.trackBar.Name = "trackBar";
            this.trackBar.Size = new System.Drawing.Size(137, 21);
            this.trackBar.TabIndex = 0;
            this.trackBar.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trackBar.Scroll += new System.EventHandler(this.trackBar_Scroll);
            // 
            // TrackerBarWidget
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBox);
            this.Name = "TrackerBarWidget";
            this.Size = new System.Drawing.Size(200, 55);
            this.groupBox.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private InnerTrackBar trackBar;
        private System.Windows.Forms.GroupBox groupBox;
        private System.Windows.Forms.NumericUpDown numericUpDown;
    }
}
