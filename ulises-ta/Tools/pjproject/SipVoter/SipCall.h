#ifndef __CORESIP_CALL_H__
#define __CORESIP_CALL_H__

#include <map>
#include "processor.h"
#include "IIR_FILT.h"
#include "SignalGen.h"
#include "NoiseGenerator.h"

enum bss_method_types
{
	NINGUNO,
	RSSI,
	RSSI_NUC,
	CENTRAL
};

class SipCall
{
	friend class SDPUtils;
public:	
	SipCall(const CORESIP_CallInfo * info, const CORESIP_CallOutInfo * outInfo);
	SipCall(pjsua_call_id call_id, const CORESIP_CallInfo * info);
	~SipCall();

	pj_pool_t* _Pool;
	char ED137Version;						//Vale 'B' para ED137B y 'C' para ED137C
	char DstUri[CORESIP_MAX_URI_LENGTH + 1];
	char IncommingReqUri[CORESIP_MAX_URI_LENGTH + 1];		//Uri del Request Line entrante
	CORESIP_CallInfo _Info;	
	CORESIP_CallFlagsMask Callflag_previous_reinvite;
	pj_bool_t SelcalSupported;					//Si vale distinto de cero, la llamada soporta SELCAL
	char _RdFr[CORESIP_MAX_RS_LENGTH + 1];					//Identificador de la frecuencia. Con valor "000.000" no se envia el fid. Debe estar terminado cn el caracter '\0'
	char _IdDestino[CORESIP_MAX_RS_LENGTH + 1];				//Identificador del destino de radio. Si esta campo tiene una longitud mayor que cero
															//entonces, el identificador para agrupar las radios es este.
															//Si no, entonces el identificador para agrupar es RdFr
	bss_method_types bss_method_type;
	int _Index_group;						//Indice que identifica el grupo al que pertenece esta llamada
	int _Index_sess;						//Indice que identifica la sesion dentro del grupo
	pj_timer_entry window_timer;			//Timer para la ventana de decisi�n del BSS
	pj_bool_t _Sending_Multicast_enabled;	//Indica si puede enviar audio por multicast a los puestos
	CORESIP_RdInfo RdInfo_prev;
	pj_mutex_t *RdInfo_prev_mutex;
	pj_sem_t* waiting_MAM_sem;

	pj_timer_entry Ptt_off_timer;			//Timer que arranca cuando en un receptor o transceptor hay Ptt off. 
											//Durante ese tiempo no se envia rdinfo a la aplicacion por la callback
	static void Ptt_off_timer_cb(pj_timer_heap_t *th, pj_timer_entry *te);
											//Callback del timer	

	int last_received_squ_status;						//Ultimo estado del squelch recibido en OnRdInfoChanged
	CORESIP_PttType last_received_ptt_status;			//Ultimo estado de ptt recibido en OnRdInfoChanged
	pj_bool_t radio_session_with_pttid_enabled;
	unsigned int assigned_pttid;			//Ptt-id asignado para esta sesion. En caso de una radio GRS es el allocado y enviado en el SDP, en caso de VCS es el recibido.
											//Si es 0 entonces es porque la sesion no requiere Ptt-id

	pj_bool_t _ConfirmPtt;		//Indica si se confirma el PTT cuando se es un agente de radio

	unsigned _R2SKeepAlivePeriod;
	unsigned _R2SKeepAliveMultiplier;

	std::map <int, tone_info *> Playing_tones_map;		//Map con las frecuencias de tono que se estan enviando en la llamada. key es la frecuencia.
	NoiseGenerator *Noise_generator;					//Generador de ruido asociado a la llamada.
	DTMFgen* DTMF_generator;							//Generador de digitos multifrecuencia a la llamada. 

	//Parametros para cuando es un agente simulador de radio
	char etm_grs_bss_method_selected[CORESIP_MAX_BSS_LENGTH + 1];	//Para ETM, como GRS receptor/transceptor, Es el metodo BSS seleccionado para enviar el Qidx. 
																	//Si su longitud es cero entonces no hay envio de Qidx
	pj_bool_t remote_grs_supports_ED137C_Selcal;		//es true si el grs remoto soporta selcal de ED137C.

	CORESIP_CallFlagsMask CallFlags_prev_reinvite;		//Valor de los flags previamente a enviar un reinvite.
		
	int GetSyncBss();
	void SetSyncBss(int qidx);
	void InitSyncBss();

	CORESIP_CallInfo *GetCORESIP_CallInfo();
	int Hacer_la_llamada_saliente();
	
	static int New(const CORESIP_CallInfo * info, const CORESIP_CallOutInfo * outInfo);

	static void Hangup(pjsua_call_id call_id, unsigned code);
	static int Force_Hangup(pjsua_call_id call_id, unsigned code);
	static void Answer(pjsua_call_id call_id, unsigned code, bool addToConference, unsigned reason_cause, char* reason_text);
	static void MovedTemporallyAnswer(pjsua_call_id call_id, const char *dst_uri, const char *reason);
	static void CallProccessRedirect(pjsua_call_id call_id, const char* dstUri, CORESIP_REDIRECT_OP op);
	static void Hold(pjsua_call_id call_id, bool hold);
	static void Reinvite(pjsua_call_id call_id, int CallType_SDP, int TxRx_SDP, char* etm_bss_methods, CORESIP_SDPSendRecvAttrForced ForceSDPSendRecvAttr);
	static void Transfer(pjsua_call_id call_id, pjsua_call_id dst_call_id, const char * dst, const char *display_name);	
	static void Conference(pjsua_call_id call_id, bool conf);
	static void SendConfInfo(pjsua_call_id call_id, const CORESIP_ConfInfo * info);
	static void SendInfoMsg(pjsua_call_id call_id, const char * info);	
	static void GetNetworkDelay(pjsua_call_id call_id, unsigned int *delay_ms);	
	static void Wg67VersionSet(pjsip_tx_data* txdata, pj_str_t* valor);
	static void SendSELCAL(pjsua_call_id call_id, const char* selcalValue);
	static void GetRdQidx(int call, int* Qidx);

	static pj_bool_t CouplingSessionInAccount(pjsua_call_id call_id);
	static void SetCallParameters(pjsua_call_id call_id, int* disableKeepAlives, int* forced_cld);

	static void TransferAnswer(const char * tsxKey, void * txData, void * evSub, unsigned code);
	static void TransferNotify(void * evSub, unsigned code);		

	static void Ptt(pjsua_call_id call_id, const CORESIP_PttInfo* info);
	static void Ptt(pjsua_call_id call_id, const CORESIP_PttInfo* info, unsigned int delay_ms);	
	static void GRS_Force_Ptt_Mute(int call_id, CORESIP_PttType PttType, unsigned short PttId, bool on);
	static void GRS_Force_Ptt(int call_id, CORESIP_PttType PttType, unsigned short PttId);
	static void GRS_Force_SCT(int call_id, bool on);
	static void Force_PTTS(int call_id, bool on);
	static void OnRdRtp(void* stream, void* frame, void* codec, unsigned seq, pj_uint32_t rtp_ext_info);
	static void OnRdInfoChanged(void* stream, void* ext_type_length, pj_uint32_t rtp_ext_info,
		const void* p_rtp_ext_info, pj_uint32_t rtp_ext_length, pj_bool_t tx_pttmute_changed, pj_bool_t GRS_force_ptt_mute_in_RTPRx_changed);
	static void window_timer_cb(pj_timer_heap_t* th, pj_timer_entry* te);
	static void OnKaTimeout(void* stream);
	static void SetConfirmPtt(pjsua_call_id call_id, pj_bool_t val);
	static int HangUpAllCallsOfAcc(pjsua_acc_id pj_acc_id, pj_bool_t exclude_NoFreqDisconn);

	static void OnStateChanged(pjsua_call_id call_id, pjsip_event * e);
	static pjsip_redirect_op On_call_redirected(pjsua_call_id call_id, const pjsip_uri* target, const pjsip_event* e);
	static void OnIncommingCall(pjsua_acc_id acc_id, pjsua_call_id call_id, pjsua_call_id replace_call_id, pjsip_rx_data * rdata);
	static void OnMediaStateChanged(pjsua_call_id call_id, int *returned_code);
	static void OnReinviteReceived(pjsua_call_id call_id, const pjsip_event* e, pjsip_tx_data* tdata, int* returned_code, pj_str_t** cause);
	static void OnTransferRequest(pjsua_call_id call_id, 
		const pj_str_t * refer_to, const pj_str_t * refer_by, 
		const pj_str_t * tsxKey, pjsip_tx_data * tdata, pjsip_evsub * sub);
	static void OnTransferStatusChanged(pjsua_call_id based_call_id, int st_code, const pj_str_t *st_text, pj_bool_t final, pj_bool_t *p_cont);
	static void OnTsxStateChanged(pjsua_call_id call_id, pjsip_transaction *tsx, pjsip_event *e);
	static void OnStreamCreated(pjsua_call_id call_id, pjmedia_session * sess, unsigned stream_idx, pjmedia_port **p_port);
	static void OnStreamDestroyed(pjsua_call_id call_id, pjmedia_session *sess, unsigned stream_idx);
	static pj_status_t OnSendACK(pjsua_call_id call_id, pjsip_inv_session* inv, pjsip_rx_data* rdata, pjsip_event* e);
	static pj_bool_t OnTxRequest(pjsip_tx_data *txdata);

	//ETM
	static void SetImpairments(pjsua_call_id call_id, CORESIP_Impairments *impcfg);	

	/** ED137 */
	static pj_str_t gWG67VersionName;
	static pj_str_t gWG67VersionRadioValue;
	static pj_str_t gWG67VersionTelefValue;
	static pj_str_t gWG67VersionRadioValueC;
	static pj_str_t gWG67VersionTelefValueC;	
	static pj_str_t gWG67VersionRadioValueBC;
	static pj_str_t gWG67VersionTelefValueBC;
	static pj_str_t gWG67VersionTelefIAValueBC;
		
	static const int WAIT_INIT_TIME_seg = 4;
	static const int WAIT_INIT_TIME_ms = 500;

	static const unsigned AUDIO_PACKET = 0;
	static const unsigned RESET_PACKET = 1;

	static const int MAX_BSS_SQU = 200;		//maxima cantidad de valores BSS almacenados en bss_rx desde que se activa un squelch
		
private:	
	static const char Selcal_designators[];
	static const unsigned int MIN_porcentajeRSSI = 0;	//Minimo valor del Peso del valor de Qidx del tipo RSSI en el calculo del Qidx final.
	static const unsigned int MAX_porcentajeRSSI = 100;	//Maximo valor del Peso del valor de Qidx del tipo RSSI en el calculo del Qidx final.
	static const pj_uint32_t MAX_RSSI_ESCALE = 15;
	static const pj_uint32_t MAX_CENTRAL_ESCALE = 50;
	static const pj_uint32_t MAX_QIDX_ESCALE = 31;
	static const pj_uint8_t RSSI_METHOD = 0;
	static const pj_uint8_t NUC_METHOD = 0x4;
	//Valor maximo de seq que se recibe en la callback OnRdRTP
	//seq es igual al Timestamp del paquete RTP dividido por SAMPLES_PER_FRAME (160)
	//El valor mas alto es 0xFFFFFFFF/SAMPLES_PER_FRAME. Es decir, el valor de 32 bits mas alto dividido por el numero de muestras por frame
	//Lo dividimos por 2 porque el codec g711 proporciona 2 paquetes por cada 20ms
	static const pj_uint32_t MAX_RTP_SEQ = 0xFFFFFFFF / (SAMPLES_PER_FRAME / 2);

	static const int SQU_OFF = 0;
	static const int SQU_ON = 1;

	static int SipCallCount;
	
	struct {
		pjsip_generic_string_hdr subject, priority, referBy, require, replaces, wg67ver;
		pjsua_acc_id acc_id;
		char dst_uri[512];
		unsigned options;
		char repl[512];
		pjsua_msg_data msg_data;
	} make_call_params;	
		
	pj_bool_t valid_sess;					//Indica si la sesion es valida para poder enviar los RMM en el timer Check_CLD_timer
	int _Id;
	pjsip_dialog* _Dlg;						//Dialogo de la sesion
	pjsua_acc_id _Acc_id;	
	pj_sock_t _RdSendSock;
	pj_sockaddr_in _RdSendTo;		
	pj_uint32_t Retardo;					//Retardo en n�mero de muestras (125us)
	pj_mutex_t *circ_buff_mutex;
	pjmedia_circ_buf *p_retbuff;			//Buffer circular para implementar retardo	
	unsigned int _Payload_type;				//Tipo de payload. (pcma, pcmu, etc);
											//Los valores pueden ser los del tipo pjmedia_rtp_pt			
	
	pj_bool_t squ_event;					//Indica un evento de squelch on.
	pj_bool_t squ_event_mcast;				//Se activa con el primer squelch de un grupo
											//el numero de secuencia que se envia con los paquetes de audio por multicast	

	pj_bool_t primer_paquete_despues_squelch;
	pj_bool_t primer_paquete_despues_ptt;	//Cuando somos un agente radio es true si se estaba esperando un primer paquete despues de recibir el ptt

	pj_bool_t squoff_event_mcast;
	unsigned waited_rtp_seq;				//Numero de secuencia esperado por rtp desde la radio
	pj_bool_t hay_retardo;					//Indica si hay retardo despu�s de squelch	
	char zerobuf[256];

	int bss_rx[MAX_BSS_SQU];				//Almacena todos los valores de BSS recibidos desde que se activa el squelch
	int index_bss_rx_w;						//Indice de escritura en bss_rx
	pj_mutex_t *bss_rx_mutex;

	pj_uint16_t GRS_Td1;					//Is the CLIMAX Time Delay received. Tid = Td1 + Ts1 que se envía en el MAM. En unidades de 125us.
	
	pj_sem_t *sem_out_circbuff;
	pj_thread_t  *out_circbuff_thread;
	pj_bool_t out_circbuff_thread_run;
	static pj_thread_proc Out_circbuff_Th;
	pj_bool_t wait_sem_out_circbuff;		//Indica si tiene que esperar el sem_out_circbuff

	static const int Check_CLD_timer_IDLE = 0;
	static const int Check_CLD_timer_SEND_CLD = 1;
	static const int Check_CLD_timer_SEND_RMM = 2;
	pj_timer_entry Check_CLD_timer;			//Timer para supervisi�n CLD
	static void Check_CLD_timer_cb(pj_timer_heap_t *th, pj_timer_entry *te);
											//Callback del timer

	pj_timer_entry Wait_init_timer;			//Timer que arranca cuando se inicia la sesi�n. Durante ese tiempo se ignora el audio
											//que se recibe de las radios. 
	static void Wait_init_timer_cb(pj_timer_heap_t *th, pj_timer_entry *te);
											//Callback del timer

	pj_timer_entry Wait_fin_timer;			//Timer que arranca cuando se ha finalizado la sesion sip. Al vencer el timer es
											//cuando se hace el delete del objeto sipcall
	static void Wait_fin_timer_cb(pj_timer_heap_t *th, pj_timer_entry *te);
											//Callback del timer

	pj_timer_entry PTT_delayed_timer;		//Timer para retrasar el PTT
	static void PTT_delayed_timer_cb(pj_timer_heap_t* th, pj_timer_entry* te);
	CORESIP_PttInfo ptt_delayed_info;

	pj_timer_entry SELCAL_timer;			//Timer para durante el cual se esta transmitiendo SELCAL
	static void SELCAL_timer_cb(pj_timer_heap_t* th, pj_timer_entry* te);

	pj_uint32_t last_rtp_ext_info;			//Ultimo rtp_ext_info con ptt y squ status que ha sido enviado

	/*** Necesarios para el calculo de Qidx ****/
	pj_uint8_t last_qidx_value;
	processor_data PdataQidx;				//Datos para el proceso de calculo del QiDx.
	float fPdataQidx[SAMPLES_PER_FRAME*2];
	float afMuestrasIfRx[SAMPLES_PER_FRAME*2];
	float a_dc[2];
	float b_dc[2];
	float fFiltroDC_IfRx_ciX;
	float fFiltroDC_IfRx_ciY;
	/***********/

	pjsip_evsub * _ConfInfoClientEvSub;
	pjsip_evsub * _ConfInfoSrvEvSub;
	bool _HangUp;

	char LastReason[CORESIP_MAX_REASON_LENGTH + 1];		//Tiene el Reason recibido en el BYE o en el CANCEL

#ifdef _ED137_
	// PlugTest FAA 05/2011
	pj_str_t _Frequency;
	static struct CORESIP_EstablishedOvrCallMembers _EstablishedOvrCallMembers;
	static char _LocalUri[CORESIP_MAX_URI_LENGTH + 1];
	static char _CallSrcUri[CORESIP_MAX_URI_LENGTH + 1];
#endif

	static pjsip_evsub_user _ConfInfoSrvCb;
	static pjsip_evsub_user _ConfInfoClientCb;	

private:
	static pj_bool_t ProcessReceivedSelcalInfo(pjsua_call_id call_id, pjsip_transaction* tsx, pjsip_event* e);
	static pj_bool_t ProcessReceivedGenericInfo(pjsua_call_id call_id, pjsip_transaction* tsx, pjsip_event* e);
	void CheckParams();
	void SubscribeToConfInfo();	
	static pjmedia_clock_callback Multicast_clock_cb;

	static void OnConfInfoSrvStateChanged(pjsip_evsub *sub, pjsip_event *event);
	static void OnConfInfoClientStateChanged(pjsip_evsub *sub, pjsip_event *event);
	static void OnConfInfoClientRxNotify(pjsip_evsub *sub, pjsip_rx_data *rdata, int *p_st_code, 
		pj_str_t **p_st_text, pjsip_hdr *res_hdr, pjsip_msg_body **p_body);
		
	static void EliminarRadSessionDelGrupo(SipCall *call);
	static int FlushSessions(pj_str_t *dst, pjsua_call_id except_cid, CORESIP_CallType calltype);
	void IniciaFinSesion();	
	void Dispose();
};

#endif
