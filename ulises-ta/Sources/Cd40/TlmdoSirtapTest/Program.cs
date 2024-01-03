using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using U5ki.Infrastructure;

namespace TlmdoSirtapTest
{
    internal class Program
    {
        private static Registry SirTapRegistry = null;

        private static void InitRegistry()
        {
            SirTapRegistry = new Registry(Identifiers.RdMasterTopic);

            SirTapRegistry.UserMsgReceived += OnMsgReceived;

            SirTapRegistry.SubscribeToTopic<TlmdoAsk>(Identifiers.RdTopic);
            SirTapRegistry.SubscribeToTopic<TlmdoRsp>(Identifiers.RdTopic);

            SirTapRegistry.Join(Identifiers.RdMasterTopic, Identifiers.RdTopic);
        }
        private static void EndRegistry()
        {
            try
            {
                if (SirTapRegistry != null) SirTapRegistry.Dispose();
            }
            catch (Exception ex)
            {

            }
            SirTapRegistry = null;
        }

        private static void OnMsgReceived(object sender, SpreadDataMsg msg)
        {
            try
            {
                if (msg.Type == Identifiers.TLMDO_RSP)
                {
                    Console.WriteLine("TLMDO_RSP receved");
                }
                else
                {
                    Console.WriteLine("OTHER receved");
                }
            }
            catch (Exception ex)
            {
            }
        }

        static void Main(string[] args)
        {
            Console.WriteLine("Telemando Test");
            InitRegistry();

            TlmdoAsk tlmdo1 = new TlmdoAsk();
            tlmdo1.msgType = TlmdoAsk.MsgType.TLMDO_GET_CHANNELS;
            tlmdo1.IdFrecuency = "sirtaptlmdotest";
            tlmdo1.HostId = "test";
            tlmdo1.IdRecurso = "";

            SirTapRegistry.Send(Identifiers.RdMasterTopic, Identifiers.TLMDO_ASK, tlmdo1);

            System.Threading.Thread.Sleep(10000);

            EndRegistry();
        }
    }
}
