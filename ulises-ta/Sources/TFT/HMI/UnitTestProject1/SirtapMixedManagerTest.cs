using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using Moq;

using HMI.CD40.Module.BusinessEntities;
using System.Security.Cryptography;
using U5ki.Infrastructure;
using System.Diagnostics;

namespace UnitTestProject1
{
    [TestClass]
    public class SirtapMixedManagerTest
    {
        [TestMethod]
        public void TestMethod1()
        {
            InitializeTest((mixer, top) =>
            {
                // Al recibir un SQH...
                mixer.Link(1, MixerDev.MhpRd, MixerDir.Send, Mixer.RD_PRIORITY, FuentesGlp.RxRadio);
            });
        }

        void InitializeTest(Action<MixerManager, ITopPlatform> executeTest)
        {
            try
            {
                var top = new Mock<ITopPlatform>();
                var coresipmixer = new Mock<IMixer>();
                var mixer = new MixerManager(top.Object, coresipmixer.Object);
                mixer.Init();
                mixer.Start();
                executeTest?.Invoke(mixer, top.Object);
                mixer.End();
            }
            catch (Exception ex)
            {
                Debug.WriteLine(ex.ToString());
                Assert.Fail();
            }
        }
    }
}
