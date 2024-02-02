using System;
using System.Collections.Generic;
using System.Diagnostics.Eventing.Reader;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Timers;
using U5ki.Infrastructure;
using static System.Collections.Specialized.BitVector32;
using static System.Net.Mime.MediaTypeNames;
using static U5ki.Infrastructure.SipAgent;
using Translate;

namespace U5ki.Gateway
{
    internal class NoED137Resource : BaseCode, IDisposable
    {
        public enum RdRsType
        {
            Rx,
            Tx,
            RxTx
        }

        private enum NOED137RadioTypes
        {
            SIRTAP
        }

        private class SIRTAP_Params
        {
            public int rtp_port_id = -1;
            public string dst_ip = "224.200.200.1";
            public int src_port = 7000;
            public int dst_port = 7002;
            public string local_multicast_ip = "224.200.200.1";
            public int payload_type = 8;   //PCMA            
            public bool transmiting = false;
            public bool receiving = false;
            public int channel = 1;            

            public u5ki.RemoteControlService.SNMPV3_priv_types priv_type = u5ki.RemoteControlService.SNMPV3_priv_types.AES;
            public u5ki.RemoteControlService.SNMPV3_auth_types auth_type = u5ki.RemoteControlService.SNMPV3_auth_types.SHA1;
            public string username = "usr-sha-aes";
            public string authpass = "authkey1";
            public string privpass = "privkey1";

            public u5ki.RemoteControlService.Sirtap RC = null;            
        }

        private bool Initialized = false;
        private CfgRecursoEnlaceExterno CfgResource;
        private AsignacionRecursosGW EquipoExterno;
        private DireccionamientoIP DireccionamientoIP;
        public string IdRecurso;
        private bool accountCreated = false;
        private bool Connected = false;
        private int CallId = -1;
        private RdRsType rdType = RdRsType.RxTx;
        private NOED137RadioTypes NOED137RadioType = NOED137RadioTypes.SIRTAP;
        private SIRTAP_Params sirtap_params;
        public System.Timers.Timer PollingPresenceTimer;

        public bool Presence = true;

        public NoED137Resource()
        {
            sirtap_params = new SIRTAP_Params();
        }

        public bool ConfigurationOfResourceChange(CfgRecursoEnlaceExterno res, AsignacionRecursosGW equipo_externo, DireccionamientoIP direccionamientoIP)
        {
            if (IdRecurso != res.IdRecurso) return true;
            if (sirtap_params.dst_ip != direccionamientoIP.IpRed1) return true;
            if (sirtap_params.src_port != (int)equipo_externo.SipPort) return true;
            if (sirtap_params.local_multicast_ip != direccionamientoIP.IpRed2) return true;
            return false;
        }

        public void Dispose()
        {
            Initialized = false;
            if (PollingPresenceTimer != null)
            {
                PollingPresenceTimer.Stop();
                PollingPresenceTimer.Dispose();
                PollingPresenceTimer = null;
            }

            if (NOED137RadioType == NOED137RadioTypes.SIRTAP)
            {
                Dispose_SIRTAP_Resource();
            }
            if (Connected && CallId != -1) SipAgent.HangupCall(CallId);
            DestroyGRSAccount();
        }

        public int Init(CfgRecursoEnlaceExterno res, AsignacionRecursosGW equipo_externo, DireccionamientoIP direccionamientoIP)
        {
            CfgResource = res;
            EquipoExterno = equipo_externo;
            DireccionamientoIP = direccionamientoIP;
            IdRecurso = res.IdRecurso;
            accountCreated = false;
            NOED137RadioType = NOED137RadioTypes.SIRTAP;            

            //HASTA QUE SE RESUELVA LA CONFIGURACION, DE MOMENTO TOMAMOS ASI LOS DATOS
            sirtap_params.dst_ip = DireccionamientoIP.IpRed2;
            sirtap_params.src_port = (int)EquipoExterno.SipPort;
            sirtap_params.dst_port = sirtap_params.src_port + 2;
            sirtap_params.local_multicast_ip = DireccionamientoIP.IpRed2;
            sirtap_params.payload_type = 8;
            //Si el identificador del recurso termina en 2, entonces consideramos que es el canal 2 de la radios SIRTAP
            //Si es cualquier otro caracter, entonces es el canal 1
            if (IdRecurso.ElementAt(IdRecurso.Length - 1) == 2) sirtap_params.channel = 2;
            else sirtap_params.channel = 1;

            PollingPresenceTimer = null;

            PollingPresenceTimer = new System.Timers.Timer(1000);
            PollingPresenceTimer.Elapsed += (sender, args) => PollingOnTimedEvent(sender, args, this);
            PollingPresenceTimer.AutoReset = true;
            PollingPresenceTimer.Enabled = true;            

            int ret = Init();
            if (ret == 0) Initialized = true;
            return ret;
        }

        public int Init()
        {
            int ret;

            ret = CreateGRSAccount();
            if (ret == 0 && NOED137RadioType == NOED137RadioTypes.SIRTAP)
            {
                ret = Create_SIRTAP_Resource();
            }

            if (ret != 0)
            {
                Dispose();
            }

            return ret;
        }

        public int CreateGRSAccount()
        {            
            try
            {
                SipAgent.CreateAccount(IdRecurso);
                accountCreated = true;
            }
            catch (Exception ex)
            {
                LogError<NoED137Resource>($"CreateGRSAccount: Creando account {IdRecurso} {ex.Message}");
                return -1;
            }

            uint Flags = 0;
            switch ((RdRsType)CfgResource.Tipo)
            {
                case RdRsType.Rx:
                    Flags = (uint)CORESIP_CallFlags.CORESIP_CALL_RD_RXONLY;
                    rdType = RdRsType.Rx;
                    break;
                case RdRsType.Tx:
                    Flags = (uint)CORESIP_CallFlags.CORESIP_CALL_RD_TXONLY;
                    rdType = RdRsType.Tx;
                    break;
                case RdRsType.RxTx:
                    Flags = 0;
                    rdType = RdRsType.RxTx;
                    break;
            }
            try
            {
                SipAgent.SetTipoGRS(IdRecurso, Flags, 1);
            }
            catch (Exception ex)
            {
                Dispose();
                LogError<NoED137Resource>($"CreateGRSAccount: SetTipoGRS {IdRecurso} {ex.Message}");
                return -1;
            }
            return 0;
        }

        public void DestroyGRSAccount()
        {
            if (accountCreated)
            {
                accountCreated = false;
                SipAgent.DestroyAccount(IdRecurso);
            }
        }       

        public void NOED137OnCallIncoming(int call, int call2replace, CORESIP_CallInfo info, CORESIP_CallInInfo inInfo)
        {
            if (!Initialized) return;

            PollingPresenceTimer.Enabled = false;

            if (inInfo.DstId != IdRecurso) SipAgent.AnswerCall(call, SipAgent.SIP_NOT_FOUND);
            else if (!accountCreated) SipAgent.AnswerCall(call, SipAgent.SIP_NOT_FOUND);
            else if (Connected) SipAgent.AnswerCall(call, SipAgent.SIP_DECLINE, 2008, null, false);
            else if (!Presence) SipAgent.AnswerCall(call, SipAgent.SIP_NOT_FOUND);
            else
            {
                CallId = call;                
                SipAgent.AnswerCall(call, SipAgent.SIP_OK);
            }

            PollingPresenceTimer.Enabled = true;
        }

        public void NOED137OnCallState(int call, CORESIP_CallInfo info, CORESIP_CallStateInfo stateInfo)
        {
            if (!Initialized) return;

            PollingPresenceTimer.Enabled = false; 

            if (call == CallId)
            {
                if (stateInfo.State == CORESIP_CallState.CORESIP_CALL_STATE_DISCONNECTED)
                {
                    CallId = -1;
                    Connected = false;
                    if (NOED137RadioType == NOED137RadioTypes.SIRTAP)
                    {
                        SIRTAP_PauseResumeDestroyRTPport(CORESIP_RTP_port_actions.CORESIP_PAUSE_ENCODING);
                    }
                }
                else if (stateInfo.State == CORESIP_CallState.CORESIP_CALL_STATE_CONFIRMED)
                {
                    Connected = true;
                    if (NOED137RadioType == NOED137RadioTypes.SIRTAP)
                    {
                        //Se solicita que al RTPport que invoque a la callback RTPport_infoCb para actualizar el estado
                        CORESIP_Error err;
                        int coreret = CORESIP_AskRTPport_info(sirtap_params.rtp_port_id, out err);
                        if (coreret != 0)
                        {
                            LogError<NoED137Resource>($"NOED137OnCallState: {IdRecurso} Error en CORESIP_AskRTPport_info: {err.Info}");
                        }
                    }
                }
            }

            PollingPresenceTimer.Enabled = true;
        }

        public bool NOED137OnKaTimeout(int call)
        {
            if (!Initialized) return false;

            PollingPresenceTimer.Enabled = false;

            bool found = false;
            if (call == CallId)
            {
                if (Connected && CallId != -1)
                {
                    SipAgent.HangupCall(CallId, 2001);
                    found = true;
                }
            }

            PollingPresenceTimer.Enabled = true;
            return found;
        }

        public bool NOED137OnRdInfo(int call, CORESIP_RdInfo info)
        {
            if (!Initialized) return false;

            PollingPresenceTimer.Enabled = false;
            bool found = false;
            if (call == CallId)
            {
                if (Connected && CallId != -1)
                {
                    if (NOED137RadioType == NOED137RadioTypes.SIRTAP)
                    {
                        SIRTAP_NOED137OnRdInfo(info);
                    }
                    found = true;
                }
            }
            PollingPresenceTimer.Enabled = true;
            return found;
        }        

        public bool RTPport_infoCb(int rtpport_id, CORESIP_RTPport_info info)
        {
            if (!Initialized) return false;

            PollingPresenceTimer.Enabled = false;

            bool found = false;
            if (sirtap_params.rtp_port_id == rtpport_id && sirtap_params.rtp_port_id != -1 && Connected && CallId != -1)
            {
                if (NOED137RadioType == NOED137RadioTypes.SIRTAP)
                {
                    SIRTAP_Set_SQUELCH(rtpport_id, info);
                }

                found = true;
            }
            PollingPresenceTimer.Enabled = true;

            return found;
        }

        private static void PollingOnTimedEvent(Object source, ElapsedEventArgs e, NoED137Resource noED137Res)
        {
            if (!noED137Res.Initialized) return;

            bool presence = true;
            if (noED137Res.NOED137RadioType == NOED137RadioTypes.SIRTAP)
            {
                presence = noED137Res.SIRTAP_Get_Presence();
            }

            if (!presence)
            {
                if (noED137Res.Presence)
                {
                    noED137Res.Presence = false;

                    noED137Res.LogError<NoED137Resource>("Equipo no presente",
                                   U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR, noED137Res.IdRecurso,
                                   CTranslate.translateResource("Equipo no presente"));

                    if (noED137Res.CallId != -1) SipAgent.HangupCall(noED137Res.CallId);
                }
            }
            else
            {
                if (!noED137Res.Presence)
                {
                    noED137Res.LogInfo<NoED137Resource>("Equipo presente",
                                   U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_INFO, noED137Res.IdRecurso,
                                   CTranslate.translateResource("Equipo presente"));

                    noED137Res.Presence = true;
                }
            }
        }

        #region SIRTAP_RESOURCE_FUNCTIONS

        int Create_SIRTAP_Resource()
        {
            int ret = 0;
            CORESIP_Error err;
            int coreret = SipAgent.CORESIP_CreateRTPport(out sirtap_params.rtp_port_id, sirtap_params.dst_ip,
                sirtap_params.src_port, sirtap_params.dst_port, sirtap_params.local_multicast_ip,
                sirtap_params.payload_type, CORESIP_RTP_port_actions.CORESIP_CREATE_ENCODING_DECODING, out err);
            if (coreret != 0)
            {
                LogError<NoED137Resource>($"Create_SIRTAP_Resource: {IdRecurso} Creating RTP port: {err.Info}");
                sirtap_params.rtp_port_id = -1;
                ret = -1;
            }
            if (ret == 0)
            {
                ret = SIRTAP_PauseResumeDestroyRTPport(CORESIP_RTP_port_actions.CORESIP_PAUSE_ENCODING);
            }
            sirtap_params.transmiting = false;

            if (ret == 0)
            {
                try
                {
                    sirtap_params.RC = new u5ki.RemoteControlService.Sirtap(161, Lextm.SharpSnmpLib.VersionCode.V3,
                        sirtap_params.auth_type, sirtap_params.priv_type,
                        sirtap_params.username, sirtap_params.authpass, sirtap_params.privpass)
                    { Id = IdRecurso };
                }
                catch (Exception ex)
                {
                    LogError<NoED137Resource>("Create_SIRTAP_Resource: " + ex.Message);
                    ret = -1;
                }
            }

            return ret;
        }

        void Dispose_SIRTAP_Resource()
        {
            if (sirtap_params.rtp_port_id != -1)
            {
                CORESIP_Error err;
                if (SipAgent.CORESIP_PauseResumeDestroyRTPport(sirtap_params.rtp_port_id,
                    CORESIP_RTP_port_actions.CORESIP_DESTROY, out err) != 0)
                {
                    LogError<NoED137Resource>($"Dispose_SIRTAP_Resource {IdRecurso} Destroying RTP port {sirtap_params.rtp_port_id} {sirtap_params.dst_port}: {err.Info}");
                }
                sirtap_params.rtp_port_id = -1;
                sirtap_params.transmiting = false;
                sirtap_params.receiving = false;
                sirtap_params.RC = null;
            }
        }

        private void SIRTAP_NOED137OnRdInfo(CORESIP_RdInfo info)
        {
            if (sirtap_params.rtp_port_id != -1)
            {
                if (info.PttType != CORESIP_PttType.CORESIP_PTT_OFF)
                {
                    if (!sirtap_params.transmiting)
                    {
                        sirtap_params.transmiting = true;
                        SIRTAP_PauseResumeDestroyRTPport(CORESIP_RTP_port_actions.CORESIP_RESUME_ENCODING);

                        //Se conecta el audio de la llamada con el rtp port
                        if (CallId != -1)
                        {
                            CORESIP_Error err;
                            if (CORESIP_BridgeLink(CallId, sirtap_params.rtp_port_id, 1, out err) != 0)
                            {
                                LogError<NoED137Resource>($"SIRTAP_NOED137OnRdInfo: CORESIP_BridgeLink {IdRecurso} CallId {CallId} sirtap_params.rtp_port_id {sirtap_params.rtp_port_id} : {err.Info}");
                            }
                        }
                    }
                }
                else
                {
                    if (sirtap_params.transmiting)
                    {
                        sirtap_params.transmiting = false;
                        SIRTAP_PauseResumeDestroyRTPport(CORESIP_RTP_port_actions.CORESIP_PAUSE_ENCODING);

                        //Se conecta el audio de la llamada con el rtp port
                        if (CallId != -1)
                        {
                            CORESIP_Error err;
                            if (CORESIP_BridgeLink(CallId, sirtap_params.rtp_port_id, 0, out err) != 0)
                            {
                                LogError<NoED137Resource>($"SIRTAP_NOED137OnRdInfo: CORESIP_BridgeLink {IdRecurso} CallId {CallId} sirtap_params.rtp_port_id {sirtap_params.rtp_port_id} : {err.Info}");
                            }
                        }
                    }
                }
                
            }
        }

        private int SIRTAP_PauseResumeDestroyRTPport(CORESIP_RTP_port_actions action)
        {
            int ret = 0;
            if (sirtap_params.rtp_port_id != -1)
            {
                CORESIP_Error err;
                if (SipAgent.CORESIP_PauseResumeDestroyRTPport(sirtap_params.rtp_port_id, action, out err) != 0)
                {
                    ret = -1;
                    LogError<NoED137Resource>($"SIRTAP_PauseResumeDestroyRTPport {IdRecurso} {sirtap_params.rtp_port_id} {action}: {err.Info}");
                }
            }
            else ret = -1;
            return ret;
        }

        private void SIRTAP_Set_SQUELCH(int rtpport_id, CORESIP_RTPport_info info)
        {
            if (sirtap_params.rtp_port_id == -1 || rtpport_id != sirtap_params.rtp_port_id) return;
            if (info.receiving == sirtap_params.receiving) return;
            sirtap_params.receiving = info.receiving;

            CORESIP_PttInfo pttinfo = new CORESIP_PttInfo();

            pttinfo.PttType = CORESIP_PttType.CORESIP_PTT_OFF;
            pttinfo.PttMute = (uint)CORESIP_PttMuteType.DESACTIVADO;

            if (info.receiving) pttinfo.Squ = 1;
            else pttinfo.Squ = 0;

            pttinfo.PttId = 0;
            pttinfo.RssiQidx = 15;

            CORESIP_Error err;

            //Se conecta o desconecta el audio del rtp port con la llamada
            if (CORESIP_BridgeLink(sirtap_params.rtp_port_id, CallId, (int) pttinfo.Squ, out err) != 0)
            {
                LogError<NoED137Resource>($"RTPport_infoCb: CORESIP_BridgeLink {IdRecurso} pttinfo.PttType {pttinfo.PttType} {pttinfo.Squ} : {err.Info}");
            }

            //Se envia el estado del Squelch en la llamada
            if (CORESIP_CallPtt(CallId, pttinfo, out err) != 0)
            {
                LogError<NoED137Resource>($"RTPport_infoCb: CORESIP_CallPtt {IdRecurso} pttinfo.PttType {pttinfo.PttType} {pttinfo.Squ} : {err.Info}");
            }
        }

        private bool SIRTAP_Get_Presence()
        {
            if (sirtap_params.RC == null) return false;

            bool presence = true;
            TlmdoAsk msg = new TlmdoAsk();
            msg.msgType = TlmdoAsk.MsgType.TLMDO_GET_FREQUENCY;
            msg.HostId = "SirtapGateway";
            msg.IdFrecuency = "0";
            msg.IdRecurso = IdRecurso;
            msg.Channel = sirtap_params.channel;

            string tlmdoIP = DireccionamientoIP.IpRed1;  //De momento es la primera ip del equipo externo
            TlmdoRsp response = new TlmdoRsp();

            TlmdoRsp.CodeTypes output = sirtap_params.RC.Tlmdo(msg, tlmdoIP, true, ref response, historic_report: false);
            if (output != TlmdoRsp.CodeTypes.TLMDO_CODE_OK && output != TlmdoRsp.CodeTypes.TLMDO_CODE_OTHER_IN_PROGRESS)
            {
                presence = false;                
            }
            return presence;
        }
        #endregion
    }
}
