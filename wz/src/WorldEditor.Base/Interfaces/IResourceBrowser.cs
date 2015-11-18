using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;

namespace WorldEditor.Base.Interfaces
{
    // 资源浏览器事件参数
    public class BrowserArgs : EventArgs
    {
        public Object mResObj;  ///< 当前资源对象
        public Object mObjTag;  ///< 
    }

    // 选择被激活后事件(双击)
    public delegate void BrowserActionHandler(Object sender, BrowserArgs e);

    // 选择改变后事件(单击)
    public delegate void BrowserSelectionChandedhandler(Object sender, BrowserArgs e);

    // 资源浏览接口
    public interface IResourceBrowser
    {
        // 获取当前选中资源
        Object GetResObject();
        Object[] GetResObjects();

        // 设置初始选中资源
        void SetResObject(String strResName);

        // 获取当前选中的Tag对象
        Object GetTagObject();
        Object[] GetTagObjects();

        // 浏览器的资源类型
        String ResType
        {
            get;
        }

        // 浏览器的显示名称
        String DisplayName
        {
            get;
        }

        // 浏览器的显示图片
        Bitmap TabImage
        {
            get;
        }

        // 刷新
        void RefreshBrowser();

        // 选择被激活(双击)
        event BrowserActionHandler ResourceActived;

        // 选择发生变化(单击)
        event BrowserSelectionChandedhandler ResourceSelectionChanged;
    }
}
