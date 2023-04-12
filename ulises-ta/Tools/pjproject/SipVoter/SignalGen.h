#ifndef __SIGNALGEN__
#define __SIGNALGEN__

#include "NoiseGenerator.h"


class tone_info
{
	pj_pool_t* pool;
	int frequency;	
	pjmedia_port* tone_port;

public:
	pjsua_conf_port_id slot_id;

	tone_info(int freq, int volume);
	~tone_info();
	int Update_tone(int volume);
};

class DTMFgen
{
	pj_pool_t* pool;
	pjmedia_port* port;
	pjsua_conf_port_id slot_id;
	pjmedia_tone_digit *_digits;

public:
	DTMFgen();
	~DTMFgen();
	int Init(int call_id, const CORESIP_tone_digit_map* digit_map, unsigned count, const CORESIP_tone_digit digits[], float volume_dbm0, pj_bool_t pcmA);
	int End(pjsua_call_id call_id);
};

class SignalGen
{
private:

	static pjmedia_port_info _WNoise_port_info;

public:
	static void SendToneToCall(int call, unsigned int frequency, float volume_dbm0, int on);
	static void StopAllTones(int call_id);
	static int dBm0_to_lineal_volume_a_law(float volume_dbm0);
	static int dBm0_to_lineal_volume_mu_law(float volume_dbm0);
	static void SendNoiseToCall(int call_id, NoiseGenerator::NoiseType type, float volume_dbm0, int on);
	static void StopNoiseToCall(int call_id);
	static void SendDTMF(int call_id, const CORESIP_tone_digit_map* digit_map, unsigned count, const CORESIP_tone_digit digits[], float volume_dbm0);
	static void StopDTMF(int call_id);
};


#endif



