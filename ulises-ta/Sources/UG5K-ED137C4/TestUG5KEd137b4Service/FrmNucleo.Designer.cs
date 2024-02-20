namespace NucleoSim
{
    partial class FrmNucleo
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FrmNucleo));
            this.ofdlgFichero = new System.Windows.Forms.OpenFileDialog();
            this.txtLog = new System.Windows.Forms.TextBox();
            this.panel2 = new System.Windows.Forms.Panel();
            this.label5 = new System.Windows.Forms.Label();
            this.cboServidor = new System.Windows.Forms.ComboBox();
            this.txtPuerto = new System.Windows.Forms.TextBox();
            this.txtIP = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.panel4 = new System.Windows.Forms.Panel();
            this.btnSesionOFF = new System.Windows.Forms.Button();
            this.btnSesionON = new System.Windows.Forms.Button();
            this.imlEstado = new System.Windows.Forms.ImageList(this.components);
            this.panel5 = new System.Windows.Forms.Panel();
            this.txtRespuesta = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.panel6 = new System.Windows.Forms.Panel();
            this.btnFin = new System.Windows.Forms.Button();
            this.btnReconfigurar = new System.Windows.Forms.Button();
            this.tmrEnvio = new System.Windows.Forms.Timer(this.components);
            this.lstRecursos = new System.Windows.Forms.ListBox();
            this.label2 = new System.Windows.Forms.Label();
            this.btnNuevo = new System.Windows.Forms.Button();
            this.btnMenos = new System.Windows.Forms.Button();
            this.btnSQHON = new System.Windows.Forms.Button();
            this.btnSQHOFF = new System.Windows.Forms.Button();
            this.btnSTON = new System.Windows.Forms.Button();
            this.btnSTOFF = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.txtFichero = new System.Windows.Forms.TextBox();
            this.btnSelFichero = new System.Windows.Forms.Button();
            this.btnSesion = new System.Windows.Forms.Button();
            this.cboTipo = new System.Windows.Forms.ComboBox();
            this.label4 = new System.Windows.Forms.Label();
            this.dudDatos = new System.Windows.Forms.DomainUpDown();
            this.picEstado = new System.Windows.Forms.PictureBox();
            this.dudVeces = new System.Windows.Forms.DomainUpDown();
            this.chkEnvioInd = new System.Windows.Forms.CheckBox();
            this.btnTodos = new System.Windows.Forms.Button();
            this.btnTest = new System.Windows.Forms.Button();
            this.pnlTelefono = new System.Windows.Forms.Panel();
            this.txtRecursoEntrante = new System.Windows.Forms.TextBox();
            this.label14 = new System.Windows.Forms.Label();
            this.label13 = new System.Windows.Forms.Label();
            this.cboCausaDesc2 = new System.Windows.Forms.ComboBox();
            this.txtTelefonoEntrante = new System.Windows.Forms.TextBox();
            this.label10 = new System.Windows.Forms.Label();
            this.btnTransferEnd = new System.Windows.Forms.Button();
            this.btnTransferConnect = new System.Windows.Forms.Button();
            this.btnTransferStart = new System.Windows.Forms.Button();
            this.btnHOLDOFF = new System.Windows.Forms.Button();
            this.btnHOLDON = new System.Windows.Forms.Button();
            this.label15 = new System.Windows.Forms.Label();
            this.cboPrioridad = new System.Windows.Forms.ComboBox();
            this.label12 = new System.Windows.Forms.Label();
            this.cboCausaDesc = new System.Windows.Forms.ComboBox();
            this.label11 = new System.Windows.Forms.Label();
            this.cboDireccion = new System.Windows.Forms.ComboBox();
            this.txtTelefonoExt = new System.Windows.Forms.TextBox();
            this.label9 = new System.Windows.Forms.Label();
            this.txtTelefono = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.btnCallEnd = new System.Windows.Forms.Button();
            this.btnConnect = new System.Windows.Forms.Button();
            this.btnCallStart = new System.Windows.Forms.Button();
            this.panel1 = new System.Windows.Forms.Panel();
            this.btnREC = new System.Windows.Forms.Button();
            this.btnPause = new System.Windows.Forms.Button();
            this.panel2.SuspendLayout();
            this.panel4.SuspendLayout();
            this.panel5.SuspendLayout();
            this.panel6.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.picEstado)).BeginInit();
            this.pnlTelefono.SuspendLayout();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // ofdlgFichero
            // 
            this.ofdlgFichero.DefaultExt = "*.wav";
            // 
            // txtLog
            // 
            this.txtLog.AcceptsReturn = true;
            this.txtLog.AcceptsTab = true;
            this.txtLog.BackColor = System.Drawing.SystemColors.Window;
            this.txtLog.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.txtLog.Location = new System.Drawing.Point(0, 368);
            this.txtLog.Multiline = true;
            this.txtLog.Name = "txtLog";
            this.txtLog.ReadOnly = true;
            this.txtLog.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.txtLog.Size = new System.Drawing.Size(657, 174);
            this.txtLog.TabIndex = 5;
            this.txtLog.TextChanged += new System.EventHandler(this.txtLog_TextChanged);
            // 
            // panel2
            // 
            this.panel2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel2.Controls.Add(this.label5);
            this.panel2.Controls.Add(this.cboServidor);
            this.panel2.Controls.Add(this.txtPuerto);
            this.panel2.Controls.Add(this.txtIP);
            this.panel2.Controls.Add(this.label6);
            this.panel2.Controls.Add(this.label7);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel2.Location = new System.Drawing.Point(0, 0);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(657, 39);
            this.panel2.TabIndex = 6;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.Location = new System.Drawing.Point(416, 13);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(60, 13);
            this.label5.TabIndex = 17;
            this.label5.Text = "Pasarela:";
            this.label5.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // cboServidor
            // 
            this.cboServidor.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboServidor.FormattingEnabled = true;
            this.cboServidor.Items.AddRange(new object[] {
            "Local - Windows",
            "Local - Debian7"});
            this.cboServidor.Location = new System.Drawing.Point(495, 7);
            this.cboServidor.Name = "cboServidor";
            this.cboServidor.Size = new System.Drawing.Size(151, 21);
            this.cboServidor.TabIndex = 16;
            this.cboServidor.SelectedIndexChanged += new System.EventHandler(this.cboServidor_SelectedIndexChanged);
            // 
            // txtPuerto
            // 
            this.txtPuerto.Location = new System.Drawing.Point(313, 6);
            this.txtPuerto.Name = "txtPuerto";
            this.txtPuerto.ReadOnly = true;
            this.txtPuerto.Size = new System.Drawing.Size(85, 20);
            this.txtPuerto.TabIndex = 4;
            this.txtPuerto.TabStop = false;
            // 
            // txtIP
            // 
            this.txtIP.Location = new System.Drawing.Point(89, 6);
            this.txtIP.Name = "txtIP";
            this.txtIP.ReadOnly = true;
            this.txtIP.Size = new System.Drawing.Size(100, 20);
            this.txtIP.TabIndex = 3;
            this.txtIP.TabStop = false;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.Location = new System.Drawing.Point(209, 13);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(98, 13);
            this.label6.TabIndex = 1;
            this.label6.Text = "Puerto Servicio:";
            this.label6.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label7.Location = new System.Drawing.Point(14, 13);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(73, 13);
            this.label7.TabIndex = 0;
            this.label7.Text = "IP Servicio:";
            this.label7.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // panel4
            // 
            this.panel4.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel4.Controls.Add(this.btnSesionOFF);
            this.panel4.Controls.Add(this.btnSesionON);
            this.panel4.Location = new System.Drawing.Point(0, 43);
            this.panel4.Name = "panel4";
            this.panel4.Size = new System.Drawing.Size(279, 46);
            this.panel4.TabIndex = 21;
            // 
            // btnSesionOFF
            // 
            this.btnSesionOFF.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSesionOFF.Location = new System.Drawing.Point(128, 7);
            this.btnSesionOFF.Name = "btnSesionOFF";
            this.btnSesionOFF.Size = new System.Drawing.Size(111, 34);
            this.btnSesionOFF.TabIndex = 6;
            this.btnSesionOFF.Text = "Finalizar Sesión";
            this.btnSesionOFF.UseVisualStyleBackColor = true;
            this.btnSesionOFF.Click += new System.EventHandler(this.btnSesionOFF_Click);
            // 
            // btnSesionON
            // 
            this.btnSesionON.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSesionON.Location = new System.Drawing.Point(11, 7);
            this.btnSesionON.Name = "btnSesionON";
            this.btnSesionON.Size = new System.Drawing.Size(111, 34);
            this.btnSesionON.TabIndex = 5;
            this.btnSesionON.Text = "Iniciar Sesión";
            this.btnSesionON.UseVisualStyleBackColor = true;
            this.btnSesionON.Click += new System.EventHandler(this.btnSesionON_Click);
            // 
            // imlEstado
            // 
            this.imlEstado.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imlEstado.ImageStream")));
            this.imlEstado.TransparentColor = System.Drawing.Color.Transparent;
            this.imlEstado.Images.SetKeyName(0, "rojo.png");
            this.imlEstado.Images.SetKeyName(1, "naranja.png");
            this.imlEstado.Images.SetKeyName(2, "verde.png");
            // 
            // panel5
            // 
            this.panel5.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel5.Controls.Add(this.txtRespuesta);
            this.panel5.Controls.Add(this.label8);
            this.panel5.Location = new System.Drawing.Point(520, 44);
            this.panel5.Name = "panel5";
            this.panel5.Size = new System.Drawing.Size(136, 44);
            this.panel5.TabIndex = 22;
            // 
            // txtRespuesta
            // 
            this.txtRespuesta.Location = new System.Drawing.Point(90, 11);
            this.txtRespuesta.Name = "txtRespuesta";
            this.txtRespuesta.ReadOnly = true;
            this.txtRespuesta.Size = new System.Drawing.Size(38, 20);
            this.txtRespuesta.TabIndex = 19;
            this.txtRespuesta.TabStop = false;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label8.Location = new System.Drawing.Point(10, 15);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(71, 13);
            this.label8.TabIndex = 18;
            this.label8.Text = "Respuesta:";
            this.label8.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // panel6
            // 
            this.panel6.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel6.Controls.Add(this.btnFin);
            this.panel6.Controls.Add(this.btnReconfigurar);
            this.panel6.Location = new System.Drawing.Point(284, 43);
            this.panel6.Name = "panel6";
            this.panel6.Size = new System.Drawing.Size(231, 46);
            this.panel6.TabIndex = 23;
            // 
            // btnFin
            // 
            this.btnFin.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnFin.Location = new System.Drawing.Point(114, 7);
            this.btnFin.Name = "btnFin";
            this.btnFin.Size = new System.Drawing.Size(98, 34);
            this.btnFin.TabIndex = 6;
            this.btnFin.Text = "Cierra Módulo";
            this.btnFin.UseVisualStyleBackColor = true;
            this.btnFin.Click += new System.EventHandler(this.btnFin_Click);
            // 
            // btnReconfigurar
            // 
            this.btnReconfigurar.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnReconfigurar.Location = new System.Drawing.Point(8, 7);
            this.btnReconfigurar.Name = "btnReconfigurar";
            this.btnReconfigurar.Size = new System.Drawing.Size(98, 34);
            this.btnReconfigurar.TabIndex = 5;
            this.btnReconfigurar.Text = "Reconfigurar Módulo";
            this.btnReconfigurar.UseVisualStyleBackColor = true;
            this.btnReconfigurar.Click += new System.EventHandler(this.btnReconfigurar_Click);
            // 
            // tmrEnvio
            // 
            this.tmrEnvio.Interval = 30;
            this.tmrEnvio.Tick += new System.EventHandler(this.tmrEnvio_Tick);
            // 
            // lstRecursos
            // 
            this.lstRecursos.FormattingEnabled = true;
            this.lstRecursos.Items.AddRange(new object[] {
            "120.88",
            "100.80",
            "95.10"});
            this.lstRecursos.Location = new System.Drawing.Point(16, 63);
            this.lstRecursos.Name = "lstRecursos";
            this.lstRecursos.ScrollAlwaysVisible = true;
            this.lstRecursos.SelectionMode = System.Windows.Forms.SelectionMode.MultiSimple;
            this.lstRecursos.Size = new System.Drawing.Size(128, 173);
            this.lstRecursos.TabIndex = 1;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(13, 44);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(64, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "Recursos:";
            // 
            // btnNuevo
            // 
            this.btnNuevo.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnNuevo.Location = new System.Drawing.Point(16, 240);
            this.btnNuevo.Name = "btnNuevo";
            this.btnNuevo.Size = new System.Drawing.Size(40, 22);
            this.btnNuevo.TabIndex = 3;
            this.btnNuevo.Text = "+";
            this.btnNuevo.UseVisualStyleBackColor = true;
            this.btnNuevo.Click += new System.EventHandler(this.btnNuevo_Click);
            // 
            // btnMenos
            // 
            this.btnMenos.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnMenos.Location = new System.Drawing.Point(62, 240);
            this.btnMenos.Name = "btnMenos";
            this.btnMenos.Size = new System.Drawing.Size(37, 22);
            this.btnMenos.TabIndex = 4;
            this.btnMenos.Text = "-";
            this.btnMenos.UseVisualStyleBackColor = true;
            this.btnMenos.Click += new System.EventHandler(this.btnMenos_Click);
            // 
            // btnSQHON
            // 
            this.btnSQHON.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSQHON.Location = new System.Drawing.Point(159, 114);
            this.btnSQHON.Name = "btnSQHON";
            this.btnSQHON.Size = new System.Drawing.Size(118, 35);
            this.btnSQHON.TabIndex = 7;
            this.btnSQHON.Text = "SQU ON";
            this.btnSQHON.UseVisualStyleBackColor = true;
            this.btnSQHON.Click += new System.EventHandler(this.btnSQUON_Click);
            // 
            // btnSQHOFF
            // 
            this.btnSQHOFF.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSQHOFF.Location = new System.Drawing.Point(159, 151);
            this.btnSQHOFF.Name = "btnSQHOFF";
            this.btnSQHOFF.Size = new System.Drawing.Size(118, 35);
            this.btnSQHOFF.TabIndex = 8;
            this.btnSQHOFF.Text = "SQU OFF";
            this.btnSQHOFF.UseVisualStyleBackColor = true;
            this.btnSQHOFF.Click += new System.EventHandler(this.btnSQUOFF_Click);
            // 
            // btnSTON
            // 
            this.btnSTON.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSTON.Location = new System.Drawing.Point(159, 188);
            this.btnSTON.Name = "btnSTON";
            this.btnSTON.Size = new System.Drawing.Size(118, 35);
            this.btnSTON.TabIndex = 9;
            this.btnSTON.Text = "Simultaneous Transmission ON";
            this.btnSTON.UseVisualStyleBackColor = true;
            this.btnSTON.Click += new System.EventHandler(this.btnSTON_Click);
            // 
            // btnSTOFF
            // 
            this.btnSTOFF.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSTOFF.Location = new System.Drawing.Point(159, 226);
            this.btnSTOFF.Name = "btnSTOFF";
            this.btnSTOFF.Size = new System.Drawing.Size(118, 35);
            this.btnSTOFF.TabIndex = 10;
            this.btnSTOFF.Text = "Simultaneous Transmission OFF";
            this.btnSTOFF.UseVisualStyleBackColor = true;
            this.btnSTOFF.Click += new System.EventHandler(this.btnSTOFF_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(484, 111);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(106, 13);
            this.label3.TabIndex = 11;
            this.label3.Text = "Fichero de datos:";
            // 
            // txtFichero
            // 
            this.txtFichero.BackColor = System.Drawing.SystemColors.Window;
            this.txtFichero.Location = new System.Drawing.Point(319, 130);
            this.txtFichero.Name = "txtFichero";
            this.txtFichero.ReadOnly = true;
            this.txtFichero.Size = new System.Drawing.Size(274, 20);
            this.txtFichero.TabIndex = 12;
            // 
            // btnSelFichero
            // 
            this.btnSelFichero.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSelFichero.Location = new System.Drawing.Point(599, 130);
            this.btnSelFichero.Name = "btnSelFichero";
            this.btnSelFichero.Size = new System.Drawing.Size(24, 22);
            this.btnSelFichero.TabIndex = 13;
            this.btnSelFichero.Text = "...";
            this.btnSelFichero.UseVisualStyleBackColor = true;
            this.btnSelFichero.Click += new System.EventHandler(this.btnSelFichero_Click);
            // 
            // btnSesion
            // 
            this.btnSesion.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnSesion.Location = new System.Drawing.Point(368, 230);
            this.btnSesion.Name = "btnSesion";
            this.btnSesion.Size = new System.Drawing.Size(182, 31);
            this.btnSesion.TabIndex = 14;
            this.btnSesion.Text = "Enviar fichero";
            this.btnSesion.UseVisualStyleBackColor = true;
            this.btnSesion.Click += new System.EventHandler(this.btnSesion_Click);
            // 
            // cboTipo
            // 
            this.cboTipo.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboTipo.FormattingEnabled = true;
            this.cboTipo.Items.AddRange(new object[] {
            "Radio - GRS",
            "Terminal - TER"});
            this.cboTipo.Location = new System.Drawing.Point(16, 10);
            this.cboTipo.Name = "cboTipo";
            this.cboTipo.Size = new System.Drawing.Size(126, 21);
            this.cboTipo.TabIndex = 15;
            this.cboTipo.SelectedIndexChanged += new System.EventHandler(this.cboTipo_SelectedIndexChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(318, 166);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(202, 13);
            this.label4.TabIndex = 18;
            this.label4.Text = "Tiempo paquetes de datos (mseg):";
            // 
            // dudDatos
            // 
            this.dudDatos.Location = new System.Drawing.Point(531, 162);
            this.dudDatos.Name = "dudDatos";
            this.dudDatos.Size = new System.Drawing.Size(92, 20);
            this.dudDatos.TabIndex = 19;
            this.dudDatos.Text = "60";
            this.dudDatos.Wrap = true;
            this.dudDatos.SelectedItemChanged += new System.EventHandler(this.dudDatos_SelectedItemChanged);
            // 
            // picEstado
            // 
            this.picEstado.Location = new System.Drawing.Point(330, 239);
            this.picEstado.Name = "picEstado";
            this.picEstado.Size = new System.Drawing.Size(16, 16);
            this.picEstado.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.picEstado.TabIndex = 20;
            this.picEstado.TabStop = false;
            // 
            // dudVeces
            // 
            this.dudVeces.Location = new System.Drawing.Point(556, 237);
            this.dudVeces.Name = "dudVeces";
            this.dudVeces.Size = new System.Drawing.Size(67, 20);
            this.dudVeces.TabIndex = 21;
            this.dudVeces.Text = "1";
            this.dudVeces.Wrap = true;
            // 
            // chkEnvioInd
            // 
            this.chkEnvioInd.AutoSize = true;
            this.chkEnvioInd.Checked = true;
            this.chkEnvioInd.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkEnvioInd.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.chkEnvioInd.Location = new System.Drawing.Point(324, 201);
            this.chkEnvioInd.Name = "chkEnvioInd";
            this.chkEnvioInd.Size = new System.Drawing.Size(258, 17);
            this.chkEnvioInd.TabIndex = 22;
            this.chkEnvioInd.Text = "Habilitar envío independiente del fichero";
            this.chkEnvioInd.TextImageRelation = System.Windows.Forms.TextImageRelation.TextAboveImage;
            this.chkEnvioInd.UseVisualStyleBackColor = true;
            this.chkEnvioInd.CheckedChanged += new System.EventHandler(this.chkEnvioInd_CheckedChanged);
            // 
            // btnTodos
            // 
            this.btnTodos.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnTodos.Location = new System.Drawing.Point(105, 240);
            this.btnTodos.Name = "btnTodos";
            this.btnTodos.Size = new System.Drawing.Size(37, 22);
            this.btnTodos.TabIndex = 23;
            this.btnTodos.Text = "*";
            this.btnTodos.UseVisualStyleBackColor = true;
            this.btnTodos.Click += new System.EventHandler(this.btnTodos_Click);
            // 
            // btnTest
            // 
            this.btnTest.Enabled = false;
            this.btnTest.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnTest.Location = new System.Drawing.Point(284, 224);
            this.btnTest.Name = "btnTest";
            this.btnTest.Size = new System.Drawing.Size(40, 35);
            this.btnTest.TabIndex = 37;
            this.btnTest.Text = "X";
            this.btnTest.UseVisualStyleBackColor = true;
            this.btnTest.Visible = false;
            this.btnTest.Click += new System.EventHandler(this.btnTest_Click);
            // 
            // pnlTelefono
            // 
            this.pnlTelefono.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.pnlTelefono.Controls.Add(this.txtRecursoEntrante);
            this.pnlTelefono.Controls.Add(this.label14);
            this.pnlTelefono.Controls.Add(this.label13);
            this.pnlTelefono.Controls.Add(this.cboCausaDesc2);
            this.pnlTelefono.Controls.Add(this.txtTelefonoEntrante);
            this.pnlTelefono.Controls.Add(this.label10);
            this.pnlTelefono.Controls.Add(this.btnTransferEnd);
            this.pnlTelefono.Controls.Add(this.btnTransferConnect);
            this.pnlTelefono.Controls.Add(this.btnTransferStart);
            this.pnlTelefono.Controls.Add(this.btnHOLDOFF);
            this.pnlTelefono.Controls.Add(this.btnHOLDON);
            this.pnlTelefono.Controls.Add(this.label15);
            this.pnlTelefono.Controls.Add(this.cboPrioridad);
            this.pnlTelefono.Controls.Add(this.label12);
            this.pnlTelefono.Controls.Add(this.cboCausaDesc);
            this.pnlTelefono.Controls.Add(this.label11);
            this.pnlTelefono.Controls.Add(this.cboDireccion);
            this.pnlTelefono.Controls.Add(this.txtTelefonoExt);
            this.pnlTelefono.Controls.Add(this.label9);
            this.pnlTelefono.Controls.Add(this.txtTelefono);
            this.pnlTelefono.Controls.Add(this.label1);
            this.pnlTelefono.Controls.Add(this.btnCallEnd);
            this.pnlTelefono.Controls.Add(this.btnConnect);
            this.pnlTelefono.Controls.Add(this.btnCallStart);
            this.pnlTelefono.Location = new System.Drawing.Point(159, 3);
            this.pnlTelefono.Name = "pnlTelefono";
            this.pnlTelefono.Size = new System.Drawing.Size(484, 91);
            this.pnlTelefono.TabIndex = 22;
            // 
            // txtRecursoEntrante
            // 
            this.txtRecursoEntrante.Enabled = false;
            this.txtRecursoEntrante.Location = new System.Drawing.Point(397, 97);
            this.txtRecursoEntrante.Name = "txtRecursoEntrante";
            this.txtRecursoEntrante.Size = new System.Drawing.Size(76, 20);
            this.txtRecursoEntrante.TabIndex = 84;
            this.txtRecursoEntrante.TabStop = false;
            // 
            // label14
            // 
            this.label14.AutoSize = true;
            this.label14.Enabled = false;
            this.label14.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label14.Location = new System.Drawing.Point(331, 100);
            this.label14.Name = "label14";
            this.label14.Size = new System.Drawing.Size(69, 13);
            this.label14.TabIndex = 83;
            this.label14.Text = "Rec. Entr.:";
            this.label14.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Enabled = false;
            this.label13.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label13.Location = new System.Drawing.Point(183, 129);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(83, 13);
            this.label13.TabIndex = 82;
            this.label13.Text = "Causa Desc.:";
            this.label13.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // cboCausaDesc2
            // 
            this.cboCausaDesc2.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboCausaDesc2.Enabled = false;
            this.cboCausaDesc2.FormattingEnabled = true;
            this.cboCausaDesc2.Items.AddRange(new object[] {
            "16 - Liberación normal de llamada",
            "17 - Usuario ocupado",
            "18 - No responde",
            "21 - Llamada rechazada",
            "41 - Fallo temporal"});
            this.cboCausaDesc2.Location = new System.Drawing.Point(282, 124);
            this.cboCausaDesc2.Name = "cboCausaDesc2";
            this.cboCausaDesc2.Size = new System.Drawing.Size(194, 21);
            this.cboCausaDesc2.TabIndex = 81;
            // 
            // txtTelefonoEntrante
            // 
            this.txtTelefonoEntrante.Enabled = false;
            this.txtTelefonoEntrante.Location = new System.Drawing.Point(246, 98);
            this.txtTelefonoEntrante.Name = "txtTelefonoEntrante";
            this.txtTelefonoEntrante.Size = new System.Drawing.Size(81, 20);
            this.txtTelefonoEntrante.TabIndex = 80;
            this.txtTelefonoEntrante.TabStop = false;
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Enabled = false;
            this.label10.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label10.Location = new System.Drawing.Point(183, 101);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(64, 13);
            this.label10.TabIndex = 79;
            this.label10.Text = "Tel. Entr.:";
            this.label10.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // btnTransferEnd
            // 
            this.btnTransferEnd.Enabled = false;
            this.btnTransferEnd.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnTransferEnd.Location = new System.Drawing.Point(6, 133);
            this.btnTransferEnd.Name = "btnTransferEnd";
            this.btnTransferEnd.Size = new System.Drawing.Size(81, 35);
            this.btnTransferEnd.TabIndex = 78;
            this.btnTransferEnd.Text = "Transfer End";
            this.btnTransferEnd.UseVisualStyleBackColor = true;
            // 
            // btnTransferConnect
            // 
            this.btnTransferConnect.Enabled = false;
            this.btnTransferConnect.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnTransferConnect.Location = new System.Drawing.Point(91, 95);
            this.btnTransferConnect.Name = "btnTransferConnect";
            this.btnTransferConnect.Size = new System.Drawing.Size(81, 35);
            this.btnTransferConnect.TabIndex = 77;
            this.btnTransferConnect.Text = "Transfer Connect";
            this.btnTransferConnect.UseVisualStyleBackColor = true;
            // 
            // btnTransferStart
            // 
            this.btnTransferStart.Enabled = false;
            this.btnTransferStart.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnTransferStart.Location = new System.Drawing.Point(6, 95);
            this.btnTransferStart.Name = "btnTransferStart";
            this.btnTransferStart.Size = new System.Drawing.Size(81, 35);
            this.btnTransferStart.TabIndex = 76;
            this.btnTransferStart.Text = "Transfer Start";
            this.btnTransferStart.UseVisualStyleBackColor = true;
            // 
            // btnHOLDOFF
            // 
            this.btnHOLDOFF.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnHOLDOFF.Location = new System.Drawing.Point(135, 47);
            this.btnHOLDOFF.Name = "btnHOLDOFF";
            this.btnHOLDOFF.Size = new System.Drawing.Size(49, 35);
            this.btnHOLDOFF.TabIndex = 75;
            this.btnHOLDOFF.Text = "HOLD OFF";
            this.btnHOLDOFF.UseVisualStyleBackColor = true;
            this.btnHOLDOFF.Click += new System.EventHandler(this.btnHOLDOFF_Click);
            // 
            // btnHOLDON
            // 
            this.btnHOLDON.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnHOLDON.Location = new System.Drawing.Point(85, 47);
            this.btnHOLDON.Name = "btnHOLDON";
            this.btnHOLDON.Size = new System.Drawing.Size(49, 35);
            this.btnHOLDON.TabIndex = 74;
            this.btnHOLDON.Text = "HOLD ON";
            this.btnHOLDON.UseVisualStyleBackColor = true;
            this.btnHOLDON.Click += new System.EventHandler(this.btnHOLDON_Click);
            // 
            // label15
            // 
            this.label15.AutoSize = true;
            this.label15.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label15.Location = new System.Drawing.Point(335, 12);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(41, 13);
            this.label15.TabIndex = 73;
            this.label15.Text = "Prior.:";
            this.label15.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // cboPrioridad
            // 
            this.cboPrioridad.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboPrioridad.FormattingEnabled = true;
            this.cboPrioridad.Items.AddRange(new object[] {
            "1 - Emergencia",
            "2 - Urgente",
            "3 - Normal",
            "4 - No Urgente"});
            this.cboPrioridad.Location = new System.Drawing.Point(396, 7);
            this.cboPrioridad.Name = "cboPrioridad";
            this.cboPrioridad.Size = new System.Drawing.Size(82, 21);
            this.cboPrioridad.TabIndex = 72;
            // 
            // label12
            // 
            this.label12.AutoSize = true;
            this.label12.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label12.Location = new System.Drawing.Point(190, 63);
            this.label12.Name = "label12";
            this.label12.Size = new System.Drawing.Size(83, 13);
            this.label12.TabIndex = 71;
            this.label12.Text = "Causa Desc.:";
            this.label12.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // cboCausaDesc
            // 
            this.cboCausaDesc.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboCausaDesc.FormattingEnabled = true;
            this.cboCausaDesc.Items.AddRange(new object[] {
            "16 - Liberación normal de llamada",
            "17 - Usuario ocupado",
            "18 - No responde",
            "21 - Llamada rechazada",
            "41 - Fallo temporal"});
            this.cboCausaDesc.Location = new System.Drawing.Point(281, 58);
            this.cboCausaDesc.Name = "cboCausaDesc";
            this.cboCausaDesc.Size = new System.Drawing.Size(197, 21);
            this.cboCausaDesc.TabIndex = 70;
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label11.Location = new System.Drawing.Point(188, 12);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(52, 13);
            this.label11.TabIndex = 69;
            this.label11.Text = "Direcc.:";
            this.label11.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // cboDireccion
            // 
            this.cboDireccion.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cboDireccion.FormattingEnabled = true;
            this.cboDireccion.Items.AddRange(new object[] {
            "1 - Entrante",
            "2 - Saliente"});
            this.cboDireccion.Location = new System.Drawing.Point(249, 7);
            this.cboDireccion.Name = "cboDireccion";
            this.cboDireccion.Size = new System.Drawing.Size(82, 21);
            this.cboDireccion.TabIndex = 68;
            this.cboDireccion.SelectedIndexChanged += new System.EventHandler(this.cboDireccion_SelectedIndexChanged);
            // 
            // txtTelefonoExt
            // 
            this.txtTelefonoExt.Location = new System.Drawing.Point(394, 34);
            this.txtTelefonoExt.Name = "txtTelefonoExt";
            this.txtTelefonoExt.Size = new System.Drawing.Size(83, 20);
            this.txtTelefonoExt.TabIndex = 67;
            this.txtTelefonoExt.TabStop = false;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label9.Location = new System.Drawing.Point(334, 37);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(63, 13);
            this.label9.TabIndex = 66;
            this.label9.Text = "Telf. Ext.:";
            this.label9.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // txtTelefono
            // 
            this.txtTelefono.Location = new System.Drawing.Point(248, 32);
            this.txtTelefono.Name = "txtTelefono";
            this.txtTelefono.Size = new System.Drawing.Size(83, 20);
            this.txtTelefono.TabIndex = 65;
            this.txtTelefono.TabStop = false;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(186, 35);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(61, 13);
            this.label1.TabIndex = 64;
            this.label1.Text = "Telefono:";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // btnCallEnd
            // 
            this.btnCallEnd.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnCallEnd.Location = new System.Drawing.Point(5, 48);
            this.btnCallEnd.Name = "btnCallEnd";
            this.btnCallEnd.Size = new System.Drawing.Size(73, 35);
            this.btnCallEnd.TabIndex = 63;
            this.btnCallEnd.Text = "Call End";
            this.btnCallEnd.UseVisualStyleBackColor = true;
            this.btnCallEnd.Click += new System.EventHandler(this.btnCallEnd_Click);
            // 
            // btnConnect
            // 
            this.btnConnect.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnConnect.Location = new System.Drawing.Point(84, 10);
            this.btnConnect.Name = "btnConnect";
            this.btnConnect.Size = new System.Drawing.Size(100, 35);
            this.btnConnect.TabIndex = 62;
            this.btnConnect.Text = "Connect";
            this.btnConnect.UseVisualStyleBackColor = true;
            this.btnConnect.Click += new System.EventHandler(this.btnConnect_Click);
            // 
            // btnCallStart
            // 
            this.btnCallStart.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnCallStart.Location = new System.Drawing.Point(5, 10);
            this.btnCallStart.Name = "btnCallStart";
            this.btnCallStart.Size = new System.Drawing.Size(73, 35);
            this.btnCallStart.TabIndex = 61;
            this.btnCallStart.Text = "Call Start";
            this.btnCallStart.UseVisualStyleBackColor = true;
            this.btnCallStart.Click += new System.EventHandler(this.btnCallStart_Click);
            // 
            // panel1
            // 
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel1.Controls.Add(this.btnPause);
            this.panel1.Controls.Add(this.btnREC);
            this.panel1.Controls.Add(this.pnlTelefono);
            this.panel1.Controls.Add(this.btnTest);
            this.panel1.Controls.Add(this.btnTodos);
            this.panel1.Controls.Add(this.chkEnvioInd);
            this.panel1.Controls.Add(this.dudVeces);
            this.panel1.Controls.Add(this.picEstado);
            this.panel1.Controls.Add(this.dudDatos);
            this.panel1.Controls.Add(this.label4);
            this.panel1.Controls.Add(this.cboTipo);
            this.panel1.Controls.Add(this.btnSesion);
            this.panel1.Controls.Add(this.btnSelFichero);
            this.panel1.Controls.Add(this.txtFichero);
            this.panel1.Controls.Add(this.label3);
            this.panel1.Controls.Add(this.btnSTOFF);
            this.panel1.Controls.Add(this.btnSTON);
            this.panel1.Controls.Add(this.btnSQHOFF);
            this.panel1.Controls.Add(this.btnSQHON);
            this.panel1.Controls.Add(this.btnMenos);
            this.panel1.Controls.Add(this.btnNuevo);
            this.panel1.Controls.Add(this.label2);
            this.panel1.Controls.Add(this.lstRecursos);
            this.panel1.Location = new System.Drawing.Point(1, 95);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(653, 272);
            this.panel1.TabIndex = 0;
            // 
            // btnREC
            // 
            this.btnREC.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnREC.Location = new System.Drawing.Point(314, 99);
            this.btnREC.Name = "btnREC";
            this.btnREC.Size = new System.Drawing.Size(75, 23);
            this.btnREC.TabIndex = 38;
            this.btnREC.Text = "RECord";
            this.btnREC.UseVisualStyleBackColor = true;
            this.btnREC.Click += new System.EventHandler(this.btnRECord_Click);
            // 
            // btnPause
            // 
            this.btnPause.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnPause.Location = new System.Drawing.Point(397, 99);
            this.btnPause.Name = "btnPause";
            this.btnPause.Size = new System.Drawing.Size(75, 23);
            this.btnPause.TabIndex = 39;
            this.btnPause.Text = "PAUse";
            this.btnPause.UseVisualStyleBackColor = true;
            this.btnPause.Click += new System.EventHandler(this.btnPAUse_Click);
            // 
            // FrmNucleo
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(657, 542);
            this.Controls.Add(this.panel6);
            this.Controls.Add(this.panel5);
            this.Controls.Add(this.panel4);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.txtLog);
            this.Controls.Add(this.panel1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "FrmNucleo";
            this.Text = "FrmNucleo";
            this.Load += new System.EventHandler(this.FrmNucleo_Load);
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.panel4.ResumeLayout(false);
            this.panel5.ResumeLayout(false);
            this.panel5.PerformLayout();
            this.panel6.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.picEstado)).EndInit();
            this.pnlTelefono.ResumeLayout(false);
            this.pnlTelefono.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.OpenFileDialog ofdlgFichero;
        private System.Windows.Forms.TextBox txtLog;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.TextBox txtPuerto;
        private System.Windows.Forms.TextBox txtIP;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.ComboBox cboServidor;
        private System.Windows.Forms.Panel panel4;
        private System.Windows.Forms.Button btnSesionOFF;
        private System.Windows.Forms.Button btnSesionON;
        private System.Windows.Forms.ImageList imlEstado;
        private System.Windows.Forms.Panel panel5;
        private System.Windows.Forms.TextBox txtRespuesta;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Panel panel6;
        private System.Windows.Forms.Button btnFin;
        private System.Windows.Forms.Button btnReconfigurar;
        private System.Windows.Forms.Timer tmrEnvio;
        private System.Windows.Forms.ListBox lstRecursos;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button btnNuevo;
        private System.Windows.Forms.Button btnMenos;
        private System.Windows.Forms.Button btnSQHON;
        private System.Windows.Forms.Button btnSQHOFF;
        private System.Windows.Forms.Button btnSTON;
        private System.Windows.Forms.Button btnSTOFF;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox txtFichero;
        private System.Windows.Forms.Button btnSelFichero;
        private System.Windows.Forms.Button btnSesion;
        private System.Windows.Forms.ComboBox cboTipo;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.DomainUpDown dudDatos;
        private System.Windows.Forms.PictureBox picEstado;
        private System.Windows.Forms.DomainUpDown dudVeces;
        private System.Windows.Forms.CheckBox chkEnvioInd;
        private System.Windows.Forms.Button btnTodos;
        private System.Windows.Forms.Button btnTest;
        private System.Windows.Forms.Panel pnlTelefono;
        private System.Windows.Forms.Button btnHOLDOFF;
        private System.Windows.Forms.Button btnHOLDON;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.ComboBox cboPrioridad;
        private System.Windows.Forms.Label label12;
        private System.Windows.Forms.ComboBox cboCausaDesc;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.ComboBox cboDireccion;
        private System.Windows.Forms.TextBox txtTelefonoExt;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.TextBox txtTelefono;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button btnCallEnd;
        private System.Windows.Forms.Button btnConnect;
        private System.Windows.Forms.Button btnCallStart;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.TextBox txtRecursoEntrante;
        private System.Windows.Forms.Label label14;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.ComboBox cboCausaDesc2;
        private System.Windows.Forms.TextBox txtTelefonoEntrante;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Button btnTransferEnd;
        private System.Windows.Forms.Button btnTransferConnect;
        private System.Windows.Forms.Button btnTransferStart;
        private System.Windows.Forms.Button btnREC;
        private System.Windows.Forms.Button btnPause;
    }
}