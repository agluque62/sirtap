using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using U5ki.Infrastructure;
using Utilities;
using ProtoBuf;

namespace u5ki.Gateway
{
    public class Gateway : BaseCode, IService
    {
        public string Name { get; set; }
        public ServiceStatus Status { get; set; }
        public bool Master { get; set; }

        private static Registry GatewayRegistry = null;
        private Mutex mut;
        private Cd40Cfg Cfg = null;

        public Gateway()
        {
            Name = "Gateway";
            Status = ServiceStatus.Stopped;
            Master = false;
        }

        public void Start()
        {
            Status = ServiceStatus.Running;
            LogInfo<Gateway>("Iniciando Servicio ...");

            try
            {
                mut = new Mutex();
                InitRegistry();
            }
            catch (Exception ex)
            {
                ExceptionManage<Gateway>("Start", ex, "Excepcion no esperada arrancando servicio Gateway. ERROR: " + ex.Message);
                Stop();
            }
        }

        public void Stop()
        {
            LogInfo<Gateway>("Finalizando Servicio ...");
            Status = ServiceStatus.Stopped;
            Thread.Sleep(1000);

            try
            {
                EndRegistry();
            }
            catch (Exception ex)
            {
                ExceptionManage<Gateway>("Stop", ex, "Excepcion no esperada parando servicio Gateway. ERROR: " + ex.Message);
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
            GatewayRegistry = new Registry(Identifiers.GatewayMasterTopic);

            GatewayRegistry.ChannelError += OnChannelError;
            GatewayRegistry.MasterStatusChanged += OnMasterStatusChanged;
            GatewayRegistry.ResourceChanged += OnRsChanged;
            GatewayRegistry.UserMsgReceived += OnMsgReceived;

            GatewayRegistry.SubscribeToMasterTopic(Identifiers.GatewayMasterTopic);
            GatewayRegistry.SubscribeToTopic<SrvMaster>(Identifiers.GatewayMasterTopic);
            GatewayRegistry.SubscribeToTopic<Cd40Cfg>(Identifiers.CfgTopic);            

            GatewayRegistry.Join(Identifiers.GatewayTopic, Identifiers.CfgTopic, Identifiers.GatewayMasterTopic);
        }
        private void EndRegistry()
        {
            try
            {
                if (GatewayRegistry != null) GatewayRegistry.Dispose();
            }
            catch (Exception ex)
            {
                LogError<Gateway>("EndRegistry: Exception " + ex.Message);
            }
            GatewayRegistry = null;
        }

        private void OnChannelError(object sender, string error)
        {
            LogError<Gateway>("OnChannelError: " + error, U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR);
        }

        public void OnMasterStatusChanged(object sender, bool master)
        {
            if (Status != ServiceStatus.Running) return;

            bool retmut = mut.WaitOne(10000);
            if (retmut == false)
            {
                LogError<Gateway>("OnMasterStatusChanged: mutex timeout. Se finaliza el servicio");
                Stop();
                return;
            }

            try
            {
                if (master && !Master)
                {
                    LogInfo<Gateway>("MASTER", U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_INFO, "Gateway", "MASTER");
                }
                else if (!master && Master)
                {
                    LogInfo<Gateway>("SLAVE", U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_INFO, "Gateway", "SLAVE");
                }
                Master = master;

                if (!Master)
                {
                    
                }
                else
                {
                    
                }
                mut.ReleaseMutex();

            }
            catch (Exception ex)
            {
                ExceptionManage<Gateway>("OnMasterStatusChanged", ex, "Excepcion no esperada: " + ex.Message);
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
                LogError<Gateway>("OnRsChanged: mutex timeout. Se finaliza el servicio");
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

                        LogInfo<Gateway>(String.Format("Recibida nueva configuracion ({0})", Cfg.Version), U5kiIncidencias.U5kiIncidencia.IGNORE);

                        try
                        {
                            ProcessNewConfig(Cfg);
                        }
                        catch (Exception ex)
                        {
                            ExceptionManage<Gateway>("ProcessNewConfig", ex, "ProcessNewConfig Exception: " + ex.Message);
                            stop_service = true;
                        }
                    }
                    catch (Exception ex)
                    {
                        ExceptionManage<Gateway>("OnRsChanged", ex, "OnRsChanged Exception: " + ex.Message);
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
                LogError<Gateway>("OnMsgReceived: mutex timeout. Se finaliza el servicio");
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
                default:
                    break;
            }

            mut.ReleaseMutex();
        }
    }
}
