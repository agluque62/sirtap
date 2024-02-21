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
    public sealed class SirtapSetGetpowerLevelCh2 : ScalarObject
    {
        private ISnmpData _PowerLevel;

        /// <summary>
        /// Initializes a new instance of the <see cref="SirtapSetGetpowerLevelCh2"/> class.
        /// </summary>
        public SirtapSetGetpowerLevelCh2() : base(new ObjectIdentifier(u5ki.RemoteControlService.OIDs.Sirtap.powerLevelCh2))
        {
            _PowerLevel = new Integer32(0);
        }

        /// <summary>
        /// Gets or sets the data.
        /// </summary>
        /// <value>The data.</value>
        public override ISnmpData Data
        {
            get
            {
                int PowerLevelCh2;
                try
                {
                    PowerLevelCh2 = int.Parse(ConfigurationManager.AppSettings["PowerLevelCh2"].ToString());
                }
                catch (Exception)
                {
                    PowerLevelCh2 = 30000;
                    SirtapRadio.Program.SetParamValueAppConfiguration("PowerLevelCh2", PowerLevelCh2.ToString());
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
                _PowerLevel = new Integer32(PowerLevelCh2);
                return _PowerLevel;
            }

            set
            {
                if (value == null)
                {
                    throw new ArgumentNullException(nameof(value));
                }

                if (value.TypeCode != SnmpType.Integer32)
                {
                    throw new ArgumentException("SirtapSetGetpowerLevelCh2. Invalid data type.", nameof(value));
                }

                int PowerLevelCh2;
                try
                {
                    PowerLevelCh2 = int.Parse(value.ToString());
                }
                catch
                {
                    throw new Exception("SirtapSetGetpowerLevelCh2. valor no es un entero");
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

                SirtapRadio.Program.SetParamValueAppConfiguration("PowerLevelCh2", PowerLevelCh2.ToString());

                _PowerLevel = value;
            }
        }
    }
}
