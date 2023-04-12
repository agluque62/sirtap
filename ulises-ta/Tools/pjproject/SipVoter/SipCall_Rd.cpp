/*
	Este archivo define las funciones de la clase SipCall referentes a RADIO
*/

#include "Global.h"
#include "SipCall.h"
#include "Exceptions.h"
#include "SipAgent.h"
#include "AccountUserData.h"

const char SipCall::Selcal_designators[] = { 'A','B','C','D','E','F','G','H','J','K','L','M','P','Q','R','S',
		'T','U','V','W','X','Y','Z','1','2','3','4','5','6','7','8','9' };

void SipCall::Ptt(pjsua_call_id call_id, const CORESIP_PttInfo* info, unsigned int delay_ms)
{
	if (call_id<0 || call_id>(int)pjsua_var.ua_cfg.max_calls)
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("Ptt delayed:", "call_id %d no valido", call_id);
	}

	pjsua_call* call1;
	pjsip_dialog* dlg1;
	pj_status_t status = acquire_call("Ptt delayed()", call_id, &call1, &dlg1);
	PJ_CHECK_STATUS(status, ("ERROR adquiriendo call", "[Call=%d]", call_id));

	SipCall* sipcall = (SipCall*)call1->user_data;

	sipcall->PTT_delayed_timer.id = 0;
	pjsua_cancel_timer(&sipcall->PTT_delayed_timer);

	pj_memcpy(&sipcall->ptt_delayed_info, info, sizeof(CORESIP_PttInfo));

	sipcall->PTT_delayed_timer.cb = PTT_delayed_timer_cb;
	sipcall->PTT_delayed_timer.user_data = (void*)sipcall;
	pj_time_val	delay1;
	delay1.sec = (long)(delay_ms / 1000);
	delay1.msec = (long)(delay_ms % 1000);
	sipcall->PTT_delayed_timer.id = 1;
	pj_status_t st = pjsua_schedule_timer(&sipcall->PTT_delayed_timer, &delay1);
	if (st != PJ_SUCCESS)
	{
		sipcall->PTT_delayed_timer.id = 0;
		pjsip_dlg_dec_lock(dlg1);
		PJ_CHECK_STATUS(st, ("ERROR en PTT_delayed_timer"));
	}

	pjsip_dlg_dec_lock(dlg1);
}

void SipCall::PTT_delayed_timer_cb(pj_timer_heap_t* th, pj_timer_entry* te)
{
	SipCall* sipcall = (SipCall*)te->user_data;

	pjsua_cancel_timer(&sipcall->PTT_delayed_timer);
	sipcall->PTT_delayed_timer.id = 0;

	CORESIP_Error Error;
	CORESIP_Error* error = &Error;
	int ret = CORESIP_OK;

	try
	{
		Ptt(sipcall->_Id, &sipcall->ptt_delayed_info);
	}
	catch (PJLibException& ex)
	{
		ret = ex.SetError(error);
	}
	catch (...)
	{
		if (error)
		{
			error->Code = 1;
			pj_ansi_strcpy(error->File, __FILE__);
			pj_ansi_strcpy(error->Info, "Unknown exception");
		}
		ret = CORESIP_ERROR;
	}

	if (ret != CORESIP_OK)
	{
		PJ_LOG(3, (error->File, "ERROR SipCall::PTT_delayed_timer_cb: %s", error->Info));
	}
}

/**
*
*/
void SipCall::Ptt(pjsua_call_id call_id, const CORESIP_PttInfo* info)
{
	if (call_id<0 || call_id>(int)pjsua_var.ua_cfg.max_calls)
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("Ptt:", "call_id %d no valido", call_id);
	}

	pj_uint32_t rtp_ext_info = 0;
	pj_uint32_t rtp_ext_info_prev = 0;
	pj_uint8_t cld = 0;

	pjsua_call* call1;
	pjsip_dialog* dlg1;
	pj_status_t status = acquire_call("Ptt()", call_id, &call1, &dlg1);
	PJ_CHECK_STATUS(status, ("ERROR adquiriendo call", "[Call=%d]", call_id));

	SipCall* call = (SipCall*)call1->user_data;

	if (call)
	{
		pjsua_cancel_timer(&call->PTT_delayed_timer);
		call->PTT_delayed_timer.id = 0;
	}

	pjsua_call_info call_info;
	if (pjsua_call_get_info(call_id, &call_info) != PJ_SUCCESS)
	{
		pjsip_dlg_dec_lock(dlg1);
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("Ptt:", "ERROR: No se puede obtener call_info. call_id %d", call_id);
		return;
	}

	pjmedia_session* session = pjsua_call_get_media_session(call_id);
	if (session == NULL)
	{
		pjsip_dlg_dec_lock(dlg1);
		return;
	}

	pjmedia_stream* stream = NULL;
	stream = pjmedia_session_get_stream(session, 0);
	if (stream == NULL)
	{
		pjsip_dlg_dec_lock(dlg1);
		return;
	}
	pjmedia_stream_get_rtp_ext_tx_info(stream, &rtp_ext_info_prev);

	pj_bool_t rdAccount = PJ_FALSE;				//Indica si acc_id es un account tipo radio GRS
	AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(call_info.acc_id);
	if (accUserData != NULL)
	{
		rdAccount = accUserData->rdAccount;
	}

	rtp_ext_info = rtp_ext_info_prev;

	unsigned int PttType_prev = (unsigned int)PJMEDIA_RTP_RD_EX_GET_PTT_TYPE(rtp_ext_info_prev);
	unsigned int PttMute_prev = (unsigned int)PJMEDIA_RTP_RD_EX_GET_PM(rtp_ext_info_prev);
	unsigned int PttMute = info->PttMute ? 1 : 0;

	if (SipAgent::_Radio_UA || rdAccount)
	{
		//Si soy un GRS no toco el PTT ni PTT mute. Ya se hace el eco del PTT en OnRdInfoChanged

		if (info->Squ)
		{
			//PTT-ID 
			if (call)
			{

				if (call->_Info.CallFlags & CORESIP_CALL_RD_TXONLY)
				{
					pjsip_dlg_dec_lock(dlg1);
					throw PJLibException(__FILE__, PJ_EINVAL).Msg("Ptt:", "ERROR: No se puede activar SQU en un transmisor. call_id %d", call_id);
					return;
				}


				if (call->last_received_ptt_status)
				{
					if (call->radio_session_with_pttid_enabled)
					{
						//Si se esta recibiendo PTT en OnRdInfoChanged, ya se esta retornando el PTT-id recibido
						if (stream != NULL && pjmedia_stream_get_GRS_force_ptt_in_RTPRx_confirmed(stream))
						{
							//Si el GRS esta con el ptt mute forzado
							PJMEDIA_RTP_RD_EX_SET_PTT_ID(rtp_ext_info, 0);
							if (stream != NULL) pjmedia_stream_set_ptt_id(stream, 0);
						}
						else
						{
							//Aqui no hacemos nada
						}
					}
					else
					{
						PJMEDIA_RTP_RD_EX_SET_PTT_ID(rtp_ext_info, 0);
						if (stream != NULL) pjmedia_stream_set_ptt_id(stream, 0);
					}
				}
				else
				{
					PJMEDIA_RTP_RD_EX_SET_PTT_ID(rtp_ext_info, 0);
					if (stream != NULL) pjmedia_stream_set_ptt_id(stream, 0);
				}
			}
			else
			{
				PJMEDIA_RTP_RD_EX_SET_PTT_ID(rtp_ext_info, 0);
				if (stream != NULL) pjmedia_stream_set_ptt_id(stream, 0);
			}
		}
		else
		{
			if (call)
			{
				if (call->last_received_ptt_status)
				{
					if (call->radio_session_with_pttid_enabled)
					{
						//Si se esta recibiendo PTT en OnRdInfoChanged, ya se esta retornando el PTT-id recibido
						if (stream != NULL && pjmedia_stream_get_GRS_force_ptt_in_RTPRx_confirmed(stream))
						{
							//Si el GRS esta con el ptt mute forzado
							PJMEDIA_RTP_RD_EX_SET_PTT_ID(rtp_ext_info, 0);
							if (stream != NULL) pjmedia_stream_set_ptt_id(stream, call->assigned_pttid);
						}
						//Aqui no hacemos nada
					}
					else
					{
						PJMEDIA_RTP_RD_EX_SET_PTT_ID(rtp_ext_info, 0);
						if (stream != NULL) pjmedia_stream_set_ptt_id(stream, 0);
					}
				}
				else
				{
					PJMEDIA_RTP_RD_EX_SET_PTT_ID(rtp_ext_info, 0);
					if (stream != NULL) pjmedia_stream_set_ptt_id(stream, call->assigned_pttid);
				}
			}
			else
			{
				PJMEDIA_RTP_RD_EX_SET_PTT_ID(rtp_ext_info, 0);
				if (stream != NULL) pjmedia_stream_set_ptt_id(stream, 0);
			}
		}

		PJMEDIA_RTP_RD_EX_SET_SQU(rtp_ext_info, info->Squ);
	}
	else
	{
		if (call)
		{
			if (call->ED137Version == 'B' && info->PttType == CORESIP_PTT_TEST)
			{
				pjsip_dlg_dec_lock(dlg1);
				PJ_CHECK_STATUS(PJ_EINVALIDOP, ("ERROR Ptt. Test PTT is not supported by ED137B", "[Call=%d]", call_id));
				return;
			}

			if (call->_Info.CallFlags & CORESIP_CALL_RD_RADIO_RXONLY)
			{
				//Si la sesion es del tipo Radio-Rxonly entonces ignoramos la orden de PTT
				pjsip_dlg_dec_lock(dlg1);
				return;
			}
		}

		PJMEDIA_RTP_RD_EX_SET_PTT_TYPE(rtp_ext_info, info->PttType);
		if (info->PttType != CORESIP_PTT_OFF)
		{
			PJMEDIA_RTP_RD_EX_SET_PTT_ID(rtp_ext_info, info->PttId);
			if (stream != NULL) pjmedia_stream_set_ptt_id(stream, info->PttId);
		}
		else if (call)
		{
			PJMEDIA_RTP_RD_EX_SET_PTT_ID(rtp_ext_info, call->assigned_pttid);
			if (stream != NULL) pjmedia_stream_set_ptt_id(stream, call->assigned_pttid);
		}
		else
		{
			PJMEDIA_RTP_RD_EX_SET_PTT_ID(rtp_ext_info, 0);
			if (stream != NULL) pjmedia_stream_set_ptt_id(stream, 0);
		}
		PJMEDIA_RTP_RD_EX_SET_SQU(rtp_ext_info, info->Squ);

		PJMEDIA_RTP_RD_EX_SET_PM(rtp_ext_info, PttMute);
	}

	if (info->PttType == CORESIP_PTT_OFF && PttMute)
	{
		pjsip_dlg_dec_lock(dlg1);
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("Ptt:", "ERROR: Ptt Mute no puede activarse con PTT OFF. call_id %d", call_id);
		PttMute = 0;
		return;
	}

	if (SipAgent::_Radio_UA || rdAccount)
	{
		//Es un agente radio o es un account del tipo radio GRS

		if (call)
		{
			if (strlen(call->etm_grs_bss_method_selected) > 0)
			{
				if (info->Squ)
				{
					if (info->Qidx > CORESIP_MAX_QIDX)
					{
						pjsip_dlg_dec_lock(dlg1);
						PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR SipCall::Ptt Qidx es demasiado grande", "[valor maximo=%d]", CORESIP_MAX_QIDX));
						return;
					}

					//Es simulador de radio y tenemos que enviar el qidx con el squelch
					PJMEDIA_RTP_RD_EX_SET_X(rtp_ext_info, 1);
					PJMEDIA_RTP_RD_EX_SET_TYPE(rtp_ext_info, 1);
					PJMEDIA_RTP_RD_EX_SET_LENGTH(rtp_ext_info, 1);
					pj_uint8_t val = ((pj_uint8_t)info->Qidx) << ((pj_uint8_t)3);

					if (strcmp(call->etm_grs_bss_method_selected, "RSSI") == 0)
					{
						val |= 0x0;
					}
					else if (strcmp(call->etm_grs_bss_method_selected, "AGC") == 0)
					{
						val |= 0x1;
					}
					else if (strcmp(call->etm_grs_bss_method_selected, "C/N") == 0)
					{
						val |= 0x2;
					}
					else if (strcmp(call->etm_grs_bss_method_selected, "PSD") == 0)
					{
						val |= 0x3;
					}
					else if (accUserData != NULL)
					{
						if (accUserData->preferred_grs_bss_method_code >= 4 && accUserData->preferred_grs_bss_method_code <= 7)
						{
							val |= (pj_uint8_t)accUserData->preferred_grs_bss_method_code;
						}
						else
						{
							val |= (pj_uint8_t)4;
						}
					}

					PJMEDIA_RTP_RD_EX_SET_VALUE(rtp_ext_info, (val));

#ifdef CHECK_QIDX_LOGARITHM
					//Si es un agente tipo radio simulada conectamos el puerto de la pjmedia que genera un tono con la llamada para que envie audio cuando hay squelch
					//Conectamos el tono a la llamada
					pjsua_conf_port_id call_port_id = pjsua_call_get_conf_port(call_id);
					if (call_port_id != PJSUA_INVALID_ID && SipAgent::_Tone_Port_Id != PJSUA_INVALID_ID)
					{
						pjsua_conf_connect(SipAgent::_Tone_Port_Id, call_port_id);
					}
#endif
				}
				else
				{
					//Es simulador de radio y al quitar el squelch dejamos de enviar el qidx
					PJMEDIA_RTP_RD_EX_SET_X(rtp_ext_info, 0);
					PJMEDIA_RTP_RD_EX_SET_TYPE(rtp_ext_info, 0);
					PJMEDIA_RTP_RD_EX_SET_LENGTH(rtp_ext_info, 0);
					PJMEDIA_RTP_RD_EX_SET_VALUE(rtp_ext_info, (0));

#ifdef CHECK_QIDX_LOGARITHM
					//Si es un agente tipo radio simulada conectamos el puerto de la pjmedia que genera un tono con la llamada para que envie audio cuando hay squelch
					//Conectamos el tono a la llamada
					pjsua_conf_port_id call_port_id = pjsua_call_get_conf_port(call_id);
					if (call_port_id != PJSUA_INVALID_ID && SipAgent::_Tone_Port_Id != PJSUA_INVALID_ID)
					{
						pjsua_conf_disconnect(SipAgent::_Tone_Port_Id, call_port_id);
					}
#endif

				}
			}
		}
	}

	if (SipAgent::_Radio_UA || rdAccount)
	{
	}
	else if (call && info->PttType != PttType_prev)
	{
		//Hay un cambio en el estado del PTT. Si es a OFF arrancamos un timer durante el cual no se
		//envian a la aplicacion infos de la radio. Al final del timer se envia la info actual.
		//Si el cambio es a ON entonces y el timer está en marcha se fuerza la llamada de la callback del timer para actualizar la info

		pjsua_cancel_timer(&call->Ptt_off_timer);

		call->Ptt_off_timer.cb = Ptt_off_timer_cb;
		call->Ptt_off_timer.user_data = (void*)call;
		pj_time_val	delay1;
		delay1.sec = (long)0;

		if (info->PttType == CORESIP_PTT_OFF)
		{
			//Arranca el timer para no enviar estados a la aplicacion				
			PJ_LOG(5, (__FILE__, "SipCall::Ptt Arranca timer PTT OFF. dst %s", call->DstUri));
			delay1.msec = (long)SipAgent::_TimeToDiscardRdInfo;
			call->Ptt_off_timer.id = 1;
			pj_status_t st = pjsua_schedule_timer(&call->Ptt_off_timer, &delay1);
			if (st != PJ_SUCCESS)
			{
				call->Ptt_off_timer.id = 0;
				pjsip_dlg_dec_lock(dlg1);
				PJ_CHECK_STATUS(st, ("ERROR en Ptt_off_timer"));
			}
		}
		else if (call->Ptt_off_timer.id != 0)
		{
			//Se fuerza la llamada a la callback para que se actualice el estado de la info en la aplicacion
			//Para ello cancelamos el timer y lo reanudamos con un delay a cero
			PJ_LOG(5, (__FILE__, "SipCall::Ptt Arranca timer PTT OFF. dst %s con delay a 0 para forzar la callback", call->DstUri));

			delay1.msec = (long)0;
			pj_status_t st = pjsua_schedule_timer(&call->Ptt_off_timer, &delay1);
			if (st != PJ_SUCCESS)
			{
				call->Ptt_off_timer.id = 0;
				pjsip_dlg_dec_lock(dlg1);
				PJ_CHECK_STATUS(st, ("ERROR en Ptt_off_timer"));
			}
		}
	}

	if (stream != NULL)
	{
		pjmedia_stream_set_rtp_ext_tx_info(stream, rtp_ext_info);
		call->last_rtp_ext_info = rtp_ext_info;

		pj_bool_t forzar_KA = PJ_FALSE;

		if (SipAgent::_Radio_UA || rdAccount)
		{
			//Es un agente radio o es un account del tipo radio GRS			
		}
		else
		{
			if (PttMute_prev != PttMute)
			{
				if ((PttType_prev != 0) && (info->PttType != CORESIP_PTT_OFF) && (PttMute == 0))
				{
					//Si no hay cambio el estado de PTT, es PTT ON, y se desactiva el PTT mute
					//Entonces no se envia el KA y el Ptt mute a cero ira en el audio
				}
				else
				{
					//Forzamos el envio de un keep alive si el estado de Ptt Mute cambia en cualquier otro caso
					forzar_KA = PJ_TRUE;
				}
			}

			/*Si la sesion es hacia un receptor entonces se fuerza un KA cuando hay cambio de PTT*/
			if (call)
			{
				CORESIP_CallFlagsMask flags = call->_Info.CallFlags;
				if ((flags & CORESIP_CALL_RD_RXONLY) && (info->PttType != PttType_prev))
				{
					forzar_KA = PJ_TRUE;
				}
			}
		}

		if (forzar_KA) pjmedia_stream_force_send_KA_packet(stream);
	}

	pjsip_dlg_dec_lock(dlg1);
}

/**
*	GRS_Force_Ptt_Mute. Como GRS Fuerza PTT mute en R2S Keepalives hacia VCS. Sirve para simular un PTT mute de otra sesion inventada.
*	@param	call_id		Identificador de la llamada/sesion SIP
*	@param	PttType		Tipo de PTT. PTT que activa el Ptt mute
*	@param	PttId		Ptt ID. PTT id del ptt que activa el mute.
*	@param	on			on. Si true lo activa, si false lo desactiva y los keepalives son los normales.
*						En caso de false se ignoran los parametros anteriores.
*/
void SipCall::GRS_Force_Ptt_Mute(int call_id, CORESIP_PttType PttType, unsigned short PttId, bool on)
{
	if (call_id<0 || call_id>(int)pjsua_var.ua_cfg.max_calls)
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("GRS_Force_Ptt_Mute:", "call_id %d no valido", call_id);
	}

	if (PttType < CORESIP_PTT_OFF || PttType > CORESIP_PTT_TEST)
	{
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR GRS_Force_Ptt_Mute PttType no valido"));
	}

	if (PttId < 0 || PttId > 63)
	{
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR GRS_Force_Ptt_Mute PttId no valido"));
	}

	pjsua_call* call1;
	pjsip_dialog* dlg1;
	pj_status_t status = acquire_call("GRS_Force_Ptt_Mute()", call_id, &call1, &dlg1);
	PJ_CHECK_STATUS(status, ("ERROR adquiriendo call", "[Call=%d]", call_id));

	pjsua_call_info call_info;
	if (pjsua_call_get_info(call_id, &call_info) != PJ_SUCCESS)
	{
		pjsip_dlg_dec_lock(dlg1);
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("GRS_Force_Ptt_Mute:", "ERROR: No se puede obtener call_info. call_id %d", call_id);
		return;
	}

	pjmedia_session* session = pjsua_call_get_media_session(call_id);
	if (session == NULL)
	{
		pjsip_dlg_dec_lock(dlg1);
		return;
	}

	pjmedia_stream* stream = NULL;
	stream = pjmedia_session_get_stream(session, 0);
	if (session == NULL)
	{
		pjsip_dlg_dec_lock(dlg1);
		return;
	}

	pj_bool_t rdAccount = PJ_FALSE;				//Indica si acc_id es un account tipo radio GRS
	AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(call_info.acc_id);
	if (accUserData != NULL)
	{
		rdAccount = accUserData->rdAccount;
	}

	if ((SipAgent::_Radio_UA == 0 && rdAccount == PJ_FALSE) || (accUserData->TipoGrsFlags & CORESIP_CALL_RD_RXONLY))
	{
		//No soy GRS
		pjsip_dlg_dec_lock(dlg1);
		PJ_CHECK_STATUS(PJ_EINVALIDOP, ("ERROR Debe ser GRS Tx/TxRx", "[Call=%d]", call_id));
		return;
	}

	if (on)
	{
		pjmedia_stream_GRS_force_ptt_mute_in_RTPRx(stream, (pj_uint32_t)PttType, (pj_uint32_t)PttId, PJ_TRUE);
	}
	else
	{
		pjmedia_stream_GRS_force_ptt_mute_in_RTPRx(stream, (pj_uint32_t)0, (pj_uint32_t)0, PJ_FALSE);
	}

	pjsip_dlg_dec_lock(dlg1);
}

/**
*	GRS_Force_Ptt. Como GRS Fuerza PTT en R2S Keepalives hacia VCS. Sirve para simular un PTT mute de otra sesion inventada.
*	@param	call_id		Identificador de la llamada/sesion SIP
*	@param	PttType		Tipo de PTT. PTT que activa el Ptt mute
*	@param	PttId		Ptt ID. PTT id del ptt que activa el mute.
*/
void SipCall::GRS_Force_Ptt(int call_id, CORESIP_PttType PttType, unsigned short PttId)
{
	if (call_id<0 || call_id>(int)pjsua_var.ua_cfg.max_calls)
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("GRS_Force_Ptt:", "call_id %d no valido", call_id);
	}

	if (PttType < CORESIP_PTT_OFF || PttType > CORESIP_PTT_TEST)
	{
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR GRS_Force_Ptt PttType no valido"));
	}

	if (PttId < 0 || PttId > 63)
	{
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR GRS_Force_Ptt PttId no valido"));
	}

	pjsua_call* call1;
	pjsip_dialog* dlg1;
	pj_status_t status = acquire_call("GRS_Force_Ptt()", call_id, &call1, &dlg1);
	PJ_CHECK_STATUS(status, ("ERROR adquiriendo call", "[Call=%d]", call_id));

	pjsua_call_info call_info;
	if (pjsua_call_get_info(call_id, &call_info) != PJ_SUCCESS)
	{
		pjsip_dlg_dec_lock(dlg1);
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("GRS_Force_Ptt_Mute:", "ERROR: No se puede obtener call_info. call_id %d", call_id);
		return;
	}

	pjmedia_session* session = pjsua_call_get_media_session(call_id);
	if (session == NULL)
	{
		pjsip_dlg_dec_lock(dlg1);
		return;
	}

	pjmedia_stream* stream = NULL;
	stream = pjmedia_session_get_stream(session, 0);
	if (session == NULL)
	{
		pjsip_dlg_dec_lock(dlg1);
		return;
	}

	pj_bool_t rdAccount = PJ_FALSE;				//Indica si acc_id es un account tipo radio GRS
	AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(call_info.acc_id);
	if (accUserData != NULL)
	{
		rdAccount = accUserData->rdAccount;
	}

	if ((SipAgent::_Radio_UA == 0 && rdAccount == PJ_FALSE) || (accUserData->TipoGrsFlags & CORESIP_CALL_RD_RXONLY))
	{
		//No soy GRS
		pjsip_dlg_dec_lock(dlg1);
		PJ_CHECK_STATUS(PJ_EINVALIDOP, ("ERROR Debe ser GRS Tx/TxRx", "[Call=%d]", call_id));
		return;
	}

	pjmedia_stream_GRS_force_ptt_in_RTPRx(stream, (pj_uint32_t)PttType, (pj_uint32_t)PttId);

	pjsip_dlg_dec_lock(dlg1);
}

/**
*	GRS_Force_SCT. Como GRS Fuerza el bit SCT en el RTPRx enviado desde un GRS
*	@param	call		Identificador de la llamada/sesion SIP
*	@param	on			on. Si true lo activa, si false lo desactiva.
*/
void SipCall::GRS_Force_SCT(int call_id, bool on)
{
	if (call_id<0 || call_id>(int)pjsua_var.ua_cfg.max_calls)
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("GRS_Force_SCT:", "call_id %d no valido", call_id);
	}

	pjsua_call* call1;
	pjsip_dialog* dlg1;
	pj_status_t status = acquire_call("GRS_Force_SCT()", call_id, &call1, &dlg1);
	PJ_CHECK_STATUS(status, ("ERROR adquiriendo call", "[Call=%d]", call_id));

	pjsua_call_info call_info;
	if (pjsua_call_get_info(call_id, &call_info) != PJ_SUCCESS)
	{
		pjsip_dlg_dec_lock(dlg1);
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("GRS_Force_SCT:", "ERROR: No se puede obtener call_info. call_id %d", call_id);
		return;
	}

	pjmedia_session* session = pjsua_call_get_media_session(call_id);
	if (session == NULL)
	{
		pjsip_dlg_dec_lock(dlg1);
		return;
	}

	pjmedia_stream* stream = NULL;
	stream = pjmedia_session_get_stream(session, 0);
	if (session == NULL)
	{
		pjsip_dlg_dec_lock(dlg1);
		return;
	}

	pj_bool_t rdAccount = PJ_FALSE;				//Indica si acc_id es un account tipo radio GRS
	AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(call_info.acc_id);
	if (accUserData != NULL)
	{
		rdAccount = accUserData->rdAccount;
	}

	if (SipAgent::_Radio_UA == 0 && rdAccount == PJ_FALSE)
	{
		//No soy GRS
		pjsip_dlg_dec_lock(dlg1);
		PJ_CHECK_STATUS(PJ_EINVALIDOP, ("ERROR Debe ser GRS Tx/TxRx", "[Call=%d]", call_id));
		return;
	}

	if (on) pjmedia_stream_GRS_force_SCT_in_RTPRx(stream, PJ_TRUE);
	else pjmedia_stream_GRS_force_SCT_in_RTPRx(stream, PJ_FALSE);

	pjsip_dlg_dec_lock(dlg1);
}

/**
*	Force_PTTS. Fuerza el bit PTTS en el RTPRx o RTPTx
*	@param	call		Identificador de la llamada/sesion SIP
*	@param	on			on. Si true lo activa, si false lo desactiva.
*/
void SipCall::Force_PTTS(int call_id, bool on)
{
	if (call_id<0 || call_id>(int)pjsua_var.ua_cfg.max_calls)
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("Force_PTTS:", "call_id %d no valido", call_id);
	}

	pjsua_call* call1;
	pjsip_dialog* dlg1;
	pj_status_t status = acquire_call("Force_PTTS()", call_id, &call1, &dlg1);
	PJ_CHECK_STATUS(status, ("ERROR adquiriendo call", "[Call=%d]", call_id));

	pjsua_call_info call_info;
	if (pjsua_call_get_info(call_id, &call_info) != PJ_SUCCESS)
	{
		pjsip_dlg_dec_lock(dlg1);
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("Force_PTTS:", "ERROR: No se puede obtener call_info. call_id %d", call_id);
		return;
	}

	pjmedia_session* session = pjsua_call_get_media_session(call_id);
	if (session == NULL)
	{
		pjsip_dlg_dec_lock(dlg1);
		return;
	}

	pjmedia_stream* stream = NULL;
	stream = pjmedia_session_get_stream(session, 0);
	if (session == NULL)
	{
		pjsip_dlg_dec_lock(dlg1);
		return;
	}

	if (on) pjmedia_stream_force_PTTS_in_RTPTxRx(stream, PJ_TRUE);
	else pjmedia_stream_force_PTTS_in_RTPTxRx(stream, PJ_FALSE);

	pjsip_dlg_dec_lock(dlg1);
}

/**
* Replica el Audio recibido en el Grupo Multicast asociado a la frecuencia.
* Se reciben paquetes de 10ms
*/
void SipCall::OnRdRtp(void* stream, void* frame, void* codec, unsigned seq, pj_uint32_t rtp_ext_info)
{
	void* session = pjmedia_stream_get_user_data((pjmedia_stream*)stream);
	if (!session) return;
	pjsua_call* pjcall = (pjsua_call*)pjmedia_session_get_user_data((pjmedia_session*)session);
	if (!pjcall) return;

	SipCall* sipCall = (SipCall*)pjcall->user_data;
	if (!sipCall) return;

	pjsua_call_info callinfo;
	if (pjsua_call_get_info(pjcall->index, &callinfo) != PJ_SUCCESS)
	{
		return;
	}

	pjmedia_frame* frame_in = (pjmedia_frame*)frame;

	pj_bool_t rdAccount = PJ_FALSE;				//Indica si acc_id es un account tipo radio GRS
	AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(callinfo.acc_id);
	if (accUserData != NULL)
	{
		rdAccount = accUserData->rdAccount;
	}

	if (frame)
	{
		if (frame_in->size == 80)
		{
			if (sipCall->waited_rtp_seq != seq)
			{
				if (SipAgent::_Radio_UA || rdAccount)
				{
					if (!sipCall->primer_paquete_despues_ptt)
					{
						pjsua_acc_info accinfo;
						pjsua_acc_get_info(pjcall->acc_id, &accinfo);
						char local_uri[128];
						strncpy(local_uri, accinfo.acc_uri.ptr, sizeof(local_uri));
						local_uri[sizeof(local_uri) - 1] = '\0';
						if (accinfo.acc_uri.slen < sizeof(local_uri)) local_uri[accinfo.acc_uri.slen] = '\0';
						PJ_LOG(3, (__FILE__, "WARNING: RTP PERDIDO seq esperado %u recibido %u rd %s",
							sipCall->waited_rtp_seq, seq, local_uri));
					}
					else
					{
						sipCall->primer_paquete_despues_ptt = PJ_FALSE;
					}
				}
				else
				{
					if (!sipCall->primer_paquete_despues_squelch)
					{
						PJ_LOG(3, (__FILE__, "WARNING: RTP PERDIDO seq esperado %u recibido %u rd %s",
							sipCall->waited_rtp_seq, seq, sipCall->DstUri));
					}
					else
					{
						sipCall->primer_paquete_despues_squelch = PJ_FALSE;
					}
				}
			}

			if (seq == MAX_RTP_SEQ)
			{
				sipCall->waited_rtp_seq = 0;
			}
			else
			{
				sipCall->waited_rtp_seq = seq + 1;
			}
		}
	}

	if ((sipCall->_RdSendSock != PJ_INVALID_SOCKET) && (sipCall->Wait_init_timer.id == 0) &&
		(sipCall->_Info.Type == CORESIP_CALL_RD) && ((sipCall->_Info.CallFlags & CORESIP_CALL_RD_TXONLY) == 0))
	{
		//assert(sipCall->_Info.Type == CORESIP_CALL_RD);
		//assert((sipCall->_Info.CallFlags & CORESIP_CALL_RD_TXONLY) == 0);

		pj_ssize_t size = 0;
		char buf[1500];

		if (frame)
		{
			pjmedia_codec* cdc = (pjmedia_codec*)codec;
			pjmedia_frame frame_out;

			frame_out.buf = buf;
			frame_out.size = sizeof(buf);

			pj_status_t st = cdc->op->decode(cdc, frame_in, (unsigned)frame_out.size, PJ_FALSE, &frame_out);
			if ((st == PJ_SUCCESS) && (frame_out.size == (SAMPLES_PER_FRAME / 2) * sizeof(pj_int16_t)))
			{
				int x = PJMEDIA_RTP_RD_EX_GET_X(rtp_ext_info);
				pj_uint32_t type = PJMEDIA_RTP_RD_EX_GET_TYPE(rtp_ext_info);
				pj_uint32_t length = PJMEDIA_RTP_RD_EX_GET_LENGTH(rtp_ext_info);

				if (sipCall->squ_event_mcast)
				{
					//Es el primer paquete que se recibe despu�s de que el primer squelch del grupo se ha activado
					//Se inicializa el n�mero de secuencia de los paquetes del grupo que se env�a por multicast					
					SipAgent::_FrecDesp->Set_mcast_seq(sipCall->_Index_group, seq);
				}

				if (sipCall->last_received_squ_status == SQU_ON)
				{
					if (sipCall->bss_method_type != NINGUNO)
					{
						//Se calcula el Qidx

						SipCall *sipcall_in_window_timer = SipAgent::_FrecDesp->GetInWindow(sipCall->_Index_group);
						pj_bool_t in_window = sipcall_in_window_timer != NULL? PJ_TRUE : PJ_FALSE;

						pj_uint32_t porcRSSI = (pj_uint32_t)sipCall->_Info.porcentajeRSSI;
						pj_uint32_t centralized_qidx_value = 0;  //Valor centralizado, calculado internamente, del qidx
						pj_uint32_t external_qidx_value = 0;	 //Valor de qidx recibido por rtp
						pj_uint32_t final_qidx_value = 0;		 //Valor del qidx resultante

						pj_uint8_t qidx_value_rtp = 0;
						pj_uint8_t qidx_method_rtp = 0;
						pj_bool_t qidx_received_by_rtp = PJ_FALSE;
						if (x && type == 0x1 && length == 1)
						{
							//Si recibimos el QIDX por el rtp lo tomamos.

							qidx_value_rtp = (pj_uint8_t)PJMEDIA_RTP_RD_EX_GET_VALUE(rtp_ext_info);

							qidx_method_rtp = qidx_value_rtp;
							qidx_method_rtp &= 0x7;
							qidx_value_rtp >>= 3;
							qidx_value_rtp &= 0x1F;
							qidx_received_by_rtp = PJ_TRUE;
						}

						pj_bool_t calculado_internamente = PJ_FALSE;
						pj_bool_t recibido_externamente = PJ_FALSE;
						if (qidx_received_by_rtp && (sipCall->bss_method_type == RSSI_NUC) && (qidx_method_rtp == NUC_METHOD))
						{
							//Hemos recibido el qidx por el metodo nucleo. Eso es porque se lo hemos solicitado asi
							//a la pasarela porque no estamos en modo centralizado
							if (in_window || SipAgent::Coresip_Local_Config._Debug_BSS)
							{
								external_qidx_value = (pj_uint32_t)qidx_value_rtp;		//Esta en escala 0-31.	
								recibido_externamente = PJ_TRUE;
							}
						}
						else if (qidx_received_by_rtp && (sipCall->bss_method_type == RSSI || sipCall->bss_method_type == RSSI_NUC) && (qidx_method_rtp == RSSI_METHOD))
						{
							//Hemos recibido el qidx de la radio y asi se lo habiamos solicitado, y el metodo no es centralizado
							if (in_window || SipAgent::Coresip_Local_Config._Debug_BSS)
							{
								//En este caso donde el peso del qidx centralizado (interno) es mayor que el minimo del de RSSI, hay que tener en cuenta el valor externo recibido por rtp del qidx
								external_qidx_value = (pj_uint32_t)qidx_value_rtp;		//Esta en escala 0-15.	
								external_qidx_value = (external_qidx_value * MAX_QIDX_ESCALE) / MAX_RSSI_ESCALE;  //Escala de 0-31
								recibido_externamente = PJ_TRUE;
							}
						}
#ifdef _ULISES_
						else if (sipCall->bss_method_type == CENTRAL)
						{
							if (qidx_received_by_rtp && (qidx_method_rtp == RSSI_METHOD))
							{
								//Hemos recibido el qidx de la radio y asi se lo habiamos solicitado, y el metodo es centralizado
								//Con lo cual, cuenta el rssi recibido de la radio.
								if (in_window || SipAgent::Coresip_Local_Config._Debug_BSS)
								{
									//En este caso donde el peso del qidx centralizado (interno) es mayor que el minimo del de RSSI, hay que tener en cuenta el valor externo recibido por rtp del qidx
									external_qidx_value = (pj_uint32_t)qidx_value_rtp;		//Esta en escala 0-15.	
									external_qidx_value = (external_qidx_value * MAX_QIDX_ESCALE) / MAX_RSSI_ESCALE;  //Escala de 0-31
									recibido_externamente = PJ_TRUE;
								}
							}

							if (porcRSSI < MAX_porcentajeRSSI)
							{
								//En este caso donde el peso del qidx centralizado (interno) es menor que el maximo del de RSSI, hay que calcular internamente el qidx.
								if (in_window || SipAgent::Coresip_Local_Config._Debug_BSS)
								{
									//Si estamos en la ventana de decision calculamos el qidx. 

									//Se procesa el paquete de voz para el calculo del Qidx
									pj_int16_t* ibuf = (pj_int16_t*)buf;
									for (unsigned int i = 0; i < frame_out.size / 2; i++)
									{
										sipCall->fPdataQidx[i] = (float)(ibuf[i]);
									}

									iir(sipCall->fPdataQidx, sipCall->afMuestrasIfRx, sipCall->b_dc, sipCall->a_dc, &sipCall->fFiltroDC_IfRx_ciX, &sipCall->fFiltroDC_IfRx_ciY, 1, frame_out.size / 2);	//Filtro para eliminar la continua							

									process(&sipCall->PdataQidx, sipCall->afMuestrasIfRx, frame_out.size / 2);

									centralized_qidx_value = (pj_uint32_t)quality_indicator(&sipCall->PdataQidx);				//Escala 0-50				
									centralized_qidx_value = (centralized_qidx_value * MAX_QIDX_ESCALE) / MAX_CENTRAL_ESCALE;		//Lo transformamos a escala 0- 15 (RSSI)																
									calculado_internamente = PJ_TRUE;

								}
							}
						}
#endif
						if (calculado_internamente && recibido_externamente)
						{
							final_qidx_value = ((external_qidx_value * porcRSSI) / MAX_porcentajeRSSI) +
								((centralized_qidx_value * (MAX_porcentajeRSSI - porcRSSI)) / MAX_porcentajeRSSI);
						}
						else if (calculado_internamente)
						{
							final_qidx_value = centralized_qidx_value;
						}
						else
						{
							final_qidx_value = external_qidx_value;
						}

						if (in_window)
						{
							//Solo si estoy en la ventana de decision y si es un squelch de avion, me guardo los valores del qidx 
							//en el array de donde se sacan los valores en la evaluacion de la mejor
							//no guardo los valores durante la cola del squelch
							sipCall->SetSyncBss((int)final_qidx_value);
						}

						if (in_window && PJMEDIA_RTP_RD_EX_GET_PTT_ID(rtp_ext_info) == 0 && sipCall->Ptt_off_timer.id == 0)
						{
							SipAgent::_FrecDesp->SetAirQidxInWindows(sipCall->_Index_group, sipCall->_Index_sess);
						}

						if (in_window || SipAgent::Coresip_Local_Config._Debug_BSS)
						{
							//Este Qidx es solo para notificarlo al NBX
							if (sipCall->last_qidx_value != (pj_uint8_t)final_qidx_value || sipCall->squ_event_mcast)
							{
								sipCall->last_qidx_value = (pj_uint8_t)final_qidx_value;

								if (SipAgent::Coresip_Local_Config._Debug_BSS)
								{
									PJ_LOG(3, (__FILE__, "BSS: %s bss_method_type %d porcRSSI %d in_window %d centralizado %d valor %u, externo %d valor %u QIDX final %u",
										sipCall->DstUri, sipCall->bss_method_type, porcRSSI, in_window,
										calculado_internamente, centralized_qidx_value, recibido_externamente, external_qidx_value, final_qidx_value));
								}
							}

							SipAgent::_FrecDesp->SetQidxMethod(sipCall->_Index_group, sipCall->_Index_sess, qidx_method_rtp);
						}
					}

					pj_mutex_lock(sipCall->circ_buff_mutex);

					if (sipCall->squ_event)
					{
						//Se a�aden tantos silencios como valor de Retardo
						pjmedia_circ_buf_reset(sipCall->p_retbuff);
						while (pj_sem_trywait(sipCall->sem_out_circbuff) == PJ_SUCCESS);
						sipCall->wait_sem_out_circbuff = PJ_TRUE;
						for (pj_uint32_t i = 0; i < sipCall->Retardo; i++)
						{
							pjmedia_circ_buf_write(sipCall->p_retbuff, (pj_int16_t*)sipCall->zerobuf, 1);
						}
					}

					//Se a�ade al buffer circular el frame													

					pjmedia_circ_buf_write(sipCall->p_retbuff, (pj_int16_t*)buf, frame_out.size / 2);

					pj_mutex_unlock(sipCall->circ_buff_mutex);

					pj_sem_post(sipCall->sem_out_circbuff);
				}

				sipCall->squ_event_mcast = PJ_FALSE;
				sipCall->squ_event = PJ_FALSE;
			}
			else if ((st == PJ_SUCCESS) && (frame_out.size != (SAMPLES_PER_FRAME / 2) * sizeof(pj_int16_t)))
			{
				PJ_LOG(5, (__FILE__, "TRAZA: PAquete diferente de 80\n"));
			}
		}
		else
		{
			buf[0] = RESTART_JBUF;
			size = 1;
		}

	}
}

/**
* ext_type_length. Primera palabra de la extension de cabecera que contiene 'type' y 'length'
* p_rtp_ext_info. Puntero que apunta donde comienza la extension de cabecera
* rtp_ext_length. Longitud en palabras de 32 bits de la extension de cabecera
*/
void SipCall::OnRdInfoChanged(void* stream, void* ext_type_length, pj_uint32_t rtp_ext_info,
	const void* p_rtp_ext_info, pj_uint32_t rtp_ext_length, pj_bool_t tx_pttmute_changed, pj_bool_t GRS_force_ptt_mute_in_RTPRx_changed)
{
	void* session = pjmedia_stream_get_user_data((pjmedia_stream*)stream);
	if (!session) return;
	pjsua_call* pjcall = (pjsua_call*)pjmedia_session_get_user_data((pjmedia_session*)session);
	if (!pjcall) return;
	pjmedia_rtp_ext_hdr* extention_type_length = (pjmedia_rtp_ext_hdr*)ext_type_length;

	SipCall* sipCall = (SipCall*)pjcall->user_data;
	if (!sipCall) return;

	/*Para ver si somos una radio. Solo para cuando somos un ETM */
	AccountUserData* accUserData = NULL;
	pj_bool_t rdAccount = PJ_FALSE;				//Indica si acc_id es un account tipo radio GRS
	if (SipAgent::ETM)
	{
		pjsua_call_info callinfo;
		if (pjsua_call_get_info(pjcall->index, &callinfo) != PJ_SUCCESS)
		{
			return;
		}

		accUserData = (AccountUserData*)pjsua_acc_get_user_data(callinfo.acc_id);
		if (accUserData != NULL)
		{
			rdAccount = accUserData->rdAccount;
		}
	}
	
	pj_bool_t radio_grs = (SipAgent::_Radio_UA != 0 || rdAccount == PJ_TRUE);

	CORESIP_RdInfo info = { CORESIP_PTT_OFF };

	info.PttType = (CORESIP_PttType)PJMEDIA_RTP_RD_EX_GET_PTT_TYPE(rtp_ext_info);
	info.PttId = (unsigned short)PJMEDIA_RTP_RD_EX_GET_PTT_ID(rtp_ext_info);
	info.PttMute = (int)PJMEDIA_RTP_RD_EX_GET_PM(rtp_ext_info);
	info.Squelch = PJMEDIA_RTP_RD_EX_GET_SQU(rtp_ext_info);
	info.Sct = PJMEDIA_RTP_RD_EX_GET_SCT(rtp_ext_info);

	info.tx_ptt_mute_changed = (int)tx_pttmute_changed;

	//PJ_LOG(5,(__FILE__, "BSS: SQU info.Squelch %d FR %s %s", info.Squelch, sipCall->_RdFr, sipCall->DstUri));

	pj_bool_t ext_type_WG67;
	if (extention_type_length != NULL)
	{
		if ((extention_type_length->profile_data & 0xFF00) == 0x6700)
		{
			//La extension del cabecera es del tipo ED137 EUROCAE WG67
			ext_type_WG67 = PJ_TRUE;
		}
		else
		{
			ext_type_WG67 = PJ_FALSE;
		}
	}
	else
	{
		ext_type_WG67 = PJ_FALSE;
	}

	int x = PJMEDIA_RTP_RD_EX_GET_X(rtp_ext_info);
	pj_uint8_t* pextinfo = (pj_uint8_t*)p_rtp_ext_info;
	pextinfo += 2;		//Se pone el puntero donde comienzan las caracteristicas adicionales
	pj_int32_t nbytes = (pj_int32_t)(rtp_ext_length * 4);
	nbytes -= 2;

	pj_bool_t rdinfo_changed = PJ_FALSE;
	info.MAM_received = 0;
	while (ext_type_WG67 && x && nbytes > 0)
	{
		//Se tratan los TLVs	
		pj_uint32_t type, length;
		type = *pextinfo >> 4;
		type &= 0xF;
		length = *pextinfo;
		length &= 0xF;

		if (rtp_ext_length == 1 && length > 1)
			PJ_LOG(3, (__FILE__, "WARNING: Recibida extension de cabecera RTP erronea. rtp_ext_length %d type %d length %d", rtp_ext_length, type, length));

		pextinfo++;
		if (--nbytes == 0) break;

		pj_bool_t etm_no_radio_call = SipAgent::ETM && (sipCall->_Info.Type != CORESIP_CALL_RD);

		if (type == 0x4 && 
			(length == 12 || length == 14) &&
			((etm_no_radio_call || radio_grs) || (sipCall->_Info.Type == CORESIP_CALL_RD && sipCall->_Info.cld_supervision_time != 0)))
		{
			//MAM recibido
			//Si cld_supervision_time es 0, entonces ignoramos el MAM. No queremos supervision de CLD
			//Si no es una llamada de radio, entonces sirve para calcular el retardo de red contr otro ETM.
			
			pj_bool_t request_MAM = PJ_FALSE;
			unsigned int Tn1_ms;
			unsigned int Tj1_ms;
			unsigned int Tid_ms;
			unsigned int Tsd_ms;
			unsigned int Ts2_ms;
			pj_uint32_t Tn1;
			pj_uint32_t Tj1;
			pj_uint32_t Tid;
			pj_uint32_t Tsd;
			pj_uint32_t Ts2;
			CORESIP_CLD_CALCULATE_METHOD requested_climax_method = Relative;
			CORESIP_CLD_CALCULATE_METHOD used_climax_method = Relative;
			int ret = 0;

			if (etm_no_radio_call || radio_grs)
			{
				//Llamada telefonica o somos GRS
				requested_climax_method = Relative;
			}
			else
			{
				//Somos SCV
				ret = SipAgent::_FrecDesp->GetClimaxMethod(sipCall->_Index_group, sipCall->_Index_sess, requested_climax_method);				
			}

			if (ret >= 0 || etm_no_radio_call || radio_grs)
			{
				ret = SipAgent::_FrecDesp->CalcTimeDelay((pjmedia_stream*)stream, pextinfo, length,
					Tn1_ms, Tj1_ms, Tid_ms, Tsd_ms, Ts2_ms,
					Tn1, Tj1, Tid, Tsd, Ts2,
					requested_climax_method, used_climax_method, &request_MAM);

				if (ret >= 0 && !request_MAM)
				{
					PJ_LOG(5, (__FILE__, "CLIMAX: SetTimeDelay  radio uri %s", sipCall->DstUri));
					ret = SipAgent::_FrecDesp->SetTimeDelay(sipCall->_Index_group, sipCall->_Index_sess,
						Tn1, Tj1, Tid, Tsd, used_climax_method);
				}
			}			

			if (etm_no_radio_call || radio_grs)
			{
				if (ret >= 0)
				{
					info.MAM_received = 1;
					info.Tn1_ms = Tn1_ms;
					info.Tj1_ms = Tj1_ms;
					info.Tid_ms = Tid_ms;
					info.Tsd_ms = Tsd_ms;
					if (length == 14) info.Ts2_ms = (int)Ts2_ms;
					else info.Ts2_ms = -1;
					rdinfo_changed = PJ_TRUE;

					if (sipCall->_Info.Type != CORESIP_CALL_RD)
					{
						pj_mutex_lock(sipCall->RdInfo_prev_mutex);
						memcpy(&sipCall->RdInfo_prev, &info, sizeof(CORESIP_RdInfo));
						if (info.MAM_received) pj_sem_post(sipCall->waiting_MAM_sem);
						pj_mutex_unlock(sipCall->RdInfo_prev_mutex);
					}
				}
			}
			else
			{
				pj_time_val	delay;
				sipCall->Check_CLD_timer.id = Check_CLD_timer_IDLE;
				pjsua_cancel_timer(&sipCall->Check_CLD_timer);
				if ((ret < 0) && request_MAM)
				{
					/*NMR esta activado. Entonces hay que volver a hacer calculo de retardo*/
					delay.sec = 0;
					delay.msec = 3;
					pj_timer_entry_init(&sipCall->Check_CLD_timer, Check_CLD_timer_SEND_RMM, (void*)sipCall, Check_CLD_timer_cb);
				}
				else if (ret < 0)
				{
					PJ_LOG(3, (__FILE__, "ERROR: Time Delay cannot be assigned to session %p\n", session));
					//Se reintentará mandando un nuevo rmm despues del periodo de supervisión del cld				
					delay.sec = (long)sipCall->_Info.cld_supervision_time;
					delay.msec = 0;
					pj_timer_entry_init(&sipCall->Check_CLD_timer, Check_CLD_timer_SEND_RMM, (void*)sipCall, Check_CLD_timer_cb);
				}
				else
				{
					info.MAM_received = 1;
					info.Tn1_ms = Tn1_ms;
					info.Tj1_ms = Tj1_ms;
					info.Tid_ms = Tid_ms;
					info.Tsd_ms = Tsd_ms;
					if (length == 14) info.Ts2_ms = (int)Ts2_ms;
					else info.Ts2_ms = -1;
					rdinfo_changed = PJ_TRUE;

					//Forzamos a enviar el CLD cuanto antes
					delay.sec = 0;
					delay.msec = 3;
					pj_timer_entry_init(&sipCall->Check_CLD_timer, Check_CLD_timer_SEND_CLD, (void*)sipCall, Check_CLD_timer_cb);
				}
				pj_status_t st = pjsua_schedule_timer(&sipCall->Check_CLD_timer, &delay);
				if (st != PJ_SUCCESS)
				{
					sipCall->Check_CLD_timer.id = Check_CLD_timer_IDLE;
					PJ_CHECK_STATUS(st, ("ERROR en Check_CLD_timer"));
				}
			}
		}
		else if (type == 0x4 && length == 3 && (SipAgent::ETM) && (accUserData != NULL))
		{
			//RMM recibido y somos un GRS
			pj_uint32_t TQV;
			pj_uint32_t T1;
			FrecDesp::GetRMMData(pextinfo, &TQV, &T1);
			pj_uint32_t T2 = (pj_uint32_t)pjmedia_stream_GetTimeClimax();
			pj_uint32_t Tid = (pj_uint32_t) ((pj_uint32_t) accUserData->Ts1 + (pj_uint32_t) sipCall->GRS_Td1);
			if (Tid > 0xFFFF) Tid = 0xFFFF;
			pjmedia_stream_send_MAM((pjmedia_stream*)stream, TQV, T1, T2, accUserData->Tj1, (pj_uint16_t) Tid, accUserData->Ts2);
		}
		else if (type == 0x2 && length == 1 && (SipAgent::ETM) && (SipAgent::_Radio_UA || rdAccount) && (accUserData != NULL))
		{
			//Como GRS se ha recibido un CLD
			pj_uint32_t cld = ((pj_uint32_t) *pextinfo) & ((pj_uint32_t)0x0000007F);	//Tenemos el CLD en unidades de 2ms, y quitamos el bit relative/absolute
			cld *= 2000;	//Unidades de 1 us
			cld /= 125;		//Unidades de 125 us
			sipCall->GRS_Td1 = (pj_uint16_t)cld;
		}

		pextinfo += length;
		nbytes -= length;

		if (nbytes < 0)
		{
			PJ_LOG(5, (__FILE__, "WARNING: Recibida extension de cabecera RTP erronea. rtp_ext_length %d type %d length %d", rtp_ext_length, type, length));
		}
	}

	if (sipCall->_Info.Type != CORESIP_CALL_RD)
	{
		return;
	}

	pj_uint8_t qidx;
	pj_uint8_t qidx_ml;
	pj_uint32_t Tred = 0;
	pj_uint32_t Ret = 0;
	SipAgent::_FrecDesp->GetQidx(sipCall->_Index_group, sipCall->_Index_sess, &qidx, &qidx_ml, &Tred);
	info.tx_owd = ((int)Tred * (int)125) / 1000; //Se pasa a unidades de 1ms

	pj_bool_t pttid_changed = PJ_FALSE;
	if (info.PttId != sipCall->RdInfo_prev.PttId)
	{
		//Actualizo aqui el Ptt id, y ya lo tiene el objeto sipcall para la seleccion de la mejor por BSS
		sipCall->RdInfo_prev.PttId = info.PttId;
		rdinfo_changed = PJ_TRUE;
		pttid_changed = PJ_TRUE;
	}

	if (tx_pttmute_changed)
	{
		//Si el PTT mute que se envia a la radio ha cambiado, entonces se fuerza la llamada a la callback RdInfoCb
		rdinfo_changed = PJ_TRUE;
	}

#if 0
#ifdef _DEBUG
	if (strcmp(sipCall->DstUri, "<sip:RX-01@192.168.1.18:5060>") == 0) qidx = 15;
	if (strcmp(sipCall->DstUri, "<sip:RX-02@192.168.1.18:5060>") == 0) qidx = 14;
	if (strcmp(sipCall->DstUri, "<sip:RX-03@192.168.1.18:5060>") == 0) qidx = 13;
	if (strcmp(sipCall->DstUri, "<sip:RX-04@192.168.1.18:5060>") == 0) qidx = 12;
#endif
#endif

	if (SipAgent::_Radio_UA || rdAccount)
	{
		//Somos un agente de radio
		if (sipCall->last_received_ptt_status != info.PttType)
		{
			if (info.PttType != CORESIP_PTT_OFF)
			{
				//PTT ON
				sipCall->primer_paquete_despues_ptt = PJ_TRUE;

				//Se cancela el timer de envio de selcal y se para la simulacion de SELCAL
				sipCall->SELCAL_timer.id = 0;
				pjsua_cancel_timer(&sipCall->SELCAL_timer);
				pjmedia_stream_set_SelCalInProgress((pjmedia_stream*)stream, PJ_FALSE);
			}
			else
			{
				//PTT OFF
				sipCall->primer_paquete_despues_ptt = PJ_TRUE;
			}
		}

		if (sipCall->_ConfirmPtt &&
			(sipCall->last_received_ptt_status != info.PttType ||
				sipCall->RdInfo_prev.PttMute != info.PttMute)
			)
		{
			//Somos una agente GRS. Se retorna el estado de PTT

			pj_uint32_t tx_rtp_ext_info;
			pjmedia_stream_get_rtp_ext_tx_info((pjmedia_stream*)stream, &tx_rtp_ext_info);

			if ((sipCall->_Info.CallFlags & CORESIP_CALL_RD_RXONLY) != 0)
			{
				if (info.PttId == 0)
				{
					PJMEDIA_RTP_RD_EX_SET_PTT_TYPE(tx_rtp_ext_info, 0);
				}
				else
				{
					PJMEDIA_RTP_RD_EX_SET_PTT_TYPE(tx_rtp_ext_info, info.PttType);
				}
			}
			else
			{
				PJMEDIA_RTP_RD_EX_SET_PTT_TYPE(tx_rtp_ext_info, info.PttType);
			}

			if (sipCall->radio_session_with_pttid_enabled)
			{
				if (info.PttType == CORESIP_PTT_OFF)
				{
					if (PJMEDIA_RTP_RD_EX_GET_SQU(tx_rtp_ext_info) != 0)
					{
						//Si como GRS esta activado el squelch y no se recibe PTT. Entonces ptt-id sera cero
						PJMEDIA_RTP_RD_EX_SET_PTT_ID(tx_rtp_ext_info, 0);
						pjmedia_stream_set_ptt_id((pjmedia_stream*)stream, 0);
					}
					else
					{
						//Si no recibo PTT ni estoy haciendo squelch, entonces envio el Ptt-id asignado
						PJMEDIA_RTP_RD_EX_SET_PTT_ID(tx_rtp_ext_info, sipCall->assigned_pttid);
						pjmedia_stream_set_ptt_id((pjmedia_stream*)stream, sipCall->assigned_pttid);
					}
				}
				else
				{
					if (PJMEDIA_RTP_RD_EX_GET_SQU(tx_rtp_ext_info) != 0 && PJMEDIA_RTP_RD_EX_GET_PM(tx_rtp_ext_info) == 0 &&
						pjmedia_stream_get_GRS_force_ptt_in_RTPRx_confirmed((pjmedia_stream*)stream))
					{
						//Si hay squelch y esta forzado el ptt mute en el GRS
						PJMEDIA_RTP_RD_EX_SET_PTT_ID(tx_rtp_ext_info, 0);
						pjmedia_stream_set_ptt_id((pjmedia_stream*)stream, 0);
					}
					else
					{
						//Recibo un PTT, tengo que hacer el eco del PTT-id
						PJMEDIA_RTP_RD_EX_SET_PTT_ID(tx_rtp_ext_info, info.PttId);
						pjmedia_stream_set_ptt_id((pjmedia_stream*)stream, info.PttId);
					}
				}
			}
			else
			{
				//Si la sesion no requiere Ptt-id entonces no se hace eco de PTT y PTT-id
				PJMEDIA_RTP_RD_EX_SET_PTT_TYPE(tx_rtp_ext_info, 0);
				PJMEDIA_RTP_RD_EX_SET_PTT_ID(tx_rtp_ext_info, 0);
				pjmedia_stream_set_ptt_id((pjmedia_stream*)stream, 0);
			}

			PJMEDIA_RTP_RD_EX_SET_PM(tx_rtp_ext_info, info.PttMute);

			pjmedia_stream_set_rtp_ext_tx_info((pjmedia_stream*)stream, tx_rtp_ext_info);
			sipCall->last_rtp_ext_info = tx_rtp_ext_info;
		}

		if (GRS_force_ptt_mute_in_RTPRx_changed)
		{
			if (pjmedia_stream_get_GRS_force_ptt_in_RTPRx_confirmed((pjmedia_stream*)stream))
			{
				info.PttMute = 1;
				if (sipCall->RdInfo_prev.PttMute == 0)
				{
					rdinfo_changed = PJ_TRUE;
				}
			}
			else
			{
				if (info.PttMute != sipCall->RdInfo_prev.PttMute)
				{
					rdinfo_changed = PJ_TRUE;
				}
			}
		}
		else
		{
			if (pjmedia_stream_get_GRS_force_ptt_in_RTPRx_confirmed((pjmedia_stream*)stream))
			{
				info.PttMute = 1;
				if (sipCall->RdInfo_prev.PttMute == 0)
				{
					rdinfo_changed = PJ_TRUE;
				}
			}
			else
			{
				if (info.PttMute != sipCall->RdInfo_prev.PttMute)
				{
					rdinfo_changed = PJ_TRUE;
				}
			}
		}
	}


	if (sipCall->last_received_ptt_status != info.PttType)
	{
		sipCall->last_received_ptt_status = info.PttType;
	}


	if ((sipCall->last_received_squ_status != info.Squelch) && (sipCall->Wait_init_timer.id == 0))
	{
		pj_ssize_t size;

		size = (pj_ssize_t)((SAMPLES_PER_FRAME / 2) * sizeof(pj_int16_t));

		//Ha cambiado el estado del squelch 
		if (info.Squelch == 1)
		{
			//SQU ON

			//Iniciamos el algoritmo para el calculo centralizado del Qidx
			sipCall->a_dc[0] = 1.0f;
			sipCall->a_dc[1] = -0.9950f;
			sipCall->b_dc[0] = 0.9975f;
			sipCall->b_dc[1] = -0.9975f;
			sipCall->fFiltroDC_IfRx_ciX = 0.0f;
			sipCall->fFiltroDC_IfRx_ciY = 0.0f;
			processor_init(&sipCall->PdataQidx, 0);

			int nsqus = SipAgent::_FrecDesp->SetSquSt(sipCall->_Index_group, sipCall->_Index_sess, PJ_TRUE, NULL);
			Ret = SipAgent::_FrecDesp->GetRetToApply(sipCall->_Index_group, sipCall->_Index_sess);

			PJ_LOG(5, (__FILE__, "BSS: SQU ON RdDestId %s %s Ret %d ms GROUP %d SESS %d", sipCall->_IdDestino, sipCall->DstUri, (Ret * 125) / 1000, sipCall->_Index_group, sipCall->_Index_sess));

			if (nsqus == 1)
			{
				//Este es el unico squelch activado y es de avion
				SipAgent::_FrecDesp->SetSelected(sipCall, PJ_TRUE, PJ_FALSE);

				//Se para el timer de la ventana de decision por si estuviera arrancado
				SipCall *sipcall_in_window_timer = SipAgent::_FrecDesp->GetInWindow(sipCall->_Index_group);
				if (sipcall_in_window_timer != NULL)
				{
					sipcall_in_window_timer->window_timer.id = 0;
					pjsua_cancel_timer(&sipcall_in_window_timer->window_timer);
					SipAgent::_FrecDesp->SetInWindow(sipCall->_Index_group, NULL);
				}
				sipCall->window_timer.id = 0;
				pjsua_cancel_timer(&sipCall->window_timer);

				SipAgent::_FrecDesp->SetSelectedUri(sipCall);   //Es el primer squelch que llega. Asignamos esta uri como seleccionada
				if (sipCall->bss_method_type != NINGUNO)
				{
					//Hay grupo bss porque tiene m�s de una sesi�n, sin contar los tx only
					if (sipCall->_Info.AudioInBssWindow)
					{
						//Se activa el envio por multicast de esta sesion
						SipAgent::_FrecDesp->EnableMulticast(sipCall, PJ_TRUE, PJ_FALSE);
						if (SipAgent::Coresip_Local_Config._Debug_BSS)
							PJ_LOG(3, (__FILE__, "BSS: SQU ON. ID DESTINO %s Fr %s uri %s PRE-SELECCIONADO", sipCall->_IdDestino, sipCall->_RdFr, sipCall->DstUri));
					}
					else
					{
						//No hay audio durante la ventana de decisi�n
						SipAgent::_FrecDesp->EnableMulticast(sipCall, PJ_FALSE, PJ_TRUE);
					}
				}
				else
				{
					//Solo hay un miembro en el grupo
					//No hay grupo bss. Se activa el env�o por multicast
					SipAgent::_FrecDesp->EnableMulticast(sipCall, PJ_TRUE, PJ_FALSE);
					if (SipAgent::Coresip_Local_Config._Debug_BSS)
						PJ_LOG(3, (__FILE__, "BSS: SQU ON. ID DESTINO %s Fr %s uri %s SELECCIONADO", sipCall->_IdDestino, sipCall->_RdFr, sipCall->DstUri));
				}

				sipCall->squ_event_mcast = PJ_TRUE;		//Se activa evento con el primer squelch de un grupo

				SipAgent::_FrecDesp->Set_group_multicast_socket(sipCall->_Index_group, &sipCall->_RdSendTo);

				if (sipCall->bss_method_type != NINGUNO)
				{
					//Se arranca el timer de la ventana de decision. En caso de que haya mas de una session en
					//el grupo
					pj_time_val	delay;
					delay.sec = ((long)sipCall->_Info.BssWindows) / 1000;
					delay.msec = ((long)sipCall->_Info.BssWindows) - (delay.sec * 1000);
					pj_timer_entry_init(&sipCall->window_timer, 1, (void*)sipCall, window_timer_cb);
					pj_status_t st = pjsua_schedule_timer(&sipCall->window_timer, &delay);
					if (st != PJ_SUCCESS)
					{
						sipCall->window_timer.id = 0;
						PJ_CHECK_STATUS(st, ("ERROR en window_timer"));
					}
					else
					{
						if (SipAgent::Coresip_Local_Config._Debug_BSS)
						{
							PJ_LOG(3, (__FILE__, "BSS: SQU ON. ARRANCA VENTANA DECISION ID DESTINO %s Fr %s uri %s PRE-SELECCIONADO", 
								sipCall->_IdDestino, sipCall->_RdFr, sipCall->DstUri));
						}
						//Al resto del grupo se activa que estamos en la ventana de decision
						SipAgent::_FrecDesp->SetInWindow(sipCall->_Index_group, sipCall);
					}
				}

				//Al llegar el squelch de la primera radio del grupo, reiniciamos los qidx almacenados en todas las radios del grupo
				SipAgent::_FrecDesp->Clear_SyncBss_In_Group(sipCall->_Index_group);
			}			

			if (!sipCall->_Info.AudioSync)
			{
				//Se fuerza que el retardo a aplicar en la recepcion de audio sea 0
				Ret = 0;
			}

			if (Ret < 80)		//Si el retardo es menor de 10ms=80*125us no se aplica retardo
			{
				sipCall->hay_retardo = PJ_FALSE;
				sipCall->Retardo = 0;
				sipCall->squoff_event_mcast = PJ_FALSE;
			}
			else
			{
				sipCall->hay_retardo = PJ_TRUE;
				sipCall->Retardo = Ret;
			}
			sipCall->squ_event = PJ_TRUE;   //El estado a cambiado a squ activo. Activamos el evento de squelch				
			sipCall->primer_paquete_despues_squelch = PJ_TRUE;

			sipCall->last_received_squ_status = info.Squelch;
		}
		else
		{
			//SQU OFF
			PJ_LOG(5, (__FILE__, "BSS: SQU OFF RdDestId %s %s", sipCall->_IdDestino, sipCall->DstUri));

			int sq_air_count = 0;	//Contador de squelches de avion activados en el grupo
			int nsqus = SipAgent::_FrecDesp->SetSquSt(sipCall->_Index_group, sipCall->_Index_sess, PJ_FALSE, &sq_air_count);
			SipCall* sipcall_in_window_timer = SipAgent::_FrecDesp->GetInWindow(sipCall->_Index_group);
			unsigned short selectedUriPttId = 0;
			SipAgent::_FrecDesp->GetSelectedUri(sipCall, NULL, &selectedUriPttId);

			if (nsqus < 1)
			{
				//No hay ningun squelch del grupo activado
				//Se para el timer de la ventana de decision por si estaba activado
				if (sipcall_in_window_timer != NULL)
				{
					sipcall_in_window_timer->window_timer.id = 0;
					pjsua_cancel_timer(&sipcall_in_window_timer->window_timer);
					SipAgent::_FrecDesp->SetInWindow(sipCall->_Index_group, NULL);
				}

				sipCall->window_timer.id = 0;
				pjsua_cancel_timer(&sipCall->window_timer);
				SipAgent::_FrecDesp->ClrSelectedUri(sipCall);
			}			
			else if (sipcall_in_window_timer != NULL)  //Estamos en la ventana de decision
			{
				//Hay otros squelch activados
				if (sipCall->_Sending_Multicast_enabled)
				{					
					if (selectedUriPttId != 0)
					{
						//Si esta rama seleccionada no era procedente de avion 					
						//Forzamos un estado de ningun squelch activado en el grupo para forzar el arranque de una nueva ventana de decision					
						sipcall_in_window_timer->window_timer.id = 0;
						pjsua_cancel_timer(&sipcall_in_window_timer->window_timer);
						SipAgent::_FrecDesp->SetInWindow(sipCall->_Index_group, NULL);
						SipAgent::_FrecDesp->EnableMulticast(sipCall, PJ_FALSE, PJ_TRUE);
						SipAgent::_FrecDesp->ClrSelectedUri(sipCall);
						SipAgent::_FrecDesp->ClrAllSquSt(sipCall->_Index_group);
					}
					else
					{
						//Esta sesion es la que estaba activada. Cambiamos a otra que tenga squelch activado
						//Por ejemplo la que tenga mejor bss.
						SipAgent::_FrecDesp->SetBetterSession(sipCall, FrecDesp::IN_WINDOW, !FrecDesp::ONLY_SELECTED_IN_WINDOW);
					}
				}
			}
			else if (sipCall->_Sending_Multicast_enabled)
			{
				//Estamos fuera de la ventana de decision y se detecta squ off en la sesion que ahora es la activa
				if (selectedUriPttId != 0)
				{
					//Si esta rama seleccionada no era procedente de avion 					
					//Forzamos un estado de ningun squelch activado en el grupo para forzar el arranque de una nueva ventana de decision					

					SipAgent::_FrecDesp->EnableMulticast(sipCall, PJ_FALSE, PJ_TRUE);
					SipAgent::_FrecDesp->ClrSelectedUri(sipCall);
					SipAgent::_FrecDesp->ClrAllSquSt(sipCall->_Index_group);
				}
				else if (selectedUriPttId == 0 && sq_air_count == 0)
				{
					//Por el contrario
					//Si el squelch de esta rama era procedente de avion pero ya solamente quedan squelches activados 
					//que no son de avion, entonces se supone que lo que transmitimos tiene que recibirse por todas las ramas 
					//Por tanto no hay que hacer recalificacion y seleccionar otra rama y por tanto en este caso
					//hay que hacer lo mismo que cuando se desactiva cualquier squelch. Es decir
					//Solo se puede activar la sesión que se activo durante la ventana de decision
					//O sea que se hace lo mismo que en el siguiente else
					//Lo hacemos asi para que quede documentado.
					SipAgent::_FrecDesp->SetBetterSession(sipCall, !FrecDesp::IN_WINDOW, FrecDesp::ONLY_SELECTED_IN_WINDOW);
				}
				else
				{
					//Solo se puede activar la sesión que se activo durante la ventana de decision
					SipAgent::_FrecDesp->SetBetterSession(sipCall, !FrecDesp::IN_WINDOW, FrecDesp::ONLY_SELECTED_IN_WINDOW);
				}
			}

			sipCall->squoff_event_mcast = PJ_TRUE;
			pj_sem_post(sipCall->sem_out_circbuff);		//Al salir del wait de este semaforo con el flag squoff_event_mcast
														//nos indica que fin de squelch y no esperar con el semaforo sino con el sleep
			//Al desactivarse  el squelch se ponen silencios en buffer circular
			pj_mutex_lock(sipCall->circ_buff_mutex);
			for (pj_uint32_t i = 0; i < 6; i++)
			{
				pjmedia_circ_buf_write(sipCall->p_retbuff, (pj_int16_t*)sipCall->zerobuf, size / 2);
			}
			pj_mutex_unlock(sipCall->circ_buff_mutex);

			sipCall->last_received_squ_status = info.Squelch;

		}
		
	}
	else if (pttid_changed && info.PttId == 0 && info.Squelch != 0 && 
				sipCall->last_received_squ_status == info.Squelch && sipCall->Wait_init_timer.id == 0)
	{
		//Ha cambiado el PTT-id a cero, pero no ha cambiado el estado de squelch y es ON
		//Si estamos fuera de la ventana de decision, todos los squelch que quedan son de avion
		//y la ventana arranco con squelch no de avion
		//Forzaremos una nueva ventana de decision

		int sq_air_count = 0;	//Contador de squelches de avion activados en el grupo
		int nsqus = SipAgent::_FrecDesp->SetSquSt(sipCall->_Index_group, sipCall->_Index_sess, PJ_TRUE, &sq_air_count);
		SipCall* sipcall_in_window_timer = SipAgent::_FrecDesp->GetInWindow(sipCall->_Index_group);
		unsigned short selectedUriPttId = 0;
		SipAgent::_FrecDesp->GetSelectedUri(sipCall, NULL, &selectedUriPttId);

		if (sipcall_in_window_timer == NULL && sq_air_count > 0 && sq_air_count == nsqus && selectedUriPttId != 0)
		{
			//No estamos en la ventana de decision, todos los squelch son de avion 
			//y la ventana de decision arranco con squelch de avion
			//Forzamos un estado de ningun squelch activado en el grupo para forzar el arranque de una nueva ventana de decision					
			SipAgent::_FrecDesp->EnableMulticast(sipCall, PJ_FALSE, PJ_TRUE);
			SipAgent::_FrecDesp->ClrSelectedUri(sipCall);
			SipAgent::_FrecDesp->ClrAllSquSt(sipCall->_Index_group);
		}
	}

	pjmedia_session_info sess_info;
	pjmedia_session_get_info((pjmedia_session*)session, &sess_info);

	pjmedia_transport_info tpinfo;
	pjmedia_transport_info_init(&tpinfo);
	pjmedia_transport_get_info(pjcall->med_tp, &tpinfo);

	info.rx_rtp_port = (int)pj_ntohs(sess_info.stream_info[0].rem_addr.ipv4.sin_port);
	info.rx_qidx = qidx;
	info.rx_selected = SipAgent::_FrecDesp->IsBssSelected(sipCall);
	info.tx_rtp_port = (int)pj_ntohs(tpinfo.sock_info.rtp_addr_name.ipv4.sin_port);
	SipAgent::_FrecDesp->GetLastCld(sipCall->_Index_group, sipCall->_Index_sess, (pj_uint8_t*)&info.tx_cld);

	info.tx_cld &= ~0x80;
	info.tx_cld *= 2;		//Se multiplica por 2 para pasar a unidades de 1 ms

	//info.tx_cld = 0;
	//info.tx_owd = 0;

	pj_mutex_lock(sipCall->RdInfo_prev_mutex);
	if (memcmp(&info, &sipCall->RdInfo_prev, sizeof(CORESIP_RdInfo)) != 0)
		rdinfo_changed = PJ_TRUE;
	pj_mutex_unlock(sipCall->RdInfo_prev_mutex);

	if (rdinfo_changed)
	{
		//Llamo a la callback si cambia algun valor en la info

		PJ_LOG(5, (__FILE__, "onRdinfochanged: ########## dst %s PttType %d PttId %d rx_selected %d Squelch %d", sipCall->DstUri, info.PttType, info.PttId, info.rx_selected, info.Squelch));
		PJ_LOG(5, (__FILE__, "onRdinfochanged: PttMute %d Sct %d rx_rtp_port %d rx_qidx %d", info.PttMute, info.Sct, info.rx_rtp_port, info.rx_qidx));
		PJ_LOG(5, (__FILE__, "onRdinfochanged: tx_rtp_port %d tx_cld %d tx_owd %d sipCall->Ptt_off_timer.id %d ############", info.tx_rtp_port, info.tx_cld, info.tx_owd, sipCall->Ptt_off_timer.id));

		pj_mutex_lock(sipCall->RdInfo_prev_mutex);
		memcpy(&sipCall->RdInfo_prev, &info, sizeof(CORESIP_RdInfo));
		if (info.MAM_received) pj_sem_post(sipCall->waiting_MAM_sem);
		pj_mutex_unlock(sipCall->RdInfo_prev_mutex);

		if (sipCall->Ptt_off_timer.id == 0)
		{
			if (info.Squelch == 0) info.rx_qidx = 0;	//Si no hay squelch el qidx que se envia a la aplicacion es cero

			//Actualizamos los parametros que no se toman en la callback OnRdInfochanged 
			//Esto hay que hacerlo siempre que se llame a RdInfoCb
			//-->
			info.rx_selected = SipAgent::_FrecDesp->IsBssSelected(sipCall);
			//<--

			if (SipAgent::Cb.RdInfoCb)
			{
				PJ_LOG(5, (__FILE__, "onRdinfochanged: envia a nodebox. dst %s PttType %d PttId %d rx_selected %d Squelch %d", sipCall->DstUri, info.PttType, info.PttId, info.rx_selected, info.Squelch));
				SipAgent::Cb.RdInfoCb(pjcall->index | CORESIP_CALL_ID, &info);
			}
		}
	}
}

/**
 * window_timer_cb.	...
 * Callback que se llama cuando window_timer expira. Termina la ventana de decision
 * @return
 */
void SipCall::window_timer_cb(pj_timer_heap_t* th, pj_timer_entry* te)
{
	SipCall* wp = (SipCall*)te->user_data;

	PJ_UNUSED_ARG(th);

	if (SipAgent::Coresip_Local_Config._Debug_BSS)
		PJ_LOG(3, (__FILE__, "BSS: VENCE VENTANA DECISION. ID DESTINO %s Fr %s uri %s", wp->_IdDestino, wp->_RdFr, wp->DstUri));

	SipAgent::_FrecDesp->SetInWindow(wp->_Index_group, NULL);

	if (wp->window_timer.id == 1)
	{
		wp->window_timer.id = 0;
		pjsua_cancel_timer(&wp->window_timer);

		SipAgent::_FrecDesp->SetBetterSession(wp, FrecDesp::IN_WINDOW, !FrecDesp::ONLY_SELECTED_IN_WINDOW);
	}
	else
	{
		wp->window_timer.id = 0;
		pjsua_cancel_timer(&wp->window_timer);
	}
}

int SipCall::Out_circbuff_Th(void* proc)
{
	SipCall* wp = (SipCall*)proc;
	pj_status_t st = PJ_SUCCESS;
	pj_bool_t primer_paquete = PJ_TRUE;

	pj_thread_desc desc;
	pj_thread_t* this_thread;
	pj_status_t rc;

	pj_bzero(desc, sizeof(desc));

	rc = pj_thread_register("Out_circbuff_Th", desc, &this_thread);
	if (rc != PJ_SUCCESS) {
		PJ_LOG(3, (__FILE__, "...error in pj_thread_register Out_circbuff_Th!"));
		return 0;
	}

	/* Test that pj_thread_this() works */
	this_thread = pj_thread_this();
	if (this_thread == NULL) {
		PJ_LOG(3, (__FILE__, "...error: Out_circbuff_Th pj_thread_this() returns NULL!"));
		return 0;
	}

	/* Test that pj_thread_get_name() works */
	if (pj_thread_get_name(this_thread) == NULL) {
		PJ_LOG(3, (__FILE__, "...error: Out_circbuff_Th pj_thread_get_name() returns NULL!"));
		return 0;
	}

	while (wp->out_circbuff_thread_run)
	{
		if (wp->wait_sem_out_circbuff) st = pj_sem_wait(wp->sem_out_circbuff);
		else pj_thread_sleep(PTIME / 2);
		if (!wp->out_circbuff_thread_run) break;
		if (wp->squoff_event_mcast)
		{
			wp->squoff_event_mcast = PJ_FALSE;
			wp->wait_sem_out_circbuff = PJ_FALSE;
			continue;
		}

		pj_bool_t packet_present = PJ_FALSE;
		pj_ssize_t size_packet = (pj_ssize_t)((SAMPLES_PER_FRAME / 2) * sizeof(pj_int16_t));   //en bytes
		pj_ssize_t size_packet_x = size_packet + sizeof(unsigned);
		char buf_out[640];

		pj_mutex_lock(wp->circ_buff_mutex);
		unsigned int cbuf_len = pjmedia_circ_buf_get_len(wp->p_retbuff);
		if (cbuf_len < ((unsigned int)(size_packet / 2)))
		{
			packet_present = PJ_FALSE;

			pjmedia_circ_buf_reset(wp->p_retbuff);
			pj_mutex_unlock(wp->circ_buff_mutex);
			wp->wait_sem_out_circbuff = PJ_TRUE;

			if (SipAgent::_FrecDesp->IsBssSelected(wp) && wp->window_timer.id == 0)
			{
				//Este es el seleccionado en el bss y no se est� en la ventana de selecci�n
				//Desaparece la seleccion de todos. 
				//SipAgent::_FrecDesp->EnableMulticast(wp, PJ_FALSE, PJ_TRUE);
				//SipAgent::_FrecDesp->SetSelected(wp, PJ_FALSE, PJ_TRUE);

				//Mandamos el dato para resetear el buffer jitter de recepci�n de audio en el puesto
				pj_sockaddr_in* RdsndTo;
				SipAgent::_FrecDesp->Get_group_multicast_socket(wp->_Index_group, &RdsndTo);
				buf_out[0] = RESTART_JBUF;
				size_packet = 1;
				pj_sock_sendto(wp->_RdSendSock, buf_out, &size_packet, 0, RdsndTo, sizeof(pj_sockaddr_in));
			}

			continue;
		}
		else
		{
			packet_present = PJ_TRUE;
		}

		if (packet_present == PJ_FALSE)
		{
			if (wp->last_received_squ_status == SQU_ON)
			{
				PJ_LOG(5, (__FILE__, "TRAZA: Multicast_clock_cb SIN MUESTRAS"));
			}
		}
		else
		{
#if 0
			if ((wp->Retardo > 0) && wp->wait_sem_out_circbuff)
			{
				pj_uint32_t umbral_l, umbral_h;
				if (wp->Retardo > size_packet / 2)
				{
					umbral_l = wp->Retardo - size_packet / 2;
					umbral_h = wp->Retardo + size_packet / 2;
				}
				else
				{
					umbral_l = cbuf_len;
					umbral_h = cbuf_len;
				}

				if (cbuf_len < umbral_l)
				{
					PJ_LOG(5, (__FILE__, "TRAZA: RETARDO MENOR antes cbuf_len %u Retardo %u\n", cbuf_len, wp->Retardo));
					//pjmedia_circ_buf_write(wp->p_retbuff, (pj_int16_t *) wp->zerobuf, size_packet/2);
					while (cbuf_len < wp->Retardo)
					{
						pjmedia_circ_buf_write(wp->p_retbuff, (pj_int16_t*)wp->zerobuf, 1);
						cbuf_len = pjmedia_circ_buf_get_len(wp->p_retbuff);
					}
					//cbuf_len = pjmedia_circ_buf_get_len(wp->p_retbuff);
					PJ_LOG(5, (__FILE__, "TRAZA: RETARDO MENOR despues cbuf_len %u Retardo %u\n", cbuf_len, wp->Retardo));
				}
				else if (cbuf_len > umbral_h)
				{
					PJ_LOG(5, (__FILE__, "TRAZA: RETARDO MAYOR antes cbuf_len %u Retardo %u\n", cbuf_len, wp->Retardo));
					//pjmedia_circ_buf_read(wp->p_retbuff, (pj_int16_t *) wp->zerobuf, size_packet/2);
					while (cbuf_len > umbral_h)
					{
						pj_int16_t nullc;
						pjmedia_circ_buf_read(wp->p_retbuff, (pj_int16_t*)&nullc, 1);
						cbuf_len = pjmedia_circ_buf_get_len(wp->p_retbuff);
					}
					//cbuf_len = pjmedia_circ_buf_get_len(wp->p_retbuff);
					PJ_LOG(5, (__FILE__, "TRAZA: RETARDO MAYOR despues cbuf_len %u Retardo %u\n", cbuf_len, wp->Retardo));
				}
				else;
			}
#endif


			pjmedia_circ_buf_read(wp->p_retbuff, (pj_int16_t*)buf_out, size_packet / 2);
			pj_mutex_unlock(wp->circ_buff_mutex);

			if (wp->_Sending_Multicast_enabled)
			{
				unsigned nseq = SipAgent::_FrecDesp->Get_mcast_seq(wp->_Index_group);

				*((unsigned*)((char*)buf_out + size_packet)) = pj_htonl(nseq);
				pj_sockaddr_in* RdsndTo;
				SipAgent::_FrecDesp->Get_group_multicast_socket(wp->_Index_group, &RdsndTo);

				if (RdsndTo == NULL)
				{
					RdsndTo = &wp->_RdSendTo;		//Si se ha perdido la direccion usamos la propia
					SipAgent::_FrecDesp->Set_group_multicast_socket(wp->_Index_group, RdsndTo);
				}

				if (primer_paquete)
				{
					char data = RESTART_JBUF;
					pj_ssize_t siz = 1;
					pj_sock_sendto(wp->_RdSendSock, &data, &siz, 0, RdsndTo, sizeof(pj_sockaddr_in));
					primer_paquete = PJ_FALSE;
				}

				pj_sock_sendto(wp->_RdSendSock, buf_out, &size_packet_x, 0, RdsndTo, sizeof(pj_sockaddr_in));
				//pj_sock_sendto(wp->_RdSendSock, buf_out, &size_packet_x, 0, &wp->_RdSendTo, sizeof(pj_sockaddr_in));
			}
			continue;
		}

		pj_mutex_unlock(wp->circ_buff_mutex);

		continue;
	}

	return 0;
}

/*Timer para solicitar periodicamente el MAM para el climax*/
void SipCall::Check_CLD_timer_cb(pj_timer_heap_t* th, pj_timer_entry* te)
{
	SipCall* wp = (SipCall*)te->user_data;
	pj_bool_t pertenece_a_grupo_con_varias_ramas_del_mismo_tipo_que_esta = PJ_FALSE;
	pj_bool_t calc_ret = PJ_FALSE;													//Indica si hay que calcular retardos para esta radio
	pjmedia_session* session = NULL;

	if (wp == NULL) return;
	//if (!wp->valid_sess) return;

	if ((wp->Check_CLD_timer.id == Check_CLD_timer_IDLE) || (!wp->valid_sess))
	{
		wp->Check_CLD_timer.id = Check_CLD_timer_IDLE;
		pjsua_cancel_timer(&wp->Check_CLD_timer);
		return;
	}

	pjsua_call_info callInfo;
	pj_status_t ret = pjsua_call_get_info(wp->_Id, &callInfo);
	if ((ret == PJ_SUCCESS) && (callInfo.state != PJSIP_INV_STATE_CONFIRMED)) return;

	/*Se evalua si esta sesion del grupo necesita calculo de retardo*/

	if (ret == PJ_SUCCESS && ((wp->_Info.CallFlags & CORESIP_CALL_RD_IDLE) != CORESIP_CALL_RD_IDLE))
	{
		//Con sesion IDLE no se calcula retardo

		session = pjsua_call_get_media_session(wp->_Id);
		int nsession_rx_only = 0;
		int nsession_tx_only = 0;
		int nsessions_in_group = SipAgent::_FrecDesp->GetSessionsCountInGroup(wp->_Index_group, &nsession_rx_only, &nsession_tx_only);

		if ((session != NULL) && (SipAgent::ETM == PJ_TRUE))
		{
			calc_ret = PJ_TRUE;			//Si se es ETM se fuerza calculo de retardo
		}
		else if ((session != NULL) && (nsessions_in_group > 1))
		{
			if (wp->_Info.CallFlags & CORESIP_CALL_RD_RXONLY)
			{
				if (nsession_rx_only > 1) pertenece_a_grupo_con_varias_ramas_del_mismo_tipo_que_esta = PJ_TRUE;
				else if (nsessions_in_group > (nsession_rx_only + nsession_tx_only)) pertenece_a_grupo_con_varias_ramas_del_mismo_tipo_que_esta = PJ_TRUE;
			}
			else if (wp->_Info.CallFlags & CORESIP_CALL_RD_TXONLY)
			{
				if (nsession_tx_only > 1) pertenece_a_grupo_con_varias_ramas_del_mismo_tipo_que_esta = PJ_TRUE;
				else if (nsessions_in_group > (nsession_rx_only + nsession_tx_only)) pertenece_a_grupo_con_varias_ramas_del_mismo_tipo_que_esta = PJ_TRUE;
			}
			else
			{
				pertenece_a_grupo_con_varias_ramas_del_mismo_tipo_que_esta = PJ_TRUE;
			}

			if ((wp->_Info.CallFlags & CORESIP_CALL_RD_RXONLY) && (!wp->_Info.AudioSync))
			{
				//Si no se desea sincronizar el audio de recepcion de las distintas ramas para los receptores entonces 
				//se fuerza a que no se haga calculo de retardo al receptor
				calc_ret = PJ_FALSE;
			}
			else
			{
				calc_ret = pertenece_a_grupo_con_varias_ramas_del_mismo_tipo_que_esta;
			}
		}
	}

	pj_time_val	delay;
	delay.sec = (long)wp->_Info.cld_supervision_time;
	delay.msec = 0;

	if (wp->_Info.cld_supervision_time == 0)
	{
		//Si el tiempo de supervision de CLD es 0, no enviamos RMM, pero queremos que pase por este timer
		//para supervisar
		delay.sec = 1;
		delay.msec = 0;
	}
	else if ((ret == PJ_SUCCESS) && (calc_ret) && (wp->Check_CLD_timer.id == Check_CLD_timer_SEND_CLD))
	{
		pj_uint32_t rtp_ext_info = 0;
		pj_uint8_t cld = 0;

		if (SipAgent::_FrecDesp->GetCLD(wp->_Id, &cld) != -1)
		{
			if (SipAgent::ETM && wp->_Info.forced_cld >= 0)
			{
				//Forzamos el CLD configurado en la sesion
				pj_uint8_t cldtmp = 0;
				cldtmp = (pj_uint8_t)(wp->_Info.forced_cld / 2);	//Pasamos a unidades de 2 ms
				cldtmp &= 0x7F;
				cld &= 0x80;						//Nos quedamos con el bit de mayor peso
				cld |= cldtmp;
			}

			ret = pjsua_call_get_info(wp->_Id, &callInfo);
			if (ret != PJ_SUCCESS) return;
			if (callInfo.state != PJSIP_INV_STATE_CONFIRMED) return;

			pjmedia_stream* stream = NULL;
			session = pjsua_call_get_media_session(wp->_Id);
			if (session != NULL)
			{
				stream = pjmedia_session_get_stream(session, 0);

				if (stream != NULL)
				{
					pjmedia_stream_get_rtp_ext_tx_info(stream, &rtp_ext_info);
					PJMEDIA_RTP_RD_EX_SET_X(rtp_ext_info, 1);
					PJMEDIA_RTP_RD_EX_SET_TYPE(rtp_ext_info, 2);
					PJMEDIA_RTP_RD_EX_SET_LENGTH(rtp_ext_info, 1);
					PJMEDIA_RTP_RD_EX_SET_CLD(rtp_ext_info, (cld));
					pjmedia_stream_set_rtp_ext_tx_info(stream, rtp_ext_info);

					if (cld != 0)
						PJ_LOG(5, (__FILE__, "CLIMAX: SipCall::Check_CLD_timer_cb GetCLD uri %s CLD DISTINTO CERO %d ms", wp->DstUri, cld * 2));
					else
						PJ_LOG(5, (__FILE__, "CLIMAX: SipCall::Check_CLD_timer_cb GetCLD uri %s CLD %d ms", wp->DstUri, cld * 2));
				}
			}
		}
	}
	else if ((ret == PJ_SUCCESS) && (calc_ret) && (wp->Check_CLD_timer.id == Check_CLD_timer_SEND_RMM))
	{
		PJ_LOG(5, (__FILE__, "CLIMAX: SipCall::Check_CLD_timer_cb antes pjmedia_session_force_request_MAM uri %s", wp->DstUri));
		ret = pjsua_call_get_info(wp->_Id, &callInfo);
		if (ret != PJ_SUCCESS) return;
		if (callInfo.state != PJSIP_INV_STATE_CONFIRMED) return;
		session = pjsua_call_get_media_session(wp->_Id);
		if (session != NULL)
			pjmedia_session_force_request_MAM(session);
	}

	wp->Check_CLD_timer.id = Check_CLD_timer_IDLE;
	pjsua_cancel_timer(&wp->Check_CLD_timer);

	pj_timer_entry_init(&wp->Check_CLD_timer, Check_CLD_timer_SEND_RMM, (void*)wp, Check_CLD_timer_cb);
	pj_status_t st = pjsua_schedule_timer(&wp->Check_CLD_timer, &delay);
	if (st != PJ_SUCCESS)
	{
		wp->Check_CLD_timer.id = Check_CLD_timer_IDLE;
		PJ_CHECK_STATUS(st, ("ERROR en Check_CLD_timer"));
	}
}

/**
*
*/
void SipCall::OnKaTimeout(void* stream)
{
	if (SipAgent::Cb.KaTimeoutCb)
	{
		void* session = pjmedia_stream_get_user_data((pjmedia_stream*)stream);
		if (session)
		{
			void* call = pjmedia_session_get_user_data((pjmedia_session*)session);

			if (call)
			{
				/*
				SipCall * sipcall = (SipCall*)pjsua_call_get_user_data(((pjsua_call*)call)->index);
				if (sipcall != NULL)
				{
					PJ_LOG(5,(__FILE__, "OnKaTimeout dst %s", sipcall->DstUri));
				}
				*/

				SipAgent::Cb.KaTimeoutCb(((pjsua_call*)call)->index | CORESIP_CALL_ID);
			}
		}
	}
}

/*Timer que arranca despues de un ptt off*/
void SipCall::Ptt_off_timer_cb(pj_timer_heap_t* th, pj_timer_entry* te)
{
	SipCall* wp = (SipCall*)te->user_data;

	if (wp->Ptt_off_timer.id == 0)
	{
		pjsua_cancel_timer(&wp->Ptt_off_timer);
		return;
	}

	wp->Ptt_off_timer.id = 0;
	pjsua_cancel_timer(&wp->Ptt_off_timer);

	//Se actualiza en la aplicacion es estado rdinfo
	CORESIP_RdInfo info_aux;
	pj_mutex_lock(wp->RdInfo_prev_mutex);
	memcpy(&info_aux, &wp->RdInfo_prev, sizeof(CORESIP_RdInfo));
	pj_mutex_unlock(wp->RdInfo_prev_mutex);

	//Actualizamos los parametros que no se toman en la callback OnRdInfochanged
	//Esto hay que hacerlo siempre que se llame a RdInfoCb
	//-->
	info_aux.rx_selected = SipAgent::_FrecDesp->IsBssSelected(wp);
	//<--

	if (info_aux.Squelch == 0) info_aux.rx_qidx = 0;	//Si no hay squelch el qidx que se envia a la aplicacion es cero
	if (SipAgent::Cb.RdInfoCb)
	{
		PJ_LOG(5, (__FILE__, "SipCall::Ptt_off_timer_cb: Fin timer. Envia a nodebox dst %s PttType %d PttId %d rx_selected %d Squelch %d", wp->DstUri, info_aux.PttType, info_aux.PttId, info_aux.rx_selected, info_aux.Squelch));
		SipAgent::Cb.RdInfoCb(wp->_Id | CORESIP_CALL_ID, &info_aux);
	}
}

void SipCall::SetConfirmPtt(pjsua_call_id call_id, pj_bool_t val)
{
	pjsua_call* call;
	pjsip_dialog* dlg;
	pj_status_t st;

	st = acquire_call("SetConfirmPtt()", call_id, &call, &dlg);
	if (st != PJ_SUCCESS)
	{
		PJ_LOG(3, ("SipCall.cpp", "ERROR: SetConfirmPtt: Invalid call_id %d", call_id));
		return;
	}

	SipCall* sipcall = (SipCall*)pjsua_call_get_user_data(call_id);
	sipcall->_ConfirmPtt = val;

	pjmedia_session* session = pjsua_call_get_media_session(call_id);
	if (session != NULL)
	{
		pjmedia_stream* stream = NULL;
		stream = pjmedia_session_get_stream(session, 0);
		if (stream != NULL)
		{
			pj_uint32_t tx_rtp_ext_info;
			pjmedia_stream_get_rtp_ext_tx_info((pjmedia_stream*)stream, &tx_rtp_ext_info);

			if (sipcall->_ConfirmPtt == PJ_FALSE)
			{
				PJMEDIA_RTP_RD_EX_SET_PTT_TYPE(tx_rtp_ext_info, CORESIP_PTT_OFF);
			}
			else
			{
				PJMEDIA_RTP_RD_EX_SET_PTT_TYPE(tx_rtp_ext_info, sipcall->last_received_ptt_status);
			}
			pjmedia_stream_set_rtp_ext_tx_info((pjmedia_stream*)stream, tx_rtp_ext_info);
		}
	}

	pjsip_dlg_dec_lock(dlg);
}

/**
*	Cierra todas las sesiones que tenga activas un account
*	@param	pj_acc_id	Account Id.
*	@param	exclude_NoFreqDisconn. Si es true se excluyen las sesiones que tienen el parametro NoFreqDisconn
*	@return Numero de llamadas que se cuelgan.
*/
int SipCall::HangUpAllCallsOfAcc(pjsua_acc_id pj_acc_id, pj_bool_t exclude_NoFreqDisconn)
{
	int ret = 0;
	for (pjsua_call_id i = 0; i < (int)pjsua_var.ua_cfg.max_calls; ++i) {
		if (pjsua_var.calls[i].inv)
		{
			if (pjsua_var.calls[i].acc_id == pj_acc_id)
			{
				if (exclude_NoFreqDisconn)
				{
					SipCall* sipcall = (SipCall*)pjsua_call_get_user_data(i);
					if (sipcall)
					{
						if (sipcall->_Info.NoFreqDisconn == 0)
						{
							pjsua_call_hangup(i, 0, NULL, NULL);
							ret++;
						}
						else
						{
						}
					}
					else
					{
						pjsua_call_hangup(i, 0, NULL, NULL);
						ret++;
					}
				}
				else
				{
					pjsua_call_hangup(i, 0, NULL, NULL);
					ret++;
				}
			}
		}
	}
	return ret;
}

void SipCall::SendSELCAL(pjsua_call_id call_id, const char* selcalValue)
{
	if (call_id<0 || call_id>(int)pjsua_var.ua_cfg.max_calls)
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("SendInfoMsg:", "call_id %d no valido", call_id);
	}

	pjsua_call* call1;
	pjsip_dialog* dlg1;
	pj_status_t status = acquire_call("SendInfoMsg()", call_id, &call1, &dlg1);
	PJ_CHECK_STATUS(status, ("ERROR SendSELCAL adquiriendo call", "[Call=%d]", call_id));
	pjsua_call_info callInfo;
	if (pjsua_call_get_info(call_id, &callInfo) != PJ_SUCCESS)
	{
		pjsip_dlg_dec_lock(dlg1);
		PJ_CHECK_STATUS(status, ("ERROR SendSELCAL adquiriendo call", "[Call=%d]", call_id));
	}		

	SipCall* sipcall = (SipCall*)pjsua_call_get_user_data(call_id);
	if (!sipcall)
	{
		pjsip_dlg_dec_lock(dlg1);
		PJ_CHECK_STATUS(PJ_ENOTFOUND, ("ERROR SendSELCAL adquiriendo sipCall", "[Call=%d]", call_id));
	}

	if (callInfo.role != PJSIP_ROLE_UAC || sipcall->_Info.Type != CORESIP_CALL_RD)
	{
		pjsip_dlg_dec_lock(dlg1);
		PJ_CHECK_STATUS(PJ_EINVALIDOP, ("ERROR SendSELCAL SELCAL can only be sent from VCS to GRS", "[Call=%d]", call_id));
	}

	if (!sipcall->SelcalSupported)
	{
		pjsip_dlg_dec_lock(dlg1);
		PJ_CHECK_STATUS(PJ_ENOTFOUND, ("ERROR SendSELCAL This call session does not support SELCAL", "[Call=%d]", call_id));
	}	

	/*
	for (int i = 0; i < CORESIP_MAX_SELCAL_LENGTH; i++)
	{
		pj_bool_t found;
		found = PJ_FALSE;
		for (int j = 0; j < sizeof(Selcal_designators); j++)
		{
			if (selcalValue[i] == Selcal_designators[j]) found = PJ_TRUE;
		}
		if (!found)
		{
			pjsip_dlg_dec_lock(dlg1);
			PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR SendSELCAL ", "designator %c is not valid [Call=%d]", selcalValue[i], call_id));
		}
	}
	*/

	pjsua_msg_data msgData;
	pj_status_t st;
	pjsua_msg_data_init(&msgData);

	pjsip_generic_string_hdr info_package_hdr;
	pj_str_t info_package_name = pj_str("Info-Package");
	pj_str_t info_package_val = pj_str("selcal");
	pjsip_generic_string_hdr_init2(&info_package_hdr, &info_package_name, &info_package_val);
	pj_list_push_back(&msgData.hdr_list, &info_package_hdr);

	msgData.content_type = pj_str("application/selcal");
	char tones[] = "tones:";
	char info_msg[sizeof(tones)+ CORESIP_MAX_SELCAL_LENGTH];
	msgData.msg_body.ptr = info_msg;
	pj_ansi_strcpy(info_msg, tones);
	for (int i = 0; i < CORESIP_MAX_SELCAL_LENGTH; i++)
	{
		info_msg[pj_ansi_strlen(tones) + i] = selcalValue[i];
	}
	msgData.msg_body.slen = pj_ansi_strlen(tones) + CORESIP_MAX_SELCAL_LENGTH;

	static pj_str_t method = { "INFO", 4 };
	st = pjsua_call_send_request(call_id, &method, &msgData);
	pjsip_dlg_dec_lock(dlg1);
	PJ_CHECK_STATUS(st, ("ERROR SendSELCAL enviando INFO", "[Call=%d]", call_id));
}

pj_bool_t SipCall::ProcessReceivedSelcalInfo(pjsua_call_id call_id, pjsip_transaction* tsx, pjsip_event* e)
{
	pj_bool_t processed = PJ_FALSE;
	pjsua_call* pjcall;
	pjsip_dialog* dlg;
	pj_status_t status;

	SipCall* sipcall = (SipCall*)pjsua_call_get_user_data(call_id);
	if (!sipcall) return processed;

	status = acquire_call("ProcessReceivedSelcalInfo()", call_id, &pjcall, &dlg);
	if (status != PJ_SUCCESS) {
		return processed;
	}
	int answer = PJSIP_SC_OK;
	pj_str_t info_package = pj_str("Info-Package");
	pjsip_rx_data* rdata;
	rdata = pjcall->incoming_rdata;
	if (rdata == NULL)
	{
		answer = PJSIP_SC_BAD_REQUEST;
		processed = PJ_FALSE;
	}

	if (answer == PJSIP_SC_OK)
	{
		if (rdata->msg_info.msg == NULL)
		{
			answer = PJSIP_SC_BAD_REQUEST;
			processed = PJ_FALSE;
		}
	}

	pjsip_generic_string_hdr* info_package_hdr;
	if (answer == PJSIP_SC_OK)
	{
		info_package_hdr = (pjsip_subject_hdr*)pjsip_msg_find_hdr_by_name(rdata->msg_info.msg, &info_package, NULL);
		if (info_package_hdr == NULL)
		{
			answer = PJSIP_SC_BAD_REQUEST;
			processed = PJ_FALSE;
		}
	}

	if (answer == PJSIP_SC_OK)
	{
		if (pj_stricmp(&pj_str("selcal"), &info_package_hdr->hvalue) != 0)
		{
			answer = PJSIP_SC_BAD_REQUEST;
			processed = PJ_FALSE;
		}
	}

	if (answer == PJSIP_SC_OK)
	{
		//Debemos ser un GRS
		pjsua_call_info call_info;
		if (pjsua_call_get_info(call_id, &call_info) != PJ_SUCCESS)
		{
			answer = PJSIP_SC_BAD_REQUEST;
			processed = PJ_FALSE;
		}
		else
		{
			pj_bool_t rdAccount = PJ_FALSE;				//Indica si acc_id es un account tipo radio GRS
			AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(call_info.acc_id);
			if (accUserData != NULL)
			{
				rdAccount = accUserData->rdAccount;
			}

			if (!SipAgent::_Radio_UA && !rdAccount)
			{
				//No somos un GRS
				answer = PJSIP_SC_BAD_REQUEST;
				processed = PJ_TRUE;
			}
		}
	}

	if (answer == PJSIP_SC_OK)
	{
		if (rdata->msg_info.msg->body == NULL)
		{
			answer = PJSIP_SC_BAD_REQUEST;
			processed = PJ_FALSE;
		}
	}

	pj_str_t tones = pj_str("tones:");
	if (answer == PJSIP_SC_OK)
	{		
		if (pj_stricmp2(&rdata->msg_info.msg->body->content_type.type, "application") != 0 ||
			pj_stricmp2(&rdata->msg_info.msg->body->content_type.subtype, "selcal") != 0 ||
			rdata->msg_info.msg->body->data == NULL ||
			rdata->msg_info.msg->body->len != (tones.slen + CORESIP_MAX_SELCAL_LENGTH))
		{
			answer = PJSIP_SC_BAD_REQUEST;
			processed = PJ_FALSE;
		}
	}

	pj_str_t data_s;
	if (answer == PJSIP_SC_OK)
	{		
		data_s.ptr = (char*)rdata->msg_info.msg->body->data;
		data_s.slen = rdata->msg_info.msg->body->len;
		if (pj_strnicmp(&data_s, &tones, tones.slen) != 0)
		{
			answer = PJSIP_SC_BAD_REQUEST;
			processed = PJ_FALSE;
		}
	}

	if (answer == PJSIP_SC_OK)
	{
		pj_bool_t found = PJ_FALSE;
		for (int i = 0; i < CORESIP_MAX_SELCAL_LENGTH; i++)
		{
			found = PJ_FALSE;
			for (int j = 0; j < sizeof(Selcal_designators); j++)
			{
				if (data_s.ptr[i + tones.slen] == Selcal_designators[j]) found = PJ_TRUE;
			}
			if (!found) break;
		}
		if (found)
		{
			//SELCALL INFO RECIBIDO CORRECTAMENTE
			processed = PJ_TRUE;
			answer = PJSIP_SC_OK;

			if (sipcall->last_received_ptt_status == CORESIP_PTT_OFF && 
				sipcall->radio_session_with_pttid_enabled && 
				sipcall->SELCAL_timer.id == 0)
			{
				//Solo se activa SELCAL si el GRS no recibe PTT y el PTT-id lo tiene activado
				//Es decir no es una sesion Rx-only ni idle
				//Si hay otro en progreso no comienza otro nuevo
				pjmedia_stream* stream = NULL;
				pjmedia_session* session = pjsua_call_get_media_session(call_id);
				if (session != NULL)
				{
					stream = pjmedia_session_get_stream(session, 0);
				}
				if (stream != NULL)
				{
					pjmedia_stream_set_SelCalInProgress(stream, PJ_TRUE);

					//Arrancamos el timer que desactiva el SELCAL simulado
					sipcall->SELCAL_timer.cb = SELCAL_timer_cb;
					sipcall->SELCAL_timer.user_data = (void*)sipcall;
					pj_time_val	delay1;		//La transmision SELCAL dura 2.2 segundos
					delay1.sec = (long)2;
					delay1.msec = (long)400;
					sipcall->SELCAL_timer.id = 1;
					pj_status_t st = pjsua_schedule_timer(&sipcall->SELCAL_timer, &delay1);
					if (st != PJ_SUCCESS)
					{
						sipcall->PTT_delayed_timer.id = 0;
						PJ_LOG(3, ("SipCall.cpp", "ERROR: ProcessReceivedSelcalInfo: SELCAL_timer no se puede arrancar"));
					}					
				}
			}
		}
		else
		{
			answer = PJSIP_SC_BAD_REQUEST;
			processed = PJ_TRUE;
		}
	}

	if (processed && rdata != NULL)
	{
		pjsip_dialog* dlg = pjsip_tsx_get_dlg(tsx);
		if (dlg != NULL)
		{
			pjsip_rx_data* rdata = e->body.tsx_state.src.rdata;
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

	pjsip_dlg_dec_lock(dlg);
	return processed;
}

void SipCall::SELCAL_timer_cb(pj_timer_heap_t* th, pj_timer_entry* te)
{
	SipCall* sipcall = (SipCall*)te->user_data;
	pjsua_cancel_timer(&sipcall->SELCAL_timer);
	sipcall->SELCAL_timer.id = 0;

	pjsua_call* pjcall;
	pjsip_dialog* dlg;
	pj_status_t status = acquire_call("SELCAL_timer_cb()", sipcall->_Id, &pjcall, &dlg);
	if (status != PJ_SUCCESS) {
		return;
	}
	pjmedia_stream* stream = NULL;
	pjmedia_session* session = pjsua_call_get_media_session(sipcall->_Id);
	if (session != NULL)
	{
		stream = pjmedia_session_get_stream(session, 0);
	}
	if (stream != NULL)
	{
		pjmedia_stream_set_SelCalInProgress(stream, PJ_FALSE);
	}

	pjsip_dlg_dec_lock(dlg);
}

void SipCall::SetSyncBss(int qidx)
{
	pj_mutex_lock(bss_rx_mutex);
	if (index_bss_rx_w < (MAX_BSS_SQU - 1))
	{
		bss_rx[index_bss_rx_w] = qidx;
		index_bss_rx_w++;
	}
	pj_mutex_unlock(bss_rx_mutex);
}

void SipCall::InitSyncBss()
{
	pj_mutex_lock(bss_rx_mutex);
	index_bss_rx_w = 0;
	pj_mutex_unlock(bss_rx_mutex);
}

void SipCall::GetRdQidx(int call, int* Qidx)
{
	pjsua_call* pjcall;
	pjsip_dialog* dlg;
	if (Qidx == NULL)
	{
		PJ_CHECK_STATUS(PJ_EINVAL, ("GetRdResourceInfo: ERROR. Qidx must not be NULL", "[Call=%d]", call));
		return;
	}
	pj_status_t status = acquire_call("GetRdQidx()", call, &pjcall, &dlg);
	if (status != PJ_SUCCESS) {
		PJ_CHECK_STATUS(status, ("GetRdResourceInfo: ERROR adquiriendo call", "[Call=%d]", call));
		return;
	}

	SipCall* sipCall = (SipCall*)pjcall->user_data;
	if (sipCall != NULL)
	{
		*Qidx = SipAgent::_FrecDesp->GetAirQidxInWindows(sipCall->_Index_group, sipCall->_Index_sess);
	}

	pjsip_dlg_dec_lock(dlg);
}

//Retorna el Bss teniendo en cuenta el retardo aplicado para la sincronizaci�n en la ventana de decision
int SipCall::GetSyncBss()
{
#if 0
#ifdef _DEBUG
	if (strcmp(DstUri, "<sip:RX-01@192.168.1.18:5060>") == 0) return 15;
	if (strcmp(DstUri, "<sip:RX-02@192.168.1.18:5060>") == 0) return 14;
	if (strcmp(DstUri, "<sip:RX-03@192.168.1.18:5060>") == 0) return 13;
	if (strcmp(DstUri, "<sip:RX-04@192.168.1.18:5060>") == 0) return 12;
#endif
#endif

	int bss = 0;
	if (Retardo == 0)
	{
		//No se ha aplicado retardo. Por tanto se retorna el ultimo bss recibido
		pj_mutex_lock(bss_rx_mutex);
		if (index_bss_rx_w > 0)
		{
			//Se ha recibido alg�n bss
			bss = bss_rx[index_bss_rx_w - 1];
		}
		pj_mutex_unlock(bss_rx_mutex);
		return bss;
	}

	//El indice del bss que buscamos, dentro del array bss_rx, es el ultimo menos el correspondiente al retardo.
	//El Retardo esta en unidades de 125us y el bss se almacena en el array cada vez que se ejecuta la funcion OnRdRtp
	//es decir, cada 10 ms
	int dif_index_bss = Retardo / (SAMPLES_PER_FRAME_RTP / 2);

	pj_mutex_lock(bss_rx_mutex);
	if ((index_bss_rx_w - 1) > dif_index_bss)
	{
		bss = bss_rx[(index_bss_rx_w - 1) - dif_index_bss];
	}
	else
	{
		bss = bss_rx[0];
	}
	pj_mutex_unlock(bss_rx_mutex);

	return bss;
}










