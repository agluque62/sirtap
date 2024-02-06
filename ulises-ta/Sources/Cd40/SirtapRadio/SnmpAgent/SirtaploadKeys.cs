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
    public sealed class SirtaploadKeys : ScalarObject
    {
        private int MAX_CRYPT_KEYS = 30;

        /// <summary>
        /// Initializes a new instance of the <see cref="SirtaploadKeys"/> class.
        /// </summary>
        public SirtaploadKeys() : base(new ObjectIdentifier(u5ki.RemoteControlService.OIDs.Sirtap.loadKeys))
        {
        }

        /// <summary>
        /// Gets or sets the data.
        /// </summary>
        /// <value>The data.</value>
        public override ISnmpData Data
        {
            get 
            {
                throw new AccessFailureException();
            }
            
            set
            {
                if (value == null)
                {
                    throw new ArgumentNullException(nameof(value));
                }

                if (value.TypeCode != SnmpType.Integer32)
                {
                    throw new ArgumentException("SirtaploadKeys. Invalid data type.", nameof(value));
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

                try
                {
                    int num_keys = int.Parse(value.ToString());
                    if (num_keys > MAX_CRYPT_KEYS || num_keys < 0)
                    {
                        throw new Exception($"SirtaploadKeys. El valor no esta entre 0 y MAX_CRYPT_KEYS");
                    }
                }
                catch
                {
                    throw new Exception("SirtaploadKeys. valor no es un entero");
                }
            }
        }
    }
}
