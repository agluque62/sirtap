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
using System.Security.Cryptography;
using static System.Net.Mime.MediaTypeNames;
using System.Text.RegularExpressions;
using System.Runtime.Remoting.Channels;
using Lextm.SharpSnmpLib.Security;

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
        public int MAX_CRYPT_KEYS = 30;

        public u5ki.RemoteControlService.SNMPV3_priv_types Priv_type = u5ki.RemoteControlService.SNMPV3_priv_types.none;
        public u5ki.RemoteControlService.SNMPV3_auth_types Auth_type = u5ki.RemoteControlService.SNMPV3_auth_types.none;
        public string Username = null;
        public string Authpass = null;
        public string Privpass = null;
        IAuthenticationProvider Auth = null;
        IPrivacyProvider Priv = null;

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
        public Sirtap(Int32 port, VersionCode sNMPVersion,
            u5ki.RemoteControlService.SNMPV3_auth_types auth_type = SNMPV3_auth_types.none,
            u5ki.RemoteControlService.SNMPV3_priv_types priv_type = SNMPV3_priv_types.none,            
            string username = null, string authpass = null, string privpass = null)
        {
            Port = port;
            SNMPVersion = sNMPVersion;            
            if (sNMPVersion == VersionCode.V3)
            {
                if (auth_type != SNMPV3_auth_types.none && priv_type != SNMPV3_priv_types.none &&
                    username != null && privpass != null && authpass != null)
                {
                    switch (auth_type)
                    {
                        case SNMPV3_auth_types.SHA1:
                            Auth = new SHA1AuthenticationProvider(new OctetString("authkey1"));
                            break;
                        case SNMPV3_auth_types.MD5:
                            Auth = new MD5AuthenticationProvider(new OctetString("authkey1"));
                            break;
                    }
                    switch (priv_type)
                    {
                        case SNMPV3_priv_types.AES:
                            Priv = new AESPrivacyProvider(new OctetString("privkey1"), Auth);
                            break;
                        case SNMPV3_priv_types.DES:
                            Priv = new DESPrivacyProvider(new OctetString("privkey1"), Auth);
                            break;
                    }

                    Priv_type = priv_type;
                    Auth_type = auth_type;
                    Username = username;
                    Privpass = privpass;
                    Authpass = authpass;
                }
                else
                {
                    throw new Exception("SNMP V3 requires all parameters");
                }
            }
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
                    ret = SNMP_TLMDO_SET_CHANNEL(msg, targetIp);
                    break;
                case TlmdoAsk.MsgType.TLMDO_SET_FREQUENCY:
                    ret = SNMP_TLMDO_SET_FREQUENCY(msg, targetIp);
                    break;
                case TlmdoAsk.MsgType.TLMDO_SET_TXPWR:
                    if (isEmitter) ret = SNMP_TLMDO_SET_TXPWR(msg, targetIp);
                    else ret = TlmdoRsp.CodeTypes.TLMDO_CODE_OK;
                    break;
                case TlmdoAsk.MsgType.TLMDO_SET_TXINHIBIT:
                    ret = SNMP_TLMDO_SET_TXINHIBIT(msg, targetIp);
                    break;
                case TlmdoAsk.MsgType.TLMDO_SET_WFALLOC:
                    ret = SNMP_TLMDO_SET_WF(msg, targetIp);
                    break;
                case TlmdoAsk.MsgType.TLMDO_ERASE_CRYPT_KEYS:
                    ret = SNMP_TLMDO_ERASE_CRYPT_KEYS(msg, targetIp);
                    break;
                case TlmdoAsk.MsgType.TLMDO_LOAD_CRYPT_KEYS:
                    ret = SNMP_TLMDO_LOAD_CRYPT_KEYS(msg, targetIp);
                    break;
            }

            return ret;
        }

        private TlmdoRsp.CodeTypes SNMP_TLMDO_GET_CHANNELS(String targetIp, ref TlmdoRsp response)
        {
            int i_rtimeout = 0;
            TlmdoRsp.CodeTypes ret = TlmdoRsp.CodeTypes.TLMDO_CODE_OK;
            string log = "[SNMP][" + "TLMDO_GET_CHANNELS" + "] [" + targetIp + "]";
            int channels = 0;
            while (i_rtimeout <= NUMMAXTimeout)
            {
                try
                {
                    channels = SnmpClient.GetInt(targetIp, Community, u5ki.RemoteControlService.OIDs.Sirtap.GetChannels,
                        SNMPCallTimeout, Port, SNMPVersion, Username, Priv);
                    LogInfo<Sirtap>(log + " [CHANNELS" + channels + "] SUCCESS ",
                        U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_INFO,
                        Id, CTranslate.translateResource(log));
                    break;
                }
                catch (Exception ex)
                {
                    if (ex is Lextm.SharpSnmpLib.Messaging.TimeoutException)
                    {
                        if (i_rtimeout >= NUMMAXTimeout)
                        {
                            LogError<Sirtap>(log + " TIMEOUT ",
                                U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR,
                                Id, CTranslate.translateResource(log));
                            ret = TlmdoRsp.CodeTypes.TLMDO_CODE_NO_RESP;
                        }
                        i_rtimeout++;
                    }
                    else
                    {
                        LogError<Sirtap>(log + " " + ex.Message,
                            U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR,
                            Id, CTranslate.translateResource(log));
                        ret = TlmdoRsp.CodeTypes.TLMDO_CODE_ERROR;
                        break;
                    }
                }
            }

            response.NumChannels = channels;

            return ret;
        }

        private TlmdoRsp.CodeTypes SNMP_TLMDO_SET_CHANNEL(TlmdoAsk msg, String targetIp)
        {
            int i_rtimeout = 0;
            TlmdoRsp.CodeTypes ret = TlmdoRsp.CodeTypes.TLMDO_CODE_OK;
            string log = "[SNMP][" + "TLMDO_SET_CHANNEL" + "] [" + targetIp + "] [" + msg.Channel + "]";

            while (i_rtimeout <= NUMMAXTimeout)
            {
                try
                {
                    SnmpClient.SetInt(targetIp, Community, u5ki.RemoteControlService.OIDs.Sirtap.SetGetRadioChannel, msg.Channel,
                        SNMPCallTimeout, Port, SNMPVersion, username: Username, priv: Priv);
                    LogInfo<Sirtap>(log + " SUCCESS ",
                        U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_INFO,
                        Id, CTranslate.translateResource(log));
                    break;
                }
                catch (Exception ex)
                {
                    if (ex is Lextm.SharpSnmpLib.Messaging.TimeoutException)
                    {
                        if (i_rtimeout >= NUMMAXTimeout)
                        {
                            LogError<Sirtap>(log + " TIMEOUT ",
                                U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR,
                                Id, CTranslate.translateResource(log));
                            ret = TlmdoRsp.CodeTypes.TLMDO_CODE_NO_RESP;
                        }
                        i_rtimeout++;
                    }
                    else
                    {
                        LogError<Sirtap>(log + " " + ex.Message,
                            U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR,
                            Id, CTranslate.translateResource(log));
                        ret = TlmdoRsp.CodeTypes.TLMDO_CODE_ERROR;
                        break;
                    }
                }
            }           

            return ret;
        }

        private TlmdoRsp.CodeTypes SNMP_TLMDO_SET_FREQUENCY(TlmdoAsk msg, String targetIp)
        {
            int i_rtimeout = 0;
            TlmdoRsp.CodeTypes ret = TlmdoRsp.CodeTypes.TLMDO_CODE_OK;
            string log = "[SNMP][" + "TLMDO_SET_FREQUENCY" + "] [" + targetIp + "] [ CHANNEL " + msg.Channel + "] [ FREQ " + msg.Frequency + "]";

            while (i_rtimeout <= NUMMAXTimeout)
            {
                try
                {
                    string sfreq = Regex.Replace(msg.Frequency, "[^0-9]", "");
                    int ifreq = int.Parse(sfreq);
                    string oid;
                    if (msg.Channel == 0) oid = u5ki.RemoteControlService.OIDs.Sirtap.frequencyCh1;
                    else if (msg.Channel == 1) oid = u5ki.RemoteControlService.OIDs.Sirtap.frequencyCh2;
                    else
                    {
                        LogError<Sirtap>(log + " INVALID CHANNEL ",
                            U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR,
                            Id, CTranslate.translateResource(log));
                        ret = TlmdoRsp.CodeTypes.TLMDO_CODE_INVALID_OP;
                        break;
                    }
                    SnmpClient.SetInt(targetIp, Community, oid, ifreq, SNMPCallTimeout, Port, SNMPVersion, username: Username, priv: Priv);
                    LogInfo<Sirtap>(log + " SUCCESS ",
                        U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_INFO,
                        Id, CTranslate.translateResource(log));
                    break;
                }
                catch (Exception ex)
                {
                    if (ex is Lextm.SharpSnmpLib.Messaging.TimeoutException)
                    {
                        if (i_rtimeout >= NUMMAXTimeout)
                        {
                            LogError<Sirtap>(log + " TIMEOUT ",
                                U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR,
                                Id, CTranslate.translateResource(log));
                            ret = TlmdoRsp.CodeTypes.TLMDO_CODE_NO_RESP;
                        }
                        i_rtimeout++;
                    }
                    else
                    {
                        LogError<Sirtap>(log + " " + ex.Message,
                            U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR,
                            Id, CTranslate.translateResource(log));
                        ret = TlmdoRsp.CodeTypes.TLMDO_CODE_ERROR;
                        break;
                    }
                }
            }

            return ret;
        }

        private TlmdoRsp.CodeTypes SNMP_TLMDO_SET_TXPWR(TlmdoAsk msg, String targetIp)
        {
            int i_rtimeout = 0;
            TlmdoRsp.CodeTypes ret = TlmdoRsp.CodeTypes.TLMDO_CODE_OK;
            string log = "[SNMP][" + "TLMDO_SET_TXPWR" + "] [" + targetIp + "] [ CHANNEL " + msg.Channel + "] [ LEVEL " + msg.PowerW + "]";

            while (i_rtimeout <= NUMMAXTimeout)
            {
                try
                {
                    int ipwr = int.Parse(msg.PowerW);
                    string oid;
                    if (msg.Channel == 0) oid = u5ki.RemoteControlService.OIDs.Sirtap.powerLevelCh1;
                    else if (msg.Channel == 1) oid = u5ki.RemoteControlService.OIDs.Sirtap.powerLevelCh2;
                    else
                    {
                        LogError<Sirtap>(log + " INVALID CHANNEL ",
                            U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR,
                            Id, CTranslate.translateResource(log));
                        ret = TlmdoRsp.CodeTypes.TLMDO_CODE_INVALID_OP;
                        break;
                    }
                    SnmpClient.SetInt(targetIp, Community, oid, ipwr, SNMPCallTimeout, Port, SNMPVersion, username: Username, priv: Priv);
                    LogInfo<Sirtap>(log + " SUCCESS ",
                        U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_INFO,
                        Id, CTranslate.translateResource(log));
                    break;
                }
                catch (Exception ex)
                {
                    if (ex is Lextm.SharpSnmpLib.Messaging.TimeoutException)
                    {
                        if (i_rtimeout >= NUMMAXTimeout)
                        {
                            LogError<Sirtap>(log + " TIMEOUT ",
                                U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR,
                                Id, CTranslate.translateResource(log));
                            ret = TlmdoRsp.CodeTypes.TLMDO_CODE_NO_RESP;
                        }
                        i_rtimeout++;
                    }
                    else
                    {
                        LogError<Sirtap>(log + " " + ex.Message,
                            U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR,
                            Id, CTranslate.translateResource(log));
                        ret = TlmdoRsp.CodeTypes.TLMDO_CODE_ERROR;
                        break;
                    }
                }
            }

            return ret;
        }

        private TlmdoRsp.CodeTypes SNMP_TLMDO_SET_TXINHIBIT(TlmdoAsk msg, String targetIp)
        {
            int i_rtimeout = 0;
            TlmdoRsp.CodeTypes ret = TlmdoRsp.CodeTypes.TLMDO_CODE_OK;
            string log = "[SNMP][" + "TLMDO_SET_TXINHIBIT" + "] [" + targetIp + "]";

            while (i_rtimeout <= NUMMAXTimeout)
            {
                try
                {
                    int itxinhibit = msg.TxInhibit ? 1 : 0;
                    string oid = u5ki.RemoteControlService.OIDs.Sirtap.SetTxInhibit;
                    SnmpClient.SetInt(targetIp, Community, oid, itxinhibit, SNMPCallTimeout, Port, SNMPVersion, username: Username, priv: Priv);
                    LogInfo<Sirtap>(log + " SUCCESS ",
                        U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_INFO,
                        Id, CTranslate.translateResource(log));
                    break;
                }
                catch (Exception ex)
                {
                    if (ex is Lextm.SharpSnmpLib.Messaging.TimeoutException)
                    {
                        if (i_rtimeout >= NUMMAXTimeout)
                        {
                            LogError<Sirtap>(log + " TIMEOUT ",
                                U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR,
                                Id, CTranslate.translateResource(log));
                            ret = TlmdoRsp.CodeTypes.TLMDO_CODE_NO_RESP;
                        }
                        i_rtimeout++;
                    }
                    else
                    {
                        LogError<Sirtap>(log + " " + ex.Message,
                            U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR,
                            Id, CTranslate.translateResource(log));
                        ret = TlmdoRsp.CodeTypes.TLMDO_CODE_ERROR;
                        break;
                    }
                }
            }

            return ret;
        }

        private TlmdoRsp.CodeTypes SNMP_TLMDO_SET_WF(TlmdoAsk msg, String targetIp)
        {
            int i_rtimeout = 0;
            TlmdoRsp.CodeTypes ret = TlmdoRsp.CodeTypes.TLMDO_CODE_OK;
            string log = "[SNMP][" + "TLMDO_SET_WF" + "] [" + targetIp + "] [ CHANNEL " + msg.Channel + "] [ WF " + msg.WF + "]";

            while (i_rtimeout <= NUMMAXTimeout)
            {
                try
                {
                    string oid;
                    if (msg.Channel == 0) oid = u5ki.RemoteControlService.OIDs.Sirtap.WFCh1;
                    else if (msg.Channel == 1) oid = u5ki.RemoteControlService.OIDs.Sirtap.WFCh2;
                    else
                    {
                        LogError<Sirtap>(log + " INVALID CHANNEL ",
                            U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR,
                            Id, CTranslate.translateResource(log));
                        ret = TlmdoRsp.CodeTypes.TLMDO_CODE_INVALID_OP;
                        break;
                    }
                    SnmpClient.SetString(targetIp, Community, oid, msg.WF, SNMPCallTimeout, Port, SNMPVersion, username: Username, priv: Priv);
                    string WF_read = SnmpClient.GetString(targetIp, Community, oid, SNMPCallTimeout, Port, SNMPVersion, username: Username, priv: Priv);
                    if (WF_read != msg.WF)
                    {
                        throw new Exception();
                    }

                    LogInfo<Sirtap>(log + " SUCCESS ",
                        U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_INFO,
                        Id, CTranslate.translateResource(log));
                    break;
                }
                catch (Exception ex)
                {
                    if (ex is Lextm.SharpSnmpLib.Messaging.TimeoutException)
                    {
                        if (i_rtimeout >= NUMMAXTimeout)
                        {
                            LogError<Sirtap>(log + " TIMEOUT ",
                                U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR,
                                Id, CTranslate.translateResource(log));
                            ret = TlmdoRsp.CodeTypes.TLMDO_CODE_NO_RESP;
                        }
                        i_rtimeout++;
                    }
                    else
                    {
                        LogError<Sirtap>(log + " " + ex.Message,
                            U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR,
                            Id, CTranslate.translateResource(log));
                        ret = TlmdoRsp.CodeTypes.TLMDO_CODE_ERROR;
                        break;
                    }
                }
            }

            return ret;
        }

        private TlmdoRsp.CodeTypes SNMP_TLMDO_ERASE_CRYPT_KEYS(TlmdoAsk msg, String targetIp)
        {
            int i_rtimeout = 0;
            TlmdoRsp.CodeTypes ret = TlmdoRsp.CodeTypes.TLMDO_CODE_OK;
            string log = "[SNMP][" + "TLMDO_ERASE_CRYPT_KEYS" + "] [" + targetIp + "]";

            while (i_rtimeout <= NUMMAXTimeout)
            {
                try
                {
                    SnmpClient.SetInt(targetIp, Community, u5ki.RemoteControlService.OIDs.Sirtap.eraseKeys, MAX_CRYPT_KEYS,
                        SNMPCallTimeout, Port, SNMPVersion, username: Username, priv: Priv);
                    LogInfo<Sirtap>(log + " SUCCESS ",
                        U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_INFO,
                        Id, CTranslate.translateResource(log));
                    break;
                }
                catch (Exception ex)
                {
                    if (ex is Lextm.SharpSnmpLib.Messaging.TimeoutException)
                    {
                        if (i_rtimeout >= NUMMAXTimeout)
                        {
                            LogError<Sirtap>(log + " TIMEOUT ",
                                U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR,
                                Id, CTranslate.translateResource(log));
                            ret = TlmdoRsp.CodeTypes.TLMDO_CODE_NO_RESP;
                        }
                        i_rtimeout++;
                    }
                    else
                    {
                        LogError<Sirtap>(log + " " + ex.Message,
                            U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR,
                            Id, CTranslate.translateResource(log));
                        ret = TlmdoRsp.CodeTypes.TLMDO_CODE_ERROR;
                        break;
                    }
                }
            }

            return ret;
        }

        private TlmdoRsp.CodeTypes SNMP_TLMDO_LOAD_CRYPT_KEYS(TlmdoAsk msg, String targetIp)
        {
            int i_rtimeout = 0;
            TlmdoRsp.CodeTypes ret = TlmdoRsp.CodeTypes.TLMDO_CODE_OK;
            string log = "[SNMP][" + "TLMDO_ERASE_LOAD_KEYS" + "] [" + targetIp + "]";

            while (i_rtimeout <= NUMMAXTimeout)
            {
                try
                {
                    SnmpClient.SetInt(targetIp, Community, u5ki.RemoteControlService.OIDs.Sirtap.loadKeys, MAX_CRYPT_KEYS,
                        SNMPCallTimeout, Port, SNMPVersion, username: Username, priv: Priv);
                    LogInfo<Sirtap>(log + " SUCCESS ",
                        U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_INFO,
                        Id, CTranslate.translateResource(log));
                    break;
                }
                catch (Exception ex)
                {
                    if (ex is Lextm.SharpSnmpLib.Messaging.TimeoutException)
                    {
                        if (i_rtimeout >= NUMMAXTimeout)
                        {
                            LogError<Sirtap>(log + " TIMEOUT ",
                                U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR,
                                Id, CTranslate.translateResource(log));
                            ret = TlmdoRsp.CodeTypes.TLMDO_CODE_NO_RESP;
                        }
                        i_rtimeout++;
                    }
                    else
                    {
                        LogError<Sirtap>(log + " " + ex.Message,
                            U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR,
                            Id, CTranslate.translateResource(log));
                        ret = TlmdoRsp.CodeTypes.TLMDO_CODE_ERROR;
                        break;
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
