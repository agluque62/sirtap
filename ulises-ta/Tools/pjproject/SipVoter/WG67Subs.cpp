/*
En este modulo se implementa la clase que gestina una subscripcion al evento WG67KEY-IN
*/
#include "WG67Subs.h"
#include "SipAgent.h"
#include "SubscriptionServer.h"
#include "AccountUserData.h"

#undef THIS_FILE
#define THIS_FILE		"WG67Subs.cpp"

pjsip_evsub_user WG67Subs::wg67_callback =
{
	&WG67Subs::wg67_on_state,
	&WG67Subs::wg67_on_tsx_state,
	NULL,
	&WG67Subs::wg67_on_rx_notify,
	&WG67Subs::wg67_on_client_refresh,
	NULL
};

int WG67Subs::SendWG67Subscription(pjsua_acc_id acc_id, int call_id, char* dest_uri, int expires, pj_bool_t noRefresh, pj_bool_t by_proxy)
{
	int ret = CORESIP_OK;

	if (call_id < 0 && dest_uri == NULL)
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("SendWG67Subscription: Parametros erroneos");
		ret = CORESIP_ERROR;
		return ret;
	}

	if (SipAgent::_WG67Manager)
	{
		WG67Subs* wg67subs = SipAgent::_WG67Manager->GetSubsObj(acc_id, call_id, dest_uri);
		if (wg67subs != NULL)
		{
			if (wg67subs->_Expires == 0 && expires != 0)
			{
				throw PJLibException(__FILE__, PJ_EINVAL).Msg("INFO: SendWG67Subscription: Subscripcion cerrandose por previo expires a cero, no se permite nuevo expires distinto de cero");
				ret = CORESIP_ERROR;
				return ret;
			}

			//La subscripcion ya ha sido creada
			wg67subs->SendSubscriptionMessage(expires, noRefresh);
		}
		else if (SipAgent::_WG67Manager->Add(acc_id, call_id, dest_uri, expires, noRefresh, by_proxy) != 0) ret = CORESIP_ERROR;
	}
	else
	{
		ret = CORESIP_ERROR;
		PJ_CHECK_STATUS(-1, ("ERROR: WG67Subs::SendWG67Subscription: _WG67Manager no inicializado"));
	}

	return ret;
}

int WG67Subs::SetWG67SubscriptionParameters(pjsua_acc_id acc_id, int call_id, char* dest_uri, int noRefresh)
{
	int ret = CORESIP_OK;

	if (call_id < 0 && dest_uri == NULL)
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("SetWG67SubscriptionParameters: Parametros erroneos");
		ret = CORESIP_ERROR;
		return ret;
	}

	if (SipAgent::_WG67Manager)
	{
		WG67Subs* wg67subs = SipAgent::_WG67Manager->GetSubsObj(acc_id, call_id, dest_uri);
		if (wg67subs != NULL)
		{
			if (noRefresh >= 0)
			{
				if (noRefresh == 0)
				{
					if (wg67subs->after_no_refresh_force_destroy_subs_timer.id == PJ_TRUE)
					{
						wg67subs->SendSubscriptionMessage(wg67subs->_Expires, noRefresh);
					}
					else
					{
						wg67subs->_NoRefresh = PJ_FALSE;
					}
				}
				else
				{
					wg67subs->_NoRefresh = PJ_TRUE;
				}
			}
		}
		else
		{
			throw PJLibException(__FILE__, PJ_ENOTFOUND).Msg("SetWG67SubscriptionParameters: La subscripcion no ha sido creada previamente");
			ret = CORESIP_ERROR;
			return ret;
		}
	}
	else
	{
		ret = CORESIP_ERROR;
		PJ_CHECK_STATUS(-1, ("ERROR: WG67Subs::SetWG67SubscriptionParameters: _WG67Manager no inicializado"));
	}

	return ret;
}



/**
 * WG67Subs.	...
 * Constructor
 * @param	acc_id				Account del agente que utilizaremos. Si vale -1 entonces se usa la default.
 * @param	call_id				Si se quiere establecer la suscripcion con el dialogo de una sesion, es el call id de la sesion. Si vale -1 entonces se establece fuera de dialogo.
 * @param	dst_uri				Uri al que enviamos la subscripcion al evento. Si call_id es distinto de -1, se ignora 
 * @param	expires.			Valor del expires. Si vale -1 entonces toma el valor por defecto
 * @param	noRefresh			Si es true entonces el subscribe no se refresca automaticamente
 * @param	by_proxy. Si true entonces el subscribe se envia a traves del proxy
 * @return	-1 si hay error.
 */
WG67Subs::WG67Subs(pjsua_acc_id acc_id, pjsua_call_id call_id, char* dst_uri, int expires, pj_bool_t noRefresh, pj_bool_t by_proxy)
{
	_wg67subs_cb = NULL;
	_Pool = NULL;
	_Module = NULL;
	_SubsManager = NULL;
	_Expires = expires;
	_NoRefresh = noRefresh;
	_ByProxy = by_proxy;
	_Call_id = call_id;

	char ED137Radioversion_, ED137Phoneversion_;
	pjsip_endpt_Get_Ed137_version(pjsua_var.endpt, &ED137Radioversion_, &ED137Phoneversion_);
	ED137Version = ED137Radioversion_;
	last_subscription_state = PJSIP_EVSUB_STATE_UNKNOWN;
	after_no_refresh_force_destroy_subs_timer.id = PJ_FALSE;

	if (acc_id == -1)
	{
		_Acc_id = pjsua_acc_get_default();
	}
	else
	{
		_Acc_id = acc_id;
	}

	if (!pjsua_acc_is_valid(_Acc_id))
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("WG67Subs: Account no valida");
		return;
	}
	
	if ((strlen(dst_uri) + 1) > sizeof(_Dst_uri))
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("WG67Subs: dst_uri demasiado largo");
		return;
	}

	strcpy(_Dst_uri, dst_uri);
	_Dst_uri_pj = pj_str(_Dst_uri);
}

WG67Subs::~WG67Subs()
{
	_wg67subs_cb = NULL;
	_SubsManager = NULL;
	after_no_refresh_force_destroy_subs_timer.id = PJ_FALSE;
	pjsua_cancel_timer(&after_no_refresh_force_destroy_subs_timer);	
}

/**
 * Init.	...
 * inicializa la subscripcion. Si no hay existo lo reintenta.
 * @param wg67sub_cb. Callback para pasar notificaciones al ClientSubscriptionManager
 * @param subsmanager. Puntero del ClientSubscriptionManager.
 * @return	-1 si hay error.
 */
int WG67Subs::Init(WG67Subs_callback wg67sub_cb, void* subsmanager)
{
	_wg67subs_cb = NULL;
	_SubsManager = subsmanager;
	
	_Pool = pjsua_pool_create(NULL, 64, 32);
	if (_Pool == NULL)
	{
		PJ_LOG(3, (__FILE__, "ERROR: WG67Subs::Init: No se puede crear pool de memoria acc_id %d dst %s", _Acc_id, _Dst_uri));
		return -1;
	}

	if (pj_sem_create(_Pool, NULL, 0, 1, &status_sem) != PJ_SUCCESS)
	{
		PJ_LOG(3, (__FILE__, "ERROR: WG67Subs::Init: No se puede crear semaforo acc_id %d dst %s", _Acc_id, _Dst_uri));
		return -1;
	}

	int tries = 5;
	while (tries > 0)
	{		
		int ret = InicializarSubscripcion();
		if (ret == 0)
		{
			if (last_subscription_state == PJSIP_EVSUB_STATE_TERMINATED)
			{
				//Se encuentra que la subscripcion ha terminado porque posiblemente ha sido rechazada. No reintentamos 
				return -1;
			}

			//Se ha activado la subscripcion sin errores
			//Activamos la callback
			_wg67subs_cb = wg67sub_cb;
			return 0;
		}
		tries--;
		pj_thread_sleep(50);
	}
	PJ_LOG(3, (__FILE__, "ERROR: WG67Subs::Init: No se ha podido realizar la subscripcion a WG67KEY-IN despues de %d intentos acc_id %d dst %s", tries, _Acc_id, _Dst_uri));
	return -1;
}

int WG67Subs::SendSubscriptionMessage(int expires, pj_bool_t noRefresh)
{
	pjsip_tx_data* tdata;

	pjsip_dialog *dlg = pjsip_evsub_get_dlg(_Module);
	if (dlg == NULL)
	{
		return -1;
	}

	if (noRefresh == PJ_FALSE)
	{
		after_no_refresh_force_destroy_subs_timer.id = PJ_FALSE;
		pjsua_cancel_timer(&after_no_refresh_force_destroy_subs_timer);		
	}

	pjsip_dlg_inc_lock(dlg);

	pj_status_t st = pjsip_wg67_initiate(_Module, expires, &tdata);
	if (st != PJ_SUCCESS)
	{
		PJ_LOG(3, (__FILE__, "ERROR: WG67Subs::SendSubscriptionMessage: Unable to initiate client %s", _Dst_uri));
		if (_Module) pjsip_wg67_terminate(_Module, PJ_FALSE);
		_Module = NULL;
		if (dlg) pjsip_dlg_dec_lock(dlg);
		return -1;
	}

	pjsua_process_msg_data(tdata, NULL);

	while (pj_sem_trywait(status_sem) == PJ_SUCCESS);
	st = pjsip_wg67_send_request(_Module, tdata);
	if (st != PJ_SUCCESS)
	{
		PJ_LOG(3, (__FILE__, "ERROR: WG67Subs::InicializarSubscripcion: Unable to send initial SUBSCRIBE message %s", _Dst_uri));
		if (_Module) pjsip_wg67_terminate(_Module, PJ_FALSE);
		_Module = NULL;
		if (dlg) pjsip_dlg_dec_lock(dlg);
		return -1;
	}

	_NoRefresh = noRefresh;
	_Expires = expires;

	if (dlg) pjsip_dlg_dec_lock(dlg);
	return 0;
}

/**
 * InicializarSubscripcion.	...
 * Inicializa la subscripcion
 * @return	-1 si hay error.
 */
int WG67Subs::InicializarSubscripcion()
{
	pj_status_t st = PJ_SUCCESS;
	pjsip_dialog* dlg = NULL;						//Dialogo necesario para la subscripcion
	last_subscription_state = PJSIP_EVSUB_STATE_UNKNOWN;	

	pjsua_acc* acc = &pjsua_var.acc[_Acc_id];
	if (!acc->valid)
	{
		PJ_LOG(3, (__FILE__, "ERROR: WG67Subs::InicializarSubscripcion: Account %d no valido", _Acc_id));
		return -1;
	}

	if (_Call_id == -1)
	{
		pj_str_t contact;
		st = pjsua_acc_create_uac_contact(_Pool, &contact, _Acc_id, &_Dst_uri_pj, NULL);
		if (st != PJ_SUCCESS)
		{
			PJ_LOG(3, (__FILE__, "ERROR: WG67Subs::InicializarSubscripcion: No se puede generar contact para acc %d dst %s", _Acc_id, _Dst_uri_pj.ptr));
			return -1;
		}

		st = pjsip_dlg_create_uac(pjsip_ua_instance(), &acc->cfg.id, &contact, &_Dst_uri_pj, NULL, &dlg);
		if (st != PJ_SUCCESS)
		{
			PJ_LOG(3, (__FILE__, "ERROR: WG67Subs::InicializarSubscripcion: Unable to create dialog  acc_id %d dst %s", _Acc_id, _Dst_uri));
			return -1;
		}

		pjsip_dlg_inc_lock(dlg);
	}
	else
	{
		pjsua_call* call;
		st = acquire_call("WG67Subs::InicializarSubscripcion()", _Call_id, &call, &dlg);
		if (st != PJ_SUCCESS)
		{
			PJ_LOG(3, (__FILE__, "ERROR: WG67Subs::InicializarSubscripcion: Unable to acquire call _Call_id %d dst %s", _Call_id, _Dst_uri));
			return -1;
		}

		//Buscamos una subscripcion a este evento dentro del dialogo y esperamos a que desaparezca
		pjsip_evsub* wg67_active_subs = search_wg67_subscription_in_dialog(dlg);
		if (wg67_active_subs != NULL)
		{
			int tries = 50;
			pjsip_evsub_state subs_state = pjsip_evsub_get_state(wg67_active_subs);

			while (wg67_active_subs != NULL && subs_state != PJSIP_EVSUB_STATE_NULL && tries > 0)
			{
				if (dlg) pjsip_dlg_dec_lock(dlg);				
				pj_thread_sleep(100);

				st = acquire_call("WG67Subs::InicializarSubscripcion()", _Call_id, &call, &dlg);
				if (st != PJ_SUCCESS)
				{
					PJ_LOG(3, (__FILE__, "ERROR: WG67Subs::InicializarSubscripcion: Unable to acquire call _Call_id %d dst %s", _Call_id, _Dst_uri));
					return -1;
				}

				tries--;
				wg67_active_subs = search_wg67_subscription_in_dialog(dlg);
				if (wg67_active_subs != NULL) subs_state = pjsip_evsub_get_state(wg67_active_subs);
			}
			if (tries == 0)
			{
				PJ_LOG(3, (__FILE__, "ERROR: WG67Subs::InicializarSubscripcion: there is already a subscription in this dialog Call_id %d dst %s", _Call_id, _Dst_uri));
				if (dlg) pjsip_dlg_dec_lock(dlg);
				return -1;
			}
			else
			{
				PJ_LOG(5, (__FILE__, "INFO: WG67Subs::InicializarSubscripcion: there is not a subscription in this dialog Call_id %d dst %s", _Call_id, _Dst_uri));
			}
		}
	}

	st = pjsip_wg67_create_uac(dlg, &wg67_callback, PJSIP_EVSUB_NO_EVENT_ID, &_Module);
	if (st != PJ_SUCCESS)
	{
		PJ_LOG(3, (__FILE__, "ERROR: WG67Subs::InicializarSubscripcion: Unable to create client %s", _Dst_uri));
		_Module = NULL;
		if (dlg) pjsip_dlg_dec_lock(dlg);
		return -1;
	}

	if (_ByProxy)
	{
		//Si el subscribe va hacia el proxy entonces se agrega la cabecera route
		if (!pj_list_empty(&acc->route_set))
		{
			pjsip_dlg_set_route_set(dlg, &acc->route_set);
		}
	}

	pjsip_evsub_set_mod_data(_Module, pjsua_var.mod.id, this);	

	pj_pool_t* pool = pjsip_evsub_get_pool(_Module);
	SubscriptionServer::subs_user_data* sub_user_data = (SubscriptionServer::subs_user_data*)pj_pool_alloc(pool, sizeof(SubscriptionServer::subs_user_data));
	if (sub_user_data == NULL)
	{
		PJ_LOG(3, (__FILE__, "ERROR: WG67Subs::InicializarSubscripcion: out of memory"));
		_Module = NULL;
		if (dlg) pjsip_dlg_dec_lock(dlg);
		return -1;
	}
	sub_user_data->accid = _Acc_id;
	memset(&sub_user_data->last_info, 0, sizeof(sub_user_data));
	pjsip_evsub_set_user_data(_Module, (void*)sub_user_data);

	//Se establece la cabacera WG67-Version para el envio de la primera subscripcion
	char ED137Radioversion_, ED137Phoneversion_;
	pjsip_endpt_Get_Ed137_version(pjsua_var.endpt, &ED137Radioversion_, &ED137Phoneversion_);
	char sWG67ver[32];
	if (ED137Radioversion_ == 'C')
		pj_ansi_sprintf(sWG67ver, "%.*s, %.*s", SipCall::gWG67VersionRadioValue.slen, SipCall::gWG67VersionRadioValue.ptr, 
			SipCall::gWG67VersionRadioValueC.slen, SipCall::gWG67VersionRadioValueC.ptr);
	else
		pj_ansi_sprintf(sWG67ver, "%.*s", SipCall::gWG67VersionRadioValue.slen, SipCall::gWG67VersionRadioValue.ptr);
	pjsip_dlg_set_WG67_version(dlg, sWG67ver, PJ_FALSE);

	if (SendSubscriptionMessage(_Expires, _NoRefresh) < 0)
	{
		PJ_LOG(3, (__FILE__, "ERROR: WG67Subs::InicializarSubscripcion: Unable to send subscription message %s", _Dst_uri));
		if (_Module) pjsip_wg67_terminate(_Module, PJ_FALSE);
		_Module = NULL;
		if (dlg) pjsip_dlg_dec_lock(dlg);
		return -1;
	}

	if (dlg) pjsip_dlg_dec_lock(dlg);
	
	pj_sem_wait_for(status_sem, pjsip_cfg()->tsx.tsx_tout / 2);		//esperamos el tiempo maximo para una transaccion	
	if (_Module)
	{
		return 0;
	}

	PJ_LOG(3, (__FILE__, "ERROR: WG67Subs::InicializarSubscripcion: SUBSCRIBE message is not answered %s", _Dst_uri));

	return -1;
}

/**
 * End.	...
 * Finaliza la subscripcion
 * @return	-1 si hay error.
 */
int WG67Subs::End()
{
	after_no_refresh_force_destroy_subs_timer.id = PJ_FALSE;
	pjsua_cancel_timer(&after_no_refresh_force_destroy_subs_timer);	

	if (last_subscription_state == PJSIP_EVSUB_STATE_TERMINATED)
	{
		goto on_return;
	}

	if (_Module == NULL)
	{
		goto on_return;
	}

	_wg67subs_cb = NULL;	//No quiero que se llame a esta callback cuando quiero finalizar la subscripcion.

	pjsip_evsub_state sta = pjsip_evsub_get_state(_Module);

	if (sta == PJSIP_EVSUB_STATE_TERMINATED)
	{
		goto on_return;
	}

	pjsip_tx_data* tdata;
	pj_status_t st = pjsip_wg67_initiate(_Module, 0, &tdata);
	if (st != PJ_SUCCESS)
	{
		if (_Module)
		{
			pjsip_evsub_set_mod_data(_Module, pjsua_var.mod.id, NULL);
			pjsip_wg67_terminate(_Module, PJ_FALSE);
		}
		goto on_return;
	}

	pjsua_process_msg_data(tdata, NULL);
	while (pj_sem_trywait(status_sem) == PJ_SUCCESS);
	st = pjsip_wg67_send_request(_Module, tdata);
	if (st != PJ_SUCCESS)
	{
		if (_Module)
		{
			pjsip_evsub_set_mod_data(_Module, pjsua_var.mod.id, NULL);
			pjsip_wg67_terminate(_Module, PJ_FALSE);
		}
		goto on_return;
	}
	else
	{
		//Esperamos a que se termine la subscripcion recibiendo el NOTIFY con estado a terminated
		int tries = pjsip_cfg()->tsx.tsx_tout / 40;
		if (tries > 2) tries -= 2;						//Paramos antes del tiempo tsx_tout para que no salte el timeout y provoque una excepcion
		else if (tries > 1) tries -= 1;
		pjsip_evsub_state status = pjsip_evsub_get_state(_Module);
		while (status != PJSIP_EVSUB_STATE_TERMINATED && _Module)
		{
			pj_sem_wait_for(status_sem, 40);
			tries--;
			if (tries == 0) break;
			if (_Module)
			{
				status = pjsip_evsub_get_state(_Module);
			}
		}

		if (status != PJSIP_EVSUB_STATE_TERMINATED && _Module)
		{
			pjsip_evsub_set_mod_data(_Module, pjsua_var.mod.id, NULL);
		}
	}

on_return:
	_Module = NULL;
	_wg67subs_cb = NULL;
	_SubsManager = NULL;
	if (_Pool != NULL)
	{
		pj_pool_release(_Pool);
	}

	return 0;
}

void WG67Subs::wg67_on_tsx_state(pjsip_evsub* sub, pjsip_transaction* tsx,	pjsip_event* event)
{
	pjsip_evsub_state state = PJSIP_EVSUB_STATE_NULL;
	WG67Subs* wg67 = reinterpret_cast<WG67Subs*>(pjsip_evsub_get_mod_data(sub, pjsua_var.mod.id));
	if ((tsx->role == PJSIP_ROLE_UAC) && pjsip_method_cmp(&tsx->method, &pjsip_subscribe_method) == 0)
	{
		state = pjsip_evsub_get_state(sub);
		if ((state == PJSIP_EVSUB_STATE_ACTIVE || state == PJSIP_EVSUB_STATE_SENT || state == PJSIP_EVSUB_STATE_PENDING) &&
			(tsx->state == PJSIP_TSX_STATE_COMPLETED && tsx->status_code == PJSIP_SC_INTERVAL_TOO_BRIEF && event->body.tsx_state.type == PJSIP_EVENT_RX_MSG))
		{
			//Si la subscripcion esta activa y se recibe un 423 
			WG67Subs* wg67 = reinterpret_cast<WG67Subs*>(pjsip_evsub_get_mod_data(sub, pjsua_var.mod.id));
			if (wg67)
			{
				pjsip_msg* msg = event->body.tsx_state.src.rdata->msg_info.msg;
				if (msg)
				{
					pjsip_min_expires_hdr* min_expires = (pjsip_min_expires_hdr*)pjsip_msg_find_hdr(msg, PJSIP_H_MIN_EXPIRES, NULL);
					if (min_expires)
					{
						if (wg67->SendSubscriptionMessage(min_expires->ivalue, wg67->_NoRefresh) < 0)
						{
							PJ_LOG(3, (__FILE__, "ERROR: WG67Subs::wg67_on_tsx_state: No se puede reenviar el Subscribe despues de recibir el 423"));
						}
					}
				}
			}
		}
	}

	pjsip_dialog *subdlg = pjsip_evsub_get_dlg(sub);
	if (wg67 && subdlg != NULL && tsx->role == PJSIP_ROLE_UAC && event->type == PJSIP_EVENT_TSX_STATE && event->body.tsx_state.type == PJSIP_EVENT_RX_MSG)
	{
		//Se ha recibido un mensaje SIP y se establece la cabecera WG67-version si no lo estaba
		if (pjsip_dlg_get_WG67_version(subdlg, NULL) == PJ_FALSE)
		{
			char remote_radio_version = 0;
			char remote_phone_version = 0;
			pjsip_endpt_Get_ED137Version_from_msg(event->body.rx_msg.rdata->msg_info.msg, &remote_radio_version, &remote_phone_version);
			wg67->ED137Version = pjsip_endpt_Negocia_ED137Version(wg67->ED137Version, remote_radio_version);
			if (wg67->ED137Version == 'C')
			{
				pjsip_dlg_set_WG67_version(subdlg, "radio.02", PJ_TRUE);
				pjsip_tsx_set_WG67_version(tsx, "radio.02");
			}
			else
			{
				pjsip_dlg_set_WG67_version(subdlg, "radio.01", PJ_TRUE);
				pjsip_tsx_set_WG67_version(tsx, "radio.01");
			}
		}
	}

}

void WG67Subs::wg67_on_state(pjsip_evsub* sub, pjsip_event* event)
{
	PJ_UNUSED_ARG(event);

	pj_bool_t process_Callback = PJ_FALSE;
	pjsip_evsub_state state = PJSIP_EVSUB_STATE_NULL;
	pj_bool_t post_status_sem = PJ_FALSE;
	int resub_delay = -1;
	pjsip_dialog* subdlg = NULL;
	
	WG67Subs* wg67 = reinterpret_cast<WG67Subs*>(pjsip_evsub_get_mod_data(sub, pjsua_var.mod.id));

	if (wg67)
	{
		PJ_LOG(3, (THIS_FILE, "WG67KEY-IN subscription to %.*s is %s",
			wg67->_Dst_uri_pj.slen, wg67->_Dst_uri_pj.ptr, pjsip_evsub_get_state_name(sub)));

		
		CORESIP_WG67_Subscription_Info info;
		strcpy(info.Role, "subscriber");
		info.NotifyReceived = 0;
		info.SubscriberUri[0] = '\0';
		info.SubscriptionState[0] = '\0';
		info.Reason[0] = '\0';
		info.Expires = -1;
		info.Retry_after = -1;
		info.RawBody[0] = '\0';
		info.Found_Parse_Errors = 0;
		info.SessionsCount = 0;
		info.RawBody[0] = '\0';
		info.Fid[0] = '\0';
		info.WG67_Version[0] = '\0';

		pj_ansi_strcpy(info.NotifierUri, wg67->_Dst_uri);

		pjsua_acc *acc = &pjsua_var.acc[wg67->_Acc_id];		
		pjsip_uri *uri = pjsip_parse_uri(wg67->_Pool, acc->cfg.id.ptr, acc->cfg.id.slen, 0);
		if (uri != NULL)
		{
			pjsip_sip_uri* url = (pjsip_sip_uri*)pjsip_uri_get_uri(uri);
			int n_writen;
			if (url->port > 0)
			{
				n_writen = pj_ansi_snprintf(info.SubscriberUri, sizeof(info.SubscriberUri), "sip:%.*s@%.*s:%d",
					url->user.slen, url->user.ptr, url->host.slen, url->host.ptr, url->port);
			}
			else
			{
				n_writen = pj_ansi_snprintf(info.SubscriberUri, sizeof(info.SubscriberUri), "sip:%.*s@%.*s",
					url->user.slen, url->user.ptr, url->host.slen, url->host.ptr);
			}
			if (n_writen <= 0)
			{
				info.SubscriberUri[0] = '\0';
			}
		}

		state = pjsip_evsub_get_state(sub);
		wg67->last_subscription_state = state;
		switch (state)
		{
		case PJSIP_EVSUB_STATE_NULL:
			PJ_LOG(3, (THIS_FILE, "WG67Subs::wg67_on_state PJSIP_EVSUB_STATE_NULL dst %s", wg67->_Dst_uri_pj.ptr));			
			break;
		case PJSIP_EVSUB_STATE_SENT:
			PJ_LOG(3, (THIS_FILE, "WG67Subs::wg67_on_state PJSIP_EVSUB_STATE_SENT dst %s", wg67->_Dst_uri_pj.ptr));			
			break;
		case PJSIP_EVSUB_STATE_ACCEPTED:
			PJ_LOG(3, (THIS_FILE, "WG67Subs::wg67_on_state PJSIP_EVSUB_STATE_ACCEPTED dst %s", wg67->_Dst_uri_pj.ptr));
			post_status_sem = PJ_TRUE;
			break;
		case PJSIP_EVSUB_STATE_PENDING:
			PJ_LOG(3, (THIS_FILE, "WG67Subs::wg67_on_state PJSIP_EVSUB_STATE_PENDING dst %s", wg67->_Dst_uri_pj.ptr));
			strcpy(info.SubscriptionState, "pending");
			process_Callback = PJ_TRUE;
			break;
		case PJSIP_EVSUB_STATE_ACTIVE:
			PJ_LOG(3, (THIS_FILE, "WG67Subs::wg67_on_state PJSIP_EVSUB_STATE_ACTIVE dst %s", wg67->_Dst_uri_pj.ptr));
			strcpy(info.SubscriptionState, "active");
			process_Callback = PJ_TRUE;
			post_status_sem = PJ_TRUE;
			break;
		case PJSIP_EVSUB_STATE_TERMINATED:
			PJ_LOG(3, (THIS_FILE, "WG67Subs::wg67_on_state PJSIP_EVSUB_STATE_TERMINATED dst %s", wg67->_Dst_uri_pj.ptr));
			strcpy(info.SubscriptionState, "terminated");
			process_Callback = PJ_TRUE;

			//Ha cambiado el estado de la subscripcion
			//wg67->_Module = NULL;
			pjsip_evsub_set_mod_data(sub, pjsua_var.mod.id, NULL);			
			
			subdlg = pjsip_evsub_get_dlg(sub);
			if (subdlg != NULL)
			{
				pjsip_dlg_inc_lock(subdlg);
				subdlg->uac_has_2xx = PJ_FALSE;		//Permitimos un nuevo dialogo de un nuevo subscribe
				pjsip_dlg_dec_lock(subdlg);
			}
			
			post_status_sem = PJ_TRUE;			

			break;
		case PJSIP_EVSUB_STATE_UNKNOWN:
			PJ_LOG(3, (THIS_FILE, "WG67Subs::wg67_on_state PJSIP_EVSUB_STATE_UNKNOWN dst %s", wg67->_Dst_uri_pj.ptr));
			break;
		}

		if (process_Callback == PJ_TRUE)
		{
			if (wg67->ED137Version == 'C') strcpy(info.WG67_Version, "radio.02");
			else strcpy(info.WG67_Version, "radio.01");
			SubscriptionServer::subs_user_data* sub_user_data = (SubscriptionServer::subs_user_data*)pjsip_evsub_get_user_data(sub);
			if (sub_user_data != NULL) CallSubscribeCallback(&sub_user_data->last_info, &info);
		}
		
		WG67Subs* ret_wg67 = wg67;
		if (wg67->_wg67subs_cb != NULL)
		{
			ret_wg67 = (WG67Subs * ) wg67->_wg67subs_cb((void*)wg67);
		}

		if (ret_wg67 != NULL && post_status_sem)
		{
			//Post del semaforo siempre y cuando el objeto de la subscripcion no se haya destruido
			pj_sem_post(wg67->status_sem);
		}
	}
}

void WG67Subs::wg67_on_rx_notify(pjsip_evsub* sub, pjsip_rx_data* rdata, int* p_st_code,
	pj_str_t** p_st_text, pjsip_hdr* res_hdr, pjsip_msg_body** p_body)
{
	WG67Subs* wg67 = reinterpret_cast<WG67Subs*>(pjsip_evsub_get_mod_data(sub, pjsua_var.mod.id));

	if (wg67)
	{	
		CORESIP_WG67_Subscription_Info info;
		strcpy(info.Role, "subscriber");
		info.NotifyReceived = 1;
		info.SubscriberUri[0] = '\0';
		info.SubscriptionState[0] = '\0';
		info.Reason[0] = '\0';
		info.Expires = -1;
		info.Retry_after = -1;
		info.RawBody[0] = '\0';
		info.Found_Parse_Errors = 0;
		info.SessionsCount = 0;
		info.RawBody[0] = '\0';
		info.Fid[0] = '\0';
		info.WG67_Version[0] = '\0';

		//Si el codigo de retorno no es 200 quiere decir que no se ha podido parsear bien. Retorno
		if (*p_st_code != 200)
		{
			return;
		}	

		pjsip_generic_string_hdr* wg67version_hdr = (pjsip_generic_string_hdr*)pjsip_msg_find_hdr_by_name(rdata->msg_info.msg, &SipCall::gWG67VersionName, NULL);
		if (wg67version_hdr != NULL)
		{
			pj_ansi_snprintf(info.WG67_Version, sizeof(info.WG67_Version), "%.*s", wg67version_hdr->hvalue.slen, wg67version_hdr->hvalue.ptr);
		}
		
		pj_ansi_strcpy(info.NotifierUri, wg67->_Dst_uri);

		pjsua_acc* acc = &pjsua_var.acc[wg67->_Acc_id];
		pjsip_uri* uri = pjsip_parse_uri(wg67->_Pool, acc->cfg.id.ptr, acc->cfg.id.slen, 0);
		if (uri != NULL)
		{
			pjsip_sip_uri* url = (pjsip_sip_uri*)pjsip_uri_get_uri(uri);
			int n_writen;
			if (url->port > 0)
			{
				n_writen = pj_ansi_snprintf(info.SubscriberUri, sizeof(info.SubscriberUri), "sip:%.*s@%.*s:%d",
					url->user.slen, url->user.ptr, url->host.slen, url->host.ptr, url->port);
			}
			else
			{
				n_writen = pj_ansi_snprintf(info.SubscriberUri, sizeof(info.SubscriberUri), "sip:%.*s@%.*s",
					url->user.slen, url->user.ptr, url->host.slen, url->host.ptr);
			}
			if (n_writen <= 0)
			{
				info.SubscriberUri[0] = '\0';
			}
		}

		/*Si se recibe un Notify durante el timer que esta esperando a cerrar la subscripcion entonces lo cancelamos*/
		wg67->after_no_refresh_force_destroy_subs_timer.id = PJ_FALSE;
		pjsua_cancel_timer(&wg67->after_no_refresh_force_destroy_subs_timer);

		const pjsip_sub_state_hdr* sub_hdr;
		pj_str_t sub_state = { "Subscription-State", 18 };
		const pjsip_msg* msg;
		msg = rdata->msg_info.msg;
		sub_hdr = (const pjsip_sub_state_hdr*)pjsip_msg_find_hdr_by_name(msg, &sub_state, NULL);
		if (sub_hdr != NULL)
		{
			pj_ansi_snprintf(info.SubscriptionState, sizeof(info.SubscriptionState), "%.*s", sub_hdr->sub_state.slen, sub_hdr->sub_state.ptr);
			pj_ansi_snprintf(info.Reason, sizeof(info.Reason), "%.*s", sub_hdr->reason_param.slen, sub_hdr->reason_param.ptr);
			info.Expires = sub_hdr->expires_param;
			info.Retry_after = sub_hdr->retry_after;
		}
		else
		{
			info.Found_Parse_Errors++;
		}

		if (rdata->msg_info.msg->body == NULL)
		{
			SubscriptionServer::subs_user_data* sub_user_data = (SubscriptionServer::subs_user_data*)pjsip_evsub_get_user_data(sub);
			if (sub_user_data != NULL) CallSubscribeCallback(&sub_user_data->last_info, &info);
			return;
		}				

		int len = rdata->msg_info.msg->body->print_body(rdata->msg_info.msg->body, info.RawBody, sizeof(info.RawBody) - 1);
		if (len < 0)
		{
			info.RawBody[0] = '\0';
		}
		else
		{
			info.RawBody[len] = '\0';
		}

		pjsip_wg67_status wg67_info;
		pjsip_wg67_get_status(sub, &wg67_info);		
		
		if (wg67_info.fid.slen > 0) pj_ansi_snprintf(info.Fid, sizeof(info.Fid), "%.*s", wg67_info.fid.slen, wg67_info.fid.ptr);

		for (unsigned i = 0; i < wg67_info.info_cnt; i++)
		{
			info.SessionInfo[info.SessionsCount].PttId = wg67_info.info[i].pttid;				

			int n_writen = pj_ansi_snprintf(info.SessionInfo[info.SessionsCount].Uri, sizeof(info.SessionInfo[info.SessionsCount].Uri), "%.*s",
				wg67_info.info[i].uri.slen, wg67_info.info[i].uri.ptr);
			if (n_writen < 0)
			{
				info.Found_Parse_Errors++;
				PJ_LOG(3, (THIS_FILE, "ERROR: WG67Subs::wg67_on_rx_notify uri recibida en NOTIFY fuera de rango"));
			}

			n_writen = pj_ansi_snprintf(info.SessionInfo[info.SessionsCount].SessionType, sizeof(info.SessionInfo[info.SessionsCount].SessionType), "%.*s",
				wg67_info.info[i].sessiontype.slen, wg67_info.info[i].sessiontype.ptr);
			if (n_writen < 0)
			{
				info.Found_Parse_Errors++;
				PJ_LOG(3, (THIS_FILE, "ERROR: WG67Subs::wg67_on_rx_notify SessionType recibido en NOTIFY fuera de rango"));
			}

			// PlugTest FAA 05/2011
			info.SessionsCount++;
		}		

		SubscriptionServer::subs_user_data* sub_user_data = (SubscriptionServer::subs_user_data*)pjsip_evsub_get_user_data(sub);
		if (sub_user_data != NULL) CallSubscribeCallback(&sub_user_data->last_info, &info);
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
 * wg67_on_client_refresh.	...
 * Funcion callback que se llama cada vez que hay que refrescar la subscripcion.
 * Se utiliza para poder agregar la cabecera WG67-version que corresponde
 */
void WG67Subs::wg67_on_client_refresh(pjsip_evsub* sub)
{
	pj_status_t status;
	pjsip_tx_data* tdata;

	WG67Subs* wg67 = reinterpret_cast<WG67Subs*>(pjsip_evsub_get_mod_data(sub, pjsua_var.mod.id));
	if (wg67)
	{
		if (wg67->_NoRefresh)
		{
			//Se arranca timer para finalizar la subscripcion en caso de que no se reciba un NOTIFY o se reanude el refresco, en cuyo caso se cancelaria el timer

			if (wg67->after_no_refresh_force_destroy_subs_timer.id == PJ_TRUE)
			{
				return;
			}

			pj_time_val delay;
			pj_timer_entry_init(&wg67->after_no_refresh_force_destroy_subs_timer, PJ_FALSE, wg67, &force_destroy_subs_timer_cb);
			delay.sec = 7;
			delay.msec = 0;
			if (pjsua_schedule_timer(&wg67->after_no_refresh_force_destroy_subs_timer, &delay) == PJ_SUCCESS)
				wg67->after_no_refresh_force_destroy_subs_timer.id = PJ_TRUE;
			else
				PJ_LOG(3, (THIS_FILE, "ERROR: WG67Subs::wg67_on_client_refresh: No se puede arrancal el timer force_destroy_subs_timer_cb"));
			return;
		}
	}

	status = pjsip_wg67_initiate(sub, -1, &tdata);
	if (status == PJ_SUCCESS) {

		if (pjsip_msg_find_hdr_by_name(tdata->msg, &pj_str("User-Agent"), NULL) == NULL)
		{
			const pj_str_t STR_USER_AGENT = { "User-Agent", 10 };
			pjsip_generic_string_hdr* pUserAgent = pjsip_generic_string_hdr_create(tdata->pool, &STR_USER_AGENT, &pjsua_var.ua_cfg.user_agent);
			pj_list_push_back(&tdata->msg->hdr, pUserAgent);
		}

		pjsip_evsub_send_request(sub, tdata);
	}
}

void WG67Subs::force_destroy_subs_timer_cb(pj_timer_heap_t* th, pj_timer_entry* entry)
{
	WG67Subs* wg67 = (WG67Subs*)entry->user_data;

	PJ_UNUSED_ARG(th);

	pjsua_cancel_timer(&wg67->after_no_refresh_force_destroy_subs_timer);
	if (entry->id == PJ_FALSE)
	{
		return;
	}
	entry->id = PJ_FALSE;

	if (wg67->last_subscription_state == PJSIP_EVSUB_STATE_TERMINATED)
	{
		return;
	}

	if (wg67->_Module == NULL)
	{
		return;
	}

	pjsip_evsub_state sta = pjsip_evsub_get_state(wg67->_Module);
	if (sta != PJSIP_EVSUB_STATE_TERMINATED)
	{
		pjsip_wg67_terminate(wg67->_Module, PJ_TRUE);
	}
}



/********************************************************************************************/
/* A partir de aqui se sefiere a cuando funciona como servidor de subscripcion*/
/********************************************************************************************/

pjsip_evsub_user WG67Subs::_WG67SrvCb =
{
	&WG67Subs::OnWG67SrvStateChanged,
	&WG67Subs::OnWG67SrvTsxChanged,
	&WG67Subs::OnWG67RxRefresh,
	NULL,
	NULL,
	&WG67Subs::OnWG67SrvTimeout
};


void WG67Subs::OnWG67SrvStateChanged(pjsip_evsub* sub, pjsip_event* event)
{
	pj_bool_t process_Callback = PJ_FALSE;
	pjsip_evsub_state state = pjsip_evsub_get_state(sub);
	pj_int32_t exp = pjsip_evsub_get_expires(sub);

	SubscriptionServer::subs_user_data* sub_user_data = (SubscriptionServer::subs_user_data*)pjsip_evsub_get_user_data(sub);
	AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(sub_user_data->accid);

	CORESIP_WG67_Subscription_Info info;
	strcpy(info.Role, "notifier");
	info.NotifyReceived = 0;
	info.SubscriberUri[0] = '\0';
	info.NotifierUri[0] = '\0';
	info.SubscriptionState[0] = '\0';
	info.Reason[0] = '\0';
	info.Expires = exp;
	info.Retry_after = -1;
	info.RawBody[0] = '\0';
	info.Found_Parse_Errors = 0;
	info.SessionsCount = 0;
	info.RawBody[0] = '\0';
	info.Fid[0] = '\0';
	info.WG67_Version[0] = '\0';

	switch (state)
	{
	case PJSIP_EVSUB_STATE_PENDING:
		strcpy(info.SubscriptionState, "pending");
		process_Callback = PJ_TRUE;
		break;
	case PJSIP_EVSUB_STATE_ACTIVE:
		strcpy(info.SubscriptionState, "active");
		process_Callback = PJ_TRUE;
		break;
	case PJSIP_EVSUB_STATE_TERMINATED:
		strcpy(info.SubscriptionState, "terminated");
		process_Callback = PJ_TRUE;

		/*Al terminar la subscripcion al evento, lo eliminamos de la lista del account correspondiente*/
		SubscriptionServer::Del_subMod(sub);
	}

	if (process_Callback)
	{
		pjsip_dialog* subdlg = pjsip_evsub_get_dlg(sub);
		
			
		pjsip_contact_hdr *remote_contact_hdr = subdlg->remote.contact;
		pjsip_uri *remote_contact_uri = remote_contact_hdr->uri;
		pjsip_sip_uri* url = (pjsip_sip_uri*)pjsip_uri_get_uri(remote_contact_uri);
		int n_writen;
		if (url->port > 0)
		{
			n_writen = pj_ansi_snprintf(info.SubscriberUri, sizeof(info.SubscriberUri), "sip:%.*s@%.*s:%d",
				url->user.slen, url->user.ptr, url->host.slen, url->host.ptr, url->port);
		}
		else
		{
			n_writen = pj_ansi_snprintf(info.SubscriberUri, sizeof(info.SubscriberUri), "sip:%.*s@%.*s",
				url->user.slen, url->user.ptr, url->host.slen, url->host.ptr);
		}
		if (n_writen <= 0)
		{
			info.SubscriberUri[0] = '\0';
		}

		pjsip_contact_hdr* local_contact_hdr = subdlg->local.contact;
		pjsip_uri* local_contact_uri = local_contact_hdr->uri;
		url = (pjsip_sip_uri*)pjsip_uri_get_uri(local_contact_uri);
		if (url->port > 0)
		{
			n_writen = pj_ansi_snprintf(info.NotifierUri, sizeof(info.NotifierUri), "sip:%.*s@%.*s:%d",
				url->user.slen, url->user.ptr, url->host.slen, url->host.ptr, url->port);
		}
		else
		{
			n_writen = pj_ansi_snprintf(info.NotifierUri, sizeof(info.NotifierUri), "sip:%.*s@%.*s",
				url->user.slen, url->user.ptr, url->host.slen, url->host.ptr);
		}
		if (n_writen <= 0)
		{
			info.NotifierUri[0] = '\0';
		}

		pj_ansi_snprintf(info.WG67_Version, sizeof(info.WG67_Version), "%.*s", subdlg->WG67_version->hvalue.slen, subdlg->WG67_version->hvalue.ptr);

		if (sub_user_data != NULL) CallSubscribeCallback(&sub_user_data->last_info, &info);
	}
}

void WG67Subs::OnWG67SrvTsxChanged(pjsip_evsub* sub, pjsip_transaction* tsx, pjsip_event* event)
{
	pjsip_evsub_state state = pjsip_evsub_get_state(sub);

	if (tsx->state == PJSIP_TSX_STATE_COMPLETED && (tsx->role == PJSIP_ROLE_UAS) && (pjsip_method_cmp(&tsx->method, &pjsip_subscribe_method) == 0)
		&& PJSIP_IS_STATUS_IN_CLASS(tsx->status_code, 200)
		&& (state == PJSIP_EVSUB_STATE_ACTIVE || state == PJSIP_EVSUB_STATE_PENDING || state == PJSIP_EVSUB_STATE_TERMINATED))
	{		
		SubscriptionServer::subs_user_data* sub_user_data = (SubscriptionServer::subs_user_data*) pjsip_evsub_get_user_data(sub);
		AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(sub_user_data->accid);
		if (accUserData != NULL && accUserData->subsServer != NULL)
		{
			if (accUserData->subsServer->_Manual_WG67_Notify == PJ_FALSE)
			{
				Send_WG67_current_notify(sub);
			}
			else //if (state == PJSIP_EVSUB_STATE_TERMINATED)
			{
				pjsip_evsub_set_state(sub, PJSIP_EVSUB_STATE_ACCEPTED);
			}
		}
	}
}

void WG67Subs::OnWG67RxRefresh(pjsip_evsub* sub, pjsip_rx_data* rdata, int* p_st_code,
	pj_str_t** p_st_text, pjsip_hdr* res_hdr, pjsip_msg_body** p_body)
{
	SubscriptionServer::subs_user_data* sub_user_data = (SubscriptionServer::subs_user_data*)pjsip_evsub_get_user_data(sub);
	AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(sub_user_data->accid);
	if (accUserData != NULL && accUserData->subsServer != NULL)
	{
		if (accUserData->subsServer->_Notify_enabled == PJ_FALSE)
		{
			pjsip_evsub_set_state(sub, PJSIP_EVSUB_STATE_TERMINATED);
			*p_st_code = PJSIP_SC_BAD_EVENT;
		}
	}
}

void WG67Subs::OnWG67SrvTimeout(pjsip_evsub* sub)
{		
	SubscriptionServer::subs_user_data* sub_user_data = (SubscriptionServer::subs_user_data*) pjsip_evsub_get_user_data(sub);
	AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(sub_user_data->accid);
	if (accUserData != NULL && accUserData->subsServer != NULL)
	{			
		if (accUserData->subsServer->_Manual_WG67_Notify == PJ_FALSE)
		{				
			SubscriptionServer::subs_user_data* sub_user_data = (SubscriptionServer::subs_user_data*)pjsip_evsub_get_user_data(sub);
			if (sub_user_data != NULL)
			{
				sub_user_data->subsState.expires = -1;
				sub_user_data->subsState.retry_after = -1;
				strcpy(sub_user_data->subsState.subscription_state, "terminated");
				strcpy(sub_user_data->subsState.reason, "timeout");
			}
			Send_WG67_current_notify(sub);
		}
		else
		{
			pjsip_wg67_terminate(sub, PJ_TRUE);
			//SubscriptionServer::Del_subMod(sub);
		}
	}
}

/**
 * SubscriptionRxRequest.	...
 * Gestona la recepcion de un request para abrir un nuevo dialogo de subscripcion al evento de WG67.
 * @param rdata. Incoming message buffer
 * @param dlg. Si no es NULL, entonces el request de la subscripcion ha sido recibido en un dialogo.
 * @return	true si se admite.
 */
pj_bool_t WG67Subs::SubscriptionRxRequest(pjsip_rx_data* rdata, pjsip_dialog* dlg)
{
	pj_str_t STR_EVENT = { "Event", 5 };
	pj_str_t STR_WG67KEY_IN = { "WG67KEY-IN", 10 };
	//Hemos recibido una subscripcion sin to-tag
	pjsip_event_hdr* event = (pjsip_event_hdr*)pjsip_msg_find_hdr_by_name(rdata->msg_info.msg, &STR_EVENT, NULL);
	pjsip_expires_hdr* expires = (pjsip_expires_hdr*)pjsip_msg_find_hdr(rdata->msg_info.msg, PJSIP_H_EXPIRES, NULL);
	pjsip_to_hdr* to = (pjsip_to_hdr*)pjsip_msg_find_hdr(rdata->msg_info.msg, PJSIP_H_TO, NULL);
	pjsip_from_hdr* from = (pjsip_from_hdr*)pjsip_msg_find_hdr(rdata->msg_info.msg, PJSIP_H_FROM, NULL);
	if (to == NULL || from == NULL) return PJ_FALSE;

	if (event == NULL)
	{
		return PJ_FALSE;
	}

	if (pj_stricmp(&event->event_type, &STR_WG67KEY_IN) != 0)
	{
		return PJ_FALSE;
	}

	//Buscamos el acc al que va dirigido
	pj_bool_t user_not_found = PJ_FALSE;
	pjsua_acc_id accid = pjsua_acc_find_for_incoming_by_uri_case_sensitive(to->uri);
	if (accid == PJSUA_INVALID_ID)
	{
		user_not_found = PJ_TRUE;		
	}
	else if (pjsua_acc_find_for_incoming_by_user_and_domain_or_transport_case_sensitive(to->uri) == PJSUA_INVALID_ID)
	{
		//Se ha encontrado un acc_id, pero el usuario no se corresponde con este agente
		//Contestamos Not Found mas abajo
		user_not_found = PJ_TRUE;		
	}

	if (user_not_found)
	{		
		pjsip_endpt_respond_stateless(pjsua_var.endpt, rdata, PJSIP_SC_NOT_FOUND, NULL, NULL, NULL);
		return PJ_TRUE;
	}

	pj_bool_t rdAccount = PJ_FALSE;				//Indica si acc_id es un account tipo radio GRS
	AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(accid);
	
	//Comprobamos si esta solicitud de subscripcion ya ha sido descartada
	if (SubscriptionServer::IsInDeletedList(accid, from->uri, &from->tag) == PJ_TRUE)
	{
		//Ya ha sido descartado. No hacemos nada
		return PJ_TRUE;
	}

	pjsip_dialog* subdlg = NULL;
	pjsip_evsub* _WG67subMod = NULL;
	pj_status_t st = PJ_SUCCESS;

	if (dlg != NULL)
	{
		subdlg = dlg;
	}
	else
	{
		//Se ha recibido una subscripcion al evento de WG67KEY-IN fuera de un dialogo
		pj_pool_t* tmppool = pjsua_pool_create(NULL, 64, 32);
		PJ_ASSERT_RETURN(tmppool != NULL, PJ_FALSE);
		pj_str_t contact;
		st = pjsua_acc_create_uas_contact(tmppool, &contact, accid, rdata);
		if (st != PJ_SUCCESS)
		{
			PJ_LOG(3, (__FILE__, "ERROR: WG67Subs::SubscriptionRxRequest: No se puede generar contact"));
			pj_pool_release(tmppool);
			return PJ_FALSE;
		}

		st = pjsip_dlg_create_uas(pjsip_ua_instance(), rdata, &contact, &subdlg);
		pj_pool_release(tmppool);

		if (st != PJ_SUCCESS)
		{
			PJ_LOG(3, (__FILE__, "ERROR: WG67Subs::SubscriptionRxRequest: No se puede crear el dialogo como UAS"));
			return PJ_FALSE;
		}		
	}

	pjsip_dlg_inc_lock(subdlg);

	char neg_wg67version[64];
	pjsip_endpt_Neg_ED137Version_from_rdata(pjsua_var.endpt, rdata, neg_wg67version, sizeof(neg_wg67version), NULL, NULL);
	if (pj_ansi_strlen(neg_wg67version) > 0) pjsip_dlg_set_WG67_version(subdlg, neg_wg67version, PJ_TRUE);

	pj_bool_t enviar_PJSIP_SC_BAD_EVENT = PJ_FALSE;

	if (accUserData == NULL && !SipAgent::_Radio_UA) enviar_PJSIP_SC_BAD_EVENT = PJ_TRUE;	//Si es un agente radio no acepta la subscripcion
	else if (accUserData != NULL)
	{
		if (!accUserData->rdAccount && !SipAgent::_Radio_UA)
		{
			enviar_PJSIP_SC_BAD_EVENT = PJ_TRUE;					//Si es un account radio no acepta la subscripcion
		}
		else if (accUserData->subsServer != NULL)
		{
			if (accUserData->subsServer->_Notify_enabled == PJ_FALSE)
			{
				enviar_PJSIP_SC_BAD_EVENT = PJ_TRUE;				//Si los notify no son admitidos no acepta la subscripcion
			}
		}
	}
		
	if (enviar_PJSIP_SC_BAD_EVENT)
	{
		//Los notify no estan permitidos Respondemos aqui 
		/* Create response: */
		pjsip_tx_data* tdata;
		pjsip_transaction* tsx;
		st = pjsip_dlg_create_response(subdlg, rdata, PJSIP_SC_BAD_EVENT, NULL, &tdata);
		if (st == PJ_SUCCESS)
		{
			/* Send the response: */
			tsx = pjsip_rdata_get_tsx(rdata);
			st = pjsip_dlg_send_response(subdlg, tsx, tdata);
		}
		pjsip_dlg_dec_lock(subdlg);
		if (st != PJ_SUCCESS) return PJ_FALSE;
		return PJ_TRUE;
	}

	pjsip_evsub* evsubtmp = SubscriptionServer::Get_subMod(accid, subdlg->remote.contact->uri, &STR_WG67KEY_IN);
	if (evsubtmp != NULL)
	{
		if (dlg != NULL && (pjsip_evsub_get_dlg(evsubtmp) == dlg))
		{
			//Es un SUBSCRIBE recibido dentro de un dialogo ya creado. Por tanto no hacemos nada
			pjsip_dlg_dec_lock(subdlg);
			return PJ_TRUE;
		}

		//El usuario ya esta subscrito e intenta crear una nueva subscripcion en otro dialogo
		SubscriptionServer::Del_subMod(evsubtmp);
		pjsip_evsub_terminate(evsubtmp, PJ_FALSE);		//Eliminamos la subscripcion antigua y nos quedamos con la nueva																				
		SubscriptionServer::Add_DeletedsubModlist(accid, subdlg->remote.info->uri, &subdlg->remote.info->tag);
		//Se agrega a la lista de borrados
	}

	st = pjsip_wg67_create_uas(subdlg, &WG67Subs::_WG67SrvCb, rdata, &_WG67subMod);
	pjsip_dlg_dec_lock(subdlg);

	if (st != PJ_SUCCESS)
	{
		PJ_LOG(3, (__FILE__, "ERROR: WG67Subs::SubscriptionRxRequest: No se puede crear la subscripcion como UAS"));
		return PJ_FALSE;
	}

	pj_pool_t* pool = pjsip_evsub_get_pool(_WG67subMod);
	SubscriptionServer::subs_user_data* sub_user_data = (SubscriptionServer::subs_user_data*) pj_pool_alloc(pool, sizeof(SubscriptionServer::subs_user_data));
	PJ_ASSERT_RETURN(sub_user_data != NULL, PJ_FALSE);
	sub_user_data->accid = accid;
	memset(&sub_user_data->last_info, 0, sizeof(CORESIP_WG67_Subscription_Info));
	memset(&sub_user_data->subsState, 0, sizeof(CORESIP_WG67Notify_SubscriptionState_Config));
	memset(&sub_user_data->wG67Notify_Body_Config, 0, sizeof(CORESIP_WG67Notify_Body_Config));
	sub_user_data->in_WG67SubscriptionReceivedCb = PJ_FALSE;

	pjsip_evsub_set_user_data(_WG67subMod, (void*)sub_user_data);

	int minimum_expires_suported = SubscriptionServer::ABSOLUTE_MINIMUM_EXPIRES_AS_UAS;
		
	if (accUserData != NULL && accUserData->subsServer != NULL)
	{
		minimum_expires_suported = accUserData->subsServer->_Minimum_expires_As_UAS;
		pjsip_evsub_set_maximum_expires_as_uas(_WG67subMod, accUserData->subsServer->_Maximum_expires_As_UAS);
		pjsip_evsub_set_minimum_expires_as_uas(_WG67subMod, accUserData->subsServer->_Minimum_expires_As_UAS);
	}

	if (expires && expires->ivalue < 0)
	{
		pjsip_evsub_set_state(_WG67subMod, PJSIP_EVSUB_STATE_TERMINATED);

		pjsip_hdr hdr_list;
		pj_str_t reason_hname = { "Reason", 6 };
		pj_str_t reason_hvalue = { "WG-67; cause=2007 ;text=\"Parameter error\"", 41 };
		pjsip_generic_string_hdr* reason_hdr = pjsip_generic_string_hdr_create(subdlg->pool, &reason_hname, &reason_hvalue);
		pj_list_init(&hdr_list);
		pj_list_push_back(&hdr_list, reason_hdr);

		st = pjsip_evsub_reject(_WG67subMod, rdata, PJSIP_SC_DECLINE, &hdr_list);
	}
	else if (expires && expires->ivalue == 0)
	{
		pjsip_evsub_set_state(_WG67subMod, PJSIP_EVSUB_STATE_ACCEPTED);
		st = pjsip_wg67_accept(_WG67subMod, rdata, PJSIP_SC_OK, NULL);
		if (st == PJ_SUCCESS)
		{
			// Con subscribe expires a cero, el estado de la subscripción es terminated y se envia notify
				
			pjsip_evsub_set_state(_WG67subMod, PJSIP_EVSUB_STATE_TERMINATED);
			Send_WG67_current_notify(_WG67subMod);
		}
	}
	else if (expires && expires->ivalue < minimum_expires_suported)
	{
		pjsip_evsub_set_state(_WG67subMod, PJSIP_EVSUB_STATE_TERMINATED);

		pjsip_hdr hdr_list;
		pjsip_min_expires_hdr* min_expires_hdr = pjsip_min_expires_hdr_create(subdlg->pool, minimum_expires_suported);
		pj_list_init(&hdr_list);
		pj_list_push_back(&hdr_list, min_expires_hdr);

		st = pjsip_evsub_reject(_WG67subMod, rdata, PJSIP_SC_INTERVAL_TOO_BRIEF, &hdr_list);
	}
	else
	{
		pjsip_evsub_set_state(_WG67subMod, PJSIP_EVSUB_STATE_ACCEPTED);
		st = pjsip_wg67_accept(_WG67subMod, rdata, PJSIP_SC_OK, NULL);
		if (st == PJ_SUCCESS)
		{
			/*
				* He visto que cuando se crea el servidor a la subscripcion  no se arranca el timeout
				* que vence cuando expire time se cumple. Sin embargo cuando se reciben los subscribes de refresco sí.
				* Por tanto creo esta funcion para arrancar el timer.
				*/
			set_timer_uas_timeout(_WG67subMod);

			PJ_LOG(5, ("SipAgent.cpp", "SipAgent::OnRxRequest dialog sub %p creado con exito", _WG67subMod));

			/*Agregamos a la lista de subscripciones del account correspondiente*/
			SubscriptionServer::Add_subMod(_WG67subMod);

			pjsip_evsub_set_state(_WG67subMod, PJSIP_EVSUB_STATE_ACTIVE);

			if (SipAgent::Cb.WG67SubscriptionReceivedCb)
			{
				char SubscriberUri[CORESIP_MAX_URI_LENGTH + 1];				//Uri del subscriptor
				pjsip_contact_hdr* remote_contact_hdr = subdlg->remote.contact;
				pjsip_uri* remote_contact_uri = remote_contact_hdr->uri;
				pjsip_sip_uri* url = (pjsip_sip_uri*)pjsip_uri_get_uri(remote_contact_uri);
				int n_writen;
				if (url->port > 0)
				{
					n_writen = pj_ansi_snprintf(SubscriberUri, sizeof(SubscriberUri), "sip:%.*s@%.*s:%d",
						url->user.slen, url->user.ptr, url->host.slen, url->host.ptr, url->port);
				}
				else
				{
					n_writen = pj_ansi_snprintf(SubscriberUri, sizeof(SubscriberUri), "sip:%.*s@%.*s",
						url->user.slen, url->user.ptr, url->host.slen, url->host.ptr);
				}
				if (n_writen <= 0)
				{
					SubscriberUri[0] = '\0';
				}
				sub_user_data->in_WG67SubscriptionReceivedCb = PJ_TRUE;
				SipAgent::Cb.WG67SubscriptionReceivedCb(accid | CORESIP_ACC_ID, SubscriberUri);
				sub_user_data->in_WG67SubscriptionReceivedCb = PJ_FALSE;
			}

			if (dlg == NULL && accUserData != NULL && accUserData->subsServer != NULL && accUserData->subsServer->_Manual_WG67_Notify == PJ_FALSE)
			{
				Send_WG67_current_notify(_WG67subMod);
			}			
		}
	}

	if (st != PJ_SUCCESS)
	{
		return PJ_FALSE;
	}

	return PJ_TRUE;

}

void WG67Subs::Send_WG67_current_notify(pjsip_evsub* sub)
{
	pj_bool_t include_sessions = PJ_FALSE;
	CORESIP_WG67Notify_SubscriptionState_Config *subsState = NULL;
	pjsip_evsub_state current_state = pjsip_evsub_get_state(sub);

	char WG67Version = 'B';
	pjsip_dialog *subdlg = pjsip_evsub_get_dlg(sub);
	if (subdlg != NULL)
	{
		if (pj_stricmp(&subdlg->WG67_version->hvalue, &pj_str("radio.02")) == 0)
		{
			WG67Version = 'C';
		}
		else if (pj_stricmp(&subdlg->WG67_version->hvalue, &pj_str("radio.01")) == 0)
		{
			WG67Version = 'B';
		}
	}

	SubscriptionServer::subs_user_data* sub_user_data = (SubscriptionServer::subs_user_data*)pjsip_evsub_get_user_data(sub);
	if (sub_user_data != NULL)
	{
		subsState = &sub_user_data->subsState;
	}
	
	if (subsState != NULL)
	{
		if (stricmp(subsState->subscription_state, "active") == 0)
		{
			include_sessions = PJ_TRUE;
		}
		else if (stricmp(subsState->subscription_state, "terminated") == 0 && 
			(current_state == PJSIP_EVSUB_STATE_ACTIVE))
		{
			include_sessions = PJ_TRUE;
		}
		else if (strlen(subsState->subscription_state) == 0 && 
			(current_state == PJSIP_EVSUB_STATE_ACTIVE || current_state == PJSIP_EVSUB_STATE_TERMINATED))
		{
			include_sessions = PJ_TRUE;
		}
		else
		{
			include_sessions = PJ_FALSE;
		}
	}
	else if (current_state == PJSIP_EVSUB_STATE_ACTIVE)
	{
		include_sessions = PJ_TRUE;
	}
	else if (current_state == PJSIP_EVSUB_STATE_TERMINATED)
	{
		if (stricmp(sub_user_data->last_info.SubscriptionState, "pending") == 0)
		{
			//El anterior estado ha sido pending
			include_sessions = PJ_FALSE;
		}
		else
		{
			include_sessions = PJ_TRUE;
		}
	}
	else
	{
		include_sessions = PJ_FALSE;
	}	
		
	int exclude_real_sessions = 0;

	if (sub_user_data != NULL)
	{
		exclude_real_sessions = sub_user_data->wG67Notify_Body_Config.exclude_real_sessions;
	}

	char charBody[CORESIP_MAX_BODY_LEN];
	charBody[0] = '\0';
	
	if (include_sessions == PJ_TRUE)
	{
		if (WG67Version == 'C')
		{
			//Se pone el fid
			char line[CORESIP_MAX_BODY_LEN];
			AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(sub_user_data->accid);
			if (accUserData != NULL && strlen(accUserData->RdFr) > 0)
			{
				pj_ansi_snprintf(line, sizeof(line), "fid: %s\n", accUserData->RdFr);
			}
			else
			{
				pj_ansi_snprintf(line, sizeof(line), "fid: 000.000\n");
			}
			if ((sizeof(charBody) - strlen(charBody)) > strlen(line))
			{
				pj_ansi_strcat(charBody, line);
			}
		}

		/*En el body, primero se ponen las sesiones reales*/
		pjsua_call_id call_ids[PJSUA_MAX_CALLS];
		unsigned call_cnt = PJ_ARRAY_SIZE(call_ids);

		pj_status_t st = pjsua_enum_calls(call_ids, &call_cnt);
		if (st != PJ_SUCCESS)
		{
			PJ_LOG(3, (__FILE__, "ERROR: WG67Subs::OnWG67SrvTsxChanged: No se pueden obtener las llamadas del account"));
			return;
		}

		for (unsigned i = 0; i < call_cnt && sub_user_data != NULL && exclude_real_sessions == 0; i++)
		{
			pjsua_call_info info;
			st = pjsua_call_get_info(call_ids[i], &info);
			if (st == PJ_SUCCESS)
			{
				if (info.acc_id == sub_user_data->accid && info.state == PJSIP_INV_STATE_CONFIRMED)
				{
					SipCall* sipcall = (SipCall*)pjsua_call_get_user_data(call_ids[i]);
					if (sipcall != NULL)
					{
						PJ_LOG(5, (__FILE__, "INFO: WG67Subs::OnWG67SrvTsxChanged: llamada encontrada"));
						char line[CORESIP_MAX_BODY_LEN];
						line[0] = '\0';
						char coupling[] = "Coupling";
						char radiorxonly[] = "Radio-Rxonly";
						char radiotxrx[] = "Radio-TxRx";
						char radioidle[] = "Radio-Idle";
						char* rdcalltype;

						if (sipcall->_Info.CallFlags & CORESIP_CALL_RD_COUPLING) rdcalltype = coupling;
						else if (sipcall->_Info.CallFlags & CORESIP_CALL_RD_RADIO_RXONLY) rdcalltype = radiorxonly;
						else if (sipcall->_Info.CallFlags & CORESIP_CALL_RD_IDLE) rdcalltype = radioidle;
						else rdcalltype = radiotxrx;

						pjsip_uri* sip_uri = pjsip_parse_uri(pjsip_evsub_get_pool(sub), info.remote_contact.ptr, info.remote_contact.slen, 0);
						if (sip_uri != NULL)
						{
							pjsip_sip_uri* uri = (pjsip_sip_uri*)pjsip_uri_get_uri(sip_uri);
							if (uri != NULL)
							{
								//Si la sesion no asigna ptt-id, porque no lo requiera. Entonces en la ED137C explicitamente dice que no se pone 
								//en la linea de la sesion, pero en la ED137B no dice nada. En una radio real se ha visto en esta caso que pone un ptt-id a cero.
								if (sipcall->assigned_pttid == 0 && WG67Version == 'C')
								{									
									if (uri->port != 0)
									{
										pj_ansi_snprintf(line, sizeof(line), "sip:%.*s@%.*s:%d,%s\n",
											uri->user.slen, uri->user.ptr, uri->host.slen, uri->host.ptr, uri->port, rdcalltype);
									}
									else
									{
										pj_ansi_snprintf(line, sizeof(line), "sip:%.*s@%.*s,%s\n",
											uri->user.slen, uri->user.ptr, uri->host.slen, uri->host.ptr, rdcalltype);
									}
								}
								else
								{
									if (uri->port != 0)
									{
										pj_ansi_snprintf(line, sizeof(line), "%u,sip:%.*s@%.*s:%d,%s\n", sipcall->assigned_pttid,
											uri->user.slen, uri->user.ptr, uri->host.slen, uri->host.ptr, uri->port, rdcalltype);
									}
									else
									{
										pj_ansi_snprintf(line, sizeof(line), "%u,sip:%.*s@%.*s,%s\n", sipcall->assigned_pttid,
											uri->user.slen, uri->user.ptr, uri->host.slen, uri->host.ptr, rdcalltype);
									}
								}
							}
						}

						if ((sizeof(charBody) - strlen(charBody)) > strlen(line))
						{
							pj_ansi_strcat(charBody, line);
						}
					}
				}
			}
		}

		/*Agregamos las sesiones forzadas*/

		if (sub_user_data != NULL)
		{
			CORESIP_WG67Notify_Body_Config& WG67Notify_Body = sub_user_data->wG67Notify_Body_Config;

			for (int i = 0; i < WG67Notify_Body.num_sessions; i++)
			{
				char line[CORESIP_MAX_BODY_LEN];
				line[0] = '\0';

				pj_ansi_snprintf(line, sizeof(line), "%u,%s,%s\n", WG67Notify_Body.SessionsInfo[i].ptt_id,
					WG67Notify_Body.SessionsInfo[i].sip_from_uri, WG67Notify_Body.SessionsInfo[i].call_type);

				if ((sizeof(charBody) - strlen(charBody)) > strlen(line))
				{
					pj_ansi_strcat(charBody, line);
				}
			}
		}
	}

	if (strlen(subsState->subscription_state) == 0)
	{
		pjsip_wg67_current_notify(sub, charBody, &pjsua_var.ua_cfg.user_agent);

		/* Para test del parseo del body de la SUBSCRIPCION AL EVENTO WG67 
		charBody[0] = '\0';
		strcpy(charBody, "fid:123.123\nsip:ww@1.1.1.1,\n1,sip@192.168.1.205:4567\n");
		pjsip_wg67_current_notify(sub, charBody, &pjsua_var.ua_cfg.user_agent);
		pj_thread_sleep(1000);

		charBody[0] = '\0';
		strcpy(charBody, "fid:123.123\nsip:ww@1.1.1.1\n1,sip:user@192.168.1.205:4567\n2,user2@192.168.1.205:4567,coupling\n");
		pjsip_wg67_current_notify(sub, charBody, &pjsua_var.ua_cfg.user_agent);

		pj_thread_sleep(1000);

		charBody[0] = '\0';
		strcpy(charBody, "fid:123.123\nsip:ww@1.1.1.1\n1,sip:user@192.168.1.205:4567\n2,sip:user2@192.168.1.205:4567,coupling\n");
		pjsip_wg67_current_notify(sub, charBody, &pjsua_var.ua_cfg.user_agent);

		pj_thread_sleep(1000);		
		
		charBody[0] = '\0';
		strcpy(charBody, "fid:123.123\nsip:ww@1.1.1.1\n1,sip@192.168.1.205:4567\n");
		pjsip_wg67_current_notify(sub, charBody, &pjsua_var.ua_cfg.user_agent);

		pj_thread_sleep(1000);

		charBody[0] = '\0';
		strcpy(charBody, "fid:123.123\nsip:ww@1.1.1.1\nfg,sip@192.168.1.205:4567\n");
		pjsip_wg67_current_notify(sub, charBody, &pjsua_var.ua_cfg.user_agent);

		pj_thread_sleep(1000);

		charBody[0] = '\0';
		strcpy(charBody, "fid:123.123\nsip:ww@1.1.1.1,\n1,sip@192.168.1.205:4567\nasdfasdfasfdasf\n");
		pjsip_wg67_current_notify(sub, charBody, &pjsua_var.ua_cfg.user_agent);

		pj_thread_sleep(1000);
		charBody[0] = '\0'; 
		strcpy(charBody, "adfadf\nadfasdfasfd\nadfasdfasdfas\nasdfasdfasfdasf\n");
		pjsip_wg67_current_notify(sub, charBody, &pjsua_var.ua_cfg.user_agent);

		pj_thread_sleep(1000);
		charBody[0] = '\0';
		strcpy(charBody, "adfadf\nadfasdfasfd\nadfasdfasdfas\nasdfasdfasfdasf");
		pjsip_wg67_current_notify(sub, charBody, &pjsua_var.ua_cfg.user_agent);

		pj_thread_sleep(1000);
		charBody[0] = '\0';
		strcpy(charBody, "fid 123.123\nadfasdfasfd\n1,sip@192.168.1.205:4567\nasdfasdfasfdasf\n");
		pjsip_wg67_current_notify(sub, charBody, &pjsua_var.ua_cfg.user_agent);

		pj_thread_sleep(1000);
		charBody[0] = '\0';
		strcpy(charBody, "fid:123.123\ns,sip:ww@1.1.1.1\n1,sip@192.168.1.205:4567\nasdfasdfasfdasf\n");
		pjsip_wg67_current_notify(sub, charBody, &pjsua_var.ua_cfg.user_agent);

		*/
	}
	else
	{
		pj_str_t pj_subs_state = pj_str(subsState->subscription_state);
		pj_str_t pj_reason = pj_str(subsState->reason);

		pjsip_evsub_state state = PJSIP_EVSUB_STATE_UNKNOWN;

		if (pj_stricmp2(&pj_subs_state, "pending") == 0)
		{
			state = PJSIP_EVSUB_STATE_PENDING;
		}
		else if (pj_stricmp2(&pj_subs_state, "active") == 0)
		{
			state = PJSIP_EVSUB_STATE_ACTIVE;
		}
		else if (pj_stricmp2(&pj_subs_state, "terminated") == 0)
		{
			state = PJSIP_EVSUB_STATE_TERMINATED;
		}
				
		pjsip_wg67_notify(sub, PJ_TRUE, subsState->expires, state, &pj_subs_state, &pj_reason, subsState->retry_after, charBody, &pjsua_var.ua_cfg.user_agent);

		subsState->subscription_state[0] = '\0';
	}
}


/**
 *	Set_WG67_Notifier_Parameters. Configura algunos parametros del WG67 notifier
 *  @param	notify_enabled. Si es 1 entonces los Notify son aceptados, 0 son rechazados, con -1 este parametro se ignora.
 *	@param	manual_notify. Si es 1 entonces las notificaciones son manuales, 0 automatico, -1 no cambia. Si no se llama a esta función, por defecto los notify se envía automáticamente
 *  @param  minimum_expires. Tiempo minimo de expires soportado. Si el valor es -1, entonces no tiene efecto. Si el subscriptor envia un valor menor entonces se rechaza
 			y hay ue enviar cabecera con el minimo expires soportado. El valor mínimo de este parámetro es de 30.
 *  @param  maximum_expires. Tiempo maximo de expires soportado. Si el valor es -1, entonces no tiene efecto. Si el subscriptor envia un valor mayor al valor de este 
			parametro entonces en el 200 OK envia este valor.
 *	REturn- CORESIP_ERROR si hay error.
 */
int WG67Subs::Set_WG67_Notifier_Parameters(int notify_enabled, int manual, int minimum_expires, int maximum_expires)
{
	pjsua_acc_info info[PJSUA_MAX_ACC];
	unsigned c;
	int ret = CORESIP_OK;

	c = PJ_ARRAY_SIZE(info);
	pj_status_t status = pjsua_acc_enum_info(info, &c);
	if (status != PJ_SUCCESS)
	{
		PJ_CHECK_STATUS(status, ("ERROR: WG67Subs::Set_WG67_Notify_As_Manual"));
		return CORESIP_ERROR;
	}

	if (minimum_expires >= 0 && minimum_expires < SubscriptionServer::ABSOLUTE_MINIMUM_EXPIRES_AS_UAS)
	{
		ret = CORESIP_ERROR;
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("Set_WG67_Notifier_Parameters:", "minimum_expires debe ser -1 o mayor o igual que %d", SubscriptionServer::ABSOLUTE_MINIMUM_EXPIRES_AS_UAS);
		return ret;
	}

	if (maximum_expires >= 0 && maximum_expires > SubscriptionServer::ABSOLUTE_MAXIMUM_EXPIRES_AS_UAS)
	{
		ret = CORESIP_ERROR;
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("Set_WG67_Notifier_Parameters:", "maximum_expires debe ser -1 o menor o igual que %d", SubscriptionServer::ABSOLUTE_MAXIMUM_EXPIRES_AS_UAS);
		return ret;
	}

	if (maximum_expires >= 0 && minimum_expires >= 0 && maximum_expires <= minimum_expires)
	{
		ret = CORESIP_ERROR;
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("Set_WG67_Notifier_Parameters:", "maximum_expires %d debe ser mayor que el valor minimum_expires %d", maximum_expires, minimum_expires);
		return ret;
	}
	else
	{
	}

	for (unsigned i = 0; i < c; i++)
	{
		AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(info[i].id);
		if (accUserData != NULL && accUserData->subsServer != NULL)
		{
			if (notify_enabled == 0)
			{
				pj_str_t STR_WG67KEY_IN = { "WG67KEY-IN", 10 };
				SubscriptionServer::TerminateSubscriptions(&STR_WG67KEY_IN);
				accUserData->subsServer->_Notify_enabled = PJ_FALSE;
			}
			else if (notify_enabled > 0)
			{
				accUserData->subsServer->_Notify_enabled = PJ_TRUE;
			}
			else {}
			
			if (manual > 0)
			{
				accUserData->subsServer->_Manual_WG67_Notify = PJ_TRUE;
			}
			else if (manual == 0)
			{
				accUserData->subsServer->_Manual_WG67_Notify = PJ_FALSE;
			}
			else
			{
			}

			if (maximum_expires >= 0 && minimum_expires >= 0)
			{
				accUserData->subsServer->_Minimum_expires_As_UAS = minimum_expires;
				accUserData->subsServer->_Maximum_expires_As_UAS = maximum_expires;
			}			
			else
			{
				if (maximum_expires >= 0)
				{
					if (maximum_expires <= accUserData->subsServer->_Minimum_expires_As_UAS)
					{
						ret = CORESIP_ERROR;
						throw PJLibException(__FILE__, PJ_EINVAL).Msg("Set_WG67_Notifier_Parameters:", "maximum_expires debe ser mayor que el minimo configurado %d", accUserData->subsServer->_Minimum_expires_As_UAS);
						return ret;
					}
					else
					{
						accUserData->subsServer->_Maximum_expires_As_UAS = maximum_expires;
					}
				}
				if (minimum_expires >= 0)
				{
					if (minimum_expires >= accUserData->subsServer->_Maximum_expires_As_UAS)
					{
						ret = CORESIP_ERROR;
						throw PJLibException(__FILE__, PJ_EINVAL).Msg("Set_WG67_Notifier_Parameters:", "minimum_expires debe ser menor que el maximo configurado %d", accUserData->subsServer->_Maximum_expires_As_UAS);
						return ret;
					}
					else
					{
						accUserData->subsServer->_Minimum_expires_As_UAS = minimum_expires;
					}
				}
			}
		}
	}

	return ret;
}

/**
 * Set_WG67_notify_status: Establece el estado de la suscripcion y las sesiones ficticias, ademas de enviar el Notify correspondiente.
 * @param   acc_id. Identificador del account. Si es PJSUA_INVALID_ID, se utiliza la default
 * @param	subscriberUri.	Uri del suscriptor recibido en la callback WG67SubscriptionReceivedCb
 * @param	subsState. Establece el estado de la subscripcion. Puede valer NULL si no queremos modificar el estado.
 *						Si el campo subscription_state de la estructura tiene longitud cero, entonces tampoco se modifica el estado.
 * @param	wG67Notify_Body. Configura el body (las sesiones activas) que se envia en los NOTIFY. Puede ser NULL si no queremos modificar la lista de sesiones
 * @return	CORESIP_ERROR si no hay error
 */
int WG67Subs::Set_WG67_notify_status(pjsua_acc_id acc_id, char* subscriberUri, CORESIP_WG67Notify_SubscriptionState_Config* subsState,
	CORESIP_WG67Notify_Body_Config* wG67Notify_Body)
{
	int ret = CORESIP_OK;
	pj_str_t STR_WG67KEY_IN = { "WG67KEY-IN", 10 };
	pjsua_acc_id pj_acc_id;

	if (acc_id == -1)
	{
		pj_acc_id = pjsua_acc_get_default();
	}
	else
	{
		pj_acc_id = acc_id;
	}

	if (!pjsua_acc_is_valid(pj_acc_id))
	{
		PJ_CHECK_STATUS(PJ_EINVAL, ("Set_WG67_notify_status: Invalid Account"));
		return CORESIP_ERROR;
	}

	/*Comprobamos que el estado que nos pasan es correcto*/
	if (subsState != NULL && strlen(subsState->subscription_state) > 0)
	{
		pj_bool_t terminated = PJ_FALSE;
		if (stricmp(subsState->subscription_state, "pending") == 0)
		{
		}
		else if (stricmp(subsState->subscription_state, "active") == 0)
		{
		}
		else if (stricmp(subsState->subscription_state, "terminated") == 0)
		{
			terminated = PJ_TRUE;
		}
		else
		{
			PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: Set_WG67_notify_status invalid subscription_state"));
			return CORESIP_ERROR;
		}

		if (strlen(subsState->reason) > 0)
		{
			if (terminated == PJ_FALSE)
			{
				PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: Set_WG67_notify_status No reason is allowed when state is not terminated"));
				return CORESIP_ERROR;
			}
			else
			{
				if (stricmp(subsState->reason, "deactivated") == 0)
				{
				}
				else if (stricmp(subsState->reason, "probation") == 0)
				{
				}
				else if (stricmp(subsState->reason, "rejected") == 0)
				{
				}
				else if (stricmp(subsState->reason, "timeout") == 0)
				{
				}
				else if (stricmp(subsState->reason, "giveup") == 0)
				{
				}
				else if (stricmp(subsState->reason, "noresource") == 0)
				{
				}
				else
				{
					PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: Set_WG67_notify_status invalid reason"));
					return CORESIP_ERROR;
				}
			}
		}

		if (terminated != PJ_TRUE)
		{
			//En estado que no es terminated Ignoraremos retry_after
			subsState->retry_after = -1;
		}
		else
		{
			//En estado terminated ignoraremos expires
			subsState->expires = -1;
		}			
	}

	if (subscriberUri == NULL)
	{
		//Se envia a todas la subscriciones
		SubscriptionServer::SendWG67NotifyFromAcc(pj_acc_id, PJ_FALSE, subsState, wG67Notify_Body, PJ_FALSE, 
			AccountUserData::ENVIAR_WG67_NOTIFY_SI_TODAS_LAS_SESIONES_SON_FICTICIAS);
		return CORESIP_OK;
	}

	//Se envia solo a la suscripcion especifica

	pj_pool_t* tmppool = pjsua_pool_create("Set_WG67_notify_status", 64, 32);
	if (tmppool == NULL)
	{
		PJ_CHECK_STATUS(PJ_ENOMEM, ("Set_WG67_notify_status: not enough memory"));
		return CORESIP_ERROR;
	}

	/*Se crea un string duplicado para el parse, ya que se ha visto que
		pjsip_parse_uri puede modificar el parametro de entrada*/
	pj_str_t uri_aux;
	uri_aux.ptr = subscriberUri;
	uri_aux.slen = (pj_ssize_t)strlen(subscriberUri);
	pj_str_t uri_dup;
	pj_strdup_with_null(tmppool, &uri_dup, &uri_aux);

	pjsip_uri* uri = pjsip_parse_uri(tmppool, uri_dup.ptr, uri_dup.slen, 0);
	if (uri == NULL)
	{
		pj_pool_release(tmppool);
		PJ_CHECK_STATUS(PJ_ENOMEM, ("Set_WG67_notify_status: not enough memory"));
		return CORESIP_ERROR;
	}
	
	pjsip_evsub* wg67_subs = SubscriptionServer::Get_subMod(pj_acc_id, uri, &STR_WG67KEY_IN);
	pj_pool_release(tmppool);
	if (wg67_subs == NULL)
	{
		PJ_CHECK_STATUS(PJ_EINVAL, ("Set_WG67_notify_status: subscription can not be found"));
		return CORESIP_ERROR;
	}

	SubscriptionServer::subs_user_data* sub_user_data = (SubscriptionServer::subs_user_data*)pjsip_evsub_get_user_data(wg67_subs);
	if (sub_user_data == NULL)
	{
		PJ_CHECK_STATUS(PJ_EINVAL, ("Set_WG67_notify_status: sub_user_data can not be found in subscription"));
		return CORESIP_ERROR;
	}

	if (subsState != NULL && strlen(subsState->subscription_state) > 0)
	{
		memcpy(&sub_user_data->subsState, subsState, sizeof(CORESIP_WG67Notify_SubscriptionState_Config));
	}

	if (wG67Notify_Body != NULL)
	{
		memcpy(&sub_user_data->wG67Notify_Body_Config, wG67Notify_Body, sizeof(CORESIP_WG67Notify_Body_Config));
	}

	if (sub_user_data->in_WG67SubscriptionReceivedCb == PJ_FALSE)
	{
		WG67Subs::Send_WG67_current_notify(wg67_subs);
	}

	return ret;
}

void WG67Subs::CallSubscribeCallback(CORESIP_WG67_Subscription_Info* last_info, CORESIP_WG67_Subscription_Info* current_info)
{
	pj_bool_t info_changed = PJ_FALSE;
	if (strcmp(current_info->SubscriptionState, last_info->SubscriptionState) != 0)
	{
		info_changed = PJ_TRUE;
	}
	if (info_changed == PJ_FALSE)
	{
		if ((strlen(current_info->Reason) > 0) && (strcmp(current_info->Reason, last_info->Reason) != 0))
		{
			info_changed = PJ_TRUE;
		}
	}
	if (info_changed == PJ_FALSE)
	{
		if (current_info->NotifyReceived)
		{
			info_changed = PJ_TRUE;
		}
	}

	if (info_changed == PJ_TRUE)
	{
		if (SipAgent::Cb.WG67SubscriptionStateCb)
		{
			SipAgent::Cb.WG67SubscriptionStateCb(current_info);
		}

		memcpy(last_info, current_info, sizeof(CORESIP_WG67_Subscription_Info));
	}
}










