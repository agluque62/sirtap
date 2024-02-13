#include "Global.h"
#include "SipCall.h"
#include "Exceptions.h"
#include "SipAgent.h"
#include "processor.h"
#include "AccountUserData.h"
#include "SDPUtils.h"

static pj_str_t gSubjectHdr = { "Subject", 7 };
static pj_str_t gPriorityHdr = { "Priority", 8 };
static pj_str_t gReferBy = { "Referred-By", 11 };
static pj_str_t gRequire = { "Require", 7 };
static pj_str_t gReplaces = { "Replaces", 8 };

/** ED137B */
pj_str_t SipCall::gWG67VersionName = {"WG67-Version", 12};
pj_str_t SipCall::gWG67VersionRadioValue = {"radio.01", 8};
pj_str_t SipCall::gWG67VersionTelefValue = {"phone.01", 8};
pj_str_t SipCall::gWG67VersionRadioValueC = { "radio.02", 8 };
pj_str_t SipCall::gWG67VersionTelefValueC = { "phone.02", 8 };
pj_str_t SipCall::gWG67VersionRadioValueBC = { "radio.02,radio.01", 17 };
pj_str_t SipCall::gWG67VersionTelefValueBC = { "phone.02,phone.01", 17 };
pj_str_t SipCall::gWG67VersionTelefIAValueBC = { "phone.add03.02,phone.01", 23 };
/** Reason Header */

static pj_str_t gWG67ReasonName = {"Reason", 6};
static pj_str_t gWG67Reason2000 = {"WG-67; cause=2000; text=\"session pre-emption\"", 45};
static pj_str_t gWG67Reason2001 = {"WG-67; cause=2001; text=\"missing R2S KeepAlive\"", 47};
static pj_str_t gWG67Reason2002 = {"WG-67; cause=2002; text=\"fid does not match\"", 44};
static pj_str_t gWG67Reason2003 = {"WG-67; cause=2003; text=\"radio in maintenance mode\"", 51};
static pj_str_t gWG67Reason2004 = {"WG-67; cause=2004; text=\"internal error\"", 40};
static pj_str_t gWG67Reason2005 = {"WG-67; cause=2005; text=\"coupling not allowed\"", 46};
static pj_str_t gWG67Reason2006 = {"WG-67; cause=2006; text=\"radio access mode does not match\"", 58};
static pj_str_t gWG67Reason2007 = {"WG-67; cause=2007; text=\"parameter error\"", 41};
static pj_str_t gWG67Reason2008 = {"WG-67; cause=2008; text=\"limit exceeded\"", 40};
static pj_str_t gWG67Reason2009 = {"WG-67; cause=2009; text=\"linked session clear command\"", 54};
static pj_str_t gWG67Reason2010 = {"WG-67; cause=2010; text=\"linked session disabled\"", 49};
static pj_str_t gWG67Reason1001 = {"WG-67; cause=1001; text=\"Emergency - Forced Release\"", 52 };
static pj_str_t gWG67Reason1005 = { "WG-67; cause=1005; text=\"WG67-Version Not Implemented\"", 54 };
/** */

/** */
static unsigned int WG67Reason = 0;
static pj_str_t gIsubParam = { "isub", 4 };
static pj_str_t gRsParam = { "cd40rs", 6 };

static pj_str_t gSubject[] = { { "IA call", 7 }, { "monitoring", 10 }, { "G/G monitoring", 14 }, { "A/G monitoring", 14 }, { "DA/IDA call", 11}, { "radio", 5 } };
// static pj_str_t gSubject[] = { { "ia call", 7 }, { "monitoring", 10 }, { "g/g monitoring", 14 }, { "a/g monitoring", 14 }, { "da/ida call", 11}, { "radio", 5 } };
static pj_str_t gPriority[] = { { "emergency", 9 }, { "urgent", 6 }, { "normal", 6 }, { "non-urgent", 10 } };

static const pj_str_t STR_EVENT = { "Event", 5 };
static const pj_str_t STR_CONFERENCE = { "conference", 10 };
static const pj_str_t STR_EXPIRES = { "Expires", 7 };
static const pj_str_t STR_WG67KEY_IN = { "WG67KEY-IN", 10 };

#ifdef _ED137_
// PlugTest FAA 05/2011
char SipCall::_LocalUri[CORESIP_MAX_URI_LENGTH + 1];
char SipCall::_CallSrcUri[CORESIP_MAX_URI_LENGTH + 1];
//bool SipCall::_LoopClosure=false;
//int SipCall::_ConfiguredCodecs=0;
//int SipCall::_UdpPortForRtpRecording=0;
//char SipCall::_IpRecordingServer[CORESIP_MAX_URI_LENGTH + 1];
// PlugTest FAA 05/2011
// OVR calls
struct CORESIP_EstablishedOvrCallMembers SipCall::_EstablishedOvrCallMembers;
#endif

/** */
pj_str_t *getWG67ReasonContent() 
{
	return WG67Reason==2000 ? (pj_str_t *) &gWG67Reason2000 :
		WG67Reason==2001 ? (pj_str_t *) &gWG67Reason2001 :
		WG67Reason==2002 ? (pj_str_t *) &gWG67Reason2002 :
		WG67Reason==2003 ? (pj_str_t *) &gWG67Reason2003 :
		WG67Reason==2004 ? (pj_str_t *) &gWG67Reason2004 :
		WG67Reason==2005 ? (pj_str_t *) &gWG67Reason2005 :
		WG67Reason==2006 ? (pj_str_t *) &gWG67Reason2006 :
		WG67Reason==2007 ? (pj_str_t *) &gWG67Reason2007 :
		WG67Reason==2008 ? (pj_str_t *) &gWG67Reason2008 :
		WG67Reason==2009 ? (pj_str_t *) &gWG67Reason2009 :
		WG67Reason==2010 ? (pj_str_t *) &gWG67Reason2010 :
		WG67Reason==1001 ? (pj_str_t *) &gWG67Reason1001 :
		WG67Reason==1005 ? (pj_str_t *) &gWG67Reason1005 : (pj_str_t*)NULL;
}

/** */
pj_str_t* getWG67ReasonContent2(int WG67Reason)
{
	return WG67Reason == 2000 ? (pj_str_t*)&gWG67Reason2000 :
		WG67Reason == 2001 ? (pj_str_t*)&gWG67Reason2001 :
		WG67Reason == 2002 ? (pj_str_t*)&gWG67Reason2002 :
		WG67Reason == 2003 ? (pj_str_t*)&gWG67Reason2003 :
		WG67Reason == 2004 ? (pj_str_t*)&gWG67Reason2004 :
		WG67Reason == 2005 ? (pj_str_t*)&gWG67Reason2005 :
		WG67Reason == 2006 ? (pj_str_t*)&gWG67Reason2006 :
		WG67Reason == 2007 ? (pj_str_t*)&gWG67Reason2007 :
		WG67Reason == 2008 ? (pj_str_t*)&gWG67Reason2008 :
		WG67Reason == 2009 ? (pj_str_t*)&gWG67Reason2009 :
		WG67Reason == 2010 ? (pj_str_t*)&gWG67Reason2010 :
		WG67Reason == 1001 ? (pj_str_t*)&gWG67Reason1001 :
		WG67Reason == 1005 ? (pj_str_t*)&gWG67Reason1005 : (pj_str_t*)NULL;
}


/** */
void SetPreferredCodec(int codec)
{
	if (codec==0)
	{
		pjsua_codec_set_priority(&(pj_str(const_cast<char*>("PCMA"))), PJMEDIA_CODEC_PRIO_HIGHEST);
		pjsua_codec_set_priority(&(pj_str(const_cast<char*>("PCMU"))), PJMEDIA_CODEC_PRIO_NEXT_HIGHER);
		pjsua_codec_set_priority(&(pj_str(const_cast<char*>("G728"))), PJMEDIA_CODEC_PRIO_NORMAL);
	}
	else if (codec==1)
	{
		pjsua_codec_set_priority(&(pj_str(const_cast<char*>("PCMA"))), PJMEDIA_CODEC_PRIO_NEXT_HIGHER);
		pjsua_codec_set_priority(&(pj_str(const_cast<char*>("PCMU"))), PJMEDIA_CODEC_PRIO_HIGHEST);
		pjsua_codec_set_priority(&(pj_str(const_cast<char*>("G728"))), PJMEDIA_CODEC_PRIO_NORMAL);
	}
	else if (codec==2)
	{
		pjsua_codec_set_priority(&(pj_str(const_cast<char*>("PCMA"))), PJMEDIA_CODEC_PRIO_NEXT_HIGHER);
		pjsua_codec_set_priority(&(pj_str(const_cast<char*>("PCMU"))), PJMEDIA_CODEC_PRIO_NORMAL);
		pjsua_codec_set_priority(&(pj_str(const_cast<char*>("G728"))), PJMEDIA_CODEC_PRIO_HIGHEST);
	}
	else if (codec == 0xFF)
	{
		//No cambiamos el codec por defecto que se configuro al inicializar la coresip
	}
}

/** */
void SipCall::Wg67VersionSet(pjsip_tx_data *txdata, pj_str_t *valor)
{
	if (pjsip_msg_find_hdr_by_name(txdata->msg, &pj_str("WG67-Version"), NULL)==NULL) 
	{
		pjsip_generic_string_hdr *pWg67version = pjsip_generic_string_hdr_create(txdata->pool, &gWG67VersionName, valor);
		pj_list_push_back(&txdata->msg->hdr, pWg67version);
	}
}

/** */
void Wg67ReasonSet(pjsip_tx_data *txdata) 
{
	//pj_str_t *wg67r;

	if (pjsip_msg_find_hdr_by_name(txdata->msg, &pj_str("Reason"), NULL)==NULL) 
	{
		//wg67r = getWG67ReasonContent();
		pjsip_generic_string_hdr *pWg67Reason = pjsip_generic_string_hdr_create(txdata->pool, &gWG67ReasonName, getWG67ReasonContent());
		//pjsip_generic_string_hdr *pWg67Reason = pjsip_generic_string_hdr_create(txdata->pool, &gWG67ReasonName, wg67r);
		pj_list_push_back(&txdata->msg->hdr, pWg67Reason);
	}
}

/** */
void Wg67ContactSet(pjsip_tx_data *txdata) 
{
	if (pjsip_msg_find_hdr_by_name(txdata->msg, &pj_str("Contact"), NULL)==NULL) 
	{
		pjsip_contact_hdr *contact = pjsip_contact_hdr_create(txdata->pool);
		contact->uri = (pjsip_uri*)SipAgent::pContacUrl;
		pjsip_msg_add_hdr(txdata->msg, (pjsip_hdr*)contact); 
	}
}

/** */
void Wg67AllowSet(pjsip_tx_data *txdata)
{
	if (pjsip_msg_find_hdr_by_name(txdata->msg, &pj_str("Allow"), NULL)==NULL) 
	{
		pjsip_allow_hdr *allow = pjsip_allow_hdr_create(txdata->pool);
		allow->count=0;
		pjsip_msg_add_hdr(txdata->msg, (pjsip_hdr*)allow);
	}
}

/** */
void Wg67SupportedSet(pjsip_tx_data *txdata)
{
	if (pjsip_msg_find_hdr_by_name(txdata->msg, &pj_str("Supported"), NULL)==NULL) 
	{
		pjsip_supported_hdr *supported = pjsip_supported_hdr_create(txdata->pool);
		supported->count=0;
		pjsip_msg_add_hdr(txdata->msg, (pjsip_hdr*)supported);
	}
}

/** */
void Wg67RadioPrioritySet(pjsip_tx_data *txdata) 
{
	if (pjsip_msg_find_hdr_by_name(txdata->msg, &pj_str("Priority"), NULL)==NULL) 
	{
		pjsip_generic_string_hdr *pPriority = pjsip_generic_string_hdr_create(txdata->pool, &gPriorityHdr, &gPriority[2]);
		pj_list_push_back(&txdata->msg->hdr, pPriority);
	}
}

/** */
void Wg67RadioSubjectSet(pjsip_tx_data *txdata) 
{
	if (pjsip_msg_find_hdr_by_name(txdata->msg, &pj_str("Subject"), NULL)==NULL) 
	{
		pjsip_generic_string_hdr *pSubject = pjsip_generic_string_hdr_create(txdata->pool, &gSubjectHdr, &gSubject[5]);
		pj_list_push_back(&txdata->msg->hdr, pSubject);
	}
}

/**
* 
*/
pjsip_evsub_user SipCall::_ConfInfoClientCb = 
{
	&SipCall::OnConfInfoClientStateChanged,  
	NULL,
	NULL,
	&SipCall::OnConfInfoClientRxNotify,
	NULL,
	NULL
};

/**
* 
*/
pjsip_evsub_user SipCall::_ConfInfoSrvCb = 
{
	&SipCall::OnConfInfoSrvStateChanged,  
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

int SipCall::SipCallCount = {0};

//int SipCall::calll = 111;


/**
* 2
*/
SipCall::SipCall(const CORESIP_CallInfo * info, const CORESIP_CallOutInfo * outInfo)
	:	_Id(PJSUA_INVALID_ID), _Pool(NULL), _RdSendSock(PJ_INVALID_SOCKET), 
	_ConfInfoClientEvSub(NULL), _ConfInfoSrvEvSub(NULL), _HangUp(false)
{	
	SipCall::SipCallCount++;

#ifdef _DEBUG
	PJ_LOG(3,(__FILE__, "CONSTRUCTOR SipCall callid %d SipCall::SipCallCount %d", _Id, SipCall::SipCallCount));
#else
	PJ_LOG(5,(__FILE__, "CONSTRUCTOR SipCall callid %d SipCall::SipCallCount %d", _Id, SipCall::SipCallCount));
#endif

	char ED137Radioversion_, ED137Phoneversion_;
	pjsip_endpt_Get_Ed137_version(pjsua_var.endpt, &ED137Radioversion_, &ED137Phoneversion_);

	if (info->Type == CORESIP_CALL_RD) ED137Version = ED137Radioversion_;
	else ED137Version = ED137Phoneversion_;

	_Pool = NULL;
	DstUri[0] = '\0';
	IncommingReqUri[0] = '\0';
	_Index_group = FrecDesp::INVALID_GROUP_INDEX;
	_Index_sess = FrecDesp::INVALID_SESS_INDEX;
	_Sending_Multicast_enabled = PJ_FALSE;
	memset(&RdInfo_prev, 0, sizeof(RdInfo_prev));
	last_received_squ_status = SQU_OFF;
	last_received_ptt_status = CORESIP_PTT_OFF;
	LastReason[0] = '\0';
	last_rtp_ext_info = 0;
	radio_session_with_pttid_enabled = PJ_FALSE;
	assigned_pttid = 0;
	_ConfirmPtt = PJ_TRUE;		
	SelcalSupported = PJ_FALSE;
	GRS_Td1 = 0;
	_IdDestino[0] = '\0';
	CallFlags_prev_reinvite = 0;
	remote_grs_supports_ED137C_Selcal = PJ_FALSE;
	_Last_pjsip_inv_state = PJSIP_INV_STATE_NULL;

	valid_sess = PJ_FALSE;
	_Id = PJSUA_INVALID_ID;
	_Dlg = NULL;
	_Acc_id = PJSUA_INVALID_ID;
	_RdSendSock = PJ_INVALID_SOCKET;
	Retardo = 0;
	circ_buff_mutex = NULL;
	p_retbuff = NULL;
	squ_event = PJ_FALSE;
	squ_event_mcast = PJ_FALSE;
	squoff_event_mcast = PJ_FALSE;
	waited_rtp_seq = 0;
	hay_retardo = PJ_FALSE;
	memset(zerobuf, 0, sizeof(zerobuf));
	index_bss_rx_w = 0;
	bss_rx_mutex = NULL;
	RdInfo_prev_mutex = NULL;
	sem_out_circbuff = NULL;
	waiting_MAM_sem = NULL;
	out_circbuff_thread_run = PJ_FALSE;
	out_circbuff_thread = NULL;
	wait_sem_out_circbuff = PJ_TRUE;
	etm_grs_bss_method_selected[0] = '\0';
	primer_paquete_despues_squelch = PJ_TRUE;
	primer_paquete_despues_ptt = PJ_TRUE;
	bss_method_type = NINGUNO;

	Noise_generator = NULL;
	DTMF_generator = NULL;

	last_qidx_value = 0;
	a_dc[0] = 1.0f;
	a_dc[1] = -0.9950f;
	b_dc[0] = 0.9975f;
	b_dc[1] = -0.9975f;
	fFiltroDC_IfRx_ciX = 0.0f;
	fFiltroDC_IfRx_ciY = 0.0f;
#ifdef _ULISES_
	processor_init(&PdataQidx, 0);
#else
	PdataQidx;
#endif

	try
	{	
		if (info->Type == CORESIP_CALL_RD)
		{
			if (info->R2SKeepAlivePeriod > 0)
			{
				if (info->R2SKeepAlivePeriod < 20 || info->R2SKeepAlivePeriod > 1000)
				{
					PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR R2SKeepAlivePeriod value must be between 20 and 1000"));
				}
				else
				{
					_R2SKeepAlivePeriod = info->R2SKeepAlivePeriod;
				}
			}
			else
			{
				_R2SKeepAlivePeriod = 200;
			}

			if (info->R2SKeepAliveMultiplier > 0)
			{
				if (info->R2SKeepAliveMultiplier < 2 || info->R2SKeepAliveMultiplier > 50)
				{
					PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR R2SKeepAliveMultiplier value must be between 2 and 50"));
				}
				else
				{
					_R2SKeepAliveMultiplier = info->R2SKeepAliveMultiplier;
				}
			}
			else
			{
				_R2SKeepAliveMultiplier = 10;
			}
		}
		
		pj_memcpy(&_Info, info, sizeof(CORESIP_CallInfo));		
		CallFlags_prev_reinvite = _Info.CallFlags;

		//Forzamos la no sincronizacion del audio en recepcion por defecto. Esta opcion estaba en antiguas versiones de la aplicacion de configuracion de Ulises.
		_Info.AudioSync = PJ_FALSE;								

		if (info->CallFlags & CORESIP_CALL_NO_TXRXMODE)
		{
			//Si CORESIP_CALL_NO_TXRXMODE esta activado, entonces desactivo CORESIP_CALL_RD_RXONLY y CORESIP_CALL_RD_TXONLY, 
			//para ignorarlos. No tiene sentido.
			_Info.CallFlags &= ~((CORESIP_CallFlagsMask)CORESIP_CALL_RD_RXONLY);
			_Info.CallFlags &= ~((CORESIP_CallFlagsMask)CORESIP_CALL_RD_TXONLY);
		}

		if (info->Type == CORESIP_CALL_RD)
		{
			if (SipAgent::ETM == PJ_FALSE)
			{
				//Si no es ETM, el CLD sera el calculado
				_Info.forced_cld = -1;
			}
			else if (_Info.forced_cld > 254)
			{
				PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR forced_cld must be less than 255ms"));
			}
			if (_Info.porcentajeRSSI < MIN_porcentajeRSSI || _Info.porcentajeRSSI > MAX_porcentajeRSSI)
			{
				pj_status_t st = PJ_EINVAL;
				PJ_CHECK_STATUS(st, ("ERROR PesoRSSIvsNucleo no valido"));
			}

			if (strcmp(info->bss_method, "Ninguno") == 0)
			{
				bss_method_type = NINGUNO;
			}
			else if (strcmp(info->bss_method, "RSSI") == 0)
			{
				bss_method_type = RSSI;
			}
			else if (strcmp(info->bss_method, "RSSI_NUCLEO") == 0)
			{
				bss_method_type = RSSI_NUC;
			}
			else if (strcmp(info->bss_method, "CENTRAL") == 0)
			{
				bss_method_type = CENTRAL;
			}

			if (ED137Radioversion_ == 'C') _Info.NoFreqDisconn = info->NoFreqDisconn;
			else _Info.NoFreqDisconn = 0;
		}
		else
		{
			_Info.NoFreqDisconn = 0;
		}

		/***¡¡¡¡ De momento ignoramos el parámetro de zona !!!!!***/
		/*Para ignorarlo forzamos que ese parametro sea siempre el mismo*/
		strcpy((char *) _Info.Zona, (const char *) "ZONA_DEF");
		
		_Pool = pjsua_pool_create(NULL, 4096*2, 4096);
		//_Pool = pjsua_pool_create(NULL, 65536, 65536);
			
		if (_Pool == NULL)
		{
			pj_status_t st = PJ_ENOMEM;
			PJ_CHECK_STATUS(st, ("ERROR creando pjsua pool en SipCall"));
		}

		if (SipAgent::ETM == PJ_TRUE)
		{
			_Info.cld_supervision_time = 10;		//En el ETM forzamos que el calculo de retardo sea cada 10 segundos
		}
		else if (_Info.cld_supervision_time > 5 || _Info.cld_supervision_time < 0) _Info.cld_supervision_time = 2;
		//Si cld_supervision_time vale 0 entonces no hay supervision de CLD

		//pjsua_msg_data msgData;
		pjsua_msg_data_init(&make_call_params.msg_data);

		//pjsip_generic_string_hdr subject, priority, referBy;
	
		pjsip_generic_string_hdr_init2(&make_call_params.subject, &gSubjectHdr, &gSubject[info->Type]);
		pj_list_push_back(&make_call_params.msg_data.hdr_list, &make_call_params.subject);
		pjsip_generic_string_hdr_init2(&make_call_params.priority, &gPriorityHdr, &gPriority[info->Priority]);
		pj_list_push_back(&make_call_params.msg_data.hdr_list, &make_call_params.priority);

		if (outInfo->ReferBy[0] != 0)
		{
			pjsip_generic_string_hdr_init2(&make_call_params.referBy, &gReferBy, &(pj_str(const_cast<char*>(outInfo->ReferBy))));
			pj_list_push_back(&make_call_params.msg_data.hdr_list, &make_call_params.referBy);
		}

		if (outInfo->RequireReplaces)
		{			
			if ((strlen(outInfo->CallIdToReplace)+strlen(";from-tag=")+strlen(outInfo->FromTag)+strlen("to-tag=")+
				strlen(outInfo->ToTag)+strlen(";early-only")) < (sizeof(make_call_params.repl)/sizeof(char)))
			{
				//nos aseguramos que cabe en el array
				pjsip_generic_string_hdr_init2(&make_call_params.require, &gRequire, &(pj_str("replaces")));
				pj_list_push_back(&make_call_params.msg_data.hdr_list, &make_call_params.require);	

				strcpy(make_call_params.repl, outInfo->CallIdToReplace);
				strcat(make_call_params.repl,";from-tag=");
				strcat(make_call_params.repl, outInfo->FromTag);
				strcat(make_call_params.repl, ";to-tag=");
				strcat(make_call_params.repl, outInfo->ToTag);
				if (outInfo->EarlyOnly) strcat(make_call_params.repl, ";early-only");

				pj_str_t strrepl = pj_str(make_call_params.repl);
				pjsip_generic_string_hdr_init2(&make_call_params.replaces, &gReplaces, &strrepl);
				pj_list_push_back(&make_call_params.msg_data.hdr_list, &make_call_params.replaces);
			}	
			else
			{
				PJ_LOG(3,(__FILE__, "ERROR: SipCall No se puede construir cabecera Replaces"));
			}
		}
						
		pj_timer_entry_init( &window_timer, 0, NULL, window_timer_cb);
		pj_timer_entry_init( &Check_CLD_timer, Check_CLD_timer_IDLE, NULL, Check_CLD_timer_cb);		
		pj_timer_entry_init( &Wait_init_timer, 0, NULL, Wait_init_timer_cb);		
		pj_timer_entry_init( &Ptt_off_timer, 0, NULL, Ptt_off_timer_cb);
		pj_timer_entry_init( &Wait_fin_timer, 0, NULL, Wait_fin_timer_cb);
		pj_timer_entry_init(&PTT_delayed_timer, 0, NULL, PTT_delayed_timer_cb);
		pj_timer_entry_init(&SELCAL_timer, 0, NULL, SELCAL_timer_cb);

#ifdef _ED137_
		if (info->Type == CORESIP_CALL_RD || info->Type == CORESIP_CALL_RRC || info->Type == CORESIP_CALL_RXTXRD)
		{
			_Frequency = pj_str(const_cast<char*>(outInfo->RdFr));
		}
#endif

		if (info->Type == CORESIP_CALL_RD)
		{
			//Ponemos aqui la cabecera WG67-version para radio
			if (ED137Radioversion_ == 'C')
				pjsip_generic_string_hdr_init2(&make_call_params.wg67ver, &gWG67VersionName, &gWG67VersionRadioValueBC);
			else
				pjsip_generic_string_hdr_init2(&make_call_params.wg67ver, &gWG67VersionName, &gWG67VersionRadioValue);
			pj_list_push_back(&make_call_params.msg_data.hdr_list, &make_call_params.wg67ver);

			pj_ansi_strcpy(_RdFr, outInfo->RdFr);			
			
			if (SipAgent::ETM && strlen(outInfo->RdFr) == 0)
			{
				//En el ETM, como siempre hay que hacer el climax, ponemos la frecuencia 000.000 para que se cree el grupo climax siempre
				pj_ansi_strcpy(_RdFr, "000.000");
			}

			if (pj_ansi_strlen(outInfo->IdDestino) == 0)
			{
				//Si IdDestino es cero entonces el identificador del destino radio será el mismo que el de la frecuencia
				pj_ansi_strcpy(_IdDestino, _RdFr);
			}
			else
			{
				pj_ansi_strcpy(_IdDestino, outInfo->IdDestino);
			}

			if ((_Info.CallFlags & CORESIP_CALL_RD_TXONLY) == 0)
			{
				pj_status_t st = pj_sock_socket(pj_AF_INET(), pj_SOCK_DGRAM(), 0, &_RdSendSock);
				PJ_CHECK_STATUS(st, ("ERROR creando socket para el envio de radio por multicast"));

				//Se fuerza que los paquetes salgan por el interfaz que utiliza el agente.
				struct pj_in_addr in_uaIpAdd;
				pj_str_t str_uaIpAdd;
				str_uaIpAdd.ptr = SipAgent::Get_uaIpAdd();
				str_uaIpAdd.slen = (pj_ssize_t) strlen(SipAgent::Get_uaIpAdd());
				pj_inet_aton((const pj_str_t *) &str_uaIpAdd, &in_uaIpAdd);

				st = pj_sock_setsockopt(_RdSendSock, IPPROTO_IP, IP_MULTICAST_IF, (void *)&in_uaIpAdd, sizeof(in_uaIpAdd));
				if (st != PJ_SUCCESS)
					PJ_LOG(3,(__FILE__, "ERROR: setsockopt, PJ_IP_MULTICAST_IF. El envio de audio a %s:%d no se puede forzar por el interface %s", 
						outInfo->RdMcastAddr, outInfo->RdMcastPort, SipAgent::Get_uaIpAdd()));
						
				pj_sockaddr_in_init(&_RdSendTo, &(pj_str(const_cast<char*>(outInfo->RdMcastAddr))), (pj_uint16_t)outInfo->RdMcastPort);
			}
		}
		else
		{
			//Ponemos aqui la cabecera WG67-version para phone
			if (ED137Phoneversion_ == 'C')
			{
				if (info->Type == CORESIP_CALL_IA)
					pjsip_generic_string_hdr_init2(&make_call_params.wg67ver, &gWG67VersionName, &gWG67VersionTelefIAValueBC);
				else
					pjsip_generic_string_hdr_init2(&make_call_params.wg67ver, &gWG67VersionName, &gWG67VersionTelefValueBC);
			}
			else
				pjsip_generic_string_hdr_init2(&make_call_params.wg67ver, &gWG67VersionName, &gWG67VersionTelefValue);
			pj_list_push_back(&make_call_params.msg_data.hdr_list, &make_call_params.wg67ver);
		}

		/** AGL. CODEC Preferido en la LLamada. */
		SetPreferredCodec(info->PreferredCodec);

		unsigned options = info->CallFlags & CORESIP_CALL_CONF_FOCUS ? PJSUA_CALL_CONFERENCE : 0;
		pjsua_acc_id acc = info->AccountId == PJSUA_INVALID_ID ? pjsua_acc_get_default() : info->AccountId & CORESIP_ID_MASK;
		
		pj_status_t st = pj_mutex_create_simple(_Pool, "RdInfo_prev_mutex", &RdInfo_prev_mutex);
		PJ_CHECK_STATUS(st, ("ERROR creando mutex RdInfo_prev_mutex"));

		st = pj_sem_create(_Pool, NULL, 0, 1, &waiting_MAM_sem);
		PJ_CHECK_STATUS(st, ("ERROR creando mutex waiting_mam_sem"));

		if (_Info.Type == CORESIP_CALL_RD)
		{
			st = pj_mutex_create_simple(_Pool, "CircBuffMtx", &circ_buff_mutex);
			PJ_CHECK_STATUS(st, ("ERROR creando mutex para buffer circular"));

			//st = pjmedia_circ_buf_create(_Pool, 16384*8, &p_retbuff);
			st = pjmedia_circ_buf_create(_Pool, 4096*2, &p_retbuff);
			PJ_CHECK_STATUS(st, ("ERROR Creando buffer circular"));	
			pjmedia_circ_buf_reset(p_retbuff);	
			
			st = pj_mutex_create_simple(_Pool, "bss_rx_mutex", &bss_rx_mutex);
			PJ_CHECK_STATUS(st, ("ERROR creando mutex bss_rx_mutex"));

			//Inicializa el timer de la ventana de decision. Este timer solo se arranca si el squelch de esta radio
			//es el primero que se activa y si est� en un grupo bss
			pj_timer_entry_init( &window_timer, 0, (void *) this, window_timer_cb);

			st = pj_sem_create(_Pool, "sem_out_circbuff", 0, 10, &sem_out_circbuff);
			PJ_CHECK_STATUS(st, ("ERROR creando sem_out_circbuff"));

			out_circbuff_thread_run = PJ_TRUE;
			st = pj_thread_create(_Pool, "Out_circbuff_Th", &Out_circbuff_Th, this, 0, 0, &out_circbuff_thread);
			PJ_CHECK_STATUS(st, ("ERROR creando thread out_circbuff_thread"));			
		}
		else
		{
			options |= PJSIP_INV_SUPPORT_TIMER;
			options |= PJSIP_INV_SUPPORT_100REL;
		}

		strncpy(DstUri, outInfo->DstUri, sizeof(DstUri));
		DstUri[sizeof(DstUri) - 1] = '\0';

		if (info->CallFlags & CORESIP_CALL_EXTERNA)
			options |= PJSIP_INV_VIA_PROXY;

		make_call_params.acc_id = acc;
		strncpy(make_call_params.dst_uri, outInfo->DstUri, sizeof(make_call_params.dst_uri));
		make_call_params.dst_uri[sizeof(make_call_params.dst_uri)-1] = '\0';
		make_call_params.options = options;
	}
	catch (...)
	{
		PJ_LOG(3,(__FILE__, "ERROR: EXCEPCION EN EL CONTRUCTOR SipCall"));
		SipCall::SipCallCount--;	
#ifdef _DEBUG
	PJ_LOG(3,(__FILE__, "CONSTRUCTOR excepcion SipCall::SipCallCount %d", SipCall::SipCallCount));
#else
	PJ_LOG(5,(__FILE__, "CONSTRUCTOR excepcion SipCall::SipCallCount %d", SipCall::SipCallCount));
#endif
		Dispose();
		throw;
	}
}


/**
* 
*/
SipCall::SipCall(pjsua_call_id call_id, const CORESIP_CallInfo * info)
	:	_Id(call_id), _Pool(NULL), _RdSendSock(PJ_INVALID_SOCKET), 
	_ConfInfoClientEvSub(NULL), _ConfInfoSrvEvSub(NULL), _HangUp(false)
{
	SipCall::SipCallCount += 1;

	char ED137Radioversion_, ED137Phoneversion_;
	pjsip_endpt_Get_Ed137_version(pjsua_var.endpt, &ED137Radioversion_, &ED137Phoneversion_);

	if (info->Type == CORESIP_CALL_RD) ED137Version = ED137Radioversion_;
	else ED137Version = ED137Phoneversion_;

	_Pool = NULL;
	_RdSendSock = PJ_INVALID_SOCKET;
	_Index_group = FrecDesp::INVALID_GROUP_INDEX;
	_Index_sess = FrecDesp::INVALID_SESS_INDEX;
	valid_sess = PJ_FALSE;
	LastReason[0] = '\0';
	last_rtp_ext_info = 0;
	radio_session_with_pttid_enabled = PJ_FALSE;
	assigned_pttid = 0;
	_ConfirmPtt = PJ_TRUE;
	_Dlg = NULL;
	_Acc_id = PJSUA_INVALID_ID;
	SelcalSupported = PJ_FALSE;
	GRS_Td1 = 0;
	Callflag_previous_reinvite = 0;
	remote_grs_supports_ED137C_Selcal = PJ_FALSE;
	_Last_pjsip_inv_state = PJSIP_INV_STATE_NULL;

	_R2SKeepAlivePeriod = 200;
	_R2SKeepAliveMultiplier = 10;

	_Pool = pjsua_pool_create(NULL, 512, 512);
	pj_memcpy(&_Info, info, sizeof(CORESIP_CallInfo));

	DstUri[0] = '\0';
	IncommingReqUri[0] = '\0';
	last_received_squ_status = SQU_OFF;
	last_received_ptt_status = CORESIP_PTT_OFF;
	squ_event = PJ_FALSE;
	squ_event_mcast = PJ_FALSE;	
	squoff_event_mcast = PJ_FALSE;
	Retardo = 0;
	out_circbuff_thread = NULL;
	out_circbuff_thread_run = PJ_FALSE;
	p_retbuff = NULL;
	wait_sem_out_circbuff = PJ_FALSE;
	_Sending_Multicast_enabled = PJ_FALSE;
	pj_timer_entry_init( &window_timer, 0, NULL, window_timer_cb);
	pj_timer_entry_init( &Check_CLD_timer, Check_CLD_timer_IDLE, NULL, Check_CLD_timer_cb);
	pj_timer_entry_init( &Wait_init_timer, 0, NULL, Wait_init_timer_cb);
	pj_timer_entry_init( &Ptt_off_timer, 0, NULL, Ptt_off_timer_cb);
	pj_timer_entry_init( &Wait_fin_timer, 0, NULL, Wait_fin_timer_cb);
	pj_timer_entry_init(&PTT_delayed_timer, 0, NULL, PTT_delayed_timer_cb);
	pj_timer_entry_init(&SELCAL_timer, 0, NULL, SELCAL_timer_cb);
	sem_out_circbuff = NULL;
	circ_buff_mutex = NULL;
	bss_rx_mutex = NULL;
	bss_method_type = NINGUNO;
	pj_status_t st = pj_mutex_create_simple(_Pool, "RdInfo_prev_mutex", &RdInfo_prev_mutex);
	PJ_CHECK_STATUS(st, ("ERROR creando mutex RdInfo_prev_mutex"));
	etm_grs_bss_method_selected[0] = '\0';
	memset(&RdInfo_prev, 0, sizeof(RdInfo_prev));
	waiting_MAM_sem = NULL;
	st = pj_sem_create(_Pool, NULL, 0, 1, &waiting_MAM_sem);
	PJ_CHECK_STATUS(st, ("ERROR creando mutex waiting_mam_sem"));

	Noise_generator = NULL;
	DTMF_generator = NULL;

	primer_paquete_despues_squelch = PJ_TRUE;
	primer_paquete_despues_ptt = PJ_TRUE;
	waited_rtp_seq = 0;

	last_qidx_value = 0;
	a_dc[0] = 1.0f;
	a_dc[1] = -0.9950f;
	b_dc[0] = 0.9975f;
	b_dc[1] = -0.9975f;
	fFiltroDC_IfRx_ciX = 0.0f;
	fFiltroDC_IfRx_ciY = 0.0f;
#ifdef _ULISES_
	processor_init(&PdataQidx, 0);
#else
	PdataQidx;
#endif
}

/**
* 
*/
SipCall::~SipCall()
{
	SipCall::SipCallCount--;
#ifdef _DEBUG
	PJ_LOG(3,(__FILE__, "DESTRUCTOR SipCall callid %d SipCall::SipCallCount %d", _Id, SipCall::SipCallCount));
#else
	PJ_LOG(5,(__FILE__, "DESTRUCTOR SipCall callid %d SipCall::SipCallCount %d", _Id, SipCall::SipCallCount));
#endif
	
	Dispose();
}


void SipCall::Dispose()
{	
	SELCAL_timer.id = 0;
	pjsua_cancel_timer(&SELCAL_timer);

	PTT_delayed_timer.id = 0;
	pjsua_cancel_timer(&PTT_delayed_timer);

	Wait_fin_timer.id = 0;
	pjsua_cancel_timer(&Wait_fin_timer);

	Ptt_off_timer.id = 0;
	pjsua_cancel_timer(&Ptt_off_timer);

	Check_CLD_timer.id = Check_CLD_timer_IDLE;
	pjsua_cancel_timer(&Check_CLD_timer);

	Wait_init_timer.id = 0;
	pjsua_cancel_timer(&Wait_init_timer);

	window_timer.id = 0;
	pjsua_cancel_timer(&window_timer);

	if (out_circbuff_thread != NULL && sem_out_circbuff != NULL)
	{
		out_circbuff_thread_run = PJ_FALSE;
		pj_sem_post(sem_out_circbuff);
		pj_thread_join(out_circbuff_thread);
		pj_thread_destroy(out_circbuff_thread);
		out_circbuff_thread = NULL;				
	}

	if (sem_out_circbuff != NULL)
	{
		pj_sem_destroy(sem_out_circbuff);
		sem_out_circbuff = NULL;
	}

	if (circ_buff_mutex != NULL)
	{
		pj_mutex_destroy(circ_buff_mutex);
		circ_buff_mutex = NULL;
	}

	if (_RdSendSock != PJ_INVALID_SOCKET)
	{
		pj_sock_close(_RdSendSock);
		_RdSendSock = PJ_INVALID_SOCKET;
	}

	if (bss_rx_mutex != NULL)
	{
		pj_mutex_destroy(bss_rx_mutex);
		bss_rx_mutex = NULL;
	}

	if (waiting_MAM_sem != NULL)
	{
		pj_sem_destroy(waiting_MAM_sem);
		waiting_MAM_sem = NULL;
	}

	if (RdInfo_prev_mutex != NULL)
	{
		pj_mutex_destroy(RdInfo_prev_mutex);
		RdInfo_prev_mutex = NULL;
	}

	if (_Pool != NULL)
	{
		pj_pool_release(_Pool);
		_Pool = NULL;
	}
}

int SipCall::Hacer_la_llamada_saliente()
{
	pj_status_t st = pjsua_call_make_call(make_call_params.acc_id, &(pj_str(const_cast<char*>(make_call_params.dst_uri))), 
			make_call_params.options, this, &make_call_params.msg_data, &_Id);	
	if (st != PJ_SUCCESS)
	{
		_Id = PJSUA_INVALID_ID;
		PJ_LOG(3,(__FILE__, "ERROR: Hacer_la_llamada_saliente: pjsua_call_make_call retorna error %s", make_call_params.dst_uri));
		return -1;
	}

	pjsua_call* pjcall;
	pjsip_dialog* dlg;
	st = acquire_call("Hacer_la_llamada_saliente", _Id, &pjcall, &dlg);
	if (st == PJ_SUCCESS)
	{
		dlg->call_id->id;

		//Envia CallStart al grabador
		pjsua_acc_info info_acc;
		pjsua_acc_get_info(make_call_params.acc_id, &info_acc);
		SipAgent::RecINV(&pj_str(const_cast<char*>(make_call_params.dst_uri)), _Info.Type);
		if (_Info.Type != CORESIP_CALL_RD)
		{			
			SipAgent::RecCallStart(SipAgent::OUTCOM, _Info.Priority, &info_acc.acc_uri, &pj_str(const_cast<char*>(make_call_params.dst_uri)), &dlg->call_id->id);
		}

		pjsip_dlg_dec_lock(dlg);
	}
	else
	{
		PJ_LOG(3, (__FILE__, "ERROR: Hacer_la_llamada_saliente: acquire_call retorna error %s", make_call_params.dst_uri));
		_Id = PJSUA_INVALID_ID;
		return -1;
	}

	return 0;
}

/**
* 1.
*/
int SipCall::New(const CORESIP_CallInfo * info, const CORESIP_CallOutInfo * outInfo) 
{
	SipCall * call = new SipCall(info, outInfo);
	int ret = PJSUA_INVALID_ID;
	if (call != NULL)
	{
		if (call->Hacer_la_llamada_saliente() != 0)
		{			
			if (call->Wait_fin_timer.id == 0)
			{
				//Se elimina el objeto si no esta corriendo el timer Wait_fin_timer que alimina el objeto.
				//Se ha detectado que la funcion pjsua_call_make_call puede llamar a OnStateChanged con el estado 
				//PJSIP_INV_STATE_DISCONNECTED y por tanto arrancarse el timer Wait_fin_timer para borrar el objeto SipCall,
				//por ejemplo en el caso de que la ip de la uri sea inalcanzable.
				delete call;
			}
		}
		else
		{
			ret = call->_Id;
		}		
	}

	if (ret == PJSUA_INVALID_ID)
	{
		char dsturi[512];
		strncpy(dsturi, outInfo->DstUri, sizeof(dsturi));
		dsturi[sizeof(dsturi)-1] = '\0';
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("SipCall::New:", "Error llamando a %s", dsturi);
	}

	return ret;
}

/**
* FlushSessions: Elimina todas las sesiones que esten abiertas para un destino
* @param dst. URI del destino cuyas sesiones queremos eliminar
* @param except_cid. Sería un call id que no queremos eliminar. Si vale PJSUA_INVALID_ID, entonces no hay excepciones
* @param calltype. Tipo de las llamadas
* @return -1 si hay error. 0 no hay error.
*/
int SipCall::FlushSessions(pj_str_t *dst, pjsua_call_id except_cid, CORESIP_CallType calltype)
{
	int ret = 0;
	unsigned calls_count = PJSUA_MAX_CALLS;
	pjsua_call_id call_ids[PJSUA_MAX_CALLS];

	pj_status_t s1 = pjsua_enum_calls(call_ids, &calls_count);
	if (s1 != PJ_SUCCESS) return -1;
	if (calls_count == 0) return 0;


	pj_pool_t* pool = pjsua_pool_create(NULL, 256, 32);
	if (pool == NULL)
	{				
		return -1;
	}

	/*Se crea un string duplicado para el parse, ya que se ha visto que
	pjsip_parse_uri puede modificar el parametro de entrada*/	
	pj_str_t uri_dup;
	pj_strdup_with_null(pool, &uri_dup, dst);

	pjsip_uri *dst_uri = pjsip_parse_uri(pool, uri_dup.ptr, uri_dup.slen, 0);	
	if (dst_uri == NULL)
	{
		pj_pool_release(pool);
		return -1;
	}

	int total_esperas = 8;
	int esperas = total_esperas;
	pj_bool_t fantasma_encontrada = PJ_TRUE;

	//Intentamos cerrar cada una de las sesiones abiertas
	while (esperas > 0 && fantasma_encontrada)
	{
		fantasma_encontrada = PJ_FALSE;
		for (unsigned i = 0; i < calls_count; i++)
		{
			if (call_ids[i] != except_cid)
			{
				CORESIP_CallType Type_call = CORESIP_CALL_UNKNOWN;
				pjsua_call * pjcall;
				pjsip_dialog * dlg;
				pj_status_t st = acquire_call("FlushSessions()", call_ids[i], &pjcall, &dlg);	
				if (st == PJ_SUCCESS)
				{
					SipCall * call = (SipCall*)pjcall->user_data;
					if (call != NULL)
					{
						Type_call = call->_Info.Type;
					}
					pjsip_dlg_dec_lock(dlg);
				}

				pjsua_call_info info;
				if ((calltype == Type_call) && (pjsua_call_get_info(call_ids[i], &info) == PJ_SUCCESS))
				{
					/*Se crea un string duplicado para el parse, ya que se ha visto que
					pjsip_parse_uri puede modificar el parametro de entrada*/	
					pj_str_t uri_dup1;
					pj_strdup_with_null(pool, &uri_dup1, &info.remote_info);

					pjsip_uri *rem_uri = pjsip_parse_uri(pool, uri_dup1.ptr, uri_dup1.slen, 0);		
					if (rem_uri != NULL)
					{
						pj_status_t stus = pjsip_uri_cmp(PJSIP_URI_IN_FROMTO_HDR, dst_uri, rem_uri);
						if (stus == PJ_SUCCESS)
						{
							//Existe una sesion abierta con el mismo destino.
							//Forzamos a cerrar la sesion
							fantasma_encontrada = PJ_TRUE; 
							char buf[256];						
							pjsip_uri_print(PJSIP_URI_IN_FROMTO_HDR, rem_uri, buf, sizeof(buf)-1);
							PJ_LOG(3,(__FILE__, "WARNING: Parece hay sesion fantasma abierta para este destino %s.", buf));

							//Si encuentra una sesion abierta hacia ese destino entonces espera a que se cierre.
							//En caso de que falten total_esperas/2 entonces se fuerza el hangup

							if (esperas == total_esperas/2)
							{
								PJ_LOG(3,(__FILE__, "WARNING: Parece hay sesion fantasma abierta para este destino %s. Se fuerza su cierre", buf));
								Force_Hangup(call_ids[i], 0);
							}
							pj_thread_sleep(20);
						}
					}
				}
			}
		}
		if (fantasma_encontrada)
		{
			esperas--;		
			pj_status_t s1 = pjsua_enum_calls(call_ids, &calls_count);
			if (s1 != PJ_SUCCESS) 
			{
				ret = -1;
				fantasma_encontrada = PJ_FALSE;		//SE fuerza salga del bucle
			}
		}
	}

	pj_pool_release(pool);
	return ret;
}

void SipCall::IniciaFinSesion()
{
	SELCAL_timer.id = 0;
	pjsua_cancel_timer(&SELCAL_timer);

	//Paramos ya el timer del PTT retrasado
	PTT_delayed_timer.id = 0;
	pjsua_cancel_timer(&PTT_delayed_timer);

	Wait_fin_timer.id = 0;
	pjsua_cancel_timer(&Wait_fin_timer);

	//Retrasamos el delete del objeto para asegurarnos que han finalizado las transacciones sip
	Wait_fin_timer.cb = Wait_fin_timer_cb;
	Wait_fin_timer.user_data = (void *) this;
	pj_time_val	delay1;
	delay1.sec = (long) 0;
	delay1.msec = (long) (pjsip_cfg()->tsx.tsx_tout * 4);	
	Wait_fin_timer.id = 1;
	pj_status_t st = pjsua_schedule_timer(&Wait_fin_timer, &delay1);
	if (st != PJ_SUCCESS)
	{
		Wait_fin_timer.id = 0;
		PJ_LOG(3, (__FILE__, "ERROR: SipCall::IniciaFinSesion: No se puede iniciar Wait_fin_timer"));
	}
}

void SipCall::Wait_fin_timer_cb(pj_timer_heap_t *th, pj_timer_entry *te)
{
	SipCall *wp = (SipCall *)te->user_data;

	if (wp == NULL) return;
	if (wp->Wait_fin_timer.id == 0) return;			

	pj_bool_t reiniciar_timer = PJ_FALSE;

	//Finaliza las subscripciones al evento de conferencia si no han sido terminadas ya. Siempre y cuando este agente no 
	//haya sido el que inicia el fin de la sesion, es decir si _HangUp es false
	if (wp->_Info.Type != CORESIP_CALL_RD && !wp->_HangUp)
	{
		//En este caso ya se ha finalizado las subscripciones al evento de presencia en la llamada a hangUp
		if (wp->_ConfInfoClientEvSub)
		{
			if (pjsip_evsub_get_state(wp->_ConfInfoClientEvSub) != PJSIP_EVSUB_STATE_TERMINATED)
			{
				pjsip_tx_data* tdata;
				if ((PJ_SUCCESS != pjsip_conf_initiate(wp->_ConfInfoClientEvSub, 0, &tdata)) ||
					(PJ_SUCCESS != pjsip_conf_send_request(wp->_ConfInfoClientEvSub, tdata)))
				{
					pjsip_conf_terminate(wp->_ConfInfoClientEvSub, PJ_FALSE);
				}
				reiniciar_timer = PJ_TRUE;
			}
		}
		if (wp->_ConfInfoSrvEvSub)
		{
			if (pjsip_evsub_get_state(wp->_ConfInfoSrvEvSub) != PJSIP_EVSUB_STATE_TERMINATED)
			{
				pjsip_tx_data* tdata;
				pjsip_conf_status info;

				info.version = 0xFFFFFFFF;
				info.state = pj_str("deleted");
				info.users_cnt = 0;

				pjsip_conf_set_status(wp->_ConfInfoSrvEvSub, &info);

				if ((PJ_SUCCESS != pjsip_conf_notify(wp->_ConfInfoSrvEvSub, PJSIP_EVSUB_STATE_TERMINATED, NULL, NULL, &tdata)) ||
					(PJ_SUCCESS != pjsip_conf_send_request(wp->_ConfInfoSrvEvSub, tdata)))
				{
					pjsip_conf_terminate(wp->_ConfInfoSrvEvSub, PJ_FALSE);
				}
				reiniciar_timer = PJ_TRUE;
			}
		}
	}

	if (reiniciar_timer)
	{
		//Se reinicia el timer para esperar el fin de las subscripciones asociadas a esta sesion SIP
		wp->Wait_fin_timer.cb = Wait_fin_timer_cb;
		wp->Wait_fin_timer.user_data = (void*)wp;
		pj_time_val	delay1;
		delay1.sec = (long)0;
		delay1.msec = (long)(pjsip_cfg()->tsx.tsx_tout * 4);
		wp->Wait_fin_timer.id = 2;	//Le asignamos un valor diferente a 1 para que solo se reinicie el timer una vez
		pj_status_t st = pjsua_schedule_timer(&wp->Wait_fin_timer, &delay1);
		if (st != PJ_SUCCESS)
		{
			wp->Wait_fin_timer.id = 0;
			PJ_LOG(3, (__FILE__, "ERROR: SipCall::Wait_fin_timer_cb: No se puede iniciar Wait_fin_timer"));
		}
	}
	else
	{
		//Eliminamos el objeto
		delete wp;
	}
}

/**
* 
*/
void SipCall::Hangup(pjsua_call_id call_id, unsigned code)
{
	pjsua_call * pjcall;
	pjsip_dialog * dlg;
	SipCall *call = NULL;	
	
	pj_status_t st;

	if (call_id<0 || call_id>(int)pjsua_var.ua_cfg.max_calls)
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("Hangup:", "call_id %d no valido", call_id);		
	}

#if 0
	//Buscamos sesiones con el mismo destino que pudieran estar tambien abiertas, cuando son radios
	//o lineas calientes. Si estuvieran abiertas serian sesiones fantasmas que cerraremos
	CORESIP_CallType Type_call = CORESIP_CALL_UNKNOWN;
	char DstUri[CORESIP_MAX_URI_LENGTH + 1];

	st = acquire_call("Hangup()", call_id, &pjcall, &dlg);
	PJ_CHECK_STATUS(st, ("ERROR adquiriendo call", "[Call=%d]", call_id));
	call = (SipCall*)pjcall->user_data;
	if (call != NULL)
	{
		Type_call = call->_Info.Type;
		strncpy(DstUri, call->DstUri, sizeof(DstUri)-1);
		DstUri[sizeof(DstUri)-1] = 0;
	}
	pjsip_dlg_dec_lock(dlg);

	if (Type_call == CORESIP_CALL_RD || Type_call == CORESIP_CALL_IA)
	{
		pj_str_t dsturi;
		dsturi.ptr = (char *) DstUri;
		dsturi.slen = strlen(DstUri);
		FlushSessions(&dsturi, call_id, Type_call);		//Se cierran las posibles sesiones a una misma uri
												//de destino menos el de este call_id
	}
#endif

	//Se procede a cerrar la sesion de este call id
	st = acquire_call("Hangup()", call_id, &pjcall, &dlg);
	PJ_CHECK_STATUS(st, ("ERROR adquiriendo call", "[Call=%d]", call_id));

	call = (SipCall*)pjcall->user_data;

	if (call)
	{
		if (call->_ConfInfoClientEvSub)
		{
			pjsip_tx_data * tdata;
			if (PJ_SUCCESS == pjsip_conf_initiate(call->_ConfInfoClientEvSub, 0, &tdata))
			{
				pjsip_conf_send_request(call->_ConfInfoClientEvSub, tdata);
			}
		}
		if (call->_ConfInfoSrvEvSub)
		{
			pjsip_tx_data * tdata;
			pjsip_conf_status info;

			info.version = 0xFFFFFFFF;
			info.state = pj_str("deleted");
			info.users_cnt = 0;
			
			pjsip_conf_set_status(call->_ConfInfoSrvEvSub, &info);

			if (PJ_SUCCESS == pjsip_conf_notify(call->_ConfInfoSrvEvSub, PJSIP_EVSUB_STATE_TERMINATED, NULL, NULL, &tdata))
			{
				pjsip_conf_send_request(call->_ConfInfoSrvEvSub, tdata);
			}
		}		

		/*if (call->_Info.Type == CORESIP_CALL_RD)
		{
			pjsua_media_channel_deinit(call_id);
		}*/

		call->_HangUp = true;
	}
		
	pjsip_dlg_dec_lock(dlg);

	if (code > 999)
	{
		//codigos de protocolo SIP tiene valores menores de 1000
		WG67Reason = code;
	}

	if (code == 0 || code > 999)
	{
		st = pjsua_call_hangup(call_id, code, NULL, NULL);
	}
	else
	{
		//Si code es distinto de cero se pone la cabecera Reason con el codigo de error
		pjsua_msg_data msg_data;
		pjsua_msg_data_init(&msg_data);
		pjsip_generic_string_hdr reason_hdr;
		pj_str_t reason = pj_str("Reason");		
		char str_reason_val[18];
		strcpy(str_reason_val, "SIP;cause=");
		char str_code[8];
		pj_utoa((unsigned long) code, str_code);
		strncat(str_reason_val, str_code, sizeof(str_reason_val) / sizeof(char));
		str_reason_val[(sizeof(str_reason_val) / sizeof(char))-1] = '\0';
		pj_str_t reason_val = pj_str(str_reason_val);
		pjsip_generic_string_hdr_init2(&reason_hdr, &reason, &reason_val);
		pj_list_push_back(&msg_data.hdr_list, &reason_hdr);
	
		st = pjsua_call_hangup(call_id, code, NULL, &msg_data);
	}

	PJ_CHECK_STATUS(st, ("ERROR finalizando llamada", "[Call=%d] [Code=%d]", call_id, code));	
}

/**
* 
*/
int SipCall::Force_Hangup(pjsua_call_id call_id, unsigned code)
{
	pjsua_call * pjcall;
	pjsip_dialog * dlg;

	pj_status_t st = acquire_call("Force_Hangup()", call_id, &pjcall, &dlg);
	if (st != PJ_SUCCESS)
	{
		return -1;
	}

	SipCall * call = (SipCall*)pjcall->user_data;
	if (!call) 
	{
		pjsip_dlg_dec_lock(dlg);
		return -1;
	}

	call->_HangUp = true;
	pjsip_dlg_dec_lock(dlg);

	WG67Reason = code;
	st = pjsua_call_hangup(call_id, code, NULL, NULL);

	if (st != PJ_SUCCESS)
	{
		return -1;
	}

	return 0;
}

/**
* 
*/
void SipCall::Answer(pjsua_call_id call_id, unsigned code, bool addToConference, unsigned reason_cause, char* reason_text)
{
	if (call_id<0 || call_id>(int)pjsua_var.ua_cfg.max_calls)
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("Answer:", "call_id %d no valido", call_id);		
	}

	if (addToConference)
	{
		pjsua_call * call;
		pjsip_dialog * dlg;

		pj_status_t st = acquire_call("Answer()", call_id, &call, &dlg);
		PJ_CHECK_STATUS(st, ("ERROR adquiriendo call", "[Call=%d]", call_id));

		dlg->local.contact->focus = PJ_TRUE;
		pjsip_dlg_dec_lock(dlg);
	}

	if (code & (CORESIP_CALL_EC << 16))
	{
		pjsua_call * call;
		pjsip_dialog * dlg;

		pj_status_t st = acquire_call("Answer()", call_id, &call, &dlg);
		PJ_CHECK_STATUS(st, ("ERROR adquiriendo call", "[Call=%d]", call_id));

		if (call->user_data)
		{
			CORESIP_CallFlagsMask flags = ((SipCall*)call->user_data)->_Info.CallFlags;
			((SipCall*)call->user_data)->_Info.CallFlags = flags | CORESIP_CALL_EC;
		}

		pjsip_dlg_dec_lock(dlg);
		code &= 0x0000FFFF;
	}

	pjsua_msg_data *answer_msg_data = NULL;
	pjsua_msg_data msg_data;
	if (reason_cause != 0)
	{
		if (reason_cause > 2000 && reason_cause < 2100 && reason_text == NULL)
		{			
			pjsua_msg_data_init(&msg_data);
			pjsip_generic_string_hdr reason_hdr;
			pj_str_t reason = pj_str("Reason");
			pjsip_generic_string_hdr_init2(&reason_hdr, &reason, getWG67ReasonContent2((int)reason_cause));
			pj_list_push_back(&msg_data.hdr_list, &reason_hdr);
			answer_msg_data = &msg_data;
		}
		else
		{			
			pjsua_msg_data_init(&msg_data);
			pjsip_generic_string_hdr reason_hdr;
			pj_str_t reason = pj_str("Reason");
			char str_reason_val[128];
			if ((reason_cause >= 1000 && reason_cause < 1100) || (reason_cause >= 2000 && reason_cause < 2100))
			{
				strcpy(str_reason_val, "WG-67;cause=");
			}
			else
			{
				strcpy(str_reason_val, "SIP;cause=");
			}
			char str_code[8];
			pj_utoa((unsigned long)reason_cause, str_code);
			size_t str_reason_val_size = sizeof(str_reason_val) / sizeof(char);
			pj_bool_t fit = PJ_TRUE;

			if ((str_reason_val_size - strlen(str_reason_val) - strlen(str_code) - 1) > 0)
			{
				strcat(str_reason_val, str_code);
			}
			else fit = PJ_FALSE;

			if (fit && reason_text != NULL)
			{
				if ((str_reason_val_size - strlen(str_reason_val) - strlen(";text=") - 1) > 0)
				{
					strcat(str_reason_val, ";text=");
				}
				else fit = PJ_FALSE;

				if (fit && ((str_reason_val_size - strlen(str_reason_val) - strlen("\"") - strlen(reason_text) - strlen("\"") - 1) > 0))
				{
					strcat(str_reason_val, "\"");
					strcat(str_reason_val, reason_text);
					strcat(str_reason_val, "\"");
				}
				else fit = PJ_FALSE;
			}
			
			pj_str_t reason_val = pj_str(str_reason_val);
			pjsip_generic_string_hdr_init2(&reason_hdr, &reason, &reason_val);
			pj_list_push_back(&msg_data.hdr_list, &reason_hdr);

			if (!fit)
			{
				PJ_LOG(3, (__FILE__, "ERROR: SipCall::Answer: reason_code o reason_text son demasiado grandes"));
			}
			else
			{
				answer_msg_data = &msg_data;
			}
		}
	}

	if (code == PJSIP_SC_OK)
	{
		//Se envia la cabecera Receive-info:selcal si somos un GRS transmisor o transceptor
		pjsua_call* call;
		pjsip_dialog* dlg;

		pj_status_t st = acquire_call("Answer()", call_id, &call, &dlg);
		PJ_CHECK_STATUS(st, ("ERROR adquiriendo call", "[Call=%d]", call_id));

		pjsua_call_info callInfo;
		st = pjsua_call_get_info(call_id, &callInfo);

		if (st == PJ_SUCCESS && dlg->role == PJSIP_ROLE_UAS)
		{
			if (call->user_data)
			{
				if (((SipCall*)call->user_data)->ED137Version == 'C' && ((SipCall*)call->user_data)->_Info.Type == CORESIP_CALL_RD)
				{
					pj_bool_t rdAccount = PJ_FALSE;				//Indica si add_id es un account tipo radio GRS
					CORESIP_CallFlagsMask TipoGrsFlags = CORESIP_CALL_NINGUNO;

					AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(callInfo.acc_id);
					if (accUserData != NULL)
					{
						rdAccount = accUserData->rdAccount;
						TipoGrsFlags = accUserData->TipoGrsFlags;
						pj_bool_t radio_grs = (SipAgent::_Radio_UA != 0 || rdAccount == PJ_TRUE);
						pj_bool_t grs_tx_txrx = ((TipoGrsFlags & CORESIP_CALL_RD_RXONLY) == 0);
						if (radio_grs && grs_tx_txrx && accUserData->SelCalSupported)
						{
							//Es un GRS que no es solo receptor y soporta SELCAL
							if (answer_msg_data == NULL)
							{
								pjsua_msg_data_init(&msg_data);
							}
							pjsip_generic_string_hdr receive_info_hdr;
							pj_str_t receive_info_name = pj_str("Receive-info");
							pj_str_t receive_info_val = pj_str("selcal");
							pjsip_generic_string_hdr_init2(&receive_info_hdr, &receive_info_name, &receive_info_val);
							pj_list_push_back(&msg_data.hdr_list, &receive_info_hdr);
							answer_msg_data = &msg_data;
						}
					}
				}
			}
		}
		pjsip_dlg_dec_lock(dlg);
	}	
	
	pj_status_t st = pjsua_call_answer(call_id, code, NULL, answer_msg_data);
	PJ_CHECK_STATUS(st, ("ERROR respondiendo llamada", "[Call=%d] [Code=%d]", call_id, code));
}

/**
* 
*/
void SipCall::MovedTemporallyAnswer(pjsua_call_id call_id, const char *dst_uri, const char *reason)
{
	if (call_id<0 || call_id>(int)pjsua_var.ua_cfg.max_calls)
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("MovedTemporallyAnswer:", "call_id %d no valido", call_id);		
	}

	pjsua_call * call;
	pjsip_dialog * dlg;

	pj_status_t st = acquire_call("MovedTemporallyAnswer()", call_id, &call, &dlg);
	PJ_CHECK_STATUS(st, ("ERROR adquiriendo call en MovedTemporallyAnswer", "[Call=%d]", call_id));

	SipCall *sipcall = (SipCall *) call->user_data;
	if (!sipcall)
	{
		pjsip_dlg_dec_lock(dlg);
		PJ_CHECK_STATUS(PJ_EINVALIDOP, ("ERROR adquiriendo SipCall->user_data en MovedTemporallyAnswer", "[Call=%d]", call_id));
		return;
	}

	/*Ponemos la cabecera diversion con la Request-URI recibida*/
	pjsua_msg_data msg_data;
    pjsip_generic_string_hdr div_hdr;
    pj_str_t hname = pj_str("Diversion");
	pj_str_t hvalue;

	hvalue.ptr = (char*)pj_pool_alloc(dlg->pool, strlen(sipcall->IncommingReqUri)+1+strlen(reason)+1);
	if (hvalue.ptr == NULL)
	{
		pjsip_dlg_dec_lock(dlg);
		st = PJ_ENOMEM;
		PJ_CHECK_STATUS(st, ("ERROR Sin memoria en MovedTemporallyAnswer", "[Call=%d]", call_id));
	}
	strcpy(hvalue.ptr, sipcall->IncommingReqUri);
	strcat(hvalue.ptr, ";");
	strcat(hvalue.ptr, reason);
	hvalue.slen = (pj_ssize_t) strlen(hvalue.ptr);
    
    /* Add header */
    pjsua_msg_data_init(&msg_data);
    pjsip_generic_string_hdr_init2(&div_hdr, &hname, &hvalue);
    pj_list_push_back(&msg_data.hdr_list, &div_hdr);

	/*Modificamos el contact con el destino del desvio*/
	pj_str_t tmp;
	pj_str_t contact_str = pj_str((char *) dst_uri);
	pj_str_t HCONTACT = { "Contact", 7 };
	pj_strdup_with_null(dlg->pool, &tmp, &contact_str);
	pjsip_contact_hdr *contacthdr = (pjsip_contact_hdr*) pjsip_parse_hdr(dlg->pool, &HCONTACT, tmp.ptr, tmp.slen, NULL);
	if (contacthdr == NULL)
	{
		pjsip_dlg_dec_lock(dlg);
		st = PJ_ENOMEM;
		PJ_CHECK_STATUS(st, ("ERROR MovedTemporallyAnswer. No se puede crear nuevo Contact", "[Call=%d] [Code=%d]", call_id, PJSIP_SC_MOVED_TEMPORARILY));
	}

	dlg->local.contact = contacthdr;

	st = pjsua_call_answer(call_id, PJSIP_SC_MOVED_TEMPORARILY, NULL, &msg_data);
	pjsip_dlg_dec_lock(dlg);
	PJ_CHECK_STATUS(st, ("ERROR MovedTemporallyAnswer", "[Call=%d] [Code=%d]", call_id, PJSIP_SC_MOVED_TEMPORARILY));
}

/**
* 
*/
void SipCall::CallProccessRedirect(pjsua_call_id call_id, const char *dstUri, CORESIP_REDIRECT_OP op)
{
	if (call_id<0 || call_id>(int)pjsua_var.ua_cfg.max_calls)
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("CallProccessRedirect:", "call_id %d no valido", call_id);		
	}

	pjsip_redirect_op cmd;
	pj_status_t st;

	pjsua_call* pjcall;
	pjsip_dialog* dlg;
	st = acquire_call("CallProccessRedirect()", call_id, &pjcall, &dlg);
	PJ_CHECK_STATUS(st, ("ERROR adquiriendo call", "[Call=%d]", call_id));

	SipCall* call = (SipCall*)pjcall->user_data;
	if (call == NULL)
	{
		cmd = PJSIP_REDIRECT_STOP;
		st = pjsua_call_process_redirect(call_id, cmd);
		pjsip_dlg_dec_lock(dlg);
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("CallProccessRedirect:", "El objeto SipCall es NULL");
		return;
	}

	pjsip_tx_data* tdata = pjcall->inv->invite_req;					//Mensaje transmitido en el anterior invite

	pjsip_uri* new_req_uri = pjsip_parse_uri(tdata->pool, (char *) dstUri, strlen(dstUri), 0);
	if (new_req_uri == NULL)
	{
		PJ_LOG(3, (__FILE__, "ERROR: CallProccessRedirect: La nueva request URI no es valida %s, se rechaza el desvio", dstUri));
		//Se procede a cerrar la sesion de este call id		
		call->_HangUp = true;
		cmd = PJSIP_REDIRECT_STOP;
		st = pjsua_call_process_redirect(call_id, cmd);
		pjsip_dlg_dec_lock(dlg);
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("CallProccessRedirect:", "La nueva request URI no es valida %s, se rechaza el desvio", dstUri);
		return;
	}

	if (op == CORESIP_REDIRECT_ACCEPT)
	{
		//Se generara una nu nuevo invite. Le ponemos las cabecera History-Info		
		pjsip_uri* req_uri_prev = tdata->msg->line.req.uri;				//Tomo la request uri del invite original
		char req_uri_buf[CORESIP_MAX_URI_LENGTH + 1];
		char history_val1[CORESIP_MAX_URI_LENGTH + CORESIP_MAX_TAG_LENGTH + 1];
		char history_val2[CORESIP_MAX_URI_LENGTH + CORESIP_MAX_TAG_LENGTH + 1];
		pj_str_t pjhistory_val1;
		pjhistory_val1.ptr = NULL;
		pjhistory_val1.slen = 0;
		pj_str_t pjhistory_val2;
		pjhistory_val2.ptr = NULL;
		pjhistory_val2.slen = 0;		

		//Ponemos un History-Info con la request uri original
		int len = pjsip_uri_print(PJSIP_URI_IN_REQ_URI, req_uri_prev, req_uri_buf, sizeof(req_uri_buf) - 1);
		if (len >= 0)
		{
			req_uri_buf[len] = '\0';
			len = pj_ansi_snprintf(history_val1, sizeof(history_val1) - 1, "<%s>;index=1", req_uri_buf);
			if (len > 0)
			{
				history_val1[len] = '\0';
				pjhistory_val1 = pj_str(history_val1);
			}
		}

		//Ponemos un History-Info con el request nuevo
		pjsip_uri* req_uri_curr = dlg->target_set.current->uri;
		len = pjsip_uri_print(PJSIP_URI_IN_REQ_URI, req_uri_curr, req_uri_buf, sizeof(req_uri_buf) - 1);
		if (len >= 0)
		{
			req_uri_buf[len] = '\0';
			len = pj_ansi_snprintf(history_val2, sizeof(history_val2) - 1, "<%s>;index=2", req_uri_buf);
			if (len > 0)
			{
				history_val2[len] = '\0';
				pjhistory_val2 = pj_str(history_val2);
			}
		}

		if (pjhistory_val1.ptr != NULL && pjhistory_val2.ptr != NULL)
		{
			//SE pueden poner las dos cabeceras
			pj_str_t hist_name = { "History-Info", 12 };
			pjsip_generic_string_hdr* pHistHdr = pjsip_generic_string_hdr_create(tdata->pool, &hist_name, &pjhistory_val1);
			pj_list_push_back(&tdata->msg->hdr, pHistHdr);
			pHistHdr = pjsip_generic_string_hdr_create(tdata->pool, &hist_name, &pjhistory_val2);
			pj_list_push_back(&tdata->msg->hdr, pHistHdr);
		}
		else
		{
			PJ_LOG(3, (__FILE__, "ERROR: CallProccessRedirect: No se han podido poner las dos cabeceras History-Info. Request uris demasiado grandes"));
		}

		//Ponemos la nueva request uri
		tdata->msg->line.req.uri = new_req_uri;

		cmd = PJSIP_REDIRECT_ACCEPT;
	}
	else
	{
		//Se procede a cerrar la sesion de este call id		
		call->_HangUp = true;
		cmd = PJSIP_REDIRECT_STOP;
	}

	st = pjsua_call_process_redirect(call_id, cmd);
	pjsip_dlg_dec_lock(dlg);
	PJ_CHECK_STATUS(st, ("ERROR CallProccessRedirect", "[Call=%d] [op=%d]", call_id, op));
}

/**
* 
*/
void SipCall::Hold(pjsua_call_id call_id, bool hold)
{
	pjsua_call * pjcall;
	pjsip_dialog * dlg;
	int tries = 5;

	if (call_id<0 || call_id>(int)pjsua_var.ua_cfg.max_calls)
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("Hold:", "call_id %d no valido", call_id);		
	}

	pj_status_t st = acquire_call("Hold()", call_id, &pjcall, &dlg);
	PJ_CHECK_STATUS(st, ("ERROR adquiriendo call", "[Call=%d]", call_id));

	SipCall * call = (SipCall*)pjcall->user_data;
	if (call->_Info.Type == CORESIP_CALL_RD)
	{
		pjsip_dlg_dec_lock(dlg);
	}
	else 
	{
		pjsua_msg_data msgData;
		pjsip_generic_string_hdr subject, priority;

		pjsua_msg_data_init(&msgData);

		pjsip_generic_string_hdr_init2(&subject, &gSubjectHdr, &gSubject[call->_Info.Type]);
		pj_list_push_back(&msgData.hdr_list, &subject);
		pjsip_generic_string_hdr_init2(&priority, &gPriorityHdr, &gPriority[call->_Info.Priority]);
		pj_list_push_back(&msgData.hdr_list, &priority);			

		tries = (int) pjsip_sip_cfg_var.tsx.tsx_tout / 20;
		if (tries <= 1) tries = 2;
		while ((pjcall->inv->state != PJSIP_INV_STATE_NULL &&
			pjcall->inv->state != PJSIP_INV_STATE_CONFIRMED) 
			||
			(pjcall->inv->invite_tsx!=NULL))
		{
			//Hay una transacción de invite pendiente. Posiblemente porque el colateral 
			//tarda en contestar. 
			//Por tanto en este bucle se espera un maximo de pjsip_sip_cfg_var.tsx.tsx_tout (ms)
			//a que termine una trasaccion anterior

			pjsip_dlg_dec_lock(dlg);
			pj_thread_sleep(20);

			st = acquire_call("Hold()", call_id, &pjcall, &dlg);
			PJ_CHECK_STATUS(st, ("ERROR adquiriendo call", "[Call=%d]", call_id));

			if (--tries == 0) break;
		}		

		if (hold)
		{			
			st = pjsua_call_set_hold(call_id, &msgData);	
			pjsip_dlg_dec_lock(dlg);		
			PJ_CHECK_STATUS(st, ("ERROR aparcando llamada", "[Call=%d]", call_id));		
		}
		else
		{
			st = pjsua_call_reinvite(call_id, PJ_TRUE, NULL, &msgData);
			pjsip_dlg_dec_lock(dlg);	
			PJ_CHECK_STATUS(st, ("ERROR reanudando llamada", "[Call=%d]", call_id));
		}
	}

}

/**
*	Reinvite
*	@param	call_id		Identificador de llamada
*	@param	CallType_SDP	9 couplig, 7 Radio-Rxonly, 5 Radio-TxRx, 6 Radio-Idle
*	@param	TxRx_SDP		4 Rx, 8 Tx, 0 TxRx, 22 Vacio
*	@param	etm_vcs_bss_methods	Para ETM, string con los literales de los metodos BSS separados por comas. El string debe terminar caracter '\0'. Si vale NULL se ignora
*   @param	ForceSDPSendRecvAttr	Sirve para forzar el valor de del atributo send-recv en el SDP 
*	@return				
*/
void SipCall::Reinvite(pjsua_call_id call_id, int CallType_SDP, int TxRx_SDP, char* etm_vcs_bss_methods, CORESIP_SDPSendRecvAttrForced ForceSDPSendRecvAttr)
{
	pjsua_call* pjcall;
	pjsip_dialog* dlg;
	int tries = 5;

	if (call_id<0 || call_id>(int)pjsua_var.ua_cfg.max_calls)
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("Reinvite:", "call_id %d no valido", call_id);
	}

	pj_status_t st = acquire_call("Reinvite()", call_id, &pjcall, &dlg);
	PJ_CHECK_STATUS(st, ("ERROR adquiriendo call", "[Call=%d]", call_id));

	SipCall* call = (SipCall*)pjcall->user_data;
	if (call == NULL)
	{
		pjsip_dlg_dec_lock(dlg);
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("Reinvite:", "call_id %d. No existe el objeto SipCall", call_id);
		return;
	}

	if (call->_Info.Type != CORESIP_CALL_RD)
	{
		pjsip_dlg_dec_lock(dlg);
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("Reinvite:", "call_id %d. No es una llamada del tipo Radio", call_id);
		return;
	}

	if (etm_vcs_bss_methods)
	{
		if (strlen(etm_vcs_bss_methods) < sizeof(call->_Info.etm_vcs_bss_methods))
		{
			strcpy(call->_Info.etm_vcs_bss_methods, etm_vcs_bss_methods);
		}
		else
		{
			pjsip_dlg_dec_lock(dlg);
			throw PJLibException(__FILE__, PJ_EINVAL).Msg("Reinvite:", "etm_bss_methods es demasiado largo");
			return;
		}
	}
	
	pjsua_msg_data msgData;
	pjsip_generic_string_hdr subject, priority, wg67ver;

	pjsua_msg_data_init(&msgData);

	pjsip_generic_string_hdr_init2(&subject, &gSubjectHdr, &gSubject[call->_Info.Type]);
	pj_list_push_back(&msgData.hdr_list, &subject);
	pjsip_generic_string_hdr_init2(&priority, &gPriorityHdr, &gPriority[call->_Info.Priority]);
	pj_list_push_back(&msgData.hdr_list, &priority);

	if (call->_Info.Type != CORESIP_CALL_RD)
	{
		pjsip_generic_string_hdr_init2(&wg67ver, &gWG67VersionName, &gWG67VersionTelefValue);
	}
	else
	{
		char ED137Radioversion_, ED137Phoneversion_;
		pjsip_endpt_Get_Ed137_version(pjsua_var.endpt, &ED137Radioversion_, &ED137Phoneversion_);

		//Ponemos aqui la cabecera WG67-version
		char sWG67ver[32];
		if (ED137Radioversion_ == 'C')
			pj_ansi_sprintf(sWG67ver, "%.*s, %.*s", gWG67VersionRadioValue.slen, gWG67VersionRadioValue.ptr, gWG67VersionRadioValueC.slen, gWG67VersionRadioValueC.ptr);
		else
			pj_ansi_sprintf(sWG67ver, "%.*s", gWG67VersionRadioValue.slen, gWG67VersionRadioValue.ptr);

		pjsip_generic_string_hdr_init2(&wg67ver, &gWG67VersionName, &pj_str(sWG67ver));
	}
	pj_list_push_back(&msgData.hdr_list, &wg67ver);

	tries = (int)pjsip_sip_cfg_var.tsx.tsx_tout / 20;
	if (tries <= 1) tries = 2;
	while ((pjcall->inv->state != PJSIP_INV_STATE_NULL &&
		pjcall->inv->state != PJSIP_INV_STATE_CONFIRMED)
		||
		(pjcall->inv->invite_tsx != NULL))
	{
		//Hay una transacción de invite pendiente. Posiblemente porque el colateral 
		//tarda en contestar. 
		//Por tanto en este bucle se espera un maximo de pjsip_sip_cfg_var.tsx.tsx_tout (ms)
		//a que termine una trasaccion anterior

		pjsip_dlg_dec_lock(dlg);
		pj_thread_sleep(20);

		st = acquire_call("Reinvite()", call_id, &pjcall, &dlg);
		PJ_CHECK_STATUS(st, ("ERROR adquiriendo call", "[Call=%d]", call_id));

		if (--tries == 0) break;
	}

	call->Callflag_previous_reinvite = call->_Info.CallFlags;

	call->_Info.CallFlags = 0;
	switch (CallType_SDP)
	{
	case 9:
		call->_Info.CallFlags |= CORESIP_CALL_RD_COUPLING;
		break;
	case 7:
		call->_Info.CallFlags |= CORESIP_CALL_RD_RADIO_RXONLY;
		break;
	case 5:
		call->_Info.CallFlags |= 0;
		break;
	case 6:
		call->_Info.CallFlags |= CORESIP_CALL_RD_IDLE;
		break;
	}

	switch (TxRx_SDP)
	{
	case 22:
		call->_Info.CallFlags |= CORESIP_CALL_NO_TXRXMODE;
		break;
	case 4:
		call->_Info.CallFlags |= CORESIP_CALL_RD_RXONLY;
		break;
	case 8:
		call->_Info.CallFlags |= CORESIP_CALL_RD_TXONLY;
		break;
	}	

	call->_Info.ForceSDPSendRecvAttr = ForceSDPSendRecvAttr;

	st = pjsua_call_reinvite(call_id, PJ_TRUE, NULL, &msgData);
	pjsip_dlg_dec_lock(dlg);
	PJ_CHECK_STATUS(st, ("ERROR Reinvite", "[Call=%d]", call_id));
}

/**
* 
*/
void SipCall::Transfer(pjsua_call_id call_id, pjsua_call_id dest_call_id, const char * dst, const char *referto_display_name)
{
	if (call_id<0 || call_id>(int)pjsua_var.ua_cfg.max_calls)
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("Transfer:", "call_id %d no valido", call_id);		
	}

	pjsua_call * call1;
	pjsip_dialog * dlg1;
	pj_status_t status = acquire_call("Transfer()", call_id, &call1, &dlg1);
	PJ_CHECK_STATUS(status, ("ERROR adquiriendo call", "[Call=%d]", call_id));
	pjsip_dlg_dec_lock(dlg1);

	/*pj_status_t st = (dest_call_id != PJSUA_INVALID_ID) ?
	pjsua_call_xfer_replaces(call_id, dest_call_id, 0, NULL) :
	pjsua_call_xfer(call_id, &(pj_str(const_cast<char*>(dst))), NULL);
	PJ_CHECK_STATUS(st, ("ERROR en transferencia de llamada", "[Call=%d][DstCall=%d] [dst=%s]", call_id, dest_call_id, dst));*/

	pj_status_t st;
	if (dest_call_id != PJSUA_INVALID_ID) 
		st = pjsua_call_xfer_replaces_dispname(call_id, dest_call_id, (char *) referto_display_name, 0, NULL);
	else
		st = pjsua_call_xfer(call_id, &(pj_str(const_cast<char*>(dst))), NULL);
	PJ_CHECK_STATUS(st, ("ERROR en transferencia de llamada", "[Call=%d][DstCall=%d] [dst=%s]", call_id, dest_call_id, dst));
}

/**
* 
*/
void SipCall::Conference(pjsua_call_id call_id, bool conf)
{
	if (call_id<0 || call_id>(int)pjsua_var.ua_cfg.max_calls)
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("Conference:", "call_id %d no valido", call_id);		
	}

	static pj_str_t focus_param = { ";isfocus", 8 };

	pjsua_call * pjcall;
	pjsip_dialog * dlg;
	pj_str_t contact = { NULL, 0 };

	pj_status_t st = acquire_call("Conference()", call_id, &pjcall, &dlg);
	PJ_CHECK_STATUS(st, ("ERROR adquiriendo call", "[Call=%d]", call_id));

	SipCall * call = (SipCall*)pjcall->user_data;

	if (call)
	{
		if (!conf && call->_ConfInfoSrvEvSub)
		{
			pjsip_tx_data * tdata;
			pjsip_conf_status info;

			info.version = 0xFFFFFFFF;
			info.state = pj_str("deleted");
			info.users_cnt = 0;

			pjsip_conf_set_status(call->_ConfInfoSrvEvSub, &info);

			if (PJ_SUCCESS == pjsip_conf_notify(call->_ConfInfoSrvEvSub, PJSIP_EVSUB_STATE_TERMINATED, NULL, NULL, &tdata))
			{
				pjsip_conf_send_request(call->_ConfInfoSrvEvSub, tdata);
			}
		}

		/*A�adimos Subject y Priority en los re-invites*/
		pjsua_msg_data msgData;
		pjsip_generic_string_hdr subject, priority;

		pjsua_msg_data_init(&msgData);

		pjsip_generic_string_hdr_init2(&subject, &gSubjectHdr, &gSubject[call->_Info.Type]);
		pj_list_push_back(&msgData.hdr_list, &subject);
		pjsip_generic_string_hdr_init2(&priority, &gPriorityHdr, &gPriority[call->_Info.Priority]);
		pj_list_push_back(&msgData.hdr_list, &priority);

		// pjsua_acc_create_uac_contact(call->_Pool, &contact, call->_Info.AccountId, NULL, conf ? &focus_param : NULL);
		pjsua_acc_create_uac_contact(call->_Pool, &contact, ((call->_Info.AccountId) & CORESIP_ID_MASK), NULL, conf ? &focus_param : NULL);
		st = pjsua_call_reinvite(call_id, PJ_TRUE, &contact, &msgData);
	}
	else
	{
		st = PJSIP_ESESSIONSTATE;
	}

	pjsip_dlg_dec_lock(dlg);
	PJ_CHECK_STATUS(st, ("ERROR modificando conferencia", "[Call=%d] [Modi=%s]", call_id, conf ? "Add" : "Remove"));
}

/**
* 
*/
void SipCall::SendConfInfo(pjsua_call_id call_id, const CORESIP_ConfInfo * conf)
{
	if (call_id<0 || call_id>(int)pjsua_var.ua_cfg.max_calls)
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("SendConfInfo:", "call_id %d no valido", call_id);		
	}

	pjsua_call * pjcall;
	pjsip_dialog * dlg;

	pj_status_t st = acquire_call("SendConfInfo()", call_id, &pjcall, &dlg);
	PJ_CHECK_STATUS(st, ("ERROR adquiriendo call", "[Call=%d]", call_id));

	SipCall * call = (SipCall*)pjcall->user_data;

	if (call && call->_ConfInfoSrvEvSub)
	{
		pjsip_conf_status info;

		info.version = conf->Version;
		info.state = pj_str(const_cast<char*>(conf->State));
		info.users_cnt = conf->UsersCount;

		for (unsigned i = 0; i < conf->UsersCount; i++)
		{
			info.users[i].id = pj_str(const_cast<char*>(conf->Users[i].Id));
			info.users[i].display = pj_str(const_cast<char*>(conf->Users[i].Name));
			info.users[i].role = pj_str(const_cast<char*>(conf->Users[i].Role));
			info.users[i].state = pj_str(const_cast<char*>(conf->Users[i].State));
		}

		st = pjsip_conf_set_status(call->_ConfInfoSrvEvSub, &info);
		if (st == PJ_SUCCESS) 
		{
			pjsip_tx_data * tdata;
			st = pjsip_conf_current_notify(call->_ConfInfoSrvEvSub, &tdata);
			if (st == PJ_SUCCESS)
			{
				PJ_LOG(5, ("sipcall.cpp", "NOTIFY CONF: Envia conference NOTIFY %s", pjsip_evsub_get_state_name(call->_ConfInfoSrvEvSub)));
				st = pjsip_conf_send_request(call->_ConfInfoSrvEvSub, tdata);
			}
		}
	}

	pjsip_dlg_dec_lock(dlg);
	PJ_CHECK_STATUS(st, ("ERROR enviando usuarios de conferencia", "[Call=%d]", call_id));
}

/**
* 
*/
void SipCall::SendInfoMsg(pjsua_call_id call_id, const char * info)
{
	if (call_id<0 || call_id>(int)pjsua_var.ua_cfg.max_calls)
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("SendInfoMsg:", "call_id %d no valido", call_id);		
	}

	pjsua_call * call1;
	pjsip_dialog * dlg1;
	pj_status_t status = acquire_call("SendInfoMsg()", call_id, &call1, &dlg1);
	PJ_CHECK_STATUS(status, ("ERROR adquiriendo call", "[Call=%d]", call_id));
	pjsip_dlg_dec_lock(dlg1);

	pjsua_msg_data msgData;
	pj_status_t st;
	pjsua_msg_data_init(&msgData);

	msgData.content_type = pj_str("text/plain");
	msgData.msg_body = pj_str(const_cast<char*>(info));

	static pj_str_t method = { "INFO", 4 };
	st = pjsua_call_send_request(call_id, &method, &msgData);
	PJ_CHECK_STATUS(st, ("ERROR enviando INFO", "[Call=%d]", call_id));
}

/**
* 
*/
void SipCall::TransferAnswer(const char * tsxKey, void * txData, void * evSub, unsigned code)
{
	pj_status_t st = pjsua_call_transfer_answer(code, &(pj_str(const_cast<char*>(tsxKey))), 
		(pjsip_tx_data*)txData, (pjsip_evsub*)evSub);
	PJ_CHECK_STATUS(st, ("ERROR en respuesta a peticion de transferencia"));
}

/**
* 
*/
void SipCall::TransferNotify(void * evSub, unsigned code)
{
	pjsip_evsub_state evSt = (code < 200) ? PJSIP_EVSUB_STATE_ACTIVE : PJSIP_EVSUB_STATE_TERMINATED;
	pj_status_t st = pjsua_call_transfer_notify(code, evSt, (pjsip_evsub*)evSub);
	PJ_CHECK_STATUS(st, ("ERROR notificando estado de la transferencia"));
}

CORESIP_CallInfo *SipCall::GetCORESIP_CallInfo()
{
	return &_Info;
}

/**
* Trata los Cambios de estado de una Llamada.
* http://www.pjsip.org/docs/latest-1/pjsip/docs/html/structpjsua__callback.htm#a06e6135aeaa81b32fdc66fa603a0546c
*/
void SipCall::OnStateChanged(pjsua_call_id call_id, pjsip_event * e)
{
	static pj_str_t _gSubjectHdr = { "Reason", 6 };

	pj_assert(SipAgent::Cb.CallStateCb);
	
	pjsua_call_info callInfo;
	if (pjsua_call_get_info(call_id, &callInfo) != PJ_SUCCESS)
	{	
		PJ_LOG(3, (__FILE__, "WARNING: OnStateChanged: pjsua_call_get_info retorna error"));
		return;
	}

	//last_status {callInfo.last_status}, state {callInfo.state} {callInfo.state_text.ptr}

	SipCall * call = (SipCall*)pjsua_call_get_user_data(call_id);
	if (!call) return;

	call->_Last_pjsip_inv_state = callInfo.state;

	if (call->_Info.Type != CORESIP_CALL_IA)
	{
		//Los eventos de dialogo de llamada del tipo IA no se envian
		SubscriptionServer::SendDialogNotifyFromAcc(call_id, DlgSubs::WITH_BODY);
	}

	CORESIP_CallStateInfo stateInfo = { (CORESIP_CallState)callInfo.state, (CORESIP_CallRole)callInfo.role };
	stateInfo.remote_grs_supports_ED137C_Selcal = call->remote_grs_supports_ED137C_Selcal;

	if (call->_HangUp)
	{
		if (callInfo.state == PJSIP_INV_STATE_DISCONNECTED)
		{
			AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(callInfo.acc_id);
			pj_bool_t radio_grs = PJ_FALSE;
			//Enviamos las notificaciones al evento WG67KEY-IN	
			if (call->_Info.Type == CORESIP_CALL_RD && accUserData != NULL)
			{
				pj_bool_t rdAccount = PJ_FALSE;				//Indica si add_id es un account tipo radio GRS
				rdAccount = accUserData->rdAccount;
				radio_grs = (SipAgent::_Radio_UA != 0 || rdAccount == PJ_TRUE);
				if (radio_grs)
				{
					SubscriptionServer::SendWG67NotifyFromAcc(call_id, PJ_TRUE, AccountUserData::NO_ENVIAR_WG67_NOTIFY_SI_TODAS_LAS_SESIONES_SON_FICTICIAS);
				}
			}

			if (call->_Info.Type == CORESIP_CALL_RD)
			{
				call->valid_sess = PJ_FALSE;						
				EliminarRadSessionDelGrupo(call);					
			}
			
			if (call->_Info.Type != CORESIP_CALL_RD)
			{
				pj_str_t* callIdHdrVal = NULL;
				pjsua_call* pjcall;
				pjsip_dialog* dlg;
				pj_status_t st = acquire_call("OnStateChanged()", call_id, &pjcall, &dlg);
				if (st == PJ_SUCCESS)
				{
					callIdHdrVal = &dlg->call_id->id;
					SipAgent::RecCallEnd(Q850_NORMAL_CALL_CLEARING, pjcall->media_st, CALLEND_UNKNOWN, callIdHdrVal);
					pjsip_dlg_dec_lock(dlg);
				}
				else
				{
					SipAgent::RecCallEnd(Q850_NORMAL_CALL_CLEARING, PJSUA_CALL_MEDIA_ACTIVE, CALLEND_UNKNOWN, callIdHdrVal);
				}
			}

			pj_str_t* remote_uri = NULL;
			if (call->_Dlg) remote_uri = &call->_Dlg->remote.info_str;
			SipAgent::RecBYE(remote_uri, call->_Info.Type);

			SignalGen::StopAllTones(call_id);
			SignalGen::StopNoiseToCall(call_id);
			SignalGen::StopDTMF(call_id);

			// Debe mandarse al cliente info de la desconexi�n de la llamada
			pjsua_call_set_user_data(call_id, (void *) NULL);  //Este call id deja de tener un SipCall asociado	
			stateInfo.isRadReinvite = 0;
			stateInfo.radReinvite_accepted = 0;			
			stateInfo.radRreinviteCallFlags = call->_Info.CallFlags;
			stateInfo.remote_grs_supports_ED137C_Selcal = call->remote_grs_supports_ED137C_Selcal;
			if (TRACE_ALL_CALLS) PJ_LOG(3, (__FILE__, "CallStateCb %d", call_id | CORESIP_CALL_ID));
			SipAgent::Cb.CallStateCb(call_id | CORESIP_CALL_ID, &call->_Info, &stateInfo);				

			if (call->assigned_pttid != 0)
			{
				if (accUserData != NULL)
				{
					accUserData->FreePTTID(call->assigned_pttid);
				}
			}

			if (pjsip_endpt_Is_ETM(pjsua_var.endpt))
			{
				//En el caso del ETM. Por orden de Enaire, cortamos el envio de RTP al hacer el hangup

				pjmedia_stream* stream = NULL;
				pjmedia_session* session = NULL;

				session = pjsua_call_get_media_session(call_id);
				if (session != NULL)
				{
					stream = pjmedia_session_get_stream(session, 0);
					if (stream != NULL)
					{
						pjmedia_stream_force_no_send_rtp(stream, PJ_TRUE);
					}
				}
			}

			call->IniciaFinSesion();
		}
		else
		{
			PJ_LOG(3,(__FILE__, "WARNING: OnStateChanged: Estado incorrecto despues de solicitar Hangup DstUri %s state %d %s.\n", call->DstUri, callInfo.state, callInfo.state_text.ptr));			
			if (callInfo.state == PJSIP_INV_STATE_CONFIRMED)
			{
				PJ_LOG(3,(__FILE__, "WARNING: OnStateChanged: Estado CONFIRMED despues de solicitar Hangup. Se fuerza Hangup DstUri %s state %d %s.\n", call->DstUri, callInfo.state, callInfo.state_text.ptr));			
				pj_status_t s1 = pjsua_call_hangup(call_id, 0, NULL, NULL);
				if (s1 != PJ_SUCCESS)
				{
					PJ_LOG(3,(__FILE__, "WARNING: OnStateChanged: pjsua_call_hangup retorna error 0x%X\n", s1));			
				}
			}			
		}						

		return;
	}

	pjmedia_session * sess = NULL;
	pjmedia_session_info sess_info;

	if (callInfo.state == PJSIP_INV_STATE_CALLING)
	{
		pjsua_call* pjcall;
		pjsip_dialog* dlg;
		pj_status_t status;

		status = acquire_call("OnStateChanged()", call_id, &pjcall, &dlg);
		if (status == PJ_SUCCESS) {
			call->_Dlg = dlg;
			call->_Acc_id = callInfo.acc_id;
			pjsip_dlg_dec_lock(dlg);
		}
		else
		{
			PJ_LOG(3, (__FILE__, "ERROR: OnStateChanged: acquire_call retorna error"));
			call->_Dlg = NULL;
			call->_Acc_id = PJSUA_INVALID_ID;
		}
	}
	else if (callInfo.state == PJSIP_INV_STATE_CONFIRMED)
	{
		sess = pjsua_call_get_media_session(call_id);
		if (pjmedia_session_get_info(sess, &sess_info ) != PJ_SUCCESS)
		{
			PJ_LOG(3,(__FILE__, "ERROR: OnStatechanged pjmedia_session_get_info retorna error")); 
			return;
		}
		call->_Payload_type = sess_info.stream_info[0].fmt.pt;
	}
	
	if (callInfo.role == PJSIP_ROLE_UAC)
	{
		pjmedia_session * sess = pjsua_call_get_media_session(call_id);
		if (callInfo.state == PJSIP_INV_STATE_CONFIRMED)
		{
			char radio_version = 0;
			char phone_version = 0;
			char WG67_version_value_buf[32];

			//Cuando pasa por aqui ya se ha enviado el ACK y sirve para comprobar que las versiones son correctas y compatibles y que no hay que enviar un BYE con reason 1005
			if (pjsip_endpt_Neg_ED137Version_from_msg(pjsua_var.endpt, e->body.tsx_state.tsx->last_tx->msg, e->body.rx_msg.rdata->msg_info.msg, &radio_version, &phone_version,
				WG67_version_value_buf, sizeof(WG67_version_value_buf)) == PJ_FALSE)
			{
				//Versiones incompatibles. 
				PJ_LOG(3, (__FILE__, "INFO: OnStatechanged WG67-version recibida es incompatible"));
				call->Hangup(call_id, 1005);
				return;
			}

			if (call->_Info.Type == CORESIP_CALL_RD) call->ED137Version = radio_version;
			else call->ED137Version = phone_version;

			if (call->_Info.Type == CORESIP_CALL_RD)
			{
				pjmedia_session_info sess_info;
					
				if (sess != NULL)
				{
					pjsua_call * psuacall = (pjsua_call *) pjmedia_session_get_user_data((pjmedia_session*)sess);
					pjmedia_transport_info tpinfo;
					pjmedia_transport_info_init(&tpinfo);
					pjmedia_transport_get_info(psuacall->med_tp, &tpinfo);
						
					pjmedia_session_get_info(sess, &sess_info);

					char *bss_method = NULL;
					if (sess_info.stream_cnt > 0)
					{						
						stateInfo.PttId = (unsigned short)sess_info.stream_info[0].pttId;
						stateInfo.ClkRate = sess_info.stream_info[0].param->info.clock_rate;
						stateInfo.ChannelCount = sess_info.stream_info[0].param->info.channel_cnt;
						stateInfo.BitsPerSample = sess_info.stream_info[0].param->info.pcm_bits_per_sample;
						stateInfo.FrameTime = sess_info.stream_info[0].param->info.frm_ptime;

						bss_method = sess_info.stream_info[0].bss_method;
					}						
						
					int n_sessions_in_his_group = SipAgent::_FrecDesp->AddToGroup(call->_IdDestino, call->_Id, call, bss_method, call->_Info.CallFlags);
					if (n_sessions_in_his_group < 0)
					{
						PJ_LOG(3,(__FILE__, "ERROR: RdDestId %s, call id %d cannot be added to any CLIMAX group\n", call->_IdDestino, call->_Id));
					}	
					else
					{
						/*Arrancamos en cualquier caso el Check_CLD_timer. Aunque no sea un grupo todavia*/

						/*Se para el timer de supervision CLD, por si estuviera arrancado*/
						call->Check_CLD_timer.id = Check_CLD_timer_IDLE;
						pjsua_cancel_timer(&call->Check_CLD_timer);

						call->valid_sess = PJ_TRUE;

						pj_status_t st = PJ_SUCCESS;

						/*Se arranca el timer de supervision CLD,*/						
						pj_time_val	delay;
						delay.sec = 0;
						delay.msec = 10;		//Pide el MAM cuanto antes

						pj_timer_entry_init( &call->Check_CLD_timer, Check_CLD_timer_SEND_RMM, (void *) call, Check_CLD_timer_cb);
						st = pjsua_schedule_timer(&call->Check_CLD_timer, &delay);
						if (st != PJ_SUCCESS)
						{
							call->Check_CLD_timer.id = Check_CLD_timer_IDLE;
							PJ_CHECK_STATUS(st, ("ERROR en Check_CLD_timer"));
						}

						call->Wait_init_timer.id = 0;
						pjsua_cancel_timer(&call->Wait_init_timer);

						call->Wait_init_timer.cb = Wait_init_timer_cb;
						call->Wait_init_timer.user_data = (void *) call;
						pj_time_val	delay1;
						delay1.sec = (long) WAIT_INIT_TIME_seg;
						delay1.msec = (long) WAIT_INIT_TIME_ms;	
						call->Wait_init_timer.id = 1;
						st = pjsua_schedule_timer(&call->Wait_init_timer, &delay1);
						if (st != PJ_SUCCESS)
						{
							call->Wait_init_timer.id = 0;
							PJ_CHECK_STATUS(st, ("ERROR en Wait_init_timer"));
						}							
					}

					//Validando el SDP
					const pjmedia_sdp_session* remote_sdp = NULL;
					pj_status_t st = pjmedia_sdp_neg_get_active_remote(psuacall->inv->neg, &remote_sdp);
					if (st != PJ_SUCCESS)
					{
						remote_sdp = NULL;
					}

					unsigned short assigned_pttid = 0;
					if (remote_sdp != NULL)
					{
						SDPUtils::RadioRemoteSdpAnalysisAsUAC(remote_sdp, &call->_Info, &call->radio_session_with_pttid_enabled, &assigned_pttid);
					}

					if (call->radio_session_with_pttid_enabled)
					{
						call->assigned_pttid = assigned_pttid;
					}
				}

				//Buscamos la cabecera Receive-info:selcal. Para notificar si se soporta selcal
				pjsip_generic_string_hdr* receive_info_hdr =
					(pjsip_generic_string_hdr*)pjsip_msg_find_hdr_by_name(e->body.rx_msg.rdata->msg_info.msg, &pj_str("Receive-info"), NULL);
				if (receive_info_hdr != NULL)
				{
					if (pj_stricmp(&receive_info_hdr->hvalue, &pj_str("selcal")) == 0)
					{
						call->remote_grs_supports_ED137C_Selcal = PJ_TRUE;
					}
					else
					{
						call->remote_grs_supports_ED137C_Selcal = PJ_FALSE;
					}
				}
				else
				{
					call->remote_grs_supports_ED137C_Selcal = PJ_FALSE;
				}
			}		
		}
		else if (callInfo.state == PJSIP_INV_STATE_DISCONNECTED)
		{
			SignalGen::StopAllTones(call_id);
			SignalGen::StopNoiseToCall(call_id);
			SignalGen::StopDTMF(call_id);

			pjsua_call_set_user_data(call_id, (void *) NULL);  //Este call id deja de tener un SipCall asociado
			if (call->_Info.Type == CORESIP_CALL_RD)
			{
				call->valid_sess = PJ_FALSE;
				EliminarRadSessionDelGrupo(call);		
			}

			if ((callInfo.last_status == PJSIP_AC_AMBIGUOUS) && (call->_Info.Type == CORESIP_CALL_RD))
			{
				/*Se ha detectado que si las radios retornan el codigo ambiguous se debe a que tienen
				abierta alguna sesion zombie. Por tanto lo que se hace aquí es buscar todas las
				sesiones que existan abiertas con esa radio y cerrarlas*/

				pjsua_call_id call_ids[PJSUA_MAX_CALLS];
				unsigned call_cnt=PJ_ARRAY_SIZE(call_ids);

				pj_status_t sst = pjsua_enum_calls(call_ids, &call_cnt);
				if (sst == PJ_SUCCESS)
				{
					for (unsigned i = 0; i < call_cnt; i++)
					{
						if (call_ids[i] != call_id)
						{
							pjsua_call_info info;
							sst = pjsua_call_get_info(call_ids[i], &info);
							if (sst == PJ_SUCCESS)
							{
								//Todas las sesiones que existan abiertas a la misma uri se cierran
								if (pj_strcmp(&info.remote_info, &callInfo.remote_info) == 0)
								{
									//pjsua_call_hangup(call_ids[i], PJSIP_SC_DECLINE, &(pj_str("Closing zombie session")), NULL);
									Hangup(call_ids[i], PJSIP_AC_AMBIGUOUS);
								}
							}
						}
					}
				}
			}
		}
	}
	else if (callInfo.role == PJSIP_ROLE_UAS)
	{
		////if (callInfo.state == PJSIP_INV_STATE_CONNECTING)
		////{
		//	if ((call->_Id != PJSUA_INVALID_ID) && (call->_Id != call_id))
		//	{
		//		// Call replaced
		//		call = new SipCall(call_id, &call->_TranferInfo);
		//		pjsua_call_set_user_data(call_id, call);
		//	}
		////}
	}

	pj_bool_t radio_grs = PJ_FALSE;	

	//Enviamos las notificaciones al evento WG67KEY-IN	
	if (call->_Info.Type == CORESIP_CALL_RD)
	{
		pj_bool_t rdAccount = PJ_FALSE;				//Indica si add_id es un account tipo radio GRS

		AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(callInfo.acc_id);
		if (accUserData != NULL)
		{
			rdAccount = accUserData->rdAccount;
		}
		radio_grs = (SipAgent::_Radio_UA != 0 || rdAccount == PJ_TRUE);

		if (radio_grs)
		{
			if (callInfo.state == PJSIP_INV_STATE_CONFIRMED || 
				(callInfo.state == PJSIP_INV_STATE_DISCONNECTED && callInfo.last_status == PJSIP_SC_OK))
			{
				//Se envian los Notify cuando se establece una sesion nueva o cuando se desconecta de una sesion con BYE				
				SubscriptionServer::SendWG67NotifyFromAcc(call_id, PJ_TRUE, 
					AccountUserData::NO_ENVIAR_WG67_NOTIFY_SI_TODAS_LAS_SESIONES_SON_FICTICIAS);
			}
		}		
	}

	if (callInfo.state == PJSIP_INV_STATE_CONFIRMED)
	{
		pjmedia_stream* stream = NULL;
		stream = pjmedia_session_get_stream(sess, 0);
		if (stream != NULL)
		{
			pjmedia_stream_set_ED137version(stream, call->ED137Version);

			if (call->_Info.Type == CORESIP_CALL_RD)
			{
				pj_uint32_t audio_flags = 0;
				if (call->_Info.CallFlags & CORESIP_CALL_RD_RXONLY ||
					call->_Info.CallFlags & CORESIP_CALL_RD_RADIO_RXONLY)
				{
					audio_flags |= STREAM_RXONLY;
				}
				if (call->_Info.CallFlags & CORESIP_CALL_RD_TXONLY)
				{
					audio_flags |= STREAM_TXONLY;
				}
				if (call->_Info.CallFlags & CORESIP_CALL_RD_IDLE)
				{
					audio_flags |= STREAM_IDLE;
				}
				if (call->radio_session_with_pttid_enabled)
				{
					audio_flags |= STREAM_PTTID_ENABLED;
				}

				pjmedia_stream_set_audio_flags(stream, audio_flags);

				pjmedia_stream_set_radio_ua(stream, radio_grs);

				pjmedia_stream_set_ptt_id(stream, call->assigned_pttid);
			}
		}
	}

	stateInfo.LastCode = callInfo.last_status;
	
	if (stateInfo.LastCode == PJSIP_SC_DECLINE && 
		e->type == PJSIP_EVENT_TSX_STATE && e->body.tsx_state.type == PJSIP_EVENT_RX_MSG)
	{
		// Comprobar si el DECLINE viene como consecuencia de que la pasarela no es el activa
		pjsip_subject_hdr * subject = (pjsip_subject_hdr*)pjsip_msg_find_hdr_by_name(e->body.tsx_state.src.rdata->msg_info.msg, &_gSubjectHdr, NULL);
		if (subject)
		{
			pj_ansi_snprintf(call->LastReason, sizeof(call->LastReason) - 1, "%.*s", 
				subject->hvalue.slen, subject->hvalue.ptr);
			strcpy(stateInfo.LastReason, call->LastReason);
		}
	}
	else if (callInfo.state == PJSIP_INV_STATE_DISCONNECTED)
	{
		if (strlen(call->LastReason) == 0)
		{
			//Si finalizada la sesion call->LastReason ya tiene contenido entonces es porque ya ha llegado en el
			//CANCEL, que es tratado en OnTsxStateChanged()
			//Aquí nos aseguramos que la sesion a terminado por un BYE
			if (
				(callInfo.media_status != PJSUA_CALL_MEDIA_NONE && callInfo.media_status != PJSUA_CALL_MEDIA_ERROR) &&
				(callInfo.connect_duration.msec != 0 || callInfo.connect_duration.sec != 0)
				)
			{		
				if (e->type == PJSIP_EVENT_TSX_STATE && e->body.tsx_state.type == PJSIP_EVENT_RX_MSG)
				{
					pjsip_rx_data *rdata;
					rdata = e->body.tsx_state.src.rdata;
					if (rdata != NULL)
					{
						if (rdata->msg_info.msg != NULL)
						{
							pjsip_subject_hdr * subject = (pjsip_subject_hdr*)pjsip_msg_find_hdr_by_name(e->body.rx_msg.rdata->msg_info.msg, &_gSubjectHdr, NULL);
							if (subject != NULL)
							{					
								if (call)
								{
									pj_ansi_snprintf(call->LastReason, sizeof(call->LastReason) - 1, "%.*s", 
										subject->hvalue.slen, subject->hvalue.ptr);
								}						
							}
						}
					}
				}
			}
		}

		strcpy(stateInfo.LastReason, call->LastReason);
	}
	
	stateInfo.LocalFocus = callInfo.local_focus;
	stateInfo.RemoteFocus = callInfo.remote_focus;
	stateInfo.MediaStatus = (CORESIP_MediaStatus)callInfo.media_status;
	stateInfo.MediaDir = (CORESIP_MediaDir)callInfo.media_dir;
	stateInfo.isRadReinvite = 0;
	stateInfo.radReinvite_accepted = 0;
	stateInfo.radRreinviteCallFlags = call->_Info.CallFlags;
	stateInfo.remote_grs_supports_ED137C_Selcal = call->remote_grs_supports_ED137C_Selcal;

	if (TRACE_ALL_CALLS) PJ_LOG(3, (__FILE__, "CallStateCb %d", call_id | CORESIP_CALL_ID));
	SipAgent::Cb.CallStateCb(call_id | CORESIP_CALL_ID, &call->_Info, &stateInfo);

	if (SipAgent::UseDefaultSoundDevices != 0 && SipAgent::DefaultSndDeviceID >= 0)
	{
		if (callInfo.state == PJSIP_INV_STATE_CONFIRMED)
		{
			SipAgent::BridgeLink(CORESIP_SNDDEV_ID, SipAgent::DefaultSndDeviceID, CORESIP_CALL_ID, call_id, true);
			SipAgent::BridgeLink(CORESIP_CALL_ID, call_id, CORESIP_SNDDEV_ID, SipAgent::DefaultSndDeviceID, true);
		}
		else if (callInfo.state == PJSIP_INV_STATE_DISCONNECTED)
		{
			SipAgent::BridgeLink(CORESIP_SNDDEV_ID, SipAgent::DefaultSndDeviceID, CORESIP_CALL_ID, call_id, false);
			SipAgent::BridgeLink(CORESIP_CALL_ID, call_id, CORESIP_SNDDEV_ID, SipAgent::DefaultSndDeviceID, false);
		}
	}

	if (callInfo.state == PJSIP_INV_STATE_CONFIRMED)
	{
		//Ajustamos el nivel de audio que se transmite por RTP. Si son llamadas IA o DIA
		if (call->_Info.Type == CORESIP_CALL_IA)
		{
			pjsua_conf_adjust_tx_level(callInfo.conf_slot, SipAgent::IA_TxAttenuation);
		}
		else if (call->_Info.Type == CORESIP_CALL_DIA)
		{
			pjsua_conf_adjust_tx_level(callInfo.conf_slot, SipAgent::DIA_TxAttenuation);
		}

		if (call->_Info.Type == CORESIP_CALL_MONITORING || call->_Info.Type == CORESIP_CALL_GG_MONITORING || call->_Info.Type == CORESIP_CALL_AG_MONITORING)
		{
			pjsua_call_set_monitoring(call_id, PJ_TRUE);
		}
		else
		{
			pjsua_call_set_monitoring(call_id, PJ_FALSE);
		}
		
		if (callInfo.remote_focus && !call->_ConfInfoClientEvSub)
		{			
			if (SipAgent::_ConfManager->GetSubsObj(callInfo.acc_id, call_id, NULL) == NULL)
			{
				//Se subscribe a la conferencia siempre y cuando el focus no sea un multidestino
				//Porque en ese caso ya estaría subscrito fuera de este dialogo

				call->SubscribeToConfInfo();				
			}
		}
		else if (!callInfo.remote_focus && call->_ConfInfoClientEvSub)
		{
			pjsip_tx_data * tdata;
			if (PJ_SUCCESS == pjsip_conf_initiate(call->_ConfInfoClientEvSub, 0, &tdata))
			{
				pjsip_conf_send_request(call->_ConfInfoClientEvSub, tdata);
			}
		}

		if (call->_Info.Type != CORESIP_CALL_RD)
		{
			pj_str_t* callIdHdrVal = NULL;
			pjsua_call* pjcall;
			pjsip_dialog* dlg;
			pj_status_t st = acquire_call("OnStateChanged()", call_id, &pjcall, &dlg);
			if (st == PJ_SUCCESS)
			{
				callIdHdrVal = &dlg->call_id->id;
				SipAgent::RecCallConnected(&callInfo.remote_info, callIdHdrVal);
				pjsip_dlg_dec_lock(dlg);
			}
			else
			{
				SipAgent::RecCallConnected(&callInfo.remote_info, callIdHdrVal);
			}
		}	
	}
	else if (callInfo.state == PJSIP_INV_STATE_DISCONNECTED)
	{	
		if (call->_Info.Type != CORESIP_CALL_RD)
		{				
			pj_str_t* callIdHdrVal = NULL;
			pjsua_call* pjcall;
			pjsip_dialog* dlg;
			pj_status_t st = acquire_call("OnStateChanged()", call_id, &pjcall, &dlg);
			if (st == PJ_SUCCESS)
			{
				callIdHdrVal = &dlg->call_id->id;
				int cause;
				switch (pjcall->inv->cause)
				{
				case PJSIP_SC_BUSY_HERE:
					cause = Q850_USER_BUSY;
					break;
				case PJSIP_SC_OK:
				case PJSIP_SC_ACCEPTED:
					cause = Q850_NORMAL_CALL_CLEARING;
					break;
				default:
					cause = Q850_CALL_REJECTED;
				}
				SipAgent::RecCallEnd(cause, pjcall->media_st, CALLEND_DEST, callIdHdrVal);
				pjsip_dlg_dec_lock(dlg);
			}
			else
			{
				SipAgent::RecCallEnd(Q850_NORMAL_CALL_CLEARING, PJSUA_CALL_MEDIA_ACTIVE, CALLEND_DEST, callIdHdrVal);
			}
		}

		pj_str_t* remote_uri = NULL;
		if (call->_Dlg) remote_uri = &call->_Dlg->remote.info_str;
		SipAgent::RecBYE(remote_uri, call->_Info.Type);

		if (call->assigned_pttid != 0)
		{
			AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(callInfo.acc_id);
			if (accUserData != NULL)
			{
				accUserData->FreePTTID(call->assigned_pttid);
			}
		}	

		if (pjsip_endpt_Is_ETM(pjsua_var.endpt))
		{
			//En el caso del ETM. Por orden de Enaire, cortamos el envio de RTP al hacer el hangup

			pjmedia_stream* stream = NULL;
			sess = pjsua_call_get_media_session(call_id);
			if (sess != NULL)
			{
				stream = pjmedia_session_get_stream(sess, 0);
				if (stream != NULL)
				{
					pjmedia_stream_force_no_send_rtp(stream, PJ_TRUE);
				}
			}
		}

		call->IniciaFinSesion();
	}
}

//Esta callback se llama cuando se recibe una respuesta que solicita redireccion, como por ejemplo 302
pjsip_redirect_op SipCall::On_call_redirected(pjsua_call_id call_id, const pjsip_uri* target, const pjsip_event* e)
{
	if (e->body.rx_msg.rdata == NULL) return PJSIP_REDIRECT_STOP;	
	if (e->body.rx_msg.rdata->msg_info.msg == NULL) return PJSIP_REDIRECT_STOP;
	
	switch (e->body.rx_msg.rdata->msg_info.msg->line.status.code)
	{
	case PJSIP_SC_MOVED_TEMPORARILY:
		if (SipAgent::Cb.MovedTemporallyCb)
		{
			char buf[CORESIP_MAX_URI_LENGTH+1];
			pjsip_sip_uri* dst = (pjsip_sip_uri*)pjsip_uri_get_uri(target);
			int len = pjsip_uri_print(PJSIP_URI_IN_FROMTO_HDR, dst, buf, sizeof(buf)-1);
			if (len > 0)
			{
				buf[len] = '\0';
				if (TRACE_ALL_CALLS) PJ_LOG(3, (__FILE__, "MovedTemporallyCb"));
				SipAgent::Cb.MovedTemporallyCb(call_id | CORESIP_CALL_ID, buf);
				//calll = call_id;
				return PJSIP_REDIRECT_PENDING;		//En este caso dejamos que el usuario ejecuta la redireccion con la funcion #pjsua_call_process_redirect
			}
			else
			{				
				PJ_LOG(3,(__FILE__, "ERROR: SipCall::On_call_redirected La uri del destino al que se redirige es demasiado grande"));
				return PJSIP_REDIRECT_STOP;
			}
		}
		else
		{
			PJ_LOG(3,(__FILE__, "ERROR: SipCall::On_call_redirected CfwrMovedTemporallyCb no definida"));
			return PJSIP_REDIRECT_STOP;
		}
	default:
		return PJSIP_REDIRECT_STOP;			//El resto de estado no estan implementados
	}
}

void SipCall::EliminarRadSessionDelGrupo(SipCall *call)
{
	if (call == NULL) return;	

	call->Wait_init_timer.id = 0;
	pjsua_cancel_timer(&call->Wait_init_timer);

	call->Check_CLD_timer.id = Check_CLD_timer_IDLE;
	pjsua_cancel_timer(&call->Check_CLD_timer);

	if (call->window_timer.id == 1)
	{
		if (call->_Index_group >= 0 && call->_Index_group < FrecDesp::MAX_GROUPS)
		{
			SipAgent::_FrecDesp->SetInWindow(call->_Index_group, NULL);
		}
	}
	call->window_timer.id = 0;
	pjsua_cancel_timer(&call->window_timer);

	if (call->out_circbuff_thread != NULL)
	{
		call->out_circbuff_thread_run = PJ_FALSE;
		pj_sem_post(call->sem_out_circbuff);
		pj_thread_join(call->out_circbuff_thread);
		pj_thread_destroy(call->out_circbuff_thread);
		call->out_circbuff_thread = NULL;
	}

	if (call->_Index_group < 0 || call->_Index_sess < 0 || 
		call->_Index_group >= FrecDesp::MAX_GROUPS || call->_Index_sess >= FrecDesp::MAX_SESSIONS)
	{
		return;
	}	
					
	pj_sockaddr_in *RdSndTo;
	SipAgent::_FrecDesp->Get_group_multicast_socket(call->_Index_group, &RdSndTo);
	if (RdSndTo != NULL && RdSndTo == &call->_RdSendTo)
	{
		//El grupo est� enviando por multicast a la misma sockaddr que esta sesion que ahora se elimina
		SipAgent::_FrecDesp->Set_group_multicast_socket(call->_Index_group, NULL);
	}	

	int n_sessions_in_his_group = SipAgent::_FrecDesp->RemFromGroup(call->_Index_group, call->_Index_sess);
	if (n_sessions_in_his_group < 0)
	{
		//PJ_LOG(3,(__FILE__, "ERROR: Freq %s, call id %d cannot be removed from any CLIMAX group\n", call->_RdFr, call->_Id));
	}	
	else if (n_sessions_in_his_group > 0)
	{
		if (call->_Sending_Multicast_enabled)
		{   //Esta sesion que se va a eliminar es la activa en el grupo bss y por tanto se va a cortar el audio
			//Forzamos un estado de ningun squelch activado en el grupo para forzar el arranque de una nueva ventana de decision
			SipAgent::_FrecDesp->EnableMulticast(call, PJ_FALSE, PJ_TRUE);
			SipAgent::_FrecDesp->SetSelected(call, PJ_FALSE, PJ_TRUE);
			SipAgent::_FrecDesp->ClrSelectedUri(call);
			SipAgent::_FrecDesp->ClrAllSquSt(call->_Index_group);
		}
	}

	//Se reinician los indices del grupo
	call->_Index_group = FrecDesp::INVALID_GROUP_INDEX;
	call->_Index_sess = FrecDesp::INVALID_SESS_INDEX;
}

/*Timer de espera despues de iniciarse la sesi�n*/
void SipCall::Wait_init_timer_cb(pj_timer_heap_t *th, pj_timer_entry *te)
{
	SipCall *wp = (SipCall *)te->user_data;

	if (wp->Wait_init_timer.id == 0)
	{
		pjsua_cancel_timer(&wp->Wait_init_timer);
		return;
	}

	wp->Wait_init_timer.id = 0;
	pjsua_cancel_timer(&wp->Wait_init_timer);
	if (!wp->valid_sess) return;
	
	pjmedia_session *sess = pjsua_call_get_media_session(wp->_Id);
	if (sess != NULL)
		pjmedia_session_reset_ext_header(sess);
}

pjsua_call_id SipCall::CouplingSessionInAccount(pjsua_call_id call_id)
{
	unsigned calls_count = PJSUA_MAX_CALLS;
	pjsua_call_id call_ids[PJSUA_MAX_CALLS];
	pjsua_call_id ret = PJSUA_INVALID_ID;

	pjsua_call* pjcall1;
	pjsip_dialog* dlg1;

	pj_status_t st = acquire_call("pjsua_call_set_monitoring()", call_id, &pjcall1, &dlg1);
	if (st != PJ_SUCCESS)
	{
		return ret;
	}

	pjsua_acc_id acc_id = pjcall1->acc_id;	

	pj_bool_t other_coupling_session_exists = PJ_FALSE;
	st = pjsua_enum_calls(call_ids, &calls_count);
	if (st == PJ_SUCCESS && calls_count > 0)
	{
		for (unsigned int i = 0; i < calls_count && ret == PJSUA_INVALID_ID; i++)
		{		
			pjsua_call* pjcall;
			pjsip_dialog* dlg;

			st = acquire_call("pjsua_call_set_monitoring()", call_ids[i], &pjcall, &dlg);
			if (st != PJ_SUCCESS)
			{
				continue;
			}

			if (pjcall->acc_id == acc_id)
			{
				SipCall* sipcall = (SipCall*)pjsua_call_get_user_data(call_ids[i]);
				if (sipcall != NULL)
				{
					if (sipcall->_Info.Type == CORESIP_CALL_RD)
					{
						if (sipcall->_Info.CallFlags & CORESIP_CALL_RD_COUPLING)
						{
							ret = call_ids[i];
						}
					}
				}
			}

			pjsip_dlg_dec_lock(dlg);
		}
	}

	pjsip_dlg_dec_lock(dlg1);

	return ret;
}

/**
* Trata la Notificacion de Llamada Entrante.
* http://www.pjsip.org/docs/latest-1/pjsip/docs/html/structpjsua__callback.htm#a402dc4b89c409507fa69b54494efef10
*/
void SipCall::OnIncommingCall(pjsua_acc_id acc_id, pjsua_call_id call_id, 
							  pjsua_call_id replace_call_id, pjsip_rx_data * rdata)
{
	PJ_UNUSED_ARG(acc_id);
	pj_assert(pjsua_call_get_user_data(call_id) == NULL);

	const pjmedia_sdp_session* neg_local_sdp = NULL;
	const pjmedia_sdp_session* initial_sdp = NULL;

	pjsua_call *call = &pjsua_var.calls[call_id];
	pj_status_t st = pjmedia_sdp_neg_get_neg_local(call->inv->neg, &neg_local_sdp);
	if (st != PJ_SUCCESS)
	{
		neg_local_sdp = NULL;
	}	
	st = pjmedia_sdp_neg_get_initial_sdp(call->inv->neg, &initial_sdp);
	if (st != PJ_SUCCESS)
	{
		initial_sdp = NULL;
	}

	pj_bool_t rdAccount = PJ_FALSE;				//Indica si add_id es un account tipo radio GRS
	CORESIP_CallFlagsMask TipoGrsFlags = CORESIP_CALL_NINGUNO;
	char* RdFr_grs = NULL;

	AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(acc_id);
	if (accUserData != NULL)
	{
		rdAccount = accUserData->rdAccount;
		TipoGrsFlags = accUserData->TipoGrsFlags;
		RdFr_grs = accUserData->RdFr;
	}

	pj_bool_t radio_grs = (SipAgent::_Radio_UA != 0 || rdAccount == PJ_TRUE);

	char radio_version = 0;
	char phone_version = 0;
	char WG67_version_value_buf[32];

	if (pjsip_endpt_Neg_ED137Version_from_msg(pjsua_var.endpt, NULL, rdata->msg_info.msg, &radio_version, &phone_version,
		WG67_version_value_buf, sizeof(WG67_version_value_buf)) == PJ_FALSE)
	{
		//Versiones incompatibles. 
		pjsua_msg_data msg_data;
		pjsua_msg_data_init(&msg_data);
		pjsip_generic_string_hdr reason_hdr;
		pj_str_t reason = pj_str("Reason");
		pjsip_generic_string_hdr_init2(&reason_hdr, &reason, &gWG67Reason1005);
		pj_list_push_back(&msg_data.hdr_list, &reason_hdr);
		pjsua_call_answer(call_id, PJSIP_SC_NOT_IMPLEMENTED, NULL, &msg_data);
		return;
	}

	pjsua_call* pjcall;
	pjsip_dialog* dlg;
	st = acquire_call("OnIncommingCall()", call_id, &pjcall, &dlg);
	if (st != PJ_SUCCESS)
	{
		PJ_LOG(3, (__FILE__, "ERROR: OnIncommingCall. acquire_call"));
		return;
	}

	if (pj_ansi_strlen(WG67_version_value_buf) > 0)
	{
		pjsip_dlg_set_WG67_version(dlg, WG67_version_value_buf, PJ_TRUE);
	}
	pjsip_dlg_dec_lock(dlg);

	if (SipAgent::Cb.CallIncomingCb)
	{
		CORESIP_CallInfo info = { acc_id | CORESIP_ACC_ID, CORESIP_CALL_UNKNOWN, CORESIP_PR_UNKNOWN };
		CORESIP_CallInInfo inInfo = { };
		memset(&inInfo, 0, sizeof(inInfo));
		inInfo.etm_grs_bss_method_selected[0] = '\0';

		info.DestinationPort = 0;
		info.CallFlags = CORESIP_CALL_NINGUNO;

		pjsip_subject_hdr* subject = (pjsip_subject_hdr*)pjsip_msg_find_hdr_by_name(rdata->msg_info.msg, &gSubjectHdr, NULL);
		pjsip_priority_hdr* priority = (pjsip_priority_hdr*)pjsip_msg_find_hdr_by_name(rdata->msg_info.msg, &gPriorityHdr, NULL);

		if (radio_grs)
		{
			info.Type = CORESIP_CALL_RD;
			if (subject)
			{
				if (pj_stricmp2(&subject->hvalue, "radio") != 0)
				{
					//Como GRS es obligado que el subject sea radio
					pjsua_call_answer(call_id, PJSIP_SC_FORBIDDEN, NULL, NULL);
					return;
				}
			}
			else
			{
				//Como GRS es obligado que el subject sea radio
				pjsua_call_answer(call_id, PJSIP_SC_FORBIDDEN, NULL, NULL);
				return;
			}

			if (!priority)
			{
				//Si como GRS priority no esta entonces se considera normal
				info.Priority = CORESIP_PR_NORMAL;
			}
			else if ((pj_stricmp2(&priority->hvalue, "emergency") == 0))
			{
				info.Priority = CORESIP_PR_EMERGENCY;
			}
			else if ((pj_stricmp2(&priority->hvalue, "normal") == 0))
			{
				info.Priority = CORESIP_PR_NORMAL;
			}
			else
			{
				pjsua_call_answer(call_id, PJSIP_SC_FORBIDDEN, NULL, NULL);
				return;
			}
		}
		else
		{
			if (!subject && !priority)
			{
				//Si no se reciben cabeceras Subject y Priority. Se simula una llamada normal.
				//Así serán admitidas llamadas desde telefonos IP comerciales.
				info.Type = CORESIP_CALL_DIA;
				info.Priority = CORESIP_PR_NORMAL;
			}
			else
			{
				if (subject)
				{
					for (info.Type = CORESIP_CALL_IA;
						info.Type < CORESIP_CALL_UNKNOWN && pj_stricmp(&subject->hvalue, &gSubject[info.Type]);
						info.Type = (CORESIP_CallType)((int)info.Type + 1));
				}
				if (priority)
				{
					for (info.Priority = CORESIP_PR_EMERGENCY;
						info.Priority < CORESIP_PR_UNKNOWN && pj_stricmp(&priority->hvalue, &gPriority[info.Priority]);
						info.Priority = (CORESIP_Priority)((int)info.Priority + 1));
				}
			}
		}

		pjsip_uri* dst_uri;	//Uri del destino de la llamada entrante

		//Primero probamos que la uri del To corresponde con la de algun account
		//Si no, entonces probamos con la del request line
		if (pjsua_acc_find_for_incoming_by_uri(rdata->msg_info.to->uri) != PJSUA_INVALID_ID)
		{
			dst_uri = rdata->msg_info.to->uri;
		}
		else if (pjsua_acc_find_for_incoming_by_uri(rdata->msg_info.msg->line.req.uri) != PJSUA_INVALID_ID)
		{
			dst_uri = rdata->msg_info.msg->line.req.uri;
		}
		else
		{
			pjsua_call_answer(call_id, PJSIP_SC_NOT_FOUND, NULL, NULL);
			return;
		}

		if ((info.Type == CORESIP_CALL_UNKNOWN) || (info.Priority == CORESIP_PR_UNKNOWN) ||
			((info.Type == CORESIP_CALL_IA) && (info.Priority != CORESIP_PR_URGENT)) ||
			(!PJSIP_URI_SCHEME_IS_SIP(dst_uri) && !PJSIP_URI_SCHEME_IS_SIPS(dst_uri)) ||
			(!PJSIP_URI_SCHEME_IS_SIP(rdata->msg_info.from->uri) && !PJSIP_URI_SCHEME_IS_SIPS(rdata->msg_info.from->uri)))
		{
			pjsua_call_answer(call_id, PJSIP_SC_BAD_REQUEST, NULL, NULL);
			return;
		}

		pjmedia_sdp_session* rem_sdp;
		pj_status_t status = pjmedia_sdp_parse(rdata->tp_info.pool, (char*)rdata->msg_info.msg->body->data, rdata->msg_info.msg->body->len, &rem_sdp);
		if (status == PJ_SUCCESS && rem_sdp != NULL)
		{
			/* Validate */
			status = pjmedia_sdp_validate(rem_sdp);
		}

		if (status != PJ_SUCCESS)
		{
			rem_sdp = NULL;
		}
		else
		{
			if (rem_sdp->media_count > 0)
			{
				info.DestinationPort = rem_sdp->media[0]->desc.port;
			}
		}

		if (radio_grs)
		{
			const pjmedia_sdp_session* neg_remote_sdp;
			st = pjmedia_sdp_neg_get_neg_remote(call->inv->neg, &neg_remote_sdp);
			if (st == PJ_SUCCESS)
			{
				//Como GRS el media direction debe ser siempre sendrecv
				//Modificamos el remote sdp para simular que es sendrecv. Es la unica forma que he encontrado de forzar 
				// que un GRS siempre ponga sendrecv
				//Borramos todos los atributos
				pjmedia_sdp_attr* a = NULL;
				for (unsigned int i = 0; i < neg_remote_sdp->media_count; i++)
				{
					pjmedia_sdp_media_remove_all_attr(neg_remote_sdp->media[i], "inactive");
					pjmedia_sdp_media_remove_all_attr(neg_remote_sdp->media[i], "sendrecv");
					pjmedia_sdp_media_remove_all_attr(neg_remote_sdp->media[i], "sendonly");
					pjmedia_sdp_media_remove_all_attr(neg_remote_sdp->media[i], "recvonly");
				}

				a = pjmedia_sdp_attr_create(call->inv->pool, "sendrecv", NULL);
				pjmedia_sdp_media_add_attr(neg_remote_sdp->media[0], a);
			}

			pj_bool_t bad_sdp = PJ_FALSE;
			if (rem_sdp == NULL)
			{
				bad_sdp = PJ_TRUE;
			}
			else if (rem_sdp->media_count == 0)
			{
				bad_sdp = PJ_TRUE;
			}

			if (bad_sdp == PJ_TRUE)
			{
				pjsua_msg_data msg_data;
				pjsua_msg_data_init(&msg_data);
				pjsip_generic_string_hdr reason_hdr;
				pj_str_t reason = pj_str("Reason");
				pjsip_generic_string_hdr_init2(&reason_hdr, &reason, &pj_str("SIP ;cause=415 ;text=\"Unsupported Media Type\""));
				pj_list_push_back(&msg_data.hdr_list, &reason_hdr);
				pjsua_call_answer(call_id, PJSIP_SC_DECLINE, NULL, &msg_data);
				return;
			}
		}
		else
		{
			if (info.Type == CORESIP_CALL_RD)
			{
				//Recibimos una llamada del tipo radio y no somos un agente del tipo GRS
				pjsua_msg_data msg_data;
				pjsua_msg_data_init(&msg_data);
				pjsip_generic_string_hdr reason_hdr;
				pj_str_t reason = pj_str("Reason");
				pjsip_generic_string_hdr_init2(&reason_hdr, &reason, getWG67ReasonContent2(2006));
				pj_list_push_back(&msg_data.hdr_list, &reason_hdr);
				pjsua_call_answer(call_id, PJSIP_SC_BAD_REQUEST, NULL, &msg_data);
				return;
			}
			else
			{
				pjmedia_sdp_attr* a;
				//Para las llamadas que no son de radio quitamos el codec 123 R2S
				if (initial_sdp != NULL)
				{
					for (unsigned int i = 0; i < initial_sdp->media_count; i++)
					{
						pj_str_t r2s = pj_str("123 R2S/8000");
						do {
							if ((a = pjmedia_sdp_media_find_attr2(initial_sdp->media[i], "rtpmap", &r2s)) != NULL)
							{
								pjmedia_sdp_media_remove_attr(initial_sdp->media[i], a);
								initial_sdp->media[i]->desc.fmt_count--;
							}
						} while (a != NULL);
					}
				}
				if (neg_local_sdp != NULL)
				{
					for (unsigned int i = 0; i < neg_local_sdp->media_count; i++)
					{
						pj_str_t r2s = pj_str("123 R2S/8000");
						do {
							if ((a = pjmedia_sdp_media_find_attr2(neg_local_sdp->media[i], "rtpmap", &r2s)) != NULL)
							{
								pjmedia_sdp_media_remove_attr(neg_local_sdp->media[i], a);
								neg_local_sdp->media[i]->desc.fmt_count--;
							}
						} while (a != NULL);
					}
				}
			}
		}

		pj_bool_t ptt_id_enabled = PJ_FALSE;
		if (info.Type == CORESIP_CALL_RD && rem_sdp != NULL)
		{
			info.CallFlags = CORESIP_CALL_NINGUNO;
			int cause = SDPUtils::RadioRemoteSdpAnalysisAsUAS(call_id, radio_grs, rem_sdp, &TipoGrsFlags, &info.Type, &info.CallFlags, &ptt_id_enabled);
			if (cause != 0)
			{
				pjsua_msg_data msg_data;
				pjsua_msg_data_init(&msg_data);
				pjsip_generic_string_hdr reason_hdr;
				pj_str_t reason = pj_str("Reason");
				pjsip_generic_string_hdr_init2(&reason_hdr, &reason, getWG67ReasonContent2(cause));
				pj_list_push_back(&msg_data.hdr_list, &reason_hdr);

				pjsua_call_answer(call_id, PJSIP_SC_DECLINE, NULL, &msg_data);
				return;
			}
		}

		SDPUtils::NegotiateNoFreqDisconn(call_id, info.Type, radio_grs, neg_local_sdp, rem_sdp, &info.NoFreqDisconn);
		SDPUtils::NegotiateNoFreqDisconn(call_id, info.Type, radio_grs, initial_sdp, rem_sdp, &info.NoFreqDisconn);

		info.ForceSDPSendRecvAttr = CORESIP_SDP_SR_ATTR_NOFORCED;

		SipCall* sipcall = new SipCall(call_id, &info);
		pjsua_call_set_user_data(call_id, sipcall);

		if (sipcall->_Info.Type == CORESIP_CALL_RD)
		{
			sipcall->ED137Version = radio_version;
		}
		else
		{
			sipcall->ED137Version = phone_version;
		}

		st = acquire_call("OnIncommingCall()", call_id, &pjcall, &dlg);
		if (st != PJ_SUCCESS)
		{
			PJ_LOG(3, (__FILE__, "ERROR: OnIncommingCall. acquire_call"));
			return;
		}
		sipcall->_Dlg = dlg;

		sipcall->_Acc_id = acc_id;

		//Para extraer la uri de destino
		pjsua_acc_info info_acc_id;
		pjsua_acc_get_info(acc_id, &info_acc_id);

		pj_bool_t RecCallStart_sent = PJ_FALSE;
		//Envia mensaje CallStart al grabador
		//Para extraer la uri origen
		pjsua_call_info info_call_id;
		st = pjsua_call_get_info(call_id, &info_call_id);
		if (st == PJ_SUCCESS)
		{
			SipAgent::RecINV(&info_call_id.remote_contact, info.Type);
			if (info.Type != CORESIP_CALL_RD)
			{
				SipAgent::RecCallStart(SipAgent::INCOM, info.Priority, &info_call_id.remote_contact, &info_acc_id.acc_uri, &dlg->call_id->id);
				RecCallStart_sent = PJ_TRUE;
			}
		}

		pjsip_dlg_dec_lock(dlg);

		if (radio_grs)
		{
			//Este es un agente radio o un account del tipo radio GRS

			sipcall->radio_session_with_pttid_enabled = ptt_id_enabled;

			if (initial_sdp != NULL)
			{
				SDPUtils::UpdateLocalSdpRadioTypeAsGRS(call->inv->pool, call_id, (pjmedia_sdp_session*)initial_sdp, rem_sdp, &TipoGrsFlags);
			}
			if (neg_local_sdp != NULL)
			{
				SDPUtils::UpdateLocalSdpRadioTypeAsGRS(call->inv->pool, call_id, (pjmedia_sdp_session*)neg_local_sdp, rem_sdp, &TipoGrsFlags);
			}

			SDPUtils::SetGRSBssMethod_from_SDP(acc_id, call_id, rem_sdp);
			if (strlen(sipcall->etm_grs_bss_method_selected) > 0)
			{
				pj_str_t bssmet = pj_str(sipcall->etm_grs_bss_method_selected);
				pjmedia_sdp_attr*  a = pjmedia_sdp_attr_create(call->inv->pool, "bss", &bssmet);
				if (initial_sdp != NULL) pjmedia_sdp_media_add_attr(initial_sdp->media[0], a);
				if (neg_local_sdp != NULL) pjmedia_sdp_media_add_attr(neg_local_sdp->media[0], a);

				strcpy(inInfo.etm_grs_bss_method_selected, sipcall->etm_grs_bss_method_selected);
			}

			if (SDPUtils::CheckGRS_fid_from_SDP(acc_id, rem_sdp) == -1)
			{
				pjsua_msg_data msg_data;
				pjsua_msg_data_init(&msg_data);
				pjsip_generic_string_hdr reason_hdr;
				pj_str_t reason = pj_str("Reason");
				pjsip_generic_string_hdr_init2(&reason_hdr, &reason, getWG67ReasonContent2(2002));
				pj_list_push_back(&msg_data.hdr_list, &reason_hdr);

				pjsua_call_answer(call_id, PJSIP_SC_DECLINE, NULL, &msg_data);
				return;
			}
			else
			{
				if (RdFr_grs != NULL)  
					pj_ansi_snprintf(inInfo.RdFr, sizeof(inInfo.RdFr) - 1, "%s", RdFr_grs);
			}

			if (SDPUtils::CheckGRS_KeepAliveParams_from_SDP(acc_id, rem_sdp, &sipcall->_R2SKeepAlivePeriod, &sipcall->_R2SKeepAliveMultiplier) == -1)
			{
				pjsua_msg_data msg_data;
				pjsua_msg_data_init(&msg_data);
				pjsip_generic_string_hdr reason_hdr;
				pj_str_t reason = pj_str("Reason");
				pjsip_generic_string_hdr_init2(&reason_hdr, &reason, getWG67ReasonContent2(2007));
				pj_list_push_back(&msg_data.hdr_list, &reason_hdr);

				pjsua_call_answer(call_id, PJSIP_SC_DECLINE, NULL, &msg_data);
				return;
			}
			else
			{
				char kap[8];
				pj_ansi_snprintf(kap, sizeof(kap), "%u", sipcall->_R2SKeepAlivePeriod);
				char kam[8];
				pj_ansi_snprintf(kam, sizeof(kam), "%u", sipcall->_R2SKeepAliveMultiplier);
				pjmedia_sdp_attr* a = pjmedia_sdp_attr_create(call->inv->pool, "R2S-KeepAlivePeriod", &(pj_str(kap)));
				if (initial_sdp != NULL) pjmedia_sdp_media_add_attr(initial_sdp->media[0], a);
				if (neg_local_sdp != NULL) pjmedia_sdp_media_add_attr(neg_local_sdp->media[0], a);
				a = pjmedia_sdp_attr_create(call->inv->pool, "R2S-KeepAliveMultiplier", &(pj_str(kam)));
				if (initial_sdp != NULL) pjmedia_sdp_media_add_attr(initial_sdp->media[0], a);
				if (neg_local_sdp != NULL) pjmedia_sdp_media_add_attr(neg_local_sdp->media[0], a);
			}

			if (rem_sdp != NULL) 
			{
				if (rem_sdp->media_count > 0)
				{
					pjmedia_sdp_attr* a;

					//Se asigna el PTT-ID
					if (ptt_id_enabled)
					{
						sipcall->assigned_pttid = accUserData->AllocPTTID(sipcall->ED137Version);
						if (sipcall->assigned_pttid != 0)
						{
							char spttid[3];
							snprintf(spttid, sizeof(spttid), "%u", sipcall->assigned_pttid);
							pj_str_t pjpttid = pj_str(spttid);
							a = pjmedia_sdp_attr_create(call->inv->pool, "ptt-id", &pjpttid);
							if (initial_sdp != NULL) pjmedia_sdp_media_add_attr(initial_sdp->media[0], a);
							if (neg_local_sdp != NULL) pjmedia_sdp_media_add_attr(neg_local_sdp->media[0], a);
						}
					}					
				}
			}
		}
		else
		{
			
		}

		sipcall->IncommingReqUri[0] = '\0';
		if (rdata->msg_info.msg->line.req.uri)
		{
			int len = pjsip_uri_print(PJSIP_URI_IN_REQ_URI, rdata->msg_info.msg->line.req.uri, 
				sipcall->IncommingReqUri, sizeof(sipcall->IncommingReqUri)-1);
			if (len > 0) sipcall->IncommingReqUri[len] = '\0';
		}
		
		pjsip_sip_uri * src = (pjsip_sip_uri*)pjsip_uri_get_uri(rdata->msg_info.from->uri);
		pjsip_sip_uri * dst = (pjsip_sip_uri*)pjsip_uri_get_uri(dst_uri);
		pj_str_t name = pjsip_uri_get_display(rdata->msg_info.from->uri);
		
		pj_ansi_snprintf(inInfo.SrcId, sizeof(inInfo.SrcId) - 1, "%.*s", src->user.slen, src->user.ptr);
		pj_ansi_snprintf(inInfo.SrcIp, sizeof(inInfo.SrcIp) - 1, "%.*s", src->host.slen, src->host.ptr);
		pj_ansi_snprintf(inInfo.DstId, sizeof(inInfo.DstId) - 1, "%.*s", dst->user.slen, dst->user.ptr);
		pj_ansi_snprintf(inInfo.DstIp, sizeof(inInfo.DstIp) - 1, "%.*s", dst->host.slen, dst->host.ptr);

		//Display name that comes optionally in SIP uri between quotation marks
		if (name.slen > 0)
		   pj_ansi_snprintf(inInfo.DisplayName, sizeof(inInfo.DisplayName) - 1, "%.*s", name.slen, name.ptr);

		pjsip_param * p = pjsip_param_find(&(src->other_param), &gIsubParam);
		if (p != NULL)
		{
			pj_ansi_snprintf(inInfo.SrcSubId, sizeof(inInfo.SrcSubId) - 1, "%.*s", p->value.slen, p->value.ptr);
		}
		p = pjsip_param_find(&(src->other_param), &gRsParam);
		if (p != NULL)
		{
			pj_ansi_snprintf(inInfo.SrcRs, sizeof(inInfo.SrcRs) - 1, "%.*s", p->value.slen, p->value.ptr);
		}
		p = pjsip_param_find(&(dst->other_param), &gIsubParam);
		if (p != NULL)
		{
			pj_ansi_snprintf(inInfo.DstSubId, sizeof(inInfo.DstSubId) - 1, "%.*s", p->value.slen, p->value.ptr);
		}

		//Para encontrar el puerto de origen buscamos una cabecera Record-route que contenga el mismo host que el From 
		pjsip_rr_hdr *rr = NULL;
		int com = -1;
		int port = 0;
		pjsip_sip_uri *rr_uri = NULL;
		int counter = 256;
		do {
			if (rr != NULL) rr = rr->next;
			rr = (pjsip_rr_hdr*) pjsip_msg_find_hdr(rdata->msg_info.msg, PJSIP_H_RECORD_ROUTE, rr);
			if (rr != NULL) rr_uri = (pjsip_sip_uri*) pjsip_uri_get_uri(&rr->name_addr);
			if (rr_uri != NULL) com = pj_strcmp(&rr_uri->host, &src->host);
			counter--;
		} while (rr != NULL && com != 0 && counter > 0);

		if (com == 0)
		{
			//Se ha encontrado un Record-route con el host igual al del from
			port = rr_uri->port;
			if (port == 0)
			{
				//no contiene el puerto. Será el de por defecto
				port = 5060;
			}
		}
		else
		{
			//No ha encontrado ninguna cabecera Record-route en la que coincida el host
			//Buscamos el del contact
			pjsip_contact_hdr *contact = (pjsip_contact_hdr*)  pjsip_msg_find_hdr(rdata->msg_info.msg, PJSIP_H_CONTACT, NULL);
			if (contact != NULL)
			{
				pjsip_sip_uri *contact_uri = (pjsip_sip_uri*) pjsip_uri_get_uri(contact->uri);
				if (contact_uri != NULL)
				{
					port = contact_uri->port;
					if (port == 0) port = 5060;	//no contiene el puerto. Será el de por defecto
				}
				else
				{
					//El contact no tiene uri
					pjsua_call_answer(call_id, PJSIP_SC_BAD_REQUEST, NULL, NULL);	

					if (RecCallStart_sent)
					{	
						if (sipcall->_Info.Type != CORESIP_CALL_RD)
						{
							SipAgent::RecCallEnd(Q850_CALL_REJECTED, PJSUA_CALL_MEDIA_NONE, CALLEND_DEST, &sipcall->_Dlg->call_id->id);
						}
						SipAgent::RecBYE(&info_call_id.remote_contact, info.Type);
					}

					return;
				}
			}
			else
			{
				//NO hay contact
				pjsua_call_answer(call_id, PJSIP_SC_BAD_REQUEST, NULL, NULL);

				if (RecCallStart_sent)
				{
					if (sipcall->_Info.Type != CORESIP_CALL_RD)
					{
						SipAgent::RecCallEnd(Q850_CALL_REJECTED, PJSUA_CALL_MEDIA_NONE, CALLEND_DEST, &sipcall->_Dlg->call_id->id);
					}
					SipAgent::RecBYE(&info_call_id.remote_contact, info.Type);
				}

				return;
			}
		}

		inInfo.SrcPort = (unsigned) port;		

		int call2replace = replace_call_id != PJSUA_INVALID_ID ? replace_call_id | CORESIP_CALL_ID : PJSUA_INVALID_ID;

		if (TRACE_ALL_CALLS) PJ_LOG(3, (__FILE__, "CallIncomingCb %d", call_id | CORESIP_CALL_ID));
		SipAgent::Cb.CallIncomingCb(call_id | CORESIP_CALL_ID, call2replace, &info, &inInfo);
	}
	else
	{
		pjsua_call_answer(call_id, PJSIP_SC_NOT_IMPLEMENTED, NULL, NULL);
	}
}

/**
* Trata la conexion Desconexion de la Media a un dispositivo.
* http://www.pjsip.org/docs/latest-1/pjsip/docs/html/structpjsua__callback.htm#a952b89dc85a47af3037c19ef06b776e3
*/
void SipCall::OnMediaStateChanged(pjsua_call_id call_id, int *returned_code)
{
	int ret_code = PJSIP_SC_OK;
	pjsua_call_info callInfo;
	pj_status_t status = pjsua_call_get_info(call_id, &callInfo);
	if (status != PJ_SUCCESS)
		return;

	pjsua_call* pjcall;
	pjsip_dialog* dlg;

	status = acquire_call("pjsua_call_set_hold()", call_id, &pjcall, &dlg);
	if (status != PJ_SUCCESS)
		return;

	const pjmedia_sdp_session* remote_sdp = NULL;
	status = pjmedia_sdp_neg_get_active_remote(pjcall->inv->neg, &remote_sdp);
	if (status != PJ_SUCCESS)
	{
		remote_sdp = NULL;
	}

	SipCall* sipcall = (SipCall*)(pjsua_call_get_user_data(call_id));	

	if (callInfo.media_status == PJSUA_CALL_MEDIA_ERROR)
	{
		pjsua_call_hangup(call_id, PJSIP_SC_INTERNAL_SERVER_ERROR, &(pj_str("SDP negotiation failed")), NULL);
	}
	else if (callInfo.state == PJSIP_INV_STATE_CONFIRMED)
	{
		if (sipcall)
		{
			if (sipcall->_Info.Type == CORESIP_CALL_MONITORING || sipcall->_Info.Type == CORESIP_CALL_GG_MONITORING || sipcall->_Info.Type == CORESIP_CALL_AG_MONITORING)
			{
				pjsua_call_set_monitoring(call_id, PJ_TRUE);
			}
			else
			{
				pjsua_call_set_monitoring(call_id, PJ_FALSE);
			}
					

			if (sipcall->_Info.Type == CORESIP_CALL_RD)
			{
				pj_bool_t rdAccount = PJ_FALSE;				//Indica si add_id es un account tipo radio GRS
				CORESIP_CallFlagsMask TipoGrsFlags = CORESIP_CALL_NINGUNO;

				AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(callInfo.acc_id);
				if (accUserData != NULL)
				{
					rdAccount = accUserData->rdAccount;
					TipoGrsFlags = accUserData->TipoGrsFlags;
				}

				pj_bool_t radio_grs = (SipAgent::_Radio_UA != 0 || rdAccount == PJ_TRUE);

				if (radio_grs)
				{
					if (remote_sdp == NULL)
					{
						ret_code = PJSIP_SC_DECLINE;
					}
					else if (remote_sdp->media_count == 0)
					{
						ret_code = PJSIP_SC_DECLINE;
					}
				}

				pjmedia_session* sess = NULL;
				pjmedia_stream* stream = NULL;
				sess = pjsua_call_get_media_session(call_id);
				if (sess != NULL)
				{					
					stream = pjmedia_session_get_stream(sess, 0);
					if (stream != NULL && ret_code == PJSIP_SC_OK)
					{
						pjmedia_stream_set_radio_ua(stream, radio_grs);
						pjmedia_stream_set_rtp_ext_tx_info(stream, sipcall->last_rtp_ext_info);		//Se actualiza el estado ptt
					}
				}

				unsigned short assigned_ptt_id = 0;

				if (ret_code == PJSIP_SC_OK && remote_sdp != NULL && remote_sdp->media_count != 0)
				{
					if (dlg->role == PJSIP_ROLE_UAC)
					{
						if (SDPUtils::RadioRemoteSdpAnalysisAsUAC(remote_sdp, &sipcall->_Info, &sipcall->radio_session_with_pttid_enabled, &assigned_ptt_id) == -1)
						{
							ret_code = PJSIP_SC_DECLINE;
						}
						else
						{
							sipcall->assigned_pttid = assigned_ptt_id;

							pj_uint32_t rtp_ext_tx_info;
							pjmedia_stream_get_rtp_ext_tx_info(stream, &rtp_ext_tx_info);

							if (PJMEDIA_RTP_RD_EX_GET_PTT_TYPE(rtp_ext_tx_info) == CORESIP_PTT_OFF && sipcall->radio_session_with_pttid_enabled)
							{
								//si no tiene PTT activado, asignamos al stream en ptt-id asignado
								pjmedia_stream_set_ptt_id(stream, sipcall->assigned_pttid);
							}
						}

						//Por aqui pasa cuando se envia un reinvite. Se activa la media direction dependiendo del atributo send-recv forzado
						switch (sipcall->_Info.ForceSDPSendRecvAttr)
						{
						case CORESIP_SDP_SR_ATTR_NOFORCED:
							break;
						case CORESIP_SDP_SR_ATTR_INACTIVE:
							pjmedia_session_pause(sess, PJMEDIA_DIR_ENCODING_DECODING);
							break;
						case CORESIP_SDP_SR_ATTR_SENDONLY:
							pjmedia_session_resume(sess, PJMEDIA_DIR_ENCODING_DECODING);
							pjmedia_session_pause(sess, PJMEDIA_DIR_DECODING);
							break;
						case CORESIP_SDP_SR_ATTR_RECVONLY:
							pjmedia_session_resume(sess, PJMEDIA_DIR_ENCODING_DECODING);
							pjmedia_session_pause(sess, PJMEDIA_DIR_ENCODING);
							break;
						case CORESIP_SDP_SR_ATTR_NONE:
						case CORESIP_SDP_SR_ATTR_SENDRECV:
							pjmedia_session_resume(sess, PJMEDIA_DIR_ENCODING_DECODING);
							break;
						}			
					}
				}

				if (ret_code == PJSIP_SC_OK)
				{
					if (stream != NULL)
					{
						pj_uint32_t audio_flags = 0;
						if (sipcall->_Info.CallFlags & CORESIP_CALL_RD_RXONLY ||
							sipcall->_Info.CallFlags & CORESIP_CALL_RD_RADIO_RXONLY)
						{
							audio_flags |= STREAM_RXONLY;
						}
						if (sipcall->_Info.CallFlags & CORESIP_CALL_RD_TXONLY)
						{
							audio_flags |= STREAM_TXONLY;
						}
						if (sipcall->_Info.CallFlags & CORESIP_CALL_RD_IDLE)
						{
							audio_flags |= STREAM_IDLE;
						}
						if (sipcall->radio_session_with_pttid_enabled)
						{
							audio_flags |= STREAM_PTTID_ENABLED;
						}

						pjmedia_stream_set_audio_flags(stream, audio_flags);						
					}
				}
			}
		}

		if (ret_code == PJSIP_SC_OK && SipAgent::Cb.CallStateCb)
		{
			if (sipcall)
			{
				CORESIP_CallStateInfo stateInfo = { (CORESIP_CallState)callInfo.state, (CORESIP_CallRole)callInfo.role };

				//stateInfo.LastCode = callInfo.last_status;
				//pj_ansi_snprintf(stateInfo.LastReason, sizeof(stateInfo.LastReason) - 1, "%.*s", callInfo.last_status_text.slen, callInfo.last_status_text.ptr);

				stateInfo.LocalFocus = callInfo.local_focus;
				stateInfo.RemoteFocus = callInfo.remote_focus;				
				stateInfo.MediaStatus = (CORESIP_MediaStatus)callInfo.media_status;
				stateInfo.MediaDir = (CORESIP_MediaDir)callInfo.media_dir;
				stateInfo.PttId = sipcall->assigned_pttid;
				stateInfo.isRadReinvite = 1;
				stateInfo.radReinvite_accepted = 1;
				stateInfo.radRreinviteCallFlags = sipcall->_Info.CallFlags;
				stateInfo.remote_grs_supports_ED137C_Selcal = sipcall->remote_grs_supports_ED137C_Selcal;

				if (TRACE_ALL_CALLS) PJ_LOG(3, (__FILE__, "CallStateCb %d", call_id | CORESIP_CALL_ID));
				SipAgent::Cb.CallStateCb(call_id | CORESIP_CALL_ID, &sipcall->_Info, &stateInfo);

				if (callInfo.remote_focus && !sipcall->_ConfInfoClientEvSub)
				{
					if (SipAgent::_ConfManager->GetSubsObj(callInfo.acc_id, call_id, NULL) == NULL)
					{
						//Se subscribe a la conferencia siempre y cuando el focus no sea un multidestino
						//Porque en ese caso ya estaría subscrito fuera de este dialogo

						sipcall->SubscribeToConfInfo();
					}
				}
				else if (!callInfo.remote_focus && sipcall->_ConfInfoClientEvSub)
				{
					pjsip_tx_data * tdata;
					if (PJ_SUCCESS == pjsip_conf_initiate(sipcall->_ConfInfoClientEvSub, 0, &tdata))
					{
						pjsip_conf_send_request(sipcall->_ConfInfoClientEvSub, tdata);
					}
				}

				if (sipcall->_Info.Type != CORESIP_CALL_RD)
				{
					if (stateInfo.Role == CORESIP_CALL_ROLE_UAC)			//Este agente es el llamante
					{
						if (callInfo.media_status == PJSUA_CALL_MEDIA_LOCAL_HOLD)
						{
							SipAgent::RecHold(true, true, callInfo.media_status, &dlg->call_id->id);
						}
						else if (callInfo.media_status == PJSUA_CALL_MEDIA_ACTIVE)
						{
							SipAgent::RecHold(false, true, callInfo.media_status, &dlg->call_id->id);
						}
						else if (callInfo.media_status == CORESIP_MEDIA_REMOTE_HOLD ||
							callInfo.media_status == PJSUA_CALL_MEDIA_NONE) 
						{
							SipAgent::RecHold(true, false, callInfo.media_status, &dlg->call_id->id);
						}
					}
					else												//Este agente es el llamado
					{
						if (callInfo.media_status == PJSUA_CALL_MEDIA_LOCAL_HOLD)
						{
							SipAgent::RecHold(true, false, callInfo.media_status, &dlg->call_id->id);
						}
						else if (callInfo.media_status == PJSUA_CALL_MEDIA_ACTIVE)
						{
							SipAgent::RecHold(false, false, callInfo.media_status, &dlg->call_id->id);
						}
						else if (callInfo.media_status == CORESIP_MEDIA_REMOTE_HOLD ||
							callInfo.media_status == PJSUA_CALL_MEDIA_NONE) 
						{
							SipAgent::RecHold(true, true, callInfo.media_status, &dlg->call_id->id);
						}										
					}
				}
			}
		}
	}

	if (returned_code != NULL) *returned_code = ret_code;
	pjsip_dlg_dec_lock(dlg);
}

/**
* Trata la recepcion de un re-INVITE
* 
*/
void SipCall::OnReinviteReceived(pjsua_call_id call_id, const pjsip_event* e, pjsip_tx_data* tdata, int* returned_code, pj_str_t** cause)
{
	int ret_code = PJSIP_SC_OK;
	if (cause != NULL) *cause = NULL;

	pjsua_call_info callInfo;
	pj_status_t status = pjsua_call_get_info(call_id, &callInfo);
	if (status != PJ_SUCCESS)
		return;

	pjsua_call* pjcall;
	pjsip_dialog* dlg;
	pjsip_rx_data* rdata = e->body.tsx_state.src.rdata;
	if (rdata == NULL)
	{
		if (returned_code != NULL) *returned_code = PJSIP_SC_DECLINE;
		if (cause != NULL) *cause = NULL;
		return;
	}

	status = acquire_call("SipCall::OnReinviteReceived()", call_id, &pjcall, &dlg);
	if (status != PJ_SUCCESS)
		return;
	
	pjsip_msg* msg = rdata->msg_info.msg;
	if (msg == NULL)
	{
		if (returned_code != NULL) *returned_code = PJSIP_SC_DECLINE;
		if (cause != NULL) *cause = NULL;
		pjsip_dlg_dec_lock(dlg);
		return;
	}

	pjsip_msg_body* body = msg->body;
	if (body == NULL)
	{		
		pjsip_dlg_dec_lock(dlg);
		return;
	}

	pj_pool_t* tmppool = pjsua_pool_create("OnReinviteReceived", 128, 32);
	if (tmppool == NULL)
	{
		PJ_LOG(3, (__FILE__, "ERROR: SipCall::OnReinviteReceived: tmppool cannot be created"));
		pjsip_dlg_dec_lock(dlg);
		return;
	}

	pjmedia_sdp_session* remote_sdp = NULL;
	status = pjmedia_sdp_parse(tmppool, (char*)body->data, body->len, &remote_sdp);
	if (status != PJ_SUCCESS)
	{
		remote_sdp = NULL;
	}

	pj_bool_t notify_session_change = PJ_FALSE;			//Indica si ha cambiado la info de sesion para enviar notify a las subscripciones. El ptt_id o el Type

	SipCall* sipcall = (SipCall*)(pjsua_call_get_user_data(call_id));

	if (sipcall && callInfo.state == PJSIP_INV_STATE_CONFIRMED && dlg->role == PJSIP_ROLE_UAS)
	{
		if (sipcall->_Info.Type == CORESIP_CALL_RD)
		{
			pj_bool_t rdAccount = PJ_FALSE;				//Indica si add_id es un account tipo radio GRS
			CORESIP_CallFlagsMask TipoGrsFlags = CORESIP_CALL_NINGUNO;

			AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(callInfo.acc_id);
			if (accUserData != NULL)
			{
				rdAccount = accUserData->rdAccount;
				TipoGrsFlags = accUserData->TipoGrsFlags;
			}

			pj_bool_t radio_grs = (SipAgent::_Radio_UA != 0 || rdAccount == PJ_TRUE);

			if (radio_grs)
			{
				if (remote_sdp == NULL)
				{
					ret_code = PJSIP_SC_DECLINE;
					if (cause != NULL) *cause = &pj_str("SIP ;cause=415 ;text=\"Unsupported Media Type\"");
				}
				else if (remote_sdp->media_count == 0)
				{
					ret_code = PJSIP_SC_DECLINE;
					if (cause != NULL) *cause = &pj_str("SIP ;cause=415 ;text=\"Unsupported Media Type\"");
				}
			}			

			if (ret_code == PJSIP_SC_OK && remote_sdp != NULL && remote_sdp->media_count != 0)
			{
				const pjmedia_sdp_session* neg_local_sdp = NULL;				

				if (SDPUtils::CheckGRS_fid_from_SDP(callInfo.acc_id, remote_sdp) == -1)
				{
					ret_code = PJSIP_SC_DECLINE;
					if (cause != NULL) *cause = getWG67ReasonContent2(2002);
				}				

				CORESIP_CallType newType = sipcall->_Info.Type;
				CORESIP_CallFlagsMask newCallflags = sipcall->_Info.CallFlags;
				pj_bool_t radio_session_with_pttid_enabled_prev = sipcall->radio_session_with_pttid_enabled;
				unsigned int assigned_pttid_prev = sipcall->assigned_pttid;
				pj_bool_t ptt_id_enabled = sipcall->radio_session_with_pttid_enabled;
				if (ret_code == PJSIP_SC_OK)
				{
					int cause_error = SDPUtils::RadioRemoteSdpAnalysisAsUAS(call_id, radio_grs, remote_sdp, &TipoGrsFlags, &newType, &newCallflags, &ptt_id_enabled);
					if (cause_error != 0)
					{
						ret_code = PJSIP_SC_DECLINE;
						if (cause != NULL) *cause = getWG67ReasonContent2(cause_error);
					}
				}

				unsigned int newR2SKeepAlivePeriod = sipcall->_R2SKeepAlivePeriod;
				unsigned int newR2SKeepAliveMultiplier = sipcall->_R2SKeepAliveMultiplier;
				if (ret_code == PJSIP_SC_OK)
				{
					if (SDPUtils::CheckGRS_KeepAliveParams_from_SDP(callInfo.acc_id, remote_sdp, &newR2SKeepAlivePeriod, &newR2SKeepAliveMultiplier) == -1)
					{
						ret_code = PJSIP_SC_DECLINE;
						if (cause != NULL) *cause = getWG67ReasonContent2(2007);
					}
				}

				if (ret_code == PJSIP_SC_OK)
				{
					if (sipcall->_Info.CallFlags != newCallflags)
					{
						//Comprobamos si ha cambiado el Type de la sesion
						if (((sipcall->_Info.CallFlags & CORESIP_CALL_RD_COUPLING) != (newCallflags & CORESIP_CALL_RD_COUPLING)) ||
							((sipcall->_Info.CallFlags & CORESIP_CALL_RD_IDLE) != (newCallflags & CORESIP_CALL_RD_IDLE)) ||
							((sipcall->_Info.CallFlags & CORESIP_CALL_RD_RADIO_RXONLY) != (newCallflags & CORESIP_CALL_RD_RADIO_RXONLY)))
						{
							notify_session_change = PJ_TRUE;
						}							
					}

					sipcall->_Info.Type = newType;
					sipcall->_Info.CallFlags = newCallflags;
					sipcall->radio_session_with_pttid_enabled = ptt_id_enabled;
					sipcall->_R2SKeepAlivePeriod = newR2SKeepAlivePeriod;
					sipcall->_R2SKeepAliveMultiplier = newR2SKeepAliveMultiplier;

					pj_status_t st = pjmedia_sdp_neg_get_neg_local(pjcall->inv->neg, &neg_local_sdp);
					if (st != PJ_SUCCESS)
					{
						neg_local_sdp = NULL;
					}

					if (neg_local_sdp != NULL)
					{
						SDPUtils::NegotiateNoFreqDisconn(call_id, sipcall->_Info.Type, radio_grs,
							neg_local_sdp, remote_sdp, &sipcall->_Info.NoFreqDisconn);
					}

					if (neg_local_sdp != NULL)
					{
						SDPUtils::UpdateLocalSdpRadioTypeAsGRS(pjcall->inv->pool, call_id, (pjmedia_sdp_session*)neg_local_sdp, (pjmedia_sdp_session *)remote_sdp, &TipoGrsFlags);

						const pjmedia_sdp_session* neg_remote_sdp;
						st = pjmedia_sdp_neg_get_neg_remote(pjcall->inv->neg, &neg_remote_sdp);
						if (st == PJ_SUCCESS)
						{
							//Como GRS el media direction debe ser siempre sendrecv
							//Modificamos el remote sdp para simular que es sendrecv. Es la unica forma que he encontrado de forzar 
							// que un GRS siempre ponga sendrecv
							//Borramos todos los atributos
							pjmedia_sdp_attr* a = NULL;
							for (unsigned int i = 0; i < neg_remote_sdp->media_count; i++)
							{
								pjmedia_sdp_media_remove_all_attr(neg_remote_sdp->media[i], "inactive");
								pjmedia_sdp_media_remove_all_attr(neg_remote_sdp->media[i], "sendrecv");
								pjmedia_sdp_media_remove_all_attr(neg_remote_sdp->media[i], "sendonly");
								pjmedia_sdp_media_remove_all_attr(neg_remote_sdp->media[i], "recvonly");
							}

							a = pjmedia_sdp_attr_create(pjcall->inv->pool, "sendrecv", NULL);
							pjmedia_sdp_media_add_attr(neg_remote_sdp->media[0], a);
						}
					}

					SDPUtils::SetGRSBssMethod_from_SDP(callInfo.acc_id, call_id, remote_sdp);
					if (strlen(sipcall->etm_grs_bss_method_selected) > 0)
					{
						pj_str_t bssmet = pj_str(sipcall->etm_grs_bss_method_selected);
						pjmedia_sdp_attr* a = pjmedia_sdp_attr_create(pjcall->inv->pool, "bss", &bssmet);
						if (neg_local_sdp != NULL) pjmedia_sdp_media_add_attr(neg_local_sdp->media[0], a);
					}

					char kap[8];
					pj_ansi_snprintf(kap, sizeof(kap), "%u", sipcall->_R2SKeepAlivePeriod);
					char kam[8];
					pj_ansi_snprintf(kam, sizeof(kam), "%u", sipcall->_R2SKeepAliveMultiplier);
					pjmedia_sdp_attr* a = pjmedia_sdp_attr_create(pjcall->inv->pool, "R2S-KeepAlivePeriod", &(pj_str(kap)));
					if (neg_local_sdp != NULL) pjmedia_sdp_media_add_attr(neg_local_sdp->media[0], a);
					a = pjmedia_sdp_attr_create(pjcall->inv->pool, "R2S-KeepAliveMultiplier", &(pj_str(kam)));
					if (neg_local_sdp != NULL) pjmedia_sdp_media_add_attr(neg_local_sdp->media[0], a);
					
					if (sipcall->radio_session_with_pttid_enabled)
					{
						pjmedia_sdp_attr* a;
						if (sipcall->assigned_pttid == 0) sipcall->assigned_pttid = accUserData->AllocPTTID(sipcall->ED137Version);
						if (sipcall->assigned_pttid != 0)
						{
							char spttid[3];
							snprintf(spttid, sizeof(spttid), "%u", sipcall->assigned_pttid);
							pj_str_t pjpttid = pj_str(spttid);
							a = pjmedia_sdp_attr_create(pjcall->inv->pool, "ptt-id", &pjpttid);
							pjmedia_sdp_media_add_attr(neg_local_sdp->media[0], a);
						}
					}
					else
					{
						//En el re-invite ponemos el Ptt-id asignado a cero cuando no es requerido
						if (sipcall->assigned_pttid != 0)
						{
							if (accUserData != NULL) accUserData->FreePTTID(sipcall->assigned_pttid);
						}
						sipcall->assigned_pttid = 0;
					}	

					if (sipcall->radio_session_with_pttid_enabled != radio_session_with_pttid_enabled_prev ||
						sipcall->assigned_pttid != assigned_pttid_prev)
					{
						notify_session_change = PJ_TRUE;
					}

					pjmedia_session* sess = NULL;
					sess = pjsua_call_get_media_session(call_id);
					if (sess != NULL)
					{
						pjmedia_stream* stream = NULL;
						stream = pjmedia_session_get_stream(sess, 0);
						if (stream != NULL)
						{
							pj_uint32_t audio_flags = 0;
							if (sipcall->_Info.CallFlags & CORESIP_CALL_RD_RXONLY ||
								sipcall->_Info.CallFlags & CORESIP_CALL_RD_RADIO_RXONLY)
							{
								audio_flags |= STREAM_RXONLY;
							}
							if (sipcall->_Info.CallFlags & CORESIP_CALL_RD_TXONLY)
							{
								audio_flags |= STREAM_TXONLY;
							}
							if (sipcall->_Info.CallFlags & CORESIP_CALL_RD_IDLE)
							{
								audio_flags |= STREAM_IDLE;
							}
							if (sipcall->radio_session_with_pttid_enabled)
							{
								audio_flags |= STREAM_PTTID_ENABLED;
							}

							pjmedia_stream_set_audio_flags(stream, audio_flags);

							pj_uint32_t rtp_ext_tx_info;
							pjmedia_stream_get_rtp_ext_tx_info(stream, &rtp_ext_tx_info);

							if (PJMEDIA_RTP_RD_EX_GET_PTT_TYPE(rtp_ext_tx_info) != CORESIP_PTT_OFF)
							{
								//Si el PTT esta activado 
								//No hacemos nada en cuanto al Ptt-id. Ya queda activado cuando simulamos el eco de su estado en OnRdInfoChanged
							}
							else if (PJMEDIA_RTP_RD_EX_GET_SQU(rtp_ext_tx_info) == 0)
							{
								//si no tiene squelch activado ni el PTT, asignamos al stream en ptt-id asignado
								pjmedia_stream_set_ptt_id(stream, sipcall->assigned_pttid);
							}							
						}
					}

					if (SipAgent::Cb.CallStateCb)
					{
						CORESIP_CallStateInfo stateInfo = { (CORESIP_CallState)callInfo.state, (CORESIP_CallRole)callInfo.role };

						//stateInfo.LastCode = callInfo.last_status;
						//pj_ansi_snprintf(stateInfo.LastReason, sizeof(stateInfo.LastReason) - 1, "%.*s", callInfo.last_status_text.slen, callInfo.last_status_text.ptr);

						stateInfo.LocalFocus = callInfo.local_focus;
						stateInfo.RemoteFocus = callInfo.remote_focus;
						stateInfo.MediaStatus = (CORESIP_MediaStatus)callInfo.media_status;
						stateInfo.MediaDir = (CORESIP_MediaDir)callInfo.media_dir;
						stateInfo.PttId = sipcall->assigned_pttid;
						stateInfo.LastCode = ret_code;
						stateInfo.isRadReinvite = 1;
						stateInfo.radReinvite_accepted = 1;
						stateInfo.radRreinviteCallFlags = sipcall->_Info.CallFlags;
						stateInfo.remote_grs_supports_ED137C_Selcal = sipcall->remote_grs_supports_ED137C_Selcal;

						if (TRACE_ALL_CALLS) PJ_LOG(3, (__FILE__, "CallStateCb %d", call_id | CORESIP_CALL_ID));
						SipAgent::Cb.CallStateCb(call_id | CORESIP_CALL_ID, &sipcall->_Info, &stateInfo);
					}
				}
			}

			if (sipcall->ED137Version == 'C' && radio_grs && ret_code == PJSIP_SC_OK)
			{
				//Se envia la cabecera Receive-info:selcal en el 200OK si somos un GRS transmisor o transceptor
				pj_bool_t grs_tx_txrx = ((TipoGrsFlags & CORESIP_CALL_RD_RXONLY) == 0);
				if (grs_tx_txrx && accUserData->SelCalSupported)
				{
					//Es un GRS que no es solo receptor y soporta SELCAL										
					pj_str_t receive_info_name = pj_str("Receive-info");
					pj_str_t receive_info_val = pj_str("selcal");
					pjsip_generic_string_hdr* receive_info_hdr = (pjsip_generic_string_hdr*)pjsip_generic_string_hdr_create(
						tdata->pool, &receive_info_name, &receive_info_val);
					if (receive_info_hdr) pj_list_push_back(&tdata->msg->hdr, receive_info_hdr);
				}
			}
		}
	}	
	
	if (returned_code != NULL) *returned_code = ret_code;
	pj_pool_release(tmppool);
	pjsip_dlg_dec_lock(dlg);

	if (ret_code == PJSIP_SC_OK && notify_session_change) SubscriptionServer::SendWG67NotifyFromAcc(call_id, PJ_TRUE, 
		AccountUserData::NO_ENVIAR_WG67_NOTIFY_SI_TODAS_LAS_SESIONES_SON_FICTICIAS);
}


/**
* Trata la notificaci�n de transferencia de llamada.
* http://www.pjsip.org/docs/latest-1/pjsip/docs/html/structpjsua__callback.htm#a598000260c40a43f81e138829377bd44
*/
void SipCall::OnTransferRequest(pjsua_call_id call_id, 
								const pj_str_t * refer_to, const pj_str_t * refer_by, 
								const pj_str_t * tsxKey, pjsip_tx_data * tdata, pjsip_evsub * sub)
{
	SipCall * old_call = (SipCall*)pjsua_call_get_user_data(call_id);
	if (!old_call)
	{
		pjsua_call_transfer_answer(PJSIP_SC_INTERNAL_SERVER_ERROR, tsxKey, tdata, sub);
		return;
	}

	if (old_call->_Info.Type != CORESIP_CALL_DIA)
	{
		pjsua_call_transfer_answer(PJSIP_SC_BAD_REQUEST, tsxKey, tdata, sub);
		return;
	}

	if (SipAgent::Cb.TransferRequestCb)
	{
		CORESIP_CallTransferInfo transferInfo = { tdata, sub };

		pj_ansi_snprintf(transferInfo.TsxKey, sizeof(transferInfo.TsxKey) - 1, "%.*s", tsxKey->slen, tsxKey->ptr);
		pj_ansi_snprintf(transferInfo.ReferTo, sizeof(transferInfo.ReferTo) - 1, "%.*s", refer_to->slen, refer_to->ptr);
		if (refer_by)
		{
			pj_ansi_snprintf(transferInfo.ReferBy, sizeof(transferInfo.ReferBy) - 1, "%.*s", refer_by->slen, refer_by->ptr);
		}

		pj_str_t dup;
		pj_strdup_with_null(old_call->_Pool, &dup, refer_to);

		char * ptr = pj_strchr(&dup, '?');
		if (ptr)
		{
			// Si no quitamos los parametros puede que los parsee mal como uri
			*ptr = '>';
			ptr++;
			*ptr = 0;

			dup.slen = (pj_ssize_t) pj_ansi_strlen(dup.ptr);
		}

		pjsip_uri * uri = pjsip_parse_uri(old_call->_Pool, dup.ptr, dup.slen, 0);

		if (PJSIP_URI_SCHEME_IS_SIP(uri) || PJSIP_URI_SCHEME_IS_SIPS(uri))
		{
			pjsip_sip_uri * dst = (pjsip_sip_uri*)pjsip_uri_get_uri(uri);

			pj_ansi_snprintf(transferInfo.DstId, sizeof(transferInfo.DstId) - 1, "%.*s", dst->user.slen, dst->user.ptr);
			pj_ansi_snprintf(transferInfo.DstIp, sizeof(transferInfo.DstIp) - 1, "%.*s", dst->host.slen, dst->host.ptr);

			pjsip_param * p = pjsip_param_find(&(dst->other_param), &gIsubParam);
			if (p != NULL)
			{
				pj_ansi_snprintf(transferInfo.DstSubId, sizeof(transferInfo.DstSubId) - 1, "%.*s", p->value.slen, p->value.ptr);
			}
			p = pjsip_param_find(&(dst->other_param), &gRsParam);
			if (p != NULL)
			{
				pj_ansi_snprintf(transferInfo.DstRs, sizeof(transferInfo.DstRs) - 1, "%.*s", p->value.slen, p->value.ptr);
			}
		}
		else if (PJSIP_URI_SCHEME_IS_TEL(uri))
		{
			pjsip_tel_uri * dst = (pjsip_tel_uri*)pjsip_uri_get_uri(uri);

			pj_ansi_snprintf(transferInfo.DstId, sizeof(transferInfo.DstId) - 1, "%.*s", dst->number.slen, dst->number.ptr);
			pj_ansi_snprintf(transferInfo.DstSubId, sizeof(transferInfo.DstSubId) - 1, "%.*s", dst->isub_param.slen, dst->isub_param.ptr);
		}
		else
		{
			pjsua_call_transfer_answer(PJSIP_SC_BAD_REQUEST, tsxKey, tdata, sub);
			return;
		}

		if (TRACE_ALL_CALLS) PJ_LOG(3, (__FILE__, "TransferRequestCb %d", call_id | CORESIP_CALL_ID));
		SipAgent::Cb.TransferRequestCb(call_id | CORESIP_CALL_ID, &old_call->_Info, &transferInfo);
	}
	else
	{
		pjsua_call_transfer_answer(PJSIP_SC_NOT_IMPLEMENTED, tsxKey, tdata, sub);
	}
}

/**
* Trata los cambios de estado de una llamada previamente transferidad
* http://www.pjsip.org/docs/latest-1/pjsip/docs/html/structpjsua__callback.htm#ac418c176dd412181d07dd54bf246faed
*/
void SipCall::OnTransferStatusChanged(pjsua_call_id based_call_id, int st_code, const pj_str_t *st_text, pj_bool_t final, pj_bool_t *p_cont)
{
	if (SipAgent::Cb.TransferStatusCb)
	{
		if (TRACE_ALL_CALLS) PJ_LOG(3, (__FILE__, "TransferStatusCb %d", based_call_id | CORESIP_CALL_ID));
		SipAgent::Cb.TransferStatusCb(based_call_id | CORESIP_CALL_ID, st_code);
	}
}

/**
* Trata cambios genericos en el estado de una llamada.
* http://www.pjsip.org/docs/latest-1/pjsip/docs/html/structpjsua__callback.htm#acec485ed428d48a6ca0d28027e5cccde
*/
void SipCall::OnTsxStateChanged(pjsua_call_id call_id, pjsip_transaction *tsx, pjsip_event *e)
{
	const pjsip_method info_method = { PJSIP_OTHER_METHOD, { "INFO", 4 } };
	
	static pj_str_t _gSubjectHdr = { "Reason", 6 };

	SipCall *sipcall = (SipCall*)pjsua_call_get_user_data(call_id);

	if (sipcall && sipcall->_Dlg != NULL && tsx->role == PJSIP_ROLE_UAC && e->type == PJSIP_EVENT_TSX_STATE && e->body.tsx_state.type == PJSIP_EVENT_RX_MSG)
	{
		//Se ha recibido un mensaje SIP y se establece la cabecera WG67-version si no lo estaba
		if (e->body.rx_msg.rdata->msg_info.msg->type == PJSIP_RESPONSE_MSG && e->body.rx_msg.rdata->msg_info.msg->line.status.code / 100 == 1)
		{
			//Es una respuesta 1xx que no requiere la cabecera WG67-version
			//No hacemos nada
		}
		else if (e->body.rx_msg.rdata->msg_info.msg->type == PJSIP_RESPONSE_MSG && pjsip_dlg_get_WG67_version(sipcall->_Dlg, NULL) == PJ_FALSE)
		{		
			char radio_version = 0;
			char phone_version = 0;
			char WG67_version_value_buf[32];

			if (pjsip_endpt_Neg_ED137Version_from_msg(pjsua_var.endpt, tsx->last_tx->msg, e->body.rx_msg.rdata->msg_info.msg, &radio_version, &phone_version,
				WG67_version_value_buf, sizeof(WG67_version_value_buf)) == PJ_TRUE)
			{
				if (pj_ansi_strlen(WG67_version_value_buf) > 0)
				{
					pjsip_dlg_set_WG67_version(sipcall->_Dlg, WG67_version_value_buf, PJ_TRUE);
					pjsip_tsx_set_WG67_version(e->body.tsx_state.tsx, WG67_version_value_buf);
				}
				if (sipcall->_Info.Type == CORESIP_CALL_RD) sipcall->ED137Version = radio_version;
				else sipcall->ED137Version = phone_version;
			}
		}		

		if (tsx->state == PJSIP_TSX_STATE_COMPLETED && tsx->method.id == PJSIP_INVITE_METHOD &&
			sipcall->_Info.Type == CORESIP_CALL_RD &&
			e->body.rx_msg.rdata->msg_info.msg->type == PJSIP_RESPONSE_MSG && 
			e->body.rx_msg.rdata->msg_info.msg->line.status.code == 603)
		{
			//Es un decline a un INVITE. Se supone que re-invite a una radio que solo puede ser rechazado
			//Porque no se acepta el tipo Radio-Rxonly o los txrxmode
			//Se restauran CallFlags
			sipcall->_Info.CallFlags = sipcall->Callflag_previous_reinvite;
		}
	}

	if (sipcall && sipcall->_Dlg != NULL && tsx->role == PJSIP_ROLE_UAC && e->type == PJSIP_EVENT_TSX_STATE && e->body.tsx_state.type == PJSIP_EVENT_RX_MSG &&
		tsx->status_code == PJSIP_SC_OK && (pjsip_method_cmp(&tsx->method, pjsip_get_invite_method()) == 0))
	{
		if (e->body.rx_msg.rdata->msg_info.msg->type == PJSIP_RESPONSE_MSG && e->body.rx_msg.rdata->msg_info.msg->line.req.method.id == PJSIP_SC_OK)
		{
			//Se ha recibido el 200OK
			//Chequeamos si soporta SELCAL
			pj_str_t receive_info = pj_str("Receive-info");
			pjsip_rx_data* rdata;
			rdata = e->body.tsx_state.src.rdata;
			if (rdata != NULL && sipcall)
			{
				if (rdata->msg_info.msg != NULL)
				{
					pjsip_generic_string_hdr* receive_info_hdr = (pjsip_subject_hdr*)pjsip_msg_find_hdr_by_name(e->body.rx_msg.rdata->msg_info.msg, &receive_info, NULL);
					if (receive_info_hdr != NULL)
					{
						if (pj_stricmp(&pj_str("selcal"), &receive_info_hdr->hvalue) == 0)
						{
							sipcall->SelcalSupported = PJ_TRUE;
						}
						else
						{
							sipcall->SelcalSupported = PJ_FALSE;
						}
					}
					else
					{
						sipcall->SelcalSupported = PJ_FALSE;
					}
				}
			}
		}
	}

	if (sipcall && sipcall->_Dlg != NULL && sipcall->_Info.Type == CORESIP_CALL_RD && sipcall->_Dlg->state == PJSIP_DIALOG_STATE_ESTABLISHED &&
		tsx->role == PJSIP_ROLE_UAC && e->type == PJSIP_EVENT_TSX_STATE && tsx->state == PJSIP_TSX_STATE_COMPLETED &&
		e->body.tsx_state.type == PJSIP_EVENT_RX_MSG &&
		tsx->status_code >= 300 && (pjsip_method_cmp(&tsx->method, pjsip_get_invite_method()) == 0))
	{
		PJ_LOG(3, ("sipcall.cpp", "Re-Invite rechazado dst %s codigo %d", sipcall->DstUri, tsx->status_code));
		pj_status_t status;
		pjsua_call* pjcall;
		pjsip_dialog* dlg;
		status = acquire_call("OnTsxStateChanged", call_id, &pjcall, &dlg);
		pj_bool_t valid_reinvite_rejected = PJ_FALSE;
		CORESIP_CallFlagsMask reinvitecallflags = sipcall->_Info.CallFlags;
		if (status == PJ_SUCCESS)
		{
			const pjmedia_sdp_session* sdp = NULL;
			status = pjmedia_sdp_neg_get_initial_sdp(pjcall->inv->neg, &sdp);
			if (status == PJ_SUCCESS)
			{
				for (unsigned int i = 0; i < sdp->media_count; i++)
				{
					pjmedia_sdp_attr* a;
					if ((a = pjmedia_sdp_media_find_attr2(sdp->media[i], "type", NULL)) != NULL)
					{
						if ((!pj_strcmp(&(a->value), &(pj_str("Radio")))) || (!pj_strcmp(&(a->value), &(pj_str("Radio-TxRx")))))
						{
							sipcall->_Info.CallFlags = sipcall->CallFlags_prev_reinvite;
							valid_reinvite_rejected = PJ_TRUE;
						}
						else if (!pj_strcmp(&(a->value), &(pj_str("Radio-Idle"))))
						{
							//Se ha rechazado re-invite Radio-Idle
							sipcall->_Info.CallFlags = sipcall->CallFlags_prev_reinvite;
							valid_reinvite_rejected = PJ_TRUE;
						}
						else if (!pj_strcmp(&(a->value), &(pj_str("Radio-Rxonly"))))
						{
							//Se ha rechazado re-invite Radio-Rxonly
							sipcall->_Info.CallFlags = sipcall->CallFlags_prev_reinvite;
							valid_reinvite_rejected = PJ_TRUE;
						}
						else if (!pj_strcmp(&(a->value), &(pj_str("Coupling"))))
						{
							//Se ha rechazado re-invite coupling
							sipcall->_Info.CallFlags = sipcall->CallFlags_prev_reinvite;
							valid_reinvite_rejected = PJ_TRUE;
						}
						else
						{
						}
					}
				}
			}

			if (SipAgent::Cb.CallStateCb && valid_reinvite_rejected)
			{
				pjsua_call_info callInfo;
				status = pjsua_call_get_info(call_id, &callInfo);
				if (status == PJ_SUCCESS)
				{
					CORESIP_CallStateInfo stateInfo = { (CORESIP_CallState)callInfo.state, (CORESIP_CallRole)callInfo.role };

					//stateInfo.LastCode = callInfo.last_status;
					//pj_ansi_snprintf(stateInfo.LastReason, sizeof(stateInfo.LastReason) - 1, "%.*s", callInfo.last_status_text.slen, callInfo.last_status_text.ptr);

					pjsip_generic_string_hdr* reason_hdr = (pjsip_generic_string_hdr*)pjsip_msg_find_hdr_by_name(e->body.tsx_state.src.rdata->msg_info.msg, &pj_str("Reason"), NULL);
					if (reason_hdr != NULL)
					{
						pj_ansi_snprintf(stateInfo.LastReason, sizeof(stateInfo.LastReason) - 1, "%.*s", reason_hdr->hvalue.slen, reason_hdr->hvalue.ptr);
						stateInfo.LastReason[sizeof(stateInfo.LastReason) - 1] = '\0';
					}
					else
					{
						stateInfo.LastReason[0] = '\0';
					}

					stateInfo.LocalFocus = callInfo.local_focus;
					stateInfo.RemoteFocus = callInfo.remote_focus;
					stateInfo.MediaStatus = (CORESIP_MediaStatus)callInfo.media_status;
					stateInfo.MediaDir = (CORESIP_MediaDir)callInfo.media_dir;
					stateInfo.PttId = sipcall->assigned_pttid;
					stateInfo.LastCode = tsx->status_code;
					stateInfo.isRadReinvite = 1;
					stateInfo.radReinvite_accepted = 0;
					stateInfo.radRreinviteCallFlags = reinvitecallflags;
					stateInfo.remote_grs_supports_ED137C_Selcal = sipcall->remote_grs_supports_ED137C_Selcal;

					SipAgent::Cb.CallStateCb(call_id | CORESIP_CALL_ID, &sipcall->_Info, &stateInfo);
				}
			}

			pjsip_dlg_dec_lock(dlg);
		}
	}

	if ((tsx->role == PJSIP_ROLE_UAS) && (tsx->state == PJSIP_TSX_STATE_TRYING) &&
		 (pjsip_method_cmp(&tsx->method, pjsip_get_cancel_method()) == 0)) 		
	{
		//Se ha recibido un CANCEL

		pjsua_call_info callInfo;
		if (pjsua_call_get_info(call_id, &callInfo) == PJ_SUCCESS)
		{	
			if (e->body.rx_msg.rdata != NULL)
			{
				if (e->body.rx_msg.rdata->msg_info.msg != NULL)
				{
					pjsip_subject_hdr * subject = (pjsip_subject_hdr*)pjsip_msg_find_hdr_by_name(e->body.rx_msg.rdata->msg_info.msg, &_gSubjectHdr, NULL);
					if (subject != NULL)
					{
						SipCall * sipcall = (SipCall*)pjsua_call_get_user_data(call_id);
						if (sipcall)
						{
							pj_ansi_snprintf(sipcall->LastReason, sizeof(sipcall->LastReason) - 1, "%.*s", 
								subject->hvalue.slen, subject->hvalue.ptr);
						}
					}
				}
			}
		}
	}
	else if ((tsx->state == PJSIP_TSX_STATE_TRYING) &&
		(pjsip_method_cmp(&tsx->method, &info_method) == 0))
	{
		pj_bool_t processed = false;
		if (tsx->role == PJSIP_ROLE_UAS) processed = ProcessReceivedSelcalInfo(call_id, tsx, e);
		if (!processed) ProcessReceivedGenericInfo(call_id, tsx, e);
	}
	else if ((tsx->role == PJSIP_ROLE_UAC) && (tsx->state == PJSIP_TSX_STATE_COMPLETED) &&
		(pjsip_method_cmp(&tsx->method, pjsip_get_refer_method()) == 0))
	{
		if (SipAgent::Cb.TransferStatusCb)
		{
			int code = e->body.tsx_state.src.rdata->msg_info.msg->line.status.code;
			if (code != PJSIP_SC_ACCEPTED)
			{
				if (TRACE_ALL_CALLS) PJ_LOG(3, (__FILE__, "TransferStatusCb %d", call_id | CORESIP_CALL_ID));
				SipAgent::Cb.TransferStatusCb(call_id | CORESIP_CALL_ID, code);
			}
		}
	}	
	else if ((tsx->role == PJSIP_ROLE_UAS) && (tsx->state == PJSIP_TSX_STATE_TRYING) &&
		(pjsip_method_cmp(&tsx->method, pjsip_get_subscribe_method()) == 0))
	{

		pjsip_rx_data * rdata = e->body.tsx_state.src.rdata;

		pjsip_event_hdr * event = (pjsip_event_hdr*)pjsip_msg_find_hdr_by_name(rdata->msg_info.msg, &STR_EVENT, NULL);
		pjsip_expires_hdr *expires = (pjsip_expires_hdr*)pjsip_msg_find_hdr(rdata->msg_info.msg, PJSIP_H_EXPIRES, NULL);

		if (event && (pj_stricmp(&event->event_type, &STR_WG67KEY_IN) == 0) && expires->ivalue != 0)
		{
			pjsip_dialog* dlg = pjsip_tsx_get_dlg(tsx);
			if (dlg == NULL)
				return;

			WG67Subs::SubscriptionRxRequest(rdata, dlg);
		}
		else if (event && (pj_stricmp(&event->event_type, &STR_CONFERENCE) == 0) && expires->ivalue != 0)
		{
			pjsip_dialog * dlg = pjsip_tsx_get_dlg(tsx);
			if (dlg == NULL)
				return;

			SipCall * call = (SipCall*)pjsua_call_get_user_data(call_id);
			pj_status_t st;

if (call && dlg->local.contact->focus)
{
	if (call->_ConfInfoSrvEvSub)
	{
		PJ_LOG(5, ("sipcall.cpp", "NOTIFY CONF: Recibe conference SUBSCRIBE STATE %s", pjsip_evsub_get_state_name(call->_ConfInfoSrvEvSub)));
	}

	if (!call->_ConfInfoSrvEvSub)
	{
		st = pjsip_conf_create_uas(dlg, &call->_ConfInfoSrvCb, rdata, &call->_ConfInfoSrvEvSub);
		if (st != PJ_SUCCESS)
		{
			pjsua_perror("sipcall.cpp", "ERROR creando servidor de subscripcion a conferencia", st);
			goto subscription_error;
		}
		//pjsip_evsub_set_mod_data(call->_ConfInfoSrvEvSub, pjsua_var.mod.id, call);	
		pjsip_evsub_set_user_data(call->_ConfInfoSrvEvSub, (void*)call);

		PJ_LOG(5, ("sipcall.cpp", "NOTIFY CONF: Servidor SUBSCRIBE creado"));

		st = pjsip_conf_accept(call->_ConfInfoSrvEvSub, rdata, PJSIP_SC_OK, NULL);
		if (st != PJ_SUCCESS)
		{
			pjsua_perror("sipcall.cpp", "ERROR aceptando subscripcion a conferencia", st);
			goto subscription_error;
		}

		pjsip_evsub_set_state(call->_ConfInfoSrvEvSub, PJSIP_EVSUB_STATE_ACCEPTED);

		pjsua_call_info info;
		st = pjsua_call_get_info(call_id, &info);
		if (st == PJ_SUCCESS && SipAgent::Cb.IncomingSubscribeConfCb)
		{
			if (TRACE_ALL_CALLS) PJ_LOG(3, (__FILE__, "IncomingSubscribeConfCb call id %d", call_id | CORESIP_CALL_ID));
			SipAgent::Cb.IncomingSubscribeConfCb(call_id | CORESIP_CALL_ID, (const char*)info.remote_info.ptr, (const int)info.remote_info.slen);
		}
	}
}

return;

subscription_error:
pjsip_tx_data* tdata;
st = pjsip_dlg_create_response(dlg, rdata, PJSIP_SC_INTERNAL_SERVER_ERROR, NULL, &tdata);
if (st != PJ_SUCCESS)
{
	pjsua_perror("sipcall.cpp", "Unable to create error response to conference subscription", st);
	return;
}

st = pjsip_dlg_send_response(dlg, tsx, tdata);
if (st != PJ_SUCCESS)
{
	pjsua_perror("sipcall.cpp", "Unable to send error response to conference subscription", st);
	return;
}
		}
	}
}

/**
* Gestiona la creaci�n de los stream y su conexion a los puertos de conferencia
* http://www.pjsip.org/docs/latest-1/pjsip/docs/html/structpjsua__callback.htm#a88e61b65e936271603e5d63503aebcf6
*/
void SipCall::OnStreamCreated(pjsua_call_id call_id, pjmedia_session* sess, unsigned stream_idx, pjmedia_port** p_port)
{
	SipCall* call = (SipCall*)pjsua_call_get_user_data(call_id);

	if (call && (call->_Info.CallFlags & CORESIP_CALL_EC))
	{
		pjmedia_session_enable_ec(sess, stream_idx, SipAgent::EchoTail, SipAgent::EchoLatency, 0);							// AGL. 20131120. Par�metros Configurables.
	}
}

/*
Se llama en stop_media_session, justo antes de destruir session. Me servirá
para eliminarlo del grupo climax
*/

void SipCall::OnStreamDestroyed(pjsua_call_id call_id, pjmedia_session* sess, unsigned stream_idx)
{
	pjsua_call_info callInfo;
	if (pjsua_call_get_info(call_id, &callInfo) != PJ_SUCCESS)
	{
		return;
	}

	SipCall* call = (SipCall*)pjsua_call_get_user_data(call_id);
	if (call != NULL)
	{
		if (call->_Info.Type == CORESIP_CALL_RD)
		{
			if (callInfo.state == PJSIP_INV_STATE_DISCONNECTED)
			{
				EliminarRadSessionDelGrupo(call);
			}
		}
	}
}

pj_status_t SipCall::OnSendACK(pjsua_call_id call_id, pjsip_inv_session* inv, pjsip_rx_data* rdata, pjsip_event* e)
{
	SipCall* sipcall = (SipCall*)pjsua_call_get_user_data(call_id);
	if (sipcall && sipcall->_Dlg != NULL && pjsip_dlg_get_WG67_version(sipcall->_Dlg, NULL) == PJ_FALSE)
	{
		char radio_version = 0;
		char phone_version = 0;
		char WG67_version_value_buf[32];
		
		if (pjsip_endpt_Neg_ED137Version_from_msg(pjsua_var.endpt, e->body.tsx_state.tsx->last_tx->msg, rdata->msg_info.msg, 
			&radio_version, &phone_version,
			WG67_version_value_buf, sizeof(WG67_version_value_buf)) == PJ_TRUE)
		{
			if (pj_ansi_strlen(WG67_version_value_buf) > 0)
			{
				pjsip_dlg_set_WG67_version(sipcall->_Dlg, WG67_version_value_buf, PJ_TRUE);
				pjsip_tsx_set_WG67_version(e->body.tsx_state.tsx, WG67_version_value_buf);
			}
			if (sipcall->_Info.Type == CORESIP_CALL_RD) sipcall->ED137Version = radio_version;
			else sipcall->ED137Version = phone_version;
		}
	}

	pj_status_t status = pjsip_inv_create_ack(inv, rdata->msg_info.cseq->cseq, &inv->last_ack);
	if (status == PJ_SUCCESS) {
		/* Send ACK */
		status = pjsip_dlg_send_request(inv->dlg, inv->last_ack, -1, NULL);		
	}
	return status;
}

/**
* 
*/
void SipCall::SubscribeToConfInfo()
{
	pjsua_call * call;
	pjsip_dialog * dlg;

	pj_status_t st = acquire_call("SubscribeToConfInfo", _Id, &call, &dlg);
	if (st != PJ_SUCCESS)
	{
		return;
	}

	try
	{
		st = pjsip_conf_create_uac(dlg, &_ConfInfoClientCb, PJSIP_EVSUB_NO_EVENT_ID, &_ConfInfoClientEvSub);
		PJ_CHECK_STATUS(st, ("ERROR creando cliente para subscripcion a conferencia", "[Call=%d]", _Id));
		pjsip_evsub_set_mod_data(_ConfInfoClientEvSub, pjsua_var.mod.id, this);

		pjsip_tx_data * tdata;
		st = pjsip_conf_initiate(_ConfInfoClientEvSub, -1, &tdata);
		PJ_CHECK_STATUS(st, ("ERROR creando mensaje de subscripcion a conferencia", "[Call=%d]", _Id));

		/* Add additional headers etc */
		pjsua_process_msg_data(tdata, NULL);

		st = pjsip_conf_send_request(_ConfInfoClientEvSub, tdata);
		PJ_CHECK_STATUS(st, ("ERROR enviando subscripcion a conferencia", "[Call=%d]", _Id));
	}
	catch (...)
	{
		if (_ConfInfoClientEvSub != NULL)
		{
			pjsip_conf_terminate(_ConfInfoClientEvSub, PJ_FALSE);
			_ConfInfoClientEvSub = NULL;
		}
	}

	pjsip_dlg_dec_lock(dlg);
}

/**
* 
*/
void SipCall::OnConfInfoSrvStateChanged(pjsip_evsub *sub, pjsip_event *event)
{
	//SipCall * call = (SipCall*)pjsip_evsub_get_mod_data(sub, pjsua_var.mod.id);
	SipCall * call = (SipCall*) pjsip_evsub_get_user_data(sub);
	if (call) 
	{
		PJ_LOG(4, ("sipcall.cpp", "Conference server subscription is %s", pjsip_evsub_get_state_name(sub)));

		if (pjsip_evsub_get_state(sub) == PJSIP_EVSUB_STATE_TERMINATED) 
		{
			call->_ConfInfoSrvEvSub = NULL;
			pjsip_evsub_set_user_data(sub, NULL);
		} 
	}

	PJ_UNUSED_ARG(event);
}

/**
* 
*/
void SipCall::OnConfInfoClientStateChanged(pjsip_evsub *sub, pjsip_event *event)
{
	SipCall * call = (SipCall*)pjsip_evsub_get_mod_data(sub, pjsua_var.mod.id);
	if (call) 
	{
		PJ_LOG(4, ("sipcall.cpp", "Conference client subscription is %s", pjsip_evsub_get_state_name(sub)));

		if (pjsip_evsub_get_state(sub) == PJSIP_EVSUB_STATE_TERMINATED) 
		{
			call->_ConfInfoClientEvSub = NULL;
			pjsip_evsub_set_mod_data(sub, pjsua_var.mod.id, NULL);
		} 
	}

	PJ_UNUSED_ARG(event);
}

/**
* 
*/
void SipCall::OnConfInfoClientRxNotify(pjsip_evsub *sub, pjsip_rx_data *rdata, int *p_st_code, 
									   pj_str_t **p_st_text, pjsip_hdr *res_hdr, pjsip_msg_body **p_body)
{
	if (SipAgent::Cb.ConfInfoCb)
	{
		SipCall * call = (SipCall*)pjsip_evsub_get_mod_data(sub, pjsua_var.mod.id);

		pjsip_conf_status conf_info;

		if (PJ_SUCCESS == pjsip_conf_get_status(sub, &conf_info))
		{
			char from_urich[CORESIP_MAX_URI_LENGTH];
			pjsip_from_hdr *from_hdr = (pjsip_from_hdr*) pjsip_msg_find_hdr(rdata->msg_info.msg, PJSIP_H_FROM, NULL);
			int size = pjsip_uri_print(PJSIP_URI_IN_FROMTO_HDR, from_hdr->uri, from_urich, CORESIP_MAX_URI_LENGTH-1);
			from_urich[size] = '\0';

			CORESIP_ConfInfo info;

			info.Version = conf_info.version;
			info.UsersCount = conf_info.users_cnt;
			pj_ansi_snprintf(info.State, sizeof(info.State) - 1, "%.*s", conf_info.state.slen, conf_info.state.ptr);

			for (unsigned i = 0; i < conf_info.users_cnt; i++)
			{
				pj_ansi_snprintf(info.Users[i].Id, sizeof(info.Users[i].Id) - 1, "%.*s", conf_info.users[i].id.slen, conf_info.users[i].id.ptr);
				pj_ansi_snprintf(info.Users[i].Name, sizeof(info.Users[i].Name) - 1, "%.*s", conf_info.users[i].display.slen, conf_info.users[i].display.ptr);
				pj_ansi_snprintf(info.Users[i].Role, sizeof(info.Users[i].Role) - 1, "%.*s", conf_info.users[i].role.slen, conf_info.users[i].role.ptr);
				pj_ansi_snprintf(info.Users[i].State, sizeof(info.Users[i].State) - 1, "%.*s", conf_info.users[i].state.slen, conf_info.users[i].state.ptr);
			}

			if (call)
			{
				if (TRACE_ALL_CALLS) PJ_LOG(3, (__FILE__, "ConfInfoCb %d", call->_Id | CORESIP_CALL_ID));
				SipAgent::Cb.ConfInfoCb(call->_Id | CORESIP_CALL_ID, &info, from_urich, size);
			}
			else
			{
				//SipAgent::Cb.ConfInfoCb(-1, &info, from_urich, size);
			}
		}
	}

	/* The default is to send 200 response to NOTIFY.
	* Just leave it there..
	*/
	PJ_UNUSED_ARG(rdata);
	PJ_UNUSED_ARG(p_st_code);
	PJ_UNUSED_ARG(p_st_text);
	PJ_UNUSED_ARG(res_hdr);
	PJ_UNUSED_ARG(p_body);
}

/** 
Se llama cada vez que se va a transmitir un REQUEST SIP.
*/
pj_bool_t SipCall::OnTxRequest(pjsip_tx_data *txdata)
{
	pjsip_msg *msg = txdata->msg;
	pj_bool_t gWG67VersionRadio = PJ_FALSE;

	/** BYE Request WG67-Reason Header */
	if (txdata->msg->line.req.method.id == PJSIP_BYE_METHOD)
	{
		if (WG67Reason != 0)
		{
			Wg67ReasonSet(txdata);
			WG67Reason = 0;
		}
	}

#if 0
	pj_str_t wg67version = { "WG67-Version", 12 };
	pjsip_hdr* hdr = (pjsip_hdr *) pjsip_msg_find_hdr_by_name(msg, &wg67version, NULL);
	if (hdr)
	{
		pj_list_erase(hdr);
	}

	pj_str_t vedr = { "radio.01", 8 };
	Wg67VersionSet(txdata, &vedr);

	return PJ_FALSE;
#endif

#if 1

	pj_str_t wg67version = { "WG67-Version", 12 };
	pjsip_generic_string_hdr* wg67version_hdr = (pjsip_generic_string_hdr*)pjsip_msg_find_hdr_by_name(msg, &wg67version, NULL);

	if (txdata->msg->type == PJSIP_RESPONSE_MSG && (txdata->msg->line.status.code / 100) == 1)
	{
		//Si es una respuesta 1xx no lleva cabecera de version
		if (wg67version_hdr != NULL)
		{
			pjsip_hdr* hdr = (pjsip_hdr *) wg67version_hdr;
			pj_list_erase(hdr);
		}
	}
	else
	{		
		int force_wg67v;
		char ForcedED137Radioversion[128];
		pjsip_endpt_Get_Force_Ed137_version_header(pjsua_var.endpt, &force_wg67v, ForcedED137Radioversion, sizeof(ForcedED137Radioversion));
		if (force_wg67v)
		{
			pjsip_generic_string_hdr* wg67version_hdr = (pjsip_generic_string_hdr * ) pjsip_msg_find_hdr_by_name(txdata->msg, &pj_str("WG67-Version"), NULL);
			if (wg67version_hdr == NULL)
			{
				if (pj_ansi_strlen(ForcedED137Radioversion) > 0)
				{
					pjsip_generic_string_hdr* pWg67version = pjsip_generic_string_hdr_create(txdata->pool, &gWG67VersionName, &pj_str(ForcedED137Radioversion));
					pj_list_push_back(&txdata->msg->hdr, pWg67version);
				}
			}
			else
			{
				if (pj_ansi_strlen(ForcedED137Radioversion) > 0)
				{
					pj_strdup(txdata->pool, &wg67version_hdr->hvalue, &pj_str(ForcedED137Radioversion));
				}
				else
				{
					pjsip_hdr* hdr = (pjsip_hdr*)wg67version_hdr;
					pj_list_erase(hdr);
				}
			}
		}
		else if (wg67version_hdr == NULL)
		{
			//No lleva la cabecera WG67-version. La añadimos
			if (SipAgent::_IsRadioServerNodebox || SipAgent::_Radio_UA)
			{
				//Es un agente Radio
				gWG67VersionRadio = PJ_TRUE;
			}
			else
			{
				//Comprobamos si el account desde el que se envia el requests o la respuestas es un
				//account del tipo GRS radio

				pjsua_acc_id acc_id = PJSUA_INVALID_ID;

				if (msg->type == PJSIP_REQUEST_MSG)
				{
					//Si el mensaje es un request, entonces comprobamos si la uri del from corresponde
					//a algun account del agente. 
					pjsip_from_hdr* from = (pjsip_from_hdr*)pjsip_msg_find_hdr(msg, PJSIP_H_FROM, NULL);
					if (from != NULL)
					{
						//char buff[64];
						//pjsip_uri_print(PJSIP_URI_IN_FROMTO_HDR, from->uri, buff, sizeof(buff));

						acc_id = pjsua_acc_find_for_incoming_by_uri(from->uri);
					}
				}
				else
				{
					//Si el mensaje es una respuesta, entonces comprobamos si la uri del to corresponde
					//a algun account del agente. 
					pjsip_to_hdr* to = (pjsip_to_hdr*)pjsip_msg_find_hdr(msg, PJSIP_H_TO, NULL);
					if (to != NULL)
					{
						//char buff[64];
						//pjsip_uri_print(PJSIP_URI_IN_FROMTO_HDR, to->uri, buff, sizeof(buff));

						acc_id = pjsua_acc_find_for_incoming_by_uri(to->uri);
					}
				}

				if (acc_id != PJSUA_INVALID_ID)
				{
					//Con el account id tomamos los AccountUserData y de ahi se obtiene si la cuenta es del tipo GRS
					pj_bool_t rdAccount = PJ_FALSE;				//Indica si acc_id es un account tipo radio GRS
					AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(acc_id);
					if (accUserData != NULL)
					{
						rdAccount = accUserData->rdAccount;
						if (rdAccount)
						{
							gWG67VersionRadio = PJ_TRUE;
						}
						else
						{
							pjsua_call_id call_ids[PJSUA_MAX_CALLS];
							unsigned call_cnt = PJ_ARRAY_SIZE(call_ids);

							pjsip_cid_hdr* txdata_callid_hdr = (pjsip_cid_hdr*)pjsip_msg_find_hdr(msg, PJSIP_H_CALL_ID, NULL);

							/* Busco la cabecera Call-Id de este Request en todas las llamadas establecidas
								* En la que coincida obtengo el objeto SipCall para obtener el tipo de llamada.
								*/
							pj_status_t st = pjsua_enum_calls(call_ids, &call_cnt);
							if (st == PJ_SUCCESS)
							{
								pj_bool_t call_id_encontrado = PJ_FALSE;
								for (unsigned int i = 0; (i < call_cnt) && (call_id_encontrado == PJ_FALSE); i++)
								{
									pjsua_call* p_call;
									pjsip_dialog* p_dlg;

									st = acquire_call("OnTxRequest", call_ids[i], &p_call, &p_dlg);
									if (st == PJ_SUCCESS)
									{
										if (pj_stricmp(&txdata_callid_hdr->id, &p_dlg->call_id->id) == 0)
										{
											SipCall* sipcall = (SipCall*)(pjsua_call_get_user_data(call_ids[i]));
											if (sipcall != NULL)
											{
												if (sipcall->_Info.Type == CORESIP_CALL_RD)
												{
													gWG67VersionRadio = PJ_TRUE;
												}
											}
											call_id_encontrado = PJ_TRUE;
										}

										pjsip_dlg_dec_lock(p_dlg);
									}
								}
							}
						}
					}
				}
			}

			char ED137Radioversion_, ED137Phoneversion_;
			pjsip_endpt_Get_Ed137_version(pjsua_var.endpt, &ED137Radioversion_, &ED137Phoneversion_);
			char sWG67ver[32];
			pj_str_t pjsWG67ver;
			if (gWG67VersionRadio)
			{
				if (ED137Radioversion_ == 'C')
					pj_ansi_sprintf(sWG67ver, "%.*s, %.*s", SipCall::gWG67VersionRadioValue.slen, SipCall::gWG67VersionRadioValue.ptr,
						SipCall::gWG67VersionRadioValueC.slen, SipCall::gWG67VersionRadioValueC.ptr);
				else
					pj_ansi_sprintf(sWG67ver, "%.*s", SipCall::gWG67VersionRadioValue.slen, SipCall::gWG67VersionRadioValue.ptr);
				pjsWG67ver.ptr = sWG67ver;
				pjsWG67ver.slen = pj_ansi_strlen(sWG67ver);
				/** WG67-Version Header */
				Wg67VersionSet(txdata, &pjsWG67ver);
			}
			else
			{
				if (ED137Radioversion_ == 'C')
					pj_ansi_sprintf(sWG67ver, "%.*s, %.*s", SipCall::gWG67VersionTelefValue.slen, SipCall::gWG67VersionTelefValue.ptr,
						SipCall::gWG67VersionTelefValueC.slen, SipCall::gWG67VersionTelefValueC.ptr);
				else
					pj_ansi_sprintf(sWG67ver, "%.*s", SipCall::gWG67VersionTelefValue.slen, SipCall::gWG67VersionTelefValue.ptr);
				pjsWG67ver.ptr = sWG67ver;
				pjsWG67ver.slen = pj_ansi_strlen(sWG67ver);
				/** WG67-Version Header */
				Wg67VersionSet(txdata, &pjsWG67ver);
			}
		}
	}
#endif

	return PJ_FALSE;

}

void SipCall::SetImpairments(pjsua_call_id call_id, CORESIP_Impairments * impcfg)
{
	pjsua_call * call;
	pjsip_dialog * dlg;
	pj_status_t st;

	if (impcfg->LatMax < impcfg->LatMin)
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: SipCall::SetImpairments: LatMax must be greater or equal than LatMin"));

	st = acquire_call("SetImpairments()", call_id, &call, &dlg);
	if (st != PJ_SUCCESS)
	{
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: SetImpairments: ", "Invalid call_id %d", call_id));
		return;
	}

	pjsua_call_info call_info;
	if (pjsua_call_get_info(call_id, &call_info) != PJ_SUCCESS)
	{
		pjsip_dlg_dec_lock(dlg);
		throw PJLibException(__FILE__, PJ_EINVALIDOP).Msg("SetImpairments:", "ERROR: No se puede obtener call_info. call_id %d", call_id);
		return;
	}

	if (call_info.state != PJSIP_INV_STATE_CONFIRMED)
	{
		pjsip_dlg_dec_lock(dlg);
		PJ_CHECK_STATUS(PJ_EINVALIDOP, ("SetImpairments: ", "ERROR: Llamada no activa [Call=%d]", call_id));
	}
	
	pjmedia_session* session = pjsua_call_get_media_session(call_id);
	if (session != NULL)
	{
		st = pjmedia_session_force_set_impairments(session, impcfg->Perdidos, impcfg->Duplicados, impcfg->LatMin, impcfg->LatMax);
		if (st != PJ_SUCCESS)
		{
			pjsip_dlg_dec_lock(dlg);
			PJ_CHECK_STATUS(PJ_EINVALIDOP, ("ERROR: SetImpairments: ", "La media de la llamada no esta activa call_id %d", call_id));
		}
	}
	else
	{
		pjsip_dlg_dec_lock(dlg);
		PJ_CHECK_STATUS(PJ_EINVALIDOP, ("ERROR: SetImpairments: ", "La media de la llamada no esta activa call_id %d", call_id));
	}

	pjsip_dlg_dec_lock(dlg);
}

/**
*	CORESIP_SetCallParameters. Configura parametros para una sesion SIP activa.
*	@param	call		Call Id que identifica la llamada
*	@param	disableKeepAlives. Si vale 1 los Keepalives dejan de enviarse. con valor 0 se envian. Si el puntero es NULL se ignora.
*	@param	forced_cld. Valor forzado del CLD en ms. Si el valor es negativo, entonces se envia el calculado (Tn1 en el caso del ETM). Si el puntero es NULL se ignora.
*	@param	error		Puntero @ref CORESIP_Error a la Estructura de error
*	@return				Codigo de Error
*/
void SipCall::SetCallParameters(pjsua_call_id call_id, int *disableKeepAlives, int *forced_cld)
{
	pjsua_call* call;
	pjsip_dialog* dlg;
	pj_status_t st;	

	if (forced_cld != NULL)
	{
		if (*forced_cld > 254)
		{
			PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: SipCall::SetCallParameters: forced_cld must be less than 255ms"));
		}
	}

	st = acquire_call("EnableKeepAlives()", call_id, &call, &dlg);
	if (st != PJ_SUCCESS)
	{
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: SipCall::SetCallParameters: invalid call_id"));
		return;
	}

	pjsua_call_info call_info;
	if (pjsua_call_get_info(call_id, &call_info) != PJ_SUCCESS)
	{
		pjsip_dlg_dec_lock(dlg);
		throw PJLibException(__FILE__, PJ_EINVALIDOP).Msg("SetCallParameters:", "ERROR: No se puede obtener call_info. call_id %d", call_id);
		return;
	}

	if (call_info.state != PJSIP_INV_STATE_CONFIRMED)
	{
		pjsip_dlg_dec_lock(dlg);
		PJ_CHECK_STATUS(PJ_EINVALIDOP, ("SetCallParameters: ", "ERROR: Llamada no activa [Call=%d]", call_id));
	}

	pjmedia_session* session = pjsua_call_get_media_session(call_id);
	if (session != NULL)
	{
		pjmedia_stream* stream = NULL;
		stream = pjmedia_session_get_stream(session, 0);
		if (stream != NULL)
		{
			if (disableKeepAlives != NULL)
			{
				pjmedia_stream_disable_keepalives(stream, *disableKeepAlives);
			}
		}
		else
		{
			pjsip_dlg_dec_lock(dlg);
			PJ_CHECK_STATUS(PJ_EINVALIDOP, ("ERROR: SipCall::SetCallParameters: La llamada no tiene activa la media"));
		}
	}
	else
	{
		pjsip_dlg_dec_lock(dlg);
		PJ_CHECK_STATUS(PJ_EINVALIDOP, ("ERROR: SipCall::SetCallParameters: La llamada no tiene activa la media"));
	}

	SipCall* sipcall = (SipCall*)pjsua_call_get_user_data(call_id);
	if (sipcall != NULL)
	{
		if (forced_cld != NULL)
		{
			sipcall->_Info.forced_cld = *forced_cld;
		}
	}
	else
	{
		pjsip_dlg_dec_lock(dlg);
		PJ_CHECK_STATUS(PJ_EINVALIDOP, ("ERROR: SipCall::SetCallParameters: Posiblemente la llamada no este activa"));
	}

	pjsip_dlg_dec_lock(dlg);
}

void SipCall::GetNetworkDelay(pjsua_call_id call_id, unsigned int* delay_ms)
{
	pjsua_call* call;
	pjsip_dialog* dlg;
	pj_status_t st;

	if (delay_ms != NULL) *delay_ms = 0;

	st = acquire_call("GetNetworkDelay()", call_id, &call, &dlg);
	if (st != PJ_SUCCESS)
	{
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: SipCall::GetNetworkDelay: invalid call_id"));
	}

	pjsua_call_info call_info;
	if (pjsua_call_get_info(call_id, &call_info) != PJ_SUCCESS)
	{
		pjsip_dlg_dec_lock(dlg);
		throw PJLibException(__FILE__, PJ_EINVALIDOP).Msg("GetNetworkDelay:", "ERROR: No se puede obtener call_info. call_id %d", call_id);
		return;
	}

	if (call_info.state != PJSIP_INV_STATE_CONFIRMED)
	{
		pjsip_dlg_dec_lock(dlg);
		PJ_CHECK_STATUS(PJ_EINVALIDOP, ("GetNetworkDelay: ", "ERROR: Llamada no activa [Call=%d]", call_id));
	}

	SipCall* sipcall = (SipCall *) pjsua_call_get_user_data(call_id);
	if (sipcall == NULL)
	{
		pjsip_dlg_dec_lock(dlg);
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: SipCall::GetNetworkDelay: invalid call_id"));
	}
		
	pj_mutex_lock(sipcall->RdInfo_prev_mutex);
	sipcall->RdInfo_prev.MAM_received = 0;
	pj_mutex_unlock(sipcall->RdInfo_prev_mutex);

	while (pj_sem_trywait(sipcall->waiting_MAM_sem) == PJ_SUCCESS);

	pjmedia_session *session = pjsua_call_get_media_session(call_id);
	if (session != NULL)
	{
		pjmedia_session_force_request_MAM(session);
	}
	else	
	{
		pjsip_dlg_dec_lock(dlg);
		PJ_CHECK_STATUS(PJ_EINVALIDOP, ("ERROR: GetNetworkDelay: La llamada no tiene activa la media"));
	}

	pjsip_dlg_dec_lock(dlg);

	pj_sem_wait_for(sipcall->waiting_MAM_sem, 2000);

	unsigned int MAM_received = 0;
	pj_mutex_lock(sipcall->RdInfo_prev_mutex);
	MAM_received = sipcall->RdInfo_prev.MAM_received;
	if (MAM_received)
	{
		if (delay_ms != NULL) *delay_ms = sipcall->RdInfo_prev.Tn1_ms;
	}
	pj_mutex_unlock(sipcall->RdInfo_prev_mutex);

	if (!MAM_received)
	{
		PJ_CHECK_STATUS(PJ_ETIMEDOUT, ("ERROR: SipCall::GetNetworkDelay: Timeout"));
	}	
}

pj_bool_t SipCall::ProcessReceivedGenericInfo(pjsua_call_id call_id, pjsip_transaction* tsx, pjsip_event* e)
{
	pj_bool_t processed = PJ_FALSE;
	int answer = PJSIP_SC_OK;
	/* Answer incoming INFO with 200/OK */
	pjsip_rx_data* rdata = e->body.tsx_state.src.rdata;
	if (rdata == NULL)
	{
		answer = PJSIP_SC_BAD_REQUEST;
		processed = PJ_TRUE;
	}

	if (answer == PJSIP_SC_OK)
	{
		if (rdata->msg_info.msg->body)
		{
			PJ_LOG(3, (__FILE__, "Call %d: incoming INFO:\n%.*s",
				call_id,
				(int)rdata->msg_info.msg->body->len,
				rdata->msg_info.msg->body->data));


			if (SipAgent::Cb.InfoReceivedCb)
			{
				if (TRACE_ALL_CALLS) PJ_LOG(3, (__FILE__, "InfoReceivedCb call id %d", call_id | CORESIP_CALL_ID));
				SipAgent::Cb.InfoReceivedCb(call_id | CORESIP_CALL_ID, (const char*)(rdata->msg_info.msg->body->data), rdata->msg_info.msg->body->len);
			}
			processed = PJ_TRUE;
		}
		else
		{
			processed = PJ_TRUE;
		}
	}

	if (processed && rdata != NULL)
	{
		pjsip_dialog* dlg = pjsip_tsx_get_dlg(tsx);
		if (dlg != NULL)
		{
			pjsip_tx_data* tdata;
			pj_status_t st = pjsip_dlg_create_response(dlg, rdata, answer, NULL, &tdata);
			if (st == PJ_SUCCESS)
			{
				st = pjsip_dlg_send_response(dlg, tsx, tdata);
				if (st != PJ_SUCCESS)
				{
					pjsua_perror("sipcall.cpp", "Unable to send response to INFO", st);
				}
			}
			else
			{
				pjsua_perror("sipcall.cpp", "Unable to send response to INFO", st);
			}
		}
	}

	return processed;
}

