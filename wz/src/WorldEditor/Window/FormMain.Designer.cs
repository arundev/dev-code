namespace WorldEditor.Window
{
    partial class FormMain
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
            this.components = new System.ComponentModel.Container();
            this.menuStrip = new System.Windows.Forms.MenuStrip();
            this.ToolStripMenuItemFile = new System.Windows.Forms.ToolStripMenuItem();
            this.newToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolStripMenuItemEdit = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolStripMenuItemView = new System.Windows.Forms.ToolStripMenuItem();
            this.windowToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ToolStripMenuItemHelp = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMain = new System.Windows.Forms.ToolStrip();
            this.statusStripMain = new System.Windows.Forms.StatusStrip();
            this.dockPanel = new WeifenLuo.WinFormsUI.Docking.DockPanel();
            this.timerMain = new System.Windows.Forms.Timer(this.components);
            this.menuStrip.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip
            // 
            this.menuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ToolStripMenuItemFile,
            this.ToolStripMenuItemEdit,
            this.ToolStripMenuItemView,
            this.windowToolStripMenuItem,
            this.ToolStripMenuItemHelp});
            this.menuStrip.Location = new System.Drawing.Point(0, 0);
            this.menuStrip.Name = "menuStrip";
            this.menuStrip.Size = new System.Drawing.Size(992, 25);
            this.menuStrip.TabIndex = 0;
            this.menuStrip.Text = "主菜单";
            // 
            // ToolStripMenuItemFile
            // 
            this.ToolStripMenuItemFile.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newToolStripMenuItem,
            this.openToolStripMenuItem,
            this.saveToolStripMenuItem});
            this.ToolStripMenuItemFile.Name = "ToolStripMenuItemFile";
            this.ToolStripMenuItemFile.Size = new System.Drawing.Size(39, 21);
            this.ToolStripMenuItemFile.Text = "File";
            // 
            // newToolStripMenuItem
            // 
            this.newToolStripMenuItem.Name = "newToolStripMenuItem";
            this.newToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.newToolStripMenuItem.Text = "New";
            this.newToolStripMenuItem.Click += new System.EventHandler(this.newToolStripMenuItem_Click);
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.openToolStripMenuItem.Text = "Open";
            this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(152, 22);
            this.saveToolStripMenuItem.Text = "Save";
            this.saveToolStripMenuItem.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
            // 
            // ToolStripMenuItemEdit
            // 
            this.ToolStripMenuItemEdit.Name = "ToolStripMenuItemEdit";
            this.ToolStripMenuItemEdit.Size = new System.Drawing.Size(42, 21);
            this.ToolStripMenuItemEdit.Text = "Edit";
            // 
            // ToolStripMenuItemView
            // 
            this.ToolStripMenuItemView.Name = "ToolStripMenuItemView";
            this.ToolStripMenuItemView.Size = new System.Drawing.Size(47, 21);
            this.ToolStripMenuItemView.Text = "View";
            // 
            // windowToolStripMenuItem
            // 
            this.windowToolStripMenuItem.Name = "windowToolStripMenuItem";
            this.windowToolStripMenuItem.Size = new System.Drawing.Size(67, 21);
            this.windowToolStripMenuItem.Text = "Window";
            // 
            // ToolStripMenuItemHelp
            // 
            this.ToolStripMenuItemHelp.Name = "ToolStripMenuItemHelp";
            this.ToolStripMenuItemHelp.Size = new System.Drawing.Size(47, 21);
            this.ToolStripMenuItemHelp.Text = "Help";
            // 
            // toolStripMain
            // 
            this.toolStripMain.Location = new System.Drawing.Point(0, 25);
            this.toolStripMain.Name = "toolStripMain";
            this.toolStripMain.Size = new System.Drawing.Size(992, 25);
            this.toolStripMain.TabIndex = 1;
            this.toolStripMain.Text = "主工具栏";
            // 
            // statusStripMain
            // 
            this.statusStripMain.Location = new System.Drawing.Point(0, 744);
            this.statusStripMain.Name = "statusStripMain";
            this.statusStripMain.Size = new System.Drawing.Size(992, 22);
            this.statusStripMain.TabIndex = 2;
            this.statusStripMain.Text = "statusStrip1";
            // 
            // dockPanel
            // 
            this.dockPanel.ActiveAutoHideContent = null;
            this.dockPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dockPanel.Location = new System.Drawing.Point(0, 50);
            this.dockPanel.Name = "dockPanel";
            this.dockPanel.Size = new System.Drawing.Size(992, 694);
            this.dockPanel.TabIndex = 3;
            // 
            // timerMain
            // 
            this.timerMain.Interval = 30;
            // 
            // FormMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(992, 766);
            this.Controls.Add(this.dockPanel);
            this.Controls.Add(this.toolStripMain);
            this.Controls.Add(this.menuStrip);
            this.Controls.Add(this.statusStripMain);
            this.IsMdiContainer = true;
            this.MainMenuStrip = this.menuStrip;
            this.Name = "FormMain";
            this.Text = "世界编辑器";
            this.Load += new System.EventHandler(this.FormMain_Load);
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FormMain_FormClosing);
            this.menuStrip.ResumeLayout(false);
            this.menuStrip.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip;
        private System.Windows.Forms.ToolStrip toolStripMain;
        private System.Windows.Forms.ToolStripMenuItem ToolStripMenuItemFile;
        private System.Windows.Forms.ToolStripMenuItem newToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem ToolStripMenuItemEdit;
        private System.Windows.Forms.ToolStripMenuItem ToolStripMenuItemView;
        private System.Windows.Forms.ToolStripMenuItem ToolStripMenuItemHelp;
        private System.Windows.Forms.StatusStrip statusStripMain;
        private WeifenLuo.WinFormsUI.Docking.DockPanel dockPanel;
        private System.Windows.Forms.Timer timerMain;
        private System.Windows.Forms.ToolStripMenuItem windowToolStripMenuItem;
    }
}

