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
	REC_ERROR_INI_SESSION
} REC_RESPONSE;

typedef enum recSessionStatus
{
	RECPORT_SESSION_IDLE,
	RECPORT_SESSION_OPEN,
	RECPORT_SESSION_CLOSED,
	RECPORT_SESSION_ERROR
} RECSESSIONSTATUS;

class RecordPort
{
public:
	pjsua_conf_port_id Slot;	

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

	char _RecursoTipoTerminal[256];

	//Contiene los Slots de telefon�a o radio que est�n conectados a trav�s de la conferencia pjsua
	//a los SndPorts (Puertos de sonido altavoces, cascos).
	//Ser�n los que se conecten al RecordPort para la grabaci�n VoIP. 
	pjsua_conf_port_id SlotsToSndPorts[CORESIP_MAX_SLOTSTOSNDPORTS];		
	pj_mutex_t *record_mutex;																	

public:
	RecordPort(int resType, const char * AddrIpToBind, const char * RecIp, unsigned recPort, const char *TerminalId);	
	~RecordPort(void);
	int RecSession(bool on, bool wait_end);	
	int RecCallStart(int dir, CORESIP_Priority priority, const pj_str_t *ori_uri, const pj_str_t *dest_uri);
	int RecCallEnd(int cause, int disc_origin);
	int RecCallConnected(const pj_str_t *connected_uri);
	int RecHold(bool on, bool llamante);
	int RecPTT(bool on, const char *freq);
	int RecPTT(bool on, const char *freq, int dev);
	int RecSQU(bool on, const char *freq);
	int RecSQU(bool on, const char *freq, int dev);	
	void Add_SlotsToSndPorts(pjsua_conf_port_id slot);
	void Del_SlotsToSndPorts(pjsua_conf_port_id slot);
	bool IsSlotConnectedToRecord(pjsua_conf_port_id slot);
	
private:
	//static const unsigned int SLEEP_FOR_SIMU = 700;   //Retardo para que el simulador de grabacion no pierda mensajes
	static const unsigned int SLEEP_FOR_SIMU = 0;   //Retardo para que el simulador de grabacion no pierda mensajes
	static const int MAX_NUM_FREQUENCIES = 64;
	static const int MAX_FREQ_LITERAL = 32;
	static const int MAX_PTT_DEVICES = 2;

	int Resource_type;
	RECSESSIONSTATUS SessStatus;
	int recording_by_rad;		//Si su valor es mayor que cero entonces se est� grabando radio
	int recording_by_tel;		//Si su valor es mayor que cero entonces se est� grabando telefonia

	struct {
		char freq_literal[MAX_FREQ_LITERAL];
		bool ptt[MAX_PTT_DEVICES];		//Indica si est� activado PTT para cada device (Instructor / Alumno)
		bool squ;						//Indica si est� activado el squelch
	} frequencies[MAX_NUM_FREQUENCIES];
		
	//Comandos grabador
	static const char *NOTIF_IPADD;
	static const char *INI_SES_REC_TERM;
	static const char *FIN_SES_REC_TERM;
	static const char *INI_SES_REC_RAD;
	static const char *FIN_SES_REC_RAD;
	static const char *SQUELCH_ON;
	static const char *SQUELCH_OFF;
	static const char *REC_CALLSTART;
	static const char *REC_CALLEND;
	static const char *REC_CALLCONNECTED;
	static const char *REC_PTTON;
	static const char *REC_PTTOFF;
	static const char *REC_HOLDON;
	static const char *REC_HOLDOFF;
	static const char *REC_RECORD;
	static const char *REC_PAUSE;
	static const char *REC_NOT_MESSAGE;

	//Respuestas del grabador
	static const char *RESPOK;
	static const char *BADDIRIP;
	static const char *NOT_ACTIVE_SESSION;
	static const char *OVRFLOW;
	static const char *ERROR_INI_SESSION;

	pj_pool_t * _Pool;
	pjmedia_port _Port;
	pj_sock_t _Sock;
	pj_lock_t * _Lock;

	char RecTerminalIpAdd[32];
	pj_sockaddr_in recAddr;			//Direcci�n y puerto del grabador
		
	char mess_media[1024];
	pj_uint32_t nsec_media;	

	pj_sem_t *sem;
	pj_mutex_t *mutex;
	static const int MAX_MSG_LEN = 7;
	char message_received[MAX_MSG_LEN+1];

	pj_thread_t  *actions_thread;
	static pj_thread_proc RecordActionsTh;
	pj_sem_t *actions_sem;
	pj_mutex_t *actions_mutex;
	PJ_ATOMIC_VALUE_TYPE actions_thread_run;

	pj_time_val t_last_command;		//Tiempo en el que se envi� el ultimo comando

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
#endif

private:
	void Dispose();
	static pj_status_t PutFrame(pjmedia_port * port, const pjmedia_frame *frame);
	static pj_status_t Reset(pjmedia_port * port);
	REC_RESPONSE ReadResp();
	REC_RESPONSE SendCommand(char *mess, pj_ssize_t *len, const pj_sockaddr_t *to, int tolen);
	int Add_Rec_Command_Queue(char *mess, size_t messlen, COMMAND_QUEUE *Command_queue);
	int NotifIp();
	int ConnectRx(bool on);
	static void send_command_timer_cb(pj_timer_heap_t *th, pj_timer_entry *te);
	static void update_session_timer_cb(pj_timer_heap_t *th, pj_timer_entry *te);
	pj_status_t StartSessionTimer(long seconds);
	void CancelSessionTimer();
	pj_status_t RefressSessionTimer(long seconds);
	int SetFreq(const char *freq_lit, int dev, bool ptt, bool change_ptt, bool squ, bool change_squ);
	int GetFreq(const char *freq_lit, bool *ptt, bool *squ);
	int SetFreqPtt(const char *freq_lit, int dev, bool ptt, bool *status_changed);
	int SetFreqSqu(const char *freq_lit, bool squ, bool *status_changed);
	void GetNumSquPtt(int *nPtt, int *nSqu);	
	void GetTelNum(char *uri, int uri_len, char *tel, int tel_len);
	int Record(bool on);

	pj_activesock_t * _RemoteSock;
	static pj_bool_t OnDataReceived(pj_activesock_t * asock, void * data, pj_size_t size, const pj_sockaddr_t *src_addr, int addr_len, pj_status_t status);

};


#endif

/*@}*/

