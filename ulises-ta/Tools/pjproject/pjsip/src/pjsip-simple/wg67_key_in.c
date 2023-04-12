#include <pjsip-simple/wg67_key_in.h>
#include <pjsip-simple/errno.h>
#include <pjsip-simple/evsub_msg.h>
#include <pjsip/sip_module.h>
#include <pjsip/sip_endpoint.h>
#include <pjsip/sip_dialog.h>
#include <pjsip/sip_parser.h>
#include <pj/assert.h>
#include <pj/guid.h>
#include <pj/log.h>
#include <pj/os.h>
#include <pj/pool.h>
#include <pj/string.h>
#include <pj/except.h>
#include <pj/ctype.h>

#define THIS_FILE						"wg67_key_in.c"
#define WG67_DEFAULT_EXPIRES		600

#define IS_NEWLINE(c)				((c)=='\r' || (c)=='\n')
#define IS_SPACE(c)					((c)==' ' || (c)=='\t')

static const pj_str_t STR_TIMEOUT = { "timeout", 7 };

/*
* WG67 KEY-IN module (mod-wg67)
*/
static struct pjsip_module mod_wg67 = 
{
	NULL, NULL,			    /* prev, next.			*/
	{ "mod-wg67", 8 },	    /* Name.				*/
	-1,				    /* Id				*/
	PJSIP_MOD_PRIORITY_DIALOG_USAGE,/* Priority				*/
	NULL,			    /* load()				*/
	NULL,			    /* start()				*/
	NULL,			    /* stop()				*/
	NULL,			    /* unload()				*/
	NULL,			    /* on_rx_request()			*/
	NULL,			    /* on_rx_response()			*/
	NULL,			    /* on_tx_request.			*/
	NULL,			    /* on_tx_response()			*/
	NULL,			    /* on_tsx_state()			*/
};


/*
* This structure describe a WG67 KEY-IN subscription
*/
struct pjsip_wg67
{
	pjsip_evsub *sub;					/**< Event subscribtion record.	    */
	pjsip_dialog *dlg;					/**< The dialog.		    */
	pjsip_wg67_status status;			/**< WG67 KEY-IN status.		    */
	pjsip_wg67_status tmp_status;	/**< Temp, before NOTIFY is answred.*/
	pjsip_evsub_user user_cb;			/**< The user callback.		    */
};


typedef struct pjsip_wg67 pjsip_wg67;


/*
* Forward decl for evsub callback.
*/
static void wg67_on_evsub_state( pjsip_evsub *sub, pjsip_event *event);
static void wg67_on_evsub_tsx_state( pjsip_evsub *sub, pjsip_transaction *tsx,
												pjsip_event *event);
static void wg67_on_evsub_rx_refresh( pjsip_evsub *sub,
												pjsip_rx_data *rdata,
												int *p_st_code,
												pj_str_t **p_st_text,
												pjsip_hdr *res_hdr,
												pjsip_msg_body **p_body);
static void wg67_on_evsub_rx_notify(pjsip_evsub* sub,
												pjsip_rx_data* rdata,
												int* p_st_code,
												pj_str_t** p_st_text,
												pjsip_hdr* res_hdr,
												pjsip_msg_body** p_body);
static void wg67_on_evsub_client_refresh(pjsip_evsub *sub);
static void wg67_on_evsub_server_timeout(pjsip_evsub* sub);

pjsip_uri *  int_parse_uri_or_name_addr( pj_scanner *scanner, 
													 pj_pool_t *pool, 
													 unsigned option);

/*
* Event subscription callback for WG67 KEY-IN.
*/
static pjsip_evsub_user wg67_user = 
{
	&wg67_on_evsub_state,
	&wg67_on_evsub_tsx_state,
	&wg67_on_evsub_rx_refresh,
	&wg67_on_evsub_rx_notify,
	&wg67_on_evsub_client_refresh,
	&wg67_on_evsub_server_timeout
};


/*
* Some static constants.
*/
static const pj_str_t STR_EVENT = { "Event", 5 };
static const pj_str_t STR_WG67 = { "WG67KEY-IN", 10 };
static const pj_str_t STR_TEXT = { "text", 4 };
static const pj_str_t STR_PLAIN = { "plain", 5 };
static const pj_str_t STR_TEXT_PLAIN = { "text/plain", 10 };


/*
* Init WG67 KEY-IN module.
*/
PJ_DEF(pj_status_t) pjsip_wg67_init_module( pjsip_endpoint *endpt)
{
	pj_status_t status;
	pj_str_t accept[1];

	/* Check arguments. */
	PJ_ASSERT_RETURN(endpt, PJ_EINVAL);

	/* Must have not been registered */
	PJ_ASSERT_RETURN(mod_wg67.id == -1, PJ_EINVALIDOP);

	/* Register to endpoint */
	status = pjsip_endpt_register_module(endpt, &mod_wg67);
	if (status != PJ_SUCCESS)
		return status;

	accept[0] = STR_TEXT_PLAIN;

	/* Register event package to event module. */
	status = pjsip_evsub_register_pkg( &mod_wg67, &STR_WG67, 
		WG67_DEFAULT_EXPIRES, 
		PJ_ARRAY_SIZE(accept), accept);
	if (status != PJ_SUCCESS) {
		pjsip_endpt_unregister_module(endpt, &mod_wg67);
		return status;
	}

	return PJ_SUCCESS;
}

PJ_DEF(pj_status_t) pjsip_wg67_deinit_module(pjsip_endpoint* endpt)
{
	pj_status_t status;

	/* Check arguments. */
	PJ_ASSERT_RETURN(endpt, PJ_EINVAL);

	/* Must have been registered */
	PJ_ASSERT_RETURN(mod_wg67.id != -1, PJ_EINVALIDOP);

	status = pjsip_evsub_unregister_pkg(&mod_wg67, &STR_WG67);
	if (status != PJ_SUCCESS) return status;

	/* Register to endpoint */
	status = pjsip_endpt_unregister_module(endpt, &mod_wg67);
	return status;
}


/*
* Get WG67 KEY-IN module instance.
*/
PJ_DEF(pjsip_module*) pjsip_wg67_instance(void)
{
	return &mod_wg67;
}

/*
 *  search_wg67_subscription_in_dialog
 * Busca una subscripcion al avento WG67KEY-IN en un dialogo sip
 * @param dlg. Dialogo
 * Return. Puntero a la subscripcion. NULL si no hay ninguna.
 */
PJ_DEF(pjsip_evsub*) search_wg67_subscription_in_dialog(pjsip_dialog* dlg)
{
	pjsip_evsub* wg67_subs;
	wg67_subs = search_subscription_in_dialog(dlg, (pj_str_t *) &STR_WG67);
	return wg67_subs;
}

/*
* Create client subscription.
*/
PJ_DEF(pj_status_t) pjsip_wg67_create_uac( pjsip_dialog *dlg,
														const pjsip_evsub_user *user_cb,
														unsigned options,
														pjsip_evsub **p_evsub )
{
	pj_status_t status;
	pjsip_wg67 *wg67;
	pjsip_evsub *sub;

	PJ_ASSERT_RETURN(dlg && p_evsub, PJ_EINVAL);

	pjsip_dlg_inc_lock(dlg);

	/* Create event subscription */
	status = pjsip_evsub_create_uac( dlg,  &wg67_user, &STR_WG67, 
		options, &sub);
	if (status != PJ_SUCCESS)
		goto on_return;

	/* Create WG67 KEY-IN */
	wg67 = PJ_POOL_ZALLOC_T(dlg->pool, pjsip_wg67);
	wg67->dlg = dlg;
	wg67->sub = sub;
	if (user_cb)
		pj_memcpy(&wg67->user_cb, user_cb, sizeof(pjsip_evsub_user));

	/* Attach to evsub */
	pjsip_evsub_set_mod_data(sub, mod_wg67.id, wg67);

	*p_evsub = sub;

on_return:
	pjsip_dlg_dec_lock(dlg);
	return status;
}

/*
* Create server subscription.
*/
PJ_DEF(pj_status_t) pjsip_wg67_create_uas(pjsip_dialog* dlg, const pjsip_evsub_user* user_cb, pjsip_rx_data* rdata, pjsip_evsub** p_evsub)
{
	pjsip_accept_hdr* accept;
	pjsip_event_hdr* event;
	pjsip_evsub* sub;
	pjsip_wg67* wg67;
	pj_status_t status;

	/* Check arguments */
	PJ_ASSERT_RETURN(dlg && rdata && p_evsub, PJ_EINVAL);

	/* Must be request message */
	PJ_ASSERT_RETURN(rdata->msg_info.msg->type == PJSIP_REQUEST_MSG, PJSIP_ENOTREQUESTMSG);

	/* Check that request is SUBSCRIBE */
	PJ_ASSERT_RETURN(pjsip_method_cmp(&rdata->msg_info.msg->line.req.method,
		&pjsip_subscribe_method) == 0,
		PJSIP_SIMPLE_ENOTSUBSCRIBE);

	/* Check that Event header contains "WG67KEY-IN" */
	event = (pjsip_event_hdr*)pjsip_msg_find_hdr_by_name(rdata->msg_info.msg, &STR_EVENT, NULL);
	if (!event) {
		return PJSIP_ERRNO_FROM_SIP_STATUS(PJSIP_SC_BAD_REQUEST);
	}
	if (pj_stricmp(&event->event_type, &STR_WG67) != 0) {
		return PJSIP_ERRNO_FROM_SIP_STATUS(PJSIP_SC_BAD_EVENT);
	}

	/* Check that request contains compatible Accept header. */
	accept = (pjsip_accept_hdr*)pjsip_msg_find_hdr(rdata->msg_info.msg, PJSIP_H_ACCEPT, NULL);
	if (accept) {
		unsigned i;
		for (i = 0; i < accept->count; ++i) {
			if (pj_stricmp(&accept->values[i], &STR_TEXT_PLAIN) == 0) {
				break;
			}
		}

		if (i == accept->count) {
			/* Nothing is acceptable */
			return PJSIP_ERRNO_FROM_SIP_STATUS(PJSIP_SC_NOT_ACCEPTABLE);
		}

	}
	else {
		/* No Accept header */
	}

	/* Lock dialog */
	pjsip_dlg_inc_lock(dlg);

	/* Create server subscription */
	status = pjsip_evsub_create_uas(dlg, &wg67_user, rdata, 0, &sub);
	if (status != PJ_SUCCESS)
		goto on_return;

	/* Create server subscription */
	wg67 = PJ_POOL_ZALLOC_T(dlg->pool, pjsip_wg67);
	wg67->dlg = dlg;
	wg67->sub = sub;
	if (user_cb)
		pj_memcpy(&wg67->user_cb, user_cb, sizeof(pjsip_evsub_user));

	/* Attach to evsub */
	pjsip_evsub_set_mod_data(sub, mod_wg67.id, wg67);

	/* Done: */
	*p_evsub = sub;

on_return:
	pjsip_dlg_dec_lock(dlg);
	return status;
}


/*
* Forcefully terminate WG67 KEY-IN.
*/
PJ_DEF(pj_status_t) pjsip_wg67_terminate( pjsip_evsub *sub,
													  pj_bool_t notify )
{
	return pjsip_evsub_terminate(sub, notify);
}


/*
* Create SUBSCRIBE
*/
PJ_DEF(pj_status_t) pjsip_wg67_initiate( pjsip_evsub *sub,
													 pj_int32_t expires,
													 pjsip_tx_data **p_tdata)
{
	return pjsip_evsub_initiate(sub, &pjsip_subscribe_method, expires, 
		p_tdata);
}

/*
* Accept incoming subscription.
*/
PJ_DEF(pj_status_t) pjsip_wg67_accept(pjsip_evsub* sub, pjsip_rx_data* rdata, int st_code, const pjsip_hdr* hdr_list)
{
	return pjsip_evsub_accept(sub, rdata, st_code, hdr_list);
}


/*
* Get WG67 KEY-IN status.
*/
PJ_DEF(pj_status_t) pjsip_wg67_get_status( pjsip_evsub *sub,
														pjsip_wg67_status *status )
{
	pjsip_wg67 *wg67;

	PJ_ASSERT_RETURN(sub && status, PJ_EINVAL);

	wg67 = (pjsip_wg67*) pjsip_evsub_get_mod_data(sub, mod_wg67.id);
	PJ_ASSERT_RETURN(wg67!=NULL, PJSIP_SIMPLE_ENOWG67);

	if (wg67->tmp_status._is_valid)
		pj_memcpy(status, &wg67->tmp_status, sizeof(pjsip_wg67_status));
	else
		pj_memcpy(status, &wg67->status, sizeof(pjsip_wg67_status));

	return PJ_SUCCESS;
}


/*
* Send request.
*/
PJ_DEF(pj_status_t) pjsip_wg67_send_request( pjsip_evsub *sub,
														  pjsip_tx_data *tdata )
{
	return pjsip_evsub_send_request(sub, tdata);
}


/*
* This callback is called by event subscription when subscription
* state has changed.
*/
static void wg67_on_evsub_state( pjsip_evsub *sub, pjsip_event *event)
{
	pjsip_wg67 *wg67;

	wg67 = (pjsip_wg67*) pjsip_evsub_get_mod_data(sub, mod_wg67.id);
	PJ_ASSERT_ON_FAIL(wg67!=NULL, {return;});

	if (wg67->user_cb.on_evsub_state)
	(*wg67->user_cb.on_evsub_state)(sub, event);
}

/*
* Called when transaction state has changed.
*/
static void wg67_on_evsub_tsx_state( pjsip_evsub *sub, pjsip_transaction *tsx,
												pjsip_event *event)
{
	pjsip_wg67 *wg67;

	wg67 = (pjsip_wg67*) pjsip_evsub_get_mod_data(sub, mod_wg67.id);
	PJ_ASSERT_ON_FAIL(wg67!=NULL, {return;});

	if (wg67->user_cb.on_tsx_state)
		(*wg67->user_cb.on_tsx_state)(sub, tsx, event);
}


/*
* Process the content of incoming NOTIFY request and update temporary
* status.
*
* return PJ_SUCCESS if incoming request is acceptable. If return value
*	  is not PJ_SUCCESS, res_hdr may be added with Warning header.
*/
static pj_status_t wg67_process_rx_notify( pjsip_wg67 *wg67,
														pjsip_rx_data *rdata, 
														int *p_st_code,
														pj_str_t **p_st_text,
														pjsip_hdr *res_hdr)
{
	pjsip_ctype_hdr *ctype_hdr;
	pj_status_t status = PJ_SUCCESS;

	const pjsip_sub_state_hdr* sub_hdr;
	pj_str_t sub_state = { "Subscription-State", 18 };
	const pjsip_msg* msg;

	*p_st_text = NULL;
	
	if (status == PJ_SUCCESS)
	{
		/* Parse content. */
		msg = rdata->msg_info.msg;
		sub_hdr = (const pjsip_sub_state_hdr*)pjsip_msg_find_hdr_by_name(msg, &sub_state, NULL);
		if (sub_hdr == NULL)
		{
			status = PJSIP_SIMPLE_EBADCONTENT;
		}
	}

	if (status == PJ_SUCCESS)
	{
		if (rdata->msg_info.msg->body != NULL)
		{
			/* Check Content-Type and msg body are present if body is present */
			ctype_hdr = rdata->msg_info.ctype;

			if (ctype_hdr == NULL)
			{
				*p_st_code = PJSIP_SC_BAD_REQUEST;
				return PJSIP_ERRNO_FROM_SIP_STATUS(PJSIP_SC_BAD_REQUEST);
			}
			else if (pj_stricmp(&ctype_hdr->media.type, &STR_TEXT) != 0 ||
				pj_stricmp(&ctype_hdr->media.subtype, &STR_PLAIN) != 0)
			{
				status = PJSIP_SIMPLE_EBADCONTENT;
			}
		}
	}

	if (status == PJ_SUCCESS)
	{
		if (rdata->msg_info.msg->body != NULL)
		{
			status = pjsip_wg67_parse_body(rdata, wg67->dlg->pool, &wg67->tmp_status);
		}
		else
		{
			wg67->tmp_status.info_cnt = 0;
		}
	}

	if (status != PJ_SUCCESS) {
		/* Unsupported or bad Content-Type */
		pjsip_accept_hdr *accept_hdr;
		pjsip_warning_hdr *warn_hdr;

		*p_st_code = PJSIP_SC_NOT_ACCEPTABLE_HERE;

		/* Add Accept header */
		accept_hdr = pjsip_accept_hdr_create(rdata->tp_info.pool);
		accept_hdr->values[accept_hdr->count++] = STR_TEXT_PLAIN;
		pj_list_push_back(res_hdr, accept_hdr);

		/* Add Warning header */
		warn_hdr = pjsip_warning_hdr_create_from_status(
			rdata->tp_info.pool,
			pjsip_endpt_name(wg67->dlg->endpt),
			status);
		pj_list_push_back(res_hdr, warn_hdr);

		return status;
	}

	/* If application calls wg67_get_status(), redirect the call to
	* retrieve the temporary status.
	*/
	wg67->tmp_status._is_valid = PJ_TRUE;

	return PJ_SUCCESS;
}

/*
* Called when SUBSCRIBE is received.
*/
static void wg67_on_evsub_rx_refresh(pjsip_evsub* sub,
										pjsip_rx_data* rdata,
										int* p_st_code,
										pj_str_t** p_st_text,
										pjsip_hdr* res_hdr,
										pjsip_msg_body** p_body)
{
	pjsip_wg67* wg67;

	wg67 = (pjsip_wg67*)pjsip_evsub_get_mod_data(sub, mod_wg67.id);
	PJ_ASSERT_ON_FAIL(wg67 != NULL, { return; });

	if (wg67->user_cb.on_rx_refresh) {
		(*wg67->user_cb.on_rx_refresh)(sub, rdata, p_st_code, p_st_text, res_hdr, p_body);

	}
	else {

	}
}

/*
* Called when NOTIFY is received.
*/
static void wg67_on_evsub_rx_notify( pjsip_evsub *sub, 
												pjsip_rx_data *rdata,
												int *p_st_code,
												pj_str_t **p_st_text,
												pjsip_hdr *res_hdr,
												pjsip_msg_body **p_body)
{
	pjsip_wg67 *wg67;
	pj_status_t status = PJ_SUCCESS;

	wg67 = (pjsip_wg67*) pjsip_evsub_get_mod_data(sub, mod_wg67.id);
	PJ_ASSERT_ON_FAIL(wg67!=NULL, {return;});

	status = wg67_process_rx_notify( wg67, rdata, p_st_code, p_st_text,	res_hdr );		

	/*Llamamos a la callback de usuario aunque se haya encontrado errores*/
	/* Notify application. */
	if (wg67->user_cb.on_rx_notify) {
		(*wg67->user_cb.on_rx_notify)(sub, rdata, p_st_code, p_st_text, 
			res_hdr, p_body);
	}

	else if (status != PJ_SUCCESS)
	{
		return;
	}


	/* If application responded NOTIFY with 2xx, copy temporary status
	* to main status, and mark the temporary status as invalid.
	*/
	if ((*p_st_code)/100 == 2) {
		pj_memcpy(&wg67->status, &wg67->tmp_status, sizeof(pjsip_wg67_status));
	}

	wg67->tmp_status._is_valid = PJ_FALSE;

	/* Done */
}


/*
* Called when it's time to send SUBSCRIBE.
*/
static void wg67_on_evsub_client_refresh(pjsip_evsub *sub)
{
	pjsip_wg67 *wg67;

	wg67 = (pjsip_wg67*) pjsip_evsub_get_mod_data(sub, mod_wg67.id);
	PJ_ASSERT_ON_FAIL(wg67!=NULL, {return;});

	if (wg67->user_cb.on_client_refresh) {
		(*wg67->user_cb.on_client_refresh)(sub);
	} else {
		pj_status_t status;
		pjsip_tx_data *tdata;

		status = pjsip_wg67_initiate(sub, -1, &tdata);
		if (status == PJ_SUCCESS)
			pjsip_wg67_send_request(sub, tdata);
	}
}

/*
* Called when it's time to send SUBSCRIBE.
*/
static void wg67_on_evsub_server_timeout(pjsip_evsub* sub)
{
	pjsip_wg67* wg67;

	wg67 = (pjsip_wg67*)pjsip_evsub_get_mod_data(sub, mod_wg67.id);
	PJ_ASSERT_ON_FAIL(wg67 != NULL, { return; });

	if (wg67->user_cb.on_server_timeout) {
		(*wg67->user_cb.on_server_timeout)(sub);
	}
	else {
		pjsip_wg67_notify(sub, -1, PJ_FALSE, PJSIP_EVSUB_STATE_TERMINATED, NULL, &STR_TIMEOUT, -1, NULL, NULL);
	}
}


/* Syntax error handler for parser. */
static void on_syntax_error(pj_scanner *scanner)
{
	PJ_UNUSED_ARG(scanner);
	PJ_THROW(PJSIP_SYN_ERR_EXCEPTION);
}


PJ_DEF(pj_status_t) pjsip_wg67_parse_body(pjsip_rx_data *rdata,
														 pj_pool_t *pool,
														 pjsip_wg67_status *status)
{
	pj_scanner scanner;
	pj_status_t ret = PJSIP_SIMPLE_EBADWG67;
	PJ_USE_EXCEPTION;

	pj_scan_init(&scanner, (char*)rdata->msg_info.msg->body->data, 
		rdata->msg_info.msg->body->len, 0, &on_syntax_error);

	// PlugTest FAA 05/2011
	// Las info de WG67 se iban acumulando...
	status->info_cnt=0;
	PJ_TRY 
	{
		/* Skip leading newlines. */
		while (IS_NEWLINE(*scanner.curptr)) 
		{
			pj_scan_get_newline(&scanner);
		}

		pj_scan_skip_whitespace(&scanner);

		pj_str_t fid;
		//Busca el fid, que solo puede estar en la primera linea
		if (pj_scan_stricmp(&scanner, "fid:", 4) == 0)
		{
			pj_scan_get_until_ch(&scanner, ':', &fid);
			pj_scan_get_char(&scanner);								// Salta ':'
			pj_scan_skip_whitespace(&scanner);
			pj_scan_get_until_chr(&scanner, "\r\n", &fid);
			pj_strdup(pool, &status->fid, &fid);
			pj_scan_get_newline(&scanner);
		}
		else
		{
			//Hay hay fid
			status->fid.slen = 0;
		}

		while (!pj_scan_is_eof(&scanner) && !IS_NEWLINE(*scanner.curptr) && 
			(status->info_cnt < PJSIP_WG67_STATUS_MAX_INFO)) 
		{
			pj_str_t param1, param2, param3;
			param1.slen = 0;
			param2.slen = 0;
			param3.slen = 0;
			pj_bool_t endline = PJ_FALSE;

			pj_scan_skip_whitespace(&scanner);
			pj_scan_get_until_chr(&scanner, ",\r\n", &param1);
			if (*scanner.curptr == ',')
			{
				pj_scan_get_char(&scanner);								// Salta la coma.
			}
			else if ((*scanner.curptr == '\r') || (*scanner.curptr == '\n'))
			{
				pj_scan_get_newline(&scanner);
				endline = PJ_TRUE;
			}

			if (!endline)
			{
				pj_scan_skip_whitespace(&scanner);
				pj_scan_get_until_chr(&scanner, ",\r\n", &param2);
				if (*scanner.curptr == ',')
				{
					pj_scan_get_char(&scanner);								// Salta la coma.
				}
				else if ((*scanner.curptr == '\r') || (*scanner.curptr == '\n'))
				{
					pj_scan_get_newline(&scanner);
					endline = PJ_TRUE;
				}
			}

			if (!endline)
			{
				pj_scan_skip_whitespace(&scanner);
				pj_scan_get_until_chr(&scanner, ",\r\n", &param3);
				if (*scanner.curptr == ',')
				{
					pj_scan_get_char(&scanner);								// Salta la coma.
				}
				else if ((*scanner.curptr == '\r') || (*scanner.curptr == '\n'))
				{
					pj_scan_get_newline(&scanner);
					endline = PJ_TRUE;
				}
			}

			if (!endline)
			{
				on_syntax_error(&scanner);
				break;
			}

			if (param1.slen > 0 && param2.slen > 0 && param3.slen > 0)
			{
				//La linea tiene ptt-id, uri y session type

				int i;
				char* c;
				pj_bool_t param1_is_pttid = PJ_TRUE;
				c = param1.ptr;

				for (i = 0; i < param1.slen; i++)
				{
					if (!pj_isdigit(*c))
					{
						param1_is_pttid = PJ_FALSE;
						break;
					}
					c++;
				}
				if (!param1_is_pttid)
				{
					on_syntax_error(&scanner);
					break;
				}
				/*No parseo la URI porque se ha visto que en radios reales que no llevan el sip:*/
				if (pj_stricmp2(&param3, "Coupling") != 0 && pj_stricmp2(&param3, "Radio-Rxonly") != 0 &&
					pj_stricmp2(&param3, "Radio-TxRx") != 0 && pj_stricmp2(&param3, "Radio") != 0  && pj_stricmp2(&param3, "Radio-Idle") != 0)
				{
					on_syntax_error(&scanner);
					break;
				}

				status->info[status->info_cnt].pttid = (unsigned short)pj_strtoul(&param1);
				pj_strdup(pool, &status->info[status->info_cnt].uri, &param2);
				pj_strdup(pool, &status->info[status->info_cnt].sessiontype, &param3);
				status->info_cnt++;
			}
			else if (param1.slen > 0 && param2.slen > 0 && param3.slen == 0)
			{
				int i;
				char* c;
				pj_bool_t param1_is_pttid = PJ_TRUE;
				c = param1.ptr;

				for (i = 0; i < param1.slen; i++)
				{
					if (!pj_isdigit(*c))
					{
						param1_is_pttid = PJ_FALSE;
						break;
					}
					c++;
				}

				if (param1_is_pttid)
				{
					//La linea tiene ptt-id y uri
					/*No parseo la URI porque se ha visto que en radios reales que no llevan el sip:*/

					status->info[status->info_cnt].pttid = (unsigned short)pj_strtoul(&param1);
					pj_strdup(pool, &status->info[status->info_cnt].uri, &param2);
					pj_strdup(pool, &status->info[status->info_cnt].sessiontype, &param3);
					status->info_cnt++;
				}
				else
				{
					//La linea tiene uri y session type
					/*No parseo la URI porque se ha visto que en radios reales que no llevan el sip:*/
					if (pj_stricmp2(&param2, "Coupling") != 0 && pj_stricmp2(&param2, "Radio-Rxonly") != 0 &&
						pj_stricmp2(&param2, "Radio-TxRx") != 0 && pj_stricmp2(&param2, "Radio") != 0  && pj_stricmp2(&param2, "Radio-Idle") != 0)
					{
						on_syntax_error(&scanner);
						break;
					}
					status->info[status->info_cnt].pttid = (unsigned short)pj_strtoul(&param3);
					pj_strdup(pool, &status->info[status->info_cnt].uri, &param1);
					pj_strdup(pool, &status->info[status->info_cnt].sessiontype, &param2);
					status->info_cnt++;
				}
			}
			else if (param1.slen > 0 && param2.slen == 0 && param3.slen == 0)
			{
				//La linea solo tiene uri
				/*No parseo la URI porque se ha visto que en radios reales que no llevan el sip:*/
				status->info[status->info_cnt].pttid = (unsigned short)pj_strtoul(&param2);
				pj_strdup(pool, &status->info[status->info_cnt].uri, &param1);
				pj_strdup(pool, &status->info[status->info_cnt].sessiontype, &param3);
				status->info_cnt++;
			}
			else
			{				
			}
		}

		ret = PJ_SUCCESS;
	}
	PJ_CATCH_ANY 
	{
		PJ_LOG(4, (THIS_FILE, "Error parsing WG67 KEY-IN body in line %d col %d",
			scanner.line, pj_scan_get_col(&scanner)));
	}
	PJ_END;

	pj_scan_fini(&scanner);
	return ret;
}

/*
 * Create NOTIFY
 */
PJ_DEF(pj_status_t) pjsip_wg67_notify(pjsip_evsub* sub,
										pj_bool_t force_subs_state,			//If true then the Notify is sent forcing state with the following parameters
										int expires,				//Value -1 if it is not required
										pjsip_evsub_state state,	//Is ignored if force_subs_state is true
										const pj_str_t* state_str,	//Value NULL if it is not required
										const pj_str_t* reason,		//Value NULL if it is not required
										const int retry_after,		//Value -1 if it is not required
										char* body,
										const pj_str_t* user_agent	//Identificador del user-agent
										)
{
	pjsip_wg67* wg67;
	pjsip_tx_data* tdata;
	pj_status_t status;
	pj_str_t pj_body;
	const pj_str_t ctype = pj_str("text");
	const pj_str_t csubtype = pj_str("plain");
	const pj_str_t pj_user_agent = pj_str("User-Agent");

	/* Check arguments. */
	PJ_ASSERT_RETURN(sub, PJ_EINVAL);

	/* Get the wg67 object. */
	wg67 = (pjsip_wg67*)pjsip_evsub_get_mod_data(sub, mod_wg67.id);
	PJ_ASSERT_RETURN(wg67 != NULL, PJSIP_SIMPLE_ENOPRESENCE);

	if (!force_subs_state)
	{
		/* Must have at least one wg67 info, unless state is
		 * PJSIP_EVSUB_STATE_TERMINATED. This could happen if subscription
		 * has not been active (e.g. we're waiting for user authorization)
		 * and remote cancels the subscription.
		 */
		PJ_ASSERT_RETURN(state == PJSIP_EVSUB_STATE_TERMINATED ||
			wg67->status.info_cnt > 0, PJSIP_SIMPLE_ENOPRESENCEINFO);
	}


	/* Lock object. */
	pjsip_dlg_inc_lock(wg67->dlg);

	/* Create the NOTIFY request. */
	if (force_subs_state)
	{
		status = pjsip_evsub_notify_force_subs_state(sub, expires, state, state_str, reason, retry_after, &tdata);
	}
	else
	{
		status = pjsip_evsub_notify(sub, state, state_str, reason, &tdata);
	}
	if (status != PJ_SUCCESS)
		goto on_return;

	if (body != NULL)
	{
		pj_body = pj_str(body);
		/* Create "text/plain" message body. */
		tdata->msg->body = pjsip_msg_body_create(tdata->pool, &ctype, &csubtype, &pj_body);
		if (tdata->msg->body == NULL) {
			pjsip_tx_data_dec_ref(tdata);
			goto on_return;
		}
	}

	if (user_agent != NULL)
	{
		if (pjsip_msg_find_hdr_by_name(tdata->msg, &pj_user_agent, NULL) == NULL)
		{
			const pj_str_t STR_USER_AGENT = { "User-Agent", 10 };
			pjsip_generic_string_hdr* pUserAgent = pjsip_generic_string_hdr_create(tdata->pool, &STR_USER_AGENT, user_agent);
			pj_list_push_back(&tdata->msg->hdr, pUserAgent);
		}
	}
	
on_return:
	pjsip_dlg_dec_lock(wg67->dlg);

	if (status == PJ_SUCCESS)
	{
		pjsip_wg67_send_request(sub, tdata);
	}

	return status;
}


/*
 * Create NOTIFY that reflect current state.
 */
PJ_DEF(pj_status_t) pjsip_wg67_current_notify(pjsip_evsub* sub, char *body, const pj_str_t* user_agent)
{
	pjsip_wg67* wg67;
	pjsip_tx_data* tdata;
	pj_status_t status;
	pj_str_t pj_body;
	const pj_str_t ctype = pj_str("text");
	const pj_str_t csubtype = pj_str("plain");
	const pj_str_t pj_user_agent = pj_str("User-Agent");

	/* Check arguments. */
	PJ_ASSERT_RETURN(sub, PJ_EINVAL);

	/* Get the presence object. */
	wg67 = (pjsip_wg67*)pjsip_evsub_get_mod_data(sub, mod_wg67.id);
	PJ_ASSERT_RETURN(wg67 != NULL, PJSIP_SIMPLE_ENOPRESENCE);

	/* We may not have a wg67 info yet, e.g. when we receive SUBSCRIBE
	 * to refresh subscription while we're waiting for user authorization.
	 */
	 //PJ_ASSERT_RETURN(pres->status.info_cnt > 0, 
	 //		       PJSIP_SIMPLE_ENOPRESENCEINFO);


	 /* Lock object. */
	pjsip_dlg_inc_lock(wg67->dlg);

	/* Create the NOTIFY request. */
	status = pjsip_evsub_current_notify(sub, &tdata);
	if (status != PJ_SUCCESS)
		goto on_return;

	pj_body = pj_str(body);

	/* Create "text/plain" message body. */
	tdata->msg->body = pjsip_msg_body_create(tdata->pool, &ctype, &csubtype, &pj_body);
	if (tdata->msg->body == NULL) {
		pjsip_tx_data_dec_ref(tdata);
		goto on_return;
	}

	if (user_agent != NULL)
	{
		if (pjsip_msg_find_hdr_by_name(tdata->msg, &pj_user_agent, NULL) == NULL)
		{
			const pj_str_t STR_USER_AGENT = { "User-Agent", 10 };
			pjsip_generic_string_hdr* pUserAgent = pjsip_generic_string_hdr_create(tdata->pool, &STR_USER_AGENT, user_agent);
			pj_list_push_back(&tdata->msg->hdr, pUserAgent);
		}
	}

on_return:
	pjsip_dlg_dec_lock(wg67->dlg);

	if (status == PJ_SUCCESS)
	{
		pjsip_wg67_send_request(sub, tdata);
	}

	return status;
}

