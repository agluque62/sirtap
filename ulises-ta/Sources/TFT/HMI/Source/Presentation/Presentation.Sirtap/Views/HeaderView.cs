//----------------------------------------------------------------------------------------
// patterns & practices - Smart Client Software Factory - Guidance Package
//
// This file was generated by the "Add View" recipe.
//
// This class is the concrete implementation of a View in the Model-View-Presenter 
// pattern. Communication between the Presenter and this class is acheived through 
// an interface to facilitate separation and testability.
// Note that the Presenter generated by the same recipe, will automatically be created
// by CAB through [CreateNew] and bidirectional references will be added.
//
// For more information see:
// ms-help://MS.VSCC.v80/MS.VSIPCC.v80/ms.practices.scsf.2007may/SCSF/html/02-09-010-ModelViewPresenter_MVP.htm
//
// Latest version of this Guidance Package: http://go.microsoft.com/fwlink/?LinkId=62182
//----------------------------------------------------------------------------------------

using HMI.Infrastructure.Interface;
using HMI.Model.Module.BusinessEntities;
using HMI.Model.Module.Messages;
using HMI.Model.Module.Services;
using HMI.Model.Module.UI;
using HMI.Presentation.Sirtap.Constants;
using HMI.Presentation.Sirtap.Properties;
using Microsoft.Practices.CompositeUI;
using Microsoft.Practices.CompositeUI.EventBroker;
using Microsoft.Practices.CompositeUI.SmartParts;
using Microsoft.Practices.ObjectBuilder;
using NLog;
using System;
using System.Collections.Generic;
using System.Configuration;
using System.Diagnostics;
using System.Drawing;
using System.Windows.Forms;

namespace HMI.Presentation.Sirtap.Views
{
    [SmartPart]
    public partial class HeaderView : UserControl
    {
        private static Logger _Logger = LogManager.GetCurrentClassLogger();

        private IModelCmdManagerService _CmdManager;
        private StateManagerService _StateManager;
        private List<string> _ConfUnused = new List<string>();

        private bool _TitleEnabled
        {
            get { return _StateManager.Tft.Enabled; }
        }
        private bool _SplitEnabled
        {
            get { return _StateManager.Tft.Enabled && _StateManager.Engine.Operative && _StateManager.Tft.Login; }
        }
        private bool _InfoEnabled
        {
            get { return _StateManager.Tft.Enabled && (_StateManager.Tlf.Listen.State == FunctionState.Idle && !_StateManager.Tlf.ListenBy.IsListen && _StateManager.Tft.Login); }
        }
        private bool _BrightnessEnabled
        {
            get { return _StateManager.Tft.Enabled && _StateManager.Brightness.Open && _StateManager.Tft.Login; }
        }
        private bool _BuzzerEnabled
        {
            get { return _StateManager.Tft.Enabled && _StateManager.Engine.Operative && _StateManager.Tft.Login; }
        }

        private bool _AAlarmaEnabled
        {
            get { return _StateManager.Tft.Enabled && _StateManager.Engine.Operative && _StateManager.Tft.Login; }
        }

        private string _Info // Miguel
        {
            get { return Resources.INF; }
        }


        private static Image CambiarTamanoImagen(Image image, Size size, bool preserveAspectRatio = true)
        {
            int newWidth;
            int newHeight;
            if (image == null)
                image = new Bitmap(size.Width, size.Height);
            if (preserveAspectRatio)
            {
                int originalWidth = image.Width;
                int originalHeight = image.Height;
                float percentWidth = (float)size.Width / (float)originalWidth;
                float percentHeight = (float)size.Height / (float)originalHeight;
                float percent = percentHeight < percentWidth ? percentHeight : percentWidth;
                newWidth = (int)(originalWidth * percent);
                newHeight = (int)(originalHeight * percent);
            }
            else
            {
                newWidth = size.Width;
                newHeight = size.Height;
            }
            Image newImage = new Bitmap(newWidth, newHeight);
            using (Graphics graphicsHandle = Graphics.FromImage(newImage))
            {
                graphicsHandle.InterpolationMode = System.Drawing.Drawing2D.InterpolationMode.HighQualityBicubic;
                graphicsHandle.DrawImage(image, 0, 0, newWidth, newHeight);
            }
            return newImage;
        }


        public HeaderView([ServiceDependency] IModelCmdManagerService cmdManager, [ServiceDependency] StateManagerService stateManager)
        {
            InitializeComponent();
            BackColor = VisualStyle.Colors.HeaderBlue;

            _CmdManager = cmdManager;
            _StateManager = stateManager;


            //_TitleBT.BackgroundImage = _StateManager.Title.Logo;
            if (!VisualStyle.ModoNocturno)
                ;// _TitleBT.ImageNormal = CambiarTamanoImagen(_StateManager.Title.Logo, new System.Drawing.Size(_StateManager.Title.WidthLogo, _StateManager.Title.HeightJacks));
            else
                ;// _TitleBT.ImageNormal = CambiarTamanoImagen(_StateManager.Title.LogoMn, new System.Drawing.Size(_StateManager.Title.WidthLogo, _StateManager.Title.HeightLogo));
            _TitleBT.Text = _StateManager.Title.Id;
            _TitleBT.DrawX = !_StateManager.Engine.Operative;
            //LALM 210212 Cambio colores del bitmap de AENA.
            if (!VisualStyle.ModoNocturno)
                _TitleBT.ButtonColor = VisualStyle.Colors.White;
            else
            {
                _TitleBT.ButtonColor = VisualStyle.Colors.Black;
                _TitleBT.ForeColor = System.Drawing.Color.White;
            }
            _TitleBT.ButtonColorDisabled = VisualStyle.Colors.Red;
            _TitleBT.Enabled = _TitleEnabled;

            // LALM Modo Nocturno 210201
            if (!VisualStyle.ModoNocturno)
            {
                _SplitUC.LeftJackOn = _StateManager.Jacks.LeftJack;
                _SplitUC.RightJackOn = _StateManager.Jacks.RightJack;
                _SplitUC.Mode = _StateManager.Split.Mode;
                _SplitUC.Enabled = _SplitEnabled;
            }
            else
            {
                _SplitUC.JackOff = CambiarTamanoImagen(_StateManager.Title.JackOffMn, new System.Drawing.Size(_StateManager.Title.WidthJaks, _StateManager.Title.HeightLogo));
                _SplitUC.JackOn = CambiarTamanoImagen(_StateManager.Title.JackOnMn, new System.Drawing.Size(_StateManager.Title.WidthJaks, _StateManager.Title.HeightLogo));


                _SplitUC.LeftJackOn = _StateManager.Jacks.LeftJack;
                _SplitUC.RightJackOn = _StateManager.Jacks.RightJack;
                _SplitUC.Mode = _StateManager.Split.Mode;
                _SplitUC.Enabled = _SplitEnabled;

                // LALM 210217 Inicializacion de las imagenes de split.
                _SplitUC.replacebitmaps(_StateManager.Title.SplitOffMn,
                _StateManager.Title.SplitLcTfMn,
                _StateManager.Title.SplitRdLcMn);


            }

            if (global::HMI.Presentation.Sirtap.Properties.Settings.Default.JackUse != HMI.Presentation.Sirtap.Constants.JackUse.Both)
            {
                this._SplitUC.Size = new System.Drawing.Size(60, 79);
                this._InfoBT.Location = new System.Drawing.Point(235, 17);
                this._MsgLB.Location = new System.Drawing.Point(295, 3);
                this._MsgLB.Size = new System.Drawing.Size(320, 66);
                this._MsgLB.Font = new Font("Microsoft Sans Serif", 11F, FontStyle.Bold, GraphicsUnit.Point, ((byte)(0)));
            }
            // LALM: 210203
            if (VisualStyle.ModoNocturno)
            {
                int r = 255 - VisualStyle.TextoHeaderColor.R;
                int g = 255 - VisualStyle.TextoHeaderColor.G;
                int b = 255 - VisualStyle.TextoHeaderColor.B;
                _MsgLB.BackColor = Color.FromArgb(255, r, g, b);
                _MsgLB.ForeColor = VisualStyle.TextoHeaderColor;
            }

            _InfoBT.Enabled = _InfoEnabled;

            //_CmdManager.SetBrightnessLevel(100); 
            // LALM Modo Nocturno 210201
            if (!VisualStyle.ModoNocturno)
            {
                _BrightnessUDB.Level = _StateManager.Brightness.Level;
                _BrightnessUDB.Visible = _BrightnessEnabled;
            }
            else
            {
                // CambiarTamanoImagen, en realidad es cambiar imagen, solo se deberia hacer una vez
                // para no machacar la memoria
                //LALM 210217 No hago CambioTamanoImagen. Cargo imagen directamente.
                this._BrightnessUDB.DownImage = _StateManager.Title.BrilloDownImagemenos;
                this._BrightnessUDB.UpImage = _StateManager.Title.BrilloDownImagemas;
                // this._BrightnessUDB.DownImage = CambiarTamanoImagen(_StateManager.Title.BrilloDownImagemenos, new System.Drawing.Size(_StateManager.Title.WidthJaks, _StateManager.Title.HeightLogo));
                // this._BrightnessUDB.UpImage = CambiarTamanoImagen(_StateManager.Title.BrilloDownImagemas, new System.Drawing.Size(_StateManager.Title.WidthJaks, _StateManager.Title.HeightLogo));
                _BrightnessUDB.Level = _StateManager.Brightness.Level;
                _BrightnessUDB.Visible = _BrightnessEnabled;

                // LALM 210202 colores en la ProgressBar
                _BrightnessUDB.setColor(VisualStyle.cccmnProgressBar);
            }


            // LALM Modo Nocturno 210201
            if (!VisualStyle.ModoNocturno)
            {
                _BuzzerUDB.Level = _StateManager.Buzzer.Level;
                _BuzzerUDB.Enabled = _BuzzerEnabled;
                _AltavozAlarmasUDB.Level = _StateManager.AltavozAlarmas.Level;
                _AltavozAlarmasUDB.Enabled = _AAlarmaEnabled;
            }
            else
            {
                //LALM 210217 No hago CambioTamanoImagen. Cargo imagen directamente.
                this._BuzzerUDB.DownImage = _StateManager.Title.Buzzermenos;
                this._BuzzerUDB.UpImage = _StateManager.Title.Buzzermas;
                // this._BuzzerUDB.DownImage = CambiarTamanoImagen(_StateManager.Title.Buzzermenos, new System.Drawing.Size(_StateManager.Title.WidthJaks, _StateManager.Title.HeightLogo));
                // this._BuzzerUDB.UpImage = CambiarTamanoImagen(_StateManager.Title.Buzzermas, new System.Drawing.Size(_StateManager.Title.WidthJaks, _StateManager.Title.HeightLogo));
                _BuzzerUDB.Level = _StateManager.Buzzer.Level;
                _BuzzerUDB.Enabled = _BuzzerEnabled;
                _AltavozAlarmasUDB.Level = _StateManager.AltavozAlarmas.Level;
                _AltavozAlarmasUDB.Enabled = _AAlarmaEnabled;
                _BuzzerUDB.setColor(VisualStyle.cccmnProgressBar);
            }

            _InfoBT.Text = _Info;   // Miguel
            //LALM 210203
            if (VisualStyle.ModoNocturno)
            {
                this._InfoBT.ForeColor = Color.White;
                this._InfoBT.BackColor = Color.Black;
                this._MsgLB.ForeColor = VisualStyle.Colors.White;
                this._MsgLB.BackColor = VisualStyle.Colors.Black;

            }
        }

        [EventSubscription(EventTopicNames.ProxyPresent, ThreadOption.Publisher)]
        public void OnProxyPresent(object sender, EventArgs e)
        {
            if (Settings.Default.EmergencyModeWarn)
                BackColor = _StateManager.Scv.ProxyState ? VisualStyle.Colors.HeaderBlue : BackColor = System.Drawing.Color.Khaki;
            // LALM: 210202
            if (VisualStyle.ModoNocturno)
                BackColor = _StateManager.Scv.ProxyState ? VisualStyle.HeaderBlue : VisualStyle.HeaderKhaki;
        }

        [EventSubscription(EventTopicNames.TftLoginChanged, ThreadOption.Publisher)]
        [EventSubscription(EventTopicNames.TftEnabledChanged, ThreadOption.Publisher)]
        [EventSubscription(EventTopicNames.EngineStateChanged, ThreadOption.Publisher)]
        public void OnTftEnabledChanged(object sender, EventArgs e)
        {
            _TitleBT.Enabled = _TitleEnabled;
            _SplitUC.Enabled = _SplitEnabled;
            _InfoBT.Enabled = _InfoEnabled;
            _BrightnessUDB.Visible = _BrightnessEnabled;
            _BuzzerUDB.Enabled = _BuzzerEnabled;
            _AltavozAlarmasUDB.Enabled = _AAlarmaEnabled;
            _TitleBT.DrawX = !_StateManager.Engine.Operative;
            if (sender.GetType() == typeof(Engine))
            {
                MensajePresenciaAltavoces(typeof(RdSpeaker));
                MensajePresenciaAltavoces(typeof(LcSpeaker));
            }
            if (_StateManager.Tft.Login)
            {

                MisionText.Text = _StateManager.TftMisionInstance.Mision;
                _TitleBT.Text = MisionText.Text;
            }
            else
            {
                MisionText.Text = "-----";
                _TitleBT.Text = _StateManager.Title.Id;
                _TitleBT.Text = GenIdAgrupacion(_StateManager.Title.Id);

            }
        }

        [EventSubscription(EventTopicNames.JacksChanged, ThreadOption.Publisher)]
        public void OnJacksChanged(object sender, EventArgs e)
        {
            Jacks jacks = _StateManager.Jacks;

            _SplitUC.LeftJackOn = jacks.LeftJack;
            _SplitUC.RightJackOn = jacks.RightJack;

            if (!string.IsNullOrEmpty(jacks.PreviusStateDescription))
            {
                _MsgLB.Text = _MsgLB.Text.Replace(jacks.PreviusStateDescription, "");
            }

            if (_StateManager.Engine.Operative)
            {
                if (!string.IsNullOrEmpty(jacks.StateDescription))
                {
                    _MsgLB.Text += jacks.StateDescription;
                }
            }
        }

        //LALM 210618
        // Funcion Que limita el numero maximo de caracteres de una agrupacion a 16.
        private String GenIdAgrupacion(String agrupacion)
        {
            int longmax = 16;
            String IdAgrupacion = agrupacion;
            int len = agrupacion.Length;
            if (len > longmax)
            {
                int mitad = longmax / 2;
                IdAgrupacion = agrupacion.Substring(0, mitad - 1) + ".." + agrupacion.Substring(len - (mitad - 1), mitad - 1);
            }
            return IdAgrupacion;
        }

        [EventSubscription(EventTopicNames.TitleIdChanged, ThreadOption.Publisher)]
        public void OnTitleIdChanged(object sender, EventArgs e)
        {
            _TitleBT.Text = _StateManager.Title.Id;
            //LALM 210617 define el tama�o maximo del identificador de puesto y
            // permite recortar la linea y ademas que se quede en la parte de abajo de su zona de pintado.
            //LALM 210618 Se unifica el criterio de generacion nombre agrupacion
            _TitleBT.Text = GenIdAgrupacion(_StateManager.Title.Id);
        }

        [EventSubscription(EventTopicNames.SplitShowModeSelectionUI, ThreadOption.Publisher)]
        public void OnSplitShowModeSelection(object sender, EventArgs e)
        {
            _SplitUC.ShowModeSelection();
        }

        bool cambiohmiexeconfig(string clave, string valor)
        {
            return false;

        }

        //LALM 210219-2
        [EventSubscription(EventTopicNames.ReinicioAppUI, ThreadOption.Publisher)]
        public void OnReinicioApp(object sender, EventArgs e)

        {
            //LALM 210219 reiniciar
            int forma = 3;
            if (!VisualStyle.ModoNocturno)
            {
                // El cambio se hara en la confirmacion
                if (!HMIConfigHelper.CambioModoNocturno("hmi.exe.config", "ModoNocturno", "False", "True"))
                {
                    if (!cambiohmiexeconfig("ModoNocturno", "False"))
                    {

                        string str = string.Format("El cambio de modo no se puede efectuar en este momento.");
                        NotifMsg msg = new NotifMsg("Error Cambio de Modo", "Estado", str, 0, MessageType.Information, MessageButtons.Ok);
                        _StateManager.ShowUIMessage(msg);


                        return;
                    }
                }
            }
            else
            {
                if (!HMIConfigHelper.CambioModoNocturno("hmi.exe.config", "ModoNocturno", "True", "False"))
                {
                    if (!cambiohmiexeconfig("ModoNocturno", "False"))
                    {
                        string str = string.Format("El cambio de modo no se puede efectuar en este momento.");
                        NotifMsg msg = new NotifMsg("Error Cambio de Modo", "Estado", str, 0, MessageType.Information, MessageButtons.Ok);
                        _StateManager.ShowUIMessage(msg);
                        return;
                    }
                }
            }

            if (forma == 0)
                Process.Start("Launcher.exe", "HMI.exe");
            if (forma == 1)
            {
                ProcessStartInfo startInfo = new ProcessStartInfo("cmd.exe");
                startInfo.WindowStyle = ProcessWindowStyle.Hidden;
                startInfo.Arguments = "/K echo taskkill /F /IM HMI.exe > reinicio.bat";
                System.Diagnostics.Process.Start(startInfo);

                startInfo = new ProcessStartInfo("cmd.exe");
                startInfo.WindowStyle = ProcessWindowStyle.Hidden;
                startInfo.Arguments = "/K echo ping 8.8.8.8 >> reinicio.bat";
                System.Diagnostics.Process.Start(startInfo);

                startInfo = new ProcessStartInfo("cmd.exe");
                startInfo.WindowStyle = ProcessWindowStyle.Hidden;
                startInfo.Arguments = "/K echo HMI.exe >> reinicio.bat";
                System.Diagnostics.Process.Start(startInfo);

                startInfo = new ProcessStartInfo("reinicio.bat");
                startInfo.WindowStyle = ProcessWindowStyle.Hidden;
                startInfo.Arguments = "";
                System.Diagnostics.Process.Start(startInfo);
                Application.Exit();

            }
            if (forma == 2)
            {
                ProcessStartInfo startInfo = new ProcessStartInfo("cmd.exe");
                startInfo = new ProcessStartInfo("resources/reinicio.bat");
                startInfo.WindowStyle = ProcessWindowStyle.Hidden;
                startInfo.Arguments = "";
                bool probando = false;
                if (probando)
                {
                    string str = string.Format("Cambio a Modo {0}.\n estoy en depuracion", "Nocturno : Diurno");
                    NotifMsg msg = new NotifMsg("No lo hago", "Estado", str, 0, MessageType.Information, MessageButtons.Ok);
                    _StateManager.ShowUIMessage(msg);
                }
                else
                {
                    System.Diagnostics.Process.Start(startInfo);
                    Application.Exit();
                }
            }
            if (forma == 3)
            {
                ProcessStartInfo startInfo = new ProcessStartInfo("PictureBox.exe");
                startInfo.WindowStyle = ProcessWindowStyle.Hidden;
                startInfo.Arguments = "";
                bool probando = false;
                if (probando)
                {
                    string str = string.Format("Cambio a Modo {0}.\n estoy en depuracion", "Nocturno : Diurno");
                    NotifMsg msg = new NotifMsg("No lo hago", "Estado", str, 0, MessageType.Information, MessageButtons.Ok);
                    _StateManager.ShowUIMessage(msg);
                }
                else
                {
                    System.Diagnostics.Process.Start(startInfo);
                    Application.Exit();
                }
            }


        }

        [EventSubscription(EventTopicNames.SplitModeChanged, ThreadOption.Publisher)]
        public void OnSplitModeChanged(object sender, EventArgs e)
        {
            _SplitUC.Mode = _StateManager.Split.Mode;
        }

        [EventSubscription(EventTopicNames.BrightnessLevelChanged, ThreadOption.Publisher)]
        public void OnBrightnessLevelChanged(object sender, EventArgs e)
        {
            _BrightnessUDB.Level = _StateManager.Brightness.Level;
        }

        [EventSubscription(EventTopicNames.BuzzerStateChanged, ThreadOption.Publisher)]
        public void OnBuzzerStateChanged(object sender, EventArgs e)
        {
            _BuzzerUDB.DrawX = !_StateManager.Buzzer.Enabled;
        }

        [EventSubscription(EventTopicNames.AlarmaStateChanged, ThreadOption.Publisher)]
        public void OnAlarmaStateChanged(object sender, EventArgs e)
        {
            _AltavozAlarmasUDB.DrawX = !_StateManager.AltavozAlarmas.Enabled;
        }

        [EventSubscription(EventTopicNames.BuzzerLevelChanged, ThreadOption.Publisher)]
        public void OnBuzzerLevelChanged(object sender, EventArgs e)
        {
            _BuzzerUDB.Level = _StateManager.Buzzer.Level;
        }
        [EventSubscription(EventTopicNames.AlarmaLevelChanged, ThreadOption.Publisher)]
        public void OnAlarmLevelChanged(object sender, EventArgs e)
        {
            _AltavozAlarmasUDB.Level = _StateManager.AltavozAlarmas.Level;
        }

        [EventSubscription(EventTopicNames.TlfChanged, ThreadOption.Publisher)]
        public void OnTlfChanged(object sender, RangeMsg e)
        {
            for (int i = e.From, to = e.From + e.Count; i < to; i++)
            {
                TlfDst dst = _StateManager.Tlf[i];

                if (!string.IsNullOrEmpty(dst.PreviusStateDescription))
                {
                    if (_MsgLB.Text.Contains(dst.PreviusStateDescription))
                    {
                        _MsgLB.Text = _MsgLB.Text.Replace(dst.PreviusStateDescription, "");
                    }
                    else
                    {
                        _ConfUnused.Remove(dst.PreviusStateDescription);
                    }
                }
                if (!string.IsNullOrEmpty(dst.StateDescription))
                {
                    if (_MsgLB.Text.Contains(dst.StateDescription))
                    {
                        _ConfUnused.Add(dst.StateDescription);
                    }
                    else
                    {
                        _MsgLB.Text += dst.StateDescription;
                    }
                }
            }
        }

        [EventSubscription(EventTopicNames.TlfForwardChanged, ThreadOption.Publisher)]
        [EventSubscription(EventTopicNames.TlfPickUpChanged, ThreadOption.Publisher)]
        [EventSubscription(EventTopicNames.TlfIntrudeToChanged, ThreadOption.Publisher)]
        [EventSubscription(EventTopicNames.TlfInterruptedByChanged, ThreadOption.Publisher)]
        [EventSubscription(EventTopicNames.TlfIntrudedByChanged, ThreadOption.Publisher)]
        [EventSubscription(EventTopicNames.TlfListenChanged, ThreadOption.Publisher)]
        [EventSubscription(EventTopicNames.TlfListenByChanged, ThreadOption.Publisher)]
        public void OnEntityDescriptionChanged(object sender, EventArgs e)
        {
            Description entity = (Description)sender;

            if (!string.IsNullOrEmpty(entity.PreviusStateDescription))
            {
                _MsgLB.Text = _MsgLB.Text.Replace(entity.PreviusStateDescription, "");
            }
            if (!string.IsNullOrEmpty(entity.StateDescription))
            {
                if (!_MsgLB.Text.Contains(entity.StateDescription))
                    _MsgLB.Text += entity.StateDescription;
            }
        }

        [EventSubscription(EventTopicNames.TlfListenChanged, ThreadOption.Publisher)]
        public void OnTlfListenChanged(object sender, EventArgs e)
        {
            _InfoBT.Enabled = _InfoEnabled;
        }

        [EventSubscription(EventTopicNames.TlfListenByChanged, ThreadOption.Publisher)]
        public void OnTlfListenByChanged(object sender, EventArgs e)
        {
            ListenBy listenBy = (ListenBy)sender;
            _MsgLB.BackColor = listenBy.IsListen ? VisualStyle.Colors.Orange : VisualStyle.Colors.White;
        }

        [EventSubscription(EventTopicNames.TlfConfListChanged, ThreadOption.Publisher)]
        public void OnTlfConfListChanged(object sender, EventArgs e)
        {
            ConfList confList = _StateManager.Tlf.ConfList;

            if (!string.IsNullOrEmpty(confList.PreviusStateDescription))
            {
                if (!_ConfUnused.Remove(confList.PreviusStateDescription))
                {
                    _MsgLB.Text = _MsgLB.Text.Replace(confList.PreviusStateDescription, "");
                }
            }
            if (!string.IsNullOrEmpty(confList.StateDescription))
            {
                if (_MsgLB.Text.Contains(confList.StateDescription))
                {
                    _ConfUnused.Add(confList.StateDescription);
                }
                else
                {
                    _MsgLB.Text += confList.StateDescription;
                }
            }
        }

        /// <summary>
        /// Este evento llega cuando hay un cambio en la presencia de un altavoz.
        /// Se utiliza para poner un mensaje 
        /// </summary>
        /// <param name="sender">no se usa</param>
        /// <param name="e">no se usa</param>
        [EventSubscription(EventTopicNames.SpeakerChanged, ThreadOption.Publisher)]
        public void OnSpeakerChanged(object sender, EventArgs e)
        {
            MensajePresenciaAltavoces(sender.GetType());
        }

        private void MensajePresenciaAltavoces(Type tipo)
        {
            if (tipo == typeof(RdSpeaker))
            {
                if (_StateManager.RdSpeaker.Presencia)
                {
                    _MsgLB.Text = _MsgLB.Text.Replace(Resources.SpeakerMissing + Resources.Radio + Environment.NewLine, "");
                }
                else if (_StateManager.Engine.Operative)
                {
                    _MsgLB.Text += Resources.SpeakerMissing + Resources.Radio + Environment.NewLine;
                }
            }
            else if (tipo == typeof(LcSpeaker))
            {
                if (_StateManager.LcSpeaker.Presencia)
                {
                    _MsgLB.Text = _MsgLB.Text.Replace(Resources.SpeakerMissing + Resources.TelephonyLC + Environment.NewLine, "");
                }
                else if (_StateManager.Engine.Operative)
                {
                    _MsgLB.Text += Resources.SpeakerMissing + Resources.TelephonyLC + Environment.NewLine;
                }
            }
            else if (tipo == typeof(HfSpeaker))
            {
                if (_StateManager.HfSpeaker.Presencia)
                {
                    _MsgLB.Text = _MsgLB.Text.Replace(Resources.SpeakerMissing + Resources.RadioAux + Environment.NewLine, "");
                }
                else if ((_StateManager.Engine.Operative) && (_StateManager.Radio.DoubleRadioSpeaker))
                {
                    _MsgLB.Text += Resources.SpeakerMissing + Resources.RadioAux + Environment.NewLine;
                }
            }
        }


        private void _TitleBT_Click(object sender, EventArgs e)
        {
            try
            {
                _CmdManager.DisableTft();
            }
            catch (Exception ex)
            {
                _Logger.Error("ERROR deshabilitando el TFT", ex);
            }
        }

        private void _SplitUC_SplitSelectionClick(object sender, EventArgs e)
        {
            try
            {
                _CmdManager.ShowSplitModeSelection();
            }
            catch (Exception ex)
            {
                _Logger.Error("ERROR intentando mostrar los posibles modos de Split", ex);
            }
        }

        private void _SplitUC_SplitModeChanging(object sender, SplitMode mode)
        {
            try
            {
                _CmdManager.SetSplitMode(mode);
            }
            catch (Exception ex)
            {
                _Logger.Error("ERROR intentando cambiar modo de Split a " + mode, ex);
            }
        }

        private void _InfoBT_Click(object sender, EventArgs e)
        {
            try
            {
                _CmdManager.ShowInfo();
            }
            catch (Exception ex)
            {
                _Logger.Error("ERROR mostrando dependencias", ex);
            }
        }

        private void _BrightnessUDB_LevelDown(object sender, EventArgs e)
        {
            int level = _BrightnessUDB.Level - 1;

            try
            {
                _CmdManager.SetBrightnessLevel(level);
            }
            catch (Exception ex)
            {
                _Logger.Error("ERROR bajando el nivel de brillo a " + level, ex);
            }
        }

        private void _BrightnessUDB_LevelUp(object sender, EventArgs e)
        {
            int level = _BrightnessUDB.Level + 1;

            try
            {
                _CmdManager.SetBrightnessLevel(level);
            }
            catch (Exception ex)
            {
                _Logger.Error("ERROR aumentando el nivel de brillo a " + level, ex);
            }
        }

        private void _BuzzerUDB_LevelDown(object sender, EventArgs e)
        {
            int level = _BuzzerUDB.Level - 1;

            try
            {
                _CmdManager.SetBuzzerLevel(level);
            }
            catch (Exception ex)
            {
                _Logger.Error("ERROR bajando el nivel de ring a " + level, ex);
            }
        }

        private void _BuzzerUDB_LevelUp(object sender, EventArgs e)
        {
            int level = _BuzzerUDB.Level + 1;

            try
            {
                _CmdManager.SetBuzzerLevel(level);
            }
            catch (Exception ex)
            {
                _Logger.Error("ERROR aumentando el nivel de ring a " + level, ex);
            }
        }

        private void _BuzzerUDB_LongClick(object sender, EventArgs e)
        {
            bool enabled = !_StateManager.Buzzer.Enabled;

            try
            {
                _CmdManager.SetBuzzerState(enabled);
            }
            catch (Exception ex)
            {
                string msg = string.Format("ERROR {0} el ring", enabled ? "habilitando" : "deshabilitando");
                _Logger.Error(msg, ex);
            }
        }
        public IModelCmdManagerService CmdManager
        {
            get
            {
                return _CmdManager;
            }
        }

        private void _AltavozAlarmasUDB_LevelDown(object sender, EventArgs e)
        {
            int level = _AltavozAlarmasUDB.Level - 1;

            try
            {
                _CmdManager.SetAlarmLevel(level);
            }
            catch (Exception ex)
            {
                _Logger.Error("ERROR bajando el nivel de alarma a " + level, ex);
            }

        }

        private void _AltavozAlarmasUDB_LevelUp(object sender, EventArgs e)
        {
            int level = _AltavozAlarmasUDB.Level + 1;

            try
            {
                _CmdManager.SetAlarmLevel(level);
            }
            catch (Exception ex)
            {
                _Logger.Error("ERROR subiendo el nivel de alarma a " + level, ex);
            }

        }

        private void _AltavozAlarmasUDB_LongClick(object sender, EventArgs e)
        {
            bool enabled = !_StateManager.AltavozAlarmas.Enabled;

            try
            {
                _CmdManager.SetAlarmState(enabled);
            }
            catch (Exception ex)
            {
                string msg = string.Format("ERROR {0} el altaavoz alarma", enabled ? "habilitando" : "deshabilitando");
                _Logger.Error(msg, ex);
            }

        }
    }
}

