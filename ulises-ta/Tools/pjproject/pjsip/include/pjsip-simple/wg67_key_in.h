#ifndef __PJSIP_WG67_KEY_IN_H__
#define __PJSIP_WG67_KEY_IN_H__

/**
 * @file wg67_key_in.h
 * @brief SIP WG67 KEY-IN
 */
#include <pjsip/sip_uri.h>
#include <pjsip-simple/evsub.h>


PJ_BEGIN_DECL


/**
* Maximum wg67 status info.
*/
#define PJSIP_WG67_STATUS_MAX_INFO  25


/**
* This structure describes wg67 status.
*/
struct pjsip_wg67_status
{
	unsigned info_cnt;	/**< Number of info in the status.  */

	pj_str_t fid;
	struct 
	{
		unsigned short pttid;
		pj_str_t uri;
		pj_str_t sessiontype;

	} info[PJSIP_WG67_STATUS_MAX_INFO];	/**< Array of info.		    */

	pj_bool_t _is_valid;	/**< Internal flag.		    */
};


/**
* @see pjsip_wg67_status
*/
typedef struct pjsip_wg67_status pjsip_wg67_status;


/**
 * Initialize the WG67 KEY-IN module and register it as endpoint module and
 * package to the event subscription module.
 *
 * @param endpt		The endpoint instance.
 *
 * @return		PJ_SUCCESS if the module is successfully 
 *			initialized and registered to both endpoint
 *			and the event subscription module.
 */
PJ_DECL(pj_status_t) pjsip_wg67_init_module(pjsip_endpoint *endpt);

PJ_DECL(pj_status_t) pjsip_wg67_deinit_module(pjsip_endpoint* endpt);


/**
 * Get the WG67 KEY-IN module instance.
 *
 * @return		The WG67 KEY-IN module instance.
 */
PJ_DECL(pjsip_module*) pjsip_wg67_instance(void);


/**
 * Create WG67 KEY-IN client subscription session.
 *
 * @param dlg		The underlying dialog to use.
 * @param user_cb	Pointer to callbacks to receive WG67 KEY-IN subscription
 *			events.
 * @param options	Option flags. Currently only PJSIP_EVSUB_NO_EVENT_ID
 *			is recognized.
 * @param p_evsub	Pointer to receive the WG67 KEY-IN subscription
 *			session.
 *
 * @return		PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t) pjsip_wg67_create_uac( pjsip_dialog *dlg,
					    const pjsip_evsub_user *user_cb,
					    unsigned options,
					    pjsip_evsub **p_evsub );

/**
* Create server subscription session.
*
* @param dlg		The underlying dialog to use.
* @param user_cb	Pointer to callbacks to receive subscription events.
* @param rdata		The incoming SUBSCRIBE request that creates the event subscription.
* @param p_evsub	Pointer to receive the subscription session.
*
* @return		PJ_SUCCESS on success.
*/
PJ_DECL(pj_status_t) pjsip_wg67_create_uas(pjsip_dialog* dlg,	const pjsip_evsub_user* user_cb, pjsip_rx_data* rdata, pjsip_evsub** p_evsub);


/**
 * Forcefully destroy the WG67 KEY-IN subscription. This function should only
 * be called on special condition, such as when the subscription 
 * initialization has failed. For other conditions, application MUST terminate
 * the subscription by sending the appropriate un(SUBSCRIBE) or NOTIFY.
 *
 * @param sub		The WG67 KEY-IN subscription.
 * @param notify	Specify whether the state notification callback
 *			should be called.
 *
 * @return		PJ_SUCCESS if subscription session has been destroyed.
 */
PJ_DECL(pj_status_t) pjsip_wg67_terminate( pjsip_evsub *sub,
					   pj_bool_t notify );



/**
 * Call this function to create request to initiate WG67 KEY-IN subscription, to 
 * refresh subcription, or to request subscription termination.
 *
 * @param sub		Client subscription instance.
 * @param expires	Subscription expiration. If the value is set to zero,
 *			this will request unsubscription.
 * @param p_tdata	Pointer to receive the request.
 *
 * @return		PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t) pjsip_wg67_initiate( pjsip_evsub *sub,
					  pj_int32_t expires,
					  pjsip_tx_data **p_tdata);

/**
* Accept the incoming subscription request by sending 2xx response to
* incoming SUBSCRIBE request.
*
* @param sub		Server subscription instance.
* @param rdata		The incoming subscription request message.
* @param st_code	Status code, which MUST be final response.
* @param hdr_list	Optional list of headers to be added in the response.
*
* @return		PJ_SUCCESS on success.
*/
PJ_DECL(pj_status_t) pjsip_wg67_accept(pjsip_evsub* sub, pjsip_rx_data* rdata, int st_code, const pjsip_hdr* hdr_list);

/**
 * Send request message that was previously created with initiate(), notify(),
 * or current_notify(). Application may also send request created with other
 * functions, e.g. authentication. But the request MUST be either request
 * that creates/refresh subscription or NOTIFY request.
 *
 * @param sub		The subscription object.
 * @param tdata		Request message to be sent.
 *
 * @return		PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t) pjsip_wg67_send_request( pjsip_evsub *sub,
					      pjsip_tx_data *tdata );


/**
 * Get the WG67 KEY-IN status. Client normally would call this function
 * after receiving NOTIFY request from server.
 *
 * @param sub		The client or server subscription.
 * @param status	The structure to receive WG67 KEY-IN status.
 *
 * @return		PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t) pjsip_wg67_get_status( pjsip_evsub *sub,
					    pjsip_wg67_status *status );


/**
 * This is a utility function to parse body into WG67 KEY-IN status.
 *
 * @param rdata		The incoming SIP message containing the body.
 * @param pool		Pool to allocate memory to copy the strings into
 *			the presence status structure.
 * @param status	The presence status to be initialized.
 *
 * @return		PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t) pjsip_wg67_parse_body(pjsip_rx_data *rdata,
					   pj_pool_t *pool,
					   pjsip_wg67_status *status);

/*
 *  search_wg67_subscription_in_dialog
 * Busca una subscripcion al avento WG67KEY-IN en un dialogo sip
 * @param dlg. Dialogo
 * Return. Puntero a la subscripcion. NULL si no hay ninguna.
 */
PJ_DECL(pjsip_evsub*) search_wg67_subscription_in_dialog(pjsip_dialog* dlg);


/**
 * For notifier, create NOTIFY request to subscriber, and set the state
 * of the subscription. Application MUST set the status to the
 * appropriate state (by calling #pjsip_pres_set_status()) before calling
 * this function.
 *
 * @param sub		The server subscription (notifier) instance.
 * @param force_subs_state	If true then the Notify is sent with the following parameters regardless the state
 * @param expires	If it is 0 or greater. Set de expires value in the notify. 
 * @param state		New state to set. Is ignored if force_subs_state is true
 * @param state_str	The state string name, if state contains value other
 *			than active, pending, or terminated. Otherwise this
 *			argument is ignored.
 * @param reason	Specify reason if new state is terminated, otherwise
 *			put NULL.
 * @param body		body in notify
 * @param retry_after		Retry-after value
 * @param p_tdata	Pointer to receive the request.
 *
 * @return		PJ_SUCCESS on success.
 */
PJ_DEF(pj_status_t) pjsip_wg67_notify(pjsip_evsub* sub,
										pj_bool_t force_subs_state,			//If true then the Notify is sent forcing state with the following parameters
										int expires,				//Value -1 if it is not required
										pjsip_evsub_state state,	//Is ignored if force_subs_state is true
										const pj_str_t* state_str,	//Value NULL if it is not required
										const pj_str_t* reason,		//Value NULL if it is not required
										const int retry_after,		//Value -1 if it is not required
										char* body,
										const pj_str_t* user_agent	//Identificador del user-agent
										);


/**
 * Create NOTIFY request to reflect current subscription status.
 *
 * @param sub		Server subscription object.
 * @param body		body in notify
 * @param user_agent		//Identificador del user-agent
 *
 * @return		PJ_SUCCESS on success.
 */
PJ_DECL(pj_status_t) pjsip_wg67_current_notify(pjsip_evsub* sub, char *body, const pj_str_t* user_agent);


/**
 * @}
 */

PJ_END_DECL


#endif	/* __PJSIP_WG67_KEY_IN_H__ */
