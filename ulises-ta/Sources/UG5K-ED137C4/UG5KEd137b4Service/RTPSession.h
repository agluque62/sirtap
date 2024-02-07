#if !defined(RTPSESSION_H__INCLUDED_)
#define RTPSESSION_H__INCLUDED_





#define SAMPLE_RATE		8000
#define FRAME_SIZE		160
#define PAYLOAD_TYPE_PCMA	8



class RTPSession
{
public:
	RTPSession();
	~RTPSession(void);

public:
	void Init();
	void HeaderInsert(void *data);
	void SetBitMarker();				//ESC. 09.01.2017. Función para activar RTP Bit Marker


protected:
	unsigned long getCurrentTimestamp();

protected:
	//Timer _timer;
	unsigned int _secuencia;
	unsigned long _timestamp;
	unsigned long _SSRC;				//ESC. 20.07.2016. Campo añadido debería inicializarse "randomly"
	bool _bActivaBitMarker;				//ESC. 04.01.2017. Flag añadido, para activar bit Marker
};


#endif
