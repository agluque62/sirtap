using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Threading.Tasks;

using NAudio;
using NAudio.CoreAudioApi;
using Moq;
using System.Linq;
using System.Diagnostics;
using System.Collections.Generic;

using U5ki.Infrastructure;
using static U5ki.Infrastructure.SipAgent;
using HMI.CD40.Module.BusinessEntities;

namespace UnitTestProject1
{
    [TestClass]
    public class SirtapAudioManagerTests
    {
        string Devicetype(List<MMDevice> devices)
        {
            if (devices.Count == 1)
            {
                return devices.ElementAt(0).DataFlow == DataFlow.Render ? "1 Altavoz" : "1 Microfono";
            }
            else if (devices.Count > 1) 
            { 
                var altavoces = devices.Where(dev => dev.DataFlow == DataFlow.Render).Count();
                var microfonos= devices.Where(dev => dev.DataFlow== DataFlow.Capture).Count();
                return $"Mixto ({altavoces} altavoz, {microfonos} microfono)";
            }
            return "No input/output";
        }
        [TestMethod]
        public void EnumerateActiveAudioDevices()
        {
            var activeDevices = new MMDeviceEnumerator()
                .EnumerateAudioEndPoints(DataFlow.All, DeviceState.Active)
                //.Select(dev => $"{dev.FriendlyName}, {dev.DataFlow}")
                .GroupBy(dev => dev.DeviceFriendlyName)
                .ToDictionary(g => g.Key, g => g.ToList());
            foreach (var item in activeDevices)
            {
                Debug.WriteLine($"Device {item.Key}, {Devicetype(item.Value)}");
            }
        }
        [TestMethod]
        public void TestMethod1()
        {
            var activeDevices = new MMDeviceEnumerator()
                .EnumerateAudioEndPoints(DataFlow.All, DeviceState.Active)
                .Select(dev => $"{dev.FriendlyName}, {dev.DataFlow}")
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
        public void SirtapAudioManagerTest1()
        {
            PrepareTests((manager, ptt) =>
            {
                manager.Init();
                manager.Start();

                ProgramEvent(TimeSpan.FromMilliseconds(500), () =>
                {
                    ptt.Raise(dev => dev.StatusChanged += null, this, true);
                    ProgramEvent(TimeSpan.FromSeconds(10), () => ptt.Raise(dev => dev.InputChanged += null, this, true));
                    ProgramEvent(TimeSpan.FromSeconds(15), () => ptt.Raise(dev => dev.InputChanged += null, this, false));
                });
                Task.Delay(TimeSpan.FromMinutes(2)).Wait();
                manager.End();
            });
        }
        private void PrepareTests(Action<IHwAudioManager, Mock<ISingleIODevice>> executeTest)
        {
            var ptt = new Mock<ISingleIODevice>();
            ptt.Setup(o => o.IsConnected).Returns(true);
            var ham = new SirtapAudioManager(null, ptt.Object);
            ham.HeadSetStatusChanged += (from, device, status) =>
            {
                Debug.WriteLine($"HeadSetStatusChanged => {status}, {device}");
            };
            ham.SpeakerStatusChanged += (from, device, status) =>
            {
                Debug.WriteLine($"SpeakerStatusChanged => {status}, {device}");
            };
            ham.PttDeviceStatusChanged += (from, device, status) =>
            {
                Debug.WriteLine($"PTTDeviceStatusChanged => {status}, {device}");
            };
            ham.PttPulsed += (from, status) =>
            {
                Debug.WriteLine($"PTT => {status}");
            };
            ham.JacksChanged += (from, jacks) =>
            {
                Debug.WriteLine($"JACKs => {jacks}");
            };
            executeTest(ham, ptt);
            ham.Dispose();
        }
        private void ProgramEvent(TimeSpan delay, Action action)
        {
            Task.Run(() =>
            {
                Task.Delay(delay).Wait();
                action();
            });
        }
    }
}
