//*************************************************************************************************
// 内容:	程序入口(参考自WZEditor)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-25
// 最后修改:    
//*************************************************************************************************
using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace WorldEditor
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            // 设置
            Application.EnableVisualStyles();
//			Application.SetCompatibleTextRenderingDefault(false);

			// 初始化
			// @{
			// 初始化服务管理器
			EditorServiceManager.Instance.Initialize();

			// 初始化模块管理器

			// @}

            // 新建并运行主窗口
			WorldEditor.Window.FormMain kFormMain = new WorldEditor.Window.FormMain();
			Application.Run(kFormMain);

			// 销毁
			// @{

			// @}
        }
    }
}
