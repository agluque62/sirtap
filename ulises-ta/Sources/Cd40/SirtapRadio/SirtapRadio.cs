using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using static U5ki.Infrastructure.SipAgent;
using U5ki.Infrastructure;

namespace SirtapRadio
{
    internal class SirtapRadio
    {
        private const int PCMU = 0;
        private const int PCMA = 8;

        private string Dst_ip = "";
        private int Src_port = 0;
        private int Dst_port = 0;
        private string Local_multicast_ip = "";
        private int Payload_type = PCMA;
        private int Rtp_port_id = -1;
        private bool SQUELCH_activado = false;

        public int Init(string dst_ip, int src_port, int dst_port, string local_multicast_ip, int payload_type, out string error)
        {
            int ret = 0;
            error = "";

            if (Rtp_port_id != -1)
            {
                error = "Init: La radio ya esta inicializada. Debe Destruirse previamente";
                ret = -1;
                return ret;
            }
            
            CORESIP_Error err;
            int coreret = CORESIP_CreateRTPport(out Rtp_port_id, dst_ip, src_port, dst_port, local_multicast_ip, payload_type, 
                CORESIP_RTP_port_actions.CORESIP_CREATE_ENCODING_DECODING, out err);
            if (coreret != 0) 
            {
                error = "Init: CORESIP_CreateRTPport " + err.Info;
                ret = -1;
                Rtp_port_id = -1;
            }

            if (ret == 0)
            {
                coreret = CORESIP_PauseResumeDestroyRTPport(Rtp_port_id, CORESIP_RTP_port_actions.CORESIP_PAUSE_ENCODING, out err);
                if (coreret != 0)
                {
                    error = "Init: CORESIP_PauseResumeDestroyRTPport " + err.Info;
                    ret = -1;
                }
            }

            return ret;
        }

        public int Dispose(out string error)
        {
            int ret = 0;
            error = "";

            if (Rtp_port_id != -1)
            {
                CORESIP_Error err;
                int coreret = CORESIP_PauseResumeDestroyRTPport(Rtp_port_id, CORESIP_RTP_port_actions.CORESIP_DESTROY, out err);
                if (coreret != 0)
                {
                    error = "Init: CORESIP_PauseResumeDestroyRTPport " + err.Info;
                    ret = -1;
                }
                else
                {
                    Rtp_port_id = -1;
                }
            }

            return ret;
        }

        public int Squelch(bool on, out string error)
        {
            int ret = 0;
            error = "";
                            
            if (Rtp_port_id == -1)
            {
                error = "ERROR: Squelch La radio no esta inicializada";
                ret = -1;
            }

            if ((on && !SQUELCH_activado) || (!on && SQUELCH_activado))
            {
                if (ret == 0)
                {
                    CORESIP_Error err;
                    CORESIP_RTP_port_actions action = CORESIP_RTP_port_actions.CORESIP_PAUSE_ENCODING;
                    if (on) action = CORESIP_RTP_port_actions.CORESIP_RESUME_ENCODING;
                    else action = CORESIP_RTP_port_actions.CORESIP_PAUSE_ENCODING;

                    int coreret = CORESIP_PauseResumeDestroyRTPport(Rtp_port_id, action, out err);
                    if (coreret != 0)
                    {
                        error = "Init: CORESIP_PauseResumeDestroyRTPport " + err.Info;
                        ret = -1;
                    }
                    else
                    {
                        SQUELCH_activado = on;
                    }
                }
            }

            return ret;
        }

        public void RTPport_infoCb(int rtpport_id, CORESIP_RTPport_info info)
        {
            if (rtpport_id != Rtp_port_id) return;
        }
    }
}
