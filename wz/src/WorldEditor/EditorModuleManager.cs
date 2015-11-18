//*************************************************************************************************
// 内容:	编辑器模块管理器
//---------------------------------------------------------
// 作者:		
// 创建:		2014-06-04
// 最后修改:	
//*************************************************************************************************
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WorldEditor
{
	public class EditorModuleManager : Common.Module.ModuleManager
	{
		// 场景编辑器模块
		//protected 

		// 获取管理器单件
		public static EditorModuleManager GetModuleManager()
		{
			return (EditorModuleManager)Instance;
		}

		// 初始化
		public override bool Initialize()
		{
			base.Initialize();

			return true;
		}

		// 销毁
		public override void Destroy()
		{
			base.Destroy();
		}
	}
}
