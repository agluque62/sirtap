#ifndef __CORESIP_WG67SUBS_H__
#define __CORESIP_WG67SUBS_H__

/*
En este modulo se implementa la clase que gestina una subscripcion al evento de WG67KEY-IN
*/

#include "Global.h"
#include "exceptions.h"

typedef void* (*WG67Subs_callback)(void* wg67subs);

class WG67Subs
{
public:

	static int SendWG67Subscription(pjsua_acc_id acc_id, int call_id, char* dest_uri, int expires, pj_bool_t noRefresh, pj_bool_t by_proxy);
	static int SetWG67SubscriptionParameters(pjsua_acc_id acc_id, int call_id, char* dest_uri, int noRefresh);

	WG67Subs(pjsua_acc_id acc_id, pjsua_call_id call_id, char* dst_uri, int expires, pj_bool_t noRefresh, pj_bool_t by_proxy);
	~WG67Subs();
	int Init(WG67Subs_callback confsub_cb, void* subsmanager);
	int End();
	int InicializarSubscripcion();
	int SendSubscriptionMessage(int expires, pj_bool_t noRefresh);

	char _Dst_uri[CORESIP_MAX_URI_LENGTH];		//Uri del destino al que enviamos la subscripcion al evento
	pjsip_evsub* _Module;
	pjsua_acc_id _Acc_id;					//Account del agente				
	pjsua_call_id _Call_id;					//Call id de la llamada, si queremos que la subscripcion se haga bajo el dialogo de una llamada
	void* _SubsManager;		//Puntero del objeto ClientSubscriptionManager que administra este tipo de subscripciones.
	pj_bool_t _NoRefresh;	//Si es true entonces los subscribes no se refrescan automaticamente
	int _Expires;			//Ultimo expires enviado
	char ED137Version;		//Vale 'B' para ED137B y 'C' para ED137C

	/*Funciones referentes a cuando funciona como servidor de subscripcion*/
	static pjsip_evsub_user _WG67SrvCb;
	static void OnWG67SrvStateChanged(pjsip_evsub* sub, pjsip_event* event);
	static void OnWG67SrvTsxChanged(pjsip_evsub* sub, pjsip_transaction* tsx, pjsip_event* event);
	static void OnWG67SrvTimeout(pjsip_evsub* sub);
	static void OnWG67RxRefresh(pjsip_evsub* sub, pjsip_rx_data* rdata, int* p_st_code,
		pj_str_t** p_st_text, pjsip_hdr* res_hdr, pjsip_msg_body** p_body);
	static pj_bool_t SubscriptionRxRequest(pjsip_rx_data* rdata, pjsip_dialog* dlg);
	static void Send_WG67_current_notify(pjsip_evsub* sub);
	static int Set_WG67_Notifier_Parameters(int notify_enabled, int manual, int minimum_expires, int maximum_expires);
	static int Set_WG67_notify_status(pjsua_acc_id acc_id, char* subscriberUri, CORESIP_WG67Notify_SubscriptionState_Config* subsState,
		CORESIP_WG67Notify_Body_Config* wG67Notify_Body);
	
private:

	static pjsip_evsub_user wg67_callback;
	pj_pool_t* _Pool;
	pj_str_t _Dst_uri_pj;	
	WG67Subs_callback _wg67subs_cb;
	pj_sem_t* status_sem;
	pj_bool_t _ByProxy;						//Indica si se envia el subscribe por el proxy
	pjsip_evsub_state last_subscription_state;

	pj_timer_entry	 after_no_refresh_force_destroy_subs_timer;	    /**< Cuando venza el timer la subscripcion sera terminada		*/
	static void force_destroy_subs_timer_cb(pj_timer_heap_t* th, pj_timer_entry* entry);

	static void wg67_on_tsx_state(pjsip_evsub* sub, pjsip_transaction* tsx, pjsip_event* event);
	static void wg67_on_state(pjsip_evsub* sub, pjsip_event* event);
	static void wg67_on_rx_notify(pjsip_evsub* sub, pjsip_rx_data* rdata, int* p_st_code, pj_str_t** p_st_text, pjsip_hdr* res_hdr, pjsip_msg_body** p_body);
	static void wg67_on_client_refresh(pjsip_evsub* sub);
	static void CallSubscribeCallback(CORESIP_WG67_Subscription_Info* last_info, CORESIP_WG67_Subscription_Info* current_info);
};

#endif
