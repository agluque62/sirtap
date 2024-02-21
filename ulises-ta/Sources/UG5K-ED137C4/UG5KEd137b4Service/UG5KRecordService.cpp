#include "stdafx.h"

#ifdef _WIN32
	#ifdef _DEBUG
//	#include "vld.h"							//ESC. 23.02.2016. Visual Leak Detector
	#endif

#else
	#undef _WINSERVICE 
#endif

#include "UG5KRecordService.h"
#include "RTSPSession.h"
#include "Utilidades.h"
#include <fstream>								//std::ifstream

std::string CtlSetIPValue = "I00";				// Eventos generales. //V,I00,<ip del puesto o pasarela> Notifica al servicio de grabacion la IP del puesto o pasarela
std::string CmdRecord = "I01";					//V,I01,<grabador>,<Identificador del Recurso Tipo Terminal>,<connref> RECORD
std::string CmdPause = "I02";					//V,I02,<grabador>,<Identificador del Recurso Tipo Terminal>,<connref> PAUSE


std::string CtlEnd = "H00";	
std::string CtlConfig = "H02";					//Se recibe cuando cambian el fichero de configuracion para que el servicio se reinicie. //C,H02	Para el servicio de grabacion			

/** Valores de eventos */        
std::string DeleteObjetoRec = "DDD";			//V,DDD,<Identificador del Recurso Tipo Terminal> Elimina en el servicio de grabacion el objeto de la sesion de grabacion
std::string EvSIP_Invite = "INV";				//VOTER. Acelera conexi�n TCP y Ejecutar�a ANN+SETUP. //V,INV,<grabador>,<Identificador del Recurso Tipo Terminal> Se envia al inicio de la llamada. Necesario cuando el grabador esta en modo VOTER
std::string EvSIP_BYE = "BYE";					//18.01.2017. VOTER, Ejecutar�a TEARDOWN. //V,BYE,<grabador>,<Identificador del Recurso Tipo Terminal> Se envia al fin de la llamada. Necesario cuando el grabador esta en modo VOTER
std::string EvSesionSIP_ON = "I10";				//Pendiente de implementar. Ejecutar�a ANN+SETUP

std::string GrsCreaObjetoRec = "G00";			//V,G00,<grabador>,<Identificador del Recurso Tipo Terminal> Inicia la sesion del Recurso rad (puesto o pasarela) en el grabador
std::string GrsReInitSession = "G01";			//V,G01,<grabador>,<Identificador del Recurso Tipo Terminal> Finaliza la sesion del Recurso rad (puesto o pasarela) en el grabador
std::string GrsSqOn = "G02";					//V,G02,<grabador>,<Identificador del Recurso Tipo Terminal>,<freq>,<connref> SQH on
std::string GrsSqOff = "G03";					//V,G03,<grabador>,<Identificador del Recurso Tipo Terminal>,<freq>,<connref> SQH off
std::string GrsSimulTxOn = "G04";
std::string GrsSimulTxOff = "G05";

std::string TerCreaObjetoRec = "T00";			//V,T00,<grabador>,<Identificador del Recurso Tipo Terminal> Inicia la sesion del Recurso tel (puesto o pasarela) en el grabador
std::string TerReInitSession = "T01";			//V,T01,<grabador>,<Identificador del Recurso Tipo Terminal> Finaliza la sesion del Recurso tel (puesto o pasarela) en el grabador
std::string TerCallStart = "T02";				//V,T02,<grabador>,<Identificador del Recurso Tipo Terminal>,<callType:in,out,other>,<priority>,<orig tel num>,<dest tel num>,<call id header> Call start
std::string TerCallEnd = "T03";					//V,T03,<grabador>,<Identificador del Recurso Tipo Terminal>,<cause>,<origin>,<call id header> Call end
std::string TerCallConnected = "T04";			//V,T04,<grabador>,<Identificador del Recurso Tipo Terminal>,<connected tel num>,<call id header> Call connected
// NO SE UTILIZAN LOS COMANDOS DE TRANSFERENCIA
//std::string TerCallTransferStart = "T05";        // Cuando se inicia la llamada al que se quiere transferir.
//std::string TerCallTransferDisc = "T06";         // Cuando se desconecta el origen...
//std::string TerCallTransferConn = "T07";         // Cuando se conecta el nuevo destino...
std::string TerCallHoldOn = "T08";				//V,T08,<grabador>,<Identificador del Recurso Tipo Terminal>,<orig num>,<call id header> Hold ON
std::string TerCallHoldOff= "T09";				//V,T09,<grabador>,<Identificador del Recurso Tipo Terminal>,<call id header> Hold OFF
												// VCS-Radio...
std::string TerPttOn = "T20";					//V,T20,<grabador>,<Identificador del Recurso Tipo Terminal>,<freq>,<ptt type>,<connref> PTT ON
std::string TerPttOff= "T21";					//V,T21,<grabador>,<Identificador del Recurso Tipo Terminal>,<freq>,<connref> PTT OFF


/** Estructura de MEDIA: MMM#RECURSO#media */
std::string RecMedia = "MMM";					//V,<grabador>,MMM,<Identificador del Recurso Tipo Terminal>,<num secuencia>,<muestras de audio> Mesaje de media

//IMPORTANTE: <grabador> Puede ser: "#0" graba en los dos, "#1" graba en el primero y "#2" graba en el segundo. 
//Si este campo no esta, entonces se considera "#0". Es decir, que grabaría en los dos


//ESC. 17.04.2017. Mensaje hacia Nucleo para informar sobre arranque M�dulo Grabaci�n.
char sMsjeStartModRecToNucleo[]= "G,T11";


/** */
UG5KRecordService::UG5KRecordService(void)
{
}

/** */
UG5KRecordService::~UG5KRecordService(void)
{
}

#define TAM_BUFFER			1024 

/** */
void UG5KRecordService::Run()
{
	unsigned char buffer[TAM_BUFFER];
	unsigned char *data = &buffer[64];		// Se deja espacio para a�adir por delante las cabeceras.						
	int lmax = TAM_BUFFER-64;
	CIPAddress IPA_msgFrom, IPA_NucleoGW("127.0.0.1", 65001);

	Init();

	NLOG_INFO("UG5K ED-137b4-Service. Atendiendo en el puerto %d", _portRx );

	//ESC. 17.04.2017. Mensaje hacia Nucleo para informar sobre arranque M�dulo Grabaci�n.
	_sckRxUDP.SendTo(sMsjeStartModRecToNucleo, strlen(sMsjeStartModRecToNucleo), IPA_NucleoGW);

	while(IsRunning() && ! _finalizarServ)
	{
		try
		{
			int recibidos = _sckRxUDP.RecvFrom(data, lmax, &IPA_msgFrom);	//Funci�n BLOQUEANTE hasta que se reciben datos

			if (recibidos > 0 && recibidos < lmax)
			{
				data[recibidos < (lmax-1) ? recibidos : (lmax-1)]=0;	// para asegurar un string...
				int estSesRecA, estSesRecB;
				
				if(_ipPasarela.empty())
				{
					_ipPasarela= IPA_msgFrom.GetStringAddress().c_str();
				}

				int res = Interprete(recibidos, data, estSesRecA, estSesRecB);

				if (res < CODIGO_MAX_ERROR)
				{
					if(IPA_msgFrom.GetPort() > 0)
					{
						EnviarRespuestaCliente(res, IPA_msgFrom, estSesRecA, estSesRecB);	//ESC. 14.12.2015. Responde al origen
//#ifdef CODE_FOR_TUNNING
						if(_bDualRecSrv)
						{
							NLOG_INFO("<<< Respuesta OK. REC_A='%s' Rec_B='%s'",
										RTSPSession::GetTxtEstadoRtsp(estSesRecA).c_str(),
										RTSPSession::GetTxtEstadoRtsp(estSesRecB).c_str());
						}
						else
						{
							NLOG_INFO("<<< Respuesta OK. REC_A='%s'",
										RTSPSession::GetTxtEstadoRtsp(estSesRecA).c_str());
						}
//#endif
					}
/*
//ESC. Comentado, la respuesta S�LO se enviar� a la IP@Port origen
					else if (_IPrespuesta.GetPort() > 0)
					{
						EnviarRespuestaCliente(res, _IPrespuesta, estSesRecA, estSesRecB);	//ESC. 14.12.2015. Responde a la IP del fichero .ini
					}
*/
				}
			}
		}
		catch(socket_error error)
		{
			NLOG_ERROR("Excepcion socket_error en UG5KRecordService::Run: (%s)", error.what());
		}
		catch(exception &e)				//ESC. 02.12.2015 A�adida referencia "&" en standard exception
		{
			NLOG_ERROR("Excepcion en UG5KRecordService::Run: (%s).", e.what());
		}
	}

	NLOG_INFO("ED-137. UG5KRecordService::Run() Finalizado" );
	if ( _finalizarServ )
	{
		AvisarFinSesiones();
		Stop();
	}
}

/** */
void UG5KRecordService::Dispose()
{
	try
	{
		_ipPasarela.clear();
		_uri.clear();
		StopThreads_sesiones();

//		Stop();									//ESC. 07.12.2015. Movido al final

		_finalizarServ = true;					//ESC. 07.12.2015. Prepara retorno funcion Run()
		_sckRxUDP.Close();						//ESC. Provocar�a retorno "RecvFrom" en Run(), al cerrar socket
//		_sckTxUDP.Close();

		sleep(1000);							//ESC. 07.12.2015. Permite a Run() ejecutar AvisarFinSesiones()
		_sesiones.clear();

//		Stop();									//ESC. 07.12.2015
	}
	catch(...)
	{
		NLOG_ERROR("Excepcion. UG5KRecordService::Dispose()");
	}
}

/** 
* Carga la configuracion del servidor e inicializa el socket de escucha...
*/
void UG5KRecordService::Init()
{
	try
	{
		LocalConfig _cfg;

		_finalizarServ = false;
		_portRx = _cfg.ServiceUdpPort();
		_maxcanales = _cfg.MaxSessions();
		_CIP_RecA = CIPAddress(_cfg.RTSPIp_RecA(), _cfg.RTSPPortA());
		_uri = _cfg.RTSPUri_frmt();
		_ThreadsRecursos = 0;
		_ipPasarela = "";						//ESC. 05.01.2017. Antes "_cfg.ServiceIpDefault()"
		_sckRxUDP.Bind(_portRx);
		_bDualRecSrv = _cfg.GrabadorDual();		//ESC. 27.01.2016. A�adido
		if(_bDualRecSrv)
			_CIP_RecB = CIPAddress(_cfg.RTSPIp_RecB(), _cfg.RTSPPortB());	//ESC. 06.06.2017. A�adido

		_EnableGrabacionED137 = _cfg.GetEnableGrabacionED137();

		_bModoVOTERSrv= _cfg.ModoVOTER();
//		_IPrespuesta = CIPAddress(_cfg.ServiceUdpResultIp(), _cfg.ServiceUdpResultPort());
	}
	catch(socket_error error)
	{
		NLOG_ERROR("Excepcion. UG5KRecordService::Init, socket_error: (%d)", error.GetCode());
		exit(0);
	}
	catch(exception &e)							//ESC. 02.12.2015 A�adida referencia "&"  en standard exception
	{
		NLOG_ERROR("Excepcion. UG5KRecordService::Init,  info: %s.", e.what());
	}
}

/** */

int UG5KRecordService::Interprete(int recibidos, void *pdata, int &estSesRecA, int &estSesRecB)
{
	estSesRecA= estSesRecB= eRtspSckDisc;		//ESC. Por defecto retornar� Sesi�n Desconectada. Considerar estado "No Config"

	std::string comando ((const char *)pdata);
	/* El primer par�metro es una letra que identifica al m�dulo emisor:*/
	std::string origmsje;
	pdata = (void *) Utilidades::parseNextField((char *) pdata, &recibidos, origmsje);

	/** id del evento*/
	std::string evento;		// = valores.at(1);
	pdata = (void*)Utilidades::parseNextField((char*)pdata, &recibidos, evento);

	//Buscamos el campo grabador
	if (recibidos >= 3)
	{
		char* datos = (char*)pdata;
		if (datos[0] == '#' && datos[2] == '#')
		{
			if (datos[1] >= 0 && datos[1] <= '9')
			{
				if (recibidos == 3 || (recibidos > 3 && datos[3] == ','))
				{
					//El mensaje incluye el campo del grabador al que va dirigido
					//Lo ignoramos de momento
					std::string grabador;
					pdata = (void*)Utilidades::parseNextField((char*)pdata, &recibidos, grabador);
				}
			}
		}
	}

	/** id del Recurso */
	std::string id;			// = valores.at(2);
	pdata = (void *) Utilidades::parseNextField((char *) pdata, &recibidos, id);		

	/** marca de la lista de sesiones */
	std::map<std::string, void*>::iterator it = _sesiones.find(id);

	/** Busco la sesion */
	bool bCreadaNewSesion = false;
	RTSPSession* pRtspSes = it != _sesiones.end() ? (RTSPSession*)it->second : NULL;

	/* Eventos especiales: */	

	if (evento == CtlConfig || evento == DeleteObjetoRec || evento == GrsCreaObjetoRec || 
		evento == GrsReInitSession || evento == TerCreaObjetoRec || evento == TerReInitSession)
	{
		//Con cualquiera de estos eventos comprobamos si ha habido cambios en el fichero de configuracion

		if (evento == CtlConfig)
		{
			NLOG_INFO("ED-137 Service. Recibido msje %s: 'ug5krec-config.ini' actualizado. Reinicia el servicio", CtlConfig.c_str());
			_finalizarServ = true;
			return TX_CLIENT_NO_ERROR;
		}
		bool config_file_changed = ValidarConfiguracion();
		if (config_file_changed || _EnableGrabacionED137 == false) return TX_CLIENT_NO_ERROR;
	}

	if (evento == CtlSetIPValue)
	{
		NLOG_INFO(">>> ED-137 Service. RX= '%s'", comando.c_str());

		// BORRAR. 10.02.2017. C�digo para ser compatible con CoreSIP de 14/06/2016
		std::string sNextField;		// = valores.at(3);
		pdata = (void*)Utilidades::parseNextField((char*)pdata, &recibidos, sNextField);
		if (!sNextField.empty())
		{
			if (sNextField.length() < 7)
				return TX_CLIENT_NO_CFG_IPCGW;

			_ipPasarela = sNextField;			//ESC. 05.01.2017. Si se recibe este comando, actualizamos la variable.
			return TX_CLIENT_NO_ERROR;
		}
		//-- BORRAR. 10.02.2017. C�digo para ser compatible con CoreSIP de 14/06/2016
		else
		{
			//ESC. 19.01.2017. Para este comando, el segundo par�metro es la direcci�n IP (y no el ID_recurso)
			if (id.empty() || (id.length() < 7))
				return TX_CLIENT_NO_CFG_IPCGW;

			_ipPasarela = id;			//ESC. 05.01.2017. Si se recibe este comando, actualizamos la variable.
			return TX_CLIENT_NO_ERROR;
		}
	}

	if (evento == CtlEnd)
	{
		NLOG_INFO ("ED-137 Service. Recibido msje de finalizacion" );
		_finalizarServ = true;
		return TX_CLIENT_NO_ERROR;
	}

	bool bEvAudio = (evento == RecMedia);
	if (!bEvAudio)
	{
		//NLOG_INFO( ">>> ED-137 Service. RX= '%s',(%s)", evento.c_str(), id.c_str());
		NLOG_INFO( ">>> ED-137 Service. RX= '%s'", comando.c_str());					//ESC. 14.11.2016. Presenta todo el mensaje
	}

	bool bEvStartSesion= false;
	if (evento == GrsCreaObjetoRec || evento == TerCreaObjetoRec )
	{
		if ( _ipPasarela.length() < 7 )		//ESC. 18.07.2016. La URI origen se obtiene de [DirIP del paquete]@[ID Recurso]
											//ESC. 05.01.2017. La direcci�n IP se emplea para generar "connref"
		{
			NLOG_ERROR ("ED-137 Service. IP del modulo no especificada o erronea." );
			return TX_CLIENT_NO_CFG_IPCGW;
		}
		bEvStartSesion = true;
	}

	/** marca de la lista de sesiones */
	//std::map<std::string, void *>::iterator it = _sesiones.find(id);

	/** Busco la sesion */
	//bool bCreadaNewSesion = false;
	//RTSPSession *pRtspSes = it != _sesiones.end() ? (RTSPSession *)it->second : NULL;
	if (pRtspSes == NULL && _EnableGrabacionED137)
	{
		if (bEvStartSesion)
		{
			if ( _ThreadsRecursos >= _maxcanales )
			{
				NLOG_ERROR ("ED-137 Service. Maximo numero de Sesiones/Threads alcanzado." );
				return TX_CLIENT_NUM_MAX_SESIONES;
			}

#ifdef _WIN32			//ESC. 10.02.2017. En el entorno de windows, los UA son VCS-Rad y VCS-Tel
			pRtspSes = new RTSPSession(id, evento == GrsCreaObjetoRec ? eUA_VcsRad : eUA_VcsTel, this);
#else
			pRtspSes = new RTSPSession(id, evento == GrsCreaObjetoRec ? eUA_GRS : eUA_VcsTel, this);
#endif
			
			if(pRtspSes==NULL)
			{
				NLOG_ERROR ("ED-137 Service. Error al crear objeto RTSPSession de '%s'.", id.c_str() );
				return TX_CLIENT_ERR_CREATE_SESION;
			}

//ESC. 03.02.2016. El thread de cada sesi�n-recurso intenta llevar autom�ticamente al estado "eRtspSetup" cada sesi�n
			pRtspSes->SetIPPasarela(_ipPasarela);			//ESC. 09.01.2017. Este campo se emplea para generar "connref". Ver 2.8.1 ED-137_4B

			pRtspSes->Start();									//Ejecuta la funci�n "RTSPSession::Run()"

			_sesiones.insert(std::pair<std::string, void *>(id, (void *)pRtspSes));
			_ThreadsRecursos = _ThreadsRecursos + 1;		//ESC. 03.02.2016. A�adido

			bCreadaNewSesion = true;
		}
		else
		{
			NLOG_ERROR ("ED-137 Service. NO existe Sesion del recurso (%s).", id.c_str() );
			return TX_CLIENT_NO_EXISTE_SESION;
		}
	}

	int res = TX_CLIENT_NO_ERROR;			//ESC. 14.12.2015. Por defecto, resultado "No hay error"

	if (pRtspSes != NULL && _EnableGrabacionED137)
	{

#ifdef TEARDOWN_NO_CONECTA_TCP
		//ESC. 15.11.2016. Si recibimos comandos en estado TearDown, intentar� la conexi�n TCP
		if(pRtspSes->GetEstadoRtsp(eRecA) == eRtspTearDown)
			pRtspSes->SetEstadoRtsp(eRtspSckDisc, eRecA);

		if(_bDualRecSrv)
		{
			if(pRtspSes->GetEstadoRtsp(eRecB) == eRtspTearDown)
				pRtspSes->SetEstadoRtsp(eRtspSckDisc, eRecB);
		}
#endif

		if (bEvAudio)
		{
			res = CODIGO_MAX_ERROR;			// Evita NLOG respuesta Media hacia m�dulo CoreSIP/Nucleo
			std::string paquete;			// = valores.at(3);
			pdata = (void *) Utilidades::parseNextField((char *) pdata, &recibidos, paquete);

			/** Inserta evento de Media */

			pRtspSes->PushframeMedia(paquete, recibidos, pdata);
		}
		else if (bEvStartSesion)
		{
			if( ! bCreadaNewSesion)
			{
				NLOG_INFO ("ED-137 Service. YA EXISTE RTSPSession de '%s'.", id.c_str() );
				return TX_CLIENT_NO_ERROR;
			}
		}
		else if (evento == DeleteObjetoRec)
		{
			pRtspSes->PushmsjeTearDown();
			sleep(20);

			pRtspSes->Dispose();
			pRtspSes->SignalFinalizacion();
			sleep(20);						//Deja tiempo para que salga de la funci�n Run() de RTSPSesion
			pRtspSes->Stop();

			_sesiones.erase(id);
			if ( _ThreadsRecursos > 0)
				_ThreadsRecursos--;
		}
		else if (evento == GrsReInitSession || evento==TerReInitSession)
		{
			/** Generar evento TEARDOWN... */
			NLOG_INFO ("ED-137 Service. RX cmd TEARDOWN recurso (%s). Threads=%d", id.c_str(), _ThreadsRecursos );
			pRtspSes->PushmsjeTearDown();		//ESC. 03.02.2016. "ProcessTearDown" no destruye el thread ni el socket
		}
		//ESC. 18.01.2016. A�adido comando RECORD
		//El cliente DEBE enviar este comando para INICIAR la grabaci�n. NO INCLUYE operaciones.
		//Previamente el cliente deber�a haber enviado mensajes-operaciones de PTT/SQU a ON, para almacenar sus metadata.
		else if (evento == CmdRecord)
		{	
			std::string connref = id;
			pdata = (void*)Utilidades::parseNextField((char*)pdata, &recibidos, connref);
			connref += "@" + _ipPasarela;

			if( GetModoVOTER() &&
				(pRtspSes->GetTipoUA() == eUA_VcsTel))		//ESC. 26.01.2017
			{
				//26.01.2017. En modo VOTER y VCS_Telephony, el msje RECORD se env�a en "CallConnected"
				//			  por lo que se descarta el comando expl�cito de Record
			}
			else
			{
				//Si ya se ha enviado m�s eventos del tipo record entonces no enviamos este
				/** Genera evento RECORD */
				rtsp_props operaciones;
				//operaciones.push_back(rtsp_prop("SQU","1"));	//ESC. 18.01.2016. NO inserta operaciones en la lista
				RTSPOrder orden = pRtspSes->PushmsjeRecord_onlyOper(operaciones, connref);
				pRtspSes->Set_last_record_ordenA(orden);
				pRtspSes->Set_last_record_ordenB(orden);
			}
		}
		//--ESC
		//ESC. 18.01.2016. A�adido comando PAUSE
		//El cliente DEBE enviar este comando para DETENER la grabaci�n. NO INCLUYE operaciones.
		//Previamente el cliente deber�a haber enviado mensajes-operaciones de PTT/SQU a OFF, para almacenar sus metadata.
		else if (evento == CmdPause)
		{	
			//Solo mandamos el pause cuando es el ultimo
			std::string connref = id;
			pdata = (void*)Utilidades::parseNextField((char*)pdata, &recibidos, connref);
			connref += "@" + _ipPasarela;

			/** Genera evento PAUSE */
			rtsp_props operaciones;
			//operaciones.push_back(rtsp_prop("SQU","0"));	//ESC. 18.01.2016. NO inserta operaciones en la lista
			RTSPOrder orden = pRtspSes->PushmsjePause_onlyOper(operaciones, connref);
			pRtspSes->Set_last_record_ordenA(orden);
			pRtspSes->Set_last_record_ordenB(orden);
		}

		//ESC. 26.07.2016. Nuevo mensaje para sincronizaci�n con VOTER en GRS (y VCS-Rad/VCS-Tel)
		else if (evento == EvSIP_Invite)
		{
			if(GetModoVOTER())
			{
				if( (pRtspSes->GetEstadoRtsp(eRecA)==eRtspSckDisc)||	//S�lo chequea "REC_A"
					(pRtspSes->GetEstadoRtsp(eRecA)==eRtspCnxSck) )		//10.11.2016. Ya que VOTER acepta la conexi�n, permite enviar comandos ANN+SETUP
				{
					pRtspSes->PushmsjeSIP_INVITE();
				}
			}
		}
		
		//ESC. 18.01.2017. Nuevo mensaje para sincronizaci�n con VOTER en GRS (y VCS-Rad/VCS-Tel)
		else if (evento == EvSIP_BYE)
		{
			if(GetModoVOTER())
			{
				if( (pRtspSes->GetEstadoRtsp(eRecA)==eRtspSetup)||		//S�lo chequea "REC_A"
					(pRtspSes->GetEstadoRtsp(eRecA)==eRtspGrabando) )
				{
					pRtspSes->PushmsjeSIP_BYE();
				}
			}
		}

		else if (evento == GrsSqOn)
		{
			//ESC. 19.01.2016. A�ade propiedades para incluir en el mensaje hacia el grabador
			rtsp_props propiedades;

			std::string sFrecRad;
			pdata = (void *) Utilidades::parseNextField((char *) pdata, &recibidos, sFrecRad);

			if(sFrecRad.empty())
				return TX_CLIENT_CMD_NOTFOUND;

			std::string connref = id;
			pdata = (void*)Utilidades::parseNextField((char*)pdata, &recibidos, connref);
			connref += "@" + _ipPasarela;

			propiedades.push_back(rtsp_prop("FrequencyID", sFrecRad));
			propiedades.push_back(rtsp_prop("Direction", "1"));

			rtsp_props operaciones;
			operaciones.push_back(rtsp_prop("SQU", "1"));
			//--ESC

#ifdef _WIN32
			//ESC. 02.06.2016. Aplicable para m�dulo "VCS_Radio_UA", que se ejecuta en WINDOWS
			/** Genera evento SET_PARAM */

			pRtspSes->PushmsjeParam(propiedades, operaciones, connref);
#else
			//ESC. 02.06.2016. Aplicable para m�dulo "GRS Radio", que se ejecuta en LINUX
			/** SQU-ON provoca que el m�dulo env�e el m�todo RECORD*/

			RTSPOrder orden = pRtspSes->PushmsjeRecord(propiedades, operaciones, connref);
			pRtspSes->Set_last_record_ordenA(orden);
			pRtspSes->Set_last_record_ordenB(orden);
#endif
		}

		else if (evento == GrsSqOff)
		{
			rtsp_props propiedades;
			std::string sFrecRad;
			pdata = (void *) Utilidades::parseNextField((char *) pdata, &recibidos, sFrecRad);

			if(sFrecRad.empty())
				return TX_CLIENT_CMD_NOTFOUND;

			std::string connref = id;
			pdata = (void*)Utilidades::parseNextField((char*)pdata, &recibidos, connref);
			connref += "@" + _ipPasarela;

			propiedades.push_back(rtsp_prop("FrequencyID", sFrecRad));

			rtsp_props operaciones;
			operaciones.push_back(rtsp_prop("SQU", "0"));
			
#ifdef _WIN32
			//ESC. 02.06.2016. Aplicable para m�dulo "VCS_Radio_UA", que se ejecuta en WINDOWS
			/** Genera evento SET_PARAM */

			pRtspSes->PushmsjeParam(propiedades, operaciones, connref);
#else
			//ESC. 02.06.2016. Aplicable para m�dulo "GRS Radio", que se ejecuta en LINUX
			/** SQU-ON provoca que el m�dulo env�e el m�todo PAUSE*/
			RTSPOrder orden = pRtspSes->PushmsjePause(propiedades, operaciones, connref);
			pRtspSes->Set_last_record_ordenA(orden);
			pRtspSes->Set_last_record_ordenB(orden);
#endif
		}

		else if (evento == TerPttOn)
		{
			/** Generar evento SET_PARAM */				//ESC. 18.01.2016. Antes generaba evento RECORD
			rtsp_props operaciones;

			//ESC. 19.01.2016. A�ade propiedades para incluir en el mensaje hacia el grabador
			rtsp_props propiedades;
			std::string sFrecRad, sTipoPTT;
			pdata = (void *) Utilidades::parseNextField((char *) pdata, &recibidos, sFrecRad);
			
			if(sFrecRad.empty())
				return TX_CLIENT_CMD_NOTFOUND;

			propiedades.push_back(rtsp_prop("FrequencyID", sFrecRad));
			propiedades.push_back(rtsp_prop("Direction", "2"));

			//29.06.2016
			pdata = (void *) Utilidades::parseNextField((char *) pdata, &recibidos, sTipoPTT);

			std::string connref = id;
			pdata = (void*)Utilidades::parseNextField((char*)pdata, &recibidos, connref);
			connref += "@" + _ipPasarela;

			if(sTipoPTT.empty())
				operaciones.push_back(rtsp_prop("PTT", "1"));		//29.06.2016; por defecto PTT=normal
			else
				operaciones.push_back(rtsp_prop("PTT", sTipoPTT));	//29.06.2016;
		
			pRtspSes->PushmsjeParam(propiedades, operaciones, connref);
		}
		else if (evento == TerPttOff)
		{
			/** Generar evento SET_PARAM */				//ESC. 18.01.2016. Antes generaba evento PAUSE
			rtsp_props operaciones;

			//ESC. 19.01.2016. A�ade propiedades para incluir en el mensaje hacia el grabador
			rtsp_props propiedades;
			std::string sFrecRad;
			pdata = (void *) Utilidades::parseNextField((char *) pdata, &recibidos, sFrecRad);			

			if(sFrecRad.empty())
				return TX_CLIENT_CMD_NOTFOUND;

			std::string connref = id;
			pdata = (void*)Utilidades::parseNextField((char*)pdata, &recibidos, connref);
			connref += "@" + _ipPasarela;

			propiedades.push_back(rtsp_prop("FrequencyID", sFrecRad));

			operaciones.push_back(rtsp_prop("PTT", "0"));

			pRtspSes->PushmsjeParam(propiedades, operaciones, connref);
		}
		else if (evento == GrsSimulTxOn)
		{
			/** Generar evento SET_PARAM */
			/*rtsp_props operaciones;
			operaciones.push_back(rtsp_prop("Simultaneous Transmission", "1"));
			pRtspSes->PushmsjeParam_onlyOper(operaciones);*/
		}
		else if (evento == GrsSimulTxOff)
		{
			/** Generar evento SET_PARAM */
			/*rtsp_props operaciones;
			operaciones.push_back(rtsp_prop("Simultaneous Transmission", "0"));
			pRtspSes->PushmsjeParam_onlyOper(operaciones);*/
		}
		else if (evento == TerCallStart)
		{
			/** Genera mensaje SET_PARAM */
			rtsp_props propiedades;
/*
			//ESC. 26.01.2017. Pendiente de a�adir este campo en el comando
			std::string valor1= "XXXXXXXX-11111111@" + _ipPasarela;
			pdata = (void *) Utilidades::parseNextField((char *) pdata, &recibidos, valor1);
			propiedades.push_back(rtsp_prop("CallRef", valor1));
*/
			std::string valor = "2";
			pdata = (void *) Utilidades::parseNextField((char *) pdata, &recibidos, valor);
			propiedades.push_back(rtsp_prop("Direction", valor));
			
			valor = "3";
			pdata = (void *) Utilidades::parseNextField((char *) pdata, &recibidos, valor);
			propiedades.push_back(rtsp_prop("Priority", valor));

			valor = "0";
			pdata = (void *) Utilidades::parseNextField((char *) pdata, &recibidos, valor);
			propiedades.push_back(rtsp_prop("CallingNr", valor));

			valor = "0";
			pdata = (void *) Utilidades::parseNextField((char *) pdata, &recibidos, valor);
			propiedades.push_back(rtsp_prop("CalledNr", valor));

			std::string connref = id;
			pdata = (void*)Utilidades::parseNextField((char*)pdata, &recibidos, connref);
			connref += "@" + _ipPasarela;

			propiedades.push_back(rtsp_prop("SetupTime", RTSPMessages::UTCTimestamp()));
			pRtspSes->PushmsjeParam_onlyProp(propiedades, connref);	//26.01.2017. Son propiedades (no operaciones)
		}
		else if (evento == TerCallConnected)
		{
			rtsp_props propiedades;
/*
			//ESC. 26.01.2017. Pendiente de a�adir este campo en el comando
			std::string valor1= "XXXXXXXX-11111111@" + _ipPasarela;
			pdata = (void *) Utilidades::parseNextField((char *) pdata, &recibidos, valor1);
			propiedades.push_back(rtsp_prop("CallRef", valor1));
*/

			std::string valor = "0";
			pdata = (void *) Utilidades::parseNextField((char *) pdata, &recibidos, valor);
			propiedades.push_back(rtsp_prop("ConnectedNr", valor));

			std::string connref = id;
			pdata = (void*)Utilidades::parseNextField((char*)pdata, &recibidos, connref);
			connref += "@" + _ipPasarela;

			propiedades.push_back(rtsp_prop("ConnectTime", RTSPMessages::UTCTimestamp()));

			if(GetModoVOTER())		//ESC. 26.01.2017
			{
				/** Genera mensaje RECORD con propiedades */
				pRtspSes->PushmsjeRecord_onlyProp(propiedades, connref);
			}
			else
			{
				/** Genera mensaje SET_PARAM */
				pRtspSes->PushmsjeParam_onlyProp(propiedades, connref);	//26.01.2017. Son propiedades (no operaciones)
			}
		}
		else if (evento == TerCallEnd)
		{
			//ESC. 26.01.2017. Seg�n "2.8.3 ED-137_4B" se requiere un posterior msje de PAUSE � TEARDOWN
			rtsp_props propiedades;
/*
			//ESC. 26.01.2017. Pendiente de a�adir este campo en el comando
			std::string valor1= "XXXXXXXX-11111111@" + _ipPasarela;
			pdata = (void *) Utilidades::parseNextField((char *) pdata, &recibidos, valor1);
			propiedades.push_back(rtsp_prop("CallRef", valor1));
*/

			propiedades.push_back(rtsp_prop("DisconnectTime",RTSPMessages::UTCTimestamp()));

			std::string valor = "16";
			pdata = (void *) Utilidades::parseNextField((char *) pdata, &recibidos, valor);
			propiedades.push_back(rtsp_prop("DisconnectCause", valor));

			valor = "1";
			pdata = (void *) Utilidades::parseNextField((char *) pdata, &recibidos, valor);
			propiedades.push_back(rtsp_prop("DisconnectSource", valor));

			std::string connref = id;
			pdata = (void*)Utilidades::parseNextField((char*)pdata, &recibidos, connref);
			connref += "@" + _ipPasarela;

			/*
			if(GetModoVOTER())		//ESC. 26.01.2017
			{
				// Auto-Genera mensaje BYE con propiedades
				pRtspSes->PushmsjeSIP_BYE(propiedades);		//Implica implementar TEARDOWN un XML body
			}
			else
			{
				//Genera mensaje SET_PARAM
			}
			*/

			/** Genera mensaje SET_PARAM */
			pRtspSes->PushmsjeParam_onlyProp(propiedades, connref);	//26.01.2017. Son propiedades (no operaciones)
		}
		else if (evento == TerCallHoldOn)
		{
			std::string sOrigHold;
			pdata = (void *) Utilidades::parseNextField((char *) pdata, &recibidos, sOrigHold);

			std::string connref = id;
			pdata = (void*)Utilidades::parseNextField((char*)pdata, &recibidos, connref);
			connref += "@" + _ipPasarela;

			rtsp_props operaciones;
			operaciones.push_back(rtsp_prop("HOLD", sOrigHold.empty()? "1": sOrigHold));

			pRtspSes->PushmsjeParam_onlyOper(operaciones, connref);
		}
		else if (evento == TerCallHoldOff)
		{
			std::string connref = id;
			pdata = (void*)Utilidades::parseNextField((char*)pdata, &recibidos, connref);
			connref += "@" + _ipPasarela;

			rtsp_props operaciones;
			operaciones.push_back(rtsp_prop("HOLD", "0"));

			pRtspSes->PushmsjeParam_onlyOper(operaciones, connref);
		}
		else
		{
			NLOG_ERROR("ED-137 Service. Comando NO soportado: %s", evento.c_str());
			res = TX_CLIENT_CMD_NOTFOUND;
		}

		//ESC. 05.02.2016. Dado que existe un thread de grabaci�n del recurso, actualiza el estado de la sesi�n
		estSesRecA= pRtspSes->GetEstadoRtsp(eRecA);
		if(_bDualRecSrv)
			estSesRecB= pRtspSes->GetEstadoRtsp(eRecB);
		//--ESC
	}
	else
	{
		NLOG_ERROR("ED-137 Service. NO Existe Sesion del recurso (%s)", evento.c_str());
		res = TX_CLIENT_NO_EXISTE_SESION;
	}

	return res;
}

void UG5KRecordService::EnviarRespuestaCliente(int valor, CIPAddress ip, int estSesRecA, int estSesRecB)
{
	char respuesta[32];
//	memset(respuesta, 0x0, sizeof(respuesta));

	sprintf( respuesta, "G,E00,%d", valor );
//	sprintf( respuesta, "G,E00,%d,%d,%d", valor, estSesRecA, estSesRecB );

	_sckRxUDP.SendTo(respuesta, strlen(respuesta), ip);

	if(valor != TX_CLIENT_NO_ERROR)
	{
		NLOG_ERROR("ED-137 Service. Respuesta [%s] a [%s:%d]. REC_A='%s' Rec_B='%s'",
					respuesta,
					ip.GetStringAddress().c_str(), ip.GetPort(),
					RTSPSession::GetTxtEstadoRtsp(estSesRecA).c_str(),
					RTSPSession::GetTxtEstadoRtsp(estSesRecB).c_str() );
	}
}

void UG5KRecordService::AvisarFinSesiones()
{
	std::map<std::string, void *>::iterator it;
	for (it= _sesiones.begin(); it != _sesiones.end(); it++)
	{
		RTSPSession *ses = (RTSPSession *)it->second;
		ses->SignalFinalizacion();
	}
}

void UG5KRecordService::StopThreads_sesiones()
{
	std::map<std::string, void *>::iterator it;
	for (it= _sesiones.begin(); it != _sesiones.end(); it++)
	{
		RTSPSession *ses = (RTSPSession *)it->second;
#ifndef _WIN32
		ses->SignalFinalizacion();		//ESC. 23.05.2017. Dado que "ses->Stop()" espera un evento, activa variable de salida
#endif
		ses->Stop();
		//ses->Dispose();
	}
}

bool UG5KRecordService::ValidarConfiguracion()
{
	bool config_file_changed = false;
	try
	{
		LocalConfig _cfg;

		if (( _portRx != _cfg.ServiceUdpPort() ) ||
			( _maxcanales != _cfg.MaxSessions() ) ||
			( _CIP_RecA.GetStringAddress() != _cfg.RTSPIp_RecA() ) ||
			( _bDualRecSrv != _cfg.GrabadorDual() )||									//ESC. 06.06.2017. A�adido
			( _bDualRecSrv && (_CIP_RecB.GetStringAddress() != _cfg.RTSPIp_RecB()) ) ||	//ESC. 06.06.2017. A�adido
			( (int) _CIP_RecA.GetPort() != _cfg.RTSPPortA() ) ||
			( _bDualRecSrv && ((int) _CIP_RecB.GetPort() != _cfg.RTSPPortB()) ) ||
			( _EnableGrabacionED137 != _cfg.GetEnableGrabacionED137()))
//			( _uri != _cfg.RTSPUri_frmt() )								//ESC. 08.01.2016. Comentado. El formato de la URI no es editable.
		{			
			config_file_changed = true;
			_finalizarServ = true;
			NLOG_INFO("ED-137 Service. Cambio en fichero de Configuracion: el servicio va a finalizar.");
			AvisarFinSesiones();
		}
		_EnableGrabacionED137 = _cfg.GetEnableGrabacionED137();
	}
	catch(...)
	{
		NLOG_ERROR("ED-137 Service. UG5KRecordService::ValidarConfiguracion. Excepcion.");
		_EnableGrabacionED137 = false;
	}
	return config_file_changed;
}


//ESC. 27.01.2016
bool UG5KRecordService::GetDualRecordCfg()
{
	return _bDualRecSrv;
}
//-ESC
bool UG5KRecordService::GetModoVOTER()
{
	return _bModoVOTERSrv;
}
//

bool UG5KRecordService::CPU_Pasarela_Es_Principal()
{
	string filepath = "/mnt/ramfs/cpumode";
	string isMain = "PRINCIPAL";

	std::ifstream ff(filepath.c_str());

	if (ff.good())
	{
		string estado;

		ff >> estado;
		if (estado==isMain)
			return true;
		return false;
	}
	return true;
}

int UG5KRecordService::GetNumItfsConfigForRecording()
{
	return _ThreadsRecursos;
}

/*
bool UG5KRecordService::GetModoVOTER_VCStelephonyUA(RTSPSession &pRtspSes)
{
	if( _bModoVOTERSrv && 
		(pRtspSes.GetTipoUA == eUA_VcsTel))
		return true;
	else
		return false;
}
*/

