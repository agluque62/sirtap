#ifndef __NOISEGEN__
#define __NOISEGEN__

class NoiseGenerator
{
public:

	enum NoiseType { White, Pink };

	pjsua_conf_port_id _Slot;

	NoiseGenerator(NoiseType type);
	~NoiseGenerator();
	int Init(float dBm0level, pj_bool_t leyA);

private:
	
	float dBm0_level;
	pjmedia_port _Port;
	pj_pool_t *_Pool;
	NoiseType _Type;
	pj_bool_t _LeyA;

	double mAmp;
	float buf0, buf1, buf2, buf3, buf4, buf5, buf6;

	static pj_status_t Noise_get_frame(pjmedia_port* port, pjmedia_frame* frame);
	static pj_status_t Pink_Noise_get_frame(pjmedia_port* port, pjmedia_frame* frame);

};

#endif



