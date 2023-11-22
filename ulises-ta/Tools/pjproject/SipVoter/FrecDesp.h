/**
 * @file FrecDesp.h
 * @brief Gestion de Frecuencias Desplazadas en CORESIP.dll
 *
 *	Implementa la clase 'FrecDesp'.
 *
 *	@addtogroup CORESIP
 */
/*@{*/

#ifndef __CORESIP_FRECDESP_H__
#define __CORESIP_FRECDESP_H__

#include "CoreSip.h"
#include "SipCall.h"

class FrecDesp
{
public:

	static const int INVALID_GROUP_INDEX = -1;
	static const int INVALID_SESS_INDEX = -1;
	static const int MAX_GROUPS = 1024;   //maximo numero de grupos
	static const int MAX_SESSIONS = 20;	 //maximo numero de sesiones en un grupo

	static const pj_bool_t IN_WINDOW = PJ_TRUE;	 //Indica que se está en la ventana de decision bss
	static const pj_bool_t ONLY_SELECTED_IN_WINDOW = PJ_TRUE;	 //Indica que solo se selecciona la que fue mejor en la ventana de decision

	pj_bool_t NTP_synchronized;			
	
	FrecDesp();
	~FrecDesp();
	
	int AddToGroup(char *rdfr, pjsua_call_id call_id, SipCall *psipcall, char *Bss_selected_method, CORESIP_CallFlagsMask flags);
	int RemFromGroup(int index_group, int index_sess);
	int SetGroupClimaxFlag(int index_group, pj_bool_t Esgrupoclimax);
	int GetGroupClimaxFlag(pjsua_call_id call_id, pj_bool_t *Esgrupoclimax);
	int GetSessionsCountInGroup(int index_group, int *nsessions_rx_only, int *nsessions_tx_only,
		pj_bool_t* every_receiver_and_transceiver_supports_ED137C);
	int SetTimeDelay(int index_group, int index_sess,
		pj_uint32_t Tn1, pj_uint32_t Tj1, pj_uint32_t Tid, pj_uint32_t Tsd, pj_uint32_t Ts2, pj_uint32_t Tn2,
		CORESIP_CLD_CALCULATE_METHOD used_climax_method);
	int CalcTimeDelay(pjmedia_stream* stream, pj_uint8_t* ext_value, pj_uint32_t length,
		unsigned int& Tn1_ms, unsigned int& Tj1_ms, unsigned int& Tid_ms, unsigned int& Tsd_ms, unsigned int& Ts2_ms, unsigned int& Tn2_ms,
		pj_uint32_t& Tn1, pj_uint32_t& Tj1, pj_uint32_t& Tid, pj_uint32_t& Tsd, pj_uint32_t& Ts2, pj_uint32_t& Tn2,
		CORESIP_CLD_CALCULATE_METHOD requested_climax_method, CORESIP_CLD_CALCULATE_METHOD& used_climax_method,
		pj_bool_t* request_MAM);
	pj_uint32_t GetRxDelayToApply(int index_group, int index_sess);
	int GetCLD(pjsua_call_id call_id, pj_uint8_t *cld);
	int SetQidxMethod(int index_group, int index_sess, pj_uint8_t qidx_method);
	int SetAirQidxInWindows(int index_group, int index_sess);
	int GetAirQidxInWindows(int index_group, int index_sess);
	void Clear_SyncBss_In_Group(int index_group);
	void GetQidx(int index_group, int index_sess, pj_uint8_t *qidx, pj_uint8_t *qidx_ml, pj_uint32_t *Tred);
	int SetSquSt(int index_group, int index_sess, pj_bool_t squ_st, int *sq_air_count);
	int ClrAllSquSt(int index_group);
	int GetSquCnt(int index_group);
	int SetBetterSession(SipCall *p_current_sipcall, pj_bool_t in_window, pj_bool_t only_selected_in_window);
	int EnableMulticast(SipCall *psipcall, pj_bool_t enable, pj_bool_t all);
	int SetSelected(SipCall *psipcall, pj_bool_t selected, pj_bool_t all);
	int SetSelectedUri(SipCall *psipcall);
	SipCall* GetSelected(int index_group);
	int ClrSelectedUri(SipCall *psipcall);
	int Set_mcast_seq(int index_group, unsigned mcast_seq);
	unsigned Get_mcast_seq(int index_group);
	int Set_group_multicast_socket(int index_group, pj_sockaddr_in *RdSendTo);
	int Get_group_multicast_socket(int index_group, pj_sockaddr_in **RdSendTo);
	pj_bool_t IsBssSelected(SipCall *psipcall);
	pj_bool_t IsValidTdTxIP(SipCall *psipcall);
	int GetLastCld(int index_group, int index_sess, pj_uint8_t *cld);
	int RefressStatus(SipCall *p_current_sipcall);
	int SetInWindow(int index_group, SipCall* sipcall_in_window_timer);
	SipCall* GetInWindow(int index_group);
	int GetSelectedUri(SipCall *psipcall, char **selectedUri, unsigned short *selectedUriPttId);
	int GetClimaxMethod(int index_group, int index_sess, CORESIP_CLD_CALCULATE_METHOD& climax_method);

	static int GetRMMData(pj_uint8_t* ext_value, pj_uint32_t* TQV, pj_uint32_t* T1);
		
private:

	static const unsigned int PERIOD_CHECK_NTP = 3000;	
	static const pj_uint32_t Tv1 = 0;
	static const pj_uint32_t Tp1 = 160;		//20 ms en unidades de 125us
	static const float OFFSET_THRESHOLD;	//Umbral de valided del Offset del ntp en milisegundos
	static const pj_uint32_t ui32_OFFSET_THRESHOLD_us = 2000;		//Umbral de valided del Offset del ntp en microsegundos
	static const pj_uint32_t INVALID_TIME_DELAY = 0xFFFFFFFF;
	static const unsigned long long INVALID_TIME_SQU = 0;
	static const pj_uint32_t INVALID_TIME_FSQU = 0xFFFFFFFF;
	static const pj_uint32_t INVALID_CLD_PREV = 0xFFFFFFFF;
	static const int Tn1_count_MAX = 1;
	static const int Tj1_count_MAX = 1;		

	pj_pool_t * _Pool;
	pj_bool_t ntp_check_thread_run;

	pj_thread_t  *ntp_check_thread;
	static pj_thread_proc NTPCheckTh;

	pj_mutex_t *fd_mutex;

	struct stgrupo
	{
		char RdFr[CORESIP_MAX_RS_LENGTH + 1];			//Frecuencia que identifica al grupo.	
		char Zona[CORESIP_MAX_ZONA_LENGTH + 1];			//Frecuencia que identifica al grupo.
		int nsessions;									//Cantidad total de sesiones
		int nsessions_tx_only;							//Cantidad de sesiones Tx only
		int nsessions_rx_only;							//Cantidad de sesiones Rx only
		pj_bool_t every_receiver_and_transceiver_supports_ED137C;
														//Indica si todos transceptores y receptores en el grupo soportan ED137C

		struct stsess
		{
			pjsua_call_id sess_callid;		//call ids de las sesiones que est�n dentro del grupo.
			SipCall *pSipcall;				//Puntero al objeto del tipo SipCall que maneja la sesion
			pj_uint32_t TdTxIP;			    //Time delay calculado para la radio de cada sesion, despues de recibir MAM
											//En unidades de 125 us
			pj_uint32_t Tj1;				//Ultimo Tj1 obtenido
			int Tj1_count;					//Cantidad de veces que el retardo es distinto al anterior
			pj_uint32_t cld_prev;			//Ultimo cld obtenido. En 32 bits
			pj_bool_t cld_absoluto;			//Indica si el metodo CLD es el absoluto o no
			int Tn1_count;					//Cantidad de veces que el retardo es distinto al anterior
			pj_uint32_t Tn1;				//Ultimo retardo de red obtenido Tn1. En unidades de 125 us
			pj_uint32_t Tid_orig;			//Tid obtenido en el primer MAM despues de que la sesion ha sido establecida o cuando NMR se recibe
			pj_uint32_t Tid;				//Ultimo Tid obtenido en el MAM
			pj_uint32_t Tsd;				//Ultimo Tsd obtenido en el MAM			
			pj_uint32_t Tn2;				//Ultimo Tn2 calculado. Retardo de red de recepcion	
			pj_uint32_t Ts2;				//Ultimo Ts2 obtenido en el MAM	
			pj_uint8_t Bss_type;			//Tipo del bss recibido
			pj_bool_t squ_status;			//Estado del squelch. 
			pj_bool_t selected;				//Indica si es el seleccionado	
			char Bss_selected_method[64];	//Metodo bss seleccionado por el GRS. Es el literal recibido en el sdp		
			CORESIP_CLD_CALCULATE_METHOD _MetodoClimax;
			CORESIP_CallFlagsMask CallFlags;			
			int PesoRSSIvsNucleo;			//Peso del valor de Qidx del tipo RSSI en el calculo del Qidx final. 0 indica que el calculo es interno (centralizado). 9 que el calculo es solo el RSSI.
			int AirQidxInWindows;	//Valor del Qidx al final de la ventana de decision cuando el squelch no es de avion, que sirve para crear la lista de Qidx del grupo
			
		} sessions[MAX_SESSIONS];

		char SelectedUri[CORESIP_MAX_URI_LENGTH + 1];   //Uri del receptor seleccionado en la ventana BSS
		unsigned short SelectedUriPttId;				//PTT-Id de la uri seleccionada
		unsigned mcast_seq;					//N�mero de secuencia que se env�a con el paquete de audio por multicast
		pj_sockaddr_in *_RdSendTo;			//Direcci�n y puerto multicast donde se env�a. Lo asigna la sesi�n que primero
											//active el squelch
		SipCall* sipcall_in_window_timer;	//Si es distinto de NULL es la llamada que arranca la ventana de decision, si es NULL, entonces no hay ventana decision en curso

	} groups[MAX_GROUPS];

	int ngroups;
		
	int UpdateGroupClimaxParams(int index_group);
	int UpdateGroupClimaxParamsAllGroups();
	int GetNextLineField(int pos, char *line, char *out, int out_size);

	pj_sock_t ntp_sd;						//Socket descriptor para conectarse a ntpd       
	pj_fd_set_t ntp_fds;					
	int ConnectNTPd(char *err, int err_size);	
	int DisConnectNTPd(char *err, int err_size);
	int NtpStat(char *err, int err_size);
};


#endif
