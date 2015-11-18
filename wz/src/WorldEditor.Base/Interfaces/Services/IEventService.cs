//*************************************************************************************************
// 内容:	(参考自WZEditor.Base)
//---------------------------------------------------------
// 作者:		
// 创建:		2014-05-29
// 最后修改:    2014-06-01
//*************************************************************************************************
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WorldEditor.Base.Interfaces.Services
{
    // 消息事件
    public class EventKey : Object
    {

    }

    // 消息中心
    public interface IEventService : IService
    {
		// 注册事件
        void RegisterEvent(EventKey kKey, EventHandler kEvent);

		// 反注册事件
        void UnregisterEvent(EventKey kKey, EventHandler kEvent);

		// 触发事件
        void FireEvent(EventKey kKey, Object kSender, EventArgs kArg);
    }
}
