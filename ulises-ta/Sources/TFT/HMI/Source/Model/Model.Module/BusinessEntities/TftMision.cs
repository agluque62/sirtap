using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Practices.CompositeUI.EventBroker;
using HMI.Model.Module.Constants;
using HMI.Model.Module.Properties;
using Utilities;

namespace HMI.Model.Module.BusinessEntities
{
    public sealed class TftMision
    {
		private string _Mision = "";

		[EventPublication(EventTopicNames.TftMisionChanged, PublicationScope.Global)]
		public event EventHandler TftMisionChanged;

		public string Mision
		{
			get { return _Mision; }
			set
			{
				if (_Mision != value)
				{
					_Mision = value;
					string msg = _Mision;
					//General.SafeLaunchEvent(TftMisionChanged, _Mision);
				}
			}
		}

		public TftMision()
		{
			_Mision = "";
		}
	}
}
