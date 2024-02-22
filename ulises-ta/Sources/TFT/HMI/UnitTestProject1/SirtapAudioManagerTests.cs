﻿using Microsoft.VisualStudio.TestTools.UnitTesting;
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
            PrepareTests((manager) =>
            {
                manager.Init();
                manager.Start();

                Task.Delay(TimeSpan.FromMinutes(2)).Wait();

                manager.End();
            });
        }

        private void PrepareTests(Action<IHwAudioManager> executeTest)
        {
            var ptt = new Mock<ISingleIODevice>();
            var ham = new SirtapAudioManager(ptt.Object);
            ham.HeadSetStatusChanged += (from, device, status) =>
            {
                Debug.WriteLine($"HeadSetStatusChanged => {status}, {device}");
            };
            ham.SpeakerStatusChanged += (from, device, status) =>
            {
                Debug.WriteLine($"SpeakerStatusChanged => {status}, {device}");
            };
            executeTest(ham);
            ham.Dispose();
        }
    }
}
