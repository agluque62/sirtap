using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using static U5ki.Infrastructure.SipAgent;
using U5ki.Infrastructure;
using System.Windows.Forms;
using System.Threading;

namespace SirtapRadio
{
    internal class SirtapRadio
    {
        private const int PCMU = 0;
        private const int PCMA = 8;

        private string Dst_ip = "";
        public int Src_port = 0;
        public int Dst_port = 0;
        private string Local_multicast_ip = "";
        private int Payload_type = PCMA;
        private int Rtp_port_id = -1;
        public bool SQUELCH_activado = false;
        public bool Transmitting = false;
        private int sch_wavplayer = -1;
        public bool Bucle = false;
        private string Sch_file = "HOLD.wav";

        private Mutex mut = new Mutex();

        public bool TheSameConfigIsAlreadyInitialized(string dst_ip, int src_port, int dst_port, string local_multicast_ip, int payload_type)
        {
            bool ret = false;
            mut.WaitOne();
            if (Rtp_port_id != -1 && Dst_ip == dst_ip && Src_port == src_port && 
                Dst_port == dst_port && Local_multicast_ip == local_multicast_ip && Payload_type == payload_type)
            {
                ret = true;
            }
            mut.ReleaseMutex();
            return ret;
        }

        public int Init(string dst_ip, int src_port, int dst_port, string local_multicast_ip, int payload_type, string sch_file)
        {
            int ret = 0;
            string messagebox_text = "";

            mut.WaitOne();

            Transmitting = false;

            if (Rtp_port_id != -1)
            {
                messagebox_text = "Init: La radio ya esta inicializada. Debe Destruirse previamente";
                ret = -1;                
            }

            if (ret == 0)
            {
                CORESIP_Error err;
                int coreret = CORESIP_CreateRTPport(out Rtp_port_id, dst_ip, src_port, dst_port, local_multicast_ip, payload_type,
                    CORESIP_RTP_port_actions.CORESIP_CREATE_ENCODING_DECODING, out err);
                if (coreret != 0)
                {
                    messagebox_text += " Init: CORESIP_CreateRTPport " + err.Info;
                    ret = -1;
                    Rtp_port_id = -1;
                }
            }

            if (ret == 0)
            {
                CORESIP_Error err;
                int coreret = CORESIP_PauseResumeDestroyRTPport(Rtp_port_id, CORESIP_RTP_port_actions.CORESIP_PAUSE_ENCODING, out err);
                if (coreret != 0)
                {
                    messagebox_text = " Init: CORESIP_PauseResumeDestroyRTPport " + err.Info;
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
                Sch_file = sch_file;
            }

            mut.ReleaseMutex();

            if (messagebox_text.Length > 0)
            {
                MessageBox.Show(messagebox_text);
            }

            return ret;
        }

        public int Dispose()
        {
            int ret = 0;
            string messagebox_text = "";

            mut.WaitOne();

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
                    messagebox_text = "Init: CORESIP_PauseResumeDestroyRTPport " + err.Info;
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

            mut.ReleaseMutex();

            if (messagebox_text.Length > 0)
            {
                MessageBox.Show(messagebox_text);
            }

            return ret;
        }

        public int Squelch(bool on)
        {
            int ret = 0;
            string messagebox_text = "";

            mut.WaitOne();

            if (Rtp_port_id == -1)
            {
                messagebox_text += "ERROR: Squelch La radio no esta inicializada";
                ret = -1;
            }

            if (ret == 0 && ((on && !SQUELCH_activado) || (!on && SQUELCH_activado)))
            {                
                if (on)
                {
                    CORESIP_Error err;
                    int coreret = CORESIP_PauseResumeDestroyRTPport(Rtp_port_id, CORESIP_RTP_port_actions.CORESIP_RESUME_ENCODING, out err);
                    if (coreret != 0)
                    {
                        messagebox_text += " Squelch: CORESIP_PauseResumeDestroyRTPport " + err.Info;
                        ret = -1;
                    }

                    if (ret == 0)
                    {
                        if (sch_wavplayer == -1)
                        {
                            CORESIP_Error error1;
                            coreret = CORESIP_CreateWavPlayer(Sch_file, 1, out sch_wavplayer, out error1);
                            if (coreret != 0)
                            {
                                messagebox_text += " Squelch: " + error1.Info;
                                sch_wavplayer = -1;
                            }
                        }
                    }

                    if (ret == 0)
                    {
                        if (sch_wavplayer != -1)
                        {
                            CORESIP_Error error1;
                            coreret = CORESIP_BridgeLink(sch_wavplayer, Rtp_port_id, 1, out error1);
                            if (coreret != 0)
                            {
                                messagebox_text += " Squelch: " + error1.Info;
                            }
                        }
                    }

                    //Quitamos bucle de audio
                    coreret = CORESIP_BridgeLink(Rtp_port_id, Rtp_port_id, 0, out err);
                    if (coreret != 0)
                    {
                        messagebox_text += " Squelch cortando audio bucle: " + err.Info;
                    }
                }
                else
                {
                    if (!Bucle || (Bucle && !Transmitting))
                    {
                        CORESIP_Error err;
                        int coreret = CORESIP_PauseResumeDestroyRTPport(Rtp_port_id, CORESIP_RTP_port_actions.CORESIP_PAUSE_ENCODING, out err);
                        if (coreret != 0)
                        {
                            messagebox_text += " Squelch: CORESIP_PauseResumeDestroyRTPport " + err.Info;
                            ret = -1;
                        }
                    }

                    if (sch_wavplayer != -1)
                    {
                        CORESIP_Error error1;
                        int coreret = CORESIP_BridgeLink(sch_wavplayer, Rtp_port_id, 0, out error1);
                        if (coreret != 0)
                        {
                            messagebox_text += " Squelch: " + error1.Info;
                        }
                    }

                    if (Bucle && Transmitting)
                    {
                        CORESIP_Error err;
                        int coreret = CORESIP_BridgeLink(Rtp_port_id, Rtp_port_id, 1, out err);
                        if (coreret != 0)
                        {
                            messagebox_text += " Squelch restaurando audio bucle: " + err.Info;
                        }
                    }
                }
                SQUELCH_activado = on;
            }

            mut.ReleaseMutex();

            if (messagebox_text.Length > 0)
            {
                MessageBox.Show(messagebox_text);
            }

            return ret;
        }

        public void SetBucle(bool on)
        {
            string messagebox_text = "";

            mut.WaitOne();
            if (Rtp_port_id != -1)
            {                
                Bucle = on;
            }
            if (Bucle)
            {
                if (Transmitting && !SQUELCH_activado)
                {
                    CORESIP_Error err;
                    int coreret = CORESIP_PauseResumeDestroyRTPport(Rtp_port_id, CORESIP_RTP_port_actions.CORESIP_RESUME_ENCODING, out err);
                    if (coreret != 0)
                    {
                        messagebox_text += " SetBucle: Activando bucle porque transmite. " + err.Info;
                    }
                    else
                    {
                        coreret = CORESIP_BridgeLink(Rtp_port_id, Rtp_port_id, on ? 1 : 0, out err);
                        if (coreret != 0)
                        {
                            messagebox_text += " SetBucle: Activando bucle de audio porque transmite. " + err.Info;
                        }
                    }
                }
                else if (!Transmitting && !SQUELCH_activado)
                {
                    CORESIP_Error err;
                    int coreret = CORESIP_PauseResumeDestroyRTPport(Rtp_port_id, CORESIP_RTP_port_actions.CORESIP_PAUSE_ENCODING, out err);
                    if (coreret != 0)
                    {
                        messagebox_text += " SetBucle: Desactivando bucle porque no transmite. " + err.Info;
                    }
                }
            }
            else if (!SQUELCH_activado)
            {
                CORESIP_Error err;
                int coreret = CORESIP_PauseResumeDestroyRTPport(Rtp_port_id, CORESIP_RTP_port_actions.CORESIP_PAUSE_ENCODING, out err);
                if (coreret != 0)
                {
                    messagebox_text += " SetBucle: Desactivando bucle porque no transmite y no hay squelch. " + err.Info;
                }
            }

            mut.ReleaseMutex();

            if (messagebox_text.Length > 0)
            {
                MessageBox.Show(messagebox_text);
            }
        }

        public void RTPport_infoCb(int rtpport_id, CORESIP_RTPport_info info)
        {
            mut.WaitOne();
            if (rtpport_id == Rtp_port_id)
            {
                Transmitting = info.receiving;
                if (Bucle)
                {
                    if (Transmitting && !SQUELCH_activado)
                    {
                        CORESIP_Error err;
                        CORESIP_PauseResumeDestroyRTPport(Rtp_port_id, CORESIP_RTP_port_actions.CORESIP_RESUME_ENCODING, out err);
                        CORESIP_BridgeLink(Rtp_port_id, Rtp_port_id, 1, out err);
                    }
                    else if (!Transmitting && !SQUELCH_activado)
                    {
                        CORESIP_Error err;
                        CORESIP_PauseResumeDestroyRTPport(Rtp_port_id, CORESIP_RTP_port_actions.CORESIP_PAUSE_ENCODING, out err);
                        CORESIP_BridgeLink(Rtp_port_id, Rtp_port_id, 0, out err);
                    }
                }
            }
            mut.ReleaseMutex();
        }
    }
}
