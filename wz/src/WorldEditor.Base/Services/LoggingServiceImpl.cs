//*************************************************************************************************
// 内容:	日志(参考自WZEditor.Base)
//
// 参考资料：
//   1.关于C#数据流(Stream)
//     Stream
//     StreamReader/TextReader
//     StreamWriter/TextWriter
//     FileStream
//     MemoryStream
//     BufferredStream
//     NetworkStream
//---------------------------------------------------------
// 作者:		
// 创建:		2014-05-29
// 最后修改:    2014-06-04
//*************************************************************************************************
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using WorldEditor.Base.Interfaces.Services;

namespace WorldEditor.Base.Services
{
    // 日志事件通知
    public delegate void LoggingServiceEventHandler(object sender, LogEvent kEvents);

    // 日志事件
    public class LogEvent
    {
        private ELogLevel mLevel;
        public ELogLevel Level
        {
            get { return mLevel;}
        }

        private String mMessage;
        public String Message
        {
            get { return mMessage;}
        }

        private DateTime mTime;
        public DateTime Time
        {
            get { return mTime;}
        }

        public LogEvent(String kMsg, DateTime kTime, ELogLevel eLevel)
        {
            this.mMessage = kMsg;
            this.mTime = kTime;
            this.mLevel = eLevel;
        }
    }

    // 日志服务实现
    public class LoggingServiceImpl : StandardServiceBaseImpl, Interfaces.Services.ILoggingService
    {
		// 日志事件订阅
		public event LoggingServiceEventHandler LoggingEventHandler;

		// 标记是否立即输入到日志文件
        protected bool mEnable;
        public bool Enable
        {
            get { return mEnable;}
            set { mEnable = value;}
        }

        protected Queue<LogEvent> mLogQueue;  ///< 日志记录队列
        protected StreamWriter mStreamWriter; ///< 日志输出流

        public LoggingServiceImpl()
        {
            // 创建文件流
            FileStream fileStream = new FileStream("WorldEditor.log",
                FileMode.Append, 
				FileAccess.Write, 
				FileShare.ReadWrite, 
				1024, 
				false);

            // 创建输出数据流
            mStreamWriter = new StreamWriter(fileStream);
            mStreamWriter.AutoFlush = false;

            // 创建消息队列
            mLogQueue = new Queue<LogEvent>();
        }

		// 更新(虚函数继承,继承自IService)
		public override void Update()
		{
			// 将缓存的日志写入日志文件
			LogEvent[] akLogEvent = null;
			Thread.BeginCriticalRegion();
			if (mLogQueue.Count > 0)
			{
				akLogEvent = mLogQueue.ToArray();
				mLogQueue.Clear();
			}
			Thread.EndCriticalRegion();

			if (akLogEvent != null)
				DoLogEvent(akLogEvent);
		}

		// 记录日志(虚函数继承,继承自ILoggingService)
		public virtual void DoLog(String kMsg, ELogLevel eLevel)
        {
			DoLog(kMsg, eLevel, false);
        }

		// 记录日志(虚函数继承,继承自ILoggingService)
		public virtual void DoLog(String kMsg, ELogLevel eLevel, bool bImmediate)
		{
			LogEvent kLogEvent = new LogEvent(kMsg, DateTime.Now, eLevel);
			if (bImmediate)
			{
				DoLogEvent(kLogEvent);
			}
			else
			{
				// 加入队列
				Thread.BeginCriticalRegion();
				mLogQueue.Enqueue(kLogEvent);
				Thread.EndCriticalRegion();
			}
		}

		// 将日志事件直接写入日志
		protected void DoLogEvent(LogEvent kLogEvent)
		{
			if (LoggingEventHandler != null)
				LoggingEventHandler(this, kLogEvent);

			mStreamWriter.WriteLine(String.Format("{0:s}[{1}]{2:s}", kLogEvent.Time.ToString(), kLogEvent.Level, kLogEvent.Message));
			mStreamWriter.Flush();
		}

		// 将日志事件直接写入日志
		protected void DoLogEvent(LogEvent[] akLogEvent)
		{
			if (LoggingEventHandler != null)
			{
				foreach (LogEvent kLogEvent in akLogEvent)
					LoggingEventHandler(this, kLogEvent);
			}

			foreach (LogEvent kLogEvent in akLogEvent)
			{
				mStreamWriter.WriteLine(String.Format("{0:s}[{1}]{2:s}", kLogEvent.Time.ToString(), kLogEvent.Level, kLogEvent.Message));
			}

			mStreamWriter.Flush();
		}
    }
}
