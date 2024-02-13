#include "Global.h"
#include "Exceptions.h"
#include "CUtils.h"

CORESIP_HMI_Resources_Info CUtils::Resources_Info;
pj_mutex_t* CUtils::Resources_Info_mutex = NULL;

void CUtils::Init(pj_pool_t* pool)
{
	Resources_Info.NumResources = 0;
	pj_status_t st = pj_mutex_create_simple(pool, "RecActionsMtx", &Resources_Info_mutex);
	if (st != PJ_SUCCESS) Resources_Info_mutex = NULL;
	PJ_CHECK_STATUS(st, ("ERROR creando mutex estatico de RecordPort"));
}

void CUtils::End()
{
	if (Resources_Info_mutex != NULL)
	{
		pj_mutex_destroy(Resources_Info_mutex);
		Resources_Info_mutex = NULL;
	}
	Resources_Info.NumResources = 0;
}

int CUtils::GetUriElements(pj_str_t *uriToParse, char* user, int len_user_buf, char* host, int len_host_buf, int *port)
{
	pj_pool_t* tmppool = pjsua_pool_create("CUtils::GetUriElements", 64, 32);
	if (tmppool == NULL)
	{
		PJ_LOG(3, (__FILE__, "ERROR: Memoria insuficiente en CUtils::GetUriElements"));
		return -1;
	}

	pjsip_uri* uri = pjsip_parse_uri(tmppool, uriToParse->ptr, uriToParse->slen, 0);
	if (uri == NULL)
	{
		PJ_LOG(3, (__FILE__, "ERROR: No se puede parsear la Uri"));
		pj_pool_release(tmppool);
		return -1;
	}

	int ret = 0;

	/*Se extrae user y domain (host)*/
	pjsip_sip_uri* url = (pjsip_sip_uri*)pjsip_uri_get_uri(uri);
	
	if (user != NULL)
	{
		if (len_user_buf > url->user.slen)
		{
			pj_ansi_strncpy(user, url->user.ptr, url->user.slen);
			user[url->user.slen] = '\0';
		}
		else
		{
			ret = -1;
		}
	}

	if (host != NULL)
	{
		if (len_host_buf > url->host.slen)
		{
			pj_ansi_strncpy(host, url->host.ptr, url->host.slen);
			user[url->host.slen] = '\0';
		}
		else
		{
			ret = -1;
		}
	}

	if (port != NULL) *port = url->port;
	
	pj_pool_release(tmppool);

	return ret;
}



void CUtils::Set_HMI_Resources_Info(CORESIP_HMI_Resources_Info* resources_Info)
{
	pj_mutex_lock(Resources_Info_mutex);

	pj_memcpy(&Resources_Info, resources_Info, sizeof(CORESIP_HMI_Resources_Info));

	pj_mutex_unlock(Resources_Info_mutex);
}

pj_bool_t CUtils::Get_HMI_Resource_Info(char *Id, CORESIP_Resource_Type Type, pj_bool_t *Secure)
{
	pj_assert(Id != NULL);

	pj_bool_t found = PJ_FALSE;

	pj_mutex_lock(Resources_Info_mutex);

	for (int i = 0; i < Resources_Info.NumResources; i++)
	{
		if (pj_ansi_strcmp(Resources_Info.HMIResources[i].Id, Id) == 0 && 
			Resources_Info.HMIResources[i].Type == Type)
		{
			found = PJ_TRUE;
			if (Secure != NULL) *Secure = Resources_Info.HMIResources[i].Secure;
			break;
		}
	}

	pj_mutex_unlock(Resources_Info_mutex);
	return found;
}




