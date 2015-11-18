//*************************************************************************************************
// 内容:	编辑器服务管理器
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
	public class EditorServiceManager : WorldEditor.Common.Services.ServiceManager
	{
		// 获取管理器单件
		public static EditorServiceManager GetServiceManager()
		{
			return (EditorServiceManager)Instance;
		}

		// 初始化(继承自ServiceManager)
		public override bool Initialize()
		{
			return base.Initialize();
		}

		// 销毁(继承自ServiceManager)
		public override void Destroy()
		{
			base.Destroy();
		}
	}
}
