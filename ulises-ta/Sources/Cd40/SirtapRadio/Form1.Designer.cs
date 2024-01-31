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
            this.label_hostip = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.label_Status = new System.Windows.Forms.Label();
            this.button_ConfigSet = new System.Windows.Forms.Button();
            this.label_Transmitting = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.generictimer = new System.Windows.Forms.Timer(this.components);
            this.button_SCH = new System.Windows.Forms.Button();
            this.button_bucle = new System.Windows.Forms.Button();
            this.label9 = new System.Windows.Forms.Label();
            this.label10 = new System.Windows.Forms.Label();
            this.button_bucle2 = new System.Windows.Forms.Button();
            this.button_SCH2 = new System.Windows.Forms.Button();
            this.label11 = new System.Windows.Forms.Label();
            this.label_Transmitting2 = new System.Windows.Forms.Label();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.label13 = new System.Windows.Forms.Label();
            this.label_Status2 = new System.Windows.Forms.Label();
            this.button_ConfigSet2 = new System.Windows.Forms.Button();
            this.textBox_RcvMcastIP2 = new System.Windows.Forms.TextBox();
            this.label15 = new System.Windows.Forms.Label();
            this.textBox_dstIP2 = new System.Windows.Forms.TextBox();
            this.comboBox_PaylType2 = new System.Windows.Forms.ComboBox();
            this.textBox_SrcPort2 = new System.Windows.Forms.TextBox();
            this.label16 = new System.Windows.Forms.Label();
            this.label17 = new System.Windows.Forms.Label();
            this.label18 = new System.Windows.Forms.Label();
            this.textBox_DstPort2 = new System.Windows.Forms.TextBox();
            this.label19 = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.SuspendLayout();
            // 
            // textBox_dstIP
            // 
            this.textBox_dstIP.Location = new System.Drawing.Point(154, 96);
            this.textBox_dstIP.Name = "textBox_dstIP";
            this.textBox_dstIP.Size = new System.Drawing.Size(221, 26);
            this.textBox_dstIP.TabIndex = 2;
            // 
            // textBox_SrcPort
            // 
            this.textBox_SrcPort.Location = new System.Drawing.Point(154, 65);
            this.textBox_SrcPort.Name = "textBox_SrcPort";
            this.textBox_SrcPort.Size = new System.Drawing.Size(221, 26);
            this.textBox_SrcPort.TabIndex = 3;
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(7, 96);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(141, 23);
            this.label1.TabIndex = 10;
            this.label1.Text = "Rem. multicast Gr";
            this.label1.Click += new System.EventHandler(this.label1_Click);
            // 
            // label2
            // 
            this.label2.Location = new System.Drawing.Point(7, 65);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(133, 23);
            this.label2.TabIndex = 13;
            this.label2.Text = "Local port  ";
            this.label2.Click += new System.EventHandler(this.label2_Click);
            // 
            // label3
            // 
            this.label3.Location = new System.Drawing.Point(7, 128);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(133, 27);
            this.label3.TabIndex = 14;
            this.label3.Text = "Rem. port  ";
            // 
            // textBox_DstPort
            // 
            this.textBox_DstPort.Location = new System.Drawing.Point(154, 128);
            this.textBox_DstPort.Name = "textBox_DstPort";
            this.textBox_DstPort.Size = new System.Drawing.Size(221, 26);
            this.textBox_DstPort.TabIndex = 4;
            // 
            // label4
            // 
            this.label4.Location = new System.Drawing.Point(7, 32);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(141, 27);
            this.label4.TabIndex = 11;
            this.label4.Text = "Local multicast Gr";
            this.label4.Click += new System.EventHandler(this.label4_Click);
            // 
            // textBox_RcvMcastIP
            // 
            this.textBox_RcvMcastIP.Location = new System.Drawing.Point(154, 33);
            this.textBox_RcvMcastIP.Name = "textBox_RcvMcastIP";
            this.textBox_RcvMcastIP.Size = new System.Drawing.Size(221, 26);
            this.textBox_RcvMcastIP.TabIndex = 1;
            // 
            // comboBox_PaylType
            // 
            this.comboBox_PaylType.FormattingEnabled = true;
            this.comboBox_PaylType.Items.AddRange(new object[] {
            "PCMA",
            "PCMU"});
            this.comboBox_PaylType.Location = new System.Drawing.Point(154, 160);
            this.comboBox_PaylType.Name = "comboBox_PaylType";
            this.comboBox_PaylType.Size = new System.Drawing.Size(221, 28);
            this.comboBox_PaylType.TabIndex = 5;
            this.comboBox_PaylType.Text = "PCMA";
            // 
            // label5
            // 
            this.label5.Location = new System.Drawing.Point(7, 161);
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
            this.groupBox1.Location = new System.Drawing.Point(12, 81);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(579, 218);
            this.groupBox1.TabIndex = 10;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "CONFIGURATION";
            // 
            // label_hostip
            // 
            this.label_hostip.AutoSize = true;
            this.label_hostip.Location = new System.Drawing.Point(95, 9);
            this.label_hostip.Name = "label_hostip";
            this.label_hostip.Size = new System.Drawing.Size(57, 20);
            this.label_hostip.TabIndex = 19;
            this.label_hostip.Text = "0.0.0.0";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(19, 9);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(66, 20);
            this.label8.TabIndex = 18;
            this.label8.Text = "Local IP";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(420, 97);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(107, 20);
            this.label7.TabIndex = 17;
            this.label7.Text = "Simul. Status:";
            this.label7.Click += new System.EventHandler(this.label7_Click);
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
            // label_Transmitting
            // 
            this.label_Transmitting.AutoSize = true;
            this.label_Transmitting.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label_Transmitting.Location = new System.Drawing.Point(150, 314);
            this.label_Transmitting.Name = "label_Transmitting";
            this.label_Transmitting.Size = new System.Drawing.Size(91, 20);
            this.label_Transmitting.TabIndex = 11;
            this.label_Transmitting.Text = "STOPPED";
            this.label_Transmitting.Click += new System.EventHandler(this.label_Transmitting_Click);
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(19, 314);
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
            // button_SCH
            // 
            this.button_SCH.Location = new System.Drawing.Point(23, 359);
            this.button_SCH.Name = "button_SCH";
            this.button_SCH.Size = new System.Drawing.Size(140, 58);
            this.button_SCH.TabIndex = 13;
            this.button_SCH.Text = "SCH ON";
            this.button_SCH.UseVisualStyleBackColor = true;
            this.button_SCH.Click += new System.EventHandler(this.button_SCH_Click);
            // 
            // button_bucle
            // 
            this.button_bucle.Location = new System.Drawing.Point(190, 359);
            this.button_bucle.Name = "button_bucle";
            this.button_bucle.Size = new System.Drawing.Size(131, 58);
            this.button_bucle.TabIndex = 14;
            this.button_bucle.Text = "BUCLE ON";
            this.button_bucle.UseVisualStyleBackColor = true;
            this.button_bucle.Click += new System.EventHandler(this.button_bucle_Click);
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label9.Location = new System.Drawing.Point(198, 49);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(146, 29);
            this.label9.TabIndex = 20;
            this.label9.Text = "CHANNEL 1";
            // 
            // label10
            // 
            this.label10.AutoSize = true;
            this.label10.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label10.Location = new System.Drawing.Point(833, 49);
            this.label10.Name = "label10";
            this.label10.Size = new System.Drawing.Size(146, 29);
            this.label10.TabIndex = 26;
            this.label10.Text = "CHANNEL 2";
            // 
            // button_bucle2
            // 
            this.button_bucle2.Location = new System.Drawing.Point(797, 359);
            this.button_bucle2.Name = "button_bucle2";
            this.button_bucle2.Size = new System.Drawing.Size(131, 58);
            this.button_bucle2.TabIndex = 25;
            this.button_bucle2.Text = "BUCLE ON";
            this.button_bucle2.UseVisualStyleBackColor = true;
            this.button_bucle2.Click += new System.EventHandler(this.button_bucle2_Click);
            // 
            // button_SCH2
            // 
            this.button_SCH2.Location = new System.Drawing.Point(632, 359);
            this.button_SCH2.Name = "button_SCH2";
            this.button_SCH2.Size = new System.Drawing.Size(140, 58);
            this.button_SCH2.TabIndex = 24;
            this.button_SCH2.Text = "SCH ON";
            this.button_SCH2.UseVisualStyleBackColor = true;
            this.button_SCH2.Click += new System.EventHandler(this.button_SCH2_Click);
            // 
            // label11
            // 
            this.label11.AutoSize = true;
            this.label11.Location = new System.Drawing.Point(628, 314);
            this.label11.Name = "label11";
            this.label11.Size = new System.Drawing.Size(125, 20);
            this.label11.TabIndex = 23;
            this.label11.Text = "Transmit Status:";
            // 
            // label_Transmitting2
            // 
            this.label_Transmitting2.AutoSize = true;
            this.label_Transmitting2.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label_Transmitting2.Location = new System.Drawing.Point(759, 314);
            this.label_Transmitting2.Name = "label_Transmitting2";
            this.label_Transmitting2.Size = new System.Drawing.Size(91, 20);
            this.label_Transmitting2.TabIndex = 22;
            this.label_Transmitting2.Text = "STOPPED";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.label13);
            this.groupBox2.Controls.Add(this.label_Status2);
            this.groupBox2.Controls.Add(this.button_ConfigSet2);
            this.groupBox2.Controls.Add(this.textBox_RcvMcastIP2);
            this.groupBox2.Controls.Add(this.label15);
            this.groupBox2.Controls.Add(this.textBox_dstIP2);
            this.groupBox2.Controls.Add(this.comboBox_PaylType2);
            this.groupBox2.Controls.Add(this.textBox_SrcPort2);
            this.groupBox2.Controls.Add(this.label16);
            this.groupBox2.Controls.Add(this.label17);
            this.groupBox2.Controls.Add(this.label18);
            this.groupBox2.Controls.Add(this.textBox_DstPort2);
            this.groupBox2.Controls.Add(this.label19);
            this.groupBox2.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.groupBox2.Location = new System.Drawing.Point(621, 81);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(579, 218);
            this.groupBox2.TabIndex = 21;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "CONFIGURATION";
            // 
            // label13
            // 
            this.label13.AutoSize = true;
            this.label13.Location = new System.Drawing.Point(420, 97);
            this.label13.Name = "label13";
            this.label13.Size = new System.Drawing.Size(107, 20);
            this.label13.TabIndex = 17;
            this.label13.Text = "Simul. Status:";
            // 
            // label_Status2
            // 
            this.label_Status2.AutoSize = true;
            this.label_Status2.Location = new System.Drawing.Point(420, 130);
            this.label_Status2.Name = "label_Status2";
            this.label_Status2.Size = new System.Drawing.Size(84, 20);
            this.label_Status2.TabIndex = 16;
            this.label_Status2.Text = "STOPPED";
            // 
            // button_ConfigSet2
            // 
            this.button_ConfigSet2.Location = new System.Drawing.Point(424, 27);
            this.button_ConfigSet2.Name = "button_ConfigSet2";
            this.button_ConfigSet2.Size = new System.Drawing.Size(88, 45);
            this.button_ConfigSet2.TabIndex = 6;
            this.button_ConfigSet2.Text = "SET";
            this.button_ConfigSet2.UseVisualStyleBackColor = true;
            this.button_ConfigSet2.Click += new System.EventHandler(this.button_ConfigSet2_Click);
            // 
            // textBox_RcvMcastIP2
            // 
            this.textBox_RcvMcastIP2.Location = new System.Drawing.Point(154, 33);
            this.textBox_RcvMcastIP2.Name = "textBox_RcvMcastIP2";
            this.textBox_RcvMcastIP2.Size = new System.Drawing.Size(221, 26);
            this.textBox_RcvMcastIP2.TabIndex = 1;
            // 
            // label15
            // 
            this.label15.Location = new System.Drawing.Point(7, 161);
            this.label15.Name = "label15";
            this.label15.Size = new System.Drawing.Size(133, 27);
            this.label15.TabIndex = 15;
            this.label15.Text = "Payload Type";
            // 
            // textBox_dstIP2
            // 
            this.textBox_dstIP2.Location = new System.Drawing.Point(154, 96);
            this.textBox_dstIP2.Name = "textBox_dstIP2";
            this.textBox_dstIP2.Size = new System.Drawing.Size(221, 26);
            this.textBox_dstIP2.TabIndex = 2;
            // 
            // comboBox_PaylType2
            // 
            this.comboBox_PaylType2.FormattingEnabled = true;
            this.comboBox_PaylType2.Items.AddRange(new object[] {
            "PCMA",
            "PCMU"});
            this.comboBox_PaylType2.Location = new System.Drawing.Point(154, 160);
            this.comboBox_PaylType2.Name = "comboBox_PaylType2";
            this.comboBox_PaylType2.Size = new System.Drawing.Size(221, 28);
            this.comboBox_PaylType2.TabIndex = 5;
            this.comboBox_PaylType2.Text = "PCMA";
            // 
            // textBox_SrcPort2
            // 
            this.textBox_SrcPort2.Location = new System.Drawing.Point(154, 65);
            this.textBox_SrcPort2.Name = "textBox_SrcPort2";
            this.textBox_SrcPort2.Size = new System.Drawing.Size(221, 26);
            this.textBox_SrcPort2.TabIndex = 3;
            // 
            // label16
            // 
            this.label16.Location = new System.Drawing.Point(7, 97);
            this.label16.Name = "label16";
            this.label16.Size = new System.Drawing.Size(141, 23);
            this.label16.TabIndex = 10;
            this.label16.Text = "Rem. multicast Gr  ";
            // 
            // label17
            // 
            this.label17.Location = new System.Drawing.Point(7, 32);
            this.label17.Name = "label17";
            this.label17.Size = new System.Drawing.Size(141, 27);
            this.label17.TabIndex = 11;
            this.label17.Text = "Local multicast Gr";
            // 
            // label18
            // 
            this.label18.Location = new System.Drawing.Point(7, 65);
            this.label18.Name = "label18";
            this.label18.Size = new System.Drawing.Size(133, 23);
            this.label18.TabIndex = 13;
            this.label18.Text = "Local port  ";
            // 
            // textBox_DstPort2
            // 
            this.textBox_DstPort2.Location = new System.Drawing.Point(154, 128);
            this.textBox_DstPort2.Name = "textBox_DstPort2";
            this.textBox_DstPort2.Size = new System.Drawing.Size(221, 26);
            this.textBox_DstPort2.TabIndex = 4;
            // 
            // label19
            // 
            this.label19.Location = new System.Drawing.Point(7, 128);
            this.label19.Name = "label19";
            this.label19.Size = new System.Drawing.Size(133, 27);
            this.label19.TabIndex = 14;
            this.label19.Text = "Rem. port  ";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1215, 444);
            this.Controls.Add(this.label10);
            this.Controls.Add(this.button_bucle2);
            this.Controls.Add(this.button_SCH2);
            this.Controls.Add(this.label11);
            this.Controls.Add(this.label_Transmitting2);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.label9);
            this.Controls.Add(this.label_hostip);
            this.Controls.Add(this.button_bucle);
            this.Controls.Add(this.label8);
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
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
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
        private System.Windows.Forms.Label label_hostip;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.Label label10;
        private System.Windows.Forms.Button button_bucle2;
        private System.Windows.Forms.Button button_SCH2;
        private System.Windows.Forms.Label label11;
        private System.Windows.Forms.Label label_Transmitting2;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label label13;
        private System.Windows.Forms.Label label_Status2;
        private System.Windows.Forms.Button button_ConfigSet2;
        private System.Windows.Forms.TextBox textBox_RcvMcastIP2;
        private System.Windows.Forms.Label label15;
        private System.Windows.Forms.TextBox textBox_dstIP2;
        private System.Windows.Forms.ComboBox comboBox_PaylType2;
        private System.Windows.Forms.TextBox textBox_SrcPort2;
        private System.Windows.Forms.Label label16;
        private System.Windows.Forms.Label label17;
        private System.Windows.Forms.Label label18;
        private System.Windows.Forms.TextBox textBox_DstPort2;
        private System.Windows.Forms.Label label19;
    }
}

