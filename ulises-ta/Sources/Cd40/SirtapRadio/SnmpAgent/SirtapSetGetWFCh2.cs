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
    public sealed class SirtapSetGetWFCh2 : ScalarObject
    {
        private OctetString _WF;

        /// <summary>
        /// Initializes a new instance of the <see cref="SirtapSetGetWFCh2"/> class.
        /// </summary>
        public SirtapSetGetWFCh2() : base(new ObjectIdentifier(u5ki.RemoteControlService.OIDs.Sirtap.WFCh2))
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
                string WFCh2;
                try
                {
                    WFCh2 = ConfigurationManager.AppSettings["WFCh2"];
                }
                catch (Exception)
                {
                    WFCh2 = "WafeForm";
                    SirtapRadio.Program.SetParamValueAppConfiguration("WFCh2", WFCh2);
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

                _WF = new OctetString(WFCh2);
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
                    throw new ArgumentException("SirtapSetGetWFCh2. Invalid data type.", nameof(value));
                }

                string WFCh2;
                try
                {
                    WFCh2 = value.ToString();
                }
                catch
                {
                    throw new Exception("SirtapSetGetWFCh2. valor no es string");
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

                SirtapRadio.Program.SetParamValueAppConfiguration("WFCh2", WFCh2);

                _WF = new OctetString(value.ToString());
            }
        }
    }
}
