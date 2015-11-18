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
    // 编辑模块类型
    public enum EModuleType
    {
        E_MT_UNKNOWN,
        E_MT_SCENE,
        E_MT_LOGIC,
    };

    // 编辑模块共享数据接口
    public interface ISharedDataHelper
    {
        void RegisterSharedDataInterface();
    }

    // 编辑模块接口
    public interface IModule
    {
        // 模块名称
        String Name
        {
            get;
        }

        // 模块是否能够使用
        bool Enable
        {
            set;
            get;
        }

        // 模块数据共享模式
        bool SharedMode
        {
            set;
            get;
        }

        // 模块类型
        EModuleType ModuleType
        {
            set;
            get;
        }

        // 模块版本
        UInt32 Version
        {
            get;
        }

        // 主文档对象
        MVC.IDocument MainDocument
        {
            get;
        }

        // 索引文档对象(用于多文档模块)
        MVC.IDocument ListDocument
        {
            get;
        }

        // 数据共享
        ISharedDataHelper SharedDataHelper
        {
            get;
        }

        // 初始化与销毁模块
        bool Initialize();
        void Destroy();

        // 进入与退出模块
        bool Enter();
        void Leave();

        // 文档处理
        bool NewDocument();
        bool OpenDocument(String kPath);
        bool CloseAllFile();
        bool SaveAllFile();

//      IDocument CreateDocument();
//      IDocument CreateListDocument();
//      ISharedDataHelper CreateSharedDataHelper();
    }

    //
}
