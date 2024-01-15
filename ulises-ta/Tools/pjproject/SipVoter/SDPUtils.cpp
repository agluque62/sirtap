#include "Global.h"
#include "SDPUtils.h"
#include "SipAgent.h"
#include "SipCall.h"
#include "AccountUserData.h"

static pj_str_t gSubjectHdr = { "Subject", 7 };
static pj_str_t gPriorityHdr = { "Priority", 8 };
static pj_str_t gSubject[] = { { "IA call", 7 }, { "monitoring", 10 }, { "G/G monitoring", 14 }, { "A/G monitoring", 14 }, { "DA/IDA call", 11}, { "radio", 5 } };
static pj_str_t gPriority[] = { { "emergency", 9 }, { "urgent", 6 }, { "normal", 6 }, { "non-urgent", 10 } };
static pj_str_t gRecvOnly = { "recvonly", 8 };
static pj_str_t gSendOnly = { "sendonly", 8 };

void SDPUtils::OnCreateSdp(pj_pool_t* pool, int call_id, void* local_sdp, void* incoming_rdata)
{
	if (pool != NULL && call_id != PJSUA_INVALID_ID && local_sdp != NULL)
	{
		pjmedia_sdp_session* sdp = (pjmedia_sdp_session*)local_sdp;
		SipCall* call = (SipCall*)pjsua_call_get_user_data(call_id);
		pjsip_rx_data* rdata = (pjsip_rx_data*)incoming_rdata;
		pjsua_call_info callinfo;
		pjsua_acc_id acc_id = PJSUA_INVALID_ID;

		if (pjsua_var.calls[call_id].inv != NULL)
		{
			if (pjsua_call_get_info(call_id, &callinfo) == PJ_SUCCESS)
			{
				acc_id = callinfo.acc_id;
			}
			else
			{
				callinfo.id = PJSUA_INVALID_ID;
			}
		}
		else
		{
			//No hay objeto inv porque estamos enviado el INVITE todavia y pjsua_call_get_info retorna error
			//y asi evitamos el log de invalid callid
			callinfo.id = PJSUA_INVALID_ID;
		}

		if (callinfo.id == PJSUA_INVALID_ID)
		{
			//No se ha terminado de establecer la sesion, obtenemos el account a partir de las uris recibidas en rdata
			if (rdata != NULL)
			{
				pjsip_uri* uri;
				//Se busca primero un account con la uri del To. Si no tiene exito, 
				//se busca con la uri de la request-line
				if (rdata->msg_info.to != NULL)
				{
					uri = rdata->msg_info.to->uri;
					acc_id = pjsua_acc_find_for_incoming_by_uri(uri);
				}
				if (acc_id == PJSUA_INVALID_ID)
				{
					if (rdata->msg_info.msg != NULL)
					{
						uri = rdata->msg_info.msg->line.req.uri;
						acc_id = pjsua_acc_find_for_incoming_by_uri(uri);
					}
				}
			}
		}

		pj_bool_t rdAccount = PJ_FALSE;				//Indica si add_id es un account tipo radio GRS
		CORESIP_CallFlagsMask TipoGrsFlags = CORESIP_CALL_NINGUNO;

		if (call != NULL && sdp != NULL)
		{
			if (sdp->media_count > 0)
			{
				call->_Info.SourcePort = sdp->media[0]->desc.port;
			}
		}

		AccountUserData* accUserData = NULL;
		if (acc_id != PJSUA_INVALID_ID)
		{
			accUserData = (AccountUserData*)pjsua_acc_get_user_data(acc_id);
			if (accUserData != NULL)
			{
				rdAccount = accUserData->rdAccount;
				TipoGrsFlags = accUserData->TipoGrsFlags;
			}
		}

		if ((!(call && call->_Info.Type != CORESIP_CALL_RD)) && (SipAgent::_Radio_UA || rdAccount) && pool)
		{
			//La llamada es del tipo radio
			//Es un agente radio o el account es de una radio GRS	

			pjmedia_sdp_session* rem_sdp = NULL;
			pjmedia_sdp_attr* remote_pttrep_attr = NULL;
			pjmedia_sdp_attr* remote_fid_attr = NULL;

			if (rdata != NULL)
			{
				pj_status_t status = pjmedia_sdp_parse(pool, (char*)rdata->msg_info.msg->body->data, rdata->msg_info.msg->body->len, &rem_sdp);
				if (status == PJ_SUCCESS && rem_sdp)
				{
					/* Validate */
					status = pjmedia_sdp_validate(rem_sdp);
				}

				if (status == PJ_SUCCESS && rem_sdp && rem_sdp->media_count > 0)
				{
					for (unsigned int i = 0; i < rem_sdp->media_count; i++)
					{
						remote_pttrep_attr = pjmedia_sdp_media_find_attr2(rem_sdp->media[i], "ptt_rep", NULL);
						remote_fid_attr = pjmedia_sdp_media_find_attr2(rem_sdp->media[i], "fid", NULL);
					}
				}
			}

			//Es un agente radio. Inicializamos aqui el SDP para este caso.
			//Utilizamos el pool que se pasa como parametro.
			pjmedia_sdp_attr* a;

			unsigned int i;
			for (i = 0; i < sdp->media[0]->desc.fmt_count; i++)
			{
				if (pj_strcmp(&sdp->media[0]->desc.fmt[i], &pj_str("123")) == 0)
				{
					break;
				}
			}

			if (i == sdp->media[0]->desc.fmt_count)
			{
				pj_str_t* fmt = &sdp->media[0]->desc.fmt[sdp->media[0]->desc.fmt_count++];
				fmt->ptr = (char*)pj_pool_alloc(call->_Pool, 8);
				fmt->slen = pj_utoa(123, fmt->ptr);

				a = pjmedia_sdp_attr_create(call->_Pool, "rtpmap", &(pj_str("123 R2S/8000")));
				pjmedia_sdp_media_add_attr(sdp->media[0], a);
			}

			UpdateLocalSdpRadioTypeAsGRS(pool, call_id, sdp, rem_sdp, &TipoGrsFlags);

			pj_str_t rtphe = pj_str("1");
			a = pjmedia_sdp_attr_create(pool, "rtphe", &rtphe);
			pjmedia_sdp_media_add_attr(sdp->media[0], a);

			if (remote_pttrep_attr)
			{
				pj_str_t pttrep = pj_str("1");
				a = pjmedia_sdp_attr_create(pool, "ptt_rep", &pttrep);
				pjmedia_sdp_media_add_attr(sdp->media[0], a);
			}

			if (remote_fid_attr)
			{
				pjmedia_sdp_media_add_attr(sdp->media[0], remote_fid_attr);
			}
		}
		else if (call)
		{
			//Actualizamos el atributo de Media Direction
			if (call->_Info.ForceSDPSendRecvAttr != CORESIP_SDP_SR_ATTR_NOFORCED)
			{
				pjmedia_sdp_attr* a;
				//Borramos todos los atributos
				for (unsigned int i = 0; i < sdp->media_count; i++)
				{	
					pjmedia_sdp_media_remove_all_attr(sdp->media[i], "inactive");
					pjmedia_sdp_media_remove_all_attr(sdp->media[i], "sendrecv");
					pjmedia_sdp_media_remove_all_attr(sdp->media[i], "sendonly");
					pjmedia_sdp_media_remove_all_attr(sdp->media[i], "recvonly");
				}

				//Ponemos el que queremos forzar
				switch (call->_Info.ForceSDPSendRecvAttr)
				{
				case CORESIP_SDP_SR_ATTR_NONE:
					break;
				case CORESIP_SDP_SR_ATTR_INACTIVE:
					a = pjmedia_sdp_attr_create(call->_Pool, "inactive", NULL);
					break;
				case CORESIP_SDP_SR_ATTR_SENDONLY:
					a = pjmedia_sdp_attr_create(call->_Pool, "sendonly", NULL);
					break;
				case CORESIP_SDP_SR_ATTR_RECVONLY:
					a = pjmedia_sdp_attr_create(call->_Pool, "recvonly", NULL);
					break;
				default:
					a = pjmedia_sdp_attr_create(call->_Pool, "sendrecv", NULL);
					break;
				}				
				if (call->_Info.ForceSDPSendRecvAttr != CORESIP_SDP_SR_ATTR_NONE)
				{
					pjmedia_sdp_media_add_attr(sdp->media[0], a);
				}

			}

			if (call->_Info.Type != CORESIP_CALL_RD)
			{
				pjmedia_sdp_attr* a;
				//Para las llamadas que no son de radio quitamos el codec 123 R2S
				for (unsigned int i = 0; i < sdp->media_count; i++)
				{
					pj_str_t r2s = pj_str("123 R2S/8000");
					do {
						if ((a = pjmedia_sdp_media_find_attr2(sdp->media[i], "rtpmap", &r2s)) != NULL)
						{
							pjmedia_sdp_media_remove_attr(sdp->media[i], a);
							sdp->media[i]->desc.fmt_count--;
						}
					} while (a != NULL);
				}
			}


			if (call->_Info.Type == CORESIP_CALL_RD)
			{
				//En este caso somos un agente de telefonia que abre sesion con una radio. Se inicializa aqui el SDP
				//SE usa el pool del objeto SipCall

				pjmedia_sdp_attr* a;

				/** ED137.. B */
				pj_str_t typedata =
					call->_Info.CallFlags & CORESIP_CALL_RD_IDLE ? (pj_str("Radio-Idle")) :
					call->_Info.CallFlags & CORESIP_CALL_RD_COUPLING ? (pj_str("Coupling")) :
					call->_Info.CallFlags & CORESIP_CALL_RD_RADIO_RXONLY ? (pj_str("Radio-Rxonly")) :
					SipAgent::ETM ? (pj_str("Radio-TxRx")) : (pj_str("Radio"));

				a = pjmedia_sdp_attr_create(call->_Pool, "type", &typedata);
				pjmedia_sdp_media_add_attr(sdp->media[0], a);

				/** CODEC 123 */

				if (0) //(call->_Info.CallFlags & CORESIP_CALL_RD_IDLE)
				{
					//Si la llamada es idle entonces solo trabajamos con el tipo de codec 123
					sdp->media[0]->desc.fmt_count = 0;

					for (unsigned int i = 0; i < sdp->media_count; i++)
					{
						do {
							if ((a = pjmedia_sdp_media_find_attr2(sdp->media[i], "rtpmap", NULL)) != NULL)
							{
								pjmedia_sdp_media_remove_attr(sdp->media[i], a);
							}
						} while (a != NULL);
					}

					pj_str_t* fmt = &sdp->media[0]->desc.fmt[sdp->media[0]->desc.fmt_count++];
					fmt->ptr = (char*)pj_pool_alloc(call->_Pool, 8);
					fmt->slen = pj_utoa(123, fmt->ptr);

					a = pjmedia_sdp_attr_create(call->_Pool, "rtpmap", &(pj_str("123 R2S/8000")));
					pjmedia_sdp_media_add_attr(sdp->media[0], a);
				}
				else
				{
					unsigned int i;
					for (i = 0; i < sdp->media[0]->desc.fmt_count; i++)
					{
						if (pj_strcmp(&sdp->media[0]->desc.fmt[i], &pj_str("123")) == 0)
						{
							break;
						}
					}

					if (i == sdp->media[0]->desc.fmt_count)
					{
						/*  Este es el bloque bueno */
						pj_str_t* fmt = &sdp->media[0]->desc.fmt[sdp->media[0]->desc.fmt_count++];
						fmt->ptr = (char*)pj_pool_alloc(call->_Pool, 8);
						fmt->slen = pj_utoa(123, fmt->ptr);

						a = pjmedia_sdp_attr_create(call->_Pool, "rtpmap", &(pj_str("123 R2S/8000")));
						pjmedia_sdp_media_add_attr(sdp->media[0], a);
					}
					else
					{
					}
				}

				if ((call->_Info.CallFlags & CORESIP_CALL_NO_TXRXMODE) == 0)
				{
					pj_str_t txrxmodedata = (call->_Info.CallFlags & CORESIP_CALL_RD_RXONLY) ? (pj_str("Rx")) :
						(call->_Info.CallFlags & CORESIP_CALL_RD_TXONLY) ? (pj_str("Tx")) : (pj_str("TxRx"));
					a = pjmedia_sdp_attr_create(call->_Pool, "txrxmode", &txrxmodedata);
					pjmedia_sdp_media_add_attr(sdp->media[0], a);
				}

				if (SipAgent::ETM)
				{
					//En el caso del ETM se envia el fid. Siempre que no sea el valor "000.000"
					if (pj_ansi_strcmp(call->_RdFr, "000.000") != 0 && pj_ansi_strlen(call->_RdFr) > 0)
					{
						a = pjmedia_sdp_attr_create(call->_Pool, "fid", &(pj_str(call->_RdFr)));
						pjmedia_sdp_media_add_attr(sdp->media[0], a);
					}
				}

				if (SipAgent::ETM)
				{
					if (pj_ansi_strlen(call->_Info.etm_vcs_bss_methods) > 0)
					{
						char method[CORESIP_MAX_BSS_LENGTH];
						method[0] = '\0';
						int j = 0;
						for (int i = 0; i < sizeof(call->_Info.etm_vcs_bss_methods); i++)
						{
							if (call->_Info.etm_vcs_bss_methods[i] == ',' || call->_Info.etm_vcs_bss_methods[i] == '\0')
							{
								//metodo bss
								if (strlen(method) > 0)
								{
									a = pjmedia_sdp_attr_create(call->_Pool, "bss", &(pj_str(method)));
									pjmedia_sdp_media_add_attr(sdp->media[0], a);
								}

								method[0] = '\0';
								j = 0;
							}
							else
							{
								if ((j + 1) < sizeof(method))
								{
									method[j] = call->_Info.etm_vcs_bss_methods[i];
									j++;
									method[j] = '\0';
								}
							}
							if (call->_Info.etm_vcs_bss_methods[i] == '\0') break;
						}
					}
				}
				else
				{
					if (!(call->_Info.CallFlags & CORESIP_CALL_RD_TXONLY) && (call->bss_method_type != NINGUNO))
					{
						if (call->bss_method_type == RSSI_NUC)
						{
							a = pjmedia_sdp_attr_create(call->_Pool, "bss", &(pj_str("NUCLEO")));
							pjmedia_sdp_media_add_attr(sdp->media[0], a);

							a = pjmedia_sdp_attr_create(call->_Pool, "bss", &(pj_str("RSSI")));
							pjmedia_sdp_media_add_attr(sdp->media[0], a);
						}
						else if (call->bss_method_type == RSSI)
						{
							a = pjmedia_sdp_attr_create(call->_Pool, "bss", &(pj_str("RSSI")));
							pjmedia_sdp_media_add_attr(sdp->media[0], a);
						}
						else if ((call->bss_method_type == CENTRAL) && (call->_Info.porcentajeRSSI > SipCall::MIN_porcentajeRSSI))
						{
							//Si es CENTRALIZADO y se necesita un porcentaje de valor RSSI
							a = pjmedia_sdp_attr_create(call->_Pool, "bss", &(pj_str("RSSI")));
							pjmedia_sdp_media_add_attr(sdp->media[0], a);
						}
					}
				}

				a = pjmedia_sdp_attr_create(call->_Pool, "interval", &(pj_str("20")));
				pjmedia_sdp_media_add_attr(sdp->media[0], a);
				a = pjmedia_sdp_attr_create(call->_Pool, "sigtime", &(pj_str("1")));
				pjmedia_sdp_media_add_attr(sdp->media[0], a);


				/*¡ATENCION! Si se modifica el valor de ptt_rep hay que modificar el valor de la etiqueta
				 * PTT_REP_COUNT en el fichero stream.c de pjmedia. Para ptt_rep=1 PTT_REP_COUNT debe valer 2,
				 * que es el numero de paquetes rtp con ptt off al finalizar un ptt on
				 */
				if ((call->_Info.CallFlags & CORESIP_CALL_RD_RXONLY) == 0)
				{
					a = pjmedia_sdp_attr_create(call->_Pool, "ptt_rep", &(pj_str("1")));
					pjmedia_sdp_media_add_attr(sdp->media[0], a);
				}

				/** AGL 1420528 */
				pj_str_t rtphe = pj_str("1");
				a = pjmedia_sdp_attr_create(call->_Pool, "rtphe", &rtphe);
				pjmedia_sdp_media_add_attr(sdp->media[0], a);

				char kap[8];
				pj_ansi_snprintf(kap, sizeof(kap), "%u", call->_R2SKeepAlivePeriod);
				char kam[8];
				pj_ansi_snprintf(kam, sizeof(kam), "%u", call->_R2SKeepAliveMultiplier);
				a = pjmedia_sdp_attr_create(call->_Pool, "R2S-KeepAlivePeriod", &(pj_str(kap)));
				pjmedia_sdp_media_add_attr(sdp->media[0], a);
				a = pjmedia_sdp_attr_create(call->_Pool, "R2S-KeepAliveMultiplier", &(pj_str(kam)));
				pjmedia_sdp_media_add_attr(sdp->media[0], a);

				if (call->_Info.NoFreqDisconn)
				{
					a = pjmedia_sdp_attr_create(call->_Pool, "NoFreqDisconn", NULL);
					pjmedia_sdp_media_add_attr(sdp->media[0], a);
				}
			}
			else if ((call->_Info.Type == CORESIP_CALL_MONITORING) ||
				(call->_Info.Type == CORESIP_CALL_GG_MONITORING) ||
				(call->_Info.Type == CORESIP_CALL_AG_MONITORING))
			{
				/*La llamada es del tipo monitoring (escucha)*/
				pjmedia_sdp_attr* attr = pjmedia_sdp_media_find_attr2(sdp->media[0], "sendrecv", NULL);
				if (attr != NULL)
				{
					if (callinfo.id != PJSUA_INVALID_ID && callinfo.role == PJSIP_ROLE_UAC)
					{
						//La llamada ya ha sido establecida y en este caso el rol es UAC
						attr->name = gRecvOnly;
					}
					else if (callinfo.id != PJSUA_INVALID_ID && callinfo.role == PJSIP_ROLE_UAS)
					{
						//La llamada ya ha sido establecida y en este caso el rol es UAS
						attr->name = gSendOnly;
					}
					else
					{
						//En este caso la llamada no ha sido establecida, nosotros somos los que actuamos de uac
						// es decir, los que llamamos
						attr->name = gRecvOnly;
					}
				}
			}
			else if (call->_Info.Type == CORESIP_CALL_IA && !SipAgent::EnableMonitoring && call->_Info.ForceSDPSendRecvAttr == CORESIP_SDP_SR_ATTR_NOFORCED)
			{
				pjmedia_sdp_attr* attr = pjmedia_sdp_media_find_attr2(sdp->media[0], "sendrecv", NULL);
				if (attr != NULL)
				{
					if (callinfo.id != PJSUA_INVALID_ID)
					{
						if (callinfo.role == PJSIP_ROLE_UAC) attr->name = gSendOnly;
						else attr->name = gRecvOnly;
					}
					else
					{
						//En este caso la llamada no ha sido establecida, nosotros somos los que actuamos de uac
						// es decir, los que llamamos
						attr->name = gSendOnly;
					}
				}
			}
		}
		else if (rdata && !SipAgent::EnableMonitoring)
		{
			//call es NULL, es el primer INVITE que recibimos con UAS

			pjsip_subject_hdr* subject = (pjsip_subject_hdr*)pjsip_msg_find_hdr_by_name(rdata->msg_info.msg, &gSubjectHdr, NULL);
			if (subject && (pj_stricmp(&subject->hvalue, &gSubject[CORESIP_CALL_IA]) == 0))
			{
				pjmedia_sdp_session* rem_sdp = NULL;
				pjmedia_sdp_attr* remote_sendrecv_attr = NULL;

				pj_status_t status = pjmedia_sdp_parse(pool, (char*)rdata->msg_info.msg->body->data, rdata->msg_info.msg->body->len, &rem_sdp);
				if (status == PJ_SUCCESS && rem_sdp)
				{
					/* Validate */
					status = pjmedia_sdp_validate(rem_sdp);
				}

				if (status == PJ_SUCCESS && rem_sdp && rem_sdp->media_count > 0)
				{
					for (unsigned int i = 0; i < rem_sdp->media_count; i++)
					{
						remote_sendrecv_attr = pjmedia_sdp_media_find_attr2(rem_sdp->media[i], "sendrecv", NULL);
					}
				}

				pjmedia_sdp_attr* attr = pjmedia_sdp_media_find_attr2(sdp->media[0], "sendrecv", NULL);
				if (attr != NULL && remote_sendrecv_attr == NULL)
					attr->name = gRecvOnly;
			}
		}
	}
}

void SDPUtils::UpdateLocalSdpRadioTypeAsGRS(pj_pool_t* pool, int call_id, pjmedia_sdp_session* local_sdp, pjmedia_sdp_session* remote_sdp, CORESIP_CallFlagsMask* TipoGrsFlags)
{
	if (local_sdp == NULL) return;
	SipCall* call = (SipCall*)pjsua_call_get_user_data(call_id);
	if (call == NULL) return;

	pjmedia_sdp_attr* a = NULL;
	for (unsigned int i = 0; i < local_sdp->media_count; i++)
	{
		if ((a = pjmedia_sdp_media_find_attr2(local_sdp->media[i], "type", NULL)) != NULL)
		{
			pjmedia_sdp_media_remove_attr(local_sdp->media[i], a);
		}
		if ((a = pjmedia_sdp_media_find_attr2(local_sdp->media[i], "txrxmode", NULL)) != NULL)
		{
			pjmedia_sdp_media_remove_attr(local_sdp->media[i], a);
		}
	}

	a = NULL;
	pj_bool_t type_created = PJ_FALSE;
	pjmedia_sdp_attr* txrxmode_received = NULL;
	if (remote_sdp != NULL)
	{
		//Confirmamos el type, que sera igual al que nos envia el remoto
		//Buscamos si el remoto tiene txrxmode para despues confirmar el adecuado		
		for (unsigned int i = 0; i < remote_sdp->media_count; i++)
		{
			if (type_created == PJ_FALSE)
			{
				if ((a = pjmedia_sdp_media_find_attr2(remote_sdp->media[i], "type", NULL)) != NULL)
				{
					a = pjmedia_sdp_attr_create(pool, "type", &a->value);
					type_created = PJ_TRUE;
				}
			}
			txrxmode_received = pjmedia_sdp_media_find_attr2(remote_sdp->media[i], "txrxmode", NULL);
			if (type_created && txrxmode_received != NULL) break;
		}
	}

	if (type_created) pjmedia_sdp_media_add_attr(local_sdp->media[0], a);

	if ((*TipoGrsFlags & CORESIP_CALL_NO_TXRXMODE) == 0 && txrxmode_received != NULL)
	{
		//Se pone txrxmode en el sdp si se ha recibido del remoto.Es opcional y solo lo ponemos en este caso
		pj_str_t txrxmodedata;
		if (*TipoGrsFlags & CORESIP_CALL_RD_RXONLY)
		{
			txrxmodedata = pj_str("Rx");
		}
		else if (*TipoGrsFlags & CORESIP_CALL_RD_TXONLY)
		{
			txrxmodedata = pj_str("Tx");
		}
		else
		{
			if (txrxmode_received == NULL)
			{
				txrxmodedata = pj_str("TxRx");
			}
			else if (pj_strcmp(&pj_str("Rx"), &txrxmode_received->value) == 0)
			{
				txrxmodedata = pj_str("Rx");
			}
			else if (pj_strcmp(&pj_str("Tx"), &txrxmode_received->value) == 0)
			{
				txrxmodedata = pj_str("Tx");
			}
			else if (pj_strcmp(&pj_str("TxRx"), &txrxmode_received->value) == 0)
			{
				txrxmodedata = pj_str("TxRx");
			}
			else
			{
				txrxmodedata = pj_str("TxRx");
			}
		}

		a = pjmedia_sdp_attr_create(pool, "txrxmode", &txrxmodedata);
		pjmedia_sdp_media_add_attr(local_sdp->media[0], a);
	}
}

/*
Analiza el SDP en el caso de una llamada de radio cuando somos UAS
Retorna:
0 si no hay error o el codigo de la causa del tipo WG67 que se incluye en la cabecera Reason en el decline
*/
int SDPUtils::RadioRemoteSdpAnalysisAsUAS(pjsua_call_id call_id, pj_bool_t radio_grs, const pjmedia_sdp_session* rem_sdp, CORESIP_CallFlagsMask* TipoGrsFlags,
	CORESIP_CallType* newType, CORESIP_CallFlagsMask* newCallflags, pj_bool_t* ptt_id_enabled)
{
	CORESIP_CallType info_Type_prov = *newType;
	CORESIP_CallFlagsMask info_CallFlags_prov = *newCallflags;
	pj_bool_t ptt_id_enabled_prov = PJ_FALSE;
	pj_bool_t rxonly_type = PJ_FALSE;

	for (unsigned int i = 0; i < rem_sdp->media_count; i++)
	{
		pjmedia_sdp_attr* a;
		if ((a = pjmedia_sdp_media_find_attr2(rem_sdp->media[i], "type", NULL)) != NULL)
		{
			if (radio_grs == PJ_FALSE)
			{
				//Si no somos una radio GRS 
				return 2006;
			}

			if ((!pj_strcmp(&(a->value), &(pj_str("Radio")))) || (!pj_strcmp(&(a->value), &(pj_str("Radio-TxRx")))))
			{
				info_Type_prov = CORESIP_CALL_RD;
				info_CallFlags_prov = *TipoGrsFlags;
				ptt_id_enabled_prov = PJ_TRUE;
			}
			else if (!pj_strcmp(&(a->value), &(pj_str("Radio-Idle"))))
			{
				info_Type_prov = CORESIP_CALL_RD;
				info_CallFlags_prov = *TipoGrsFlags | CORESIP_CALL_RD_IDLE;
			}
			else if (!pj_strcmp(&(a->value), &(pj_str("Radio-Rxonly"))))
			{
				if ((*TipoGrsFlags & CORESIP_CALL_RD_RXONLY) != 0)
				{
					//Es un GRS configurado como receptor, por lo tanto se admite en cualquier caso
				}
				else if (((*TipoGrsFlags & CORESIP_CALL_RD_RXONLY) == 0) && ((*TipoGrsFlags & CORESIP_CALL_RD_TXONLY) == 0))
				{
					//Es un GRS configurado como transceptor, por lo tanto se admite en cualquier caso
				}
				else
				{
					//Si no es receptor ni esta declarado como un destino Radio-Rxonly entonces retornamos decline
					return 2006;
				}
				rxonly_type = PJ_TRUE;
				info_Type_prov = CORESIP_CALL_RD;
				info_CallFlags_prov = *TipoGrsFlags | CORESIP_CALL_RD_RADIO_RXONLY | CORESIP_CALL_RD_RXONLY;
			}
			else if (!pj_strcmp(&(a->value), &(pj_str("Coupling"))))
			{
				pjsua_call_id couplig_call_id = SipCall::CouplingSessionInAccount(call_id);
				if (couplig_call_id != PJSUA_INVALID_ID && couplig_call_id != call_id)
				{
					//Hay otra sesion distinta de esta que es coupling
					return 2005;
				}
				info_Type_prov = CORESIP_CALL_RD;
				info_CallFlags_prov = *TipoGrsFlags | CORESIP_CALL_RD_COUPLING;
				ptt_id_enabled_prov = PJ_TRUE;
			}
			else
			{
				return 2007;
			}
		}
		else if (radio_grs)
		{
			//Si somo un GRS el Type es obligatorio
			return 2007;
		}

		if ((a = pjmedia_sdp_media_find_attr2(rem_sdp->media[i], "txrxmode", NULL)) != NULL)
		{
			if (radio_grs == PJ_FALSE)
			{
				//Si no somos una radio GRS 
				return 2006;
			}

			if (!pj_strcmp(&(a->value), &(pj_str("Rx"))))
			{
				if (*TipoGrsFlags & CORESIP_CALL_RD_TXONLY)
				{
					return 2006;
				}
				else
				{
					info_CallFlags_prov |= CORESIP_CALL_RD_RXONLY;
				}
			}
			else if (!pj_strcmp(&(a->value), &(pj_str("Tx"))))
			{
				if (rxonly_type) return 2007;

				if (*TipoGrsFlags & CORESIP_CALL_RD_RXONLY)
				{
					return 2006;
				}
				else
				{
					info_CallFlags_prov |= CORESIP_CALL_RD_TXONLY;
				}
			}
			else if (!pj_strcmp(&(a->value), &(pj_str("TxRx"))))
			{
				if (*TipoGrsFlags & CORESIP_CALL_RD_RXONLY)
				{
					info_CallFlags_prov |= CORESIP_CALL_RD_RXONLY;
				}
				else if (*TipoGrsFlags & CORESIP_CALL_RD_TXONLY)
				{
					info_CallFlags_prov |= CORESIP_CALL_RD_TXONLY;
				}
			}
			else
			{
				return 2007;
			}
		}
		else
		{
			//Si no esta txrxmode entonces se trata como si hubiese llegado con el valor TxRx
			if (*TipoGrsFlags & CORESIP_CALL_RD_RXONLY)
			{
				info_CallFlags_prov |= CORESIP_CALL_RD_RXONLY;
			}
			else if (*TipoGrsFlags & CORESIP_CALL_RD_TXONLY)
			{
				info_CallFlags_prov |= CORESIP_CALL_RD_TXONLY;
			}
		}
	}

	if (radio_grs == PJ_TRUE)
	{
		if (info_Type_prov != CORESIP_CALL_RD)
		{
			//Somos un GRS y no hemos recibido ni type ni txrxmode
			return 2006;
		}
	}

	*newType = info_Type_prov;
	*newCallflags = info_CallFlags_prov;
	if (ptt_id_enabled != NULL) *ptt_id_enabled = ptt_id_enabled_prov;
	return 0;
}

/*
Analiza el SDP en el caso de una llamada de radio cuando somos UAC
0 si no hay error o el codigo de la causa del tipo WG67 que se incluye en la cabecera Reason en el decline
*/
int SDPUtils::RadioRemoteSdpAnalysisAsUAC(const pjmedia_sdp_session* rem_sdp, CORESIP_CallInfo* info, pj_bool_t* ptt_id_enabled, unsigned short* ptt_id_received_in_sdp)
{
	CORESIP_CallType info_Type_prov = info->Type;
	CORESIP_CallFlagsMask info_CallFlags_prov = info->CallFlags;
	pj_bool_t ptt_id_enabled_prov = PJ_FALSE;
	unsigned short ptt_id_received_in_sdp_prov = 0;

	for (unsigned int i = 0; i < rem_sdp->media_count; i++)
	{
		pjmedia_sdp_attr* a;
		if ((a = pjmedia_sdp_media_find_attr2(rem_sdp->media[i], "type", NULL)) != NULL)
		{
			if ((!pj_strcmp(&(a->value), &(pj_str("Radio")))) || (!pj_strcmp(&(a->value), &(pj_str("Radio-TxRx")))))
			{
				info_Type_prov = CORESIP_CALL_RD;
				ptt_id_enabled_prov = PJ_TRUE;
			}
			else if (!pj_strcmp(&(a->value), &(pj_str("Radio-Idle"))))
			{
				info_Type_prov = CORESIP_CALL_RD;
				info_CallFlags_prov |= CORESIP_CALL_RD_IDLE;
			}
			else if (!pj_strcmp(&(a->value), &(pj_str("Radio-Rxonly"))))
			{
				info_Type_prov = CORESIP_CALL_RD;
				info_CallFlags_prov |= CORESIP_CALL_RD_RADIO_RXONLY;
			}
			else if (!pj_strcmp(&(a->value), &(pj_str("Coupling"))))
			{
				info_Type_prov = CORESIP_CALL_RD;
				info_CallFlags_prov |= CORESIP_CALL_RD_COUPLING;
				ptt_id_enabled_prov = PJ_TRUE;
			}
			else
			{
				return 2007;
			}
		}

		if ((a = pjmedia_sdp_media_find_attr2(rem_sdp->media[i], "txrxmode", NULL)) != NULL)
		{
			if (!pj_strcmp(&(a->value), &(pj_str("Rx"))))
			{
				info_CallFlags_prov |= CORESIP_CALL_RD_RXONLY;
			}
			else if (!pj_strcmp(&(a->value), &(pj_str("Tx"))))
			{
				info_CallFlags_prov |= CORESIP_CALL_RD_TXONLY;
			}
			else if (!pj_strcmp(&(a->value), &(pj_str("TxRx"))))
			{				
			}
			else
			{
				return 2007;
			}
		}

		a = pjmedia_sdp_media_find_attr2(rem_sdp->media[i], "ptt-id", NULL);
		if (a != NULL)
		{
			if (ptt_id_enabled_prov == PJ_TRUE)
			{
				ptt_id_received_in_sdp_prov = (unsigned short)(pj_strtoul(&a->value));
			}
		}
	}

	info->Type = info_Type_prov;
	info->CallFlags = info_CallFlags_prov;
	if (ptt_id_enabled != NULL) *ptt_id_enabled = ptt_id_enabled_prov;
	if (ptt_id_received_in_sdp != NULL) *ptt_id_received_in_sdp = ptt_id_received_in_sdp_prov;

	return 0;
}

void SDPUtils::SetGRSBssMethod_from_SDP(pjsua_acc_id acc_id, pjsua_call_id call_id, const pjmedia_sdp_session* rem_sdp)
{
	if (rem_sdp == NULL) return;
	if (rem_sdp->media_count == 0) return;

	pj_bool_t rdAccount = PJ_FALSE;				//Indica si add_id es un account tipo radio GRS
	AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(acc_id);
	if (accUserData != NULL)
	{
		rdAccount = accUserData->rdAccount;
	}
	else return;

	SipCall* sipcall = (SipCall*)pjsua_call_get_user_data(call_id);
	if (sipcall == NULL) return;

	pj_bool_t radio_grs = (SipAgent::_Radio_UA != 0 || rdAccount == PJ_TRUE);
	if (!radio_grs) return;

	pj_bool_t remote_sdp_bss = PJ_FALSE;
	sipcall->etm_grs_bss_method_selected[0] = '\0';
	char preferred_grs_bss_method_found[CORESIP_MAX_BSS_LENGTH + 1];
	preferred_grs_bss_method_found[0] = '\0';

	for (unsigned int i = 0; i < rem_sdp->media_count; i++)
	{
		unsigned start = 0;
		unsigned index_found = 0;
		pjmedia_sdp_attr* a;
		do {
			if ((a = pjmedia_sdp_attr_find_next(rem_sdp->media[i]->attr_count, rem_sdp->media[i]->attr, &pj_str("bss"), NULL, start, &index_found)) != NULL)
			{
				remote_sdp_bss = PJ_TRUE;
				if (accUserData != NULL && pj_strcmp2(&a->value, accUserData->preferred_grs_bss_method) == 0)
				{
					pj_ansi_strncpy(preferred_grs_bss_method_found, accUserData->preferred_grs_bss_method, sizeof(preferred_grs_bss_method_found));
					break;
				}
			}
			start = index_found + 1;
		} while (a != NULL);
	}

	if (remote_sdp_bss)
	{
		pj_str_t bssmet;
		if (strlen(preferred_grs_bss_method_found) > 0) bssmet = pj_str(preferred_grs_bss_method_found);
		else
		{
			strcpy(preferred_grs_bss_method_found, "RSSI");
			bssmet = pj_str("RSSI");
		}
		strcpy(sipcall->etm_grs_bss_method_selected, preferred_grs_bss_method_found);
	}
}

/*Chequea R2S-KeepAlivePeriod y R2S-KeepAliveMultiplier. Si son correctos retorna 0 y los asigna a la llamada, si hay error retorna -1 */
int SDPUtils::CheckGRS_KeepAliveParams_from_SDP(pjsua_acc_id acc_id, const pjmedia_sdp_session* rem_sdp, unsigned int* newR2SKeepAlivePeriod, unsigned int* newR2SKeepAliveMultiplier)
{
	if (rem_sdp == NULL) return -1;
	if (rem_sdp->media_count == 0) return -1;

	pj_bool_t rdAccount = PJ_FALSE;				//Indica si add_id es un account tipo radio GRS
	AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(acc_id);
	if (accUserData != NULL)
	{
		rdAccount = accUserData->rdAccount;
	}
	else return -1;

	pj_bool_t radio_grs = (SipAgent::_Radio_UA != 0 || rdAccount == PJ_TRUE);
	if (!radio_grs) return -1;

	unsigned int sipcall_R2SKeepAlivePeriod = *newR2SKeepAlivePeriod;
	unsigned int sipcall_R2SKeepAliveMultiplier = *newR2SKeepAliveMultiplier;

	pjmedia_sdp_attr* a;
	for (unsigned int i = 0; i < rem_sdp->media_count; i++)
	{
		if ((a = pjmedia_sdp_media_find_attr2(rem_sdp->media[i], "R2S-KeepAlivePeriod", NULL)) != NULL)
		{
			sipcall_R2SKeepAlivePeriod = (unsigned int)pj_strtoul(&a->value);
			if (sipcall_R2SKeepAlivePeriod < 20 || sipcall_R2SKeepAlivePeriod > 1000)
			{
				return -1;
			}
		}
		else
		{
			sipcall_R2SKeepAlivePeriod = 200;
		}
		if ((a = pjmedia_sdp_media_find_attr2(rem_sdp->media[i], "R2S-KeepAliveMultiplier", NULL)) != NULL)
		{
			sipcall_R2SKeepAliveMultiplier = (unsigned int)pj_strtoul(&a->value);
			if (sipcall_R2SKeepAliveMultiplier < 2 || sipcall_R2SKeepAliveMultiplier > 50)
			{
				return -1;
			}
		}
		else
		{
			sipcall_R2SKeepAliveMultiplier = 10;
		}
	}

	*newR2SKeepAlivePeriod = sipcall_R2SKeepAlivePeriod;
	*newR2SKeepAliveMultiplier = sipcall_R2SKeepAliveMultiplier;

	return 0;
}

/*Busca el campo fid en el SDP y retorna 0 si lo encuentra y es igual al del GRS, 1 si no lo encuentra y -1 si lo encuentra y no es igual */
int SDPUtils::CheckGRS_fid_from_SDP(pjsua_acc_id acc_id, const pjmedia_sdp_session* rem_sdp)
{
	if (rem_sdp == NULL) return 1;
	if (rem_sdp->media_count == 0) return 1;

	pj_bool_t rdAccount = PJ_FALSE;				//Indica si add_id es un account tipo radio GRS
	AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(acc_id);
	if (accUserData != NULL)
	{
		rdAccount = accUserData->rdAccount;
	}
	else return 1;

	pj_bool_t radio_grs = (SipAgent::_Radio_UA != 0 || rdAccount == PJ_TRUE);
	if (!radio_grs) return 1;

	pjmedia_sdp_attr* a;
	for (unsigned int i = 0; i < rem_sdp->media_count; i++)
	{
		if ((a = pjmedia_sdp_media_find_attr2(rem_sdp->media[i], "fid", NULL)) != NULL)
		{
			if (strlen(accUserData->RdFr) == 0 || strcmp(accUserData->RdFr, "000.000") == 0)
			{
				return 0;
			}
			else if (pj_strcmp2(&a->value, accUserData->RdFr) == 0)
			{
				return 0;
			}
			else
			{
				return -1;
			}
		}
	}
	return 1;
}

int SDPUtils::NegotiateNoFreqDisconn(pjsua_call_id call_id, CORESIP_CallType callType, pj_bool_t radio_grs,
	const pjmedia_sdp_session* local_sdp, const pjmedia_sdp_session* rem_sdp, int* NoFreqDisconn)
{
	pj_bool_t NoFreqDisconn_found = PJ_FALSE;

	if (NoFreqDisconn == NULL) *NoFreqDisconn = 0;
	if (!radio_grs) return -1;
	if (callType != CORESIP_CALL_RD) return -1;
	if (rem_sdp == NULL || local_sdp == NULL) return -1;

	char ED137Radioversion_, ED137Phoneversion_;
	pjsip_endpt_Get_Ed137_version(pjsua_var.endpt, &ED137Radioversion_, &ED137Phoneversion_);
	if (ED137Radioversion_ != 'C') return -1;

	pjsua_call* call;
	pjsip_dialog* dlg;
	pj_status_t st;
	st = acquire_call("NegotiateNoFreqDisconn()", call_id, &call, &dlg);
	if (st != PJ_SUCCESS)
	{
		return -1;
	}

	for (unsigned int i = 0; i < rem_sdp->media_count; i++)
	{
		pjmedia_sdp_attr* a;
		if ((a = pjmedia_sdp_media_find_attr2(rem_sdp->media[i], "NoFreqDisconn", NULL)) != NULL)
		{
			NoFreqDisconn_found = PJ_TRUE;
			break;
		}
	}

	if (NoFreqDisconn_found)
	{
		if (NoFreqDisconn != NULL) *NoFreqDisconn = 1;
		pjmedia_sdp_attr* a = pjmedia_sdp_attr_create(call->inv->pool, "NoFreqDisconn", NULL);
		pjmedia_sdp_media_add_attr(local_sdp->media[0], a);
	}
	pjsip_dlg_dec_lock(dlg);
	return 0;
}







