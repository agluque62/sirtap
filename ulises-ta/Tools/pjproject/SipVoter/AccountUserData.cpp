/**
 * @file AccountUserData.cpp
 *
 * Define la clase del objeto que se asigna como user data a un account id
 */
#include "AccountUserData.h"

AccountUserData::AccountUserData()
{
	Tj1 = 80;	//Por defecto 10 ms
	Ts1 = 0;
	Ts2 = 160;	//Por defecto 20 ms
	RdFr[0] = '\0';
	rdAccount = PJ_FALSE;
	TipoGrsFlags = CORESIP_CALL_NINGUNO;	
	pj_memset(PTTIDs, 0, sizeof(PTTIDs));

	preferred_grs_bss_method[0] = '\0';
	preferred_grs_bss_method_code = 4;

	forced_grs_ptt_id = -1;
	SelCalSupported = 1;

	no_enviar_WG67_Notify_si_todas_las_sesiones_son_ficticias = ENVIAR_WG67_NOTIFY_SI_TODAS_LAS_SESIONES_SON_FICTICIAS;

	subsServer = new SubscriptionServer();
	if (subsServer == NULL)
	{
		throw PJLibException(__FILE__, PJ_ENOMEM).Msg("AccountUserData: No se puede crear SubscriptionServer");
	}

	pj_timer_entry_init(&SendWG67NotifyFromAcc_delayed_timer, 0, NULL, SendWG67NotifyFromAcc_delayed_timer_cb);
}

AccountUserData::~AccountUserData()
{
	pjsua_cancel_timer(&SendWG67NotifyFromAcc_delayed_timer);
	SendWG67NotifyFromAcc_delayed_timer.id = 0;
	if (subsServer != NULL)
	{
		delete subsServer;
	}
}

/**
 * AllocPTTID: Reserva un PTT-id para el account
 * @param	ED137version. Vale 'B' para ED137B o 'C' para ED137C
 * @return	0 si no hay error. Y si no retorna el PTT-id asignado
 */
unsigned int AccountUserData::AllocPTTID(char ED137version)
{
	if (forced_grs_ptt_id >= 0)
	{
		return forced_grs_ptt_id;
	}
	unsigned int max_ptt_id = MAX_PTTID_ED137B;					//En ED137B se pueden asignar valores entre 1 y 63
	if (ED137version == 'C') max_ptt_id = MAX_PTTID_ED137C;		//En ED137C se pueden asignar valores entre 1 y 59
	for (unsigned int i = 1; i < (max_ptt_id+1); i++)
	{
		if (PTTIDs[i] == PJ_FALSE)
		{
			PTTIDs[i] = PJ_TRUE;
			return i;
		}
	}

	//Si no hay ptt-id libres retorna 0
	return 0;
}

/**
 * FreePTTID: Libera un PTT id
 * @param	pttid. PTT -id a liberar
 * @return	0 si no hay error
 */
unsigned int AccountUserData::FreePTTID(unsigned int pttid)
{
	if (forced_grs_ptt_id >= 0)
	{
		//Si el ptt id es forzado no hacemos nada
		return 0;
	}

	if (pttid > 0 && pttid < sizeof(PTTIDs))
	{
		PTTIDs[pttid] = PJ_FALSE;
		//Retorna el mismo ptt-id si es valido
		return pttid;
	}
	//Si el ptt-id no es valido entonces retorna 0
	return 0;
}

void AccountUserData::SendWG67NotifyFromAcc_delayed_timer_cb(pj_timer_heap_t* th, pj_timer_entry* te)
{
	AccountUserData* accUserData = (AccountUserData*)te->user_data;
	if (accUserData == NULL) return;

	pjsua_cancel_timer(&accUserData->SendWG67NotifyFromAcc_delayed_timer);
	accUserData->SendWG67NotifyFromAcc_delayed_timer.id = 0;

	SubscriptionServer::SendWG67NotifyFromAcc(accUserData->accid, PJ_TRUE, NULL, NULL, PJ_FALSE, accUserData->no_enviar_WG67_Notify_si_todas_las_sesiones_son_ficticias);
}

