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
    class MicronasDevManager : ISndDevManager
    {

        public override byte Init()
        {
            ISndDevManager._context = SynchronizationContext.Current ?? new SynchronizationContext();

            Guid hidGuid = new Guid();
            HidD_GetHidGuid(ref hidGuid);

            int cuantos = UacBuildDeviceList(0);
            byte i = 0;
            for (IntPtr usbHandle = UacGetFirstDevice(); usbHandle != IntPtr.Zero; usbHandle = UacGetNextDevice(usbHandle))
            {
                IntPtr hidHandle = UacGetHidDevice(usbHandle);
                if (hidHandle != IntPtr.Zero)
                {
                    StringBuilder id = new StringBuilder(512);

                    if (UacGetInstanceID(hidHandle, id, 512) != 0)
                    {
                        id.Replace('\\', '#');

                        string devPath = string.Format("\\\\?\\{0}#{{{1}}}", id.ToString().ToLower(), hidGuid);
                        ISoundDevice sndDev = new MicronasSoundDevice(usbHandle, devPath);

                        if (sndDev.IsValid)
                        {
                            _sndDevs[sndDev.Id] = sndDev;
                            i++;
                        }
                    }
                }
            }

            return i;
        }

        #region Private Members


        #region Uac2 Dll Interface

        [DllImport("uac2", EntryPoint = "_UacBuildDeviceList@4", CharSet = CharSet.Ansi, ExactSpelling = true)]
        static extern int UacBuildDeviceList(uint uFlags);
        [DllImport("uac2", EntryPoint = "_UacGetFirstDevice@0", CharSet = CharSet.Ansi, ExactSpelling = true)]
        static extern IntPtr UacGetFirstDevice();
        [DllImport("uac2", EntryPoint = "_UacGetNextDevice@4", CharSet = CharSet.Ansi, ExactSpelling = true)]
        static extern IntPtr UacGetNextDevice(IntPtr uacHandle);
        [DllImport("uac2", EntryPoint = "_UacGetHidDevice@4", CharSet = CharSet.Ansi, ExactSpelling = true)]
        static extern IntPtr UacGetHidDevice(IntPtr uacHandle);
        [DllImport("uac2", EntryPoint = "_UacGetInstanceID_A@12", CharSet = CharSet.Ansi, ExactSpelling = true)]
        static extern int UacGetInstanceID(IntPtr uacHandle, StringBuilder id, int size);

        #endregion

        #region Hid Dll Interface

        [DllImport("hid.dll", SetLastError = true)]
        static extern void HidD_GetHidGuid(ref Guid hidGuid);

        #endregion
        #endregion
    }
}
