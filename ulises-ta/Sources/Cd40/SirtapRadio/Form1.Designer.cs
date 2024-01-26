namespace SirtapRadio
{
    partial class Form1
    {
        /// <summary>
        /// Variable del diseñador necesaria.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Limpiar los recursos que se estén usando.
        /// </summary>
        /// <param name="disposing">true si los recursos administrados se deben desechar; false en caso contrario.</param>
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
        /// el contenido de este método con el editor de código.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.textBox_dstIP = new System.Windows.Forms.TextBox();
            this.textBox_SrcPort = new System.Windows.Forms.TextBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.textBox_DstPort = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.textBox_RcvMcastIP = new System.Windows.Forms.TextBox();
            this.comboBox_PaylType = new System.Windows.Forms.ComboBox();
            this.label5 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.button_ConfigSet = new System.Windows.Forms.Button();
            this.label_Status = new System.Windows.Forms.Label();
            this.label_Transmitting = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.generictimer = new System.Windows.Forms.Timer(this.components);
            this.label7 = new System.Windows.Forms.Label();
            this.button_SCH = new System.Windows.Forms.Button();
            this.button_bucle = new System.Windows.Forms.Button();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // textBox_dstIP
            // 
            this.textBox_dstIP.Location = new System.Drawing.Point(147, 27);
            this.textBox_dstIP.Name = "textBox_dstIP";
            this.textBox_dstIP.Size = new System.Drawing.Size(221, 26);
            this.textBox_dstIP.TabIndex = 0;
            // 
            // textBox_SrcPort
            // 
            this.textBox_SrcPort.Location = new System.Drawing.Point(147, 91);
            this.textBox_SrcPort.Name = "textBox_SrcPort";
            this.textBox_SrcPort.Size = new System.Drawing.Size(221, 26);
            this.textBox_SrcPort.TabIndex = 3;
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(8, 27);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(133, 23);
            this.label1.TabIndex = 10;
            this.label1.Text = "Destination IP  ";
            this.label1.Click += new System.EventHandler(this.label1_Click);
            // 
            // label2
            // 
            this.label2.Location = new System.Drawing.Point(8, 91);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(133, 23);
            this.label2.TabIndex = 13;
            this.label2.Text = "Source port  ";
            // 
            // label3
            // 
            this.label3.Location = new System.Drawing.Point(8, 123);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(133, 27);
            this.label3.TabIndex = 14;
            this.label3.Text = "Destination port  ";
            // 
            // textBox_DstPort
            // 
            this.textBox_DstPort.Location = new System.Drawing.Point(147, 123);
            this.textBox_DstPort.Name = "textBox_DstPort";
            this.textBox_DstPort.Size = new System.Drawing.Size(221, 26);
            this.textBox_DstPort.TabIndex = 4;
            // 
            // label4
            // 
            this.label4.Location = new System.Drawing.Point(8, 59);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(133, 27);
            this.label4.TabIndex = 11;
            this.label4.Text = "Rcv. multicast IP  ";
            this.label4.Click += new System.EventHandler(this.label4_Click);
            // 
            // textBox_RcvMcastIP
            // 
            this.textBox_RcvMcastIP.Location = new System.Drawing.Point(147, 59);
            this.textBox_RcvMcastIP.Name = "textBox_RcvMcastIP";
            this.textBox_RcvMcastIP.Size = new System.Drawing.Size(221, 26);
            this.textBox_RcvMcastIP.TabIndex = 2;
            // 
            // comboBox_PaylType
            // 
            this.comboBox_PaylType.FormattingEnabled = true;
            this.comboBox_PaylType.Items.AddRange(new object[] {
            "PCMA",
            "PCMU"});
            this.comboBox_PaylType.Location = new System.Drawing.Point(147, 155);
            this.comboBox_PaylType.Name = "comboBox_PaylType";
            this.comboBox_PaylType.Size = new System.Drawing.Size(221, 28);
            this.comboBox_PaylType.TabIndex = 5;
            this.comboBox_PaylType.Text = "PCMA";
            // 
            // label5
            // 
            this.label5.Location = new System.Drawing.Point(8, 155);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(133, 27);
            this.label5.TabIndex = 15;
            this.label5.Text = "Payload Type";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.label7);
            this.groupBox1.Controls.Add(this.label_Status);
            this.groupBox1.Controls.Add(this.button_ConfigSet);
            this.groupBox1.Controls.Add(this.textBox_RcvMcastIP);
            this.groupBox1.Controls.Add(this.label5);
            this.groupBox1.Controls.Add(this.textBox_dstIP);
            this.groupBox1.Controls.Add(this.comboBox_PaylType);
            this.groupBox1.Controls.Add(this.textBox_SrcPort);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.label4);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.textBox_DstPort);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.groupBox1.Location = new System.Drawing.Point(12, 12);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(579, 202);
            this.groupBox1.TabIndex = 10;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "CONFIGURATION";
            // 
            // button_ConfigSet
            // 
            this.button_ConfigSet.Location = new System.Drawing.Point(424, 27);
            this.button_ConfigSet.Name = "button_ConfigSet";
            this.button_ConfigSet.Size = new System.Drawing.Size(88, 45);
            this.button_ConfigSet.TabIndex = 6;
            this.button_ConfigSet.Text = "SET";
            this.button_ConfigSet.UseVisualStyleBackColor = true;
            this.button_ConfigSet.Click += new System.EventHandler(this.button_ConfigSet_Click);
            // 
            // label_Status
            // 
            this.label_Status.AutoSize = true;
            this.label_Status.Location = new System.Drawing.Point(420, 130);
            this.label_Status.Name = "label_Status";
            this.label_Status.Size = new System.Drawing.Size(84, 20);
            this.label_Status.TabIndex = 16;
            this.label_Status.Text = "STOPPED";
            // 
            // label_Transmitting
            // 
            this.label_Transmitting.AutoSize = true;
            this.label_Transmitting.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label_Transmitting.Location = new System.Drawing.Point(143, 230);
            this.label_Transmitting.Name = "label_Transmitting";
            this.label_Transmitting.Size = new System.Drawing.Size(137, 30);
            this.label_Transmitting.TabIndex = 11;
            this.label_Transmitting.Text = "STOPPED";
            this.label_Transmitting.Click += new System.EventHandler(this.label_Transmitting_Click);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(12, 230);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(125, 20);
            this.label6.TabIndex = 12;
            this.label6.Text = "Transmit Status:";
            // 
            // generictimer
            // 
            this.generictimer.Interval = 50;
            this.generictimer.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(420, 97);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(161, 30);
            this.label7.TabIndex = 17;
            this.label7.Text = "Simul. Status:";
            this.label7.Click += new System.EventHandler(this.label7_Click);
            // 
            // button_SCH
            // 
            this.button_SCH.Location = new System.Drawing.Point(16, 263);
            this.button_SCH.Name = "button_SCH";
            this.button_SCH.Size = new System.Drawing.Size(140, 58);
            this.button_SCH.TabIndex = 13;
            this.button_SCH.Text = "SCH ON";
            this.button_SCH.UseVisualStyleBackColor = true;
            this.button_SCH.Click += new System.EventHandler(this.button_SCH_Click);
            // 
            // button_bucle
            // 
            this.button_bucle.Location = new System.Drawing.Point(199, 263);
            this.button_bucle.Name = "button_bucle";
            this.button_bucle.Size = new System.Drawing.Size(131, 58);
            this.button_bucle.TabIndex = 14;
            this.button_bucle.Text = "BUCLE ON";
            this.button_bucle.UseVisualStyleBackColor = true;
            this.button_bucle.Click += new System.EventHandler(this.button_bucle_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(611, 340);
            this.Controls.Add(this.button_bucle);
            this.Controls.Add(this.button_SCH);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label_Transmitting);
            this.Controls.Add(this.groupBox1);
            this.Name = "Form1";
            this.Text = "SIRTAP RADIO SIMULATOR";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Form1_FormClosed);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox textBox_dstIP;
        private System.Windows.Forms.TextBox textBox_SrcPort;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.TextBox textBox_DstPort;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox textBox_RcvMcastIP;
        private System.Windows.Forms.ComboBox comboBox_PaylType;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button button_ConfigSet;
        private System.Windows.Forms.Label label_Status;
        private System.Windows.Forms.Label label_Transmitting;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Timer generictimer;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Button button_SCH;
        private System.Windows.Forms.Button button_bucle;
    }
}

