using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Configuration;

namespace SirtapRadio
{
    public partial class FormSnmpData : Form
    {
        public FormSnmpData()
        {
            InitializeComponent();
        }

        private void FormSnmpData_Load(object sender, EventArgs e)
        {
            try
            {
                textBox_numChannels.Text = ConfigurationManager.AppSettings["numChannels"].ToString();
            }
            catch {
                textBox_numChannels.Text = "";
            }
            try
            {
                textBox_ChannelSelected.Text = ConfigurationManager.AppSettings["ChannelSelected"].ToString();
            }
            catch
            {
                textBox_ChannelSelected.Text = "";
            }
            try
            {
                textBox_FrequencyCh1.Text = ConfigurationManager.AppSettings["FrequencyCh1"].ToString();
            }
            catch
            {
                textBox_FrequencyCh2.Text = "";
            }
            try
            {
                textBox_FrequencyCh2.Text = ConfigurationManager.AppSettings["FrequencyCh2"].ToString();
            }
            catch
            {
                textBox_FrequencyCh2.Text = "";
            }
            try
            {
                textBox_PowerLevelCh1.Text = ConfigurationManager.AppSettings["PowerLevelCh1"].ToString();
            }
            catch
            {
                textBox_PowerLevelCh1.Text = "";
            }
            try
            {
                textBox_PowerLevelCh2.Text = ConfigurationManager.AppSettings["PowerLevelCh2"].ToString();
            }
            catch
            {
                textBox_PowerLevelCh2.Text = "";
            }
            try
            {
                textBox_TxInhibit.Text = ConfigurationManager.AppSettings["TxInhibit"].ToString();
            }
            catch
            {
                textBox_TxInhibit.Text = "";
            }
            try
            {
                textBox_WFCh1.Text = ConfigurationManager.AppSettings["WFCh1"].ToString();
            }
            catch
            {
                textBox_WFCh1.Text = "";
            }
            try
            {
                textBox_WFCh2.Text = ConfigurationManager.AppSettings["WFCh2"].ToString();
            }
            catch
            {
                textBox_WFCh2.Text = "";
            }
        }

        private void button_Save_Click(object sender, EventArgs e)
        {
            try
            {
                int n = int.Parse(textBox_numChannels.Text);
                if (n != 1 && n != 2) throw new Exception();
                Program.SetParamValueAppConfiguration("numChannels", textBox_numChannels.Text);
            }
            catch {
                MessageBox.Show("numChannels no valido. Debe ser 1 o 2");
            }
            try
            {
                int n = int.Parse(textBox_ChannelSelected.Text);
                if (n != 1 && n != 2) throw new Exception();
                Program.SetParamValueAppConfiguration("ChannelSelected", textBox_ChannelSelected.Text);
            }
            catch
            {
                MessageBox.Show("ChannelSelected no valido. Debe ser 1 o 2");
            }

            int FrequencyMin, FrequencyMax;
            try
            {
                FrequencyMin = int.Parse(ConfigurationManager.AppSettings["FrequencyMin"].ToString());
            }
            catch (Exception)
            {
                FrequencyMin = 30000;
                Program.SetParamValueAppConfiguration("FrequencyMin", FrequencyMin.ToString());
            }

            try
            {
                FrequencyMax = int.Parse(ConfigurationManager.AppSettings["FrequencyMax"].ToString());
            }
            catch (Exception)
            {
                FrequencyMax = 511975;
                Program.SetParamValueAppConfiguration("FrequencyMax", FrequencyMax.ToString());
            }

            try
            {
                int n = int.Parse(textBox_FrequencyCh1.Text);
                if (n < FrequencyMin && n > FrequencyMax) throw new Exception();
                Program.SetParamValueAppConfiguration("FrequencyCh1", textBox_FrequencyCh1.Text);
            }
            catch
            {
                MessageBox.Show($"FrequencyCh1 no valido. Debe ser > {FrequencyMin} < {FrequencyMax}");
            }

            try
            {
                int n = int.Parse(textBox_FrequencyCh2.Text);
                if (n < FrequencyMin && n > FrequencyMax) throw new Exception();
                Program.SetParamValueAppConfiguration("FrequencyCh2", textBox_FrequencyCh2.Text);
            }
            catch
            {
                MessageBox.Show($"FrequencyCh2 no valido. Debe estar entre {FrequencyMin} y {FrequencyMax}");
            }

            try
            {
                int n = int.Parse(textBox_PowerLevelCh1.Text);
                Program.SetParamValueAppConfiguration("PowerLevelCh1", textBox_PowerLevelCh1.Text);
            }
            catch
            {
                MessageBox.Show("PowerLevelCh1 no valido. Debe ser un entero");
            }

            try
            {
                int n = int.Parse(textBox_PowerLevelCh2.Text);
                Program.SetParamValueAppConfiguration("PowerLevelCh2", textBox_PowerLevelCh2.Text);
            }
            catch
            {
                MessageBox.Show("PowerLevelCh2 no valido. Debe ser un entero");
            }

            try
            {
                int n = int.Parse(textBox_TxInhibit.Text);
                if(n != 0 && n != 1) throw new Exception();
                Program.SetParamValueAppConfiguration("TxInhibit", textBox_TxInhibit.Text);
            }
            catch
            {
                MessageBox.Show("TxInhibit no valido. Deber ser 0 o 1");
            }

            try
            {
                Program.SetParamValueAppConfiguration("WFCh1", textBox_WFCh1.Text);
            }
            catch
            {
                MessageBox.Show("WFCh1 no valido.");
            }

            try
            {
                Program.SetParamValueAppConfiguration("WFCh2", textBox_WFCh2.Text);
            }
            catch
            {
                MessageBox.Show("WFCh2 no valido.");
            }

        }

        private void button_Close_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}
