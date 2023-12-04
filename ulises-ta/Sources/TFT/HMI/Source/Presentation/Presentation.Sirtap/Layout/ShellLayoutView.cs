using HMI.Model.Module.Constants;
using HMI.Presentation.Sirtap.Properties;
using Microsoft.Practices.ObjectBuilder;
using System;
using System.Windows.Forms;

namespace HMI.Presentation.Sirtap.Layout
{
    public partial class ShellLayoutView : UserControl
    {
        /// <summary>
		/// Initializes a new instance of the <see cref="T:ShellLayoutView"/> class.
		/// </summary>
		public ShellLayoutView()
        {
            InitializeComponent();

            if (Settings.Default.LcRows == 100)//231227 lo quito.
            {
                this._MainTLP.RowStyles.Clear();
                 this._MainTLP.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 12F));
                this._MainTLP.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 74.5F));
                this._MainTLP.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 13.5F));
                this._MainTLP.Size = new System.Drawing.Size(675, 609);

                this._LcWS.Location = new System.Drawing.Point(1, 450);
                this._LcWS.Size = new System.Drawing.Size(673, Settings.Default.LcRows * 65 + 3);
                this._TlfWS.Size = new System.Drawing.Size(269, 385);
                this._RdWS.Size = new System.Drawing.Size(403, 385);
            }

            _MainToolsWS.Name = WorkspaceNames.HeaderWorkspace;
            _RdWS.Name = WorkspaceNames.RdWorkspace;
            _TlfWS.Name = WorkspaceNames.TlfWorkspace;
            _LcWS.Name = WorkspaceNames.LcWorkspace;
        }

        /// <summary>
        /// 
        /// </summary>
        public Microsoft.Practices.CompositeUI.WinForms.DeckWorkspace MainToolsWS
        {
            get
            {
                return _MainToolsWS;
            }
        }

        private void _MainTLP_Paint(object sender, PaintEventArgs e)
        {

        }

        private void _TlfWS_Click(object sender, EventArgs e)
        {

        }
    }
}
