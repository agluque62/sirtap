namespace HMI.Presentation.Sirtap.Layout
{
    partial class ShellLayoutView
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
            this._MainTLP = new System.Windows.Forms.TableLayoutPanel();
            this._MainToolsWS = new Microsoft.Practices.CompositeUI.WinForms.DeckWorkspace();
            this._TlfWS = new Microsoft.Practices.CompositeUI.WinForms.DeckWorkspace();
            this._RdWS = new Microsoft.Practices.CompositeUI.WinForms.DeckWorkspace();
            this._LcWS = new Microsoft.Practices.CompositeUI.WinForms.DeckWorkspace();
            this._MainTLP.SuspendLayout();
            this.SuspendLayout();
            // 
            // _MainTLP
            // 
            this._MainTLP.CellBorderStyle = System.Windows.Forms.TableLayoutPanelCellBorderStyle.Single;
            this._MainTLP.ColumnCount = 2;
            this._MainTLP.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this._MainTLP.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 121F));
            this._MainTLP.Controls.Add(this._MainToolsWS, 0, 0);
            this._MainTLP.Controls.Add(this._TlfWS, 0, 2);
            this._MainTLP.Controls.Add(this._RdWS, 0, 1);
            this._MainTLP.Controls.Add(this._LcWS, 0, 3);
            this._MainTLP.Dock = System.Windows.Forms.DockStyle.Fill;
            this._MainTLP.Location = new System.Drawing.Point(0, 0);
            this._MainTLP.Margin = new System.Windows.Forms.Padding(0);
            this._MainTLP.Name = "_MainTLP";
            this._MainTLP.RowCount = 4;
            this._MainTLP.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 7.811587F));
            this._MainTLP.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 38.30475F));
            this._MainTLP.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 41.78699F));
            this._MainTLP.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 12.09667F));
            this._MainTLP.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 20F));
            //this._MainTLP.Size = new System.Drawing.Size(600, 800);
            this._MainTLP.Size = new System.Drawing.Size(800, 1280);//240221
            this._MainTLP.TabIndex = 0;
            this._MainTLP.Paint += new System.Windows.Forms.PaintEventHandler(this._MainTLP_Paint);
            // 
            // _MainToolsWS
            // 
            this._MainTLP.SetColumnSpan(this._MainToolsWS, 2);
            this._MainToolsWS.Dock = System.Windows.Forms.DockStyle.Fill;
            this._MainToolsWS.Location = new System.Drawing.Point(1, 1);
            this._MainToolsWS.Margin = new System.Windows.Forms.Padding(0);
            this._MainToolsWS.Name = "_MainToolsWS";
            //this._MainToolsWS.Size = new System.Drawing.Size(598, 62);
            this._MainToolsWS.Size = new System.Drawing.Size(800-2, 62);//240221
            this._MainToolsWS.TabIndex = 0;
            this._MainToolsWS.Text = "deckWorkspace1";
            // 
            // _TlfWS
            // 
            this._MainTLP.SetColumnSpan(this._TlfWS, 2);
            this._TlfWS.Dock = System.Windows.Forms.DockStyle.Fill;
            this._TlfWS.Location = new System.Drawing.Point(1, 369);
            this._TlfWS.Margin = new System.Windows.Forms.Padding(0);
            this._TlfWS.Name = "_TlfWS";
            //this._TlfWS.Size = new System.Drawing.Size(598, 332);
            this._TlfWS.Size = new System.Drawing.Size(800-2, 332);//240221
            this._TlfWS.TabIndex = 2;
            this._TlfWS.Text = "deckWorkspace3";
            this._TlfWS.Click += new System.EventHandler(this._TlfWS_Click);
            // 
            // _RdWS
            // 
            this._MainTLP.SetColumnSpan(this._RdWS, 2);
            this._RdWS.Dock = System.Windows.Forms.DockStyle.Fill;
            this._RdWS.Location = new System.Drawing.Point(1, 64);
            this._RdWS.Margin = new System.Windows.Forms.Padding(0);
            this._RdWS.Name = "_RdWS";
            //this._RdWS.Size = new System.Drawing.Size(598, 304);
            this._RdWS.Size = new System.Drawing.Size(800-2, 304);//240221
            this._RdWS.TabIndex = 1;
            this._RdWS.Text = "deckWorkspace2";
            // 
            // _LcWS
            // 
            this._MainTLP.SetColumnSpan(this._LcWS, 2);
            this._LcWS.Dock = System.Windows.Forms.DockStyle.Fill;
            this._LcWS.Location = new System.Drawing.Point(1, 702);
            this._LcWS.Margin = new System.Windows.Forms.Padding(0);
            this._LcWS.Name = "_LcWS";
            //this._LcWS.Size = new System.Drawing.Size(598, 97);
            this._LcWS.Size = new System.Drawing.Size(800-2, 97);//240221
            this._LcWS.TabIndex = 3;
            this._LcWS.Text = "deckWorkspace4";
            // 
            // ShellLayoutView
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Inherit;
            this.BackColor = System.Drawing.SystemColors.Window;
            this.Controls.Add(this._MainTLP);
            this.Name = "ShellLayoutView";
            //this.Size = new System.Drawing.Size(600, 800);
            this.Size = new System.Drawing.Size(800,1280);//240225
            this._MainTLP.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private Microsoft.Practices.CompositeUI.WinForms.DeckWorkspace _MainToolsWS;
        private Microsoft.Practices.CompositeUI.WinForms.DeckWorkspace _RdWS;
        private Microsoft.Practices.CompositeUI.WinForms.DeckWorkspace _LcWS;
        private Microsoft.Practices.CompositeUI.WinForms.DeckWorkspace _TlfWS;
        private System.Windows.Forms.TableLayoutPanel _MainTLP;
    }
}

