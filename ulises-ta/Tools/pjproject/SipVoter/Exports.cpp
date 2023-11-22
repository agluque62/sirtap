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
	}\
	catch (...)\
	{\
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
		SipAgent::Init(cfg);

		//UNIFETM: En el etm si retorna error 130049 (el bind con la ip da error) entonces lo reintenta con la localhost

	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_Start Rutina de Arranque del Modulo. @ref SipAgent::Start
 */
CORESIP_API int CORESIP_Start(CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		SipAgent::Start();
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_End Rutina de Parada del Modulo. @ref SipAgent::Stop
 */
CORESIP_API void CORESIP_End()
{
	int ret = CORESIP_OK;
	CORESIP_Error * error = NULL;

	Try
	{
		SipAgent::Stop();
	}
	catch_all;
}

/**
 *	CORESIP_Set_Ed137_version 
 */
CORESIP_API int	CORESIP_Set_Ed137_version(char ED137Radioversion, char ED137Phoneversion, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	Try
	{
		SipAgent::Set_Ed137_version(ED137Radioversion, ED137Phoneversion);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_Force_Ed137_version_header
 */
CORESIP_API int	CORESIP_Force_Ed137_version_header(int force, char* ED137Radioversion, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	Try
	{
		SipAgent::Force_Ed137_version_header(force, ED137Radioversion);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_Get_Ed137_version
 */
CORESIP_API int	CORESIP_Get_Ed137_version(char *ED137Radioversion, char *ED137Phoneversion, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	Try
	{
		SipAgent::Get_Ed137_version(ED137Radioversion, ED137Phoneversion);
	}
	catch_all;

	return ret;
}

/**
 * CORESIP_SetSipPort. Establece el puerto SIP
 */
CORESIP_API int	CORESIP_SetSipPort(int port, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		SipAgent::SetSipPort(port);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_SetLogLevel Establece el nivel de LOG del Modulo. @ref SipAgent::SetLogLevel
 */
CORESIP_API int CORESIP_SetLogLevel(unsigned level, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		SipAgent::SetLogLevel(level);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_SetParams Establece los Parametros del Modulo. @ref SipAgent::SetParams
 */
CORESIP_API int CORESIP_SetParams(const int* MaxForwards, const int* Options_answer_code, CORESIP_Error *error)
{
	int ret = CORESIP_OK;

	Try
	{
		SipAgent::SetParams(MaxForwards, Options_answer_code);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_SetJitterBuffer Establece los Parametros del buffer de jitter
 */
CORESIP_API int	CORESIP_SetJitterBuffer(unsigned adaptive, unsigned initial_prefetch, unsigned min_prefetch, unsigned max_prefetch, unsigned discard, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	Try
	{
		SipAgent::SetJitterBuffer(adaptive, initial_prefetch, min_prefetch, max_prefetch, discard);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_CreateAccount. Registra una cuenta SIP en el Módulo. @ref SipAgent::CreateAccount
 */
CORESIP_API int CORESIP_CreateAccount(const char * acc, int defaultAcc, int * accId, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		*accId = (SipAgent::CreateAccount(acc, defaultAcc) | CORESIP_ACC_ID);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_CreateAccountProxyRouting. Registra una cuenta SIP en el Módulo y los paquetes sip se enrutan por el proxy. 
 */
CORESIP_API int CORESIP_CreateAccountProxyRouting(const char * acc, int defaultAcc, int * accId, const char *proxy_ip, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		*accId = (SipAgent::CreateAccount(acc, defaultAcc, proxy_ip) | CORESIP_ACC_ID);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_CreateAccountAndRegisterInProxy. Crea una cuenta y se registra en el SIP proxy. Los paquetes sip se rutean por el SIP proxy también.
 */
CORESIP_API int CORESIP_CreateAccountAndRegisterInProxy(const char * acc, int defaultAcc, int * accId, const char *proxy_ip, 
														unsigned int expire_seg, const char *username, const char *pass, const char * displayName, int isfocus, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		*accId = (SipAgent::CreateAccountAndRegisterInProxy(acc, defaultAcc, proxy_ip, expire_seg, username, pass, displayName, isfocus) | CORESIP_ACC_ID);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_DestroyAccount. Elimina una cuenta SIP del modulo. @ref SipAgent::DestroyAccount
 */
CORESIP_API int CORESIP_DestroyAccount(int accId, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		pj_assert((accId & CORESIP_ID_TYPE_MASK) == CORESIP_ACC_ID);
		SipAgent::DestroyAccount(accId & CORESIP_ID_MASK);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_SetTipoGRS. Configura el tipo de GRS. El ETM lo llama cuando crea un account tipo GRS.
 */
CORESIP_API int CORESIP_SetTipoGRS(int accId, CORESIP_CallFlagsMask FlagGRS, int on, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		pj_assert((accId & CORESIP_ID_TYPE_MASK) == CORESIP_ACC_ID);
		SipAgent::SetTipoGRS(accId & CORESIP_ID_MASK, FlagGRS, on);
	}
	catch_all;

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

	Try
	{
		pjsua_acc_id pj_acc_id;
		if (accId < 0)
		{
			pj_acc_id = PJSUA_INVALID_ID;
		}
		else
		{
			pj_assert((accId & CORESIP_ID_TYPE_MASK) == CORESIP_ACC_ID);
			pj_acc_id = accId & CORESIP_ID_MASK;
		}

		SipAgent::SetGRSParams(pj_acc_id, RdFr, Tj1, Ts1, Ts2, preferred_grs_bss_method, preferred_grs_bss_method_code, forced_ptt_id, selcal_supported);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_GRS_Force_Ptt_Mute. Como GRS Fuerza PTT mute en R2S Keepalives hacia VCS. Sirve para simular un PTT mute de otra sesion inventada.
 */
CORESIP_API int CORESIP_GRS_Force_Ptt_Mute(int call, CORESIP_PttType PttType, unsigned short PttId, bool on, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	Try
	{
		if ((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID)
			SipCall::GRS_Force_Ptt_Mute(call & CORESIP_ID_MASK, PttType, PttId, on);
		else
		{
			ret = CORESIP_ERROR;
			if (error)
			{
				error->Code = 0;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "CORESIP_GRS_Force_Ptt_Mute: Invalid call id 0x%X", call);
			}
		}
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_GRS_Force_Ptt.
 */
CORESIP_API int CORESIP_GRS_Force_Ptt(int call, CORESIP_PttType PttType, unsigned short PttId, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	Try
	{
		if ((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID)
			SipCall::GRS_Force_Ptt(call & CORESIP_ID_MASK, PttType, PttId);
		else
		{
			ret = CORESIP_ERROR;
			if (error)
			{
				error->Code = 0;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "CORESIP_GRS_Force_Ptt_Mute: Invalid call id 0x%X", call);
			}
		}
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_GRS_Force_SCT. Como GRS Fuerza el bit SCT en el RTPRx enviado desde un GRS
 */
CORESIP_API int CORESIP_GRS_Force_SCT(int call, bool on, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	Try
	{
		if ((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID)
			SipCall::GRS_Force_SCT(call & CORESIP_ID_MASK, on);
		else
		{
			ret = CORESIP_ERROR;
			if (error)
			{
				error->Code = 0;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "CORESIP_GRS_Force_SCT: Invalid call id 0x%X", call);
			}
		}
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_Force_PTTS. Fuerza el bit PTTS en el RTPRx o RTPTx
 */
CORESIP_API int CORESIP_Force_PTTS(int call, bool on, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	Try
	{
		if ((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID)
			SipCall::Force_PTTS(call & CORESIP_ID_MASK, on);
		else
		{
			ret = CORESIP_ERROR;
			if (error)
			{
				error->Code = 0;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "CORESIP_Force_PTTS: Invalid call id 0x%X", call);
			}
		}
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_AddSndDevice		Añade un dispositvo de audio al módulo. @ref SipAgent::AddSndDevice
 */
CORESIP_API int CORESIP_AddSndDevice(const CORESIP_SndDeviceInfo * info, int * dev, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		*dev = (SipAgent::AddSndDevice(info) | CORESIP_SNDDEV_ID);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_CreateWavPlayer		Crea un 'Reproductor' WAV. @ref SipAgent::CreateWavPlayer
 */
CORESIP_API int CORESIP_CreateWavPlayer(const char * file, unsigned loop, int * wavPlayer, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		*wavPlayer = (SipAgent::CreateWavPlayer(file, loop != 0) | CORESIP_WAVPLAYER_ID);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_DestroyWavPlayer	Elimina un Reproductor WAV. @ref SipAgent::DestroyWavPlayer
 */
CORESIP_API int CORESIP_DestroyWavPlayer(int wavPlayer, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		//pj_assert((wavPlayer & CORESIP_ID_TYPE_MASK) == CORESIP_WAVPLAYER_ID);
		if((wavPlayer & CORESIP_ID_TYPE_MASK) == CORESIP_WAVPLAYER_ID)
			SipAgent::DestroyWavPlayer(wavPlayer & CORESIP_ID_MASK);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_CreateWavRecorder	Crea un 'grabador' en formato WAV. @ref SipAgent::CreateWavRecorder
 */
CORESIP_API int CORESIP_CreateWavRecorder(const char * file, int * wavRecorder, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		*wavRecorder = (SipAgent::CreateWavRecorder(file) | CORESIP_WAVRECORDER_ID);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_DestroyWavRecorder	Elimina un 'grabador' WAV. @ref SipAgent::DestroyWavRecorder
 */
CORESIP_API int CORESIP_DestroyWavRecorder(int wavRecorder, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		//pj_assert((wavRecorder & CORESIP_ID_TYPE_MASK) == CORESIP_WAVRECORDER_ID);
		if ((wavRecorder & CORESIP_ID_TYPE_MASK) == CORESIP_WAVRECORDER_ID)
			SipAgent::DestroyWavRecorder(wavRecorder & CORESIP_ID_MASK);
	}
	catch_all;

	return ret;
}


/**
 *	CORESIP_CreateRdRxPort		Crea un 'PORT' @ref RdRxPort de Recepcion Radio. @ref SipAgent::CreateRdRxPort
 */
CORESIP_API int CORESIP_CreateRdRxPort(const CORESIP_RdRxPortInfo * info, const char * localIp, int * rdRxPort, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		*rdRxPort = (SipAgent::CreateRdRxPort(info, localIp) | CORESIP_RDRXPORT_ID);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_DestroyRdRxPort		Elimina un Puerto @ref RdRxPort. @ref SipAgent::DestroyRdRxPort
 */
CORESIP_API int CORESIP_DestroyRdRxPort(int rdRxPort, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		pj_assert((rdRxPort & CORESIP_ID_TYPE_MASK) == CORESIP_RDRXPORT_ID);
		SipAgent::DestroyRdRxPort(rdRxPort & CORESIP_ID_MASK);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_CreateSndRxPort.	Crea un puerto @ref SoundRxPort. @ref SipAgent::CreateSndRxPort
 */
CORESIP_API int CORESIP_CreateSndRxPort(const char * id, int * sndRxPort, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		*sndRxPort = (SipAgent::CreateSndRxPort(id) | CORESIP_SNDRXPORT_ID);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_DestroySndRxPort	Eliminar un puerto @ref SoundRxPort. @ref SipAgent::DestroySndRxPort
 */
CORESIP_API int CORESIP_DestroySndRxPort(int sndRxPort, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		pj_assert((sndRxPort & CORESIP_ID_TYPE_MASK) == CORESIP_SNDRXPORT_ID);
		SipAgent::DestroySndRxPort(sndRxPort & CORESIP_ID_MASK);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_BridgeLink			Configura un enlace de conferencia. @ref SipAgent::BridgeLink
 */
CORESIP_API int CORESIP_BridgeLink(int src, int dst, int on, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		SipAgent::BridgeLink(src & CORESIP_ID_TYPE_MASK, src & CORESIP_ID_MASK, 
			dst & CORESIP_ID_TYPE_MASK, dst & CORESIP_ID_MASK, on != 0);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_SendToRemote		Configura El puerto de Sonido apuntado para los envios UNICAST de Audio. @ref SipAgent::SendToRemote
 */
CORESIP_API int CORESIP_SendToRemote(int dev, int on, const char * id, const char * ip, unsigned port, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		SipAgent::SendToRemote(dev & CORESIP_ID_TYPE_MASK, dev & CORESIP_ID_MASK, on != 0, id, ip, port);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_ReceiveFromRemote
 */
CORESIP_API int CORESIP_ReceiveFromRemote(const char * localIp, const char * mcastIp, unsigned mcastPort, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		SipAgent::ReceiveFromRemote(localIp, mcastIp, mcastPort);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_SetVolume
 */
CORESIP_API int CORESIP_SetVolume(int id, unsigned volume, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		SipAgent::SetVolume(id & CORESIP_ID_TYPE_MASK, id & CORESIP_ID_MASK, volume);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_GetVolume
 */
CORESIP_API int CORESIP_GetVolume(int id, unsigned * volume, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		*volume = SipAgent::GetVolume(id & CORESIP_ID_TYPE_MASK, id & CORESIP_ID_MASK);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_CallMake
 */
CORESIP_API int CORESIP_CallMake(const CORESIP_CallInfo * info, const CORESIP_CallOutInfo * outInfo, int * call, CORESIP_Error * error)
{
	int ret = CORESIP_OK;	

	Try
	{
		*call = (SipCall::New(info, outInfo) | CORESIP_CALL_ID);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_CallAnswer
 */
CORESIP_API int CORESIP_CallAnswer(int call, unsigned code, int addToConference, unsigned reason_code, char* reason_text, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		//pj_assert((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID);
		if ((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID)
			SipCall::Answer(call & CORESIP_ID_MASK, code, addToConference != 0, reason_code, reason_text);
		else
		{
			ret = CORESIP_ERROR;
			if (error)
			{
				error->Code = 0;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "CORESIP_CallAnswer: Invalid call id 0x%X", call);
			}
		}
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_CallMovedTemporallyAnswer
 */
CORESIP_API int CORESIP_CallMovedTemporallyAnswer(int call, const char *dst, const char *reason, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	Try
	{
		//pj_assert((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID);
		if ((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID)
			SipCall::MovedTemporallyAnswer(call & CORESIP_ID_MASK, dst, reason);
			//SipCall::MovedTemporallyAnswer(call & CORESIP_ID_MASK, "sip:314002@192.168.1.202", "busy");
		else
		{
			ret = CORESIP_ERROR;
			if (error)
			{
				error->Code = 0;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "CORESIP_MovedTemporallyAnswer: Invalid call id 0x%X", call);
			}
		}
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_CallProccessRedirect
 *	Esta funcion debe llamarse despues de recibirse la callback MovedTemporallyCb para 
 */
CORESIP_API int CORESIP_CallProccessRedirect(int call, const char *dstUri, CORESIP_REDIRECT_OP op, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	Try
	{
		if ((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID)
			SipCall::CallProccessRedirect(call & CORESIP_ID_MASK, dstUri, op);
		else
		{
			ret = CORESIP_ERROR;
			if (error)
			{
				error->Code = 0;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "CORESIP_CallProccessRedirect: Invalid call id 0x%X", call);
			}
		}
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_CallHangup
 */
CORESIP_API int CORESIP_CallHangup(int call, unsigned code, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		//pj_assert((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID);
		if ((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID)
			SipCall::Hangup(call & CORESIP_ID_MASK, code);
		else
		{
			ret = CORESIP_ERROR;
			if (error)
			{
				error->Code = 0;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "CORESIP_CallHangup: Invalid call id 0x%X", call);
			}
		}
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_CallHold
 */
CORESIP_API int CORESIP_CallHold(int call, int hold, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		//pj_assert((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID);
		if ((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID)
			SipCall::Hold(call & CORESIP_ID_MASK, hold != 0);
		else
		{
			ret = CORESIP_ERROR;
			if (error)
			{
				error->Code = 0;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "CORESIP_CallHold: Invalid call id 0x%X", call);
			}
		}
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_CallReinvite
 */
CORESIP_API int CORESIP_CallReinvite(int call, CORESIP_Error* error, int CallType_SDP, int TxRx_SDP, char* etm_vcs_bss_methods)
{
	int ret = CORESIP_OK;

	Try
	{
		pj_assert((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID);
		SipCall::Reinvite(call & CORESIP_ID_MASK, CallType_SDP, TxRx_SDP, etm_vcs_bss_methods);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_CallTransfer
 */
CORESIP_API int CORESIP_CallTransfer(int call, int dstCall, const char * dst, const char *display_name, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		//pj_assert((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID);
		if ((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID)
			SipCall::Transfer(call & CORESIP_ID_MASK, dstCall != PJSUA_INVALID_ID ? dstCall & CORESIP_ID_MASK : PJSUA_INVALID_ID, dst, display_name);
		else
		{
			ret = CORESIP_ERROR;
			if (error)
			{
				error->Code = 0;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "CORESIP_CallTransfer: Invalid call id 0x%X", call);
			}
		}
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_CallPtt
 */
CORESIP_API int CORESIP_CallPtt(int call, const CORESIP_PttInfo * info, CORESIP_Error * error)
{
	//UNIFETM: Esta funcion tiene que utilizarse en el ETM tambien para Squelch

	int ret = CORESIP_OK;

	Try
	{
		//pj_assert((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID);
		if ((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID)
			SipCall::Ptt(call & CORESIP_ID_MASK, info);
		else
		{
			ret = CORESIP_ERROR;
			if (error)
			{
				error->Code = 0;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "CORESIP_CallPtt: Invalid call id 0x%X", call);
			}
		}
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_CallPtt_Delayed
 */
CORESIP_API int CORESIP_CallPtt_Delayed(int call, const CORESIP_PttInfo* info, unsigned int delay_ms, CORESIP_Error* error)
{
	//UNIFETM: Esta funcion tiene que utilizarse en el ETM tambien para Squelch

	int ret = CORESIP_OK;

	Try
	{
		//pj_assert((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID);
		if ((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID)
			SipCall::Ptt(call & CORESIP_ID_MASK, info, delay_ms);
		else
		{
			ret = CORESIP_ERROR;
			if (error)
			{
				error->Code = 0;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "CORESIP_CallPtt: Invalid call id 0x%X", call);
			}
		}
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_CallSendInfo
 */
CORESIP_API int CORESIP_CallSendInfo(int call, const char * info, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{		
		//pj_assert((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID);
		if ((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID)
			SipCall::SendInfoMsg(call & CORESIP_ID_MASK, info);
		else
		{
			ret = CORESIP_ERROR;
			if (error)
			{
				error->Code = 0;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "CORESIP_CallSendInfo: Invalid call id 0x%X", call);
			}
		}
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_GetRdQidx
 */
CORESIP_API int	CORESIP_GetRdQidx(int call, int* Qidx, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	Try
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
				sprintf(error->Info, "CORESIP_GetRdResourceInfo: Invalid call id 0x%X", call);
			}
		}
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_CallConference
 */
CORESIP_API int CORESIP_CallConference(int call, int conf, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		//pj_assert((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID);
		if ((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID)
			SipCall::Conference(call & CORESIP_ID_MASK, conf != 0);
		else
		{
			ret = CORESIP_ERROR;
			if (error)
			{
				error->Code = 0;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "CORESIP_CallConference: Invalid call id 0x%X", call);
			}
		}
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_CallSendConfInfo
 */
CORESIP_API int CORESIP_CallSendConfInfo(int call, const CORESIP_ConfInfo * info, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		//pj_assert((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID);
		if ((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID)
			SipCall::SendConfInfo(call & CORESIP_ID_MASK, info);
		else
		{
			ret = CORESIP_ERROR;
			if (error)
			{
				error->Code = 0;
				strcpy(error->File, __FILE__);
				sprintf(error->Info, "CORESIP_CallSendConfInfo: Invalid call id 0x%X", call);
			}
		}
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_SendConfInfoFromAcc
 */
CORESIP_API int CORESIP_SendConfInfoFromAcc(int accId, const CORESIP_ConfInfo * info, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		pj_assert((accId & CORESIP_ID_TYPE_MASK) == CORESIP_ACC_ID);
		SubscriptionServer::SendConfInfoFromAcc(accId & CORESIP_ID_MASK, info);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_TransferAnswer
 */
CORESIP_API int CORESIP_TransferAnswer(const char * tsxKey, void * txData, void * evSub, unsigned code, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		SipCall::TransferAnswer(tsxKey, txData, evSub, code);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_TransferNotify
 */
CORESIP_API int CORESIP_TransferNotify(void * evSub, unsigned code, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		SipCall::TransferNotify(evSub, code);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_SendOptionsMsg
 *  Esta función no envia OPTIONS a traves del proxy
 */
CORESIP_API int CORESIP_SendOptionsMsg(const char * dst, char * callid, int isRadio, CORESIP_Error * error)
{
	//UNIFETM: Esta funcion tiene parametros diferentes. Cambiar en ETM para que sea como en ULISES

	int ret = CORESIP_OK;

	Try
	{
		OptionsMod::SendOptionsMsg(dst, callid, isRadio);
	}
	catch_all;

	return ret;
}

/**
 * CORESIP_SendOptionsCFWD.	...
 * Envia mensaje OPTIONS necesario para la negociacion Call Forward
 */
CORESIP_API int CORESIP_SendOptionsCFWD(int accId, const char* dst, CORESIP_CFWR_OPT_TYPE cfwr_options_type, const char* body, char* callid, pj_bool_t by_proxy, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	Try
	{
		pj_assert((accId & CORESIP_ID_TYPE_MASK) == CORESIP_ACC_ID);
		OptionsMod::SendOptionsCFWD(accId & CORESIP_ID_MASK, dst, cfwr_options_type, body, callid, by_proxy);
	}
	catch_all;

	return ret;
}

/**
 * CORESIP_SendResponseCFWD.	...
 * Envia la respuesta al options utilizado para la negociacion de call forward
 */
CORESIP_API int CORESIP_SendResponseCFWD(int st_code, const char* body, unsigned int hresp, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	Try
	{
		OptionsMod::SendResponseCFWD(st_code, body, hresp);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_SendOptionsMsgProxy
 *  Envia OPTIONS a traves del proxy.
 */
CORESIP_API int CORESIP_SendOptionsMsgProxy(const char * dst, char * callid, int isRadio, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		OptionsMod::SendOptionsMsg(dst, callid, isRadio, PJ_TRUE);
	}
	catch_all;

	return ret;
}

/** AGL. Para el SELCAL */
/**
 *	CORESIP_Wav2RemoteStart
 */
CORESIP_API int CORESIP_Wav2RemoteStart(const char *filename, const char * id, const char * ip, unsigned port, void (*eofCb)(void *), CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		/**
		WavPlayerToRemote *wp=new WavPlayerToRemote(filename, PTIME, eofCb);
		wp->Send2Remote(id, ip, port);
		*/
		SipAgent::CreateWavPlayer2Remote(filename, id, ip, port);
	}
	catch_all;
	return ret;
}

/**
 *	CORESIP_Wav2RemoteEnd
 */
CORESIP_API int CORESIP_Wav2RemoteEnd(void *obj, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		/**
		WavPlayerToRemote *wp = (WavPlayerToRemote *)obj;
		delete wp;
		*/
		SipAgent::DestroyWavPlayer2Remote();
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_RdPttEvent. Se llama cuando hay un evento de PTT en el HMI. Sirve sobretodo para enviar los metadata de grabacion VoIP en el puesto
 */
CORESIP_API int CORESIP_RdPttEvent(bool on, const char *freqId, int dev, CORESIP_Error * error, CORESIP_PttType PTT_type) 
{
	int ret = CORESIP_OK;

	Try
	{
		dev &= ~CORESIP_SNDDEV_ID;
		SipAgent::RdPttEvent(on, freqId, dev, PTT_type);		
	}
	catch_all;

	return ret;
}


/**
 *	CORESIP_RdSquEvent. Se llama cuando hay un evento de Squelch en el HMI. Sirve sobretodo para enviar los metadata de grabacion VoIP en el puesto
 */
CORESIP_API int CORESIP_RdSquEvent(bool on, const char *freqId, const char *resourceId, const char *bssMethod, unsigned int bssQidx, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		SipAgent::RdSquEvent(on, freqId, resourceId, bssMethod, bssQidx);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_RecorderCmd. Se pasan comandos para realizar acciones sobre el grabador VoIP
 */
CORESIP_API int CORESIP_RecorderCmd(CORESIP_RecCmdType cmd, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		ret = SipAgent::RecorderCmd(cmd, error);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_CreatePresenceSubscription. Crea una subscripcion por evento de presencia
 */
CORESIP_API int CORESIP_CreatePresenceSubscription(char *dest_uri, CORESIP_Error * error)
{
	int ret = CORESIP_OK;
	
	Try
	{
		ret = SipAgent::CreatePresenceSubscription(dest_uri);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_DestroyPresenceSubscription. destruye una subscripcion por evento de presencia
 */
CORESIP_API int CORESIP_DestroyPresenceSubscription(char *dest_uri, CORESIP_Error * error)
{
	int ret = CORESIP_OK;
	
	Try
	{
		ret = SipAgent::DestroyPresenceSubscription(dest_uri);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_CreateConferenceSubscription. Crea una subscripcion por evento de conferencia
 */
CORESIP_API int CORESIP_CreateConferenceSubscription(int accId, char *dest_uri, pj_bool_t by_proxy, CORESIP_Error * error)
{
	int ret = CORESIP_OK;
	
	Try
	{
		pj_assert((accId & CORESIP_ID_TYPE_MASK) == CORESIP_ACC_ID);
		ret = SipAgent::CreateConferenceSubscription(accId & CORESIP_ID_MASK, dest_uri, by_proxy);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_DestroyConferenceSubscription. Destruye una subscripcion por evento de conferencia
 */
CORESIP_API int CORESIP_DestroyConferenceSubscription(char *dest_uri, CORESIP_Error * error)
{
	int ret = CORESIP_OK;
	
	Try
	{
		ret = SipAgent::DestroyConferenceSubscription(dest_uri);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_CreateDialogSubscription. Crea una subscripcion por evento de Dialogo
 */
CORESIP_API int CORESIP_CreateDialogSubscription(int accId, char *dest_uri, pj_bool_t by_proxy, CORESIP_Error * error)
{
	int ret = CORESIP_OK;
	
	Try
	{
		pj_assert((accId & CORESIP_ID_TYPE_MASK) == CORESIP_ACC_ID);
		ret = SipAgent::CreateDialogSubscription(accId & CORESIP_ID_MASK, dest_uri, by_proxy);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_DestroyDialogSubscription. Destruye una subscripcion por evento de Dialogo
 */
CORESIP_API int CORESIP_DestroyDialogSubscription(char* dest_uri, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	Try
	{
		ret = SipAgent::DestroyDialogSubscription(dest_uri);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_SendWG67Subscription. Crea y envia una subscripcion por evento WG67KEY-IN
 */
CORESIP_API int CORESIP_SendWG67Subscription(int accId, char* dest_uri, int expires, int noRefresh, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	Try
	{
		pjsua_acc_id pj_acc_id = PJSUA_INVALID_ID;
		if (accId < 0)
		{
			pj_acc_id = PJSUA_INVALID_ID;
		}
		else
		{
			pj_assert((accId & CORESIP_ID_TYPE_MASK) == CORESIP_ACC_ID);
			pj_acc_id = accId & CORESIP_ID_MASK;
		}

		ret = WG67Subs::SendWG67Subscription(pj_acc_id, -1, dest_uri, expires, (pj_bool_t) noRefresh, PJ_FALSE);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_SetWG67SubscriptionParameters. Modifica parametros de la subscripcion por evento WG67KE-IN. 
 */
CORESIP_API int CORESIP_SetWG67SubscriptionParameters(int accId, char* dest_uri, int noRefresh, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	Try
	{
		pjsua_acc_id pj_acc_id;
		if (accId < 0)
		{
			pj_acc_id = PJSUA_INVALID_ID;
		}
		else
		{
			pj_assert((accId & CORESIP_ID_TYPE_MASK) == CORESIP_ACC_ID);
			pj_acc_id = accId & CORESIP_ID_MASK;
		}

		ret = WG67Subs::SetWG67SubscriptionParameters(pj_acc_id, -1, dest_uri, noRefresh);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_Set_WG67_Notifier_Parameters. Configura algunos parametros del WG67 notifier
 */
CORESIP_API int CORESIP_Set_WG67_Notifier_Parameters(int notify_enabled, int manual, int minimum_expires, int maximum_expires, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	Try
	{
		ret = WG67Subs::Set_WG67_Notifier_Parameters(notify_enabled, manual, minimum_expires, maximum_expires);
	}
	catch_all;

	return ret;
}

/**
 * CORESIP_Set_WG67_notify_status: Establece el estado de la suscripcion y las sesiones ficticias, ademas de enviar el Notify correspondiente. 
 */
CORESIP_API int CORESIP_Set_WG67_notify_status(int accId, char* subscriberUri, CORESIP_WG67Notify_SubscriptionState_Config* subsState,
	CORESIP_WG67Notify_Body_Config* wG67Notify_Body, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	Try
	{
		pjsua_acc_id pj_acc_id;
		if (accId < 0)
		{
			pj_acc_id = PJSUA_INVALID_ID;
		}
		else
		{
			pj_assert((accId & CORESIP_ID_TYPE_MASK) == CORESIP_ACC_ID);
			pj_acc_id = accId & CORESIP_ID_MASK;
		}

		ret = WG67Subs::Set_WG67_notify_status(pj_acc_id, subscriberUri, subsState, wG67Notify_Body);
	}
	catch_all;

	return ret;
}

/**
 * CORESIP_Get_GRS_WG67SubscriptionsList.
 */
CORESIP_API int CORESIP_Get_GRS_WG67SubscriptionsList(int accId, int* nSubscriptions, CORESIP_WG67_Subscription_Info* WG67Subscriptions[], CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	Try
	{
		pjsua_acc_id pj_acc_id;
		if (accId < 0)
		{
			pj_acc_id = PJSUA_INVALID_ID;
		}
		else
		{
			pj_assert((accId & CORESIP_ID_TYPE_MASK) == CORESIP_ACC_ID);
			pj_acc_id = accId & CORESIP_ID_MASK;
		}

		ret = SubscriptionServer::GetWG67SubscriptionList(pj_acc_id, nSubscriptions, WG67Subscriptions);
	}
	catch_all;

	return ret;
}


/**
 *	CORESIP_CreateGenericPort		Crea un puerto generico de media
 */
CORESIP_API int CORESIP_CreateGenericPort(int* gen_port_id, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	Try
	{
		*gen_port_id = (GenericPort::CreateGenericPort() | CORESIP_GENPORT_ID);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_DestroyGenericPort		Destruye un puerto generico de media
 */
CORESIP_API int CORESIP_DestroyGenericPort(int gen_port_id, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	Try
	{
		pj_assert((gen_port_id & CORESIP_ID_TYPE_MASK) == CORESIP_GENPORT_ID);
		GenericPort::DestroyGenericPort(gen_port_id & CORESIP_ID_MASK);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_PutInGenericPort: Pone muestras en el generic port.
 */
CORESIP_API int CORESIP_PutInGenericPort(int gen_port_id, char* buffer, int buffer_length, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	Try
	{
		pj_assert((gen_port_id & CORESIP_ID_TYPE_MASK) == CORESIP_GENPORT_ID);
		GenericPort::PutInGenericPort(gen_port_id & CORESIP_ID_MASK, buffer, buffer_length);
	}
	catch_all;

	return ret;
}

/**
*CORESIP_GetFromGenericPort: Toma muestras del generic port
*/
CORESIP_API int CORESIP_GetFromGenericPort(int gen_port_id, char* buffer, int buffer_length, CORESIP_Error * error)
{
	int ret = CORESIP_OK;

	Try
	{
		pj_assert((gen_port_id & CORESIP_ID_TYPE_MASK) == CORESIP_GENPORT_ID);
		GenericPort::GetFromGenericPort(gen_port_id & CORESIP_ID_MASK, buffer, buffer_length);
	}
	catch_all;

	return ret;
}

/**
 * CORESIP_SendInstantMessage. Envia un mensaje instantaneo
 */
CORESIP_API int CORESIP_SendInstantMessage(int acc_id, char *dest_uri, char *text, pj_bool_t by_proxy, CORESIP_Error * error)
{
	int ret = CORESIP_OK;
	
	Try
	{
		pj_assert((acc_id & CORESIP_ID_TYPE_MASK) == CORESIP_ACC_ID);
		ret = SipAgent::SendInstantMessage(acc_id & CORESIP_ID_MASK, dest_uri, text, by_proxy);
	}
	catch_all;

	return ret;
}

/**
 * CORESIP_EchoCancellerLCMic.	...
 * Activa/desactiva cancelador de eco altavoz LC y Microfonos. Sirve para el modo manos libres 
 */
CORESIP_API int CORESIP_EchoCancellerLCMic(bool on, CORESIP_Error * error)
{
	int ret = CORESIP_OK;
	
	Try
	{
		ret = SipAgent::EchoCancellerLCMic(on);
	}
	catch_all;

	return ret;
}

CORESIP_API int CORESIP_SetImpairments(int call, CORESIP_Impairments * impairments, CORESIP_Error * error)
{
	int ret = CORESIP_OK;
	Try
	{
		pj_assert((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID);
		SipCall::SetImpairments(call & CORESIP_ID_MASK, impairments);
	}
	catch_all;

	return ret;
}

CORESIP_API int CORESIP_SetCallParameters(int call, int *disableKeepAlives, int* forced_cld, CORESIP_Error* error)
{
	int ret = CORESIP_OK;
	Try
	{
		pj_assert((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID);
		SipCall::SetCallParameters(call & CORESIP_ID_MASK, disableKeepAlives, forced_cld);
	}
	catch_all;

	return ret;
}

/**
 * CORESIP_SetConfirmPtt.	...
 */
CORESIP_API int CORESIP_SetConfirmPtt(int call, pj_bool_t val, CORESIP_Error* error)
{
	int ret = CORESIP_OK;
	Try
	{
		pj_assert((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID);
		SipCall::SetConfirmPtt(call & CORESIP_ID_MASK, val);
	}
	catch_all;

	return ret;
}

/**
 * CORESIP_GetNetworkDelay.	...
 */
CORESIP_API int CORESIP_GetNetworkDelay(int call, unsigned int* delay_ms, CORESIP_Error* error)
{
	int ret = CORESIP_OK;
	Try
	{
		pj_assert((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID);
		SipCall::GetNetworkDelay(call & CORESIP_ID_MASK, delay_ms);
	}
	catch_all;

	return ret;
}

/**
 * SendToneToCall.	...
 */
CORESIP_API int CORESIP_SendToneToCall(int call, unsigned int frequency, float volume_dbm0, int on, CORESIP_Error* error)
{
	int ret = CORESIP_OK;
	Try
	{
		pj_assert((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID);
		SignalGen::SendToneToCall(call & CORESIP_ID_MASK, frequency, volume_dbm0, on);
	}
	catch_all;

	return ret;
}

/**
 * CORESIP_SendNoiseToCall.	...
 */
CORESIP_API int CORESIP_SendNoiseToCall(int call, float volume_dbm0, int on, CORESIP_Error* error)
{
	int ret = CORESIP_OK;
	Try
	{
		pj_assert((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID);
		SignalGen::SendNoiseToCall(call & CORESIP_ID_MASK, NoiseGenerator::White, volume_dbm0, on);
	}
	catch_all;

	return ret;
}

/**
 * CORESIP_SendPinkNoiseToCall.	...
 */
CORESIP_API int CORESIP_SendPinkNoiseToCall(int call, float volume_dbm0, int on, CORESIP_Error* error)
{
	int ret = CORESIP_OK;
	Try
	{
		pj_assert((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID);
		SignalGen::SendNoiseToCall(call & CORESIP_ID_MASK, NoiseGenerator::Pink, volume_dbm0, on);
	}
	catch_all;

	return ret;
}

/**
 * SendDTMF.	...
 */
CORESIP_API int CORESIP_SendDTMF(int call, const CORESIP_tone_digit_map* digit_map, unsigned count, const CORESIP_tone_digit digits[], float volume_dbm0, CORESIP_Error* error)
{
	int ret = CORESIP_OK;
	Try
	{
		pj_assert((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID);
		SignalGen::SendDTMF(call & CORESIP_ID_MASK, digit_map, count, digits, volume_dbm0);
	}
	catch_all;

	return ret;
}

/**
 *	CORESIP_SendSELCAL
 */
CORESIP_API int CORESIP_SendSELCAL(int call, const char* selcalValue, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	Try
	{
		pj_assert((call & CORESIP_ID_TYPE_MASK) == CORESIP_CALL_ID);
		SipCall::SendSELCAL(call & CORESIP_ID_MASK, selcalValue);
	}
	catch_all;

	return ret;
}

CORESIP_API int CORESIP_SetSNDDeviceWindowsName(CORESIP_SndDevType UlisesDev, const char* DevWinName, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	Try
	{
		SoundDevHw::SetSNDDeviceWindowsName(UlisesDev, DevWinName);
	}
	catch_all;

	return ret;
}

CORESIP_API int CORESIP_GetWindowsSoundDeviceNames(int captureType, CORESIP_SndWindowsDevices* Devices, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	Try
	{
		SoundDevHw::GetWindowsSoundDeviceNames(captureType, Devices, PJ_FALSE, NULL);
	}
	catch_all;

	return ret;
}

CORESIP_API int CORESIP_SetVolumeOutputDevice(CORESIP_SndDevType dev, unsigned int volume, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	Try
	{
		pj_status_t st = SoundDevHw::SetVolumeOutputDevice(dev, volume);
		PJ_CHECK_STATUS(st, ("ERROR CORESIP_SetVolumeOutputDevice. Ver logs"));
	}
	catch_all;

	return ret;
}

CORESIP_API int CORESIP_GetVolumeOutputDevice(CORESIP_SndDevType dev, unsigned int *volume, CORESIP_Error* error)
{
	int ret = CORESIP_OK;

	Try
	{
		pj_status_t st = SoundDevHw::GetVolumeOutputDevice(dev, volume);
		PJ_CHECK_STATUS(st, ("ERROR CORESIP_GetVolumeOutputDevice. Ver logs"));
	}
	catch_all;

	return ret;
}


/*@}*/