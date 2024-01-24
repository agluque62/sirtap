using System;
using System.Collections.Generic;
using System.Diagnostics.Eventing.Reader;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using U5ki.Infrastructure;
using static U5ki.Infrastructure.SipAgent;

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
        }

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
            if (NOED137RadioType == NOED137RadioTypes.SIRTAP)
            {
                if (sirtap_params.rtp_port_id != -1)
                {
                    CORESIP_Error err;
                    if (SipAgent.CORESIP_PauseResumeDestroyRTPport(sirtap_params.rtp_port_id,
                        CORESIP_RTP_port_actions.CORESIP_DESTROY, out err) != 0)
                    {
                        LogError<NoED137Resource>($"Dispose {IdRecurso} Destroying RTP port {sirtap_params.rtp_port_id} {sirtap_params.dst_port}: {err.Info}");                        
                    }
                    sirtap_params.rtp_port_id = -1;
                }
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
            sirtap_params.dst_ip = DireccionamientoIP.IpRed1;
            sirtap_params.src_port = (int)EquipoExterno.SipPort;
            sirtap_params.dst_port = sirtap_params.src_port + 2;
            sirtap_params.local_multicast_ip = DireccionamientoIP.IpRed2;
            sirtap_params.payload_type = 8;

            return Init();
        }

        public int Init()
        {
            int ret;

            ret = CreateGRSAccount();
            if (ret == 0 && NOED137RadioType == NOED137RadioTypes.SIRTAP)
            {
                CORESIP_Error err;
                int coreret = SipAgent.CORESIP_CreateRTPport(out sirtap_params.rtp_port_id, sirtap_params.dst_ip, 
                    sirtap_params.src_port, sirtap_params.dst_port, sirtap_params.local_multicast_ip,
                    sirtap_params.payload_type, CORESIP_RTP_port_actions.CORESIP_CREATE_DECODING, out err);
                if (coreret != 0)
                {
                    LogError<NoED137Resource>($"Init: {IdRecurso} Creating RTP port: {err.Info}");
                    sirtap_params.rtp_port_id = -1;
                    ret = -1;
                }
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
            if (inInfo.DstId != IdRecurso) SipAgent.AnswerCall(call, SipAgent.SIP_NOT_FOUND);
            else if (!accountCreated) SipAgent.AnswerCall(call, SipAgent.SIP_NOT_FOUND);
            else if (Connected) SipAgent.AnswerCall(call, SipAgent.SIP_DECLINE, 2008, null, false);
            else
            {
                CallId = call;                
                SipAgent.AnswerCall(call, SipAgent.SIP_OK);
            }
        }

        public void NOED137OnCallState(int call, CORESIP_CallInfo info, CORESIP_CallStateInfo stateInfo)
        {
            if (call != CallId) return;
            if (stateInfo.State == CORESIP_CallState.CORESIP_CALL_STATE_DISCONNECTED) 
            {
                CallId = -1;
                Connected = false;
            }
            else if (stateInfo.State == CORESIP_CallState.CORESIP_CALL_STATE_CONFIRMED) 
            {
                Connected = true;
            }
        }

        public bool NOED137OnKaTimeout(int call)
        {
            bool found = false;
            if (call != CallId) return found;
            if (Connected && CallId != -1)
            {
                SipAgent.HangupCall(CallId, 2001);
                found = true;
            }
            return found;
        }

        public bool NOED137OnRdInfo(int call, CORESIP_RdInfo info)
        {
            bool found = false;
            if (call != CallId) return found;
            if (Connected && CallId != -1)
            {
                if (sirtap_params.rtp_port_id != -1)
                {
                    CORESIP_RTP_port_actions action;
                    if (info.PttType != CORESIP_PttType.CORESIP_PTT_OFF) action = CORESIP_RTP_port_actions.CORESIP_CREATE_ENCODING_DECODING;
                    else action = CORESIP_RTP_port_actions.CORESIP_CREATE_DECODING;
                    CORESIP_Error err;
                    if (SipAgent.CORESIP_PauseResumeDestroyRTPport(sirtap_params.rtp_port_id,
                        action, out err) != 0)
                    {
                        LogError<NoED137Resource>($"NOED137OnRdInfo {IdRecurso} {sirtap_params.rtp_port_id} {sirtap_params.dst_port}: {err.Info}");
                    }
                }
                found = true;
            }
            return found;
        }

        public bool RTPport_infoCb(int rtpport_id, CORESIP_RTPport_info info)
        {
            bool found = false;
            if (rtpport_id == sirtap_params.rtp_port_id && Connected && CallId != -1)
            {
                CORESIP_PttInfo pttinfo = new CORESIP_PttInfo();

                pttinfo.PttType = CORESIP_PttType.CORESIP_PTT_OFF;
                pttinfo.PttMute = (uint)CORESIP_PttMuteType.DESACTIVADO;

                if (info.receiving) pttinfo.Squ = 1;
                else pttinfo.Squ = 0;

                pttinfo.PttId = 0;
                pttinfo.RssiQidx = 15;

                CORESIP_Error err;

                if (CORESIP_CallPtt(CallId, pttinfo, out err) != 0)
                {
                    LogError<NoED137Resource>($"RTPport_infoCb: CORESIP_CallPtt {IdRecurso} pttinfo.PttType {pttinfo.PttType} {pttinfo.Squ} : {err.Info}");
                }

                found = true;
            }
            return found;
        }

    }
}
