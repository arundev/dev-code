//*************************************************************************************************
// 内容:	(参考自WZEditor.Common)
//---------------------------------------------------------
// 作者:		
// 创建:		2014-06-04
// 最后修改:    2014-06-04
//*************************************************************************************************
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WorldEditor.Common.Module
{
	[AttributeUsage(AttributeTargets.Class, AllowMultiple = false)]
	public class ModuleAttribute : Attribute
	{
		protected String mDisplayName = String.Empty;
		public String DisplayName
		{
			set { mDisplayName = value; }
			get { return mDisplayName; }
		}

		protected String mDescription = String.Empty;
		public String Description
		{
			set { mDescription = value; }
			get { return mDescription; }
		}

		protected String mImage = String.Empty;
		public String Image
		{
			set { mImage = value; }
			get { return mImage; }
		}

		protected bool mShareMode = false;
		public bool ShareMode
		{
			set { mShareMode = value; }
			get { return mShareMode; }
		}

//		protected bool mSimplified = false;
//      public bool Simplified
//      {
//          set { mSimplified = value; }
//          get { return mSimplified; }
//      }
	}
}
