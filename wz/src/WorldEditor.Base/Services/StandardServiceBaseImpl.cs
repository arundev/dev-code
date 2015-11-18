//*************************************************************************************************
// 内容:	事件(参考自WZEditor.Base)
//---------------------------------------------------------
// 作者:		
// 创建:		2014-06-01
// 最后修改:    2014-06-04
//*************************************************************************************************
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WorldEditor.Base.Services
{
    // 实现Service最基本的接口
	public class StandardServiceBaseImpl : Interfaces.Services.IService
    {
        public virtual string Name
        {
            get { return this.GetType().ToString(); }
        }

        public virtual bool Initialize()
        {
            return true;
        }

        public virtual void Destroy()
        {
			return;
        }

        public virtual bool Start()
        {
            return true;
        }

        public virtual bool End()
        {
            return true;
        }

        public virtual void Update()
        {
			return;
        }
    }
}
