/**
 * @file SubscriptionServer.cpp
 *
 * Esta clase define un servidor de subscripcion a eventos
 */

#include"SubscriptionServer.h"
#include "AccountUserData.h"
#include "dlgsub.h"
#include "WG67Subs.h"

SubscriptionServer::SubscriptionServer()
{
	_Notify_enabled = PJ_TRUE;
	_Manual_WG67_Notify = PJ_FALSE;
	_Minimum_expires_As_UAS = ABSOLUTE_MINIMUM_EXPIRES_AS_UAS;
	_Maximum_expires_As_UAS = ABSOLUTE_MAXIMUM_EXPIRES_AS_UAS;

	_Pool = pjsua_pool_create(NULL, 64, 32);
	if (_Pool == NULL)
	{
		throw PJLibException(__FILE__, PJ_ENOMEM).Msg("SubscriptionServer: No hay suficiente memoria");
		return;
	}

	_subModlist_mutex = NULL;
	pj_status_t st = pj_mutex_create_simple(_Pool, NULL, &_subModlist_mutex);
	if (st != PJ_SUCCESS)
	{
		throw PJLibException(__FILE__, PJ_ENOMEM).Msg("SubscriptionServer: No se puede crear _subModlist_mutex");
		return;
	}

	pWG67Subscriptions_array = NULL;
}

SubscriptionServer::~SubscriptionServer()
{
	if (_subModlist_mutex != NULL)
	{
		pj_mutex_destroy(_subModlist_mutex);
		_subModlist_mutex = NULL;
	}
	if (_Pool != NULL)
	{
		pj_pool_release(_Pool);
	}
}

/**
 * Add_subMod: Agrega a la lista de subscripciones correspondientes a un account
 * @param	sub	Subscripcion que agrega
 * @return	0 si no hay error
 */
int SubscriptionServer::Add_subMod(pjsip_evsub* sub)
{
	subs_user_data* sub_user_data = (subs_user_data*)pjsip_evsub_get_user_data(sub);
	if (sub_user_data == NULL)
	{
		PJ_LOG(3, ("SipAgent.cpp", "ERROR: Add_subMod evsub invalido"));
		return -1;
	}
	pjsua_acc_id accid = sub_user_data->accid;
	if (accid == PJSUA_INVALID_ID)
	{
		PJ_LOG(3, ("SipAgent.cpp", "ERROR: Add_subMod evsub invalido"));
		return -1;
	}

	AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(accid);
	if (accUserData != NULL)
	{
		size_t subs_count = accUserData->subsServer->_subModlist.size();
		PJ_LOG(3, ("SubscriptionServer::Add_subMod antes de add", "INFO: Add_subMod antes de add subs_count %d", subs_count));

		pj_mutex_lock(accUserData->subsServer->_subModlist_mutex);
		accUserData->subsServer->_subModlist.remove(sub);		//Lo quito por si ya estuviera y me aseguro de que solo hay uno
		accUserData->subsServer->_subModlist.push_back(sub);	//Agrego el nuevo elemento
		pj_mutex_unlock(accUserData->subsServer->_subModlist_mutex);
	}
	return 0;
}

/**
 * Del_subMod: Quita de la lista de subscripciones correspondientes a un account
 * @param	sub	Subscripcion que quita
 * @return	0 si no hay error
 */
int SubscriptionServer::Del_subMod(pjsip_evsub* sub)
{
	subs_user_data* sub_user_data = (subs_user_data*)pjsip_evsub_get_user_data(sub);
	if (sub_user_data == NULL)
	{
		PJ_LOG(5, ("AccountUserData", "WARNING: Del_subMod evsub parece que ya ha sido borrada"));
		return -1;
	}
	pjsua_acc_id accid = sub_user_data->accid;
	if (accid == PJSUA_INVALID_ID)
	{
		PJ_LOG(5, ("AccountUserData", "WARNING: Del_subMod evsub parece que ya ha sido borrada"));
		return -1;
	}

	AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(accid);
	if (accUserData != NULL)
	{
		pj_mutex_lock(accUserData->subsServer->_subModlist_mutex);
		accUserData->subsServer->_subModlist.remove(sub);				//Lo quitamos de la lista de subscripciones activas
		size_t subs_count = accUserData->subsServer->_subModlist.size();
		PJ_LOG(3, ("SubscriptionServer::Del_subMod", "INFO: Del_subMod: despues de del subs_count %d", subs_count));
		pj_mutex_unlock(accUserData->subsServer->_subModlist_mutex);
	}
	return 0;
}

/**
 * Get_subMod: Retorna el objeto pjsip_evsub para un contact concreto y un tipo de evento
 * @param	accid	Account id			Account id
 * @param	remote_uri	Uri del contact
 * @param	event_type	Tipo de evento de la subscripcion
 * @return	NULL si no lo encuentra
 */
pjsip_evsub* SubscriptionServer::Get_subMod(pjsua_acc_id accid, pjsip_uri* remote_uri, pj_str_t* event_type)
{
	pjsip_evsub* ret = NULL;

	if (accid < 0 || accid >= (int)PJ_ARRAY_SIZE(pjsua_var.acc))
	{
		PJ_LOG(3, ("SubscriptionServer::Get_subMod", "Get_subMod:", "accid %d no valido", accid));
		return ret;
	}

	AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(accid);
	if (accUserData == NULL) return ret;
	if (accUserData->subsServer == NULL) return ret;
	if (accUserData->subsServer->_subModlist_mutex == NULL) return ret;

	pj_mutex_lock(accUserData->subsServer->_subModlist_mutex);
	std::list<pjsip_evsub*, std::allocator<pjsip_evsub*>>::iterator it;
	it = accUserData->subsServer->_subModlist.begin();
	for (it = accUserData->subsServer->_subModlist.begin(); it != accUserData->subsServer->_subModlist.end(); it++)
	{
		pjsip_evsub* sub = *it;

		if (sub)
		{
			pjsip_dialog* dlg = pjsip_evsub_get_dlg(sub);
			if (dlg != NULL)
			{
				pjsip_dlg_inc_lock(dlg);
				pjsip_event_hdr* eventhdr = (pjsip_event_hdr*)pjsip_evsub_get_event_hdr(sub);
				if (eventhdr != NULL)
				{
					pjsip_sip_uri* sip_remote_uri = (pjsip_sip_uri*)pjsip_uri_get_uri(remote_uri);
					pjsip_sip_uri* sip_dlg_remote_uri = (pjsip_sip_uri*)pjsip_uri_get_uri(dlg->remote.contact->uri);
					if (sip_remote_uri != NULL && sip_dlg_remote_uri != NULL)
					{
						int sip_remote_uri_port = sip_remote_uri->port;
						if (sip_remote_uri->port == 0 || sip_remote_uri->port == 5060) sip_remote_uri_port = 0;
						int sip_dlg_remote_uri_port = sip_dlg_remote_uri->port;
						if (sip_dlg_remote_uri->port == 0 || sip_dlg_remote_uri->port == 5060) sip_dlg_remote_uri_port = 0;

						if (pj_stricmp(&sip_remote_uri->user, &sip_dlg_remote_uri->user) == 0 &&
							pj_stricmp(&sip_remote_uri->host, &sip_dlg_remote_uri->host) == 0 &&
							sip_remote_uri_port == sip_dlg_remote_uri_port &&
							pj_strcmp(event_type, &eventhdr->event_type) == 0)
						{
							ret = sub;
							pjsip_dlg_dec_lock(dlg);
							break;
						}
					}
				}
				pjsip_dlg_dec_lock(dlg);
			}
		}
	}
	pj_mutex_unlock(accUserData->subsServer->_subModlist_mutex);

	return ret;
}
/**
 * Get_subMod: Retorna el objeto pjsip_evsub para un dialogo
 * @param	accid	Account id			Account id
 * @param	dlg		Dialogo
 * @param	event_type	Tipo de evento de la subscripcion
 * @return	NULL si no lo encuentra
 */
pjsip_evsub* SubscriptionServer::Get_subMod(pjsua_acc_id accid, pjsip_dialog *dlg, pj_str_t* event_type)
{
	if (dlg == NULL) return NULL;

	pjsip_evsub* ret = NULL;

	AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(accid);
	if (accUserData == NULL) return ret;
	if (accUserData->subsServer == NULL) return ret;
	if (accUserData->subsServer->_subModlist_mutex == NULL) return ret;

	pj_mutex_lock(accUserData->subsServer->_subModlist_mutex);
	std::list<pjsip_evsub*, std::allocator<pjsip_evsub*>>::iterator it;
	it = accUserData->subsServer->_subModlist.begin();
	for (it = accUserData->subsServer->_subModlist.begin(); it != accUserData->subsServer->_subModlist.end(); it++)
	{
		pjsip_evsub* sub = *it;

		if (sub)
		{
			pjsip_dialog* dlgsub = pjsip_evsub_get_dlg(sub);
			if (dlgsub != NULL)
			{
				pjsip_dlg_inc_lock(dlgsub);
				pjsip_event_hdr* eventhdr = (pjsip_event_hdr*)pjsip_evsub_get_event_hdr(sub);
				if (eventhdr != NULL)
				{
					if ((pj_strcmp(event_type, &eventhdr->event_type) == 0) && (dlg == dlgsub))
					{
						ret = sub;
						pjsip_dlg_dec_lock(dlgsub);
						break;
					}
				}
				pjsip_dlg_dec_lock(dlgsub);				
			}
		}
	}
	pj_mutex_unlock(accUserData->subsServer->_subModlist_mutex);

	return ret;
}

/**
 * Add_DeletedsubModlist: Agrega un elemento a _DeletedConfsubModlist
 * @param	accid	Account id			Account id
 * @param	remote_uri	Uri del from
 * @param	remote_from_tag	Uri del from
 * Retorn -1 si hay error
 */
int SubscriptionServer::Add_DeletedsubModlist(pjsua_acc_id accid, pjsip_uri* remote_uri, pj_str_t* remote_from_tag)
{
	if (accid < 0 || accid >= (int)PJ_ARRAY_SIZE(pjsua_var.acc))
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("Add_DeletedsubModlist:", "accid %d no valido", accid);
		return -1;
	}

	AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(accid);
	if (accUserData != NULL)
	{
		pj_bool_t agregar_a_DeletedsubModlist = PJ_TRUE;
		from_user_subs* fuser = (from_user_subs*)malloc(sizeof(from_user_subs));
		if (fuser == NULL)
		{
			PJ_LOG(3, ("SubscriptionServer", "ERROR: Add_DeletedsubModlist No hay memoria"));
			agregar_a_DeletedsubModlist = PJ_FALSE;
		}
		else
		{
			pjsip_sip_uri* from_uri = (pjsip_sip_uri*)pjsip_uri_get_uri(remote_uri);
			if (from_uri == NULL) agregar_a_DeletedsubModlist = PJ_FALSE;
			else
			{
				if (IsInDeletedList(accid, remote_uri, remote_from_tag))
				{
					//Ya esta en la lista de borrados. No lo agregamos
					agregar_a_DeletedsubModlist = PJ_FALSE;
				}
				else if ((from_uri->user.slen > ((sizeof(fuser->user) / sizeof(char)) - 1)) ||
					(from_uri->host.slen > ((sizeof(fuser->host) / sizeof(char)) - 1)) ||
					(remote_from_tag->slen > ((sizeof(fuser->tag) / sizeof(char)) - 1)))
				{
					agregar_a_DeletedsubModlist = PJ_FALSE;
				}
				else
				{
					strncpy(fuser->user, from_uri->user.ptr, from_uri->user.slen);
					fuser->user[from_uri->user.slen] = '\0';
					strncpy(fuser->host, from_uri->host.ptr, from_uri->host.slen);
					fuser->user[from_uri->host.slen] = '\0';
					strncpy(fuser->tag, remote_from_tag->ptr, remote_from_tag->slen);
					fuser->user[remote_from_tag->slen] = '\0';
				}
			}
		}

		if (agregar_a_DeletedsubModlist)
		{
			//Lo ponemos en la lista de subscripciones borradas
			pj_mutex_lock(accUserData->subsServer->_subModlist_mutex);
			if (accUserData->subsServer->_DeletedsubModlist.size() == MAX_DeletedsubModlist_size)
			{
				//La lista está llena. Quitamos el elemento mas antiguo
				from_user_subs* fuser_tmp = accUserData->subsServer->_DeletedsubModlist.front();
				free(fuser_tmp);
				accUserData->subsServer->_DeletedsubModlist.pop_front();
			}
			//Agregamos un nuevo elemento a la lista de los borrados
			accUserData->subsServer->_DeletedsubModlist.push_back(fuser);
			pj_mutex_unlock(accUserData->subsServer->_subModlist_mutex);
		}
		else if (fuser != NULL)
		{
			free(fuser);
		}
	}
	return 0;
}

/**
 * IsInDeletedList: Busca en la lista de subscripciones borradas una from uri y un from tag.
 * Es posible que nos llegue una solicitud de subscripcion rezagada de una que ya se había descartado
 * @param	accid	Account id			Account id
 * @param	remote_uri	Uri del from
 * @param	remote_from_tag	Uri del from
 * @return	Retorna true si esta en la lista
 */
pj_bool_t SubscriptionServer::IsInDeletedList(pjsua_acc_id accid, pjsip_uri* remote_uri, pj_str_t* remote_from_tag)
{
	if (accid < 0 || accid >= (int)PJ_ARRAY_SIZE(pjsua_var.acc))
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("SendConfInfoFromAcc:", "accid %d no valido", accid);
	}

	pj_bool_t ret = PJ_FALSE;

	AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(accid);
	if (accUserData == NULL) return ret;

	pjsip_sip_uri* from_uri = (pjsip_sip_uri*)pjsip_uri_get_uri(remote_uri);
	if (from_uri == NULL) return ret;

	pj_mutex_lock(accUserData->subsServer->_subModlist_mutex);
	std::list<from_user_subs*, std::allocator<from_user_subs*>>::iterator it;
	it = accUserData->subsServer->_DeletedsubModlist.begin();
	for (it = accUserData->subsServer->_DeletedsubModlist.begin(); it != accUserData->subsServer->_DeletedsubModlist.end(); it++)
	{
		from_user_subs* fuser = *it;
		if (fuser)
		{
			if ((strncmp(fuser->user, from_uri->user.ptr, from_uri->user.slen) == 0) &&
				(strncmp(fuser->host, from_uri->host.ptr, from_uri->host.slen) == 0) &&
				(strncmp(fuser->tag, remote_from_tag->ptr, remote_from_tag->slen) == 0))
			{
				ret = PJ_TRUE;
				break;
			}
		}
	}
	pj_mutex_unlock(accUserData->subsServer->_subModlist_mutex);

	return ret;
}

/**
 * TerminateSubscriptions: Termina todas las subscripciones a un evento
 * @param	accid	Account id			Account id
 * @param	event_type	Tipo de evento de la subscripcion
 * @return	
 */
void SubscriptionServer::TerminateSubscriptions(pj_str_t* event_type)
{
	pj_str_t STR_WG67KEY_IN = { "WG67KEY-IN", 10 };
	pjsua_acc_info info[PJSUA_MAX_ACC];
	unsigned c;
	int ret = CORESIP_OK;

	c = PJ_ARRAY_SIZE(info);
	pj_status_t status = pjsua_acc_enum_info(info, &c);
	if (status != PJ_SUCCESS)
	{
		return;
	}

	for (unsigned i = 0; i < c; i++)
	{
		AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(info[i].id);
		if (accUserData == NULL) return;
		if (accUserData->subsServer == NULL) return;
		if (accUserData->subsServer->_subModlist_mutex == NULL) return;

		std::list<pjsip_evsub*, std::allocator<pjsip_evsub*>> Subscriptions_to_terminate;

		pj_mutex_lock(accUserData->subsServer->_subModlist_mutex);
		std::list<pjsip_evsub*, std::allocator<pjsip_evsub*>>::iterator it;
		it = accUserData->subsServer->_subModlist.begin();
		for (it = accUserData->subsServer->_subModlist.begin(); it != accUserData->subsServer->_subModlist.end(); it++)
		{
			pjsip_evsub* sub = *it;

			if (sub)
			{
				pjsip_dialog* dlgsub = pjsip_evsub_get_dlg(sub);
				if (dlgsub != NULL)
				{
					pjsip_dlg_inc_lock(dlgsub);
					pjsip_event_hdr* eventhdr = (pjsip_event_hdr*)pjsip_evsub_get_event_hdr(sub);
					if (eventhdr != NULL)
					{
						if (pj_strcmp(event_type, &eventhdr->event_type) == 0)
						{
							Subscriptions_to_terminate.push_back(sub);							
						}
					}
					pjsip_dlg_dec_lock(dlgsub);					
				}
			}
		}
		pj_mutex_unlock(accUserData->subsServer->_subModlist_mutex);

		it = Subscriptions_to_terminate.begin();
		for (it = Subscriptions_to_terminate.begin(); it != Subscriptions_to_terminate.end(); it++)
		{
			pjsip_evsub* sub = *it;

			if (pj_stricmp(event_type, &STR_WG67KEY_IN) == 0)
			{
				SubscriptionServer::subs_user_data* sub_user_data = (SubscriptionServer::subs_user_data*)pjsip_evsub_get_user_data(sub);
				if (sub_user_data != NULL)
				{
					strcpy(sub_user_data->subsState.subscription_state, "terminated");
					strcpy(sub_user_data->subsState.reason, "noresource");
					sub_user_data->subsState.expires = -1;
					sub_user_data->subsState.retry_after = -1;
				}				

				WG67Subs::Send_WG67_current_notify(sub);
			}
			else
			{
				pjsip_evsub_terminate(sub, PJ_TRUE);
			}
		}
	}

	return;
}

/**
 * SendConfInfoFromAcc: Envia Notify con la info de la conferencia a todas las subscripciones al evento
 *						de conferencia que tiene un account
 * @param	accid	Account id
 * @param	conf	Info de de conferencia
 * @return	0 si no hay error
 */
void SubscriptionServer::SendConfInfoFromAcc(pjsua_acc_id accid, const CORESIP_ConfInfo* conf)
{
	if (accid < 0 || accid >= (int)PJ_ARRAY_SIZE(pjsua_var.acc))
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("SendConfInfoFromAcc:", "accid %d no valido", accid);
	}

	AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(accid);
	if (accUserData == NULL) return;

	std::list<pjsip_evsub*, std::allocator<pjsip_evsub*>> sub_to_send_current_notify_list;	//Contiene las subscripciones que envian los Notify

	//Busca las las subscripciones que envian los Notify. No lo enviamos dentro del buble ya que la funcion pjsip_dialog_current_notify puede modificar la lista _subModlist
	pj_mutex_lock(accUserData->subsServer->_subModlist_mutex);
	std::list<pjsip_evsub*, std::allocator<pjsip_evsub*>>::iterator it;
	for (it = accUserData->subsServer->_subModlist.begin(); it != accUserData->subsServer->_subModlist.end(); it++)
	{
		pjsip_evsub* confsub = *it;
		if (confsub)
		{
			pjsip_event_hdr* eventhdr = (pjsip_event_hdr*)pjsip_evsub_get_event_hdr(confsub);
			if (eventhdr)
			{
				pj_str_t STR_CONFERENCE = { "conference", 10 };
				if (pj_strcmp(&eventhdr->event_type, &STR_CONFERENCE) == 0)
				{
					sub_to_send_current_notify_list.push_back(confsub);					
				}
			}
		}
	}
	pj_mutex_unlock(accUserData->subsServer->_subModlist_mutex);

	for (it = sub_to_send_current_notify_list.begin(); it != sub_to_send_current_notify_list.end(); it++)
	{
		pjsip_evsub* confsub = *it;
		if (confsub)
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

			pj_status_t st = pjsip_conf_set_status(confsub, &info);
			if (st == PJ_SUCCESS)
			{
				pjsip_tx_data* tdata;
				st = pjsip_conf_current_notify(confsub, &tdata);
				if (st == PJ_SUCCESS)
				{
					PJ_LOG(5, ("sipcall.cpp", "NOTIFY CONF: Envia conference NOTIFY %s", pjsip_evsub_get_state_name(confsub)));
					st = pjsip_conf_send_request(confsub, tdata);
				}
			}
		}
	}
}

/**
 * SendDialogNotifyFromAcc: Envia el notify del evento de dialogo a todas las subscripciones activas en un account
 * @param	callid. Llamada cuyo evento de dialogo hay que enviar a todas las subscripciones
 * @param with_body. Includes body in the notify
 * @return	0 si no hay error
 */
void SubscriptionServer::SendDialogNotifyFromAcc(pjsua_call_id call_id, pj_bool_t with_body)
{
	pjsua_call_info callInfo;
	if (pjsua_call_get_info(call_id, &callInfo) != PJ_SUCCESS)
	{
		return;
	}

	pjsua_acc_id accid = callInfo.acc_id;

	AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(accid);
	if (accUserData == NULL) return;

	std::list<pjsip_evsub*, std::allocator<pjsip_evsub*>> sub_to_send_current_notify_list;	//Contiene las subscripciones que envian los Notify

	//Busca las las subscripciones que envian los Notify. No lo enviamos dentro del buble ya que la funcion pjsip_dialog_current_notify puede modificar la lista _subModlist
	pj_mutex_lock(accUserData->subsServer->_subModlist_mutex);
	if (accUserData->subsServer->_subModlist.size() > 0)
	{
		std::list<pjsip_evsub*, std::allocator<pjsip_evsub*>>::iterator it;
		it = accUserData->subsServer->_subModlist.begin();
		for (it = accUserData->subsServer->_subModlist.begin(); it != accUserData->subsServer->_subModlist.end(); it++)
		{
			pjsip_evsub* dlgsub = *it;
			if (dlgsub)
			{
				pjsip_event_hdr* eventhdr = (pjsip_event_hdr*)pjsip_evsub_get_event_hdr(dlgsub);
				if (eventhdr)
				{
					pj_str_t STR_DIALOG = { "dialog", 6 };
					if (pj_strcmp(&eventhdr->event_type, &STR_DIALOG) == 0)
					{
						sub_to_send_current_notify_list.push_back(dlgsub);						
					}
				}
			}
		}
	}
	pj_mutex_unlock(accUserData->subsServer->_subModlist_mutex);

	std::list<pjsip_evsub*, std::allocator<pjsip_evsub*>>::iterator it;
	for (it = sub_to_send_current_notify_list.begin(); it != sub_to_send_current_notify_list.end(); it++)
	{
		pjsip_evsub* dlgsub = *it;
		if (dlgsub)
		{
			pjsip_dialog_current_notify(call_id, dlgsub, with_body);
		}
	}
}

/**
 * SendWG67NotifyFromAcc: Envia el notify del evento WG67KEY-IN a todas las subscripciones activas en un account
 * @param	call_id. Llamada cuyo evento de WG67 hay que enviar a todas las subscripciones
 * @return	0 si no hay error
 */
void SubscriptionServer::SendWG67NotifyFromAcc(pjsua_call_id call_id, pj_bool_t delayed, pj_bool_t no_enviar_WG67_Notify_si_todas_las_sesiones_son_ficticias)
{
	pjsua_call_info callInfo;
	if (pjsua_call_get_info(call_id, &callInfo) != PJ_SUCCESS)
	{
		return;
	}

	pjsua_acc_id accid = callInfo.acc_id;

	AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(accid);
	if (accUserData == NULL) return;

	if (accUserData->subsServer->_Manual_WG67_Notify == PJ_TRUE) return;

	if (!delayed)
	{
		SendWG67NotifyFromAcc(accid, PJ_TRUE, NULL, NULL, PJ_FALSE, no_enviar_WG67_Notify_si_todas_las_sesiones_son_ficticias);
	}
	else
	{
		pjsua_cancel_timer(&accUserData->SendWG67NotifyFromAcc_delayed_timer);
		accUserData->SendWG67NotifyFromAcc_delayed_timer.id = 0;

		accUserData->SendWG67NotifyFromAcc_delayed_timer.cb = AccountUserData::SendWG67NotifyFromAcc_delayed_timer_cb;
		accUserData->no_enviar_WG67_Notify_si_todas_las_sesiones_son_ficticias = no_enviar_WG67_Notify_si_todas_las_sesiones_son_ficticias;
		accUserData->SendWG67NotifyFromAcc_delayed_timer.user_data = (void*)accUserData;
		pj_time_val	delay1;
		delay1.sec = (long)0;
		delay1.msec = 20;		
		accUserData->SendWG67NotifyFromAcc_delayed_timer.id = 1;
		pj_status_t st = pjsua_schedule_timer(&accUserData->SendWG67NotifyFromAcc_delayed_timer, &delay1);
		if (st != PJ_SUCCESS)
		{
			accUserData->SendWG67NotifyFromAcc_delayed_timer.id = 0;
			PJ_LOG(3, ("SubscriptionServer.cpp", "ERROR: SendWG67NotifyFromAcc SendWG67NotifyFromAcc_delayed_timer cannot be scheduled"));
		}
	}
}

/**
 * SendWG67NotifyFromAcc: Envia el notify del evento WG67KEY-IN a todas las subscripciones activas en un account
 * @param	acc_id. Account. Si es -1 entonces es la default
 * @param   only_actives. Solo manda los notify a las suscripciones que esten activas.
 * @param	subsState. Si es distinto de NULL se envia este nuevo estado
 * @param	wG67Notify_Body. Si es distinto de NULL se envia este body
 * @param	only_ED137C. Solo se envian en las subscripciones que cumple la ED137C.
 * @return	0 si no hay error
 */
void SubscriptionServer::SendWG67NotifyFromAcc(pjsua_acc_id acc_id, pj_bool_t only_actives,
	CORESIP_WG67Notify_SubscriptionState_Config* subsState,	CORESIP_WG67Notify_Body_Config* wG67Notify_Body,
	pj_bool_t only_ED137C, pj_bool_t no_enviar_WG67_Notify_si_todas_las_sesiones_son_ficticias)
{
	pjsua_acc_id accid;
	if (acc_id == -1)
	{
		accid = pjsua_acc_get_default();
	}
	else
	{
		accid = acc_id;
	}

	if (!pjsua_acc_is_valid(accid))
	{
		PJ_LOG(3, ("SubscriptionServer.cpp", "SendWG67NotifyFromAcc invalid account"));
		return;
	}

	AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(accid);
	if (accUserData == NULL) return;

	std::list<pjsip_evsub*, std::allocator<pjsip_evsub*>> sub_to_send_current_notify_list;	//Contiene las subscripciones que envian los Notify

	//Busca las subscripciones que envian los Notify. No lo enviamos dentro del bucle ya que la funcion Send_WG67_current_notify puede modificar la lista _subModlist
	pj_mutex_lock(accUserData->subsServer->_subModlist_mutex);
	if (accUserData->subsServer->_subModlist.size() > 0)
	{
		std::list<pjsip_evsub*, std::allocator<pjsip_evsub*>>::iterator it;
		for (it = accUserData->subsServer->_subModlist.begin(); it != accUserData->subsServer->_subModlist.end(); it++)
		{
			pjsip_evsub* wg76sub = *it;
			if (wg76sub)
			{
				pjsip_event_hdr* eventhdr = (pjsip_event_hdr*)pjsip_evsub_get_event_hdr(wg76sub);
				if (eventhdr)
				{
					pj_str_t STR_SG67 = { "WG67KEY-IN", 10 };
					if (pj_strcmp(&eventhdr->event_type, &STR_SG67) == 0)
					{
						pjsip_evsub_state state = pjsip_evsub_get_state(wg76sub);
						if (!only_actives || (only_actives && state == PJSIP_EVSUB_STATE_ACTIVE))
						{
							SubscriptionServer::subs_user_data* sub_user_data = (SubscriptionServer::subs_user_data*)pjsip_evsub_get_user_data(wg76sub);
							pj_bool_t add_subs_to_list = PJ_TRUE;
							if (sub_user_data != NULL)
							{
								if (subsState != NULL)
								{
									memcpy(&sub_user_data->subsState, subsState, sizeof(CORESIP_WG67Notify_SubscriptionState_Config));
								}
								if (wG67Notify_Body != NULL)
								{
									memcpy(&sub_user_data->wG67Notify_Body_Config, wG67Notify_Body, sizeof(CORESIP_WG67Notify_Body_Config));
								}

								if (no_enviar_WG67_Notify_si_todas_las_sesiones_son_ficticias && sub_user_data->wG67Notify_Body_Config.exclude_real_sessions)
								{
									add_subs_to_list = PJ_FALSE;
								}
							}							

							if (add_subs_to_list) sub_to_send_current_notify_list.push_back(wg76sub);
						}
					}
				}
			}
		}
	}
	pj_mutex_unlock(accUserData->subsServer->_subModlist_mutex);

	std::list<pjsip_evsub*, std::allocator<pjsip_evsub*>>::iterator it;
	for (it = sub_to_send_current_notify_list.begin(); it != sub_to_send_current_notify_list.end(); it++)
	{
		pjsip_evsub* wg76sub = *it;
		if (wg76sub)
		{	
			if (only_ED137C)
			{
				pjsip_dialog* dlg = pjsip_evsub_get_dlg(wg76sub);
				if (dlg->WG67_version != NULL && pj_stricmp2(&dlg->WG67_version->hvalue, "radio.02") == 0)
				{
					WG67Subs::Send_WG67_current_notify(wg76sub);
				}
			}
			else
			{
				WG67Subs::Send_WG67_current_notify(wg76sub);
			}
		}
	}
}

/**
 * GetWG67SubscriptionList: Envia el notify del evento WG67KEY-IN a todas las subscripciones activas en un account
 * @param	acc_id. Account. Si es -1 entonces es la default
 * @param   WG67Subscriptions. Returna un puntero a un array de elementos del tipo CORESIP_WG67_Subscription_Info. Si es NULL entonces no hay subscripciones
 * @param	nSubscriptions. Retorna el número de subscripciones.
 */
int SubscriptionServer::GetWG67SubscriptionList(pjsua_acc_id acc_id, int *nSubscriptions, CORESIP_WG67_Subscription_Info **WG67Subscriptions)
{
	int ret = CORESIP_OK;

	*nSubscriptions = 0;

	pjsua_acc_id accid;
	if (acc_id == -1)
	{
		accid = pjsua_acc_get_default();
	}
	else
	{
		accid = acc_id;
	}

	if (!pjsua_acc_is_valid(accid))
	{
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: GetWG67SubscriptionList invalid account"));
	}

	AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(accid);
	if (accUserData == NULL) return ret;

	std::list<pjsip_evsub*, std::allocator<pjsip_evsub*>> wg67_subs_list;

	//Busca las subscripciones que envian los Notify. No lo enviamos dentro del bucle ya que la funcion Send_WG67_current_notify puede modificar la lista _subModlist
	pj_mutex_lock(accUserData->subsServer->_subModlist_mutex);
	if (accUserData->subsServer->_subModlist.size() > 0)
	{
		std::list<pjsip_evsub*, std::allocator<pjsip_evsub*>>::iterator it;
		for (it = accUserData->subsServer->_subModlist.begin(); it != accUserData->subsServer->_subModlist.end(); it++)
		{
			pjsip_evsub* wg76sub = *it;
			if (wg76sub)
			{
				pjsip_event_hdr* eventhdr = (pjsip_event_hdr*)pjsip_evsub_get_event_hdr(wg76sub);
				if (eventhdr)
				{
					pj_str_t STR_SG67 = { "WG67KEY-IN", 10 };
					if (pj_strcmp(&eventhdr->event_type, &STR_SG67) == 0)
					{
						SubscriptionServer::subs_user_data* sub_user_data = (SubscriptionServer::subs_user_data*)pjsip_evsub_get_user_data(wg76sub);
						if (sub_user_data != NULL)
						{
							wg67_subs_list.push_back(wg76sub);
						}
					}
				}
			}
		}
	}

	size_t n_subscriptions = wg67_subs_list.size();
	if (accUserData->subsServer->pWG67Subscriptions_array != NULL)
	{
		free(accUserData->subsServer->pWG67Subscriptions_array);
		accUserData->subsServer->pWG67Subscriptions_array = NULL;
	}

	if (n_subscriptions > 0)
	{
		accUserData->subsServer->pWG67Subscriptions_array = (CORESIP_WG67_Subscription_Info*)malloc(n_subscriptions * sizeof(CORESIP_WG67_Subscription_Info));
		if (accUserData->subsServer->pWG67Subscriptions_array != NULL)
		{
			std::list<pjsip_evsub*, std::allocator<pjsip_evsub*>>::iterator it;
			unsigned int psubsciption_array_index = 0;
			for (it = wg67_subs_list.begin(); it != wg67_subs_list.end(); it++)
			{
				pjsip_evsub* wg76sub = *it;
				if (wg76sub)
				{
					SubscriptionServer::subs_user_data* sub_user_data = (SubscriptionServer::subs_user_data*)pjsip_evsub_get_user_data(wg76sub);
					memcpy(&accUserData->subsServer->pWG67Subscriptions_array[psubsciption_array_index], &sub_user_data->last_info, sizeof(CORESIP_WG67_Subscription_Info));
					psubsciption_array_index++;
				}
			}
		}
	}

	pj_mutex_unlock(accUserData->subsServer->_subModlist_mutex);

	*WG67Subscriptions = accUserData->subsServer->pWG67Subscriptions_array;
	*nSubscriptions = n_subscriptions;

	return ret;
}













