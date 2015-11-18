namespace WorldEditor.Common.CustomWidget
{
    partial class ColorVerticalSliderUserControl
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
            this.SuspendLayout();
            // 
            // ColorVerticalSliderUserControl
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Name = "ColorVerticalSliderUserControl";
            this.Size = new System.Drawing.Size(40, 264);
            this.Load += new System.EventHandler(this.ColorVerticalSliderUserControl_Load);
            this.Paint += new System.Windows.Forms.PaintEventHandler(this.ColorVerticalSliderUserControl_Paint);
            this.MouseMove += new System.Windows.Forms.MouseEventHandler(this.ColorVerticalSliderUserControl_MouseMove);
            this.MouseDown += new System.Windows.Forms.MouseEventHandler(this.ColorVerticalSliderUserControl_MouseDown);
            this.Resize += new System.EventHandler(this.ColorVerticalSliderUserControl_Resize);
            this.MouseUp += new System.Windows.Forms.MouseEventHandler(this.ColorVerticalSliderUserControl_MouseUp);
            this.ResumeLayout(false);

        }

        #endregion
    }
}
