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
    public sealed class SirtapSetGetpowerLevelCh1 : ScalarObject
    {
        private ISnmpData _PowerLevel;

        /// <summary>
        /// Initializes a new instance of the <see cref="SirtapSetGetpowerLevelCh1"/> class.
        /// </summary>
        public SirtapSetGetpowerLevelCh1() : base(new ObjectIdentifier(u5ki.RemoteControlService.OIDs.Sirtap.powerLevelCh1))
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
                int PowerLevelCh1;
                try
                {
                    PowerLevelCh1 = int.Parse(ConfigurationManager.AppSettings["PowerLevelCh1"].ToString());
                }
                catch (Exception)
                {
                    PowerLevelCh1 = 25;
                    SirtapRadio.Program.SetParamValueAppConfiguration("PowerLevelCh1", PowerLevelCh1.ToString());
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
                _PowerLevel = new Integer32(PowerLevelCh1);
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
                    throw new ArgumentException("SirtapSetGetpowerLevelCh1. Invalid data type.", nameof(value));
                }

                int PowerLevelCh1;
                try
                {
                    PowerLevelCh1 = int.Parse(value.ToString());
                }
                catch
                {
                    throw new Exception("SirtapSetGetpowerLevelCh1. valor no es un entero");
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

                SirtapRadio.Program.SetParamValueAppConfiguration("PowerLevelCh1", PowerLevelCh1.ToString());

                _PowerLevel = value;
            }
        }
    }
}
