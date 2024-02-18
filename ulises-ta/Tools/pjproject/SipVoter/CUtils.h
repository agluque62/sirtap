#ifndef __CORESIP_UTILS_H__
#define __CORESIP_UTILS_H__

class CUtils
{
public:
	static void Init(pj_pool_t* pool);
	static void End();
	static int GetUriElements(const pj_str_t* uriToParse, char* user, int len_user_buf, char* host, int len_host_buf, int* port);
	static void Set_HMI_Resources_Info(CORESIP_HMI_Resources_Info* resources_Info);
	static pj_bool_t Get_HMI_Resource_Info(char* Id, CORESIP_Resource_Type Type, pj_bool_t* Secure);

private:
	static CORESIP_HMI_Resources_Info Resources_Info;
	static pj_mutex_t* Resources_Info_mutex;
};

#endif