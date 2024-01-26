using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using static U5ki.Infrastructure.SipAgent;
using U5ki.Infrastructure;
using System.Windows.Forms;

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
        public bool SQUELCH_activado = false;
        public bool Transmitting = false;
        private int sch_wavplayer = -1;
        public bool Bucle = false;

        public bool TheSameConfigIsAlreadyInitialized(string dst_ip, int src_port, int dst_port, string local_multicast_ip, int payload_type)
        {
            if (Rtp_port_id != -1 && Dst_ip == dst_ip && Src_port == src_port && 
                Dst_port == dst_port && Local_multicast_ip == local_multicast_ip && Payload_type == payload_type)
            {
                return true;
            }
            return false;
        }

        public int Init(string dst_ip, int src_port, int dst_port, string local_multicast_ip, int payload_type, out string error)
        {
            int ret = 0;
            error = "";
            Transmitting = false;

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

            if (ret == 0)
            {
                Dst_ip = dst_ip;
                Src_port = src_port;
                Dst_port = dst_port;
                Local_multicast_ip = local_multicast_ip;
                Payload_type = payload_type;
            }

            return ret;
        }

        public int Dispose(out string error)
        {
            int ret = 0;
            error = "";

            if (sch_wavplayer != -1)
            {
                CORESIP_Error err;
                CORESIP_DestroyWavPlayer(sch_wavplayer, out err);
                sch_wavplayer = -1;
            }

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

            Transmitting = false;
            Bucle = false;
            SQUELCH_activado = false;

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
                        error = "Squelch: CORESIP_PauseResumeDestroyRTPport " + err.Info;
                        ret = -1;
                    }
                    else
                    {
                        SQUELCH_activado = on;
                        CORESIP_Error error1;
                        if (sch_wavplayer == -1)
                        {
                            coreret = CORESIP_CreateWavPlayer("Hold.wav", 1, out sch_wavplayer, out error1);
                            if (coreret != 0)
                            {
                                MessageBox.Show("Squelch: " + error1.Info);
                                sch_wavplayer = -1;
                            }
                        }
                            
                        if (Bucle)
                        {
                            coreret = CORESIP_BridgeLink(Rtp_port_id, Rtp_port_id, on ? 0 : 1, out error1);
                            if (coreret != 0)
                            {
                                MessageBox.Show("Squelch cortando audio bucle: " + error1.Info);
                            }
                        }

                        if (sch_wavplayer != -1)
                        {
                            coreret = CORESIP_BridgeLink(sch_wavplayer, Rtp_port_id, 1, out error1);
                            if (coreret != 0)
                            {
                                MessageBox.Show("Squelch: " + error1.Info);
                            }
                        }
                    }
                }            
            }

            return ret;
        }

        public void SetBucle(bool on)
        {
            if (Rtp_port_id != -1)
            {
                CORESIP_Error error1;
                int coreret = CORESIP_BridgeLink(Rtp_port_id, Rtp_port_id, on? 1:0, out error1);
                if (coreret != 0)
                {
                    MessageBox.Show("SetBucle: " +  error1.Info);
                }
                else
                {
                    Bucle = on;
                }
            }
        }

        public void RTPport_infoCb(int rtpport_id, CORESIP_RTPport_info info)
        {
            if (rtpport_id != Rtp_port_id) return;
            Transmitting = info.receiving;
        }
    }
}
