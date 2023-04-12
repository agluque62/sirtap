using System;
using System.IO;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;
using System.Diagnostics;
using Microsoft.Win32.SafeHandles;
using System.Threading;

namespace Nucleo.CheckIO
{
    /// <summary>
    /// 
    /// </summary>
    public static class Kernel32
    {
        public const uint GENERIC_READ = 0x80000000;
        public const uint GENERIC_WRITE = 0x40000000;
        public const uint OPEN_EXISTING = 3;
        public const uint FILE_FLAG_OVERLAPPED = 0x40000000;
        public const uint FILE_SHARE_READ = 1;
        public const uint FILE_SHARE_WRITE = 2;

        [Flags]
        public enum ProcessAccessFlags : int
        {
            All = 0x001F0FFF,
            Terminate = 0x00000001,
            CreateThread = 0x00000002,
            VMOperation = 0x00000008,
            VMRead = 0x00000010,
            VMWrite = 0x00000020,
            DupHandle = 0x00000040,
            SetInformation = 0x00000200,
            QueryInformation = 0x00000400,
            Synchronize = 0x00100000
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct SystemTime
        {
            public short Year;
            public short Month;
            public short DayOfWeek;
            public short Day;
            public short Hour;
            public short Minute;
            public short Second;
            public short Millisecond;
        }

        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern bool AllocConsole();

        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern bool FreeConsole();

        [DllImport("kernel32", SetLastError = true)]
        public static extern bool AttachConsole(int dwProcessId);

        [DllImport("kernel32.dll")]
        public static extern bool SetConsoleTitle(string lpConsoleTitle);

        [DllImport("kernel32.dll")]
        public static extern IntPtr OpenProcess(ProcessAccessFlags dwDesiredAccess, [MarshalAs(UnmanagedType.Bool)] bool bInheritHandle, int dwProcessId);

        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern IntPtr CreateFile([In] string strName, uint nAccess, uint nShareMode, IntPtr lpSecurity, uint nCreationFlags, uint nAttributes, IntPtr lpTemplate);

        [DllImport("kernel32.dll", SetLastError = true)]
        [return: MarshalAs(UnmanagedType.Bool)]
        public static extern bool CloseHandle(IntPtr hObject);

        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern int SetLocalTime(ref SystemTime s);

        [DllImport("kernel32.dll")]
        public static extern bool WritePrivateProfileString(string lpAppName, string lpKeyName, string lpString, string lpFileName);

        [DllImport("kernel32.dll")]
        public static extern uint GetPrivateProfileString(string lpAppName, string lpKeyName, string lpDefault, StringBuilder lpReturnedString, int nSize, string lpFileName);
    }
    
    /// <summary>
    /// 
    /// </summary>
    public abstract class ISoundDevice
	{
        /// <summary>
        /// 
        /// </summary>
        public event Action<object, bool, bool> StateChanged = delegate { };

        public byte Id
        {
            get { return _id; }
            set { _id = value; }
        }

        public bool Jack
        {
            get { return _jack; }
        }

        public bool Ptt
        {
            get { return _ptt; }
        }

        public bool Gpio6
        {
            get { return _gpio6; }
        }

        public bool Gpio7
        {
            get { return _gpio7; }
        }


        public virtual bool IsValid
        {
            get { return (_id < 16); }
        }

        /// <summary>
        /// 
        /// </summary>
        public bool Error { get; set; }

        /// <summary>
        /// 
        /// </summary>
        public virtual void Start(){}
        public virtual void GetState(){}
        public virtual void Close(){}
        public virtual bool SetGpio(int gpio, byte estado){return false;}
        public virtual bool GetGpio(out byte _ret) { _ret = 0; return false; }

        /// <summary>
        /// 
        /// </summary>
        protected object _sync = new object();
        protected byte _id = 255;
        protected bool _jack;
        protected bool _ptt;
        protected bool _gpio6, _gpio7;


        protected void SetState(bool jack, bool ptt, bool notif = true)
        {
            bool changes = false;

            if (Error==true)
                StateChanged(this, _jack, _ptt);

            if (_jack != jack)
            {
                _jack = jack;
                changes = true;
            }
            if (_ptt != ptt)
            {
                _ptt = ptt;
                changes = true;
            }
            if (changes && notif)
            {
                StateChanged(this, _jack, _ptt);
            }
        }
    }

    /// <summary>
    /// 
    /// </summary>
	public abstract class ISndDevManager
	{
        /// <summary>
        /// 
        /// </summary>
        public static SynchronizationContext _context;
        static public SynchronizationContext Context
        {
            get { return _context; }
            set { _context = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public virtual byte Init() { return 0; }

        /// <summary>
        /// 
        /// </summary>
        public void Start()
        {
            foreach (ISoundDevice sndDev in _sndDevs.Values)
            {
                sndDev.Error = false;
                sndDev.Start();
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public void Close() 
        {
            foreach (ISoundDevice sndDev in _sndDevs.Values)
            {
                sndDev.Close();
            }
            _sndDevs.Clear();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="id"></param>
        /// <returns></returns>
        public virtual ISoundDevice Device(byte id) 
        {
            ISoundDevice dev = null;
            if (_sndDevs.TryGetValue(id, out dev))
            {
                return dev;
            }

            return null;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public virtual SortedDictionary<byte, ISoundDevice> ListDevice() 
        {
            return _sndDevs;
        }

        /// <summary>
        /// 
        /// </summary>
        protected SortedDictionary<byte, ISoundDevice> _sndDevs = new SortedDictionary<byte, ISoundDevice>();

    }
}
