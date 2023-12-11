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

    public partial class login : UserControl
    {
        private bool _user_focus = true;
        
        public login()
        {
            InitializeComponent();
        }

        private void _1BT_Click(object sender, EventArgs e)
        {
            if (_user_focus)
            {
                txtUsuario.Text += "1";
            }
            else
            {
                txtContrasena.Text += "1";
            }
        }          
    }
}
