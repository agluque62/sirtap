using HMI.Model.Module;
using HMI.Model.Module.BusinessEntities;
using HMI.Model.Module.Messages;
using HMI.Model.Module.Services;
using HMI.Model.Module.UI;
using HMI.Presentation.Sirtap.Constants;
using HMI.Presentation.Sirtap.Properties;
using Microsoft.Practices.CompositeUI;
using NLog;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Utilities;

namespace HMI.Presentation.Sirtap.UI
{
    public partial class ListaDeParticipantes : UserControl
    {
        private IModelCmdManagerService _CmdManager;
        private StateManagerService _StateManager;
        public void setup([ServiceDependency] IModelCmdManagerService cmdManager, [ServiceDependency] StateManagerService stateManager)
        {
            _CmdManager = cmdManager;
            _StateManager = stateManager;
            if (!VisualStyle.ModoNocturno)
            {

            }
            else
            {
                BackColor = VisualStyle.ButtonColorN;
                listBox1.BackColor = VisualStyle.ButtonColorN;
                listBox1.ForeColor = VisualStyle.TextoTfColorN;
                button1.BackColor = VisualStyle.ButtonColorN;
                button1.ForeColor = VisualStyle.TextoTfColorN;
                button2.BackColor = VisualStyle.ButtonColorN;
                button2.ForeColor = VisualStyle.TextoTfColorN;
                IdConferencia.BackColor = VisualStyle.ButtonColorN;
                IdConferencia.ForeColor = VisualStyle.TextoTfColorN;

            }
        }

    }
}
