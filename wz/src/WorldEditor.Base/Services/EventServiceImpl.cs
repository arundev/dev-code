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
using WorldEditor.Base.Interfaces.Services;

namespace WorldEditor.Base.Services
{
	// 用于多事件类型的事件集合，提供线程访问安全，节约多事件类型事件代理存储
	public class EventSet
	{
		// 事件映射表
		protected Dictionary<EventKey, Delegate> mEventDict = new Dictionary<EventKey, Delegate>();

		// 添加事件映射，组合存在的代理
		public void Add(EventKey kEventKey, Delegate kHandler)
		{
			lock (mEventDict)
			{
				Delegate kTemp;
				mEventDict.TryGetValue(kEventKey, out kTemp);
				mEventDict[kEventKey] = Delegate.Combine(kTemp, kHandler);
			}
		}

		// 移除事件映射，移除存在的代理
		public void Remove(EventKey kEventKey, Delegate kHandler)
		{
			lock (mEventDict)
			{
				Delegate kTemp;
				if (mEventDict.TryGetValue(kEventKey, out kTemp))
				{
					kTemp = Delegate.Remove(kTemp, kHandler);
					if (kTemp != null)
						mEventDict[kEventKey] = kTemp;
					else
						mEventDict.Remove(kEventKey);
				}
			}
		}

		// 触发事件
		public void Raise(EventKey kEventKey, Object kSender, EventArgs kArg)
		{
			Delegate kHandler;
			lock (mEventDict)
			{
				mEventDict.TryGetValue(kEventKey, out kHandler);
			}

			if (kHandler != null)
				kHandler.DynamicInvoke(new Object[] { kSender, kArg });
		}
	}


	// 事件服务实现
	public class EventServiceImpl : StandardServiceBaseImpl, Interfaces.Services.IEventService
	{
		// 事件集合
		protected EventSet mEventSet = new EventSet();

		// 注册事件(虚函数继承,继承自ILoggingService)
		public virtual void RegisterEvent(EventKey kKey, EventHandler kEvent)
		{
			mEventSet.Add(kKey, kEvent);
		}

		// 反注册事件(虚函数继承,继承自IEventService)
		public virtual void UnregisterEvent(EventKey kKey, EventHandler kEvent)
		{
			mEventSet.Remove(kKey, kEvent);
		}

		// 触发事件(虚函数继承,继承自IEventService)
		public virtual void FireEvent(EventKey kKey, Object kSender, EventArgs kArg)
		{
			mEventSet.Raise(kKey, kSender, kArg);
		}
	}
}
