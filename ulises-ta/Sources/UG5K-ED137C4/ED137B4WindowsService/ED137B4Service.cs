using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;

using NLog;

namespace ED137B4WindowsService
{
    public partial class ED137B4Service : ServiceBase
    {
        /// <summary>
        /// 
        /// </summary>
        public ED137B4Service()
        {
            InitializeComponent();
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="args"></param>
        protected override void OnStart(string[] args)
        {
            _ServiceMainTh = new Thread(MainTask);
            _ServiceMainTh.IsBackground = true;
            _ServiceMainTh.Start();

            base.OnStart(args);
        }

        /// <summary>
        /// 
        /// </summary>
        protected override void OnStop()
        {
            _EndEvent.Set();
            if (_ServiceMainTh != null)
            {
                _ServiceMainTh.Join();
            }

            base.OnStop();
        }

        #region Atributos

        Process _Process=null;
        Thread _ServiceMainTh = null;
        ManualResetEvent _EndEvent = new ManualResetEvent(false);
        static Logger _Logger = LogManager.GetCurrentClassLogger();

        #endregion

        #region Procedimientos Locales

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void OnUnhandledException(object sender, UnhandledExceptionEventArgs e)
        {
            try
            {
                if (e.ExceptionObject is Exception)
                {
                    _Logger.Fatal((Exception)e.ExceptionObject, "ED137B4Service: " + "Excepcion no manejada");
                }
            }
            catch (Exception) { }
        }

        /// <summary>
        /// 
        /// </summary>
        private void StartExternalProcess()
        {
            try
            {
                _Logger.Info("ED137B4Service: " + "Arrancando UG5KEd137b4Service.exe");
                _Process = new Process();

                _Process.StartInfo.FileName = "UG5KEd137b4Service.exe";
                _Process.StartInfo.Arguments = "";
                _Process.StartInfo.CreateNoWindow = false;
                _Process.StartInfo.ErrorDialog = false;
                _Process.StartInfo.UseShellExecute = false;

                _Process.Start();
                Thread.Sleep(500);
            }
            catch (Exception x)
            {
                _Process.Close();
                _Process = null;
                _Logger.Error(x, "ED137B4Service: " + "Error al arrancar UG5KEd137b4Service.exe");
            }
        }

        /// <summary>
        /// 
        /// </summary>
        void MainTask()
        {
            AppDomain.CurrentDomain.UnhandledException += OnUnhandledException;
            Environment.CurrentDirectory = Path.GetDirectoryName(System.Reflection.Assembly.GetEntryAssembly().Location);

            _Logger.Info("ED137B4Service: " + "Nodo iniciado");

            StartExternalProcess();

            while (!_EndEvent.WaitOne(2000, false))
            {
                if ((_Process != null) && _Process.HasExited)
                {
                    _Process.Close();
                    _Process = null;
                }

                if (_Process == null)
                {
                    StartExternalProcess();
                }
            }

            if (_Process != null)
            {
                if (!_Process.HasExited)
                {
                    _Process.Kill();
                }

                _Process.Close();
                _Process = null;
            }

            _Logger.Info("ED137B4Service: " + "Nodo detenido");
        }

        #endregion

#if DEBUG
        public void onDebug() 
        {
            this.OnStart(null);
        }
#endif
    }
}
