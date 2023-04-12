#define _GPIO_POLLING_
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Runtime.InteropServices;
using Microsoft.Win32.SafeHandles;

namespace Nucleo.CheckIO
{
#if _GPIO_POLLING_
    
    class LastHIDReports
    {
        /// <summary>
        /// 
        /// </summary>
        static List<byte[]> _last_reports = new List<byte[]>();
        static byte [] _last_report;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="_rpt"></param>
        /// <returns></returns>
        static bool IsEqual(byte[] _rpt)
        {
            if (_last_report.Count() != _rpt.Count())
                return false;

            for (int i = 9; i < _rpt.Count(); i++)
            {
                if (_rpt[i] != _last_report[i])
                    return false;
            }
            return true;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="_report"></param>
        /// <returns></returns>
        public static bool AddCurrent(byte[] _report)
        {
            bool _ret = true;

            if (_last_reports.Count > 1)
            {
                _ret = IsEqual(_report);
            }

            if (_last_reports.Count >= 8)
            {
                _last_reports.RemoveAt(0);
            }
            _last_reports.Add(_report);

            _last_report = _report;
            return _ret;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public static string GetLastReports()
        {
            StringBuilder str = new StringBuilder();
            str.Append("LOG-START\n");
            foreach (byte[] _report in _last_reports)
            {
                str.Append(BitConverter.ToString(_report));
                str.Append("\n");
            }
            str.Append("LOG-END");
            _last_reports.Clear();
            return str.ToString();
        }
    }

#endif

    class CMediaSoundDevice : ISoundDevice
    {
        [DllImport("kernel32.dll")]
        static extern uint GetLastError();

        public override bool IsValid { get { return true; } }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="_dd"></param>
        public CMediaSoundDevice(GenericHid.HidDeviceManagement.DeviceDescription dev)
        {
            try
            {
                _dev = dev;
                Reading = false;
                
                Open();

                /** Leer el Hardware */
                byte _initial_report;
                if (GetGpio(out _initial_report) == true)
                {
                    _id = HidType(_initial_report);

                    /** Leer JACK en e HW */
                    _ljack = HidJack(_initial_report);

                    /** Leer PTT en el HW */
                    _lptt = HidPtt(_initial_report);

                    /** */
                    _gpio6 = GPIO4(_initial_report);
                    _gpio7 = GPIO5(_initial_report);
                }
                else
                {
                    Dispose();
                    Open();
                }
                Error = false;
            }
            catch (Exception x)
            {
                NLog.LogManager.GetCurrentClassLogger().Trace("Excepcion Creando DISPOSITIVO {0}: {1}", _id, x.Message);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        public override void Start()
        {
#if !_GPIO_POLLING_
            /** Arrancar el Lazo de Escucha */
            StartRead();
#endif
        }

        /// <summary>
        /// 
        /// </summary>
        public override void GetState()
        {
            /** Leer el Hardware */
#if _GPIO_POLLING_
            byte _initial_report;
            if (GetGpio(out _initial_report) == true)
            {
                /** Leer JACK en e HW */
                _ljack = HidJack(_initial_report);

                /** Leer PTT en el HW */
                _lptt = HidPtt(_initial_report);

                /** */
                _gpio6 = GPIO4(_initial_report);
                _gpio7 = GPIO5(_initial_report);

                //bool _tgpio6 = GPIO4(_initial_report);
                //bool _tgpio7 = GPIO5(_initial_report);
                //if (_gpio6 != _gpio6 || _tgpio7 != _gpio7)
                //    NLog.LogManager.GetCurrentClassLogger().Fatal("Error 1 Reading GPIO Report. Device: {0}.", _id);

                SetState(_ljack, _lptt);
            }
            else
            {
                NLog.LogManager.GetCurrentClassLogger().Fatal("Error 2 Reading GPIO Report. Device: {0}.", _id);
                /** REARRANQUE */
                hidHandle.Close();
                Open();
            }
#endif
            SetState(_ljack, _lptt);
        }

        /// <summary>
        /// 
        /// </summary>
        public override void Close()
        {
            Dispose();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="gpio"></param>
        /// <param name="estado"></param>
        /// <returns></returns>
        public override bool SetGpio(int gpio, byte estado)
        {
            /** Leo Registro C0 */
            byte[] cmd = new byte[] { 0x01, 0x00, 0xFE, 0x08, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
            GenericHid.Hid.HidD_SetOutputReport(hidHandle, ref cmd[0], MyHid.Capabilities.OutputReportByteLength);

            cmd[0] = 1;
            if (GenericHid.Hid.HidD_GetInputReport(hidHandle, ref cmd[0], MyHid.Capabilities.InputReportByteLength) == true)
            {
                byte[] _set = new byte[] { 0x01, 0x00, 0xC0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
                int bit = 0x0001 << gpio;
                int val = estado == 0 ? cmd[6] & ~bit : cmd[6] | bit;

                _set[4] = (byte)val;
                _set[5] = cmd[7];
                GenericHid.Hid.HidD_SetOutputReport(hidHandle, ref _set[0], MyHid.Capabilities.OutputReportByteLength);

                _gpio6 = gpio == 4 && estado == 1 ? true : gpio == 4 && estado == 0 ? false : _gpio6;
                _gpio7 = gpio == 5 && estado == 1 ? true : gpio == 5 && estado == 0 ? false : _gpio7;
                return true;
            }
            return false;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        public override bool GetGpio(out byte _ret)
        {
            /** Marco el Registro a Leer */
            byte[] cmd = new byte[] { 0x01, 0x00, 0xFE, 0x08, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
            if (GenericHid.Hid.HidD_SetOutputReport(hidHandle, ref cmd[0], MyHid.Capabilities.OutputReportByteLength) == true)
            {
                cmd[0] = 1;
                if (GenericHid.Hid.HidD_GetInputReport(hidHandle, ref cmd[0], MyHid.Capabilities.InputReportByteLength) == true)
                {
#if _GPIO_POLLING_
                    if (LastHIDReports.AddCurrent(cmd) == false)
                    {
                        NLog.LogManager.GetCurrentClassLogger().Fatal("Error 1 Reading GPIO Report. Device: {0}.", _id);
                        NLog.LogManager.GetCurrentClassLogger().Trace("Error Reading GPIO Report. Device: {0}. Last Report Readed:\n{1}", _id, LastHIDReports.GetLastReports());
                    }
#endif
                    ///** 
                    // * 8.0 --> R.0      PTT
                    // * 8.1 --> R.1      JACK
                    // * 9.2 --> R.2      TIPO-0
                    // * 8.3 --> R.3      TIPO-1
                    // * 8.2 --> R.4      TIPO-2
                    // * 8.4 --> R.5      GPIO 4
                    // * 8.5 --> R.6      GPIO 5
                    // */
                    //int lsb0 = (cmd[8] & 0x03);
                    //int lsb1 = (cmd[8] & 0x04) << 2;
                    //int lsb2 = (cmd[8] & 0x08);
                    //int msb2 = (cmd[9] & 0x04);
                    //int gpio4 = (cmd[8] & 0x10) << 1;
                    //int gpio5 = (cmd[8] & 0x20) << 1;

                    //int ret = lsb0 | lsb1 | lsb2 | msb2 | gpio4 | gpio5;
                    //_ret = (byte)ret;
                    _ret = NormalizeRead(cmd[8], cmd[9]);
                    return true;
                }
            }
            _ret = 0;
            return false;
        }

        #region PRIVATE
        GenericHid.HidDeviceManagement.DeviceDescription _dev = null;
        FileStream _Stream;
        byte[] _Buffer;

        bool Reading { get; set; }
        private SafeFileHandle hidHandle;
        private SafeFileHandle readHandle;
        private GenericHid.Hid MyHid = new GenericHid.Hid();

        bool _ljack = false;
        bool _lptt = false;
        /// <summary>
        /// 
        /// </summary>
        /// <param name="iResult"></param>
        private void ReadCompleted(IAsyncResult iResult)
        {
            try
            {
                if (_Stream.EndRead(iResult) == 16)
                {
                    byte val = NormalizeRead((byte)(~_Buffer[1]), (byte)(~_Buffer[2]));

                    /** Leer JACK en e HW. En el Evento viene con lógica cambiada */
                    // _ljack = HidJack((byte)(~_Buffer[1]));

                    /** Leer PTT en el HW En el Evento viene con lógica cambiada */
                    // _lptt = HidPtt((byte)(~_Buffer[1]));

                    _ljack = HidJack(val);
                    _lptt = HidPtt(val);

                    /** */
                    //_gpio6 = GPIO4(val);
                    //_gpio7 = GPIO5(val);                    
                    
                    _Stream.BeginRead(_Buffer, 0, 16, new AsyncCallback(ReadCompleted), null);
                }
                else
                {
                    NLog.LogManager.GetCurrentClassLogger().Trace("Error HID Callback... en DISPOSITIVO {0}.", _id);
                }
            }
            catch (System.OperationCanceledException)
            {
            }
            catch (Exception x)
            {
                // _Stream.BeginRead(_Buffer, 0, 16, new AsyncCallback(ReadCompleted), null);
                NLog.LogManager.GetCurrentClassLogger().Trace("Excepcion ReadComplete en DISPOSITIVO {0}. {1}", _id, x.Message);
                // System.Diagnostics.Debug.Assert(false, x.Message);
                Error = true;
            }
        }

        /// <summary>
        /// BIT 1. A Cero PTT-OFF, a 1 PTT-ON
        /// </summary>
        /// <param name="_data"></param>
        /// <returns></returns>
        private bool HidPtt(byte _data)
        {
            int bptt = _data & (byte)0x02;
            return bptt != 0 ? true : false;
        }

        /// <summary>
        /// BIT 0. 0 => JACK-OFF, 1 => JACK ON
        /// </summary>
        /// <param name="_data"></param>
        /// <returns></returns>
        private bool HidJack(byte _data)
        {
            int bjack = _data & (byte)0x01;
            return bjack != 0 ? true : false;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="_data"></param>
        /// <returns></returns>
        private byte HidType(byte _data)
        {
            int tipo = (_data >> 2) & 0x03;
            return (byte)tipo;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="_data"></param>
        /// <returns></returns>
        private bool GPIO4(byte _data)
        {
            return ((int)_data & 0x20) != 0 ? true : false;
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="_data"></param>
        /// <returns></returns>
        private bool GPIO5(byte _data)
        {
            return ((int)_data & 0x40) != 0 ? true : false;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="_dt1"></param>
        /// <param name="_dt2"></param>
        /// <returns></returns>
        private byte NormalizeRead(byte _dt1, byte _dt2)
        {
            /** 
             * dt1.0 --> R.0      PTT
             * dt1.1 --> R.1      JACK
             * dt2.2 --> R.2      TIPO-0
             * dt1.3 --> R.3      TIPO-1
             * dt1.2 --> R.4      TIPO-2
             * dt1.4 --> R.5      GPIO 4
             * dt1.5 --> R.6      GPIO 5
             */
            int lsb0 = (_dt1 & 0x03);
            int lsb1 = (_dt1 & 0x04) << 2;
            int lsb2 = (_dt1 & 0x08);
            int msb2 = (_dt2 & 0x04);
            int gpio4 = (_dt1 & 0x10) << 1;
            int gpio5 = (_dt1 & 0x20) << 1;

            int ret = lsb0 | lsb1 | lsb2 | msb2 | gpio4 | gpio5;
            return (byte)ret;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="_data"></param>
        /// <param name="error"></param>
        /// <returns></returns>
        private byte[] GetReport()
        {
            byte[] _data = new byte[MyHid.Capabilities.InputReportByteLength];
            _data[0] = 1;

            if (GenericHid.Hid.HidD_GetInputReport(hidHandle, ref _data[0], MyHid.Capabilities.InputReportByteLength) == true)
                return _data;
            return null;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="_report"></param>
        private void SetReport(byte[] _report)
        {
            byte[] _data = new byte[MyHid.Capabilities.OutputReportByteLength];
            _report.CopyTo(_data, 0);
            GenericHid.Hid.HidD_SetOutputReport(hidHandle, ref _data[0], MyHid.Capabilities.OutputReportByteLength);
        }

        /// <summary>
        /// 
        /// </summary>
        private void Open()
        {
            hidHandle = GenericHid.FileIO.CreateFile(_dev.path, 0,
                GenericHid.FileIO.FILE_SHARE_READ | GenericHid.FileIO.FILE_SHARE_WRITE, null, GenericHid.FileIO.OPEN_EXISTING, 0, 0);

            MyHid.DeviceAttributes.Size = Marshal.SizeOf(MyHid.DeviceAttributes);
            GenericHid.Hid.HidD_GetAttributes(hidHandle, ref MyHid.DeviceAttributes);
            MyHid.GetDeviceCapabilities(hidHandle);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private bool StartRead()
        {
            if (!Reading)
            {
                readHandle = GenericHid.FileIO.CreateFile(_dev.path,
                    GenericHid.FileIO.GENERIC_READ, GenericHid.FileIO.FILE_SHARE_READ | GenericHid.FileIO.FILE_SHARE_WRITE, null,
                    GenericHid.FileIO.OPEN_EXISTING, GenericHid.FileIO.FILE_FLAG_OVERLAPPED, 0);

                if (!readHandle.IsInvalid)
                {
                    _Stream = new FileStream(readHandle, FileAccess.Read, MyHid.Capabilities.InputReportByteLength, true);
                    _Buffer = new byte[MyHid.Capabilities.InputReportByteLength];
                    _Stream.BeginRead(_Buffer, 0, MyHid.Capabilities.InputReportByteLength, new AsyncCallback(ReadCompleted), null);
                    Reading = true;
                    return true;
                }
            }

            return false;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns></returns>
        private bool StopRead()
        {
            if (Reading)
            {
                _Stream.Close();
                readHandle.Close();

                Reading = false;
                return true;
            }
            return false;
        }

        /// <summary>
        /// 
        /// </summary>
        private void Dispose()
        {
            if (Reading)
            {
                StopRead();
            }
            hidHandle.Close();
        }
        #endregion

    }
}
