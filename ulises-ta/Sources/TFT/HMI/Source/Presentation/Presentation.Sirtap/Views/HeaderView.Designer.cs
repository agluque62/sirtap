
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

namespace HMI.Presentation.Sirtap.Views
{
    partial class HeaderView
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
            this._InfoBT = new HMI.Model.Module.UI.HMIButton();
            this._TitleBT = new HMI.Model.Module.UI.HMIButton();
            this._MsgLB = new System.Windows.Forms.TextBox();
            this._SplitUC = new HMI.Presentation.Sirtap.UI.SplitUC();
            this._BrightnessUDB = new HMI.Presentation.Sirtap.UI.UpDownButton();
            this._BuzzerUDB = new HMI.Presentation.Sirtap.UI.UpDownButton();
            this.MisionText = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // _InfoBT
            // 
            this._InfoBT.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this._InfoBT.DrawX = false;
            this._InfoBT.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._InfoBT.IsButtonTlf = false;
            this._InfoBT.IsConferencePreprogramada = false;
            this._InfoBT.Location = new System.Drawing.Point(275, 17);
            this._InfoBT.Name = "_InfoBT";
            this._InfoBT.Permitted = true;
            this._InfoBT.Size = new System.Drawing.Size(54, 54);
            this._InfoBT.TabIndex = 2;
            this._InfoBT.Text = "INFO";
            this._InfoBT.Visible = false;
            this._InfoBT.Click += new System.EventHandler(this._InfoBT_Click);
            // 
            // _TitleBT
            // 
            this._TitleBT.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this._TitleBT.DrawX = false;
            this._TitleBT.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            //this._TitleBT.ImageNormal = global::HMI.Presentation.Sirtap.Properties.Resources.Aena;
            this._TitleBT.IsButtonTlf = false;
            this._TitleBT.IsConferencePreprogramada = false;
            this._TitleBT.Location = new System.Drawing.Point(10, 3);
            this._TitleBT.Name = "_TitleBT";
            this._TitleBT.Permitted = true;
            this._TitleBT.Size = new System.Drawing.Size(218, 80);
            this._TitleBT.TabIndex = 4;
            this._TitleBT.Text = "PuestoPuesto0123";
            this._TitleBT.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this._TitleBT.Click += new System.EventHandler(this._TitleBT_Click);
            // 
            // _MsgLB
            // 
            this._MsgLB.AcceptsReturn = true;
            this._MsgLB.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._MsgLB.Location = new System.Drawing.Point(234, 4);
            this._MsgLB.MaxLength = 8276;
            this._MsgLB.Multiline = true;
            this._MsgLB.Name = "_MsgLB";
            this._MsgLB.ReadOnly = true;
            this._MsgLB.Size = new System.Drawing.Size(78, 66);
            this._MsgLB.TabIndex = 5;
            this._MsgLB.Visible = false;
            // 
            // _SplitUC
            // 
            this._SplitUC.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this._SplitUC.BackColor = System.Drawing.Color.White;
            this._SplitUC.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this._SplitUC.JackOff = global::HMI.Presentation.Sirtap.Properties.Resources.JackOffTwr;
            this._SplitUC.JackOn = global::HMI.Presentation.Sirtap.Properties.Resources.JackOnTwr;
            this._SplitUC.Location = new System.Drawing.Point(196, 4);
            this._SplitUC.Margin = new System.Windows.Forms.Padding(5);
            this._SplitUC.Name = "_SplitUC";
            this._SplitUC.Size = new System.Drawing.Size(147, 77);
            this._SplitUC.TabIndex = 3;
            this._SplitUC.Visible = false;
            this._SplitUC.SplitSelectionClick += new System.EventHandler(this._SplitUC_SplitSelectionClick);
            this._SplitUC.SplitModeChanging += new Utilities.GenericEventHandler<HMI.Model.Module.BusinessEntities.SplitMode>(this._SplitUC_SplitModeChanging);
            // 
            // _BrightnessUDB
            // 
            this._BrightnessUDB.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this._BrightnessUDB.BackColor = System.Drawing.Color.Transparent;
            this._BrightnessUDB.DownImage = global::HMI.Presentation.Sirtap.Properties.Resources.BrightnessDown;
            this._BrightnessUDB.DrawX = false;
            this._BrightnessUDB.Location = new System.Drawing.Point(587, 3);
            this._BrightnessUDB.Name = "_BrightnessUDB";
            this._BrightnessUDB.Size = new System.Drawing.Size(85, 80);
            this._BrightnessUDB.TabIndex = 0;
            this._BrightnessUDB.UpImage = global::HMI.Presentation.Sirtap.Properties.Resources.BrightnessUp;
            this._BrightnessUDB.LevelDown += new System.EventHandler(this._BrightnessUDB_LevelDown);
            this._BrightnessUDB.LevelUp += new System.EventHandler(this._BrightnessUDB_LevelUp);
            // 
            // _BuzzerUDB
            // 
            this._BuzzerUDB.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this._BuzzerUDB.BackColor = System.Drawing.Color.Transparent;
            this._BuzzerUDB.DownImage = global::HMI.Presentation.Sirtap.Properties.Resources.BuzzerDown;
            this._BuzzerUDB.DrawX = false;
            this._BuzzerUDB.Location = new System.Drawing.Point(678, 1);
            this._BuzzerUDB.Name = "_BuzzerUDB";
            this._BuzzerUDB.Size = new System.Drawing.Size(85, 80);
            this._BuzzerUDB.TabIndex = 1;
            this._BuzzerUDB.UpImage = global::HMI.Presentation.Sirtap.Properties.Resources.BuzzerUp;
            this._BuzzerUDB.LevelDown += new System.EventHandler(this._BuzzerUDB_LevelDown);
            this._BuzzerUDB.LevelUp += new System.EventHandler(this._BuzzerUDB_LevelUp);
            this._BuzzerUDB.LongClick += new System.EventHandler(this._BuzzerUDB_LongClick);
            // 
            // MisionText
            // 
            this.MisionText.Location = new System.Drawing.Point(10, 67);
            this.MisionText.Name = "MisionText";
            this.MisionText.Size = new System.Drawing.Size(100, 20);
            this.MisionText.TabIndex = 6;
            // 
            // HeaderView
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Inherit;
            this.BackColor = System.Drawing.Color.Transparent;
            this.Controls.Add(this.MisionText);
            this.Controls.Add(this._MsgLB);
            this.Controls.Add(this._SplitUC);
            this.Controls.Add(this._BrightnessUDB);
            this.Controls.Add(this._BuzzerUDB);
            this.Controls.Add(this._InfoBT);
            this.Controls.Add(this._TitleBT);
            this.Name = "HeaderView";
            this.Size = new System.Drawing.Size(797, 87);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private HMI.Model.Module.UI.HMIButton _TitleBT;
        private HMI.Model.Module.UI.HMIButton _InfoBT;
        private HMI.Presentation.Sirtap.UI.UpDownButton _BrightnessUDB;
        private HMI.Presentation.Sirtap.UI.UpDownButton _BuzzerUDB;
        private HMI.Presentation.Sirtap.UI.SplitUC _SplitUC;
        private System.Windows.Forms.TextBox _MsgLB;
        private System.Windows.Forms.TextBox MisionText;
    }
}

