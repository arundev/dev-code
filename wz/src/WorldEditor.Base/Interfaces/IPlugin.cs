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

namespace WorldEditor.Base.Interfaces
{
    // 插件接口
    public interface IPlugin
    {
        // 插件名称
        String Name
        {
            get;
        }

        // 插件版本
        System.Version Version
        {
            get;
        }

        // 插件期望版本
        System.Version ExpectedVersion
        {
            get;
        }

        // 插件加载
        bool Load(int iMajorVersion, int iMinorVersion);

        // 插件启动
        bool Start();

        // 插件服务列表
        Services.IService[] GetProvidedServices();
    }
}
