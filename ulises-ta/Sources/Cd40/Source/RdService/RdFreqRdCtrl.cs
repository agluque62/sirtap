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
            public uint code_returned;
            public string selectedFrequency;
        }

        private bool ProcessChangingFreqRunning = false;

        public bool ProcessChangingFreq(string from, FrChangeAsk msg)
        {
            bool ret = true;

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

            if (_TxIds.Count > 0 || _RxIds.Count > 0)
            {          
                ret.result = false;
                ret.code_returned = Identifiers.FR_IS_IN_USE;
                return ret;
            }

            RdService.evQueueRd.Enqueue("SubProcessChangingFreq", delegate ()
            { 
                RdRegistry.Publish(_IdDestino, null);
            });

            Thread.Sleep(2000);
            
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
                        RdRegistry.Publish(_IdDestino, _FrRs);
                    }
                    ProcessChangingFreqRunning = false;
                    MSTxPersistence.SelectFrequency(_IdDestino, resultProcess.selectedFrequency);
                });                
            }
        }

    }

    
}
