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
    public sealed class SirtapGetChannels : ScalarObject
    {
        private ISnmpData _NumChannels;

        /// <summary>
        /// Initializes a new instance of the <see cref="SirtapGetChannels"/> class.
        /// </summary>
        public SirtapGetChannels(int numchannels) : base(new ObjectIdentifier(u5ki.RemoteControlService.OIDs.Sirtap.GetChannels))
        {
            _NumChannels = new Integer32(numchannels);
        }
        public SirtapGetChannels() : base(new ObjectIdentifier(u5ki.RemoteControlService.OIDs.Sirtap.GetChannels))
        {
            _NumChannels = new Integer32(1);
        }

        /// <summary>
        /// Gets or sets the data.
        /// </summary>
        /// <value>The data.</value>
        public override ISnmpData Data
        {
            get 
            {
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
               _NumChannels = new Integer32(nChannels);
                return _NumChannels; 
            }
            
            set
            {
                throw new AccessFailureException();
            }
        }
    }
}
