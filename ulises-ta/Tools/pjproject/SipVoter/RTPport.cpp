#include "Global.h"
#include "Exceptions.h"
#include "Guard.h"
#include "RTPport.h"
#include "SipAgent.h"

RTPport* RTPport::_RTP_Ports[MAX_RTP_PORTS];

RTPport::RTPport(int rtp_port_id)
{
	RTPport_id = rtp_port_id;
	_Pool = NULL;
	_Lock = NULL;
	_Port = NULL;
	_Slot = PJSUA_INVALID_ID;
	Transport = NULL;
	Stream = NULL;
	ReceivingRTP = -1;
}

int RTPport::Init(char* dst_ip, int src_port, int dst_port, char* local_multicast_ip, pjmedia_rtp_pt payload_type, CORESIP_RTP_port_actions action)
{
	pjmedia_dir dir;
	switch (action)
	{
	case CORESIP_CREATE_ENCODING:
		dir = PJMEDIA_DIR_ENCODING;
		break;
	case CORESIP_CREATE_DECODING:
		dir = PJMEDIA_DIR_DECODING;
		break;
	case CORESIP_CREATE_ENCODING_DECODING:
		dir = PJMEDIA_DIR_ENCODING_DECODING;
		break;
	default:
		PJ_LOG(3, (__FILE__, "ERROR: RTPport::Init invalid action"));
		return -1;
	}

	pj_status_t st = PJ_SUCCESS;
	if (_Pool != NULL || _Slot != PJSUA_INVALID_ID)
	{
		PJ_LOG(3, (__FILE__, "ERROR: RTPport::Init Already initialized"));
		return -1;
	}

	_Pool = pjsua_pool_create(NULL, 1024, 512);
	if (_Pool == NULL)
	{
		PJ_LOG(3, (__FILE__, "ERROR: RTPport::Init Not enough memory"));
		return -1;
	}

	st = pj_lock_create_recursive_mutex(_Pool, NULL, &_Lock);
	if (st != PJ_SUCCESS)
	{
		PJ_LOG(3, (__FILE__, "ERROR: RTPport::Init _Lock cannot be created"));
		return -1;
	}

	pj_sockaddr_in rem_addr;
	st = pj_sockaddr_in_init(&rem_addr, &pj_str(dst_ip), dst_port);
	if (st != PJ_SUCCESS)
	{
		PJ_LOG(3, (__FILE__, "ERROR: RTPport::Init IP address or dst_port is invalid"));
		return -1;
	}	

	pjmedia_stream_info info;		

	/* Reset stream info. */
	pj_bzero(&info, sizeof(info));

	pj_str_t codec_id;
	
	if (payload_type == PJMEDIA_RTP_PT_PCMA)
	{
		codec_id = pj_str("PCMA");
	}
	else
	{
		codec_id = pj_str("PCMU");
	}
	
	/* Initialize stream info formats */
	info.type = PJMEDIA_TYPE_AUDIO;
	info.proto = PJMEDIA_TP_PROTO_RTP_AVP;
	info.dir = dir;
	//pj_memcpy(&info.fmt, &codec_info, sizeof(pjmedia_codec_info));
	info.ssrc = pj_rand();

	/* Copy remote address */
	pj_memcpy(&info.rem_addr, &rem_addr, sizeof(pj_sockaddr_in));

	/* Get the codec info and param */
	const pjmedia_codec_info* ci;
	pjmedia_codec_mgr* cm = pjmedia_endpt_get_codec_mgr(pjsua_var.med_endpt);
	unsigned int count = 1;
	st = pjmedia_codec_mgr_find_codecs_by_id(cm, &codec_id, &count, &ci, NULL);
	if (st != PJ_SUCCESS) {
		PJ_LOG(3, (__FILE__, "ERROR: RTPport::Init codec id cannot be found"));
		return -1;
	}

	pj_memcpy(&info.fmt, ci, sizeof(*ci));

	info.param = PJ_POOL_ALLOC_T(_Pool, struct pjmedia_codec_param);
	st = pjmedia_codec_mgr_get_default_param(cm, &info.fmt, info.param);
	if (st != PJ_SUCCESS) {
		PJ_LOG(3, (__FILE__, "ERROR: RTPport::Init UDP default param cannot be get"));
		return -1;
	}

	info.tx_pt = info.fmt.pt;

	/* Create media transport */
	pj_str_t src_addr = pj_str(SipAgent::uaIpAdd);
	st = pjmedia_transport_udp_create2(pjsua_var.med_endpt, NULL, &src_addr, src_port, PJMEDIA_UDP_NO_RTCP, &Transport);
	if (st != PJ_SUCCESS)
	{
		Transport = NULL;
		PJ_LOG(3, (__FILE__, "ERROR: RTPport::Init UDP transport cannot be created"));
		return -1;
	}

	//Si la ip de destino es muticast entonces se agrega al grupo
	if (local_multicast_ip != NULL)
	{
		pjmedia_transport_info info;
		st = pjmedia_transport_get_info(Transport, &info);
		if (st != PJ_SUCCESS)
		{
			PJ_LOG(3, (__FILE__, "ERROR: RTPport::Init pjmedia_transport_get_info failed"));
			return -1;
		}

		pj_sockaddr_in local_addr, mcastAddr;
		pj_sockaddr_in_init(&local_addr, &src_addr, (pj_uint16_t)src_port);
		pj_sockaddr_in_init(&mcastAddr, &pj_str(local_multicast_ip), (pj_uint16_t)src_port);

		struct ip_mreq	mreq;
		mreq.imr_multiaddr.S_un.S_addr = mcastAddr.sin_addr.s_addr;
		mreq.imr_interface.S_un.S_addr = local_addr.sin_addr.s_addr;

		st = pj_sock_setsockopt(info.sock_info.rtp_sock, IPPROTO_IP, pj_IP_ADD_MEMBERSHIP(), (void*)&mreq, sizeof(mreq));
		if (st != PJ_SUCCESS)
		{
			PJ_LOG(3, (__FILE__, "ERROR: RTPport::Init pj_sock_setsockopt pj_IP_ADD_MEMBERSHIP failed"));
			return -1;
		}
	}

	/* Now that the stream info is initialized, we can create the
	 * stream.
	 */

	st = pjmedia_stream_create(pjsua_var.med_endpt, _Pool, &info, Transport, this, &Stream);
	if (st != PJ_SUCCESS) {
		Stream = NULL;
		PJ_LOG(3, (__FILE__, "ERROR: RTPport::Init stream cannot be created"));
		return -1;
	}

	//Si la direccion remota a la que se envia es multicast se fuerza a que salga por el interfaz
	if ((pj_ntohl(rem_addr.sin_addr.s_addr) & 0xF0000000) == 0xE0000000)
	{
		pjmedia_transport_info tpinfo;
		st = pjmedia_transport_get_info(Transport, &tpinfo);
		if (st == PJ_SUCCESS)
		{
			//Se fuerza que los paquetes salgan por el interfaz que utiliza el agente.
			struct pj_in_addr in_uaIpAdd;
			pj_str_t str_uaIpAdd;
			str_uaIpAdd.ptr = SipAgent::Get_uaIpAdd();
			str_uaIpAdd.slen = (pj_ssize_t)strlen(SipAgent::Get_uaIpAdd());
			pj_inet_aton((const pj_str_t*)&str_uaIpAdd, &in_uaIpAdd);
			if (tpinfo.sock_info.rtcp_sock != PJ_INVALID_SOCKET)
			{
				st = pj_sock_setsockopt(tpinfo.sock_info.rtcp_sock, IPPROTO_IP, IP_MULTICAST_IF, (void*)&in_uaIpAdd, sizeof(in_uaIpAdd));
				if (st != PJ_SUCCESS)
					PJ_LOG(3, (__FILE__, "ERROR: RTPport::Init setsockopt, PJ_IP_MULTICAST_IF. El envio de audio a %s:%d no se puede forzar por el interface %s",
						dst_ip, dst_port, SipAgent::Get_uaIpAdd()));
			}

			st = pj_sock_setsockopt(tpinfo.sock_info.rtp_sock, IPPROTO_IP, IP_MULTICAST_IF, (void*)&in_uaIpAdd, sizeof(in_uaIpAdd));
			if (st != PJ_SUCCESS)
				PJ_LOG(3, (__FILE__, "ERROR: RTPport::Init setsockopt, PJ_IP_MULTICAST_IF. El envio de audio a %s:%d no se puede forzar por el interface %s",
					dst_ip, dst_port, SipAgent::Get_uaIpAdd()));

			if (src_port == dst_port)
			{
				pj_uint32_t multicastloop = 0;
				if (tpinfo.sock_info.rtcp_sock != PJ_INVALID_SOCKET)
				{
					st = pj_sock_setsockopt(tpinfo.sock_info.rtcp_sock, IPPROTO_IP, IP_MULTICAST_LOOP, (void*)&multicastloop, sizeof(multicastloop));
					if (st != PJ_SUCCESS)
						PJ_LOG(3, (__FILE__, "ERROR: RTPport::Init setsockopt, IP_MULTICAST_LOOP. %s:%d",
							dst_ip, dst_port));
				}

				multicastloop = 0;
				st = pj_sock_setsockopt(tpinfo.sock_info.rtp_sock, IPPROTO_IP, IP_MULTICAST_LOOP, (void*)&multicastloop, sizeof(multicastloop));
				if (st != PJ_SUCCESS)
					PJ_LOG(3, (__FILE__, "ERROR: RTPport::Init setsockopt, IP_MULTICAST_LOOP. %s:%d",
						dst_ip, dst_port));
			}
		}
	}

	pjmedia_stream_set_is_for_coresip_RTPport(Stream, PJ_TRUE);

	st = pjmedia_stream_get_port(Stream, &_Port);
	if (st != PJ_SUCCESS) {
		_Port = NULL;
		PJ_LOG(3, (__FILE__, "ERROR: RTPport::Init UDP stream port cannot be get"));
		return -1;
	}

	st = pjsua_conf_add_port(_Pool, _Port, &_Slot);
	if (st != PJ_SUCCESS)
	{
		_Slot = PJSUA_INVALID_ID;
		PJ_LOG(3, (__FILE__, "ERROR: RTPport::Init _Slot cannot be added to pjsua_conf"));
		return -1;
	}

	st = pjmedia_stream_start(Stream);
	if (st != PJ_SUCCESS) {
		PJ_LOG(3, (__FILE__, "ERROR: RTPport::Init stream cannot be started"));
		return -1;
	}	

	pj_timer_entry_init(&RTP_Timeout_timer, 0, this, on_RTP_Timeout_timer);
	pj_time_val	delay;
	delay.sec = (long)RTP_Timeout_TIME_seg;
	delay.msec = (long)RTP_Timeout_TIME_ms;
	RTP_Timeout_timer.id = 1;
	st = pjsua_schedule_timer(&RTP_Timeout_timer, &delay);
	if (st != PJ_SUCCESS)
	{
		RTP_Timeout_timer.id = 0;
		PJ_LOG(3, (__FILE__, "ERROR: RTPport::Init RTP_Timeout_timer cannot be started"));
		return -1;
	}

	return 0;
}

int RTPport::Pause(CORESIP_RTP_port_actions action)
{
	pjmedia_dir dir;
	switch (action)
	{
	case CORESIP_PAUSE_ENCODING:
		dir = PJMEDIA_DIR_ENCODING;
		break;
	case CORESIP_PAUSE_DECODING:
		dir = PJMEDIA_DIR_DECODING;
		break;
	case CORESIP_PAUSE_ENCODING_DECODING:
		dir = PJMEDIA_DIR_ENCODING_DECODING;
		break;
	default:
		PJ_LOG(3, (__FILE__, "ERROR: RTPport::Pause invalid action"));
		return -1;
	}
	pj_status_t st = pjmedia_stream_pause(Stream, dir);
	if (st != PJ_SUCCESS) {
		_Port = NULL;
		PJ_LOG(3, (__FILE__, "ERROR: RTPport::Pause pjmedia_stream_pause "));
		return -1;
	}
	return 0;
}

int RTPport::Resume(CORESIP_RTP_port_actions action)
{
	pjmedia_dir dir;
	switch (action)
	{
	case CORESIP_RESUME_ENCODING:
		dir = PJMEDIA_DIR_ENCODING;
		break;
	case CORESIP_RESUME_DECODING:
		dir = PJMEDIA_DIR_DECODING;
		break;
	case CORESIP_RESUME_ENCODING_DECODING:
		dir = PJMEDIA_DIR_ENCODING_DECODING;
		break;
	default:
		PJ_LOG(3, (__FILE__, "ERROR: RTPport::Resume invalid action"));
		return -1;
	}
	pj_status_t st = pjmedia_stream_resume(Stream, dir);
	if (st != PJ_SUCCESS) {
		_Port = NULL;
		PJ_LOG(3, (__FILE__, "ERROR: RTPport::resume pjmedia_stream_pause "));
		return -1;
	}
	return 0;
}

void RTPport::Dispose()
{
	RTP_Timeout_timer.id = 0;
	pjsua_cancel_timer(&RTP_Timeout_timer);

	if (_Slot != PJSUA_INVALID_ID)
	{
		pjsua_conf_remove_port(_Slot);
		_Slot = PJSUA_INVALID_ID;
	}

	if (Stream != NULL)
	{
		pjmedia_stream_destroy(Stream);
		Stream = NULL;
	}

	RTP_Timeout_timer.id = 0;
	pjsua_cancel_timer(&RTP_Timeout_timer);

	if (Transport != NULL)
	{
		pjmedia_transport_close(Transport);
		Transport = NULL;
	}

	if (_Lock)
	{
		pj_lock_destroy(_Lock);
	}

	if (_Pool)
	{
		pj_pool_release(_Pool);
		_Pool = NULL;
	}
}

void RTPport::RTP_Received(void* stream, void* frame, void* codec, unsigned seq, pj_uint32_t rtp_ext_info)
{
	pj_lock_acquire(_Lock);

	RTP_Timeout_timer.id = 0;
	pjsua_cancel_timer(&RTP_Timeout_timer);

	if (ReceivingRTP != 1)
	{
		ReceivingRTP = 1;
		CORESIP_RTPport_info info;
		info.receiving = PJ_TRUE;

		if (SipAgent::Cb.RTPport_infoCb) SipAgent::Cb.RTPport_infoCb(RTPport_id | CORESIP_RTPPORT_ID, &info);
	}

	pj_time_val	delay;
	delay.sec = (long)RTP_Timeout_TIME_seg;
	delay.msec = (long)RTP_Timeout_TIME_ms;
	RTP_Timeout_timer.id = 1;
	pj_status_t st = pjsua_schedule_timer(&RTP_Timeout_timer, &delay);
	if (st != PJ_SUCCESS)
	{
		RTP_Timeout_timer.id = 0;
	}

	pj_lock_release(_Lock);
}

void RTPport::AskRTPport_info()
{
	pj_lock_acquire(_Lock);
	ReceivingRTP = 0;
	CORESIP_RTPport_info info;
	info.receiving = (ReceivingRTP) ? PJ_TRUE : PJ_FALSE;

	if (SipAgent::Cb.RTPport_infoCb) SipAgent::Cb.RTPport_infoCb(RTPport_id | CORESIP_RTPPORT_ID, &info);
	pj_lock_release(_Lock);
}

void RTPport::on_RTP_Timeout_timer(pj_timer_heap_t* th, pj_timer_entry* te)
{	
	RTPport* rtpport = (RTPport*)te->user_data;
	if (rtpport == NULL) return;

	pj_lock_acquire(rtpport->_Lock);

	if (rtpport->RTP_Timeout_timer.id != 0)
	{
		rtpport->RTP_Timeout_timer.id = 0;
		pjsua_cancel_timer(&rtpport->RTP_Timeout_timer);

		if (rtpport->ReceivingRTP != 0)
		{
			rtpport->ReceivingRTP = 0;
			CORESIP_RTPport_info info;
			info.receiving = PJ_FALSE;

			if (SipAgent::Cb.RTPport_infoCb) SipAgent::Cb.RTPport_infoCb(rtpport->RTPport_id | CORESIP_RTPPORT_ID, &info);
		}
	}
	else
	{
		pjsua_cancel_timer(&rtpport->RTP_Timeout_timer);
	}

	pj_lock_release(rtpport->_Lock);
}

RTPport::~RTPport(void)
{
	Dispose();
}


// -----------   Funciones estaticas ----------------------------------------------

int RTPport::CreateRTPport(char *dst_ip, int src_port, int dst_port, char* local_multicast_ip, int payload_type, CORESIP_RTP_port_actions action)
{
	if (dst_ip == NULL)
	{
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: RTPport::CreateRTPport dst_ip  is NULL"));
		return -1;
	}

	if (payload_type != ((int) PJMEDIA_RTP_PT_PCMU) && payload_type != ((int) PJMEDIA_RTP_PT_PCMA))
	{
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: RTPport::CreateRTPport payload_type must be 0 or 8\n"));
		return -1;
	}

	if (action != CORESIP_CREATE_ENCODING && action != CORESIP_CREATE_DECODING && action != CORESIP_CREATE_ENCODING_DECODING)
	{
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: RTPport::CreateRTPport action is not valid\n"));
		return -1;
	}

	int port_id = 0;

	for (; port_id < PJ_ARRAY_SIZE(_RTP_Ports); port_id++)
	{
		if (_RTP_Ports[port_id] == NULL)
		{
			break;
		}
	}

	if (port_id == PJ_ARRAY_SIZE(_RTP_Ports))
	{
		throw PJLibException(__FILE__, PJ_ETOOMANY).Msg("CreateRTPport: No se pueden sumar mas elementos CreateRTPport");
		return -1;
	}

	_RTP_Ports[port_id] = new RTPport(port_id);
	if (_RTP_Ports[port_id] == NULL)
	{
		throw PJLibException(__FILE__, PJ_ENOMEM).Msg("CreateRTPport: No se puede crear RTP Port. new failed");
		return -1;
	}

	if (_RTP_Ports[port_id]->Init(dst_ip, src_port, dst_port, local_multicast_ip, (pjmedia_rtp_pt) payload_type, action) < 0)
	{
		delete _RTP_Ports[port_id];
		_RTP_Ports[port_id] = NULL;
		PJ_CHECK_STATUS(PJ_EINVALIDOP, ("ERROR: CreateRTPport Init failed"));
		return -1;
	}

	return port_id;
}

void RTPport::PauseResumeDestroyRTPport(int port_id, CORESIP_RTP_port_actions action)
{
	if (port_id < 0 || port_id >= PJ_ARRAY_SIZE(_RTP_Ports))
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("DestroyRTPport: Id no valido");
		return;
	}
	if (_RTP_Ports[port_id] == NULL)
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("DestroyRTPport: Id no valido");
		return;
	}

	RTPport* rtp_port = _RTP_Ports[port_id];

	switch (action)
	{
	case CORESIP_PAUSE_ENCODING:
	case CORESIP_PAUSE_DECODING:
	case CORESIP_PAUSE_ENCODING_DECODING:
		if (rtp_port->Pause(action) != 0) 
			PJ_CHECK_STATUS(PJ_EUNKNOWN, ("ERROR: CORESIP_PauseResumeDestroyRTPport: Pause error"));
		break;
	case CORESIP_RESUME_ENCODING:
	case CORESIP_RESUME_DECODING:
	case CORESIP_RESUME_ENCODING_DECODING:
		if (rtp_port->Resume(action) != 0) 
			PJ_CHECK_STATUS(PJ_EUNKNOWN, ("ERROR: CORESIP_PauseResumeDestroyRTPport: Resume error"));
		break;
	case CORESIP_DESTROY:		
		_RTP_Ports[port_id] = NULL;
		if (rtp_port != NULL) delete rtp_port;
		break;
	default:
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: CORESIP_PauseResumeDestroyRTPport: invalid action"));
	}
	
}

void RTPport::AskRTPport_info(int port_id)
{
	if (port_id < 0 || port_id >= PJ_ARRAY_SIZE(_RTP_Ports))
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("AskRTPport_info: Id no valido");
		return;
	}
	if (_RTP_Ports[port_id] == NULL)
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("AskRTPport_info: Id no valido");
		return;
	}

	RTPport* rtp_port = _RTP_Ports[port_id];
	rtp_port->AskRTPport_info();
}

void RTPport::DestroyAllRTPports()
{
	for (int i = 0; i < PJ_ARRAY_SIZE(_RTP_Ports); i++)
	{
		if (_RTP_Ports[i] != NULL)
		{
			delete _RTP_Ports[i];
			_RTP_Ports[i] = NULL;
		}
	}
}

void RTPport::OnRTP_Received(void* stream, void* frame, void* codec, unsigned seq, pj_uint32_t rtp_ext_info)
{
	RTPport* rtp_port = (RTPport *) pjmedia_stream_get_user_data((pjmedia_stream*) stream);
	if (rtp_port == NULL) return;

	rtp_port->RTP_Received(stream, frame, codec, seq, rtp_ext_info);
}




