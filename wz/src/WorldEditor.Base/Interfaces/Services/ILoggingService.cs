//*************************************************************************************************
// 内容:	(参考自WZEditor.Base)
//---------------------------------------------------------
// 作者:		
// 创建:		2014-05-29
// 最后修改:    2014-05-29
//*************************************************************************************************
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WorldEditor.Base.Interfaces.Services
{
    // 日志等级
    public enum ELogLevel
    {
        E_LL_ERROR,
        E_LL_WARNNING,
        E_LL_MESSAGE,
        E_LL_INFO,
        E_LL_DEBUG,
        E_LL_STATUS,
    }

    // 编辑器日志接口
    public interface ILoggingService : IService
    {
        // 记录日志
        void DoLog(String kMsg, ELogLevel eLevel);
		void DoLog(String kMsg, ELogLevel eLevel, bool bImmediate);
    }
}
