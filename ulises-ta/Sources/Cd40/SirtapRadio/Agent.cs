using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using U5ki.Infrastructure;
using System.Configuration;
using System.Diagnostics;
using System.IO;
using System.Windows.Forms;

namespace SirtapRadio
{
    internal class Agent
    {        
        public static SirtapRadio SirtapRd = new SirtapRadio();
        private static bool CoresipInitiated = false;
        private static CORESIP_Callbacks CoresipCallbacks = new CORESIP_Callbacks();

        public static int Init()
        {
            int ret = 0;
            SipAgent.CORESIP_Config ua_cfg = new SipAgent.CORESIP_Config();

            string param = "";
            try
            {
                param = "UserAgent";
                ua_cfg.UserAgent = ConfigurationManager.AppSettings["UserAgent"].ToString();
                param = "UserId";
                ua_cfg.HostId = ConfigurationManager.AppSettings["UserId"].ToString();
                param = "BindIpAddress";
                ua_cfg.IpAddress = ConfigurationManager.AppSettings["BindIpAddress"].ToString();
                param = "SipPort";
                ua_cfg.Port = uint.Parse(ConfigurationManager.AppSettings["SipPort"].ToString());
                param = "DefaultCodec";
                ua_cfg.DefaultCodec = ConfigurationManager.AppSettings["DefaultCodec"].ToString();
            }
            catch (Exception) {
                MessageBox.Show($"CORESIP_Init: Fichero config incorrecto. Parece que falta el parametro {param}. La aplicacion se cerrara", "ERROR",
                    MessageBoxButtons.OK, MessageBoxIcon.Error);
                return -1;
            }

            ua_cfg.DefaultDelayBufPframes = 3;
            ua_cfg.DefaultJBufPframes = 4;
            ua_cfg.SndSamplingRate = 8000;
            ua_cfg.RxLevel = 1;
            ua_cfg.TxLevel = 1;
            ua_cfg.LogLevel = 3;
            ua_cfg.TsxTout = 400;
            ua_cfg.InvProceedingIaTout = 1000;
            ua_cfg.InvProceedingMonitoringTout = 300000;
            ua_cfg.InvProceedingDiaTout = 300000;
            ua_cfg.InvProceedingRdTout = 1000;
            ua_cfg.EchoTail = 100;
            ua_cfg.EchoLatency = 0;
            ua_cfg.RecordingEd137 = 0;
            ua_cfg.max_calls = 8;
            ua_cfg.Radio_UA = 0;
            ua_cfg.UseDefaultSoundDevices = 0;            

            CoresipCallbacks.OnLog = LogCb;
            CoresipCallbacks.OnRTPport_info = RTPport_infoCb;

            ua_cfg.Cb = CoresipCallbacks;

            SipAgent.CORESIP_Error err;
            if (SipAgent.CORESIP_Init(ua_cfg, out err) != 0)
            {
                MessageBox.Show($"CORESIP_Init {err.Info}. La aplicacion se cerrara", "ERROR", 
                    MessageBoxButtons.OK, MessageBoxIcon.Error);
                ret = -1;
            }
            if (ret == 0) CoresipInitiated = true;

            if (ret == 0 && SipAgent.CORESIP_Start(out err) != 0)
            {
                MessageBox.Show($"CORESIP_Start {err.Info}. La aplicacion se cerrara", "ERROR",
                    MessageBoxButtons.OK, MessageBoxIcon.Error);
                ret = -1;
            }            

            return ret;
        }

        public static void End()
        {
            if (CoresipInitiated) SipAgent.CORESIP_End();
            CoresipInitiated = false;
        }

        static void LogCb(int level, string data, int len)
        {
            Write(data);
        }

        static void RTPport_infoCb(int rtpport_id, CORESIP_RTPport_info info)
        {
            SirtapRd.RTPport_infoCb(rtpport_id, info);
        }

        private static string _Path;
        private static string _Path0;
        private static string _Path1;
        public static void Write(string msg)
        {
            _Path = Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
            try
            {
                _Path0 = Path.Combine(_Path, "log.txt");
                _Path1 = Path.Combine(_Path, "log1.txt");               

                using (StreamWriter w = File.AppendText(_Path0))
                {
                    Log(msg, w);
                }

                FileInfo fi = new FileInfo(_Path0);
                if (fi.Length > 30000000)
                {
                    try
                    {
                        System.IO.File.Delete(_Path1);                                              
                    }
                    catch { }
                    System.IO.File.Move(_Path0, _Path1);
                }
            }
            catch (Exception)
            {
                //Handle
            }
        }

        static private void Log(string msg, TextWriter w)
        {
            try
            {
                //.Write(Environment.NewLine);
                w.Write("[{0} {1}]", DateTime.Now.ToShortDateString(), DateTime.Now.ToLongTimeString());
                w.Write("\t");
                w.WriteLine(" {0}", msg);
            }
            catch (Exception)
            {
                //Handle
            }
        }


    }
}
