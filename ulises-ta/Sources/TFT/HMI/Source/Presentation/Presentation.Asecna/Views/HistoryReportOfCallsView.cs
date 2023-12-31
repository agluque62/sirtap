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

using System;
using System.Drawing;
using System.Windows.Forms;
using System.Collections.Generic;
using Microsoft.Practices.CompositeUI;
using Microsoft.Practices.CompositeUI.SmartParts;
using Microsoft.Practices.CompositeUI.EventBroker;
using Microsoft.Practices.ObjectBuilder;
using HMI.Infrastructure.Interface;
using HMI.Model.Module.Services;
using HMI.Model.Module.Messages;
using HMI.Model.Module.BusinessEntities;
using HMI.Model.Module.UI;
using HMI.Presentation.Asecna.Constants;
using HMI.Presentation.Asecna.Properties;
using Utilities;
using NLog;

namespace HMI.Presentation.Asecna.Views
{
	[SmartPart]
	public partial class HistoryReportOfCalls : UserControl
	{
		private static Logger _Logger = LogManager.GetCurrentClassLogger();

		private IModelCmdManagerService _CmdManager = null;
		private StateManagerService _StateManager = null;

        private LlamadaHistorica[] LastCalls;

        // Delegado necesario para llamar al procedimiento ShowHistoricalOfCalls
        // desde el proceso StateManager, proceso distinto del que cre� el control _LVCalls
        delegate void ShowHistoricalOfCallsCallback();

		private bool _CloseEnabled
		{
			get { return _StateManager.Tft.Enabled; }
		}

        private string _Close
        {
            get { return Resources.Cerrar; }
        }       
        
        private string _Title
        {
            get { return Resources.HistoricoLlamadas; }
        }


		public HistoryReportOfCalls([ServiceDependency] IModelCmdManagerService cmdManager, [ServiceDependency] StateManagerService stateManager)
		{
			InitializeComponent();            
			_CmdManager = cmdManager;
			_StateManager = stateManager;
            
            _CloseBT.Text = _Close;
            _TitleLB.Text = _Title;
            _BtnDependencesView.Text = Resources.DirectorioDependenciasUsuario;
            // Etiqueta ultima llamada
            _LblTitleLastCall.Text = Resources.LastCall;
            // Radio buttons tipo de llamada
            _RBIncomming.Text = Resources.LlamadasEntrantes;
            _RBNotAnswered.Text = Resources.LlamadasNoAtendidas;
            _RBOutgoing.Text = Resources.LlamadasSalientes;
            //Encabezamientos de la tabla
            columnHeader1.Text = Resources.DateTime; 
            columnHeader2.Text = Resources.Origen;
            columnHeader3.Text = Resources.Destino;
		}

        [EventSubscription(EventTopicNames.HistoricalOfLocalCalls, ThreadOption.Publisher)]
        public void OnHistoricalOfLocalCalls(object sender, EventArgs e)
        {
            _Logger.Debug("Procesando {0}", EventTopicNames.HistoricalOfLocalCalls);

            LastCalls = _StateManager.HistoricalOfCalls.Call;

            ShowHistoricalOfCalls();
        }

        private void ShowHistoricalOfCalls()
        {
            // Mecanismo necesario para invocar a ShowHistoricalOfCalls de forma segura desde
            // procesos distintos de aquel que cre� el control _LVCalls
            if (_LVCalls.InvokeRequired)
            {
                ShowHistoricalOfCallsCallback d = new ShowHistoricalOfCallsCallback(ShowHistoricalOfCalls);
                this.Invoke(d);
            }
            else
            {
                _LVCalls.Items.Clear();

                foreach (LlamadaHistorica s in LastCalls)
                {
                    if ((_RBIncomming.Checked && s.Tipo == "Entrantes") ||
                        (_RBOutgoing.Checked && s.Tipo == "Salientes") ||
                        (_RBNotAnswered.Checked && s.Tipo == "NoAtendidas"))
                    {
                        ListViewItem.ListViewSubItem[] subItem = new ListViewItem.ListViewSubItem[3];
                        subItem[1] = new ListViewItem.ListViewSubItem();
                        subItem[0] = new ListViewItem.ListViewSubItem();
                        subItem[2] = new ListViewItem.ListViewSubItem();

                        _LblTextLastCall.Text = s.Ultima;

                        subItem[0].Text = s.Fecha_Hora;
                        subItem[1].Text = s.Acceso;
                        subItem[2].Text = s.Colateral;

                        ListViewItem item = new ListViewItem(subItem, null);

                        _LVCalls.Items.Add(item);
                    }
                }
            }
        }

		private void _CloseBT_Click(object sender, EventArgs e)
		{
			try
			{
				_CmdManager.SwitchTlfView(null);
			}
			catch (Exception ex)
			{
				_Logger.Error("ERROR cerrando la vista dependencia", ex);
			}
		}

		private void HistoryReportOfCalls_BackColorChanged(object sender, EventArgs e)
		{
			Invalidate(true);
		}

        private void _BtnDependencesView_Click(object sender, EventArgs e)
        {
            try
            {
                _CmdManager.SwitchTlfView(ViewNames.Depencences);
            }
            catch (Exception ex)
            {
                _Logger.Error("ERROR cerrando la vista dependencia", ex);
            }
        }

        private void CallType_CheckedChanged(object sender, EventArgs e)
        {
            if (((RadioButton)sender).Checked)
                ShowHistoricalOfCalls();
        }
	}
}

