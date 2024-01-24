using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Practices.CompositeUI.EventBroker;
using HMI.Model.Module.Messages;
using HMI.Model.Module.Constants;
using Utilities;
using System.Threading;

namespace HMI.Model.Module.BusinessEntities
{
	public sealed class Tft
	{
        private bool _Enabled = true;
        private bool _Login = false;
        private string _LoginName = "";
        private string _LastLogin = "";
        
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
        [EventPublication(EventTopicNames.TftGoodLogin, PublicationScope.Global)]
        public event EventHandler TftGoodLogin;

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
            Login1 = valor;
            General.SafeLaunchEvent(TftLoginChanged, this);
        }
        public bool Login
        {
            get { return Login1; }
            set
            {
                if (Login1 != value)
                {
                    Login1 = value;
                   
                    General.SafeLaunchEvent(TftLoginChanged, this);
                    
                }
            }
        }
        public string LoginName
        {
            get { return (Login1) ? _LoginName : "" ; }
            set
            {
                if (_LoginName != value)
                {
                    _LoginName = value;
                    //General.SafeLaunchEvent(TftGoodLogin, this._LastLogin);
                    General.SafeLaunchEvent(TftGoodLogin, this._LoginName);

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

        public bool ModoNocturno
        {
            get => _ModoNocturno;
            set
            {
                _ModoNocturno = value;
            }
        }

        public string LastLogin { get => _LastLogin; set => _LastLogin = value; }
        public bool Login1 { get => _Login; set => _Login = value; }
    }
}
