using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Practices.CompositeUI.EventBroker;
using HMI.Model.Module.Messages;
using HMI.Model.Module.Constants;
using Utilities;

namespace HMI.Model.Module.BusinessEntities
{
	public sealed class Tft
	{
        private bool _Enabled = true;
        private bool _Login = false;
        private string _LastLogin = "";
        private string _Mision = "";
        private bool _ModoNocturno = !(DateTime.Now.Hour >= 6 && DateTime.Now.Hour< 18);

        private bool _Briefing = false;
        private bool _Playing = false;
        public event GenericEventHandler<SnmpStringMsg<string, string>> SendSnmpTrapString;

        [EventPublication(EventTopicNames.TftEnabledChanged, PublicationScope.Global)]
        public event EventHandler TftEnabledChanged;
        [EventPublication(EventTopicNames.TftLoginChanged, PublicationScope.Global)]
        public event EventHandler TftLoginChanged;
        [EventPublication(EventTopicNames.TftBadLogin, PublicationScope.Global)]
        public event EventHandler TftBadLogin;
        [EventPublication(EventTopicNames.BriefingChanged, PublicationScope.Global)]
        public event EventHandler BriefingChanged;
        [EventPublication(EventTopicNames.PlayingChanged, PublicationScope.Global)]
        public event EventHandler PlayingChanged;

		public bool Enabled
		{
			get { return _Enabled; }
			set 
			{ 
				if (_Enabled != value)
				{
					_Enabled = value;
					General.SafeLaunchEvent(TftEnabledChanged, this);
				}
			}
		}

        public void  SetLogin(bool valor)
        {
            _Login = valor;
            General.SafeLaunchEvent(TftLoginChanged, this);
        }
        public bool Login
        {
            get { return _Login; }
            set
            {
                if (_Login != value)
                {
                    _Login = value;
                    General.SafeLaunchEvent(TftLoginChanged, this);
                }
            }
        }
        public string GenerarHistoricoLoginIncorrecto
        {
            set
            {
                _LastLogin = value;
                General.SafeLaunchEvent(TftBadLogin, this._LastLogin);

            }
        }
        public bool Briefing
        {
            get { return _Briefing; }
            set
            {
                if (_Briefing != value) 
                {
                    _Briefing = value;

                    General.SafeLaunchEvent(BriefingChanged, this);
                }
            }
        }

        public bool Playing
        {
            get { return _Playing; }
            set
            {
                if (_Playing != value)
                {
                    _Playing = value;

                    General.SafeLaunchEvent(PlayingChanged, this);
                }
            }
        }

        public string Mision { get => _Mision; set => _Mision = value; }
        public bool ModoNocturno
        {
            get => _ModoNocturno;
            set
            {
                _ModoNocturno = value;
            }
        }
    }
}
