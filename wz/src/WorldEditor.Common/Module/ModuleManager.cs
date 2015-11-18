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
using WorldEditor.Base.Interfaces.Services;
using WorldEditor.Base.Services;
using WorldEditor.Common.Services;

namespace WorldEditor.Common.Module
{
    // 编辑模块管理器
    public class ModuleManager : WorldEditor.Base.TSingleton<ModuleManager>
    {
		// 模块
		protected Dictionary<String, IModule> mModules = new Dictionary<String, IModule>();

		// 初始化
		public virtual bool Initialize()
		{


			return true;
		}

		// 销毁
		public virtual void Destroy()
		{
			foreach (KeyValuePair<String, IModule> kPair in mModules)
			{
				kPair.Value.Destroy();
			}

			mModules.Clear();
		}

        // 加载插件
		protected void RegisterPlugin(String kAssemblyName)
		{
		    System.Reflection.Assembly kAssembly = AppDomain.CurrentDomain.Load(kAssemblyName);
		    if (kAssembly == null)
		    {
		        ServiceManager.Instance.LoggingService.DoLog(
					String.Format("插件加载失败{0:s}", kAssemblyName),
					ELogLevel.E_LL_ERROR);

		        return;
		    }

			foreach (Type kType in kAssembly.GetExportedTypes())
			{
				if (!kType.IsSubclassOf(typeof(IModule)))
					continue;

				ServiceManager.Instance.LoggingService.DoLog(
					String.Format("开始注册模块{0:s}", kType.ToString()),
					ELogLevel.E_LL_MESSAGE);

				foreach (ModuleAttribute kModuleAttribute in kType.GetCustomAttributes(typeof(ModuleAttribute), false))
				{
					IModule kModule = Activator.CreateInstance(kType) as IModule;
					RegisterModule(kModule);

					break;
				}
			}
		}

		// 注册模块
		protected void RegisterModule(IModule kModule)
		{
			if (kModule == null)
				return;

			IModule kExistModule = null;
			if (mModules.TryGetValue(kModule.Name, out kExistModule))
				return;

			mModules.Add(kModule.Name, kModule);
			kModule.Initialize();
		}

		// 反注册模块
		protected void UnregisterModule(IModule kModule)
		{
			if (kModule == null)
				return;

			IModule kExistModule = null;
			if (mModules.TryGetValue(kModule.Name, out kExistModule))
			{
				if (kModule == kExistModule)
				{
					mModules.Remove(kModule.Name);
					kModule.Destroy();
				}
			}
		}
    }
}
