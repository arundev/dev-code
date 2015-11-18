namespace WorldEditor.Common.CustomWidget
{
    partial class TrackerBarWidget2
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
            this.label = new System.Windows.Forms.Label();
            this.numericUpDown = new System.Windows.Forms.NumericUpDown();
            this.trackBar = new WorldEditor.Common.CustomWidget.TrackerBarWidget2.InnerTrackBar();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar)).BeginInit();
            this.SuspendLayout();
            // 
            // label
            // 
            this.label.AutoSize = true;
            this.label.Dock = System.Windows.Forms.DockStyle.Left;
            this.label.Location = new System.Drawing.Point(0, 0);
            this.label.Name = "label";
            this.label.Padding = new System.Windows.Forms.Padding(2);
            this.label.Size = new System.Drawing.Size(39, 16);
            this.label.TabIndex = 0;
            this.label.Text = "label";
            this.label.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // numericUpDown
            // 
            this.numericUpDown.Dock = System.Windows.Forms.DockStyle.Right;
            this.numericUpDown.Location = new System.Drawing.Point(185, 0);
            this.numericUpDown.Name = "numericUpDown";
            this.numericUpDown.Size = new System.Drawing.Size(52, 21);
            this.numericUpDown.TabIndex = 3;
            this.numericUpDown.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            this.numericUpDown.ValueChanged += new System.EventHandler(this.numericUpDown_ValueChanged);
            // 
            // trackBar
            // 
            this.trackBar.AutoSize = false;
            this.trackBar.Dock = System.Windows.Forms.DockStyle.Fill;
            this.trackBar.Location = new System.Drawing.Point(39, 0);
            this.trackBar.Name = "trackBar";
            this.trackBar.Size = new System.Drawing.Size(146, 21);
            this.trackBar.TabIndex = 4;
            this.trackBar.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trackBar.Scroll += new System.EventHandler(this.trackBar_Scroll);
            // 
            // TrackerBarWidget2
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.trackBar);
            this.Controls.Add(this.numericUpDown);
            this.Controls.Add(this.label);
            this.Name = "TrackerBarWidget2";
            this.Size = new System.Drawing.Size(237, 21);
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label;
        private System.Windows.Forms.NumericUpDown numericUpDown;
        private TrackerBarWidget2.InnerTrackBar trackBar;
    }
}
