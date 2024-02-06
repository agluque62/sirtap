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
    public sealed class SirtapSetGetTxInhibit : ScalarObject
    {
        private ISnmpData _TxInhibit;

        /// <summary>
        /// Initializes a new instance of the <see cref="SirtapSetGetfrequencyCh1"/> class.
        /// </summary>
        public SirtapSetGetTxInhibit() : base(new ObjectIdentifier(u5ki.RemoteControlService.OIDs.Sirtap.SetTxInhibit))
        {
            _TxInhibit = new Integer32(0);
        }

        /// <summary>
        /// Gets or sets the data.
        /// </summary>
        /// <value>The data.</value>
        public override ISnmpData Data
        {
            get 
            {
                int TxInhibit;
                try
                {
                    TxInhibit = int.Parse(ConfigurationManager.AppSettings["TxInhibit"].ToString());
                }
                catch (Exception)
                {
                    TxInhibit = 0;
                    SirtapRadio.Program.SetParamValueAppConfiguration("TxInhibit", TxInhibit.ToString());
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
                _TxInhibit = new Integer32(TxInhibit);
                return _TxInhibit;
            }
            
            set
            {
                if (value == null)
                {
                    throw new ArgumentNullException(nameof(value));
                }

                if (value.TypeCode != SnmpType.Integer32)
                {
                    throw new ArgumentException("SirtapSetGetTxInhibit. Invalid data type.", nameof(value));
                }

                int TxInhibit;
                try
                {
                    TxInhibit = int.Parse(value.ToString());
                }catch
                {
                    throw new Exception("SirtapSetGetTxInhibit. valor no es un entero");
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

                if (TxInhibit != 0) TxInhibit = 1;
                SirtapRadio.Program.SetParamValueAppConfiguration("TxInhibit", TxInhibit.ToString());

                _TxInhibit = value;
            }
        }
    }
}
