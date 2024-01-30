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
using System.Net.NetworkInformation;

namespace SnmpAgent
{
    /// <summary>
    /// SirtapGetChannels object.
    /// </summary>
    public sealed class SirtapSetGetWFCh1 : ScalarObject
    {
        private OctetString _WF;

        /// <summary>
        /// Initializes a new instance of the <see cref="SirtapSetGetWFCh1"/> class.
        /// </summary>
        public SirtapSetGetWFCh1() : base(new ObjectIdentifier(u5ki.RemoteControlService.OIDs.Sirtap.WFCh1))
        {
            _WF = new OctetString("WafeForm");
        }

        /// <summary>
        /// Gets or sets the data.
        /// </summary>
        /// <value>The data.</value>
        public override ISnmpData Data
        {
            get 
            {
                string WFCh1;
                try
                {
                    WFCh1 = ConfigurationManager.AppSettings["WFCh1"];
                }
                catch (Exception)
                {
                    WFCh1 = "WafeForm";
                    SirtapRadio.Program.SetParamValueAppConfiguration("WFCh1", WFCh1);
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

                _WF = new OctetString(WFCh1);
                return _WF;
            }
            
            set
            {
                if (value == null)
                {
                    throw new ArgumentNullException(nameof(value));
                }

                if (value.TypeCode != SnmpType.OctetString)
                {
                    throw new ArgumentException("SirtapSetGetWFCh1. Invalid data type.", nameof(value));
                }

                string WFCh1;
                try
                {
                    WFCh1 = value.ToString();
                }catch
                {
                    throw new Exception("SirtapSetGetWFCh1. valor no es string");
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

                SirtapRadio.Program.SetParamValueAppConfiguration("WFCh1", WFCh1);

                _WF = new OctetString(value.ToString());
            }
        }
    }
}
