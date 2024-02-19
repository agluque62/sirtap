/**
 * @file sipagent.cpp
 * @brief Gestion de un Agente SIP en CORESIP.dll
 *
 *	Implementa la clase 'SipAgent', que encapsula las funciones 'pjsip' correspondientes al Agente.
 *
 *	Utiliza como base la libreria 'pjsua'
 *
 *	@addtogroup CORESIP
 */
/*@{*/
#include "Global.h"
#include "SipAgent.h"
#include "Exceptions.h"
#include "SipCall.h"
#include "Guard.h"
#ifdef PJ_USE_ASIO
#include <pa_asio.h>
#endif
#include "PresenceManag.h"
#include "AccountUserData.h"
#include "OptionsMod.h"

#include <iphlpapi.h>
#include <sys/stat.h>
#include "SDPUtils.h"
#include "SoundDevHw.h"
#include "CUtils.h"

/**
 * OnMemAllocFail: Rutina de tratamiento de las excepciones de Memoria.
 * - Genera una excepcion global.
 * @param	pool	Puntero 'pj_pool' al objeto que ha generado el error.
 * @param	size	Tamaño 'pj_size': del Objeto.
 * @return	nada
 */
static void OnMemAllocFail(pj_pool_t *pool, pj_size_t size)
{
	throw PJLibException(__FILE__, PJ_ENOMEM).Msg("ERROR reservando memoria");
}

/**
 * coresip_mod: Estructura de configuracion del Módulo.
 * - Informacion: http://www.pjsip.org/pjsip/docs/html/structpjsip__module.htm
 */
static pjsip_module coresip_mod = 
{
	NULL, NULL,									/* prev, next.			*/
	{ "mod-coresip", 11 },					/* Name.				*/
	-1,											/* Id				*/
	PJSIP_MOD_PRIORITY_APPLICATION + 1,	/* Priority			*/
	NULL,											/* load()				*/
	NULL,											/* start()				*/
	NULL,											/* stop()				*/
	NULL,											/* unload()				*/
	&SipAgent::OnRxRequest,											/* on_rx_request()			*/
	&SipAgent::OnRxResponse,				/* on_rx_response()			*/
	&SipCall::OnTxRequest,											/* on_tx_request.			*/
	&SipCall::OnTxRequest,											/* on_tx_response()			*/
	NULL,											/* on_tsx_state()			*/
};

pj_bool_t SipAgent::SIP_AGENT_INITIALIZED_AND_STARTED = PJ_FALSE;
pj_bool_t SipAgent::ETM = PJ_FALSE;				//Si es true entonces indica que el agente es un ETM
UINT SipAgent::timePeriodApplied = 0;
int SipAgent::ESTADO_INICIALIZACION = NO_INICIALIZADO;

HANDLE SipAgent::ghMutex = NULL;

unsigned SipAgent::UseDefaultSoundDevices = 0;				//Si es distinto de cero entonces se utilizan los dispositivos de microfono y altavoz 
															//por defecto en el sistema automáticamente, sin que lo tenga que manejar la  aplicacion.
PaDeviceIndex SipAgent::Pa_DefaultInputDev = paNoDevice;	//Indice ASIO del dispositivo de entrada por defecto
PaDeviceIndex SipAgent::Pa_DefaultOutputDev = paNoDevice;	//Indice ASIO del dispositivo de entrada por defecto
int SipAgent::DefaultSndDeviceID = -1;						//Es el device devuelto por la funcion AddSndDevice que retorna el dispositivo de sonido del tipo SoundPort

/**
 * SipAgent::Cb: Bloque de Callbacks asociados al agente 'pjsua'
 */
CORESIP_Callbacks SipAgent::Cb;
/**
 *	SipAgent::EnableMonitoring: ...
 */
bool SipAgent::EnableMonitoring = false;
/**
 *	SipAgent::SndSamplingRate: ...
 */
unsigned SipAgent::SndSamplingRate = SND_SAMPLING_RATE;
/**
 *	SipAgent::DefaultDelayBufPframes: ...
 */
unsigned SipAgent::DefaultDelayBufPframes = DELAY_BUF_PFRAMES;
/**
 *	SipAgent::RxLevel: ...
 */
float SipAgent::RxLevel = 1.0;
/**
 *	SipAgent::TxLevel: ...
 */
float SipAgent::TxLevel = 1.0;
/**
 *	SipAgent::_Lock: Puntero a ...
 */
pj_lock_t * SipAgent::_Lock = NULL;
/**
 *	SipAgent::_SndPorts: Lista de Punteros a los Dispositivos de Sonido.
 */
SoundPort * SipAgent::_SndPorts[CORESIP_MAX_SOUND_DEVICES];
/**
 *	SipAgent::_WavPlayers: Lista de Punteros a los Reproductores Wav.
 */
WavPlayer * SipAgent::_WavPlayers[CORESIP_MAX_WAV_PLAYERS];
/**
 *	SipAgent::_WavRecorders: Lista de Punteros a los Grabadores Wav.
 */
WavRecorder * SipAgent::_WavRecorders[CORESIP_MAX_WAV_RECORDERS];
/**
 *	SipAgent::_RdRxPorts: Lista de Punteros a los Puertos de Recepcion Radio.
 */
RdRxPort * SipAgent::_RdRxPorts[CORESIP_MAX_RDRX_PORTS];
/**
 *	SipAgent::_SndRxPorts: Lista de Punteros a los Puertos de Recepcion.
 */
SoundRxPort * SipAgent::_SndRxPorts[CORESIP_MAX_SOUND_RX_PORTS];

/**
 *	SipAgent::FrecDesp: Gestor de grupos de climax.
 */
FrecDesp *	SipAgent::_FrecDesp = NULL;

/**
 *	SipAgent::_PresenceManager: Gestor de presencias
 */
PresenceManag *	SipAgent::_PresenceManager = NULL;

/**
 *	SipAgent::_ConfManager: Gestor de conferencias
 */
ClientSubscriptionManager<ConfSubs> *	SipAgent::_ConfManager = NULL;

/**
 *	SipAgent::_DlgManager: Gestor de subscripciones al evento de dialogo
 */
ClientSubscriptionManager<DlgSubs> *	SipAgent::_DlgManager = NULL;

/**
 *	SipAgent::_DlgManager: Gestor de subscripciones al evento de dialogo
 */
ClientSubscriptionManager<WG67Subs>* SipAgent::_WG67Manager = NULL;

//Cancela eco entre altavoz y microfono cuando 
//telefonia sale por altavoz
pjmedia_echo_state *SipAgent::_EchoCancellerLCMic = NULL;	
pj_bool_t SipAgent::_AltavozLCActivado = PJ_FALSE;			//Si true, el altavoz LC esta activado reproduciendo audio
pj_lock_t *SipAgent::_ECLCMic_mutex = NULL;					//Mutex para el cancelador de eco

/**
 *	SipAgent::_SndRxIds: Mapa de acceso por nombre (string) a los punteros a los dispositos de sonido.
 */
std::map<std::string, SoundRxPort*> SipAgent::_SndRxIds;
/**
 *	SipAgent::_Sock: pj_sock. Socket asociado al agente.
 */
pj_sock_t SipAgent::_Sock = PJ_INVALID_SOCKET;
/**
 *	SipAgent::_RemoteSock: Puntero a 'pj_activesock_t ...
 */
pj_activesock_t * SipAgent::_RemoteSock = NULL;
/**
 *	SipAgent::_NumInChannels: ...
 */
int SipAgent::_NumInChannels = 0;
/**
 *	SipAgent::_NumOutChannels: ...
 */
int SipAgent::_NumOutChannels = 0;
/**
 *	SipAgent::_InChannels: Tabla ...
 */
int SipAgent::_InChannels[10 * CORESIP_MAX_SOUND_DEVICES];
/**
 *	SipAgent::_OutChannels: Tabla ...
 */
int SipAgent::_OutChannels[10 * CORESIP_MAX_SOUND_DEVICES];
/**
 *	SipAgent::_SndDev: Puntero a pjmedia_aud_stream ...
 */
pjmedia_aud_stream * SipAgent::_SndDev = NULL;
/**
 *	SipAgent::_ConfMasterPort: Puntero a pjmedia_port ...
 */
pjmedia_port * SipAgent::_ConfMasterPort = NULL;

/**
 *	AGL 21131121. Valores de Configuración del Cancelador.
 */
unsigned SipAgent::EchoTail = 100;
unsigned SipAgent::EchoLatency = 0;
/* FM */

char SipAgent::StaticContact[256];
pjsip_sip_uri *SipAgent::pContacUrl = NULL;

char SipAgent::uaIpAdd[32];
char SipAgent::HostId[33];
CORESIP_Agent_Type SipAgent::AgentType = ULISES; 		//Es el tipo de agente.
unsigned int SipAgent::uaPort = 0;
pjsua_transport_id SipAgent::SipTransportId = PJSUA_INVALID_ID;

unsigned SipAgent::_TimeToDiscardRdInfo = 0;	//Tiempo durante el cual no se envia RdInfo al Nodebox tras un PTT OFF

pj_bool_t SipAgent::_IsRadioServerNodebox = PJ_FALSE;

/*Parametros para simulador de radio*/
unsigned SipAgent::_Radio_UA = 0;				//Con valor distinto de 0, indica que se comporta como un agente de radio

pjmedia_port *SipAgent::_Tone_Port = NULL;				//Puerto de la pjmedia para reproducir un tono
pjsua_conf_port_id SipAgent::_Tone_Port_Id = PJSUA_INVALID_ID;
stCoresip_Local_Config SipAgent::Coresip_Local_Config = {5};

pj_bool_t SipAgent::PTT_local_activado = PJ_FALSE;		//Estado del PTT local

float SipAgent::DIA_TxAttenuation = 1.0f;				//Atenuacion de las llamadas DIA en Tx (Antes de transmistir por RTP). En atenuacion de voltage
float SipAgent::IA_TxAttenuation = 1.0f;				//Atenuacion de las llamadas IA en Tx (Antes de transmistir por RTP). En atenuacion de voltage
float SipAgent::RD_TxAttenuation = 1.0f;				//Atenuacion del Audio que se transmite hacia el multicas al hacer PTT en el HMI. En atenuacion de voltage

/**
 * Init. Inicializacion de la clase:
 * - Incializa las Variables de la Clase, tanto locales como de librería.
 * - Crea el Agente SIP.
 * @param	cfg		Puntero a la configuracion local.
 * @return	Nada
 */
void SipAgent::Init(const CORESIP_Config * cfg)
{
	SipAgent::AgentType = cfg->AgentType;

	if (strlen(cfg->UserAgent) >= sizeof(cfg->UserAgent)) 
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR User Agent name is very long"));

	//if (uaCfg.user_agent.slen >= 3 && pj_strnicmp2(&uaCfg.user_agent, "ETM", 3) == 0)
	if (pj_ansi_strlen(cfg->UserAgent) >= 3 && pj_ansi_strnicmp(cfg->UserAgent, "ETM", 3) == 0)
	{
		//Si el User Agent comienza por "ETM"
		SipAgent::ETM = PJ_TRUE;
	}
	else
	{
		SipAgent::ETM = PJ_FALSE;
	}

	pj_status_t st = PJ_FALSE;
	TIMECAPS ptc;
	UINT currentPeriod = 0;
	MMRESULT ee = timeGetDevCaps(&ptc, sizeof(ptc));
	if (ee == 0)
	{
		for (currentPeriod = ptc.wPeriodMin; currentPeriod < ptc.wPeriodMax && currentPeriod <= 20; currentPeriod++)
		{
			if (20 % currentPeriod == 0)
			{
				ee = timeBeginPeriod(currentPeriod);
				SipAgent::timePeriodApplied = currentPeriod;
				break;
			}
		}
	}

	ReadiniFile();

	if (SipAgent::ETM == PJ_TRUE) Corelogfile::Init();
		
	for (unsigned i = 0; i < PJ_ARRAY_SIZE(_SndPorts); i++)
	{
		_SndPorts[i] = NULL;
	}

	for (unsigned i = 0; i < PJ_ARRAY_SIZE(_WavPlayers); i++)
	{
		_WavPlayers[i] = NULL;
	}

	for (unsigned i = 0; i < PJ_ARRAY_SIZE(_RdRxPorts); i++)
	{
		_RdRxPorts[i] = NULL;
	}

	for (unsigned i = 0; i < PJ_ARRAY_SIZE(_SndRxPorts); i++)
	{
		_SndRxPorts[i] = NULL;
	}

	for (unsigned i = 0; i < PJ_ARRAY_SIZE(_WavRecorders); i++)
	{
		_WavRecorders[i] = NULL;
	}

	for (unsigned i = 0; i < PJ_ARRAY_SIZE(GenericPort::_Generic_Ports); i++)
	{
		GenericPort::_Generic_Ports[i] = NULL;
	}

	_TimeToDiscardRdInfo = cfg->TimeToDiscardRdInfo;
	_Radio_UA = cfg->Radio_UA;

	//Ajustes de la atenuacion del audio en TX
	if (cfg->DIA_TxAttenuation_dB > CORESIP_MAX_ATTENUATION_DB)
	{
		PJ_LOG(3,(__FILE__, "ERROR: DIA_TxAttenuation_dB es demasiado grande. Lo forzamos al valor maximo %d dB", CORESIP_MAX_ATTENUATION_DB));
		DIA_TxAttenuation = (float) pow(10.0f, ((float) (-((int) CORESIP_MAX_ATTENUATION_DB)) )/20.0f);
	}
	else
	{
		DIA_TxAttenuation = (float) pow(10.0f, ((float) (-((int) cfg->DIA_TxAttenuation_dB)) )/20.0f);
	}

	if (cfg->IA_TxAttenuation_dB > CORESIP_MAX_ATTENUATION_DB)
	{
		PJ_LOG(3,(__FILE__, "ERROR: IA_TxAttenuation_dB es demasiado grande. Lo forzamos al valor maximo %d dB", CORESIP_MAX_ATTENUATION_DB));
		IA_TxAttenuation = (float) pow(10.0f, ((float) (-((int) CORESIP_MAX_ATTENUATION_DB)) )/20.0f);
	}
	else
	{
		IA_TxAttenuation = (float) pow(10.0f, ((float) (-((int) cfg->IA_TxAttenuation_dB)) )/20.0f);
	}

	if (cfg->RD_TxAttenuation_dB > CORESIP_MAX_ATTENUATION_DB)
	{
		PJ_LOG(3,(__FILE__, "ERROR: RD_TxAttenuation_dB es demasiado grande. Lo forzamos al valor maximo %d dB", CORESIP_MAX_ATTENUATION_DB));
		RD_TxAttenuation = (float) pow(10.0f, ((float) (-((int) CORESIP_MAX_ATTENUATION_DB)) )/20.0f);
	}
	else
	{
		RD_TxAttenuation = (float) pow(10.0f, ((float) (-((int) cfg->RD_TxAttenuation_dB)) )/20.0f);
	}

	/**
	 * Carga la configuracion Recibida en la configuracion local de la clase.
	 */	
	Cb = cfg->Cb;
	DefaultDelayBufPframes = cfg->DefaultDelayBufPframes;
	SndSamplingRate = cfg->SndSamplingRate;
	RxLevel = cfg->RxLevel;
	TxLevel = cfg->TxLevel;
	gJBufPframes = cfg->DefaultJBufPframes;

	/* AGL 20131121. Variables para la configuracion del Cancelador de Echo */
	EchoTail = cfg->EchoTail;
	EchoLatency = cfg->EchoLatency;

	/*
	char msg[64];
	sprintf(msg,"EchoTail=%d, EchoLatency=%d.", EchoTail, EchoLatency);
	Cb.LogCb(2,msg,strlen(msg));
	*/
	/* FM */


	/**
	 * Callback para el tratamiento de los Errores de Memoria.
	 */
	pj_pool_factory_default_policy.callback = &OnMemAllocFail;

	/**
	 * Configura otra serie de Callbacks
	 */
	pj_app_cbs.on_stream_rtp = &SipCall::OnRdRtp;
	pj_app_cbs.on_stream_rtp_ext_info_changed = &SipCall::OnRdInfoChanged;
	pj_app_cbs.on_stream_ka_timeout = &SipCall::OnKaTimeout;
	pj_app_cbs.on_create_sdp = &SDPUtils::OnCreateSdp;
	pj_app_cbs.on_stream_rtp_RTPport = &RTPport::OnRTP_Received;
	
#ifdef _ED137_
	// Heredadas de coresip PlugTest FAA 05/2011
	//pj_app_cbs.on_create_uac_tsx = &OnCreateUacTsx;
	//pjsip_app_cbs.on_create_sdp = &OnCreateSdp;
	pj_app_cbs.on_negociate_sdp = &SipCall::OnNegociateSdp;
	//pj_app_cbs.on_creating_transfer_call = &OnCreatingTransferCall;
#endif

	/**
	 * Crea el agente SIP.
	 */
	st = pjsua_create();
	PJ_CHECK_STATUS(st, ("ERROR creando agente Sip"));

	try
	{
		/**
		 * Bloques de Configuracion de 'pjsua'.
		 * - Declaracion e
		 * - Inicializacion con los valores por defecto
		 */		
		pjsua_config uaCfg;
		pjsua_logging_config logCfg;
		pjsua_media_config mediaCfg;
		pjsua_transport_config transportCfg;
		pjsua_config_default(&uaCfg);
		pjsua_logging_config_default(&logCfg);
		pjsua_media_config_default(&mediaCfg);
		pjsua_transport_config_default(&transportCfg);

		/** Para Utilizar en las CALLBACKS */
		if (strnicmp(cfg->HostId, "UV5KI", 5) == 0)
		{
			_IsRadioServerNodebox = PJ_TRUE;
		}
		sprintf(StaticContact, "<sip:%s@%s>", cfg->HostId, cfg->IpAddress);
		
		pContacUrl =  pjsip_sip_uri_create(pjsua_var.pool, 0);
		pj_strdup2(pjsua_var.pool, &pContacUrl->user, cfg->HostId);
		pj_strdup2(pjsua_var.pool, &pContacUrl->host, cfg->IpAddress);
		pContacUrl->port = cfg->Port;
		/*****/

		/**
		 * Configuracion del Bloque de Configuracion de 'pjsua'.
		 * http://www.pjsip.org/docs/latest-1/pjsip/docs/html/structpjsua__config.htm
		 */		
		if (cfg->max_calls > PJSUA_MAX_CALLS) uaCfg.max_calls = 30;		
		else uaCfg.max_calls = cfg->max_calls;		

		uaCfg.user_agent.slen = 0;
		if (strlen(cfg->UserAgent) == 0)
		{
			uaCfg.user_agent = pj_str("U5K-UA/1.0.0");
		}
		else
		{
			uaCfg.user_agent = pj_str((char*)cfg->UserAgent);
		}

		pjsip_endpt_Set_ETM(pjsua_var.endpt, SipAgent::ETM);

		uaCfg.cb.on_call_state = &SipCall::OnStateChanged;						// Trata los Cambios de estado de una Llamada.
		uaCfg.cb.on_incoming_call = &SipCall::OnIncommingCall;					// Trata la Notificacion de Llamada Entrante.
		uaCfg.cb.on_call_media_state = &SipCall::OnMediaStateChanged;			// Trata la conexion Desconexion de la Media a un dispositivo.
		uaCfg.cb.on_pjsua_reinvite_received = &SipCall::OnReinviteReceived;
		uaCfg.cb.on_call_transfer_request = &SipCall::OnTransferRequest;		// Trata la notificación de transferencia de llamada.
		uaCfg.cb.on_call_transfer_status = &SipCall::OnTransferStatusChanged;	// Trata los cambios de estado de una llamada previamente transferidad
		uaCfg.cb.on_call_tsx_state = &SipCall::OnTsxStateChanged;				// Trata cambios genericos en el estado de una llamada.
		uaCfg.cb.on_stream_created = &SipCall::OnStreamCreated;					// Gestiona la creación de los stream y su conexion a los puertos de conferencia
		uaCfg.cb.on_stream_destroyed = &SipCall::OnStreamDestroyed;				// Se llama en stop_media_session, justo antes de destruir session. Me servirá
																				// para eliminarlo del grupo climax
		uaCfg.cb.on_pager = &SipAgent::OnPager;									//Notify application on incoming pager (i.e. MESSAGE request).
		uaCfg.cb.on_call_redirected = &SipCall::On_call_redirected;				//Se llama cuando se recibe una respuesta que solicita redireccion. Por ejemplo un 302.
		uaCfg.nat_type_in_sdp = 0;												// No information will be added in SDP, and parsing is disabled.
		uaCfg.cb.on_pjsua_send_ack = &SipCall::OnSendACK;				//Se llama cuando hay que enviar el ACK.
		/** WG67_KEY_IN */
		uaCfg.cb.on_incoming_subscribe = NULL;

		/** AGL */
		uaCfg.require_timer=PJ_FALSE;
		uaCfg.require_100rel=PJ_FALSE;


		/**
		 * Configuracion del Bloque de 'LOG' de 'pjsua'.
		 */
		logCfg.msg_logging = cfg->LogLevel >= 4;
		logCfg.level = cfg->LogLevel;
		if (logCfg.level >= 6) logCfg.level = 6;
		logCfg.console_level = 3;	// cfg->LogLevel;

		if (SipAgent::ETM == PJ_TRUE)
		{			
			logCfg.console_level = logCfg.level;
			logCfg.cb = Corelogfile::Writelog;
		}
		else if (cfg->Cb.LogCb)
		{
			logCfg.decor = 0;
			logCfg.cb = Cb.LogCb;
		}
		else
		{
			char* dirname = ".\\logs";
			struct _stat buf;
			if (_stat(dirname, &buf) == -1)
			{
				logCfg.log_filename = pj_str(const_cast<char*>("./coresip.txt"));
			}
			else
			{
				logCfg.log_filename = pj_str(const_cast<char*>("./logs/coresip.txt"));
			}
		}

		/**
		 * Configuracion del Bloque MEDIA de 'pjsua'.
		 * http://www.pjsip.org/docs/latest-1/pjsip/docs/html/structpjsua__media__config.htm#a2c95e5ce554bbee9cc60d0328f508658
		 */
		mediaCfg.clock_rate = SAMPLING_RATE;
		if (SipAgent::ETM)
		{
			mediaCfg.audio_frame_ptime = 1;						//Ponemos 1 ms para que los ticks de la conferencia sea de 1ms
																//De esa forma se puede activar el PTT o SQU segun esos ticks.
																//Y no con los de 20ms de PTIME
		}
		else
		{
			mediaCfg.audio_frame_ptime = PTIME;
		}

		mediaCfg.channel_count = CHANNEL_COUNT;
		mediaCfg.max_media_ports = uaCfg.max_calls + 10;
		mediaCfg.no_vad = PJ_TRUE;
		mediaCfg.snd_auto_close_time = -1;

		/**
		 * PJSIP run-time configurations/settings.
		 * http://www.pjsip.org/pjsip/docs/html/structpjsip__cfg__t.htm
		 */
		pjsip_sip_cfg_var.tsx.t1 = cfg->TsxTout / 2;
		pjsip_sip_cfg_var.tsx.tsx_tout = cfg->TsxTout;
		pjsip_sip_cfg_var.tsx.inv_proceeding_ia_tout = cfg->InvProceedingIaTout;
		pjsip_sip_cfg_var.tsx.inv_proceeding_monitoring_tout = cfg->InvProceedingMonitoringTout;
		pjsip_sip_cfg_var.tsx.inv_proceeding_dia_tout = cfg->InvProceedingDiaTout;
		pjsip_sip_cfg_var.tsx.inv_proceeding_rd_tout = cfg->InvProceedingRdTout;
			

		/**
		 * Inicializa 'pjsua', con la configuracion establecida.
		 */
		st = pjsua_init(&uaCfg, &logCfg, &mediaCfg);
		PJ_CHECK_STATUS(st, ("ERROR inicializando Sip"));

		st = pjsip_wg67_init_module(pjsua_get_pjsip_endpt());
		PJ_CHECK_STATUS(st, ("ERROR inicialiando modulo WG67KEY-IN"));

		/**
		 * Inicializacion del módulo de conferencia.
		 */
		st = pjsip_conf_init_module(pjsua_get_pjsip_endpt());
		PJ_CHECK_STATUS(st, ("ERROR inicialiando modulo conference"));

		/**
		 * Inicializacion del módulo para eventos de dialogo.
		 */
		st = pjsip_dialog_init_module(pjsua_get_pjsip_endpt());
		PJ_CHECK_STATUS(st, ("ERROR inicialiando modulo dialog"));

		if (cfg->DefaultCodec[0])
		{
			pjsua_codec_set_priority(&(pj_str(const_cast<char*>(cfg->DefaultCodec))), PJMEDIA_CODEC_PRIO_HIGHEST);
		}

		/**
		 * Registra el modulo en la libreria.
		 */
		st = pjsip_endpt_register_module(pjsua_var.endpt, &coresip_mod);
		PJ_CHECK_STATUS(st, ("ERROR registrando modulo coresip"));

		/*
		Se registra el modulo para los mensajes options
		*/
		st = OptionsMod::RegisterModule();
		PJ_CHECK_STATUS(st, ("ERROR registrando modulo OPTIONS"));

		/**
		 * Inicializa la configuracion del Bloque de Transporte.
		 * http://www.pjsip.org/docs/latest-1/pjsip/docs/html/structpjsua__transport__config.htm
		 */
		transportCfg.port = cfg->Port;
		transportCfg.public_addr = pj_str(const_cast<char*>(cfg->IpAddress));
		transportCfg.bound_addr = pj_str(const_cast<char*>(cfg->IpAddress));
		strcpy(SipAgent::uaIpAdd, cfg->IpAddress);
		strcpy(SipAgent::HostId, cfg->HostId);
		SipAgent::uaPort = cfg->Port;
		transportCfg.options = PJMEDIA_UDP_NO_SRC_ADDR_CHECKING;

		/**
		 * Crea el Transporte para SIP. UDP en puerto Base.
		 */
		st = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &transportCfg, &SipTransportId);
		if (st != PJ_SUCCESS) SipTransportId = PJSUA_INVALID_ID;
		PJ_CHECK_STATUS(st, ("ERROR creando puertos UDP para Sip", "(%s:%d)", cfg->IpAddress, cfg->Port));	

		pj_sock_t sip_socket = pjsip_udp_transport_get_socket(pjsua_var.tpdata[SipTransportId].data.tp);
		if (sip_socket != PJ_INVALID_SOCKET)
		{
			//Se fuerza que los paquetes salgan por el interfaz que utiliza el agente.
			struct pj_in_addr in_uaIpAdd;
			pj_inet_aton((const pj_str_t *) &pj_str(const_cast<char*>(cfg->IpAddress)), &in_uaIpAdd);
			st = pj_sock_setsockopt(sip_socket, IPPROTO_IP, IP_MULTICAST_IF, (void *)&in_uaIpAdd, sizeof(in_uaIpAdd));
			if (st != PJ_SUCCESS)
				PJ_LOG(3,(__FILE__, "ERROR: setsockopt, PJ_IP_MULTICAST_IF. El transporte SIP no se puede forzar por el interface %s", cfg->IpAddress));
		}

		/**
		 * Crea el Transporte para RTP
		 */
		if (cfg->RtpPorts == 0)
		{
			unsigned int porto = cfg->Port + 2;
			//st = pjsua_media_transports_create(&transportCfg);
			//PJ_CHECK_STATUS(st, ("ERROR creando puertos UDP para RTP", "(%s:%d)", cfg->IpAddress, transportCfg.port));

#ifdef _ULISES_

			//Intentamos varias veces abrir los puertos rtp. 
			//Por si acaso hay otra aplicacion que este abriendo puertos a la vez que esta

			int tries = 5;
			while (tries > 0)
			{
				if (GetRTPPort(&porto) < 0)
				{
					PJ_CHECK_STATUS(PJ_EUNKNOWN, ("ERROR No se pueden obtener los puertos libres necesarios para RTP"));
				}
				transportCfg.port = porto;	
				st = pjsua_media_transports_create(&transportCfg);
				if (st == PJ_SUCCESS)
				{
					break;
				}
				pj_thread_sleep(250);
				tries--;
			}
			if (tries == 0)
			{
				PJ_CHECK_STATUS(PJ_EUNKNOWN, ("ERROR No se pueden obtener los puertos para RTP", " despues de %d intentos", tries));
			}
#else
			transportCfg.port = porto;	
			st = pjsua_media_transports_create(&transportCfg);
			PJ_CHECK_STATUS(st, ("ERROR creando puertos UDP para Sip", "(%s:%d)", cfg->IpAddress, cfg->Port));
#endif
		}
		else
		{
			transportCfg.port = cfg->RtpPorts;

			st = pjsua_media_transports_create(&transportCfg);
			PJ_CHECK_STATUS(st, ("ERROR creando puertos UDP para RTP", "(%s:%d)", cfg->IpAddress, transportCfg.port));
		}

		PJ_LOG(3,(__FILE__, "Puertos para RTP reservados: (%u - %u), para un maximo de %d sesiones", transportCfg.port, transportCfg.port+(pjsua_call_get_max_count()*2)-1, pjsua_call_get_max_count()));

		/**
		 * Crea un control de acceso a la memoria del agente.
		 */
		st = pj_lock_create_recursive_mutex(pjsua_var.pool, NULL, &_Lock);
		PJ_CHECK_STATUS(st, ("ERROR creando seccion critica"));

		/**
		 * Crea mutex para acceso al cancelador de eco LC-mic.
		 */
		pj_status_t st = pj_lock_create_recursive_mutex(pjsua_var.pool, NULL, &_ECLCMic_mutex);
		PJ_CHECK_STATUS(st, ("ERROR creando _ECLCMic_mutex"));

		CUtils::Init(pjsua_var.pool);

		/**
			* Se crea el puerto pjmedia para la grabacion
		 */
		RecordPort::Init(pjsua_var.pool, cfg->RecordingEd137, cfg->AgentType, cfg->IpAddress, cfg->HostId);

		_FrecDesp = new FrecDesp;

		_PresenceManager = new PresenceManag;	//Se inicializa la gestion de presencias
		_PresenceManager->SetPresenceSubscriptionCallBack(cfg->Cb.Presence_callback);

		_ConfManager = new ClientSubscriptionManager<ConfSubs>;			//se inicializa el gestor de subscripcion conferencias
		_DlgManager = new ClientSubscriptionManager<DlgSubs>;			//se inicializa el gestor de subscripcion de dialogo
		_WG67Manager = new ClientSubscriptionManager<WG67Subs>;			//se inicializa el gestor de subscripcion de WG67KEY-IN

		if (!_IsRadioServerNodebox && !ETM)
		{
			SoundDevHw::Init();
		}

#ifdef CHECK_QIDX_LOGARITHM

		//Creamos un puerto de media que reproduzca un tono

		if (SipAgent::_Radio_UA)
		{			
			//Creamos un generador de tono para enviar cuando hay squelch
			st = pjmedia_tonegen_create(pjsua_var.pool, SAMPLING_RATE, CHANNEL_COUNT, SAMPLES_PER_FRAME, BITS_PER_SAMPLE, PJMEDIA_TONEGEN_LOOP, &_Tone_Port);
			if (st != PJ_SUCCESS)
			{
				PJ_LOG(3,(__FILE__, "ERROR: SipCall: No se puede crear _Tone_Port"));
				_Tone_Port = NULL;
			}
			else
			{
				st = pjsua_conf_add_port(pjsua_var.pool, _Tone_Port, &_Tone_Port_Id);
				if (st != PJ_SUCCESS)
				{
					PJ_LOG(3,(__FILE__, "ERROR: SipCall: pjsua_conf_add_port para obtener _Tone_Port_Id retorna error"));
					_Tone_Port_Id = PJSUA_INVALID_ID;
				}
				else
				{
					pjmedia_tone_desc tonegen_desc[1];

					tonegen_desc[0].freq1 = 1000;
					tonegen_desc[0].freq2 = 600;
					tonegen_desc[0].on_msec = 20;
					tonegen_desc[0].off_msec = 0;
					tonegen_desc[0].flags = 0;

					st = pjmedia_tonegen_play(_Tone_Port, 1, tonegen_desc, PJMEDIA_TONEGEN_LOOP);
					if (st != PJ_SUCCESS)
					{
						PJ_LOG(3,(__FILE__, "ERROR: SipCall: pjmedia_tonegen_play retorna error"));
					}						
				}
			}
		}
#endif

#ifdef PJ_USE_ASIO
		
		UseDefaultSoundDevices = cfg->UseDefaultSoundDevices;
		if (UseDefaultSoundDevices != 0)
		{
			PaDeviceIndex DevIndexCount = Pa_GetDeviceCount();
			PaDeviceInfo* Pa_DevInfo = NULL;

			for (PaDeviceIndex i = 0; i < DevIndexCount; i++)
			{
				Pa_DevInfo = (PaDeviceInfo*)Pa_GetDeviceInfo(i);
				PJ_LOG(3, (__FILE__, "INFO: SipAgent::Init PaDeviceInfo->name %s, maxInputChannels %d, maxOutputChannels %d", 
					Pa_DevInfo->name, Pa_DevInfo->maxInputChannels, Pa_DevInfo->maxOutputChannels));
			}

			Pa_DefaultInputDev = Pa_GetDefaultInputDevice();
			PJ_LOG(3, (__FILE__, "INFO: SipAgent::Init Defaul Input Device %d", Pa_DefaultInputDev));

			Pa_DefaultOutputDev = Pa_GetDefaultOutputDevice();
			PJ_LOG(3, (__FILE__, "INFO: SipAgent::Init Defaul Output Device %d", Pa_DefaultInputDev));
		}
		else
		{
			Pa_DefaultInputDev = paNoDevice;
			Pa_DefaultOutputDev = paNoDevice;
		}		
#endif

		PJ_LOG(3, (__FILE__, "INFO: wPeriodMax %u ms wPeriodMin %u ms establecida %u", ptc.wPeriodMax, ptc.wPeriodMin, currentPeriod));


#undef TEST_VOLUMEN 

		if (!_IsRadioServerNodebox && !ETM)
		{
			try
			{
				CORESIP_SndWindowsDevices Devices;
				SoundDevHw::GetWindowsSoundDeviceNames(1, &Devices, PJ_FALSE, NULL);
				PJ_LOG(3, (__FILE__, "INFO: ------------------------------------------------"));
				PJ_LOG(3, (__FILE__, "INFO: SipAgent::Init Dispositivos de entrada manejados por API Windows:"));
				if (Devices.ndevices_found > 0)
				{
					std::string devnames = Devices.DeviceNames;
					size_t pos = 0;
					size_t splitter_pos = 0;
					for (unsigned int i = 0; i < Devices.ndevices_found; i++)
					{
						splitter_pos = devnames.find(SoundDevHw::splitter, pos);
						std::string name;
						if (splitter_pos != std::string::npos)
						{
							name = devnames.substr(pos, splitter_pos - pos);
							pos = splitter_pos + strlen(SoundDevHw::splitter);
						}
						else
						{
							name = devnames.substr(pos);
						}
						char cname[CORESIP_MAX_SOUND_NAME_LENGTH];
						unsigned int j;
						for (j = 0; j < name.length() && j < (CORESIP_MAX_SOUND_NAME_LENGTH - 1); j++)
						{
							cname[j] = name[j];
						}
						cname[j] = '\0';
						PJ_LOG(3, (__FILE__, "%s", cname));

#if 0

						std::map<CORESIP_SndDevType, std::string>::iterator it;
						if (name.find("CWP USB Device # 01 2") != std::string::npos) {
							SoundDevHw::SetSNDDeviceWindowsName(CORESIP_SND_ALUMN_MHP, cname);
						}
						else if (name.find("CWP USB Device # 02 2") != std::string::npos) {
							SoundDevHw::SetSNDDeviceWindowsName(CORESIP_SND_INSTRUCTOR_MHP, cname);
						}
						else if (name.find("CWP USB Device # 03 2") != std::string::npos) {
							SoundDevHw::SetSNDDeviceWindowsName(CORESIP_SND_RD_SPEAKER, cname);
						}
						else if (name.find("CWP USB Device # 04 2") != std::string::npos) {
							SoundDevHw::SetSNDDeviceWindowsName(CORESIP_SND_LC_SPEAKER, cname);
						}
#endif 
					}
				}
				PJ_LOG(3, (__FILE__, "INFO: ------------------------------------------------"));
				SoundDevHw::GetWindowsSoundDeviceNames(0, &Devices, PJ_FALSE, NULL);
				PJ_LOG(3, (__FILE__, "INFO: SipAgent::Init Dispositivos de salida manejados por API Windows:"));
				if (Devices.ndevices_found > 0)
				{
					std::string devnames = Devices.DeviceNames;
					size_t pos = 0;
					size_t splitter_pos = 0;
					for (unsigned int i = 0; i < Devices.ndevices_found; i++)
					{
						splitter_pos = devnames.find(SoundDevHw::splitter, pos);
						std::string name;
						if (splitter_pos != std::string::npos)
						{
							name = devnames.substr(pos, splitter_pos - pos);
							pos = splitter_pos + strlen(SoundDevHw::splitter);
						}
						else
						{
							name = devnames.substr(pos);
						}
						char cname[CORESIP_MAX_SOUND_NAME_LENGTH];
						unsigned int j;
						for (j = 0; j < name.length() && j < (CORESIP_MAX_SOUND_NAME_LENGTH - 1); j++)
						{
							cname[j] = name[j];
						}
						cname[j] = '\0';
						PJ_LOG(3, (__FILE__, "%s", cname));

#ifdef TEST_VOLUMEN
						std::map<CORESIP_SndDevType, std::string>::iterator it;
						if (name.find("CWP USB Device # 01 2") != std::string::npos) {
							SoundDevHw::SetSNDDeviceWindowsName(CORESIP_SND_ALUMN_MHP, cname);
						}
						else if (name.find("CWP USB Device # 02 2") != std::string::npos) {
							SoundDevHw::SetSNDDeviceWindowsName(CORESIP_SND_INSTRUCTOR_MHP, cname);
						}
						else if (name.find("CWP USB Device # 03 2") != std::string::npos) {
							SoundDevHw::SetSNDDeviceWindowsName(CORESIP_SND_RD_SPEAKER, cname);
						}
						else if (name.find("CWP USB Device # 04 2") != std::string::npos) {
							SoundDevHw::SetSNDDeviceWindowsName(CORESIP_SND_LC_SPEAKER, cname);
						}
#endif // TEST_VOLUMEN
					}
				}
				PJ_LOG(3, (__FILE__, "INFO: ------------------------------------------------"));
			}
			catch (PJLibException& ex)
			{
				PJ_LOG(3, (__FILE__, "INFO: GetWindowsSoundDevices: %s", ex.Info));
			}
			catch (...)
			{
				PJ_LOG(3, (__FILE__, "INFO: GetWindowsSoundDevices no ha podido ser ejecutado"));
			}
		}

		if (ETM)
		{
			//Si es un ETM entonces el Max forward por defecto es 70
			pjsip_endpt_modify_max_forward_header(pjsua_var.endpt, 70);
		}
	}
	catch (...)
	{
		pjsua_destroy();
		throw;
	}
}

/**
 * Start. Rutina de Arranque del módulo. Esta rutina, está realizada para ser utilizada con el Driver ASIO.
 * @return	Nada
 */
void SipAgent::Start()
{

#ifdef PJ_USE_ASIO

	if (UseDefaultSoundDevices != 0)
	{
		CORESIP_SndDeviceInfo info;
		info.Type = CORESIP_SND_ALUMN_MHP;
		info.OsInDeviceIndex = Pa_DefaultInputDev;
		info.OsOutDeviceIndex = Pa_DefaultOutputDev;
		DefaultSndDeviceID = SipAgent::AddSndDevice(&info);
	}
	else
	{
		DefaultSndDeviceID = -1;
	}

	/**
	 * Solo si hay Canales (puertos) de audio de Entrada y/o salida.
	 */
	if ((_NumInChannels + _NumOutChannels) > 0)
	{
		/**
		 * Desconecta el Puerto por defecto que asigna la librería a los modulos.
		 * INFO: http://www.pjsip.org/pjsip/docs/html/group__PJSUA__LIB__MEDIA.htm#ga4b6ffc203b8799f08f072d0921777161
		 */
		_ConfMasterPort = pjsua_set_no_snd_dev();
		/** 
		 * Desconecta los puertos, también del módulo de Conferencias.
		 */
		pjmedia_conf_configure_port(pjsua_var.mconf, 0, PJMEDIA_PORT_DISABLE, PJMEDIA_PORT_DISABLE);

		/**
		 * Para configurar canales 'pjmedia' que utilicen ASIO.
		 */
		pjmedia_aud_param param;
		PaAsioStreamInfo inInfo = { sizeof(PaAsioStreamInfo), paASIO, 1, paAsioUseChannelSelectors, _InChannels };
		PaAsioStreamInfo outInfo = { sizeof(PaAsioStreamInfo), paASIO, 1, paAsioUseChannelSelectors, _OutChannels };

		pj_status_t st = pjmedia_aud_dev_default_param(0, &param);		
		PJ_CHECK_STATUS(st, ("ERROR configurando dispositivo de sonido"));

		/**
		 * Rellena los parámetros 'pjmedia', en relacion a la gestion del audio.
		 */
		param.dir = (pjmedia_dir)((_NumInChannels > 0 ? PJMEDIA_DIR_CAPTURE : 0) | (_NumOutChannels > 0 ? PJMEDIA_DIR_PLAYBACK : 0));
		param.clock_rate = SipAgent::SndSamplingRate;
		param.channel_count = CHANNEL_COUNT;
		param.in_channel_count = _NumInChannels;
		param.out_channel_count = _NumOutChannels;
		param.samples_per_frame = ((SipAgent::SndSamplingRate * CHANNEL_COUNT * PTIME) / 1000);
		param.in_samples_per_frame = param.samples_per_frame * _NumInChannels;
		param.out_samples_per_frame = param.samples_per_frame * _NumOutChannels;
		param.bits_per_sample = BITS_PER_SAMPLE;
		param.non_interleaved = PJ_TRUE;
		param.inHostApiSpecificStreamInfo = &inInfo;
		param.outHostApiSpecificStreamInfo = &outInfo;

		/**
		 * Crea el STREAM de audio.
		 *	- RecCB: Callback Recepcion STREAM audio
		 *	- PlayCB: Callback Fin Transmision STREAM audio.
		 *
		 * INFO: http://www.pjsip.org/pjmedia/docs/html/group__s2__audio__device__reference.htm#ga476ecc42ccb9744bbc40e42d457ad1aa
		 */
		st = pjmedia_aud_stream_create(&param, &RecCb, &PlayCb, NULL, &_SndDev);
		PJ_CHECK_STATUS(st, ("ERROR creando dispositivo de sonido"));

		try
		{
			/**
			 * Arranca el STREAM de audio.
			 */
			st = pjmedia_aud_stream_start(_SndDev);
			PJ_CHECK_STATUS(st, ("ERROR iniciando dispositivo de sonido"));
		}
		catch (...)
		{
			pjmedia_aud_stream_destroy(_SndDev);
			_SndDev = NULL;
			throw;
		}
	}
	else
	{
		pj_status_t st = pjsua_set_null_snd_dev();
		PJ_CHECK_STATUS(st, ("ERROR creando reloj para el mezclador"));
	}
#else
	pj_status_t st = pjsua_set_null_snd_dev();
	PJ_CHECK_STATUS(st, ("ERROR Hay que utilizar ASIO!!!!"));
#endif


	pj_status_t st = pjsua_start();
	PJ_CHECK_STATUS(st, ("ERROR arrancando agente Sip"));	

	SIP_AGENT_INITIALIZED_AND_STARTED = PJ_TRUE;
}

/**
 * Stop. Rutina de Parada del Módulo
 */
void SipAgent::Stop()
{
	SIP_AGENT_INITIALIZED_AND_STARTED = PJ_FALSE;

	if (!_IsRadioServerNodebox && !ETM)
	{
		SoundDevHw::Dispose();
	}

	if (_Lock != NULL)
	{
		/**
		 * Cuelga todas las llamadas activas en el módulo.
		 */
		pj_bzero(&SipAgent::Cb, sizeof(SipAgent::Cb));
		pj_bzero(&pjsua_var.ua_cfg.cb, sizeof(pjsua_var.ua_cfg.cb));	//Inhibimos todas las callbacks de la aplicacion

		pjsua_call_id call_ids[PJSUA_MAX_CALLS];
		unsigned call_cnt = PJ_ARRAY_SIZE(call_ids);

		pj_status_t st = pjsua_enum_calls(call_ids, &call_cnt);
		if (st != PJ_SUCCESS)
		{
			PJ_LOG(3, (__FILE__, "ERROR: SipAgent::Stop: No se pueden obtener las llamadas del account"));
			return;
		}

		for (unsigned i = 0; i < call_cnt; i++)
		{
			SipCall::Force_Hangup(call_ids[i], 0);			
			pj_thread_sleep(250);
		}

		/**
		 * Espera a que todas las llamadas se hayan cerrado.
		 */

		for (int i = 0; i < PJ_ARRAY_SIZE(pjsua_var.calls); ++i)
		{
			if (pjsua_var.calls[i].inv != NULL)
			{
				int cnt = 0;
				while (cnt < 10 && pjsua_var.calls[i].inv != NULL)
				{
					pj_thread_sleep(100);
					cnt++;
				}
				// goto CHECK_CALLS;
			}
		}

		/* Si el publish esta habilitado. He visto que el servidor de presencia
		tarda en contestar a veces mas de 400ms. en cuyo caso se destruye acc->publish_sess
		y cuando se cierra la aplicacion no envia el PUBLISH con expires a cero.
		Aqui me aseguro de que se va a enviar*/
		for (int i = 0; i < PJ_ARRAY_SIZE(pjsua_var.acc); ++i) {
			pjsua_acc* acc = &pjsua_var.acc[i];
			if (acc->valid)
			{
				if (acc->cfg.publish_enabled && acc->publish_sess == NULL)
				{
					pjsua_acc_set_online_status((pjsua_acc_id)i, PJ_FALSE);	//Deja de publicarse en el servidor de presencia	
					pjsua_pres_init_publish_acc(acc->index);
				}
			}
		}	

		Guard lock(_Lock);

		RTPport::DestroyAllRTPports();

		if (_PresenceManager) 
		{
			delete _PresenceManager;
			_PresenceManager = NULL;
		}

		if (_ConfManager)
		{
			delete _ConfManager;
			_ConfManager = NULL;
		}

		if (_DlgManager)
		{
			delete _DlgManager;
			_DlgManager = NULL;
		}

		if (_WG67Manager)
		{
			delete _WG67Manager;
			_WG67Manager = NULL;
		}

		/**
		 * Cierra el Socket Remoto ???
		 */
		if (_RemoteSock)
		{
			pj_activesock_close(_RemoteSock);
			_RemoteSock = NULL;
		}

		//Libera FD
		if (_FrecDesp)
		{
			delete _FrecDesp;
			_FrecDesp = NULL;
		}

		RecordPort::End();

		/**
		 * Libera los 'puertos'.
		 */
		for (unsigned i = 0; i < PJ_ARRAY_SIZE(_SndPorts); i++)
		{
			if (_SndPorts[i])
			{
				delete _SndPorts[i];
				_SndPorts[i] = NULL;
			}
		}

		/**
		 * Libera los 'reproductors wav'.
		 */
		for (unsigned i = 0; i < PJ_ARRAY_SIZE(_WavPlayers); i++)
		{
			if (_WavPlayers[i])
			{
				delete _WavPlayers[i];
				_WavPlayers[i] = NULL;
			}
		}

		/**
		 * Libera los 'grabadores wav'.
		 */
		for (unsigned i = 0; i < PJ_ARRAY_SIZE(_WavRecorders); i++)
		{
			if (_WavRecorders[i])
			{
				delete _WavRecorders[i];
				_WavRecorders[i] = NULL;
			}
		}

		/**
		 * Libera los puertos de Recepcion Radio.
		 */
		for (unsigned i = 0; i < PJ_ARRAY_SIZE(_RdRxPorts); i++)
		{
			if (_RdRxPorts[i])
			{
				delete _RdRxPorts[i];
				_RdRxPorts[i] = NULL;
			}
		}

		/**
		 * Libera los puertos de Recepcion Audio.
		 */
		for (unsigned i = 0; i < PJ_ARRAY_SIZE(_SndRxPorts); i++)
		{
			if (_SndRxPorts[i])
			{
				delete _SndRxPorts[i];
				_SndRxPorts[i] = NULL;
			}
		}

		/**
		 * Libera la memoria asociada al modulo.
		 */
		_SndRxIds.clear();
		memset(_InChannels, 0, sizeof(_InChannels));
		memset(_OutChannels, 0, sizeof(_OutChannels));

		if (_EchoCancellerLCMic != NULL)
		{
			EchoCancellerLCMic(false);
		}

		pjmedia_aud_stream *_SndDev_aux = _SndDev;
		_SndDev = NULL;		

		lock.Unlock();

		if (_SndDev_aux)
		{
			pjmedia_aud_stream_stop(_SndDev_aux);
			pjmedia_aud_stream_destroy(_SndDev_aux);
		}

		pj_thread_sleep(100);

		if (SipTransportId != PJSUA_INVALID_ID)
		{
			pjsua_transport_close(SipTransportId, PJ_FALSE);
			SipTransportId = PJSUA_INVALID_ID;
		}

		pj_thread_sleep(100);

		pj_lock_destroy(_Lock);

		CUtils::End();

		SipAgent::ETM = PJ_FALSE;

		pjsua_destroy();

		if (SipAgent::timePeriodApplied != 0) timeEndPeriod(SipAgent::timePeriodApplied);
	}
}

/**
 * SetSipPort.				Establece el puerto SIP
 * @param	port			Puerto SIP
 * @return	Nada
 */
void SipAgent::SetSipPort(unsigned int port)
{
	pj_status_t st = PJ_SUCCESS;

	st = pjsua_transport_close(SipTransportId, PJ_FALSE);
	PJ_CHECK_STATUS(st, ("ERROR cerrando SIP transport", " SipTransportId = %d ", SipTransportId));

	SipTransportId = PJSUA_INVALID_ID;

	pjsua_transport_config transportCfg;
	pjsua_transport_config_default(&transportCfg);
	transportCfg.port = port;
	transportCfg.bound_addr = pj_str(const_cast<char*>(SipAgent::uaIpAdd));
	transportCfg.options = PJMEDIA_UDP_NO_SRC_ADDR_CHECKING;

	st = pjsua_transport_create(PJSIP_TRANSPORT_UDP, &transportCfg, &SipTransportId);
	PJ_CHECK_STATUS(st, ("ERROR creando puertos UDP para Sip", "(%s:%d)", SipAgent::uaIpAdd, port));
}

/**
 * SetRTPPort.				Establece el primer puerto RTP para la media.
 * @param	port			Puerto SIP
 * @return	Nada
 */
void SipAgent::SetRTPPort(unsigned int port)
{
	pj_status_t st = PJ_SUCCESS;

	pjsua_transport_config transportCfg;
	pjsua_transport_config_default(&transportCfg);
	transportCfg.port = port;
	transportCfg.bound_addr = pj_str(const_cast<char*>(SipAgent::uaIpAdd));
	transportCfg.options = PJMEDIA_UDP_NO_SRC_ADDR_CHECKING;

	//La siguiente funcion crea los transports para el RTP. Internamente borra los que tuviera creados.
	//Crea un par de sockets (rtp/rtcp) para cada llamada. Por tanto en total crea pjsua_var.ua_cfg.max_calls*2
	st = pjsua_media_transports_create(&transportCfg);
	PJ_CHECK_STATUS(st, ("ERROR creando puertos UDP para RTP", "(%s:%d)", SipAgent::uaIpAdd, port));
}

#ifdef _ULISES_
/**
 * GetRTPPort.				Obtiene el primer puerto RTP/RTCP. Busca un hueco de puertos UDP libres en el S.O. 
 *							para todas las posibles llamadas que pueda utilizar la CORESIP.
 * @param					port. Valor del puerto obtenido
 * @return					-1 si a ocurrido algún error.
 */
int SipAgent::GetRTPPort(unsigned int *port)
{	
	int ret = 0;

	//Para cada llamada es necesario dos sockets, uno para rtp y otro para rtcp.
	//Por tanto buscamos en nuestro sistema operativo una cantidad de puertos consecutivos
	//igual a pjsua_var.ua_cfg.max_calls*2
	//Empezamos por los puertos altos. Concretamente desde el 64998u y hacia atrás. Dejamos el 65000 porque es el que se utiliza para la grabacion.
	
	int max_rtp_rtcp_ports = pjsua_call_get_max_count() * 2;			//pjsua_var.ua_cfg.max_calls*2
	
	PMIB_UDPTABLE pUdpTable;
	unsigned short *pUdpPorts;
	DWORD dwSize = 0;
	DWORD dwRetVal = 0;

	unsigned short *port_ptr;

	/* Get size required by GetUdpTable() */
	if (GetUdpTable(NULL, &dwSize, 0) == ERROR_INSUFFICIENT_BUFFER) 
	{
		pUdpTable = (MIB_UDPTABLE *) malloc (dwSize);	
		if (pUdpTable == NULL)
		{
			PJ_CHECK_STATUS(PJ_ENOMEM, ("ERROR. GetUdpTable retorna con error. No se puede obtener la lista de puertos UDP utilizados"));				
			ret = -1;
			return ret;
		}
	}
	else
	{
		ret = -1;
	}

	unsigned short limite_superior = 64998u;
	unsigned short puerto_obtenido = -1;

	if (ret == 0)
	{
		/* Get actual data using GetUdpTable() */
		//Obtenemos todos los puertos que utiliza nuestro sistema operativo, ordenados de menor a mayor
		//Buscamos un hueco entre puertos utilizados donde podamos utilizar todos los puertos que requerimos. Es decir, max_rtp_rtcp_ports
		//Empezamos por el puerto mas alto ulilizado de la tabla obtenida y calculamos la diferencia con limite_superior.
		//Si la diferencia es mayor que la cantidad de puertos requeridos entonces ya tenemos hueco.
		//Si no hay hueco entonces limite_superior toma el valor del puerto de la tabla que hemos utilizado, así hasta obtener el hueco.	

		if ((dwRetVal = GetUdpTable(pUdpTable, &dwSize, 0)) == NO_ERROR) 
		{
			if (pUdpTable->dwNumEntries > 0) 
			{
				//Guardamos los puertos en un array
				pUdpPorts = (unsigned short *) malloc (pUdpTable->dwNumEntries * sizeof(unsigned short));
				if (pUdpPorts == NULL)
				{
					PJ_CHECK_STATUS(PJ_ENOMEM, ("ERROR. GetUdpTable retorna con error. No se puede obtener la lista de puertos UDP utilizados"));
					free(pUdpTable);
					ret = -1;
					return ret;
				}

				for (unsigned int i = 0; i < pUdpTable->dwNumEntries; i++)
				{
					port_ptr = (unsigned short *)&pUdpTable->table[i].dwLocalPort;
					unsigned short used_port = htons(*port_ptr);
					pUdpPorts[i] = used_port;
				}

				//Ordenamos el array de menor puerto a mayor
				for (unsigned int i = 0; i < pUdpTable->dwNumEntries-1; i++)
				{
					for (unsigned j = i+1; j < pUdpTable->dwNumEntries; j++)
					{				
						if (pUdpPorts[i] > pUdpPorts[j])
						{
							unsigned short aux = pUdpPorts[i];
							pUdpPorts[i] = pUdpPorts[j];
							pUdpPorts[j] = aux;
						}
					}
				}

				PJ_LOG(5,(__FILE__, "Puertos usados: #############################################"));
				for (unsigned int i = 0; i < pUdpTable->dwNumEntries; i++)
				{					
					PJ_LOG(5,(__FILE__, "Puerto usado: %u", pUdpPorts[i]));
				}
				PJ_LOG(5,(__FILE__, "#############################################"));
				
				for (unsigned int i = pUdpTable->dwNumEntries-1; i >= 0; i--)
				{			

					unsigned short nport = pUdpPorts[i];
					//Le sumanos 2 y nos aseguramos de que es par
					nport += 2;
					if ((nport % 2) != 0)
					{
						nport += 1;
					}
										
					if (nport < limite_superior)
					{
						//El valor tiene que ser menor que el de limite_superior, si no es asi no hacemos nada y tomaremos el siguiente mas bajo de la tabla

						if ((limite_superior - nport) > max_rtp_rtcp_ports)
						{
							//Hemos encontrado un hueco donde caben todos los puertos que necesitamos
							puerto_obtenido = limite_superior - max_rtp_rtcp_ports;
							break;
						}
						else
						{
							//Si no cabe entonces el limite superior cambia al del valor de la tabla que hemos utilizado
							limite_superior = pUdpPorts[i];

							//Si es impar le restamos 1
							if ((limite_superior % 2) != 0)
							{
								limite_superior -= 1;
							}
						}
					}					
				}

				free(pUdpPorts);

			}
			else
			{
				ret = -1;
			}
		} 
		else 
		{
			ret = -1;
		}

		free(pUdpTable);
	}

	if (ret == -1)
	{
		PJ_CHECK_STATUS(PJ_EUNKNOWN, ("ERROR. GetUdpTable retorna con error. No se puede obtener la lista de puertos UDP utilizados"));				
	}

	if (puerto_obtenido < 0)
	{
		//No hemos encontrado hueco
		PJ_CHECK_STATUS(PJ_EUNKNOWN, ("ERROR. No se encuentra una cantidad de puertos RTP/RTCP consecutivos suficientes en el S.O."));
		ret = -1;
	}
	else
	{
		*port = puerto_obtenido;
	}

	return ret;
}
#else
int SipAgent::GetRTPPort(unsigned int *port)
{
	return 0;
}
#endif

void SipAgent::Set_Ed137_version(char ED137Radioversion, char ED137Phoneversion)
{
	if (ED137Radioversion != 'B' && ED137Radioversion != 'C')
	{
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR Set_Ed137_version. ED137Radioversion solo puede valer B o C"));
	}
	if (ED137Phoneversion != 'B' && ED137Phoneversion != 'C')
	{
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR Set_Ed137_version. ED137Phoneversion solo puede valer B o C"));
	}

	pjsip_endpt_Set_Ed137_version(pjsua_var.endpt, ED137Radioversion, ED137Phoneversion);
}

void SipAgent::Get_Ed137_version(char *ED137Radioversion, char *ED137Phoneversion)
{	
	char ED137Radioversion_, ED137Phoneversion_;
	pjsip_endpt_Get_Ed137_version(pjsua_var.endpt, &ED137Radioversion_, &ED137Phoneversion_);

	if (ED137Radioversion != NULL) *ED137Radioversion = ED137Radioversion_;
	if (ED137Phoneversion != NULL) *ED137Phoneversion = ED137Phoneversion_;
}

/**
* Fuerza la cabecera de WG67-version
* @param force. Si el valor es 0 entonces no se fuerza el valor de la cabecera
* @param ED137Radioversion	Valor que se fuerza a la cabecera WG67-version, en caso de que force sea 1
*/
void SipAgent::Force_Ed137_version_header(int force, char* ED137Radioversion)
{
	pjsip_endpt_Force_Ed137_version_header(pjsua_var.endpt, force, ED137Radioversion);
}

/**
 * SetLogLevel. Configura el Nivel de LOG de la clase.
 * @param	level	Nivel de Log
 * @return	Nada
 */
void SipAgent::SetLogLevel(unsigned level)
{
	pjsua_logging_config logCfg;
	pjsua_logging_config_default(&logCfg);

	logCfg.msg_logging = level >= 4;
	logCfg.level = level;
	logCfg.console_level = level;

	if (Cb.LogCb)
	{
		logCfg.decor = 0;
		logCfg.cb = Cb.LogCb;
	}
	else
	{
		logCfg.log_filename = pj_str(const_cast<char*>("./logs/coresip.txt"));
	}

	pjsua_reconfigure_logging(&logCfg);
}

/**
 * SetParams. Configura la lista de parámetros del módulo.
 * @param	MaxForwards	Valor de la cabecera Max-Forwards. Si vale NULL se ignora.
 * @param	Options_answer_code		Codigo de la respuesta a los mensajes OPTIONS (200, 404, etc.)
 *									Si el codigo es 0, entonces no se envia respuesta
 *									Si se pasa un NULL, este parametro se ignora.
 * @return	Nada
 */
void SipAgent::SetParams(const int * MaxForwards, const int * Options_answer_code)
{
	if (MaxForwards != NULL)
	{
		pjsip_endpt_modify_max_forward_header(pjsua_var.endpt, (pj_int32_t) *MaxForwards);
	}

	if (Options_answer_code != NULL)
	{
		pjsip_endpt_modify_options_answer(pjsua_var.endpt, *Options_answer_code);
	}
}

void SipAgent::SetJitterBuffer(unsigned adaptive, unsigned initial_prefetch, unsigned min_prefetch, unsigned max_prefetch, unsigned discard)
{
	/*Tomamos el valor de ptime para el codec pcma que es igual que el pcmu*/
	pjmedia_codec_mgr* codec_mgr = pjmedia_endpt_get_codec_mgr(pjsua_var.med_endpt);
	const pjmedia_codec_info* codec_info;
	pjmedia_codec_param codec_param;
	if (!codec_mgr)
		PJ_CHECK_STATUS(PJ_EUNKNOWN, (__FILE__, "ERROR SetJitterBuffer pjmedia_endpt_get_codec_mgr returns error"));
	unsigned count = 1;
	pj_str_t codec_id = pj_str("pcma");
	pj_status_t status = pjmedia_codec_mgr_find_codecs_by_id(codec_mgr, &codec_id, &count, &codec_info, NULL);
	if (status != PJ_SUCCESS)
		PJ_CHECK_STATUS(PJ_EUNKNOWN, (__FILE__, "ERROR SetJitterBuffer pjmedia_codec_mgr_find_codecs_by_id returns error"));

	status = pjmedia_codec_mgr_get_default_param(codec_mgr, codec_info, &codec_param);
	if (status != PJ_SUCCESS)
		PJ_CHECK_STATUS(PJ_EUNKNOWN, (__FILE__, "ERROR SetJitterBuffer pjmedia_codec_mgr_get_default_param returns error"));
	
	//Convertimos a frames los parametros del buffer
	unsigned ptime = (unsigned) codec_param.info.frm_ptime;
	unsigned max_buf_size_fr = max_prefetch / ptime;
	unsigned initial_prefetch_fr = initial_prefetch / ptime;
	unsigned min_prefetch_fr = min_prefetch / ptime;
	unsigned max_prefetch_fr = max_prefetch / ptime;
	
	if (adaptive)
	{
		if (max_prefetch_fr == 0)
		{
			PJ_CHECK_STATUS(PJ_EINVAL, (__FILE__, "ERROR SetJitterBuffer max_prefetch/%d must be > 0", ptime));
		}
		if (max_prefetch > 4000)
		{
			PJ_CHECK_STATUS(PJ_EINVAL, (__FILE__, "ERROR SetJitterBuffer max_prefetch must be < 4000ms"));
		}

		if (initial_prefetch_fr > max_prefetch_fr || min_prefetch_fr > max_prefetch_fr)
		{
			PJ_CHECK_STATUS(PJ_EINVAL, (__FILE__, "ERROR SetJitterBuffer initial_prefetch/%d, min_prefetch/%d must be <= max_prefetch/%d", ptime, ptime, ptime, ptime));
		}
		if (min_prefetch_fr >= max_prefetch_fr)
		{
			PJ_CHECK_STATUS(PJ_EINVAL, (__FILE__, "ERROR SetJitterBuffer (min_prefetch / %d) must be < (max_prefetch / %d)", ptime, ptime));
		}
		if (initial_prefetch_fr > max_prefetch_fr)
		{
			PJ_CHECK_STATUS(PJ_EINVAL, (__FILE__, "ERROR SetJitterBuffer (initial_prefetch / %d) must be <= (max_prefetch / %d)", ptime, ptime));
		}		
				
		pjmedia_stream_jitter_buffer_max_count = max_prefetch_fr * 5 / 4;		
		pjmedia_stream_jitter_buffer_prefetch = initial_prefetch_fr;
		pjmedia_stream_jitter_buffer_min_prefetch = min_prefetch_fr;
		pjmedia_stream_jitter_buffer_max_prefetch = max_prefetch_fr;
		pjmedia_stream_jitter_buffer_discard = discard;
	}
	else 
	{
		if (initial_prefetch_fr == 0)
		{
			PJ_CHECK_STATUS(PJ_EINVAL, (__FILE__, "ERROR SetJitterBuffer in fixed buffer initial_prefetch/%d must be > 0", ptime));
		}
		if (initial_prefetch > 4000)
		{
			PJ_CHECK_STATUS(PJ_EINVAL, (__FILE__, "ERROR SetJitterBuffer in fixed buffer initial_prefetch must be < 4000ms"));
		}
		pjmedia_stream_jitter_buffer_max_count = initial_prefetch_fr * 5 / 4;
		pjmedia_stream_jitter_buffer_prefetch = initial_prefetch_fr;
		pjmedia_stream_jitter_buffer_min_prefetch = initial_prefetch_fr;
		pjmedia_stream_jitter_buffer_max_prefetch = initial_prefetch_fr;
		pjmedia_stream_jitter_buffer_discard = 0;
	}	

	pjmedia_stream_jitter_buffer_force = 1;
	pjmedia_stream_jitter_buffer_adaptive = adaptive;
}

/**
 * CreateAccount:		Crea una cuenta SIP. No registra en SIP Proxy
 *	- INFO: http://www.pjsip.org/docs/latest/pjsip/docs/html/group__PJSUA__LIB__ACC.htm#gad6e01e4e7ac7c8b1d9f629a6189ca2b2
 * @param	acc			Puntero a la URI que se crea como agente. 
 * @param	defaultAcc	Si es diferente a '0', indica que se creará la cuentra por Defecto.
 * @param	proxy_ip	Si es distinto de NULL. IP del proxy Donde se quieren enrutar los paquetes.
 * @param	forced__contact	Si es distinto de NULL. Fuerza un contact.
 * @return	Indentificador de la cuenta.
 */
int SipAgent::CreateAccount(const char * acc, int defaultAcc, const char *proxy_ip, const char* forced__contact)
{
	pjsua_acc_config accCfg;
	pjsua_acc_config_default(&accCfg);
	pj_str_t sturi = pj_str(const_cast<char*>("sip:"));
	pj_str_t stproxip;
	pjsua_acc_id accId = PJSUA_INVALID_ID;
	pj_status_t st;	
	char accaux[CORESIP_MAX_URI_LENGTH];
	char proxy_ip_aux[CORESIP_MAX_URI_LENGTH];
	char forced__contact_aux[CORESIP_MAX_URI_LENGTH];

	strncpy(accaux, acc, sizeof(accaux));
	accaux[sizeof(accaux) - 1] = 0;
	accCfg.id = pj_str(accaux);

	if (proxy_ip != NULL)
	{
		strncpy(proxy_ip_aux, proxy_ip, sizeof(proxy_ip_aux));
		proxy_ip_aux[sizeof(proxy_ip_aux) - 1] = 0;
		stproxip = pj_str(proxy_ip_aux);
	}
	else
	{
		stproxip.ptr = NULL;
	}

	if (forced__contact != NULL)
	{
		strncpy(forced__contact_aux, forced__contact, sizeof(forced__contact_aux));
		forced__contact_aux[sizeof(forced__contact_aux) - 1] = 0;
	}
	else
	{
		forced__contact_aux[0] = 0;
	}
	
	//"sip:proxy_ip"
	char cproxy_route[128];
	if (stproxip.ptr != NULL)
	{
		if (strlen(proxy_ip_aux) > 0)
		{
			if ((strlen(proxy_ip_aux) + sturi.slen + 1) > sizeof(cproxy_route))
			{
				st = PJ_ENOMEM;
				PJ_CHECK_STATUS(st, ("ERROR creando usuario Sip. Memoria insuficiente", "(%s)", accaux));
				return accId;
			}

			pj_bzero(cproxy_route, sizeof(cproxy_route));
			accCfg.proxy_cnt = 1;
			accCfg.proxy[0] = pj_str(cproxy_route);	
			pj_strcpy(&accCfg.proxy[0], &sturi);
			pj_strcat(&accCfg.proxy[0], &stproxip);

			accCfg.publish_enabled = PJ_TRUE;		//Se activa PUBLISH para un servidor de presencia
		}
	}	

	AccountUserData *accUserData = new AccountUserData();
	accCfg.user_data = (void *)accUserData;

	if (strlen(forced__contact_aux) > 0)
	{
		accCfg.force_contact = pj_str((char*)forced__contact_aux);
	}

	st = pjsua_acc_add(&accCfg, defaultAcc, &accId);
	PJ_CHECK_STATUS(st, ("ERROR creando usuario Sip", "(%s)", accaux));
	accUserData->accid = accId;

	if (accCfg.publish_enabled == PJ_TRUE)
	{
		pjsua_acc_set_online_status(accId, PJ_TRUE);		//Inicia el envio de publish al servidor de presencia indicando que
															//está activo
		//pjsua_pres_update_acc(accId, PJ_TRUE);				//Forzamos la actualizacion del estado de presencia

		pjsua_pres_init_publish_acc(accId);

		pjsua_acc_info info;
		if (st == PJ_SUCCESS)
		{		
			int tries = 10;
			do {
				pj_thread_sleep(5);
				st = pjsua_acc_get_info(accId, &info);
				PJ_CHECK_STATUS(st, ("ERROR en pjsua_acc_get_info", "(%s)", accaux));
			} while ((info.status != PJSIP_SC_OK) && (tries-- > 0));
		}	
	}

	return accId;
}

/**
 * RegisterInProxy:		Crea una cuenta y se registra en el SIP proxy. Los paquetes sip se rutean por el SIP proxy también.
 * @param	acc			Puntero al Numero de Abonado (usuario). NO a la uri.
 * @param	defaultAcc	Si es diferente a '0', indica que se creará la cuenta por Defecto.
 * @param	proxy_ip	IP del proxy.
 * @param	expire_seg  Tiempo en el que expira el registro en segundos.
 * @param	username	Si no es necesario autenticación, este parametro será NULL
 * @param   pass		Password. Si no es necesario autenticación, este parametro será NULL
 * @param   displayName	friendly name to be displayed included before SIP uri
 * @param	isfocus		Si el valor es distinto de cero, indica que es Focus, para establecer llamadas multidestino
 * @param	forced__contact	Si es distinto de NULL. Fuerza un contact.
 * @return	Indentificador de la cuenta.
 */
int SipAgent::CreateAccountAndRegisterInProxy(const char * acc, int defaultAcc, const char *proxy_ip, unsigned int expire_seg, const char *username, const char *pass, const char *displayName, int isfocus, const char* forced__contact)
{
	pjsua_acc_id accId = PJSUA_INVALID_ID;
	pj_status_t st = PJ_SUCCESS;
	char forced__contact_aux[CORESIP_MAX_URI_LENGTH];
	pjsua_acc_config accCfg;
	pjsua_acc_config_default(&accCfg);

	char accaux[CORESIP_MAX_URI_LENGTH];
	strncpy(accaux, acc, sizeof(accaux));
	accaux[sizeof(accaux) - 1] = 0;

	char proxy_ip_aux[CORESIP_MAX_URI_LENGTH];
	strncpy(proxy_ip_aux, proxy_ip, sizeof(proxy_ip_aux));
	proxy_ip_aux[sizeof(proxy_ip_aux) - 1] = 0;

	char username_aux[CORESIP_MAX_URI_LENGTH];
	if (username != NULL)
	{
		strncpy(username_aux, username, sizeof(username_aux));
		username_aux[sizeof(username_aux) - 1] = 0;
	}
	else
	{
		username_aux[0] = 0;
	}

	char pass_aux[CORESIP_MAX_URI_LENGTH];
	if (pass != NULL)
	{
		strncpy(pass_aux, pass, sizeof(pass_aux));
		pass_aux[sizeof(pass_aux) - 1] = 0;
	}
	else
	{
		pass_aux[0] = 0;
	}

	char displayName_aux[CORESIP_MAX_URI_LENGTH];
	if (displayName != NULL)
	{
		strncpy(displayName_aux, displayName, sizeof(displayName_aux));
		displayName_aux[sizeof(displayName_aux) - 1] = 0;
	}
	else
	{
		displayName_aux[0] = 0;
	}

	pj_str_t sturi = pj_str(const_cast<char*>("sip:"));
	pj_str_t starr = pj_str(const_cast<char*>("@"));
	pj_str_t stacc = pj_str(const_cast<char*>(accaux));
	pj_str_t stproxip = pj_str(const_cast<char*>(proxy_ip_aux));
	pj_str_t stpp = pj_str(const_cast<char*>(":"));
	pj_str_t st_uaIpAdd = pj_str(const_cast<char*>(uaIpAdd));
	pj_str_t st_doubleQuote = pj_str(const_cast<char*>("\""));
	pj_str_t st_space = pj_str(const_cast<char*>(" "));
	pj_str_t st_menorque = pj_str(const_cast<char*>("<"));
	pj_str_t st_mayorque = pj_str(const_cast<char*>(">"));
	pj_str_t st_displayName = pj_str(const_cast<char*>(displayName_aux));

	if (st_displayName.slen > 32)
	{
		//La longitud del Display Name lo limitamos a 32 caracteres. Ya que no afecta al protocolo
		st_displayName.slen = 32;
	}

	char uaport[16];
	pj_utoa(SipAgent::uaPort, uaport);	 
	pj_str_t stuaport = pj_str(const_cast<char*>(uaport));
			
	//"displayname" <sip:accID@user_agent_ip:5060>
	char cid[256];

	for (int i = 0; i < stacc.slen; i++)
		stacc.ptr[i] = pj_tolower(stacc.ptr[i]);	

	if ((pj_strlen(&st_doubleQuote)+pj_strlen(&st_displayName)+pj_strlen(&st_doubleQuote) + pj_strlen(&st_space) + 
		pj_strlen(&st_menorque) + pj_strlen(&sturi) + pj_strlen(&stacc) + pj_strlen(&starr) + pj_strlen(&st_uaIpAdd) + 
		pj_strlen(&stpp) + pj_strlen(&stuaport) +  pj_strlen(&st_mayorque) + 1) > sizeof(cid))
	{
		st = PJ_ENOMEM;
		PJ_CHECK_STATUS(st, ("ERROR creando usuario Sip. Uri demasiado larga", "(%s)", accaux));
		return accId;
	}

	pj_bzero(cid, sizeof(cid));
	accCfg.id = pj_str(cid);
	pj_strcpy(&accCfg.id, &st_doubleQuote);
	pj_strcat(&accCfg.id, &st_displayName);
	pj_strcat(&accCfg.id, &st_doubleQuote);
	pj_strcat(&accCfg.id, &st_space);
	pj_strcat(&accCfg.id, &st_menorque);
	pj_strcat(&accCfg.id, &sturi);
	pj_strcat(&accCfg.id, &stacc);
	pj_strcat(&accCfg.id, &starr);
	pj_strcat(&accCfg.id, &pj_str(const_cast<char*>(uaIpAdd)));
	pj_strcat(&accCfg.id, &stpp);
	pj_strcat(&accCfg.id, &stuaport);
	pj_strcat(&accCfg.id, &st_mayorque);

	//"sip:proxy_ip"
	char cprox[128];

	if ((pj_strlen(&sturi) + pj_strlen(&stproxip) + 1) > sizeof(cprox))
	{
		st = PJ_ENOMEM;
		PJ_CHECK_STATUS(st, ("ERROR creando usuario Sip. Memoria insuficiente", "(%s)", accaux));
		return accId;
	}

	pj_bzero(cprox, sizeof(cprox));
	accCfg.reg_uri = pj_str(cprox);
	pj_strcpy(&accCfg.reg_uri, &sturi);
	pj_strcat(&accCfg.reg_uri, &stproxip);

	if (forced__contact != NULL)
	{
		strncpy(forced__contact_aux, forced__contact, sizeof(forced__contact_aux));
		forced__contact_aux[sizeof(forced__contact_aux) - 1] = 0;
	}
	else
	{
		forced__contact_aux[0] = 0;
	}

	if (strlen(forced__contact_aux) > 0)
	{
		accCfg.force_contact = pj_str((char*)forced__contact_aux);
	}
		
	accCfg.reg_timeout = expire_seg;	
	accCfg.reg_retry_interval = 20;
	if ((strlen(username_aux) > 0) && (strlen(pass_aux) > 0))
	{
		accCfg.cred_count = 1;
		accCfg.cred_info[0].realm = pj_str(const_cast<char*>("*")); 
		accCfg.cred_info[0].scheme = pj_str(const_cast<char*>("digest")); 
		accCfg.cred_info[0].username = pj_str(const_cast<char*>(username_aux));
		for (int i = 0; i < accCfg.cred_info[0].username.slen; i++)
			accCfg.cred_info[0].username.ptr[i] = pj_tolower(accCfg.cred_info[0].username.ptr[i]);
		accCfg.cred_info[0].data_type = 0;   //Plain text
		accCfg.cred_info[0].data = pj_str(const_cast<char*>(pass_aux));
		for (int i = 0; i < accCfg.cred_info[0].data.slen; i++)
			accCfg.cred_info[0].data.ptr[i] = pj_tolower(accCfg.cred_info[0].data.ptr[i]);
	}


	//Agragamos la ip del proxy para que todos los sips lleven la cabecera route.
	char cproxy_route[128];	
	pj_bzero(cproxy_route, sizeof(cproxy_route));
	accCfg.proxy_cnt = 1;
	accCfg.proxy[0] = pj_str(cproxy_route);	
	pj_strcpy(&accCfg.proxy[0], &sturi);
	pj_strcat(&accCfg.proxy[0], &stproxip);

	if (isfocus)
	{
		accCfg.contact_params = pj_str(const_cast<char*>(";isfocus"));
	}

	accCfg.publish_enabled = PJ_TRUE;		//Se activa PUBLISH para un servidor de presencia

	AccountUserData * accUserData = new AccountUserData();
	accCfg.user_data = (void *)accUserData;

#if 0
	//Generamos pidf_tuple_id para el PIDF ID de los PUBLISH y NOTIFIes
	char tuple_id_buf[PJ_GUID_MAX_LENGTH+2];
	pj_bzero(tuple_id_buf, sizeof(tuple_id_buf));
	accCfg.pidf_tuple_id = pj_str(tuple_id_buf);

    accCfg.pidf_tuple_id.ptr += 2;
	pj_generate_unique_string(&accCfg.pidf_tuple_id);
	accCfg.pidf_tuple_id.ptr -= 2;
	accCfg.pidf_tuple_id.ptr[0] = 'n';
	accCfg.pidf_tuple_id.ptr[1] = 'u';
	accCfg.pidf_tuple_id.slen += 2;
#endif
		
	st = pjsua_acc_add(&accCfg, defaultAcc, &accId);
	PJ_CHECK_STATUS(st, ("ERROR creando usuario Sip", "(%s)", accaux));
	accUserData->accid = accId;

	if (accCfg.publish_enabled == PJ_TRUE)
	{
		pjsua_acc_set_online_status(accId, PJ_TRUE);		//Inicia el envio de publish al servidor de presencia indicando que
															//está activo
		//pjsua_pres_update_acc(accId, PJ_TRUE);				//Forzamos la actualizacion del estado de presencia

		pjsua_pres_init_publish_acc(accId);

		pjsua_acc_info info;
		if (st == PJ_SUCCESS)
		{		
			int tries = 10;
			do {
				pj_thread_sleep(5);
				st = pjsua_acc_get_info(accId, &info);
				PJ_CHECK_STATUS(st, ("ERROR en pjsua_acc_get_info", "(%s)", accaux));
			} while ((info.status != PJSIP_SC_OK) && (tries-- > 0));
		}	
	}

	return accId;
}

/**
 * DestroyAccount: Borra la Cuenta SIP.
 * @param	id		Identificador de la cuenta. Corresponde al retorno de un 'CreateAccount'
 * @return	Nada
 */
void SipAgent::DestroyAccount(int id)
{
	pj_status_t st = PJ_SUCCESS;
	pjsua_acc_info info;	
		
	pjsua_acc_set_online_status((pjsua_acc_id) id, PJ_FALSE);	//Deja de publicarse en el servidor de presencia	

	//Envia mensaje REGISTER para eliminarse del servidor
	st = pjsua_acc_get_info((pjsua_acc_id) id, &info);
	if (info.has_registration)
	{
		//Esta cuenta esta registrada en servidor y se procede a eliminarla
		st = pjsua_acc_set_registration((pjsua_acc_id) id, PJ_FALSE);

		if (st == PJ_SUCCESS)
		{			
			//Espera a que el servidor responda un 200
			int tries = 20;
			do {
				pj_thread_sleep(5);
				st = pjsua_acc_get_info((pjsua_acc_id) id, &info);
				//PJ_CHECK_STATUS(st, ("ERROR pjsua_acc_get_info", "(%d)", id));
			} while ((info.status != PJSIP_SC_OK) && (--tries > 0) && (st == PJ_SUCCESS));	
			pj_thread_sleep(50);
		}

		//Necesario para que la funcion pjsua_acc_del no produzca una excepcion al intentar volver a
		//eliminar el registro en el servidor. Es un bug de la libreria
		if (pjsua_var.acc[id].regc) {
			pjsip_regc_destroy(pjsua_var.acc[id].regc);
		}
		pjsua_var.acc[id].regc = NULL;
	} 	

	AccountUserData * accUserData = (AccountUserData *) pjsua_acc_get_user_data(id);
	if (accUserData != NULL) delete accUserData;

	st = pjsua_acc_del((pjsua_acc_id) id);
	PJ_CHECK_STATUS(st, ("ERROR eliminando PJSUA account", "(%d)", id));

	pj_thread_sleep(250);		//Se pone un retardo paradejar tiempo a que se desregistre la cuenta antes de poder volver
								//a registrar otra
	
	return;
}

/**
 *	SetTipoGRS. Configura el tipo de GRS. El ETM lo llama cuando crea un account tipo GRS.
 */
void SipAgent::SetTipoGRS(int id, CORESIP_CallFlagsMask Flag, int on)
{
	AccountUserData * accUserData = (AccountUserData *) pjsua_acc_get_user_data(id);
	if (accUserData != NULL)
	{	
		if (on)
		{
			accUserData->rdAccount = PJ_TRUE;		//Indicamos que este account es de una radio GRS
		}
		else
		{
			accUserData->rdAccount = PJ_FALSE;
		}
		accUserData->TipoGrsFlags = Flag;
	}
}

pjsua_acc_id SipAgent::GetTipoGRS(int id, CORESIP_CallFlagsMask* pTipoGRS)
{
	AccountUserData * accUserData = (AccountUserData *) pjsua_acc_get_user_data(id);
	if (accUserData != NULL)
	{
		*pTipoGRS = accUserData->TipoGrsFlags;
		return id;
	}	
	return PJSUA_INVALID_ID;
}

/**
*	SetGRSParams. Configura parametros del GRS
*	@param	accId		Identificador de la cuenta. Si vale -1 entonces se utiliza la default
* 	@param	RdFr		Frecuencia del GRS (fid). String acabado en '\0'. Con NULL se ignora
*	@param	Tj1			Jitter buffer delay en milisegundos. Con NULL se ignora
*	@param	Ts1			GRS System Delay. En milisegundos. Con NULL se ignora
*   @param	Ts2			GRS system RX delay (including packetizing time). En milisegundos. Con NULL se ignora
* *	@param	preferred_grs_bss_method   Metodo BSS preferido. Si en la lista de metodo recibido del VCS aparece entonces se selecciona, 
										si no entonces se selecciona "RSSI". Es un string terminado com caracter cero o si es NULL se ignora este parametro
										Su longitud maxima es CORESIP_MAX_BSS_LENGTH
*	@param  preferred_grs_bss_method_code		Si #preferred_grs_bss_method no es "RSSI", "AGC", "C/N" ni "PSD", 
													este parametro es el valor del codigo del Vendor specific method. Debera ser entre 4 y 7.
													Si es NULL se ignora este parametro
*	@param  forced_pttid	Si es NULL se ignora este parametro. Si el valor es -1, entonces el GRS asigna automaticamente el ptt-id.
							Si el valor es distinto de -1 entonces es el valor de ptt-id que se fuerza cuando se establece una sesion.
*	@param	selcal_supported	Si es NULL se ignore. Si el valor es 1 entonces el GRS soporta SELCAL, y vale 0 no.
*	@return				Codigo de Error
*/
void SipAgent::SetGRSParams(int accId, char* RdFr, unsigned int *Tj1, unsigned int *Ts1, unsigned int* Ts2, char* preferred_grs_bss_method,
	unsigned int *preferred_grs_bss_method_code, int* forced_ptt_id, int* selcal_supported)
{
	if (Tj1 != NULL && *Tj1 > 8191)
	{
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR SipAgent::CORESIP_SetGRSParams Invalid Tj1 or Ts1. It must be less than 8192"));
	}

	if (Ts1 != NULL && *Ts1 > 8191)
	{
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR SipAgent::CORESIP_SetGRSParams Invalid Tj1 or Ts1. It must be less than 8192"));
	}

	if (Ts2 != NULL && *Ts2 > 8191)
	{
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR SipAgent::CORESIP_SetGRSParams Invalid Ts2. It must be less than 8192"));
	}

	if (preferred_grs_bss_method != NULL)
	{
		if (strlen(preferred_grs_bss_method) > CORESIP_MAX_BSS_LENGTH)
		{
			PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR SipAgent::CORESIP_SetGRSParams Invalid preferred_grs_bss_method. Size must be less than CORESIP_MAX_BSS_LENGTH"));
		}
	}

	if (preferred_grs_bss_method_code != NULL)
	{
		if (*preferred_grs_bss_method_code < 4 || *preferred_grs_bss_method_code > 7)
		{
			PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR SipAgent::CORESIP_SetGRSParams preferred_grs_bss_method_code debe ser entre 4 y 7"));
		}
	}

	if (forced_ptt_id != NULL)
	{
		char ED137Radioversion_, ED137Phoneversion_;
		pjsip_endpt_Get_Ed137_version(pjsua_var.endpt, &ED137Radioversion_, &ED137Phoneversion_);
		if (ED137Radioversion_ == 'C')
		{
			if (*forced_ptt_id > 0 && *forced_ptt_id > AccountUserData::MAX_PTTID_ED137C)
			{
				PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR SipAgent::CORESIP_SetGRSParams ", "forced_ptt_id no puede ser mayor de %u en ED137C", AccountUserData::MAX_PTTID_ED137C));
			}
		}
		else if (*forced_ptt_id > 0 && *forced_ptt_id > AccountUserData::MAX_PTTID_ED137B)
		{
			PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR SipAgent::CORESIP_SetGRSParams ", "forced_ptt_id no puede ser mayor de %u en ED137B", AccountUserData::MAX_PTTID_ED137B));
		}
	}

	pjsua_acc_id pj_acc_id;
	if (accId == -1)
	{
		pj_acc_id = pjsua_acc_get_default();
	}
	else
	{
		pj_acc_id = accId;
	}

	if (!pjsua_acc_is_valid(pj_acc_id))
	{
		PJ_CHECK_STATUS(PJ_EINVAL, ("SipAgent::SetGRSParams Invalid Account"));
		return;
	}

	AccountUserData* accUserData = (AccountUserData*)pjsua_acc_get_user_data(pj_acc_id);
	if (accUserData != NULL)
	{
		if (!accUserData->rdAccount && !SipAgent::_Radio_UA)
		{
			PJ_CHECK_STATUS(PJ_EINVALIDOP, ("ERROR SipAgent::CORESIP_SetGRSParams. accId is not a GRS"));
		}

		if (Tj1 != NULL)
		{
			pj_uint32_t Taux = (pj_uint32_t) *Tj1;
			Taux *= 1000;	//microsegundos
			Taux /= 125;	//Unidades de 125 us		
			accUserData->Tj1 = (pj_uint16_t)Taux;
		}

		if (Ts1 != NULL)
		{
			pj_uint32_t Taux = (pj_uint32_t) *Ts1;
			Taux *= 1000;	//microsegundos
			Taux /= 125;	//Unidades de 125 us		
			accUserData->Ts1 = (pj_uint16_t)Taux;
		}

		if (Ts2 != NULL)
		{
			pj_uint32_t Taux = (pj_uint32_t)*Ts2;
			Taux *= 1000;	//microsegundos
			Taux /= 125;	//Unidades de 125 us		
			accUserData->Ts2 = (pj_uint16_t)Taux;
		}

		if (RdFr != NULL)
		{
			if (pj_ansi_strlen(RdFr) < sizeof(accUserData->RdFr))
			{
				if (pj_ansi_strcmp(accUserData->RdFr, RdFr) != 0)
				{
					//El identificador de Frecuencia Fid ha cambiado
					pj_ansi_strcpy(accUserData->RdFr, RdFr);
					int num_hanged_sesions = SipCall::HangUpAllCallsOfAcc(pj_acc_id, PJ_TRUE);	//Se cierran las sesiones excepto las que tiene activo NoFreqDisconn
					if (num_hanged_sesions == 0)
					{
						//Se envian los NOTIFY a las subscripciones activas si no se ha cerrado ninguna sesion, 
						//ya que si se cierra alguna sesion ya se mandan NOTIFY a todos
						//Se envia solo a las subscripciones del tipo ED137C
						SubscriptionServer::SendWG67NotifyFromAcc(pj_acc_id, PJ_TRUE, NULL, NULL, PJ_TRUE, 
							AccountUserData::ENVIAR_WG67_NOTIFY_SI_TODAS_LAS_SESIONES_SON_FICTICIAS);
					}
				}
			}
			else
			{
				PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR SipAgent::CORESIP_SetGRSParams RdFr is very large"));
			}
		}
		if (preferred_grs_bss_method != NULL)
		{
			pj_ansi_strcpy(accUserData->preferred_grs_bss_method, preferred_grs_bss_method);
		}
		if (preferred_grs_bss_method_code != NULL)
		{
			accUserData->preferred_grs_bss_method_code = *preferred_grs_bss_method_code;
		}
		if (forced_ptt_id != NULL)
		{			
			accUserData->forced_grs_ptt_id = *forced_ptt_id;
		}
		if (selcal_supported != NULL)
		{
			accUserData->SelCalSupported = *selcal_supported;
		}
	}
	else
	{
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR SipAgent::CORESIP_SetGRSParams Invalid accId"));
	}
}

/**
 * AddSndDevice: Añade un dispositivo de sonido en la Tabla de dispositivos del modulo.
 * @param	info	Puntero a la Informacion del dispositivo de Sonido.
 * @return	Identificador del Dispositivo. 
 */
int SipAgent::AddSndDevice(const CORESIP_SndDeviceInfo * info)
{
	int dev = 0;

	for (; dev < PJ_ARRAY_SIZE(_SndPorts); dev++)
	{
		if (_SndPorts[dev] == NULL)
		{
			break;
		}
	}
	
	pj_assert(dev < PJ_ARRAY_SIZE(_SndPorts));
	if (dev == PJ_ARRAY_SIZE(_SndPorts)) 
	{
		throw PJLibException(__FILE__, PJ_ETOOMANY).Msg("AddSndDevice: No es posible sumar mas dispositivos de sonido");
	}

	_SndPorts[dev] = new SoundPort(info->Type, info->OsInDeviceIndex, info->OsOutDeviceIndex);

	if (_SndPorts[dev] == NULL)
	{
		throw PJLibException(__FILE__, PJ_ETOOMANY).Msg("AddSndDevice: No se puede crear un nuevo SoundPort");
	}

	if (_SndPorts[dev]->InDevIndex >= 0)
	{
		_InChannels[_NumInChannels++] = _SndPorts[dev]->InDevIndex;
	}
	if (_SndPorts[dev]->OutDevIndex >= 0)
	{
		_OutChannels[_NumOutChannels++] = _SndPorts[dev]->OutDevIndex;
	}

	return dev;
}

/**
 * CreateWavPlayer: Crea un 'Reproductor Wav'
 * @param	file	Path al fichero Wav
 * @param	loop	Se reproduce indefinidamente Si/No
 * @return	Identificador del Reproductor. Corresponde al Indice de la tabla de control.
 */
int SipAgent::CreateWavPlayer(const char * file, bool loop)
{
	int id = 0;

	/**
	 * Busca un lugar libre en la tabla de 'retproductores wav'
	 */
	for (; id < PJ_ARRAY_SIZE(_WavPlayers); id++)
	{
		if (_WavPlayers[id] == NULL)
		{
			break;
		}
	}

	pj_assert(id < PJ_ARRAY_SIZE(_WavPlayers));
	if (id == PJ_ARRAY_SIZE(_WavPlayers)) 
	{
		throw PJLibException(__FILE__, PJ_ETOOMANY).Msg("CreateWavPlayer: No es posible sumar mas reproductores wav");
	}

	/**
	 * Crea el Reproductor a través de la clase @ref WavPlayer.
	 */
	_WavPlayers[id] = new WavPlayer(file, PTIME, loop, OnWavPlayerEof, (void*)(size_t)id);

	if (_WavPlayers[id] == NULL)
	{
		throw PJLibException(__FILE__, PJ_ETOOMANY).Msg("CreateWavPlayer: No se puede crear un nuevo WavPlayer");
	}

	return id;
}

/**
 * DestroyWavPlayer: Destruye un 'Reproductor Wav'
 * @param	id		 Identificador del 'Reproductor Wav'
 * @return	Nada
 */
void SipAgent::DestroyWavPlayer(int id)
{
	Guard lock(_Lock);	// Por el autodelete que tienen por puertos wav

	if (_WavPlayers[id] != NULL)
	{
		delete _WavPlayers[id];
		_WavPlayers[id] = NULL;
	}
}

/**
 * CreateWavRecorder: Crea un Grabador Wav
 * @param	file	Puntero al Path del fichero donde grabar.
 * @return	identificador del grabador.
 */
int SipAgent::CreateWavRecorder(const char * file)
{
	int id = 0;

	/**
	 * Busca un lugar libe en la tabla de control.
	 */
	for (; id < PJ_ARRAY_SIZE(_WavRecorders); id++)
	{
		if (_WavRecorders[id] == NULL)
		{
			break;
		}
	}

	pj_assert(id < PJ_ARRAY_SIZE(_WavRecorders));
	if (id == PJ_ARRAY_SIZE(_WavRecorders))
	{
		throw PJLibException(__FILE__, PJ_ETOOMANY).Msg("CreateWavRecorder: No se pueden sumar mas elementos _WavRecorders");
	}
	/**
	 * Crea el 'grabador' a través de la clase @ref WavRecorder
	 */
	_WavRecorders[id] = new WavRecorder(file);
	if (_WavRecorders[id] == NULL)
	{
		throw PJLibException(__FILE__, PJ_ETOOMANY).Msg("CreateWavRecorder: No se puede crear mas elementos _WavRecorders");
	}
	return id;
}

/**
 * DestroyWavRecorder: Destruye un Grabador Wav.
 * @param	id		 Identificador del Grabador
 * @return	Nada
 */
void SipAgent::DestroyWavRecorder(int id)
{
	if (_WavRecorders[id] != NULL)
	{
		delete _WavRecorders[id];
		_WavRecorders[id] = NULL;
	}
}

/**
 * CreateRdRxPort: Crea un 'PORT' de Recepcion Radio.
 * @param	info	Puntero a la Informacion del PORT.
 * @param	localIp	Puentero a la direccion Ip de la Máquina.
 * @return	identificador del 'PORT'. Indice de la tabla de control.
 */
int SipAgent::CreateRdRxPort(const CORESIP_RdRxPortInfo * info, const char * localIp)
{
	int id = 0;

	for (; id < PJ_ARRAY_SIZE(_RdRxPorts); id++)
	{
		if (_RdRxPorts[id] == NULL)
		{
			break;
		}
	}

	pj_assert(id < PJ_ARRAY_SIZE(_RdRxPorts));
	if (id == PJ_ARRAY_SIZE(_RdRxPorts))
	{
		throw PJLibException(__FILE__, PJ_ETOOMANY).Msg("CreateRdRxPort: No se pueden sumar mas elementos _RdRxPorts");
	}

	_RdRxPorts[id] = new RdRxPort(SAMPLING_RATE, CHANNEL_COUNT, BITS_PER_SAMPLE, PTIME,
		info->ClkRate, info->ChannelCount, info->BitsPerSample, info->FrameTime, localIp, info->Ip, info->Port);

	if (_RdRxPorts[id] == NULL)
	{
		throw PJLibException(__FILE__, PJ_ETOOMANY).Msg("CreateRdRxPort: No se puede crear mas elementos _RdRxPorts");
	}

	return id;
}

/**
 * DestroyRdRxPort: Destruye un PORT de Recepción Radio.
 * @param	id		Identificador del PORT
 * @return	Nada
 */
void SipAgent::DestroyRdRxPort(int id)
{
	if (_RdRxPorts[id] != NULL)
	{
		delete _RdRxPorts[id];
		_RdRxPorts[id] = NULL;
	}
}

/**
 * CreateSndRxPort: Crea un PORT de Recepcion de Audio.
 * @param	name	Puntero al Nombre del Puerto
 * @return	Identificador del PORT.
 */
int SipAgent::CreateSndRxPort(const char * name)
{
	Guard lock(_Lock);
	int id = 0;

	for (; id < PJ_ARRAY_SIZE(_SndRxPorts); id++)
	{
		if (_SndRxPorts[id] == NULL)
		{
			break;
		}
	}

	pj_assert(id < PJ_ARRAY_SIZE(_SndRxPorts));
	if (id == PJ_ARRAY_SIZE(_SndRxPorts))
	{
		throw PJLibException(__FILE__, PJ_ETOOMANY).Msg("CreateSndRxPort: No se pueden sumar mas elementos _SndRxPorts");
	}

	_SndRxPorts[id] = new SoundRxPort(name, SAMPLING_RATE, CHANNEL_COUNT, BITS_PER_SAMPLE, PTIME);

	if (_SndRxPorts[id] == NULL)
	{
		throw PJLibException(__FILE__, PJ_ETOOMANY).Msg("CreateSndRxPort: No se pueden crear mas elementos _SndRxPorts");
	}

	_SndRxIds[name] = _SndRxPorts[id];
	return id;
}

/**
 * DestroySndRxPort: Destruye un PORT 'SndRx'
 * @param	id		Identificador del PORT.
 * @return	Nada
 */
void SipAgent::DestroySndRxPort(int id)
{
	Guard lock(_Lock);

	if (_SndRxPorts[id] != NULL)
	{
		_SndRxIds.erase(_SndRxPorts[id]->Id);
		delete _SndRxPorts[id];
		_SndRxPorts[id] = NULL;
	}
}

/**
 * EchoCancellerLCMic.	...
 * Activa/desactiva cancelador de eco altavoz LC y Microfonos. Sirve para el modo manos libres 
 * @param	on						true - activa / false - desactiva
 * @return	CORESIP_OK OK, CORESIP_ERROR  error.
 */
int SipAgent::EchoCancellerLCMic(bool on)
{
	pj_status_t st;
	int ret = CORESIP_OK;

	st = pj_lock_acquire(_ECLCMic_mutex);
	if (st != PJ_SUCCESS)
	{
		PJ_LOG(3,(__FILE__, "ERROR: EchoCancellerLCMic: SipAgent::_ECLCMic_mutex No puede adquirirse"));
	}

	if (on)
	{
		if (_EchoCancellerLCMic == NULL)
		{
			/*Creamos un cancelador de eco para manos libres. En el caso de que el audio de telefonia
			salga por altavoz de manos libres se cancela el eco en los microfonos*/
			st = pjmedia_echo_create2(pjsua_var.pool, SAMPLING_RATE, CHANNEL_COUNT,
					  ((SAMPLING_RATE * CHANNEL_COUNT * PTIME) / 1000), //dn_port->info.samples_per_frame,
					  SipAgent::EchoTail, SipAgent::EchoLatency, 0 , &_EchoCancellerLCMic);
			if (st != PJ_SUCCESS) 
			{
				_EchoCancellerLCMic = NULL;	
				PJ_LOG(3,(__FILE__, "ERROR: SipAgent::EchoCancellerLCMic No puede crearse"));
				ret = CORESIP_ERROR;
				PJ_CHECK_STATUS(st, ("ERROR: SipAgent::EchoCancellerLCMic No puede crearse"));				
			}
		}		
	}
	else
	{
		if (_EchoCancellerLCMic != NULL)
		{
			pjmedia_echo_state *ec_aux = _EchoCancellerLCMic;
			_EchoCancellerLCMic = NULL;
			st = pjmedia_echo_destroy(ec_aux);
			if (st != PJ_SUCCESS) 
			{
				PJ_LOG(3,(__FILE__, "ERROR: SipAgent::EchoCancellerLCMic No puede ser destruido"));
				ret = CORESIP_ERROR;
				PJ_CHECK_STATUS(st, ("ERROR: SipAgent::EchoCancellerLCMic No puede ser destruido"));
			}
		}
	}

	st = pj_lock_release(_ECLCMic_mutex);
	if (st != PJ_SUCCESS)
	{
		PJ_LOG(3,(__FILE__, "ERROR: EchoCancellerLCMic: SipAgent::_ECLCMic_mutex No puede liberarse"));
	}

	return ret;
}

/**
 * RecConnectSndPort.	...
 * Conecta/desconecta un puerto de sonido al de grabacion
 * @param	on						true - record / false - pause
 * @param  dev			Indice del array _SndPorts. Es dispositivo (microfono) fuente del audio.
 * @return	0 OK, -1  error.
 */
int SipAgent::RecConnectSndPort(bool on, int dev, RecordPort *recordport)
{
	pj_status_t st = PJ_SUCCESS;
	int ret = 0;

	if (dev >= CORESIP_MAX_SOUND_DEVICES)
		return ret;

	if (on)
	{
		if (_SndPorts[dev] != NULL)
		{
			if (_SndPorts[dev]->Slot != PJSUA_INVALID_ID)
			{
				if (!IsSlotValid(_SndPorts[dev]->Slot))
				{
				}
				else if (!recordport->IsSlotConnectedToRecord(_SndPorts[dev]->Slot))
				{
					st = pjsua_conf_connect(_SndPorts[dev]->Slot, recordport->Slot);			
					PJ_CHECK_STATUS(st, ("ERROR conectando puertos", "(%d --> puerto grabacion %d)", _SndPorts[dev]->Slot, recordport->Slot));
				}
			}
		}
	}
	else
	{
		if (_SndPorts[dev] != NULL)
		{
			if (_SndPorts[dev]->Slot != PJSUA_INVALID_ID)
			{
				if (!IsSlotValid(_SndPorts[dev]->Slot))
				{
				}
				else if (recordport->IsSlotConnectedToRecord(_SndPorts[dev]->Slot))
				{
					st = pjsua_conf_disconnect(_SndPorts[dev]->Slot, recordport->Slot);			
					PJ_CHECK_STATUS(st, ("ERROR desconectando puertos", "(%d --> puerto grabacion %d)", _SndPorts[dev]->Slot, recordport->Slot));
				}
			}
		}
	}
	
	return ret;
}

/**
 * RecConnectSndPorts.	...
 * Conecta/desconecta los puertos de sonido al de grabacion
 * @param	on						true - record / false - pause
 * @return	0 OK, -1  error.
 */
int SipAgent::RecConnectSndPorts(bool on, RecordPort *recordport)
{
	pj_status_t st = PJ_SUCCESS;
	int ret = 0;

	if (on)
	{
		//Envia el puerto de grabación el Rx de los _SndPorts
		//Conecta los Rx de los dispositivos de sonido (microfonos) al puerto de grabación VoIP
		for (int dev = 0; dev < PJ_ARRAY_SIZE(_SndPorts); dev++)
		{
			if (_SndPorts[dev] != NULL)
			{
				if (_SndPorts[dev]->Slot != PJSUA_INVALID_ID)
				{
					if (!IsSlotValid(_SndPorts[dev]->Slot))
					{
						_SndPorts[dev]->Slot = PJSUA_INVALID_ID;
					}
					else if (!recordport->IsSlotConnectedToRecord(_SndPorts[dev]->Slot))
					{
						st = pjsua_conf_connect(_SndPorts[dev]->Slot, recordport->Slot);			
						PJ_CHECK_STATUS(st, ("ERROR conectando puertos", "(%d --> puerto grabacion %d)", _SndPorts[dev]->Slot, recordport->Slot));
					}
				}
			}
		}				
	}
	else
	{
		//Corta el envío al puerto de grabación del Rx de los _SndPorts
		//Desconecta los Rx de los dispositivos de sonido (microfonos) del puerto de grabación VoIP
		for (int dev = 0; dev < PJ_ARRAY_SIZE(_SndPorts); dev++)
		{			
			if (_SndPorts[dev] != NULL)
			{
				if (_SndPorts[dev]->Slot != PJSUA_INVALID_ID)
				{
					if (!IsSlotValid(_SndPorts[dev]->Slot))
					{
						_SndPorts[dev]->Slot = PJSUA_INVALID_ID;
					}
					else if (recordport->IsSlotConnectedToRecord(_SndPorts[dev]->Slot))
					{
						st = pjsua_conf_disconnect(_SndPorts[dev]->Slot, recordport->Slot);			
						PJ_CHECK_STATUS(st, ("ERROR desconectando puertos", "(%d --> puerto grabacion %d)", _SndPorts[dev]->Slot, recordport->Slot));
					}
				}
			}
		}
	}
	
	return ret;
}

/**
 * NumConfirmedCalls.	...
 * Retorna Retorna el número de llamadas confirmadas (en curso) y que tienen media, es decir, no estan en hold
 * @return	Numero de llamadas confirmadas.
 */
unsigned SipAgent::NumConfirmedCalls()
{
	unsigned ret = 0;
	unsigned call_count = PJSUA_MAX_CALLS;
	pjsua_call_id ids[PJSUA_MAX_CALLS];
	
	pj_status_t st = pjsua_enum_calls(ids, &call_count);
	if (st != PJ_SUCCESS)
	{
		PJ_CHECK_STATUS(st, ("ERROR enum calls in  SipAgent::RecordTel"));
	}

	for (unsigned i = 0; i < call_count; i++)
	{
		pjsua_call_info info;
		st = pjsua_call_get_info(ids[i], &info);

		if (st == PJ_SUCCESS)
		{
			if (info.state == PJSIP_INV_STATE_CONNECTING || info.state == PJSIP_INV_STATE_CONFIRMED) 
			{
				if (info.media_status != PJSUA_CALL_MEDIA_NONE && info.media_status != PJSUA_CALL_MEDIA_ERROR)
				{
					if (info.media_dir != PJMEDIA_DIR_NONE)
					{
						ret++;
					}
				}
			}
		}
	}
	
	return ret;
}

/**
 * IsSlotValid.	...
 * Retorna si el Slot del puerto es válido. Puede ser qye ya no lo sea porque
 * la llamada ha finalizado
 * @param	slot. slot a validar
 * @return	true o false.
 */
bool SipAgent::IsSlotValid(pjsua_conf_port_id slot)
{
	pjsua_conf_port_info info;

	if (slot == PJSUA_INVALID_ID)
	{
		return false;
	}

	pj_status_t st = pjsua_conf_get_port_info(slot, &info);
	if (st == PJ_SUCCESS) 
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**
 * RecINV.	...
 * Envía el comando INV al modulo de grabacion para telefonia 
 * @param uri. Uri del llamado si la llamada es saliente o llamante si es entrante
 * @param callType. Tipo de llamada
 * @return	0 OK, -1  error.
 */
int SipAgent::RecINV(pj_str_t *uri, CORESIP_CallType callType)
{
	CORESIP_Resource_Type type;
	if (callType == CORESIP_CALL_RD) type = Rd;
	else if (callType == CORESIP_CALL_IA) type = IA;
	else type = Tlf;

	RecordPort* recPort = RecordPort::GetRecordPortFromResource(uri, NULL, type);

	if (!recPort) return -1;
	return recPort->RecINV();
}

/**
 * RecBYE.	...
 * Envía el comando BYE al modulo de grabacion para telefonia 
 * @param uri. Uri del llamado si la llamada es saliente o llamante si es entrante
 * @param callType. Tipo de llamada
 * @return	0 OK, -1  error.
 */
int SipAgent::RecBYE(pj_str_t* uri, CORESIP_CallType callType)
{
	CORESIP_Resource_Type type;
	if (callType == CORESIP_CALL_RD) type = Rd;
	else if (callType == CORESIP_CALL_IA) type = IA;
	else type = Tlf;

	RecordPort* recPort = RecordPort::GetRecordPortFromResource(uri, NULL, type);

	if (!recPort) return -1;
	return recPort->RecBYE();
}

/**
 * RecCallStart.	...
 * Envía Inicio llamada telefonia al grabador
 * @param	dir			Direccion de la llamada 
 * @param	priority	Prioridad
 * @param	ori_uri		URI del llamante
 * @param	dest_uri	URI del llamado
 * @param	callIdHdrVal. Valor de la cabecera Call ID.
 * @param	callType. Tipo de llamada
 * @return	0 OK, -1  error.
 */
int SipAgent::RecCallStart(int dir, CORESIP_Priority priority, const pj_str_t *ori_uri, const pj_str_t *dest_uri, const pj_str_t* callIdHdrVal, CORESIP_CallType callType)
{
	CORESIP_Resource_Type type;
	if (callType == CORESIP_CALL_RD) type = Rd;
	else if (callType == CORESIP_CALL_IA) type = IA;
	else type = Tlf;

	RecordPort* recPort = RecordPort::GetRecordPortFromResource(dest_uri, NULL, type);

	if (!recPort) return -1;
	return recPort->RecCallStart(dir, priority, ori_uri, dest_uri, callIdHdrVal);
}

/**
 * RecCallEnd.	...
 * Envía Fin llamada telefonia al grabador
 * @param	cause		causa de desconexion
 * @param	inv_cause	causa de inv->cause
 * @param	disc_origin		origen de la desconexion
 * @param	callIdHdrVal. Valor de la cabecera Call ID.
 * @param   remote_uri.	Uri del remoto
 * @param	callType. Tipo de llamada
 * @return	0 OK, -1  error.
 */
int SipAgent::RecCallEnd(int cause, pjsua_call_media_status media_status, int disc_origin, const pj_str_t* callIdHdrVal, const pj_str_t* remote_uri, CORESIP_CallType callType)
{
	CORESIP_Resource_Type type;
	if (callType == CORESIP_CALL_RD) type = Rd;
	else if (callType == CORESIP_CALL_IA) type = IA;
	else type = Tlf;

	RecordPort* recPort = RecordPort::GetRecordPortFromResource(remote_uri, NULL, type);

	if (!recPort) return -1;
	return recPort->RecCallEnd(cause, media_status, disc_origin, callIdHdrVal);
	
}

/**
 * RecCallConnected.	...
 * @param	connected_uri	Uri con la que se ha conectado
 * @param	callIdHdrVal. Valor de la cabecera Call ID.
 * @param	callType. Tipo de llamada
 * @return	0 OK, -1  error.
 */
int SipAgent::RecCallConnected(const pj_str_t *connected_uri, const pj_str_t* callIdHdrVal, CORESIP_CallType callType)
{
	CORESIP_Resource_Type type;
	if (callType == CORESIP_CALL_RD) type = Rd;
	else if (callType == CORESIP_CALL_IA) type = IA;
	else type = Tlf;

	RecordPort* recPort = RecordPort::GetRecordPortFromResource(connected_uri, NULL, type);

	if (!recPort) return -1;
	return recPort->RecCallConnected(connected_uri, callIdHdrVal);
}

/**
 * RecHold.	...
 * Envia evento Hold.
 * @param	on	true=ON, false=OFF
 * @param	llamante true=llamante false=llamado
 * @param	media_status	media status
 * @param	callIdHdrVal. Valor de la cabecera Call ID.
 * @param	remote_uri
 * @param	callType. Tipo de llamada
 * @return	0 OK, -1  error.
 */
int SipAgent::RecHold(bool on, bool llamante, pjsua_call_media_status media_status, const pj_str_t* callIdHdrVal, const pj_str_t* remote_uri, CORESIP_CallType callType)
{
	CORESIP_Resource_Type type;
	if (callType == CORESIP_CALL_RD) type = Rd;
	else if (callType == CORESIP_CALL_IA) type = IA;
	else type = Tlf;

	RecordPort* recPort = RecordPort::GetRecordPortFromResource(remote_uri, NULL, type);

	if (!recPort) return -1;
	return recPort->RecHold(on, llamante, media_status, callIdHdrVal);
}

/**
 *	RecorderCmd. Se pasan comandos para realizar acciones sobre el grabador VoIP
 *  @param  cmd			Comando
 *	@param	error		Puntero a la Estructura de error
 *	@return				Codigo de Error
 */
int SipAgent::RecorderCmd(CORESIP_RecCmdType cmd, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	switch (cmd)
	{
	case CORESIP_REC_RESET:
		//Cuanquier objeto de grabacion puede enviar el comando al servicio de grabador. Lo manda el primero que no sea NULL
		if (RecordPort::_RecordPortTel != NULL) RecordPort::_RecordPortTel->RecReset();
		else if (RecordPort::_RecordPortRad != NULL) RecordPort::_RecordPortRad->RecReset();
		else if (RecordPort::_RecordPortTelSec != NULL) RecordPort::_RecordPortTelSec->RecReset();
		else if (RecordPort::_RecordPortRadSec != NULL) RecordPort::_RecordPortRadSec->RecReset();
		else if (RecordPort::_RecordPortIA != NULL) RecordPort::_RecordPortIA->RecReset();
		else if (RecordPort::_RecordPortIASec != NULL) RecordPort::_RecordPortIASec->RecReset();
		break;
	case CORESIP_REC_ENABLE:
		RecordPort::End();
		pj_thread_sleep(250);
		RecordPort::Init(pjsua_var.pool, PJ_TRUE, SipAgent::AgentType, SipAgent::uaIpAdd, SipAgent::HostId);
		break;
	case CORESIP_REC_DISABLE:
		RecordPort::End();
		break;
	default:
		ret = CORESIP_ERROR;
		if (error != NULL)
		{
			error->Code = ret;
			strcpy(error->File, __FILE__);
			sprintf(error->Info, "RecorderCmd: %d command is not valid", cmd);
		}
		break;
	}
		
	return ret;
}

/**
 *	RdPttEvent. Se llama cuando hay un evento de PTT
 *  @param  on			true=ON/false=OFF
 *	@param	freqId		Identificador de la frecuencia
 *  @param  dev			Indice del array _SndPorts. Es dispositivo (microfono) fuente del audio.
 *  @param  PTT_type	Tipo de PTT.
 */
void SipAgent::RdPttEvent(bool on, const char *freqId, int dev, CORESIP_PttType PTT_type)
{
	//Este evento de PTT lo llama la aplicacion del HMI cuando cambia el estado de PTT que nos retorna el transmisor
	//tendremos que tener en cuenta el estado del PTT local para dar por bueno este evento

	if (on == false && SipAgent::PTT_local_activado == PJ_TRUE)
	{
		//Si el estado de ptt que nos envia la aplicacion es false pero el estado en local es true
		//entonces puede ser que el transmisor nos haya enviado por su cuenta un estado de ptt falso
		//y por tanto no lo vamos a tener en cuenta.
		return;
	}

	if (on == true && SipAgent::PTT_local_activado == PJ_FALSE)
	{
		//Si el estado de ptt que nos envia la aplicacion es true pero el estado en local es false
		//entonces puede ser que el transmisor nos haya enviado por su cuenta un estado de ptt falso
		//y por tanto no lo vamos a tener en cuenta ya que ese PTT no es nuestro.
		return;
	}

	CORESIP_Resource_Type type = Rd;
	RecordPort* recPort = RecordPort::GetRecordPortFromResource(NULL, NULL, type);

	if (!recPort) return;
	recPort->RecPTT(on, freqId, dev, PTT_type);
}

/**
 *	RdSquEvent. Se llama cuando hay un evento de SQUELCH
 *  @param  on			true=ON/false=OFF
 *  @param  dev			Indice del array _SndPorts. Es dispositivo (microfono) fuente del audio.
 *	@param	freqId		Identificador de la frecuencia
 *	@param	resourceId  Identificador del recurso seleccionado en el bss
 *	@param	bssMethod	Método bss
 *	@param  bssQidx		Indice de calidad
 */
void SipAgent::RdSquEvent(bool on, const char *freqId, const char *resourceId, const char *bssMethod, unsigned int bssQidx)
{
	CORESIP_Resource_Type type = Rd;
	RecordPort* recPort = RecordPort::GetRecordPortFromResource(NULL, &pj_str((char *)resourceId), type);

	if (!recPort) return;
	recPort->RecSQU(on, freqId, resourceId, bssMethod, bssQidx);
}

/**
 * BridgeLink: Rutina para Conectar o Desconectar 'puertos'
 * @param	srcType		Tipo de Puerto Origen. Pueden ser del siguiente tipo:
 *							- CORESIP_CALL_ID: Un flujo RTP
 *							- CORESIP_SNDDEV_ID: Un dispositivo de Audio.
 *							- CORESIP_WAVPLAYER_ID: Reproductor de Fichero Wav.
 *							- CORESIP_RDRXPORT_ID: 
 *							- CORESIP_SNDRXPORT_ID:
 * @param	src			Identificador del Puerto.
 * @param	dstType		Tipo de Puerto de Destino. Pueden ser del siguiente tipo
 *							- CORESIP_CALL_ID: Un flujo RTP
 *							- CORESIP_SNDDEV_ID: Un dispositivo de Audio.
 *							- CORESIP_WAVRECORDER_ID: Grabador de Fichero Wav.
 * @param	dst			Identificador del Puerto de Destino,
 * @param	on			Indica Conexión o Desconexión.
 * @return	Codigo de error 
 */
void SipAgent::BridgeLink(int srcType, int src, int dstType, int dst, bool on)
{
	pjsua_conf_port_id conf_src = PJSUA_INVALID_ID, conf_dst = PJSUA_INVALID_ID;
	pj_bool_t error_src = PJ_FALSE;
	pj_bool_t error_dst = PJ_FALSE;

	if (src < 0 || dst < 0)
	{
		//throw PJLibException(__FILE__, PJ_EINVAL).Msg("BridgeLink:", "src %d y dst %d deben ser mayores o iguales a 0", src, dst);		
		return;
	}

	Guard lock(_Lock);
	PJ_LOG(5, (__FILE__, "######### GRABACION BridgeLink ENTRA on %d src %d dst %d ############", on, src, dst));
	
	char src_string[64];
	char dst_string[64];
	
	/**
	 * Obtiene el Puerto de Conferencia asociado al origen.
	 */
	switch (srcType)
	{
	case CORESIP_CALL_ID:
		PJ_LOG(5, (__FILE__, "######### GRABACION BridgeLink srcType CORESIP_CALL_ID "));
		conf_src = pjsua_call_get_conf_port(src);
		if (conf_src == PJSUA_INVALID_ID) 
		{
			if (!on)
			{
				PJ_LOG(3,(__FILE__, "WARNING: BridgeLink: call_id %d invalido. Posiblemente intentando desconectar audio de una llamada ya finalizada", src));
			}
			else
			{
				PJ_LOG(3,(__FILE__, "ERROR: BridgeLink: call_id %d invalido. Intentando conectar audio de una llamada", src));
			}
			error_src = PJ_TRUE;
		}
		break;
	case CORESIP_SNDDEV_ID:
		RecordPort::GetSndTypeString(_SndPorts[src]->_Type, src_string, sizeof(src_string));
		PJ_LOG(5, (__FILE__, "######### GRABACION BridgeLink srcType CORESIP_SNDDEV_ID %s", src_string));
		if (src >= CORESIP_MAX_SOUND_DEVICES) error_src = PJ_TRUE;
		else if (_SndPorts[src] == NULL) error_src = PJ_TRUE;
		else if (!IsSlotValid(_SndPorts[src]->Slot)) error_src = PJ_TRUE;
		else conf_src = _SndPorts[src]->Slot;
		break;
	case CORESIP_WAVPLAYER_ID:
		PJ_LOG(5, (__FILE__, "######### GRABACION BridgeLink srcType CORESIP_WAVPLAYER_ID "));
		if (src >= CORESIP_MAX_WAV_PLAYERS) error_src = PJ_TRUE;
		else if (_WavPlayers[src] == NULL) error_src = PJ_TRUE;
		else if (!IsSlotValid(_WavPlayers[src]->Slot)) error_src = PJ_TRUE;
		else conf_src = _WavPlayers[src]->Slot;
		break;
	case CORESIP_RDRXPORT_ID:
		PJ_LOG(5, (__FILE__, "######### GRABACION BridgeLink srcType CORESIP_RDRXPORT_ID "));
		if (src >= CORESIP_MAX_RDRX_PORTS) error_src = PJ_TRUE;
		else if (_RdRxPorts[src] == NULL) error_src = PJ_TRUE;
		else if (!IsSlotValid(_RdRxPorts[src]->Slot)) error_src = PJ_TRUE;
		else conf_src = _RdRxPorts[src]->Slot;
		break;
	case CORESIP_SNDRXPORT_ID:
		PJ_LOG(5, (__FILE__, "######### GRABACION BridgeLink srcType CORESIP_SNDRXPORT_ID "));
		if ((src & 0x0000FFFF) >= CORESIP_MAX_SOUND_RX_PORTS) error_src = PJ_TRUE;
		else if (_SndRxPorts[src & 0x0000FFFF] == NULL) error_src = PJ_TRUE;
		else if (!IsSlotValid(_SndRxPorts[src & 0x0000FFFF]->Slots[src >> 16])) error_src = PJ_TRUE;
		else conf_src = _SndRxPorts[src & 0x0000FFFF]->Slots[src >> 16];
		break;
	case CORESIP_GENPORT_ID:
		PJ_LOG(5, (__FILE__, "######### GRABACION BridgeLink srcType CORESIP_GENPORT_ID "));
		if (src >= CORESIP_MAX_GENERIC_PORTS) error_src = PJ_TRUE;
		else if (GenericPort::_Generic_Ports[src] == NULL) error_src = PJ_TRUE;
		else if (!IsSlotValid(GenericPort::_Generic_Ports[src]->_UpStreamSlot)) error_src = PJ_TRUE;
		else conf_src = GenericPort::_Generic_Ports[src]->_UpStreamSlot;
		break;
	case CORESIP_RTPPORT_ID:
		PJ_LOG(5, (__FILE__, "######### GRABACION BridgeLink srcType CORESIP_RTPPORT_ID "));
		if (src >= RTPport::MAX_RTP_PORTS) error_src = PJ_TRUE;
		else if (RTPport::_RTP_Ports[src] == NULL) error_src = PJ_TRUE;
		else if (!IsSlotValid(RTPport::_RTP_Ports[src]->_Slot)) error_src = PJ_TRUE;
		else conf_src = RTPport::_RTP_Ports[src]->_Slot;
		break;
	default:
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("BridgeLink:", "Tipo de Puerto de origen srcType 0x%08x no es valido", srcType);		
	}

	/**
	 * Obtiene el Puerto de Conferencia asociado al Destino.
	 */
	switch (dstType)
	{
	case CORESIP_CALL_ID:
		PJ_LOG(5, (__FILE__, "######### GRABACION BridgeLink dstType CORESIP_CALL_ID "));
		conf_dst = pjsua_call_get_conf_port(dst);
		if (conf_dst == PJSUA_INVALID_ID) 
		{
			if (!on)
			{
				PJ_LOG(3,(__FILE__, "WARNING: BridgeLink: call_id %d invalido. Posiblemente intentando desconectar audio de una llamada ya finalizada", src));
			}
			else
			{
				PJ_LOG(3,(__FILE__, "ERROR: BridgeLink: call_id %d invalido. Intentando conectar audio de una llamada", src));
			}
			error_dst = PJ_TRUE;
		}

		if (srcType == CORESIP_SNDDEV_ID && RecordPort::_RecordPortTel != NULL)
		{
			//Si se conecta un puerto de sonido hacia un puerto del tipo telefonia
			//Entonces hay que conectar ese puerto de sonido con la grabacion
			if (on)
			{
				RecConnectSndPort(on, src, RecordPort::_RecordPortTel);
			}
			else
			{
				int ncalls = NumConfirmedCalls();
				if (ncalls <= 1)
				{
					//Si hay más de una llamada en curso entonces no cortamos el puerto de sonido de la grabación.
					RecConnectSndPort(on, src, RecordPort::_RecordPortTel);
				}
			}			
		}

		break;
	case CORESIP_SNDDEV_ID:
		RecordPort::GetSndTypeString(_SndPorts[dst]->_Type, dst_string, sizeof(dst_string));
		PJ_LOG(5, (__FILE__, "######### GRABACION BridgeLink dstType CORESIP_SNDDEV_ID %s", dst_string));
		if (dst >= CORESIP_MAX_SOUND_DEVICES) error_dst = PJ_TRUE;
		else if (_SndPorts[dst] == NULL) error_dst = PJ_TRUE;
		else if (!IsSlotValid(_SndPorts[dst]->Slot)) error_dst = PJ_TRUE;
		else 
		{
			conf_dst = _SndPorts[dst]->Slot;

			if (_SndPorts[dst]->_Type == CORESIP_SND_LC_SPEAKER)
			{
				//Cada vez que se conecta o desconecta el audio de los altavoces LC reiniciamos el
				//cancelador de eco

				pj_status_t st = pj_lock_acquire(_ECLCMic_mutex);
				if (st != PJ_SUCCESS)
				{
					PJ_LOG(3,(__FILE__, "ERROR: BridgeLink: SipAgent::_ECLCMic_mutex No puede adquirirse"));
				}

				if (on) _AltavozLCActivado = PJ_TRUE;
				else _AltavozLCActivado = PJ_FALSE;
			
				if (SipAgent::_EchoCancellerLCMic != NULL)
				{					
					st = pjmedia_echo_reset(SipAgent::_EchoCancellerLCMic);
					if (st != PJ_SUCCESS)
					{
						PJ_LOG(3,(__FILE__, "ERROR: Reseteando el cancelador de eco"));
					}
				}
			
				st = pj_lock_release(_ECLCMic_mutex);
				if (st != PJ_SUCCESS)
				{
					PJ_LOG(3,(__FILE__, "ERROR: BridgeLink: SipAgent::_ECLCMic_mutex No puede liberarse"));
				}
			}
		}

		if (srcType == CORESIP_CALL_ID)
		{
			//Si el origen es una llamada telefonica y el destino es un dispositivo de salida de audio
			//Grabamos el retorno del dispositivo de salida de audio.
			if (on && RecordPort::_RecordPortTel != NULL && !error_src) 
			{
				int dev_to_record = RecordPort::GetSndDevToRecord(dst);		//Buscamos el dispositivo de retorno de audio del mismo tipo para grabarlo
				if (dev_to_record != -1)
				{
					RecordPort::GetSndTypeString(_SndPorts[dev_to_record]->_Type, src_string, sizeof(src_string));
					PJ_LOG(5, (__FILE__, "######### GRABACION 1 srcType CORESIP_SNDDEV_ID %s a RecordPort::_RecordPortTel", src_string));

					RecConnectSndPort(true, dev_to_record, RecordPort::_RecordPortTel);
				}
			}
			else if (!on && RecordPort::_RecordPortTel != NULL && !error_src)
			{
				int dev_to_record = RecordPort::GetSndDevToRecord(dst);		//Buscamos el dispositivo de retorno de audio del mismo tipo para grabarlo
				if (dev_to_record != -1)
				{
					pj_bool_t from_tlf = PJ_TRUE;
					int slots_connected_to_dst = GetPortsCountAreConnectedToSlot(conf_dst, from_tlf);
					PJ_LOG(5, (__FILE__, "######### GRABACION 0 slots_connected_to_dst %d tlf", slots_connected_to_dst));

					if (slots_connected_to_dst <= 1)
					{	
						//Si se se va a quedar sin slots de telefonia conectados a este dispositivo, entonces lo desconectamos de la grabacion de telefonia
						RecordPort::GetSndTypeString(_SndPorts[dev_to_record]->_Type, src_string, sizeof(src_string));
						PJ_LOG(5, (__FILE__, "######### GRABACION 0 srcType CORESIP_SNDDEV_ID %s a RecordPort::_RecordPortTel", src_string));

						RecConnectSndPort(false, dev_to_record, RecordPort::_RecordPortTel);
					}
				}
			}
		}		

		if (srcType == CORESIP_RDRXPORT_ID)
		{
			//Si el origen es un puerto RDRX y el destino es un dispositivo de salida de audio
			//Grabamos el retorno del dispositivo de salida de audio.
			if (on && RecordPort::_RecordPortRad != NULL && !error_src) 
			{
				int dev_to_record = RecordPort::GetSndDevToRecord(dst);		//Buscamos el dispositivo de retorno de audio del mismo tipo para grabarlo
				if (dev_to_record != -1)
				{
					RecordPort::GetSndTypeString(_SndPorts[dev_to_record]->_Type, src_string, sizeof(src_string));
					PJ_LOG(5, (__FILE__, "######### GRABACION 1 srcType CORESIP_SNDDEV_ID %s a RecordPort::_RecordPortRad", src_string));

					RecConnectSndPort(true, dev_to_record, RecordPort::_RecordPortRad);
				}
			}
			else if (!on && RecordPort::_RecordPortTel != NULL && !error_src)
			{
				int dev_to_record = RecordPort::GetSndDevToRecord(dst);		//Buscamos el dispositivo de retorno de audio del mismo tipo para grabarlo
				if (dev_to_record != -1)
				{
					pj_bool_t not_from_tlf = PJ_FALSE;
					int slots_connected_to_dst = GetPortsCountAreConnectedToSlot(conf_dst, not_from_tlf);
					PJ_LOG(5, (__FILE__, "######### GRABACION 0 slots_connected_to_dst %d radio", slots_connected_to_dst));

					if (slots_connected_to_dst <= 1)
					{
						//Si se se va a quedar sin slots, que no son de telefonia, conectados a este dispositivo, entonces lo desconectamos de la grabacion de radio
						RecordPort::GetSndTypeString(_SndPorts[dev_to_record]->_Type, src_string, sizeof(src_string));
						PJ_LOG(5, (__FILE__, "######### GRABACION 0 srcType CORESIP_SNDDEV_ID %s a RecordPort::_RecordPortRad", src_string));

						RecConnectSndPort(false, dev_to_record, RecordPort::_RecordPortRad);
					}
				}
			}
		}

		break;
	case CORESIP_WAVRECORDER_ID:
		PJ_LOG(5, (__FILE__, "######### GRABACION BridgeLink dstType CORESIP_WAVRECORDER_ID "));
		if (dst >= CORESIP_MAX_WAV_RECORDERS) error_dst = PJ_TRUE;
		else if (_WavRecorders[dst] == NULL) error_dst = PJ_TRUE;
		else if (!IsSlotValid(_WavRecorders[dst]->Slot)) error_dst = PJ_TRUE;
		else conf_dst = _WavRecorders[dst]->Slot;
		break;

	case CORESIP_GENPORT_ID:
		PJ_LOG(5, (__FILE__, "######### GRABACION BridgeLink dstType CORESIP_GENPORT_ID "));
		if (dst >= CORESIP_MAX_GENERIC_PORTS) error_dst = PJ_TRUE;
		else if (GenericPort::_Generic_Ports[dst] == NULL) error_dst = PJ_TRUE;
		else if (!IsSlotValid(GenericPort::_Generic_Ports[dst]->_DownStreamSlot)) error_dst = PJ_TRUE;
		else conf_dst = GenericPort::_Generic_Ports[dst]->_DownStreamSlot;
		break;

	case CORESIP_RTPPORT_ID:
		PJ_LOG(5, (__FILE__, "######### GRABACION BridgeLink dstType CORESIP_RTPPORT_ID "));
		if (dst >= RTPport::MAX_RTP_PORTS) error_dst = PJ_TRUE;
		else if (RTPport::_RTP_Ports[dst] == NULL) error_dst = PJ_TRUE;
		else if (!IsSlotValid(RTPport::_RTP_Ports[dst]->_Slot)) error_dst = PJ_TRUE;
		else conf_dst = RTPport::_RTP_Ports[dst]->_Slot;
		break;

	default:
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("BridgeLink:", "Tipo de Puerto de destino dstType 0x%08x no es valido", dstType);		
	}

	if (error_src || error_dst)
	{
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: BridgeLink: src o dst no valido"));
		return;		//Alguno de los slots no es valido. Por tanto no seguimos.
	}

	/**
	 * Conecta o Desconecta los puertos.
	 */
	if (on)
	{
		pj_status_t st = pjsua_conf_connect(conf_src, conf_dst);
		//PJ_CHECK_STATUS(st, ("ERROR conectando puertos", "(%d --> %d)", conf_src, conf_dst));
		if (st != PJ_SUCCESS)
		{
			throw PJLibException(__FILE__, st).Msg("BridgeLink:", "ERROR conectando puertos %d --> %d", conf_src, conf_dst);		
		}
	}
	else
	{
		pj_status_t st = pjsua_conf_disconnect(conf_src, conf_dst);
		if (st != PJ_SUCCESS)
		{
			throw PJLibException(__FILE__, st).Msg("BridgeLink:", "ERROR desconectando puertos %d --> %d", conf_src, conf_dst);		
		}
		//PJ_CHECK_STATUS(st, ("ERROR desconectando puertos", "(%d --> %d)", conf_src, conf_dst));
	}
}

/**
 * GetPortsCountAreConnectedTo:	Retorna el numero de slots que estan enviando audio a un slot. Es decir, lo tienen como listener
 * @param	slot_id		Slot
 * @param	from_tlf	Indica si los slots son de puertos de telefonia
 * @return	El numero de slots
 */
int SipAgent::GetPortsCountAreConnectedToSlot(pjsua_conf_port_id slot_id, pj_bool_t from_tlf)
{
	pjsua_conf_port_id port_ids[PJSUA_MAX_CONF_PORTS];
	unsigned ports_count = PJ_ARRAY_SIZE(port_ids);	
	pj_status_t status;
	unsigned i;
	int count = 0;

	unsigned call_count = PJSUA_MAX_CALLS;
	pjsua_call_id ids[PJSUA_MAX_CALLS];

	PJSUA_LOCK();
	status = pjsua_enum_calls(ids, &call_count);
	if (status != PJ_SUCCESS)
	{
		PJSUA_UNLOCK();
		return 0;
	}

	status = pjsua_enum_conf_ports(port_ids, &ports_count);
	if (status == PJ_SUCCESS)
	{
		for (i = 0; i < ports_count; i++)
		{
			unsigned j;
			pjsua_conf_port_info port_info;
			status = pjsua_conf_get_port_info(port_ids[i], &port_info);
			if (status == PJ_SUCCESS)
			{
				for (j = 0; j < port_info.listener_cnt; j++)
				{					
					if (port_info.listeners[j] == slot_id)
					{
						pj_bool_t slot_is_in_a_call = PJ_FALSE;
						for (unsigned k = 0; k < call_count; k++)
						{
							pjsua_call_info info;
							status = pjsua_call_get_info(ids[k], &info);
							if (status == PJ_SUCCESS)
							{
								if (port_info.listeners[j] == info.conf_slot)
								{
									slot_is_in_a_call = PJ_TRUE;
									break;
								}
							}
						}

						if (from_tlf && slot_is_in_a_call) count++;
						else if (!from_tlf && !slot_is_in_a_call) count++;
					}
				}
			}
		}
	}

	PJSUA_UNLOCK();

	return count;
}

/**
 * SendToRemote:	Configura El puerto de Sonido apuntado para los envios UNICAST de Audio.
 * @param	dev		Identificador de Dispositivo 'SndPort'
 * @param	on		Indica si se configura o se desconfigura.
 * @param	id		Literal que identifica al emisor.
 * @param	ip		Direccion IP de Destino.
 * @param	port	Puerto UDP de Destino.
 * @return	nada
 */
void SipAgent::SendToRemote(int typeDev, int dev, bool on, const char * id, const char * ip, unsigned port)
{	
	if (typeDev == CORESIP_SNDDEV_ID)
	{
		_SndPorts[dev]->Remote(on, id, ip, port);
		SipAgent::PTT_local_activado = on;			//Esta funcion se llama cuando hay un PTT local en el puesto. Esta variable 
													//guarda el estado del PTT local
		if (RecordPort::_RecordPortRad != NULL)
		{
			//Se conecta el puerto de audio TX (microfono) pasado por dev al puerto de grabacion
			SipAgent::RecConnectSndPort(on, dev, RecordPort::_RecordPortRad);
		}
	}
	else
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("SendToRemote:", "Primer parametro typeDev no valido");
	}
}

/**
 * ReceiveFromRemote:	Configura el múdulo para recibir audio por Multicast. En Nodebox se recibe de los puestos cuando hay PTT
 * @param	localIp		Dirección Ip Local
 * @param	mcastIp		Direccion Ip Multicast de escucha.
 * @param	mcastPort	Puerto UDP de escucha
 * @return	Nada
 */
void SipAgent::ReceiveFromRemote(const char * localIp, const char * mcastIp, unsigned mcastPort)
{
	pj_sockaddr_in addr, mcastAddr;
	pj_sockaddr_in_init(&addr, &(pj_str(const_cast<char*>(localIp))), (pj_uint16_t)mcastPort);
	pj_sockaddr_in_init(&mcastAddr, &(pj_str(const_cast<char*>(mcastIp))), (pj_uint16_t)mcastPort);

	/**
	 * Crea el socket de recepcion.
	 */
	pj_status_t st = pj_sock_socket(pj_AF_INET(), pj_SOCK_DGRAM(), 0, &_Sock);
	PJ_CHECK_STATUS(st, ("ERROR creando socket para puerto de recepcion sndDev radio"));

	try
	{
		int on = 1;
		/**
		 * Configura el socket para que sea 'reutizable' y habilita (joint) el grupo Multicast.
		 */
		pj_sock_setsockopt(_Sock, pj_SOL_SOCKET(), SO_REUSEADDR, (void *)&on, sizeof(on));

		st = pj_sock_bind(_Sock, &addr, sizeof(addr));
		PJ_CHECK_STATUS(st, ("ERROR enlazando socket para puerto de recepcion sndDev radio", "[Ip=%s][Port=%d]", localIp, mcastPort));

		struct ip_mreq	mreq;
		mreq.imr_multiaddr.S_un.S_addr = mcastAddr.sin_addr.s_addr;
		mreq.imr_interface.S_un.S_addr = addr.sin_addr.s_addr;

		st = pj_sock_setsockopt(_Sock, IPPROTO_IP, pj_IP_ADD_MEMBERSHIP(), (void *)&mreq, sizeof(mreq));
		PJ_CHECK_STATUS(st, ("ERROR añadiendo socket a multicast para puerto de recepcion sndDev radio", "[Mcast=%s][Port=%d]", mcastIp, mcastPort));

		/**
		 * Configura el Callback para Recibir Datos.
		 */ 
		pj_activesock_cb cb = { NULL };
		cb.on_data_recvfrom = &OnDataReceived;

		/**
		 * Configura el socket para que funcione como 'Active Socket':
		 *	- http://www.pjsip.org/pjlib/docs/html/group__PJ__ACTIVESOCK.htm#_details
		 */
		st = pj_activesock_create(pjsua_var.pool, _Sock, pj_SOCK_DGRAM(), NULL, pjsip_endpt_get_ioqueue(pjsua_get_pjsip_endpt()), &cb, NULL, &_RemoteSock);
		PJ_CHECK_STATUS(st, ("ERROR creando servidor de lectura para puerto de recepcion sndDev radio"));

		/**
		 * Activa la Recepcion en el Socket.
		 */ 
		unsigned samplesPerFrame = SAMPLING_RATE * CHANNEL_COUNT * PTIME / 1000;
		st = pj_activesock_start_recvfrom(_RemoteSock, pjsua_var.pool, sizeof(RemotePayload), 0);
		PJ_CHECK_STATUS(st, ("ERROR iniciando lectura en puerto de recepcion sndDev radio"));
	}
	catch (...)
	{
		/**
		 * Tratamiento de Errores. Excepciones.
		 *	- Cierra los socket's abierto
		 *	- Reinicializa las variables globales.
		 */
		if (_RemoteSock)
		{
			pj_activesock_close(_RemoteSock);
			_RemoteSock = NULL;
		}
		else if (_Sock != PJ_INVALID_SOCKET)
		{
			pj_sock_close(_Sock);
		}
		_Sock = PJ_INVALID_SOCKET;

		throw;
	}
}

/**
 * SetVolume: Ajusta el volumen de Recepcion en un PORT.
 * @param	idType		Tipo de PORT.
 *							- CORESIP_CALL_ID: Un flujo RTP
 *							- CORESIP_SNDDEV_ID: Un dispositivo de Audio.
 *							- CORESIP_WAVPLAYER_ID: Reproductor de Fichero Wav.
 *							- CORESIP_RDRXPORT_ID: 
 *							- CORESIP_SNDRXPORT_ID:
 * @param	id			Identificador del PORT.
 * @param	volume		Es el porcentaje del nivel maximo para cada dispoisitivo, segun winaudio.ini
 * @return	Nada
 *
 * INFO: http://www.pjsip.org/pjmedia/docs/html/group__PJMEDIA__CONF.htm#ga8895228fdc9b7d6892320aa03b198574
 */
void SipAgent::SetVolume(int idType, int id, unsigned volume)
{
	pjsua_conf_port_id conf_id = PJSUA_INVALID_ID;

	if (id < 0)
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("SetVolume:", "Segundo parametro (id=%d) debe ser mayor o igual a 0", id);		
	}

	/**
	 * Obtiene el Puerto de Conferencia asociado al origen.
	 */

	pj_bool_t Snd_output_device_found = PJ_FALSE;
	switch (idType)
	{
	case CORESIP_CALL_ID:
		conf_id = pjsua_call_get_conf_port(id);
		if (conf_id != PJSUA_INVALID_ID) 
			Snd_output_device_found = SoundDevHw::SetVolumeToListenerSndPort(conf_id, volume);
		break;
	case CORESIP_SNDDEV_ID:
		if (id >= CORESIP_MAX_SOUND_DEVICES) break;
		if (_SndPorts[id] == NULL) break;
		if (!IsSlotValid(_SndPorts[id]->Slot)) break;
		conf_id = _SndPorts[id]->Slot;
		if (conf_id != PJSUA_INVALID_ID)
		{
			if (_SndPorts[id]->_Type == CORESIP_SND_ALUMN_MHP || _SndPorts[id]->_Type == CORESIP_SND_INSTRUCTOR_MHP ||
				_SndPorts[id]->_Type == CORESIP_SND_RD_SPEAKER || _SndPorts[id]->_Type == CORESIP_SND_LC_SPEAKER || 
				_SndPorts[id]->_Type == CORESIP_SND_HF_SPEAKER)
			{
				SoundDevHw::SetVolumeOutputDevice(_SndPorts[id]->_Type, volume);
				Snd_output_device_found = PJ_TRUE;
				break;
			}
		}
		break;
	case CORESIP_WAVPLAYER_ID:
		if (id >= CORESIP_MAX_WAV_PLAYERS) break;
		if (_WavPlayers[id] == NULL) break;
		if (!IsSlotValid(_WavPlayers[id]->Slot)) break;
		conf_id = _WavPlayers[id]->Slot;
		if (conf_id != PJSUA_INVALID_ID) 
			Snd_output_device_found = SoundDevHw::SetVolumeToListenerSndPort(conf_id, volume);
		break;
	case CORESIP_RDRXPORT_ID:
		if (id >= CORESIP_MAX_RDRX_PORTS) break;
		if (_RdRxPorts[id] == NULL) break;
		if (!IsSlotValid(_RdRxPorts[id]->Slot)) break;
		conf_id = _RdRxPorts[id]->Slot;
		if (conf_id != PJSUA_INVALID_ID) 
			Snd_output_device_found = SoundDevHw::SetVolumeToListenerSndPort(conf_id, volume);
		break;
	case CORESIP_SNDRXPORT_ID:
		if ((id & 0x0000FFFF) >= CORESIP_MAX_SOUND_RX_PORTS) break;
		if (_SndRxPorts[id & 0x0000FFFF] == NULL) break;
		if (!IsSlotValid(_SndRxPorts[id & 0x0000FFFF]->Slots[id >> 16])) break;
		conf_id = _SndRxPorts[id & 0x0000FFFF]->Slots[id >> 16];
		if (conf_id != PJSUA_INVALID_ID) 
			Snd_output_device_found = SoundDevHw::SetVolumeToListenerSndPort(conf_id, volume);
		break;
	default:
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("SetVolume:", "Primer parametro (Tipo dispositivo 0x%08x) no valido", idType);
	}

	if (conf_id == PJSUA_INVALID_ID)
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("SetVolume:", "Segundo parametro (id=%d) no valido", id);
	}

	/**
	 * Si no hay error ajusta el volumen.
	 */
	if (!Snd_output_device_found)
	{
		pj_status_t st = pjmedia_conf_adjust_rx_level(pjsua_var.mconf, conf_id, (256 * ((int)volume - 50)) / 100);
		PJ_CHECK_STATUS(st, ("SetVolume:", "ERROR ajustando volumen idType=%d, id=%d, volume=%u", idType, id, volume));
		PJ_LOG(3, (__FILE__, "INFO: SipAgent::SetVolume: idType 0x%x id %d. No se encuentra dispositivo de reproduccion. Se aplica volumen con pjmedia_conf_adjust_rx_level", idType, id));
	}
	else
	{
		pj_status_t st = pjmedia_conf_adjust_rx_level(pjsua_var.mconf, conf_id, 64);
	}
}

/**
 * GetVolume: Obtiene el valor de Volumen de recepcion asociado a un PORT.
 * @param	idType		Tipo de PORT.
 *							- CORESIP_CALL_ID: Un flujo RTP
 *							- CORESIP_SNDDEV_ID: Un dispositivo de Audio.
 *							- CORESIP_WAVPLAYER_ID: Reproductor de Fichero Wav.
 *							- CORESIP_RDRXPORT_ID: 
 *							- CORESIP_SNDRXPORT_ID:
 * @param	id			Identificador del PORT.
 * @return	Valor del Volumen. Rango 0 (Mute) y 100 (Maximo)
 */
unsigned SipAgent::GetVolume(int idType, int id)
{
	pjsua_conf_port_id conf_id = PJSUA_INVALID_ID;

	if (id < 0)
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("GetVolume:", "Segundo parametro (id=%d) debe ser mayor o igual a 0", id);		
	}

	/**
	 * Obtiene el Puerto de Conferencia asociado al origen.
	 */
	switch (idType)
	{
	case CORESIP_CALL_ID:
		conf_id = pjsua_call_get_conf_port(id);
		break;
	case CORESIP_SNDDEV_ID:
		if (id >= CORESIP_MAX_SOUND_DEVICES) break;
		if (_SndPorts[id] == NULL) break;
		if (!IsSlotValid(_SndPorts[id]->Slot)) break;
		conf_id = _SndPorts[id]->Slot;
		break;
	case CORESIP_WAVPLAYER_ID:
		if (id >= CORESIP_MAX_WAV_PLAYERS) break;
		if (_WavPlayers[id] == NULL) break;
		if (!IsSlotValid(_WavPlayers[id]->Slot)) break;
		conf_id = _WavPlayers[id]->Slot;
		break;
	case CORESIP_RDRXPORT_ID:
		if (id >= CORESIP_MAX_RDRX_PORTS) break;
		if (_RdRxPorts[id] == NULL) break;
		if (!IsSlotValid(_RdRxPorts[id]->Slot)) break;
		conf_id = _RdRxPorts[id]->Slot;
		break;
	case CORESIP_SNDRXPORT_ID:
		if ((id & 0x0000FFFF) >= CORESIP_MAX_SOUND_RX_PORTS) break;
		if (_SndRxPorts[id & 0x0000FFFF] == NULL) break;
		if (!IsSlotValid(_SndRxPorts[id & 0x0000FFFF]->Slots[id >> 16])) break;
		conf_id = _SndRxPorts[id & 0x0000FFFF]->Slots[id >> 16];
		break;
	default:
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("GetVolume:", "Primer parametro (Tipo dispositivo 0x%08x) no valido", idType);
	}

	if (conf_id == PJSUA_INVALID_ID)
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("GetVolume:", "Segundo parametro (id=%d) no valido", id);
	}

	/**
	 * Si no hay error, devuelve un valor entre 0 y 100.
	 */
	pjmedia_conf_port_info info;
	pj_status_t st = pjmedia_conf_get_port_info(pjsua_var.mconf, conf_id, &info);
	PJ_CHECK_STATUS(st, ("GetVolume:", "ERROR tomando volumen idType=%d, id=%d", idType, id));

	return (int)((info.rx_adj_level + 128) / 2,56);
}

/**
 * OnRxResponse:	Callback. Se invoca cuando se recibe una request SIP
 *						- Cuando corresponde a un metodo 'subscribe' no asociado a un invite. No lleva to-tag
 * @param	rdata		Puntero 'pjsip_rx_data' a los datos recibidos.
 * @return	'pj_bool_t' Siempre true si tiene exito.
 *
 */
pj_bool_t SipAgent::OnRxRequest(pjsip_rx_data *rdata)
{
	pjsip_method *req_method = &rdata->msg_info.msg->line.req.method;	

	if (
		(rdata->msg_info.to->tag.slen == 0) &&
		(rdata->msg_info.msg->line.req.method.id == PJSIP_OTHER_METHOD) &&
		(pjsip_method_cmp(req_method, pjsip_get_subscribe_method()) == 0)
		)
	{
		if (ConfSubs::SubscriptionRxRequest(rdata) == PJ_TRUE)
		{
			return PJ_TRUE;
		}
		else if (DlgSubs::SubscriptionRxRequest(rdata) == PJ_TRUE)
		{
			return PJ_TRUE;
		}
		else if (WG67Subs::SubscriptionRxRequest(rdata, NULL) == PJ_TRUE)
		{
			return PJ_TRUE;
		}
	}
	return PJ_FALSE;
}

/**
 * OnRxResponse:	Callback. Se invoca cuando se recibe una 'respuesta' SIP
 *						- Cuando la Respuesta corresponde a un metodo 'option'
 *						- Invoca a una rutina de Aplicacion 'OptionsReceive' Indicando el URI en un string.
 * @param	rdata		Puntero 'pjsip_rx_data' a los datos recibidos.
 * @return	'pj_bool_t' Siempre retorna 'PJ_FALSE', para que continue el tratamiento del evento la propia libreria.
 *
 *	INFO: http://www.pjsip.org/pjsip/docs/html/group__PJSIP__MSG__METHOD.htm#gafdd26e26092275d7f156a0d8efe90b78
 *		  http://www.pjsip.org/pjsip/docs/html/group__PJSIP__MSG__METHOD.htm#ga66b57e1b5645d2ee843141a0e657b0d1
 */
pj_bool_t SipAgent::OnRxResponse(pjsip_rx_data *rdata)
{
	return PJ_FALSE;
}

/**
 * OnWavPlayerEof: Callback. Se llama al finalizar la reproducción deun fichero wav
 * @param	port		Puntero 'pj_media_port' al puerto Implicado.
 * @param	userData	Datos de Usuario. Por configuracion del sistema debe corresponder al id del Reproductor
 * @return	Si no hay excepciones, retorna siempre PJ_EEOF.
 */
pj_status_t SipAgent::OnWavPlayerEof(pjmedia_port *port, void *userData)
{
	if (TRACE_ALL_CALLS) PJ_LOG(3, (__FILE__, "FinWavCb"));
	if (Cb.FinWavCb) Cb.FinWavCb((int)(size_t)userData | CORESIP_WAVPLAYER_ID);
	DestroyWavPlayer((int)(size_t)userData);
	return PJ_EEOF;
}

/**
 * OnDataReceived: Callback Recepcion Audio Multicast.
 * @param	asock		Puntero 'pj_activesock' al SOCKET involucrado en el proceso de Recepcion.
 * @param	data		Puntero a los datos recibidos.
 * @param	size		Tamaño de los datos recibidos.
 * @param	src_addr	Puntero a Direccion Ip que envia los datos.
 * @param	addr_len	Longitud del campo de Direccion Ip.
 * @param	status		Estado de los Datos Recibidos.
 * @return	PJ_TRUE
 */
pj_bool_t SipAgent::OnDataReceived(pj_activesock_t * asock, void * data, pj_size_t size, const pj_sockaddr_t *src_addr, int addr_len, pj_status_t status)
{
	/**
	 * Comprueba que no hay error en la recepcion y es una trama del sistema.
	 */
	if ((status == PJ_SUCCESS) && (size == sizeof(RemotePayload)))
	{
		RemotePayload * pl = (RemotePayload*)data;
		pj_assert(pl->Size == (SAMPLES_PER_FRAME * (BITS_PER_SAMPLE / 8)));
		Guard lock(_Lock);

		/**
		 * Envia la trama a todos los puertos tipo @ref SoundRxPort
		 */
		std::map<std::string, SoundRxPort*>::iterator it = _SndRxIds.find(pl->SrcId);
		if (it != _SndRxIds.end())
		{
			it->second->PutFrame(pl->SrcType, pl->Data, pl->Size);
		}
	}

	return PJ_TRUE;
}

#ifdef PJ_USE_ASIO

/**
 * RecCb: Callback. Se llama cada vez que se captura una trama de Audio en Recepcion (en RTP).
 * - Se compila solo con la Macro 'PJ_USE_ASIO' activada.
 *
 * @param	userData	Puntero a datos de Usuario. 
 * @param	frame		Puntero 'pjmedia_frame' a la trama recibida.
 * @return	PJ_SUCCESS
 *
 * INFO: http://www.pjsip.org/pjmedia/docs/html/group__s2__audio__device__reference.htm#ga476ecc42ccb9744bbc40e42d457ad1aa
 */
pj_status_t SipAgent::RecCb(void * userData, pjmedia_frame * frame)
{
	/**
	 * Comprueba que la trama corresponde por tamaño a una trama RTP del sistema.
	 */
	pj_assert(frame->size == ((SipAgent::SndSamplingRate * CHANNEL_COUNT * PTIME) / 1000) * (BITS_PER_SAMPLE / 8) * _NumInChannels);

	/**
	 * Envia la trama recibida a todos los canales de Entrada. Los objetos son del tipo @ref SoundPort
	 */ 
	if (_NumInChannels)
	{
		Guard lock(_Lock);

		if (_SndDev == NULL) return PJ_SUCCESS;

		for (int i = 0, inIdx = 0; i < PJ_ARRAY_SIZE(_SndPorts); i++)
		{
			SoundPort * sndPort = _SndPorts[i];

			if (sndPort && (sndPort->InDevIndex == _InChannels[inIdx]))
			{
				sndPort->SetInBuf(frame, inIdx * ((SipAgent::SndSamplingRate * CHANNEL_COUNT * PTIME) / 1000) * (BITS_PER_SAMPLE / 8));
				inIdx++;
			}
		}

	}

	/**
	 * Hace un 'get_frame' al @ref _ConfMasterPort.
	 * En los SipAgent asociados a las llamadas Radio (que ejecuta nodebox.master), las tramas recibidas se han
	 * transmitido al grupo mcast asociado, y además se han introducido en el Buffer de Jitter, por lo cual hay
	 * que sacarlas y ya no se utilizan para nada.
	 */
	if (!_NumOutChannels)
	{
		pjmedia_frame f = { PJMEDIA_FRAME_TYPE_NONE };
		pj_int16_t buf[SAMPLES_PER_FRAME];

		f.buf = buf;
		f.size = SAMPLES_PER_FRAME * (BITS_PER_SAMPLE / 8);
		f.timestamp.u64 = frame->timestamp.u64;

		_ConfMasterPort->get_frame(_ConfMasterPort, &f);
	}

			
	/** AGL. Tick Multimedia */
	if (_wp2r != 0)	
	{
		if (_wp2r->Tick()==FALSE)		
		{		
			DestroyWavPlayer2Remote();			
		}		
	}
	/** */

	return PJ_SUCCESS;
}

/**
 * PlayCb: Callback. Se llamaa cada vez que los dispositivos (puertos) necesitan AUDIO que reproducir.
 * - Se compila solo con la Macro 'PJ_USE_ASIO' activada.
 *
 * @param	userData	Puntero a datos recibidos.
 * @param	frame		Puntero 'pjmedia_frame' a ...
 * @return	PJ_SUCCESS
 *
 * INFO: http://www.pjsip.org/pjmedia/docs/html/group__s2__audio__device__reference.htm#ga476ecc42ccb9744bbc40e42d457ad1aa
 */
pj_status_t SipAgent::PlayCb(void * userData, pjmedia_frame * frame)
{
	/**
	 * Comprueba que el frame enviado corresponde a una trama del sisema.
	 */
	pj_assert(frame->size == ((SipAgent::SndSamplingRate * CHANNEL_COUNT * PTIME) / 1000) * (BITS_PER_SAMPLE / 8) * _NumOutChannels);

	/**
	 * Solo si hay canales (puertos) de salida.
	 */ 
	if (_NumOutChannels)
	{
		Guard lock(_Lock);

		if (_SndDev == NULL) return PJ_SUCCESS;

		/**
			* Envia a cada puerto de salida configurado la trama.
			*/
		for (int i = 0, outIdx = 0; i < PJ_ARRAY_SIZE(_SndPorts); i++)
		{
			SoundPort* sndPort = _SndPorts[i];

			if (sndPort && (sndPort->OutDevIndex == _OutChannels[outIdx]))
			{
				sndPort->SetOutBuf(frame, outIdx * ((SipAgent::SndSamplingRate * CHANNEL_COUNT * PTIME) / 1000) * (BITS_PER_SAMPLE / 8));
				outIdx++;
			}
		}

		/**
			* Hace un 'get_frame' al @ref _ConfMasterPort. ???
			*/
		pjmedia_frame f = { PJMEDIA_FRAME_TYPE_NONE };
		pj_int16_t buf[SAMPLES_PER_FRAME];

		f.buf = buf;
		f.size = SAMPLES_PER_FRAME * (BITS_PER_SAMPLE / 8);
		f.timestamp.u64 = frame->timestamp.u64;

		_ConfMasterPort->get_frame(_ConfMasterPort, &f);
	}

	return PJ_SUCCESS;
}

#endif

/** AGL */
WavPlayerToRemote *SipAgent::_wp2r=0;
int SipAgent::CreateWavPlayer2Remote(const char *filename, const char * id, const char * ip, unsigned port)
{
	if (_wp2r == 0)
	{
		_wp2r = new WavPlayerToRemote(filename, PTIME, 0);
		_wp2r->Send2Remote(id, ip, port);
	}
	return 0;
}

void SipAgent::DestroyWavPlayer2Remote()
{
	if (_wp2r != 0)
	{
		delete _wp2r;
		_wp2r=0;
	}
}

int SipAgent::CreatePresenceSubscription(char *dest_uri)
{
	int ret = CORESIP_OK;

	if (_PresenceManager)
	{
		if (_PresenceManager->Add(dest_uri) != 0) ret = CORESIP_ERROR;
	}
	else
	{
		ret = CORESIP_ERROR;
		PJ_CHECK_STATUS(-1, ("ERROR: SipAgent::CreatePresenceSubscription: _PresenceManager no inicializado"));		
	}

	return ret;
}

int SipAgent::DestroyPresenceSubscription(char *dest_uri)
{
	int ret = CORESIP_OK;

	if (_PresenceManager)
	{
		if (_PresenceManager->Remove(dest_uri) != 0) ret = CORESIP_ERROR;
	}
	else
	{
		ret = CORESIP_ERROR;
		PJ_CHECK_STATUS(-1, ("ERROR: SipAgent::CreatePresenceSubscription: _PresenceManager no inicializado"));		
	}

	return ret;
}

int SipAgent::CreateConferenceSubscription(int acc_id, pjsua_call_id call_id, char *dest_uri, pj_bool_t by_proxy)
{
	int ret = CORESIP_OK;

	if (_ConfManager)
	{
		if (_ConfManager->Remove(-1, dest_uri) != 0) ret = CORESIP_ERROR;
#ifdef _DEBUG
		else if (_ConfManager->Add(acc_id, call_id, dest_uri, 60, PJ_FALSE, by_proxy) != 0) ret = CORESIP_ERROR;
#else
		else if (_ConfManager->Add(acc_id, -1, dest_uri, -1, PJ_FALSE, by_proxy) != 0) ret = CORESIP_ERROR;
#endif
	}
	else
	{
		ret = CORESIP_ERROR;
		PJ_CHECK_STATUS(-1, ("ERROR: SipAgent::CreateConferenceSubscription: _ConfManager no inicializado"));		
	}

	return ret;
}

int SipAgent::DestroyConferenceSubscription(char *dest_uri)
{
	int ret = CORESIP_OK;

	if (_ConfManager)
	{
		if (_ConfManager->Remove(-1, dest_uri) != 0) ret = CORESIP_ERROR;
	}
	else
	{
		ret = CORESIP_ERROR;
		PJ_CHECK_STATUS(-1, ("ERROR: SipAgent::DestroyConferenceSubscription: _ConfManager no inicializado"));		
	}

	return ret;
}

int SipAgent::CreateDialogSubscription(int acc_id, char *dest_uri, pj_bool_t by_proxy)
{
	int ret = CORESIP_OK;

	if (_DlgManager)
	{
		if (_DlgManager->Remove(-1, dest_uri) != 0) ret = CORESIP_ERROR;
#ifdef _DEBUG
		else if (_DlgManager->Add(acc_id, -1, dest_uri, 60, PJ_FALSE, by_proxy) != 0) ret = CORESIP_ERROR;
#else
		else if (_DlgManager->Add(acc_id, -1, dest_uri, -1, PJ_FALSE, by_proxy) != 0) ret = CORESIP_ERROR;
#endif
	}
	else
	{
		ret = CORESIP_ERROR;
		PJ_CHECK_STATUS(-1, ("ERROR: SipAgent::CreateDialogSubscription: _DlgManager no inicializado"));		
	}

	return ret;
}

int SipAgent::DestroyDialogSubscription(char *dest_uri)
{
	int ret = CORESIP_OK;

	if (_DlgManager)
	{
		if (_DlgManager->Remove(-1, dest_uri) != 0) ret = CORESIP_ERROR;
	}
	else
	{
		ret = CORESIP_ERROR;
		PJ_CHECK_STATUS(-1, ("ERROR: SipAgent::DestroyDialogSubscription: _DlgManager no inicializado"));		
	}

	return ret;
}

/**
 * SendInstantMessage. Envia un mensaje instantaneo
 *
 * @param	acc_id		Account ID to be used to send the request.
 * @param	dest_uri	Uri del destino del mensaje. Acabado en 0.
 * @param	text		Texto plano a enviar. Acabado en 0
 * @param	by_proxy	Si es true el mensaje se envia por el proxy
 * @return	PJ_SUCCESS
 *
 */
int SipAgent::SendInstantMessage(int acc_id, char *dest_uri, char *text, pj_bool_t by_proxy)
{	
	pj_status_t st;
	if (by_proxy)
	{
		st = pjsua_im_send(acc_id, &pj_str(dest_uri), NULL,  &pj_str(text), NULL, NULL);
	}
	else
	{
		st = pjsua_im_send_no_proxy(acc_id, &pj_str(dest_uri), NULL,  &pj_str(text), NULL, NULL);
	}
	PJ_CHECK_STATUS(st, ("ERROR: SipAgent::SendInstantMessage: No se puede enviar ", "acc_id=%d dest=%s text %s", acc_id, dest_uri, text));
	return CORESIP_OK;
}

/**
* Notify application on incoming pager (i.e. MESSAGE request).
* Argument call_id will be -1 if MESSAGE request is not related to an
* existing call.
*
* See also \a on_pager2() callback for the version with \a pjsip_rx_data
* passed as one of the argument.
*
* @param call_id	    Containts the ID of the call where the IM was
*			    sent, or PJSUA_INVALID_ID if the IM was sent
*			    outside call context.
* @param from	    URI of the sender.
* @param to	    URI of the destination message.
* @param contact	    The Contact URI of the sender, if present.
* @param mime_type	    MIME type of the message.
* @param body	    The message content.
*/
void SipAgent::OnPager(pjsua_call_id call_id, const pj_str_t *from,
		     const pj_str_t *to, const pj_str_t *contact,
		     const pj_str_t *mime_type, const pj_str_t *body)
{
	PJ_LOG(5,(__FILE__, "SipAgent::OnPager: Recibido mensaje call_id %d, from %s, to %s contact %s mime_type %s body %s", 
		call_id, from->ptr, to->ptr, contact->ptr, mime_type->ptr, body->ptr)); 
	
	if (SipAgent::Cb.PagerCb)
	{
		pj_pool_t *tmppool = pjsua_pool_create(NULL, 512, 64);
		pj_assert(tmppool != NULL);

		pj_str_t tmpfrom;
		pj_strdup_with_null(tmppool, &tmpfrom, from);
		pj_str_t tmpto;
		pj_strdup_with_null(tmppool, &tmpto, to);
		pj_str_t tmpcontact;
		pj_strdup_with_null(tmppool, &tmpcontact, contact);
		pj_str_t tmpmime_type;
		pj_strdup_with_null(tmppool, &tmpmime_type, mime_type);
		pj_str_t tmpbody;
		pj_strdup_with_null(tmppool, &tmpbody, body);

		if (TRACE_ALL_CALLS) PJ_LOG(3, (__FILE__, "PagerCb"));
		SipAgent::Cb.PagerCb(tmpfrom.ptr, (int)tmpfrom.slen, tmpto.ptr, (int)tmpto.slen, tmpcontact.ptr, (int)tmpcontact.slen,
			tmpmime_type.ptr, (int)tmpmime_type.slen, tmpbody.ptr, (int)tmpbody.slen);

		pj_pool_release(tmppool);
	}
	
}

void SipAgent::ReadiniFile()
{
	//Lee el fichero ini y rellena la structura Coresip_Local_Config
	char curdir[256];
	char inipath[512];
	inipath[0] = '\0';

	if(GetCurrentDirectory(sizeof(curdir), curdir) > 0)
	{
		strcpy(inipath, curdir);
		strcat(inipath, "\\coresip.ini");
	}

	UINT DBSS = GetPrivateProfileInt("CORESIP", "Debug_BSS", 0, inipath);
	if (DBSS) Coresip_Local_Config._Debug_BSS = PJ_TRUE;
	else Coresip_Local_Config._Debug_BSS = PJ_FALSE;

	UINT Simular_menor_Qidx_rama_mas_lenta = GetPrivateProfileInt("CORESIP", "Simular_menor_Qidx_rama_mas_lenta", 0, inipath);
	if (Simular_menor_Qidx_rama_mas_lenta) Coresip_Local_Config._Simular_menor_Qidx_rama_mas_lenta = PJ_TRUE;
	else Coresip_Local_Config._Simular_menor_Qidx_rama_mas_lenta = PJ_FALSE;	

	UINT Test_ED137C_Dynamic_Delay_Compensation = GetPrivateProfileInt("CORESIP", "Test_ED137C_Dynamic_Delay_Compensation", 0, inipath);
	if (Test_ED137C_Dynamic_Delay_Compensation) Coresip_Local_Config._Test_ED137C_Dynamic_Delay_Compensation = PJ_TRUE;
	else Coresip_Local_Config._Test_ED137C_Dynamic_Delay_Compensation = PJ_FALSE;

	/*Ejemplo fichero coresip.ini*/

	/*

	[CORESIP]
	Debug_BSS=1
	Simular_menor_Qidx_rama_mas_lenta=1
	Simular_receptor_mas_rapido_con_menor_Qidx=1
	*/
}

/** */
