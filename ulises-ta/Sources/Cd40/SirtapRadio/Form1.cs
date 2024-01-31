using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Configuration;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using SnmpAgent;

namespace SirtapRadio
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();            
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            try
            {
                textBox_dstIP.Text = ConfigurationManager.AppSettings["DstIP"].ToString();
            }
            catch { }
            try
            {
                textBox_RcvMcastIP.Text = ConfigurationManager.AppSettings["RcvMcastIP"].ToString();
            }
            catch { }
            try
            {
                textBox_SrcPort.Text = ConfigurationManager.AppSettings["SrcPort"].ToString();
            }
            catch { }
            try
            {
                textBox_DstPort.Text = ConfigurationManager.AppSettings["DstPort"].ToString();
            }
            catch { }
            try
            {
                comboBox_PaylType.Text = ConfigurationManager.AppSettings["PayloadType"].ToString();
            }
            catch { }

            try
            {
                textBox_dstIP2.Text = ConfigurationManager.AppSettings["DstIP2"].ToString();
            }
            catch { }
            try
            {
                textBox_RcvMcastIP2.Text = ConfigurationManager.AppSettings["RcvMcastIP2"].ToString();
            }
            catch { }
            try
            {
                textBox_SrcPort2.Text = ConfigurationManager.AppSettings["SrcPort2"].ToString();
            }
            catch { }
            try
            {
                textBox_DstPort2.Text = ConfigurationManager.AppSettings["DstPort2"].ToString();
            }
            catch { }
            try
            {
                comboBox_PaylType2.Text = ConfigurationManager.AppSettings["PayloadType2"].ToString();
            }
            catch { }


            if (Agent.Init() != 0)
            {
                Application.Exit();
            }

            try
            {
                string IpAddress = ConfigurationManager.AppSettings["BindIpAddress"].ToString();
                label_hostip.Text = IpAddress;
            }
            catch (Exception)
            {
                MessageBox.Show($"CORESIP_Init: Fichero config incorrecto. Parece que falta el parametro BindIpAddress", "ERROR",
                    MessageBoxButtons.OK, MessageBoxIcon.Error);                
            }

            button_ConfigSet_Click(sender, e);
            button_ConfigSet2_Click(sender, e);

            try
            {
                SnmpAgent.SnmpAgent.Init(label_hostip.Text, null, 161, 262);
                SnmpAgent.SnmpAgent.Store.Add(new SirtapGetChannels());
                SnmpAgent.SnmpAgent.Store.Add(new SirtapSetGetChannel());
                SnmpAgent.SnmpAgent.Store.Add(new SirtapSetGetpowerLevelCh1());
                SnmpAgent.SnmpAgent.Store.Add(new SirtapSetGetpowerLevelCh2());
                SnmpAgent.SnmpAgent.Store.Add(new SirtapSetGetfrequencyCh1());
                SnmpAgent.SnmpAgent.Store.Add(new SirtapSetGetfrequencyCh2());
                SnmpAgent.SnmpAgent.Store.Add(new SirtapSetGetTxInhibit());
                SnmpAgent.SnmpAgent.Store.Add(new SirtapSetGetWFCh1());
                SnmpAgent.SnmpAgent.Store.Add(new SirtapSetGetWFCh2());
                SnmpAgent.SnmpAgent.Store.Add(new SirtaperaseKeys());
                SnmpAgent.SnmpAgent.Store.Add(new SirtaploadKeys());

                SnmpAgent.SnmpAgent.Start();
            }
            catch (Exception ex)
            {
                MessageBox.Show($"SnmpAgent.Init: ERROR: {ex.Message}. La aplicacion se cerrara");
                Application.Exit();
            }


            generictimer.Start();
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            SnmpAgent.SnmpAgent.Close();
            Agent.End();
        }

        private void label1_Click(object sender, EventArgs e)
        {

        }

        private void label4_Click(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
        }

        private void button_ConfigSet_Click(object sender, EventArgs e)
        {
            IPAddress ipaddress;
            if (!System.Net.IPAddress.TryParse(textBox_dstIP.Text, out ipaddress))
            {
                MessageBox.Show("Configuring: Destination IP CH1 not valid");
                return;
            }
            if (!System.Net.IPAddress.TryParse(textBox_RcvMcastIP.Text, out ipaddress))
            {
                MessageBox.Show("Configuring: Multicast IP CH1 not valid");
                return;
            }
            int src_port;
            try
            {
                src_port = int.Parse(textBox_SrcPort.Text);
            }
            catch
            {
                MessageBox.Show("Configuring: Src port CH1 not valid");
                return;
            }

            if (src_port == Agent.SirtapRd2.Src_port)
            {
                MessageBox.Show("Configuring: Src port CH1 cannot be equal that CH2");
                return;
            }

            int dst_port;
            try
            {
                dst_port = int.Parse(textBox_DstPort.Text);
            }
            catch
            {
                MessageBox.Show("Configuring: Dst port CH1 not valid");
                return;
            }

            int payload;
            switch (comboBox_PaylType.Text)
            {
                case "PCMA":
                    payload = 8;
                    break;
                case "PCMU":
                    payload = 0;
                    break;
                default:
                    MessageBox.Show("Configuring: Payload Type CH1 not valid");
                    return;
            }

            Program.SetParamValueAppConfiguration("DstIP", textBox_dstIP.Text);
            Program.SetParamValueAppConfiguration("RcvMcastIP", textBox_RcvMcastIP.Text);
            Program.SetParamValueAppConfiguration("SrcPort", textBox_SrcPort.Text);
            Program.SetParamValueAppConfiguration("DstPort", textBox_DstPort.Text);
            Program.SetParamValueAppConfiguration("PayloadType", comboBox_PaylType.Text);

            if (Agent.SirtapRd.TheSameConfigIsAlreadyInitialized(textBox_dstIP.Text, src_port,
                dst_port, textBox_RcvMcastIP.Text, payload))
            {
                label_Status.Text = "RUNNING";
                return;
            }

            label_Status.Text = "STOPPED";
            label_Transmitting.Text = "STOPPED";
            button_SCH.Text = "SCH ON";
            button_bucle.Text = "BUCLE ON";

            string sch_file = "Hold.wav";
            try
            {
                sch_file = ConfigurationManager.AppSettings["SchFile1"].ToString();
            }
            catch {
                sch_file = "Hold.wav";
            }

            Agent.SirtapRd.Dispose();
            if (Agent.SirtapRd.Init(textBox_dstIP.Text, src_port, dst_port, textBox_RcvMcastIP.Text, payload, sch_file) == 0)
                label_Status.Text = "RUNNING";            
        }

        private void label_Transmitting_Click(object sender, EventArgs e)
        {

        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (Agent.SirtapRd.Transmitting) label_Transmitting.Text = "TRANSMITTING";
            else label_Transmitting.Text = "STOPPED";
            if (Agent.SirtapRd2.Transmitting) label_Transmitting2.Text = "TRANSMITTING";
            else label_Transmitting2.Text = "STOPPED";
        }

        private void label7_Click(object sender, EventArgs e)
        {

        }

        private void button_SCH_Click(object sender, EventArgs e)
        {
            if (Agent.SirtapRd.SQUELCH_activado)
            {
                string error = "";
                if (Agent.SirtapRd.Squelch(false) != 0)
                {
                    MessageBox.Show("ERROR: SQUELCH CH1: " + error);
                }
                else
                {
                    button_SCH.Text = "SCH ON";
                }
            }
            else
            {
                if (Agent.SirtapRd.Squelch(true) == 0)
                {
                    button_SCH.Text = "SCH OFF";
                }
            }
        }

        private void button_bucle_Click(object sender, EventArgs e)
        {
            Agent.SirtapRd.SetBucle(!Agent.SirtapRd.Bucle);
            if (Agent.SirtapRd.Bucle) button_bucle.Text = "BUCLE OFF";
            else button_bucle.Text = "BUCLE ON";
        }

        private void label2_Click(object sender, EventArgs e)
        {

        }

        private void button_ConfigSet2_Click(object sender, EventArgs e)
        {
            IPAddress ipaddress;
            if (!System.Net.IPAddress.TryParse(textBox_dstIP2.Text, out ipaddress))
            {
                MessageBox.Show("Configuring: Destination IP CH2 not valid");
                return;
            }
            if (!System.Net.IPAddress.TryParse(textBox_RcvMcastIP2.Text, out ipaddress))
            {
                MessageBox.Show("Configuring: Multicast IP CH2 not valid");
                return;
            }
            int src_port;
            try
            {
                src_port = int.Parse(textBox_SrcPort2.Text);
            }
            catch
            {
                MessageBox.Show("Configuring: Src port CH2 not valid");
                return;
            }

            if (src_port == Agent.SirtapRd.Src_port)
            {
                MessageBox.Show("Configuring: Src port CH2 cannot be equal that CH1");
                return;
            }

            int dst_port;
            try
            {
                dst_port = int.Parse(textBox_DstPort2.Text);
            }
            catch
            {
                MessageBox.Show("Configuring: Dst port CH2 not valid");
                return;
            }

            int payload;
            switch (comboBox_PaylType2.Text)
            {
                case "PCMA":
                    payload = 8;
                    break;
                case "PCMU":
                    payload = 0;
                    break;
                default:
                    MessageBox.Show("Configuring: Payload Type CH2 not valid");
                    return;
            }

            Program.SetParamValueAppConfiguration("DstIP2", textBox_dstIP2.Text);
            Program.SetParamValueAppConfiguration("RcvMcastIP2", textBox_RcvMcastIP2.Text);
            Program.SetParamValueAppConfiguration("SrcPort2", textBox_SrcPort2.Text);
            Program.SetParamValueAppConfiguration("DstPort2", textBox_DstPort2.Text);
            Program.SetParamValueAppConfiguration("PayloadType2", comboBox_PaylType2.Text);

            if (Agent.SirtapRd2.TheSameConfigIsAlreadyInitialized(textBox_dstIP2.Text, src_port,
                dst_port, textBox_RcvMcastIP2.Text, payload))
            {
                label_Status2.Text = "RUNNING";
                return;
            }

            label_Status2.Text = "STOPPED";
            label_Transmitting2.Text = "STOPPED";
            button_SCH2.Text = "SCH ON";
            button_bucle2.Text = "BUCLE ON";

            string sch_file = "Hold.wav";
            try
            {
                sch_file = ConfigurationManager.AppSettings["SchFile2"].ToString();
            }
            catch
            {
                sch_file = "Hold.wav";
            }

            Agent.SirtapRd2.Dispose();
            if (Agent.SirtapRd2.Init(textBox_dstIP2.Text, src_port, dst_port, textBox_RcvMcastIP2.Text, payload, sch_file) == 0)
                label_Status2.Text = "RUNNING";
        }

        private void button_SCH2_Click(object sender, EventArgs e)
        {
            if (Agent.SirtapRd2.SQUELCH_activado)
            {
                string error = "";
                if (Agent.SirtapRd2.Squelch(false) != 0)
                {
                    MessageBox.Show("ERROR: SQUELCH CH2: " + error);
                }
                else
                {
                    button_SCH2.Text = "SCH ON";
                }
            }
            else
            {
                if (Agent.SirtapRd2.Squelch(true) == 0)
                {
                    button_SCH2.Text = "SCH OFF";
                }
            }
        }

        private void button_bucle2_Click(object sender, EventArgs e)
        {
            Agent.SirtapRd2.SetBucle(!Agent.SirtapRd2.Bucle);
            if (Agent.SirtapRd2.Bucle) button_bucle2.Text = "BUCLE OFF";
            else button_bucle2.Text = "BUCLE ON";
        }
    }
}
