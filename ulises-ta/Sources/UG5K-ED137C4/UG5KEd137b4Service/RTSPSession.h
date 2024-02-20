#if !defined(RTSPSESSION_H__INCLUDED_)
#define RTSPSESSION_H__INCLUDED_

//#define CODE_FOR_TUNNING			1		//ESC. 11.02.2016. COMENTAR


// #pragma once

#include "MiSocket.h"
#include "Thread.h"
#include "RTPSession.h"
#include "RTSPMessages.h"

#include "LocalConfig.h"
#include "NLOG.h"

#include "UG5KRecordService.h"	//ESC. 27.01.2016. A�adido para acceso a esta objeto "com�n"
#include "Mtimer.h"

#include <string>
#include <map>
#include <list>
#include <queue>


/** Tipos de Sesiones */
enum eRtspTipos
{
	eUA_GRS=0,					// Sesion de Radio GW. 18.07.2016: se emplea tanto para GRS como para VCS-Radio
	eUA_VcsRad=1,				// Sesion de Terminal en operacion radio.
	eUA_VcsTel=2				// Sesion de Terminal en operacion telefonia
};

/** Valores de los estados de la Session */
enum eRtspStd 
{
	eRtspSckDisc=0,				// TCP Desconectado, pero la intenci�n es abrir conexi�n con SRV ASAP.
	eRtspCnxSck=1,				// Socket Conectado.	Admite ANNOUNCE.
	eRtspAnnc=2,				// ANNOUNCE efectuado.	Admite SETUP.
	eRtspSetup=3,				// SETUP efectuado.		Admite RECORD, PARAM, TEARDOWN
	eRtspGrabando=4,			// RECORD efectuado.	Admite PAUSE, PARAM, TEARDOWN, MEDIA
	eRtspTearDown=5,			// TEARDOWN efectuado.
};

/** Modos de Transporte de MEDIA */
/* NOTA: De momento s�lo se usa Embedded
enum eRtspMediaTransport
{
	eTrpNone = 0,
	eTrpEmbedded = 1,
	eTrpRtpOverTcp = 2,
	eTrpRtpOverUdp = 3
};
*/

enum eRecorder
{
	eRecA=1,
	eRecB=2,
	eRecAll=9
};

/** Configuracion de la session */
struct RTSPConfig
{
	CIPAddress CIPA_Recorder;
//	std::string uri_frmt;
	std::string recurso;
	std::string IP_Recorder;				//ESC. 23.12.2015. A�adido.
	int			Port_Recorder;				//ESC. 12.01.2016. A�adido.
};

/** Ordenes encoladas */
typedef struct
{
	int tipo;
	string connref;
	rtsp_props propiedades;					//### ESC. 19.01.2016. A�adido
	rtsp_props operaciones;					//ESC. 18.01.2016. Antes miembro era "propiedades"
	void * pMedia;
	int tamMedia;
}  RTSPOrder;


class RTSPSession : public CThread
{
public:
	RTSPSession(void);
	RTSPSession(std::string recurso, int _tipo= eUA_GRS, UG5KRecordService *pRecordService= NULL);

	~RTSPSession(void);

public:
	void Run();

public:			// Ordenes a la sesion...
	bool AbreConexionTCP(eRecorder nRec);
	bool EjecutaAnnounce(eRecorder nRec);
	bool EjecutaSetup(eRecorder nRec);

	RTSPOrder PushmsjeRecord_onlyOper(rtsp_props operaciones, string connref);
	RTSPOrder PushmsjeRecord_onlyOper(rtsp_props operaciones, string connref, eRecorder nRec);
	void PushmsjeRecord_onlyProp(rtsp_props propiedades, string connref);				//ESC. 26.01.2017. A�adido
	RTSPOrder PushmsjeRecord(rtsp_props propiedades, rtsp_props operaciones, string connref);
	RTSPOrder PushmsjeRecord(rtsp_props propiedades, rtsp_props operaciones, string connref, eRecorder nRec);

	RTSPOrder PushmsjePause_onlyOper(rtsp_props operaciones, string connref);
	RTSPOrder PushmsjePause(rtsp_props propiedades, rtsp_props operaciones, string connref);

	void PushmsjeParam_onlyOper(rtsp_props operaciones, string connref);				//GRS y VCS-Radio. SET_PARAM con Operaciones
	void PushmsjeParam_onlyProp(rtsp_props propiedades, string connref);				//26.01.2017. VCS-Telephony. SET_PARAM con Propiedades
	void PushmsjeParam(rtsp_props propiedades, rtsp_props operaciones, string connref);	//ESC. 19.01.2016. A�adido

	void PushmsjeTearDown();
	bool PushframeMedia(std::string paquete, int tam, void * pMedia);
	void PushmsjeSIP_INVITE();
	void PushmsjeSIP_BYE();												//ESC. VOTER

protected:
	void Init(std::string recurso);
#ifdef _WIN32
	HANDLE m_hThreadB;
	static DWORD WINAPI sRunB(LPVOID pParam);
#else
	pthread_t m_hThreadB;
	static void* sRunB(void* pParam);
    bool end_sRunB;
#endif

	bool AutomataConectandoTCP(eRecorder nRec);
	void AutomataAbriendoSesionRTSP(eRecorder nRec);
	void AutomataGestionOrdenes(eRecorder nRec);
	void AutomataEsperaCmdUA(eRecorder nRec);

	bool MetodoAnnounce(eRecorder nRec);
	bool MetodoSetup(eRecorder nRec);
	bool ProcessRecord(rtsp_props propiedades, rtsp_props operaciones, string connref, eRecorder nRec);
	bool ProcessPause(rtsp_props propiedades, rtsp_props operaciones, string connref, eRecorder nRec);
	bool ProcessParam(rtsp_props propiedades, rtsp_props operaciones, string connref, eRecorder nRec);
	bool ProcessTearDown(eRecorder nRec);
	void ProcessSipBYE(eRecorder nRec);												//ESC. 18.01.2017. VOTER
	void VaciarCola_Ordenes(eRecorder nRec);											//ESC. 23.01.2017
	bool ProcessKeepAlive(eRecorder nRec);

	void MediaTransportInit(eRecorder nRec);

	bool SendAndProcessCmdSesion(std::string cmd, eRecorder nRec);
	unsigned char* CreateEmbeddedMedia(char* media, int nbytes, eRecorder nRec, int& pcktFrame_size);
	bool SendEmbeddedMedia(unsigned char* pcktFrame, int pcktFrame_size, eRecorder nRec);
	bool SendCmdSesion(std::string cmd, eRecorder nRec);
	bool ProcesaRespuestaRecorder(eRecorder nRec);						//ESC. 11.02.2016

#ifdef _WIN32
	void ConfiguraParamsTCP_KeepAlive_Windows(CTCPSocket &sckRec);		//ESC. 05.06.2017
#else
	void ConfiguraParamsTCP_KeepAlive_Linux(CTCPSocket &sckRec);		//ESC. 10.04.2017
	void Configura_QoS_Linux(CTCPSocket &sckRec);
#endif

	std::string GetHeaderVal(std::string header, eRecorder nRec);

private:
	void GeneraRtspURI(eRecorder nRec);						//ESC. 02.02.2016
	void PushPriority_Orden(RTSPOrder orden, eRecorder nRec);
	void CheckSizeBeforePush_Orden(RTSPOrder orden, eRecorder nRec);		//ESC. 28.01.2016
	bool SetSocketBlockingModeON(int iHndlSck, bool bBlocking);
	unsigned int uiCountEvSobrecargaPrioA;
	unsigned int uiCountEvSobrecargaPrioB;
	unsigned int uiCountEvSobrecargaMediaA;
	unsigned int uiCountEvSobrecargaMediaB;
	unsigned int uiBuclesDiscA, uiBuclesDiscB;	

public:
	void SetIPPasarela(std::string ip);
	int GetEstadoRtsp(eRecorder nRec);						//ESC. 28.01.2106. A�adido par�metro "nRec"
	void SetEstadoRtsp(int valor, eRecorder nRec);			//ESC. 28.01.2106. A�adido par�metro "nRec"
	void Dispose();
	void SignalFinalizacion ();
	std::string GetTxtTipoOrden(int iOrden);				//ESC. 12.01.2016
	int GetTipoUA();										//ESC. 19.01.2016
	bool SesionGrabacionReady(eRecorder nRec);				//ESC. 04.02.2016
	static std::string GetTxtEstadoRtsp(/*eRtspStd*/int eStd);		//ESC. 12.01.2016

	std::string GetFrecID_GRS();							//10.02.2017
	void SetFrecID_GRS(const std::string &sFrecID);			//10.02.2017
	void SetSIP_Call_ID(std::string &sSIPcallID);			//10.02.2017
	unsigned int iNumFramesRxModoNoRecordA;
	unsigned int iNumFramesRxModoNoRecordB;

	RTSPOrder last_record_ordenA;							//Guarda la �ltima orden del tipo record
	RTSPOrder last_record_ordenB;							//Guarda la �ltima orden del tipo record
	void Set_last_record_ordenA(RTSPOrder orden);
	void Set_last_record_ordenB(RTSPOrder orden);
	RTSPOrder Get_last_record_ordenA();
	RTSPOrder Get_last_record_ordenB();

	static void keepalive_mtimerA_cb(void* param);
	static void keepalive_mtimerB_cb(void* param);

	std::string _uriSamplerRecA;			//URI de sampler/recurso en RecorderA

protected:
	UG5KRecordService *pRecordService;						//ESC. 27.01.2016. Puntero a este objeto "com�n"
	bool _bDualRec;											//ESC. 27.01.2016
	bool _bModoVOTER;										//ESC. 18.01.2017

	int _InterleavedA;										//valor del interlaved devuelto por el grabador
	int _InterleavedB;										//valor del interlaved devuelto por el grabador
	
	int _tipoUA;	

	int _SessionTimeoutA;									//En ED137C es el timeout de los Keepalives.
	int _SessionTimeoutB;									//En ED137C es el timeout de los Keepalives.
	Mtimer* keepalive_mtimerA;
	CCritSec m_lock_keepalive_mtimerA;
	bool _SendKeepAliveA;
	Mtimer* keepalive_mtimerB;
	CCritSec m_lock_keepalive_mtimerB;
	bool _SendKeepAliveB;

	CCritSec m_lock_ordenesA;
	std::queue<RTSPOrder> _ordenesA;
	CCritSec m_lock_ordenesB;
	std::queue<RTSPOrder> _ordenesB;

	CCritSec m_lock_EstRtsp;

	/** */
	std::list<std::pair<std::string, std::string> > _headersRecA, _headersRecB;

	/** */
	std::string _ipPasarela;
	std::string _uri_frmt;
	std::string _frecID_GRS;				//ESC. 10.02.2017

	CTCPSocket _sckRecA;
	RTSPConfig _cfgRecA;
	//std::string _uriSamplerRecA;			//URI de sampler/recurso en RecorderA
	int _eRtspRecA;
	int _cseqRecA;
	std::string _sessionRecA;

	CTCPSocket _sckRecB;
	RTSPConfig _cfgRecB;
	std::string _uriSamplerRecB;			//URI de sampler/recurso en RecorderB
	int _eRtspRecB;
	int _cseqRecB;
	std::string _sessionRecB;

	/** */
	//eRtspMediaTransport _transporte; 
	RTPSession _rtpA;						
	RTPSession _rtpB;
	void CreaNuevoSocket(eRecorder nRec);	//ESC. 14.01.2016

private:
	char _ED137VersionA;		//Puede tener el valor B o C
	char _ED137VersionB;		//Puede tener el valor B o C
	bool _bTimeoutRecA;
	bool _bTimeoutRecB;
	bool _finalizarSesion;

	unsigned int uiCountForCheckConexTCP_A;
	unsigned int uiCountForCheckConexTCP_B;
};

#endif
