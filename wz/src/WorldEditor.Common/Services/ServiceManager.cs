//*************************************************************************************************
// 内容:	服务管理器(参考自WZEditor.Base)
//---------------------------------------------------------
// 作者:		
// 创建:		2014-06-04
// 最后修改:    2014-06-04
//*************************************************************************************************
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using WorldEditor.Base.Interfaces.Services;

namespace WorldEditor.Common.Services
{
	public class ServiceManager : Base.TSingleton<ServiceManager>
	{
		// 服务列表
		protected Hashtable mServices = new Hashtable();

		// 事件服务
		Base.Interfaces.Services.IEventService mEventService = null;
		public Base.Interfaces.Services.IEventService EventService
		{
			get { return mEventService; }
		}

		// 日志服务
		Base.Interfaces.Services.ILoggingService mLoggingService = null;
		public Base.Interfaces.Services.ILoggingService LoggingService
		{
			get { return mLoggingService; }
		}

		// 初始化
		public virtual bool Initialize()
        {
			mEventService = new Base.Services.EventServiceImpl();
			AddService(mEventService);

			mLoggingService = new Base.Services.LoggingServiceImpl();
			AddService(mLoggingService);

			InitializeService();
			StartService();

            return true;
        }

		// 销毁
        public virtual void Destroy()
        {
			foreach (IService kService in mServices)
			{
				kService.Destroy();
			}

			mServices.Clear();
        }

		// 初始化服务
		protected virtual bool InitializeService()
		{
			foreach (IService kService in mServices.Values)
			{
				kService.Initialize();
			}

			return true;
		}

		// 启动服务
		protected virtual bool StartService()
		{
			foreach (IService kService in mServices.Values)
			{
				kService.Start();
			}

			return true;
		}

		// 添加服务
		protected virtual bool AddService(IService kService)
		{
			IService kServiceExist = GetService(kService.GetType());
			if (kServiceExist == null)
				mServices.Add(kService.GetType(), kService);
			else
				throw new ArgumentException(
					"An attempt was made to add a service that already existed",
					kService.GetType().ToString());


			return true;
		}

		// 获取服务
		protected virtual IService GetService(Type kType)
		{
			foreach (IService kService in mServices.Values)
			{
				if (kType.IsAssignableFrom(kService.GetType()))
					return kService;
			}

			return null;
		}
	}
}
