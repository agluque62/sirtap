/**
 * @file CoreSip.h
 * @brief Define la interfaz pública de la librería CORESIP.dll
 *
 *	Contiene las definiciones y prototipos de acceso a la libreria 'coresip'
 *	para interfasarse a una tajeta de sonido.
 *
 *	@addtogroup CORESIP
 */
#ifndef __CORESIP_H__
#define __CORESIP_H__

#define ETM5

#define TRACE_ALL_CALLS (SipAgent::ETM)

#if defined(_WIN32) && defined(_USRDLL)
#	ifdef CORESIP_EXPORTS
#		define CORESIP_API __declspec(dllexport)
#	else
#		define CORESIP_API __declspec(dllimport)
#	endif
#else
#	define CORESIP_API
#endif

typedef int		pj_bool_t;

#define CORESIP_OK								0
#define CORESIP_ERROR							1

#define CORESIP_MAX_USER_ID_LENGTH			100
#define CORESIP_MAX_FILE_PATH_LENGTH		256
#define CORESIP_MAX_ERROR_INFO_LENGTH		512
#define CORESIP_MAX_HOSTID_LENGTH			32
#define CORESIP_MAX_IP_LENGTH				25
#define CORESIP_MAX_URI_LENGTH				256
#define CORESIP_MAX_TAG_LENGTH				256
#define CORESIP_MAX_SOUND_DEVICES			20
#define CORESIP_MAX_WAV_PLAYERS				50
#define CORESIP_MAX_WAV_RECORDERS			50
#define CORESIP_MAX_RDRX_PORTS				128
#define CORESIP_MAX_SOUND_RX_PORTS			128
#define CORESIP_MAX_GENERIC_PORTS			16
#define CORESIP_MAX_RTP_PORTS				16
#define CORESIP_MAX_RS_LENGTH				128
#define CORESIP_MAX_REASON_LENGTH			128
#define CORESIP_MAX_WG67_SUBSCRIBERS		25
#define CORESIP_MAX_WG67_NOTIFY_SESSIONS	7
#define CORESIP_MAX_CODEC_LENGTH			50
#define CORESIP_MAX_CONF_USERS				25
#define CORESIP_MAX_CONF_STATE_LENGTH		25
#define CORESIP_MAX_ZONA_LENGTH				128
#define CORESIP_MAX_BSS_LENGTH				32
#define CORESIP_MAX_SUPPORTED_LENGTH		512
#define CORESIP_MAX_NAME_LENGTH	         	20
#define CORESIP_MAX_CALLID_LENGTH			256
#define CORESIP_MAX_RSSI_QIDX				15			//Valor maximo de QIDX RSSI
#define CORESIP_MAX_QIDX					31			//Valor maximo de other QIDX 
#define CORESIP_MAX_ATTENUATION_DB			100
#define CORESIP_MAX_BODY_LEN				1024
#define CORESIP_MAX_SESSTYPE_LENGTH			32
#define CORESIP_MAX_SELCAL_LENGTH			4
#define CORESIP_MAX_SOUND_NAME_LENGTH		512
#define CORESIP_MAX_SOUND_NAMES				16
#define CORESIP_MAX_DTMF_DIGITS				128
#define CORESIP_AUD_PACKET_LEN				160
#define CORESIP_MAX_RESOURCEID_LENGTH		256
#define CORESIP_MAX_HMI_RESOURCES			1024

#ifdef _ED137_
// PlugTest FAA 05/2011
// Recording
#define CORESIP_MAX_FRECUENCY_LENGTH		CORESIP_MAX_RS_LENGTH + 1
#define CORESIP_TIME_LENGTH					28
#define CORESIP_CALLREF_LENGTH				50
#define CORESIP_CONNREF_LENGTH				50
// OVR Calls
#define CORESIP_MAX_OVR_CALLS_MEMBERS		10
#endif

typedef enum CORESIP_CallType
{
	CORESIP_CALL_IA,
	CORESIP_CALL_MONITORING,
	CORESIP_CALL_GG_MONITORING,
	CORESIP_CALL_AG_MONITORING,
	CORESIP_CALL_DIA,
	CORESIP_CALL_RD,
#ifdef _ED137_
	CORESIP_CALL_RRC,
	CORESIP_CALL_OVR,
	CORESIP_CALL_RECORDING,
	CORESIP_CALL_RXTXRD,
#endif
	CORESIP_CALL_UNKNOWN

} CORESIP_CallType;

/*

CORESIP_CALL_PORD_IDLE,
CORESIP_CALL_PORD_RXONLY,

UNIFETM: Estos valores los quitamos y utilizamos los de los flags

*/


typedef enum CORESIP_Priority
{
	CORESIP_PR_EMERGENCY,
	CORESIP_PR_URGENT,
	CORESIP_PR_NORMAL,
	CORESIP_PR_NONURGENT,
	CORESIP_PR_UNKNOWN

} CORESIP_Priority;


typedef enum CORESIP_CallFlags
{
	CORESIP_CALL_CONF_FOCUS = 0x1,
	CORESIP_CALL_RD_COUPLING = 0x2,			//Tipo de sesion Radio Cou<fpling.	Type: Couplig
	CORESIP_CALL_RD_RXONLY = 0x4,			//Como VCS	txrxmode=Rx. Como GRS indica que es del tipo receptor
	CORESIP_CALL_RD_TXONLY = 0x8,			//Como VCS	txrxmode=Tx. Como GRS indica que es del tipo transmisor
	CORESIP_CALL_EC = 0x10,
	CORESIP_CALL_EXTERNA = 0x20,
	CORESIP_CALL_RD_IDLE = 0x40,			//Tipo de sesion Radio IDLE			Type: Radio-idle
	CORESIP_CALL_RD_RADIO_RXONLY = 0x80,	//Tipo de sesion Radio-Rxonly		Type: Radio-rxonly

	CORESIP_CALL_NO_TXRXMODE = 0x100,		//Sin txrxmode. 
											//Como VCS, si este flag es activado, CORESIP_CALL_RD_RXONLY y CORESIP_CALL_RD_TXONLY no pueden estar activados.											

	CORESIP_CALL_NINGUNO = 0x0				//Type: Radio-TxRx txrxmode=TxRx
	
} CORESIP_CallFlags;

typedef unsigned int CORESIP_CallFlagsMask;			//Sera una mascara compuesta de CORESIP_CallFlags


typedef enum CORESIP_CallState
{
	CORESIP_CALL_STATE_NULL,				/**< Before INVITE is sent or received  */
	CORESIP_CALL_STATE_CALLING,				/**< After INVITE is sent		    */
	CORESIP_CALL_STATE_INCOMING,			/**< After INVITE is received.	    */
	CORESIP_CALL_STATE_EARLY,				/**< After response with To tag.	    */
	CORESIP_CALL_STATE_CONNECTING,			/**< After 2xx is sent/received.	    */
	CORESIP_CALL_STATE_CONFIRMED,			/**< After ACK is sent/received.	    */
	CORESIP_CALL_STATE_DISCONNECTED,		/**< Session is terminated.		    */

#ifdef ETM5
	CORESIP_CALL_STATE_STATE_DESCONOCIDO,	/** < Estado DESCONOCIDO **/
	CORESIP_CALL_STATE_DISCONNECTED2		/**< Session LC (Tx) is terminated.		*/
#endif

} CORESIP_CallState;


typedef enum CORESIP_CallRole
{
	CORESIP_CALL_ROLE_UAC,
	CORESIP_CALL_ROLE_UCS

} CORESIP_CallRole;

typedef enum CORESIP_SDPSendRecvAttrForced
{
	CORESIP_SDP_SR_ATTR_NOFORCED,		//Este valor indica que no se fuerza un nuevo valor del atributo
	CORESIP_SDP_SR_ATTR_NONE,			//Con este valor no se pone el atributo en el SDP
	CORESIP_SDP_SR_ATTR_INACTIVE,
	CORESIP_SDP_SR_ATTR_SENDONLY,
	CORESIP_SDP_SR_ATTR_RECVONLY,
	CORESIP_SDP_SR_ATTR_SENDRECV	
} CORESIP_SDPSendRecvAttrForced;

typedef enum CORESIP_MediaStatus
{
	CORESIP_MEDIA_NONE,
	CORESIP_MEDIA_ACTIVE,
	CORESIP_MEDIA_LOCAL_HOLD,
	CORESIP_MEDIA_REMOTE_HOLD,
	CORESIP_MEDIA_ERROR

} CORESIP_MediaStatus;


typedef enum CORESIP_MediaDir
{
	CORESIP_DIR_NONE,
	CORESIP_DIR_SENDONLY,
	CORESIP_DIR_RECVONLY,
	CORESIP_DIR_SENDRECV
} CORESIP_MediaDir;


typedef enum CORESIP_PttType
{
	CORESIP_PTT_OFF,
	CORESIP_PTT_NORMAL,
	CORESIP_PTT_COUPLING,
	CORESIP_PTT_PRIORITY,
	CORESIP_PTT_EMERGENCY,
	CORESIP_PTT_TEST			//Solo valido para ED137C

} CORESIP_PttType;

typedef enum CORESIP_SndDevType
{
	CORESIP_SND_INSTRUCTOR_MHP,				//Tambien llamado ayudante. La aplicacion crea 2. El In es la entrada de microfono, Out es la salida del altavoz
	CORESIP_SND_ALUMN_MHP,					//Tambien llamado ejecutivo. La aplicacion crea 2. El In es la entrada de microfono, Out es la salida del altavoz
	CORESIP_SND_MAX_IN_DEVICES,
	CORESIP_SND_MAIN_SPEAKERS = CORESIP_SND_MAX_IN_DEVICES,
	CORESIP_SND_LC_SPEAKER,					//La aplicacion crea 2. In Es la linea de retorno del altavoz LC. Out es la salida
	CORESIP_SND_RD_SPEAKER,					//La aplicacion crea 2. In Es la linea de retorno del altavoz RD. Out es la salida
	CORESIP_SND_INSTRUCTOR_RECORDER,		//La aplicacion crea 2 (in o out). El In es el retorno del altavoz del instructor y el out es el del grabador analogico
	CORESIP_SND_ALUMN_RECORDER,				//La aplicacion crea 2 (in o out). El In es el retorno del altavoz del alumno y el Out es el del grabador analogico
	CORESIP_SND_RADIO_RECORDER,				//La aplicacion crea solo 1. Que es el Out al grabador analogico
	CORESIP_SND_LC_RECORDER,				//La aplicacion crea solo 1. Que es el Out al grabador analogico
	CORESIP_SND_HF_SPEAKER,					//La aplicacion crea 2. In Es la linea de retorno del altavoz HF. Out es la salida
	CORESIP_SND_HF_RECORDER,				//Es la linea Out hacia el grabador analogico
	CORESIP_SND_UNKNOWN

} CORESIP_SndDevType;

//Comandos para el grabador
typedef enum CORESIP_RecCmdType			
{	
	CORESIP_REC_RESET=0,		//Resetea el servicio de grabacion
	CORESIP_REC_ENABLE,		//Activa la grabacion
	CORESIP_REC_DISABLE		//Desactiva la grabacion
} CORESIP_RecCmdType;

typedef enum CORESIP_FREQUENCY_TYPE 
{ Simple = 0, Dual = 1, FD = 2, ME = 3 } 
CORESIP_FREQUENCY_TYPE;         // 0. Normal, 1: 1+1, 2: FD, 3: EM

typedef enum CORESIP_FREQUENCY_MODO_TRANSMISION {
	Climax = 0,
	UltimoReceptor = 1,
	Manual = 2,
	Ninguno = 3
} CORESIP_FREQUENCY_MODO_TRANSMISION;

typedef enum CORESIP_CLD_CALCULATE_METHOD 
{ Relative, Absolute } 
CORESIP_CLD_CALCULATE_METHOD;

typedef enum CORESIP_CFWR_OPT_TYPE
{
	CORESIP_CFWR_OPT_REQUEST,
	CORESIP_CFWR_OPT_RELEASE,
	CORESIP_CFWR_OPT_UPDATE
} CORESIP_CFWR_TYPE;

typedef enum CORESIP_REDIRECT_OP
{
	CORESIP_REDIRECT_REJECT,
	CORESIP_REDIRECT_ACCEPT
} CORESIP_REDIRECT_OP;

typedef enum CORESIP_RTP_port_actions
{
	CORESIP_CREATE_ENCODING,
	CORESIP_CREATE_DECODING,
	CORESIP_CREATE_ENCODING_DECODING,
	CORESIP_PAUSE_ENCODING,
	CORESIP_PAUSE_DECODING,
	CORESIP_PAUSE_ENCODING_DECODING,
	CORESIP_RESUME_ENCODING,
	CORESIP_RESUME_DECODING,
	CORESIP_RESUME_ENCODING_DECODING,
	CORESIP_STOP,
	CORESIP_DESTROY
} CORESIP_RTP_port_actions;

typedef enum CORESIP_Agent_Type
{
	ULISES = 0,
	SIRTAP_HMI = 1,
	SIRTAP_NBX = 2
} CORESIP_Agent_Type;

typedef struct CORESIP_Error
{
	int Code;
	char File[CORESIP_MAX_FILE_PATH_LENGTH + 1];
	char Info[CORESIP_MAX_ERROR_INFO_LENGTH + 1];

} CORESIP_Error;


typedef struct CORESIP_SndDeviceInfo
{
	CORESIP_SndDevType Type;

	int OsInDeviceIndex;
	int OsOutDeviceIndex;

} CORESIP_SndDeviceInfo;


typedef struct CORESIP_RdRxPortInfo
{
	unsigned ClkRate;
	unsigned ChannelCount;
	unsigned BitsPerSample;
	unsigned FrameTime;
	char Ip[CORESIP_MAX_IP_LENGTH + 1];
	unsigned Port;

} CORESIP_RdRxPortInfo;

typedef struct CORESIP_CallInfo
{
	int AccountId;
	CORESIP_CallType Type;						//Tipo de llamada
	CORESIP_Priority Priority;					//Prioridad. Se refiere a los valores soportados en la cabecera Priority
	CORESIP_CallFlagsMask CallFlags;		

	int SourcePort;								//UNIFETM: Este campo falta en ULISES. En el ETM no se utiliza. Se le asigna valor, pero para nada. Yo lo quitaria en ETM
	int DestinationPort;						//UNIFETM: Este campo falta en ULISES. Se asigna el valor en onIncomingCall. es un valor que se retorna en la callback. No es de entrada.
		

	/** */
    int PreferredCodec;							//UNIFETM: Este campo falta en ETM. Si no se quiere utilizar, asignar valor 0xFF en el ETM

	char Zona[CORESIP_MAX_ZONA_LENGTH + 1];		//UNIFETM: Este campo falta en ETM. Asignarle el valor 0 en ETM
    CORESIP_FREQUENCY_TYPE FrequencyType;		//UNIFETM: Este campo falta en ETM. Asignarle el valor Simple en ETM
	CORESIP_FREQUENCY_MODO_TRANSMISION ModoTransmision;
    CORESIP_CLD_CALCULATE_METHOD CLDCalculateMethod;	//UNIFETM: Este campo falta en ETM. Asignarle el valor Relative en ETM
    int BssWindows;								//UNIFETM: Este campo falta en ETM. Asignarle el valor 0 en ETM
    pj_bool_t AudioSync;						//UNIFETM: Este campo falta en ETM. Asignarle el valor 0 en ETM
    pj_bool_t AudioInBssWindow;					//UNIFETM: Este campo falta en ETM. Asignarle el valor 0 en ETM
	int cld_supervision_time;					//Tiempo de supervision CLD en segundos. Si el valor es 0 entonces no hay supervison de CLD. //UNIFETM: Este campo falta en ETM. Asignarle el valor 0 en ETM
	int forced_cld;								//Para ETM, es el CLD que se envia de forma forzada en ms. 
												//Si vale -1 entonces no se fuerza y se envia el CLD calculado o el Tn1 en el caso del ETM
	char bss_method[CORESIP_MAX_BSS_LENGTH + 1];					//Solo para VCS ULISES, es el metodo BSS. Para ETM deber ser un string de longitud cero
	char etm_vcs_bss_methods[CORESIP_MAX_BSS_LENGTH * 3 + 1];		//Para ETM, string con los literales de los metodos BSS separados por comas, enviados por VCS. En ulises string long 0
	unsigned int porcentajeRSSI;				//Peso del valor de Qidx del tipo RSSI en el calculo del Qidx final. 0 indica que el calculo es interno (centralizado). 9 que el calculo es solo el RSSI.  //UNIFETM: Este campo falta en ETM. Asignarle el valor 0 en ETM

	int R2SKeepAlivePeriod;						//Valor entre 20 y 1000 del periodo de los KeepAlives. Si el valor es -1, se ignora y se utilizará el valor por defecto (200)
	int R2SKeepAliveMultiplier;					//Valor entre 2 y 50 del numbero de R2S-Keepalive no recibidos antes producirse un Keep Alive time-out. Si el valor es -1, se ignora y se utilizará el valor por defecto (10).

	int NoFreqDisconn;							//Si vale distinto de cero para llamadas hacia GRS, indica que la sesion no se desconecte cuando se modifica 
												//el identificador de la frecuencia (Fid) en el GRS
												//Y se envia Notify al evento WG67 cuando se modifica le Fid. Solo es valido en ED137C
	
	CORESIP_SDPSendRecvAttrForced ForceSDPSendRecvAttr;	//Sirve para forzar el atributo send-recv en el SDP

#ifdef _ED137_	
	int Codecs;
	int BssMethods;
	char Frequency[CORESIP_MAX_FRECUENCY_LENGTH + 1];
#endif
} CORESIP_CallInfo;

typedef struct CORESIP_CallOutInfo
{
	char DstUri[CORESIP_MAX_URI_LENGTH + 1];

	char ReferBy[CORESIP_MAX_URI_LENGTH + 1];

	char RdFr[CORESIP_MAX_RS_LENGTH + 1];					//Con valor "000.000" no se envia el fid. Debe estar terminado cn el caracter '\0'
	char IdDestino[CORESIP_MAX_RS_LENGTH + 1];				//Identificador del destino de radio. Si esta campo tiene una longitud mayor que cero
															//entonces, el identificador para agrupar las radios es este.
															//Si no, entonces el identificador para agrupar es RdFr
															//Para el ETM este parametro tiene que tener longitud cero.
	char RdMcastAddr[CORESIP_MAX_IP_LENGTH + 1];
	unsigned RdMcastPort;

	//Referente al replaces. Esto no se necesita cuando el DstUri se obtiene de un REFER y ya tiene la info de replaces
	pj_bool_t RequireReplaces;								//Vale true si requiere replaces
	char CallIdToReplace[CORESIP_MAX_CALLID_LENGTH + 1];	//Call id de la llamada a reemplazar
	char ToTag[CORESIP_MAX_TAG_LENGTH + 1];					//Tag del To de la llamada a reemplazar
	char FromTag[CORESIP_MAX_TAG_LENGTH + 1];				//Tag del From de la llamada a reemplazar
	pj_bool_t EarlyOnly;									//Vale true si se requiere el parametro early-only en el replaces

} CORESIP_CallOutInfo;

//UNIFETM: en el ETM esta estructura tiene menos campos, Se utiliza solo para la callback. No habria que cambiar nada en el ETM
typedef struct CORESIP_CallInInfo
{
	char SrcId[CORESIP_MAX_USER_ID_LENGTH + 1];
	char SrcIp[CORESIP_MAX_IP_LENGTH + 1];
	unsigned SrcPort;
	char SrcSubId[CORESIP_MAX_USER_ID_LENGTH + 1];
	char SrcRs[CORESIP_MAX_RS_LENGTH + 1];
	char DstId[CORESIP_MAX_USER_ID_LENGTH + 1];
	char DstIp[CORESIP_MAX_IP_LENGTH + 1];
	char DstSubId[CORESIP_MAX_USER_ID_LENGTH + 1];
	char DisplayName[CORESIP_MAX_NAME_LENGTH + 1];
	char RdFr[CORESIP_MAX_RS_LENGTH + 1];					//Valor de fid de la llamada entrante hacia el GRS
	char etm_grs_bss_method_selected[CORESIP_MAX_BSS_LENGTH + 1];	//Para ETM, como GRS receptor/transceptor, Es el metodo BSS seleccionado para enviar el Qidx. 
																	//Si su longitud es cero entonces no hay envio de Qidx

} CORESIP_CallInInfo;

typedef struct CORESIP_CallTransferInfo
{
	void * TxData;
	void * EvSub;
	char TsxKey[CORESIP_MAX_RS_LENGTH + 1];

	char ReferBy[CORESIP_MAX_URI_LENGTH + 1];
	char ReferTo[2 * CORESIP_MAX_URI_LENGTH + 1];

	char DstId[CORESIP_MAX_USER_ID_LENGTH + 1];
	char DstIp[CORESIP_MAX_IP_LENGTH + 1];
	char DstSubId[CORESIP_MAX_USER_ID_LENGTH + 1];
	char DstRs[CORESIP_MAX_RS_LENGTH + 1];

} CORESIP_CallTransferInfo;


typedef struct CORESIP_CallStateInfo
{	
	CORESIP_CallState State;
	CORESIP_CallRole Role;
	unsigned int isRadReinvite;							//Si vale distinto de 0 indica que el estado ha isdo provocado por un reinvite
	unsigned int radReinvite_accepted;					//Este parametro solo se tiene en cuenta si isRadReinvite=1. si reinvite_accepted=1 entonces ha habido un reinvite aceptado
														//si reinvite_accepted=0 entonces ha habido un reinvite rechazado
	CORESIP_CallFlagsMask radRreinviteCallFlags;		//Este parametro solo se tiene en cuenta si isRadReinvite=1. Son los flags del re-invite.
															

	int LastCode;										// Util cuando State == PJSIP_INV_STATE_DISCONNECTED
	char LastReason[CORESIP_MAX_REASON_LENGTH + 1];

	int LocalFocus;
	int RemoteFocus;
	CORESIP_MediaStatus MediaStatus;
	CORESIP_MediaDir MediaDir;

	// CORESIP_CALL_RD y PJSIP_INV_STATE_CONFIRMED
	unsigned short PttId;
	unsigned ClkRate;
	unsigned ChannelCount;
	unsigned BitsPerSample;
	unsigned FrameTime;

	pj_bool_t remote_grs_supports_ED137C_Selcal;		//es true si la sesion con el grs remoto soporta selcal de ED137C

} CORESIP_CallStateInfo;

//UNIFETM: Esta estructura hay que modificarla en el ETM. para que sea como esta.
typedef struct CORESIP_PttInfo
{
	CORESIP_PttType PttType;
	unsigned short PttId;
	unsigned PttMute;
	unsigned Squ;				//Si valor es 1 -> squelch on, 0 -> off
	unsigned Qidx;				//Valor de Qidx que se envia cuando es un GRS

} CORESIP_PttInfo;

//UNIFETM: Esta estructura cambia en el ETM. Pero las que utiliza el ETM siguen estando. No hay que cambiar nada en el ETM
typedef struct CORESIP_RdInfo
{
	CORESIP_PttType PttType;
	unsigned short PttId;
	int PttMute;
	int Squelch;
	int Sct;

	int tx_ptt_mute_changed;			//Si es distinto de cero indica que ha habido un cambio en el PTT mute que el servidor de radio transmite a la radio.
										//No es el valor del Ptt Mute que viene en la extension de cabecera, el cual es el campo PttMute

	int rx_rtp_port;
	int rx_qidx;
	pj_bool_t rx_selected;
	int tx_rtp_port;
	int tx_cld;
	int tx_owd;

	unsigned int MAM_received;			//Si es distinto de cero entonces se ha recibido un MAM	y los siguientes campos son validos	
	unsigned int Tn1_ms;				//Tn1 en ms calculado del MAM recibido
	unsigned int Tj1_ms;				//Tj1 en ms calculado del MAM recibido
	unsigned int Tid_ms;				//Tid en ms calculado del MAM recibido
	unsigned int Tsd_ms;				//Tsd en ms calculado del MAM recibido
	int Ts2_ms;							//Ts2 en ms calculado del MAM recibido. Un valor negativo indica que no se ha recibido.

} CORESIP_RdInfo;

typedef struct CORESIP_RTPport_info
{
	pj_bool_t receiving;				//Indica si se esta recibiendo RTP
} CORESIP_RTPport_info;

#ifdef _ED137_
// PlugTest FAA 05/2011
typedef enum CORESIP_TypeCrdInfo
{
	CORESIP_CRD_SET_PARAMETER,
	CORESIP_CRD_RECORD,
	CORESIP_CRD_PAUSE,
	CORESIP_CRD_PTT,
	CORESIP_SQ	
} CORESIP_TypeCrdInfo;

typedef struct CORESIP_CRD
{
	CORESIP_TypeCrdInfo _Info;
	char CallRef[CORESIP_CALLREF_LENGTH+1];
    char ConnRef[CORESIP_CONNREF_LENGTH + 1];
    int Direction;
    int Priority;
	char CallingNr[CORESIP_MAX_URI_LENGTH + 1];
    char CalledNr[CORESIP_MAX_URI_LENGTH + 1];
    char SetupTime[CORESIP_TIME_LENGTH + 1];

    char ConnectedNr[CORESIP_MAX_URI_LENGTH + 1];
	char ConnectedTime[CORESIP_TIME_LENGTH + 1];

     char DisconnectTime[CORESIP_TIME_LENGTH + 1];
     int DisconnectCause;
	int DisconnectSource;

    char FrecuencyId[CORESIP_MAX_FRECUENCY_LENGTH + 1];

	char Squ[CORESIP_TIME_LENGTH + 1];
	char Ptt[CORESIP_TIME_LENGTH + 1];

} CORESIP_CRD;
#endif

typedef struct CORESIP_WG67_Subscription_Info
{
	char Role[CORESIP_MAX_URI_LENGTH + 1];						//Puede vale "subscriber" o "notifier"
	char SubscriberUri[CORESIP_MAX_URI_LENGTH + 1];				//Uri del subscriptor
	char NotifierUri[CORESIP_MAX_URI_LENGTH + 1];				//Uri del GRS notificador
	char SubscriptionState[CORESIP_MAX_REASON_LENGTH + 1];		/**< Subscription state. */
	char WG67_Version[CORESIP_MAX_URI_LENGTH + 1];				//Es el valor de la cabecera WG67-version
	int NotifyReceived;											//Si es distinto de cero entonces la llamada a la 
																//callback se debe a la recepción de un Notify
	//Los siguientes campos solo son validos si se ha recibido un Notify. Es decir NotifyReceived es distinto de cero
	char Reason[CORESIP_MAX_REASON_LENGTH + 1];					/**< Optional termination reason. */
	int Expires;												/**< Expires param, or -1 if not present. */
	int	Retry_after;											/**< Retry after param, or -1 if not present. */
		
	int Found_Parse_Errors;			//Si el valor es distinto de cero entonces indica que se han encontrado errores parseando.

	char Fid[CORESIP_MAX_IP_LENGTH + 1];		//Identificador de la frecuencia, en ED137C es obligatorio y en la ED137B siempre llegara vacio
	unsigned SessionsCount;
	struct
	{
		unsigned short PttId;
		char Uri[CORESIP_MAX_URI_LENGTH + 1];
		char SessionType[CORESIP_MAX_SESSTYPE_LENGTH + 1];

	} SessionInfo[CORESIP_MAX_WG67_SUBSCRIBERS];

	char RawBody[CORESIP_MAX_BODY_LEN];

} CORESIP_WG67_Subscription_Info;

//Estructura que define el cuerpo de los Notify al evento WG67KEY-IN
typedef struct CORESIP_WG67Notify_Body_Config
{
	int exclude_real_sessions;							//Si el valor es distinto de cero, entonces en el NOTIFY se excluyen las sesiones reales y 
														//solo aparecen las definidas en esta estructura.
	int num_sessions;									//Número de sesiones del array SessionsInfo

	struct
	{
		 unsigned int ptt_id;							//Un valor numérico con el ptt-id. 
		 char sip_from_uri[CORESIP_MAX_URI_LENGTH];		//Uri del tipo sip:user@host:port. cadena acabada en '\0'
		 char call_type[CORESIP_MAX_URI_LENGTH];		//Posibles valores "Coupling", "Radio-Rxonly", "Radio-TxRx", "Radio-Idle". cadena acabada en '\0'
	} SessionsInfo[CORESIP_MAX_WG67_NOTIFY_SESSIONS];

} CORESIP_WG67Notify_Body_Config;

//Estructura que define la cabecera Subscription-State y Expires.
typedef struct CORESIP_WG67Notify_SubscriptionState_Config
{
	char subscription_state[CORESIP_MAX_URI_LENGTH];	//Es obligatorio y puede tener los valores: "pending", "active", "terminated". Cadena terminada en cero.

	int expires;										//Valor en segundos del tiempo en que expira la subscripcion. 
														//Es opcional, si es negativo se ignora. Con estado "terminated" tambien se ignora.

	int retry_after;									//Valor en segundos del tiempo durante el cual no se permite una resubscripcion. 
														//Es opcional, si es negativo se ignora. Con un estado distinto de "terminated" tambien se ignora.

	char reason[CORESIP_MAX_URI_LENGTH];				//Es opcional y puede ser de longitud cero. Puede tener uno de estos valores. (Cadena terminada en cero):
														// "deactivated", "probation", "rejected", "timeout", "giveup", "noresource"
														// Se explica en RFC3265, apartado 3.2.4

} CORESIP_WG67Notify_SubscriptionState_Config;

#ifdef _ED137_
// PlugTest FAA 05/2011
typedef struct CORESIP_EstablishedOvrCallMembers
{
	unsigned short MembersCount;
	struct
	{
	char Member[CORESIP_MAX_URI_LENGTH + 1];
	int CallId;
	bool IncommingCall;
	}
	EstablishedOvrCallMembers[CORESIP_MAX_OVR_CALLS_MEMBERS];

} CORESIP_EstablishedOvrCallMembers;
#endif

typedef struct CORESIP_ConfInfo
{
	unsigned Version;
	unsigned UsersCount;
	char State[CORESIP_MAX_CONF_STATE_LENGTH + 1];

	struct
	{
		char Id[CORESIP_MAX_URI_LENGTH + 1];
		char Name[CORESIP_MAX_USER_ID_LENGTH + 1];
		char Role[CORESIP_MAX_USER_ID_LENGTH + 1];
		char State[CORESIP_MAX_CONF_STATE_LENGTH + 1];

	} Users[CORESIP_MAX_CONF_USERS];

} CORESIP_ConfInfo;

typedef struct CORESIP_tone_digits
{
	unsigned int count;		/**< Numero de digitos que se van a reproducir */
	struct {
		char    digit;	    /**< The ASCI identification for the digit.
							En caso de que quiera reproducir una pausa, digit debe ser una coma, on_msec debe valer cero y off_msec valdra el tiempo de la pausa */
		short   on_msec;	    /**< Playback ON duration, in miliseconds.	    */
		short   off_msec;	    /**< Playback OFF duration, ini miliseconds.    */
	} digits[CORESIP_MAX_DTMF_DIGITS];
} CORESIP_tone_digits;

typedef struct CORESIP_tone_digit_map
{
	unsigned count;	    /**< Number of digits in the map. 16 maximo	*/

	struct {
		char    digit;	    /**< The ASCI identification for the digit.	*/
		short   freq1;	    /**< First frequency.			*/
		short   freq2;	    /**< Optional second frequency.		*/
	} digits[16];	    /**< Array of digits in the digit map.	*/
} CORESIP_tone_digit_map;

typedef struct CORESIP_SndWindowsDevices
{
	unsigned int ndevices_found;	//Son los canales encontrados.
	char DeviceNames[CORESIP_MAX_SOUND_NAME_LENGTH * CORESIP_MAX_SOUND_NAMES];	//array con los nombres, separados por '<###>'
	char FriendlyName[CORESIP_MAX_SOUND_NAME_LENGTH * CORESIP_MAX_SOUND_NAMES]; //array con los nombres, separados por '<###>'
	char GUID[CORESIP_MAX_SOUND_NAME_LENGTH * CORESIP_MAX_SOUND_NAMES];			//array con los nombres, separados por '<###>'
} CORESIP_SndWindowsDevices;

typedef struct CORESIP_GenericPortBuffer
{
	short buffer[CORESIP_AUD_PACKET_LEN];		//Paquete audio. 
} CORESIP_GenericPortBuffer;

typedef enum CORESIP_Resource_Type
{
	Rd,
	Tlf,
	IA
} CORESIP_Resource_Type;

typedef struct CORESIP_HMI_Resources_Info
{
	int NumResources;	//Numero de recursos

	struct
	{
		char Id[CORESIP_MAX_RESOURCEID_LENGTH];		//Identificador del recurso.
		//En caso de radio, es el identificador del destino de radio. 
		//En la aplicacion web aparede como Id. En el SOAP es DescDestino
		//En el caso de telefonia y IA, es el numero del llamado, es decir, es el user de la URI del destino
		CORESIP_Resource_Type Type;					//Tipo de recurso
		pj_bool_t Secure;								//Establece si es seguro
	} HMIResources[CORESIP_MAX_HMI_RESOURCES];
} CORESIP_HMI_Resources_Info;

typedef struct CORESIP_Callbacks
{
	void (*LogCb)(int level, const char * data, int len);
	void (*KaTimeoutCb)(int call);
	void (*RdInfoCb)(int call, CORESIP_RdInfo * info);
	void (*CallStateCb)(int call, CORESIP_CallInfo * info, CORESIP_CallStateInfo * stateInfo);
	void (*CallIncomingCb)(int call, int call2replace, CORESIP_CallInfo * info, CORESIP_CallInInfo * inInfo);
	void (*TransferRequestCb)(int call, CORESIP_CallInfo * info, CORESIP_CallTransferInfo * transferInfo);
	void (*TransferStatusCb)(int call, int code);
	void (*ConfInfoCb)(int call, CORESIP_ConfInfo * confInfo, const char *from_uri, int from_uri_len);
	void (*OptionsReceiveCb)(const char * fromUri, const char * callid, const int statusCode, const char *supported, const char *allow );
																									//UNIFETM: En el ETM esta funcion tiene menos parametros
	//void (*OptionsReceiveCb)(const char * fromUri);												//Esta es la del ETM. 


	/**
	* WG67SubscriptionCb
	* Esta funcion se llama cuando hay un cambio en el estado de una subscripcion al evento WG67KEY-IN.
	  Como Suscriptor se llama cuando ha cambiado el estado, o porque se ha recibido un NOTIFY.
	  Como Notificador se llama cuando ha cambiado el estado de la suscripcion.
	* @param	info			Estructura con la info
	* @return
	*/
	void (*WG67SubscriptionStateCb)(CORESIP_WG67_Subscription_Info *info);

	/**
	* WG67SubscriptionReceivedCb
	* Esta funcion se llama cuando se recibe el primer request de suscripcion al evento WG67KEY-IN.
	* Si dentro de esta callback se llama a la funcion CORESIP_Set_WG67_notify_status se establece un estado inicial de la suscripcion
	* @param	accId. Identificador del account.
	* @param	subscriberUri. uri del suscriptor
	* @return
	*/
	void (*WG67SubscriptionReceivedCb)(int accId, char* subscriberUri);

	void (*InfoReceivedCb)(int call, const char * info, unsigned int lenInfo);	
	void (*IncomingSubscribeConfCb)(int call, const char *from_uri, const int from_uri_len);		//UNIFETM: Este campo falta en ETM. Inicializarlo a NULL
	void (*Presence_callback)(char *dst_uri, int subscription_status, int presence_status);			//UNIFETM: Este campo falta en ETM. Inicializarlo a NULL
	void (*FinWavCb)(int Code);																		//UNIFETM: Este campo falta en ULISES. Inicializarlo a NULL

	void (*DialogNotifyCb)(const char *xml_body, unsigned int length);			//Callback que se llama cuando se recibe un notify al evento de dialogo
	
	void (*PagerCb)(const char *from_uri, const int from_uri_len,
		     const char *to_uri, const int to_uri_len, const char *contact_uri, const int contact_uri_len,
		     const char *mime_type, const int mime_type_len, const char *body, const int body_len);
																				//Callback que se llama cuando se recibe un mensaje de texto

	/**
	*CfwrOptReceivedCb:
	*Esta funcion se llama cuando se recibe un options del tipo utilizado para
	* la negociacion Call Forward
	* @param	accId			Account obtenido de la uri To
	* @param    from_uri		Uri de la cabecera From
	* @param    cfwr_options_type.	Tipo de OPTIONS para la negociacion. Es del tipo CORESIP_CFWR_OPT_TYPE
	* @param	body			Es el cuerpo del mensaje, terminado con el caracter '\0'.
	* @param	hresp			Manejador necesario para enviar la respuesta
	* @return	
	*
	*/
	void (*CfwrOptReceivedCb)(int accId, const char *from_uri, CORESIP_CFWR_OPT_TYPE cfwr_options_type,	const char* body, const unsigned int hresp);

	/**
	*CfwrOptResponseCb:
	*Esta funcion se llama cuando se recibe la respuesta a un options del tipo utilizado para
	* la negociacion Call Forward
	* @param	accId			Account obtenido de la uri From
	* @param    dstUri			Uri de la cabecera To. Es decir, es la uri del agente que nos envia la respuesta. Finalizado con '\0'
	* @param    callid			Call Id recibido. Finalizado con '\0'
	* @param    st_code			Code de la respuesta
	* @param    cfwr_options_type.	Tipo de OPTIONS para la negociacion. Es del tipo CORESIP_CFWR_OPT_TYPE
	* @param	body			Es el cuerpo del mensaje, terminado con el caracter '\0'.
	* @return
	*
	*/
	void (*CfwrOptResponseCb)(int iccId, const char *dstUri, const char *callid, int st_code, CORESIP_CFWR_OPT_TYPE cfwr_options_type, const char *body);

	/**
	* MovedTemporallyCb:
	* Esta funcion se llama cuando se recibe un 302 (Moved Temporally) avisando que hay una redireccion pendiente de la llamada
	* Para aceptar o rechazar la redireccion, la aplicacion debe llamar a la funcion #CallProccessRedirect
	* @param	call			Call id de la llamada
	* @param    dstUri			Uri a la que se quiere redirigir la llamada. String terminado en cero.
	* @return
	*
	*/
	void (*MovedTemporallyCb)(int call, const char *dstUri);

	/**
	* RTPport_infoCb:
	* Esta funcion se llama desde un objeto RTP port para informar a la aplicacion sobre eventos
	* @param	rtpport_id		Identificador del RTPport
	* @param    info			Estructora de datos que se reporta
	* @return
	*
	*/
	void (*RTPport_infoCb)(int rtpport_id, CORESIP_RTPport_info* info);

	 
#ifdef _ED137_
	// PlugTest FAA 05/2011
	void (*OnUpdateOvrCallMembers)(CORESIP_EstablishedOvrCallMembers info);
	void (*InfoCrd)(CORESIP_CRD InfoCrd);
#endif
} CORESIP_Callbacks;

typedef struct CORESIP_Config
{	
	char HostId[CORESIP_MAX_HOSTID_LENGTH + 1];			//UNIFETM: Este campo falta en ETM. Inicializarlo a 0
	char IpAddress[CORESIP_MAX_IP_LENGTH + 1];
	char UserAgent[CORESIP_MAX_USER_ID_LENGTH + 1];		//Nombre del agente SIP. Si es un string de longitud cero 
														//entonces se usa el de por defecto que es "U5K-UA/1.0.0"
														//Para el ETM, tiene que comenzar por ETM, por ejemplo ETM6, ETMx, ETM6.1, etc.

	unsigned Port;										//Puerto SIP
	unsigned RtpPorts;									//Valor por el que empiezan a crearse los puertos RTP	//UNIFETM: Este campo falta en ETM. Inicializarlo a 0

	unsigned UseDefaultSoundDevices;					//Si es distinto de cero entonces se utilizan los dispositivos de microfono y altavoz 
														//por defecto en el sistema automáticamente, sin que lo tenga que manejar la  aplicacion.

	CORESIP_Callbacks Cb;
	char DefaultCodec[CORESIP_MAX_CODEC_LENGTH + 1];
	unsigned DefaultDelayBufPframes;
	unsigned DefaultJBufPframes;
	unsigned SndSamplingRate;
	float RxLevel;
	float TxLevel;
	unsigned LogLevel;

	unsigned TsxTout;
	unsigned InvProceedingIaTout;
	unsigned InvProceedingMonitoringTout;
	unsigned InvProceedingDiaTout;
	unsigned InvProceedingRdTout;
/* AGL 20131121. Variables para la configuracion del Cancelador de Eco */
	unsigned EchoTail;									//UNIFETM: Este campo falta en ETM. Inicializarlo a 100
	unsigned EchoLatency;								//UNIFETM: Este campo falta en ETM. inicializarlo a 0
/* FM */

	// Grabación según norma ED-137
    unsigned RecordingEd137;							//UNIFETM: Este campo falta en ETM. inicializarlo a 0

	unsigned max_calls;		//Máximo número de llamadas que soporta el agente.  //UNIFETM: Este campo falta en ETM. inicializarlo a numero de llamadas maximas. Ej. 32

	unsigned Radio_UA;		//Con valor distinto de 0, indica que se comporta como un agente de radio. //UNIFETM: Este campo falta en ETM. inicializarlo a 0

	unsigned TimeToDiscardRdInfo;		//Tiempo durante el cual no se envia RdInfo al Nodebox tras un PTT OFF. //UNIFETM: Este campo falta en ETM. inicializarlo a 0

	unsigned DIA_TxAttenuation_dB;					//Atenuacion de las llamadas DIA en Tx (Antes de transmistir por RTP). En dB
	unsigned IA_TxAttenuation_dB;					//Atenuacion de las llamadas IA en Tx (Antes de transmistir por RTP). En dB
	unsigned RD_TxAttenuation_dB;					//Atenuacion del Audio que se transmite hacia el multicas al hacer PTT en el HMI. En dB

	CORESIP_Agent_Type AgentType;		//Es el tipo de agente.

} CORESIP_Config;

typedef struct CORESIP_Impairments
{
	int Perdidos;
	int Duplicados;
	int LatMin;
	int LatMax;
} CORESIP_Impairments;

/*Callback para recibir notificaciones por la subscripcion de presencia*/
/*	dst_uri: uri del destino cuyo estado de presencia ha cambiado.
 *	subscription_status: vale 0 la subscripcion al evento no ha tenido exito. 
 *	presence_status: vale 0 si no esta presente. 1 si esta presente.
 */
typedef void (*SubPresCb)(char *dst_uri, int subscription_status, int presence_status);

#ifdef __cplusplus
extern "C" {
#endif

	/**
	 *	CORESIP_Init Rutina de Inicializacion del Modulo. @ref SipAgent::Init
	 *	@param	cfg		Puntero @ref CORESIP_Config a la configuracion.
	 *	@param	error	Puntero @ref CORESIP_Error a la estructura de Error
	 *	@return			Codigo de Error
	 */
	CORESIP_API int	CORESIP_Init(const CORESIP_Config * info, CORESIP_Error * error);

	/**
	*	CORESIP_Start Rutina de Arranque del Modulo. @ref SipAgent::Start
	*	@param	error	Puntero a la estructura de Error. @ref CORESIP_Error
	*	@return			Codigo de Error
	*/
	CORESIP_API int	CORESIP_Start(CORESIP_Error * error);

	/**
	*	CORESIP_End Rutina de Parada del Modulo. @ref SipAgent::Stop
	*	@return			Sin Retorno
	*/
	CORESIP_API void CORESIP_End();

	CORESIP_API void CORESIP_ED137Record(int on);

	/**
	*	CORESIP_Set_Ed137_version Establece la version de la ED137. El agente arranca por defecto con la ED137B
	*	@param	ED137Radioversion	Version para radio. Vale 'B' para ED137B y 'C' para ED137C
	*	@param	ED137Phoneversion	Version para telefonia. Vale 'B' para ED137B y 'C' para ED137C
	*	@param	error	Puntero a la estructura de Error. @ref CORESIP_Error
	*	@return			Codigo de Error
	*/
	CORESIP_API int	CORESIP_Set_Ed137_version(char ED137Radioversion, char ED137Phoneversion, CORESIP_Error* error);

	/**
	*	CORESIP_Get_Ed137_version Obtiene la version de la ED137.
	*	@param	ED137Radioversion	Se retorna un caracter con la Version del agente para radio. Vale 'B' para ED137B y 'C' para ED137C
	*	@param	ED137Phoneversion	Se retorna un caracter con la Version del agente para telefonia. Vale 'B' para ED137B y 'C' para ED137C
	*	@param	error	Puntero a la estructura de Error. @ref CORESIP_Error
	*	@return			Codigo de Error
	*/
	CORESIP_API int	CORESIP_Get_Ed137_version(char *ED137Radioversion, char *ED137Phoneversion, CORESIP_Error* error);

	/**
	* CORESIP_Force_Ed137_version_header. Fuerza la cabecera de WG67-version
	* @param force. Si el valor es 0 entonces no se fuerza el valor de la cabecera
	* @param ED137Radioversion	Valor que se fuerza a la cabecera WG67-version, en caso de que force sea 1
	*/
	CORESIP_API int	CORESIP_Force_Ed137_version_header(int force, char* ED137Radioversion, CORESIP_Error* error);

	/**
	 * CORESIP_SetSipPort. Establece el puerto SIP
	 * @param	port	Puerto SIP
	 * @return			Codigo de Error
	 */
	CORESIP_API int	CORESIP_SetSipPort(int port, CORESIP_Error * error);

	/**
	 *	CORESIP_SetLogLevel Establece el nivel de LOG del Modulo. @ref SipAgent::SetLogLevel
	 *	@param	level	Nivel de LOG solicitado
	 *	@param	error	Puntero @ref CORESIP_Error a la estructura de Error.
	 *	@return			Codigo de Error
	 */
	CORESIP_API int	CORESIP_SetLogLevel(unsigned level, CORESIP_Error * error);

	/**
	 *	CORESIP_SetParams Establece los Parametros del Modulo. @ref SipAgent::SetParams
	 *	@param	MaxForwards	Valor de la cabecera Max-Forwards. Si vale NULL se ignora.
	 *	@param	Options_answer_code		Codigo de la respuesta a los mensajes OPTIONS (200, 404, etc.)
	 *									Si el codigo es 0, entonces no se envia respuesta
	 *									Si se pasa un NULL, este parametro se ignora.
	 *	@param	error	Puntero @ref CORESIP_Error a la estructura de Error.
	 *	@return			Codigo de Error
	 */
	CORESIP_API int	CORESIP_SetParams(const int *MaxForwards, const int* Options_answer_code, CORESIP_Error * error);

	/**
	 *	CORESIP_SetJitterBuffer. Establece el buffer Jitter. Esta basado en el Jitter Buffer implementado en la libreria PJSIP
	 *						Si nunca se llama a esta funcion, los valores del buffer son: 
							adaptativo
							initial_prefetch = 0
							min_prefetch = 10 ms
							max_prefetch = DefaultJBufPframes * 10ms * 4 / 5;
	 * @param	adaptive	Si es distinto de cero, entonces el buffer es adaptativo, si es 0 entonces es fijo
	 * @param	initial_prefetch Prefetch del buffer de jitter. En ms. 
	 *						Si es adaptativo:	Precarga inicial aplicada al jitter buffer. 
	 *											El prefetch es una funcion del jitter buffer que se aplica cada vez que queda vacio o desde el inicio cuando initial_prefetch no es 0.
												Si el valor es mayor que 0 activara la precarga del jitter 
												retorna un frame hasta que su longitud alcanza el numero de frames especificados en este parametro.
							Si es fijo: The fixed delay value, in ms. Por ejemplo 40ms.
	 * @param	min_prefetch	Para buffer adaptativo (en fijo se ignora): El minimo prefetch que se aplica, in ms. Ej: 10ms
	 * @param	max_prefetch	Para buffer adaptativo (en fijo se ignora): El maximo prefetch que se aplica, in ms. Ej: 60 ms
	 * @param	discard			Para buffer adaptativo (en fijo se ignora):
	 *							Si su valor es distinto de cero, buffer descarta paquetes para minimizar el retardo progresivamente, incluso por debajo de min_prefetch.
	 *							El prefetch es una funcion del jitter buffer que se aplica cada vez que queda vacio.
	 *							Si su valor es cero entonces no se descarta ningun paquete.
	 *							En cualquiera de los casos, cuando el buffer se llena se descarta un paquete.
	 *	@param	error	Puntero @ref CORESIP_Error a la estructura de Error.
	 *	@return			Codigo de Error
	 */
	CORESIP_API int	CORESIP_SetJitterBuffer(unsigned adaptive, unsigned initial_prefetch, unsigned min_prefetch, unsigned max_prefetch, unsigned discard, CORESIP_Error* error);
		
	/**
	 *	CORESIP_CreateAccount. Registra una cuenta SIP en el Módulo. @ref SipAgent::CreateAccount
	 *	@param	acc			Puntero a la sip URI que se crea como agente.
	 *	@param	defaultAcc	Marca si esta cuenta pasa a ser la Cuenta por Defecto.
	 *	@param	accId		Puntero a el identificador de cuenta asociado.
	 *	@param	error		Puntero @ref CORESIP_Error a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int	CORESIP_CreateAccount(const char * acc, int defaultAcc, int * accId, CORESIP_Error * error);

	/**
	 *	CORESIP_CreateAccountForceContact. Registra una cuenta SIP en el Módulo y fuerza la uri del contact. @ref SipAgent::CreateAccount
	 *	@param	acc			Puntero a la sip URI que se crea como agente.
	 *	@param	defaultAcc	Marca si esta cuenta pasa a ser la Cuenta por Defecto.
	 *	@param	accId		Puntero a el identificador de cuenta asociado.
	 *  @param	forced_contact. Uri del contact que se fuerza.
	 *	@param	error		Puntero @ref CORESIP_Error a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int CORESIP_CreateAccountForceContact(const char* acc, int defaultAcc, int* accId, char* forced_contact, CORESIP_Error* error);

	/**
	 *	CORESIP_CreateAccountProxyRouting. Registra una cuenta SIP en el Módulo y los paquetes sip se enrutan por el proxy. 
	 *	@param	acc			Puntero a la sip URI que se crea como agente.
	 *	@param	defaultAcc	Marca si esta cuenta pasa a ser la Cuenta por Defecto.
	 *	@param	accId		Puntero a el identificador de cuenta asociado.
	 *  @param	proxy_ip	Si es distinto de NULL. IP del proxy Donde se quieren enrutar los paquetes.
	 *	@param	error		Puntero @ref CORESIP_Error a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int CORESIP_CreateAccountProxyRouting(const char * acc, int defaultAcc, int * accId, const char *proxy_ip, CORESIP_Error * error);

	/**
	 *	CORESIP_CreateAccountAndRegisterInProxy. Crea una cuenta y se registra en el SIP proxy. Los paquetes sip se rutean por el SIP proxy también.
	 *	@param	acc			Puntero al Numero de Abonado (usuario). NO a la uri.
	 *	@param	defaultAcc	Si es diferente a '0', indica que se creará la cuenta por Defecto.
	 *	@param	accId		Puntero a el identificador de cuenta asociado que retorna.
	 *	@param	proxy_ip	IP del proxy.
	 *	@param	expire_seg  Tiempo en el que expira el registro en segundos.
	 *	@param	username	Si no es necesario autenticación, este parametro será NULL
	 *	@param  pass		Password. Si no es necesario autenticación, este parametro será NULL
	 *	@param  DisplayName	Display name que va antes de la sip URI, se utiliza para como nombre a mostrar
	 *	@param	isfocus		Si el valor es distinto de cero, indica que es Focus, para establecer llamadas multidestino
	 *	@param	error		Puntero @ref CORESIP_Error a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int CORESIP_CreateAccountAndRegisterInProxy(const char * acc, int defaultAcc, int * accId, const char *proxy_ip, 
														unsigned int expire_seg, const char *username, const char *pass, const char * displayName, int isfocus, CORESIP_Error * error);

	/**
	 *	CORESIP_CreateAccountAndRegisterInProxyForceContact. Crea una cuenta y se registra en el SIP proxy. Los paquetes sip se rutean por el SIP proxy también.
	 *	@param	acc			Puntero al Numero de Abonado (usuario). NO a la uri.
	 *	@param	defaultAcc	Si es diferente a '0', indica que se creará la cuenta por Defecto.
	 *	@param	accId		Puntero a el identificador de cuenta asociado que retorna.
	 *	@param	proxy_ip	IP del proxy.
	 *	@param	expire_seg  Tiempo en el que expira el registro en segundos.
	 *	@param	username	Si no es necesario autenticación, este parametro será NULL
	 *	@param  pass		Password. Si no es necesario autenticación, este parametro será NULL
	 *	@param  DisplayName	Display name que va antes de la sip URI, se utiliza para como nombre a mostrar
	 *	@param	isfocus		Si el valor es distinto de cero, indica que es Focus, para establecer llamadas multidestino
	 *  @param  forced_contact Uri con el Contact que se quiere forzar
	 *	@param	error		Puntero @ref CORESIP_Error a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int CORESIP_CreateAccountAndRegisterInProxyForceContact(const char* acc, int defaultAcc, int* accId, const char* proxy_ip,
		unsigned int expire_seg, const char* username, const char* pass, const char* displayName, int isfocus, char* forced_contact, CORESIP_Error* error);

	/**
	 *	CORESIP_DestroyAccount. Elimina una cuenta SIP del modulo. @ref SipAgent::DestroyAccount
	 *	@param	accId		Identificador de la cuenta.
	 *	@param	error		Puntero @ref CORESIP_Error a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int	CORESIP_DestroyAccount(int accId, CORESIP_Error * error);

	/**
	 *	CORESIP_AddSndDevice		Añade un dispositvo de audio al módulo. @ref SipAgent::AddSndDevice
	 *	@param	info		Puntero @ref CORESIP_SndDeviceInfo a la Informacion asociada al dispositivo.
	 *	@param	dev			Puntero donde se recorre el identificador del dispositivo.
	 *	@param	error		Puntero @ref CORESIP_Error a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int	CORESIP_AddSndDevice(const CORESIP_SndDeviceInfo * info, int * dev, CORESIP_Error * error);

	/**
	 *	CORESIP_CreateWavPlayer		Crea un 'Reproductor' WAV. @ref SipAgent::CreateWavPlayer
	 *	@param	file		Puntero al path del fichero.
	 *	@param	loop		Marca si se reproduce una sola vez o indefinidamente.
	 *	@param	wavPlayer	Puntero donde se recorre el identificador del 'reproductor'.
	 *	@param	error		Puntero @ref CORESIP_Error a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int	CORESIP_CreateWavPlayer(const char * file, unsigned loop, int * wavPlayer, CORESIP_Error * error);

	/**
	 *	CORESIP_DestroyWavPlayer	Elimina un Reproductor WAV. @ref SipAgent::DestroyWavPlayer
	 *	@param	wavPlayer	Identificador del Reproductor.
	 *	@param	error		Puntero @ref CORESIP_Error a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int	CORESIP_DestroyWavPlayer(int wavPlayer, CORESIP_Error * error);
	
	/**
	 *	CORESIP_CreateWavRecorder	Crea un 'grabador' en formato WAV. @ref SipAgent::CreateWavRecorder
	 *	@param	file		Puntero al path del fichero, donde guardar el sonido.
	 *	@param	wavRecorder	Puntero donde se recoge el identificador del 'grabador'
	 *	@param	error		Puntero @ref CORESIP_Error a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int	CORESIP_CreateWavRecorder(const char * file, int * wavRecorder, CORESIP_Error * error);

	/**
	 *	CORESIP_DestroyWavRecorder	Elimina un 'grabador' WAV. @ref SipAgent::DestroyWavRecorder
	 *	@param	wavRecorder	Identificador del Grabador.
	 *	@param	error		Puntero @ref CORESIP_Error a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int	CORESIP_DestroyWavRecorder(int wavRecorder, CORESIP_Error * error);

	/**
	 *	CORESIP_CreateRdRxPort		Crea un 'PORT' @ref RdRxPort de Recepcion Radio. @ref SipAgent::CreateRdRxPort
	 *	@param	info		Puntero @ref CORESIP_RdRxPortInfo a la informacion del puerto
	 *	@param	localIp		Puntero a la Ip Local.
	 *	@param	rdRxPort	Puntero que recoge el identificador del puerto.
	 *	@param	error		Puntero @ref CORESIP_Error a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int	CORESIP_CreateRdRxPort(const CORESIP_RdRxPortInfo * info, const char * localIp, int * rdRxPort, CORESIP_Error * error);

	/**
	 *	CORESIP_DestroyRdRxPort		Elimina un Puerto @ref RdRxPort. @ref SipAgent::DestroyRdRxPort
	 *	@param	rdRxPort	Identificador del Puerto.
	 *	@param	error		Puntero @ref CORESIP_Error a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int	CORESIP_DestroyRdRxPort(int rdRxPort, CORESIP_Error * error);

	/**
	 *	CORESIP_CreateSndRxPort.	Crea un puerto @ref SoundRxPort. @ref SipAgent::CreateSndRxPort
	 *	@param	id			Puntero al nombre del puerto.
	 *	@param	sndRxPort	Puntero que recoge el identificador del puerto.
	 *	@param	error		Puntero @ref CORESIP_Error a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int	CORESIP_CreateSndRxPort(const char * id, int * sndRxPort, CORESIP_Error * error);

	/**
	 *	CORESIP_DestroySndRxPort	Eliminar un puerto @ref SoundRxPort. @ref SipAgent::DestroySndRxPort
	 *	@param	sndRxPort	Identificador del puerto.
	 *	@param	error		Puntero a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int	CORESIP_DestroySndRxPort(int sndRxPort, CORESIP_Error * error);

	/**
	 *	CORESIP_BridgeLink			Configura un enlace de conferencia. @ref SipAgent::BridgeLink
	 *	@param	src			Tipo e Identificador de Puerto Origen. @ref CORESIP_ID_TYPE_MASK, @ref CORESIP_ID_MASK
	 *	@param	dst			Tipo e Identificador de Puerto Destino. @ref CORESIP_ID_TYPE_MASK, @ref CORESIP_ID_MASK
	 *	@param	on			Indica Conexión o Desconexión.
	 *	@param	error		Puntero @ref CORESIP_Error a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int	CORESIP_BridgeLink(int src, int dst, int on, CORESIP_Error * error);

	/**
	 *	CORESIP_SendToRemote		Configura El puerto de Sonido apuntado para los envios UNICAST de Audio. @ref SipAgent::SendToRemote
	 *	@param	dev			...
	 *	@param	on			...
	 *	@param	id			Puntero a ...
	 *	@param	ip			Puntero a ...
	 *	@param	port		...
	 *	@param	error		Puntero a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int	CORESIP_SendToRemote(int dev, int on, const char * id, const char * ip, unsigned port, CORESIP_Error * error);

	/**
	 *	CORESIP_ReceiveFromRemote
	 *	@param	localIp		Puntero a ...
	 *	@param	mcastIp		Puntero a ...
	 *	@param	mcastPort	...
	 *	@param	error		Puntero a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int	CORESIP_ReceiveFromRemote(const char * localIp, const char * mcastIp, unsigned mcastPort, CORESIP_Error * error);

	/**
	 *	CORESIP_SetVolume
	 *	@param	id			...
	 *	@param	volume		Es el porcentaje del nivel maximo para cada dispoisitivo, segun winaudio.ini
	 *	@param	error		Puntero a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int	CORESIP_SetVolume(int id, unsigned volume, CORESIP_Error * error);

	/**
	 *	CORESIP_GetVolume
	 *	@param	id			...
	 *	@param	volume		Puntero a ...
	 *	@param	error		Puntero a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int	CORESIP_GetVolume(int id, unsigned * volume, CORESIP_Error * error);

	/**
	 *	CORESIP_CallMake
	 *	@param	info		Puntero a la informacion de llamada
	 *	@param	outInfo
	 *	@param	call		Puntero a ...
	 *	@param	error		Puntero a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int	CORESIP_CallMake(const CORESIP_CallInfo * info, const CORESIP_CallOutInfo * outInfo, int * call, CORESIP_Error * error);

	/**
	 *	CORESIP_CallHangup
	 *	@param	call		Identificador de Llamada
	 *	@param	code		...
	 *	@param	error		Puntero a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int	CORESIP_CallHangup(int call, unsigned code, CORESIP_Error * error);

	/**
	 *	CORESIP_CallAnswer
	 *	@param	call		Identificador de Llamada
	 *	@param	code		...
	 *	@param	addToConference		...
	 *	@param	reason_cause. Es el codigo del campo cause de la cabecera reason. En caso radio y el codigo esta entre 2000 y 2099 reason_text podria ser NULL porque se pone internamente.
	 *						En el caso de que no se utilice este parametro entonces su valor debera ser cero
	 *	@param	reason_text. Es el texto del campo text de la cabecera Reason. En caso de ser NULL no se incluira el campo text.
	 *						Debe de ser un string acabado con el caracter cero.
	 *	@param	error		Puntero a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int CORESIP_CallAnswer(int call, unsigned code, int addToConference, unsigned reason_cause, char* reason_text, CORESIP_Error* error);

	/**
	 *	CORESIP_CallMovedTemporallyAnswer
	 *	@param	call		Identificador de Llamada
	 *	@param	dst			Uri del usuario al que la llamada es desviada
	 *	@param	reason		Es la razon del desvio. Posibles valores "unconditional", "user-busy", etc.
	 *	@param	error		Puntero a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int CORESIP_CallMovedTemporallyAnswer(int call, const char* dst, const char* reason, CORESIP_Error* error);

	/**
	 *	CORESIP_CallProccessRedirect
	 *	Esta funcion debe llamarse despues de recibirse la callback MovedTemporallyCb para
	 *	aceptar o rechazar la redireccion de la llamada.
	 *	@param	call		Identificador de Llamada
	 *	@param  dstUri		Nueva request uri hacia donde se desvia la llamada
	 *	@param	op			Opcion (aceptar o rechazar)
	 *	@param	error		Puntero a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int CORESIP_CallProccessRedirect(int call, const char* dstUri, CORESIP_REDIRECT_OP op, CORESIP_Error* error);

	/**
	 *	CORESIP_CallHold
	 *	@param	call		Identificador de llamada
	 *	@param	hold		...
	 *	@param	error		Puntero a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int	CORESIP_CallHold(int call, int hold, CORESIP_Error * error);

	/**
	 *	CORESIP_CallReinvite
	 *	@param	call		Identificador de llamada
	 *	@param	error		Puntero a la Estructura de error
	 *	@param	CallType_SDP	9 couplig, 7 Radio-Rxonly, 5 Radio-TxRx, 6 Radio-Idle
	 *	@param	TxRx_SDP		4 Rx, 8 Tx, 0 TxRx, 22 Vacio
	 *  @param	etm_vcs_bss_methods	Para ETM, como VCS, string con los literales de los metodos BSS separados por comas. El string debe terminar caracter '\0'. Si vale NULL se ignora
	 *  @param	ForceSDPSendRecvAttr	Sirve para forzar el valor de del atributo send-recv en el SDP
	 *	@return				Codigo de Error
	 */
	CORESIP_API int	CORESIP_CallReinvite(int call, CORESIP_Error* error, int CallType_SDP, int TxRx_SDP, char * etm_vcs_bss_methods, CORESIP_SDPSendRecvAttrForced ForceSDPSendRecvAttr);

	/**
	 *	CORESIP_CallTransfer
	 *	@param	call		Identificador de llamada
	 *	@param	dstCall		...
	 *	@param	dst			Puntero a ...
	 *	@param	error		Puntero a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int	CORESIP_CallTransfer(int call, int dstCall, const char * dst, const char *display_name, CORESIP_Error * error);

	/**
	 *	CORESIP_CallPtt
	 *	@param	call		Identificador de llamada
	 *	@param	info		Puntero a la Informacion asociada al PTT
	 *	@param	error		Puntero a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int	CORESIP_CallPtt(int call, const CORESIP_PttInfo * info, CORESIP_Error * error);
	CORESIP_API int	CORESIP_CallPtt_Delayed(int call, const CORESIP_PttInfo* info, unsigned int delay_ms, CORESIP_Error* error);

	/**
	 *	CORESIP_GetRdQidx
	 *	@param	call		Identificador de llamada
	 *	@param	Qidx		Qidx del recurso de radio receptor que se retorna. Sera el manejado por el BSS.
	 *	@param	error		Puntero a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int	CORESIP_GetRdQidx(int call, int *Qidx, CORESIP_Error* error);

	/**
	 *	CORESIP_CallConference
	 *	@param	call		Identificador de llamada
	 *	@param	conf		Identificador de conferencia
	 *	@param	error		Puntero a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int	CORESIP_CallConference(int call, int conf, CORESIP_Error * error);

	/**
	 *	CORESIP_CallSendConfInfo
	 *	@param	call		Identificador de llamada
	 *	@param	info		Puntero a ...
	 *	@param	error		Puntero a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int	CORESIP_CallSendConfInfo(int call, const CORESIP_ConfInfo * info, CORESIP_Error * error);

	/**
	 *	CORESIP_SendConfInfoFromAcc
	 *	@param	accId		Identificador del account del agente
	 *	@param	info		Puntero a ...
	 *	@param	error		Puntero a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int CORESIP_SendConfInfoFromAcc(int accId, const CORESIP_ConfInfo * info, CORESIP_Error * error);

	/**
	 *	CORESIP_CallSendInfo
	 *	@param	call		Identificador de llamada
	 *	@param	info		Puntero a ...
	 *	@param	error		Puntero a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int	CORESIP_CallSendInfo(int call, const char * info, CORESIP_Error * error);

	/**
	 *	CORESIP_TransferAnswer
	 *	@param	tsxKey		Puntero a ...
	 *	@param	txData		Puntero a ...
	 *	@param	evSub		Puntero a ...
	 *	@param	code		...
	 *	@param	error		Puntero a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int	CORESIP_TransferAnswer(const char * tsxKey, void * txData, void * evSub, unsigned code, CORESIP_Error * error);

	/**
	 *	CORESIP_TransferNotify
	 *	@param	evSub		Puntero a ...
	 *	@param	code		...
	 *	@param	error		Puntero a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int	CORESIP_TransferNotify(void * evSub, unsigned code, CORESIP_Error * error);
	

	/**
	 *	CORESIP_SendOptionsMsg
	 *  Esta función no envia OPTIONS a traves del proxy
	 *	@param	dst			Puntero a uri donde enviar OPTIONS
	 *  @param	callid		callid que retorna.
	 *  @param	isRadio		Si tiene valor distinto de cero el agente se identifica como radio. Si es cero, como telefonia.
	 *						Sirve principalmente para poner radio.01 o phone.01 en la cabecera WG67-version
	 *	@param	error		Puntero a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int	CORESIP_SendOptionsMsg(const char * dst, char * callid, int isRadio, CORESIP_Error * error);

	/**
	 *	CORESIP_SendOptionsMsgProxy
	 *  Envia OPTIONS a traves del proxy.
	 *	@param	dst			Puntero a uri donde enviar OPTIONS
	 *  @param	callid		callid que retorna.
	 *  @param	isRadio		Si tiene valor distinto de cero el agente se identifica como radio. Si es cero, como telefonia.
	 *						Sirve principalmente para poner radio.01 o phone.01 en la cabecera WG67-version
	 *	@param	error		Puntero a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int CORESIP_SendOptionsMsgProxy(const char * dst, char * callid, int isRadio, CORESIP_Error * error);

	/**
	 * CORESIP_SendOptionsCFWD.	...
	 * Envia mensaje OPTIONS necesario para la negociacion Call Forward
	 * @param	accId				Account de la Coresip que utilizamos.
	 * @param	dst					Uri a la que se envia OPTIONS
	 * @param	cfwr_options_type	Tipo de OPTIONS para la negociacion. Es del tipo CORESIP_CFWR_OPT_TYPE
	 * @param	body				Contenido del body (XML). Acabado en '\0'
	 * @param	callid				callid que se retorna, acabado en '\0'.
	 * @param	by_proxy			TRUE si queremos que se envie a través del proxy. Agregara cabecera route
	 * @param	error		Puntero a la Estructura de error
	 * @return				Codigo de Error
	 */
	CORESIP_API int CORESIP_SendOptionsCFWD(int accId, const char* dst, CORESIP_CFWR_OPT_TYPE cfwr_options_type, const char* body, char* callid, pj_bool_t by_proxy, CORESIP_Error* error);

	/**
	 * CORESIP_SendResponseCFWD.	...
	 * Envia la respuesta al options utilizado para la negociacion de call forward
	 * @param	st_code				Code de la respuesta. Si no es 200 entonces se ignora el parametro del body
	 * @param	body				Contenido del body (XML). Acabado en '\0'
	 * @param	hresp				Manejador necesario para enviar la respuesta
	 * @param	error		Puntero a la Estructura de error
	 * @return				Codigo de Error
	 */
	CORESIP_API int CORESIP_SendResponseCFWD(int st_code, const char* body, unsigned int hresp, CORESIP_Error* error);

	/** AGL. Para el SELCAL */
	/**
	 *	CORESIP_Wav2RemoteStart
	 *	@param
	 *	@param
	 *	@return
	 */
	CORESIP_API int CORESIP_Wav2RemoteStart(const char *filename, const char * id, const char * ip, unsigned port, void (*eofCb)(void *), CORESIP_Error * error);

	/**
	 *	CORESIP_Wav2RemoteEnd
	 *	@param
	 *	@param
	 *	@return
	 */
	CORESIP_API int CORESIP_Wav2RemoteEnd(void *, CORESIP_Error * error);
	/****/

	/**
	 *	CORESIP_RdPttEvent. Se llama cuando hay un evento de PTT en el HMI. Sirve sobretodo para enviar los metadata de grabacion VoIP en el puesto
	 *  @param  on			true=ON/false=OFF
	 *	@param	freqId		Identificador de la frecuencia
	 *  @param  dev			Indice del array _SndPorts. Es dispositivo (microfono) fuente del audio.
	 *	@return				Codigo de Error
	 */
	CORESIP_API int CORESIP_RdPttEvent(bool on, const char *freqId, int dev, CORESIP_Error * error, CORESIP_PttType PTT_type = CORESIP_PTT_NORMAL);
	
	/**
	 *	CORESIP_RdSquEvent. Se llama cuando hay un evento de Squelch en el HMI. Sirve sobretodo para enviar los metadata de grabacion VoIP en el puesto
	 *  @param  on			true=ON/false=OFF
	 *	@param	freqId		Identificador de la frecuencia
	 *	@param	resourceId  Identificador del recurso seleccionado en el bss
	 *	@param	bssMethod	Método bss
	 *	@param  bssQidx		Indice de calidad
	 *	@return				Codigo de Error
	 */
	CORESIP_API int CORESIP_RdSquEvent(bool on, const char *freqId, const char *resourceId, const char *bssMethod, unsigned int bssQidx, CORESIP_Error * error);

	/**
	 *	CORESIP_RecorderCmd. Se pasan comandos para realizar acciones sobre el grabador VoIP
	 *  @param  cmd			Comando
	 *	@param	error		Puntero a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int CORESIP_RecorderCmd(CORESIP_RecCmdType cmd, CORESIP_Error * error);

	/**
	 *	CORESIP_CreatePresenceSubscription. Crea una subscripcion por evento de presencia
	 *  @param  dest_uri.	Uri del destino al que nos subscribimos
	 *	@param	error		Puntero a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int CORESIP_CreatePresenceSubscription(char *dest_uri, CORESIP_Error * error);

	/**
	 *	CORESIP_DestroyPresenceSubscription. destruye una subscripcion por evento de presencia
	 *  @param  dest_uri.	Uri del destino al que nos desuscribimos
	 *	@param	error		Puntero a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int CORESIP_DestroyPresenceSubscription(char *dest_uri, CORESIP_Error * error);

	/**
	 *	CORESIP_CreateConferenceSubscription. Crea una subscripcion por evento de conferencia
	 *	@param	accId		Identificador del account. Si es -1, se utiliza la default
	 *  @param  call		Call id. si es negativo usa la de por defecto
	 *  @param  dest_uri.	Uri del destino a monitorizar
	 *  @param	by_proxy. Si true entonces el subscribe se envia a traves del proxy
	 *	@param	error		Puntero a la Estructura de error
	 *	@return				CORESIP_OK si no hay error.
	 */
	CORESIP_API int CORESIP_CreateConferenceSubscription(int accId, int call, char *dest_uri, pj_bool_t by_proxy, CORESIP_Error * error);

	/**
	 *	CORESIP_DestroyConferenceSubscription. Destruye una subscripcion por evento de conferencia
	 *  @param  dest_uri.	Uri del destino a monitorizar
	 *	@param	error		Puntero a la Estructura de error
	 *	@return				CORESIP_OK si no hay error.
	 */
	CORESIP_API int CORESIP_DestroyConferenceSubscription(char *dest_uri, CORESIP_Error * error);

	/**
	 *	CORESIP_CreateDialogSubscription. Crea una subscripcion por evento de Dialogo
	 *	@param	accId		Identificador del account. Si es -1, se utiliza la default
	 *  @param  dest_uri.	Uri del destino a monitorizar
	 *  @param	by_proxy. Si true entonces el subscribe se envia a traves del proxy
	 *	@param	error		Puntero a la Estructura de error
	 *	@return				CORESIP_OK si no hay error.
	 */
	CORESIP_API int CORESIP_CreateDialogSubscription(int accId, char *dest_uri, pj_bool_t by_proxy, CORESIP_Error * error);

	/**
	 *	CORESIP_DestroyDialogSubscription. Destruye una subscripcion por evento de Dialogo
	 *  @param  dest_uri.	Uri del destino a monitorizar
	 *	@param	error		Puntero a la Estructura de error
	 *	@return				CORESIP_OK si no hay error.
	 */
	CORESIP_API int CORESIP_DestroyDialogSubscription(char *dest_uri, CORESIP_Error * error);

	/**
	 *	CORESIP_SendWG67Subscription. Crea y envia una subscripcion por evento WG67KE-IN. 
	 *	Una vez creada la suscripcion se puede enviar un nuevo request de la susbcripcion llamando a esta funcion, la suscripcion se identifica con:
			acc_id + dest_uri. 
	 *	@param	accId		Identificador del account. Si es -1, se utiliza la default
	 *  @param  dest_uri.	Uri del destino GRS al que nos subscribimos. Si callId es disinto de -1 entonces este parametro se ignora y puede ser NULL.
	 *	@param	expires.	Valor del expires. Si vale -1 entonces toma el valor por defecto, si vale 0 entonces se terminará la subscripcion
	 *	@param	noRefresh	Si es 1 entonces la suscripcion no se refresca automaticamente a partir del momento en que se envia. Si es 0 entonces si refresca
	 *	@param	error		Puntero a la Estructura de error
	 *	@return				CORESIP_OK si no hay error.
	 */
	CORESIP_API int CORESIP_SendWG67Subscription(int accId, char* dest_uri, int expires, int noRefresh, CORESIP_Error* error);

	/**
	 *	CORESIP_SetWG67SubscriptionParameters. Modifica parametros de la subscripcion por evento WG67KE-IN. 
	 *						Esta funcion no puede llamarse si previamente no se ha llamado a CORESIP_SendWG67Subscription
	 *	Una vez creada la suscripcion se puede enviar un nuevo request de la susbcripcion llamando a esta funcion, la suscripcion se identifica con:
			acc_id + dest_uri. 
	 *	@param	accId		Identificador del account. Si es -1, se utiliza la default
	 *  @param  dest_uri.	Uri del destino GRS al que nos subscribimos. 
	 *						Debe ser el mismo parametro que se utilizo con CORESIP_SendWG67Subscription
	 *	@param	noRefresh	Si es 1 entonces la suscripcion no se refresca automaticamente. Si es 0 entonces si refresca. Si es -1 entonces no tiene efecto
	 *	@param	error		Puntero a la Estructura de error
	 *	@return				CORESIP_OK si no hay error.
	 */
	CORESIP_API int CORESIP_SetWG67SubscriptionParameters(int accId, char* dest_uri, int noRefresh, CORESIP_Error* error);

	/**
	 *	CORESIP_Set_WG67_Notifier_Parameters. Configura algunos parametros del WG67 notifier
	 *  @param	notify_enabled. Si es 1 entonces los Notify son aceptados, 0 son rechazados, con -1 este parametro se ignora.
	 *	@param	manual_notify. Si es 1 entonces las notificaciones son manuales, 0 automatico, -1 no cambia. Si no se llama a esta función, por defecto los notify se envía automáticamente
	 *  @param  minimum_expires. Tiempo minimo de expires soportado. Si el valor es -1, entonces no tiene efecto. Si el subscriptor envia un valor menor entonces se rechaza
 			y hay ue enviar cabecera con el minimo expires soportado. El valor mínimo de este parámetro es de 30.
	 *  @param  maximum_expires. Tiempo maximo de expires soportado. Si el valor es -1, entonces no tiene efecto. Si el subscriptor envia un valor mayor al valor de este 
			parametro entonces en el 200 OK envia este valor.
	 *	@param error. Si hay error contiene la descripcion
	 *	@return	CORESIP_OK si no hay error.
	 */
	CORESIP_API int CORESIP_Set_WG67_Notifier_Parameters(int notify_enabled, int manual, int minimum_expires, int maximum_expires, CORESIP_Error* error);

	/**
	 * CORESIP_Set_WG67_notify_status: Establece el estado de la suscripcion y las sesiones ficticias, ademas de enviar el Notify correspondiente.
	 * Esta funcion puede ser llamada en la callback WG67SubscriptionReceivedCb para establecer el estado inicial de la suscripcion
	 * La función CORESIP_Set_WG67_notify_status establece el estado de una subscripción (si el parámetro subscriberUri no es NULL), o de todas
		las subscripciones activas o futuras (si el parámetro subscriberUri es NULL).
		Es decir, que si se quiere establecer las sesiones y estados ficticios para todas las subscripciones activas o las futuras,
		se llamaría a esta función con el parámetro subscriberUri a NULL. En este caso, para las suscripciones activas se envía en ese momento un 
		NOTIFY. Y para las nuevas subscripciones el primer NOTIFY ya lleva las sesiones configuradas con esta función.

		En el caso de que el parámetro subscriberUri no sea NULL, afecta sólo al subscriptor de esa uri. Y esta función se podría llamar dentro 
		de la callback WG67SubscriptionReceivedCb para que el primero notify de esa subscripción concreta ya lleva las funciones ficticias.
		Y si se llama cuando la suscripción ya está activa entonces se envía un NOTIFY.
	 * @param	accId		Identificador del account. Si es -1, se utiliza la default
	 * @param	subscriberUri.	Uri del suscriptor recibido en la callback WG67SubscriptionReceivedCb. Si es NULL establece el estado y envia notify a todos los sbuscriptores.
	 * @param	subsState. Establece el estado de la subscripcion. Puede valer NULL si no queremos modificar el estado.
	 *						Si el campo subscription_state de la estructura tiene longitud cero, entonces tampoco se modifica el estado.
	 * @param	wG67Notify_Body. Configura el body (las sesiones activas) que se envia en los NOTIFY. Puede ser NULL si no queremos modificar la lista de sesiones
	 *	@param error. Si hay error contiene la descripcion
	 *	@return	CORESIP_OK si no hay error.
	 */
	CORESIP_API int CORESIP_Set_WG67_notify_status(int accId, char *subscriberUri, CORESIP_WG67Notify_SubscriptionState_Config* subsState,
		CORESIP_WG67Notify_Body_Config* wG67Notify_Body, CORESIP_Error* error);

	/**
	 * CORESIP_Get_GRS_WG67SubscriptionsList: Retorna las subscripciones al evento WG67 en el GRS
	 * @param	accId.	Identificador del account. Si es -1, se utiliza la default
	 * @param   WG67Subscriptions. Returna un puntero a un array de elementos del tipo CORESIP_WG67_Subscription_Info. Si es NULL entonces no hay subscripciones
	 * @param	nSubscriptions. Retorna el número de subscripciones.
	 * @return	CORESIP_OK si no hay error.
	 */
	CORESIP_API int CORESIP_Get_GRS_WG67SubscriptionsList(int accId, int* nSubscriptions, CORESIP_WG67_Subscription_Info* WG67Subscriptions[], CORESIP_Error* error);

	/**
	*	CORESIP_CreateGenericPort: Crea un puerto generico de media. 
	*   A este puerto se pueden poner y tomar paquetes de audio de 160 muestras 16 bits PCM con signo
	*	con las funciones #PutFrame_GenericPort y #GetFrame_GenericPort.	*   
	*	Tambien se puede conectar a otros puertos con la funcion CORESIP_BridgeLink. Por ejemplo a una llamada VoIP
	*	@param gen_port_id. Es el identificador del puerto.
	*   @param jitter_buff_size. Capacidad del buffer de jitter. Valor recomendado: 2 paquetes.
	*	@param error. Si hay error contiene la descripcion
	*	@return	CORESIP_OK si no hay error. Si hay error entonces genericPort devuelto no sera valido.
	*/
	CORESIP_API int CORESIP_CreateGenericPort(int* gen_port_id, int jitter_buff_size, CORESIP_Error* error);
	
	/**
	*	CORESIP_DestroyGenericPort: Destruye el puerto del tipo GenericPort
	*	@param gen_port_id. Es el identificador del puerto.
	*	@param error. Si hay error contiene la descripcion
	*	@return	CORESIP_OK si no hay error. Si hay error entonces genericPort devuelto no sera valido.
	*/
	CORESIP_API int CORESIP_DestroyGenericPort(int gen_port_id, CORESIP_Error* error);

	/**
	*	CORESIP_PutInGenericPort: Pone muestras en el generic port.
	* 	@param gen_port_id. Es el identificador del puerto.
	*	@param genBuff. Buffer de muestras. 160 muestras 16 bits PCM con signo	
	*	@param blocking. Si es true se bloquea la funcion hasta que el paquete es transferido. Si es false entonces la funcion retorna sin esperar.
	*	@param error. Si hay error contiene la descripcion
	*	@return	CORESIP_OK si no hay error. Si hay error entonces genericPort devuelto no sera valido.
	*/
	CORESIP_API int CORESIP_PutInGenericPort(int gen_port_id, CORESIP_GenericPortBuffer *genBuff, pj_bool_t blocking, CORESIP_Error* error);

	/**
	*	CORESIP_GetFromGenericPort: Toma muestras del generic port
	* 	@param gen_port_id. Es el identificador del puerto.
	*	@param genBuff. Buffer en el que se retornan las muestras. 160 muestras 16 bits PCM con signo	
	*	@param blocking. Si es true se bloquea la funcion hasta que hay disponible un paquete de audio. 
						 Si es false entonces la funcion retorna sin esperar con un paquete de audio, si no hay disponible entonces es un paquete calculado por el PLC.
	*	@param error. Si hay error contiene la descripcion
	*	@return	CORESIP_OK si no hay error. Si hay error entonces genericPort devuelto no sera valido.
	*/
	CORESIP_API int CORESIP_GetFromGenericPort(int gen_port_id, CORESIP_GenericPortBuffer *genBuff, pj_bool_t blocking, CORESIP_Error* error);

	/**
	*	CORESIP_GetJitterStatusGenericPort: Retorna la ocupacion del jitter buffer
	* *	@param gen_port_id. Es el identificador del puerto.
	*	@param size. Numero de paquetes que tiene.
	*	@return	CORESIP_OK si no hay error. Si hay error entonces genericPort devuelto no sera valido.
	*/
	CORESIP_API int CORESIP_GetJitterStatusGenericPort(int gen_port_id, unsigned int *size, CORESIP_Error* error);

	/**
	 * CORESIP_SendInstantMessage. Envia un mensaje instantaneo
	 *
	 * @param	acc_id		Account ID to be used to send the request.
	 * @param	dest_uri	Uri del destino del mensaje. Acabado en 0.
	 * @param	text		Texto plano a enviar. Acabado en 0
	 * @param	by_proxy	Si es true el mensaje se envia por el proxy
	 * @return	Codigo de Error
	 *
	 */
	CORESIP_API int CORESIP_SendInstantMessage(int acc_id, char *dest_uri, char *text, pj_bool_t by_proxy, CORESIP_Error * error);

	/**
	 * CORESIP_EchoCancellerLCMic.	...
	 * Activa/desactiva cancelador de eco altavoz LC y Microfonos. Sirve para el modo manos libres
	 * Por defecto está desactivado en la CORESIP
	 * @param	on						true - activa / false - desactiva
	 * @return	CORESIP_OK OK, CORESIP_ERROR  error.
	 */
	CORESIP_API int CORESIP_EchoCancellerLCMic(bool on, CORESIP_Error * error);

	/**
	 *	CORESIP_SetTipoGRS. Configura el tipo de GRS. El ETM lo llama cuando crea un account tipo GRS.
	 *	@param	accId		Identificador de la cuenta.
	 *	@param	FlagGRS	Tipo de GRS.
	 *	@param	on			Indicamos que este account es de una radio GRS
	 *	@param	error		Puntero @ref CORESIP_Error a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int CORESIP_SetTipoGRS(int accId, CORESIP_CallFlagsMask Flag, int on, CORESIP_Error * error);

	/**
	 *	CORESIP_SetGRSParams. Configura parametros del GRS
	 *	@param	accId		Identificador de la cuenta. Si es -1 entonces se utiliza la default.
	 *	@param	RdFr		Frecuencia del GRS (fid). String acabado en '\0'. Con NULL se ignora
	 *	@param	Tj1			Jitter buffer delay en milisegundos. Con NULL se ignora.
	 *	@param	Ts1			GRS System Delay. En milisegundos. Con NULL se ignora
	 *  @param	Ts2			GRS system RX delay (including packetizing time). En milisegundos. Con NULL se ignora
	 *	@param	preferred_grs_bss_method   Metodo BSS preferido. Si en la lista de metodo recibido del VCS aparece entonces se selecciona, 
										si no entonces se selecciona "RSSI". Es un string terminado com caracter cero o si es NULL se ignora este parametro
										Su longitud maxima es CORESIP_MAX_BSS_LENGTH
	 *	@param  preferred_grs_bss_method_code		Si #preferred_grs_bss_method no es "RSSI", "AGC", "C/N" ni "PSD", 
													este parametro es el valor del codigo del Vendor specific method. Debera ser entre 4 y 7.
													Si es NULL se ignora este parametro
	 *	@param  forced_pttid	Si es NULL se ignora este parametro. Si el valor es -1, entonces el GRS asigna automaticamente el ptt-id.
								Si el valor es distinto de -1 entonces es el valor de ptt-id que se fuerza cuando se establece una sesion.
								Si el valor es cero entonces no aparece el atribueto ptt-id.
	 *	@param	selcal_supported	Si es NULL se ignore. Si el valor es 1 entonces el GRS soporta SELCAL, y vale 0 no.
	 *	@param	error		Puntero @ref CORESIP_Error a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int CORESIP_SetGRSParams(int accId, char *RdFr, unsigned int *Tj1, unsigned int *Ts1, unsigned int* Ts2,
		char * preferred_grs_bss_method, unsigned int *preferred_grs_bss_method_code, 
		int *forced_ptt_id, int *selcal_supported, CORESIP_Error* error);

	/**
	 *	CORESIP_GRS_Force_Ptt_Mute. Como GRS Fuerza PTT mute en R2S Keepalives hacia VCS. Sirve para simular un PTT mute de otra sesion inventada.
	 *	@param	call		Identificador de la llamada/sesion SIP
	 *	@param	PttType		Tipo de PTT. PTT que activa el Ptt mute
	 *	@param	PttId		Ptt ID. PTT id del ptt que activa el mute.
	 *	@param	on			on. Si true lo activa, si false lo desactiva y los keepalives son los normales. 
	 *						En caso de false se ignoran los parametros anteriores excepto el call.
	 *	@param	error		Puntero @ref CORESIP_Error a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int CORESIP_GRS_Force_Ptt_Mute(int call, CORESIP_PttType PttType, unsigned short PttId, bool on, CORESIP_Error* error);

	/**
	 *	CORESIP_GRS_Force_Ptt. Como GRS Fuerza PTT en R2S Keepalives hacia VCS. Sirve para simular un PTT de otra sesion inventada.
	 *	@param	call		Identificador de la llamada/sesion SIP
	 *	@param	PttType		Tipo de PTT. PTT que activa el Ptt mute
	 *	@param	PttId		Ptt ID. PTT id del ptt que activa el mute.
	 *	@param	error		Puntero @ref CORESIP_Error a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int CORESIP_GRS_Force_Ptt(int call, CORESIP_PttType PttType, unsigned short PttId, CORESIP_Error* error);

	/**
	 *	CORESIP_GRS_Force_SCT. Como GRS Fuerza el bit SCT en el RTPRx enviado desde un GRS
	 *	@param	call		Identificador de la llamada/sesion SIP
	 *	@param	on			on. Si true lo activa, si false lo desactiva.
	 *	@param	error		Puntero @ref CORESIP_Error a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int CORESIP_GRS_Force_SCT(int call, bool on, CORESIP_Error* error);

	/**
	 *	CORESIP_Force_PTTS. Fuerza el bit PTTS en el RTPRx o RTPTx
	 *	@param	call		Identificador de la llamada/sesion SIP
	 *	@param	on			on. Si true lo activa, si false lo desactiva.
	 *	@param	error		Puntero @ref CORESIP_Error a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int CORESIP_Force_PTTS(int call, bool on, CORESIP_Error* error);


	CORESIP_API int CORESIP_SetImpairments(int call, CORESIP_Impairments * impairments, CORESIP_Error * error);
		

	/**
	 *	CORESIP_SetCallParameters. Configura parametros para una sesion SIP activa.
	 *	@param	call		Call Id que identifica la llamada
	 *	@param	disableKeepAlives. Si vale 1 los Keepalives dejan de enviarse. con valor 0 se envian. Si el puntero es NULL se ignora.
	 *	@param	forced_cld. Valor forzado del CLD en ms. Si el valor es negativo, entonces se envia el calculado (Tn1 en el caso del ETM). Si el puntero es NULL se ignora.
	 *	@param	error		Puntero @ref CORESIP_Error a la Estructura de error
	 *	@return				Codigo de Error
	 */
	CORESIP_API int CORESIP_SetCallParameters(int call, int *disableKeepAlives, int *forced_cld, CORESIP_Error* error);

	/**
	 * CORESIP_SetConfirmPtt.	...
	 * Activa/desactiva la confirmacion de PTT cuando es un agente de radio
	 * Por defecto esta activado en la CORESIP
	 * @param	on						true - activa / false - desactiva
	 * @param	error		Puntero @ref CORESIP_Error a la Estructura de error
	 * @return	CORESIP_OK OK, CORESIP_ERROR  error.
	 */
	CORESIP_API int CORESIP_SetConfirmPtt(int call, pj_bool_t val, CORESIP_Error* error);

	/**
	 * CORESIP_GetNetworkDelay.	...
	 * Retorna el retardo de red para una llamada VoIP
	 * @param	call		Call Id que identifica la llamada
	 * @param	delay_ms	Retardo en ms
	 * @param	error		Puntero @ref CORESIP_Error a la Estructura de error
	 * @return	CORESIP_OK OK, CORESIP_ERROR  error.
	 */
	CORESIP_API int CORESIP_GetNetworkDelay(int call, unsigned int *delay_ms, CORESIP_Error* error);

	/**
	 * CORESIP_SendToneToCall
	 * Envia un tono por una llamada establecida. Si se llama a esta funcion varias veces para la misma llamada los tonos se suman.
	 * @param	call		Call Id que identifica la llamada
	 * @param	frequency	Frecuencia en Hz. Si vale 0 entonces se aplica a todos los tonos activos. 
	 * @param	volume_dbm0		en dBm0. Rango valores (-60 a +3.14)
	 * @param	on			Si vale 1 el tono se emite, si vale 0 el tono deja de emitirse
	 * @param	error		Puntero @ref CORESIP_Error a la Estructura de error
	 * @return	CORESIP_OK OK, CORESIP_ERROR  error.
	 */
	CORESIP_API int CORESIP_SendToneToCall(int call, unsigned int frequency, float volume_dbm0, int on, CORESIP_Error* error);

	/**
	 * CORESIP_SendNoiseToCall
	 * Envia un ruido blanco por una llamada establecida. 
	 * @param	call		Call Id que identifica la llamada
	 * @param	volume_dbm0		en dBm0. Rango valores (-60 a +3.14)
	 * @param	on			Si vale 1 el tono se emite, si vale 0 el tono deja de emitirse
	 * @param	error		Puntero @ref CORESIP_Error a la Estructura de error
	 * @return	CORESIP_OK OK, CORESIP_ERROR  error.
	 */
	CORESIP_API int CORESIP_SendNoiseToCall(int call, float volume_dbm0, int on, CORESIP_Error* error);

	/**
	 * CORESIP_SendNoiseToCall
	 * Envia un ruido rosa por una llamada establecida.
	 * @param	call		Call Id que identifica la llamada
	 * @param	volume_dbm0		en dBm0. Rango valores (-60 a +3.14)
	 * @param	on			Si vale 1 el tono se emite, si vale 0 el tono deja de emitirse
	 * @param	error		Puntero @ref CORESIP_Error a la Estructura de error
	 * @return	CORESIP_OK OK, CORESIP_ERROR  error.
	 */
	CORESIP_API int CORESIP_SendPinkNoiseToCall(int call, float volume_dbm0, int on, CORESIP_Error* error);

	/**
	 * CORESIP_SendDTMF
	 * Envia secuencia de digitos DTMF
	 * @param	call		Call Id que identifica la llamada
	 * @param	digit_map	Definicion de los digitos.
	 * @param	digits		Digitos que se quieren reproducir.
	 * @param	error		Puntero @ref CORESIP_Error a la Estructura de error
	 * @return	CORESIP_OK OK, CORESIP_ERROR  error.
	 */
	CORESIP_API int CORESIP_SendDTMF(int call, const CORESIP_tone_digit_map *digit_map, const CORESIP_tone_digits *digits, float volume_dbm0, CORESIP_Error* error);

	/**
	 * CORESIP_SendSELCAL
	 * Envia secuencia SELCAL
	 * @param	call		Call Id que identifica la llamada
	 * @param	selcalValue	string con los identificadores de los tonos. La longitud debe ser #CORESIP_MAX_SELCAL_LENGTH
	 * @param	error		Puntero @ref CORESIP_Error a la Estructura de error
	 * @return	CORESIP_OK OK, CORESIP_ERROR  error.
	 */
	CORESIP_API int CORESIP_SendSELCAL(int call, const char* selcalValue, CORESIP_Error* error);

	/*
	Funcion que retorna los dispositivos de sonido en Windows (no en asio)
	@param captureType. Si vale distinto de cero retorna los de tipo entrada (capture), si vale cero los de tipo salida (play)
	@param Devices. Retorna la lista de dispositivos encontrados.	 
	@return	CORESIP_OK OK, CORESIP_ERROR  error.
	*/
	CORESIP_API int CORESIP_GetWindowsSoundDeviceNames(int captureType, CORESIP_SndWindowsDevices *Devices, CORESIP_Error* error);

	/*
	Asigna el literal del dispositivo de sonido que maneja la API de Windows al tipo de dispositivo que maneja el puesto de Ulises.
	@param UlisesDev. Dispositivo del puesto de Ulises
	@param DevWinName. Cadena de caracteres terminada con el caracter cero, con el nombre del dispositivo que maneja Windows
	@return	CORESIP_OK OK, CORESIP_ERROR  error.
	*/
	CORESIP_API int CORESIP_SetSNDDeviceWindowsName(CORESIP_SndDevType UlisesDev, const char* DevWinName, CORESIP_Error* error);

	/*
	Funcion que establece volumen de un dispositivo de salida
	@param dev. dispositivo. 
	@param volume. Valor entre MinVolume y MaxVolume de HMI.exe.config
	*/
	CORESIP_API int CORESIP_SetVolumeOutputDevice(CORESIP_SndDevType dev, unsigned int volume, CORESIP_Error* error);

	/*
	Funcion que retorna volumen de un dispositivo de salida
	@param dev. dispositivo.
	@param volume. Valor entre MinVolume y MaxVolume de HMI.exe.config
	*/
	CORESIP_API int CORESIP_GetVolumeOutputDevice(CORESIP_SndDevType dev, unsigned int *volume, CORESIP_Error* error);

	/*
	Funcion que Crea un puerto de media para enviar y recibir por RTP
	@param rtpport_id. Manejador del puerto que se retorna.
	@param dst_ip. IP de destino del flujo RTP. Puede ser unicast o multicast.
	@param src_port. Puerto source del flujo RTP. Puede ser cero para que coja cualquiera.
	@param dst_port. Puerto de destino del flujo RTP.
	@param local_multicast_ip. Si no es NULL, es la direccion del grupo multicas al que se agrega para recibir rtp
	@param payload_type. Valor 0: PCMU, valor 8: PCMA
	@param action. Indica el puerto RTP enconde, decode o las dos cosas.
	*/
	CORESIP_API int CORESIP_CreateRTPport(int* rtpport_id, char* dst_ip, int src_port, int dst_port, char* local_multicast_ip, int payload_type, CORESIP_RTP_port_actions action, CORESIP_Error* error);

	/*
	Funcion que pausar, reanudar y destruir un puerto de media para enviar y recibir por RTP
	@param rtpport_id. Manejador del puerto.
	@param action. Indica el puerto RTP enconde, decode o las dos cosas
	*/
	CORESIP_API int CORESIP_PauseResumeDestroyRTPport(int rtpport_id, CORESIP_RTP_port_actions action, CORESIP_Error* error);

	/*
	Funcion para solicitar que se genere la callback RTPport_infoCb para actualizar el estado
	@param rtpport_id. Manejador del puerto.
	*/
	CORESIP_API int CORESIP_AskRTPport_info(int rtpport_id, CORESIP_Error* error);	

	/*
	Funcion para para pasar la informacion de los recursos configurados en el HMI. 
	Se necesita para que la Coresip tenga la informacion de que recursos son seguros o no,
	necesario para que en la grabacion el audio sea enviado al grabador seguro o al otro.
	@param Resources_Info. Estructura con la informacion.
	* @param	error		Puntero @ref CORESIP_Error a la Estructura de error
	* @return	CORESIP_OK OK, CORESIP_ERROR  error.
	*/
	CORESIP_API int CORESIP_Set_HMI_Resources_Info(CORESIP_HMI_Resources_Info* Resources_Info, CORESIP_Error* error);



#ifdef __cplusplus
}
#endif

#endif