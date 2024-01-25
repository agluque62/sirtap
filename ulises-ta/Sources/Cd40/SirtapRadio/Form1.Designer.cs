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
            this.button1 = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // textBox_dstIP
            // 
            this.textBox_dstIP.Location = new System.Drawing.Point(153, 26);
            this.textBox_dstIP.Name = "textBox_dstIP";
            this.textBox_dstIP.Size = new System.Drawing.Size(221, 26);
            this.textBox_dstIP.TabIndex = 0;
            // 
            // textBox_SrcPort
            // 
            this.textBox_SrcPort.Location = new System.Drawing.Point(153, 61);
            this.textBox_SrcPort.Name = "textBox_SrcPort";
            this.textBox_SrcPort.Size = new System.Drawing.Size(221, 26);
            this.textBox_SrcPort.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(14, 26);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(133, 23);
            this.label1.TabIndex = 2;
            this.label1.Text = "Destination IP  ";
            this.label1.Click += new System.EventHandler(this.label1_Click);
            // 
            // label2
            // 
            this.label2.Location = new System.Drawing.Point(14, 61);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(133, 23);
            this.label2.TabIndex = 3;
            this.label2.Text = "Source port  ";
            // 
            // label3
            // 
            this.label3.Location = new System.Drawing.Point(14, 96);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(133, 27);
            this.label3.TabIndex = 4;
            this.label3.Text = "Destination port  ";
            // 
            // textBox_DstPort
            // 
            this.textBox_DstPort.Location = new System.Drawing.Point(153, 96);
            this.textBox_DstPort.Name = "textBox_DstPort";
            this.textBox_DstPort.Size = new System.Drawing.Size(221, 26);
            this.textBox_DstPort.TabIndex = 5;
            // 
            // label4
            // 
            this.label4.Location = new System.Drawing.Point(14, 131);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(133, 27);
            this.label4.TabIndex = 6;
            this.label4.Text = "Rcv. multicast IP  ";
            this.label4.Click += new System.EventHandler(this.label4_Click);
            // 
            // textBox_RcvMcastIP
            // 
            this.textBox_RcvMcastIP.Location = new System.Drawing.Point(153, 131);
            this.textBox_RcvMcastIP.Name = "textBox_RcvMcastIP";
            this.textBox_RcvMcastIP.Size = new System.Drawing.Size(221, 26);
            this.textBox_RcvMcastIP.TabIndex = 7;
            // 
            // comboBox_PaylType
            // 
            this.comboBox_PaylType.FormattingEnabled = true;
            this.comboBox_PaylType.Items.AddRange(new object[] {
            "PCMA",
            "PCMU"});
            this.comboBox_PaylType.Location = new System.Drawing.Point(153, 170);
            this.comboBox_PaylType.Name = "comboBox_PaylType";
            this.comboBox_PaylType.Size = new System.Drawing.Size(221, 28);
            this.comboBox_PaylType.TabIndex = 8;
            this.comboBox_PaylType.Text = "PCMA";
            // 
            // label5
            // 
            this.label5.Location = new System.Drawing.Point(14, 170);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(133, 27);
            this.label5.TabIndex = 9;
            this.label5.Text = "Payload Type";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(521, 75);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(174, 69);
            this.button1.TabIndex = 10;
            this.button1.Text = "button1";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(9F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1121, 676);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.comboBox_PaylType);
            this.Controls.Add(this.textBox_RcvMcastIP);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.textBox_DstPort);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.textBox_SrcPort);
            this.Controls.Add(this.textBox_dstIP);
            this.Name = "Form1";
            this.Text = "SIRTAP RADIO SIMULATOR";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.Form1_FormClosed);
            this.Load += new System.EventHandler(this.Form1_Load);
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
        private System.Windows.Forms.Button button1;
    }
}

