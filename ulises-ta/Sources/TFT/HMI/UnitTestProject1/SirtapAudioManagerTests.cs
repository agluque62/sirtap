using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Threading.Tasks;

using NAudio;
using NAudio.CoreAudioApi;
using System.Linq;
using System.Diagnostics;

using U5ki.Infrastructure;
using static U5ki.Infrastructure.SipAgent;
using HMI.CD40.Module.BusinessEntities;

namespace UnitTestProject1
{
    [TestClass]
    public class SirtapAudioManagerTests
    {
        [TestMethod]
        public void TestMethod1()
        {
            var activeDevices = new MMDeviceEnumerator()
                .EnumerateAudioEndPoints(DataFlow.All, DeviceState.Active)
                .Select(dev => $"{dev.FriendlyName}")
                .ToList();
            foreach (var item in activeDevices)
            {
                Debug.WriteLine(item);
            }

            CORESIP_Error err;
            CORESIP_SndWindowsDevices Devices;
            if (CORESIP_GetWindowsSoundDeviceNames(0, out Devices, out err) == 0)
            {
                string[] separatingStrings = { "<###>" };
                string[] DevWinName1 = Devices.DeviceNames.Split(separatingStrings, System.StringSplitOptions.RemoveEmptyEntries);
                foreach (var item in DevWinName1)
                {
                    Debug.WriteLine(item);
                }
            }
        }
        [TestMethod]
        public void AudioDeviceTest1()
        {
            var winDeviceName = "2- USB Headset a";
            var asioDevicename = "2- USB Headset";
            var device = new SirtapAudioDevice(winDeviceName, asioDevicename, CORESIP_SndDevType.CORESIP_SND_ALUMN_MHP);
            device.StatusChanged += (from, status) =>
            {
                Debug.WriteLine($"Audio device: <<{winDeviceName}>> status changed => {status}");
                if (status == true)
                {
                    Debug.WriteLine($"Asio Channels => {device.AsioIds}");
                }
            };
            Task.Delay(TimeSpan.FromSeconds(60)).Wait();
            device.Dispose();
        }
    }
}
