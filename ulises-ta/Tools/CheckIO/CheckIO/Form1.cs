#define _GPIO_POLLING_
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using Nucleo.CheckIO;

namespace CheckIO
{
    /// <summary>
    /// 
    /// </summary>
    public enum eDevTypes { CMedia, Micronas };
    /// <summary>
    /// 
    /// </summary>
    public partial class Form1 : Form
    {
        Timer _checkingState;
        private byte[] SndDvcId = new byte[4];

        protected ISndDevManager SndDevManager = null;
        // protected Dictionary<byte, ISoundDevice> deviceList;
        protected SortedDictionary<byte, ISoundDevice> deviceList;

        /// <summary>
        /// 
        /// </summary>
        eDevTypes _type = eDevTypes.CMedia;

        public Form1()
        {
            InitializeComponent();

            label6.Text = "V " + System.Reflection.Assembly.GetExecutingAssembly().GetName().Version.ToString();

            cbType.SelectedIndex = 0;
            _type = eDevTypes.CMedia;
#if _GPIO_POLLING_
                cbType.Enabled = false;
#endif

            //byte cuantos = SndDevManager.Init();
            //SndDevManager.Start();
            //deviceList = SndDevManager.ListDevice();

            //_checkingState = new Timer();
            //_checkingState.Interval = 100;
            //_checkingState.Tick += new EventHandler(OnCheckingState);
            //_checkingState.Enabled = true;
            // Init();

            //byte i = 0;
            //foreach (SoundDevice sndDev in deviceList.Values)
            //{
            //    if (sndDev != null)
            //    {
            //        SndDvcId[i] = sndDev.Id;
            //        sndDev.StateChanged += DeviceChangingState;

            //        switch (i++)
            //        {
            //            case 0:
            //                GBDevice0.Enabled = sndDev.IsValid;
            //                GBDevice0.Text = "Device " + sndDev.Id.ToString();
            //                LblStatusA1_0.Text = sndDev.Jack ? "ON" : "OFF";
            //                LblStatusA2_0.Text = sndDev.Ptt ? "ON" : "OFF";
            //                LblStatusA1_0.ForeColor = sndDev.Jack ? Color.Green : Color.Red;
            //                LblStatusA2_0.ForeColor = sndDev.Ptt ? Color.Green : Color.Red;
            //                CBOut_0_A1.Checked = sndDev.Gpio6;
            //                CBOut_0_A2.Checked = sndDev.Gpio7;
            //                break;
            //            case 1:
            //                GBDevice1.Text = "Device " + sndDev.Id.ToString();
            //                GBDevice1.Enabled = sndDev.IsValid;
            //                LblStatusA1_1.Text = sndDev.Jack ? "ON" : "OFF";
            //                LblStatusA2_1.Text = sndDev.Ptt ? "ON" : "OFF";
            //                LblStatusA1_1.ForeColor = sndDev.Jack ? Color.Green : Color.Red;
            //                LblStatusA2_1.ForeColor = sndDev.Ptt ? Color.Green : Color.Red;
            //                CBOut_1_A1.Checked = sndDev.Gpio6;
            //                CBOut_1_A2.Checked = sndDev.Gpio7;
            //                break;
            //            case 2:
            //                GBDevice2.Text = "Device " + sndDev.Id.ToString();
            //                GBDevice2.Enabled = sndDev.IsValid;
            //                LblStatusA1_2.Text = sndDev.Jack ? "ON" : "OFF";
            //                LblStatusA2_2.Text = sndDev.Ptt ? "ON" : "OFF";
            //                LblStatusA1_2.ForeColor = sndDev.Jack ? Color.Green : Color.Red;
            //                LblStatusA2_2.ForeColor = sndDev.Ptt ? Color.Green : Color.Red;
            //                CBOut_2_A1.Checked = sndDev.Gpio6;
            //                CBOut_2_A2.Checked = sndDev.Gpio7;
            //                break;
            //            case 3:
            //                GBDevice3.Text = "Device " + sndDev.Id.ToString();
            //                GBDevice3.Enabled = sndDev.IsValid;
            //                LblStatusA1_3.Text = sndDev.Jack ? "ON" : "OFF";
            //                LblStatusA2_3.Text = sndDev.Ptt ? "ON" : "OFF";
            //                LblStatusA1_3.ForeColor = sndDev.Jack ? Color.Green : Color.Red;
            //                LblStatusA2_3.ForeColor = sndDev.Ptt ? Color.Green : Color.Red;
            //                CBOut_3_A1.Checked = sndDev.Gpio6;
            //                CBOut_3_A2.Checked = sndDev.Gpio7;
            //                break;
            //        }
            //    }
            //}
            ShowEstado();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void OnCheckingState(object sender, EventArgs e)
        {
            // Dictionary<byte, SoundDevice> deviceList = SndDevManager.ListDevice();

            try
            {
                _checkingState.Enabled = false;
                foreach (ISoundDevice sndDev in deviceList.Values)
                {
                    if (sndDev != null)
                    {
                        sndDev.GetState();
                    }
                }
                _checkingState.Enabled = true;
            }
            catch (Exception x)
            {
                NLog.LogManager.GetCurrentClassLogger().Trace("OnCheckingState: ", x);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void OnClose(object sender, FormClosedEventArgs e)
        {
            if (SndDevManager != null) SndDevManager.Close();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="jack"></param>
        /// <param name="ptt"></param>
        private void DeviceChangingState(object sender, bool jack, bool ptt)
        {
            //SoundDevice sndDev = (SoundDevice)sender;

            //int i = 0;
            //bool encontrado = false;
            //for (; i < 4; i++)
            //{
            //    if (SndDvcId[i] == sndDev.Id)
            //    {
            //        encontrado = true;
            //        break;
            //    }
            //}

            //if (encontrado)
            //{
            //    switch (i)
            //    {
            //        case 0:
            //            LblStatusA1_0.Text = sndDev.Jack ? "ON" : "OFF";
            //            LblStatusA2_0.Text = sndDev.Ptt ? "ON" : "OFF";
            //            LblStatusA1_0.ForeColor = sndDev.Jack ? Color.Green : Color.Red;
            //            LblStatusA2_0.ForeColor = sndDev.Ptt ? Color.Green : Color.Red;
            //            break;
            //        case 1:
            //            LblStatusA1_1.Text = sndDev.Jack ? "ON" : "OFF";
            //            LblStatusA2_1.Text = sndDev.Ptt ? "ON" : "OFF";
            //            LblStatusA1_1.ForeColor = sndDev.Jack ? Color.Green : Color.Red;
            //            LblStatusA2_1.ForeColor = sndDev.Ptt ? Color.Green : Color.Red;
            //            break;
            //        case 2:
            //            LblStatusA1_2.Text = sndDev.Jack ? "ON" : "OFF";
            //            LblStatusA2_2.Text = sndDev.Ptt ? "ON" : "OFF";
            //            LblStatusA1_2.ForeColor = sndDev.Jack ? Color.Green : Color.Red;
            //            LblStatusA2_2.ForeColor = sndDev.Ptt ? Color.Green : Color.Red;
            //            break;
            //        case 3:
            //            LblStatusA1_3.Text = sndDev.Jack ? "ON" : "OFF";
            //            LblStatusA2_3.Text = sndDev.Ptt ? "ON" : "OFF";
            //            LblStatusA1_3.ForeColor = sndDev.Jack ? Color.Green : Color.Red;
            //            LblStatusA2_3.ForeColor = sndDev.Ptt ? Color.Green : Color.Red;
            //            break;
            //    }
            //}

            foreach (ISoundDevice sndDev in deviceList.Values)
            {
                if (sndDev != null && sndDev.Error == true)
                {
                    _arrancado = false;
                    btStart.Text = "Start Check";
                    cbType.Enabled = true;
                    End();
                    break;
                }
            }

            ShowEstado();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void CBOutA1_CheckedChanged(object sender, EventArgs e)
        {
            //SoundDevice sndDev = SndDevManager.Device(SndDvcId[0]);
            ISoundDevice sndDev = SndDevManager.Device(SndDvcId[0]); // deviceList.Count > 0 ? deviceList[0] : null;
            System.Diagnostics.Debug.Assert(sndDev != null, "Dispositivo 0 no encontrado");
            sndDev.SetGpio(_type== eDevTypes.CMedia ? 4 : 6, (byte)(CBOut_0_A1.Checked ? 0x01 : 0x00));
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void CBOutA2_CheckedChanged(object sender, EventArgs e)
        {
            //SoundDevice sndDev = SndDevManager.Device(SndDvcId[0]);
            ISoundDevice sndDev = SndDevManager.Device(SndDvcId[0]);    //  deviceList.Count > 0 ? deviceList[0] : null;
            System.Diagnostics.Debug.Assert(sndDev != null, "Dispositivo 0 no encontrado");
            sndDev.SetGpio(_type== eDevTypes.CMedia ? 5 : 7, (byte)(CBOut_0_A2.Checked ? 0x01 : 0x00));
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void CBOut_1_A1_CheckedChanged(object sender, EventArgs e)
        {
            //SoundDevice sndDev = SndDevManager.Device(SndDvcId[1]);
            ISoundDevice sndDev = SndDevManager.Device(SndDvcId[1]);    // deviceList.Count > 1 ? deviceList[1] : null;
            System.Diagnostics.Debug.Assert(sndDev != null, "Dispositivo 1 no encontrado");
            sndDev.SetGpio(_type== eDevTypes.CMedia ? 4 : 6, (byte)(CBOut_1_A1.Checked ? 0x01 : 0x00));
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void CBOut_1_A2_CheckedChanged(object sender, EventArgs e)
        {
            //SoundDevice sndDev = SndDevManager.Device(SndDvcId[1]);
            ISoundDevice sndDev = SndDevManager.Device(SndDvcId[1]);    //  deviceList.Count > 1 ? deviceList[1] : null;
            System.Diagnostics.Debug.Assert(sndDev != null, "Dispositivo 1 no encontrado");
            sndDev.SetGpio(_type== eDevTypes.CMedia ? 5 : 7, (byte)(CBOut_1_A2.Checked ? 0x01 : 0x00));
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void CBOut_2_A1_CheckedChanged(object sender, EventArgs e)
        {
            //SoundDevice sndDev = SndDevManager.Device(SndDvcId[2]);
            ISoundDevice sndDev = SndDevManager.Device(SndDvcId[2]);    // deviceList.Count > 2 ? deviceList[2] : null;
            System.Diagnostics.Debug.Assert(sndDev != null, "Dispositivo 2 no encontrado");
            sndDev.SetGpio(_type== eDevTypes.CMedia ? 4 : 6, (byte)(CBOut_2_A1.Checked ? 0x01 : 0x00));
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void CBOut_2_A2_CheckedChanged(object sender, EventArgs e)
        {
            //SoundDevice sndDev = SndDevManager.Device(SndDvcId[2]);
            ISoundDevice sndDev = SndDevManager.Device(SndDvcId[2]);    // deviceList.Count > 2 ? deviceList[2] : null;
            System.Diagnostics.Debug.Assert(sndDev != null, "Dispositivo 2 no encontrado");
            sndDev.SetGpio(_type== eDevTypes.CMedia ? 5 : 7, (byte)(CBOut_2_A2.Checked ? 0x01 : 0x00));
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void CBOut_3_A1_CheckedChanged(object sender, EventArgs e)
        {
            //SoundDevice sndDev = SndDevManager.Device(SndDvcId[3]);
            ISoundDevice sndDev = SndDevManager.Device(SndDvcId[3]);    // deviceList.Count > 3 ? deviceList[3] : null;
            System.Diagnostics.Debug.Assert(sndDev != null, "Dispositivo 3 no encontrado");
            sndDev.SetGpio(_type== eDevTypes.CMedia ? 4 : 6, (byte)(CBOut_3_A1.Checked ? 0x01 : 0x00));
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void CBOut_3_A2_CheckedChanged(object sender, EventArgs e)
        {
            //SoundDevice sndDev = SndDevManager.Device(SndDvcId[3]);
            ISoundDevice sndDev = SndDevManager.Device(SndDvcId[3]);    // deviceList.Count > 3 ? deviceList[3] : null;
            System.Diagnostics.Debug.Assert(sndDev != null, "Dispositivo 3 no encontrado");
            sndDev.SetGpio(_type== eDevTypes.CMedia ? 5 : 7, (byte)(CBOut_3_A2.Checked ? 0x01 : 0x00));
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void cbType_SelectedIndexChanged(object sender, EventArgs e)
        {
            _type = cbType.SelectedIndex == 1 ? eDevTypes.Micronas : eDevTypes.CMedia;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        static bool _arrancado = false;
        private void btStart_Click(object sender, EventArgs e)
        {
            if (!_arrancado)
            {
                _arrancado = true;

                Init();
                btStart.Text = "Stop Check";
                cbType.Enabled = false;
            }
            else
            {
                _arrancado = false;

                btStart.Text = "Start Check";
#if !_GPIO_POLLING_

                cbType.Enabled = true;
#endif

                End();
            }
            ShowEstado();
        }

        /// <summary>
        /// 
        /// </summary>
        private void Init()
        {
            if (_type == eDevTypes.Micronas)
                SndDevManager = new MicronasDevManager();
            else
                SndDevManager = new CMediaDevManager();

            byte cuantos = SndDevManager.Init();
            SndDevManager.Start();
            deviceList = SndDevManager.ListDevice();

            _checkingState = new Timer();
            _checkingState.Interval = 100;
            _checkingState.Tick += new EventHandler(OnCheckingState);
            _checkingState.Enabled = true;

            // byte i = 0;
            foreach (ISoundDevice sndDev in deviceList.Values)
            {
                if (sndDev != null)
                {
                    // SndDvcId[i++] = sndDev.Id;
                    sndDev.StateChanged += DeviceChangingState;
                }
            }

            ShowEstado();
        }

        /// <summary>
        /// 
        /// </summary>
        private void End()
        {
            _checkingState.Enabled = false;
            _checkingState.Dispose();
            // deviceList.Clear();

            if (SndDevManager != null) SndDevManager.Close();
            SndDevManager = null;
        }

        /// <summary>
        /// 
        /// </summary>
        private void ShowEstado()
        {
            byte i = 0;
            if (deviceList != null && _arrancado)
            {
                foreach (ISoundDevice sndDev in deviceList.Values)
                {
                    if (sndDev != null)
                    {
                        SndDvcId[i] = sndDev.Id;

                        /** todo quitar */
                        //if (i!=sndDev.Id)
                        //    System.Diagnostics.Debug.Assert(false, "Error enumerando Devices...");

                        //sndDev.StateChanged += DeviceChangingState;

                        switch (i++)
                        {
                            case 0:
                                GBDevice0.Enabled = sndDev.IsValid;
                                GBDevice0.Text = "Device " + sndDev.Id.ToString();
                                LblStatusA1_0.Text = sndDev.Jack ? "ON" : "OFF";
                                LblStatusA2_0.Text = sndDev.Ptt ? "ON" : "OFF";
                                LblStatusA1_0.ForeColor = sndDev.Jack ? Color.Green : Color.Red;
                                LblStatusA2_0.ForeColor = sndDev.Ptt ? Color.Green : Color.Red;
                                CBOut_0_A1.Checked = sndDev.Gpio6;
                                CBOut_0_A2.Checked = sndDev.Gpio7;
                                break;
                            case 1:
                                GBDevice1.Text = "Device " + sndDev.Id.ToString();
                                GBDevice1.Enabled = sndDev.IsValid;
                                LblStatusA1_1.Text = sndDev.Jack ? "ON" : "OFF";
                                LblStatusA2_1.Text = sndDev.Ptt ? "ON" : "OFF";
                                LblStatusA1_1.ForeColor = sndDev.Jack ? Color.Green : Color.Red;
                                LblStatusA2_1.ForeColor = sndDev.Ptt ? Color.Green : Color.Red;
                                CBOut_1_A1.Checked = sndDev.Gpio6;
                                CBOut_1_A2.Checked = sndDev.Gpio7;
                                break;
                            case 2:
                                GBDevice2.Text = "Device " + sndDev.Id.ToString();
                                GBDevice2.Enabled = sndDev.IsValid;
                                LblStatusA1_2.Text = sndDev.Jack ? "ON" : "OFF";
                                LblStatusA2_2.Text = sndDev.Ptt ? "ON" : "OFF";
                                LblStatusA1_2.ForeColor = sndDev.Jack ? Color.Green : Color.Red;
                                LblStatusA2_2.ForeColor = sndDev.Ptt ? Color.Green : Color.Red;
                                CBOut_2_A1.Checked = sndDev.Gpio6;
                                CBOut_2_A2.Checked = sndDev.Gpio7;
                                break;
                            case 3:
                                GBDevice3.Text = "Device " + sndDev.Id.ToString();
                                GBDevice3.Enabled = sndDev.IsValid;
                                LblStatusA1_3.Text = sndDev.Jack ? "ON" : "OFF";
                                LblStatusA2_3.Text = sndDev.Ptt ? "ON" : "OFF";
                                LblStatusA1_3.ForeColor = sndDev.Jack ? Color.Green : Color.Red;
                                LblStatusA2_3.ForeColor = sndDev.Ptt ? Color.Green : Color.Red;
                                CBOut_3_A1.Checked = sndDev.Gpio6;
                                CBOut_3_A2.Checked = sndDev.Gpio7;
                                break;
                        }
                    }
                }
            }
            else
            {
                GBDevice0.Enabled = false;
                GBDevice1.Enabled = false;
                GBDevice2.Enabled = false;
                GBDevice3.Enabled = false;
            }
        }


    }   // Clase
}       // Namespace
