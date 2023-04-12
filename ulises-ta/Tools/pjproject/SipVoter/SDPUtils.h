#ifndef __SDPUTILS_H__
#define __SDPUTILS_H__

class SDPUtils
{
public:
	static void OnCreateSdp(pj_pool_t* pool, int call_id, void* local_sdp, void* incoming_rdata);
	static void UpdateLocalSdpRadioTypeAsGRS(pj_pool_t* pool, int call_id, pjmedia_sdp_session* local_sdp, pjmedia_sdp_session* remote_sdp, CORESIP_CallFlagsMask* TipoGrsFlags);
	static int RadioRemoteSdpAnalysisAsUAS(pjsua_call_id call_id, pj_bool_t radio_grs, const pjmedia_sdp_session* rem_sdp, CORESIP_CallFlagsMask* TipoGrsFlags,
		CORESIP_CallType* newType, CORESIP_CallFlagsMask* newCallflags, pj_bool_t* ptt_id_enabled);
	static int RadioRemoteSdpAnalysisAsUAC(const pjmedia_sdp_session* rem_sdp, CORESIP_CallInfo* info, pj_bool_t* ptt_id_enabled, unsigned short* ptt_id_received_in_sdp);
	static void SetGRSBssMethod_from_SDP(pjsua_acc_id acc_id, pjsua_call_id call_id, const pjmedia_sdp_session* rem_sdp);
	static int CheckGRS_KeepAliveParams_from_SDP(pjsua_acc_id acc_id, const pjmedia_sdp_session* rem_sdp, unsigned int* newR2SKeepAlivePeriod, unsigned int* newR2SKeepAliveMultiplier);
	static int CheckGRS_fid_from_SDP(pjsua_acc_id acc_id, const pjmedia_sdp_session* rem_sdp);
	static int NegotiateNoFreqDisconn(pjsua_call_id call_id, CORESIP_CallType callType, pj_bool_t radio_grs,
		const pjmedia_sdp_session* local_sdp, const pjmedia_sdp_session* rem_sdp, int* NoFreqDisconn);
private:

};

#endif

