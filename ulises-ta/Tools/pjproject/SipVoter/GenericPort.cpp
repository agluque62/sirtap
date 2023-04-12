#include "Global.h"
#include "Exceptions.h"
#include "Guard.h"
#include "GenericPort.h"

GenericPort* GenericPort::_Generic_Ports[MAX_GENERIC_PORTS];

/*Tiene dos puertos _UpStreamPort.Con PutInGenericPort se escribe en un buffer jitter, el cual luego es leido en la función get_frame.Con esto
* implementamos un puerto de subida con un buffer jitter.Puede servir para mandar audio a una llamada call_id
* _DownStreamPort.Con GetFromGenericPort, se lee de un buffer jitter, el cual es escrito en put_frame.Con esto implementamos un
* puerto de subida, por ejemplo para recibir audio de una llamada call_id
* 
* Se ha implementado para poder conectar a una llamada call_id audio del DSP del ETM.
* Para conectarlo a un CALL_ID hay que llamar a la función CORESIP_BridgeLink.
* Para enviar hacia el callid hay que llamar CORESIP_BridgeLink donde el source es un puerto de este tipo CORESIP_GENPORT_ID y el destino es el call_id.
* Para recibir desde el callid hay que llamar CORESIP_BridgeLink donde el source es el call_id y el destino un puerto de este tipo CORESIP_GENPORT_ID.
*/


GenericPort::GenericPort()
{
	_Pool = NULL;
	_UpStreamLock = NULL;
	_UpStreamSlot = PJSUA_INVALID_ID;
	_UpStreamJbuf = NULL;
	_UpStreamPlc = NULL;
	UpStreamSeq = 0;
	Prev_UpStream_frame_type = PJMEDIA_JB_ZERO_EMPTY_FRAME;
	_DownStreamLock = NULL;
	_DownStreamSlot = PJSUA_INVALID_ID;
	_DownStreamJbuf = NULL;
	_DownStreamPlc = NULL;
	DownStreamSeq = 0;
	Prev_DownStream_frame_type = PJMEDIA_JB_ZERO_EMPTY_FRAME;
}

int GenericPort::Init()
{
	pj_status_t st = PJ_SUCCESS;
	if (_Pool != NULL || _UpStreamLock != NULL || _UpStreamSlot != PJSUA_INVALID_ID || _UpStreamJbuf != NULL ||
		_DownStreamLock != NULL || _DownStreamSlot != PJSUA_INVALID_ID || _DownStreamJbuf != NULL)
	{
		PJ_LOG(3, (__FILE__, "ERROR: GenericPort::Init Already initialized"));
		return -1;
	}

	_Pool = pjsua_pool_create(NULL, 1024, 512);
	if (_Pool == NULL)
	{
		PJ_LOG(3, (__FILE__, "ERROR: GenericPort::Init Not enough memory"));
		return -1;
	}

	st = pj_lock_create_recursive_mutex(_Pool, NULL, &_UpStreamLock);
	if (st != PJ_SUCCESS)
	{
		_UpStreamLock = NULL;
		PJ_LOG(3, (__FILE__, "ERROR: GenericPort::Init _UpStreamLock cannot be created"));
		return -1;
	}

	st = pj_lock_create_recursive_mutex(_Pool, NULL, &_DownStreamLock);
	if (st != PJ_SUCCESS)
	{
		_DownStreamLock = NULL;
		PJ_LOG(3, (__FILE__, "ERROR: GenericPort::Init _DownStreamLock cannot be created"));
		return -1;
	}

	st = pjmedia_jbuf_create(_Pool, &(pj_str("GPORT")), SAMPLES_PER_FRAME * (BITS_PER_SAMPLE / 8), PTIME, 2, &_UpStreamJbuf);
	if (st != PJ_SUCCESS)
	{
		_UpStreamJbuf = NULL;
		PJ_LOG(3, (__FILE__, "ERROR: GenericPort::Init _UpStreamJbuf cannot be created"));
		return -1;
	}

	st = pjmedia_jbuf_create(_Pool, &(pj_str("GPORT")), SAMPLES_PER_FRAME * (BITS_PER_SAMPLE / 8), PTIME, 2, &_DownStreamJbuf);
	if (st != PJ_SUCCESS)
	{
		_DownStreamJbuf = NULL;
		PJ_LOG(3, (__FILE__, "ERROR: GenericPort::Init _DownStreamJbuf cannot be created"));
		return -1;
	}

	st = pjmedia_plc_create(_Pool, SAMPLING_RATE, SAMPLES_PER_FRAME, 0, &_UpStreamPlc);
	if (st != PJ_SUCCESS)
	{
		_UpStreamPlc = NULL;
		PJ_LOG(3, (__FILE__, "ERROR: GenericPort::Init _UpStreamPlc cannot be created"));
		return -1;
	}

	st = pjmedia_plc_create(_Pool, SAMPLING_RATE, SAMPLES_PER_FRAME, 0, &_DownStreamPlc);
	if (st != PJ_SUCCESS)
	{
		_DownStreamPlc = NULL;
		PJ_LOG(3, (__FILE__, "ERROR: GenericPort::Init _DownStreamPlc cannot be created"));
		return -1;
	}

	pjmedia_port_info_init(&_UpStreamPort.info, &(pj_str("GUPP")), PJMEDIA_PORT_SIGNATURE('G', 'U', 'P', 'P'),
		SAMPLING_RATE, CHANNEL_COUNT, BITS_PER_SAMPLE, SAMPLES_PER_FRAME);
	pjmedia_port_info_init(&_DownStreamPort.info, &(pj_str("GDOP")), PJMEDIA_PORT_SIGNATURE('G', 'D', 'O', 'P'),
		SAMPLING_RATE, CHANNEL_COUNT, BITS_PER_SAMPLE, SAMPLES_PER_FRAME);

	_UpStreamPort.port_data.pdata = this;
	_UpStreamPort.get_frame = &UpStreamGetFrame;
	_UpStreamPort.put_frame = NULL;
	_UpStreamPort.reset = &UpStreamReset;

	_DownStreamPort.port_data.pdata = this;
	_DownStreamPort.get_frame = NULL;
	_DownStreamPort.put_frame = &DownStreamPutFrame;
	_DownStreamPort.reset = &DownStreamReset;

	st = pjsua_conf_add_port(_Pool, &_UpStreamPort, &_UpStreamSlot);
	if (st != PJ_SUCCESS)
	{
		_UpStreamSlot = PJSUA_INVALID_ID;
		PJ_LOG(3, (__FILE__, "ERROR: GenericPort::Init _UpStreamSlot cannot be added to pjsua_conf"));
		return -1;
	}	

	st = pjsua_conf_add_port(_Pool, &_DownStreamPort, &_DownStreamSlot);
	if (st != PJ_SUCCESS)
	{
		_DownStreamSlot = PJSUA_INVALID_ID;
		PJ_LOG(3, (__FILE__, "ERROR: GenericPort::Init _DownStreamSlot cannot be added to pjsua_conf"));
		return -1;
	}

	return 0;
}

void GenericPort::Dispose()
{
	if (_UpStreamSlot != PJSUA_INVALID_ID)
	{
		pjsua_conf_remove_port(_UpStreamSlot);
	}	
	if (_DownStreamSlot != PJSUA_INVALID_ID)
	{
		pjsua_conf_remove_port(_DownStreamSlot);
	}

	if (_UpStreamJbuf != NULL)
	{
		pjmedia_jbuf_destroy(_UpStreamJbuf);
		_UpStreamJbuf = NULL;
	}

	if (_DownStreamJbuf != NULL)
	{
		pjmedia_jbuf_destroy(_DownStreamJbuf);
		_DownStreamJbuf = NULL;
	}

	if (_Pool)
	{
		pj_pool_release(_Pool);
	}
}

GenericPort::~GenericPort(void)
{
	Dispose();
}

pj_status_t GenericPort::UpStreamReset(pjmedia_port* this_port)
{
	GenericPort* gen_port = reinterpret_cast<GenericPort*>(this_port->port_data.pdata);
	Guard lock(gen_port->_UpStreamLock);
	pjmedia_jbuf_reset(gen_port->_UpStreamJbuf);
	gen_port->UpStreamSeq = 0;

	return PJ_SUCCESS;
}

pj_status_t GenericPort::DownStreamReset(pjmedia_port* this_port)
{
	GenericPort* gen_port = reinterpret_cast<GenericPort*>(this_port->port_data.pdata);
	Guard lock(gen_port->_DownStreamLock);
	pjmedia_jbuf_reset(gen_port->_DownStreamJbuf);
	gen_port->DownStreamSeq = 0;

	return PJ_SUCCESS;
}

/// <summary>
/// Sirve para poner muestras en el jitter buffer, del cual se extren en la función UpStreamGetFrame.
/// La cual será llamada cuando otro puerto de la pjmedia u otro elemento haga un get_frame
/// </summary>
/// <param name="buffer"></param>
/// <param name="buffer_length"></param>
/// <returns></returns>
int GenericPort::Put(char* buffer, int buffer_length)
{	
	Guard lock(_UpStreamLock);

	pjmedia_jbuf_put_frame(_UpStreamJbuf, (const void *) buffer, buffer_length, UpStreamSeq);
	UpStreamSeq++;

	return buffer_length;
}

pj_status_t GenericPort::UpStreamGetFrame(pjmedia_port* port, pjmedia_frame* frame)
{
	GenericPort* gen_port = reinterpret_cast<GenericPort*>(port->port_data.pdata);
	unsigned samples_required = port->info.samples_per_frame;
	pj_int16_t* p_out_samp = (pj_int16_t*)frame->buf;							// Las tramas son 16 bit con signo
	char frame_type;
	pj_uint32_t bit_info;

	frame->type = PJMEDIA_FRAME_TYPE_AUDIO;

	Guard lock(gen_port->_UpStreamLock);

	pjmedia_jb_state state;
	pjmedia_jbuf_get_state(gen_port->_UpStreamJbuf, &state);

#if 0
	unsigned	frame_size;	    /**< Individual frame size, in bytes.   */
	unsigned	min_prefetch;	    /**< Minimum allowed prefetch, in frms. */
	unsigned	max_prefetch;	    /**< Maximum allowed prefetch, in frms. */

	/* Status */
	unsigned	burst;		    /**< Current burst level, in frames	    */
	unsigned	prefetch;	    /**< Current prefetch value, in frames  */
	unsigned	size;		    /**< Current buffer size, in frames.    */

	/* Statistic */
	unsigned	avg_delay;	    /**< Average delay, in ms.		    */
	unsigned	min_delay;	    /**< Minimum delay, in ms.		    */
	unsigned	max_delay;	    /**< Maximum delay, in ms.		    */
	unsigned	dev_delay;	    /**< Standard deviation of delay, in ms.*/
	unsigned	avg_burst;	    /**< Average burst, in frames.	    */
	unsigned	lost;		    /**< Number of lost frames.		    */
	unsigned	discard;	    /**< Number of discarded frames.	    */
	unsigned	empty;		    /**< Number of empty on GET events.	    */
#endif

	PJ_LOG(5, (__FILE__, "DEBUG: #### frame_size %d min_prefetch %d max_prefetch %d burst %d prefetch %d size %d", 
		state.frame_size, state.min_prefetch, state.max_prefetch, state.burst, state.prefetch, state.size));

	pjmedia_jbuf_get_frame2(gen_port->_UpStreamJbuf, p_out_samp, &frame->size, &frame_type, &bit_info);	

	if (frame_type == PJMEDIA_JB_MISSING_FRAME || frame_type == PJMEDIA_JB_ZERO_PREFETCH_FRAME)
	{
		pj_status_t st = pjmedia_plc_generate(gen_port->_UpStreamPlc, p_out_samp);
		PJ_LOG(5, (__FILE__, "DEBUG: #### 1 frame_type %d", frame_type));
		if (st != PJ_SUCCESS)
		{
			PJ_LOG(5, (__FILE__, "DEBUG: #### 2 frame_type %d", frame_type));
			pjmedia_zero_samples(p_out_samp, frame->size);
		}
	}
	else if (frame_type == PJMEDIA_JB_ZERO_EMPTY_FRAME)
	{
		if (gen_port->Prev_UpStream_frame_type != PJMEDIA_JB_ZERO_EMPTY_FRAME)
		{
			PJ_LOG(5, (__FILE__, "DEBUG: #### 3 frame_type %d", frame_type));
			pj_status_t st = pjmedia_plc_generate(gen_port->_UpStreamPlc, p_out_samp);
			if (st != PJ_SUCCESS)
			{
				PJ_LOG(5, (__FILE__, "DEBUG: #### 4 frame_type %d", frame_type));
				pjmedia_zero_samples(p_out_samp, frame->size);
			}
		}
		else
		{
			PJ_LOG(5, (__FILE__, "DEBUG: #### 5 frame_type %d", frame_type));
			pjmedia_zero_samples(p_out_samp, frame->size);
		}
	}
	else
	{
		pjmedia_plc_save(gen_port->_UpStreamPlc, p_out_samp);
		PJ_LOG(5, (__FILE__, "DEBUG: #### 0 frame_type %d", frame_type));
	}

	gen_port->Prev_UpStream_frame_type = frame_type;

	return PJ_SUCCESS;
}

int GenericPort::Get(char* buffer, unsigned int buffer_length)
{
	char frame_type;
	pj_uint32_t bit_info;
	unsigned int frame_size = SAMPLES_PER_FRAME * (BITS_PER_SAMPLE / 8);
	if (buffer_length != frame_size)
	{
		PJ_LOG(3, (__FILE__, "ERROR: GenericPort::Get buffer_length %d must be %d", buffer_length, frame_size));
		return -1;
	}

	Guard lock(_DownStreamLock);

	pjmedia_jbuf_get_frame2(_DownStreamJbuf, buffer, (pj_size_t *) &frame_size, &frame_type, &bit_info);

	if (frame_type == PJMEDIA_JB_MISSING_FRAME || frame_type == PJMEDIA_JB_ZERO_PREFETCH_FRAME)
	{
		pj_status_t st = pjmedia_plc_generate(_DownStreamPlc, (pj_int16_t *) buffer);
		if (st != PJ_SUCCESS)
		{
			pjmedia_zero_samples((pj_int16_t*)buffer, buffer_length);
		}
	}
	else if (frame_type == PJMEDIA_JB_ZERO_EMPTY_FRAME)
	{
		if (Prev_DownStream_frame_type != PJMEDIA_JB_ZERO_EMPTY_FRAME)
		{
			pj_status_t st = pjmedia_plc_generate(_DownStreamPlc, (pj_int16_t*)buffer);
			if (st != PJ_SUCCESS)
			{
				pjmedia_zero_samples((pj_int16_t*)buffer, buffer_length);
			}
		}
		else
		{
			pjmedia_zero_samples((pj_int16_t*)buffer, buffer_length);
		}
	}
	else
	{
		pjmedia_plc_save(_DownStreamPlc, (pj_int16_t*)buffer);
	}

	Prev_DownStream_frame_type = frame_type;

	return 0;
}

pj_status_t GenericPort::DownStreamPutFrame(pjmedia_port* port, const pjmedia_frame* frame)
{
	GenericPort* gen_port = reinterpret_cast<GenericPort*>(port->port_data.pdata);

	Guard lock(gen_port->_DownStreamLock);

	pjmedia_jbuf_put_frame(gen_port->_DownStreamJbuf, (const void*)frame->buf, frame->size, gen_port->DownStreamSeq);
	gen_port->DownStreamSeq++;

	return PJ_SUCCESS;
}


// -----------   Funciones estaticas ----------------------------------------------

int GenericPort::CreateGenericPort()
{
	int gen_port_id = 0;

	for (; gen_port_id < PJ_ARRAY_SIZE(_Generic_Ports); gen_port_id++)
	{
		if (_Generic_Ports[gen_port_id] == NULL)
		{
			break;
		}
	}

	pj_assert(gen_port_id < PJ_ARRAY_SIZE(_Generic_Ports));
	if (gen_port_id == PJ_ARRAY_SIZE(_Generic_Ports))
	{
		throw PJLibException(__FILE__, PJ_ETOOMANY).Msg("CreateGenericPort: No se pueden sumar mas elementos CreateGenericPort");
		return gen_port_id;
	}

	_Generic_Ports[gen_port_id] = new GenericPort();
	if (_Generic_Ports[gen_port_id] == NULL)
	{
		throw PJLibException(__FILE__, PJ_ENOMEM).Msg("CreateGenericPort: No se puede crear Generic Port. No hay suficiente memoria");
		return gen_port_id;
	}

	if (_Generic_Ports[gen_port_id]->Init() < 0)
	{
		delete _Generic_Ports[gen_port_id];
		_Generic_Ports[gen_port_id] = NULL;
		PJ_CHECK_STATUS(PJ_ENOMEM, ("ERROR: GenericPort::CreateGenericPort Init failed"));
		return gen_port_id;
	}

	return gen_port_id;
}

void GenericPort::DestroyGenericPort(int gen_port_id)
{
	if (_Generic_Ports[gen_port_id] == NULL)
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("DestroyGenericPort: Id no valido");
		return;
	}
	GenericPort* gen_port = _Generic_Ports[gen_port_id];
	_Generic_Ports[gen_port_id] = NULL;
	delete gen_port;
}

void GenericPort::PutInGenericPort(int gen_port_id, char* buffer, int buffer_length)
{
	if (_Generic_Ports[gen_port_id] == NULL)
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("PutInGenericPort: Id no valido");
		return;
	}
	GenericPort* gen_port = _Generic_Ports[gen_port_id];
	gen_port->Put(buffer, buffer_length);
}

void GenericPort::GetFromGenericPort(int gen_port_id, char* buffer, int buffer_length)
{
	if (_Generic_Ports[gen_port_id] == NULL)
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("GetFromGenericPort: Id no valido");
		return;
	}
	GenericPort* gen_port = _Generic_Ports[gen_port_id];
	if (gen_port->Get(buffer, buffer_length) < 0)
	{
		throw PJLibException(__FILE__, PJ_EINVAL).Msg("GetFromGenericPort: buffer_length no valido");
		return;
	}
}

