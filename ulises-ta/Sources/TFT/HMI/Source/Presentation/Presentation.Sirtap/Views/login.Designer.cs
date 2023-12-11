
using System;
using System.Windows.Forms;

namespace HMI.Presentation.Sirtap.Views
{
    partial class login : UserControl
    {
        private TextBox txtUsuario;
        private TextBox txtContrasena;
        private Button btnIniciarSesion;
        private Label lblMensaje;

        public void login1()
        {
            InitializeComponent();
            //InitializeUI();
        }

        private void InitializeComponent()
        {
            System.Windows.Forms.TableLayoutPanel _KeypadTLP;
            System.Windows.Forms.TableLayoutPanel _DisplayTLP;
            this._1BT = new HMI.Model.Module.UI.HMIButton();
            this._2BT = new HMI.Model.Module.UI.HMIButton();
            this._3BT = new HMI.Model.Module.UI.HMIButton();
            this._4BT = new HMI.Model.Module.UI.HMIButton();
            this._5BT = new HMI.Model.Module.UI.HMIButton();
            this._6BT = new HMI.Model.Module.UI.HMIButton();
            this._7BT = new HMI.Model.Module.UI.HMIButton();
            this._8BT = new HMI.Model.Module.UI.HMIButton();
            this._9BT = new HMI.Model.Module.UI.HMIButton();
            this._AstBT = new HMI.Model.Module.UI.HMIButton();
            this._0BT = new HMI.Model.Module.UI.HMIButton();
            this._AlmBT = new HMI.Model.Module.UI.HMIButton();
            this._ClearBT = new HMI.Model.Module.UI.HMIButton();
            this._DisplayTB = new System.Windows.Forms.TextBox();
            this._PauseBt = new HMI.Model.Module.UI.HMIButton();
            this.txtUsuario = new System.Windows.Forms.TextBox();
            this.txtContrasena = new System.Windows.Forms.TextBox();
            this.btnIniciarSesion = new System.Windows.Forms.Button();
            this.lblMensaje = new System.Windows.Forms.Label();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            _KeypadTLP = new System.Windows.Forms.TableLayoutPanel();
            _DisplayTLP = new System.Windows.Forms.TableLayoutPanel();
            _KeypadTLP.SuspendLayout();
            _DisplayTLP.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // _KeypadTLP
            // 
            _KeypadTLP.BackColor = System.Drawing.Color.Silver;
            _KeypadTLP.ColumnCount = 3;
            _KeypadTLP.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
            _KeypadTLP.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
            _KeypadTLP.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
            _KeypadTLP.Controls.Add(this._1BT, 0, 1);
            _KeypadTLP.Controls.Add(this._2BT, 1, 1);
            _KeypadTLP.Controls.Add(this._3BT, 2, 1);
            _KeypadTLP.Controls.Add(this._4BT, 0, 2);
            _KeypadTLP.Controls.Add(this._5BT, 1, 2);
            _KeypadTLP.Controls.Add(this._6BT, 2, 2);
            _KeypadTLP.Controls.Add(this._7BT, 0, 3);
            _KeypadTLP.Controls.Add(this._8BT, 1, 3);
            _KeypadTLP.Controls.Add(this._9BT, 2, 3);
            _KeypadTLP.Controls.Add(this._AstBT, 0, 4);
            _KeypadTLP.Controls.Add(this._0BT, 1, 4);
            _KeypadTLP.Controls.Add(this._AlmBT, 2, 4);
            _KeypadTLP.Controls.Add(_DisplayTLP, 0, 0);
            _KeypadTLP.Dock = System.Windows.Forms.DockStyle.Fill;
            _KeypadTLP.Location = new System.Drawing.Point(5, 10);
            _KeypadTLP.Margin = new System.Windows.Forms.Padding(5, 10, 0, 0);
            _KeypadTLP.Name = "_KeypadTLP";
            _KeypadTLP.RowCount = 5;
            _KeypadTLP.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 10F));
            _KeypadTLP.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 10F));
            _KeypadTLP.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 10F));
            _KeypadTLP.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 10F));
            _KeypadTLP.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 10F));
            _KeypadTLP.Size = new System.Drawing.Size(410, 347);
            _KeypadTLP.TabIndex = 2;
            // 
            // _1BT
            // 
            this._1BT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._1BT.DrawX = false;
            this._1BT.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._1BT.IsButtonTlf = false;
            this._1BT.IsConferencePreprogramada = false;
            this._1BT.Location = new System.Drawing.Point(3, 72);
            this._1BT.Name = "_1BT";
            this._1BT.Permitted = true;
            this._1BT.Size = new System.Drawing.Size(130, 63);
            this._1BT.TabIndex = 1;
            this._1BT.Tag = "";
            this._1BT.Text = "1";
            this._1BT.Click += new System.EventHandler(this._1BT_Click);
            // 
            // _2BT
            // 
            this._2BT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._2BT.DrawX = false;
            this._2BT.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._2BT.IsButtonTlf = false;
            this._2BT.IsConferencePreprogramada = false;
            this._2BT.Location = new System.Drawing.Point(139, 72);
            this._2BT.Name = "_2BT";
            this._2BT.Permitted = true;
            this._2BT.Size = new System.Drawing.Size(130, 63);
            this._2BT.TabIndex = 2;
            this._2BT.Text = "2";
            // 
            // _3BT
            // 
            this._3BT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._3BT.DrawX = false;
            this._3BT.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._3BT.IsButtonTlf = false;
            this._3BT.IsConferencePreprogramada = false;
            this._3BT.Location = new System.Drawing.Point(275, 72);
            this._3BT.Name = "_3BT";
            this._3BT.Permitted = true;
            this._3BT.Size = new System.Drawing.Size(132, 63);
            this._3BT.TabIndex = 3;
            this._3BT.Text = "3";
            // 
            // _4BT
            // 
            this._4BT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._4BT.DrawX = false;
            this._4BT.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._4BT.IsButtonTlf = false;
            this._4BT.IsConferencePreprogramada = false;
            this._4BT.Location = new System.Drawing.Point(3, 141);
            this._4BT.Name = "_4BT";
            this._4BT.Permitted = true;
            this._4BT.Size = new System.Drawing.Size(130, 63);
            this._4BT.TabIndex = 4;
            this._4BT.Text = "4";
            // 
            // _5BT
            // 
            this._5BT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._5BT.DrawX = false;
            this._5BT.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._5BT.IsButtonTlf = false;
            this._5BT.IsConferencePreprogramada = false;
            this._5BT.Location = new System.Drawing.Point(139, 141);
            this._5BT.Name = "_5BT";
            this._5BT.Permitted = true;
            this._5BT.Size = new System.Drawing.Size(130, 63);
            this._5BT.TabIndex = 5;
            this._5BT.Text = "5";
            // 
            // _6BT
            // 
            this._6BT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._6BT.DrawX = false;
            this._6BT.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._6BT.IsButtonTlf = false;
            this._6BT.IsConferencePreprogramada = false;
            this._6BT.Location = new System.Drawing.Point(275, 141);
            this._6BT.Name = "_6BT";
            this._6BT.Permitted = true;
            this._6BT.Size = new System.Drawing.Size(132, 63);
            this._6BT.TabIndex = 6;
            this._6BT.Text = "6";
            // 
            // _7BT
            // 
            this._7BT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._7BT.DrawX = false;
            this._7BT.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._7BT.IsButtonTlf = false;
            this._7BT.IsConferencePreprogramada = false;
            this._7BT.Location = new System.Drawing.Point(3, 210);
            this._7BT.Name = "_7BT";
            this._7BT.Permitted = true;
            this._7BT.Size = new System.Drawing.Size(130, 63);
            this._7BT.TabIndex = 7;
            this._7BT.Text = "7";
            // 
            // _8BT
            // 
            this._8BT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._8BT.DrawX = false;
            this._8BT.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._8BT.IsButtonTlf = false;
            this._8BT.IsConferencePreprogramada = false;
            this._8BT.Location = new System.Drawing.Point(139, 210);
            this._8BT.Name = "_8BT";
            this._8BT.Permitted = true;
            this._8BT.Size = new System.Drawing.Size(130, 63);
            this._8BT.TabIndex = 8;
            this._8BT.Text = "8";
            // 
            // _9BT
            // 
            this._9BT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._9BT.DrawX = false;
            this._9BT.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._9BT.IsButtonTlf = false;
            this._9BT.IsConferencePreprogramada = false;
            this._9BT.Location = new System.Drawing.Point(275, 210);
            this._9BT.Name = "_9BT";
            this._9BT.Permitted = true;
            this._9BT.Size = new System.Drawing.Size(132, 63);
            this._9BT.TabIndex = 9;
            this._9BT.Text = "9";
            // 
            // _AstBT
            // 
            this._AstBT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._AstBT.DrawX = false;
            this._AstBT.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._AstBT.IsButtonTlf = false;
            this._AstBT.IsConferencePreprogramada = false;
            this._AstBT.Location = new System.Drawing.Point(3, 279);
            this._AstBT.Name = "_AstBT";
            this._AstBT.Permitted = true;
            this._AstBT.Size = new System.Drawing.Size(130, 65);
            this._AstBT.TabIndex = 10;
            this._AstBT.Text = "*";
            // 
            // _0BT
            // 
            this._0BT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._0BT.DrawX = false;
            this._0BT.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._0BT.IsButtonTlf = false;
            this._0BT.IsConferencePreprogramada = false;
            this._0BT.Location = new System.Drawing.Point(139, 279);
            this._0BT.Name = "_0BT";
            this._0BT.Permitted = true;
            this._0BT.Size = new System.Drawing.Size(130, 65);
            this._0BT.TabIndex = 11;
            this._0BT.Text = "0";
            // 
            // _AlmBT
            // 
            this._AlmBT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._AlmBT.DrawX = false;
            this._AlmBT.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._AlmBT.IsButtonTlf = false;
            this._AlmBT.IsConferencePreprogramada = false;
            this._AlmBT.Location = new System.Drawing.Point(275, 279);
            this._AlmBT.Name = "_AlmBT";
            this._AlmBT.Permitted = true;
            this._AlmBT.Size = new System.Drawing.Size(132, 65);
            this._AlmBT.TabIndex = 12;
            this._AlmBT.Text = "#";
            // 
            // _DisplayTLP
            // 
            _DisplayTLP.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            _DisplayTLP.ColumnCount = 2;
            _KeypadTLP.SetColumnSpan(_DisplayTLP, 3);
            _DisplayTLP.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 80F));
            _DisplayTLP.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 20F));
            _DisplayTLP.Controls.Add(this._ClearBT, 1, 0);
            _DisplayTLP.Controls.Add(this._DisplayTB, 0, 0);
            _DisplayTLP.Controls.Add(this._PauseBt, 1, 1);
            _DisplayTLP.Location = new System.Drawing.Point(0, 0);
            _DisplayTLP.Margin = new System.Windows.Forms.Padding(0);
            _DisplayTLP.Name = "_DisplayTLP";
            _DisplayTLP.RowCount = 2;
            _DisplayTLP.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            _DisplayTLP.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            _DisplayTLP.Size = new System.Drawing.Size(410, 69);
            _DisplayTLP.TabIndex = 13;
            // 
            // _ClearBT
            // 
            this._ClearBT.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this._ClearBT.DrawX = false;
            this._ClearBT.Font = new System.Drawing.Font("Arial Black", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._ClearBT.IsButtonTlf = false;
            this._ClearBT.IsConferencePreprogramada = false;
            this._ClearBT.Location = new System.Drawing.Point(328, 1);
            this._ClearBT.Margin = new System.Windows.Forms.Padding(0, 1, 3, 3);
            this._ClearBT.Name = "_ClearBT";
            this._ClearBT.Permitted = true;
            this._ClearBT.Size = new System.Drawing.Size(79, 30);
            this._ClearBT.TabIndex = 15;
            this._ClearBT.Text = "<--";
            // 
            // _DisplayTB
            // 
            this._DisplayTB.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this._DisplayTB.BackColor = System.Drawing.SystemColors.Window;
            this._DisplayTB.Font = new System.Drawing.Font("Trebuchet MS", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._DisplayTB.Location = new System.Drawing.Point(3, 1);
            this._DisplayTB.Margin = new System.Windows.Forms.Padding(3, 1, 3, 3);
            this._DisplayTB.Name = "_DisplayTB";
            this._DisplayTB.ReadOnly = true;
            this._DisplayTB.Size = new System.Drawing.Size(322, 26);
            this._DisplayTB.TabIndex = 14;
            this._DisplayTB.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // _PauseBt
            // 
            this._PauseBt.DrawX = false;
            this._PauseBt.Font = new System.Drawing.Font("Microsoft Sans Serif", 18.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._PauseBt.IsButtonTlf = false;
            this._PauseBt.IsConferencePreprogramada = false;
            this._PauseBt.Location = new System.Drawing.Point(331, 37);
            this._PauseBt.Name = "_PauseBt";
            this._PauseBt.Permitted = true;
            this._PauseBt.Size = new System.Drawing.Size(42, 28);
            this._PauseBt.TabIndex = 16;
            this._PauseBt.Text = ",";
            this._PauseBt.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            // 
            // txtUsuario
            // 
            this.txtUsuario.Location = new System.Drawing.Point(145, 87);
            this.txtUsuario.Name = "txtUsuario";
            this.txtUsuario.Size = new System.Drawing.Size(320, 20);
            this.txtUsuario.TabIndex = 0;
            // 
            // txtContrasena
            // 
            this.txtContrasena.Location = new System.Drawing.Point(145, 113);
            this.txtContrasena.Name = "txtContrasena";
            this.txtContrasena.PasswordChar = '*';
            this.txtContrasena.Size = new System.Drawing.Size(317, 20);
            this.txtContrasena.TabIndex = 1;
            // 
            // btnIniciarSesion
            // 
            this.btnIniciarSesion.Location = new System.Drawing.Point(153, 505);
            this.btnIniciarSesion.Name = "btnIniciarSesion";
            this.btnIniciarSesion.Size = new System.Drawing.Size(151, 29);
            this.btnIniciarSesion.TabIndex = 2;
            this.btnIniciarSesion.Text = "Iniciar Sesión";
            this.btnIniciarSesion.Click += new System.EventHandler(this.btnIniciarSesion_Click);
            // 
            // lblMensaje
            // 
            this.lblMensaje.Location = new System.Drawing.Point(50, 140);
            this.lblMensaje.Name = "lblMensaje";
            this.lblMensaje.Size = new System.Drawing.Size(276, 26);
            this.lblMensaje.TabIndex = 3;
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.Controls.Add(_KeypadTLP, 0, 0);
            this.tableLayoutPanel1.Location = new System.Drawing.Point(50, 139);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 1;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(415, 357);
            this.tableLayoutPanel1.TabIndex = 4;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(53, 93);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(43, 13);
            this.label1.TabIndex = 5;
            this.label1.Text = "Usuario";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(55, 113);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(34, 13);
            this.label2.TabIndex = 6;
            this.label2.Text = "Clave";
            // 
            // login
            // 
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Controls.Add(this.txtUsuario);
            this.Controls.Add(this.txtContrasena);
            this.Controls.Add(this.btnIniciarSesion);
            this.Controls.Add(this.lblMensaje);
            this.Name = "login";
            this.Size = new System.Drawing.Size(471, 547);
            _KeypadTLP.ResumeLayout(false);
            _DisplayTLP.ResumeLayout(false);
            _DisplayTLP.PerformLayout();
            this.tableLayoutPanel1.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        private void btnIniciarSesion_Click(object sender, EventArgs e)
        {
            // Lógica para validar las credenciales y establecer la autenticación
            string usuario = txtUsuario.Text;
            string contrasena = txtContrasena.Text;

            // Aquí debes implementar la lógica para validar el usuario y contrasena
            // Puedes usar una base de datos, servicio de autenticación, etc.

            if (usuario == "usuario" && contrasena == "contrasena")
            {
                // Autenticación exitosa, puedes realizar alguna acción o redirigir a otra pantalla
                lblMensaje.Text = "Inicio de sesión exitoso";
            }
            else
            {
                // Muestra un mensaje de error en caso de autenticación fallida
                lblMensaje.Text = "Credenciales incorrectas. Por favor, inténtalo de nuevo.";
            }
        }

        private TableLayoutPanel tableLayoutPanel1;
        private Model.Module.UI.HMIButton _1BT;
        private Model.Module.UI.HMIButton _2BT;
        private Model.Module.UI.HMIButton _3BT;
        private Model.Module.UI.HMIButton _4BT;
        private Model.Module.UI.HMIButton _5BT;
        private Model.Module.UI.HMIButton _6BT;
        private Model.Module.UI.HMIButton _7BT;
        private Model.Module.UI.HMIButton _8BT;
        private Model.Module.UI.HMIButton _9BT;
        private Model.Module.UI.HMIButton _AstBT;
        private Model.Module.UI.HMIButton _0BT;
        private Model.Module.UI.HMIButton _AlmBT;
        private Model.Module.UI.HMIButton _ClearBT;
        private TextBox _DisplayTB;
        private Model.Module.UI.HMIButton _PauseBt;
        private Label label1;
        private Label label2;
    }
}
