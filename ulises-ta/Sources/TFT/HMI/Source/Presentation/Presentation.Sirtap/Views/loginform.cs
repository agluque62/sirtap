using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace HMI.Presentation.Sirtap.Views
{
    public partial class loginform : Form
    {
        private bool sellogin = true;
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
            sellogin = !sellogin;
        }

        private void _AstBT_Click(object sender, EventArgs e)
        {
            if (sellogin)
                _AstBT.Text = "Usuario";
            else
                _AstBT.Text = "Clave";
            _AstBT.Update();

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
    }
}
