using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using U5ki.RdService;

namespace UnitTestSirtap
{
    [TestClass]
    public class UnitTestTlmdo
    {
        static RdService service = null;

        [ClassInitialize]
        public static void ClassInitialize(TestContext testContext)
        {
            UnitTestTlmdo.service = new RdService();
        }

        [ClassCleanup]
        public static void ClassCleanup()
        {
        }


        [TestMethod]
        public void TestMethod1()
        {

        }
    }
}
