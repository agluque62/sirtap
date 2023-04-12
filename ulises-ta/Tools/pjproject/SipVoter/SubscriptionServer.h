#ifndef __SUBSCRIPTIONSERVER_H__
#define __SUBSCRIPTIONSERVER_H__

/*
Esta clase define un servidor de subscripcion a eventos
*/

#include "Global.h"
#include "exceptions.h"
#include <list>


class SubscriptionServer
{
private:

	struct from_user_subs
	{
		char user[CORESIP_MAX_USER_ID_LENGTH];
		char host[CORESIP_MAX_URI_LENGTH];
		char tag[CORESIP_MAX_TAG_LENGTH];
	};

	pj_pool_t* _Pool;
	pjsip_evsub* _ConfSrvEvSub;		//Modulo para la la subscripcion al evento de converencia fuera de un dialogo INV
	std::list<pjsip_evsub*, std::allocator<pjsip_evsub*>> _subModlist;
									//Contiene los punteros a los modulos de subscripcion 
									//correspondiente a un account del agente

	std::list<from_user_subs*, std::allocator<from_user_subs*>> _DeletedsubModlist;
									//Contiene los punteros a los usuarios incluyendo su tag que han solicitado 
									//subscripcion y han sido borrados,
									//correspondientes a un account del agente. 
									//Sirve para cuando se recibe tarde un reintento de peticion de subscripcion 
									//con el mismo from pero que ya ha sido borrado de la lista _subModlist
	static const size_t MAX_DeletedsubModlist_size = 64;
									//Maxima cantidad de elementos de la lista _DeletedsubModlist. 
									//Si se alcanza esta cantidad entonces se elimina el elemento mas antiguo

	pj_mutex_t* _subModlist_mutex;

	CORESIP_WG67_Subscription_Info* pWG67Subscriptions_array;	//Tiene todas las subscripciones que se le pasan a la aplicacion en la funcion GetWG67SubscriptionList

public:

	const static int ABSOLUTE_MINIMUM_EXPIRES_AS_UAS = 0;
	const static int ABSOLUTE_MAXIMUM_EXPIRES_AS_UAS = 3600;

	//Estructura para el user_data de un modulo de subscripcion
	struct subs_user_data
	{
		pjsua_acc_id accid;			//Account del agente sobre el que se realiza la subscripcion
		pj_bool_t send_notify_at_tsx_terminate;
									//Indica que cuando ha terminado la transaccion del metodo subscribe envie el notify		
		CORESIP_WG67_Subscription_Info last_info;
		CORESIP_WG67Notify_SubscriptionState_Config subsState;
		CORESIP_WG67Notify_Body_Config wG67Notify_Body_Config;
									//Es el body que se envia en los Notify de las subscripciones al evento WG67KEY-IN.
		pj_bool_t in_WG67SubscriptionReceivedCb;		//Si es true indica que se esta en la callback WG67SubscriptionReceivedCb
	};

	pj_bool_t _Notify_enabled;		//Si true entonces el Notify está permitido.
	pj_bool_t _Manual_WG67_Notify;	//Si el valor es true entonces los Notify de las subscripciones al evento WG67KEY-IN no se envian automaticamente.
	int _Minimum_expires_As_UAS;	//Tiempo inimo de expires soportado. el subscriptor envia un valor menor al valor de este
									//parametro entonces se rechaza y hay que enviar la cabacera Min-expires con este valor
	int _Maximum_expires_As_UAS;	//Tiempo maximo de expires soportado. Si el subscriptor envia un valor mayor al valor de este
									//parametro entonces en el 200 OK hay que enviar este valor.
	
	SubscriptionServer();
	~SubscriptionServer();

	static int Add_subMod(pjsip_evsub* conf);
	static int Del_subMod(pjsip_evsub* conf);
	static pjsip_evsub* Get_subMod(pjsua_acc_id accid, pjsip_uri* remote_uri, pj_str_t* event_type);
	static pjsip_evsub* Get_subMod(pjsua_acc_id accid, pjsip_dialog* dlg, pj_str_t* event_type);
	static int Add_DeletedsubModlist(pjsua_acc_id accid, pjsip_uri* remote_uri, pj_str_t* remote_from_tag);
	static pj_bool_t IsInDeletedList(pjsua_acc_id accid, pjsip_uri* remote_uri, pj_str_t* remote_from_tag);
	static void TerminateSubscriptions(pj_str_t* event_type);
	static void SendConfInfoFromAcc(pjsua_acc_id accid, const CORESIP_ConfInfo* conf);
	static void SendDialogNotifyFromAcc(pjsua_call_id call_id, pj_bool_t with_body);
	static void SendWG67NotifyFromAcc(pjsua_call_id call_id, pj_bool_t delayed, pj_bool_t no_enviar_WG67_Notify_si_todas_las_sesiones_son_ficticias);
	static void SendWG67NotifyFromAcc(pjsua_acc_id acc_id, pj_bool_t only_actives,
		CORESIP_WG67Notify_SubscriptionState_Config* subsState, CORESIP_WG67Notify_Body_Config* wG67Notify_Body,
		pj_bool_t only_ED137C, pj_bool_t no_enviar_WG67_Notify_si_todas_las_sesiones_son_ficticias);
	static int GetWG67SubscriptionList(pjsua_acc_id acc_id, int* nSubscriptions, CORESIP_WG67_Subscription_Info* WG67Subscriptions[]);
};

#endif
