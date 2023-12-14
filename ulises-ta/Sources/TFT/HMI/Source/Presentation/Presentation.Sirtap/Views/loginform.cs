using HMI.Infrastructure.Interface;
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

        }
        public loginform()
        {
            InitializeComponent();
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
                _LogClv.Text = "Usuario";
            else
                _LogClv.Text = "Clave";
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

        private async void _OK_Click(object sender, EventArgs e)
        {
            Utilities.IValidadorCredenciales ValidadorCredenciales = new Utilities.ValidadorCredenciales();
            var mision=ValidadorCredenciales.SimuladorValidarCredenciales(txtUsuario.Text, txtContrasena.Text);
            if (mision.Length>0)
            {
                _StateManager.Tft.Enabled = true;
                _StateManager.Tft.Login = true;
                this.DialogResult = DialogResult.OK;
                this.Close();
            }
            else
            {
                _StateManager.Tft.Enabled = true;
                _StateManager.Tft.Login = true;
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
            string httpResult = await operationService.PerformOperation("HttpServiceSimulated", httpServiceData);
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
    }
}
