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
    public sealed class SirtapSetGetfrequencyCh2 : ScalarObject
    {
        private ISnmpData _Frequency;

        /// <summary>
        /// Initializes a new instance of the <see cref="SirtapSetGetfrequencyCh2"/> class.
        /// </summary>
        public SirtapSetGetfrequencyCh2() : base(new ObjectIdentifier(u5ki.RemoteControlService.OIDs.Sirtap.frequencyCh2))
        {
            _Frequency = new Integer32(0);
        }

        /// <summary>
        /// Gets or sets the data.
        /// </summary>
        /// <value>The data.</value>
        public override ISnmpData Data
        {
            get
            {
                int FrequencyCh2;
                try
                {
                    FrequencyCh2 = int.Parse(ConfigurationManager.AppSettings["FrequencyCh2"].ToString());
                }
                catch (Exception)
                {
                    FrequencyCh2 = 30000;
                    SirtapRadio.Program.SetParamValueAppConfiguration("FrequencyCh2", FrequencyCh2.ToString());
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
                _Frequency = new Integer32(FrequencyCh2);
                return _Frequency;
            }

            set
            {
                if (value == null)
                {
                    throw new ArgumentNullException(nameof(value));
                }

                if (value.TypeCode != SnmpType.Integer32)
                {
                    throw new ArgumentException("SirtapSetGetfrequencyCh2. Invalid data type.", nameof(value));
                }

                int FrequencyCh2;
                try
                {
                    FrequencyCh2 = int.Parse(value.ToString());
                }
                catch
                {
                    throw new Exception("SirtapSetGetfrequencyCh2. valor no es un entero");
                }

                int FrequencyMin, FrequencyMax;
                try
                {
                    FrequencyMin = int.Parse(ConfigurationManager.AppSettings["FrequencyMin"].ToString());
                }
                catch (Exception)
                {
                    FrequencyMin = 30000;
                    SirtapRadio.Program.SetParamValueAppConfiguration("FrequencyMin", FrequencyMin.ToString());
                }

                try
                {
                    FrequencyMax = int.Parse(ConfigurationManager.AppSettings["FrequencyMax"].ToString());
                }
                catch (Exception)
                {
                    FrequencyMax = 511975;
                    SirtapRadio.Program.SetParamValueAppConfiguration("FrequencyMax", FrequencyMax.ToString());
                }

                if (FrequencyCh2 < FrequencyMin || FrequencyCh2 > FrequencyMax)
                {
                    throw new Exception("SirtapSetGetfrequencyCh2. El valor de frecuencia no esta entre el valor minimo y maximo");
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

                SirtapRadio.Program.SetParamValueAppConfiguration("FrequencyCh2", FrequencyCh2.ToString());

                _Frequency = value;
            }
        }
    }
}
