// SirtapGetChannels class.

using System;
using System.Configuration;
using System.Reflection;
using System.Xml.Linq;
using System.IO;
using Lextm.SharpSnmpLib;
using Lextm.SharpSnmpLib.Pipeline;
using Newtonsoft.Json;
using Lextm.SharpSnmpLib.Objects;

namespace SnmpAgent
{
    /// <summary>
    /// SirtapGetChannels object.
    /// </summary>
    public sealed class SirtapSetGetChannel : ScalarObject
    {
        private ISnmpData _ChannelSelected;

        /// <summary>
        /// Initializes a new instance of the <see cref="SirtapSetGetChannel"/> class.
        /// </summary>
        public SirtapSetGetChannel() : base(new ObjectIdentifier(u5ki.RemoteControlService.OIDs.Sirtap.SetGetRadioChannel))
        {
            _ChannelSelected = new Integer32(0);
        }

        /// <summary>
        /// Gets or sets the data.
        /// </summary>
        /// <value>The data.</value>
        public override ISnmpData Data
        {
            get 
            {
                int channel_selected;
                try
                {
                    channel_selected = int.Parse(ConfigurationManager.AppSettings["ChannelSelected"].ToString());
                }
                catch (Exception)
                {
                    channel_selected = 1;
                    SirtapRadio.Program.SetParamValueAppConfiguration("ChannelSelected", channel_selected.ToString());
                }

                int force_error;
                try
                {
                    force_error = int.Parse(ConfigurationManager.AppSettings["FORCE_ERROR"].ToString());
                }
                catch (Exception)
                {
                    force_error = 0;
                    SirtapRadio.Program.SetParamValueAppConfiguration("FORCE_ERROR", force_error.ToString());
                }

                if (force_error != 0) throw new Exception("ERROR FORZADO EN EL SIMULADOR");
                _ChannelSelected = new Integer32(channel_selected);
                return _ChannelSelected; 
            }
            
            set
            {
                if (value == null)
                {
                    throw new ArgumentNullException(nameof(value));
                }

                if (value.TypeCode != SnmpType.Integer32)
                {
                    throw new ArgumentException("SirtapSetGetChannel. Invalid data type.", nameof(value));
                }

                int ch;
                try
                {
                    ch = int.Parse(value.ToString());
                }
                catch
                {
                    throw new Exception("SirtapSetGetChannel. valor no es un entero");
                }

                int nChannels;
                try
                {
                    nChannels = int.Parse(ConfigurationManager.AppSettings["numChannels"].ToString());
                }
                catch (Exception)
                {
                    nChannels = 2;
                    SirtapRadio.Program.SetParamValueAppConfiguration("numChannels", nChannels.ToString());
                }

                if (ch < 1 || ch > nChannels)
                {
                    throw new Exception("SirtapSetGetChannel. El valor no esta entre 1 y el numero de canales");
                }

                int force_error;
                try
                {
                    force_error = int.Parse(ConfigurationManager.AppSettings["FORCE_ERROR"].ToString());
                }
                catch (Exception)
                {
                    force_error = 0;
                    SirtapRadio.Program.SetParamValueAppConfiguration("FORCE_ERROR", force_error.ToString());
                }

                if (force_error != 0) throw new Exception("ERROR FORZADO EN EL SIMULADOR");

                SirtapRadio.Program.SetParamValueAppConfiguration("ChannelSelected", ch.ToString());

                _ChannelSelected = value;
            }
        }
    }
}
