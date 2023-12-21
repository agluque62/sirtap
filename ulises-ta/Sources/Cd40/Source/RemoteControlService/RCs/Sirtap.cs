using Lextm.SharpSnmpLib;
// using NLog;
using System;
using System.Collections.Generic;
using System.Collections;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Timers;
using U5ki.Enums;
using U5ki.Infrastructure;
using U5ki.Infrastructure.Code;
using U5ki.Infrastructure.Resources;
using Utilities;

using Translate;

namespace u5ki.RemoteControlService
{
    /// <summary>
    /// Esta es la clase que engloba la funcionalidad principal del telemando de Jotron modelo 7000.
    /// </summary>
    public class Sirtap : BaseCode, IRemoteControl
    {
        public readonly Int32 Port = 160;
        public readonly VersionCode SNMPVersion = VersionCode.V1;

        /// <summary>
        /// 
        /// </summary>
        public Sirtap()
        {
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="port"></param>
        public Sirtap(Int32 port)
        {
            Port = port;
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="port"></param>
        /// <param name="sNMPVersion"></param>
        public Sirtap(Int32 port, VersionCode sNMPVersion)
        {
            Port = port;
            SNMPVersion = sNMPVersion;
        }

        /// <summary>
        /// 
        /// </summary>
        public string Id { get; set; }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="response"></param>
        /// <param name="node"></param>
        public void CheckNode(Action<GearOperationStatus> response, BaseNode node)
        {
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="response"></param>
        /// <param name="node"></param>
        public void FrecuencyGet(Action<String> response, BaseNode node)
        {
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="action"></param>
        /// <param name="response"></param>
        /// <param name="node"></param>
        /// <param name="isEmitter"></param>
        /// <param name="isMaster"></param>
        public void ConfigureNode(RCConfigurationAction action, Action<GearOperationStatus> response, BaseNode node, Boolean isEmitter, Boolean isMaster)
        {
            
        }

        public TlmdoRsp.CodeTypes Tlmdo(TlmdoAsk msg, String targetIp, Boolean openSession = true)
        {
            return TlmdoRsp.CodeTypes.TLMDO_CODE_OK;
        }


        /// <summary>
        /// 
        /// </summary>
        /// <param name="input"></param>
        private void DeviceFrecuencyGet(Object input)
        {
            
        }
        

        /// <summary>
        /// 
        /// </summary>
        /// <param name="targetIp"></param>
        /// <param name="frecuency"></param>
        /// <param name="openSession"></param>
        /// <returns></returns>
        public GearOperationStatus SNMPFrecuencySet(String targetIp, String frecuency, bool isEmitter, Boolean openSession = true)
        {
            return GearOperationStatus.None;
        }


    }
}
