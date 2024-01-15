/**
 * @file OptionsMod.cpp
 * @brief Gestion de los mensajes OPTIONS.
 *
 *	Implementa la clase 'OptionsMod'.
 *
 *	@addtogroup CORESIP
 */
 /*@{*/


#include "Global.h"
#include "Exceptions.h"
#include "SipAgent.h"
#include "OptionsMod.h"

/**
 * options_mod: Estructura de configuracion del Módulo.
 * - Informacion: http://www.pjsip.org/pjsip/docs/html/structpjsip__module.htm
 */
static pjsip_module options_mod =
{
	NULL, NULL,										/* prev, next.			*/
	{ "mod-coresip-options", 19 },					/* Name.				*/
	-1,												/* Id				*/
	PJSIP_MOD_PRIORITY_APPLICATION + 1,				/* Priority			*/
	NULL,											/* load()				*/
	NULL,											/* start()				*/
	NULL,											/* stop()				*/
	NULL,											/* unload()				*/
	&OptionsMod::OnRxRequest,						/* on_rx_request()			*/
	&OptionsMod::OnRxResponse,						/* on_rx_response()			*/
	NULL,											/* on_tx_request.			*/
	NULL,											/* on_tx_response()			*/
	NULL,											/* on_tsx_state()			*/
};


const char* OptionsMod::C_TYPE_CFWR = "application";
const char* OptionsMod::C_SUBTYPE_CFWR_REQUEST = "wg67.cfwd.basic.request+xml";
const char* OptionsMod::C_SUBTYPE_CFWR_RELEASE = "wg67.cfwd.basic.release+xml";
const char* OptionsMod::C_SUBTYPE_CFWR_UPDATE = "wg67.cfwd.basic.update+xml";

const char* OptionsMod::C_TYPESUBTYPE_CFWR_REQUEST = "application/wg67.cfwd.basic.request+xml";
const char* OptionsMod::C_TYPESUBTYPE_CFWR_RELEASE = "application/wg67.cfwd.basic.release+xml";
const char* OptionsMod::C_TYPESUBTYPE_CFWR_UPDATE = "application/wg67.cfwd.basic.update+xml";

pj_status_t OptionsMod::RegisterModule()
{
	return pjsip_endpt_register_module(pjsua_var.endpt, &options_mod);
}

/**
 * SendOptionsMsg.	...
 * Envia OPTIONS
 * @param	target				Uri a la que se envia OPTIONS
 * @param	callid				callid que retorna.
 * @param	isRadio		Si tiene valor distinto de cero el agente se identifica como radio. Si es cero, como telefonia.
 *						Sirve principalmente para poner radio.01 o phone.01 en la cabecera WG67-version
 * @param	by_proxy			TRUE si queremos que se envie a través del proxy. Agregara cabecera route
 */
void OptionsMod::SendOptionsMsg(const char* target, char* callid, int isRadio, pj_bool_t by_proxy)
{
	pjsip_tx_data* tdata;
	pj_str_t to = pj_str(const_cast<char*>(target));
	pjsua_acc_id acc_id = PJSUA_INVALID_ID;
	pj_str_t callId;

	acc_id = pjsua_acc_get_default();
	if (!pjsua_acc_is_valid(acc_id))
	{
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: SendOptionsMsg: Default account no valido"));
		return;
	}

	/*Se comprueba si la URI es valida*/
	pj_bool_t urivalida = PJ_TRUE;
	pj_pool_t* pool = pjsua_pool_create(NULL, 256, 32);
	if (pool == NULL)
	{
		PJ_CHECK_STATUS(PJ_ENOMEM, ("ERROR: SendOptionsMsg: No se puede crear pj_pool"));
		return;
	}

	/*Se crea un string duplicado para el parse, ya que se ha visto que
	pjsip_parse_uri puede modificar el parametro de entrada*/
	pj_str_t uri_dup;
	pj_strdup_with_null(pool, &uri_dup, &to);
	pjsip_uri* to_uri = pjsip_parse_uri(pool, uri_dup.ptr, uri_dup.slen, 0);

	if (to_uri == NULL)
		urivalida = PJ_FALSE;
	else if (!PJSIP_URI_SCHEME_IS_SIP(to_uri) &&
		!PJSIP_URI_SCHEME_IS_SIPS(to_uri))
		urivalida = PJ_FALSE;


	if (!urivalida)
	{
		//target no es una uri valida
		pj_pool_release(pool);
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: La URI a la que se intenta enviar OPTIONS no es valida"));
		return;
	}

	pj_create_unique_string(pool, &callId);

	pj_status_t st;
	if (callId.slen > (CORESIP_MAX_CALLID_LENGTH - 1))
	{
		pj_pool_release(pool);
		st = PJ_EINVAL;
		PJ_CHECK_STATUS(st, ("ERROR creando mensaje OPTIONS. CallId generado es demasiado largo", "[Target=%s]", target));
		return;
	}

	strncpy(callid, callId.ptr, callId.slen);
	callid[callId.slen] = '\0';

	/*Se envia el request*/
	st = pjsip_endpt_create_request(pjsua_var.endpt, &pjsip_options_method,
		&to, &pjsua_var.acc[acc_id].cfg.id, &to, NULL, &callId, -1, NULL, &tdata);

	pj_pool_release(pool);
	PJ_CHECK_STATUS(st, ("ERROR creando mensaje OPTIONS", "[Target=%s]", target));

	if (by_proxy)
	{
		pjsua_acc* acc = &pjsua_var.acc[acc_id];
		if (!pj_list_empty(&acc->route_set))
		{
			pjsua_set_msg_route_set(tdata, &acc->route_set);
		}
#if 0
		else if (pj_strlen(&acc->cfg.reg_uri) > 0)
		{
			//Se fuerza la cabecera Route al Proxy
			/* If we want the initial INVITE to travel to specific SIP proxies,
				* then we should put the initial dialog's route set here. The final
				* route set will be updated once a dialog has been established.
				* To set the dialog's initial route set, we do it with something
				* like this:*/

			pjsip_route_hdr route_set;
			pjsip_route_hdr* route;
			const pj_str_t hname = { "Route", 5 };
			char uri[256];

			//char *uri = "sip:proxy.server;lr";

			if (acc->cfg.reg_uri.slen >= sizeof(uri))
			{
				acc->cfg.reg_uri.slen = sizeof(uri) - 1;
			}
			strncpy(uri, acc->cfg.reg_uri.ptr, acc->cfg.reg_uri.slen);
			uri[acc->cfg.reg_uri.slen] = '\0';
			strcat(uri, ";lr");

			pj_list_init(&route_set);

			route = (pjsip_route_hdr*)pjsip_parse_hdr(tdata->pool, &hname,
				uri, strlen(uri),
				NULL);
			if (route != NULL)
				pj_list_push_back(&route_set, route);

			pjsua_set_msg_route_set(tdata, &route_set);

			/*
			* Note that Route URI SHOULD have an ";lr" parameter!
			*/
		}
#endif
	}

	//Establecemos aqui la cabecera WG67-version con la version correspondiente.
	//En la funcion OnTxRequest de SipCall.cpp es donde se establece esta cabecera para todos
	//los paquetes SIP, siempre y cuando no se hayan establecido antes, como es este el caso.
	char ED137Radioversion_, ED137Phoneversion_;
	pjsip_endpt_Get_Ed137_version(pjsua_var.endpt, &ED137Radioversion_, &ED137Phoneversion_);
	char sWG67ver[32];
	sWG67ver[0] = '\0';
	if (isRadio)
	{		
		if (ED137Radioversion_ == 'C')
			pj_ansi_sprintf(sWG67ver, "%.*s, %.*s", SipCall::gWG67VersionRadioValue.slen, SipCall::gWG67VersionRadioValue.ptr,
				SipCall::gWG67VersionRadioValueC.slen, SipCall::gWG67VersionRadioValueC.ptr);
		else
			pj_ansi_sprintf(sWG67ver, "%.*s", SipCall::gWG67VersionRadioValue.slen, SipCall::gWG67VersionRadioValue.ptr);
	}
	else
	{
		if (ED137Radioversion_ == 'C')
			pj_ansi_sprintf(sWG67ver, "%.*s, %.*s", SipCall::gWG67VersionTelefValue.slen, SipCall::gWG67VersionTelefValue.ptr,
				SipCall::gWG67VersionTelefValueC.slen, SipCall::gWG67VersionTelefValueC.ptr);
		else
			pj_ansi_sprintf(sWG67ver, "%.*s", SipCall::gWG67VersionTelefValue.slen, SipCall::gWG67VersionTelefValue.ptr);
	}

	pj_str_t pjsWG67ver;
	pjsWG67ver.ptr = sWG67ver;
	pjsWG67ver.slen = pj_ansi_strlen(sWG67ver);
	SipCall::Wg67VersionSet(tdata, &pjsWG67ver);

	st = pjsip_endpt_send_request_stateless(pjsua_var.endpt, tdata, NULL, NULL);
	if (st != PJ_SUCCESS)
	{
		pjsip_tx_data_dec_ref(tdata);
	}
	PJ_CHECK_STATUS(st, ("ERROR enviando mensaje OPTIONS", "[Target=%s]", target));
}

/**
 * SendOptionsCFWD.	...
 * Envia mensaje OPTIONS necesario para la negociacion Call Forward
 * @param	accId				Account de la Coresip que utilizamos.
 * @param	target				Uri a la que se envia OPTIONS
 * @param	cfwr_options_type	Tipo de OPTIONS para la negociacion. Es del tipo CORESIP_CFWR_OPT_TYPE
 * @param	body				Contenido del body (XML). Acabado en '\0'
 * @param	callid				callid que se retorna, acabado en '\0'.
 * @param	by_proxy			TRUE si queremos que se envie a través del proxy. Agregara cabecera route
 */
void OptionsMod::SendOptionsCFWD(int accId, const char* target, CORESIP_CFWR_OPT_TYPE cfwr_options_type, const char* body, char* callid, pj_bool_t by_proxy)
{
	pjsip_tx_data* tdata;
	pj_str_t to = pj_str(const_cast<char*>(target));
	pj_str_t callId;
	pj_status_t st;

	char ED137Radioversion_, ED137Phoneversion_;
	pjsip_endpt_Get_Ed137_version(pjsua_var.endpt, &ED137Radioversion_, &ED137Phoneversion_);
	char sWG67ver[32];
	sWG67ver[0] = 0;
	if (ED137Phoneversion_ == 'C')
		pj_ansi_sprintf(sWG67ver, "%.*s", SipCall::gWG67VersionTelefValueC.slen, SipCall::gWG67VersionTelefValueC.ptr);
	else
	{
		PJ_CHECK_STATUS(PJ_EINVALIDOP, ("ERROR: SendOptionsCFWD: Call forward solo se permite en ED137C y el agente esta configurado como ED137B"));
	}

	if (!pjsua_acc_is_valid(accId))
	{
		pj_status_t st = PJ_EINVAL;
		PJ_CHECK_STATUS(st, ("ERROR: SendOptionsCFWD ", "Account %d no es valida", accId));
		return;
	}

	pj_str_t accept_value;
	pj_str_t ctype = pj_str(const_cast<char*>(C_TYPE_CFWR));
	pj_str_t csubtype;
	switch (cfwr_options_type)
	{
	case CORESIP_CFWR_OPT_REQUEST:
		accept_value = pj_str(const_cast<char*>(C_TYPESUBTYPE_CFWR_REQUEST));
		csubtype = pj_str(const_cast<char*>(C_SUBTYPE_CFWR_REQUEST));
		break;
	case CORESIP_CFWR_OPT_RELEASE:
		accept_value = pj_str(const_cast<char*>(C_TYPESUBTYPE_CFWR_RELEASE));
		csubtype = pj_str(const_cast<char*>(C_SUBTYPE_CFWR_RELEASE));
		break;
	case CORESIP_CFWR_OPT_UPDATE:
		accept_value = pj_str(const_cast<char*>(C_TYPESUBTYPE_CFWR_UPDATE));
		csubtype = pj_str(const_cast<char*>(C_SUBTYPE_CFWR_UPDATE));
		break;
	default:
		pj_status_t st = PJ_EINVAL;
		PJ_CHECK_STATUS(st, ("ERROR: SendOptionsCFWD", "cfwr_options_type %d no valido", cfwr_options_type));
		return;
	}

	/*Se comprueba si la URI es valida*/
	pj_bool_t urivalida = PJ_TRUE;
	pj_pool_t* pool = pjsua_pool_create(NULL, 256, 32);
	if (pool == NULL)
	{
		PJ_LOG(3, (__FILE__, "ERROR: SendOptionsCFWD: No se puede crear pj_pool"));
		pj_status_t st = PJ_EINVAL;
		PJ_CHECK_STATUS(st, ("ERROR: SendOptionsCFWD: No se puede crear pj_pool"));
		return;
	}

	/*Se crea un string duplicado para el parse, ya que se ha visto que
	pjsip_parse_uri puede modificar el parametro de entrada*/
	pj_str_t uri_dup;
	pj_strdup_with_null(pool, &uri_dup, &to);
	pjsip_uri* to_uri = pjsip_parse_uri(pool, uri_dup.ptr, uri_dup.slen, 0);

	if (to_uri == NULL)
		urivalida = PJ_FALSE;
	else if (!PJSIP_URI_SCHEME_IS_SIP(to_uri) &&
		!PJSIP_URI_SCHEME_IS_SIPS(to_uri))
		urivalida = PJ_FALSE;

	if (!urivalida)
	{
		//target no es una uri valida
		PJ_LOG(3, (__FILE__, "ERROR: SendOptionsCFWD: La URI a la que se intenta enviar OPTIONS no es valida: %s", target));
		pj_pool_release(pool);
		pj_status_t st = PJ_EINVAL;
		PJ_CHECK_STATUS(st, ("ERROR: SendOptionsCFWD: La URI a la que se intenta enviar OPTIONS no es valida", "[Target=%s]", target));
		return;
	}

	pj_create_unique_string(pool, &callId);
		
	if (callId.slen > (CORESIP_MAX_CALLID_LENGTH - 1))
	{
		pj_pool_release(pool);
		st = PJ_EINVAL;
		PJ_CHECK_STATUS(st, ("ERROR: SendOptionsCFWD. CallId generado es demasiado largo", "[Target=%s]", target));
		return;
	}

	if (callid != NULL)
	{
		strncpy(callid, callId.ptr, callId.slen);
		callid[callId.slen] = '\0';
	}

	/*Se envia el request*/
	st = pjsip_endpt_create_request(pjsua_var.endpt, &pjsip_options_method,
		&to, &pjsua_var.acc[accId].cfg.id, &to, NULL, &callId, -1, NULL, &tdata);

	pj_pool_release(pool);
	PJ_CHECK_STATUS(st, ("ERROR: SendOptionsCFWD: creando mensaje OPTIONS", "[Target=%s]", target));

	if (by_proxy)
	{
		pjsua_acc* acc = &pjsua_var.acc[accId];
		if (!pj_list_empty(&acc->route_set))
		{
			pjsua_set_msg_route_set(tdata, &acc->route_set);
		}
	}

	/* Add Body*/
	if (body != NULL)
	{
		pj_str_t body_str;
		body_str.ptr = (char*)body;
		body_str.slen = (pj_ssize_t) strlen(body);
		tdata->msg->body = pjsip_msg_body_create(tdata->pool, &ctype, &csubtype, &body_str);
		if (tdata->msg->body == NULL)
		{
			pjsip_tx_data_dec_ref(tdata);
			st = PJ_EINVAL;
			PJ_CHECK_STATUS(st, ("ERROR: SendOptionsCFWD. No se puede crear el Body del mensaje posiblemente por falta de memoria", "[Target=%s]", target));
			return;
		}
	}

	pj_str_t accept_name = { "Accept", 6 };
	pjsip_generic_string_hdr* pAcceptHdr = pjsip_generic_string_hdr_create(tdata->pool, &accept_name, &accept_value);
	pj_list_push_back(&tdata->msg->hdr, pAcceptHdr);

	//Establecemos aqui la cabecera WG67-version con la version correspondiente.
	//En la funcion OnTxRequest de SipCall.cpp es donde se establece esta cabecera para todos
	//los paquetes SIP, siempre y cuando no se hayan establecido antes, como es este el caso.	
	pj_str_t pjsWG67ver;
	pjsWG67ver.ptr = sWG67ver;
	pjsWG67ver.slen = pj_ansi_strlen(sWG67ver);
	SipCall::Wg67VersionSet(tdata, &pjsWG67ver);

	st = pjsip_endpt_send_request(pjsua_var.endpt, tdata, pjsip_sip_cfg_var.tsx.tsx_tout, NULL, &send_CFWD_callback); 
	if (st != PJ_SUCCESS)
	{
		pjsip_tx_data_dec_ref(tdata);
	}
	PJ_CHECK_STATUS(st, ("ERROR SendOptionsCFWD: enviando mensaje OPTIONS", "[Target=%s]", target));
}

void OptionsMod::send_CFWD_callback(void *token, pjsip_event *e)
{
	static const pj_str_t STR_CFWR_PREFIX = { "application/wg67.cfwd.basic", 27 };

	char dstUri[CORESIP_MAX_URI_LENGTH + 1] = { 0 };
	char callid[CORESIP_MAX_CALLID_LENGTH + 1] = { 0 };

	if (e->type == PJSIP_EVENT_TSX_STATE) 
	{
		pjsip_transaction *tsx = e->body.tsx_state.tsx;		
		pjsip_msg* msg_tx = tsx->last_tx->msg;
		pjsip_msg* msg_rx = e->body.rx_msg.rdata->msg_info.msg;

		//Si es un 200 entonces procesamos el mensaje recibido, si no  entonces tomamos los
		//parametros del mensaje que se transmitio
		pjsip_msg* msg = msg_tx;
		if ((tsx->status_code == 200) && (e->body.rx_msg.rdata != NULL))
		{
			msg = msg_rx;
		}

		pjsip_cid_hdr* callid_hdr = (pjsip_cid_hdr*)pjsip_msg_find_hdr(msg, PJSIP_H_CALL_ID, NULL);
		if (callid_hdr->id.slen < (sizeof(callid)-1))
		{
			strncpy(callid, callid_hdr->id.ptr, callid_hdr->id.slen);
		}
		else
		{
			PJ_LOG(3, (__FILE__, "ERROR: OptionsMod::send_CFWD_callback: Call ID es demasiado grande"));
			return;
		}

		pjsip_to_hdr* to_hdr = (pjsip_to_hdr*)pjsip_msg_find_hdr(msg, PJSIP_H_TO, NULL);
		pjsip_sip_uri* dst = (pjsip_sip_uri*)pjsip_uri_get_uri(to_hdr->uri);
		if (dst->user.slen > 0)
		{
			//La uri destino del OPTIONS tiene usuario
			pj_ansi_snprintf(dstUri, sizeof(dstUri) - 1, "<sip:%.*s@%.*s>",
				dst->user.slen, dst->user.ptr, dst->host.slen, dst->host.ptr);
		}
		else
		{
			//La uri destino del OPTIONS no tiene usuario
			pj_ansi_snprintf(dstUri, sizeof(dstUri) - 1, "<sip:%.*s>",
				dst->host.slen, dst->host.ptr);
		}

		pjsip_from_hdr* from_hdr = (pjsip_from_hdr*)pjsip_msg_find_hdr(msg, PJSIP_H_FROM, NULL);
		pjsip_uri* sip_from_uri = (pjsip_uri*)pjsip_uri_get_uri(from_hdr->uri);		
		pjsip_sip_uri* sip_from_uri2 = (pjsip_sip_uri*) sip_from_uri;

		pjsua_acc_id acc_id_for_incomming = pjsua_acc_find_for_incoming_by_uri(sip_from_uri);
		if (!pjsua_acc_is_valid(acc_id_for_incomming))
		{
			return;
		}
		if (acc_id_for_incomming == pjsua_var.default_acc)
		{
			//Si es la default puede ser que no ha encontrado el abonado
			if (pj_stricmp(&pjsua_var.acc[acc_id_for_incomming].user_part, &sip_from_uri2->user) != 0)
			{
				//El user part del options no coincide con el default. Por tanto el From del options no corresponde a ningun usuario
				//No se por que he recibido esta respuesta
				return;
			}
		}

		pj_bool_t valid_accept = PJ_FALSE;
		pjsip_accept_hdr* accept_hdr = (pjsip_accept_hdr*)pjsip_msg_find_hdr(msg, PJSIP_H_ACCEPT, NULL);
		if (accept_hdr != NULL)
		{
			for (unsigned int i = 0; i < accept_hdr->count; i++)
			{
				if (pj_strnicmp(&accept_hdr->values[i], &STR_CFWR_PREFIX, STR_CFWR_PREFIX.slen) == 0) 
				{
					valid_accept = PJ_TRUE;
					break;
				}
			}
		}

		CORESIP_CFWR_OPT_TYPE cfwr_options_type = CORESIP_CFWR_OPT_REQUEST;
		char body[CORESIP_MAX_BODY_LEN];
		body[0] = '\0';

		if (!valid_accept)
		{
			//La respuesta al options no se corresponde con las del tipo call forward. 
			//Puede que hay respondido un agente que no lo soporta
			//El puntero del mensaje lo ponemos en el del tx para obtener cfwr_options_type
			msg = msg_tx;
		}
				
		pj_str_t PJ_SUBTYPE_CFWR_REQUEST = pj_str((char*) C_SUBTYPE_CFWR_REQUEST);
		pj_str_t PJ_SUBTYPE_CFWR_RELEASE = pj_str((char*) C_SUBTYPE_CFWR_RELEASE);
		pj_str_t PJ_SUBTYPE_CFWR_UPDATE = pj_str((char*) C_SUBTYPE_CFWR_UPDATE);

		if (msg->body != NULL)
		{
			if (pj_stricmp(&msg->body->content_type.subtype, &PJ_SUBTYPE_CFWR_REQUEST) == 0)
			{
				cfwr_options_type = CORESIP_CFWR_OPT_REQUEST;
			}
			else if (pj_stricmp(&msg->body->content_type.subtype, &PJ_SUBTYPE_CFWR_RELEASE) == 0)
			{
				cfwr_options_type = CORESIP_CFWR_OPT_RELEASE;
			}
			else if (pj_stricmp(&msg->body->content_type.subtype, &PJ_SUBTYPE_CFWR_UPDATE) == 0)
			{
				cfwr_options_type = CORESIP_CFWR_OPT_UPDATE;
			}
			else
			{
				PJ_LOG(3, (__FILE__, "ERROR: OptionsMod::send_CFWD_callback: Transmitido OPTIONS del tipo Call forward con un Content-Type no valido"));
				return;
			}			
		}

		if (msg->body != NULL)
		{
			int body_len = pjsip_print_text_body(msg->body, body, sizeof(body) - 1);
			if (body_len <= 0)
			{
				PJ_LOG(3, (__FILE__, "ERROR: send_CFWD_callback: El Body recibido es demasiado grande"));
				return;
			}
			body[body_len] = '\0';
		}	

		if (!valid_accept)
		{
			//La respuesta al options no se corresponde con las del tipo call forward
			//Enviamos en la callback un body vacio, el code era bad request
			body[0] = '\0';
			if (SipAgent::Cb.CfwrOptResponseCb)
			{
				if (TRACE_ALL_CALLS) PJ_LOG(3, (__FILE__, "CfwrOptResponseCb"));
				SipAgent::Cb.CfwrOptResponseCb(acc_id_for_incomming | CORESIP_ACC_ID, dstUri, callid, PJSIP_SC_BAD_REQUEST, cfwr_options_type, body);
			}
		}
		else
		{
			if (SipAgent::Cb.CfwrOptResponseCb)
			{
				if (TRACE_ALL_CALLS) PJ_LOG(3, (__FILE__, "CfwrOptResponseCb"));
				SipAgent::Cb.CfwrOptResponseCb(acc_id_for_incomming | CORESIP_ACC_ID, dstUri, callid, tsx->status_code, cfwr_options_type, body);
			}
		}
	}
}

/**
 * OnRxResponse:	Callback. Se invoca cuando se recibe una request SIP para este modulo
 *					Esta funcion se llama cuando se recibe un options que rechaza el funcion options_on_rx_request
 *					del modulo pjsua_options_handler. Como por ejemplo cuando es un options de call forward
 * @param	rdata		Puntero 'pjsip_rx_data' a los datos recibidos.
 * @return	'pj_bool_t' Siempre true si tiene exito.
 *
 */
pj_bool_t OptionsMod::OnRxRequest(pjsip_rx_data* rdata)
{
	static const pj_str_t STR_APPLICATION = { "application", 11 };
	static const pj_str_t STR_CFWR_XML = { "wg67.cfwd.basic", 15 };

	/* Only want to handle OPTIONS requests */
	if (pjsip_method_cmp(&rdata->msg_info.msg->line.req.method,
		pjsip_get_options_method()) != 0)
	{
		return PJ_FALSE;
	}

	/* Don't want to handle if shutdown is in progress */
	if (pjsua_var.thread_quit_flag) {
		pjsip_endpt_respond_stateless(pjsua_var.endpt, rdata,
			PJSIP_SC_TEMPORARILY_UNAVAILABLE, NULL,
			NULL, NULL);
		return PJ_TRUE;
	}

	pjsip_ctype_hdr* content_type = rdata->msg_info.ctype;
	if (content_type != NULL)
	{
		if (pj_stricmp(&content_type->media.type, &STR_APPLICATION) == 0 &&
			pj_strnicmp(&content_type->media.subtype, &STR_CFWR_XML, STR_CFWR_XML.slen) == 0)
		{
			//Se trata de un OPTIONS para la negociacion de Call Forward.
			return ProcessOptCfwrRequest(rdata);
		}
	}

	return PJ_FALSE;
}

/**
 * ProcessOptCfwrRequest:	
 *					Esta funcion se llama cuando se recibe un options del tipo utilizado para
 *					la negociacion Call Forward
 * @param	rdata		Puntero 'pjsip_rx_data' a los datos recibidos.
 * @return	'pj_bool_t' Siempre true si tiene exito.
 *
 */
pj_bool_t OptionsMod::ProcessOptCfwrRequest(pjsip_rx_data* rdata)
{
	pjsip_tx_data* tdata;
	pjsip_response_addr res_addr;
	pj_status_t status;
	pjsua_acc_id acc_id_for_incomming;
	pjsip_uri* uri;
	pjsip_sip_uri* sip_uri;
	int st_code = 200;
	pj_str_t *pst_text = NULL;
	pj_str_t st_text;

	/*Comprueba si la cabecera To tiene un usuario correcto*/
	uri = rdata->msg_info.to->uri;
	if (uri == NULL) return PJ_TRUE;

	if (!PJSIP_URI_SCHEME_IS_SIP(uri) && !PJSIP_URI_SCHEME_IS_SIPS(uri))
	{
		//La uri no es tipo SIP:
		st_code = 404;
	}

	pj_str_t PJ_CTYPE_CFWR_REQUEST = pj_str((char*)C_TYPESUBTYPE_CFWR_REQUEST);
	pj_str_t PJ_CTYPE_CFWR_RELEASE = pj_str((char*)C_TYPESUBTYPE_CFWR_RELEASE);
	pj_str_t PJ_CTYPE_CFWR_UPDATE = pj_str((char*)C_TYPESUBTYPE_CFWR_UPDATE);

	pjsip_accept_hdr* accept = (pjsip_accept_hdr*)pjsip_msg_find_hdr(rdata->msg_info.msg, PJSIP_H_ACCEPT, NULL);
	pj_bool_t accept_valido = PJ_FALSE;
	if (accept != NULL)
	{
		for (unsigned int i = 0; i < accept->count; ++i)
		{
			if ((pj_stricmp(&accept->values[i], &PJ_CTYPE_CFWR_REQUEST) == 0) ||
				(pj_stricmp(&accept->values[i], &PJ_CTYPE_CFWR_RELEASE) == 0) ||
				(pj_stricmp(&accept->values[i], &PJ_CTYPE_CFWR_UPDATE) == 0))
				accept_valido = PJ_TRUE;
			break;
		}
	}
	if (!accept_valido || rdata->msg_info.msg->body == NULL)
	{
		PJ_LOG(3, (__FILE__, "ERROR: ProcessOptCfwrRequest: Accept recibido no es correcto"));
		st_code = 415;
	}

	char ED137Radioversion_, ED137Phoneversion_;
	pjsip_endpt_Get_Ed137_version(pjsua_var.endpt, &ED137Radioversion_, &ED137Phoneversion_);
	char sWG67ver[32];
	if (ED137Phoneversion_ == 'C')
		pj_ansi_sprintf(sWG67ver, "%.*s", SipCall::gWG67VersionTelefValueC.slen, SipCall::gWG67VersionTelefValueC.ptr);
	else
	{
		pj_ansi_sprintf(sWG67ver, "%.*s", SipCall::gWG67VersionTelefValue.slen, SipCall::gWG67VersionTelefValue.ptr);
		PJ_LOG(3, (__FILE__, "ERROR: ProcessOptCfwrRequest: Call forward solo se permite en ED137C y el agente esta configurado como ED137B"));
		st_code = 406;
		st_text = pj_str("WG67-version does not match. This agent supports ED137B");
		pst_text = &st_text;
	}

	pj_str_t PJ_SUBTYPE_CFWR_REQUEST = pj_str((char*)C_SUBTYPE_CFWR_REQUEST);
	pj_str_t PJ_SUBTYPE_CFWR_RELEASE = pj_str((char*)C_SUBTYPE_CFWR_RELEASE);
	pj_str_t PJ_SUBTYPE_CFWR_UPDATE = pj_str((char*)C_SUBTYPE_CFWR_UPDATE);

	CORESIP_CFWR_OPT_TYPE cfwr_options_type = CORESIP_CFWR_OPT_REQUEST;

	if (st_code == 200)
	{
		if (pj_stricmp(&rdata->msg_info.msg->body->content_type.subtype, &PJ_SUBTYPE_CFWR_REQUEST) == 0)
		{
			cfwr_options_type = CORESIP_CFWR_OPT_REQUEST;
		}
		else if (pj_stricmp(&rdata->msg_info.msg->body->content_type.subtype, &PJ_SUBTYPE_CFWR_RELEASE) == 0)
		{
			cfwr_options_type = CORESIP_CFWR_OPT_RELEASE;
		}
		else if (pj_stricmp(&rdata->msg_info.msg->body->content_type.subtype, &PJ_SUBTYPE_CFWR_UPDATE) == 0)
		{
			cfwr_options_type = CORESIP_CFWR_OPT_UPDATE;
		}
		else
		{
			PJ_LOG(3, (__FILE__, "ERROR: ProcessOptCfwrRequest: Content-Type recibido no es correcto"));
			st_code = 415;
		}
	}

	if (st_code == 200)
	{
		sip_uri = (pjsip_sip_uri*)pjsip_uri_get_uri(uri);
		if (sip_uri == NULL) return PJ_TRUE;

		if (sip_uri->user.slen != 0)
		{
			//La uri tiene user part. Se comprueba si existe el usuario en el agente
			acc_id_for_incomming = pjsua_acc_find_for_incoming(rdata);
			if (!pjsua_acc_is_valid(acc_id_for_incomming)) return PJ_TRUE;
			if (acc_id_for_incomming == pjsua_var.default_acc)
			{
				//Si es la default puede ser que no ha encontrado el abonado
				if (pj_stricmp(&pjsua_var.acc[acc_id_for_incomming].user_part, &sip_uri->user) != 0)
				{
					//El user part del options no coincide con el default. Por tanto el To del options no corresponde a ningun usuario
					st_code = 404;
				}
			}
		}
		else st_code = 404;
	}

	char from_uri[CORESIP_MAX_URI_LENGTH];
	if (st_code == 200)
	{		
		if (pjsip_uri_print(PJSIP_URI_IN_FROMTO_HDR, rdata->msg_info.from->uri, from_uri, sizeof(from_uri) - 1) <= 0)
		{
			PJ_LOG(3, (__FILE__, "ERROR: ProcessOptCfwrRequest: La URI del from es demasiado grande"));
			st_code = 400;
		}		
	}

	char body[CORESIP_MAX_BODY_LEN];
	int body_len = sizeof(body);
	if (st_code == 200)
	{		
		body_len = pjsip_print_text_body(rdata->msg_info.msg->body, body, sizeof(body) - 1);
		if (body_len <= 0)
		{
			PJ_LOG(3, (__FILE__, "ERROR: ProcessOptCfwrRequest: El Body recibido es demasiado grande"));
			st_code = 400;
		}
		body[body_len] = '\0';
	}

	if (st_code == 200)
	{
		if (SipAgent::Cb.CfwrOptReceivedCb)
		{
			/* Create basic response. */
			pj_status_t st = pjsip_endpt_create_response(pjsua_var.endpt, rdata, st_code, NULL, &tdata);
			if (st != PJ_SUCCESS) {
				PJ_LOG(3, (__FILE__, "ERROR: ProcessOptCfwrRequest: No se puede crear la respuesta"));
				return PJ_FALSE;
			}

			struct response *r = PJ_POOL_ALLOC_T(tdata->pool, struct response);
			if (r == NULL)
			{
				pjsip_tx_data_dec_ref(tdata);
				PJ_LOG(3, (__FILE__, "ERROR: ProcessOptCfwrRequest: No se puede crear la respuesta. No hay memoria suficiente"));
				return PJ_FALSE;
			}
			r->tdata = tdata;		
			pjsip_get_response_addr(tdata->pool, rdata, &r->res_addr);

			/* Add Accept header */
			pjsip_accept_hdr* accept = (pjsip_accept_hdr*)pjsip_msg_find_hdr(rdata->msg_info.msg, PJSIP_H_ACCEPT, NULL);
			pjsip_msg_add_hdr(tdata->msg, (pjsip_hdr*)pjsip_hdr_clone(tdata->pool, accept));

			/* Se pone un body provisional que luego se sustituira por el que ponga la aplicacion*/
			pj_str_t body_str;
			body_str.ptr = body;
			body_str.slen = body_len;
			tdata->msg->body = pjsip_msg_body_create(tdata->pool, &rdata->msg_info.msg->body->content_type.type,
				&rdata->msg_info.msg->body->content_type.subtype, &body_str);
			if (tdata->msg->body == NULL)
			{
				PJ_LOG(3, (__FILE__, "ERROR: ProcessOptCfwrRequest: No se puede crear el Body del mensaje"));
			}

			//Enviamos a la aplicacion el contenido del OPTIONS y un manejador de la respuesta (en este caso es dlg). 
			//La aplicacion llamara a la funcion que envie la respuesta con este manejador
			//Retornamos
			if (TRACE_ALL_CALLS) PJ_LOG(3, (__FILE__, "CfwrOptReceivedCb"));
			SipAgent::Cb.CfwrOptReceivedCb(acc_id_for_incomming | CORESIP_ACC_ID, from_uri, cfwr_options_type, body, (unsigned int) r);
			return PJ_TRUE;
		}
		else
		{
			PJ_LOG(3, (__FILE__, "ERROR: ProcessOptCfwrRequest: CfwrOptReceivedCb callback no definida"));
		}
		st_code = 400;
	}

	/* Create basic response. */
	status = pjsip_endpt_create_response(pjsua_var.endpt, rdata, st_code, pst_text, &tdata);
	if (status != PJ_SUCCESS) {
		PJ_LOG(3, (__FILE__, "ERROR: ProcessOptCfwrRequest: Unable to create OPTIONS response"));
		return PJ_TRUE;
	}

	//Si st_code no es 200 entonces contestamos aqui
	/* Add User-Agent header */
	if (pjsua_var.ua_cfg.user_agent.slen) {
		const pj_str_t USER_AGENT = { "User-Agent", 10 };
		pjsip_hdr* h;

		h = (pjsip_hdr*)pjsip_generic_string_hdr_create(tdata->pool,
			&USER_AGENT,
			&pjsua_var.ua_cfg.user_agent);
		pjsip_msg_add_hdr(tdata->msg, h);
	}

	//Establecemos aqui la cabecera WG67-version con la version correspondiente.
	//En la funcion OnTxRequest de SipCall.cpp es donde se establece esta cabecera para todos
	//los paquetes SIP, siempre y cuando no se hayan establecido antes, como es este el caso.
	pj_str_t pjsWG67ver;
	pjsWG67ver.ptr = sWG67ver;
	pjsWG67ver.slen = pj_ansi_strlen(sWG67ver);
	SipCall::Wg67VersionSet(tdata, &pjsWG67ver);

	/* Send response statelessly */
	pjsip_get_response_addr(tdata->pool, rdata, &res_addr);
	status = pjsip_endpt_send_response(pjsua_var.endpt, &res_addr, tdata, NULL, NULL);
	if (status != PJ_SUCCESS)
		pjsip_tx_data_dec_ref(tdata);

	return PJ_TRUE;
}

/**
 * SendResponseCFWD.	...
 * Envia la respuesta al options utilizado para la negociacion de call forward
 * @param	st_code				Code de la respuesta. Si no es 200 entonces se ignora el parametro del body
 * @param	body				Contenido del body (XML). Acabado en '\0'
 * @param	hresp				Manejador necesario para enviar la respuesta
 */
void OptionsMod::SendResponseCFWD(int st_code, const char* body, unsigned int hresp)
{
	char ED137Radioversion_, ED137Phoneversion_;
	pjsip_endpt_Get_Ed137_version(pjsua_var.endpt, &ED137Radioversion_, &ED137Phoneversion_);
	char sWG67ver[32];
	sWG67ver[0] = 0;
	if (ED137Phoneversion_ == 'C')
		pj_ansi_sprintf(sWG67ver, "%.*s", SipCall::gWG67VersionTelefValueC.slen, SipCall::gWG67VersionTelefValueC.ptr);
	else
	{
		PJ_CHECK_STATUS(PJ_EINVALIDOP, ("ERROR: SendResponseCFWD: Call forward solo se permite en ED137C y el agente esta configurado como ED137B"));
	}

	struct response *r = (struct response *) hresp;
	pjsip_tx_data* tdata = r->tdata;

	if (body != NULL)
	{
		//El body
		pj_str_t body_str = pj_str((char*)body);
		tdata->msg->body = pjsip_msg_body_create(tdata->pool, &tdata->msg->body->content_type.type,
			&tdata->msg->body->content_type.subtype, &body_str);
		if (tdata->msg->body == NULL)
		{
			PJ_LOG(3, (__FILE__, "ERROR: SendResponseCFWD: No se puede crear el Body del mensaje"));
		}
	}
	else
	{
		tdata->msg->body = NULL;
	}

	tdata->msg->line.status.code = st_code;
	tdata->msg->line.status.reason = *pjsip_get_status_text(st_code);

	//Establecemos aqui la cabecera WG67-version con la version correspondiente.
	//En la funcion OnTxRequest de SipCall.cpp es donde se establece esta cabecera para todos
	//los paquetes SIP, siempre y cuando no se hayan establecido antes, como es este el caso.
	pj_str_t pjsWG67ver;
	pjsWG67ver.ptr = sWG67ver;
	pjsWG67ver.slen = pj_ansi_strlen(sWG67ver);
	SipCall::Wg67VersionSet(tdata, &pjsWG67ver);
	
	/* Add User-Agent header */
	if (pjsua_var.ua_cfg.user_agent.slen) {
		const pj_str_t USER_AGENT = { "User-Agent", 10 };
		pjsip_hdr* h;

		h = (pjsip_hdr*)pjsip_generic_string_hdr_create(tdata->pool,
			&USER_AGENT,
			&pjsua_var.ua_cfg.user_agent);
		pjsip_msg_add_hdr(tdata->msg, h);
	}	

	/* Send response statelessly */
	pj_status_t st = pjsip_endpt_send_response(pjsua_var.endpt, &r->res_addr, tdata, NULL, NULL);
	if (st != PJ_SUCCESS)
		pjsip_tx_data_dec_ref(tdata);
}


/**
 * OnRxResponse:	Callback. Se invoca cuando se recibe una 'respuesta' SIP
 *						- Cuando la Respuesta corresponde a un metodo 'option'
 * @param	rdata		Puntero 'pjsip_rx_data' a los datos recibidos.
 * @return	'pj_bool_t' Siempre retorna 'PJ_FALSE', para que continue el tratamiento del evento la propia libreria.
 *
 *	INFO: http://www.pjsip.org/pjsip/docs/html/group__PJSIP__MSG__METHOD.htm#gafdd26e26092275d7f156a0d8efe90b78
 *		  http://www.pjsip.org/pjsip/docs/html/group__PJSIP__MSG__METHOD.htm#ga66b57e1b5645d2ee843141a0e657b0d1
 */
pj_bool_t OptionsMod::OnRxResponse(pjsip_rx_data* rdata)
{
	if (pjsip_method_cmp(&rdata->msg_info.cseq->method, pjsip_get_options_method()) == 0)
	{
		//Se procesan las respuestas a los mensajes OPTIONS normales

		char dstUri[CORESIP_MAX_URI_LENGTH + 1] = { 0 };
		char supported[CORESIP_MAX_SUPPORTED_LENGTH + 1] = { 0 };
		char allow[CORESIP_MAX_SUPPORTED_LENGTH + 1] = { 0 };
		char callid[CORESIP_MAX_CALLID_LENGTH + 1] = { 0 };
		char accept[CORESIP_MAX_SUPPORTED_LENGTH + 1] = { 0 };
		char content_type[CORESIP_MAX_SUPPORTED_LENGTH + 1] = { 0 };

		if (rdata->msg_info.cid != NULL)
		{
			if (rdata->msg_info.cid->id.slen < sizeof(callid))
			{
				strncpy(callid, rdata->msg_info.cid->id.ptr, rdata->msg_info.cid->id.slen);
			}
		}

		pjsip_msg *msg = rdata->msg_info.msg;

		pjsip_sip_uri * dst = (pjsip_sip_uri*)pjsip_uri_get_uri(rdata->msg_info.to->uri);

		if (dst->user.slen > 0)
		{
			//La uri destino del OPTIONS tiene usuario
			pj_ansi_snprintf(dstUri, sizeof(dstUri) - 1, "<sip:%.*s@%.*s>", 
				dst->user.slen, dst->user.ptr, dst->host.slen, dst->host.ptr);
		}
		else
		{
			//La uri destino del OPTIONS no tiene usuario
			pj_ansi_snprintf(dstUri, sizeof(dstUri) - 1, "<sip:%.*s>", 
				dst->host.slen, dst->host.ptr);
		}

		if (rdata->msg_info.supported != NULL)
		{		
			for (unsigned int i = 0; i < rdata->msg_info.supported->count; i++)
			{
				char method[64];
				if ((rdata->msg_info.supported->values[i].slen+2) < sizeof(method))
				{
					strncpy(method, rdata->msg_info.supported->values[i].ptr, rdata->msg_info.supported->values[i].slen);
					method[rdata->msg_info.supported->values[i].slen] = '\0';
					strcat(method, ", ");				

					if ((sizeof(supported) - (strlen(supported)+1)) > strlen(method))
					{
						strcat(supported, method);
					}
				}
			}
		}

		pjsip_allow_hdr *allow_hdr = (pjsip_allow_hdr*) pjsip_msg_find_hdr(msg, PJSIP_H_ALLOW, NULL);
		if (allow_hdr != NULL)
		{
			for (unsigned int i = 0; i < allow_hdr->count; i++)
			{
				char method[64];
				if ((allow_hdr->values[i].slen+2) < sizeof(method))
				{
					strncpy(method, allow_hdr->values[i].ptr, allow_hdr->values[i].slen);
					method[allow_hdr->values[i].slen] = '\0';
					strcat(method, ", ");				

					if ((sizeof(allow) - (strlen(allow)+1)) > strlen(method))
					{
						strcat(allow, method);
					}
				}
			}
		}

		pjsip_accept_hdr* accept_hdr = (pjsip_accept_hdr*)pjsip_msg_find_hdr(msg, PJSIP_H_ACCEPT, NULL);
		if (accept_hdr != NULL)
		{
			for (unsigned int i = 0; i < accept_hdr->count; i++)
			{
				char val[64];
				if ((accept_hdr->values[i].slen + 2) < sizeof(val))
				{
					strncpy(val, accept_hdr->values[i].ptr, accept_hdr->values[i].slen);
					val[accept_hdr->values[i].slen] = '\0';
					strcat(val, ", ");

					if ((sizeof(accept) - (strlen(accept) + 1)) > strlen(val))
					{
						strcat(accept, val);
					}
				}
			}
		}

		if (strlen(supported) > 1)
			supported[strlen(supported)-2] = '\0';  //Se quita la ultima coma y el ultimo espacio
		if (strlen(allow) > 1)
			allow[strlen(allow)-2] = '\0';  //Se quita la ultima coma y el ultimo espacio

		if (SipAgent::Cb.OptionsReceiveCb)
		{
			if (TRACE_ALL_CALLS) PJ_LOG(3, (__FILE__, "OptionsReceiveCb dstUri %s", dstUri));
			SipAgent::Cb.OptionsReceiveCb(dstUri, callid, msg->line.status.code, supported, allow);
		}
	}

	return PJ_TRUE;
}
