using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Practices.CompositeUI.EventBroker;
using HMI.Model.Module.Constants;
using HMI.Model.Module.Properties;
using Utilities;

namespace HMI.Model.Module.BusinessEntities
{
	public sealed class AltavozAlarmas
	{
		private int _Level = 0;
		private bool _Enabled = true;

		[EventPublication(EventTopicNames.AlarmaLevelChanged, PublicationScope.Global)]
		public event EventHandler AlarmaLevelChanged;

		[EventPublication(EventTopicNames.AlarmaStateChanged, PublicationScope.Global)]
		public event EventHandler AlarmaStateChanged;

		public int Level
		{
			get { return _Level; }
			set
			{
				if (_Level != value)
				{
					_Level = value;

					Settings.Default.BuzzerLevel = value;
					Settings.Default.Save();

					General.SafeLaunchEvent(AlarmaLevelChanged, this);
				}
			}
		}

		public bool Enabled
		{
			get { return _Enabled; }
			set
			{
				if (_Enabled != value)
				{
					_Enabled = value;

					//Settings.Default.BuzzerEnabled = value;
					//Settings.Default.Save();

					General.SafeLaunchEvent(AlarmaStateChanged, this);
				}
			}
		}

		public AltavozAlarmas()
		{
			_Enabled = Settings.Default.AALarmaEnabled;
			_Level = Settings.Default.AAlarmaLevel;
		}
	}
}
