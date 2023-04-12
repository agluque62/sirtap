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
using HMI.Model.Module.UI;
using HMI.Model.Module.BusinessEntities;
using HMI.Presentation.Rabat.Constants;
using HMI.Presentation.Rabat.Properties;  // Miguel
using NLog;

namespace HMI.Presentation.Rabat.Views
{
	[SmartPart]
	public partial class TlfFunctionsView : UserControl
	{
		// Miguel
        //private const string AD = "AD";
		//private const string AI = "AI";

		private static Logger _Logger = LogManager.GetCurrentClassLogger();

		private IModelCmdManagerService _CmdManager;
		private StateManagerService _StateManager;
		private Dictionary<HMIButton, Color> _SlowBlinkList;
		private Dictionary<HMIButton, Color> _FastBlinkList;
		private bool _SlowBlinkOn = true;
		private bool _FastBlinkOn = true;

		private bool _PriorityEnabled
		{
			get
			{
				return _StateManager.Tft.Enabled && _StateManager.Engine.Operative &&
					((_StateManager.Tlf.Priority.State != FunctionState.Idle) ||
					(((_StateManager.Permissions & Permissions.Priority) == Permissions.Priority) &&
					 _StateManager.Jacks.SomeJack &&
					(_StateManager.Tlf.Listen.State == FunctionState.Idle) &&
					(_StateManager.Tlf.Transfer.State == FunctionState.Idle) &&
					(_StateManager.Tlf[TlfState.Set] + _StateManager.Tlf[TlfState.Conf] +
					_StateManager.Tlf[TlfState.Out] + _StateManager.Tlf[TlfState.RemoteHold] == 0)));
			}
		}
		private bool _ListenEnabled
		{
			get
			{
				return _StateManager.Tft.Enabled && _StateManager.Engine.Operative &&
					((_StateManager.Tlf.Listen.State != FunctionState.Idle) ||
					(((_StateManager.Permissions & Permissions.Listen) == Permissions.Listen) &&
					 _StateManager.Jacks.SomeJack &&
					(_StateManager.Tlf.Priority.State == FunctionState.Idle) &&
					(_StateManager.Tlf.Transfer.State == FunctionState.Idle) &&
					(_StateManager.Tlf[TlfState.Congestion] + _StateManager.Tlf[TlfState.Busy] +
					_StateManager.Tlf[TlfState.Hold] + _StateManager.Tlf[TlfState.RemoteHold] +
					_StateManager.Tlf[TlfState.Set] + _StateManager.Tlf[TlfState.Conf] +
					_StateManager.Tlf[TlfState.Out] + _StateManager.Tlf[TlfState.RemoteIn] +
					_StateManager.Tlf[TlfState.In] + _StateManager.Tlf[TlfState.InPrio] == 0)));
			}
		}
		private bool _HoldEnabled
		{
			get
			{
				return _StateManager.Tft.Enabled && _StateManager.Engine.Operative &&
					((_StateManager.Permissions & Permissions.Hold) == Permissions.Hold) &&
					 _StateManager.Jacks.SomeJack &&
					(_StateManager.Tlf.Priority.State == FunctionState.Idle) &&
					(_StateManager.Tlf.Listen.State == FunctionState.Idle) && 
					(_StateManager.Tlf.Transfer.State == FunctionState.Idle) &&
					(_StateManager.Tlf[TlfState.Set] + _StateManager.Tlf[TlfState.Conf] + _StateManager.Tlf[TlfState.RemoteHold] > 0);
			}
		}
		private bool _TransferEnabled
		{
			get
			{
				return _StateManager.Tft.Enabled && _StateManager.Engine.Operative &&
					((_StateManager.Tlf.Transfer.State != FunctionState.Idle) ||
					(((_StateManager.Permissions & Permissions.Transfer) == Permissions.Transfer) &&
					 _StateManager.Jacks.SomeJack &&
					(_StateManager.Tlf.Priority.State == FunctionState.Idle) &&
					(_StateManager.Tlf.Listen.State == FunctionState.Idle) &&
					(_StateManager.Tlf[TlfState.Set] + _StateManager.Tlf[TlfState.Conf] == 1)));
			}
		}
        private bool _ConferenceEnabled
        {
            get
            {
                return ((_StateManager.Permissions & Permissions.Conference) == Permissions.Conference) &&
                     _StateManager.Jacks.SomeJack;
            }
        }

        private bool _ReplayEnabled
        {
            get
            {
                return ((_StateManager.Permissions & Permissions.Replay) == Permissions.Replay) &&
                     _StateManager.Jacks.SomeJack;
            }
        }


		private bool _TlfViewEnabled
		{
			get 
			{
				return _StateManager.Tft.Enabled;
			}
		}
		private bool _CancelEnabled
		{
			get { return _StateManager.Tft.Enabled && _StateManager.Engine.Operative; }
		}
        private string _Prioridad   // Miguel
        {
            get { return Resources.Prioridad; }
        }
        private string _Retener // Miguel
        {
            get { return Resources.Retener; }
        }
        private string _Transferir // Miguel
        {
            get { return Resources.Transferir; }
        }
        private string _Escucha // Miguel
        {
            get { return Resources.Escucha; }
        }
        private string _AI // Miguel
        {
            get { return Resources.AI; }
        }
        private string _AD // Miguel
        {
            get { return Resources.AD; }
        }
        private string _Conferencia
        {
            get { return Resources.Conferencia; }
        }
        private string _Reproduccion
        {
            get { return Resources.Reproduccion; }
        }

        
		public TlfFunctionsView([ServiceDependency] IModelCmdManagerService cmdManager, [ServiceDependency] StateManagerService stateManager)
		{
			InitializeComponent();

			_CmdManager = cmdManager;
			_StateManager = stateManager;
			_SlowBlinkList = new Dictionary<HMIButton, Color>();
			_FastBlinkList = new Dictionary<HMIButton, Color>();

			_PriorityBT.Enabled = _PriorityEnabled;
			_ListenBT.Enabled = _ListenEnabled;
			_HoldBT.Enabled = _HoldEnabled;
			_TransferBT.Enabled = _TransferEnabled;
			_TlfViewBT.Enabled = _TlfViewEnabled;
			_CancelBT.Enabled = _CancelEnabled;
            _ConfBT.Enabled = _ConferenceEnabled;
            _ReplayBT.Enabled = _ReplayEnabled;

            // Miguel
            _PriorityBT.Text = _Prioridad; 
            _ListenBT.Text = _Escucha; 
            _TransferBT.Text = _Transferir;
            _HoldBT.Text = _Retener;
            _TlfViewBT.Text = _AI;
            _ConfBT.Text = _Conferencia;
            _ReplayBT.Text = _Reproduccion;
		}

		[EventSubscription(EventTopicNames.TftEnabledChanged, ThreadOption.Publisher)]
		[EventSubscription(EventTopicNames.EngineStateChanged, ThreadOption.Publisher)]
		public void OnTftEngineChanged(object sender, EventArgs e)
		{
			_PriorityBT.Enabled = _PriorityEnabled;
			_ListenBT.Enabled = _ListenEnabled;
			_HoldBT.Enabled = _HoldEnabled;
			_TransferBT.Enabled = _TransferEnabled;
			_TlfViewBT.Enabled = _TlfViewEnabled;
			_CancelBT.Enabled = _CancelEnabled;
            _ConfBT.Enabled = _ConferenceEnabled;
		}

		[EventSubscription(EventTopicNames.ActiveViewChanging, ThreadOption.Publisher)]
		public void OnActiveViewChanging(object sender, EventArgs<string> e)
		{
			if (e.Data == ViewNames.TlfDa)
			{
				_TlfViewBT.Text = _AI;
				ResetTlfViewBt(_AI);
			}
			else if (e.Data == ViewNames.TlfIa)
			{
				_TlfViewBT.Text = _AD;
				ResetTlfViewBt(_AD);
			}
		}

		[EventSubscription(EventTopicNames.JacksChanged, ThreadOption.Publisher)]
		public void OnJacksChanged(object sender, EventArgs e)
		{
			_PriorityBT.Enabled = _PriorityEnabled;
			_ListenBT.Enabled = _ListenEnabled;
			_HoldBT.Enabled = _HoldEnabled;
			_TransferBT.Enabled = _TransferEnabled;
            _ConfBT.Enabled = _ConferenceEnabled;
            _ReplayBT.Enabled = _ReplayEnabled;
		}

		[EventSubscription(EventTopicNames.TlfPriorityChanged, ThreadOption.Publisher)]
		public void OnTlfPriorityChanged(object sender, EventArgs e)
		{
			_PriorityBT.Enabled = _PriorityEnabled;
			_ListenBT.Enabled = _ListenEnabled;
			_HoldBT.Enabled = _HoldEnabled;
			_TransferBT.Enabled = _TransferEnabled;
            _ConfBT.Enabled = _ConferenceEnabled;

			switch (_StateManager.Tlf.Priority.State)
			{
				case FunctionState.Idle:
					if (_SlowBlinkList.Remove(_PriorityBT) && (_SlowBlinkList.Count == 0))
					{
						_SlowBlinkTimer.Enabled = false;
						_SlowBlinkOn = true;
					}
					_PriorityBT.ButtonColor = VisualStyle.ButtonColor;
					break;
				case FunctionState.Ready:
					_PriorityBT.ButtonColor = _SlowBlinkOn ? VisualStyle.Colors.Yellow : VisualStyle.ButtonColor;
					_SlowBlinkList[_PriorityBT] = VisualStyle.Colors.Yellow;
					_SlowBlinkTimer.Enabled = true;
					break;
				case FunctionState.Error:
					if (_SlowBlinkList.Remove(_PriorityBT) && (_SlowBlinkList.Count == 0))
					{
						_SlowBlinkTimer.Enabled = false;
						_SlowBlinkOn = true;
					}
					_PriorityBT.ButtonColor = VisualStyle.Colors.Red;
					break;
			}
		}

		[EventSubscription(EventTopicNames.TlfListenChanged, ThreadOption.Publisher)]
		public void OnTlfListenChanged(object sender, EventArgs e)
		{
			_PriorityBT.Enabled = _PriorityEnabled;
			_ListenBT.Enabled = _ListenEnabled;
			_HoldBT.Enabled = _HoldEnabled;
			_TransferBT.Enabled = _TransferEnabled;
            _ConfBT.Enabled = _ConferenceEnabled;

			switch (_StateManager.Tlf.Listen.State)
			{
				case FunctionState.Idle:
					if (_SlowBlinkList.Remove(_ListenBT) && (_SlowBlinkList.Count == 0))
					{
						_SlowBlinkTimer.Enabled = false;
						_SlowBlinkOn = true;
					}
					_ListenBT.ButtonColor = VisualStyle.ButtonColor;
					break;
				case FunctionState.Ready:
					_ListenBT.ButtonColor = _SlowBlinkOn ? VisualStyle.Colors.Yellow : VisualStyle.ButtonColor;
					_SlowBlinkList[_ListenBT] = VisualStyle.Colors.Yellow;
					_SlowBlinkTimer.Enabled = true;
					break;
				case FunctionState.Executing:
					if (_SlowBlinkList.Remove(_ListenBT) && (_SlowBlinkList.Count == 0))
					{
						_SlowBlinkTimer.Enabled = false;
						_SlowBlinkOn = true;
					}
					_ListenBT.ButtonColor = VisualStyle.Colors.Yellow;
					break;
				case FunctionState.Error:
					if (_SlowBlinkList.Remove(_ListenBT) && (_SlowBlinkList.Count == 0))
					{
						_SlowBlinkTimer.Enabled = false;
						_SlowBlinkOn = true;
					}
					_ListenBT.ButtonColor = VisualStyle.Colors.Red;
					break;
			}
		}

		[EventSubscription(EventTopicNames.TlfTransferChanged, ThreadOption.Publisher)]
		public void OnTlfTransferChanged(object sender, EventArgs e)
		{
			_PriorityBT.Enabled = _PriorityEnabled;
			_ListenBT.Enabled = _ListenEnabled;
			_HoldBT.Enabled = _HoldEnabled;
			_TransferBT.Enabled = _TransferEnabled;
            _ConfBT.Enabled = _ConferenceEnabled;

			switch (_StateManager.Tlf.Transfer.State)
			{
				case FunctionState.Idle:
					if (_SlowBlinkList.Remove(_TransferBT) && (_SlowBlinkList.Count == 0))
					{
						_SlowBlinkTimer.Enabled = false;
						_SlowBlinkOn = true;
					}
					_TransferBT.ButtonColor = VisualStyle.ButtonColor;
					break;
				case FunctionState.Ready:
				case FunctionState.Executing:
					_TransferBT.ButtonColor = _SlowBlinkOn ? VisualStyle.Colors.Yellow : VisualStyle.ButtonColor;
					_SlowBlinkList[_TransferBT] = VisualStyle.Colors.Yellow;
					_SlowBlinkTimer.Enabled = true;
					break;
				case FunctionState.Error:
					if (_SlowBlinkList.Remove(_TransferBT) && (_SlowBlinkList.Count == 0))
					{
						_SlowBlinkTimer.Enabled = false;
						_SlowBlinkOn = true;
					}
					_TransferBT.ButtonColor = VisualStyle.Colors.Red;
					break;
			}
		}

		[EventSubscription(EventTopicNames.TlfHangToneChanged, ThreadOption.Publisher)]
		public void OnTlfHangToneChanged(object sender, EventArgs e)
		{
			_CancelBT.ButtonColor = _StateManager.Tlf.HangTone.On ? VisualStyle.Colors.Yellow : VisualStyle.ButtonColor;
		}

		[EventSubscription(EventTopicNames.PermissionsChanged, ThreadOption.Publisher)]
		public void OnPermissionsChanged(object sender, EventArgs e)
		{
			_PriorityBT.Enabled = _PriorityEnabled;
			_ListenBT.Enabled = _ListenEnabled;
			_HoldBT.Enabled = _HoldEnabled;
			_TransferBT.Enabled = _TransferEnabled;
            _ConfBT.Enabled = _ConferenceEnabled;
            _ReplayBT.Enabled = _ReplayEnabled;
        }

		[EventSubscription(EventTopicNames.TlfChanged, ThreadOption.Publisher)]
		public void OnTlfChanged(object sender, RangeMsg e)
		{
			_PriorityBT.Enabled = _PriorityEnabled;
			_HoldBT.Enabled = _HoldEnabled;
			_TransferBT.Enabled = _TransferEnabled;
			_ListenBT.Enabled = _ListenEnabled;
            _ConfBT.Enabled = _ConferenceEnabled;

            _HoldBT.ButtonColor = _StateManager.Tlf[TlfState.Hold] == 0 ? VisualStyle.ButtonColor : VisualStyle.Colors.Yellow;
            _ConfBT.ButtonColor = _StateManager.Tlf[TlfState.Conf] <= 1 ? VisualStyle.ButtonColor : VisualStyle.Colors.Yellow;

			ResetTlfViewBt(_AD);
		}

		private void ResetTlfViewBt(string text)
		{
			if (_TlfViewBT.Text == text)
			{
				if (_FastBlinkList.Remove(_TlfViewBT) && (_FastBlinkList.Count == 0))
				{
					_FastBlinkTimer.Enabled = false;
					_FastBlinkOn = true;
				}
				if (_SlowBlinkList.Remove(_TlfViewBT) && (_SlowBlinkList.Count == 0))
				{
					_SlowBlinkTimer.Enabled = false;
					_SlowBlinkOn = true;
				}

				if (text == _AD)
				{
					TlfState st = _StateManager.Tlf.GetTlfState(0, Tlf.NumDestinations);

					if (_StateManager.Tlf.Unhang.AssociatePosition != Tlf.IaMappedPosition)
					{
						st = (TlfState)Math.Max((int)st, (int)_StateManager.Tlf[Tlf.IaMappedPosition].State);
					}

					_TlfViewBT.ButtonColor = GetStateColor(_TlfViewBT, st);
				}
				else
				{
					_TlfViewBT.ButtonColor = VisualStyle.ButtonColor;
				}
			}
		}

		private Color GetStateColor(HMIButton bt, TlfState st)
		{
			Color backColor = VisualStyle.ButtonColor;

			switch (st)
			{
				case TlfState.Idle:
				case TlfState.PaPBusy:
					break;
				case TlfState.In:
					backColor = _SlowBlinkOn ? VisualStyle.Colors.Orange : VisualStyle.ButtonColor;
					_SlowBlinkList[bt] = VisualStyle.Colors.Orange;
					_SlowBlinkTimer.Enabled = true;
					break;
				case TlfState.Out:
					backColor = VisualStyle.Colors.Blue;
					break;
				case TlfState.Set:
				case TlfState.Conf:
					backColor = VisualStyle.Colors.Green;
					break;
				case TlfState.Busy:
					backColor = VisualStyle.Colors.Red;
					break;
				case TlfState.Mem:
					backColor = VisualStyle.Colors.Orange;
					break;
				case TlfState.RemoteMem:
					backColor = VisualStyle.Colors.DarkGray;
					break;
				case TlfState.Hold:
				case TlfState.RemoteHold:
					backColor = _SlowBlinkOn ? VisualStyle.Colors.Green : VisualStyle.ButtonColor;
					_SlowBlinkList[bt] = VisualStyle.Colors.Green;
					_SlowBlinkTimer.Enabled = true;
					break;
				case TlfState.RemoteIn:
					backColor = _SlowBlinkOn ? VisualStyle.Colors.DarkGray : VisualStyle.ButtonColor;
					_SlowBlinkList[bt] = VisualStyle.Colors.DarkGray;
					_SlowBlinkTimer.Enabled = true;
					break;
				case TlfState.Congestion:
					backColor = _SlowBlinkOn ? VisualStyle.Colors.Red : VisualStyle.ButtonColor;
					_SlowBlinkList[bt] = VisualStyle.Colors.Red;
					_SlowBlinkTimer.Enabled = true;
					break;
				case TlfState.InPrio:
					backColor = _FastBlinkOn ? VisualStyle.Colors.Orange : VisualStyle.ButtonColor;
					_FastBlinkList[bt] = VisualStyle.Colors.Orange;
					_FastBlinkTimer.Enabled = true;
					break;
				case TlfState.NotAllowed:
					backColor = _FastBlinkOn ? VisualStyle.Colors.Yellow : VisualStyle.ButtonColor;
					_FastBlinkList[bt] = VisualStyle.Colors.Yellow;
					_FastBlinkTimer.Enabled = true;
					break;
			}

			return backColor;
		}

		private void _PriorityBT_Click(object sender, EventArgs e)
		{
			try
			{
				_CmdManager.PriorityClick();
			}
			catch (Exception ex)
			{
				_Logger.Error("ERROR pulsando tecla de prioridad", ex);
			}
		}

		private void _ListenBT_Click(object sender, EventArgs e)
		{
			try
			{
				_CmdManager.ListenClick();
			}
			catch (Exception ex)
			{
				_Logger.Error("ERROR pulsando tecla de escucha", ex);
			}
		}

		private void _HoldBT_Click(object sender, EventArgs e)
		{
			try
			{
				_CmdManager.HoldClick();
			}
			catch (Exception ex)
			{
				_Logger.Error("ERROR pulsando tecla de aparcar", ex);
			}
		}

		private void _TransferBT_Click(object sender, EventArgs e)
		{
			try
			{
				_CmdManager.TransferClick();
			}
			catch (Exception ex)
			{
				_Logger.Error("ERROR pulsando tecla de transferencia", ex);
			}
		}

        private void _ConfBT_Click(object sender, EventArgs e)
        {
            try
            {
                _CmdManager.ConferenceClick();
            }
            catch (Exception ex)
            {
                _Logger.Error("ERROR pulsando tecla de transferencia", ex);
            }
        }

        private void _ReplaylBT_Click(object sender, EventArgs e)
        {
            try
            {
                _CmdManager.ReplayClick();
            }
            catch (Exception ex)
            {
                _Logger.Error("ERROR mostrando dependencias", ex);
            }
        }

        private void _TlfViewBT_Click(object sender, EventArgs e)
		{
			try
			{
				_CmdManager.SwitchTlfView(null);
			}
			catch (Exception ex)
			{
				_Logger.Error("ERROR solicitando cambio de pagina AD <--> AI", ex);
			}
		}

		private void _CancelBT_Click(object sender, EventArgs e)
		{
			try
			{
				_CmdManager.CancelTlfClick();
			}
			catch (Exception ex)
			{
				_Logger.Error("ERROR pulsando tecla de anular", ex);
			}
		}

		private void _SlowBlinkTimer_Tick(object sender, EventArgs e)
		{
			try
			{
				if (_SlowBlinkTimer.Enabled)
				{
					_SlowBlinkOn = !_SlowBlinkOn;
					foreach (KeyValuePair<HMIButton, Color> p in _SlowBlinkList)
					{
						p.Key.ButtonColor = _SlowBlinkOn ? p.Value : VisualStyle.ButtonColor;
					}
				}
			}
			catch (Exception ex)
			{
				_Logger.Error("ERROR generando parpadeo lento para teclas de funcionalidad telefonica", ex);
			}
		}

		private void _FastBlinkTimer_Tick(object sender, EventArgs e)
		{
			try
			{
				if (_FastBlinkTimer.Enabled)
				{
					_FastBlinkOn = !_FastBlinkOn;
					foreach (KeyValuePair<HMIButton, Color> p in _FastBlinkList)
					{
						p.Key.ButtonColor = _FastBlinkOn ? p.Value : VisualStyle.ButtonColor;
					}
				}
			}
			catch (Exception ex)
			{
				_Logger.Error("ERROR generando parpadeo rapido para teclas de funcionalidad telefonica", ex);
			}
		}
	}
}

