//*************************************************************************************************
// 内容:
//---------------------------------------------------------
// 作者:		
// 创建:		2014-06-04
// 最后修改:    2014-06-04
//*************************************************************************************************
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WorldEditor.Base.Interfaces.Services
{
	public interface IServiceManager
	{
		// 服务初始化与销毁
		bool Initialize();
		void Destroy();
	}
}
