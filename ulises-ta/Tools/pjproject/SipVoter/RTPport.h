/**
 * @file RTPport.cpp
 * @brief Gestion de un 'puerto' Para enviar y recibir un flujo de RTP
 *	@addtogroup CORESIP
 */
/*@{*/

#ifndef __CORESIP_RTPPORT_H__
#define __CORESIP_RTPPORT_H__

#include "Global.h"

class RTPport
{
public:
	static const int MAX_RTP_PORTS = CORESIP_MAX_SOUND_RX_PORTS;
	static RTPport* _RTP_Ports[MAX_RTP_PORTS];
	pjsua_conf_port_id _Slot;		//su Slot de la conferencia pjmedia

public:
	static int CreateRTPport(char* dst_ip, int src_port, int dst_port, int payload_type);
	static void DestroyRTPport(int gen_port_id);
	
	RTPport();
	~RTPport();
	int Init(char* dst_ip, int src_port, int dst_port, pjmedia_rtp_pt payload_type);
	
private:
	
	static pj_status_t GetFrame(pjmedia_port* port, pjmedia_frame* frame);
	static pj_status_t Reset(pjmedia_port* this_port);
	static pj_status_t PutFrame(pjmedia_port* port, const pjmedia_frame* frame);
		
	pj_pool_t * _Pool;
	pjmedia_port _Port;	
	pjmedia_transport* Transport;
	pjmedia_stream* Stream;

private:

	void Dispose();
	
};


#endif

/*@}*/

