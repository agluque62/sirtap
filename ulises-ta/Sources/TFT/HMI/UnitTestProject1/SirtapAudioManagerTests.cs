using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Threading.Tasks;

using NAudio.Wave.Asio;
using NAudio.CoreAudioApi;
using Moq;
using System.Linq;
using System.Diagnostics;
using System.Collections.Generic;

using U5ki.Infrastructure;
using static U5ki.Infrastructure.SipAgent;
using HMI.CD40.Module.BusinessEntities;
using NAudio.Wave;

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
                foreach (var i in item.Value)
                    Debug.WriteLine($"Device {item.Key}: {i.FriendlyName}, {Devicetype(item.Value)}");
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
                Task.Delay(TimeSpan.FromMinutes(1)).Wait();
                manager.End();
            });
        }
        [TestMethod]
        public void SmartCompare()
        {
            var st1 = "Sennheiser USB headset";
            var st2 = "Altavoces (Sennheiser USB headset)";
            var distance = LevenshteinDistance(st1, st2);
            Debug.WriteLine($"SmartCompare => {distance}");
        }
        [TestMethod]
        public void OldAsioTest()
        {
            for (int i = 0; i < 10; i++)
            {
                AsioChannels.Init();
                Debug.WriteLine($"Asio channels => {Channels()}");
                Task.Delay(TimeSpan.FromSeconds(1)).Wait();
            }
        }
        [TestMethod]
        public void NAudioAsioTest_outputs()
        {
            var drivers = AsioOut.GetDriverNames();
            var asio4allDriver = AsioDriver.GetAsioDriverByName(drivers[0]);
            asio4allDriver.ControlPanel();
            for (int a = 0; a < 10; a++)
            {

                asio4allDriver.GetChannels(out int inputs, out int outputs);
                var asioInputs = new List<AsioChannelInfo>();
                var asioOutputs = new List<AsioChannelInfo>();
                for (int i = 0; i < inputs; i++)
                    asioInputs.Add(asio4allDriver.GetChannelInfo(i, true));
                for (int i = 0; i < outputs; i++)
                    asioOutputs.Add(asio4allDriver.GetChannelInfo((int)i, false));

                Debug.WriteLine($"Asio channels => {Channels(asioInputs, asioOutputs)}");
                Task.Delay(TimeSpan.FromSeconds(5)).Wait();
            }
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
            AsioChannels.Init();
            Debug.WriteLine($"Asio channels => {Channels()}");
            executeTest(ham, ptt);
            ham.Dispose();
            Debug.WriteLine($"Asio channels => {Channels()}");
            AsioChannels.End();
        }
        string Channels(List<AsioChannelInfo> inputs = null, List<AsioChannelInfo> outputs = null)
        {
            if (inputs != null &&  outputs != null)
            {
                var nameInputs = inputs.Select(i => i.name).ToArray();
                var nameOutputs = outputs.Select(i => i.name).ToList();
                var str = $"Inputs ({inputs.Count()}): {string.Join("##", nameInputs)} <==> Outputs ({outputs.Count()}): {string.Join("##", nameOutputs)}";
                return str;
            }
            else
            {
                var str = $"Inputs ({AsioChannels.InChannels.Count()}): {string.Join("##", AsioChannels.InChannels)} <==> Outputs ({AsioChannels.OutChannels.Count()}): {string.Join("##", AsioChannels.OutChannels)}";
                return str;
            }
        }
        private int LevenshteinDistance(string str1, string str2)
        {
            int len1 = str1.Length;
            int len2 = str2.Length;
            var matrix = new int[len1 + 1, len2 + 1];

            for (int i = 0; i <= len1; i++)
                matrix[i, 0] = i;

            for (int j = 0; j <= len2; j++)
                matrix[0, j] = j;

            for (int i = 1; i <= len1; i++)
            {
                for (int j = 1; j <= len2; j++)
                {
                    int cost = (str2[j - 1] == str1[i - 1]) ? 0 : 1;
                    matrix[i, j] = Math.Min(
                        Math.Min(matrix[i - 1, j] + 1, matrix[i, j - 1] + 1),
                        matrix[i - 1, j - 1] + cost);
                }
            }
            return matrix[len1, len2];
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
