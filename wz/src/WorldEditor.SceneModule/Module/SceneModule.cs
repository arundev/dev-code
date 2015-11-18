using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Forms;

using WorldEditorFramework;
using WorldEditor.Base;
using WorldEditor.Common;
using WorldEditor.SceneModule.Window;

using WeifenLuo.WinFormsUI.Docking;

namespace WorldEditor.SceneModule.Module
{
	[WorldEditor.Common.Module.Module(DisplayName = "场景", Description = "场景编辑模块")]
	public class SceneModule : WorldEditor.Common.Module.Module
    {


        // 主操作接口
        private MWorldEditor mWorldEditor;
        public MWorldEditor WorldEditor
        {
            get { return mWorldEditor; }
        }

        // 窗口
        private MainForm mMainForm;         ///< 主窗口
        private WorldForm mWorldForm;       ///< 世界编辑窗口 
        private SceneForm mSceneForm;       ///< 场景编辑窗口
        private EntityForm mEntityForm;     ///< 实体编辑窗口
        private TerrainForm mTerrainForm;   ///< 地形相关编辑窗口
        private SettingForm mSettingForm;   ///< 渲染设置窗口
        private PropertyForm mPropertyForm; ///< 属性窗口
        private OutputForm mOutputForm;     ///< 输出窗口

        //
        public SceneModule()
        {
            try
            {
                // 新建面板
                this.mMainForm = new MainForm();
                this.mWorldForm = new WorldForm();
                this.mSceneForm = new SceneForm();
                this.mEntityForm = new EntityForm();
                this.mTerrainForm = new TerrainForm();
                this.mSettingForm = new SettingForm();
                this.mPropertyForm = new PropertyForm();
                this.mOutputForm = new OutputForm();
            }
            catch (System.Exception ex)
            {
                System.Windows.Forms.MessageBox.Show(ex.ToString());
            }
        }

		// 初始化(继承自IModule)
		public override bool Initialize()
		{
			base.Initialize();




			return true;
		}


        // 初始化
        public bool Initialize(DockPanel dockPanel)
        {
            // 加载配置界面
            String applicationPath = System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName;
            String configFile = Path.Combine(Path.GetDirectoryName(applicationPath), "WorldEditor.DockPanel.config");
            if (File.Exists(configFile))
            {
                DeserializeDockContent mDeserializeDockContent = new DeserializeDockContent(GetDockContentFromPersistString);
                dockPanel.LoadFromXml(configFile, mDeserializeDockContent);
            }
            else
            {
                dockPanel.SuspendLayout(true);
                this.mMainForm.Show(dockPanel, DockState.Document);
                this.mWorldForm.Show(dockPanel, DockState.DockRight);
                this.mSceneForm.Show(dockPanel, DockState.DockRight);
                this.mEntityForm.Show(dockPanel, DockState.DockRight);
                this.mTerrainForm.Show(dockPanel, DockState.DockRight);
                this.mSettingForm.Show(dockPanel, DockState.DockRight);               
                this.mPropertyForm.Show(dockPanel, DockState.DockRight);         
                this.mOutputForm.Show(dockPanel, DockState.DockBottom);
                dockPanel.ResumeLayout(true, true);
            }

            // 初始化操作接口
            this.mWorldEditor = new WorldEditorFramework.MWorldEditor();
            this.mWorldEditor.Initialize(this.mMainForm.Handle);

            //***********
            // 场景加载
            this.mTerrainForm.OnSceneLoad();
            this.mSettingForm.OnSceneLoad();
            this.mSceneForm.OnSceneLoad();
            //***********

            return true;
        }

        // 销毁
        public void Destroy(DockPanel dockPanel)
        {
            // 保存配置界面
            String applicationPath = System.Diagnostics.Process.GetCurrentProcess().MainModule.FileName;
            String szConfigFile = Path.Combine(Path.GetDirectoryName(applicationPath), "WorldEditor.DockPanel.config");
            dockPanel.SaveAsXml(szConfigFile);
        }

        // 更新
        public void Update()
        {
            if (mWorldEditor != null)
                mWorldEditor.Update();
        }

        // 用于DockPanel的布局保存
        private IDockContent GetDockContentFromPersistString(string szPersist)
        {
            if (typeof(MainForm).ToString() == szPersist)
                return mMainForm;
            else if (typeof(WorldForm).ToString() == szPersist)
                return mWorldForm;
            else if (typeof(SceneForm).ToString() == szPersist)
                return mSceneForm;
            else if (typeof(EntityForm).ToString() == szPersist)
                return mEntityForm;
            else if (typeof(TerrainForm).ToString() == szPersist)
                return mTerrainForm;
            else if (typeof(SettingForm).ToString() == szPersist)
                return mSettingForm;
            else if (typeof(PropertyForm).ToString() == szPersist)
                return mPropertyForm;
            else if (typeof(OutputForm).ToString() == szPersist)
                return mOutputForm;
            else
                return null;
        }
    }
}
