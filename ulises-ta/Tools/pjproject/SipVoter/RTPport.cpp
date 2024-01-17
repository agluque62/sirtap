#include "Global.h"
#include "Exceptions.h"
#include "Guard.h"
#include "RTPport.h"

RTPport* RTPport::_RTP_Ports[MAX_RTP_PORTS];

RTPport::RTPport()
{
	_Pool = NULL;
	_Slot = PJSUA_INVALID_ID;
	Transport = NULL;
	Stream = NULL;
}

int RTPport::Init(char* dst_ip, int src_port, int dst_port, pjmedia_rtp_pt payload_type)
{
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

	pjmedia_port_info_init(&_Port.info, &(pj_str("RTPP")), PJMEDIA_PORT_SIGNATURE('R', 'T', 'P', 'P'),
		SAMPLING_RATE, CHANNEL_COUNT, BITS_PER_SAMPLE, SAMPLES_PER_FRAME);

	_Port.port_data.pdata = this;
	_Port.get_frame = &GetFrame;
	_Port.put_frame = &PutFrame;
	_Port.reset = &Reset;

	st = pjsua_conf_add_port(_Pool, &_Port, &_Slot);
	if (st != PJ_SUCCESS)
	{
		_Slot = PJSUA_INVALID_ID;
		PJ_LOG(3, (__FILE__, "ERROR: RTPport::Init _Slot cannot be added to pjsua_conf"));
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

	pjmedia_codec_info codec_info;
	pj_str_t codec_id;
	
	codec_info.type = PJMEDIA_TYPE_AUDIO;
	if (payload_type == PJMEDIA_RTP_PT_PCMA)
	{
		codec_info.pt = PJMEDIA_RTP_PT_PCMA;
		codec_info.encoding_name = pj_str("PCMA");
		codec_id = pj_str("PCMA");
	}
	else
	{
		codec_info.pt = PJMEDIA_RTP_PT_PCMU;
		codec_info.encoding_name = pj_str("PCMU");
		codec_id = pj_str("PCMU");
	}
	
	codec_info.clock_rate = SAMPLING_RATE;
	codec_info.channel_cnt = CHANNEL_COUNT;

	/* Initialize stream info formats */
	info.type = PJMEDIA_TYPE_AUDIO;
	info.proto = PJMEDIA_TP_PROTO_RTP_AVP;
	info.dir = PJMEDIA_DIR_ENCODING_DECODING;
	//pj_memcpy(&info.fmt, &codec_info, sizeof(pjmedia_codec_info));
	info.tx_pt = codec_info.pt;
	info.ssrc = pj_rand();

	/* Copy remote address */
	pj_memcpy(&info.rem_addr, &rem_addr, sizeof(pj_sockaddr_in));

	/* Get the codec info and param */
	const pjmedia_codec_info* ci;
	pjmedia_codec_mgr* cm = pjmedia_endpt_get_codec_mgr(pjsua_var.med_endpt);
	unsigned int count = 1;
	st = pjmedia_codec_mgr_find_codecs_by_id(cm, &codec_id, &count, &ci, NULL);
	if (st != PJ_SUCCESS) {
		return -1;
	}

	pj_memcpy(&info.fmt, ci, sizeof(*ci));

	info.param = PJ_POOL_ALLOC_T(_Pool, struct pjmedia_codec_param);
	st = pjmedia_codec_mgr_get_default_param(cm, &info.fmt, info.param);
	if (st != PJ_SUCCESS) {
		return -1;
	}

	info.tx_pt = info.fmt.pt;

	/* Create media transport */
	st = pjmedia_transport_udp_create(pjsua_var.med_endpt, NULL, src_port,	0, &Transport);
	if (st != PJ_SUCCESS)
	{
		Transport = NULL;
		PJ_LOG(3, (__FILE__, "ERROR: RTPport::Init UDP transport cannot be created"));
		return -1;
	}

	/* Now that the stream info is initialized, we can create the
	 * stream.
	 */

	st = pjmedia_stream_create(pjsua_var.med_endpt, _Pool, &info, Transport, NULL, &Stream);
	if (st != PJ_SUCCESS) {
		Stream = NULL;
		PJ_LOG(3, (__FILE__, "ERROR: RTPport::Init stream cannot be created"));
		return -1;
	}

	st = pjmedia_stream_start(Stream);
	if (st != PJ_SUCCESS) {
		PJ_LOG(3, (__FILE__, "ERROR: RTPport::Init stream cannot be started"));
		return -1;
	}

	return 0;
}

void RTPport::Dispose()
{
	if (Stream != NULL)
	{
		pjmedia_stream_destroy(Stream);
		Stream = NULL;
	}

	if (Transport != NULL)
	{
		pjmedia_transport_close(Transport);
		Transport = NULL;
	}

	if (_Slot != PJSUA_INVALID_ID)
	{
		pjsua_conf_remove_port(_Slot);
		_Slot = PJSUA_INVALID_ID;
	}	

	if (_Pool)
	{
		pj_pool_release(_Pool);
		_Pool = NULL;
	}
}

RTPport::~RTPport(void)
{
	Dispose();
}

pj_status_t RTPport::Reset(pjmedia_port* this_port)
{
	return PJ_SUCCESS;
}

pj_status_t RTPport::GetFrame(pjmedia_port* port, pjmedia_frame* frame)
{
	RTPport* rtp_port = reinterpret_cast<RTPport*>(port->port_data.pdata);
	

	return PJ_SUCCESS;
}

pj_status_t RTPport::PutFrame(pjmedia_port* port, const pjmedia_frame* frame)
{
	RTPport* rtp_port = reinterpret_cast<RTPport*>(port->port_data.pdata);


	return PJ_SUCCESS;
}


// -----------   Funciones estaticas ----------------------------------------------

int RTPport::CreateRTPport(char *dst_ip, int src_port, int dst_port, int payload_type)
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

	_RTP_Ports[port_id] = new RTPport();
	if (_RTP_Ports[port_id] == NULL)
	{
		throw PJLibException(__FILE__, PJ_ENOMEM).Msg("CreateRTPport: No se puede crear RTP Port. No hay suficiente memoria");
		return -1;
	}

	if (_RTP_Ports[port_id]->Init(dst_ip, src_port, dst_port, (pjmedia_rtp_pt) payload_type) < 0)
	{
		delete _RTP_Ports[port_id];
		_RTP_Ports[port_id] = NULL;
		PJ_CHECK_STATUS(PJ_ENOMEM, ("ERROR: CreateRTPport Init failed"));
		return -1;
	}

	return port_id;
}

void RTPport::DestroyRTPport(int port_id)
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
	_RTP_Ports[port_id] = NULL;
	delete rtp_port;
}


