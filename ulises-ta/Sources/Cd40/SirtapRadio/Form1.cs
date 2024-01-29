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
            generictimer.Start();
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
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
                MessageBox.Show("Configuring: Destination IP not valid");
                return;
            }
            if (!System.Net.IPAddress.TryParse(textBox_RcvMcastIP.Text, out ipaddress))
            {
                MessageBox.Show("Configuring: Multicast IP not valid");
                return;
            }
            int src_port;
            try
            {
                src_port = int.Parse(textBox_SrcPort.Text);
            }
            catch
            {
                MessageBox.Show("Configuring: Src port not valid");
                return;
            }
            int dst_port;
            try
            {
                dst_port = int.Parse(textBox_DstPort.Text);
            }
            catch
            {
                MessageBox.Show("Configuring: Dst port not valid");
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
                    MessageBox.Show("Configuring: Payload Type not valid");
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

            Agent.SirtapRd.Dispose();
            if (Agent.SirtapRd.Init(textBox_dstIP.Text, src_port, dst_port, textBox_RcvMcastIP.Text, payload) == 0)
                label_Status.Text = "RUNNING";            
        }

        private void label_Transmitting_Click(object sender, EventArgs e)
        {

        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (Agent.SirtapRd.Transmitting) label_Transmitting.Text = "TRANSMITTING";
            else label_Transmitting.Text = "STOPPED";
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
                    MessageBox.Show(error);
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
    }
}
