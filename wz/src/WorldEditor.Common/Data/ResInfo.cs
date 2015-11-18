using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Xml.Serialization;

namespace WorldEditor.Common.Data
{
    //
    //  资源对象
    //
    //  [Serializable()] 类可以被序列化
    //  [TypeConverter()] 提供一种将值的类型转换为其他类型以及访问标准值和子属性的统一方法
    //
    [Serializable()]
    [TypeConverter(typeof(ExpandableObjectConverter))]
    class ResInfo
    {
        #region 成员变量

        private String mKey;
        private UInt32 mID;
        private String mDisplayName;    ///< 资源显示名称
        private String mResType;        ///< 资源类型
        private String mResName;        ///< 资源名称
        private String mResFileName;    ///< 资源对象
        private String mCategory;       ///< 类别
        private String mAuthor;         ///< 作者
        private bool mRef;              ///< 资源能否被引用
        private bool mCopy;             ///< 资源能否被拷贝
        private bool mInProject;    
        private Object mTag;            ///< 标签
        private List<ResInfo> mAdditionalReses; 
        private List<ResInfo> mRefReses; 
        private List<String>  mContanPathes;  

        #endregion

        #region 属性

        [BrowsableAttribute(false), DisplayNameAttribute("键值"), CategoryAttribute("基本属性"), DescriptionAttribute("资源键值"), ReadOnlyAttribute(true)]
        [XmlAttribute("key")]
        public System.String Key
        {
            get {return mKey;}
            set {mKey = value;}
        }

        [BrowsableAttribute(false), DisplayNameAttribute("ID"), CategoryAttribute("基本属性"), DescriptionAttribute("资源ID"), ReadOnlyAttribute(true)]
        [XmlAttribute("id")]
        public System.UInt32 ID
        {
            get { return mID; }
            set { mID = value; }
        }

        [BrowsableAttribute(true), DisplayNameAttribute("显示名称"), CategoryAttribute("基本属性"), DescriptionAttribute("资源显示的名称"), ReadOnlyAttribute(false)]
        [XmlAttribute("name")]
        public System.String DisplayName
        {
            get { return mDisplayName; }
            set { mDisplayName = value; }
        }

        [BrowsableAttribute(false), DisplayNameAttribute("资源类型"), CategoryAttribute("基本属性"), DescriptionAttribute("资源的类型"), ReadOnlyAttribute(true)]
        [XmlAttribute("restype")]
        public System.String ResType
        {
            get { return mResType; }
            set { mResType = value; }
        }

        [BrowsableAttribute(true), DisplayNameAttribute("资源名"), CategoryAttribute("基本属性"), DescriptionAttribute("资源名"), ReadOnlyAttribute(true)]
        [XmlAttribute("resname")]
        public System.String ResName
        {
            get { return mResName; }
            set { mResName = value; }
        }

        [BrowsableAttribute(true), DisplayNameAttribute("文件名"), CategoryAttribute("基本属性"), DescriptionAttribute("资源文件名,资源在物理磁盘上的位置"), ReadOnlyAttribute(true)]
        [XmlIgnoreAttribute]
        public System.String FileName
        {
            get { return mResFileName; }
        }

        [BrowsableAttribute(false), DisplayNameAttribute("类别"), CategoryAttribute("基本属性"), DescriptionAttribute("资源的类别,用于资源的分类"), ReadOnlyAttribute(false)]
        [XmlAttribute("category")]
        public System.String Category
        {
            get { return mCategory; }
            set { mCategory = value; }
        }

        [BrowsableAttribute(true), DisplayNameAttribute("作者"), CategoryAttribute("开放属性"), DescriptionAttribute("资源的作者"), ReadOnlyAttribute(true)]
        [XmlAttribute("author")]
        public System.String Author
        {
            get { return mAuthor; }
            set { mAuthor = value; }
        }

        [BrowsableAttribute(true), DisplayNameAttribute("引用"), CategoryAttribute("开放属性"), DescriptionAttribute("资源是否能够被引用"), ReadOnlyAttribute(false)]
        [XmlAttribute("ref")]
        public bool Ref
        {
            get { return mRef; }
            set { mRef = value; }
        }

        [BrowsableAttribute(true), DisplayNameAttribute("拷贝"), CategoryAttribute("开放属性"), DescriptionAttribute("资源是否能够被拷贝"), ReadOnlyAttribute(false)]
        [XmlAttribute("copy")]
        public bool Copy
        {
            get { return mCopy; }
            set { mCopy = value; }
        }

        [BrowsableAttribute(false)]
        [XmlIgnoreAttribute]
        public bool InProject
        {
            get { return mInProject; }
            set { mInProject = value; }
        }

        [BrowsableAttribute(false)]
        [XmlIgnoreAttribute]
        public Object Tag
        {
            get { return mTag; }
            set { mTag = value; }
        }

        [BrowsableAttribute(false)]
        [XmlElement(ElementName = "depend", IsNullable = true)]
        public List<ResInfo> AdditionalReses
        {
            get { return mAdditionalReses; }
        }

        [BrowsableAttribute(false)]
        [XmlElement(ElementName = "ref", IsNullable = true)]
        public List<ResInfo> RefReses
        {
            get { return mRefReses; }
        }

        [BrowsableAttribute(false)]
        [XmlIgnoreAttribute]
        public List<String> ContanPathes
        {
            get { return mContanPathes; }
        }

        #endregion

        #region 公开方法



        #endregion

        #region 重载基类

        public override bool Equals(object obj)
        {
            ResInfo kResInfo = obj as ResInfo;
            if (kResInfo == null)
                return false;
            return FileName.Equals(kResInfo.FileName);
        }

        public override int GetHashCode()
        {
            return base.GetHashCode();
        }

        #endregion

        #region 构造函数
        
        public ResInfo()
            : this("", "", "", "")
        {
            
        }

        public ResInfo(String strDisplayName, String strResType, String strResName, String strFileName)
        {
            mDisplayName = strDisplayName.ToLower();
            mResType = strResType;
            mResName = strResName.ToLower();
            mResFileName = strFileName.ToLower();
            mInProject = true;
            mAdditionalReses = new List<ResInfo>();
            mRefReses = new List<ResInfo>();
            mContanPathes = new List<String>();
        }

        #endregion
    }
}
