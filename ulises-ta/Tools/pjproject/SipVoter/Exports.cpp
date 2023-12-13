/**
 * @file Exports.cpp
 * @brief Interfaz de usuario a CORESIP.dll
 *
 *	Implementacion de las rutinas de interfaz con la DLL
 *
 *	@addtogroup CORESIP
 */
/*@{*/
#include "Global.h"
#include "Exceptions.h"
#include "SipAgent.h"
#include "SipCall.h"
#include "OptionsMod.h"
#include "AccountUserData.h"
#include "WG67subs.h"
#include "WavPlayerToRemote.h"
#include "SoundDevHw.h"

#define Try\
	pj_thread_desc desc;\
	pj_thread_t * th = NULL;\
	if (!pj_thread_is_registered())\
	{\
		pj_thread_register(NULL, desc, &th);\
	}\
	try

#define catch_all\
	catch (PJLibException & ex)\
	{\
		ret = ex.SetError(error);\
		if (SipAgent::ESTADO_INICIALIZACION == SipAgent::INICIALIZADO)\
			PJ_LOG(3,("Export.cpp","PJLibException: Code %d, File %s, Info %s",ex.Code, ex.File, ex.Info));\
	}\
	catch (...)\
	{\
		PJ_LOG(3,("Export.cpp","Unknown exception"));\
		if (error)\
		{\
			error->Code = 1;\
			pj_ansi_strcpy(error->File, __FILE__);\
			pj_ansi_strcpy(error->Info, "Unknown exception");\
		}\
		ret = CORESIP_ERROR;\
	}\
	if (th != NULL)\
	{\
		pj_thread_unregister();\
	}

#ifdef _WIN32
#include <windows.h>

/**
 *	Punto de Entrada Principal de la Libreria.
 */
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hinstDLL);
		break;
	}

	return TRUE;
}

#endif

/**
 *	CORESIP_Init Rutina de Inicializacion del Modulo. @ref SipAgent::Init
 */
CORESIP_API int CORESIP_Init(const CORESIP_Config * cfg, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		if (SipAgent::ghMutex == NULL)
		{
			SipAgent::ghMutex = CreateMutex(NULL, FALSE, NULL);
			if (SipAgent::ghMutex == NULL)
			{
				ret = CORESIP_ERROR;
				if (error != NULL)
				{
					error->Code = ret;
					strcpy(error->File, __FILE__);
					sprintf(error->Info, "ERROR CORESIP_Init: No se puede crear ghMutex");
				}
				return ret;
			}
		}

		if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 2000);

		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::NO_INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_Init: CORESIP YA INICIALIZADA O INICIALIZANDOSE");
			}
		}
		else
		{
			SipAgent::ESTADO_INICIALIZACION = SipAgent::INICIALIZANDO;
			SipAgent::Init(cfg);
		}

		//UNIFETM: En el etm si retorna error 130049 (el bind con la ip da error) entonces lo reintenta con la localhost

	}
	catch_all;

	if (ret != CORESIP_OK)
	{
		SipAgent::ESTADO_INICIALIZACION = SipAgent::NO_INICIALIZADO;
	}
	else if (SipAgent::ETM == PJ_FALSE)
	{
		//Si la aplicacion no es el ETM entonces damos por inicializada la Coresip. 
		//Asi lo requiere ulises.
		//Sin embargo en el ETM no la damos por inicializada hasta que no ha terminado el Start.
		//Es porque en el ETM intentar realizar operaciones que requieren el Start.
		SipAgent::ESTADO_INICIALIZACION = SipAgent::INICIALIZADO;
	}

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_Start Rutina de Arranque del Modulo. @ref SipAgent::Start
 */
CORESIP_API int CORESIP_Start(CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 2000);

	Try
	{	
		if ((SipAgent::ETM && SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZANDO) ||
			(!SipAgent::ETM && SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO && SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZANDO))
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_Start: Debe ser llamada despues de CORESIP_Init");
			}
		}
		else
		{
			SipAgent::Start();
		}
	}
	catch_all;

	if (ret != CORESIP_OK)
	{
		SipAgent::ESTADO_INICIALIZACION = SipAgent::NO_INICIALIZADO;
	}
	else
	{
		pj_thread_sleep(100);
		SipAgent::ESTADO_INICIALIZACION = SipAgent::INICIALIZADO;
	}

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_End Rutina de Parada del Modulo. @ref SipAgent::Stop
 */
CORESIP_API void CORESIP_End()
{
	int ret = CORESIP_OK;
	CORESIP_Error * error = NULL;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 2000);

	SipAgent::ESTADO_INICIALIZACION = SipAgent::NO_INICIALIZADO;

	Try
	{
		SipAgent::Stop();
	}
	catch_all;	

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return;
}

/**
 *	CORESIP_Set_Ed137_version 
 */
CORESIP_API int	CORESIP_Set_Ed137_version(char ED137Radioversion, char ED137Phoneversion, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 2000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_Set_Ed137_version: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			SipAgent::Set_Ed137_version(ED137Radioversion, ED137Phoneversion);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_Force_Ed137_version_header
 */
CORESIP_API int	CORESIP_Force_Ed137_version_header(int force, char* ED137Radioversion, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 2000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_Set_Ed137_version: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			SipAgent::Force_Ed137_version_header(force, ED137Radioversion);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_Get_Ed137_version
 */
CORESIP_API int	CORESIP_Get_Ed137_version(char *ED137Radioversion, char *ED137Phoneversion, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 2000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_Get_Ed137_version: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			SipAgent::Get_Ed137_version(ED137Radioversion, ED137Phoneversion);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 * CORESIP_SetSipPort. Establece el puerto SIP
 */
CORESIP_API int	CORESIP_SetSipPort(int port, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 2000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_SetSipPort: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			SipAgent::SetSipPort(port);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_SetLogLevel Establece el nivel de LOG del Modulo. @ref SipAgent::SetLogLevel
 */
CORESIP_API int CORESIP_SetLogLevel(unsigned level, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 2000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_SetLogLevel: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			SipAgent::SetLogLevel(level);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_SetParams Establece los Parametros del Modulo. @ref SipAgent::SetParams
 */
CORESIP_API int CORESIP_SetParams(const int* MaxForwards, const int* Options_answer_code, CORESIP_Error *error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 2000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_SetParams: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			SipAgent::SetParams(MaxForwards, Options_answer_code);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_SetJitterBuffer Establece los Parametros del buffer de jitter
 */
CORESIP_API int	CORESIP_SetJitterBuffer(unsigned adaptive, unsigned initial_prefetch, unsigned min_prefetch, unsigned max_prefetch, unsigned discard, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 2000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_SetJitterBuffer: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			SipAgent::SetJitterBuffer(adaptive, initial_prefetch, min_prefetch, max_prefetch, discard);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_CreateAccount. Registra una cuenta SIP en el M�dulo. @ref SipAgent::CreateAccount
 */
CORESIP_API int CORESIP_CreateAccount(const char * acc, int defaultAcc, int * accId, CORESIP_Error * error)
{
	int ret = CORESIP_OK;
	int defaultAcc_aux = defaultAcc;

	char accaux[CORESIP_MAX_URI_LENGTH];
	strncpy(accaux, acc, sizeof(accaux));
	accaux[sizeof(accaux) - 1] = 0;

	int tries = 100;
	while (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
	{
		Sleep(50);
		if (--tries <= 0) break;
	}

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_CreateAccount: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			*accId = (SipAgent::CreateAccount(accaux, defaultAcc_aux) | CORESIP_ACC_ID);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_CreateAccount. Registra una cuenta SIP en el M�dulo. @ref SipAgent::CreateAccount
 */
CORESIP_API int CORESIP_CreateAccountForceContact(const char* acc, int defaultAcc, int* accId, char *forced_contact, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_CreateAccountForceContact: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			*accId = (SipAgent::CreateAccount(acc, defaultAcc, NULL, forced_contact) | CORESIP_ACC_ID);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_CreateAccountProxyRouting. Registra una cuenta SIP en el M�dulo y los paquetes sip se enrutan por el proxy. 
 */
CORESIP_API int CORESIP_CreateAccountProxyRouting(const char * acc, int defaultAcc, int * accId, const char *proxy_ip, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_CreateAccountProxyRouting: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			*accId = (SipAgent::CreateAccount(acc, defaultAcc, proxy_ip) | CORESIP_ACC_ID);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_CreateAccountAndRegisterInProxy. Crea una cuenta y se registra en el SIP proxy. Los paquetes sip se rutean por el SIP proxy tambi�n.
 */
CORESIP_API int CORESIP_CreateAccountAndRegisterInProxy(const char * acc, int defaultAcc, int * accId, const char *proxy_ip, 
														unsigned int expire_seg, const char *username, const char *pass, const char * displayName, int isfocus, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_CreateAccountAndRegisterInProxy: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			*accId = (SipAgent::CreateAccountAndRegisterInProxy(acc, defaultAcc, proxy_ip, expire_seg, username, pass, displayName, isfocus, NULL) | CORESIP_ACC_ID);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_CreateAccountAndRegisterInProxyForceContact. Crea una cuenta y se registra en el SIP proxy. Los paquetes sip se rutean por el SIP proxy tambi�n.
 */
CORESIP_API int CORESIP_CreateAccountAndRegisterInProxyForceContact(const char* acc, int defaultAcc, int* accId, const char* proxy_ip,
	unsigned int expire_seg, const char* username, const char* pass, const char* displayName, int isfocus, char* forced_contact, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_CreateAccountAndRegisterInProxyForceContact: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			*accId = (SipAgent::CreateAccountAndRegisterInProxy(acc, defaultAcc, proxy_ip, expire_seg, username, pass, displayName, isfocus, forced_contact) | CORESIP_ACC_ID);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_DestroyAccount. Elimina una cuenta SIP del modulo. @ref SipAgent::DestroyAccount
 */
CORESIP_API int CORESIP_DestroyAccount(int accId, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			if (SipAgent::ETM)
			{
				//En el caso de ETM no retorno error porque le provoca un error a Labview
				ret = CORESIP_OK;
			}
			else
			{
				ret = CORESIP_ERROR;
				if (error != NULL)
				{
					error->Code = ret;
					strcpy(error->File, __FILE__);
					sprintf(error->Info, "ERROR CORESIP_DestroyAccount: CORESIP NO INICIALIZADA");
				}
			}
		}
		else if ((accId & CORESIP_ID_TYPE_MASK) != CORESIP_ACC_ID)
		{
			pj_status_t st = PJ_EINVALIDOP;
			PJ_CHECK_STATUS(st, ("ERROR CORESIP_DestroyAccount: accId no valido"));
		}
		else
		{			
			SipAgent::DestroyAccount(accId & CORESIP_ID_MASK);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_SetTipoGRS. Configura el tipo de GRS. El ETM lo llama cuando crea un account tipo GRS.
 */
CORESIP_API int CORESIP_SetTipoGRS(int accId, CORESIP_CallFlagsMask FlagGRS, int on, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_SetTipoGRS: CORESIP NO INICIALIZADA");
			}
		}
		else if ((accId & CORESIP_ID_TYPE_MASK) != CORESIP_ACC_ID)
		{
			pj_status_t st = PJ_EINVALIDOP;
			PJ_CHECK_STATUS(st, ("ERROR CORESIP_SetTipoGRS: accId no valido"));
		}
		else
		{
			SipAgent::SetTipoGRS(accId & CORESIP_ID_MASK, FlagGRS, on);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_SetGRSParams. Configura parametros del GRS
 */
CORESIP_API int CORESIP_SetGRSParams(int accId, char* RdFr, unsigned int *Tj1, unsigned int *Ts1, unsigned int* Ts2,
	char* preferred_grs_bss_method, unsigned int *preferred_grs_bss_method_code, 
	int* forced_ptt_id, int* selcal_supported, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_SetGRSParams: CORESIP NO INICIALIZADA");
			}
		}
		else if ((accId & CORESIP_ID_TYPE_MASK) != CORESIP_ACC_ID)
		{
			pj_status_t st = PJ_EINVALIDOP;
			PJ_CHECK_STATUS(st, ("ERROR CORESIP_SetTipoGRS: accId no valido"));
		}
		else
		{
			pjsua_acc_id pj_acc_id;
			if (accId < 0)
			{
				pj_acc_id = PJSUA_INVALID_ID;
			}
			else
			{
				pj_acc_id = accId & CORESIP_ID_MASK;
			}

			SipAgent::SetGRSParams(pj_acc_id, RdFr, Tj1, Ts1, Ts2, preferred_grs_bss_method, preferred_grs_bss_method_code, forced_ptt_id, selcal_supported);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_GRS_Force_Ptt_Mute. Como GRS Fuerza PTT mute en R2S Keepalives hacia VCS. Sirve para simular un PTT mute de otra sesion inventada.
 */
CORESIP_API int CORESIP_GRS_Force_Ptt_Mute(int call, CORESIP_PttType PttType, unsigned short PttId, bool on, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_GRS_Force_Ptt_Mute: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			if ((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID)
				SipCall::GRS_Force_Ptt_Mute(call & CORESIP_ID_MASK, PttType, PttId, on);
			else
			{
				ret = CORESIP_ERROR;
				if (error != NULL)
				{
					error->Code = 0;
					strcpy(error->File, __FILE__);
					sprintf(error->Info, "CORESIP_GRS_Force_Ptt_Mute: Invalid call id 0x%X", call);
				}
			}
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_GRS_Force_Ptt.
 */
CORESIP_API int CORESIP_GRS_Force_Ptt(int call, CORESIP_PttType PttType, unsigned short PttId, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_GRS_Force_Ptt: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			if ((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID)
				SipCall::GRS_Force_Ptt(call & CORESIP_ID_MASK, PttType, PttId);
			else
			{
				ret = CORESIP_ERROR;
				if (error != NULL)
				{
					error->Code = 0;
					strcpy(error->File, __FILE__);
					sprintf(error->Info, "CORESIP_GRS_Force_Ptt_Mute: Invalid call id 0x%X", call);
				}
			}
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_GRS_Force_SCT. Como GRS Fuerza el bit SCT en el RTPRx enviado desde un GRS
 */
CORESIP_API int CORESIP_GRS_Force_SCT(int call, bool on, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_GRS_Force_SCT: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			if ((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID)
				SipCall::GRS_Force_SCT(call & CORESIP_ID_MASK, on);
			else
			{
				ret = CORESIP_ERROR;
				if (error != NULL)
				{
					error->Code = 0;
					strcpy(error->File, __FILE__);
					sprintf(error->Info, "CORESIP_GRS_Force_SCT: Invalid call id 0x%X", call);
				}
			}
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_Force_PTTS. Fuerza el bit PTTS en el RTPRx o RTPTx
 */
CORESIP_API int CORESIP_Force_PTTS(int call, bool on, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_Force_PTTS: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			if ((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID)
				SipCall::Force_PTTS(call & CORESIP_ID_MASK, on);
			else
			{
				ret = CORESIP_ERROR;
				if (error != NULL)
				{
					error->Code = 0;
					strcpy(error->File, __FILE__);
					sprintf(error->Info, "CORESIP_Force_PTTS: Invalid call id 0x%X", call);
				}
			}
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_AddSndDevice		A�ade un dispositvo de audio al m�dulo. @ref SipAgent::AddSndDevice
 */
CORESIP_API int CORESIP_AddSndDevice(const CORESIP_SndDeviceInfo * info, int * dev, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_AddSndDevice: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			*dev = (SipAgent::AddSndDevice(info) | CORESIP_SNDDEV_ID);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_CreateWavPlayer		Crea un 'Reproductor' WAV. @ref SipAgent::CreateWavPlayer
 */
CORESIP_API int CORESIP_CreateWavPlayer(const char * file, unsigned loop, int * wavPlayer, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_CreateWavPlayer: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			*wavPlayer = (SipAgent::CreateWavPlayer(file, loop != 0) | CORESIP_WAVPLAYER_ID);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_DestroyWavPlayer	Elimina un Reproductor WAV. @ref SipAgent::DestroyWavPlayer
 */
CORESIP_API int CORESIP_DestroyWavPlayer(int wavPlayer, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_DestroyWavPlayer: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			if ((wavPlayer & CORESIP_ID_TYPE_MASK) == CORESIP_WAVPLAYER_ID)
				SipAgent::DestroyWavPlayer(wavPlayer & CORESIP_ID_MASK);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_CreateWavRecorder	Crea un 'grabador' en formato WAV. @ref SipAgent::CreateWavRecorder
 */
CORESIP_API int CORESIP_CreateWavRecorder(const char * file, int * wavRecorder, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_CreateWavRecorder: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			*wavRecorder = (SipAgent::CreateWavRecorder(file) | CORESIP_WAVRECORDER_ID);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_DestroyWavRecorder	Elimina un 'grabador' WAV. @ref SipAgent::DestroyWavRecorder
 */
CORESIP_API int CORESIP_DestroyWavRecorder(int wavRecorder, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_DestroyWavRecorder: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			if ((wavRecorder & CORESIP_ID_TYPE_MASK) == CORESIP_WAVRECORDER_ID)
				SipAgent::DestroyWavRecorder(wavRecorder & CORESIP_ID_MASK);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}


/**
 *	CORESIP_CreateRdRxPort		Crea un 'PORT' @ref RdRxPort de Recepcion Radio. @ref SipAgent::CreateRdRxPort
 */
CORESIP_API int CORESIP_CreateRdRxPort(const CORESIP_RdRxPortInfo * info, const char * localIp, int * rdRxPort, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_CreateRdRxPort: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			*rdRxPort = (SipAgent::CreateRdRxPort(info, localIp) | CORESIP_RDRXPORT_ID);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_DestroyRdRxPort		Elimina un Puerto @ref RdRxPort. @ref SipAgent::DestroyRdRxPort
 */
CORESIP_API int CORESIP_DestroyRdRxPort(int rdRxPort, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_DestroyRdRxPort: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			SipAgent::DestroyRdRxPort(rdRxPort & CORESIP_ID_MASK);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_CreateSndRxPort.	Crea un puerto @ref SoundRxPort. @ref SipAgent::CreateSndRxPort
 */
CORESIP_API int CORESIP_CreateSndRxPort(const char * id, int * sndRxPort, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_CreateSndRxPort: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			*sndRxPort = (SipAgent::CreateSndRxPort(id) | CORESIP_SNDRXPORT_ID);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_DestroySndRxPort	Eliminar un puerto @ref SoundRxPort. @ref SipAgent::DestroySndRxPort
 */
CORESIP_API int CORESIP_DestroySndRxPort(int sndRxPort, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_DestroySndRxPort: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			SipAgent::DestroySndRxPort(sndRxPort & CORESIP_ID_MASK);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_BridgeLink			Configura un enlace de conferencia. @ref SipAgent::BridgeLink
 */
CORESIP_API int CORESIP_BridgeLink(int src, int dst, int on, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_BridgeLink: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			SipAgent::BridgeLink(src & CORESIP_ID_TYPE_MASK, src & CORESIP_ID_MASK,
				dst & CORESIP_ID_TYPE_MASK, dst & CORESIP_ID_MASK, on != 0);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_SendToRemote		Configura El puerto de Sonido apuntado para los envios UNICAST de Audio. @ref SipAgent::SendToRemote
 */
CORESIP_API int CORESIP_SendToRemote(int dev, int on, const char * id, const char * ip, unsigned port, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_SendToRemote: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			SipAgent::SendToRemote(dev & CORESIP_ID_TYPE_MASK, dev & CORESIP_ID_MASK, on != 0, id, ip, port);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_ReceiveFromRemote
 */
CORESIP_API int CORESIP_ReceiveFromRemote(const char * localIp, const char * mcastIp, unsigned mcastPort, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_ReceiveFromRemote: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			SipAgent::ReceiveFromRemote(localIp, mcastIp, mcastPort);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_SetVolume
 */
CORESIP_API int CORESIP_SetVolume(int id, unsigned volume, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_SetVolume: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			SipAgent::SetVolume(id & CORESIP_ID_TYPE_MASK, id & CORESIP_ID_MASK, volume);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_GetVolume
 */
CORESIP_API int CORESIP_GetVolume(int id, unsigned * volume, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_GetVolume: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			*volume = SipAgent::GetVolume(id & CORESIP_ID_TYPE_MASK, id & CORESIP_ID_MASK);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_CallMake
 */
CORESIP_API int CORESIP_CallMake(const CORESIP_CallInfo * info, const CORESIP_CallOutInfo * outInfo, int * call, CORESIP_Error * error)
{
	int ret = CORESIP_OK;	

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_CallMake: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			*call = (SipCall::New(info, outInfo) | CORESIP_CALL_ID);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_CallAnswer
 */
CORESIP_API int CORESIP_CallAnswer(int call, unsigned code, int addToConference, unsigned reason_code, char* reason_text, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_CallAnswer: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			if ((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID)
				SipCall::Answer(call & CORESIP_ID_MASK, code, addToConference != 0, reason_code, reason_text);
			else
			{
				ret = CORESIP_ERROR;
				if (error != NULL)
				{
					error->Code = 0;
					strcpy(error->File, __FILE__);
					sprintf(error->Info, "CORESIP_CallAnswer: Invalid call id 0x%X", call);
				}
			}
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_CallMovedTemporallyAnswer
 */
CORESIP_API int CORESIP_CallMovedTemporallyAnswer(int call, const char *dst, const char *reason, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_CallMovedTemporallyAnswer: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			if ((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID)
				SipCall::MovedTemporallyAnswer(call & CORESIP_ID_MASK, dst, reason);
				//SipCall::MovedTemporallyAnswer(call & CORESIP_ID_MASK, "sip:314002@192.168.1.202", "busy");
			else
			{
				ret = CORESIP_ERROR;
				if (error != NULL)
				{
					error->Code = 0;
					strcpy(error->File, __FILE__);
					sprintf(error->Info, "CORESIP_MovedTemporallyAnswer: Invalid call id 0x%X", call);
				}
			}
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_CallProccessRedirect
 *	Esta funcion debe llamarse despues de recibirse la callback MovedTemporallyCb para 
 */
CORESIP_API int CORESIP_CallProccessRedirect(int call, const char *dstUri, CORESIP_REDIRECT_OP op, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_CallProccessRedirect: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			if ((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID)
				SipCall::CallProccessRedirect(call & CORESIP_ID_MASK, dstUri, op);
			else
			{
				ret = CORESIP_ERROR;
				if (error != NULL)
				{
					error->Code = 0;
					strcpy(error->File, __FILE__);
					sprintf(error->Info, "CORESIP_CallProccessRedirect: Invalid call id 0x%X", call);
				}
			}
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_CallHangup
 */
CORESIP_API int CORESIP_CallHangup(int call, unsigned code, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_CallHangup: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			if ((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID)
				SipCall::Hangup(call & CORESIP_ID_MASK, code);
			else
			{
				ret = CORESIP_ERROR;
				if (error != NULL)
				{
					error->Code = 0;
					strcpy(error->File, __FILE__);
					sprintf(error->Info, "CORESIP_CallHangup: Invalid call id 0x%X", call);
				}
			}
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_CallHold
 */
CORESIP_API int CORESIP_CallHold(int call, int hold, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_CallHold: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			if ((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID)
				SipCall::Hold(call & CORESIP_ID_MASK, hold != 0);
			else
			{
				ret = CORESIP_ERROR;
				if (error != NULL)
				{
					error->Code = 0;
					strcpy(error->File, __FILE__);
					sprintf(error->Info, "CORESIP_CallHold: Invalid call id 0x%X", call);
				}
			}
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_CallReinvite
 */
CORESIP_API int CORESIP_CallReinvite(int call, CORESIP_Error* error, int CallType_SDP, int TxRx_SDP, char* etm_vcs_bss_methods, CORESIP_SDPSendRecvAttrForced ForceSDPSendRecvAttr)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_CallReinvite: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			SipCall::Reinvite(call & CORESIP_ID_MASK, CallType_SDP, TxRx_SDP, etm_vcs_bss_methods, ForceSDPSendRecvAttr);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_CallTransfer
 */
CORESIP_API int CORESIP_CallTransfer(int call, int dstCall, const char * dst, const char *display_name, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_CallTransfer: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			if ((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID)
				SipCall::Transfer(call & CORESIP_ID_MASK, dstCall != PJSUA_INVALID_ID ? dstCall & CORESIP_ID_MASK : PJSUA_INVALID_ID, dst, display_name);
			else
			{
				ret = CORESIP_ERROR;
				if (error != NULL)
				{
					error->Code = 0;
					strcpy(error->File, __FILE__);
					sprintf(error->Info, "CORESIP_CallTransfer: Invalid call id 0x%X", call);
				}
			}
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_CallPtt
 */
CORESIP_API int CORESIP_CallPtt(int call, const CORESIP_PttInfo * info, CORESIP_Error * error)
{
	//UNIFETM: Esta funcion tiene que utilizarse en el ETM tambien para Squelch

	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_CallPtt: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			if ((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID)
				SipCall::Ptt(call & CORESIP_ID_MASK, info);
			else
			{
				ret = CORESIP_ERROR;
				if (error != NULL)
				{
					error->Code = 0;
					strcpy(error->File, __FILE__);
					sprintf(error->Info, "CORESIP_CallPtt: Invalid call id 0x%X", call);
				}
			}
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_CallPtt_Delayed
 */
CORESIP_API int CORESIP_CallPtt_Delayed(int call, const CORESIP_PttInfo* info, unsigned int delay_ms, CORESIP_Error* error)
{
	//UNIFETM: Esta funcion tiene que utilizarse en el ETM tambien para Squelch

	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_CallPtt_Delayed: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			if ((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID)
				SipCall::Ptt(call & CORESIP_ID_MASK, info, delay_ms);
			else
			{
				ret = CORESIP_ERROR;
				if (error != NULL)
				{
					error->Code = 0;
					strcpy(error->File, __FILE__);
					sprintf(error->Info, "CORESIP_CallPtt: Invalid call id 0x%X", call);
				}
			}
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_CallSendInfo
 */
CORESIP_API int CORESIP_CallSendInfo(int call, const char * info, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{		
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_CallSendInfo: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			if ((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID)
				SipCall::SendInfoMsg(call & CORESIP_ID_MASK, info);
			else
			{
				ret = CORESIP_ERROR;
				if (error != NULL)
				{
					error->Code = 0;
					strcpy(error->File, __FILE__);
					sprintf(error->Info, "CORESIP_CallSendInfo: Invalid call id 0x%X", call);
				}
			}
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_GetRdQidx
 */
CORESIP_API int	CORESIP_GetRdQidx(int call, int* Qidx, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_GetRdQidx: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			//pj_assert((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID);
			if ((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID)
				SipCall::GetRdQidx(call & CORESIP_ID_MASK, Qidx);
			else
			{
				ret = CORESIP_ERROR;
				if (error)
				{
					error->Code = 0;
					strcpy(error->File, __FILE__);
					sprintf(error->Info, "CORESIP_GetRdQidx: Invalid call id 0x%X", call);
				}
			}
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_CallConference
 */
CORESIP_API int CORESIP_CallConference(int call, int conf, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_CallConference: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			if ((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID)
				SipCall::Conference(call & CORESIP_ID_MASK, conf != 0);
			else
			{
				ret = CORESIP_ERROR;
				if (error != NULL)
				{
					error->Code = 0;
					strcpy(error->File, __FILE__);
					sprintf(error->Info, "CORESIP_CallConference: Invalid call id 0x%X", call);
				}
			}
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_CallSendConfInfo
 */
CORESIP_API int CORESIP_CallSendConfInfo(int call, const CORESIP_ConfInfo * info, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_CallSendConfInfo: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			if ((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID)
				SipCall::SendConfInfo(call & CORESIP_ID_MASK, info);
			else
			{
				ret = CORESIP_ERROR;
				if (error != NULL)
				{
					error->Code = 0;
					strcpy(error->File, __FILE__);
					sprintf(error->Info, "CORESIP_CallSendConfInfo: Invalid call id 0x%X", call);
				}
			}
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_SendConfInfoFromAcc
 */
CORESIP_API int CORESIP_SendConfInfoFromAcc(int accId, const CORESIP_ConfInfo * info, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_SendConfInfoFromAcc: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			SubscriptionServer::SendConfInfoFromAcc(accId & CORESIP_ID_MASK, info);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_TransferAnswer
 */
CORESIP_API int CORESIP_TransferAnswer(const char * tsxKey, void * txData, void * evSub, unsigned code, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_TransferAnswer: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			SipCall::TransferAnswer(tsxKey, txData, evSub, code);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_TransferNotify
 */
CORESIP_API int CORESIP_TransferNotify(void * evSub, unsigned code, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_TransferNotify: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			SipCall::TransferNotify(evSub, code);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_SendOptionsMsg
 *  Esta funci�n no envia OPTIONS a traves del proxy
 */
CORESIP_API int CORESIP_SendOptionsMsg(const char * dst, char * callid, int isRadio, CORESIP_Error * error)
{
	//UNIFETM: Esta funcion tiene parametros diferentes. Cambiar en ETM para que sea como en ULISES

	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_SendOptionsMsg: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			OptionsMod::SendOptionsMsg(dst, callid, isRadio);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 * CORESIP_SendOptionsCFWD.	...
 * Envia mensaje OPTIONS necesario para la negociacion Call Forward
 */
CORESIP_API int CORESIP_SendOptionsCFWD(int accId, const char* dst, CORESIP_CFWR_OPT_TYPE cfwr_options_type, const char* body, char* callid, pj_bool_t by_proxy, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_SendOptionsCFWD: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			OptionsMod::SendOptionsCFWD(accId & CORESIP_ID_MASK, dst, cfwr_options_type, body, callid, by_proxy);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 * CORESIP_SendResponseCFWD.	...
 * Envia la respuesta al options utilizado para la negociacion de call forward
 */
CORESIP_API int CORESIP_SendResponseCFWD(int st_code, const char* body, unsigned int hresp, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_SendResponseCFWD: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			OptionsMod::SendResponseCFWD(st_code, body, hresp);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_SendOptionsMsgProxy
 *  Envia OPTIONS a traves del proxy.
 */
CORESIP_API int CORESIP_SendOptionsMsgProxy(const char * dst, char * callid, int isRadio, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_SendOptionsMsgProxy: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			OptionsMod::SendOptionsMsg(dst, callid, isRadio, PJ_TRUE);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/** AGL. Para el SELCAL */
/**
 *	CORESIP_Wav2RemoteStart
 */
CORESIP_API int CORESIP_Wav2RemoteStart(const char *filename, const char * id, const char * ip, unsigned port, void (*eofCb)(void *), CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_Wav2RemoteStart: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			SipAgent::CreateWavPlayer2Remote(filename, id, ip, port);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_Wav2RemoteEnd
 */
CORESIP_API int CORESIP_Wav2RemoteEnd(void *obj, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_Wav2RemoteEnd: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			SipAgent::DestroyWavPlayer2Remote();
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_RdPttEvent. Se llama cuando hay un evento de PTT en el HMI. Sirve sobretodo para enviar los metadata de grabacion VoIP en el puesto
 */
CORESIP_API int CORESIP_RdPttEvent(bool on, const char *freqId, int dev, CORESIP_Error * error, CORESIP_PttType PTT_type) 
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_RdPttEvent: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			dev &= ~CORESIP_SNDDEV_ID;
			SipAgent::RdPttEvent(on, freqId, dev, PTT_type);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}


/**
 *	CORESIP_RdSquEvent. Se llama cuando hay un evento de Squelch en el HMI. Sirve sobretodo para enviar los metadata de grabacion VoIP en el puesto
 */
CORESIP_API int CORESIP_RdSquEvent(bool on, const char *freqId, const char *resourceId, const char *bssMethod, unsigned int bssQidx, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_RdSquEvent: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			SipAgent::RdSquEvent(on, freqId, resourceId, bssMethod, bssQidx);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_RecorderCmd. Se pasan comandos para realizar acciones sobre el grabador VoIP
 */
CORESIP_API int CORESIP_RecorderCmd(CORESIP_RecCmdType cmd, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_RecorderCmd: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			ret = SipAgent::RecorderCmd(cmd, error);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_CreatePresenceSubscription. Crea una subscripcion por evento de presencia
 */
CORESIP_API int CORESIP_CreatePresenceSubscription(char *dest_uri, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);
	
	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_CreatePresenceSubscription: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			ret = SipAgent::CreatePresenceSubscription(dest_uri);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_DestroyPresenceSubscription. destruye una subscripcion por evento de presencia
 */
CORESIP_API int CORESIP_DestroyPresenceSubscription(char *dest_uri, CORESIP_Error * error)
{
	int ret = CORESIP_OK;
	
	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_DestroyPresenceSubscription: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			ret = SipAgent::DestroyPresenceSubscription(dest_uri);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_CreateConferenceSubscription. Crea una subscripcion por evento de conferencia
 */
CORESIP_API int CORESIP_CreateConferenceSubscription(int accId, int call, char *dest_uri, pj_bool_t by_proxy, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);
	
	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_CreateConferenceSubscription: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			pjsua_acc_id pj_acc_id = PJSUA_INVALID_ID;
			if (accId < 0)
			{
				pj_acc_id = PJSUA_INVALID_ID;
			}
			else
			{
				pj_acc_id = accId & CORESIP_ID_MASK;
			}

			int call_id;
			if (call == -1)
			{
				call_id = -1;
			}
			else
			{
				call_id = call & CORESIP_ID_MASK;
			}

			ret = SipAgent::CreateConferenceSubscription(pj_acc_id, call_id, dest_uri, by_proxy);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_DestroyConferenceSubscription. Destruye una subscripcion por evento de conferencia
 */
CORESIP_API int CORESIP_DestroyConferenceSubscription(char *dest_uri, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);
	
	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_DestroyConferenceSubscription: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			ret = SipAgent::DestroyConferenceSubscription(dest_uri);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_CreateDialogSubscription. Crea una subscripcion por evento de Dialogo
 */
CORESIP_API int CORESIP_CreateDialogSubscription(int accId, char *dest_uri, pj_bool_t by_proxy, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);
	
	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_CreateDialogSubscription: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			ret = SipAgent::CreateDialogSubscription(accId & CORESIP_ID_MASK, dest_uri, by_proxy);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_DestroyDialogSubscription. Destruye una subscripcion por evento de Dialogo
 */
CORESIP_API int CORESIP_DestroyDialogSubscription(char* dest_uri, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_DestroyDialogSubscription: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			ret = SipAgent::DestroyDialogSubscription(dest_uri);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_SendWG67Subscription. Crea y envia una subscripcion por evento WG67KEY-IN
 */
CORESIP_API int CORESIP_SendWG67Subscription(int accId, char* dest_uri, int expires, int noRefresh, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_SendWG67Subscription: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			pjsua_acc_id pj_acc_id = PJSUA_INVALID_ID;
			if (accId < 0)
			{
				pj_acc_id = PJSUA_INVALID_ID;
			}
			else
			{
				pj_acc_id = accId & CORESIP_ID_MASK;
			}

			ret = WG67Subs::SendWG67Subscription(pj_acc_id, -1, dest_uri, expires, (pj_bool_t)noRefresh, PJ_FALSE);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_SetWG67SubscriptionParameters. Modifica parametros de la subscripcion por evento WG67KE-IN. 
 */
CORESIP_API int CORESIP_SetWG67SubscriptionParameters(int accId, char* dest_uri, int noRefresh, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_SetWG67SubscriptionParameters: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			pjsua_acc_id pj_acc_id;
			if (accId < 0)
			{
				pj_acc_id = PJSUA_INVALID_ID;
			}
			else
			{
				pj_acc_id = accId & CORESIP_ID_MASK;
			}

			ret = WG67Subs::SetWG67SubscriptionParameters(pj_acc_id, -1, dest_uri, noRefresh);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_Set_WG67_Notifier_Parameters. Configura algunos parametros del WG67 notifier
 */
CORESIP_API int CORESIP_Set_WG67_Notifier_Parameters(int notify_enabled, int manual, int minimum_expires, int maximum_expires, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_Set_WG67_Notifier_Parameters: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			ret = WG67Subs::Set_WG67_Notifier_Parameters(notify_enabled, manual, minimum_expires, maximum_expires);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 * CORESIP_Set_WG67_notify_status: Establece el estado de la suscripcion y las sesiones ficticias, ademas de enviar el Notify correspondiente. 
 */
CORESIP_API int CORESIP_Set_WG67_notify_status(int accId, char* subscriberUri, CORESIP_WG67Notify_SubscriptionState_Config* subsState,
	CORESIP_WG67Notify_Body_Config* wG67Notify_Body, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_Set_WG67_notify_status: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			pjsua_acc_id pj_acc_id;
			if (accId < 0)
			{
				pj_acc_id = PJSUA_INVALID_ID;
			}
			else
			{
				pj_acc_id = accId & CORESIP_ID_MASK;
			}

			ret = WG67Subs::Set_WG67_notify_status(pj_acc_id, subscriberUri, subsState, wG67Notify_Body);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 * CORESIP_Get_GRS_WG67SubscriptionsList.
 */
CORESIP_API int CORESIP_Get_GRS_WG67SubscriptionsList(int accId, int* nSubscriptions, CORESIP_WG67_Subscription_Info* WG67Subscriptions[], CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_Get_GRS_WG67SubscriptionsList: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			pjsua_acc_id pj_acc_id;
			if (accId < 0)
			{
				pj_acc_id = PJSUA_INVALID_ID;
			}
			else
			{
				//pj_assert((accId & CORESIP_ID_TYPE_MASK) == CORESIP_ACC_ID);
				pj_acc_id = accId & CORESIP_ID_MASK;
			}

			ret = SubscriptionServer::GetWG67SubscriptionList(pj_acc_id, nSubscriptions, WG67Subscriptions);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}


/**
 *	CORESIP_CreateGenericPort		Crea un puerto generico de media
 */
CORESIP_API int CORESIP_CreateGenericPort(int* gen_port_id, int jitter_buff_size, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_CreateGenericPort: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			*gen_port_id = (GenericPort::CreateGenericPort(jitter_buff_size) | CORESIP_GENPORT_ID);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_DestroyGenericPort		Destruye un puerto generico de media
 */
CORESIP_API int CORESIP_DestroyGenericPort(int gen_port_id, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_DestroyGenericPort: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			GenericPort::DestroyGenericPort(gen_port_id & CORESIP_ID_MASK);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_PutInGenericPort: Pone muestras en el generic port.
 */
CORESIP_API int CORESIP_PutInGenericPort(int gen_port_id, CORESIP_GenericPortBuffer *genBuff, pj_bool_t blocking, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL && !blocking) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (genBuff == NULL)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_PutInGenericPort: genBuff es NULL");
			}
		}
		else if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_PutInGenericPort: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			GenericPort::PutInGenericPort(gen_port_id & CORESIP_ID_MASK, genBuff->buffer, sizeof(genBuff->buffer), blocking);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL && !blocking) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
*CORESIP_GetFromGenericPort: Toma muestras del generic port
*/
CORESIP_API int CORESIP_GetFromGenericPort(int gen_port_id, CORESIP_GenericPortBuffer *genBuff, pj_bool_t blocking, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL && !blocking) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (genBuff == NULL)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_GetFromGenericPort: genBuff es NULL");
			}
		}
		else if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_GetFromGenericPort: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			GenericPort::GetFromGenericPort(gen_port_id & CORESIP_ID_MASK, genBuff->buffer, sizeof(genBuff->buffer), blocking);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL && !blocking) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
*CORESIP_GetJitterStatusGenericPort:
*/
CORESIP_API int CORESIP_GetJitterStatusGenericPort(int gen_port_id, unsigned int* size, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (size == NULL)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_GetFromGenericPort: size es NULL");
			}
		}
		else if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_GetFromGenericPort: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			GenericPort::GetJitterStatusGenericPort(gen_port_id & CORESIP_ID_MASK, size);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 * CORESIP_SendInstantMessage. Envia un mensaje instantaneo
 */
CORESIP_API int CORESIP_SendInstantMessage(int acc_id, char *dest_uri, char *text, pj_bool_t by_proxy, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);
	
	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_SendInstantMessage: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			ret = SipAgent::SendInstantMessage(acc_id & CORESIP_ID_MASK, dest_uri, text, by_proxy);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 * CORESIP_EchoCancellerLCMic.	...
 * Activa/desactiva cancelador de eco altavoz LC y Microfonos. Sirve para el modo manos libres 
 */
CORESIP_API int CORESIP_EchoCancellerLCMic(bool on, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);
	
	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_EchoCancellerLCMic: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			ret = SipAgent::EchoCancellerLCMic(on);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

CORESIP_API int CORESIP_SetImpairments(int call, CORESIP_Impairments * impairments, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_SetImpairments: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			SipCall::SetImpairments(call & CORESIP_ID_MASK, impairments);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

CORESIP_API int CORESIP_SetCallParameters(int call, int *disableKeepAlives, int* forced_cld, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_SetCallParameters: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			SipCall::SetCallParameters(call & CORESIP_ID_MASK, disableKeepAlives, forced_cld);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 * CORESIP_SetConfirmPtt.	...
 */
CORESIP_API int CORESIP_SetConfirmPtt(int call, pj_bool_t val, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_SetConfirmPtt: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			SipCall::SetConfirmPtt(call & CORESIP_ID_MASK, val);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 * CORESIP_GetNetworkDelay.	...
 */
CORESIP_API int CORESIP_GetNetworkDelay(int call, unsigned int* delay_ms, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_GetNetworkDelay: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			SipCall::GetNetworkDelay(call & CORESIP_ID_MASK, delay_ms);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 * SendToneToCall.	...
 */
CORESIP_API int CORESIP_SendToneToCall(int call, unsigned int frequency, float volume_dbm0, int on, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_SendToneToCall: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			SignalGen::SendToneToCall(call & CORESIP_ID_MASK, frequency, volume_dbm0, on);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 * CORESIP_SendNoiseToCall.	...
 */
CORESIP_API int CORESIP_SendNoiseToCall(int call, float volume_dbm0, int on, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_SendNoiseToCall: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			SignalGen::SendNoiseToCall(call & CORESIP_ID_MASK, NoiseGenerator::White, volume_dbm0, on);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 * CORESIP_SendPinkNoiseToCall.	...
 */
CORESIP_API int CORESIP_SendPinkNoiseToCall(int call, float volume_dbm0, int on, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_SendPinkNoiseToCall: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			SignalGen::SendNoiseToCall(call & CORESIP_ID_MASK, NoiseGenerator::Pink, volume_dbm0, on);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 * SendDTMF.	...
 */
CORESIP_API int CORESIP_SendDTMF(int call, const CORESIP_tone_digit_map* digit_map, const CORESIP_tone_digits *digits, float volume_dbm0, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_SendDTMF: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			SignalGen::SendDTMF(call & CORESIP_ID_MASK, digit_map, digits, volume_dbm0);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

/**
 *	CORESIP_SendSELCAL
 */
CORESIP_API int CORESIP_SendSELCAL(int call, const char* selcalValue, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_SendSELCAL: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			SipCall::SendSELCAL(call & CORESIP_ID_MASK, selcalValue);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

CORESIP_API int CORESIP_SetSNDDeviceWindowsName(CORESIP_SndDevType UlisesDev, const char* DevWinName, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_SetSNDDeviceWindowsName: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			SoundDevHw::SetSNDDeviceWindowsName(UlisesDev, DevWinName);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

CORESIP_API int CORESIP_GetWindowsSoundDeviceNames(int captureType, CORESIP_SndWindowsDevices* Devices, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_GetWindowsSoundDeviceNames: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			SoundDevHw::GetWindowsSoundDeviceNames(captureType, Devices, PJ_FALSE, NULL);
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

CORESIP_API int CORESIP_SetVolumeOutputDevice(CORESIP_SndDevType dev, unsigned int volume, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_SetVolumeOutputDevice: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			pj_status_t st = SoundDevHw::SetVolumeOutputDevice(dev, volume);
			PJ_CHECK_STATUS(st, ("ERROR CORESIP_SetVolumeOutputDevice. Ver logs"));
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}

CORESIP_API int CORESIP_GetVolumeOutputDevice(CORESIP_SndDevType dev, unsigned int *volume, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	if (SipAgent::ghMutex != NULL) WaitForSingleObject(SipAgent::ghMutex, 5000);

	Try
	{
		if (SipAgent::ESTADO_INICIALIZACION != SipAgent::INICIALIZADO)
		{
			ret = CORESIP_ERROR;
			if (error != NULL)
			{
				error->Code = ret;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "ERROR CORESIP_GetVolumeOutputDevice: CORESIP NO INICIALIZADA");
			}
		}
		else
		{
			pj_status_t st = SoundDevHw::GetVolumeOutputDevice(dev, volume);
			PJ_CHECK_STATUS(st, ("ERROR CORESIP_GetVolumeOutputDevice. Ver logs"));
		}
	}
	catch_all;

	if (SipAgent::ghMutex != NULL) ReleaseMutex(SipAgent::ghMutex);

	return ret;
}


/*@}*/