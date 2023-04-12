
#include <string>
#include "Global.h"
#include "exceptions.h"
#include "SipCall.h"
#include "NoiseGenerator.h"
#include "SignalGen.h"

tone_info::tone_info(int freq, int volume)
{
	frequency = freq;	
	pool = pjsua_pool_create("tonegen", 512, 512);
	if (pool == NULL)
	{
		PJ_LOG(3, ("tone_info::tone_info", "ERROR: pjsua_pool_create returns error"));
		throw -1;
	}

	slot_id = PJSUA_INVALID_ID;
	tone_port = NULL;
	pj_status_t st = pjmedia_tonegen_create(pool, SAMPLING_RATE, CHANNEL_COUNT, SAMPLES_PER_FRAME,
		BITS_PER_SAMPLE, PJMEDIA_TONEGEN_LOOP, &tone_port);
	if (st != PJ_SUCCESS)
	{
		PJ_LOG(3, ("tone_info::tone_info", "ERROR: pjmedia_tonegen_create returns error"));
		pj_pool_release(pool);
		pool = NULL;
		tone_port = NULL;
		throw -1;
	}

	pjmedia_tone_desc tone;
	tone.freq1 = (short)frequency;
	tone.freq2 = 0;
	tone.on_msec = 10000;	//Duracion infinita
	tone.off_msec = 0;
	tone.volume = (short) volume;
	st = pjmedia_tonegen_play(tone_port, 1, &tone, PJMEDIA_TONEGEN_LOOP);
	if (st != PJ_SUCCESS)
	{
		PJ_LOG(3, ("tone_info::tone_info", "ERROR: pjmedia_tonegen_play returns error"));
		pj_pool_release(pool);
		pool = NULL;
		throw -1;
	}

	st = pjsua_conf_add_port(pool, tone_port, &slot_id);
	if (st != PJ_SUCCESS)
	{
		PJ_LOG(3, ("tone_info::tone_info", "ERROR: pjsua_conf_add_port returns error"));
		pj_pool_release(pool);
		pool = NULL;
		slot_id = PJSUA_INVALID_ID;
		throw -1;
	}
}

tone_info::~tone_info()
{
	if (slot_id != PJSUA_INVALID_ID)
	{
		pjsua_conf_remove_port(slot_id);
		slot_id = PJSUA_INVALID_ID;
	}
	if (pool)
	{
		pj_pool_release(pool);
		pool = NULL;
	}	
}

int tone_info::Update_tone(int volume)
{
	pj_status_t st = pjmedia_tonegen_stop(tone_port);
	if (st != PJ_SUCCESS)
	{
		PJ_LOG(3, ("tone_info::Update_tone", "ERROR: tone_info::Update_tone: pjmedia_tonegen_stop returns error"));
		return -1;
	}

	if (volume > 0)
	{
		pjmedia_tone_desc tone;
		tone.freq1 = (short)frequency;
		tone.freq2 = 0;
		tone.on_msec = 10000;	//Duracion infinita
		tone.off_msec = 0;
		tone.volume = (short) volume;
		st = pjmedia_tonegen_play(tone_port, 1, &tone, PJMEDIA_TONEGEN_LOOP);
		if (st != PJ_SUCCESS)
		{
			PJ_LOG(3, ("tone_info::Update_tone", "ERROR: tone_info::Update_tone: pjmedia_tonegen_play returns error"));
			return -1;
		}
	}

	return 0;
}

void SignalGen::SendToneToCall(int call_id, unsigned int frequency, float volume_dbm0, int on)
{
	pjsua_call* call;
	pjsip_dialog* dlg;
	pj_status_t st;
	int lineal_volume = 0;

	if (frequency == 0 && on == 0)
	{
		StopAllTones(call_id);
		return;
	}

	if (on == 0)
	{
		lineal_volume = 0;
	}

	st = acquire_call("SendToneToCall()", call_id, &call, &dlg);
	if (st != PJ_SUCCESS)
	{
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: SignalGen::SendToneToCall: invalid call_id"));
	}

	pjmedia_session* session = pjsua_call_get_media_session(call_id);
	if (session == NULL)
	{
		pjsip_dlg_dec_lock(dlg);
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: SignalGen::SendToneToCall: no session"));
	}

	pjmedia_session_info sess_info;
	if (pjmedia_session_get_info((pjmedia_session*)session, &sess_info) != PJ_SUCCESS)
	{
		pjsip_dlg_dec_lock(dlg);
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: SignalGen::SendToneToCall: pjmedia_session_get_info returns error"));
	}

	if (sess_info.stream_cnt > 0)
	{
		if (sess_info.stream_info[0].fmt.pt == PJMEDIA_RTP_PT_PCMA)
		{
			lineal_volume = (on != 0) ? dBm0_to_lineal_volume_a_law(volume_dbm0) : 0;
		}
		else if (sess_info.stream_info[0].fmt.pt == PJMEDIA_RTP_PT_PCMU)
		{
			lineal_volume = (on != 0) ? dBm0_to_lineal_volume_mu_law(volume_dbm0) : 0;
		}
		else
		{
			pjsip_dlg_dec_lock(dlg);
			PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: SignalGen::SendToneToCall: pjmedia_session_get_info format error"));
		}
	}
	else
	{
		pjsip_dlg_dec_lock(dlg);
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: SignalGen::SendToneToCall: pjmedia_session_get_info returns 0 sessions"));
	}	

	SipCall* sipcall = (SipCall*)pjsua_call_get_user_data(call_id);
	if (sipcall == NULL)
	{
		pjsip_dlg_dec_lock(dlg);
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: SignalGen::SendToneToCall: invalid call_id"));
	}

	std::map <int, tone_info *>::iterator it;

	//Se actualizan los volumenes de todos los tonos activos si frequency es cero, y si es distinto ce cero solo el de la frecuencia
	bool freq_found = false;
	std::map <int, tone_info*> Playing_tones_map_to_erase;
	for (it = sipcall->Playing_tones_map.begin(); it != sipcall->Playing_tones_map.end(); ++it)
	{
		if (frequency == 0 || (frequency != 0 && frequency == it->first))
		{
			tone_info* tone = it->second;
			if (lineal_volume > 0)
			{
				if (tone->Update_tone(lineal_volume) != 0)
				{
					pjsip_dlg_dec_lock(dlg);
					PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: SignalGen::SendToneToCall: tone->Update_tone returns error"));
				}
			}
			else
			{
				//Se agrega en esta lista los tonos a eliminar porque el volumen es cero
				Playing_tones_map_to_erase[it->first] = tone;
			}
		}

		if (frequency != 0 && frequency == it->first)
		{
			freq_found = true;
			break;
		}
	}

	//Se aliminan los tonos almacenados en la lista de los que se quiere que el volumen sea cero
	for (it = Playing_tones_map_to_erase.begin(); it != Playing_tones_map_to_erase.end(); ++it)
	{
		tone_info* tone = it->second;
		st = pjsua_conf_disconnect(tone->slot_id, pjsua_call_get_conf_port(call_id));
		if (st != PJ_SUCCESS)
		{
			PJ_LOG(3, ("SignalGen::SendToneToCall", "ERROR: SignalGen::SendToneToCall: pjsua_conf_disconnect returns error"));
		}
		sipcall->Playing_tones_map.erase(it->first);
		delete tone;
	}

	if (frequency != 0 && !freq_found && lineal_volume > 0)
	{
		//Creamos un tono nuevo

		tone_info* tone = NULL;
		try
		{
			tone = new tone_info(frequency, lineal_volume);
		}
		catch (...)
		{
			if (tone != NULL) delete tone;
			pjsip_dlg_dec_lock(dlg);
			PJ_CHECK_STATUS(PJ_ENOMEM, ("ERROR: SignalGen::SendToneToCall: Tone cannot be created"));
		}		

		st = pjsua_conf_connect(tone->slot_id, pjsua_call_get_conf_port(call_id));
		if (st != PJ_SUCCESS)
		{
			if (tone != NULL) delete tone;
			pjsip_dlg_dec_lock(dlg);
			PJ_CHECK_STATUS(st, ("ERROR: SignalGen::SendToneToCall: pjsua_conf_connect returns error"));
		}

		sipcall->Playing_tones_map[frequency] = tone;
	}
	
	pjsip_dlg_dec_lock(dlg);
}


void SignalGen::StopAllTones(int call_id)
{
	pjsua_call* call;
	pjsip_dialog* dlg;
	pj_status_t st;

	st = acquire_call("StopAllTones()", call_id, &call, &dlg);
	if (st != PJ_SUCCESS)
	{
		PJ_LOG(3, ("SignalGen::StopNoiseToCall", "ERROR: SignalGen::StopAllTones: invalid call_id"));
		return;
	}

	SipCall* sipcall = (SipCall*)pjsua_call_get_user_data(call_id);
	if (sipcall == NULL)
	{
		pjsip_dlg_dec_lock(dlg);
		PJ_LOG(3, ("SignalGen::StopNoiseToCall", "ERROR: SignalGen::StopAllTones: invalid call_id"));
		return;
	}

	std::map <int, tone_info*>::iterator it;
	for (it = sipcall->Playing_tones_map.begin(); it != sipcall->Playing_tones_map.end(); ++it)
	{
		tone_info* tone = it->second;
		pjsua_conf_disconnect(tone->slot_id, pjsua_call_get_conf_port(call_id));
		delete tone;
	}

	sipcall->Playing_tones_map.clear();
	pjsip_dlg_dec_lock(dlg);
}

int SignalGen::dBm0_to_lineal_volume_a_law(float volume_dbm0)
{
	/*
		Ley A. 12-bit + signo = 13
		Valor maximo: 4095 => +3.14 dBm0
		Por tanto, 0 dBm0  = 4095 * [ 10 exp (-3,14/20) ] = 2852,68557

		lineal_volume = 2852.68557 * pow(10, volume_dbm0/20);

		Lo traducimos al volumen que maneja el generador de tonos
		4095       -> 30000 -> 3.14 dBm
		2852.68557 -> 20893 -> 0 dBm

		tone_volume = 30000 * lineal_volume / 4095;
	*/

	if (volume_dbm0 > 3.14f) volume_dbm0 = 3.14f;
	float lineal_volume = 2852.68557f * (float) pow(10.0f, volume_dbm0 / 20.0f);
	float tone_volume = (32767.0f / 4095.0f) * lineal_volume;
	return (int)tone_volume;
}

int SignalGen::dBm0_to_lineal_volume_mu_law(float volume_dbm0)
{
	/*
		Ley A. 13-bit + signo = 14
		Valor maximo: 8192 => +3.17 dBm0
		Por tanto, 0 dBm0  = 8192 * [ 10 exp (-3,17/20) ] = 5687,08793

		lineal_volume = 5687.08793 * pow(10, volume_dbm0/20);

		Lo traducimos al volumen que maneja el generador de tonos
		8192       -> 30000 -> 3.17 dBm
		5687,08793 -> 20826 -> 0 dBm

		tone_volume = 30000 * lineal_volume / 8192;
	*/

	if (volume_dbm0 > 3.17f) volume_dbm0 = 3.17f;
	float lineal_volume = 5687.08793f * (float)pow(10.0f, volume_dbm0 / 20.0f);
	float tone_volume = (32767.0f / 8192.0f) * lineal_volume;
	return (int)tone_volume;
}

void SignalGen::SendNoiseToCall(int call_id, NoiseGenerator::NoiseType type, float volume_dbm0, int on)
{
	pjsua_call* call;
	pjsip_dialog* dlg;
	pj_status_t st;

	st = acquire_call("SendNoiseToCall()", call_id, &call, &dlg);
	if (st != PJ_SUCCESS)
	{
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: SignalGen::SendNoiseToCall: invalid call_id"));
	}

	SipCall* sipcall = (SipCall*)pjsua_call_get_user_data(call_id);
	if (sipcall == NULL)
	{
		pjsip_dlg_dec_lock(dlg);
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: SignalGen::SendNoiseToCall: invalid call_id"));
	}

	pjmedia_session* session = pjsua_call_get_media_session(call_id);
	if (session == NULL)
	{
		pjsip_dlg_dec_lock(dlg);
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: SignalGen::SendNoiseToCall: no session"));
	}

	pjmedia_session_info sess_info;
	if (pjmedia_session_get_info((pjmedia_session*)session, &sess_info) != PJ_SUCCESS)
	{
		pjsip_dlg_dec_lock(dlg);
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: SignalGen::SendNoiseToCall: pjmedia_session_get_info returns error"));
	}

	pj_bool_t pcmA = PJ_TRUE;

	if (sess_info.stream_cnt > 0)
	{
		if (sess_info.stream_info[0].fmt.pt == PJMEDIA_RTP_PT_PCMA)
		{
			pcmA = PJ_TRUE;
		}
		else if (sess_info.stream_info[0].fmt.pt == PJMEDIA_RTP_PT_PCMU)
		{
			pcmA = PJ_FALSE;
		}
		else
		{
			pjsip_dlg_dec_lock(dlg);
			PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: SignalGen::SendNoiseToCall: pjmedia_session_get_info format error"));
		}
	}
	else
	{
		pjsip_dlg_dec_lock(dlg);
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: SignalGen::SendNoiseToCall: pjmedia_session_get_info returns 0 sessions"));
	}

	if (sipcall->Noise_generator != NULL)
	{
		pjsua_conf_disconnect(sipcall->Noise_generator->_Slot, pjsua_call_get_conf_port(call_id));
		delete sipcall->Noise_generator;
		sipcall->Noise_generator = NULL;
	}	

	if (on)
	{
		sipcall->Noise_generator = new NoiseGenerator(type);
		if (sipcall->Noise_generator->Init(volume_dbm0, pcmA) == -1)
		{
			delete sipcall->Noise_generator;
			sipcall->Noise_generator = NULL;
			pjsip_dlg_dec_lock(dlg);
			PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: SignalGen::SendNoiseToCall: sipcall->Noise_generator->Init return error"));
		}

		if (pjsua_conf_connect(sipcall->Noise_generator->_Slot, pjsua_call_get_conf_port(call_id)) != PJ_SUCCESS)
		{
			pjsip_dlg_dec_lock(dlg);
			PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: SignalGen::SendNoiseToCall: Noise generator cannot be connected to call_id"));
		}
	}

	pjsip_dlg_dec_lock(dlg);
}

void SignalGen::StopNoiseToCall(int call_id)
{
	pjsua_call* call;
	pjsip_dialog* dlg;
	pj_status_t st;

	st = acquire_call("SendNoiseToCall()", call_id, &call, &dlg);
	if (st != PJ_SUCCESS)
	{
		PJ_LOG(3, ("SignalGen::StopNoiseToCall", "ERROR: SignalGen::SendNoiseToCall: invalid call_id"));
		return;
	}

	SipCall* sipcall = (SipCall*)pjsua_call_get_user_data(call_id);
	if (sipcall == NULL)
	{
		pjsip_dlg_dec_lock(dlg);
		PJ_LOG(3, ("SignalGen::StopNoiseToCall", "ERROR: SignalGen::SendNoiseToCall: invalid call_id"));
		return;
	}

	if (sipcall->Noise_generator != NULL)
	{
		pjsua_conf_disconnect(sipcall->Noise_generator->_Slot, pjsua_call_get_conf_port(call_id));
		delete sipcall->Noise_generator;
		sipcall->Noise_generator = NULL;
	}

	pjsip_dlg_dec_lock(dlg);
}

void SignalGen::SendDTMF(int call_id, const CORESIP_tone_digit_map* digit_map, unsigned count, const CORESIP_tone_digit digits[], float volume_dbm0)
{
	pjsua_call* call;
	pjsip_dialog* dlg;
	pj_status_t st;

	if (count == 0) PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: SignalGen::SendDTMF: invalid count, must be > 0"));
	if (digit_map == NULL) PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: SignalGen::SendDTMF: invalid digit_map, must be not NULL"));
	if (digits == NULL) PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: SignalGen::SendDTMF: invalid digits, must be not NULL"));
	if (digit_map->count == 0 || digit_map->count > 16) PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: SignalGen::SendDTMF: invalid digit_map->count, must be > 0 and <= 16"));

	for (unsigned int i = 0; i < count; i++)
	{
		if (digits[i].digit != ',')
		{
			pj_bool_t in_map = PJ_FALSE;
			for (unsigned int j = 0; j < digit_map->count; j++)
			{
				if (digits[i].digit == digit_map->digits[j].digit)
				{
					in_map = PJ_TRUE;
					break;
				}				
			}
			if (!in_map) PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: SignalGen::SendDTMF: invalid digits, all must be in digit map except \',\'"));
		}
	}

	st = acquire_call("SignalGen::SendDTMF()", call_id, &call, &dlg);
	if (st != PJ_SUCCESS)
	{
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: SignalGen::SendDTMF: invalid call_id"));
	}

	SipCall* sipcall = (SipCall*)pjsua_call_get_user_data(call_id);
	if (sipcall == NULL)
	{
		pjsip_dlg_dec_lock(dlg);
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: SignalGen::SendDTMF: invalid call_id"));
	}

	pjmedia_session* session = pjsua_call_get_media_session(call_id);
	if (session == NULL)
	{
		pjsip_dlg_dec_lock(dlg);
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: SignalGen::SendDTMF: no session"));
	}

	pjmedia_session_info sess_info;
	if (pjmedia_session_get_info((pjmedia_session*)session, &sess_info) != PJ_SUCCESS)
	{
		pjsip_dlg_dec_lock(dlg);
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: SignalGen::SendDTMF: pjmedia_session_get_info returns error"));
	}

	pj_bool_t pcmA = PJ_TRUE;

	if (sess_info.stream_cnt > 0)
	{
		if (sess_info.stream_info[0].fmt.pt == PJMEDIA_RTP_PT_PCMA)
		{
			pcmA = PJ_TRUE;
		}
		else if (sess_info.stream_info[0].fmt.pt == PJMEDIA_RTP_PT_PCMU)
		{
			pcmA = PJ_FALSE;
		}
		else
		{
			pjsip_dlg_dec_lock(dlg);
			PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: SignalGen::SendDTMF: pjmedia_session_get_info format error"));
		}
	}
	else
	{
		pjsip_dlg_dec_lock(dlg);
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: SignalGen::SendDTMF: pjmedia_session_get_info returns 0 sessions"));
	}

	if (sipcall->DTMF_generator != NULL)
	{
		//Paramos el generador de digitos que ya estaba en curso
		sipcall->DTMF_generator->End(call_id);
		delete sipcall->DTMF_generator;
		sipcall->DTMF_generator = NULL;
	}
	
	sipcall->DTMF_generator = new DTMFgen();
	if (sipcall->DTMF_generator == NULL)
	{
		pjsip_dlg_dec_lock(dlg);
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: SignalGen::SendDTMF: sipcall->DTMF_generator cannot be created"));
	}
	if (sipcall->DTMF_generator->Init(call_id, digit_map, count, digits, volume_dbm0, pcmA) < 0)
	{
		delete sipcall->DTMF_generator;
		sipcall->DTMF_generator = NULL;
		pjsip_dlg_dec_lock(dlg);
		PJ_CHECK_STATUS(PJ_EINVAL, ("ERROR: SignalGen::SendDTMF: sipcall->DTMF_generator->Init returns error. See log"));
	}

	pjsip_dlg_dec_lock(dlg);
}

void SignalGen::StopDTMF(int call_id)
{
	pjsua_call* call;
	pjsip_dialog* dlg;
	pj_status_t st;

	st = acquire_call("SignalGen::StopDTMF()", call_id, &call, &dlg);
	if (st != PJ_SUCCESS)
	{
		PJ_LOG(3, ("SignalGen::StopDTMF", "ERROR: SignalGen::StopDTMF: invalid call_id"));
		return;
	}

	SipCall* sipcall = (SipCall*)pjsua_call_get_user_data(call_id);
	if (sipcall == NULL)
	{
		pjsip_dlg_dec_lock(dlg);
		PJ_LOG(3, ("SignalGen::StopDTMF", "ERROR: SignalGen::StopDTMF: invalid call_id"));
		return;
	}
	
	if (sipcall->DTMF_generator != NULL)
	{
		//Paramos el generador de digitos que ya estaba en curso
		sipcall->DTMF_generator->End(call_id);
		delete sipcall->DTMF_generator;
		sipcall->DTMF_generator = NULL;
	}
	pjsip_dlg_dec_lock(dlg);
}

DTMFgen::DTMFgen()
{
	pool = NULL;
	port = NULL;
	slot_id = PJSUA_INVALID_ID;
	_digits = NULL;
}

DTMFgen::~DTMFgen()
{	
	port = NULL;
	slot_id = PJSUA_INVALID_ID;
	_digits = NULL;
	if (pool != NULL)
	{
		pj_pool_release(pool);
		pool = NULL;
	}
}

int DTMFgen::Init(int call_id, const CORESIP_tone_digit_map* digit_map, unsigned count, const CORESIP_tone_digit digits[], float volume_dbm0, pj_bool_t pcmA)
{
	pool = pjsua_pool_create("DTMFgen::Init", 512, 512);
	if (pool == NULL)
	{
		PJ_LOG(3, ("DTMFgen::Init", "ERROR: pjsua_pool_create returns error"));
		pool = NULL;
		port = NULL;
		slot_id = PJSUA_INVALID_ID;
		return -1;
	}

	pj_status_t st = pjmedia_tonegen_create(pool, SAMPLING_RATE, CHANNEL_COUNT, SAMPLES_PER_FRAME,
		BITS_PER_SAMPLE, 0 /*PJMEDIA_TONEGEN_LOOP*/, &port);
	if (st != PJ_SUCCESS)
	{
		PJ_LOG(3, ("DTMFgen::Init", "ERROR: pjmedia_tonegen_create returns error"));
		pj_pool_release(pool);
		pool = NULL;
		port = NULL;
		slot_id = PJSUA_INVALID_ID;
		return -1;
	}

	if (digit_map != NULL)
	{
		pjmedia_tone_digit_map d_map;
		d_map.count = digit_map->count;
		for (unsigned int i = 0; i < d_map.count; i++)
		{
			d_map.digits[i].digit = digit_map->digits[i].digit;
			d_map.digits[i].freq1 = digit_map->digits[i].freq1;
			d_map.digits[i].freq2 = digit_map->digits[i].freq2;
		}

		st = pjmedia_tonegen_set_digit_map(port, &d_map);
		if (st != PJ_SUCCESS)
		{
			PJ_LOG(3, ("DTMFgen::Init", "ERROR: pjmedia_tonegen_set_digit_map returns error"));
			pj_pool_release(pool);
			pool = NULL;
			port = NULL;
			slot_id = PJSUA_INVALID_ID;
			return -1;
		}
	}

	st = pjsua_conf_add_port(pool, port, &slot_id);
	if (st != PJ_SUCCESS)
	{
		PJ_LOG(3, ("DTMFgen::Init", "ERROR: pjsua_conf_add_port returns error"));
		pj_pool_release(pool);
		pool = NULL;
		port = NULL;
		slot_id = PJSUA_INVALID_ID;
		return -1;
	}

	if (pjsua_conf_connect(slot_id, pjsua_call_get_conf_port(call_id)) != PJ_SUCCESS)
	{
		PJ_LOG(3, ("DTMFgen::Init", "ERROR: DTMF slot cannot be connected to call_id"));
		pjsua_conf_remove_port(slot_id);
		pj_pool_release(pool);
		pool = NULL;
		port = NULL;
		slot_id = PJSUA_INVALID_ID;
		return -1;
	}

	_digits = (pjmedia_tone_digit *) pj_pool_alloc(pool, sizeof(pjmedia_tone_digit) * count);
	if (_digits == NULL)
	{
		PJ_LOG(3, ("DTMFgen::Init", "ERROR: pj_pool_alloc returns NULL"));
		pjsua_conf_disconnect(slot_id, pjsua_call_get_conf_port(call_id));
		pjsua_conf_remove_port(slot_id);
		pj_pool_release(pool);
		pool = NULL;
		port = NULL;
		slot_id = PJSUA_INVALID_ID;
		return -1;
	}

	short volume = 0;
	if (pcmA)
	{
		volume = SignalGen::dBm0_to_lineal_volume_a_law(volume_dbm0);
	}
	else
	{
		volume = SignalGen::dBm0_to_lineal_volume_mu_law(volume_dbm0);
	}

	for (unsigned int i = 0; i < count; i++)
	{
		if (digits[i].digit == ',')
		{
			//Si recibimos la pausa simulamos que es otro digito del map, pero con el tiempo de on a cero
			_digits[i].digit = digit_map->digits[0].digit;
			_digits[i].off_msec = digits[i].off_msec;
			_digits[i].on_msec = 0;
			_digits[i].volume = 1;
		}
		else
		{
			_digits[i].digit = digits[i].digit;
			_digits[i].off_msec = digits[i].off_msec;
			_digits[i].on_msec = digits[i].on_msec;
			_digits[i].volume = volume;
		}
	}

	st = pjmedia_tonegen_play_digits(port, count, _digits, 0);
	if (st != PJ_SUCCESS)
	{
		PJ_LOG(3, ("DTMFgen::Init", "ERROR: pjmedia_tonegen_play_digits returns error"));
		pjsua_conf_disconnect(slot_id, pjsua_call_get_conf_port(call_id));
		pjsua_conf_remove_port(slot_id);
		pj_pool_release(pool);
		pool = NULL;
		port = NULL;
		slot_id = PJSUA_INVALID_ID;
		_digits = NULL;
		return -1;
	}

	return 0;
}

int DTMFgen::End(int call_id)
{
	if (slot_id != PJSUA_INVALID_ID)
	{
		pjmedia_tonegen_stop(port);
		pjsua_conf_disconnect(slot_id, pjsua_call_get_conf_port(call_id));
		pjsua_conf_remove_port(slot_id);
	}
	if (pool != NULL)
	{
		pj_pool_release(pool);
		pool = NULL;
	}
	return 0;
}









