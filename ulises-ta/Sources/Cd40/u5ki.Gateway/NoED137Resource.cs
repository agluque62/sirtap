using System;
using System.Collections.Generic;
using System.Diagnostics.Eventing.Reader;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using U5ki.Infrastructure;
using static System.Collections.Specialized.BitVector32;
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
            public bool transmiting = false;
            public bool receiving = false;
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
                if (NOED137RadioType == NOED137RadioTypes.SIRTAP)
                {
                    SIRTAP_NOED137OnRdInfo(info);
                }
                found = true;
            }
            return found;
        }        

        public bool RTPport_infoCb(int rtpport_id, CORESIP_RTPport_info info)
        {
            bool found = false;
            if (sirtap_params.rtp_port_id != -1 && Connected && CallId != -1)
            {
                if (NOED137RadioType == NOED137RadioTypes.SIRTAP)
                {
                    SIRTAP_Set_SQUELCH(rtpport_id, info);
                }

                found = true;
            }
            
            return found;
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
                    }
                }
                else
                {
                    if (sirtap_params.transmiting)
                    {
                        sirtap_params.transmiting = false;
                        SIRTAP_PauseResumeDestroyRTPport(CORESIP_RTP_port_actions.CORESIP_PAUSE_ENCODING);
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

            if (CORESIP_CallPtt(CallId, pttinfo, out err) != 0)
            {
                LogError<NoED137Resource>($"RTPport_infoCb: CORESIP_CallPtt {IdRecurso} pttinfo.PttType {pttinfo.PttType} {pttinfo.Squ} : {err.Info}");
            }
        }

        #endregion
    }
}
