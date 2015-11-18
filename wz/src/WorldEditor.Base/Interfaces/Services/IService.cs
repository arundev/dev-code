//*************************************************************************************************
// 内容:	(参考自WZEditor.Base)
//---------------------------------------------------------
// 作者:		
// 创建:		2014-05-29
// 最后修改:    2014-05-29
//*************************************************************************************************
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WorldEditor.Base.Interfaces.Services
{
    // 服务接口
    public interface IService
    {
        // 服务名称
        String Name
        {
            get;
        }

        // 服务初始化与销毁
        bool Initialize();
        void Destroy();

        // 服务启动与停止
        bool Start();
        bool End();

        // 服务更新
        void Update();
    }
}
