/********************************************************************************
 * 
 * Se implementan las funciones de la clase RdFrecuency correspondientes a control
 * de los equipos de radio. Como por ejemplo el cambio de frecuencia
 * 
 * ******************************************************************************/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using U5ki.Infrastructure;
using Utilities;

namespace U5ki.RdService
{    
    public partial class RdFrecuency : BaseCode, IDisposable
    {
        class subProcessChangingFreq_thread
        {
            public string from;
            public FrChangeAsk msg;
            public bool result;
            public string cause_returned;
            public string selectedFrequency;
        }

        private bool ProcessChangingFreqRunning = false;

        public bool ProcessChangingFreq(string from, FrChangeAsk msg)
        {
            bool ret = true;

            if (ProcessChangingFreqRunning)
            {
                RdRegistry.RespondToChangingFreq(from, false, msg.IdFrecuency, msg.NewFrecuency, "ANOTHER PROCESS IS RUNNING");
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
            ret.cause_returned = "OK";
            ret.selectedFrequency = msg.NewFrecuency;

            if (_TxIds.Count > 0 || _RxIds.Count > 0)
            {
                ret.cause_returned = "";
                if (_TxIds.Count > 0)
                {
                    ret.cause_returned += "TX SELECTED IN:";
                    foreach (string txids in _TxIds)
                    {
                        ret.cause_returned += txids;
                        ret.cause_returned += ",";
                    }
                }
                if (_RxIds.Count > 0)
                {
                    ret.cause_returned += "RX SELECTED IN:";
                    foreach (string rxids in _RxIds)
                    {
                        ret.cause_returned += rxids;
                        ret.cause_returned += ",";
                    }
                }

                ret.result = false;
                return ret;
            }

            RdService.evQueueRd.Enqueue("SubProcessChangingFreq", delegate ()
            { 
                RdRegistry.Publish(_IdDestino, null);
            });

            Thread.Sleep(3000);
            
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
                    resultProcess.msg.IdFrecuency, resultProcess.selectedFrequency, resultProcess.cause_returned);

                    SelectedFrequency = resultProcess.selectedFrequency;
                    _FrRs.SelectedFrequency = resultProcess.selectedFrequency;
                    RdRegistry.Publish(_IdDestino, _FrRs);
                    ProcessChangingFreqRunning = false;
                });                
            }
        }

    }

    
}
