using System;
using System.Collections.Generic;
using System.Configuration;
using System.IO;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

using NLog;

namespace Utilities
{
    public interface ILogger
    {
        void Trace<T>(String msg,
                    [System.Runtime.CompilerServices.CallerMemberName] string caller = null,
                    [System.Runtime.CompilerServices.CallerLineNumber] int lineNumber = 0,
                    [System.Runtime.CompilerServices.CallerFilePath] string file = null);
        void Debug<T>(String msg,
                [System.Runtime.CompilerServices.CallerMemberName] string caller = null,
                [System.Runtime.CompilerServices.CallerLineNumber] int lineNumber = 0,
                [System.Runtime.CompilerServices.CallerFilePath] string file = null);
        void Info<T>(String msg,
                    [System.Runtime.CompilerServices.CallerMemberName] string caller = null,
                    [System.Runtime.CompilerServices.CallerLineNumber] int lineNumber = 0,
                    [System.Runtime.CompilerServices.CallerFilePath] string file = null);
        void Error<T>(String msg,
                    [System.Runtime.CompilerServices.CallerMemberName] string caller = null,
                    [System.Runtime.CompilerServices.CallerLineNumber] int lineNumber = 0,
                    [System.Runtime.CompilerServices.CallerFilePath] string file = null);
        void Fatal<T>(String msg,
                    [System.Runtime.CompilerServices.CallerMemberName] string caller = null,
                    [System.Runtime.CompilerServices.CallerLineNumber] int lineNumber = 0,
                    [System.Runtime.CompilerServices.CallerFilePath] string file = null);
        void Exception<T>(Exception x, string msg1 = "",
                [System.Runtime.CompilerServices.CallerMemberName] string caller = null,
                [System.Runtime.CompilerServices.CallerLineNumber] int lineNumber = 0,
                [System.Runtime.CompilerServices.CallerFilePath] string file = null);

    }

    public class NLogLogger : ILogger
    {
        protected Logger GetLogger<T>() => LogManager.GetLogger(typeof(T).Name);
        protected void Log<T>(LogLevel level, string msg, string caller, int line, string file)
        {
            lock (Locker)
            {
                if (LogManager.Configuration != null)
                {
                    var filename = Path.GetFileName(file);
                    //var from = $"[{filename}:{caller ?? ""}:{line}]";
                    var from = $"[{filename}:{line}]";
                    var logger = GetLogger<T>();
                    //var eventInfo = new LogEventInfo(level, typeof(T).Name, msg);
                    //eventInfo.Properties["where"] = from;
                    //eventInfo.Properties["master"] = key;
                    //eventInfo.Properties["inci"] = subkey;
                    //logger.Log(eventInfo);
                    //logger.Log(level, msgTemplate, filename, line, key, subkey, msg);
                    logger.Log(level, $"{from} => {msg}");
                }
            }
        }
        public void Trace<T>(String msg,
            [System.Runtime.CompilerServices.CallerMemberName] string caller = null,
            [System.Runtime.CompilerServices.CallerLineNumber] int lineNumber = 0,
            [System.Runtime.CompilerServices.CallerFilePath] string file = null)
        {
            Log<T>(LogLevel.Trace, msg, caller, lineNumber, file);
        }
        public void Debug<T>(String msg,
            [System.Runtime.CompilerServices.CallerMemberName] string caller = null,
            [System.Runtime.CompilerServices.CallerLineNumber] int lineNumber = 0,
            [System.Runtime.CompilerServices.CallerFilePath] string file = null)
        {
            Log<T>(LogLevel.Debug, msg, caller, lineNumber, file);
        }
        public void Info<T>(String msg,
            [System.Runtime.CompilerServices.CallerMemberName] string caller = null,
            [System.Runtime.CompilerServices.CallerLineNumber] int lineNumber = 0,
            [System.Runtime.CompilerServices.CallerFilePath] string file = null)
        {
            Log<T>(LogLevel.Info, msg, caller, lineNumber, file);
        }
        public void Warn<T>(String msg,
            [System.Runtime.CompilerServices.CallerMemberName] string caller = null,
            [System.Runtime.CompilerServices.CallerLineNumber] int lineNumber = 0,
            [System.Runtime.CompilerServices.CallerFilePath] string file = null)
        {
            Log<T>(LogLevel.Warn, msg, caller, lineNumber, file);
        }
        public void Error<T>(String msg,
            [System.Runtime.CompilerServices.CallerMemberName] string caller = null,
            [System.Runtime.CompilerServices.CallerLineNumber] int lineNumber = 0,
            [System.Runtime.CompilerServices.CallerFilePath] string file = null)
        {
            Log<T>(LogLevel.Error, msg, caller, lineNumber, file);
        }
        public void Fatal<T>(String msg,
            [System.Runtime.CompilerServices.CallerMemberName] string caller = null,
            [System.Runtime.CompilerServices.CallerLineNumber] int lineNumber = 0,
            [System.Runtime.CompilerServices.CallerFilePath] string file = null)
        {
            Log<T>(LogLevel.Fatal, msg, caller, lineNumber, file);
        }
        public void Exception<T>(Exception x, string msg1 = "",
            [System.Runtime.CompilerServices.CallerMemberName] string caller = null,
            [System.Runtime.CompilerServices.CallerLineNumber] int lineNumber = 0,
            [System.Runtime.CompilerServices.CallerFilePath] string file = null)
        {
            while (x != null)
            {
                var msg = msg1 + " [EXCEPTION]: " + x.Message; // + (x.InnerException != null ? (" [INNER EXCEPTION]: " + x.InnerException.Message) : "");
                Log<T>(LogLevel.Error, msg, caller, lineNumber, file);
                Log<T>(LogLevel.Trace, $"[EXCEPTION TRACE] => {x}", caller, lineNumber, file);
                x = x.InnerException;
            }
            //Uv5kLog.GetLogger<T>().Trace($"[EXCEPTION TRACE] => {x}");
        }
        object Locker { get; set; } = new object();
    }
}
