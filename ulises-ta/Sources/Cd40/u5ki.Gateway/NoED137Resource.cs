using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using U5ki.Infrastructure;

namespace U5ki.Gateway
{
    internal class NoED137Resource : BaseCode, IDisposable
    {
        public enum RdRsType
        {
            Rx,
            Tx,
            RxTx
        }

        private CfgRecursoEnlaceExterno CfgResource;
        public string IdRecurso;
        private bool accountCreated = false;
        private bool Connected = false;
        private int CallId = -1;
        private RdRsType rdType = RdRsType.RxTx;

        public NoED137Resource(CfgRecursoEnlaceExterno res)
        {
            CfgResource = res;
            IdRecurso = res.IdRecurso;
            accountCreated = false;         
        }

        public void Dispose()
        {
            DestroyGRSAccount();
        }

        public void CreateGRSAccount()
        {
            try
            {
                SipAgent.CreateAccount(IdRecurso);
                accountCreated = true;
            }
            catch (Exception ex)
            {
                LogError<NoED137Resource>($"CreateGRSAccount: Creando account {IdRecurso} {ex.Message}");
                return;
            }

            uint Flags = 0;
            switch ((RdRsType)CfgResource.Tipo)
            {
                case RdRsType.Rx:
                    Flags = (uint)CORESIP_CallFlags.CORESIP_CALL_RD_RXONLY;
                    rdType = RdRsType.Rx;
                    break;
                case RdRsType.Tx:
                    Flags = (uint)CORESIP_CallFlags.CORESIP_CALL_RD_TXONLY;
                    rdType = RdRsType.Tx;
                    break;
                case RdRsType.RxTx:
                    Flags = 0;
                    rdType = RdRsType.RxTx;
                    break;
            }
            try
            {
                SipAgent.SetTipoGRS(IdRecurso, Flags, 1);
            }
            catch (Exception ex)
            {
                Dispose();
                LogError<NoED137Resource>($"CreateGRSAccount: SetTipoGRS {IdRecurso} {ex.Message}");
            }
        }

        public void DestroyGRSAccount()
        {
            if (accountCreated)
            {
                accountCreated = false;
                SipAgent.DestroyAccount(IdRecurso);
            }
        }

        public void NOED137OnCallIncoming(int call, int call2replace, CORESIP_CallInfo info, CORESIP_CallInInfo inInfo)
        {
            if (inInfo.DstId != IdRecurso) SipAgent.AnswerCall(call, SipAgent.SIP_NOT_FOUND);
            else if (!accountCreated) SipAgent.AnswerCall(call, SipAgent.SIP_NOT_FOUND);
            else if (Connected) SipAgent.AnswerCall(call, SipAgent.SIP_DECLINE, 2008, null, false);
            else
            {
                CallId = call;                
                SipAgent.AnswerCall(call, SipAgent.SIP_OK);
            }
        }

        public void NOED137OnCallState(int call, CORESIP_CallInfo info, CORESIP_CallStateInfo stateInfo)
        {
            if (call != CallId) return;
            if (stateInfo.State == CORESIP_CallState.CORESIP_CALL_STATE_DISCONNECTED) 
            {
                CallId = -1;
                Connected = false;
            }
            else if (stateInfo.State == CORESIP_CallState.CORESIP_CALL_STATE_CONFIRMED) 
            {
                Connected = true;
            }
        }

    }
}
