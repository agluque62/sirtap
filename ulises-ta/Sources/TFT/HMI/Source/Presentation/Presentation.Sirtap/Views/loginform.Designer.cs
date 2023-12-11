
namespace HMI.Presentation.Sirtap.Views
{
    partial class loginform
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
            System.Windows.Forms.TableLayoutPanel _KeypadTLP;
            this._AstBT = new HMI.Model.Module.UI.HMIButton();
            this._1BT = new HMI.Model.Module.UI.HMIButton();
            this._2BT = new HMI.Model.Module.UI.HMIButton();
            this._3BT = new HMI.Model.Module.UI.HMIButton();
            this._4BT = new HMI.Model.Module.UI.HMIButton();
            this._5BT = new HMI.Model.Module.UI.HMIButton();
            this._6BT = new HMI.Model.Module.UI.HMIButton();
            this._7BT = new HMI.Model.Module.UI.HMIButton();
            this._8BT = new HMI.Model.Module.UI.HMIButton();
            this._9BT = new HMI.Model.Module.UI.HMIButton();
            this._0BT = new HMI.Model.Module.UI.HMIButton();
            this._AlmBT = new HMI.Model.Module.UI.HMIButton();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.txtUsuario = new System.Windows.Forms.TextBox();
            this.txtContrasena = new System.Windows.Forms.TextBox();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this._ClearBT = new HMI.Model.Module.UI.HMIButton();
            _KeypadTLP = new System.Windows.Forms.TableLayoutPanel();
            _KeypadTLP.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // _KeypadTLP
            // 
            _KeypadTLP.BackColor = System.Drawing.Color.Silver;
            _KeypadTLP.ColumnCount = 3;
            _KeypadTLP.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
            _KeypadTLP.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33.33334F));
            _KeypadTLP.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33.33334F));
            _KeypadTLP.Controls.Add(this._AstBT, 0, 3);
            _KeypadTLP.Controls.Add(this._1BT, 0, 0);
            _KeypadTLP.Controls.Add(this._2BT, 1, 0);
            _KeypadTLP.Controls.Add(this._3BT, 2, 0);
            _KeypadTLP.Controls.Add(this._4BT, 0, 1);
            _KeypadTLP.Controls.Add(this._5BT, 1, 1);
            _KeypadTLP.Controls.Add(this._6BT, 2, 1);
            _KeypadTLP.Controls.Add(this._7BT, 0, 2);
            _KeypadTLP.Controls.Add(this._8BT, 1, 2);
            _KeypadTLP.Controls.Add(this._9BT, 2, 2);
            _KeypadTLP.Controls.Add(this._0BT, 1, 3);
            _KeypadTLP.Controls.Add(this._AlmBT, 2, 3);
            _KeypadTLP.Location = new System.Drawing.Point(18, 61);
            _KeypadTLP.Margin = new System.Windows.Forms.Padding(5, 10, 0, 0);
            _KeypadTLP.Name = "_KeypadTLP";
            _KeypadTLP.RowCount = 4;
            _KeypadTLP.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 20F));
            _KeypadTLP.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 20F));
            _KeypadTLP.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 20F));
            _KeypadTLP.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 20F));
            _KeypadTLP.Size = new System.Drawing.Size(410, 347);
            _KeypadTLP.TabIndex = 11;
            // 
            // _AstBT
            // 
            this._AstBT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._AstBT.DrawX = false;
            this._AstBT.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._AstBT.IsButtonTlf = false;
            this._AstBT.IsConferencePreprogramada = false;
            this._AstBT.Location = new System.Drawing.Point(3, 261);
            this._AstBT.Name = "_AstBT";
            this._AstBT.Permitted = true;
            this._AstBT.Size = new System.Drawing.Size(130, 83);
            this._AstBT.TabIndex = 16;
            this._AstBT.Text = "Clave";
            this._AstBT.Click += new System.EventHandler(this._AstBT_Click);
            // 
            // _1BT
            // 
            this._1BT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._1BT.DrawX = false;
            this._1BT.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._1BT.IsButtonTlf = false;
            this._1BT.IsConferencePreprogramada = false;
            this._1BT.Location = new System.Drawing.Point(3, 3);
            this._1BT.Name = "_1BT";
            this._1BT.Permitted = true;
            this._1BT.Size = new System.Drawing.Size(130, 80);
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
            this._2BT.Location = new System.Drawing.Point(139, 3);
            this._2BT.Name = "_2BT";
            this._2BT.Permitted = true;
            this._2BT.Size = new System.Drawing.Size(130, 80);
            this._2BT.TabIndex = 2;
            this._2BT.Text = "2";
            this._2BT.Click += new System.EventHandler(this._2BT_Click);
            // 
            // _3BT
            // 
            this._3BT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._3BT.DrawX = false;
            this._3BT.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._3BT.IsButtonTlf = false;
            this._3BT.IsConferencePreprogramada = false;
            this._3BT.Location = new System.Drawing.Point(275, 3);
            this._3BT.Name = "_3BT";
            this._3BT.Permitted = true;
            this._3BT.Size = new System.Drawing.Size(132, 80);
            this._3BT.TabIndex = 3;
            this._3BT.Text = "3";
            this._3BT.Click += new System.EventHandler(this._3BT_Click);
            // 
            // _4BT
            // 
            this._4BT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._4BT.DrawX = false;
            this._4BT.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._4BT.IsButtonTlf = false;
            this._4BT.IsConferencePreprogramada = false;
            this._4BT.Location = new System.Drawing.Point(3, 89);
            this._4BT.Name = "_4BT";
            this._4BT.Permitted = true;
            this._4BT.Size = new System.Drawing.Size(130, 80);
            this._4BT.TabIndex = 4;
            this._4BT.Text = "4";
            this._4BT.Click += new System.EventHandler(this._4BT_Click);
            // 
            // _5BT
            // 
            this._5BT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._5BT.DrawX = false;
            this._5BT.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._5BT.IsButtonTlf = false;
            this._5BT.IsConferencePreprogramada = false;
            this._5BT.Location = new System.Drawing.Point(139, 89);
            this._5BT.Name = "_5BT";
            this._5BT.Permitted = true;
            this._5BT.Size = new System.Drawing.Size(130, 80);
            this._5BT.TabIndex = 5;
            this._5BT.Text = "5";
            this._5BT.Click += new System.EventHandler(this._5BT_Click);
            // 
            // _6BT
            // 
            this._6BT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._6BT.DrawX = false;
            this._6BT.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._6BT.IsButtonTlf = false;
            this._6BT.IsConferencePreprogramada = false;
            this._6BT.Location = new System.Drawing.Point(275, 89);
            this._6BT.Name = "_6BT";
            this._6BT.Permitted = true;
            this._6BT.Size = new System.Drawing.Size(132, 80);
            this._6BT.TabIndex = 6;
            this._6BT.Text = "6";
            this._6BT.Click += new System.EventHandler(this._6BT_Click);
            // 
            // _7BT
            // 
            this._7BT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._7BT.DrawX = false;
            this._7BT.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._7BT.IsButtonTlf = false;
            this._7BT.IsConferencePreprogramada = false;
            this._7BT.Location = new System.Drawing.Point(3, 175);
            this._7BT.Name = "_7BT";
            this._7BT.Permitted = true;
            this._7BT.Size = new System.Drawing.Size(130, 80);
            this._7BT.TabIndex = 7;
            this._7BT.Text = "7";
            this._7BT.Click += new System.EventHandler(this._7BT_Click);
            // 
            // _8BT
            // 
            this._8BT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._8BT.DrawX = false;
            this._8BT.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._8BT.IsButtonTlf = false;
            this._8BT.IsConferencePreprogramada = false;
            this._8BT.Location = new System.Drawing.Point(139, 175);
            this._8BT.Name = "_8BT";
            this._8BT.Permitted = true;
            this._8BT.Size = new System.Drawing.Size(130, 80);
            this._8BT.TabIndex = 8;
            this._8BT.Text = "8";
            this._8BT.Click += new System.EventHandler(this._8BT_Click);
            // 
            // _9BT
            // 
            this._9BT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._9BT.DrawX = false;
            this._9BT.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._9BT.IsButtonTlf = false;
            this._9BT.IsConferencePreprogramada = false;
            this._9BT.Location = new System.Drawing.Point(275, 175);
            this._9BT.Name = "_9BT";
            this._9BT.Permitted = true;
            this._9BT.Size = new System.Drawing.Size(132, 80);
            this._9BT.TabIndex = 9;
            this._9BT.Text = "9";
            this._9BT.Click += new System.EventHandler(this._9BT_Click);
            // 
            // _0BT
            // 
            this._0BT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._0BT.DrawX = false;
            this._0BT.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._0BT.IsButtonTlf = false;
            this._0BT.IsConferencePreprogramada = false;
            this._0BT.Location = new System.Drawing.Point(139, 261);
            this._0BT.Name = "_0BT";
            this._0BT.Permitted = true;
            this._0BT.Size = new System.Drawing.Size(130, 83);
            this._0BT.TabIndex = 11;
            this._0BT.Text = "0";
            this._0BT.Click += new System.EventHandler(this._0BT_Click);
            // 
            // _AlmBT
            // 
            this._AlmBT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._AlmBT.DrawX = false;
            this._AlmBT.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._AlmBT.IsButtonTlf = false;
            this._AlmBT.IsConferencePreprogramada = false;
            this._AlmBT.Location = new System.Drawing.Point(275, 261);
            this._AlmBT.Name = "_AlmBT";
            this._AlmBT.Permitted = true;
            this._AlmBT.Size = new System.Drawing.Size(132, 83);
            this._AlmBT.TabIndex = 12;
            this._AlmBT.Text = "OK";
            this._AlmBT.Click += new System.EventHandler(this._AlmBT_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label2.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.Location = new System.Drawing.Point(3, 23);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(53, 23);
            this.label2.TabIndex = 10;
            this.label2.Text = "Clave";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(3, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(53, 23);
            this.label1.TabIndex = 9;
            this.label1.Text = "Usuario";
            // 
            // txtUsuario
            // 
            this.txtUsuario.Location = new System.Drawing.Point(62, 3);
            this.txtUsuario.Name = "txtUsuario";
            this.txtUsuario.Size = new System.Drawing.Size(258, 20);
            this.txtUsuario.TabIndex = 7;
            // 
            // txtContrasena
            // 
            this.txtContrasena.Location = new System.Drawing.Point(62, 26);
            this.txtContrasena.Name = "txtContrasena";
            this.txtContrasena.PasswordChar = '*';
            this.txtContrasena.Size = new System.Drawing.Size(258, 20);
            this.txtContrasena.TabIndex = 8;
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 3;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 18.20988F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 81.79012F));
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 70F));
            this.tableLayoutPanel1.Controls.Add(this.txtUsuario, 1, 0);
            this.tableLayoutPanel1.Controls.Add(this.txtContrasena, 1, 1);
            this.tableLayoutPanel1.Controls.Add(this.label2, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.label1, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this._ClearBT, 2, 0);
            this.tableLayoutPanel1.Location = new System.Drawing.Point(34, 12);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 2;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(394, 46);
            this.tableLayoutPanel1.TabIndex = 12;
            // 
            // _ClearBT
            // 
            this._ClearBT.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this._ClearBT.DrawX = false;
            this._ClearBT.Font = new System.Drawing.Font("Arial Black", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._ClearBT.IsButtonTlf = false;
            this._ClearBT.IsConferencePreprogramada = false;
            this._ClearBT.Location = new System.Drawing.Point(323, 1);
            this._ClearBT.Margin = new System.Windows.Forms.Padding(0, 1, 3, 3);
            this._ClearBT.Name = "_ClearBT";
            this._ClearBT.Permitted = true;
            this.tableLayoutPanel1.SetRowSpan(this._ClearBT, 2);
            this._ClearBT.Size = new System.Drawing.Size(68, 42);
            this._ClearBT.TabIndex = 16;
            this._ClearBT.Text = "<--";
            this._ClearBT.Click += new System.EventHandler(this._ClearBT_Click);
            // 
            // loginform
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(449, 416);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Controls.Add(_KeypadTLP);
            this.Name = "loginform";
            this.Text = "loginform";
            _KeypadTLP.ResumeLayout(false);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel1.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox txtUsuario;
        private System.Windows.Forms.TextBox txtContrasena;
        private Model.Module.UI.HMIButton _1BT;
        private Model.Module.UI.HMIButton _2BT;
        private Model.Module.UI.HMIButton _3BT;
        private Model.Module.UI.HMIButton _4BT;
        private Model.Module.UI.HMIButton _5BT;
        private Model.Module.UI.HMIButton _6BT;
        private Model.Module.UI.HMIButton _7BT;
        private Model.Module.UI.HMIButton _8BT;
        private Model.Module.UI.HMIButton _9BT;
        private Model.Module.UI.HMIButton _0BT;
        private Model.Module.UI.HMIButton _AlmBT;
        private Model.Module.UI.HMIButton _AstBT;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private Model.Module.UI.HMIButton _ClearBT;
    }
}