/**
 * @file OptionsMod.h
 * @brief Gestion de los mensajes OPTIONS.
 *
 *	Implementa la clase 'OptionsMod'.
 *
 *	@addtogroup CORESIP
 */
 /*@{*/

#ifndef __CORESIP_OPTIONSMOD_H__
#define __CORESIP_OPTIONSMOD_H__

class OptionsMod
{
public:

	typedef enum OptionsType
	{
		NORMAL,
		CALLFORWARD
	} OptionsType;

	static const char* C_TYPE_CFWR;
	static const char* C_SUBTYPE_CFWR_REQUEST;
	static const char* C_SUBTYPE_CFWR_RELEASE;
	static const char* C_SUBTYPE_CFWR_UPDATE;
	static const char* C_TYPESUBTYPE_CFWR_REQUEST;
	static const char* C_TYPESUBTYPE_CFWR_RELEASE;
	static const char* C_TYPESUBTYPE_CFWR_UPDATE;

	static pj_status_t RegisterModule();
	static void SendOptionsMsg(const char* target, char* callid, int isRadio, pj_bool_t by_proxy = PJ_FALSE);
	static void SendOptionsCFWD(int accId, const char* target, CORESIP_CFWR_OPT_TYPE cfwr_options_type, const char* body, char* callid, pj_bool_t by_proxy = PJ_FALSE);
	static void SendResponseCFWD(int st_code, const char* body, unsigned int hresp);
	static pj_bool_t OnRxRequest(pjsip_rx_data* rdata);
	static pj_bool_t OnRxResponse(pjsip_rx_data* rdata);

private:

	/*Para almacenar los datos necesarios para poder enviar una respuesta desde la aplicacion*/
	struct response
	{
		pjsip_response_addr	 res_addr;
		pjsip_tx_data	*tdata;
	};

	static void send_CFWD_callback(void *token, pjsip_event *e);
	static pj_bool_t ProcessOptCfwrRequest(pjsip_rx_data* rdata);

};


#endif

