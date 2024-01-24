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
	static const int MAX_RTP_PORTS = CORESIP_MAX_RTP_PORTS;
	static RTPport* _RTP_Ports[MAX_RTP_PORTS];
	pjsua_conf_port_id _Slot;		//su Slot de la conferencia pjmedia

public:
	static int CreateRTPport(char* dst_ip, int src_port, int dst_port, char* local_multicast_ip, int payload_type, CORESIP_actions action);
	static void PauseResumeDestroyRTPport(int port_id, CORESIP_actions action);
	static void DestroyAllRTPports();	
	static void OnRTP_Received(void* stream, void* frame, void* codec, unsigned seq, pj_uint32_t rtp_ext_info);
	
	RTPport(int rtp_port_id);
	~RTPport();
	int Init(char* dst_ip, int src_port, int dst_port, char* local_multicast_ip, pjmedia_rtp_pt payload_type, CORESIP_actions action);
	void RTP_Received(void* stream, void* frame, void* codec, unsigned seq, pj_uint32_t rtp_ext_info);		
	
private:
	static const int RTP_Timeout_TIME_seg = 0;
	static const int RTP_Timeout_TIME_ms = 250;
	
	int RTPport_id;
	pj_pool_t * _Pool;
	pj_lock_t* _Lock;
	pjmedia_port* _Port;
	pjmedia_transport* Transport;
	pjmedia_stream* Stream;
	int ReceivingRTP;

	pj_timer_entry RTP_Timeout_timer;		//Timer para el timeout de decisión de que no se recibe RTP.

private:

	void Dispose();
	int Pause(CORESIP_actions action);
	int Resume(CORESIP_actions action);
	static void on_RTP_Timeout_timer(pj_timer_heap_t* th, pj_timer_entry* te);
	
};


#endif

/*@}*/

