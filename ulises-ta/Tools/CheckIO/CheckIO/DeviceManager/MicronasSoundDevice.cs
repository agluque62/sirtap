using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Runtime.InteropServices;
using System.Diagnostics;
using Microsoft.Win32.SafeHandles;
using System.Threading;

namespace Nucleo.CheckIO
{
    class MicronasSoundDevice : ISoundDevice
    {
        /// <summary>
        /// 
        /// </summary>
        /// <param name="usbHandle"></param>
        /// <param name="devPath"></param>
		public MicronasSoundDevice(IntPtr usbHandle, string devPath)
		{
			IntPtr devFileHandle = Kernel32.CreateFile(devPath, Kernel32.GENERIC_READ | Kernel32.GENERIC_WRITE,
				Kernel32.FILE_SHARE_READ | Kernel32.FILE_SHARE_WRITE, IntPtr.Zero, Kernel32.OPEN_EXISTING,
				Kernel32.FILE_FLAG_OVERLAPPED, IntPtr.Zero);

			_devHandle = new SafeFileHandle(devFileHandle, true);
			if (!_devHandle.IsInvalid)
			{
				byte[] data = new byte[1];
				if (UacGetMem(usbHandle, UAC2_GPI_REG, 1, data) == 1)
				{
					_id = (byte)((data[0] >> 3) & 0x7);
					_usbHandle = usbHandle;
				}
			}
		}
        /// <summary>
        /// 
        /// </summary>
		public override void Start()
		{
			Debug.Assert(IsValid);
            #if !_SIMULADOR_
			byte[] buffer = new byte[3];

			_stream = new FileStream(_devHandle, FileAccess.Read, 3, true);
			_stream.BeginRead(buffer, 0, 3, new AsyncCallback(ReadCompleted), buffer);

			byte[] data = new byte[1];
			if (UacGetMem(_usbHandle, UAC2_GPI_REG, 1, data) == 1)
			{
                bool jack = ((data[0] & JACK_FLAG) == JACK_FLAG);
                bool ptt = ((data[0] & PTT_FLAG) == PTT_FLAG);

				SetState(jack, ptt);

                _gpio6 = (data[0] & 0x40) == 0x40;
                _gpio7 = (data[0] & 0x80) == 0x80;
            }
            #endif
		}
        /// <summary>
        /// 
        /// </summary>
        public override void GetState()
        {
            byte[] data = new byte[1];
            if (UacGetMem(_usbHandle, UAC2_GPI_REG, 1, data) == 1)
            {
                bool jack = ((data[0] & JACK_FLAG) == JACK_FLAG);
                bool ptt = ((data[0] & PTT_FLAG) == PTT_FLAG);

                SetState(jack, ptt);
            
                _gpio6 = (data[0] & 0x40) == 0x40;
                _gpio7 = (data[0] & 0x80) == 0x80;
            }
        }
        /// <summary>
        /// 
        /// </summary>
		public override void Close()
		{
			if (_stream != null)
			{
				_stream.Close();
				_stream = null;
			}
			else if (_devHandle != null)
			{
				_devHandle.Close();
			}
		}
        /// <summary>
        /// 
        /// </summary>
        /// <param name="gpio"></param>
        /// <param name="estado"></param>
        /// <returns></returns>
        public override bool SetGpio(int gpio, byte estado)
        {
            byte mask = gpio == 6 ? (byte)0x40 : (byte)0x80;
            byte[] datos = new byte[1];

            // Recuperar estado actual
            // datos[0] = GetGpio();
            GetGpio(out datos[0]);

            datos[0] = (estado == 0 ? (byte)(datos[0] & ((byte)(~mask))) : (byte)(datos[0] | mask));
            /*return */UacSetMem(_usbHandle, UAC2_GPO_REG, 1, datos);
            return true;
        }
        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public override bool GetGpio(out byte _ret)
        {
            byte[] datos = new byte[1];
            if (UacGetMem(_usbHandle, UAC2_GPI_REG, 1, datos) == 1)
            {
                _gpio6 = (datos[0] & 0x40) == 0x40;
                _gpio7 = (datos[0] & 0x80) == 0x80;

                _ret = datos[0];
                return true;
            }
            _ret = 0;
            return false;
        }

		#region Private Members

		private const int JACK_FLAG = 0x01;
		private const int AUC_FLAG = 0x02;
		private const int PTT_FLAG = 0x04;

        private IntPtr _usbHandle;
		private FileStream _stream;
		private SafeFileHandle _devHandle;

		#region Uac2 Dll Interface

		private const int UAC2_GPI_REG = 0xB0A2;
		private const int UAC2_GPO_REG = 0xB0A0;

		[DllImport("uac2", EntryPoint = "_UacSetMem@16", CharSet = CharSet.Ansi, ExactSpelling = true)]
		static extern int UacSetMem(IntPtr uacHandle, int addr, int dataLen, [In] byte[] data);
		[DllImport("uac2", EntryPoint = "_UacGetMem@16", CharSet = CharSet.Ansi, ExactSpelling = true)]
		static extern int UacGetMem(IntPtr uacHandle, int addr, int dataLen, [Out, MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 2)] byte[] data);

        #endregion
        /// <summary>
        /// 
        /// </summary>
        /// <param name="iResult"></param>
		private void ReadCompleted(IAsyncResult iResult)
		{
			try
			{
                lock (_sync)
                {
                    _stream.EndRead(iResult);

                    byte[] buffer = (byte[])iResult.AsyncState;
                    byte state = buffer[1];

                    byte[] data = new byte[1];
                    byte[] data1 = new byte[1];
                    data1[0] = state;
                    if (UacGetMem(_usbHandle, UAC2_GPI_REG, 1, data) == 1)
                    {
                        while (data[0] != data1[0])
                        {
                            Thread.Sleep(50);
                            data1[0] = data[0];
                            UacGetMem(_usbHandle, UAC2_GPI_REG, 1, data);
                        }
                        state = data[0];
                    }

                    MicronasDevManager.Context.Post(delegate
                    {
                        if (!_devHandle.IsClosed)
                        {
                            bool jack = ((state & JACK_FLAG) == JACK_FLAG);
                            bool ptt = ((state & PTT_FLAG) == PTT_FLAG);

                            SetState(jack, ptt);
                        }
                    },
                    "SoundDevice." + _id + ".SetState");

                    _stream.BeginRead(buffer, 0, 3, new AsyncCallback(ReadCompleted), buffer);
                }
			}
			catch (Exception)
			{
				if (!_devHandle.IsClosed)
				{
					_stream.Close();
                    MicronasDevManager.Context.Post(delegate 
					{
						SetState(false, false);
					},
					"SoundDevice." + _id + ".SetState");
				}
			}
        }

        #endregion
    }
}
