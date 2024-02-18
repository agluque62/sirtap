/**
 * @file RecordPort.cpp
 * @brief Gestion de un 'puerto' de grabacion en CORESIP.dll
 *
 *	Implementa la clase 'RecordPort'.
 *
 *	@addtogroup CORESIP
 */
/*@{*/

#ifndef __CORESIP_RECORDPORT_H__
#define __CORESIP_RECORDPORT_H__

/*Para depurar la grabación sin necesidad del servicio de grabación. Se escriben en el LOG los mensajes que se enviarian al 
servicio de grabacion. No se espera la respuesta*/
#undef DEBUG_GRABACION
//#define DEBUG_GRABACION			

/*Escribe en binario el audio que se grabaria*/
#undef REC_IN_FILE
//#define REC_IN_FILE 1 

typedef enum recResponses
{
	REC_OK_RESPONSE,
	REC_NO_RESPONSE,
	REC_BAD_RESPONSE,
	REC_IP_INCORRECT,
	REC_SESSION_CLOSED,
	REC_OVERFLOW,
	REC_SESSION_IS_ALREADY_CREATED,
	REC_ERROR_INI_SESSION,
	REC_COMMAND_NO_SUPPORTED
} REC_RESPONSE;

typedef enum recSessionStatus
{
	RECPORT_SESSION_IDLE,
	RECPORT_SESSION_IPSEND,
	RECPORT_SESSION_OPEN,
	RECPORT_SESSION_CLOSED,
	RECPORT_SESSION_REMOBJSENT,
	RECPORT_SESSION_ERROR
} RECSESSIONSTATUS;

class RecordPort
{
public:

	static RecordPort* _RecordPortTel;
	static RecordPort* _RecordPortRad;
	static RecordPort* _RecordPortIA;
	static RecordPort* _RecordPortTelSec;
	static RecordPort* _RecordPortRadSec;
	static RecordPort* _RecordPortIASec;

	pjsua_conf_port_id Slot;	

	//Puerto estatico por para comunicarse con el servicio de grabacion, por el que se reciben ordenes de actuacion sobre la coresip.
	static const pj_uint16_t ST_PORT = 65001;

	//Tipos de recursos
	static const int TEL_RESOURCE = 0;	//Telefonia
	static const int RAD_RESOURCE = 1;	//Radio

	//Direccion llamada
	static const int INCOM = 0;
	static const int OUTCOM = 1;

	//
	static const int MAX_REC_COMMANDS_QUEUE = 32;
	static const int MAX_COMMAND_LEN = 512;

	static const long NO_SESSION_TIMER = 15;	//15 sec. Tiempo de reinio de sesion si hay error en la comunicacion con el grabador

	static const int TRIES_SENDING_CMD = 3;

	typedef enum RECORDERS_TO_RECORD
	{
		BOTH_RECORDERS = (int)'0',		//Graba en los dos grabadores
		RECORDER1 = (int)'1',			//Graba en el 1
		RECORDER2 = (int)'2',			//Graba en el 2
		NONSECURE_RECORDER = RECORDER1,
		SECURE_RECORDER = RECORDER2		//Grabador seguro
	} RECORDERS_TO_RECORD;

	char _RecursoTipoTerminal[256];
		
	pj_mutex_t *record_mutex;																	

public:
	static void Init(pj_pool_t* pool, int eD137_record_enabled, CORESIP_Agent_Type agentType, const char* IpAddress, const char* HostId);
	static void End();
	RecordPort(int resType, const char * AddrIpToBind, const char * RecIp, unsigned recPort, 
		const char *TerminalId, const char* TerminalId_sufix, RECORDERS_TO_RECORD recorders_to_record);
	~RecordPort(void);
	int RecResetSession();	
	int RecSession(bool on, bool wait_end);	
	int RecRemoveRecObj();
	int RecINV();
	int RecBYE();
	int RecReset();
	int RecCallStart(int dir, CORESIP_Priority priority, const pj_str_t *ori_uri, const pj_str_t *dest_uri, const pj_str_t* callIdHdrVal);
	int RecCallEnd(int cause, pjsua_call_media_status media_status, int disc_origin, const pj_str_t* callIdHdrVal);
	int RecCallConnected(const pj_str_t *connected_uri, const pj_str_t* callIdHdrVal);
	int RecHold(bool on, bool llamante, pjsua_call_media_status media_status, const pj_str_t* callIdHdrVal);
	int RecPTT_send(bool on, const char *freq, CORESIP_PttType PTT_type, char* pttConnRef);
	int RecPTT(bool on, const char *freq, int dev, CORESIP_PttType PTT_type);
	int RecSQU_send(bool on, const char *freq, char* squConnRef);
	int RecSQU(bool on, const char *freq, const char *resourceId, const char *bssMethod, unsigned int bssQidx);		
	bool IsSlotConnectedToRecord(pjsua_conf_port_id slot);
	static int GetSndDevToRecord(int dev_in);
	static void GetSndTypeString(CORESIP_SndDevType type, char* SndDevType_returned, int size_SndDevType_returned);
	static RecordPort* GetRecordPortFromResource(const pj_str_t* uri, const pj_str_t* Id, CORESIP_Resource_Type type);
	
private:
	//static const unsigned int SLEEP_FOR_SIMU = 700;   //Retardo para que el simulador de grabacion no pierda mensajes
	static const unsigned int SLEEP_FOR_SIMU = 0;   //Retardo para que el simulador de grabacion no pierda mensajes
	static const int MAX_NUM_FREQUENCIES = 64;
	static const int MAX_FREQ_LITERAL = 32;
	static const int MAX_BSSMETHOD_LITERAL = 32;
	static const int MAX_RESOURCEID_LITERAL = 32;
	static const int MAX_CONNREF_LEN = 32;

	int Resource_type;
	RECSESSIONSTATUS SessStatus;
	int recording_by_rad;		//Si su valor es mayor que cero entonces se está grabando radio
	int recording_by_tel;		//Si su valor es mayor que cero entonces se está grabando telefonia

	struct {
		char freq_literal[MAX_FREQ_LITERAL];
		bool ptt;						//Indica si está activado el PTT 
		bool squ;						//Indica si está activado el squelch
		char resourceId[MAX_RESOURCEID_LITERAL]; //Identificador del recurso seleccionado en el bss
		char bssMethod[MAX_BSSMETHOD_LITERAL];
		char pttConnref[MAX_CONNREF_LEN+1];
		char squConnref[MAX_CONNREF_LEN+1];
		unsigned int bssQidx;

	} frequencies[MAX_NUM_FREQUENCIES];
	pj_mutex_t *frequencies_mutex;
		
	//Comandos grabador
	static const char *NOTIF_IPADD;
	static const char *INI_SES_REC_TERM;
	static const char *FIN_SES_REC_TERM;
	static const char *INI_SES_REC_RAD;
	static const char *FIN_SES_REC_RAD;
	static const char *REMOVE_REC_OBJ;		//Borrado del objeto de grabación
	static const char *REC_INV;
	static const char *REC_BYE;
	static const char *SQUELCH_ON;
	static const char *SQUELCH_OFF;
	static const char *REC_BSS;
	static const char *REC_CALLSTART;
	static const char *REC_CALLEND;
	static const char *REC_CALLCONNECTED;
	static const char *REC_PTTON;
	static const char *REC_PTTOFF;
	static const char *REC_HOLDON;
	static const char *REC_HOLDOFF;
	static const char *REC_RECORD;
	static const char *REC_PAUSE;
	static const char *REC_RESET;
	static const char* REC_MEDIA;
	static const char *REC_NOT_MESSAGE;

	//Respuestas del grabador
	static const char *RESPOK;
	static const char *BADDIRIP;
	static const char *NOT_ACTIVE_SESSION;
	static const char *OVRFLOW;
	static const char *COMMAND_ERROR;
	static const char *SESSION_IS_CREATED;
	static const char *ERROR_INI_SESSION;
	static const char *RECORD_SRV_REINI;

	static CORESIP_Agent_Type AgentType;
	static pj_bool_t ED137_record_enabled;

	static pj_sock_t _SockSt;

	pj_pool_t * _Pool;
	pjmedia_port _Port;	
	pj_sock_t _Sock;
	pj_lock_t * _Lock;

	RECORDERS_TO_RECORD RecordersToRecord;	

	char RecTerminalIpAdd[32];
	pj_sockaddr_in recAddr;			//Dirección y puerto del grabador
		
	char mess_media[1024];
	pj_uint32_t nsec_media;	

	pj_sem_t *sem;
	pj_mutex_t *mutex;
	static const int MAX_MSG_LEN = 7;
	char message_received[MAX_MSG_LEN+1];

	long Wait_response_timeout_sec;				//Tiempo máximo de espera a las respuestas del servicio de grabacion (seg)
	pj_thread_t  *actions_thread;
	static pj_thread_proc RecordActionsTh;
	pj_sem_t *actions_sem;
	pj_mutex_t *actions_mutex;
	PJ_ATOMIC_VALUE_TYPE actions_thread_run;

	pj_time_val t_last_command;		//Tiempo en el que se envió el ultimo comando

	pj_timer_entry send_command_timer;
	int COM_TIM_ID;

	pj_thread_t  *session_thread;
	static pj_thread_proc SessionControlTh;
	pj_event_t *ctrlSessEvent;
	PJ_ATOMIC_VALUE_TYPE sessionControlTh_run;

	pj_timer_entry update_session_timer;
	int SES_TIM_ID;
	static int timer_id;
		
	typedef struct {
		struct {
			char buf[MAX_COMMAND_LEN];
			pj_ssize_t len;
		} queue[MAX_REC_COMMANDS_QUEUE];
		unsigned int iRecw;
		unsigned int iRecr;
		unsigned int RecComHoles;
	} COMMAND_QUEUE;	
	
	COMMAND_QUEUE Rec_Command_queue;	//Cola de mensajes de acciones sobre el grabador, excepto RECORD y PAUSE
	COMMAND_QUEUE Rec_RecPau_queue;		//Cola de mensajes RECORD y PAUSE. tienen prioridad sobre Rec_Command_queue	
	

#ifdef REC_IN_FILE
	pj_oshandle_t sim_rec_fd;
	pj_oshandle_t sim_rec_tx_fd;
#endif

private:
	void Dispose();
	void RecordsRecovering();
	static pj_status_t PutFrame(pjmedia_port * port, const pjmedia_frame *frame);
	static pj_status_t Reset(pjmedia_port * port);
	REC_RESPONSE ReadResp();
	REC_RESPONSE SendCommand(char *mess, pj_ssize_t *len, const pj_sockaddr_t *to, int tolen, pj_bool_t espera_respuesta);
	int Add_Rec_Command_Queue(char *mess, size_t messlen, COMMAND_QUEUE *Command_queue);
	int NotifIp(bool wait_end);
	static void send_command_timer_cb(pj_timer_heap_t *th, pj_timer_entry *te);
	static void update_session_timer_cb(pj_timer_heap_t *th, pj_timer_entry *te);
	pj_status_t StartSessionTimer(long seconds);
	void CancelSessionTimer();	
	int SetFreq(const char *freq_lit, bool ptt, bool squ, const char *resourceId, const char *bssMethod, unsigned int bssQidx);
	int GetFreq(const char *freq_lit, bool *ptt, bool *squ, char *resourceId, char *bssMethod, unsigned int *bssQidx, char* pttConnRef, char* squConnRef);
	int SetFreqPtt(const char *freq_lit, bool ptt, bool *status_changed, char* pttConnRef, char* squConnRef);
	int SetFreqSqu(const char *freq_lit, bool squ, bool *status_changed, 
		const char *resourceId, const char *bssMethod, unsigned int bssQidx, bool *status_bss_changed,
		char* pttConnRef, char* squConnRef);
	void GetNumSquPtt(int *nPtt, int *nSqu);	
	void GetTelNum(char *uri, pj_ssize_t uri_len, char *tel, pj_ssize_t tel_len);
	int Record(bool on, char* connRef);
	int RecBSS_send(const char *freq, const char *selected_resource, const char *bss_method, unsigned int qidx);

	pj_activesock_t * _RemoteSock;
	static pj_bool_t OnDataReceived(pj_activesock_t * asock, void * data, pj_size_t size, const pj_sockaddr_t *src_addr, int addr_len, pj_status_t status);
	
	static pj_activesock_t* _RemoteStSock;
	static pj_bool_t OnDataReceivedSt(pj_activesock_t * asock, void * data, pj_size_t size, const pj_sockaddr_t *src_addr, int addr_len, pj_status_t status);
};


#endif

/*@}*/

