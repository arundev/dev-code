//*************************************************************************************************
// 内容:	(参考自WZEditor.Common)
//---------------------------------------------------------
// 作者:		
// 创建:		2014-05-29
// 最后修改:    
//*************************************************************************************************
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using WorldEditor.Base.Interfaces;
using WorldEditor.Base.Interfaces.MVC;

namespace WorldEditor.Common.Module
{
    //模块实现
    public class Module : IModule
    {
        // 模块名称(实现IModule成员变量)
        protected String mName;
        public String Name
        {
            get { return mName;}
        }

        // 模块是否能够使用(实现IModule成员变量)
        protected bool mEnable;
        public bool Enable
        {
            set { mEnable = value;}
            get { return mEnable;}
        }

        // 模块数据共享模式(实现IModule成员变量)
        protected bool bShareMode;
        public bool SharedMode
        {
            set { bShareMode = value;}
            get { return bShareMode;}
        }

        // 模块类型(实现IModule成员变量)
        protected EModuleType mModuleType;
        public EModuleType ModuleType
        {
            set { mModuleType = value;}
            get { return mModuleType;}
        }

        // 模块版本(实现IModule成员变量)
        protected UInt32 mVersion;
        public UInt32 Version
        {
            get { return mVersion;}
        }

        // 主文档对象(实现IModule成员变量)
        protected IDocument mMainDocument;
        public IDocument MainDocument
        {
            get { return mMainDocument;}
        }

        // 索引文档对象(实现IModule成员变量)
        protected IDocument mListDocument;
        public IDocument ListDocument
        {
            get { return mListDocument;}
        }

        // 数据共享(实现IModule成员变量)
        protected ISharedDataHelper mSharedDataHelper;
        public ISharedDataHelper SharedDataHelper
        {
            get { return mSharedDataHelper;}
        }

        // 初始化模块(实现IModule成员函数)
        public virtual bool Initialize()
        {
            return true;
        }

        // 销毁模块(实现IModule成员函数)
        public virtual void Destroy()
        {

        }

        // 进入模块(实现IModule成员函数)
        public virtual bool Enter()
        {
            return true;
        }

        // 退出模块(实现IModule成员函数)
        public virtual void Leave()
        {

        }

        // 新建文档(实现IModule成员函数)
        public virtual bool NewDocument()
        {
            return true;
        }

        // 打开文档(实现IModule成员函数)
        public virtual bool OpenDocument(String kPath)
        {
            return true;
        }

        // 关闭文档(实现IModule成员函数)
        public virtual bool CloseAllFile()
        {
            return true;
        }

        // 保存文档(实现IModule成员函数)
        public virtual bool SaveAllFile()
        {
            return true;
        }

        #region 界面命令

        protected void Module_Save(object sender, EventArgs args)
        {
            if (MainDocument == null)
                return;

            MainDocument.Save();
        }

        protected void Module_Close(object sender, EventArgs args)
        {
            if (MainDocument == null)
                return;

            MainDocument.Close();
        }

        #endregion
    }
}
