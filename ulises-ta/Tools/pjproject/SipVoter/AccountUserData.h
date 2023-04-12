/**
 * @file AccountUserData.h
 *
 * Define la clase del objeto que se asigna como user data a un account id
 */
#ifndef __ACCOUNTUSERDATA_H__
#define __ACCOUNTUSERDATA_H__

#include "Global.h"
#include "exceptions.h"
#include <list>
#include "SubscriptionServer.h"

class AccountUserData
{
private:
		

public:
	static const unsigned int MAX_PTTID_ED137B = 63;
	static const unsigned int MAX_PTTID_ED137C = 59;
	static const pj_bool_t ENVIAR_WG67_NOTIFY_SI_TODAS_LAS_SESIONES_SON_FICTICIAS = PJ_FALSE;
	static const pj_bool_t NO_ENVIAR_WG67_NOTIFY_SI_TODAS_LAS_SESIONES_SON_FICTICIAS = PJ_TRUE;
	pjsua_acc_id accid;
	pj_bool_t rdAccount;				//Indica si es un account tipo radio. Se pone a true cuando es llamado por el ETM 
										//con la funcion CORESIP_SetTipoGRS.
										//Tambien lo pondremos a true si el agente el del tipo Radio_UA
	CORESIP_CallFlagsMask TipoGrsFlags;

	pj_bool_t PTTIDs[MAX_PTTID_ED137B+1];	//Como radio GRS, son los PTT-ID que ha sido ya asignados. Son validos desde  1 a 63, segun la ED137B.
											//Sin un PTT-id ha sido asignado entonces su valor se pone a true

	SubscriptionServer* subsServer;			//Servidor de subscripciones a este account

	pj_uint16_t Tj1;					//Jitter buffer delay en unidades de 125us
	pj_uint16_t Ts1;					//GRS System Delay. En unidades de 125 us
	pj_uint16_t Ts2;					//GRS system RX delay (including packetizing time). En unidades de 125 us

	char RdFr[CORESIP_MAX_RS_LENGTH + 1];	//Identificador de frecuencia para cuando es GRS 

	char preferred_grs_bss_method[CORESIP_MAX_BSS_LENGTH + 1];		//Como GRS receptor/transceptor, es el metodo BSS preferido
	unsigned int preferred_grs_bss_method_code;		//Si #preferred_grs_bss_method no es RSSI, AGC, C/N ni PSD, este sera el valor del codigo del
													//Vendor specific method. Debera ser entre 4 y 7.
	int forced_grs_ptt_id;	//Si el valor es negativo, entonces como GRS asigna el ptt-id automaticamente, 
							//Si es mayor o igual que cero, es el valor que se fuerza cuando se establezca una sesion.
	int SelCalSupported;	//Si el valor es 0, no soporta SELCAL. Si es distinto de cero si.

	pj_bool_t no_enviar_WG67_Notify_si_todas_las_sesiones_son_ficticias;
	pj_timer_entry SendWG67NotifyFromAcc_delayed_timer;
	static void SendWG67NotifyFromAcc_delayed_timer_cb(pj_timer_heap_t* th, pj_timer_entry* te);

	AccountUserData();
	~AccountUserData();
	
	unsigned int AllocPTTID(char ED137version);
	unsigned int FreePTTID(unsigned int i);
};

#endif