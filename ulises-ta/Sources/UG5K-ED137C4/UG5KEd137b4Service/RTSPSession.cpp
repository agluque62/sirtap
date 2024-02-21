#include "stdafx.h"															//ESC. 01.12.2015. Añadido
#include "RTSPSession.h"
#include "RTSPMessages.h"
#include <string.h>

#include "Utilidades.h"
#include "NLOG.h"

#ifdef _WIN32
//#include <ws2tcpip.h>
#include "mstcpip.h"							//struct "tcp_keepalive"
#endif

#ifndef _WIN32
#include <pthread.h>
#endif

#define OP_RECORD						0
#define OP_PAUSE						1
#define OP_PARAM						2
#define OP_MEDIA						3
#define OP_TEARDOWN						4
#define OP_SIP_INVITE					5		//ESC. 26.07.2016. Msje compatibilidad VOTER
#define OP_SIP_BYE						6		//ESC. 18.01.2017. Msje compatibilidad VOTER
#define OP_INVALID						0xFF

#ifdef _WIN32
#define NUM_MAX_ORDENES_ENCOLADAS_RECURSO		1500 //100
#else
#define NUM_MAX_ORDENES_ENCOLADAS_RECURSO		100
#endif
#define NUM_MSJES_PRIO_SOBRECARGA_FILTRADOS		10
#define NUM_MSJES_MEDIA_SOBRECARGA_FILTRADOS	50		//ESC. Aprox. 1 'log' cada segundo de audio/recurso.

#ifdef _WIN32
#define STRICMP stricmp
#else
#define STRICMP strcasecmp
#endif

int mystoi(string s);
int mystoi(string s)
{
    int ret = 0;
    unsigned int i;

    char *sret = (char *) malloc(s.length()+1);
    if (sret == NULL) return 0;

    for (i = 0; i < s.length(); i++)
    {
        sret[i] = (char) s[i];
    }
    sret[i] = '\0';

    ret = (int) atoi(sret);
    free(sret);
    return ret;
}


/** */
RTSPSession::RTSPSession(void)
{
	_tipoUA = eUA_GRS;
	Init("recurso");
}

/** */
RTSPSession::RTSPSession(std::string recurso, int tipo/*= eUA_GRS*/, UG5KRecordService *pRecordService/*= NULL*/) 
{
	_tipoUA = tipo;

	if(pRecordService)
		_bDualRec= pRecordService->GetDualRecordCfg();
	else
		_bDualRec= false;

	Init(recurso);
}

/** */
RTSPSession::~RTSPSession(void)
{
}

void RTSPSession::Dispose()
{
	try
	{
		NLOG_DEBUG("RTSP Sesion (%s). Dispose()", _cfgRecA.recurso.c_str());		
		
		CCSLock* p_lock_keep_mtimer = new CCSLock(m_lock_keepalive_mtimerA);
		_SendKeepAliveA = false;
		if (keepalive_mtimerA != NULL)
		{
			keepalive_mtimerA->EndThread();
			keepalive_mtimerA->Stop();
			delete keepalive_mtimerA;
			keepalive_mtimerA = NULL;
		}
		delete p_lock_keep_mtimer;
		_SendKeepAliveA = false;

		p_lock_keep_mtimer = new CCSLock(m_lock_keepalive_mtimerB);
		_SendKeepAliveB = false;
		if (keepalive_mtimerB != NULL)
		{
			keepalive_mtimerB->EndThread();
			keepalive_mtimerB->Stop();
			delete keepalive_mtimerB;
			keepalive_mtimerB = NULL;
		}
		delete p_lock_keep_mtimer;
		_SendKeepAliveB = false;

		_uri_frmt.clear();

//		_cfgRecA.recurso.clear();				//18.07.2016. NO BORRA IDent del recurso
		_headersRecA.clear();		
		_sckRecA.Close();

		SetEstadoRtsp ( eRtspSckDisc, eRecA );

		if(_bDualRec)
		{
//			_cfgRecB.recurso.clear();			//18.07.2016. NO BORRA IDent del recurso
			_headersRecB.clear();		
			_sckRecB.Close();
		
			SetEstadoRtsp ( eRtspSckDisc, eRecB );
		}

		VaciarCola_Ordenes(eRecA);
		VaciarCola_Ordenes(eRecB);
	}
	catch (...)
	{
		NLOG_ERROR("RTSP Sesion (%s). Dispose() Excepcion.", _cfgRecA.recurso.c_str());
	}
}

void RTSPSession::SetIPPasarela(std::string ip)
{
	_ipPasarela = ip;
//	NLOG_DEBUG( "Set IP PASARELA=%s", _ipPasarela.c_str());
}

void RTSPSession::SetEstadoRtsp(int valor, eRecorder nRec)
{
	CCSLock _lock(m_lock_EstRtsp);

	switch(nRec)
	{
		case eRecA:
			_eRtspRecA = valor;
			//NLOG_DEBUG( "REC_A: Estado RTSP='%d'", valor);
		break;

		case eRecB:
			_eRtspRecB = valor;
			//NLOG_DEBUG( "REC_B: Estado RTSP='%d'", valor);
		break;

		case eRecAll: 
			//_eRtspRecA = valor;
			//_eRtspRecB = valor;
			//NLOG_DEBUG( "REC_A y REC_B: Estado RTSP='%d'", valor);
		default:
			NLOG_ERROR( "RTSP Sesion (%s). SetEstadoRtsp(). Recorder Inexistente='%d'", _cfgRecA.recurso.c_str(), nRec);
		break;
	}
}

int RTSPSession::GetEstadoRtsp(eRecorder nRec)
{
	CCSLock _lock(m_lock_EstRtsp);

	switch(nRec)
	{
		case eRecA:
			return _eRtspRecA;
		break;

		case eRecB:
			return _eRtspRecB;
		break;

		default:
			NLOG_ERROR( "RTSP Sesion (%s). GetEstadoRtsp(). Recorder Inexistente='%d'", _cfgRecA.recurso.c_str(), nRec);
			return _eRtspRecA;
		break;
	}

}


/** */
void RTSPSession::Init(std::string recurso)
{
	LocalConfig _FileCfg;
	_finalizarSesion = false;
	_SessionTimeoutA = 0;
	_SessionTimeoutB= 0;
	_InterleavedA = 0;
	_InterleavedB = 0;
	keepalive_mtimerA = NULL;
	_SendKeepAliveA = false;
	keepalive_mtimerB = NULL;
	_SendKeepAliveB = false;
	_ED137VersionA = 'B';
	_ED137VersionB = 'B';

	_ipPasarela = "0.0.0.0";
	_uri_frmt = _FileCfg.RTSPUri_frmt();		//ESC. 02.02.2016. Formato COMÚN para ambos grabadores (y para todos los recursos)

	struct timeval tv;
#ifdef _WIN32
	tv.tv_sec= 1000;			//ESC. 15.10.2018.
	tv.tv_usec = 0;
#else
//	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
    end_sRunB = false;
#endif

#ifdef _WIN32
	BOOL op = TRUE;
	//_sckRecA.SetSockOpt (IPPROTO_TCP, TCP_NODELAY, (char *)&op, sizeof(BOOL));
#else
		//int op = 1;
		//_sckRecA.SetSockOpt (SOL_TCP, TCP_NODELAY, (char *)&op, sizeof(int));
#endif
	_sckRecA.SetSockOpt (SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval));
	_sckRecA.SetSockOpt (SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(struct timeval));

#ifdef ACTIVA_KEEPALIVE_TCP
//ESC. 07.04.2017
	int optval= 1;				//Activa KeepAlive en TCP socket
	_sckRecA.SetSockOpt (SOL_SOCKET, SO_KEEPALIVE, /*(int *)*/&optval, sizeof(optval));

#ifdef _WIN32
	ConfiguraParamsTCP_KeepAlive_Windows(_sckRecA);
#else
	ConfiguraParamsTCP_KeepAlive_Linux(_sckRecA);
#endif
//ESC. 07.04.2017
#endif

	_cfgRecA.IP_Recorder   = _FileCfg.RTSPIp_RecA();								//ESC. 23.12.2015. Añadido.
	_cfgRecA.Port_Recorder = _FileCfg.RTSPPortA();							//ESC. 12.01.2016. Añadido.
	_cfgRecA.CIPA_Recorder = CIPAddress(_cfgRecA.IP_Recorder, _cfgRecA.Port_Recorder);		
	_cfgRecA.recurso = recurso;
	_cseqRecA = 1;	

	SetEstadoRtsp ( eRtspSckDisc, eRecA );
	GeneraRtspURI(eRecA);

	//_transporte = eTrpNone;

	_bDualRec= _FileCfg.GrabadorDual();									//ESC. 27.01.2016. Añadido.
	_bModoVOTER= _FileCfg.ModoVOTER();									//ESC. 18.01.2017

	if(_bDualRec)
	{
#ifdef _WIN32
		BOOL op = TRUE;
		//_sckRecB.SetSockOpt (IPPROTO_TCP, TCP_NODELAY, (char *)&op, sizeof(BOOL));
#else
		//int op = 1;
		//_sckRecB.SetSockOpt (SOL_TCP, TCP_NODELAY, (char *)&op, sizeof(int));
#endif
		_sckRecB.SetSockOpt (SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval));
		_sckRecB.SetSockOpt (SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(struct timeval));

#ifdef ACTIVA_KEEPALIVE_TCP
//ESC. 07.04.2017
	int optval= 1;				//Activa KeepAlive en TCP socket
	_sckRecB.SetSockOpt (SOL_SOCKET, SO_KEEPALIVE, /*(int *)*/&optval, sizeof(optval));

#ifdef _WIN32
	ConfiguraParamsTCP_KeepAlive_Windows(_sckRecB);
#else
	ConfiguraParamsTCP_KeepAlive_Linux(_sckRecB);
#endif
//ESC. 07.04.2017
#endif

		_cfgRecB.IP_Recorder   = _FileCfg.RTSPIp_RecB();				//ESC. Dirección IP RecB
		_cfgRecB.Port_Recorder = _FileCfg.RTSPPortB();					
		_cfgRecB.CIPA_Recorder = CIPAddress(_cfgRecB.IP_Recorder, _cfgRecB.Port_Recorder);		
		_cfgRecB.recurso = recurso;
		_cseqRecB = 1;

		SetEstadoRtsp ( eRtspSckDisc, eRecB );
		GeneraRtspURI( eRecB );
	}

	uiCountEvSobrecargaMediaA = NUM_MSJES_MEDIA_SOBRECARGA_FILTRADOS;		//ESC. 17.03.2016 Contador filtro avisos de sobrecarga de mensajes
	uiCountEvSobrecargaMediaB = NUM_MSJES_MEDIA_SOBRECARGA_FILTRADOS;		//ESC. 17.03.2016 Contador filtro avisos de sobrecarga de mensajes
	uiCountEvSobrecargaPrioA =  NUM_MSJES_PRIO_SOBRECARGA_FILTRADOS;
	uiCountEvSobrecargaPrioB = NUM_MSJES_PRIO_SOBRECARGA_FILTRADOS;
	iNumFramesRxModoNoRecordA= 0;
	iNumFramesRxModoNoRecordB = 0;

	uiCountForCheckConexTCP_A = 0;
	uiCountForCheckConexTCP_B = 0;

	last_record_ordenA.tipo = OP_INVALID;
	last_record_ordenB.tipo = OP_INVALID;
}

void RTSPSession::GeneraRtspURI(eRecorder nRec/*= eRecA*/)
{
/*
	//ESC. Formato URI empleado en Demo unit recorder de julio.2015 con Oivind
	//"rtsp://%s:554/%s/"
	_uri = Utilidades::StrFormat(_config.uri.c_str(), _ipPasarela.c_str(), _config.recurso.c_str());
*/

//ESC. 23.12.2015. Cambio formato URI para adaptarse a nueva versión del grabador de Ricochet
//	OJO!!!: El formateador de la URI no se carga del fichero de inicialización.
#ifdef RTSPURL_ALGIRDAS
	_uriSamplerRecA = Utilidades::StrFormat(/*"rtsp://%s:554/iprecorder/%s"*/_uri_frmt.c_str(), _cfgRecA.IP_Recorder.c_str(), _cfgRecA.Port_Recorder, _cfgRecA.recurso.c_str());
	if(_bDualRec)
	{
		_uriSamplerRecB = Utilidades::StrFormat(/*"rtsp://%s:554/iprecorder/%s"*/_uri_frmt.c_str(), _cfgRecB.IP_Recorder.c_str(), _cfgRecA.Port_Recorder, _cfgRecB.recurso.c_str());
	}

#else
	_uri = Utilidades::StrFormat(/*"rtsp://%s@%s:554/iprecorder"*/_config.uri.c_str(), _config.recurso.c_str(), _config.IP_grabador.c_str());	//ESC. 23.12.2015. ¿No sería más lógico? "ipPasarela.c_str()"
#endif
//--ESC

//	NLOG_DEBUG( "RtspURI RecA='%s'; RtspURI RecB='%s'; ", _uriSamplerRecA.c_str(), _uriSamplerRecB.c_str());
}

/** */
#ifdef _WIN32
#define SLEEP_RUN_AUTOMATAS						40							//milisegundos
#else
#define SLEEP_RUN_AUTOMATAS						80							//milisegundos
#endif

#define BUCLES_SLEEP_AUTOMATA_TCPDESCONECTADO		10000/SLEEP_RUN_AUTOMATAS
#define	BUCLES_SLEEP_AUTOMATA_ABRIENDOSESION_GW		2000/SLEEP_RUN_AUTOMATAS	//Temporización para pasarela
//MJ pongo menos tiempo #define	BUCLES_SLEEP_AUTOMATA_ABRIENDOSESION_GW		5000/SLEEP_RUN_AUTOMATAS	//Temporización para pasarela
#define	BUCLES_SLEEP_AUTOMATA_ABRIENDOSESION_CWP	2000/SLEEP_RUN_AUTOMATAS	//Temporización para P.O.



void RTSPSession::Run()
{
	if (_bDualRec)
	{
		//Arrancamos hilo para gestionar el grabador B
#ifdef _WIN32
		DWORD dwThreadId;
		m_hThreadB = CreateThread(NULL, 0, sRunB, (LPVOID)this, CREATE_SUSPENDED, &dwThreadId);
		if (m_hThreadB)
		{
			SetThreadPriority(m_hThreadB, THREAD_PRIORITY_NORMAL);
			ResumeThread(m_hThreadB);
		}
		else
		{
			NLOG_ERROR("HILO para gestion de grabador B no ha podido ser creado. Sesion (%s)", _cfgRecA.recurso.c_str());
		}
#else
        end_sRunB = false;
		assert(pthread_create(&m_hThreadB, NULL, sRunB, this) == 0);
#endif
	}

	NLOG_INFO("RTSP Sesion (%s). Thread A '%d' creado...", _cfgRecA.recurso.c_str(), GetId());
	sleep(100);

	uiBuclesDiscA = BUCLES_SLEEP_AUTOMATA_TCPDESCONECTADO;

#ifdef _WIN32
	const unsigned int uiBuclesToSes= BUCLES_SLEEP_AUTOMATA_ABRIENDOSESION_CWP;
#else
	const unsigned int uiBuclesToSes= BUCLES_SLEEP_AUTOMATA_ABRIENDOSESION_GW;
#endif
	unsigned int uiBuclesSessA = uiBuclesToSes;

	while ( IsRunning() && !_finalizarSesion )
	{
		sleep(SLEEP_RUN_AUTOMATAS);

		try
		{	
			switch (GetEstadoRtsp(eRecA))
			{
#ifdef TEARDOWN_NO_CONECTA_TCP
				case eRtspTearDown:				//ESC. 15.11.2016. VOTER: En estado TearDown no intenta la conexión TCP
					break;						//
#else
				case eRtspTearDown:				//ESC. Estado "teórico", transitorio a Desconectado
#endif
				case eRtspSckDisc:
				{
					uiBuclesDiscA++;
					if(uiBuclesDiscA >= BUCLES_SLEEP_AUTOMATA_TCPDESCONECTADO)
					{
						AutomataConectandoTCP(eRecA);
						uiBuclesDiscA= 0;
					}
				}break;

				case eRtspCnxSck:				//ESC. 04.02.2016. Estados "transitorios", gestionados por "AutomataAbriendoSesionRTSP()"
				{
					if (_bModoVOTER)							//25.01.2017. En modo VOTER esperaría a cmd 'INV' para establecer sesión RTSP (ANN+SETUP)
					{
						AutomataEsperaCmdUA(eRecA);
					}
					else
					{
						uiBuclesSessA++;
						if(uiBuclesSessA >= uiBuclesToSes)
						{
							AutomataAbriendoSesionRTSP(eRecA);	//25.01.2017. En modo ULISES establece automáticamente sesión RTSP (ANN+SETUP)
							uiBuclesSessA= 0;
						}
					}
				}break;

				case eRtspAnnc:					//ESC. Excepcionalmente llega aquí, porque "AutomataAbriendoSesionRTSP()" ejecuta ANN+SETUP
				{
					uiBuclesSessA++;
					if(uiBuclesSessA >= uiBuclesToSes)
					{
						AutomataAbriendoSesionRTSP(eRecA);
						uiBuclesSessA= 0;
					}
				}break;

				case eRtspSetup:
				case eRtspGrabando:
				{
//					NLOG_DEBUG("RTSP Sesion (%s). AutomataGestionOrdenes desde REC_A", _cfgRecA.recurso.c_str());
					AutomataGestionOrdenes(eRecA);
				}break;
			}
		}
		catch(socket_error err)
		{
			NLOG_ERROR("ED-137. RTSP Sesion (%s). SOCKET-ERROR: '%s'", _cfgRecA.recurso.c_str(), err.what());
		}
		catch (...)
		{
			NLOG_ERROR("ED-137. RTSP Sesion (%s). Excepcion No Controlada", _cfgRecA.recurso.c_str());
		}
	}

	if (_bDualRec)
	{
		//Esperamos el final del hilo del otro grabador
#ifdef _WIN32
		if (m_hThreadB) WaitForSingleObject(m_hThreadB, 5000);
#else
        int tries = 20;
        while (!end_sRunB && tries > 0)
        {
            sleep(250);
            tries--;
        }
#endif
	}

	NLOG_INFO("RTSP Sesion/Thread (%s). FINALIZADA...", _cfgRecA.recurso.c_str());
}

#ifdef _WIN32
DWORD WINAPI RTSPSession::sRunB(LPVOID pParam)
#else
void* RTSPSession::sRunB(void* pParam)
#endif
{
	RTSPSession* rtspSesB = (RTSPSession*)pParam;

	NLOG_INFO("RTSP Sesion (%s). Thread B '%d' creado...", rtspSesB->_cfgRecB.recurso.c_str(), rtspSesB->GetId());

	sleep(100);

	rtspSesB->uiBuclesDiscB = BUCLES_SLEEP_AUTOMATA_TCPDESCONECTADO;

#ifdef _WIN32
	const unsigned int uiBuclesToSes = BUCLES_SLEEP_AUTOMATA_ABRIENDOSESION_CWP;
#else
	const unsigned int uiBuclesToSes = BUCLES_SLEEP_AUTOMATA_ABRIENDOSESION_GW;
#endif
	unsigned int uiBuclesSessB = uiBuclesToSes;

	while (rtspSesB->IsRunning() && !rtspSesB->_finalizarSesion)
	{
		sleep(SLEEP_RUN_AUTOMATAS);

		try
		{
			switch (rtspSesB->GetEstadoRtsp(eRecB))
			{
#ifdef TEARDOWN_NO_CONECTA_TCP
			case eRtspTearDown:				//ESC. 15.11.2016. VOTER: En estado TearDown no intenta la conexión TCP
				break;						//
#else
			case eRtspTearDown:				//ESC. Estado "teórico", transitorio a Desconectado
#endif
			case eRtspSckDisc:
			{
				rtspSesB->uiBuclesDiscB++;
				if (rtspSesB->uiBuclesDiscB >= BUCLES_SLEEP_AUTOMATA_TCPDESCONECTADO)
				{
					rtspSesB->AutomataConectandoTCP(eRecB);
					rtspSesB->uiBuclesDiscB = 0;
				}
			}break;


			case eRtspCnxSck:			//ESC. 04.02.2016. Estados "transitorios", gestionados por "AutomataAbriendoSesionRTSP()"
			{
				if (rtspSesB->_bModoVOTER)						//25.01.2017. En modo VOTER esperaría a cmd 'INV' para establecer sesión RTSP (ANN+SETUP)
					rtspSesB->AutomataEsperaCmdUA(eRecB);
				else
				{
					uiBuclesSessB++;
					if (uiBuclesSessB >= uiBuclesToSes)
					{
						NLOG_INFO("RTSP Sesion (%s). AutomataAbriendoSesionRTSP para REC_B", rtspSesB->_cfgRecB.recurso.c_str());
						rtspSesB->AutomataAbriendoSesionRTSP(eRecB);	//25.01.2017. En modo ULISES establece automáticamente sesión RTSP (ANN+SETUP)
						uiBuclesSessB = 0;
					}
				}
			}break;

			case eRtspAnnc:				//ESC. Excepcionalmente llega aquí, porque "AutomataAbriendoSesionRTSP()" ejecuta ANN+SETUP
			{
				uiBuclesSessB++;
				if (uiBuclesSessB >= uiBuclesToSes)
				{
					NLOG_INFO("RTSP Sesion (%s). AutomataAbriendoSesionRTSP para REC_B", rtspSesB->_cfgRecB.recurso.c_str());
					rtspSesB->AutomataAbriendoSesionRTSP(eRecB);
					uiBuclesSessB = 0;
				}
			}break;

			case eRtspSetup:
			case eRtspGrabando:
			{
				rtspSesB->AutomataGestionOrdenes(eRecB);
			}break;
			}
		}
		catch (socket_error err)
		{
			NLOG_ERROR("RTSP Sesion (%s)REC_B. Socket-ERROR: %s", rtspSesB->_cfgRecB.recurso.c_str(), err.what());
		}
		catch (...)
		{
			NLOG_ERROR("RTSP Sesion (%s)REC_B. Excepcion No Controlada", rtspSesB->_cfgRecB.recurso.c_str());
		}
	}

	NLOG_INFO("RTSP Sesion/Thread (%s). FINALIZADA...", rtspSesB->_cfgRecB.recurso.c_str());
#ifndef _WIN32
    rtspSesB->end_sRunB = true;
#endif
	return 0;
}


/** */
bool RTSPSession::AutomataConectandoTCP(eRecorder nRec)
{
/*
	//ESC. 14.01.2016. Comentado.
	//CCSLock _lock(m_lock_ordenes);
*/
	RTSPConfig& cfgRec= (nRec== eRecB) ? _cfgRecB : _cfgRecA;
	CTCPSocket& sckRec= (nRec== eRecB) ? _sckRecB : _sckRecA;

	int estado = GetEstadoRtsp(nRec);
	NLOG_INFO("AutomataConectandoTCP(%s). estado %i con %s:%u", cfgRec.recurso.c_str(), estado, cfgRec.CIPA_Recorder.GetHostName().c_str(), cfgRec.CIPA_Recorder.GetPort() );
	if ( estado == eRtspSckDisc )
	{
		try
		{
			SetSocketBlockingModeON(sckRec.GetHandle(), false);
			NLOG_INFO("RTSP Sesion (%s). Intentando conectar con %s:%u", cfgRec.recurso.c_str(), cfgRec.CIPA_Recorder.GetHostName().c_str(), cfgRec.CIPA_Recorder.GetPort());
#ifdef _WIN32
			if (sckRec.SetDSCPQoS(34, cfgRec.CIPA_Recorder) == -1)
			{
				LPVOID lpMsgBuf;
				int e;

				lpMsgBuf = (LPVOID)"Unknown error";
				e = WSAGetLastError();
				if (FormatMessage(
					FORMAT_MESSAGE_ALLOCATE_BUFFER |
					FORMAT_MESSAGE_FROM_SYSTEM |
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, e,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					// Default language
					(LPTSTR)&lpMsgBuf, 0, NULL)) {
					NLOG_ERROR("No se puede establecer DSCP. Error %s", (char*)lpMsgBuf);
					LocalFree(lpMsgBuf);
				}
				else
					NLOG_ERROR("No se puede establecer DSCP error %d", e);
			}
#else
			Configura_QoS_Linux(sckRec);
#endif

			if (sckRec.Connect(cfgRec.CIPA_Recorder) == true)
			{
				VaciarCola_Ordenes(nRec);
				SetEstadoRtsp(eRtspCnxSck, nRec);
				NLOG_INFO("RTSP Sesion (%s). Connect OK con %s:%u", cfgRec.recurso.c_str(), cfgRec.CIPA_Recorder.GetHostName().c_str(), cfgRec.CIPA_Recorder.GetPort() );

				SetSocketBlockingModeON(sckRec.GetHandle(), true);
			}
			else
			{
//ESC. 19.02.2016. NO_BLOQUEANTE
#ifdef _WIN32
				sleep(250);		//07.06.2017. Espera/conmuta 250 mseg, y después comprueba, bloqueando 50 mseg, si el socket es writable

				if( !sckRec.IsWritable(50))	

#else
/*
If you try to call connect() in non-blocking mode, and the API can't connect instantly,
it will return the error code for 'Operation In Progress'.
When you call connect() again, later, it may tell you 'Operation Already In Progress'
to let you know that it's still trying to connect, or it may give you a successful return code,
telling you that the connect has been made.
*/
				bool bSckConnected=false;
/*
				unsigned int iNumIntentosConnect=0;
				while (iNumIntentosConnect < 2)	//(5)intentos+sleep(100) da problemas en VOTER
				{
					if (sckRec.Connect(cfgRec.CIPA_Recorder) == true)
					{
						bSckConnected= true;
						break;
					}
					sleep(400);					//

					iNumIntentosConnect++;
				}
*/
				sleep(50);				//21.04.2017. Añade sleep para "separar" los dos Connect

				if (sckRec.Connect(cfgRec.CIPA_Recorder) == true)
				{
					bSckConnected= true;
				}
				else
				{
					sleep(150);			//Espera para comprobar si un nuevo intento de Connect tiene éxito

					if (sckRec.Connect(cfgRec.CIPA_Recorder) == true)
						bSckConnected= true;

				}

				if( !bSckConnected)
#endif
				{
					NLOG_ERROR("RTSP Sesion (%s). Connect error > %s:%u", cfgRec.recurso.c_str(), cfgRec.CIPA_Recorder.GetHostName().c_str(), cfgRec.CIPA_Recorder.GetPort() );
					return false;		//OJO: el socket sigue en modo NO Blocking
				}
				else
				{	//Pasa el socket a modo Blocking.
					VaciarCola_Ordenes(nRec);
					SetEstadoRtsp(eRtspCnxSck, nRec);
					NLOG_INFO("RTSP Sesion (%s). Sck Writable con %s:%u", cfgRec.recurso.c_str(), cfgRec.CIPA_Recorder.GetHostName().c_str(), cfgRec.CIPA_Recorder.GetPort() );

					SetSocketBlockingModeON(sckRec.GetHandle(), true);
				}
//-- NO_BLOQUEANTE
			}

			sleep(20);
			return true;
		}
		catch(socket_error error)
		{
			NLOG_ERROR("Excepcion socket_error. Connect: (%s). %s, %s", error.what(), cfgRec.recurso.c_str(), cfgRec.CIPA_Recorder.GetHostName().c_str() );
			return false;
		}
	}

	return false;
}

/** */
bool RTSPSession::EjecutaAnnounce(eRecorder nRec/*= eRecA*/)
{
/*
	//ESC. 14.01.2016. Comentado.
	//CCSLock _lock(m_lock_ordenes);
*/
	bool res = false;
	int estado;

	estado = GetEstadoRtsp(nRec);
	if (estado == eRtspCnxSck || estado == eRtspAnnc )
	{
		res = MetodoAnnounce(nRec);
	}
	else
	{
		RTSPConfig& cfgRec= (nRec== eRecB) ? _cfgRecB : _cfgRecA;
		NLOG_ERROR("RTSP Sesion (%s). EjecutaAnnounce(), estado RTSP=%s.", cfgRec.recurso.c_str(), GetTxtEstadoRtsp(estado).c_str());
	}

	return ( res );
}

/** */
bool RTSPSession::EjecutaSetup(eRecorder nRec/*= eRecA*/)
{
	bool bRes = false;

	RTSPConfig& cfgRec= (nRec == eRecB) ? _cfgRecB : _cfgRecA;

	GeneraRtspURI(nRec);

//	NLOG_DEBUG("RTSP Sesion (%s). Recibido SETUP", cfgRec.recurso.c_str());
	int estado = GetEstadoRtsp(nRec);
	if (estado == eRtspAnnc)
	{
//		NLOG_DEBUG("RTSP Sesion (%s). Enviando SETUP...", cfgRec.recurso.c_str());
		bRes = MetodoSetup(nRec);
	}
	else
	{
		NLOG_ERROR("RTSP Sesion (%s). metodo==SETUP, estado=%d.", cfgRec.recurso.c_str(), estado);
	}

	if ( ! bRes)
	{
		SetEstadoRtsp(eRtspCnxSck, nRec);
	}

	return ( bRes );
}

/** */
RTSPOrder RTSPSession::PushmsjeRecord_onlyOper(rtsp_props operaciones, string connref)
{
	RTSPOrder ordenA;
	ordenA.connref = connref;
	ordenA.tipo = OP_RECORD;
	ordenA.operaciones = operaciones;
	ordenA.pMedia = NULL;
	ordenA.tamMedia = 0;

	PushPriority_Orden(ordenA, eRecA);
	if (_bDualRec)
	{
		RTSPOrder ordenB;
		ordenB.connref = connref;
		ordenB.tipo = OP_RECORD;
		ordenB.operaciones = operaciones;
		ordenB.pMedia = NULL;
		ordenB.tamMedia = 0;

		PushPriority_Orden(ordenB, eRecB);
	}
	return ordenA;
}

/** */
RTSPOrder RTSPSession::PushmsjeRecord_onlyOper(rtsp_props operaciones, string connref, eRecorder nRec)
{
	RTSPOrder orden;
	orden.connref = connref;
	orden.tipo = OP_RECORD;
	orden.operaciones = operaciones;
	orden.pMedia = NULL;
	orden.tamMedia = 0;

	PushPriority_Orden(orden, nRec);
	return orden;
}

/** */
void RTSPSession::PushmsjeRecord_onlyProp(rtsp_props propiedades, string connref)
{
	RTSPOrder ordenA;
	ordenA.connref = connref;
	ordenA.tipo = OP_RECORD;
	ordenA.propiedades = propiedades;
	ordenA.pMedia = NULL;
	ordenA.tamMedia = 0;

	PushPriority_Orden(ordenA, eRecA);
	if (_bDualRec)
	{
		RTSPOrder ordenB;
		ordenB.connref = connref;
		ordenB.tipo = OP_RECORD;
		ordenB.propiedades = propiedades;
		ordenB.pMedia = NULL;
		ordenB.tamMedia = 0;
		PushPriority_Orden(ordenB, eRecB);
	}
}

// ESC. 03.06.2016. Método "PushmsjeRecord" con parámetros propiedades y operaciones.
/** */
RTSPOrder RTSPSession::PushmsjeRecord(rtsp_props propiedades, rtsp_props operaciones, string connref)
{
	RTSPOrder ordenA;
	ordenA.connref = connref;
	ordenA.tipo = OP_RECORD;
	ordenA.propiedades = propiedades;		//ESC. 03.06.2016. Añadido
	ordenA.operaciones = operaciones;
	ordenA.pMedia = NULL;
	ordenA.tamMedia = 0;

	PushPriority_Orden(ordenA, eRecA);
	if (_bDualRec)
	{
		RTSPOrder ordenB;
		ordenB.connref = connref;
		ordenB.tipo = OP_RECORD;
		ordenB.propiedades = propiedades;		//ESC. 03.06.2016. Añadido
		ordenB.operaciones = operaciones;
		ordenB.pMedia = NULL;
		ordenB.tamMedia = 0;
		PushPriority_Orden(ordenB, eRecB);
	}
	return (ordenA);
}

RTSPOrder RTSPSession::PushmsjeRecord(rtsp_props propiedades, rtsp_props operaciones, string connref, eRecorder nRec)
{
	RTSPOrder orden;
	orden.connref = connref;
	orden.tipo = OP_RECORD;
	orden.propiedades = propiedades;		//ESC. 03.06.2016. Añadido
	orden.operaciones = operaciones;
	orden.pMedia = NULL;
	orden.tamMedia = 0;

	PushPriority_Orden(orden, nRec);	
	return (orden);
}


/** */
RTSPOrder RTSPSession::PushmsjePause_onlyOper(rtsp_props operaciones, string connref)
{
	RTSPOrder ordenA;
	ordenA.connref = connref;
	ordenA.tipo = OP_PAUSE;
	ordenA.operaciones = operaciones;
	ordenA.pMedia = NULL;
	ordenA.tamMedia = 0;

	PushPriority_Orden(ordenA, eRecA);
	if (_bDualRec)
	{
		RTSPOrder ordenB;
		ordenB.connref = connref;
		ordenB.tipo = OP_PAUSE;
		ordenB.operaciones = operaciones;
		ordenB.pMedia = NULL;
		ordenB.tamMedia = 0;
		PushPriority_Orden(ordenB, eRecB);
	}
	return (ordenA);
}

//ESC. 19.01.2017. "PushmsjePause" con parámetros propiedades y operaciones.
/** */
RTSPOrder RTSPSession::PushmsjePause(rtsp_props propiedades, rtsp_props operaciones, string connref)
{
	RTSPOrder ordenA;
	ordenA.connref = connref;
	ordenA.tipo = OP_PAUSE;
	ordenA.propiedades = propiedades;		//ESC. 19.01.2017. Añadido
	ordenA.operaciones = operaciones;
	ordenA.pMedia = NULL;
	ordenA.tamMedia = 0;

	PushPriority_Orden(ordenA, eRecA);
	if (_bDualRec)
	{
		RTSPOrder ordenB;
		ordenB.connref = connref;
		ordenB.tipo = OP_PAUSE;
		ordenB.propiedades = propiedades;		//ESC. 19.01.2017. Añadido
		ordenB.operaciones = operaciones;
		ordenB.pMedia = NULL;
		ordenB.tamMedia = 0;
		PushPriority_Orden(ordenB, eRecB);
	}
	return (ordenA);
}

/** */
void RTSPSession::PushmsjeTearDown()
{
	RTSPOrder ordenA;
	ordenA.connref.clear();
	ordenA.tipo = OP_TEARDOWN;
	ordenA.pMedia = NULL;
	ordenA.tamMedia = 0;

	PushPriority_Orden(ordenA, eRecA);
	if (_bDualRec)
	{
		RTSPOrder ordenB;
		ordenB.connref.clear();
		ordenB.tipo = OP_TEARDOWN;
		ordenB.pMedia = NULL;
		ordenB.tamMedia = 0;
		PushPriority_Orden(ordenB, eRecB);
	}
}

/** */
void RTSPSession::PushmsjeSIP_BYE()
{
	RTSPOrder ordenA;
	ordenA.connref.clear();
	ordenA.tipo = OP_SIP_BYE;
	ordenA.pMedia = NULL;
	ordenA.tamMedia = 0;

	PushPriority_Orden(ordenA, eRecA);
	if (_bDualRec)
	{
		RTSPOrder ordenB;
		ordenB.connref.clear();
		ordenB.tipo = OP_SIP_BYE;
		ordenB.pMedia = NULL;
		ordenB.tamMedia = 0;
		PushPriority_Orden(ordenB, eRecB);
	}
}

/** */
void RTSPSession::PushmsjeParam_onlyOper(rtsp_props operaciones, string connref)	//GRS y VCS-Radio
{
	RTSPOrder ordenA;
	ordenA.connref = connref;
	ordenA.tipo = OP_PARAM;
	ordenA.operaciones = operaciones;		//ESC. 19.01.2016. La lista "orden.propiedades" está vacía
	ordenA.pMedia = NULL;
	ordenA.tamMedia = 0;

	CheckSizeBeforePush_Orden(ordenA, eRecA);
	if (_bDualRec)
	{
		RTSPOrder ordenB;
		ordenB.connref = connref;
		ordenB.tipo = OP_PARAM;
		ordenB.operaciones = operaciones;		//ESC. 19.01.2016. La lista "orden.propiedades" está vacía
		ordenB.pMedia = NULL;
		ordenB.tamMedia = 0;
		CheckSizeBeforePush_Orden(ordenB, eRecB);
	}
}

/** */
void RTSPSession::PushmsjeParam_onlyProp(rtsp_props propiedades, string connref)	//26.01.2017. VCS-Telephony
{
	RTSPOrder ordenA;
	ordenA.tipo = OP_PARAM;
	ordenA.propiedades = propiedades;		//ESC. 26.01.2017. La lista "orden.operaciones" está vacía
	ordenA.pMedia = NULL;
	ordenA.tamMedia = 0;
	ordenA.connref = connref;

	CheckSizeBeforePush_Orden(ordenA, eRecA);
	if (_bDualRec)
	{
		RTSPOrder ordenB;
		ordenB.tipo = OP_PARAM;
		ordenB.propiedades = propiedades;		//ESC. 26.01.2017. La lista "orden.operaciones" está vacía
		ordenB.pMedia = NULL;
		ordenB.tamMedia = 0;
		ordenB.connref = connref;
		CheckSizeBeforePush_Orden(ordenB, eRecB);
	}
}


// ESC. 19.01.2016. Método "PushmsjeParam" con parámetros propiedades y operaciones.
/** */
void RTSPSession::PushmsjeParam(rtsp_props propiedades, rtsp_props operaciones, string connref)
{
	RTSPOrder ordenA;
	ordenA.connref = connref;
	ordenA.tipo = OP_PARAM;
	ordenA.propiedades = propiedades;		//ESC. 19.01.2016. Añadido
	ordenA.operaciones = operaciones;
	ordenA.pMedia = NULL;
	ordenA.tamMedia = 0;

	CheckSizeBeforePush_Orden(ordenA, eRecA);
	if (_bDualRec)
	{
		RTSPOrder ordenB;
		ordenB.connref = connref;
		ordenB.tipo = OP_PARAM;
		ordenB.propiedades = propiedades;		//ESC. 19.01.2016. Añadido
		ordenB.operaciones = operaciones;
		ordenB.pMedia = NULL;
		ordenB.tamMedia = 0;
		CheckSizeBeforePush_Orden(ordenB, eRecB);
	}
}

/** */
bool RTSPSession::PushframeMedia(std::string paquete, int tam, void * pMedia)
{
	bool ret = true;

	RTSPOrder ordenA;
	ordenA.connref.clear();
	ordenA.tipo = OP_MEDIA;
	ordenA.pMedia = NULL;
	ordenA.tamMedia = 0;

	RTSPOrder ordenB;
	ordenB.connref.clear();
	ordenB.tipo = OP_MEDIA;
	ordenB.pMedia = NULL;
	ordenB.tamMedia = 0;

	if ( pMedia != NULL )
	{
		ordenA.pMedia = CreateEmbeddedMedia((char *) pMedia, tam, eRecA, ordenA.tamMedia);
		CheckSizeBeforePush_Orden(ordenA, eRecA);
		if (_bDualRec)
		{
			ordenB.pMedia = CreateEmbeddedMedia((char*)pMedia, tam, eRecB, ordenB.tamMedia);
			CheckSizeBeforePush_Orden(ordenB, eRecB);
		}
	}

	//OJO. En caso de que el mensaje no tenga Media sería un ERROR 

#define NUM_MAX_FRAMES_MEDIA_CONSECUTIVOS_MODO_NO_RECORD	100
#define AUTO_RECORD_WITH_MEDIA	1

	if (GetEstadoRtsp(eRecA)==eRtspGrabando)
	{
		iNumFramesRxModoNoRecordA= 0;
	}
	else
	{
		//Si alguno de los dos grabadores está en Setup y tengo media vuelvo a mandar record.
		iNumFramesRxModoNoRecordA++;
		if (iNumFramesRxModoNoRecordA >= NUM_MAX_FRAMES_MEDIA_CONSECUTIVOS_MODO_NO_RECORD)
		{
#ifdef AUTO_RECORD_WITH_MEDIA

			//21.07.2016. Mecanismo para intentar grabar audio, aunque no estuviera la sesión en Record
			if (GetEstadoRtsp(eRecA) == eRtspSetup)
			{
				//SOLO Inserta mensaje de RECORD si el modo de la Sesión es SETUP
				RTSPOrder last_record_orden = Get_last_record_ordenA();
				if (last_record_orden.tipo == OP_RECORD)
				{
#ifdef _WIN32
					PushmsjeRecord_onlyOper(last_record_orden.operaciones, last_record_orden.connref, eRecA);
#else
					PushmsjeRecord(last_record_orden.propiedades, last_record_orden.operaciones, last_record_orden.connref, eRecA);
#endif
					NLOG_INFO("RTSP Sesion (%s) REC_A. RX '%d' frames MEDIA estado NO_RECORD=> Auto-RECORD", _cfgRecA.recurso.c_str(), iNumFramesRxModoNoRecordA);
				}
			}

#else
			NLOG_ERROR("RTSP Sesion (%s). RX '%d' frames MEDIA estado NO_RECORD", _cfgRecA.recurso.c_str(), iNumFramesRxModoNoRecord);
#endif
			iNumFramesRxModoNoRecordA = 0;
			ret = false;
		}
	}

	if (_bDualRec)
	{
		if (GetEstadoRtsp(eRecB) == eRtspGrabando)
		{
			iNumFramesRxModoNoRecordB = 0;
		}
		else
		{
			//Si alguno de los dos grabadores está en Setup y tengo media vuelvo a mandar record.
			iNumFramesRxModoNoRecordB++;
			if (iNumFramesRxModoNoRecordB >= NUM_MAX_FRAMES_MEDIA_CONSECUTIVOS_MODO_NO_RECORD)
			{
#ifdef AUTO_RECORD_WITH_MEDIA

				//21.07.2016. Mecanismo para intentar grabar audio, aunque no estuviera la sesión en Record
				if (GetEstadoRtsp(eRecB) == eRtspSetup)
				{
					//SOLO Inserta mensaje de RECORD si el modo de la Sesión es SETUP
					RTSPOrder last_record_orden = Get_last_record_ordenB();
					if (last_record_orden.tipo == OP_RECORD)
					{
#ifdef _WIN32
						PushmsjeRecord_onlyOper(last_record_orden.operaciones, last_record_orden.connref, eRecB);
#else
						PushmsjeRecord(last_record_orden.propiedades, last_record_orden.operaciones, last_record_orden.connref, eRecB);
#endif
						NLOG_INFO("RTSP Sesion (%s) REC_B. RX '%d' frames MEDIA estado NO_RECORD=> Auto-RECORD", _cfgRecB.recurso.c_str(), iNumFramesRxModoNoRecordB);
					}
				}
#else
				NLOG_ERROR("RTSP Sesion (%s). RX '%d' frames MEDIA estado NO_RECORD", _cfgRecA.recurso.c_str(), iNumFramesRxModoNoRecord);
#endif
				iNumFramesRxModoNoRecordB = 0;
				ret = false;
			}
		}
	}

	return ret;
}

/** */
void RTSPSession::PushmsjeSIP_INVITE()
{
	RTSPOrder ordenA;
	ordenA.connref.clear();
	ordenA.tipo = OP_SIP_INVITE;
	ordenA.pMedia = NULL;
	ordenA.tamMedia = 0;

	PushPriority_Orden(ordenA, eRecA);
	if (_bDualRec)
	{
		RTSPOrder ordenB;
		ordenB.connref.clear();
		ordenB.tipo = OP_SIP_INVITE;
		ordenB.pMedia = NULL;
		ordenB.tamMedia = 0;
		PushPriority_Orden(ordenB, eRecB);
	}

	uiBuclesDiscA = BUCLES_SLEEP_AUTOMATA_TCPDESCONECTADO;					//Fuerza establecimiento socket TCP.
	if (_bDualRec) uiBuclesDiscB = BUCLES_SLEEP_AUTOMATA_TCPDESCONECTADO;	//Fuerza establecimiento socket TCP.
}


/** */
void RTSPSession::AutomataAbriendoSesionRTSP(eRecorder nRec/*= eRecA*/)
{
	if (_finalizarSesion) 
	{
		return;
	}

//	CCSLock _lock(m_lock_ordenes);		// Este método NO accede a la cola "_ordenes"
	int estado = GetEstadoRtsp(nRec);

	switch(estado)
	{
		case eRtspCnxSck:
//		case eRtspTearDown:
		{
			if(EjecutaAnnounce(nRec))
				EjecutaSetup(nRec);
		}
		break;

		case eRtspAnnc:
		{
			EjecutaSetup(nRec);
		}break;

		default:
		{
			NLOG_ERROR("RTSP Sesion (%s). AutomataAbriendoSesionRTSP() en estado '%s' ", _cfgRecA.recurso.c_str(), GetTxtEstadoRtsp(estado).c_str());
		}break;
	}

}

/** */
#define NUM_MAX_ORDENES_SLOT	30				//ESC. Número máximo de órdenes "procesadas" al invocar este método
void RTSPSession::AutomataGestionOrdenes(eRecorder nRec)
{
	bool bSesReadyRec_In= SesionGrabacionReady(nRec);
	CCritSec& m_lock_ordenes = (nRec == eRecA)? m_lock_ordenesA : m_lock_ordenesB;	
	RTSPConfig& cfgRec = (nRec == eRecA) ? _cfgRecA : _cfgRecB;
	int iOrdenesProc = 0;

	//std::queue<RTSPOrder>& _ordenes_ = (nRec == eRecA) ? _ordenesA : _ordenesB;
	//NLOG_INFO("TRAZA: RTSP Sesion (%s) %s (%s) ordenes %d",
	//	cfgRec.recurso.c_str(), (nRec == eRecA) ? "REC_A" : "REC_B", cfgRec.CIPA_Recorder.GetHostName().c_str(), _ordenes_.size());

	while (1)
	{			
		CCSLock* p_lock = new CCSLock(m_lock_ordenes);
		std::queue<RTSPOrder>& _ordenes = (nRec == eRecA) ? _ordenesA : _ordenesB;		
		if (_ordenes.empty())
		{
			delete p_lock;
			break;
		}

		if (iOrdenesProc >= NUM_MAX_ORDENES_SLOT)
		{
			delete p_lock;
			break;								//ESC: 27.01.2016 Cuando se han procesado "NUM_MAX_ORDENES_SLOT", sale de esta función
		}

		iOrdenesProc++;

		/** "Lee" la orden más antigua almacenada en la lista */
		RTSPOrder orden_aux = _ordenes.front();		

		if (orden_aux.tipo == OP_MEDIA)			//ESC. Comentado "&& staRtsp == eRtspGrabando)"
		{
//			NLOG_DEBUG("ED-137. RTSP Sesion (%s). Enviando MEDIA...", _cfgRecA.recurso.c_str());
			// En caso de datos de audio siempre se elimina aunque falle la transmisión.
			if (bSesReadyRec_In)
			{
				bool error_envio = false;
				if (orden_aux.pMedia != NULL)
				{
					unsigned char* pMedia = (unsigned char*) orden_aux.pMedia;
					int tamMedia = orden_aux.tamMedia;	
					orden_aux.pMedia = NULL;
					orden_aux.tamMedia = 0;
					_ordenes.pop();
					delete p_lock;
					if (SendEmbeddedMedia(pMedia, tamMedia, nRec) == false)
					{
#ifdef _WIN32
						NLOG_ERROR("RTSP Sesion (%s). Error .Send_Media(%s). WSAErr(%d). CreaNuevoSocket", cfgRec.recurso.c_str(), cfgRec.CIPA_Recorder.GetHostName().c_str(), WSAGetLastError());	//Antes "sckRec.GetHostAddress().GetStringAddress().c_str()"
#else
						NLOG_ERROR("RTSP Sesion (%s). Error .Send_Media(%s). errno(%d). CreaNuevoSocket", cfgRec.recurso.c_str(), (nRec == eRecA) ? "REC_A" : "REC_B", errno);	//ESC. provoca excepción "sckRec.GetHostAddress().GetStringAddress().c_str()"
#endif
						error_envio = true;						
					}					
					free(pMedia);
				}
				else
				{
					_ordenes.pop();
					delete p_lock;
				}
				if (error_envio) CreaNuevoSocket(nRec);
			}
			else
			{
				if (orden_aux.pMedia != NULL)
				{
					free(orden_aux.pMedia);
					orden_aux.pMedia = NULL;
					orden_aux.tamMedia = 0;
				}
				_ordenes.pop();
				delete p_lock;
			}
		}
		else
		{
			//Clonamos el objeto para poderlo quitar de la lista con pop
			RTSPOrder orden;
			orden.connref = orden_aux.connref;
			orden.operaciones.assign(orden_aux.operaciones.begin(), orden_aux.operaciones.end());
			orden.propiedades.assign(orden_aux.propiedades.begin(), orden_aux.propiedades.end());
			if (orden_aux.pMedia != NULL && orden_aux.tamMedia > 0)
			{
				orden.pMedia = malloc(orden_aux.tamMedia);
				memcpy(orden.pMedia, orden_aux.pMedia, orden_aux.tamMedia);
				orden.tamMedia = orden_aux.tamMedia;
				free(orden_aux.pMedia);
				orden_aux.pMedia = NULL;
			}
			else
			{
				orden.pMedia = NULL;
				orden.tamMedia = 0;
			}
			orden.tipo = orden_aux.tipo;

			_ordenes.pop();
			delete p_lock;

			if (orden.tipo == OP_RECORD)		//ESC. Comentado "&& staRtsp == eRtspSetup)"
			{
				//			NLOG_DEBUG("ED-137. RTSP Sesion (%s). Enviando RECORD...", _cfgRecA.recurso.c_str());
				ProcessRecord(orden.propiedades, orden.operaciones, orden.connref, nRec);
			}
			else if (orden.tipo == OP_PAUSE)		//ESC. Comewntado "&& staRtsp == eRtspGrabando)"
			{
				//			NLOG_DEBUG("ED-137. RTSP Sesion (%s). Enviando PAUSE...", _cfgRecA.recurso.c_str());
				ProcessPause(orden.propiedades, orden.operaciones, orden.connref, nRec);
			}
			else if (orden.tipo == OP_PARAM)		//ESC. Comentado "&& (staRtsp == eRtspSetup || staRtsp == eRtspGrabando))"
			{
				//			NLOG_DEBUG("ED-137. RTSP Sesion (%s). Enviando PARAM...", _cfgRecA.recurso.c_str());
				ProcessParam(orden.propiedades, orden.operaciones, orden.connref, nRec);
			}
			else if (orden.tipo == OP_TEARDOWN)		//ESC. Comentado "&& (staRtsp == eRtspSetup || staRtsp == eRtspGrabando))"
			{
				//			NLOG_DEBUG("ED-137. RTSP Sesion (%s). Enviando TEARDOWN...", _cfgRecA.recurso.c_str());
				ProcessTearDown(nRec);
				break;				//===> ProcessTearDown(), ejecuta "VaciarCola_Ordenes"; evitamos "_ordenes.pop()"
			}
			else if (orden.tipo == OP_SIP_BYE)		//ESC. VOTER. Mensaje para sincronizar SIP-BYE con RTSP-TearDown
			{
				if (_bModoVOTER)	//18.01.2017. SÓLO se va a procesar SIP-BYE en modo VOTER.
				{
					ProcessSipBYE(nRec);
					break;			//===> ProcessSipBYE(), ejecuta "VaciarCola_Ordenes"; evitamos "_ordenes.pop()"
				}
			}
			else if (orden.tipo == OP_SIP_INVITE)	//ESC. Mensaje para acelerar la conexión TCP con el Recorder. VOTER
			{
			}
			else
			{
				NLOG_ERROR("ED-137. RTSP AutomataGestionOrdenes (%s). Orden Desconocida='%d'.", cfgRec.recurso.c_str(), orden.tipo);
			}
		}

		//===---===---===---===
		//ESC. 05.02.2016. Con independencia del resultado de envío hacia REC_A y REC_B, ¡¡¡BORRA!!! la orden de la lista.
		//EVALUAR: Existencia de dos listas de órdenes, una para REC_A y otra para REC_B, con borrado selectivo.

		/*p_lock = new CCSLock(m_lock_ordenes);
		if (!_ordenes.empty()) _ordenes.pop();
		delete p_lock;
		*/

		/*
		//ESC. 28.01.2015. Al añadir un límite de órdenes a procesar, comento el sleep(). Anteriormente "10" mseg
		sleep(1);
		*/
	}

	bool& SendKeepAlive = (nRec == eRecA) ? _SendKeepAliveA : _SendKeepAliveB;
	if (SendKeepAlive)
	{
		SendKeepAlive = false;
		ProcessKeepAlive(nRec);
	}

	bool bSesReadyRec_Now;

	bSesReadyRec_Now= SesionGrabacionReady(nRec);
	if((bSesReadyRec_Now != bSesReadyRec_In)&& (!bSesReadyRec_Now))
		NLOG_INFO ( "RTSP Sesion (%s) CERRADA con %s (%s).", cfgRec.recurso.c_str(), (nRec == eRecA)?"REC_A":"REC_B", cfgRec.CIPA_Recorder.GetHostName().c_str());

// ESC. 20.07.2017
// Chequeo para comprobar en períodos de inactividad, si el socket TCP sigue conectado con el Recorder

#ifdef CHECK_TCP_SOCKET_WHEN_NO_DATA

	unsigned int& uiCountForCheckConexTCP = (nRec == eRecA) ? uiCountForCheckConexTCP_A : uiCountForCheckConexTCP_B;
	if(iOrdenesProc <= 0)
	{		
		uiCountForCheckConexTCP++;

		if(uiCountForCheckConexTCP >= (2000/SLEEP_RUN_AUTOMATAS))	//Cada 2" comprueba estado de sockets
		{
			uiCountForCheckConexTCP= 0;

			if(bSesReadyRec_In)		//Chequea estado socket con RecA, en caso de que modo == [Setup || Recording]
			{
				/*
				//ESC. Pese a cerrar el socket del peer, este procedimiento no retorna error
				int error_code;
				int error_code_sz= sizeof(error_code);
				iRx= _sckRecA.GetSockOpt(SOL_SOCKET, SO_ERROR, &error_code, &error_code_sz);
				*/
				CTCPSocket& sckRec = (nRec == eRecA) ? _sckRecA : _sckRecB;
				if(!sckRec.TCPcnxAlive())	//ESC. 26.09.2018. Nuevo método de la clase.
				{
					CreaNuevoSocket(nRec);
					NLOG_ERROR("RTSP Sesion (%s). Error en 'TCPcnxAlive'(%s). CreaNuevoSocket", cfgRec.recurso.c_str(), (nRec == eRecA) ? "REC_A" : "REC_B");
					unsigned int& uiBuclesDisc = (nRec == eRecA) ? uiBuclesDiscA : uiBuclesDiscB;
					uiBuclesDisc = BUCLES_SLEEP_AUTOMATA_TCPDESCONECTADO/2;	//MJ prueba espero la mitad del tiempo para conectarme
				}
			}
		}
	}
	else
	{
		uiCountForCheckConexTCP = 0;		//Se ha procesado alguna orden, por lo que se han chequeado los socket.
	}

#endif	//--CHECK_TCP_SOCKET_WHEN_NO_DATA

}


void RTSPSession::AutomataEsperaCmdUA(eRecorder nRec)	//04.07.2016. VOTER: Thread creado, CON conexión TCP/RTSP.
{
	CCritSec& m_lock_ordenes = (nRec == eRecA) ? m_lock_ordenesA : m_lock_ordenesB;	
	CCSLock _lock(m_lock_ordenes);
	std::queue<RTSPOrder>& _ordenes = (nRec == eRecA) ? _ordenesA : _ordenesB;

	if(!_ordenes.empty())			//04.07.2016. Si hay mensajes encolados, intenta abrir conexión TCP+RTSP
	{
		RTSPOrder orden= _ordenes.front();

		if(orden.tipo != OP_TEARDOWN)
		{
			AutomataAbriendoSesionRTSP(nRec);
		}
		else
		{
			ProcessTearDown(nRec);
		}
	}
}

// NOTA: La respuesta a SETUP del grabador real es mayor de 4096 bytes.
#define TAM_BUFFER			1024

/** */
bool RTSPSession::SendAndProcessCmdSesion(std::string cmd, eRecorder nRec)
{
	std::string result;
	unsigned char respuesta [TAM_BUFFER];
	int err;

	//Esta función se llama siempre que se quiere enviar un ANNOUNCE, SETUP o TEARDOWN, entonces matamos el timer de keepalives siempre
	if (nRec == eRecA)
	{
		CCSLock* p_lock_keep_mtimer = new CCSLock(m_lock_keepalive_mtimerA);
		_SendKeepAliveA = false;
		if (keepalive_mtimerA != NULL)
		{
			keepalive_mtimerA->EndThread();
			keepalive_mtimerA->Stop();
			delete keepalive_mtimerA;
			keepalive_mtimerA = NULL;
		}
		delete p_lock_keep_mtimer;
	}
	else
	{
		CCSLock* p_lock_keep_mtimer = new CCSLock(m_lock_keepalive_mtimerB);
		_SendKeepAliveB = false;
		if (keepalive_mtimerB != NULL)
		{
			keepalive_mtimerB->EndThread();
			keepalive_mtimerB->Stop();
			delete keepalive_mtimerB;
			keepalive_mtimerB = NULL;
		}
		delete p_lock_keep_mtimer;
	}

	int& sessionTimeout = (nRec == eRecB) ? _SessionTimeoutB : _SessionTimeoutA;
	int& interleaved = (nRec == eRecB) ? _InterleavedB : _InterleavedA;
	RTSPConfig& cfgRec= (nRec== eRecB) ? _cfgRecB : _cfgRecA;
	CTCPSocket& sckRec= (nRec== eRecB) ? _sckRecB : _sckRecA;
	std::list<std::pair<std::string, std::string> >& headersRec= (nRec== eRecB) ? _headersRecB : _headersRecA;

	// Si se utiliza timeout puede ser que haya una respuesta antigua preparada.

	try
	{
		if (sckRec.IsReadable(20))					
		{
			while (sckRec.Recv(respuesta, TAM_BUFFER) > 0 )
			{
			}
		}
	}
	catch (...)
	{
	}

	result= "0";
	headersRec.clear();


#ifdef CODE_FOR_TUNNING
//		NLOG_INFO("(%s)-%s. TX mensaje al grabador", cfgRec.recurso.c_str(), (nRec==eRecB)? "REC_B":"REC_A");
#endif

	if ( (err= sckRec.Send(cmd.c_str(), cmd.length())) < (int )cmd.length())			
	{
#ifdef _WIN32
		NLOG_ERROR("RTSP Sesion (%s). Error .Send_Method(%s). WSAErr(%d). CreaNuevoSocket", cfgRec.recurso.c_str(), cfgRec.CIPA_Recorder.GetHostName().c_str(), WSAGetLastError());	//Antes "sckRec.GetHostAddress().GetStringAddress().c_str()"
#else
		NLOG_ERROR("RTSP Sesion (%s). Error .Send_Method(%s). errno(%d). CreaNuevoSocket", cfgRec.recurso.c_str(), (nRec== eRecB)? "REC_B":"REC_A", errno);	//ESC. provoca excepción "sckRec.GetHostAddress().GetStringAddress().c_str()"
#endif
		CreaNuevoSocket(nRec);

		return false;
	}

//	NLOG_DEBUG("ED-137. RTSP Sesion (%s). Recibiendo respuesta", cfgRec.recurso.c_str() );
	int recibidos = 0;
	try
	{
		//memset(respuesta, 0x0, TAM_BUFFER );
		recibidos = sckRec.Recv(respuesta, TAM_BUFFER);
#ifdef CODE_FOR_TUNNING
//		NLOG_INFO("(%s)-%s. RX respuesta del grabador", _cfgRecA.recurso.c_str(), (nRec==eRecB)? "REC_B":"REC_A");
#endif
	}
	catch(socket_error err)
	{
		if (err.GetCode() == WSAETIMEDOUT )
		{
			NLOG_ERROR("RTSP Sesion (%s). TIMEOUT en espera de respuesta", cfgRec.recurso.c_str() );
			_bTimeoutRecA = true;
			return false;
		}
		else
		{
			throw err;
		}
	}

	if (recibidos <= 0 )
	{
#ifdef _WIN32
/*
		//15.10.2018. En caso de activar esta funcionalidad, habría que retransmitir el mensaje RTSP
		int iRet= WSAGetLastError();		//ESC. 15.10.2018. No cierra el socket TCP en caso de no respuesta del grabador
		if(iRet != WSAETIMEDOUT)			//En caso de mantener esta modalidad, considerar retransmisión
*/
#else
		//EPG
#endif
		{
#ifdef _WIN32
			NLOG_ERROR("RTSP Sesion (%s). Error o TimeOut socket.Recv_(%s). WSAErr(%d). CreaNuevoSocket", cfgRec.recurso.c_str(), sckRec.GetHostAddress().GetStringAddress().c_str(), WSAGetLastError());
#else
			NLOG_ERROR("RTSP Sesion (%s). Error o TimeOut socket.Recv_(%s). errno(%d). CreaNuevoSocket", cfgRec.recurso.c_str(), (nRec== eRecB)? "REC_B":"REC_A", errno);	//ESC. provoca excepción "sckRec.GetHostAddress().GetStringAddress().c_str()"
#endif
			CreaNuevoSocket(nRec);
		}

		return false;
	}
	
//	NLOG_DEBUG("ED-137. RTSP Sesion (%s). Recibidos %d bytes", cfgRec.recurso.c_str(), recibidos );
	if ( recibidos > 0 )
	{
		/** */
		respuesta[recibidos < TAM_BUFFER ? recibidos : TAM_BUFFER-1]=0;
		std::string rsp((char *)respuesta);
//		NLOG_DEBUG("[\n%s\n]", respuesta );
		if (RTSPMessages::RspParse(rsp, result, headersRec)==false)
		{
			NLOG_ERROR("ED-137. RTSP Sesion (%s). Error en Formato de Respuesta: <%s>", cfgRec.recurso.c_str(), rsp.c_str());
			return false; //!respOKObl;
		}

#ifdef CODE_FOR_TUNNING
		NLOG_INFO("(%s)-%s. Procesada respuesta del grabador", _cfgRecA.recurso.c_str(), (nRec==eRecB)? "REC_B":"REC_A");
#endif

//		NLOG_DEBUG("ED-137. RTSP Sesion (%s). Resultado=%s", cfgRec.recurso.c_str(), result.c_str() );
		if (result != "200")
		{
			NLOG_ERROR("ED-137. RTSP Sesion (%s) en [%s]. Respuesta Incorrecta <%s>", cfgRec.recurso.c_str(), (nRec==eRecB)?"REC_B":"REC_A", rsp.c_str());
			return false; //!respOKObl;
		}

		//Dependiendo de la cabecera WG67-Version recibida en la respuesta al ANNOUNCE, establecemos la version		
		char& ED137Version = (nRec == eRecA) ? _ED137VersionA : _ED137VersionB;
		string announce = "ANNOUNCE";
		if (cmd.compare(0, announce.size(), announce) == 0)
		{
			ED137Version = 'B';
			std::list< std::pair<std::string, std::string> >::iterator it;
			for (it = headersRec.begin(); it != headersRec.end(); ++it)
			{
				std::pair<std::string, std::string> header = *it;
                if (STRICMP(header.first.c_str(), "WG67-Version") == 0)
				{
					for (unsigned int i = 0; i < header.second.size(); i++)
					{
						header.second[i] = tolower(header.second[i]);
					}
					if (header.second.find("recorder.02") != string::npos)
					{
						ED137Version = 'C';
					}
				}
			}
		}

		string setup_ = "SETUP";
		if (cmd.compare(0, setup_.size(), setup_) == 0)
		{
			sessionTimeout = 0;
			std::list< std::pair<std::string, std::string> >::iterator it;
			for (it = headersRec.begin(); it != headersRec.end(); ++it)
			{
				std::pair<std::string, std::string> header = *it;
                if (STRICMP(header.first.c_str(), "Session") == 0)
				{
					for (unsigned int i = 0; i < header.second.size(); i++)
					{
						header.second[i] = tolower(header.second[i]);
					}
					string header_second = header.second;
					(void) remove(header_second.begin(), header_second.end(), ' ');	//Quitamos posibles espacios en blanco
					size_t pos = header_second.find("timeout=");
					if (pos != string::npos)
					{
						pos += string("timeout=").length();
						size_t pos_coma = header_second.find(";", pos);
						size_t pos_cr = header_second.find("\r", pos);
						if (pos_coma != string::npos)
						{
							string timeout = header_second.substr(pos, pos_coma-pos);
							try {
								sessionTimeout = Utilidades::mystoi(timeout);
							}
							catch (...)
							{
								NLOG_ERROR("ED-137. RTSP Sesion (%s) en [%s]. Respuesta Incorrecta. timeout no correcto", cfgRec.recurso.c_str(), (nRec == eRecB) ? "REC_B" : "REC_A");
								sessionTimeout = 0;
								return false;
							}
						}
						else if (pos_cr != string::npos)
						{
							string timeout = header_second.substr(pos, pos_cr-pos);
							try {
								sessionTimeout = Utilidades::mystoi(timeout);
							}
							catch (...)
							{
								NLOG_ERROR("ED-137. RTSP Sesion (%s) en [%s]. Respuesta Incorrecta. timeout no correcto", cfgRec.recurso.c_str(), (nRec == eRecB) ? "REC_B" : "REC_A");
								sessionTimeout = 0;
								return false;
							}
						}						
					}
					else if (ED137Version == 'C')
					{
						//Si la version es ED137C y no hay parametro de timeout entonces la norma dice que por defecto son 60 segundos
						sessionTimeout = 60;
					}
					else
					{
						sessionTimeout = 60;
					}
				}

                if (STRICMP(header.first.c_str(), "Transport") == 0)
				{
					interleaved = 0;
					for (unsigned int i = 0; i < header.second.size(); i++)
					{
						header.second[i] = tolower(header.second[i]);
					}
					string header_second = header.second;
					(void)remove(header_second.begin(), header_second.end(), ' ');	//Quitamos posibles espacios en blanco
					size_t pos = header_second.find("interleaved=");
					if (pos != string::npos)
					{
						pos += string("interleaved=").length();
						size_t pos_coma = header_second.find(";", pos);
						size_t pos_cr = header_second.find("\r", pos);
						if (pos_coma != string::npos)
						{
							string interleaved_val = header_second.substr(pos, pos_coma - pos);
							try {
								interleaved = Utilidades::mystoi(interleaved_val);
							}
							catch (...)
							{
								NLOG_ERROR("ED-137. RTSP Sesion (%s) en [%s]. Respuesta Incorrecta. interleaved no correcto", cfgRec.recurso.c_str(), (nRec == eRecB) ? "REC_B" : "REC_A");
								interleaved = 0;
								return false;
							}
						}
						else if (pos_cr != string::npos)
						{
							string interleaved_val = header_second.substr(pos, pos_cr - pos);
							try {
								interleaved = Utilidades::mystoi(interleaved_val);
							}
							catch (...)
							{
								NLOG_ERROR("ED-137. RTSP Sesion (%s) en [%s]. Respuesta Incorrecta. interleaved no correcto", cfgRec.recurso.c_str(), (nRec == eRecB) ? "REC_B" : "REC_A");								
								interleaved = 0;
								return false;
							}
						}
					}					
				}
			}
			if (sessionTimeout > 0)
			{
				int keep_alive_time;
				if (sessionTimeout > 60) keep_alive_time = sessionTimeout - 20;
				if (sessionTimeout > 20) keep_alive_time = sessionTimeout - 10;
				else if (sessionTimeout > 10) keep_alive_time = sessionTimeout - 5;
				else if (sessionTimeout > 5) keep_alive_time = sessionTimeout - 3;
				else if (sessionTimeout > 2) keep_alive_time = sessionTimeout - 1;
				else keep_alive_time = sessionTimeout;

				if (nRec == eRecA)
				{
					CCSLock* p_lock_keep_mtimer = new CCSLock(m_lock_keepalive_mtimerA);
					_SendKeepAliveA = false;
					keepalive_mtimerA = new Mtimer(keep_alive_time, (void*)this, &keepalive_mtimerA_cb);
					keepalive_mtimerA->Start();
					delete p_lock_keep_mtimer;
				}
				else
				{
					CCSLock* p_lock_keep_mtimer = new CCSLock(m_lock_keepalive_mtimerB);
					_SendKeepAliveB = false;
					keepalive_mtimerB = new Mtimer(keep_alive_time, (void*)this, &keepalive_mtimerB_cb);
					keepalive_mtimerB->Start();
					delete p_lock_keep_mtimer;
				}
			}
		}				
	}
	return true;
}

void RTSPSession::keepalive_mtimerA_cb(void* param)
{
	if (param != NULL)
	{
		RTSPSession *rtsp_sess = (RTSPSession*)param;
		rtsp_sess->_SendKeepAliveA = true;
	}
}

void RTSPSession::keepalive_mtimerB_cb(void* param)
{
	if (param != NULL)
	{
		RTSPSession* rtsp_sess = (RTSPSession*)param;
		rtsp_sess->_SendKeepAliveB = true;
	}
}

//ESC. 14.01.2016
void RTSPSession::CreaNuevoSocket(eRecorder nRec)
{
	CTCPSocket& sckRec= (nRec== eRecB) ? _sckRecB : _sckRecA;

	SetEstadoRtsp ( eRtspSckDisc, nRec );		//10.04.2017. En primer lugar asigna estado Desconectado.

	VaciarCola_Ordenes(nRec);		
									//	(+) No se ejecutarían comandos "antiguos/innecesarios"
									//  (+) Se ejecutarán los comandos correspondientes al estado actual
									//  (-) Se dejan de grabar eventos y audio almacenados en la cola

	try
	{
		sckRec.NewSocket();

		sleep(300);

		//ESC. Añadido al código original, procedente de Init()
		struct timeval tv;
#ifdef _WIN32
		tv.tv_sec= 1000;			//ESC. 15.10.2018.
		tv.tv_usec = 0;
#else
		tv.tv_sec = 1;
		tv.tv_usec = 0;
#endif

#ifdef _WIN32
		BOOL op = TRUE;
		//sckRec.SetSockOpt (IPPROTO_TCP, TCP_NODELAY, (char *)&op, sizeof(BOOL));
#else
		//int op = 1;
		//sckRec.SetSockOpt (SOL_TCP, TCP_NODELAY, (char *)&op, sizeof(int));
#endif
		sckRec.SetSockOpt (SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval));
		sckRec.SetSockOpt (SOL_SOCKET, SO_SNDTIMEO, (char *)&tv, sizeof(struct timeval));

#ifdef ACTIVA_KEEPALIVE_TCP
//ESC. 07.04.2017
	int optval= 1;				//Activa KeepAlive en TCP socket
	sckRec.SetSockOpt (SOL_SOCKET, SO_KEEPALIVE, /*(int *)*/&optval, sizeof(optval));

#ifdef _WIN32
	ConfiguraParamsTCP_KeepAlive_Windows(sckRec);
#else
	ConfiguraParamsTCP_KeepAlive_Linux(sckRec);
#endif

//ESC. 07.04.2017
#endif
	}
	catch (...)
	{
	}
}
//--ESC



/** */
bool RTSPSession::MetodoAnnounce(eRecorder nRec/*= eRecA*/)
{
	int& Rcseq= (nRec == eRecB) ? _cseqRecB : _cseqRecA;
	RTSPConfig& cfgRec= (nRec == eRecB) ? _cfgRecB : _cfgRecA;
	std::string& uriSamplerRec= (nRec == eRecB) ? _uriSamplerRecB : _uriSamplerRecA;

	const char* sdpForAnnounce =
		"v=0\r\n"
		"o=UG5K 2520644554 2838152170 IN IP4 %s\r\n"
		"s=ED137C4-Session\r\n"
		"c=IN IP4 %s\r\n"
		"t=0 0\r\n"
		"m=audio 0 TCP/RTP 8\r\n"
		"a=rtpmap:0 PCMA/8000\r\n";

	string sdpForAnnounce_string = Utilidades::StrFormat(sdpForAnnounce, _ipPasarela.c_str(), _ipPasarela.c_str());

	if ( ! SendAndProcessCmdSesion(RTSPMessages::MsgANNOUNCE(Rcseq, uriSamplerRec, sdpForAnnounce_string), nRec) )
	{
		NLOG_ERROR("RTSP Sesion (%s). ERROR respuesta a ANNOUNCE de %s", cfgRec.recurso.c_str(), (nRec == eRecB)? "REC_B":"REC_A" );
		return false;
	}
	// TODO. Analisis en detalle de la respuesta. ANNOUNCE

	NLOG_INFO("RTSP Sesion (%s). ANNOUNCE OK con %s", cfgRec.recurso.c_str(), (nRec==eRecB)? "REC_B":"REC_A");

	Rcseq++;

	SetEstadoRtsp ( eRtspAnnc, nRec );

	return true;
}

/** */
bool RTSPSession::MetodoSetup(eRecorder nRec/*= eRecA*/)
{
	int& Rcseq= (nRec == eRecB) ? _cseqRecB : _cseqRecA;
	RTSPConfig& cfgRec= (nRec == eRecB) ? _cfgRecB : _cfgRecA;
	std::string& uriSamplerRec= (nRec == eRecB) ? _uriSamplerRecB : _uriSamplerRecA;
	std::string& RsessionRec= (nRec == eRecB) ? _sessionRecB : _sessionRecA;
	char& ED137Version = (nRec == eRecA) ? _ED137VersionA : _ED137VersionB;

	if ( ! SendAndProcessCmdSesion(RTSPMessages::MsgSETUP(Rcseq, uriSamplerRec, 0, ED137Version), nRec) )
	{
		SetEstadoRtsp ( eRtspCnxSck, nRec );
		NLOG_ERROR("RTSP Sesion (%s). ERROR respuesta a SETUP de %s", cfgRec.recurso.c_str(), (nRec == eRecB)? "REC_B":"REC_A" );
		return false;
	}

	std::string esperado = "RTP/AVP/TCP;interleaved=";
	std::string Transport_rcv = GetHeaderVal("Transport", nRec);
	(void)remove(Transport_rcv.begin(), Transport_rcv.end(), ' ');	//Quitamos posibles espacios en blanco
	size_t pos = Transport_rcv.find(esperado);
	if (pos == string::npos)
	{
		NLOG_ERROR("RTSP Sesion (%s). ERROR respuesta a SETUP en Transport header %s", GetHeaderVal("Transport", nRec).c_str());
		return false;
	}

	// SETUP. Hay que recoger el header "Session"
	RsessionRec = RTSPMessages::NormalizeHeaderValue( GetHeaderVal("Session", nRec));
	//_session = "1";
	Rcseq++;
	//_transporte = eTrpEmbedded;

	MediaTransportInit(nRec);

	SetEstadoRtsp ( eRtspSetup, nRec );

	NLOG_INFO ("RTSP Sesion (%s). SETUP OK con %s.", cfgRec.recurso.c_str(), (nRec==eRecB)? "REC_B":"REC_A");

	return true;
}

/** */
bool RTSPSession::ProcessKeepAlive(eRecorder nRec)
{
	int estadoRtspRec = GetEstadoRtsp(nRec);
	
	if ((estadoRtspRec == eRtspSetup) || (estadoRtspRec == eRtspGrabando))
	{
		if (nRec == eRecA)
		{
			int i = 0;
			for (i = 0; i < 3; i++)
			{
				if (SendCmdSesion(RTSPMessages::MsgGetPARAMS(_cseqRecA, _uriSamplerRecA, _sessionRecA, _ED137VersionA), eRecA))
				{
					if (ProcesaRespuestaRecorder(nRec))
					{
						break;
					}
				}
			}
		}
		else
		{
			int i = 0;
			for (i = 0; i < 3; i++)
			{
				if (SendCmdSesion(RTSPMessages::MsgGetPARAMS(_cseqRecB, _uriSamplerRecB, _sessionRecB, _ED137VersionB), eRecB))
				{
					if (ProcesaRespuestaRecorder(nRec))
					{
						break;
					}
				}
			}
		}
	}
	return true;
}

/** */
bool RTSPSession::ProcessRecord(rtsp_props propiedades, rtsp_props operaciones, string connref, eRecorder nRec)
{
	rtsp_props props;

	if (propiedades.size() > 0)
	{
		props= propiedades;
	}
	else
	{
		/*
		//ESC. 23.01.2017. UA Radio (GRS ó VCS). Si NO se reciben PROPIEDADES, NO las rellena por DEFECTO.
		//	=> eUA_GRS: A partir de un msje de "SQU ON", inserta comando REC+"propiedades". No recibe msje"REC"
		//	=> eUA_VcsRad: (*) Se recibe msje "REC", pero NO incluye "propiedades" 
		//				   (*) Se recibe msje "SQU ON", que inserta comando SET_PARAM+"propiedades"
		if ( _tipoUA==eUA_GRS )
		{
			if( ! GetFrecID_GRS().empty())					//ESC. 10.02.2017
				props.push_back(rtsp_prop("FrequencyID", GetFrecID_GRS()));
		}
		else if ( _tipoUA==eUA_VcsRad )
		{
		}
		*/
	}

	if ( _tipoUA== eUA_VcsTel )
	{
		//Como en el caso de telefonia connref es call-id@ip, nos quedamos solo con el call-id
		string callref = connref.substr(0, connref.find("@"));		
		props.push_back(rtsp_prop("CallRef", callref));
	}

	RTSPConfig& cfgRec = (nRec == eRecA) ? _cfgRecA : _cfgRecB;
	string clientid = cfgRec.recurso + "@" + _ipPasarela;
	props.push_back(rtsp_prop("ClientId", clientid));

	int estadoRtspRec = GetEstadoRtsp(nRec);

#ifdef GRABACION_SECUENCIAL
	if((estadoRtspRecA == eRtspSetup)|| (estadoRtspRecA == eRtspGrabando))
	{
		std::string crd = RTSPMessages::CRD(_connrefRecA, props, operaciones);

		if(SendAndProcessCmdSesion(RTSPMessages::MsgRECORD(_cseqRecA, _uriSamplerRecA, _sessionRecA, crd), eRecA))
		{
//			NLOG_DEBUG("ED-137. RTSP Sesion (%s). Procesando Respuesta a RECORD...", _cfgRecA.recurso.c_str());
			_cseqRecA++;
			SetEstadoRtsp (eRtspGrabando, eRecA);
		}
	}
	else
	{
		//ESC. No puede enviar el mensaje a REC_X, teniendo en cuenta su estado
	}

	if(_bDualRec)
	{
		estadoRtspRecB= GetEstadoRtsp(eRecB);
		if((estadoRtspRecB == eRtspSetup)|| (estadoRtspRecB == eRtspGrabando))
		{
			std::string crd = RTSPMessages::CRD(_connrefRecB, props, operaciones);

			if(SendAndProcessCmdSesion(RTSPMessages::MsgRECORD(_cseqRecB, _uriSamplerRecB, _sessionRecB, crd), eRecB))
			{
//				NLOG_DEBUG("ED-137. RTSP Sesion (%s). Procesando Respuesta a RECORD...", _cfgRecB.recurso.c_str());
				_cseqRecB++;
				SetEstadoRtsp (eRtspGrabando, eRecB);
			}
		}
		else
		{
			//ESC. No puede enviar el mensaje a REC_X, teniendo en cuenta su estado
		}	
	}
#else

	//ESC. 11.02.2016. Grabación DUAL en "PARALELO".

	int& cseqRec = (nRec == eRecA) ? _cseqRecA : _cseqRecB;
	std::string& uriSamplerRec = (nRec == eRecA) ? _uriSamplerRecA : _uriSamplerRecB;
	std::string& sessionRec = (nRec == eRecA) ? _sessionRecA : _sessionRecB;
	char& ED137Version = (nRec == eRecA) ? _ED137VersionA : _ED137VersionB;
	if((estadoRtspRec == eRtspSetup)|| (estadoRtspRec == eRtspGrabando))
	{
		int i = 0;
		for (i = 0; i < 3; i++)
		{
			std::string crd = RTSPMessages::CRD(connref, props, operaciones);
			if (SendCmdSesion(RTSPMessages::MsgRECORD(cseqRec, uriSamplerRec, sessionRec, crd, ED137Version), nRec))
			{
				if (ProcesaRespuestaRecorder(nRec))
				{
					cseqRec++;
					SetEstadoRtsp(eRtspGrabando, nRec);
					break;
				}
			}
		}
		if (i == 3)
		{
			if (i == 3) CreaNuevoSocket(nRec);
		}
	}
	

#endif

	RTPSession& rtp = (nRec == eRecA) ? _rtpA : _rtpB;
	rtp.SetBitMarker();		//ESC. 09.01.2017. Al recibir el método RECORD, activará Bit Marker en la siguientre trama RTP.
	//MediaTransportInit();		//ESC. 09.01.2017. Al recibir el método RECORD, inicializa sesión RTP

	return true;
}

/** */
bool RTSPSession::ProcessPause(rtsp_props propiedades, rtsp_props operaciones, string connref, eRecorder nRec)
{
	rtsp_props props;

	if (propiedades.size() > 0)
	{
		props= propiedades;
	}
	else
	{
		/*
		//ESC. 13.02.2017. UA Radio (GRS ó VCS). Si NO se reciben PROPIEDADES, NO las rellena por DEFECTO.
		//	=> eUA_GRS: A partir de un msje de "SQU OFF", inserta comando PAU+"propiedades". No recibe msje"PAUSE"
		//	=> eUA_VcsRad: (*) Se recibe msje "PAU", pero NO incluye "propiedades" 
		//				   (*) Se recibe msje "SQU OFF", que inserta comando SET_PARAM+"propiedades"
		if ( _tipoUA==eUA_GRS )
		{
			if( ! GetFrecID_GRS().empty())					//ESC. 10.02.2017
				props.push_back(rtsp_prop("FrequencyID", GetFrecID_GRS()));
		}
		else ( _tipoUA==eUA_VcsRad )
		{
		}
		*/
	}

	if ( _tipoUA== eUA_VcsTel )
	{
		//Como en el caso de telefonia connref es call-id@ip, nos quedamos solo con el call-id
		string callref = connref.substr(0, connref.find("@"));
		props.push_back(rtsp_prop("CallRef", callref));
	}

	RTSPConfig& cfgRec = (nRec == eRecA) ? _cfgRecA : _cfgRecB;
	string clientid = cfgRec.recurso + "@" + _ipPasarela;
	props.push_back(rtsp_prop("ClientId", clientid));

	int estadoRtspRec = GetEstadoRtsp(nRec);

#ifdef GRABACION_SECUENCIAL
	if((estadoRtspRecA == eRtspSetup)|| (estadoRtspRecA == eRtspGrabando))
	{
		std::string crd = RTSPMessages::CRD(_connrefRecA, props, operaciones);

		if (SendAndProcessCmdSesion(RTSPMessages::MsgPAUSE(_cseqRecA, _uriSamplerRecA, _sessionRecA, crd), eRecA))
		{
//			NLOG_DEBUG("ED-137. RTSP Sesion (%s). Procesando Respuesta a PAUSE...", _cfgRecA.recurso.c_str());
			_cseqRecA++;
			SetEstadoRtsp ( eRtspSetup, eRecA );
		}
	}
	else
	{
		//ESC. No puede enviar el mensaje a REC_X, teniendo en cuenta su estado
	}

	if(_bDualRec)
	{
		estadoRtspRecB= GetEstadoRtsp(eRecB);
		if((estadoRtspRecB == eRtspSetup)|| (estadoRtspRecB == eRtspGrabando))
		{
			std::string crd = RTSPMessages::CRD(_connrefRecB, props, operaciones);

			if (SendAndProcessCmdSesion(RTSPMessages::MsgPAUSE(_cseqRecB, _uriSamplerRecB, _sessionRecB, crd), eRecB))
			{
//				NLOG_DEBUG("ED-137. RTSP Sesion (%s). Procesando Respuesta a PAUSE...", _cfgRecB.recurso.c_str());
				_cseqRecB++;
				SetEstadoRtsp ( eRtspSetup, eRecB );
			}
		}
	}
#else
	//ESC. 11.02.2016. Grabación DUAL en "PARALELO".

	int& cseqRec = (nRec == eRecA) ? _cseqRecA : _cseqRecB;
	std::string& uriSamplerRec = (nRec == eRecA) ? _uriSamplerRecA : _uriSamplerRecB;
	std::string& sessionRec = (nRec == eRecA) ? _sessionRecA : _sessionRecB;
	char& ED137Version = (nRec == eRecA) ? _ED137VersionA : _ED137VersionB;
	if((estadoRtspRec == eRtspSetup)|| (estadoRtspRec == eRtspGrabando))
	{
		int i = 0;
		for (i = 0; i < 3; i++)
		{
			std::string crd = RTSPMessages::CRD(connref, props, operaciones);
			if (SendCmdSesion(RTSPMessages::MsgPAUSE(cseqRec, uriSamplerRec, sessionRec, crd, ED137Version), nRec))
			{
				if (ProcesaRespuestaRecorder(nRec))
				{
					cseqRec++;
					SetEstadoRtsp(eRtspSetup, nRec);
					break;
				}
			}
		}
		if (i == 3) CreaNuevoSocket(nRec);
	}

#endif

	return true;
}

/** */
bool RTSPSession::ProcessParam(rtsp_props propiedades, rtsp_props operaciones, string connref, eRecorder nRec)
{
	if(propiedades.empty())		//ESC. 19.01.2016. Si la orden NO incluye propiedades, añade por defecto ...
	{
		rtsp_props props;

		/*
		if ( _tipoUA==eUA_GRS )
		{
			if( ! GetFrecID_GRS().empty())					//ESC. 10.02.2017
				props.push_back(rtsp_prop("FrequencyID", GetFrecID_GRS()));
		}
		else if ( _tipoUA==eUA_VcsRad )
		{
		}
		*/

		propiedades= props;		//ESC. asigna las propiedades por defecto
	}

	if ( _tipoUA== eUA_VcsTel )
	{
		//Como en el caso de telefonia connref es call-id@ip, nos quedamos solo con el call-id
		string callref = connref.substr(0, connref.find("@"));
		propiedades.push_back(rtsp_prop("CallRef", callref));
	}

	RTSPConfig& cfgRec = (nRec == eRecA) ? _cfgRecA : _cfgRecB;
	string clientid = cfgRec.recurso + "@" + _ipPasarela;
	propiedades.push_back(rtsp_prop("ClientId", clientid));

	int estadoRtspRec = GetEstadoRtsp(nRec);

#ifdef GRABACION_SECUENCIAL
	if((estadoRtspRecA == eRtspSetup)|| (estadoRtspRecA == eRtspGrabando))
	{
		std::string crd = RTSPMessages::CRD(_connrefRecA, propiedades, operaciones);

		if (SendAndProcessCmdSesion(RTSPMessages::MsgPARAMS(_cseqRecA, _uriSamplerRecA, _sessionRecA, crd), eRecA) )
		{
//			NLOG_DEBUG("ED-137. RTSP Sesion (%s). Procesando Respuesta a PARAM...", _cfgRecA.recurso.c_str());
			_cseqRecA++;
		}
	}

	if(_bDualRec)
	{
		estadoRtspRecB= GetEstadoRtsp(eRecB);
		if((estadoRtspRecB == eRtspSetup)|| (estadoRtspRecB == eRtspGrabando))
		{
			std::string crd = RTSPMessages::CRD(_connrefRecB, propiedades, operaciones);

			if (SendAndProcessCmdSesion(RTSPMessages::MsgPARAMS(_cseqRecB, _uriSamplerRecB, _sessionRecB, crd), eRecB) )
			{
//				NLOG_DEBUG("ED-137. RTSP Sesion (%s). Procesando Respuesta a PARAM...", _cfgRecB.recurso.c_str());
				_cseqRecB++;
			}
		}
	}
#else

	int& cseqRec = (nRec == eRecA) ? _cseqRecA : _cseqRecB;
	std::string& uriSamplerRec = (nRec == eRecA) ? _uriSamplerRecA : _uriSamplerRecB;
	std::string& sessionRec = (nRec == eRecA) ? _sessionRecA : _sessionRecB;
	char& ED137Version = (nRec == eRecA) ? _ED137VersionA : _ED137VersionB;
	if ((estadoRtspRec == eRtspSetup) || (estadoRtspRec == eRtspGrabando))
	{
		int i = 0;
		for (i = 0; i < 3; i++)
		{
			std::string crd = RTSPMessages::CRD(connref, propiedades, operaciones);
			if (SendCmdSesion(RTSPMessages::MsgPARAMS(cseqRec, uriSamplerRec, sessionRec, crd, ED137Version), nRec))
			{
				if (ProcesaRespuestaRecorder(nRec))
				{
					cseqRec++;
					break;
				}
			}
		}
		if (i == 3) CreaNuevoSocket(nRec);
	}
#endif

	return true;
}

/** */
bool RTSPSession::ProcessTearDown(eRecorder nRec)
{
	if (nRec == eRecA) NLOG_INFO("ProcessTearDown eRecA");
	else NLOG_INFO("ProcessTearDown eRecB");

	//OJO: La función que llame a este método, debe ejecutar "CCSLock _lock(m_lock_ordenes)"
	VaciarCola_Ordenes(nRec);

	int estado= GetEstadoRtsp(nRec);

	RTSPConfig& cfgRec = (nRec == eRecA) ? _cfgRecA : _cfgRecB;
	int& cseqRec = (nRec == eRecA) ? _cseqRecA : _cseqRecB;
	std::string& uriSamplerRec = (nRec == eRecA) ? _uriSamplerRecA : _uriSamplerRecB;
	std::string& sessionRec = (nRec == eRecA) ? _sessionRecA : _sessionRecB;
	unsigned int& uiBuclesDisc = (nRec == eRecA) ? uiBuclesDiscA : uiBuclesDiscB;
	char& ED137Version = (nRec == eRecA) ? _ED137VersionA : _ED137VersionB;

	if((estado== eRtspSetup)|| (estado== eRtspGrabando))		//ESC. 05.02.2016. Evaluar si BORRAR socket. En ese caso, NO comprobar estado
	{
		if (!SendAndProcessCmdSesion(RTSPMessages::MsgTEARDOWN(cseqRec, uriSamplerRec, sessionRec, ED137Version), nRec))
		{
			NLOG_ERROR("RTSP Sesion (%s). ERROR respuesta a TEARDOWN de %s. CreaNuevoSocket()", cfgRec.recurso.c_str(), (nRec == eRecA)?"REC_A":"REC_B");
		}
	}
	cseqRec = 1;
	sessionRec = "";

	CreaNuevoSocket( nRec );		//¡OJO! asigna estado Rtsp a 'eRtspSckDisc'

#ifdef TEARDOWN_NO_CONECTA_TCP		
	SetEstadoRtsp(eRtspTearDown, eRecA);				//ESC. 15.11.2016. Asigna estado TearDown (no intenta conexión TCP)
	uiBuclesDiscA= BUCLES_SLEEP_AUTOMATA_TCPDESCONECTADO -1;	//Cuando "eRtspSckDisc", intentará reconexión TCP "INMEDIATA" con REC_x
#else
	uiBuclesDisc = 0;	//ESC. 10.11.2016. VOTER, impide una nueva conexión, antes de que finalice la prueba. Si Rx cmd 'INV', intenta conexión inmediata.
#endif

	return true;
}

void RTSPSession::ProcessSipBYE(eRecorder nRec)
{
	//OJO: La función que llame a este método, debe ejecutar "CCSLock _lock(m_lock_ordenes)"
	VaciarCola_Ordenes(nRec);

	//Enviará método TEARDOWN, pero no cierra socket TCP
	int estado= GetEstadoRtsp(nRec);

	RTSPConfig& cfgRec = (nRec == eRecA) ? _cfgRecA : _cfgRecB;
	int& cseqRec = (nRec == eRecA) ? _cseqRecA : _cseqRecB;
	std::string& uriSamplerRec = (nRec == eRecA) ? _uriSamplerRecA : _uriSamplerRecB;
	std::string& sessionRec = (nRec == eRecA) ? _sessionRecA : _sessionRecB;
	unsigned int& uiBuclesDisc = (nRec == eRecA) ? uiBuclesDiscA : uiBuclesDiscB;
	char& ED137Version = (nRec == eRecA) ? _ED137VersionA : _ED137VersionB;

	if((estado== eRtspSetup)|| (estado== eRtspGrabando))
	{
		if( ! SendAndProcessCmdSesion(RTSPMessages::MsgTEARDOWN(cseqRec, uriSamplerRec, sessionRec, ED137Version), nRec))
			NLOG_ERROR("RTSP Sesion (%s). ERROR respuesta a BYE de %s. CreaNuevoSocket()", cfgRec.recurso.c_str(), (nRec == eRecA) ? "REC_A" : "REC_B");
	}
	cseqRec = 1;
	sessionRec = "";

	SetEstadoRtsp(eRtspCnxSck, nRec);
	uiBuclesDisc = BUCLES_SLEEP_AUTOMATA_TCPDESCONECTADO -1;	//Enviará método "ANN" al recibir el próximo comando de CoreSIP/LibOSIP.
}


/** */ 
void RTSPSession::MediaTransportInit(eRecorder nRec)
{
	RTPSession& rtp = (nRec == eRecA) ? _rtpA : _rtpB;
	rtp.Init();
}

/**  */
unsigned char* RTSPSession::CreateEmbeddedMedia(char *media, int nbytes, eRecorder nRec, int& pcktFrame_size)
{
	/*
		Estructura del paquete a enviar...
		| CB-EMBEDDED (4 bytes) | CB RTP (12 bytes) | MEDIA (nbytes bytes) |
	*/

#define NUM_FRAMESTX_FOR_CHECK_CONNECTION	100
	pcktFrame_size = 0;
	if (media == NULL) return NULL;
	if (GetEstadoRtsp(nRec) != eRtspGrabando)
	{
		return NULL;
	}

	unsigned char* pcktFrame;
	int iRtpHeadersz;
	char& ED137Version = (nRec == eRecA) ? _ED137VersionA : _ED137VersionB;
	if (ED137Version == 'C')
	{
		iRtpHeadersz = 18;									// 6+12
	}
	else
	{
		iRtpHeadersz = 16;									// 4+12
	}
	int lmedia = nbytes + 12;								// Incluye la cabecera RTP.	

	pcktFrame = (unsigned char*)malloc(nbytes + iRtpHeadersz);
	memcpy(&(pcktFrame[iRtpHeadersz]), media, nbytes);

	/** Cabecera RTSP transport EMBEDDED */
	RTPSession& rtp = (nRec == eRecA) ? _rtpA : _rtpB;
	if (ED137Version == 'C')
	{
		pcktFrame[0] = '#';
		pcktFrame[1] = 0;	// PADDING. Siempre 0.
		u_short valor = htons((u_short)((nRec == eRecA) ? _InterleavedA : _InterleavedB));
		memcpy(&(pcktFrame[2]), &valor, sizeof(u_short));
		valor = htons((u_short)lmedia);
		memcpy(&(pcktFrame[4]), &valor, sizeof(u_short));
		// Inserta cabecera RTP.
		rtp.HeaderInsert(&pcktFrame[6]);
	}
	else
	{
		pcktFrame[0] = '$';
		pcktFrame[1] = 0;	// CID, canal siempre 0.	
		u_short valor = htons(lmedia);
		memcpy(&(pcktFrame[2]), &valor, sizeof(u_short));
		// Inserta cabecera RTP.
		rtp.HeaderInsert(&pcktFrame[4]);
	}

	pcktFrame_size = nbytes + iRtpHeadersz;

	return pcktFrame;
}

/**  */
bool RTSPSession::SendEmbeddedMedia(unsigned char* pcktFrame, int pcktFrame_size, eRecorder nRec)
{
	bool res = true;

	//OJO: "Send" doesn't actually send anything, it's just a memory-copying function/system call
	CTCPSocket& sckRec = (nRec == eRecA) ? _sckRecA : _sckRecB;
	if (sckRec.Send(pcktFrame, pcktFrame_size) < (pcktFrame_size))
	{
		res = false;
	}
	return ( res );
}

/** */
std::string RTSPSession::GetHeaderVal(std::string header, eRecorder nRec/*= eRecA*/)
{
	std::list< std::pair<std::string, std::string> >::const_iterator iterator;
	std::list<std::pair<std::string, std::string> >& headersRec= (nRec==eRecB) ? _headersRecB : _headersRecA;
	
	for (iterator =  headersRec.begin(); iterator != headersRec.end(); ++iterator) 
	{
		if(RTSPMessages::StringsInsensEqual(iterator->first, header))		//ESC. 18.01.2017. Antes "if (iterator->first == header)"
			return iterator->second;
	}
	return "";
}

void RTSPSession::SignalFinalizacion()
{
	_finalizarSesion = true;
}

//ESC. 12.01.2016
std::string RTSPSession::GetTxtTipoOrden(int iOrden)
{
	std::string stAux;
	switch (iOrden)
	{
		case OP_RECORD:
			stAux= "RECORD";
			break;

		case OP_PAUSE:
			stAux= "PAUSE";
			break;

		case OP_PARAM:
			stAux= "PARAM";
			break;

		case OP_MEDIA:
			stAux= "MEDIA";
			break;

		case OP_TEARDOWN:
			stAux= "TEARDOWN";
			break;

		default:
			stAux= "¿?";
			break;
	}

	return stAux;
}

std::string RTSPSession::GetTxtEstadoRtsp(/*eRtspStd*/int eStd)
{
	std::string stAux;
	switch (eStd)
	{
		case eRtspSckDisc:
			stAux= "RtspDisc";
			break;

		case eRtspCnxSck:
			stAux= "RtspCnxSck";
			break;

		case eRtspAnnc:
			stAux= "RtspAnnc";
			break;

		case eRtspSetup:
			stAux= "RtspSetup";
			break;

		case eRtspGrabando:
			stAux= "RtspRecord";
			break;

		case eRtspTearDown:
			stAux= "RtspTearDown";
			break;

		default:
			stAux= "¿?";
			break;
	}

	return stAux;

}

int RTSPSession::GetTipoUA()
{
	return _tipoUA;
}


void RTSPSession::PushPriority_Orden(RTSPOrder orden, eRecorder nRec)
{
// 28.01.2016. Impide que se encolen más de 'N' órdenes en una sesión NO CONECTADA/OPERATIVA.
	//Se envia a las colas ordenes de los dos grabadores

	if (nRec == eRecA)
	{
		CCSLock* p_lockA = new CCSLock(m_lock_ordenesA);

		if (_ordenesA.size() >= NUM_MAX_ORDENES_ENCOLADAS_RECURSO)
		{
			// "Lee" la orden más antigua ALMACENADA en la lista, y borra la posible memoria dinámica de la media
			RTSPOrder orden_oldest = _ordenesA.front();
			if (orden_oldest.pMedia != NULL)
			{
				free(orden_oldest.pMedia);
				orden_oldest.pMedia = NULL;
			}

			_ordenesA.pop();

			uiCountEvSobrecargaPrioA++;
			if (uiCountEvSobrecargaPrioA >= NUM_MSJES_PRIO_SOBRECARGA_FILTRADOS)
			{
				NLOG_INFO("RTSP Session (%s). SOBRECARGA cola REC_A. Borrados-%d msje cola", _cfgRecA.recurso.c_str(), NUM_MSJES_PRIO_SOBRECARGA_FILTRADOS);
				uiCountEvSobrecargaPrioA = 0;
			}
		}

		_ordenesA.push(orden);
		delete p_lockA;
	}
	else
	{
		CCSLock* p_lockB = new CCSLock(m_lock_ordenesB);

		if (_ordenesB.size() >= NUM_MAX_ORDENES_ENCOLADAS_RECURSO)
		{
			// "Lee" la orden más antigua ALMACENADA en la lista, y borra la posible memoria dinámica de la media
			RTSPOrder orden_oldest = _ordenesB.front();
			if (orden_oldest.pMedia != NULL)
			{
				free(orden_oldest.pMedia);
				orden_oldest.pMedia = NULL;
			}

			_ordenesB.pop();

			uiCountEvSobrecargaPrioB++;
			if (uiCountEvSobrecargaPrioB >= NUM_MSJES_PRIO_SOBRECARGA_FILTRADOS)
			{
				NLOG_INFO("RTSP Session (%s) SOBRECARGA cola REC_B. Borrados-%d msje cola", _cfgRecB.recurso.c_str(), NUM_MSJES_PRIO_SOBRECARGA_FILTRADOS);
				uiCountEvSobrecargaPrioB = 0;
			}
		}

		_ordenesB.push(orden);

		delete p_lockB;
	}
}

void RTSPSession::CheckSizeBeforePush_Orden(RTSPOrder orden, eRecorder nRec)
{
// 28.01.2016. Impide que nuevas órdenes desborden la lista

	//Se envia a las colas ordenes de los dos grabadores

	if (nRec == eRecA)
	{
		CCSLock* p_lockA = new CCSLock(m_lock_ordenesA);
		if (_ordenesA.size() >= NUM_MAX_ORDENES_ENCOLADAS_RECURSO)
		{
			//Como se va a descartar el NUEVO mensaje, si es de MEDIA, borra la posible memoria dinámica del audio
			if (orden.pMedia != NULL)
			{
				free(orden.pMedia);
				orden.pMedia = NULL;
			}

			uiCountEvSobrecargaMediaA++;
			if (uiCountEvSobrecargaMediaA >= NUM_MSJES_MEDIA_SOBRECARGA_FILTRADOS)
			{
				NLOG_INFO("RTSP Session (%s). SOBRECARGA cola REC_A. Descarta-%d msje MEDIA-PARAM", _cfgRecA.recurso.c_str(), NUM_MSJES_MEDIA_SOBRECARGA_FILTRADOS);
				uiCountEvSobrecargaMediaA = 0;
			}
		}
		else
		{
			_ordenesA.push(orden);
		}
		delete p_lockA;
	}
	else
	{
		CCSLock* p_lockB = new CCSLock(m_lock_ordenesB);
		if (_ordenesB.size() >= NUM_MAX_ORDENES_ENCOLADAS_RECURSO)
		{
			//Como se va a descartar el NUEVO mensaje, si es de MEDIA, borra la posible memoria dinámica del audio
			if (orden.pMedia != NULL)
			{
				free(orden.pMedia);
				orden.pMedia = NULL;
			}

			uiCountEvSobrecargaMediaB++;
			if (uiCountEvSobrecargaMediaB >= NUM_MSJES_MEDIA_SOBRECARGA_FILTRADOS)
			{
				NLOG_INFO("RTSP Session (%s). SOBRECARGA cola REC_B. Descarta-%d msje MEDIA-PARAM", _cfgRecB.recurso.c_str(), NUM_MSJES_MEDIA_SOBRECARGA_FILTRADOS);
				uiCountEvSobrecargaMediaB = 0;
			}
		}
		else
		{
			_ordenesB.push(orden);
		}
		delete p_lockB;
	}
}

bool RTSPSession::SesionGrabacionReady(eRecorder nRec/*= eRecA*/)
{
	int estado= GetEstadoRtsp(nRec);

	if((estado== eRtspSetup)|| (estado== eRtspGrabando))
		return true;
	else
		return false;
}


bool RTSPSession::SendCmdSesion(std::string cmd, eRecorder nRec)
{
	int err;

	RTSPConfig cfgRec= (nRec== eRecB) ? _cfgRecB : _cfgRecA;
	CTCPSocket& sckRec= (nRec== eRecB) ? _sckRecB : _sckRecA;

	// Si se utiliza timeout puede ser que haya una respuesta antigua preparada.
	try
	{
		if (sckRec.IsReadable(20))						
		{
			unsigned char sPapelera[TAM_BUFFER];
			while (sckRec.Recv(sPapelera, TAM_BUFFER) > 0 )
			{
			}
		}
	}
	catch (...)
	{
	}

#ifdef CODE_FOR_TUNNING
//		NLOG_INFO("(%s)-%s. TX mensaje al grabador", cfgRec.recurso.c_str(), (nRec==eRecB)? "REC_B":"REC_A");
#endif

	if ((err= sckRec.Send(cmd.c_str(), cmd.length())) < (int )cmd.length())			
	{
#ifdef _WIN32
		NLOG_ERROR("RTSP Sesion (%s). Error socket.Send_(%s). WSAErr(%d). CreaNuevoSocket", cfgRec.recurso.c_str(), cfgRec.CIPA_Recorder.GetHostName().c_str(), WSAGetLastError());	//Antes "sckRec.GetHostAddress().GetStringAddress().c_str()"
#else
		NLOG_ERROR("RTSP Sesion (%s). Error socket.Send_(%s). errno(%d). CreaNuevoSocket", cfgRec.recurso.c_str(), (nRec== eRecB)? "REC_B":"REC_A", errno);	//ESC. provoca excepción "sckRec.GetHostAddress().GetStringAddress().c_str()"
#endif
		CreaNuevoSocket(nRec);

		return false;
	}

	return true;
}

bool RTSPSession::ProcesaRespuestaRecorder(eRecorder nRec)
{
	unsigned char respuesta [TAM_BUFFER];

	RTSPConfig& cfgRec= (nRec== eRecB) ? _cfgRecB : _cfgRecA;
	CTCPSocket& sckRec= (nRec== eRecB) ? _sckRecB : _sckRecA;
	bool& bTimeout= (nRec== eRecB) ? _bTimeoutRecB :  _bTimeoutRecA;
	bTimeout= false;

	int recibidos = 0;
	try
	{
		//memset(respuesta, 0x0, TAM_BUFFER );
		recibidos = sckRec.Recv(respuesta, TAM_BUFFER);
#ifdef CODE_FOR_TUNNING
//		NLOG_INFO("(%s)-%s. RX respuesta del grabador", cfgRec.recurso.c_str(), (nRec==eRecB)? "REC_B":"REC_A");
#endif
	}
	catch(socket_error err)
	{
		if (err.GetCode() == WSAETIMEDOUT )
		{
			NLOG_ERROR("ED-137. RTSP Sesion (%s). TIMEOUT en espera de respuesta", cfgRec.recurso.c_str() );
			bTimeout = true;
		}
		return false;
	}

	if (recibidos <= 0 )		
	{
#ifdef _WIN32

		//15.10.2018. En caso de activar esta funcionalidad, habría que retransmitir el mensaje RTSP
		int iRet= WSAGetLastError();		//ESC. 15.10.2018. No cierra el socket TCP en caso de no respuesta del grabador
		if(iRet != WSAETIMEDOUT)			//En caso de mantener esta modalidad, considerar retransmisión
#else
		
		//if (errno != EAGAIN && errno != EWOULDBLOCK) Quiero que salga el log

#endif
		{
#ifdef _WIN32
			NLOG_ERROR("ED-137. RTSP Sesion (%s). Error o TimeOut socket.Recv_(%s). WSAErr(%d). ", cfgRec.recurso.c_str(), sckRec.GetHostAddress().GetStringAddress().c_str(), WSAGetLastError());
#else
			NLOG_ERROR("ED-137. RTSP Sesion (%s). Error o TimeOut socket.Recv_(%s). errno(%d). ", cfgRec.recurso.c_str(), (nRec== eRecB)? "REC_B":"REC_A", errno);	//ESC. provoca excepción "sckRec.GetHostAddress().GetStringAddress().c_str()"
#endif
			//CreaNuevoSocket(nRec);
		}

		return false;
	}
	
//	NLOG_DEBUG("ED-137. RTSP Sesion (%s). Recibidos %d bytes", cfgRec.recurso.c_str(), recibidos );
	if ( recibidos > 0 )
	{
		std::string result;
		std::list<std::pair<std::string, std::string> >& headersRec= (nRec== eRecB) ? _headersRecB : _headersRecA;
		headersRec.clear();

		/** */
		respuesta[recibidos < TAM_BUFFER ? recibidos : TAM_BUFFER-1]=0;
		std::string rsp((char *)respuesta);
//		NLOG_DEBUG("[\n%s\n]", respuesta );
		if (RTSPMessages::RspParse(rsp, result, headersRec)==false)
		{
			NLOG_ERROR("ED-137. RTSP Sesion (%s). Error en Formato de Respuesta: <%s>", cfgRec.recurso.c_str(), rsp.c_str());
			return false; //!respOKObl;
		}

#ifdef CODE_FOR_TUNNING
		NLOG_INFO("(%s)-%s. Procesada respuesta del grabador", _cfgRecA.recurso.c_str(), (nRec==eRecB)? "REC_B":"REC_A");
#endif

//		NLOG_DEBUG("ED-137. RTSP Sesion (%s). Resultado=%s", cfgRec.recurso.c_str(), result.c_str() );
		if (result != "200")
		{
			NLOG_ERROR("ED-137. RTSP Sesion (%s) en [%s]. Respuesta Incorrecta <%s>", cfgRec.recurso.c_str(), (nRec==eRecB)?"REC_B":"REC_A", rsp.c_str());
			return false; //!respOKObl;
		}
	}
	return true;
}
//--ESC

void RTSPSession::VaciarCola_Ordenes(eRecorder nRec)
{
	//¡¡¡OJO!!!: La función que llame a este método, debe ejecutar "CCSLock _lock(m_lock_ordenes)"
	CCritSec& m_lock_ordenes = (nRec == eRecA) ? m_lock_ordenesA : m_lock_ordenesB;	
	CCSLock _lock(m_lock_ordenes);

	std::queue<RTSPOrder>& _ordenes = (nRec == eRecA) ? _ordenesA : _ordenesB;

	RTSPOrder ordenTmp;
	while(!_ordenes.empty())
	{
		ordenTmp= _ordenes.front();

		if (ordenTmp.pMedia != NULL)
		{
			free(ordenTmp.pMedia);
			ordenTmp.pMedia = NULL;
		}

		_ordenes.pop();
	}
}

std::string RTSPSession::GetFrecID_GRS()
{
	return _frecID_GRS;
}
void RTSPSession::SetFrecID_GRS(const std::string &sFrecID)
{
	_frecID_GRS= sFrecID;
}


bool RTSPSession::SetSocketBlockingModeON(int iHndlSck, bool bBlocking)
{
	if (iHndlSck < 0) return false;

#ifdef _WIN32
	u_long uMode= bBlocking ? 0 : 1;
	return (ioctlsocket(iHndlSck, FIONBIO, &uMode) == 0) ? true : false;
#else
	int flags = fcntl(iHndlSck, F_GETFL, 0);
	if (flags < 0) return false;
	flags = bBlocking ? (flags&~O_NONBLOCK) : (flags|O_NONBLOCK);
	return (fcntl(iHndlSck, F_SETFL, flags) == 0) ? true : false;
#endif
}


#ifdef _WIN32
void RTSPSession::ConfiguraParamsTCP_KeepAlive_Windows(CTCPSocket &sckRec)		//ESC. 05.06.2017
{
	struct tcp_keepalive alive;
	DWORD	dwBytesRet= 0;

	alive.onoff= TRUE;
	alive.keepalivetime= 5000;		//The keepalivetime member specifies the timeout, in milliseconds, with no activity until the first keep-alive packet is sent
	alive.keepaliveinterval= 400;	//The keepaliveinterval member specifies the interval, in milliseconds, between when successive keep-alive packets are sent if no acknowledgement is received
									//The default setting for number of keep-alive probes is "10", and cannot be changed.
   
	if (WSAIoctl(sckRec.GetHandle(), SIO_KEEPALIVE_VALS, &alive, sizeof(alive),   
            NULL, 0, &dwBytesRet, NULL, NULL) == SOCKET_ERROR)   
	{   
//		printf("WSAIotcl(SIO_KEEPALIVE_VALS) failed; %d\n", WSAGetLastError());
    }
}
#else
void RTSPSession::ConfiguraParamsTCP_KeepAlive_Linux(CTCPSocket &sckRec)
{
	int keepidle = 5;		//The time (in seconds) the connection needs to remain idle before TCP starts sending keepalive probes
	int keepintvl = 1;		//The time (in seconds) between individual keepalive probes
	int keepcnt = 4;		//The maximum number of keepalive probes TCP should send before dropping the connection
	
	sckRec.SetSockOpt (SOL_TCP, TCP_KEEPIDLE, &keepidle, sizeof(int));
	sckRec.SetSockOpt (SOL_TCP, TCP_KEEPINTVL, &keepintvl, sizeof(int));
	sckRec.SetSockOpt (SOL_TCP, TCP_KEEPCNT, &keepcnt, sizeof(int));
}

void RTSPSession::Configura_QoS_Linux(CTCPSocket &sckRec)
{
	int difserv = 0x22<<2;
	sckRec.SetSockOpt (SOL_IP, IP_TOS, &difserv, sizeof(int));
}
#endif


void RTSPSession::Set_last_record_ordenA(RTSPOrder orden)
{
	last_record_ordenA = orden;
}
void RTSPSession::Set_last_record_ordenB(RTSPOrder orden)
{
	last_record_ordenB = orden;
}
RTSPOrder RTSPSession::Get_last_record_ordenA()
{
	return last_record_ordenA;
}
RTSPOrder RTSPSession::Get_last_record_ordenB()
{
	return last_record_ordenB;
}


