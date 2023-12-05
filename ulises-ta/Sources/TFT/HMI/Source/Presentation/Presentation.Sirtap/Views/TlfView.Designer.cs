
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
    partial class TlfView
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
            System.Windows.Forms.TableLayoutPanel _TlfTLP;
            this._TlfNumbersWS = new Microsoft.Practices.CompositeUI.WinForms.DeckWorkspace();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this._TlfHeadPhonesUDB = new HMI.Presentation.Sirtap.UI.UpDownButton();
            this._TlfPageBT = new HMI.Presentation.Sirtap.UI.RdPageButton();
            this._TlfFunctionsWS = new Microsoft.Practices.CompositeUI.WinForms.DeckWorkspace();
            _TlfTLP = new System.Windows.Forms.TableLayoutPanel();
            _TlfTLP.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // _TlfTLP
            // 
            _TlfTLP.ColumnCount = 3;
            _TlfTLP.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            _TlfTLP.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle());
            _TlfTLP.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 200F));
            _TlfTLP.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            _TlfTLP.Controls.Add(this._TlfNumbersWS, 0, 0);
            _TlfTLP.Controls.Add(this.tableLayoutPanel1, 2, 0);
            _TlfTLP.Controls.Add(this._TlfFunctionsWS, 0, 1);
            _TlfTLP.Dock = System.Windows.Forms.DockStyle.Fill;
            _TlfTLP.Location = new System.Drawing.Point(0, 0);
            _TlfTLP.Margin = new System.Windows.Forms.Padding(0);
            _TlfTLP.Name = "_TlfTLP";
            _TlfTLP.RowCount = 2;
            _TlfTLP.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
            _TlfTLP.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 104F));
            _TlfTLP.Size = new System.Drawing.Size(639, 519);
            _TlfTLP.TabIndex = 5;
            _TlfTLP.Paint += new System.Windows.Forms.PaintEventHandler(this._TlfTLP_Paint);
            // 
            // _TlfNumbersWS
            // 
            this._TlfNumbersWS.Dock = System.Windows.Forms.DockStyle.Fill;
            this._TlfNumbersWS.Location = new System.Drawing.Point(0, 0);
            this._TlfNumbersWS.Margin = new System.Windows.Forms.Padding(0);
            this._TlfNumbersWS.Name = "_TlfNumbersWS";
            this._TlfNumbersWS.Size = new System.Drawing.Size(487, 415);
            this._TlfNumbersWS.TabIndex = 3;
            this._TlfNumbersWS.Text = "deckWorkspace3";
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel1.Controls.Add(this._TlfHeadPhonesUDB, 0, 2);
            this.tableLayoutPanel1.Controls.Add(this._TlfPageBT, 0, 1);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Left;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(490, 3);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 3;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 145F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 107F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(146, 409);
            this.tableLayoutPanel1.TabIndex = 5;
            // 
            // _TlfHeadPhonesUDB
            // 
            this._TlfHeadPhonesUDB.BackColor = System.Drawing.Color.Transparent;
            this._TlfHeadPhonesUDB.DownImage = global::HMI.Presentation.Sirtap.Properties.Resources.HeadPhonesDown;
            this._TlfHeadPhonesUDB.DrawX = false;
            this._TlfHeadPhonesUDB.Enabled = false;
            this._TlfHeadPhonesUDB.Location = new System.Drawing.Point(3, 168);
            this._TlfHeadPhonesUDB.Name = "_TlfHeadPhonesUDB";
            this._TlfHeadPhonesUDB.Size = new System.Drawing.Size(85, 98);
            this._TlfHeadPhonesUDB.TabIndex = 9;
            this._TlfHeadPhonesUDB.UpImage = global::HMI.Presentation.Sirtap.Properties.Resources.HeadPhonesUp;
            // 
            // _TlfPageBT
            // 
            this._TlfPageBT.CornerRadius = 4;
            this._TlfPageBT.DownDisabledImage = global::HMI.Presentation.Sirtap.Properties.Resources.RdPageDownDisabled;
            this._TlfPageBT.DownEnabledImage = global::HMI.Presentation.Sirtap.Properties.Resources.RdPageDown;
            this._TlfPageBT.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this._TlfPageBT.Location = new System.Drawing.Point(3, 23);
            this._TlfPageBT.Name = "_TlfPageBT";
            this._TlfPageBT.Size = new System.Drawing.Size(85, 139);
            this._TlfPageBT.TabIndex = 8;
            this._TlfPageBT.UpDisabledImage = global::HMI.Presentation.Sirtap.Properties.Resources.RdPageUpDisabled;
            this._TlfPageBT.UpEnabledImage = global::HMI.Presentation.Sirtap.Properties.Resources.RdPageUp;
            this._TlfPageBT.UpClick += new Utilities.GenericEventHandler(this._TlfPageBT_UpClick);
            this._TlfPageBT.DownClick += new Utilities.GenericEventHandler(this._TlfPageBT_DownClick);
            // 
            // _TlfFunctionsWS
            // 
            this._TlfFunctionsWS.Location = new System.Drawing.Point(0, 415);
            this._TlfFunctionsWS.Margin = new System.Windows.Forms.Padding(0);
            this._TlfFunctionsWS.Name = "_TlfFunctionsWS";
            this._TlfFunctionsWS.Size = new System.Drawing.Size(487, 104);
            this._TlfFunctionsWS.TabIndex = 4;
            this._TlfFunctionsWS.Text = "deckWorkspace3";
            // 
            // TlfView
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Inherit;
            this.Controls.Add(_TlfTLP);
            this.Margin = new System.Windows.Forms.Padding(0);
            this.Name = "TlfView";
            this.Size = new System.Drawing.Size(639, 519);
            this.BackColorChanged += new System.EventHandler(this.TlfView_BackColorChanged);
            _TlfTLP.ResumeLayout(false);
            this.tableLayoutPanel1.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private Microsoft.Practices.CompositeUI.WinForms.DeckWorkspace _TlfFunctionsWS;
        private Microsoft.Practices.CompositeUI.WinForms.DeckWorkspace _TlfNumbersWS;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private UI.RdPageButton _TlfPageBT;
        private UI.UpDownButton _TlfHeadPhonesUDB;
    }
}

