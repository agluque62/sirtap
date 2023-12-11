/*
En este modulo se implementa la clase que gestina una subscripcion al evento de conferencia
*/
#include "ConfSubs.h"
#include "SipCall.h"
#include "SipAgent.h"
#include "AccountUserData.h"

#undef THIS_FILE
#define THIS_FILE		"ConfSubs.cpp"

pjsip_evsub_user ConfSubs::conf_callback = 
{
	&ConfSubs::conf_on_state,  
	&ConfSubs::conf_on_tsx_state,
	NULL,
	&ConfSubs::conf_on_rx_notify,
	&ConfSubs::conf_on_client_refresh,
	NULL
};

/**
 * ConfSubs.	...
 * Constructor
 * @param	acc_id				Account del agente que utilizaremos. Si vale -1 entonces se usa la default.
 * @param	call_id				Si se quiere establecer la suscripcion con el dialogo de una sesion, es el call id de la sesion. Si vale -1 entonces se establece fuera de dialogo.
 * @param	dst_uri				Uri al que enviamos la subscripcion al evento. Si call_id es distinto de -1, se ignora
 * @param	expires. Valor del expires. Si vale -1 entonces toma el valor por defecto
 * @param	manual				Si es true entonces el subscribe se envia de forma manual y no se refresca automaticamente
 * @param	by_proxy. Si true entonces el subscribe se envia a traves del proxy
 * @return	-1 si hay error.
 */
ConfSubs::ConfSubs(pjsua_acc_id acc_id, pjsua_call_id call_id, char *dst_uri, int expires, pj_bool_t manual, pj_bool_t by_proxy)
{
	_confsub_cb = NULL;
	_Pool = NULL;
	_Dlg = NULL;
	_Module = NULL;
	_SubsManager = NULL;
	_Expires = expires;
	_Manual = manual;
	_ByProxy = by_proxy;
	_Call_id = call_id;
	_Resubscribe = PJ_FALSE;

	char ED137Radioversion_, ED137Phoneversion_;
	pjsip_endpt_Get_Ed137_version(pjsua_var.endpt, &ED137Radioversion_, &ED137Phoneversion_);
	ED137Version = ED137Phoneversion_;

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
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("ConfSubs: Account no valida");
		return;
	}

	if ((strlen(dst_uri)+1) > sizeof(_Dst_uri))
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("ConfSubs: dst_uri demasiado largo");
		return;
	}

	strcpy(_Dst_uri, dst_uri);
	_Dst_uri_pj = pj_str(_Dst_uri);

	_Pool = pjsua_pool_create(NULL, 64, 32);
	if (_Pool == NULL)
	{
		throw PJLibException(__FILE__, PJ_ENOMEM).Msg("ConfSubs: No hay suficiente memoria");
		return;
	}

	if (pj_sem_create(_Pool, NULL, 0, 1, &status_sem) != PJ_SUCCESS)
	{
		throw PJLibException(__FILE__, PJ_ENOMEM).Msg("ConfSubs: No se puede crear status_sem");
		return;
	}
}


ConfSubs::~ConfSubs()
{
	_confsub_cb = NULL;
	_SubsManager = NULL;
	if (_Pool != NULL)
	{
		pj_pool_release(_Pool);
	}
}

/**
 * Init.	...
 * inicializa la subscripcion. Si no hay existo lo reintenta.
 * @param confsub_cb. Callback para pasar notificaciones al ClientSubscriptionManager
 * @param subsmanager. Puntero del ClientSubscriptionManager.
 * @return	-1 si hay error. 
 */
int ConfSubs::Init(ConfSubs_callback confsub_cb, void *subsmanager)
{
	_confsub_cb = NULL;
	_SubsManager = subsmanager;
	int tries = 5;
	while (tries > 0)
	{
		if (InicializarSubscripcion() == 0)
		{
			//Se ha activado la subscripcion sin errores
			//Activamos la callback
			_confsub_cb = confsub_cb;
			return 0;
		}
		tries--;
	}
	PJ_LOG(3,(__FILE__, "ERROR: ConfSubs::Init: No se ha podido realizar la subscripcion a conf despues de %d intentos acc_id %d dst %s", tries, _Acc_id, _Dst_uri)); 
	return -1;
}

/**
 * InicializarSubscripcion.	...
 * Inicializa la subscripcion
 * @return	-1 si hay error. 
 */
int ConfSubs::InicializarSubscripcion()
{
	pj_status_t st = PJ_SUCCESS;

	pjsua_acc * acc = &pjsua_var.acc[_Acc_id];
	if (!acc->valid)
	{
		PJ_LOG(3,(__FILE__, "ERROR: ConfSubs::InicializarSubscripcion: Account %d no valido", _Acc_id)); 
		return -1;
	}

	if (_Call_id == -1)
	{
		pj_str_t contact;
		st = pjsua_acc_create_uac_contact(_Pool, &contact, _Acc_id, &_Dst_uri_pj, NULL);
		if (st != PJ_SUCCESS)
		{
			PJ_LOG(3, (__FILE__, "ERROR: ConfSubs::InicializarSubscripcion: No se puede generar contact para acc %d dst %s", _Acc_id, _Dst_uri_pj.ptr));
			return -1;
		}

		st = pjsip_dlg_create_uac(pjsip_ua_instance(), &acc->cfg.id, &contact, &_Dst_uri_pj, NULL, &_Dlg);
		if (st != PJ_SUCCESS)
		{
			PJ_LOG(3, (__FILE__, "ERROR: ConfSubs::InicializarSubscripcion: Unable to create dialog  acc_id %d dst %s", _Acc_id, _Dst_uri));
			return -1;
		}

		pjsip_dlg_inc_lock(_Dlg);
	}
	else
	{
		pjsua_call* call;
		st = acquire_call("ConfSubs::InicializarSubscripcion()", _Call_id, &call, &_Dlg);
		if (st != PJ_SUCCESS)
		{
			PJ_LOG(3, (__FILE__, "ERROR: ConfSubs::InicializarSubscripcion: Unable to acquire call _Call_id %d dst %s", _Call_id, _Dst_uri));
			return -1;
		}
	}
	
	st = pjsip_conf_create_uac(_Dlg, &conf_callback, PJSIP_EVSUB_NO_EVENT_ID, &_Module);
	if (st != PJ_SUCCESS)
	{
		PJ_LOG(3,(__FILE__, "ERROR: ConfSubs::InicializarSubscripcion: Unable to create client %s", _Dst_uri)); 
		_Module = NULL;
		if (_Dlg) pjsip_dlg_dec_lock(_Dlg);
		return -1;
	}

	if (_ByProxy)
	{
		//Si el subscribe va hacia el proxy entonces se agrega la cabecera route
		if (!pj_list_empty(&acc->route_set)) 
		{
			pjsip_dlg_set_route_set(_Dlg, &acc->route_set);
		}
	}

	pjsip_evsub_set_mod_data(_Module, pjsua_var.mod.id, this);

	//Se establece la cabacera WG67-Version para el envio de la primera subscripcion
	char ED137Radioversion_, ED137Phoneversion_;
	pjsip_endpt_Get_Ed137_version(pjsua_var.endpt, &ED137Radioversion_, &ED137Phoneversion_);
	char sWG67ver[32];
	if (ED137Phoneversion_ == 'C')
		pj_ansi_sprintf(sWG67ver, "%.*s, %.*s", SipCall::gWG67VersionTelefValue.slen, SipCall::gWG67VersionTelefValue.ptr,
			SipCall::gWG67VersionTelefValueC.slen, SipCall::gWG67VersionTelefValueC.ptr);
	else
		pj_ansi_sprintf(sWG67ver, "%.*s", SipCall::gWG67VersionTelefValue.slen, SipCall::gWG67VersionTelefValue.ptr);
	pjsip_dlg_set_WG67_version(_Dlg, sWG67ver, PJ_FALSE);

	pjsip_tx_data *tdata;

	st = pjsip_conf_initiate(_Module, _Expires, &tdata);

	if (st != PJ_SUCCESS)
	{
		PJ_LOG(3,(__FILE__, "ERROR: ConfSubs::InicializarSubscripcion: Unable to initiate client %s", _Dst_uri)); 
		if (_Module) pjsip_conf_terminate(_Module, PJ_FALSE);
		_Module = NULL;
		if (_Dlg) pjsip_dlg_dec_lock(_Dlg);
		return -1;
	}

	pjsua_process_msg_data(tdata, NULL);

	while (pj_sem_trywait(status_sem) == PJ_SUCCESS);
	st = pjsip_conf_send_request(_Module, tdata);
	if (st != PJ_SUCCESS)
	{
		PJ_LOG(3,(__FILE__, "ERROR: ConfSubs::InicializarSubscripcion: Unable to send initial SUBSCRIBE message %s", _Dst_uri)); 
		if (_Module) pjsip_conf_terminate(_Module, PJ_FALSE);
		_Module = NULL;
		if (_Dlg) pjsip_dlg_dec_lock(_Dlg);
		return -1;
	}

	if (_Dlg) pjsip_dlg_dec_lock(_Dlg);

	pj_sem_wait_for(status_sem, pjsip_cfg()->tsx.tsx_tout / 2);		//esperamos el tiempo maximo para una transaccion

	if (_Module)
	{
		pjsip_evsub_state status = pjsip_evsub_get_state(_Module);
		if (status == PJSIP_EVSUB_STATE_ACCEPTED || status == PJSIP_EVSUB_STATE_ACTIVE)
		{		
			return 0;
		}	

		//No se ha recibido a tiempo la confirmacion de la subscripcion. Terminamos.
		pjsip_conf_terminate(_Module, PJ_FALSE);
		_Module = NULL;
	}

	PJ_LOG(5,(__FILE__, "ERROR: ConfSubs::InicializarSubscripcion: SUBSCRIBE message is not answered %s", _Dst_uri)); 

	return -1;
}

/**
 * End.	...
 * Finaliza la subscripcion
 * @return	-1 si hay error.
 */
int ConfSubs::End()
{
	if (_Module == NULL) return 0;

	_confsub_cb = NULL;	//No quiero que se llame a esta callback cuando quiero finalizar la subscripcion.

	pjsip_evsub_state sta = pjsip_evsub_get_state(_Module);

	if (sta == PJSIP_EVSUB_STATE_TERMINATED)
	{		
		_Module = NULL;
		return 0;
	}

	pjsip_evsub_set_mod_data(_Module, pjsua_var.mod.id, NULL);

	pjsip_tx_data *tdata;
	pj_status_t st = pjsip_conf_initiate(_Module, 0, &tdata);
	if (st != PJ_SUCCESS)
	{
		if (_Module) 
		{
			pjsip_conf_terminate(_Module, PJ_FALSE);
		}
		_Module = NULL;
		return 0;
	}

	pjsua_process_msg_data(tdata, NULL);
	while (pj_sem_trywait(status_sem) == PJ_SUCCESS);
	st = pjsip_conf_send_request(_Module, tdata);
	if (st != PJ_SUCCESS)
	{
		pjsip_conf_terminate(_Module, PJ_FALSE);
		_Module = NULL;
		return 0;
	}
	else
	{
		//Esperamos a que se termine la subscripcion recibiendo el NOTIFY con estado a terminated
		int tries = pjsip_cfg()->tsx.tsx_tout / 40;
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

		_Module = NULL;
	}

	return 0;
}

void ConfSubs::conf_on_rx_notify(pjsip_evsub *sub, pjsip_rx_data *rdata, int *p_st_code, 
									pj_str_t **p_st_text, pjsip_hdr *res_hdr, pjsip_msg_body **p_body)
{
	ConfSubs * conf = reinterpret_cast<ConfSubs*>(pjsip_evsub_get_mod_data(sub, pjsua_var.mod.id));
	if (conf && SipAgent::Cb.ConfInfoCb) 
	{	
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

			SipAgent::Cb.ConfInfoCb(conf->_Acc_id | CORESIP_ACC_ID, &info, from_urich, size);
		}
	}
}

void ConfSubs::conf_on_state(pjsip_evsub *sub, pjsip_event *event)
{
	PJ_UNUSED_ARG(event);

	pjsip_evsub_state state;
	pj_bool_t post_status_sem = PJ_FALSE;

	ConfSubs * conf = reinterpret_cast<ConfSubs*>(pjsip_evsub_get_mod_data(sub, pjsua_var.mod.id));

	if (conf) 
	{
		PJ_LOG(4,(THIS_FILE, "CONF-IN subscription to %.*s is %s",
			conf->_Dst_uri_pj.slen, conf->_Dst_uri_pj.ptr, pjsip_evsub_get_state_name(sub)));

		state = pjsip_evsub_get_state(sub);
		switch (state)
		{
		case PJSIP_EVSUB_STATE_NULL:
			PJ_LOG(5,(THIS_FILE, "ConfSubs::conf_on_state PJSIP_EVSUB_STATE_NULL dst %s", conf->_Dst_uri_pj.ptr));
			break;
		case PJSIP_EVSUB_STATE_SENT:
			PJ_LOG(5,(THIS_FILE, "ConfSubs::conf_on_state PJSIP_EVSUB_STATE_SENT dst %s", conf->_Dst_uri_pj.ptr));
			break;
		case PJSIP_EVSUB_STATE_ACCEPTED:
			PJ_LOG(5,(THIS_FILE, "ConfSubs::conf_on_state PJSIP_EVSUB_STATE_ACCEPTED dst %s", conf->_Dst_uri_pj.ptr));
			post_status_sem = PJ_TRUE;
			break;
		case PJSIP_EVSUB_STATE_PENDING:
			PJ_LOG(5,(THIS_FILE, "ConfSubs::conf_on_state PJSIP_EVSUB_STATE_PENDING dst %s", conf->_Dst_uri_pj.ptr));
			break;
		case PJSIP_EVSUB_STATE_ACTIVE:
			PJ_LOG(5,(THIS_FILE, "ConfSubs::conf_on_state PJSIP_EVSUB_STATE_ACTIVE dst %s", conf->_Dst_uri_pj.ptr));
			post_status_sem = PJ_TRUE;
			break;
		case PJSIP_EVSUB_STATE_TERMINATED:
			PJ_LOG(5,(THIS_FILE, "ConfSubs::conf_on_state PJSIP_EVSUB_STATE_TERMINATED dst %s", conf->_Dst_uri_pj.ptr));

			//Ha cambiado el estado de la subscripcion
			conf->_Module = NULL;
			pjsip_evsub_set_mod_data(sub, pjsua_var.mod.id, NULL);	

			post_status_sem = PJ_TRUE;
			break;
		case PJSIP_EVSUB_STATE_UNKNOWN:
			PJ_LOG(5,(THIS_FILE, "ConfSubs::conf_on_state PJSIP_EVSUB_STATE_UNKNOWN dst %s", conf->_Dst_uri_pj.ptr));
			break;
		}

		ConfSubs* ret_conf = conf;
		if (conf->_confsub_cb != NULL)
		{
			ret_conf = (ConfSubs*)conf->_confsub_cb((void*)conf);
		}

		if (ret_conf != NULL && post_status_sem)
		{
			//Post del semaforo siempre y cuando el objeto de la subscripcion no se haya destruido
			pj_sem_post(conf->status_sem);
		}		
	}
}

void ConfSubs::conf_on_tsx_state(pjsip_evsub* sub, pjsip_transaction* tsx, pjsip_event* event)
{
	ConfSubs* conf_subs = reinterpret_cast<ConfSubs*>(pjsip_evsub_get_mod_data(sub, pjsua_var.mod.id));
	pjsip_dialog* subdlg = pjsip_evsub_get_dlg(sub);
	if (conf_subs && subdlg != NULL && tsx->role == PJSIP_ROLE_UAC && event->type == PJSIP_EVENT_TSX_STATE && event->body.tsx_state.type == PJSIP_EVENT_RX_MSG)
	{
		//Se ha recibido un mensaje SIP y se establece la cabecera WG67-version si no lo estaba
		if (pjsip_dlg_get_WG67_version(subdlg, NULL) == PJ_FALSE)
		{
			char radio_version = 0;
			char phone_version = 0;
			char WG67_version_value_buf[32];

			if (pjsip_endpt_Neg_ED137Version_from_msg(pjsua_var.endpt, tsx->last_tx->msg, event->body.rx_msg.rdata->msg_info.msg, &radio_version, &phone_version,
				WG67_version_value_buf, sizeof(WG67_version_value_buf)) == PJ_TRUE)
			{
				pjsip_dlg_set_WG67_version(subdlg, WG67_version_value_buf, PJ_TRUE);
				pjsip_tsx_set_WG67_version(tsx, WG67_version_value_buf);
			}
		}
	}
}

/**
 * conf_on_client_refresh.	...
 * Funcion callback que se llama cada vez que hay que refrescar la subscripcion.
 * Se utiliza para poder agregar la cabecera WG67-version que corresponde
 */
void ConfSubs::conf_on_client_refresh(pjsip_evsub *sub)
{
	pj_status_t status;
	pjsip_tx_data *tdata;

	ConfSubs* conf = reinterpret_cast<ConfSubs*>(pjsip_evsub_get_mod_data(sub, pjsua_var.mod.id));
	if (conf)
	{
		if (conf->_Manual) return;
	}

	status = pjsip_conf_initiate(sub, -1, &tdata);
	if (status == PJ_SUCCESS) {
	    pjsip_evsub_send_request(sub, tdata);
    }
}

/********************************************************************************************/
/* A partir de aqui se sefiere a cuando funciona como servidor de subscripcion*/
/********************************************************************************************/

pjsip_evsub_user ConfSubs::_ConfSrvCb = 
{
	&ConfSubs::OnConfSrvStateChanged,  
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

void ConfSubs::OnConfSrvStateChanged(pjsip_evsub *sub, pjsip_event *event)
{
	PJ_LOG(5, ("SipAgent.cpp", "Conference server subscription is %s sub %p", pjsip_evsub_get_state_name(sub), sub));
	if (pjsip_evsub_get_state(sub) == PJSIP_EVSUB_STATE_TERMINATED) 
	{	
		/*Al terminar la subscripcion al evento de conferencia, lo eliminamos de la lista del account correspondiente*/
		SubscriptionServer::Del_subMod(sub);
		pjsip_evsub_set_user_data(sub, NULL);		
	} 
}

/**
 * SubscriptionRxRequest.	...
 * Gestona la recepcion de un request para abrir un nuevo dialogo de subscripcion al evento de conferencia.
 * @param rdata. Incoming message buffer 
 * @return	true si se admite.
 */
pj_bool_t ConfSubs::SubscriptionRxRequest(pjsip_rx_data* rdata)
{
	//Hemos recibido una subscripcion al evento de conferencia sin to-tag. O sea fuera de dialogo INV.				

	pj_str_t STR_EVENT = { "Event", 5 };
	pj_str_t STR_CONFERENCE = { "conference", 10 };

	//Hemos recibido una subscripcion sin to-tag
	pjsip_event_hdr* event = (pjsip_event_hdr*)pjsip_msg_find_hdr_by_name(rdata->msg_info.msg, &STR_EVENT, NULL);
	pjsip_expires_hdr* expires = (pjsip_expires_hdr*)pjsip_msg_find_hdr(rdata->msg_info.msg, PJSIP_H_EXPIRES, NULL);
	pjsip_to_hdr* to = (pjsip_to_hdr*)pjsip_msg_find_hdr(rdata->msg_info.msg, PJSIP_H_TO, NULL);
	pjsip_from_hdr* from = (pjsip_from_hdr*)pjsip_msg_find_hdr(rdata->msg_info.msg, PJSIP_H_FROM, NULL);
	if (to == NULL || from == NULL) return PJ_FALSE;

	//Buscamos el acc al que va dirigido
	pjsua_acc_id accid = pjsua_acc_find_for_incoming_by_uri(to->uri);
	if (accid == PJSUA_INVALID_ID) return PJ_FALSE;

	//Comprobamos si esta solicitud de subscripcion ya ha sido descartada
	if (SubscriptionServer::IsInDeletedList(accid, from->uri, &from->tag) == PJ_TRUE)
	{
		//Ya ha sido descartado. No hacemos nada
		return PJ_TRUE;
	}

	if (event == NULL)
	{
		return PJ_FALSE;
	}

	if (pj_stricmp(&event->event_type, &STR_CONFERENCE) != 0)
	{
		return PJ_FALSE;
	}

	if (expires->ivalue == 0)
	{
		return PJ_FALSE;
	}

	//Hemos recibido una subscripcion al evento de conferencia sin to-tag. O sea fuera de dialogo INV.				

	pj_pool_t* tmppool = pjsua_pool_create(NULL, 64, 32);
	PJ_ASSERT_RETURN(tmppool != NULL, PJ_FALSE);
	pj_str_t contact;
	pj_status_t st = pjsua_acc_create_uas_contact(tmppool, &contact, accid, rdata);
	if (st != PJ_SUCCESS)
	{
		PJ_LOG(3, (__FILE__, "ERROR: ConfSubs::SubscriptionRxRequest: No se puede generar contact"));
		pj_pool_release(tmppool);
		return PJ_FALSE;
	}

	pjsip_dialog* dlg;
	pjsip_evsub* _ConfsubMod;
	st = pjsip_dlg_create_uas(pjsip_ua_instance(), rdata, &contact, &dlg);
	pj_pool_release(tmppool);

	if (st != PJ_SUCCESS)
	{
		PJ_LOG(3, (__FILE__, "ERROR: ConfSubs::SubscriptionRxRequest: No se puede crear el dialogo como UAS"));
		return PJ_FALSE;
	}

	pjsip_dlg_inc_lock(dlg);

	char radio_version = 0;
	char phone_version = 0;
	char WG67_version_value_buf[32];

	if (pjsip_endpt_Neg_ED137Version_from_msg(pjsua_var.endpt, NULL, rdata->msg_info.msg, &radio_version, &phone_version,
		WG67_version_value_buf, sizeof(WG67_version_value_buf)) == PJ_TRUE)
	{
		pjsip_dlg_set_WG67_version(dlg, WG67_version_value_buf, PJ_TRUE);
	}

	pjsip_evsub* conftmp = SubscriptionServer::Get_subMod(accid, dlg->remote.contact->uri, &STR_CONFERENCE);
	if (conftmp != NULL)
	{
		//El usuario ya está subscrito.
		SubscriptionServer::Del_subMod(conftmp);
		pjsip_conf_terminate(conftmp, PJ_FALSE);		//Eliminamos la subscripcion antigua y nos quedamos con la nueva																				
		SubscriptionServer::Add_DeletedsubModlist(accid, dlg->remote.info->uri, &dlg->remote.info->tag);
		//Se agrega a la lista de borrados
	}

	st = pjsip_conf_create_uas(dlg, &ConfSubs::_ConfSrvCb, rdata, &_ConfsubMod);
	pjsip_dlg_dec_lock(dlg);

	if (st != PJ_SUCCESS)
	{
		PJ_LOG(3, (__FILE__, "ERROR: ConfSubs::SubscriptionRxRequest: No se puede crear la subscripcion como UAS"));
		return PJ_FALSE;
	}

	pj_pool_t* pool = pjsip_evsub_get_pool(_ConfsubMod);
	SubscriptionServer::subs_user_data* sub_user_data = (SubscriptionServer::subs_user_data*) pj_pool_alloc(pool, sizeof(SubscriptionServer::subs_user_data));
	PJ_ASSERT_RETURN(sub_user_data != NULL, PJ_FALSE);
	sub_user_data->accid = accid;
	sub_user_data->send_notify_at_tsx_terminate = PJ_FALSE;
	pjsip_evsub_set_user_data(_ConfsubMod, (void*)sub_user_data);

	st = pjsip_conf_accept(_ConfsubMod, rdata, PJSIP_SC_OK, NULL);
	if (st == PJ_SUCCESS)
	{
		/*
			* He visto que cuando se crea el servidor a la subscripcion de conferencia no se arranca el timeout
			* que vence cuando expire time se cumple. Sin embargo cuando se reciben los subscribes de refresco sí.
			* Por tanto creo esta funcion para arrancar el timer.
			*/
		set_timer_uas_timeout(_ConfsubMod);

		PJ_LOG(5, ("SipAgent.cpp", "SipAgent::OnRxRequest conference sub %p creado con exito", _ConfsubMod));
		pjsip_evsub_set_state(_ConfsubMod, PJSIP_EVSUB_STATE_ACCEPTED);

		/*Agregamos a la lista de subscripciones al evento de conferencia del account correspondiente*/
		SubscriptionServer::Add_subMod(_ConfsubMod);

		if (SipAgent::Cb.IncomingSubscribeConfCb)
		{
			//Se notifica a la aplicacion que se ha aceptado la subscripcion al evento de conferencia
			//para que envie el notify con el estado
			char from_urich[CORESIP_MAX_URI_LENGTH];
			int size = pjsip_uri_print(PJSIP_URI_IN_FROMTO_HDR, from->uri, from_urich, CORESIP_MAX_URI_LENGTH - 1);
			from_urich[size] = '\0';
			SipAgent::Cb.IncomingSubscribeConfCb(accid | CORESIP_ACC_ID, (const char*)from_urich, (const int)size);
		}		
	}
	else
	{
		return PJ_FALSE;
	}

	return PJ_TRUE;
}