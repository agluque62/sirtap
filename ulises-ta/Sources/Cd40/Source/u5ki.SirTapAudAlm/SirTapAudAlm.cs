using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using U5ki.Infrastructure;
using Utilities;
using ProtoBuf;
using System.Net.Sockets;

namespace u5ki.SirTapAudAlm
{
    public class SirTapAudAlm : BaseCode, IService
    {
        const int WAIT_THREAD_TIMEOUT_MS = 2000;

        public string Name { get; set; }

        public ServiceStatus Status { get; set; }

        public bool Master { get; set; }

        private static Registry SirTapAudAlmRegistry = null;

        private Mutex mut;

        private Cd40Cfg Cfg = null;

        private Task ProcessIncommingAlarmsTask = null;

        TcpClient AlarmsIPclient = null;

        public SirTapAudAlm()
        {
            Name = "SirTapAudAlm";
            Status = ServiceStatus.Stopped;
            Master = false;
        }

        public void Start()
        {
            Status = ServiceStatus.Running;
            LogInfo<SirTapAudAlm>("Iniciando Servicio ...");

            try
            {
                mut = new Mutex();
                InitRegistry();                
            }
            catch (Exception ex)
            {
                ExceptionManage<SirTapAudAlm>("Start", ex, "Excepcion no esperada arrancando servicio SirTapAudAlm. ERROR: " + ex.Message);
                Stop();
            }
        }

        public void Stop()
        {
            LogInfo<SirTapAudAlm>("Finalizando Servicio ...");
            Status = ServiceStatus.Stopped;
            Thread.Sleep(1000);

            try
            {
                try
                {
                    AlarmsIPclient.Close();
                }
                catch { }
                ProcessIncommingAlarmsTask.Wait(WAIT_THREAD_TIMEOUT_MS);
                EndRegistry();
            }
            catch (Exception ex)
            {
                ExceptionManage<SirTapAudAlm>("Stop", ex, "Excepcion no esperada parando servicio SirTapAudAlm. ERROR: " + ex.Message);
            }

            mut.Close();
        }

        public bool Commander(ServiceCommands cmd, string par, ref string err, List<string> resp = null)
        {
            throw new NotImplementedException();
        }
        public bool DataGet(ServiceCommands cmd, ref List<object> rsp)
        {
            throw new NotImplementedException();
        }
        public object AllDataGet()
        {
            throw new NotImplementedException();
        }

        private void InitRegistry()
        {
            SirTapAudAlmRegistry = new Registry(Identifiers.SirTapAudAlmMasterTopic);

            SirTapAudAlmRegistry.ChannelError += OnChannelError;
            SirTapAudAlmRegistry.MasterStatusChanged += OnMasterStatusChanged;
            SirTapAudAlmRegistry.ResourceChanged += OnRsChanged;
            SirTapAudAlmRegistry.UserMsgReceived += OnMsgReceived;

            SirTapAudAlmRegistry.SubscribeToMasterTopic(Identifiers.SirTapAudAlmMasterTopic);
            SirTapAudAlmRegistry.SubscribeToTopic<SrvMaster>(Identifiers.SirTapAudAlmMasterTopic);
            SirTapAudAlmRegistry.SubscribeToTopic<Cd40Cfg>(Identifiers.CfgTopic);
            //ConfRegistry.SubscribeToTopic<TopRs>(Identifiers.TopTopic);

            SirTapAudAlmRegistry.Join(Identifiers.SirTapAudAlmTopic, Identifiers.CfgTopic, Identifiers.SirTapAudAlmMasterTopic);
        }
        private void EndRegistry()
        {
            try
            {
                if (SirTapAudAlmRegistry != null) SirTapAudAlmRegistry.Dispose();
            }
            catch (Exception ex)
            {
                LogError<SirTapAudAlm>("EndRegistry: Exception " + ex.Message);
            }
            SirTapAudAlmRegistry = null;
        }

        private void OnChannelError(object sender, string error)
        {
            LogError<SirTapAudAlm>("OnChannelError: " + error, U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR);
        }

        public void OnMasterStatusChanged(object sender, bool master)
        {
            if (Status != ServiceStatus.Running) return;

            bool retmut = mut.WaitOne(10000);
            if (retmut == false)
            {
                LogError<SirTapAudAlm>("OnMasterStatusChanged: mutex timeout. Se finaliza el servicio");
                Stop();
                return;
            }

            try
            {
                if (master && !Master)
                {
                    LogInfo<SirTapAudAlm>("MASTER", U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_INFO, "SirTapAudAlm", "MASTER");
                }
                else if (!master && Master)
                {
                    LogInfo<SirTapAudAlm>("SLAVE", U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_INFO, "SirTapAudAlm", "SLAVE");
                }
                Master = master;    
                
                if (!Master)
                {
                    //Si es SLAVE se fuerza a que finalice la tarea de recepscion de alarmas por IP
                    try
                    {
                        AlarmsIPclient.Close();                        
                    }
                    catch { }                    
                }
                else
                {
                    AlarmsIPclient = new TcpClient();
                    ProcessIncommingAlarmsTask = new Task(ProcessIncommingAlarms);
                    ProcessIncommingAlarmsTask.Start();
                }
                mut.ReleaseMutex();

            }
            catch (Exception ex)
            {
                ExceptionManage<SirTapAudAlm>("OnMasterStatusChanged", ex, "Excepcion no esperada: " + ex.Message);
                mut.ReleaseMutex();
                Stop();
            }            
        }

        private void OnRsChanged(object sender, RsChangeInfo e)
        {
            if (Status != ServiceStatus.Running) return;

            bool retmut = mut.WaitOne(10000);
            if (retmut == false)
            {
                LogError<SirTapAudAlm>("OnRsChanged: mutex timeout. Se finaliza el servicio");
                Stop();
                return;
            }

            bool stop_service = false;

            if (e.Type == Identifiers.TypeId(typeof(Cd40Cfg)))
            {
                if (e.Content != null)
                {
                    try
                    {
                        MemoryStream ms = new MemoryStream(Tools.Decompress(e.Content));
                        Cfg = Serializer.Deserialize<Cd40Cfg>(ms);

                        LogInfo<SirTapAudAlm>(String.Format("Recibida nueva configuracion ({0})", Cfg.Version), U5kiIncidencias.U5kiIncidencia.IGNORE);

                        try
                        {
                            ProcessNewConfig(Cfg);
                        }
                        catch (Exception ex)
                        {
                            ExceptionManage<SirTapAudAlm>("ProcessNewConfig", ex, "ProcessNewConfig Exception: " + ex.Message);
                            stop_service = true;
                        }
                    }
                    catch (Exception ex)
                    {
                        ExceptionManage<SirTapAudAlm>("OnRsChanged", ex, "OnRsChanged Exception: " + ex.Message);
                    }
                }
            }
            mut.ReleaseMutex();

            if (stop_service == true)
            {
                Stop();
            }
        }

        private void ProcessNewConfig(Cd40Cfg cfg)
        {
            
        }

        private void OnMsgReceived(object sender, SpreadDataMsg msg)
        {
            if (Status != ServiceStatus.Running) return;

            bool retmut = mut.WaitOne(10000);
            if (retmut == false)
            {
                LogError<SirTapAudAlm>("OnMsgReceived: mutex timeout. Se finaliza el servicio");
                Stop();
                return;
            }

            if (!Master)
            {
                mut.ReleaseMutex();
                return;
            }

            MemoryStream ms = new MemoryStream(msg.Data, 0, msg.Length);
            switch (msg.Type)
            {
                case Identifiers.SIRTAP_AUD_ALM:       
                    //Cuando se recibe el mensaje de alarmas se envia a todos los puestos.
                    byte[] data_to_send = ms.ToArray();
                    SirTapAudAlmRegistry.Channel.Send(Identifiers.SIRTAP_AUD_ALM, data_to_send, Identifiers.TopTopic);

                    break;

                default:
                    break;
            }

            mut.ReleaseMutex();
        }

        private void ProcessIncommingAlarms()
        {
            LogInfo<SirTapAudAlm>("ProcessIncommingAlarms Task Started");

            Int32 port = 13000;
            String server = "127.0.0.1";
                        
            NetworkStream stream = null;

            while (Status == ServiceStatus.Running)
            {
                bool retmut = mut.WaitOne(10000);
                if (retmut == false)
                {
                    LogError<SirTapAudAlm>("ProcessIncommingAlarms: mutex timeout. ");
                }

                if (!Master)
                {
                    //Si no es master finaliza el hilo
                    if (retmut) mut.ReleaseMutex();
                    break;
                }

                if (retmut) mut.ReleaseMutex();

                if (!AlarmsIPclient.Connected)
                {
                    try
                    {
                        AlarmsIPclient.Connect(server, port);
                    }
                    catch (Exception e) 
                    {
                        LogError<SirTapAudAlm>($"Excepcion {e.Message} Conectando a {server}:{port}");
                    }
                    Thread.Sleep(250);
                    continue;
                }

                try
                {
                    Int32 bytes = 0;
                    stream = AlarmsIPclient.GetStream();

                    Byte[] data = new Byte[256];
                    bytes = stream.Read(data, 0, data.Length);
                    LogInfo<SirTapAudAlm>($"################################################## Mensaje recibido bytes {bytes}");
                    if (bytes > 0)
                    {
                        MemoryStream ms = new MemoryStream(data);
                        SirTapAudAlarmMsg alarmaMsg = Serializer.Deserialize<SirTapAudAlarmMsg>(ms);

                        MemoryStream mstream = new MemoryStream();
                        Serializer.Serialize(mstream, alarmaMsg);
                        byte[] data_to_send = mstream.ToArray();

                        SirTapAudAlmRegistry.Channel.Send(Identifiers.SIRTAP_AUD_ALM, data_to_send, Identifiers.TopTopic);
                    }
                    else
                    {
                        LogError<SirTapAudAlm>($"Read retorna 0. Reiniciamos la conexion con {server}:{port}");
                        AlarmsIPclient.Dispose();
                        AlarmsIPclient = new TcpClient();
                        Thread.Sleep(250);
                        continue;
                    }
                }                
                catch (Exception e)
                {
                    LogError<SirTapAudAlm>($"Excepcion {e.Message} Posiblemente se ha desconectado el socket  {server}:{port}");
                    if (Status != ServiceStatus.Running)
                    {
                        LogError<SirTapAudAlm>($"Excepcion Status no es Running. Sale de la tarea ProcessIncommingAlarms");
                        break;
                    }                    
                    AlarmsIPclient.Dispose();
                    AlarmsIPclient = new TcpClient();
                    Thread.Sleep(250);
                    continue;
                }           
            }

            try
            {
                stream.Close();
            }
            catch { }
            try
            {
                AlarmsIPclient.Dispose();
            }
            catch { }

            LogInfo<SirTapAudAlm>("ProcessIncommingAlarms Task End");
        }
    }
}
