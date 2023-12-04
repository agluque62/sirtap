//----------------------------------------------------------------------------------------
// patterns & practices - Smart Client Software Factory - Guidance Package
//
// This file was generated by the "Add View" recipe.
//
// This class is the concrete implementation of a View in the Model-View-Presenter 
// pattern. Communication between the Presenter and this class is acheived through 
// an interface to facilitate separation and testability.
// Note that the Presenter generated by the same recipe, will automatically be created
// by CAB through [CreateNew] and bidirectional references will be added.
//
// For more information see:
// ms-help://MS.VSCC.v80/MS.VSIPCC.v80/ms.practices.scsf.2007may/SCSF/html/02-09-010-ModelViewPresenter_MVP.htm
//
// Latest version of this Guidance Package: http://go.microsoft.com/fwlink/?LinkId=62182
//----------------------------------------------------------------------------------------

using HMI.Infrastructure.Interface;
using HMI.Presentation.Sirtap.Constants;
using Microsoft.Practices.CompositeUI.SmartParts;
using Microsoft.Practices.ObjectBuilder;
using System;
using System.Windows.Forms;

namespace HMI.Presentation.Sirtap.Views
{
    [SmartPart]
    public partial class TlfView : UserControl
    {
        public TlfView()
        {
            InitializeComponent();
            _TlfNumbersWS.Name = WorkspaceNames.TlfNumbersWorkspace;
            _TlfFunctionsWS.Name = WorkspaceNames.TlfFunctionsWorkspace;
        }

        private void TlfView_BackColorChanged(object sender, EventArgs e)
        {
            Invalidate(true);
        }

        private void _TlfTLP_Paint(object sender, PaintEventArgs e)
        {

        }
    }
}

