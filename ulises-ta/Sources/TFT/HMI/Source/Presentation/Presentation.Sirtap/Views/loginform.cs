using HMI.Infrastructure.Interface;
using HMI.Model.Module.Messages;
using HMI.Model.Module.Services;
using Microsoft.Practices.CompositeUI;
using NLog;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.Remoting.Messaging;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Utilities;


namespace HMI.Presentation.Sirtap.Views
{
    public partial class loginform : Form
    {
        private bool sellogin = true;
        private IModelCmdManagerService _CmdManager = null;
        private StateManagerService _StateManager = null;
        private static Logger _Logger = LogManager.GetCurrentClassLogger();
        public void setuploginform([ServiceDependency] IModelCmdManagerService cmdManager, [ServiceDependency] StateManagerService stateManager)
        {
            _CmdManager = cmdManager;
            _StateManager = stateManager;
            //InitializeComponent();
            if (stateManager.Tft.ModoNocturno)
                this.hmiButtonModo.Text = "Modo Diurno";
            else
                this.hmiButtonModo.Text = "Modo Nocturno";
            txtContrasena.Text = "";
            MostrarModo(null);

        }
        public loginform()
        {
            InitializeComponent();
            ControlBox = false;
        }

        private void pulsa(string tecla)
        {
            if (sellogin)
            {
                txtUsuario.Text += tecla;
                txtUsuario.SelectionStart = txtUsuario.Text.Length;
                txtUsuario.SelectionLength = 0;
            }
            else
            {
                txtContrasena.Text += tecla;
                txtContrasena.SelectionStart = txtContrasena.Text.Length;
                txtContrasena.SelectionLength = 0;

            }
        }
        private void _0BT_Click(object sender, EventArgs e)
        {
            pulsa("0");
        }
        private void _1BT_Click(object sender, EventArgs e)
        {
            pulsa("1");
        }
        private void _2BT_Click(object sender, EventArgs e)
        {
            pulsa("2");
        }
        private void _3BT_Click(object sender, EventArgs e)
        {
            pulsa("3");
        }
        private void _4BT_Click(object sender, EventArgs e)
        {
            pulsa("4");
        }
        private void _5BT_Click(object sender, EventArgs e)
        {
            pulsa("5");
        }
        private void _6BT_Click(object sender, EventArgs e)
        {
            pulsa("6");
        }
        private void _7BT_Click(object sender, EventArgs e)
        {
            pulsa("7");
        }
        private void _8BT_Click(object sender, EventArgs e)
        {
            pulsa("8");
        }
        private void _9BT_Click(object sender, EventArgs e)
        {
            pulsa("9");
        }


        private void _AlmBT_Click(object sender, EventArgs e)
        {

        }

        private void _LogClv_Click(object sender, EventArgs e)
        {
            if (sellogin)
            {
                _LogClv.Text = "Usuario";
                txtContrasena.Enabled = true;
                txtUsuario.Enabled = false;
            }
            else
            {
                _LogClv.Text = "Clave";
                txtContrasena.Enabled = false;
                txtUsuario.Enabled = true;
            }
            _LogClv.Update();

            sellogin = !sellogin;
            if (sellogin)
            {
                txtUsuario.Focus();
                txtUsuario.SelectionStart = txtUsuario.Text.Length;
                txtUsuario.SelectionLength = 0;
            }
            else
            {
                txtContrasena.Focus();
                txtContrasena.SelectionStart = txtContrasena.Text.Length;
                txtContrasena.SelectionLength = 0;
            }
        }

        private void _ClearBT_Click(object sender, EventArgs e)
        {
            if (sellogin)
            {
                txtUsuario.Text = (txtUsuario.TextLength > 0) ? txtUsuario.Text.Substring(0, txtUsuario.TextLength - 1) : txtUsuario.Text;
                txtUsuario.SelectionStart = txtUsuario.Text.Length;
                txtUsuario.SelectionLength = 0;
            }
            else
            {
                txtContrasena.Text = (txtContrasena.TextLength > 0) ? txtContrasena.Text.Substring(0, txtContrasena.TextLength - 1) : txtContrasena.Text;
                txtContrasena.SelectionStart = txtContrasena.Text.Length;
                txtContrasena.SelectionLength = 0;
            }

        }

        private void loginform_Load(object sender, EventArgs e)
        {

        }
        private void MostrarVentanaModal(string mensaje)
        {
            using (var errorForm = new Form())
            {
                // Configuración para ocultar la barra de título y el ícono
                errorForm.FormBorderStyle = FormBorderStyle.FixedToolWindow;
                errorForm.ControlBox = false;
                // Establecer el título de la ventana
                errorForm.Text = "Error";

                var label = new Label();
                label.Text = mensaje;
                label.Dock = DockStyle.Fill;
                label.TextAlign = ContentAlignment.MiddleCenter;
                
                // Ajustar la apariencia del texto
                label.Font = new System.Drawing.Font("Arial", 14, FontStyle.Bold);
                
                errorForm.Controls.Add(label);

                // Cerrar automáticamente después de 3 segundos
                var timer = new Timer();
                timer.Interval = 3000; // 3000 milisegundos = 3 segundos
                timer.Tick += (s, e) => errorForm.Close();
                timer.Start();

                errorForm.ShowDialog();
            }
        }


        void MostrarVentanaModal1(string mensaje)
        {
            NotifMsg msg = new NotifMsg("", "Intentelo de nuevo", mensaje, 0, MessageType.Information, MessageButtons.Ok);
            _StateManager.ShowUIMessage(msg);

        }


        private void _OK_Click(object sender, EventArgs e)
        {
            _CmdManager.TftSolicitaSesion(txtUsuario.Text, txtContrasena.Text);
            IValidadorCredenciales ValidadorCredenciales = new Utilities.ValidadorCredenciales();


            //var mision = ValidadorCredenciales.SimuladorValidarCredenciales(txtUsuario.Text, txtContrasena.Text);
            var mision = ValidadorCredenciales.Login(txtUsuario.Text, txtContrasena.Text);
            //_StateManager.TftMision.Mision = mision.Result; // quitar si no se usa
            _StateManager.TftMision.Mision = mision.Result;
            if (mision.Result.Length>0)
            {
                //_StateManager.Tft.Enabled = true;
                _StateManager.Tft.Login = true;
                _StateManager.Tft.LoginName = txtUsuario.Text;
                this.DialogResult = DialogResult.OK;
                this.Close();
            }
            else
            {
                //MostrarVentanaModal("Usuario o contraseña incorrectos. Por favor, inténtelo de nuevo.");
                MostrarVentanaModal1("Usuario o contraseña incorrectos. Por favor, inténtelo de nuevo.");
                _StateManager.Tft.Login = false;
                _StateManager.Tft.GenerarHistoricoLoginIncorrecto=txtUsuario.Text;
            }
            /*

            Utilities.IOperationService operationService = new Utilities.OperationService();
            // Define los datos para la operación HttpService
            var httpServiceData = new HttpServiceData
            {
                ApiUrl = "http://localhost:3000/login",
                PostData = $"{{\"username\": \"{ txtUsuario.Text}\", \"password\": \"{ txtContrasena.Text}\"}}"
            };

            // Ejecuta la operación HttpService
            string httpResult = await operationServic
            e.PerformOperation("HttpServiceSimulated", httpServiceData);
            Console.WriteLine("Resultado de la solicitud HTTP: " + httpResult);

            //MessageBox.Show(httpResult, "Título del mensaje", MessageBoxButtons.OK);
            if (httpResult == "OK")
            {
                _StateManager.Tft.Enabled = true;
                _StateManager.Tft.Login = true;
            }
            else
            {
                _StateManager.Tft.Enabled = false;
                _StateManager.Tft.Login = false;
            }
            */
        }

        private void hmiButtonModo_MouseUp(object sender, MouseEventArgs e)
        {
            _StateManager.Tft.ModoNocturno = !_StateManager.Tft.ModoNocturno;
            if (_StateManager.Tft.ModoNocturno)
                hmiButtonModo.Text = "Modo Diurno";
            else
                hmiButtonModo.Text = "Modo Nocturno";
            ChangeColors();
        }
        private void ChangeColors()
        {
            if (_StateManager.Tft.ModoNocturno)
                BackColor = Color.Gray;
            else
                BackColor = Color.White;
        }
        private void MostrarModo(object sender)
        {
            ChangeColors();
        }


    }
}
