//*************************************************************************************************
// 内容:	单件(参考自WZEditor.Base)
//---------------------------------------------------------
// 作者:		
// 创建:		2014-06-01
// 最后修改:    2014-06-01
//*************************************************************************************************
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WorldEditor.Base
{
    // 单件
    public class TSingleton<T> where T : new ()
    {
        private static T mInstance = default(T);
        public static T Instance
        {
            get
            {
				if (mInstance == null)
                {
                    try
                    {
						mInstance = new T();
                    }
                    catch (System.Exception ex)
                    {
                        System.Windows.Forms.MessageBox.Show(ex.ToString());
                    }
                }

				return mInstance;
            }
        }

        protected TSingleton()
		{
			
		}
    }
}
