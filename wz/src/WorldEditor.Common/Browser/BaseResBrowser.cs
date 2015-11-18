using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using WorldEditor.Base.Interfaces;

namespace WorldEditor.Common.Browser
{
    public partial class BaseResBrowser : UserControl, IResourceBrowser
    {
        #region 成员变量

        private String mInitResName = String.Empty; ///< 初始选中资源
                                       
        private String mResType = String.Empty;     ///< 资源类型
        private String mDisplayName = String.Empty;

        #endregion

        public BaseResBrowser()
        {
            InitializeComponent();
        }

        #region IResourceBrowser Members

        public virtual Object GetResObject()
        {
            Object[] kObjects = GetResObjects();
            if (kObjects == null || kObjects.Length == 0)
                return null;
            return kObjects[0];
        }

        public virtual Object[] GetResObjects()
        {
            return null;
        }

        public virtual void SetResObject(String strResName)
        {
            mInitResName = strResName;
        }

        public virtual Object GetTagObject()
        {
            Object[] kObjects = GetTagObjects();
            if (kObjects == null || kObjects.Length == 0)
                return null;
            return kObjects[0];
        }

        public virtual Object[] GetTagObjects()
        {
            return null;
        }

        public virtual String ResType
        {
            get
            {
                return mResType;
            }
        }

        public virtual String DisplayName
        {
            get
            {
                return mDisplayName;
            }
        }

        public virtual Bitmap TabImage
        {
            get
            {
                return null;
            }
        }

        public virtual void RefreshBrowser()
        {

        }

        public event BrowserActionHandler ResourceActived;
        protected void FireResourceActived()
        {
            if (ResourceActived != null)
            {
                BrowserArgs e = new BrowserArgs();
                e.mResObj = GetResObject();
                e.mObjTag = GetTagObject();
                ResourceActived(this, e);
            }
        }

        public event BrowserSelectionChandedhandler ResourceSelectionChanged;
        protected void FireResourceSelectionChanged()
        {
            if (ResourceSelectionChanged != null)
            {
                BrowserArgs e = new BrowserArgs();
                e.mResObj = GetResObject();
                e.mObjTag = GetTagObject();
                ResourceSelectionChanged(this, e);
            }
        }

        #endregion
    }
}
