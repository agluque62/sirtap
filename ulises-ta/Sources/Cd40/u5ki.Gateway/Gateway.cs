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
using System.Resources;

namespace U5ki.Gateway
{
    public class Gateway : BaseCode, IService
    {
        public string Name { get; set; }
        public ServiceStatus Status { get; set; }
        private static ServiceStatus privStatus { get; set; }
        public bool Master { get; set; }

        private static Registry GatewayRegistry = null;
        private static Mutex mut;
        private Cd40Cfg Cfg = null;

        private static Dictionary<string, NoED137Resource> Resources = new Dictionary<string, NoED137Resource>();

        public Gateway()
        {
            Name = "Gateway";
            Status = ServiceStatus.Stopped;
            privStatus = ServiceStatus.Stopped;
            Master = false;
        }

        public void Start()
        {
            Status = ServiceStatus.Running;
            LogInfo<Gateway>("Iniciando Servicio ...");
            SipAgent.RTPport_info += RTPport_infoCb;

            try
            {
                mut = new Mutex();
                InitRegistry();
                privStatus = ServiceStatus.Running;
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
            privStatus = ServiceStatus.Stopped;
            Status = ServiceStatus.Stopped;
            SipAgent.RTPport_info -= RTPport_infoCb;
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

                if (Master)
                {
                    foreach (NoED137Resource noEd137res in Resources.Values)
                    {
                        noEd137res.Init();
                    }
                }
                else
                {
                    foreach (NoED137Resource noEd137res in Resources.Values)
                    {
                        noEd137res.Dispose();
                    }
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
            LogInfo<Gateway>("ProcessNewConfig: Recibida nueva configuracion");            

            Dictionary<string, CfgRecursoEnlaceExterno> resourcesFound = new Dictionary<string, CfgRecursoEnlaceExterno>();
            foreach (ConfiguracionUsuario confuser in cfg.ConfiguracionUsuarios)
            {
                foreach (CfgEnlaceExterno enl in confuser.RdLinks)
                {
                    foreach (CfgRecursoEnlaceExterno res in enl.ListaRecursos)
                    {
                        if (ConfiguracionSistema.IsRadioNOED137(res) && !resourcesFound.ContainsKey(res.IdRecurso))
                        {
                            resourcesFound[res.IdRecurso] = res;
                        }
                    }
                }
            }

            foreach (CfgRecursoEnlaceExterno res in resourcesFound.Values)
            {
                //Buscamos la configuracion del equipo externo y del direccionamiento IP
                AsignacionRecursosGW equipo_externo = null;
                foreach (AsignacionRecursosGW asigRecGW in cfg.ConfiguracionGeneral.PlanAsignacionRecursos)
                {
                    if (asigRecGW.IdRecurso == res.IdRecurso)
                    {
                        equipo_externo = asigRecGW;
                        break;
                    }
                }

                DireccionamientoIP direccionamientoIP = null;
                if (equipo_externo != null)
                {
                    foreach (DireccionamientoIP dirIP in cfg.ConfiguracionGeneral.PlanDireccionamientoIP)
                    {
                        if (dirIP.TipoHost == Tipo_Elemento_HW.TEH_EXTERNO_RADIO && dirIP.IdHost == equipo_externo.IdHost)
                        {
                            direccionamientoIP = dirIP;
                            break;
                        }
                    }
                }

                if (!Resources.ContainsKey(res.IdRecurso))
                {                  
                    if (equipo_externo != null && direccionamientoIP != null)
                    {
                        NoED137Resource noEd137res = new NoED137Resource();
                        if (Master) noEd137res.Init(res, equipo_externo, direccionamientoIP);
                        Resources[res.IdRecurso] = noEd137res;
                    }
                }
                else if (Resources[res.IdRecurso].ConfigurationOfResourceChange(res, equipo_externo, direccionamientoIP))
                {
                    Resources[res.IdRecurso].Dispose();
                    if (Master) Resources[res.IdRecurso].Init(res, equipo_externo, direccionamientoIP);                    
                }
            }

            List<string> idResToRemove = new List<string>();
            foreach (NoED137Resource noEd137res in Resources.Values)
            {
                if (!resourcesFound.ContainsKey(noEd137res.IdRecurso))
                {
                    noEd137res.Dispose();
                    idResToRemove.Add(noEd137res.IdRecurso);                    
                }
            }
            foreach (string idRes in idResToRemove)
            {                
                Resources.Remove(idRes);
            }

            string recursos_no_ed137_resultantes = "";
            foreach (NoED137Resource noEd137res in Resources.Values)
            {
                recursos_no_ed137_resultantes += $"{noEd137res.IdRecurso},";
            }

            LogInfo<Gateway>($"ProcessNewConfig: Recursos No ED137: {recursos_no_ed137_resultantes}");
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

        public static void NOED137OnCallState(int call, CORESIP_CallInfo info, CORESIP_CallStateInfo stateInfo)
        {
            if (privStatus != ServiceStatus.Running) return;

            bool retmut = mut.WaitOne(10000);
            if (retmut == false) return;            

            foreach (string idRes in Resources.Keys)
            {
                Resources[idRes].NOED137OnCallState(call, info, stateInfo);
            }

            mut.ReleaseMutex();
        }

        public static void NOED137OnCallIncoming(int call, int call2replace, CORESIP_CallInfo info, CORESIP_CallInInfo inInfo)
        {
            if (privStatus != ServiceStatus.Running) return;

            bool retmut = mut.WaitOne(10000);
            if (retmut == false) return;

            bool found = false;
            foreach (string idRes in Resources.Keys)
            {
                if (inInfo.DstId == idRes) 
                {
                    Resources[idRes].NOED137OnCallIncoming(call, call2replace, info, inInfo);
                    found = true;
                }
            }

            mut.ReleaseMutex();

            if (!found) SipAgent.AnswerCall(call, SipAgent.SIP_NOT_FOUND);
        }

        public static bool NOED137OnKaTimeout(int call)
        {
            if (privStatus != ServiceStatus.Running) return false;

            bool found = false;
            bool retmut = mut.WaitOne(10000);
            if (retmut == false) return found;

            foreach (string idRes in Resources.Keys)
            {
                bool callfound = Resources[idRes].NOED137OnKaTimeout(call);
                if (callfound)
                {
                    found = true;
                    break;
                }
            }

            mut.ReleaseMutex();
            return found;
        }

        public static bool NOED137OnRdInfo(int call, CORESIP_RdInfo info)
        {
            if (privStatus != ServiceStatus.Running) return false;

            bool found = false;
            bool retmut = mut.WaitOne(10000);
            if (retmut == false) return found;

            foreach (string idRes in Resources.Keys)
            {
                bool callfound = Resources[idRes].NOED137OnRdInfo(call, info);
                if (callfound)
                {
                    found = true;
                    break;
                }
            }

            mut.ReleaseMutex();
            return found;
        }


        private void RTPport_infoCb(int rtpport_id, CORESIP_RTPport_info info)
        {
            if (privStatus != ServiceStatus.Running) return;

            bool retmut = mut.WaitOne(10000);
            if (retmut == false) return;

            foreach (string idRes in Resources.Keys)
            {
                bool rtpport_id_found = Resources[idRes].RTPport_infoCb(rtpport_id, info);
                if (rtpport_id_found)
                {
                    break;
                }
            }

            mut.ReleaseMutex();
        }

    }    
}
