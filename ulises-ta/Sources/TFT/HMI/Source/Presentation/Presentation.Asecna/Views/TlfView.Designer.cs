
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
            this._TlfFunctionsWS = new Microsoft.Practices.CompositeUI.WinForms.DeckWorkspace();
            this._TlfNumbersWS = new Microsoft.Practices.CompositeUI.WinForms.DeckWorkspace();
            _TlfTLP = new System.Windows.Forms.TableLayoutPanel();
            _TlfTLP.SuspendLayout();
            this.SuspendLayout();
            // 
            // _TlfTLP
            // 
            _TlfTLP.ColumnCount = 1;
            _TlfTLP.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            _TlfTLP.Controls.Add(this._TlfFunctionsWS, 0, 1);
            _TlfTLP.Controls.Add(this._TlfNumbersWS, 0, 0);
            _TlfTLP.Dock = System.Windows.Forms.DockStyle.Fill;
            _TlfTLP.Location = new System.Drawing.Point(0, 0);
            _TlfTLP.Margin = new System.Windows.Forms.Padding(0);
            _TlfTLP.Name = "_TlfTLP";
            _TlfTLP.RowCount = 2;
            _TlfTLP.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 77F));
            _TlfTLP.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 23F));
            _TlfTLP.Size = new System.Drawing.Size(440, 450);
            _TlfTLP.TabIndex = 5;
            // 
            // _TlfFunctionsWS
            // 
            this._TlfFunctionsWS.Dock = System.Windows.Forms.DockStyle.Fill;
            this._TlfFunctionsWS.Location = new System.Drawing.Point(0, 346);
            this._TlfFunctionsWS.Margin = new System.Windows.Forms.Padding(0);
            this._TlfFunctionsWS.Name = "_TlfFunctionsWS";
            this._TlfFunctionsWS.Size = new System.Drawing.Size(440, 104);
            this._TlfFunctionsWS.TabIndex = 4;
            this._TlfFunctionsWS.Text = "deckWorkspace3";
            // 
            // _TlfNumbersWS
            // 
            this._TlfNumbersWS.Dock = System.Windows.Forms.DockStyle.Fill;
            this._TlfNumbersWS.Location = new System.Drawing.Point(0, 0);
            this._TlfNumbersWS.Margin = new System.Windows.Forms.Padding(0);
            this._TlfNumbersWS.Name = "_TlfNumbersWS";
            this._TlfNumbersWS.Size = new System.Drawing.Size(440, 346);
            this._TlfNumbersWS.TabIndex = 3;
            this._TlfNumbersWS.Text = "deckWorkspace3";
            // 
            // TlfView
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Inherit;
            this.Controls.Add(_TlfTLP);
            this.Margin = new System.Windows.Forms.Padding(0);
            this.Name = "TlfView";
            this.Size = new System.Drawing.Size(440, 450);
            this.BackColorChanged += new System.EventHandler(this.TlfView_BackColorChanged);
            _TlfTLP.ResumeLayout(false);
            this.ResumeLayout(false);

		}

		#endregion

		private Microsoft.Practices.CompositeUI.WinForms.DeckWorkspace _TlfFunctionsWS;
		private Microsoft.Practices.CompositeUI.WinForms.DeckWorkspace _TlfNumbersWS;
	}
}

