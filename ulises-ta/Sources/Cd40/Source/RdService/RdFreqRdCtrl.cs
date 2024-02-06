﻿/********************************************************************************
 * 
 * Se implementan las funciones de la clase RdFrecuency correspondientes a control
 * de los equipos de radio. Como por ejemplo el cambio de frecuencia
 * 
 * ******************************************************************************/

using NLog.Targets;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Translate;
using u5ki.RemoteControlService;
using u5ki.RemoteControlService.OIDs;
using U5ki.Enums;
using U5ki.Infrastructure;
using Utilities;
using static U5ki.Infrastructure.TlmdoAsk;
using static U5ki.RdService.RdResource;

namespace U5ki.RdService
{
    public partial class RdFrecuency : BaseCode, IDisposable
    {
        class subProcessChangingFreq_thread
        {
            public string from;
            public FrChangeAsk msg;
            public bool result;
            public uint code_returned;
            public string selectedFrequency;
        }

        private bool ProcessChangingFreqRunning = false;
        
        public bool ProcessChangingFreq(string from, FrChangeAsk msg)
        {
            bool ret = true;

            if (!_MultifreqSupported)
            {
                LogError<RdFrecuency>("Cambiando la frecuencia. " + this.Frecuency + " no soporta Multifrecuencia.");
                return ret;
            }

            bool allResourcesHaveTelecontrol = true;
            foreach (IRdResource res in RdRs.Values)
            {
                if (res is RdResourcePair)
                {
                    RdResourcePair respair = res as RdResourcePair;
                    if (respair.ActiveResource.TelemandoType == RdResource.TelemandoTypes.none)
                    {
                        allResourcesHaveTelecontrol = false;
                        break;
                    }
                    if (respair.StandbyResource.TelemandoType == RdResource.TelemandoTypes.none)
                    {
                        allResourcesHaveTelecontrol = false;
                        break;
                    }
                }
                else
                {
                    if ((res as RdResource).TelemandoType == RdResource.TelemandoTypes.none)
                    {
                        allResourcesHaveTelecontrol = false;
                        break;
                    }
                }
            }
            if (!allResourcesHaveTelecontrol)
            {
                LogError<RdFrecuency>("No se puede cambiar de frecuencia porque algun equipo no soporta telemando. " + msg.NewFrecuency,
                                       U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR, this.Frecuency,
                                       CTranslate.translateResource("No se puede cambiar de frecuencia porque algun equipo no soporta telemando. " + msg.NewFrecuency));
                RdRegistry.RespondToChangingFreq(from, false, msg.IdFrecuency, msg.NewFrecuency, Identifiers.FR_CH_REJECTED);
                return ret;
            }

            if (ProcessChangingFreqRunning)
            {
                RdRegistry.RespondToChangingFreq(from, false, msg.IdFrecuency, msg.NewFrecuency, Identifiers.FR_IN_PROGRESS);
                return ret;
            }
            ProcessChangingFreqRunning = true;

            Func<string, FrChangeAsk, subProcessChangingFreq_thread> subProcessChangingFreq = SubProcessChangingFreq;
            subProcessChangingFreq.BeginInvoke(from, msg, SubProcessChangingFreqProcessed, subProcessChangingFreq);

            return ret;
        }

        private subProcessChangingFreq_thread SubProcessChangingFreq(string from, FrChangeAsk msg)
        {
            subProcessChangingFreq_thread ret = new subProcessChangingFreq_thread();

            ret.from = from;
            ret.msg = msg;
            ret.result = true;
            ret.code_returned = Identifiers.FR_CHANGE_OK;
            ret.selectedFrequency = msg.NewFrecuency;

            if (from != null && (_TxIds.Count > 0 || _RxIds.Count > 0))
            {
                //Si from es null, entonces se invoka la tarea desde el timer y no por un mensaje
                //Entonces no mandamos respuesta
                ret.result = false;
                ret.code_returned = Identifiers.FR_IS_IN_USE;
                ret.selectedFrequency = SelectedFrequency;
                return ret;
            }

            if (from != null)
            {
                //Si from es null, entonces se invoka la tarea desde el timer no por un mensaje
                //No ponemos aspas en el destino
                RdService.evQueueRd.Enqueue("SubProcessChangingFreq", delegate ()
                {
                    RdRegistry.Publish(_IdDestino, null);
                });
            }

            bool only_not_tuned_resources = (from == null) ? true : false;
            uint returned_code = ChangeFreqInAllResources(msg.NewFrecuency, only_not_tuned_resources);
            if (returned_code != Identifiers.FR_CHANGE_OK)
            {
                if (from != null)
                {
                    //Si from es null, entonces se invoka la tarea desde el timer no por un mensaje
                    LogError<RdFrecuency>("Error al intentar sintonizar los equipos. " + msg.NewFrecuency,
                                        U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR, this.Frecuency,
                                        CTranslate.translateResource("Error al intentar sintonizar los equipos. " + msg.NewFrecuency));
                }

                if (SelectedFrequency != msg.NewFrecuency)
                {
                    ret.result = false;
                    ret.code_returned = returned_code;
                    ret.selectedFrequency = SelectedFrequency;
                    returned_code = ChangeFreqInAllResources(SelectedFrequency, only_not_tuned_resources);
                    if (returned_code != Identifiers.FR_CHANGE_OK)
                    {
                        if (from != null)
                        {
                            //Si from es null, entonces se invoka la tarea desde el timer no por un mensaje
                            LogError<RdFrecuency>("No se puede volver a sintonizar la frecuencia " + SelectedFrequency,
                                        U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR, this.Frecuency,
                                        CTranslate.translateResource("No se puede volver a sintonizar la frecuencia " + SelectedFrequency));
                        }
                    }
                    else
                    {
                        if (from != null)
                        {
                            //Si from es null, entonces se invoka la tarea desde el timer no por un mensaje
                            LogInfo<RdFrecuency>("Equipos resintonizados a la frecuencia" + SelectedFrequency,
                                        U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_INFO, this.Frecuency,
                                        CTranslate.translateResource("Equipos resintonizados a la frecuencia " + SelectedFrequency));
                        }
                    }
                }
            }
            else
            {
                if (from != null)
                {
                    //Si from es null, entonces se invoka la tarea desde el timer no por un mensaje
                    LogInfo<RdFrecuency>("Equipos sintonizados a la frecuencia " + msg.NewFrecuency,
                                    U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_INFO, this.Frecuency,
                                    CTranslate.translateResource("Equipos sintonizados a la frecuencia " + msg.NewFrecuency));
                }
            }

            Update_1plus1_status();

            return ret;
        }

        private void SubProcessChangingFreqProcessed(IAsyncResult cookie)
        {
            var target = (Func<string, FrChangeAsk, subProcessChangingFreq_thread>)cookie.AsyncState;

            subProcessChangingFreq_thread resultProcess = target.EndInvoke(cookie);

            if (resultProcess != null)
            {
                RdService.evQueueRd.Enqueue("SubProcessChangingFreqProcessed", delegate ()
                {
                    RdRegistry.RespondToChangingFreq(resultProcess.from, resultProcess.result,
                    resultProcess.msg.IdFrecuency, resultProcess.selectedFrequency, resultProcess.code_returned);

                    SelectedFrequency = resultProcess.selectedFrequency;
                    if (_FrRs != null)
                    {
                        _FrRs.SelectedFrequency = resultProcess.selectedFrequency;
                        _FrRs.FrequencyStatus = this.Status;

                        RdRegistry.Publish(_IdDestino, _FrRs);
                        if (_FrRs.FrequencyStatus == RdSrvFrRs.FrequencyStatusType.NotAvailable)
                            RdRegistry.Publish(_IdDestino, null);
                    }
                    ProcessChangingFreqRunning = false;
                    MSTxPersistence.SelectFrequency(_IdDestino, resultProcess.selectedFrequency);
                });
            }
        }

        private uint ChangeFreqInAllResources(string newFrequency, bool only_not_tuned_resources)
        {
            uint returned_code = Identifiers.FR_CHANGE_OK;

            foreach (IRdResource res in RdRs.Values)
            {
                if (res is RdResourcePair)
                {
                    RdResourcePair respair = (RdResourcePair)res;
                    uint ret_code_active = Identifiers.FR_CHANGE_OK;
                    uint ret_code_standby = Identifiers.FR_CHANGE_OK;

                    if (respair.ActiveResource.TelemandoType == RdResource.TelemandoTypes.none)
                    {
                        LogError<RdFrecuency>("Equipo configurado sin telemando. No se puede sintonizar. " + respair.ActiveResource.ID,
                            U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR, this.Frecuency,
                            CTranslate.translateResource("Equipo configurado sin telemando. No se puede sintonizar. " + respair.ActiveResource.ID));
                    }
                    else if (!only_not_tuned_resources || (only_not_tuned_resources && !respair.ActiveResource.TunedFrequencyOK))
                    {
                        ret_code_active = ChangeFreqInAResource(respair.ActiveResource, newFrequency);
                    }

                    if (respair.StandbyResource.TelemandoType == RdResource.TelemandoTypes.none)
                    {
                        LogError<RdFrecuency>("Equipo configurado sin telemando. No se puede sintonizar. " + respair.StandbyResource.ID,
                            U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR, this.Frecuency,
                            CTranslate.translateResource("Equipo configurado sin telemando. No se puede sintonizar. " + respair.StandbyResource.ID));
                    }
                    else if (!only_not_tuned_resources || (only_not_tuned_resources && !respair.StandbyResource.TunedFrequencyOK))
                    {
                        ret_code_standby = ChangeFreqInAResource(respair.StandbyResource, newFrequency);
                    }

                    //ret_code_standby = Identifiers.FR_CH_REJECTED;                    
                    //respair.StandbyResource.TunedFrequencyOK = false;
                    //ret_code_active = Identifiers.FR_CHANGE_OK;
                    //respair.ActiveResource.TunedFrequencyOK = true;

                    if (ret_code_active == Identifiers.FR_CHANGE_OK && ret_code_standby == Identifiers.FR_CHANGE_OK)
                    {
                        if (ret_code_active > returned_code) returned_code = ret_code_active;
                    }
                    else if (ret_code_active == Identifiers.FR_CHANGE_OK && ret_code_standby != Identifiers.FR_CHANGE_OK)
                    {
                        if (ret_code_active > returned_code) returned_code = ret_code_active;
                    }
                    else if (ret_code_active != Identifiers.FR_CHANGE_OK && ret_code_standby == Identifiers.FR_CHANGE_OK)
                    {
                        if (ret_code_standby > returned_code) returned_code = ret_code_standby;
                    }
                    else 
                    {
                        if (ret_code_active > returned_code) returned_code = ret_code_active;
                    }
                }
                else
                {
                    if ((res as RdResource).TelemandoType == RdResource.TelemandoTypes.none)
                    {
                        LogError<RdFrecuency>("Equipo configurado sin telemando. No se puede sintonizar. " + res.ID,
                            U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR, this.Frecuency,
                            CTranslate.translateResource("Equipo configurado sin telemando. No se puede sintonizar. " + res.ID));
                    }
                    else if (!only_not_tuned_resources || (only_not_tuned_resources && !res.TunedFrequencyOK))
                    {
                        uint ret_code = ChangeFreqInAResource(res as RdResource, newFrequency);
                        if (ret_code > returned_code) returned_code = ret_code;
                    }
                }
            }

            return returned_code;
        }

        private void Update_1plus1_status()
        {
            foreach (IRdResource res in RdRs.Values)
            {
                if (res is RdResourcePair)
                {
                    RdResourcePair respair = (RdResourcePair)res;

                    if (respair.ActiveResource.TunedFrequencyOK && respair.StandbyResource.TunedFrequencyOK)
                    {
                    }
                    else if (respair.ActiveResource.TunedFrequencyOK && !respair.StandbyResource.TunedFrequencyOK)
                    {
                        if (respair.StandbyResource.isRx)
                        {
                            RdService.evQueueRd.Enqueue("ChangeFreqInAllResources", delegate ()
                            {
                                if (!MSTxPersistence.IsNodeDisabled(respair.StandbyResource))
                                {
                                    MSTxPersistence.DisableNode(respair.StandbyResource, true);
                                    string msg = "Equipo del grupo 1+1 esta deshabilitado. No se puede establecer ni leer su frecuencia por SNMP. " + respair.StandbyResource.ID + ", 1+1: " + res.ID;
                                    LogError<RdService>(msg,
                                        U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR,
                                        Frecuency,
                                        CTranslate.translateResource(msg));
                                }
                            });
                        }
                    }
                    else if (!respair.ActiveResource.TunedFrequencyOK && respair.StandbyResource.TunedFrequencyOK)
                    {
                        if (respair.isTx)
                        {
                            RdService.evQueueRd.Enqueue("ChangeFreqInAllResources", delegate ()
                            {
                                respair.ActivateResource(respair.StandbyResource.ID);
                            });
                        }
                        if (respair.isRx)
                        {
                            RdService.evQueueRd.Enqueue("ChangeFreqInAllResources", delegate ()
                            {
                                if (!MSTxPersistence.IsNodeDisabled(respair.ActiveResource))
                                {
                                    MSTxPersistence.DisableNode(respair.ActiveResource, true);
                                    string msg = "Equipo del grupo 1+1 esta deshabilitado. No se puede establecer ni leer su frecuencia por SNMP. " + respair.ActiveResource.ID + ", 1+1: " + res.ID;
                                    LogError<RdService>(msg,
                                        U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR,
                                        Frecuency,
                                        CTranslate.translateResource(msg));
                                }
                            });
                        }
                    }
                    else
                    {
                        if (respair.isRx)
                        {
                            RdService.evQueueRd.Enqueue("ChangeFreqInAllResources", delegate ()
                            {
                                if (!MSTxPersistence.IsNodeDisabled(respair.ActiveResource))
                                {
                                    MSTxPersistence.DisableNode(respair.ActiveResource, true);
                                    string msg = "Equipo del grupo 1+1 esta deshabilitado. No se puede establecer ni leer su frecuencia por SNMP. " + respair.ActiveResource.ID + ", 1+1: " + res.ID;
                                    LogError<RdService>(msg,
                                        U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR,
                                        Frecuency,
                                        CTranslate.translateResource(msg));
                                }

                                if (!MSTxPersistence.IsNodeDisabled(respair.StandbyResource))
                                {
                                    MSTxPersistence.DisableNode(respair.StandbyResource, true);
                                    string msg = "Equipo del grupo 1+1 esta deshabilitado. No se puede establecer ni leer su frecuencia por SNMP. " + respair.StandbyResource.ID + ", 1+1: " + res.ID;
                                    LogError<RdService>(msg,
                                        U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR,
                                        Frecuency,
                                        CTranslate.translateResource(msg));
                                }
                            });
                        }
                    }
                }
                else
                {
                }
            }
        }

        private bool CheckTunedFreqInAllResources()
        {
            bool all_tuned = true;

            if (ProcessChangingFreqRunning) return all_tuned;

            if (!_MultifreqSupported)
            {
                //No es una frecuencia sintonizable y no hay que chequear
                return all_tuned;
            }

            if (SelectedFrequency.Length == 0)
            {
                return all_tuned;
            }

            foreach (IRdResource res in RdRs.Values)
            {
                if (res is RdResourcePair)
                {
                    RdResourcePair resourcePair = (RdResourcePair)res;

                    if (resourcePair.ActiveResource.TelemandoType != RdResource.TelemandoTypes.none)
                    {
                        string current_freq = GetFrecuencyBySNMP(resourcePair.ActiveResource);
                        if (current_freq == null)
                        {
                            resourcePair.ActiveResource.TunedFrequencyOK = false;
                            all_tuned = false;
                        }
                        string selectedFreq = SelectedFrequency;
                        selectedFreq = selectedFreq.Replace(".", "");
                        selectedFreq = selectedFreq.Replace(",", "");
                        if (selectedFreq != current_freq)
                        {
                            resourcePair.ActiveResource.TunedFrequencyOK = false;
                            all_tuned = false;
                        }
                    }

                    if (resourcePair.StandbyResource.TelemandoType != RdResource.TelemandoTypes.none)
                    {
                        string current_freq = GetFrecuencyBySNMP(resourcePair.StandbyResource);
                        if (current_freq == null)
                        {
                            resourcePair.StandbyResource.TunedFrequencyOK = false;
                            all_tuned = false;
                        }
                        string selectedFreq = SelectedFrequency;
                        selectedFreq = selectedFreq.Replace(".", "");
                        selectedFreq = selectedFreq.Replace(",", "");
                        if (selectedFreq != current_freq)
                        {
                            resourcePair.StandbyResource.TunedFrequencyOK = false;
                            all_tuned = false;
                        }
                    }
                }
                else
                {
                    if ((res as RdResource).TelemandoType != RdResource.TelemandoTypes.none)
                    {
                        string current_freq = GetFrecuencyBySNMP(res as RdResource);
                        if (current_freq == null)
                        {
                            res.TunedFrequencyOK = false;
                            all_tuned = false;
                        }
                        string selectedFreq = SelectedFrequency;
                        selectedFreq = selectedFreq.Replace(".", "");
                        selectedFreq = selectedFreq.Replace(",", "");
                        if (selectedFreq != current_freq)
                        {
                            res.TunedFrequencyOK = false;
                            all_tuned = false;
                        }
                    }
                }
            }

            if (!all_tuned)
            {
                FrChangeAsk msg = new FrChangeAsk();
                msg.NewFrecuency = SelectedFrequency;
                ProcessChangingFreq(null, msg);
            }

            return all_tuned;
        }

        private string GetFrecuencyBySNMP(RdResource res)
        {
            //El puerto SNMP es 161, excepto los receptores de Rhode que es 160
            string current_freq = "000.000";
            SipUtilities.SipUriParser sipuri = new SipUtilities.SipUriParser(res.Uri1);
            bool isEmitter = res.isTx;

            switch (res.TelemandoType)
            {
                case RdResource.TelemandoTypes.RCRohde4200:
                    {
                        u5ki.RemoteControlService.RCRohde4200 RC;
                        if (isEmitter)
                        {
                            RC = new u5ki.RemoteControlService.RCRohde4200(161) { Id = res.ID };
                        }
                        else
                        {
                            RC = new u5ki.RemoteControlService.RCRohde4200(160) { Id = res.ID };
                        }
                        current_freq = RC.SNMPFrecuencyGet(sipuri.Host);
                    }
                    break;
                case RdResource.TelemandoTypes.RCJotron7000:
                    {
                        u5ki.RemoteControlService.RCJotron7000 RC = new u5ki.RemoteControlService.RCJotron7000(161) { Id = res.ID };
                        current_freq = RC.SNMPFrecuencyGet(sipuri.Host, isEmitter);
                    }
                    break;
                case RdResource.TelemandoTypes.none:
                    LogError<RdFrecuency>("GetFrecuencyBySNMP: Resource " + res.ID + " no soporta Telemando");
                    break;
            }        
            
            return current_freq;        
        }

        private uint ChangeFreqInAResource(RdResource res ,string newFrequency)
        {            
            uint returned_code = Identifiers.FR_CHANGE_OK;

            //El puerto SNMP es 161, excepto los receptores de Rhode que es 160

            SipUtilities.SipUriParser sipuri = new SipUtilities.SipUriParser(res.Uri1);
            bool isEmitter = res.isTx;
            GearOperationStatus output = GearOperationStatus.Fail;

            switch (res.TelemandoType)
            {
                case RdResource.TelemandoTypes.RCRohde4200:
                    {
                        u5ki.RemoteControlService.RCRohde4200 RC;
                        if (isEmitter)
                        {
                            RC = new u5ki.RemoteControlService.RCRohde4200(161) { Id = res.ID };                            
                        }
                        else
                        {
                            RC = new u5ki.RemoteControlService.RCRohde4200(160) { Id = res.ID };
                        }
                        output = RC.SNMPFrecuencySet(sipuri.Host, newFrequency, res.Connected);
                    }
                    break;
                case RdResource.TelemandoTypes.RCJotron7000:
                    {
                        u5ki.RemoteControlService.RCJotron7000 RC = new u5ki.RemoteControlService.RCJotron7000(161) { Id = res.ID };
                        output = RC.SNMPFrecuencySet(sipuri.Host, newFrequency, isEmitter);
                    }
                    break;
                case RdResource.TelemandoTypes.none:
                    LogError<RdFrecuency>("ChangeFreqInAResource: Resource " + res.ID + " no soporta Telemando");
                    return returned_code;
            }            

            string log_;
            switch (output)
            {
                case GearOperationStatus.OK:
                    res.TunedFrequencyOK = true;

                    log_ = "Frecuencia del equipo sintonizada con exito. " + "Eq: " + res.ID + " FR: " + newFrequency;
                    if (log_ != res.Last_tuned_freq_log)
                    {
                        LogInfo<RdFrecuency>(log_,
                            U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_INFO, this.Frecuency,
                            CTranslate.translateResource(log_));
                        res.Last_tuned_freq_log = log_;
                    }

                    break;
                case GearOperationStatus.Fail:
                case GearOperationStatus.Rejected:
                    returned_code = Identifiers.FR_CH_REJECTED;
                    res.TunedFrequencyOK = false;

                    log_ = "Frecuencia del equipo no puede ser sintonizada. Rechazada. " + "Eq: " + res.ID + " FR: " + newFrequency;
                    if (log_ != res.Last_tuned_freq_log)
                    {
                        LogError<RdFrecuency>(log_,
                            U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR, this.Frecuency,
                            CTranslate.translateResource(log_));
                        res.Last_tuned_freq_log = log_;
                    }

                    break;
                case GearOperationStatus.Timeout:
                    returned_code = Identifiers.FR_EQ_NO_RESPOND;
                    res.TunedFrequencyOK = false;

                    log_ = "Frecuencia del equipo no puede ser sintonizada. Timeout. " + "Eq: " + res.ID + " FR: " + newFrequency;
                    if (log_ != res.Last_tuned_freq_log)
                    {
                        LogError<RdFrecuency>(log_,
                        U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR, this.Frecuency,
                        CTranslate.translateResource(log_));
                        res.Last_tuned_freq_log = log_;
                    }

                    break;
                default:
                    returned_code = Identifiers.FR_GENERIC_ERROR;
                    res.TunedFrequencyOK = false;

                    log_ = "Frecuencia del equipo no puede ser sintonizada. Error desconocido. " + "Eq: " + res.ID + " FR: " + newFrequency;
                    if (log_ != res.Last_tuned_freq_log)
                    {
                        LogError<RdFrecuency>(log_,
                        U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR, this.Frecuency,
                        CTranslate.translateResource(log_));
                        res.Last_tuned_freq_log = log_;
                    }

                    break;
            }

            return returned_code;
        }

        //-----------------------------------------------------------------------------------------------------------------
        //-----------------------------------------------------------------------------------------------------------------

        class subProcessTlmdoAsk_thread
        {
            public string from;
            public TlmdoAsk msg;
            public TlmdoRsp response;
        }

        private bool ProcessTlmdoRunning = false;

        public bool ProcessTlmdoAsk(string from, TlmdoAsk msg)
        {
            bool ret = true;

            TlmdoRsp response = new TlmdoRsp();
            response.msgType = msg.msgType;
            response.resultado = false;
            response.IdFrecuency = msg.IdFrecuency;
            response.IdRecurso = msg.IdRecurso;
            response.NumChannels = 0;
            response.Code = TlmdoRsp.CodeTypes.TLMDO_CODE_INVALID_OP;

            bool allResourcesHaveTelecontrol = true;
            bool onlyTxResources = false;

            if (msg.msgType == TlmdoAsk.MsgType.TLMDO_SET_TXPWR || 
                msg.msgType == TlmdoAsk.MsgType.TLMDO_SET_TXINHIBIT ||
                msg.msgType == TlmdoAsk.MsgType.TLMDO_ERASE_CRYPT_KEYS ||
                msg.msgType == TlmdoAsk.MsgType.TLMDO_LOAD_CRYPT_KEYS)
            {
                onlyTxResources = true;     //Este telemando solo se aplica a los del tipo Tx
            }

            if (msg.IdRecurso.Length > 0)
            {
                //Solo se aplica el telemando a un recurso
                bool not_found = true;
                foreach (IRdResource res in RdRs.Values)
                {
                    if (msg.IdRecurso == res.ID)
                    {
                        not_found = false;
                        if (onlyTxResources && !res.isTx)
                        {
                            LogError<RdFrecuency>($"El telemando {msg.msgType.ToString()} no se puede aplicar en el recurso {res.ID} porque no es transmisor.");
                            LogError<RdFrecuency>("No se puede aplicar telemando porque algun equipo no lo soporta.",
                                       U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR, this.Frecuency,
                                       CTranslate.translateResource("No se puede aplicar telemando porque algun equipo no lo soporta."));
                            RdRegistry.RespondToTlmdo(from, response);
                            return ret;
                        }
                        if (res is RdResourcePair)
                        {
                            RdResourcePair respair = res as RdResourcePair;
                            if (respair.ActiveResource.TelemandoType == RdResource.TelemandoTypes.none)
                            {
                                allResourcesHaveTelecontrol = false;
                            }
                            if (respair.StandbyResource.TelemandoType == RdResource.TelemandoTypes.none)
                            {
                                allResourcesHaveTelecontrol = false;
                            }
                        }
                        else
                        {
                            if ((res as RdResource).TelemandoType == RdResource.TelemandoTypes.none)
                            {
                                allResourcesHaveTelecontrol = false;
                            }
                        }
                        break;
                    }
                }
                if (!allResourcesHaveTelecontrol || not_found)
                {
                    LogError<RdFrecuency>($"El telemando {msg.msgType} no se puede aplicar en el recurso {msg.IdRecurso} porque no soporta telemando o no pertenece al destino.");
                    LogError<RdFrecuency>("No se puede aplicar telemando porque algun equipo no lo soporta.",
                                       U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR, this.Frecuency,
                                       CTranslate.translateResource("No se puede aplicar telemando porque algun equipo no lo soporta."));
                    RdRegistry.RespondToTlmdo(from, response);
                    return ret;
                }
            }
            else
            {
                foreach (IRdResource res in RdRs.Values)
                {
                    if (onlyTxResources && !res.isTx) continue;

                    if (res is RdResourcePair)
                    {
                        RdResourcePair respair = res as RdResourcePair;
                        if (respair.ActiveResource.TelemandoType == RdResource.TelemandoTypes.none)
                        {
                            allResourcesHaveTelecontrol = false;
                            break;
                        }
                        if (respair.StandbyResource.TelemandoType == RdResource.TelemandoTypes.none)
                        {
                            allResourcesHaveTelecontrol = false;
                            break;
                        }
                    }
                    else
                    {
                        if ((res as RdResource).TelemandoType == RdResource.TelemandoTypes.none)
                        {
                            allResourcesHaveTelecontrol = false;
                            break;
                        }
                    }
                }
            }
            if (!allResourcesHaveTelecontrol)
            {
                LogError<RdFrecuency>("No se puede aplicar telemando porque algun equipo no lo soporta.",
                                       U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR, this.Frecuency,
                                       CTranslate.translateResource("No se puede aplicar telemando porque algun equipo no lo soporta."));
                
                RdRegistry.RespondToTlmdo(from, response);
                return ret;
            }

            if (ProcessTlmdoRunning)
            {
                response.Code = TlmdoRsp.CodeTypes.TLMDO_CODE_OTHER_IN_PROGRESS;
                RdRegistry.RespondToTlmdo(from, response);
                return ret;
            }
            ProcessTlmdoRunning = true;

            Func<string, TlmdoAsk, subProcessTlmdoAsk_thread> subProcessTlmdo = SubProcessTlmdo;
            subProcessTlmdo.BeginInvoke(from, msg, SubProcessTlmdoProcessed, subProcessTlmdo);

            return ret;
        }

        private subProcessTlmdoAsk_thread SubProcessTlmdo(string from, TlmdoAsk msg)
        {
            subProcessTlmdoAsk_thread ret = new subProcessTlmdoAsk_thread();

            TlmdoRsp response = new TlmdoRsp();
            response.msgType = msg.msgType;
            response.resultado = false;
            response.IdFrecuency = msg.IdFrecuency;
            response.IdRecurso = msg.IdRecurso;
            response.NumChannels = 0;
            response.Code = TlmdoRsp.CodeTypes.TLMDO_CODE_OK;

            ret.from = from;
            ret.msg = msg;
            ret.response = response;

            if (from != null && (_TxIds.Count > 0 || _RxIds.Count > 0))
            {
                //La frecuencia esta seleccionada en Tx o Rx. Entonces no se puede ejecutar
                //Si from es null, entonces se invoka la tarea desde el timer y no por un mensaje
                //Entonces no mandamos respuesta
                response.Code = TlmdoRsp.CodeTypes.TLMDO_CODE_INVALID_OP;
                ret.from = from;
                ret.msg = msg;
                ret.response = response;
                return ret;
            }

            if (from != null)
            {
                //Si from es null, entonces se invoka la tarea desde el timer no por un mensaje
                //No ponemos aspas en el destino
                RdService.evQueueRd.Enqueue("SubProcessTlmdo", delegate ()
                {
                    RdRegistry.Publish(_IdDestino, null);
                });
            }

            TlmdoRsp.CodeTypes ret_tlmdo = TmdoInAllResources(msg, ref response);
            if (ret_tlmdo != TlmdoRsp.CodeTypes.TLMDO_CODE_OK)
            {
                LogError<RdFrecuency>("Telemando. Ejecutado con error en algún equipo.",
                                        U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR, this.Frecuency,
                                        CTranslate.translateResource("Telemando. Ejecutado con error en algún equipo."));
            }

            response.Code = ret_tlmdo;

            return ret;
        }

        private void SubProcessTlmdoProcessed(IAsyncResult cookie)
        {
            var target = (Func<string, TlmdoAsk, subProcessTlmdoAsk_thread>)cookie.AsyncState;

            subProcessTlmdoAsk_thread resultProcess = target.EndInvoke(cookie);

            if (resultProcess != null)
            {
                RdService.evQueueRd.Enqueue("SubProcessTlmdoProcessed", delegate ()
                {
                    RdRegistry.RespondToTlmdo(resultProcess.from, resultProcess.response);
                    ProcessTlmdoRunning = false;
                });
            }
        }

        private TlmdoRsp.CodeTypes TmdoInAllResources(TlmdoAsk msg, ref TlmdoRsp response)
        {
            TlmdoRsp.CodeTypes returned_code = TlmdoRsp.CodeTypes.TLMDO_CODE_OK;

            foreach (IRdResource res in RdRs.Values)
            {
                if (msg.IdRecurso.Length > 0 && res.ID != msg.IdRecurso)
                {
                    //El telemando solo se aplica a un recurso cuando IdRecurso del mensaje no esta vacio.
                    continue;
                }
                if (res is RdResourcePair)
                {
                    RdResourcePair respair = (RdResourcePair)res;
                    TlmdoRsp.CodeTypes ret_code_active = TlmdoRsp.CodeTypes.TLMDO_CODE_OK;
                    TlmdoRsp.CodeTypes ret_code_standby = TlmdoRsp.CodeTypes.TLMDO_CODE_OK;

                    if (respair.ActiveResource.TelemandoType == RdResource.TelemandoTypes.none)
                    {
                        LogError<RdFrecuency>("Equipo configurado sin telemando. " + respair.ActiveResource.ID,
                            U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR, this.Frecuency,
                            CTranslate.translateResource("Equipo configurado sin telemando. " + respair.ActiveResource.ID));
                    }
                    else
                    {
                        ret_code_active = TlmdoInAResource(msg, respair.StandbyResource, ref response);
                    }

                    if (respair.StandbyResource.TelemandoType == RdResource.TelemandoTypes.none)
                    {
                        LogError<RdFrecuency>("Equipo configurado sin telemando. " + respair.StandbyResource.ID,
                            U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR, this.Frecuency,
                            CTranslate.translateResource("Equipo configurado sin telemando. " + respair.StandbyResource.ID));
                    }
                    else
                    {
                        ret_code_standby = TlmdoInAResource(msg, respair.StandbyResource, ref response) ;
                    }

                    //ret_code_standby = Identifiers.FR_CH_REJECTED;                    
                    //respair.StandbyResource.TunedFrequencyOK = false;
                    //ret_code_active = Identifiers.FR_CHANGE_OK;
                    //respair.ActiveResource.TunedFrequencyOK = true;

                    if (ret_code_active == TlmdoRsp.CodeTypes.TLMDO_CODE_OK && ret_code_standby == TlmdoRsp.CodeTypes.TLMDO_CODE_OK)
                    {
                        if (ret_code_active > returned_code) returned_code = ret_code_active;
                    }
                    else if (ret_code_active == TlmdoRsp.CodeTypes.TLMDO_CODE_OK && ret_code_standby != TlmdoRsp.CodeTypes.TLMDO_CODE_OK)
                    {
                        if (ret_code_active > returned_code) returned_code = ret_code_active;
                    }
                    else if (ret_code_active != TlmdoRsp.CodeTypes.TLMDO_CODE_OK && ret_code_standby == TlmdoRsp.CodeTypes.TLMDO_CODE_OK)
                    {
                        if (ret_code_standby > returned_code) returned_code = ret_code_standby;
                    }
                    else
                    {
                        if (ret_code_active > returned_code) returned_code = ret_code_active;
                    }
                }
                else
                {
                    if ((res as RdResource).TelemandoType == RdResource.TelemandoTypes.none)
                    {
                        LogError<RdFrecuency>("Equipo configurado sin telemando. " + res.ID,
                            U5kiIncidencias.U5kiIncidencia.IGRL_U5KI_NBX_ERROR, this.Frecuency,
                            CTranslate.translateResource("Equipo configurado sin telemando. " + res.ID));
                    }
                    else
                    {
                        TlmdoRsp.CodeTypes ret_code = TlmdoInAResource(msg, res as RdResource, ref response);
                        if (ret_code > returned_code) returned_code = ret_code;
                    }
                }
            }

            return returned_code;
        }

        private TlmdoRsp.CodeTypes TlmdoInAResource(TlmdoAsk msg, RdResource res, ref TlmdoRsp response)
        {
            //El puerto SNMP es 161, excepto los receptores de Rhode que es 160

            SipUtilities.SipUriParser sipuri = new SipUtilities.SipUriParser(res.Uri1);
            bool isEmitter = res.isTx;
            TlmdoRsp.CodeTypes output = TlmdoRsp.CodeTypes.TLMDO_CODE_ERROR;
            TelemandoTypes telemandoType = res.TelemandoType;

            if (res.ID.StartsWith("NOED137"))
            {
                //De momento esta es la unica forma de saber si es un recuros que no es ED137
                telemandoType = TelemandoTypes.Sirtap;
            }

            switch (telemandoType)
            {
                case RdResource.TelemandoTypes.RCRohde4200:
                    {
                        u5ki.RemoteControlService.RCRohde4200 RC;                        
                        if (isEmitter)
                        {
                            RC = new u5ki.RemoteControlService.RCRohde4200(161) { Id = res.ID };
                        }
                        else
                        {
                            RC = new u5ki.RemoteControlService.RCRohde4200(160) { Id = res.ID };
                        }
                        output = RC.Tlmdo(msg, sipuri.Host, isEmitter, ref response, res.Connected);
                    }
                    break;
                case RdResource.TelemandoTypes.RCJotron7000:
                    {
                        u5ki.RemoteControlService.RCJotron7000 RC = new u5ki.RemoteControlService.RCJotron7000(161) { Id = res.ID };
                        output = RC.Tlmdo(msg, sipuri.Host, isEmitter, ref response, res.Connected);
                    }
                    break;
                case RdResource.TelemandoTypes.Sirtap:
                    {
                        u5ki.RemoteControlService.Sirtap RC;
                        try
                        {
                            RC = new u5ki.RemoteControlService.Sirtap(161, Lextm.SharpSnmpLib.VersionCode.V3,
                                res.SNMPV3Credentials.auth_type, res.SNMPV3Credentials.priv_type,
                                res.SNMPV3Credentials.username, res.SNMPV3Credentials.authpass, res.SNMPV3Credentials.privpass)
                            { Id = res.ID };
                        }
                        catch (Exception ex)
                        {
                            LogError<RdFrecuency>("ChangeFreqInAResource: Sirtap" + ex.Message);
                            return output;
                        }
                        output = RC.Tlmdo(msg, sipuri.Host, isEmitter, ref response, res.Connected);
                    }
                    break;
                case RdResource.TelemandoTypes.none:
                    LogError<RdFrecuency>("ChangeFreqInAResource: Resource " + res.ID + " no soporta Telemando");
                    return output;
            }

            return output;
        }

        

















    }

}

