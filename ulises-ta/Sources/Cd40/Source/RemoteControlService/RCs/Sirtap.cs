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
        public readonly Int32 NUMMAXTimeout = 1;
        public readonly String Community = "public";
        public readonly Int32 SNMPCallTimeout = 500; // Miliseconds

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

        public TlmdoRsp.CodeTypes Tlmdo(TlmdoAsk msg, String targetIp, bool isEmitter, ref TlmdoRsp response, Boolean openSession = true)
        {
            TlmdoRsp.CodeTypes ret = TlmdoRsp.CodeTypes.TLMDO_CODE_INVALID_OP;
            switch (msg.msgType)
            {
                case TlmdoAsk.MsgType.TLMDO_GET_CHANNELS:
                    ret = SNMP_TLMDO_GET_CHANNELS(targetIp, ref response);
                    break;
                case TlmdoAsk.MsgType.TLMDO_SET_CHANNEL:
                    ret = TLMDO_SET_CHANNEL(msg, targetIp);
                    break;
                case TlmdoAsk.MsgType.TLMDO_SET_TXPWR:
                    if (isEmitter) ret = TLMDO_SET_TXPWR(msg, targetIp);
                    else ret = TlmdoRsp.CodeTypes.TLMDO_CODE_OK;
                    break;
                case TlmdoAsk.MsgType.TLMDO_SET_TXINHIBIT:
                    break;
                case TlmdoAsk.MsgType.TLMDO_SET_WFALLOC:
                    break;
                case TlmdoAsk.MsgType.TLMDO_SET_CRYPT_KEYS:
                    break;
            }

            return ret;
        }

        private TlmdoRsp.CodeTypes SNMP_TLMDO_GET_CHANNELS(String targetIp, ref TlmdoRsp response)
        {
            int i_rtimeout = 0;
            TlmdoRsp.CodeTypes ret = TlmdoRsp.CodeTypes.TLMDO_CODE_OK;
            string channels = "";
            while (i_rtimeout <= NUMMAXTimeout)
            {
                try
                {
                    channels = SnmpClient.GetString(targetIp, Community, u5ki.RemoteControlService.OIDs.Sirtap.GetChannels,
                        SNMPCallTimeout, Port, SNMPVersion);
                }
                catch (Exception ex)
                {
                    if (ex is Lextm.SharpSnmpLib.Messaging.TimeoutException)
                    {
                        if (i_rtimeout >= NUMMAXTimeout)
                        {
                            string log = "[SNMP][" + "TLMDO_GET_CHANNELS" + "] [" + targetIp + "] TIMEOUT ";
                            LogWarn<RCJotron7000>(log,
                                U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR,
                                Id, CTranslate.translateResource(log));
                            ret = TlmdoRsp.CodeTypes.TLMDO_CODE_NO_RESP;
                        }
                        i_rtimeout++;
                    }
                    else
                    {
                        string log = "[SNMP][" + "TLMDO_GET_CHANNELS" + "] [" + targetIp + "] ERROR ";
                        LogWarn<RCJotron7000>(log,
                            U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR,
                            Id, CTranslate.translateResource(log));
                        ret = TlmdoRsp.CodeTypes.TLMDO_CODE_ERROR;
                    }
                }
            }

            if (ret == TlmdoRsp.CodeTypes.TLMDO_CODE_OK)
            {
                response.ChannelList += " " + channels;
            }

            return ret;
        }

        private TlmdoRsp.CodeTypes TLMDO_SET_CHANNEL(TlmdoAsk msg, String targetIp)
        {
            int i_rtimeout = 0;
            TlmdoRsp.CodeTypes ret = TlmdoRsp.CodeTypes.TLMDO_CODE_OK;
            
            while (i_rtimeout <= NUMMAXTimeout)
            {
                try
                {
                    SnmpClient.SetString(targetIp, Community, u5ki.RemoteControlService.OIDs.Sirtap.Frecuency, msg.Channel,
                        SNMPCallTimeout, Port, SNMPVersion);
                }
                catch (Exception ex)
                {
                    if (ex is Lextm.SharpSnmpLib.Messaging.TimeoutException)
                    {
                        if (i_rtimeout >= NUMMAXTimeout)
                        {
                            string log = "[SNMP][" + "TLMDO_SET_CHANNEL" + "] [" + targetIp + "] [" + msg.Channel + "] TIMEOUT ";
                            LogWarn<RCJotron7000>(log,
                                U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR,
                                Id, CTranslate.translateResource(log));
                            ret = TlmdoRsp.CodeTypes.TLMDO_CODE_NO_RESP;
                        }
                        i_rtimeout++;
                    }
                    else
                    {
                        string log = "[SNMP][" + "TLMDO_SET_CHANNEL" + "] [" + targetIp + "] [" + msg.Channel + "] ERROR ";
                        LogWarn<RCJotron7000>(log,
                            U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR,
                            Id, CTranslate.translateResource(log));
                        ret = TlmdoRsp.CodeTypes.TLMDO_CODE_ERROR;
                    }
                }
            }           

            return ret;
        }

        private TlmdoRsp.CodeTypes TLMDO_SET_TXPWR(TlmdoAsk msg, String targetIp)
        {
            int i_rtimeout = 0;
            TlmdoRsp.CodeTypes ret = TlmdoRsp.CodeTypes.TLMDO_CODE_OK;

            while (i_rtimeout <= NUMMAXTimeout)
            {
                try
                {
                    SnmpClient.SetInt(
                        targetIp,
                        Community,
                        u5ki.RemoteControlService.OIDs.Sirtap.TxPowerLevel, 
                        int.Parse(msg.PowerW),
                        SNMPCallTimeout,
                        Port,
                        SNMPVersion);
                }
                catch (Exception ex)
                {
                    if (ex is Lextm.SharpSnmpLib.Messaging.TimeoutException)
                    {
                        if (i_rtimeout >= NUMMAXTimeout)
                        {
                            string log = "[SNMP][" + "TLMDO_SET_TXPWR" + "] [" + targetIp + "] [" + msg.Channel + "] TIMEOUT ";
                            LogWarn<RCJotron7000>(log,
                                U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR,
                                Id, CTranslate.translateResource(log));
                            ret = TlmdoRsp.CodeTypes.TLMDO_CODE_NO_RESP;
                        }
                        i_rtimeout++;
                    }
                    else
                    {
                        string log = "[SNMP][" + "TLMDO_SET_TXPWR" + "] [" + targetIp + "] [" + msg.Channel + "] ERROR ";
                        LogWarn<RCJotron7000>(log,
                            U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR,
                            Id, CTranslate.translateResource(log));
                        ret = TlmdoRsp.CodeTypes.TLMDO_CODE_ERROR;
                    }
                }
            }

            return ret;
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
