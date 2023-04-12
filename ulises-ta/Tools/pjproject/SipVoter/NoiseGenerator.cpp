#include "Global.h"
#include "NoiseGenerator.h"
#include "SignalGen.h"

#define THIS_FILE "NoiseGenerator.cpp"

NoiseGenerator::NoiseGenerator(NoiseType type)
{
	_Pool = NULL;
	_Slot = PJSUA_INVALID_ID;
	_Type = type;
	_LeyA = PJ_TRUE;

	buf0 = buf1 = buf2 = buf3 = buf4 = buf5 = buf6 = 0;
	mAmp = 1.0;
}

NoiseGenerator::~NoiseGenerator()
{
	if (_Slot != PJSUA_INVALID_ID) pjsua_conf_remove_port(_Slot);
	if (_Pool != NULL) pj_pool_release(_Pool);
}

int NoiseGenerator::Init(float dBm0level, pj_bool_t leyA)
{
	_Pool = pjsua_pool_create(NULL, 1024, 512);
	if (_Pool == NULL)
	{
		PJ_LOG(3, (THIS_FILE, "pjsua_pool_create retorna error"));
		return -1;
	}

	pj_memset(&_Port, 0, sizeof(pjmedia_port));

	pj_status_t st = pjmedia_port_info_init(&_Port.info, &(pj_str("WNOI")), PJMEDIA_PORT_SIGNATURE('W', 'N', 'O', 'I'),
		SAMPLING_RATE, CHANNEL_COUNT, BITS_PER_SAMPLE, SAMPLES_PER_FRAME);
	if (st != PJ_SUCCESS)
	{
		pj_pool_release(_Pool);
		_Pool = NULL;
		PJ_LOG(3, (THIS_FILE, "pjmedia_port_info_init retorna error"));
		return -1;
	}

	dBm0_level = dBm0level;
	if (_LeyA && dBm0_level > 3.14f) dBm0_level = 3.14f;
	else if (!_LeyA && dBm0_level > 3.17f) dBm0_level = 3.17f;

	_Port.port_data.pdata = this;
	if (_Type == Pink) _Port.get_frame = Pink_Noise_get_frame;
	else _Port.get_frame = Noise_get_frame;
	_Port.reset = NULL;	
	_Port.put_frame = NULL;

	st = pjsua_conf_add_port(_Pool, &_Port, &_Slot);
	if (st != PJ_SUCCESS)
	{
		PJ_LOG(3, (THIS_FILE, "ERROR: pjsua_conf_add_port returns error"));
		pj_pool_release(_Pool);
		_Pool = NULL;
		_Slot = PJSUA_INVALID_ID;
		return -1;
	}

	return 0;
}

pj_status_t NoiseGenerator::Noise_get_frame(pjmedia_port* port, pjmedia_frame* frame)
{
	NoiseGenerator* noisegen = (NoiseGenerator * ) port->port_data.pdata;
	if (noisegen == NULL)
	{
		frame->type = PJMEDIA_FRAME_TYPE_NONE;
		return PJ_SUCCESS;
	}

	int size_16bits = frame->size / 2;

	float nivel_16bits;
	if (noisegen->_LeyA)
	{			
		/*
			Todas estas formulas se obtienen de forma empirica, comparando los resultados 
			que el DSP obtiene al convertir en analogico y calcular los dBm
			Consideramos 
			Ley A. 12-bit + signo = 13
			Valor maximo : 4095 => 0 dBm0

			nivel_13bits = 4095 * pow(10, volume_dbm0 / 20);
			nivel_16bits = nivel_13bits * 32767 / 4095;

			Se ha visto que restando 0.5 dBm se obtiene lo requerido: Quedando
			nivel_13bits = 4095 * pow(10, (volume_dbm0-0.5) / 20);
			nivel_16bits = nivel_13bits * 32767 / 4095;

			Quedando finalmente:
			nivel_13bits = pow(10, (volume_dbm0-0.5) / 20);
			nivel_16bits = nivel_13bits * 32767;
		*/

		float nivel_13bits = (float)pow(10.0f, (noisegen->dBm0_level - 0.5f) / 20.0f);
		nivel_16bits = 32767.0f * nivel_13bits;
	}
	else
	{
		/*
			Todas estas formulas se obtienen de forma empirica, comparando los resultados
			que el DSP obtiene al convertir en analogico y calcular los dBm
			Consideramos
			Ley A. 13-bit + signo = 13
			Valor maximo : 8191 => 0 dBm0

			nivel_13bits = 8191 * pow(10, volume_dbm0 / 20);
			nivel_16bits = nivel_13bits * 32767 / 8191;

			Se ha visto que restando 0.5 dBm se obtiene lo requerido: Quedando
			nivel_13bits = 8191 * pow(10, (volume_dbm0-0.5) / 20);
			nivel_16bits = nivel_13bits * 32767 / 8191;

			Quedando finalmente:
			nivel_13bits = pow(10, (volume_dbm0-0.5) / 20);
			nivel_16bits = nivel_13bits * 32767;
		*/

		float nivel_13bits = (float)pow(10.0f, (noisegen->dBm0_level - 0.5f) / 20.0f);
		nivel_16bits = 32767.0f * nivel_13bits;
	}

	pj_int16_t* p_out_samp = (pj_int16_t*)frame->buf;

	for (int i = 0; i < size_16bits; i++)
	{
		double rnd = (((double) rand()) / ((double) RAND_MAX)) * 2.0;		//Random entre 0-2
		rnd -= 1.0;	// random entre -1 y 1
		
		rnd *= nivel_16bits;
		pj_int16_t int16_rnd = (pj_int16_t)rnd;							
		*p_out_samp = int16_rnd;
		p_out_samp++;
	}

	frame->type = PJMEDIA_FRAME_TYPE_AUDIO;	

	return PJ_SUCCESS;
}

pj_status_t NoiseGenerator::Pink_Noise_get_frame(pjmedia_port* port, pjmedia_frame* frame)
{
	NoiseGenerator* noisegen = (NoiseGenerator*)port->port_data.pdata;
	if (noisegen == NULL)
	{
		frame->type = PJMEDIA_FRAME_TYPE_NONE;
		return PJ_SUCCESS;
	}

	int size_16bits = frame->size / 2;

	float white;
	float amplitude;
	
	float div = ((float)RAND_MAX) / 2.0f;

	float* buffer = (float*)malloc(sizeof(float) * size_16bits);

	float nivel_16bits;
	if (noisegen->_LeyA)
	{
		/*
			Todas estas formulas se obtienen de forma empirica, comparando los resultados
			que el DSP obtiene al convertir en analogico y calcular los dBm
			Consideramos
			Ley A. 12-bit + signo = 13
			Valor maximo : 4095 => 0 dBm0

			nivel_13bits = 4095 * pow(10, volume_dbm0 / 20);
			nivel_16bits = nivel_13bits * 32767 / 4095;

			Se ha visto que sumando 10.5 dBm se obtiene lo requerido: Quedando
			nivel_13bits = 4095 * pow(10, (volume_dbm0+10.5) / 20);
			nivel_16bits = nivel_13bits * 32767 / 4095;

			Quedando finalmente:
			nivel_13bits = pow(10, (volume_dbm0+10.5) / 20);
			nivel_16bits = nivel_13bits * 32767;
		*/

		float nivel_13bits = (float)pow(10.0f, (noisegen->dBm0_level + 10.5f) / 20.0f);
		nivel_16bits = 32767.0f * nivel_13bits;
	}
	else
	{
		/*
			Todas estas formulas se obtienen de forma empirica, comparando los resultados
			que el DSP obtiene al convertir en analogico y calcular los dBm
			Consideramos
			Ley A. 13-bit + signo = 13
			Valor maximo : 8191 => 0 dBm0

			nivel_13bits = 8191 * pow(10, volume_dbm0 / 20);
			nivel_16bits = nivel_13bits * 32767 / 8191;

			Se ha visto que sumando 10.5 dBm se obtiene lo requerido: Quedando
			nivel_13bits = 8191 * pow(10, (volume_dbm0+10.5) / 20);
			nivel_16bits = nivel_13bits * 32767 / 8191;

			Quedando finalmente:
			nivel_13bits = pow(10, (volume_dbm0+10.5) / 20);
			nivel_16bits = nivel_13bits * 32767;
		*/

		float nivel_13bits = (float)pow(10.0f, (noisegen->dBm0_level + 10.5f) / 20.0f);
		nivel_16bits = 32767.0f * nivel_13bits;
	}

	pj_int16_t* p_out_samp = (pj_int16_t*)frame->buf;

	// based on Paul Kellet's "instrumentation grade" algorithm.

	  // 0.129f is an experimental normalization factor.
	amplitude = (float) (noisegen->mAmp * 0.129);
	for (decltype(size_16bits) i = 0; i < size_16bits; i++)
	{
		white = (rand() / div) - 1.0f;
		noisegen->buf0 = 0.99886f * noisegen->buf0 + 0.0555179f * white;
		noisegen->buf1 = 0.99332f * noisegen->buf1 + 0.0750759f * white;
		noisegen->buf2 = 0.96900f * noisegen->buf2 + 0.1538520f * white;
		noisegen->buf3 = 0.86650f * noisegen->buf3 + 0.3104856f * white;
		noisegen->buf4 = 0.55000f * noisegen->buf4 + 0.5329522f * white;
		noisegen->buf5 = -0.7616f * noisegen->buf5 - 0.0168980f * white;
		buffer[i] = amplitude *
			(noisegen->buf0 + noisegen->buf1 + noisegen->buf2 + noisegen->buf3 + noisegen->buf4 + noisegen->buf5 + noisegen->buf6 + white * 0.5362f);
		noisegen->buf6 = white * 0.115926f;

		float dat = buffer[i] * nivel_16bits;
		pj_int16_t int16_rnd = (pj_int16_t)dat;
		*p_out_samp = int16_rnd;

		p_out_samp++;
	}

	frame->type = PJMEDIA_FRAME_TYPE_AUDIO;

	free(buffer);

	return PJ_SUCCESS;
}













