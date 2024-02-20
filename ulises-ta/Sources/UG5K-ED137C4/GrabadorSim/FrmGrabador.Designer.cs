namespace GrabadorSim
{
    partial class FrmGrabador
    {
        /// <summary>
        /// Variable del diseñador requerida.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Limpiar los recursos que se estén utilizando.
        /// </summary>
        /// <param name="disposing">true si los recursos administrados se deben eliminar; false en caso contrario.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Código generado por el Diseñador de Windows Forms

        /// <summary>
        /// Método necesario para admitir el Diseñador. No se puede modificar
        /// el contenido del método con el editor de código.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FrmGrabador));
            System.Windows.Forms.DataGridViewCellStyle dataGridViewCellStyle1 = new System.Windows.Forms.DataGridViewCellStyle();
            this.panel1 = new System.Windows.Forms.Panel();
            this.picEstado = new System.Windows.Forms.PictureBox();
            this.txtRTP = new System.Windows.Forms.TextBox();
            this.txtRTSP = new System.Windows.Forms.TextBox();
            this.txtIP = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.btnIniciar = new System.Windows.Forms.Button();
            this.btnParar = new System.Windows.Forms.Button();
            this.tmrControl = new System.Windows.Forms.Timer(this.components);
            this.imlEstado = new System.Windows.Forms.ImageList(this.components);
            this.panel2 = new System.Windows.Forms.Panel();
            this.txtSesionesTot = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.txtSesiones = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.btnIniciarLocal = new System.Windows.Forms.Button();
            this.btnIniciarRedan = new System.Windows.Forms.Button();
            this.panel3 = new System.Windows.Forms.Panel();
            this.dgvSamplers = new System.Windows.Forms.DataGridView();
            this.txtLog = new System.Windows.Forms.TextBox();
            this.chkLog = new System.Windows.Forms.CheckBox();
            this.panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.picEstado)).BeginInit();
            this.panel2.SuspendLayout();
            this.panel3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dgvSamplers)).BeginInit();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel1.Controls.Add(this.picEstado);
            this.panel1.Controls.Add(this.txtRTP);
            this.panel1.Controls.Add(this.txtRTSP);
            this.panel1.Controls.Add(this.txtIP);
            this.panel1.Controls.Add(this.label3);
            this.panel1.Controls.Add(this.label2);
            this.panel1.Controls.Add(this.label1);
            this.panel1.Location = new System.Drawing.Point(0, 0);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(336, 62);
            this.panel1.TabIndex = 0;
            // 
            // picEstado
            // 
            this.picEstado.Location = new System.Drawing.Point(311, 10);
            this.picEstado.Name = "picEstado";
            this.picEstado.Size = new System.Drawing.Size(16, 16);
            this.picEstado.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.picEstado.TabIndex = 6;
            this.picEstado.TabStop = false;
            // 
            // txtRTP
            // 
            this.txtRTP.Location = new System.Drawing.Point(247, 34);
            this.txtRTP.Name = "txtRTP";
            this.txtRTP.ReadOnly = true;
            this.txtRTP.Size = new System.Drawing.Size(53, 20);
            this.txtRTP.TabIndex = 5;
            this.txtRTP.TabStop = false;
            // 
            // txtRTSP
            // 
            this.txtRTSP.Location = new System.Drawing.Point(247, 6);
            this.txtRTSP.Name = "txtRTSP";
            this.txtRTSP.ReadOnly = true;
            this.txtRTSP.Size = new System.Drawing.Size(53, 20);
            this.txtRTSP.TabIndex = 4;
            this.txtRTSP.TabStop = false;
            // 
            // txtIP
            // 
            this.txtIP.Location = new System.Drawing.Point(46, 6);
            this.txtIP.Name = "txtIP";
            this.txtIP.ReadOnly = true;
            this.txtIP.Size = new System.Drawing.Size(100, 20);
            this.txtIP.TabIndex = 3;
            this.txtIP.TabStop = false;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(164, 37);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(77, 13);
            this.label3.TabIndex = 2;
            this.label3.Text = "Puerto RTP:";
            this.label3.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(164, 10);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(85, 13);
            this.label2.TabIndex = 1;
            this.label2.Text = "Puerto RTSP:";
            this.label2.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(14, 13);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(23, 13);
            this.label1.TabIndex = 0;
            this.label1.Text = "IP:";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // btnIniciar
            // 
            this.btnIniciar.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnIniciar.Location = new System.Drawing.Point(3, 99);
            this.btnIniciar.Name = "btnIniciar";
            this.btnIniciar.Size = new System.Drawing.Size(81, 44);
            this.btnIniciar.TabIndex = 1;
            this.btnIniciar.Text = "Iniciar";
            this.btnIniciar.UseVisualStyleBackColor = true;
            this.btnIniciar.Click += new System.EventHandler(this.btnIniciar_Click);
            // 
            // btnParar
            // 
            this.btnParar.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnParar.Location = new System.Drawing.Point(254, 99);
            this.btnParar.Name = "btnParar";
            this.btnParar.Size = new System.Drawing.Size(81, 44);
            this.btnParar.TabIndex = 2;
            this.btnParar.Text = "Parar";
            this.btnParar.UseVisualStyleBackColor = true;
            this.btnParar.Click += new System.EventHandler(this.btnParar_Click);
            // 
            // tmrControl
            // 
            this.tmrControl.Interval = 10;
            this.tmrControl.Tick += new System.EventHandler(this.tmrControl_Tick);
            // 
            // imlEstado
            // 
            this.imlEstado.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imlEstado.ImageStream")));
            this.imlEstado.TransparentColor = System.Drawing.Color.Transparent;
            this.imlEstado.Images.SetKeyName(0, "rojo.png");
            this.imlEstado.Images.SetKeyName(1, "naranja.png");
            this.imlEstado.Images.SetKeyName(2, "verde.png");
            // 
            // panel2
            // 
            this.panel2.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel2.Controls.Add(this.txtSesionesTot);
            this.panel2.Controls.Add(this.label5);
            this.panel2.Controls.Add(this.txtSesiones);
            this.panel2.Controls.Add(this.label4);
            this.panel2.Location = new System.Drawing.Point(2, 67);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(258, 28);
            this.panel2.TabIndex = 3;
            // 
            // txtSesionesTot
            // 
            this.txtSesionesTot.Location = new System.Drawing.Point(197, 3);
            this.txtSesionesTot.Name = "txtSesionesTot";
            this.txtSesionesTot.ReadOnly = true;
            this.txtSesionesTot.Size = new System.Drawing.Size(51, 20);
            this.txtSesionesTot.TabIndex = 7;
            this.txtSesionesTot.TabStop = false;
            this.txtSesionesTot.Text = "0";
            this.txtSesionesTot.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label5.Location = new System.Drawing.Point(142, 8);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(53, 13);
            this.label5.TabIndex = 6;
            this.label5.Text = "Totales:";
            this.label5.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // txtSesiones
            // 
            this.txtSesiones.Location = new System.Drawing.Point(78, 2);
            this.txtSesiones.Name = "txtSesiones";
            this.txtSesiones.ReadOnly = true;
            this.txtSesiones.Size = new System.Drawing.Size(47, 20);
            this.txtSesiones.TabIndex = 5;
            this.txtSesiones.TabStop = false;
            this.txtSesiones.Text = "0";
            this.txtSesiones.TextAlign = System.Windows.Forms.HorizontalAlignment.Right;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(12, 8);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(62, 13);
            this.label4.TabIndex = 4;
            this.label4.Text = "Sesiones:";
            this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleLeft;
            // 
            // btnIniciarLocal
            // 
            this.btnIniciarLocal.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnIniciarLocal.Location = new System.Drawing.Point(169, 99);
            this.btnIniciarLocal.Name = "btnIniciarLocal";
            this.btnIniciarLocal.Size = new System.Drawing.Size(83, 44);
            this.btnIniciarLocal.TabIndex = 5;
            this.btnIniciarLocal.Text = "Iniciar en LOCAL";
            this.btnIniciarLocal.UseVisualStyleBackColor = true;
            this.btnIniciarLocal.Click += new System.EventHandler(this.btnIniciarLocal_Click);
            // 
            // btnIniciarRedan
            // 
            this.btnIniciarRedan.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnIniciarRedan.Location = new System.Drawing.Point(86, 99);
            this.btnIniciarRedan.Name = "btnIniciarRedan";
            this.btnIniciarRedan.Size = new System.Drawing.Size(81, 44);
            this.btnIniciarRedan.TabIndex = 7;
            this.btnIniciarRedan.Text = "Iniciar en REDAN";
            this.btnIniciarRedan.UseVisualStyleBackColor = true;
            this.btnIniciarRedan.Click += new System.EventHandler(this.btnIniciarRedan_Click);
            // 
            // panel3
            // 
            this.panel3.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel3.Controls.Add(this.dgvSamplers);
            this.panel3.Location = new System.Drawing.Point(342, 0);
            this.panel3.Name = "panel3";
            this.panel3.Size = new System.Drawing.Size(359, 331);
            this.panel3.TabIndex = 8;
            // 
            // dgvSamplers
            // 
            this.dgvSamplers.AllowUserToAddRows = false;
            this.dgvSamplers.AllowUserToDeleteRows = false;
            this.dgvSamplers.AllowUserToResizeColumns = false;
            this.dgvSamplers.AllowUserToResizeRows = false;
            this.dgvSamplers.AutoSizeColumnsMode = System.Windows.Forms.DataGridViewAutoSizeColumnsMode.Fill;
            this.dgvSamplers.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dgvSamplers.ColumnHeadersVisible = false;
            this.dgvSamplers.Cursor = System.Windows.Forms.Cursors.Default;
            dataGridViewCellStyle1.Alignment = System.Windows.Forms.DataGridViewContentAlignment.MiddleLeft;
            dataGridViewCellStyle1.BackColor = System.Drawing.SystemColors.Window;
            dataGridViewCellStyle1.Font = new System.Drawing.Font("Courier New", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            dataGridViewCellStyle1.ForeColor = System.Drawing.SystemColors.ControlText;
            dataGridViewCellStyle1.SelectionBackColor = System.Drawing.SystemColors.Highlight;
            dataGridViewCellStyle1.SelectionForeColor = System.Drawing.SystemColors.HighlightText;
            dataGridViewCellStyle1.WrapMode = System.Windows.Forms.DataGridViewTriState.False;
            this.dgvSamplers.DefaultCellStyle = dataGridViewCellStyle1;
            this.dgvSamplers.Dock = System.Windows.Forms.DockStyle.Fill;
            this.dgvSamplers.Location = new System.Drawing.Point(0, 0);
            this.dgvSamplers.MultiSelect = false;
            this.dgvSamplers.Name = "dgvSamplers";
            this.dgvSamplers.ReadOnly = true;
            this.dgvSamplers.RowHeadersVisible = false;
            this.dgvSamplers.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.dgvSamplers.Size = new System.Drawing.Size(357, 329);
            this.dgvSamplers.TabIndex = 10;
            // 
            // txtLog
            // 
            this.txtLog.AcceptsReturn = true;
            this.txtLog.AcceptsTab = true;
            this.txtLog.BackColor = System.Drawing.SystemColors.Window;
            this.txtLog.Location = new System.Drawing.Point(0, 150);
            this.txtLog.Multiline = true;
            this.txtLog.Name = "txtLog";
            this.txtLog.ReadOnly = true;
            this.txtLog.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.txtLog.Size = new System.Drawing.Size(336, 181);
            this.txtLog.TabIndex = 4;
            this.txtLog.TextChanged += new System.EventHandler(this.txtLog_TextChanged);
            // 
            // chkLog
            // 
            this.chkLog.AutoSize = true;
            this.chkLog.Checked = true;
            this.chkLog.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkLog.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.chkLog.Location = new System.Drawing.Point(269, 75);
            this.chkLog.Name = "chkLog";
            this.chkLog.Size = new System.Drawing.Size(47, 17);
            this.chkLog.TabIndex = 9;
            this.chkLog.Text = "Log";
            this.chkLog.UseVisualStyleBackColor = true;
            this.chkLog.CheckedChanged += new System.EventHandler(this.chkLog_CheckedChanged);
            // 
            // FrmGrabador
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(700, 331);
            this.Controls.Add(this.chkLog);
            this.Controls.Add(this.panel3);
            this.Controls.Add(this.btnIniciarRedan);
            this.Controls.Add(this.btnIniciarLocal);
            this.Controls.Add(this.txtLog);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.btnParar);
            this.Controls.Add(this.btnIniciar);
            this.Controls.Add(this.panel1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "FrmGrabador";
            this.Text = "Grabador Simulado";
            this.Load += new System.EventHandler(this.FrmGrabador_Load);
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.picEstado)).EndInit();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.panel3.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.dgvSamplers)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox txtRTP;
        private System.Windows.Forms.TextBox txtRTSP;
        private System.Windows.Forms.TextBox txtIP;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button btnIniciar;
        private System.Windows.Forms.Button btnParar;
        private System.Windows.Forms.Timer tmrControl;
        private System.Windows.Forms.PictureBox picEstado;
        private System.Windows.Forms.ImageList imlEstado;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.TextBox txtSesiones;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox txtSesionesTot;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Button btnIniciarLocal;
        private System.Windows.Forms.Button btnIniciarRedan;
        private System.Windows.Forms.Panel panel3;
        private System.Windows.Forms.DataGridView dgvSamplers;
        private System.Windows.Forms.TextBox txtLog;
        private System.Windows.Forms.CheckBox chkLog;
    }
}

