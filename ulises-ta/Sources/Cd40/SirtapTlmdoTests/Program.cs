using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Threading;
using System.IO;
using U5ki.Infrastructure;
using Newtonsoft.Json;
using ProtoBuf;
using Utilities;

namespace TlmdoSirtapTest
{
    internal class Program
    {
        private static Registry SirTapRegistry = null;
        private static Semaphore sem = new Semaphore(initialCount: 0, maximumCount: 1);

        private static void InitRegistry()
        {
            SirTapRegistry = new Registry(Identifiers.TopTopic);

            SirTapRegistry.UserMsgReceived += OnMsgReceived;
            SirTapRegistry.ResourceChanged += OnRsChanged;

            SirTapRegistry.SubscribeToTopic<TlmdoAsk>(Identifiers.RdTopic);
            SirTapRegistry.SubscribeToTopic<TlmdoRsp>(Identifiers.RdTopic);
            SirTapRegistry.SubscribeToTopic<Cd40Cfg>(Identifiers.CfgTopic);

            SirTapRegistry.Join(Identifiers.CfgTopic, Identifiers.TopTopic, Identifiers.RdTopic);
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

        static private T Deserialize<T>(byte[] data) where T : class
        {
            T rs = null;

            if (data != null)
            {
                MemoryStream ms = new MemoryStream(data);
                rs = Serializer.Deserialize<T>(ms);
            }

            return rs;
        }

        private static void OnMsgReceived(object sender, SpreadDataMsg msg)
        {
            try
            {
                if (msg.Type == Identifiers.TLMDO_RSP)
                {
                    Console.WriteLine("-----TLMDO_RSP receved---------------------------------------------------------------");
                    Console.WriteLine(JsonConvert.SerializeObject(msg));
                    MemoryStream ms = new MemoryStream(msg.Data, 0, msg.Length);
                    TlmdoRsp ask = Serializer.Deserialize<TlmdoRsp>(ms);
                    Console.WriteLine("TlmdoRsp receved");
                    Console.WriteLine(JsonConvert.SerializeObject(ask));
                    string s = $"msgType: {ask.msgType.ToString()} code: {ask.Code.ToString()}";
                    Console.WriteLine(JsonConvert.SerializeObject(s));
                    Console.WriteLine("-------------------------------------------------------------------------------------");
                    sem.Release();
                }
                else
                {
                    //Console.WriteLine($"OTHER receved Type {msg.Type.ToString()}");
                }
            }
            catch
            {
            }
        }

        private static void OnRsChanged(object sender, RsChangeInfo change)
        {
            try
            {
                if (change.Type == Identifiers.TypeId(typeof(Cd40Cfg)))
                {                    
                    Cd40Cfg cfg = Deserialize<Cd40Cfg>(Tools.Decompress(change.Content));
                    foreach (ConfiguracionUsuario confuser in cfg.ConfiguracionUsuarios)
                    {
                        foreach (CfgEnlaceExterno enl in confuser.RdLinks)
                        {
                            foreach (CfgRecursoEnlaceExterno rec in enl.ListaRecursos)
                            {
                                if (rec.IdRecurso == Identifiers.SimulSirtapGRS)
                                {
                                    IdFrecuencyToTest = enl.Literal;
                                }
                            }
                        }
                    }
                    Console.WriteLine("--CFG received--");
                }                
            }
            catch
            {
            }
        }

        static string IdFrecuencyToTest = "invalid";

        static void Main(string[] args)
        {
            Console.CancelKeyPress += Console_CancelKeyPress;

            Console.WriteLine("Telemando Test");
            InitRegistry();

            Console.WriteLine("+++++++++++++ Prueba TLMDO_GET_CHANNELS. Pulsa una tecla +++++++++++++");
            Console.ReadKey();
            Console.WriteLine("Esperando respuesta");

            TlmdoAsk tlmdo1 = new TlmdoAsk();
            tlmdo1.msgType = TlmdoAsk.MsgType.TLMDO_GET_CHANNELS;
            tlmdo1.IdFrecuency = IdFrecuencyToTest;
            tlmdo1.HostId = "test";
            tlmdo1.IdRecurso = "";

            SirTapRegistry.Send(Identifiers.RdMasterTopic, Identifiers.TLMDO_ASK, tlmdo1);

            if (sem.WaitOne(5000) == false) Console.WriteLine("SIN RESPUESTA");

            Console.WriteLine("+++++++++++++ Prueba TLMDO_SET_CHANNEL. Pulsa una tecla +++++++++++++");
            Console.ReadKey();
            Console.WriteLine("Esperando respuesta");

            TlmdoAsk tlmdo2 = new TlmdoAsk();
            tlmdo2.msgType = TlmdoAsk.MsgType.TLMDO_SET_CHANNEL;
            tlmdo2.IdFrecuency = IdFrecuencyToTest;
            tlmdo2.HostId = "test";
            tlmdo2.IdRecurso = "";
            tlmdo2.Channel = 1;

            SirTapRegistry.Send(Identifiers.RdMasterTopic, Identifiers.TLMDO_ASK, tlmdo2);

            if (sem.WaitOne(5000) == false) Console.WriteLine("SIN RESPUESTA");

            Console.WriteLine("+++++++++++++ Prueba TLMDO_SET_FREQUENCY. Pulsa una tecla +++++++++++++");
            Console.ReadKey();
            Console.WriteLine("Esperando respuesta");

            TlmdoAsk tlmdo3 = new TlmdoAsk();
            tlmdo3.msgType = TlmdoAsk.MsgType.TLMDO_SET_FREQUENCY;
            tlmdo3.IdFrecuency = IdFrecuencyToTest;
            tlmdo3.HostId = "test";
            tlmdo3.IdRecurso = "";
            tlmdo3.Channel = 1;
            tlmdo3.Frequency = "200.000";

            SirTapRegistry.Send(Identifiers.RdMasterTopic, Identifiers.TLMDO_ASK, tlmdo3);

            if (sem.WaitOne(5000) == false) Console.WriteLine("SIN RESPUESTA");

            Console.WriteLine("+++++++++++++ Prueba TLMDO_SET_TXPWR. Pulsa una tecla +++++++++++++");
            Console.ReadKey();
            Console.WriteLine("Esperando respuesta");

            TlmdoAsk tlmdo4 = new TlmdoAsk();
            tlmdo4.msgType = TlmdoAsk.MsgType.TLMDO_SET_TXPWR;
            tlmdo4.IdFrecuency = IdFrecuencyToTest;
            tlmdo4.HostId = "test";
            tlmdo4.IdRecurso = "";
            tlmdo4.Channel = 1;
            tlmdo4.PowerW = "0.50";

            SirTapRegistry.Send(Identifiers.RdMasterTopic, Identifiers.TLMDO_ASK, tlmdo4);

            if (sem.WaitOne(5000) == false) Console.WriteLine("SIN RESPUESTA");

            Console.WriteLine("+++++++++++++ Prueba TLMDO_SET_TXINHIBIT. Pulsa una tecla +++++++++++++");
            Console.ReadKey();
            Console.WriteLine("Esperando respuesta");

            TlmdoAsk tlmdo5 = new TlmdoAsk();
            tlmdo5.msgType = TlmdoAsk.MsgType.TLMDO_SET_TXINHIBIT;
            tlmdo5.IdFrecuency = IdFrecuencyToTest;
            tlmdo5.HostId = "test";
            tlmdo5.IdRecurso = "";
            tlmdo5.TxInhibit = false;

            SirTapRegistry.Send(Identifiers.RdMasterTopic, Identifiers.TLMDO_ASK, tlmdo5);

            if (sem.WaitOne(5000) == false) Console.WriteLine("SIN RESPUESTA");

            Console.WriteLine("+++++++++++++ Prueba TLMDO_SET_WFALLOC. Pulsa una tecla +++++++++++++");
            Console.ReadKey();
            Console.WriteLine("Esperando respuesta");

            TlmdoAsk tlmdo6 = new TlmdoAsk();
            tlmdo6.msgType = TlmdoAsk.MsgType.TLMDO_SET_WFALLOC;
            tlmdo6.IdFrecuency = IdFrecuencyToTest;
            tlmdo6.HostId = "test";
            tlmdo6.IdRecurso = "";
            tlmdo6.Channel = 1;
            tlmdo6.WF = "WF";

            SirTapRegistry.Send(Identifiers.RdMasterTopic, Identifiers.TLMDO_ASK, tlmdo6);

            if (sem.WaitOne(5000) == false) Console.WriteLine("SIN RESPUESTA");

            Console.WriteLine("+++++++++++++ Prueba TLMDO_ERASE_CRYPT_KEYS. Pulsa una tecla +++++++++++++");
            Console.ReadKey();
            Console.WriteLine("Esperando respuesta");

            TlmdoAsk tlmdo7 = new TlmdoAsk();
            tlmdo7.msgType = TlmdoAsk.MsgType.TLMDO_ERASE_CRYPT_KEYS;
            tlmdo7.IdFrecuency = IdFrecuencyToTest;
            tlmdo7.HostId = "test";
            tlmdo7.IdRecurso = "";

            SirTapRegistry.Send(Identifiers.RdMasterTopic, Identifiers.TLMDO_ASK, tlmdo7);

            if (sem.WaitOne(5000) == false) Console.WriteLine("SIN RESPUESTA");

            Console.WriteLine("+++++++++++++ Prueba TLMDO_LOAD_CRYPT_KEYS. Pulsa una tecla +++++++++++++");
            Console.ReadKey();
            Console.WriteLine("Esperando respuesta");

            TlmdoAsk tlmdo8 = new TlmdoAsk();
            tlmdo8.msgType = TlmdoAsk.MsgType.TLMDO_LOAD_CRYPT_KEYS;
            tlmdo8.IdFrecuency = IdFrecuencyToTest;
            tlmdo8.HostId = "test";
            tlmdo8.IdRecurso = "";

            SirTapRegistry.Send(Identifiers.RdMasterTopic, Identifiers.TLMDO_ASK, tlmdo8);

            if (sem.WaitOne(5000) == false) Console.WriteLine("SIN RESPUESTA");

            Console.WriteLine("*****************   Prueba finalizada. Pulsa una tecla");
            Console.ReadKey();
            Console.WriteLine("Esperando respuesta");

            EndRegistry();
        }

        private static void Console_CancelKeyPress(object sender, ConsoleCancelEventArgs e)
        {
            e.Cancel = true;
            Console.WriteLine("Programa interrumpido");
            System.Environment.Exit(1);
        }
    }
}
