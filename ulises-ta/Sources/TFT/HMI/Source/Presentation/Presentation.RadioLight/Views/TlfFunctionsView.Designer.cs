
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

namespace HMI.Presentation.RadioLight.Views
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
            this._TlfFunctionsTLP = new System.Windows.Forms.TableLayoutPanel();
            this._ForwardBT = new HMI.Model.Module.UI.HMIButton();
            this._PickUpBT = new HMI.Model.Module.UI.HMIButton();
            this._MoreBT = new HMI.Model.Module.UI.HMIButton();
            this._TlfSpeakerBT = new HMI.Model.Module.UI.HMIButton();
            this._PriorityBT = new HMI.Model.Module.UI.HMIButton();
            this._ListenBT = new HMI.Model.Module.UI.HMIButton();
            this._HoldBT = new HMI.Model.Module.UI.HMIButton();
            this._TransferBT = new HMI.Model.Module.UI.HMIButton();
            this._TlfViewBT = new HMI.Model.Module.UI.HMIButton();
            this._CancelBT = new HMI.Model.Module.UI.HMIButton();
            this._SlowBlinkTimer = new System.Windows.Forms.Timer(this.components);
            this._FastBlinkTimer = new System.Windows.Forms.Timer(this.components);
            this._TlfFunctionsTLP.SuspendLayout();
            this.SuspendLayout();
            // 
            // _TlfFunctionsTLP
            // 
            this._TlfFunctionsTLP.ColumnCount = 4;
            this._TlfFunctionsTLP.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 28F));
            this._TlfFunctionsTLP.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 28F));
            this._TlfFunctionsTLP.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 22F));
            this._TlfFunctionsTLP.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 22F));
            this._TlfFunctionsTLP.Controls.Add(this._ForwardBT, 0, 2);
            this._TlfFunctionsTLP.Controls.Add(this._PickUpBT, 0, 2);
            this._TlfFunctionsTLP.Controls.Add(this._MoreBT, 1, 0);
            this._TlfFunctionsTLP.Controls.Add(this._TlfSpeakerBT, 3, 1);
            this._TlfFunctionsTLP.Controls.Add(this._PriorityBT, 0, 0);
            this._TlfFunctionsTLP.Controls.Add(this._ListenBT, 0, 2);
            this._TlfFunctionsTLP.Controls.Add(this._HoldBT, 0, 1);
            this._TlfFunctionsTLP.Controls.Add(this._TransferBT, 1, 1);
            this._TlfFunctionsTLP.Controls.Add(this._TlfViewBT, 2, 0);
            this._TlfFunctionsTLP.Controls.Add(this._CancelBT, 3, 0);
            this._TlfFunctionsTLP.Dock = System.Windows.Forms.DockStyle.Fill;
            this._TlfFunctionsTLP.Location = new System.Drawing.Point(0, 0);
            this._TlfFunctionsTLP.Margin = new System.Windows.Forms.Padding(5, 0, 0, 0);
            this._TlfFunctionsTLP.Name = "_TlfFunctionsTLP";
            this._TlfFunctionsTLP.RowCount = 2;
            this._TlfFunctionsTLP.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this._TlfFunctionsTLP.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this._TlfFunctionsTLP.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this._TlfFunctionsTLP.Size = new System.Drawing.Size(429, 129);
            this._TlfFunctionsTLP.TabIndex = 2;
            // 
            // _ForwardBT
            // 
            this._ForwardBT.DataBindings.Add(new System.Windows.Forms.Binding("Visible", global::HMI.Presentation.RadioLight.Properties.Settings.Default, "EnableForward", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this._ForwardBT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._ForwardBT.DrawX = false;
            this._ForwardBT.Enabled = false;
            this._ForwardBT.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._ForwardBT.Location = new System.Drawing.Point(122, 110);
            this._ForwardBT.Margin = new System.Windows.Forms.Padding(2);
            this._ForwardBT.Name = "_ForwardBT";
            this._ForwardBT.Size = new System.Drawing.Size(116, 17);
            this._ForwardBT.TabIndex = 9;
            this._ForwardBT.Text = "Desv�o";
            this._ForwardBT.Visible = false;
            this._ForwardBT.Click += new System.EventHandler(this._ForwardBT_Click);
            // 
            // _PickUpBT
            // 
            this._PickUpBT.DataBindings.Add(new System.Windows.Forms.Binding("Visible", global::HMI.Presentation.RadioLight.Properties.Settings.Default, "EnablePickUp", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this._PickUpBT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._PickUpBT.DrawX = false;
            this._PickUpBT.Enabled = false;
            this._PickUpBT.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._PickUpBT.Location = new System.Drawing.Point(242, 110);
            this._PickUpBT.Margin = new System.Windows.Forms.Padding(2);
            this._PickUpBT.Name = "_PickUpBT";
            this._PickUpBT.Size = new System.Drawing.Size(90, 17);
            this._PickUpBT.TabIndex = 8;
            this._PickUpBT.Text = "Captura";
            this._PickUpBT.Visible = false;
            this._PickUpBT.Click += new System.EventHandler(this._PickUpBT_Click);
            // 
            // _MoreBT
            // 
            this._MoreBT.DataBindings.Add(new System.Windows.Forms.Binding("Visible", global::HMI.Presentation.RadioLight.Properties.Settings.Default, "EnableMore", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this._MoreBT.DrawX = false;
            this._MoreBT.Enabled = false;
            this._MoreBT.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._MoreBT.Location = new System.Drawing.Point(122, 2);
            this._MoreBT.Margin = new System.Windows.Forms.Padding(2);
            this._MoreBT.Name = "_MoreBT";
            this._MoreBT.Size = new System.Drawing.Size(116, 50);
            this._MoreBT.TabIndex = 7;
            this._MoreBT.Text = "M�s";
            this._MoreBT.Click += new System.EventHandler(this._MoreBT_Click);
            this._MoreBT.Visible =global::HMI.Presentation.RadioLight.Properties.Settings.Default.EnableMore;
            // 
            // _TlfSpeakerBT
            // 
            this._TlfSpeakerBT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._TlfSpeakerBT.DrawX = false;
            this._TlfSpeakerBT.Enabled = false;
            this._TlfSpeakerBT.ImageNormal = global::HMI.Presentation.RadioLight.Properties.Resources.SpeakerTlf;
            this._TlfSpeakerBT.Location = new System.Drawing.Point(2, 110);
            this._TlfSpeakerBT.LongClickTout = 5000;
            this._TlfSpeakerBT.Margin = new System.Windows.Forms.Padding(2);
            this._TlfSpeakerBT.Name = "_TlfSpeakerBT";
            this._TlfSpeakerBT.Size = new System.Drawing.Size(116, 17);
            this._TlfSpeakerBT.TabIndex = 6;
            this._TlfSpeakerBT.Visible = false;
            this._TlfSpeakerBT.Click += new System.EventHandler(this._SpeakerTlfBT_Click);
            // 
            // _PriorityBT
            // 
            this._PriorityBT.DataBindings.Add(new System.Windows.Forms.Binding("Visible", global::HMI.Presentation.RadioLight.Properties.Settings.Default, "EnablePriority", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this._PriorityBT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._PriorityBT.DrawX = false;
            this._PriorityBT.Enabled = false;
            this._PriorityBT.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._PriorityBT.Location = new System.Drawing.Point(2, 2);
            this._PriorityBT.Margin = new System.Windows.Forms.Padding(2);
            this._PriorityBT.Name = "_PriorityBT";
            this._PriorityBT.Size = new System.Drawing.Size(116, 50);
            this._PriorityBT.TabIndex = 0;
            this._PriorityBT.Text = "Prioridad";
            this._PriorityBT.Visible = global::HMI.Presentation.RadioLight.Properties.Settings.Default.EnablePriority;
            this._PriorityBT.Click += new System.EventHandler(this._PriorityBT_Click);
            // 
            // _ListenBT
            // 
            this._ListenBT.DataBindings.Add(new System.Windows.Forms.Binding("Visible", global::HMI.Presentation.RadioLight.Properties.Settings.Default, "EnableListen", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this._ListenBT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._ListenBT.DrawX = false;
            this._ListenBT.Enabled = false;
            this._ListenBT.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._ListenBT.Location = new System.Drawing.Point(336, 110);
            this._ListenBT.Margin = new System.Windows.Forms.Padding(2);
            this._ListenBT.Name = "_ListenBT";
            this._ListenBT.Size = new System.Drawing.Size(91, 17);
            this._ListenBT.TabIndex = 1;
            this._ListenBT.Text = "Escucha";
            this._ListenBT.Visible = false;
            this._ListenBT.Click += new System.EventHandler(this._ListenBT_Click);
            // 
            // _HoldBT
            // 
            this._HoldBT.DataBindings.Add(new System.Windows.Forms.Binding("Visible", global::HMI.Presentation.RadioLight.Properties.Settings.Default, "EnableHold", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this._HoldBT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._HoldBT.DrawX = false;
            this._HoldBT.Enabled = false;
            this._HoldBT.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._HoldBT.Location = new System.Drawing.Point(2, 56);
            this._HoldBT.Margin = new System.Windows.Forms.Padding(2);
            this._HoldBT.Name = "_HoldBT";
            this._HoldBT.Size = new System.Drawing.Size(116, 50);
            this._HoldBT.TabIndex = 2;
            this._HoldBT.Text = "Retener";
            this._HoldBT.Visible = global::HMI.Presentation.RadioLight.Properties.Settings.Default.EnableHold;
            this._HoldBT.Click += new System.EventHandler(this._HoldBT_Click);
            // 
            // _TransferBT
            // 
            this._TransferBT.DataBindings.Add(new System.Windows.Forms.Binding("Visible", global::HMI.Presentation.RadioLight.Properties.Settings.Default, "EnableTransfer", true, System.Windows.Forms.DataSourceUpdateMode.OnPropertyChanged));
            this._TransferBT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._TransferBT.DrawX = false;
            this._TransferBT.Enabled = false;
            this._TransferBT.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._TransferBT.Location = new System.Drawing.Point(122, 56);
            this._TransferBT.Margin = new System.Windows.Forms.Padding(2);
            this._TransferBT.Name = "_TransferBT";
            this._TransferBT.Size = new System.Drawing.Size(116, 50);
            this._TransferBT.TabIndex = 3;
            this._TransferBT.Text = "Transferir";
            this._TransferBT.Visible = global::HMI.Presentation.RadioLight.Properties.Settings.Default.EnableTransfer;
            this._TransferBT.Click += new System.EventHandler(this._TransferBT_Click);
            // 
            // _TlfViewBT
            // 
            this._TlfViewBT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._TlfViewBT.DrawX = false;
            this._TlfViewBT.Font = new System.Drawing.Font("Arial Black", 15.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._TlfViewBT.Location = new System.Drawing.Point(243, 7);
            this._TlfViewBT.Margin = new System.Windows.Forms.Padding(3, 7, 3, 7);
            this._TlfViewBT.Name = "_TlfViewBT";
            this._TlfFunctionsTLP.SetRowSpan(this._TlfViewBT, 2);
            this._TlfViewBT.Size = new System.Drawing.Size(88, 94);
            this._TlfViewBT.TabIndex = 4;
            this._TlfViewBT.Text = "AI";
            this._TlfViewBT.Click += new System.EventHandler(this._TlfViewBT_Click);
            // 
            // _CancelBT
            // 
            this._CancelBT.Dock = System.Windows.Forms.DockStyle.Fill;
            this._CancelBT.DrawX = false;
            this._CancelBT.Enabled = false;
            this._CancelBT.ImageNormal = global::HMI.Presentation.RadioLight.Properties.Resources.Anular;
            this._CancelBT.Location = new System.Drawing.Point(337, 7);
            this._CancelBT.LongClickTout = 5000;
            this._CancelBT.Margin = new System.Windows.Forms.Padding(3, 7, 3, 7);
            this._CancelBT.Name = "_CancelBT";
            this._TlfFunctionsTLP.SetRowSpan(this._CancelBT, 2);
            this._CancelBT.Size = new System.Drawing.Size(89, 94);
            this._CancelBT.TabIndex = 5;
            this._CancelBT.Click += new System.EventHandler(this._CancelBT_Click);
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
            this.Controls.Add(this._TlfFunctionsTLP);
            this.Name = "TlfFunctionsView";
            this.Size = new System.Drawing.Size(429, 129);
            this._TlfFunctionsTLP.ResumeLayout(false);
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
        private Model.Module.UI.HMIButton _TlfSpeakerBT;
        private System.Windows.Forms.TableLayoutPanel _TlfFunctionsTLP;
        private HMI.Model.Module.UI.HMIButton _MoreBT;
        private Model.Module.UI.HMIButton _PickUpBT;
        private Model.Module.UI.HMIButton _ForwardBT;
    }
}

