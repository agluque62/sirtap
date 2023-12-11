namespace HMI.Model.Module.UI
{
	partial class Keypad
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

		#region Component Designer generated code

		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
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
            _KeypadTLP = new System.Windows.Forms.TableLayoutPanel();
            _DisplayTLP = new System.Windows.Forms.TableLayoutPanel();
            _KeypadTLP.SuspendLayout();
            _DisplayTLP.SuspendLayout();
            this.SuspendLayout();
            // 
            // _KeypadTLP
            // 
            _KeypadTLP.BackColor = System.Drawing.Color.Silver;
            _KeypadTLP.ColumnCount = 3;
            _KeypadTLP.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
            _KeypadTLP.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33.33334F));
            _KeypadTLP.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33.33334F));
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
            _KeypadTLP.Location = new System.Drawing.Point(0, 0);
            _KeypadTLP.Margin = new System.Windows.Forms.Padding(5, 10, 0, 0);
            _KeypadTLP.Name = "_KeypadTLP";
            _KeypadTLP.RowCount = 5;
            _KeypadTLP.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 24F));
            _KeypadTLP.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 19F));
            _KeypadTLP.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 19F));
            _KeypadTLP.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 19F));
            _KeypadTLP.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 19F));
            _KeypadTLP.Size = new System.Drawing.Size(379, 532);
            _KeypadTLP.TabIndex = 1;
            // 
            // _1BT
            // 
            this._1BT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._1BT.DrawX = false;
            this._1BT.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._1BT.IsButtonTlf = false;
            this._1BT.IsConferencePreprogramada = false;
            this._1BT.Location = new System.Drawing.Point(3, 130);
            this._1BT.Name = "_1BT";
            this._1BT.Permitted = true;
            this._1BT.Size = new System.Drawing.Size(120, 95);
            this._1BT.TabIndex = 1;
            this._1BT.Tag = "";
            this._1BT.Text = "1";
            this._1BT.Click += new System.EventHandler(this._BT_Click);
            // 
            // _2BT
            // 
            this._2BT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._2BT.DrawX = false;
            this._2BT.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._2BT.IsButtonTlf = false;
            this._2BT.IsConferencePreprogramada = false;
            this._2BT.Location = new System.Drawing.Point(129, 130);
            this._2BT.Name = "_2BT";
            this._2BT.Permitted = true;
            this._2BT.Size = new System.Drawing.Size(120, 95);
            this._2BT.TabIndex = 2;
            this._2BT.Text = "2";
            this._2BT.Click += new System.EventHandler(this._BT_Click);
            // 
            // _3BT
            // 
            this._3BT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._3BT.DrawX = false;
            this._3BT.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._3BT.IsButtonTlf = false;
            this._3BT.IsConferencePreprogramada = false;
            this._3BT.Location = new System.Drawing.Point(255, 130);
            this._3BT.Name = "_3BT";
            this._3BT.Permitted = true;
            this._3BT.Size = new System.Drawing.Size(121, 95);
            this._3BT.TabIndex = 3;
            this._3BT.Text = "3";
            this._3BT.Click += new System.EventHandler(this._BT_Click);
            // 
            // _4BT
            // 
            this._4BT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._4BT.DrawX = false;
            this._4BT.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._4BT.IsButtonTlf = false;
            this._4BT.IsConferencePreprogramada = false;
            this._4BT.Location = new System.Drawing.Point(3, 231);
            this._4BT.Name = "_4BT";
            this._4BT.Permitted = true;
            this._4BT.Size = new System.Drawing.Size(120, 95);
            this._4BT.TabIndex = 4;
            this._4BT.Text = "4";
            this._4BT.Click += new System.EventHandler(this._BT_Click);
            // 
            // _5BT
            // 
            this._5BT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._5BT.DrawX = false;
            this._5BT.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._5BT.IsButtonTlf = false;
            this._5BT.IsConferencePreprogramada = false;
            this._5BT.Location = new System.Drawing.Point(129, 231);
            this._5BT.Name = "_5BT";
            this._5BT.Permitted = true;
            this._5BT.Size = new System.Drawing.Size(120, 95);
            this._5BT.TabIndex = 5;
            this._5BT.Text = "5";
            this._5BT.Click += new System.EventHandler(this._BT_Click);
            // 
            // _6BT
            // 
            this._6BT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._6BT.DrawX = false;
            this._6BT.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._6BT.IsButtonTlf = false;
            this._6BT.IsConferencePreprogramada = false;
            this._6BT.Location = new System.Drawing.Point(255, 231);
            this._6BT.Name = "_6BT";
            this._6BT.Permitted = true;
            this._6BT.Size = new System.Drawing.Size(121, 95);
            this._6BT.TabIndex = 6;
            this._6BT.Text = "6";
            this._6BT.Click += new System.EventHandler(this._BT_Click);
            // 
            // _7BT
            // 
            this._7BT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._7BT.DrawX = false;
            this._7BT.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._7BT.IsButtonTlf = false;
            this._7BT.IsConferencePreprogramada = false;
            this._7BT.Location = new System.Drawing.Point(3, 332);
            this._7BT.Name = "_7BT";
            this._7BT.Permitted = true;
            this._7BT.Size = new System.Drawing.Size(120, 95);
            this._7BT.TabIndex = 7;
            this._7BT.Text = "7";
            this._7BT.Click += new System.EventHandler(this._BT_Click);
            // 
            // _8BT
            // 
            this._8BT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._8BT.DrawX = false;
            this._8BT.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._8BT.IsButtonTlf = false;
            this._8BT.IsConferencePreprogramada = false;
            this._8BT.Location = new System.Drawing.Point(129, 332);
            this._8BT.Name = "_8BT";
            this._8BT.Permitted = true;
            this._8BT.Size = new System.Drawing.Size(120, 95);
            this._8BT.TabIndex = 8;
            this._8BT.Text = "8";
            this._8BT.Click += new System.EventHandler(this._BT_Click);
            // 
            // _9BT
            // 
            this._9BT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._9BT.DrawX = false;
            this._9BT.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._9BT.IsButtonTlf = false;
            this._9BT.IsConferencePreprogramada = false;
            this._9BT.Location = new System.Drawing.Point(255, 332);
            this._9BT.Name = "_9BT";
            this._9BT.Permitted = true;
            this._9BT.Size = new System.Drawing.Size(121, 95);
            this._9BT.TabIndex = 9;
            this._9BT.Text = "9";
            this._9BT.Click += new System.EventHandler(this._BT_Click);
            // 
            // _AstBT
            // 
            this._AstBT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._AstBT.DrawX = false;
            this._AstBT.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._AstBT.IsButtonTlf = false;
            this._AstBT.IsConferencePreprogramada = false;
            this._AstBT.Location = new System.Drawing.Point(3, 433);
            this._AstBT.Name = "_AstBT";
            this._AstBT.Permitted = true;
            this._AstBT.Size = new System.Drawing.Size(120, 96);
            this._AstBT.TabIndex = 10;
            this._AstBT.Text = "*";
            this._AstBT.Click += new System.EventHandler(this._BT_Click);
            // 
            // _0BT
            // 
            this._0BT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._0BT.DrawX = false;
            this._0BT.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._0BT.IsButtonTlf = false;
            this._0BT.IsConferencePreprogramada = false;
            this._0BT.Location = new System.Drawing.Point(129, 433);
            this._0BT.Name = "_0BT";
            this._0BT.Permitted = true;
            this._0BT.Size = new System.Drawing.Size(120, 96);
            this._0BT.TabIndex = 11;
            this._0BT.Text = "0";
            this._0BT.Click += new System.EventHandler(this._BT_Click);
            // 
            // _AlmBT
            // 
            this._AlmBT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._AlmBT.DrawX = false;
            this._AlmBT.Font = new System.Drawing.Font("Arial Black", 18F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._AlmBT.IsButtonTlf = false;
            this._AlmBT.IsConferencePreprogramada = false;
            this._AlmBT.Location = new System.Drawing.Point(255, 433);
            this._AlmBT.Name = "_AlmBT";
            this._AlmBT.Permitted = true;
            this._AlmBT.Size = new System.Drawing.Size(121, 96);
            this._AlmBT.TabIndex = 12;
            this._AlmBT.Text = "#";
            this._AlmBT.Click += new System.EventHandler(this._BT_Click);
            // 
            // _DisplayTLP
            // 
            _DisplayTLP.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            _DisplayTLP.ColumnCount = 2;
            _KeypadTLP.SetColumnSpan(_DisplayTLP, 3);
            _DisplayTLP.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 70F));
            _DisplayTLP.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 30F));
            _DisplayTLP.Controls.Add(this._ClearBT, 1, 0);
            _DisplayTLP.Controls.Add(this._DisplayTB, 0, 0);
            _DisplayTLP.Controls.Add(this._PauseBt, 1, 1);
            _DisplayTLP.Location = new System.Drawing.Point(0, 0);
            _DisplayTLP.Margin = new System.Windows.Forms.Padding(0);
            _DisplayTLP.Name = "_DisplayTLP";
            _DisplayTLP.RowCount = 2;
            _DisplayTLP.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            _DisplayTLP.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            _DisplayTLP.Size = new System.Drawing.Size(379, 127);
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
            this._ClearBT.Location = new System.Drawing.Point(265, 1);
            this._ClearBT.Margin = new System.Windows.Forms.Padding(0, 1, 3, 3);
            this._ClearBT.Name = "_ClearBT";
            this._ClearBT.Permitted = true;
            this._ClearBT.Size = new System.Drawing.Size(111, 33);
            this._ClearBT.TabIndex = 15;
            this._ClearBT.Text = "<--";
            this._ClearBT.Click += new System.EventHandler(this._ClearBT_Click);
            this._ClearBT.LongClick += new System.EventHandler(this._ClearBT_LongClick);
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
            this._DisplayTB.Size = new System.Drawing.Size(259, 26);
            this._DisplayTB.TabIndex = 14;
            this._DisplayTB.TextAlign = System.Windows.Forms.HorizontalAlignment.Center;
            // 
            // _PauseBt
            // 
            this._PauseBt.DrawX = false;
            this._PauseBt.Font = new System.Drawing.Font("Microsoft Sans Serif", 18.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._PauseBt.IsButtonTlf = false;
            this._PauseBt.IsConferencePreprogramada = false;
            this._PauseBt.Location = new System.Drawing.Point(268, 66);
            this._PauseBt.Name = "_PauseBt";
            this._PauseBt.Permitted = true;
            this._PauseBt.Size = new System.Drawing.Size(42, 32);
            this._PauseBt.TabIndex = 16;
            this._PauseBt.Text = ",";
            this._PauseBt.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this._PauseBt.Click += new System.EventHandler(this._PauseBt_Click);
            // 
            // Keypad
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Inherit;
            this.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.Controls.Add(_KeypadTLP);
            this.Name = "Keypad";
            this.Size = new System.Drawing.Size(379, 532);
            _KeypadTLP.ResumeLayout(false);
            _DisplayTLP.ResumeLayout(false);
            _DisplayTLP.PerformLayout();
            this.ResumeLayout(false);

		}

		#endregion

		private HMIButton _1BT;
		private HMIButton _2BT;
		private HMIButton _3BT;
		private HMIButton _4BT;
		private HMIButton _5BT;
		private HMIButton _6BT;
		private HMIButton _7BT;
		private HMIButton _8BT;
		private HMIButton _9BT;
		private HMIButton _ClearBT;
		private System.Windows.Forms.TextBox _DisplayTB;
        private HMIButton _PauseBt;
        private HMIButton _AstBT;
        private HMIButton _0BT;
        private HMIButton _AlmBT;
    }
}
