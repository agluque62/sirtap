
//----------------------------------------------------------------------------------------
// patterns & practices - Smart Client Software Factory - Guidance Package
//
// This file was generated by the "Add View" recipe.
//
// For more information see: 
// ms-help://MS.VSCC.v80/MS.VSIPCC.v80/ms.practices.scsf.2007may/SCSF/html/02-09-010-ModelViewPresenter_MVP.htm
//
// Latest version of this Guidance Package: http://go.microsoft.com/fwlink/?LinkId=62182
//----------------------------------------------------------------------------------------

namespace HMI.Presentation.Asecna.Views
{
	partial class TlfFunctionsView
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
			if (disposing)
			{
				if (components != null)
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
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.TableLayoutPanel _TlfFunctionsTLP;
            this._PriorityBT = new HMI.Model.Module.UI.HMIButton();
            this._ListenBT = new HMI.Model.Module.UI.HMIButton();
            this._HoldBT = new HMI.Model.Module.UI.HMIButton();
            this._TransferBT = new HMI.Model.Module.UI.HMIButton();
            this._TlfViewBT = new HMI.Model.Module.UI.HMIButton();
            this._CancelBT = new HMI.Model.Module.UI.HMIButton();
            this._ConfBT = new HMI.Model.Module.UI.HMIButton();
            this._ReplayBT = new HMI.Model.Module.UI.HMIButton();
            this._SlowBlinkTimer = new System.Windows.Forms.Timer(this.components);
            this._FastBlinkTimer = new System.Windows.Forms.Timer(this.components);
            _TlfFunctionsTLP = new System.Windows.Forms.TableLayoutPanel();
            _TlfFunctionsTLP.SuspendLayout();
            this.SuspendLayout();
            // 
            // _TlfFunctionsTLP
            // 
            _TlfFunctionsTLP.ColumnCount = 4;
            _TlfFunctionsTLP.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
            _TlfFunctionsTLP.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
            _TlfFunctionsTLP.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
            _TlfFunctionsTLP.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 25F));
            _TlfFunctionsTLP.Controls.Add(this._PriorityBT, 0, 0);
            _TlfFunctionsTLP.Controls.Add(this._ListenBT, 1, 0);
            _TlfFunctionsTLP.Controls.Add(this._HoldBT, 0, 1);
            _TlfFunctionsTLP.Controls.Add(this._TransferBT, 1, 1);
            _TlfFunctionsTLP.Controls.Add(this._TlfViewBT, 3, 0);
            _TlfFunctionsTLP.Controls.Add(this._CancelBT, 3, 1);
            _TlfFunctionsTLP.Controls.Add(this._ConfBT, 2, 0);
            _TlfFunctionsTLP.Controls.Add(this._ReplayBT, 2, 1);
            _TlfFunctionsTLP.Dock = System.Windows.Forms.DockStyle.Fill;
            _TlfFunctionsTLP.Location = new System.Drawing.Point(0, 0);
            _TlfFunctionsTLP.Margin = new System.Windows.Forms.Padding(5, 0, 0, 0);
            _TlfFunctionsTLP.Name = "_TlfFunctionsTLP";
            _TlfFunctionsTLP.RowCount = 2;
            _TlfFunctionsTLP.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            _TlfFunctionsTLP.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            _TlfFunctionsTLP.Size = new System.Drawing.Size(429, 85);
            _TlfFunctionsTLP.TabIndex = 2;
            // 
            // _PriorityBT
            // 
            this._PriorityBT.DataBindings.Add(new System.Windows.Forms.Binding("Visible", global::HMI.Presentation.Asecna.Properties.Settings.Default, "EnablePriority", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this._PriorityBT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._PriorityBT.Enabled = false;
            this._PriorityBT.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._PriorityBT.Location = new System.Drawing.Point(2, 2);
            this._PriorityBT.Margin = new System.Windows.Forms.Padding(2);
            this._PriorityBT.Name = "_PriorityBT";
            this._PriorityBT.Size = new System.Drawing.Size(103, 38);
            this._PriorityBT.TabIndex = 0;
            this._PriorityBT.Text = "Prioridad";
            this._PriorityBT.Visible = global::HMI.Presentation.Asecna.Properties.Settings.Default.EnablePriority;
            this._PriorityBT.Click += new System.EventHandler(this._PriorityBT_Click);
            // 
            // _ListenBT
            // 
            this._ListenBT.DataBindings.Add(new System.Windows.Forms.Binding("Visible", global::HMI.Presentation.Asecna.Properties.Settings.Default, "EnableListen", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this._ListenBT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._ListenBT.Enabled = false;
            this._ListenBT.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._ListenBT.Location = new System.Drawing.Point(109, 2);
            this._ListenBT.Margin = new System.Windows.Forms.Padding(2);
            this._ListenBT.Name = "_ListenBT";
            this._ListenBT.Size = new System.Drawing.Size(103, 38);
            this._ListenBT.TabIndex = 1;
            this._ListenBT.Text = "Escucha";
            this._ListenBT.Visible = global::HMI.Presentation.Asecna.Properties.Settings.Default.EnableListen;
            this._ListenBT.Click += new System.EventHandler(this._ListenBT_Click);
            // 
            // _HoldBT
            // 
            this._HoldBT.DataBindings.Add(new System.Windows.Forms.Binding("Visible", global::HMI.Presentation.Asecna.Properties.Settings.Default, "EnableHold", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this._HoldBT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._HoldBT.Enabled = false;
            this._HoldBT.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._HoldBT.Location = new System.Drawing.Point(2, 44);
            this._HoldBT.Margin = new System.Windows.Forms.Padding(2);
            this._HoldBT.Name = "_HoldBT";
            this._HoldBT.Size = new System.Drawing.Size(103, 39);
            this._HoldBT.TabIndex = 2;
            this._HoldBT.Text = "Retener";
            this._HoldBT.Visible = global::HMI.Presentation.Asecna.Properties.Settings.Default.EnableHold;
            this._HoldBT.Click += new System.EventHandler(this._HoldBT_Click);
            // 
            // _TransferBT
            // 
            this._TransferBT.DataBindings.Add(new System.Windows.Forms.Binding("Visible", global::HMI.Presentation.Asecna.Properties.Settings.Default, "EnableTransfer", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this._TransferBT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._TransferBT.Enabled = false;
            this._TransferBT.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._TransferBT.Location = new System.Drawing.Point(109, 44);
            this._TransferBT.Margin = new System.Windows.Forms.Padding(2);
            this._TransferBT.Name = "_TransferBT";
            this._TransferBT.Size = new System.Drawing.Size(103, 39);
            this._TransferBT.TabIndex = 3;
            this._TransferBT.Text = "Transferir";
            this._TransferBT.Visible = global::HMI.Presentation.Asecna.Properties.Settings.Default.EnableTransfer;
            this._TransferBT.Click += new System.EventHandler(this._TransferBT_Click);
            // 
            // _TlfViewBT
            // 
            this._TlfViewBT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._TlfViewBT.Font = new System.Drawing.Font("Arial Black", 15.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._TlfViewBT.Location = new System.Drawing.Point(324, 3);
            this._TlfViewBT.Name = "_TlfViewBT";
            this._TlfViewBT.Size = new System.Drawing.Size(102, 36);
            this._TlfViewBT.TabIndex = 4;
            this._TlfViewBT.Text = "AI";
            this._TlfViewBT.Click += new System.EventHandler(this._TlfViewBT_Click);
            // 
            // _CancelBT
            // 
            this._CancelBT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._CancelBT.Enabled = false;
            this._CancelBT.ImageNormal = global::HMI.Presentation.Asecna.Properties.Resources.Anular;
            this._CancelBT.Location = new System.Drawing.Point(324, 45);
            this._CancelBT.LongClickTout = 5000;
            this._CancelBT.Name = "_CancelBT";
            this._CancelBT.Size = new System.Drawing.Size(102, 37);
            this._CancelBT.TabIndex = 5;
            this._CancelBT.Click += new System.EventHandler(this._CancelBT_Click);
            // 
            // _ConfBT
            // 
            this._ConfBT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._ConfBT.Enabled = false;
            this._ConfBT.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F, System.Drawing.FontStyle.Bold);
            this._ConfBT.Location = new System.Drawing.Point(216, 2);
            this._ConfBT.Margin = new System.Windows.Forms.Padding(2);
            this._ConfBT.Name = "_ConfBT";
            this._ConfBT.Size = new System.Drawing.Size(103, 38);
            this._ConfBT.TabIndex = 6;
            this._ConfBT.Text = "Conferencia";
            this._ConfBT.Click += new System.EventHandler(this._ConfBT_Click);
            // 
            // _ReplayBT
            // 
            this._ReplayBT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._ReplayBT.Enabled = false;
            this._ReplayBT.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Bold);
            this._ReplayBT.Location = new System.Drawing.Point(216, 44);
            this._ReplayBT.Margin = new System.Windows.Forms.Padding(2);
            this._ReplayBT.Name = "_ReplayBT";
            this._ReplayBT.Size = new System.Drawing.Size(103, 39);
            this._ReplayBT.TabIndex = 7;
            this._ReplayBT.Text = "Replay";
            this._ReplayBT.Click += new System.EventHandler(this._ReplaylBT_Click);
            // 
            // _SlowBlinkTimer
            // 
            this._SlowBlinkTimer.Interval = 500;
            this._SlowBlinkTimer.Tick += new System.EventHandler(this._SlowBlinkTimer_Tick);
            // 
            // _FastBlinkTimer
            // 
            this._FastBlinkTimer.Interval = 250;
            this._FastBlinkTimer.Tick += new System.EventHandler(this._FastBlinkTimer_Tick);
            // 
            // TlfFunctionsView
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(_TlfFunctionsTLP);
            this.Name = "TlfFunctionsView";
            this.Size = new System.Drawing.Size(429, 85);
            _TlfFunctionsTLP.ResumeLayout(false);
            this.ResumeLayout(false);

		}

		#endregion

		private HMI.Model.Module.UI.HMIButton _PriorityBT;
		private HMI.Model.Module.UI.HMIButton _ListenBT;
		private HMI.Model.Module.UI.HMIButton _HoldBT;
		private HMI.Model.Module.UI.HMIButton _TransferBT;
		private HMI.Model.Module.UI.HMIButton _TlfViewBT;
		private HMI.Model.Module.UI.HMIButton _CancelBT;
		private System.Windows.Forms.Timer _SlowBlinkTimer;
		private System.Windows.Forms.Timer _FastBlinkTimer;
        private Model.Module.UI.HMIButton _ConfBT;
        private Model.Module.UI.HMIButton _ReplayBT;
       

        public void setHoldNuevo(HMI.Model.Module.UI.HMIButton button)
        {
            this._HoldBT = button;
         }

        public HMI.Model.Module.UI.HMIButton getHoldNuevo()
        {
            return this._HoldBT;
        }

    }

}

