#include "RTPSession.h"

#ifdef _WIN32
 #include <WinSock2.h>		//ESC. 04.12.2015. Añadido
#else
 #include <arpa/inet.h>
 #include <inttypes.h>
#endif

#include "NLOG.h"
#include <stdlib.h>			//srand, rand
#include <time.h>




/** */
RTPSession::RTPSession()
{
	srand ((unsigned int)time(NULL));
}

/** */
RTPSession::~RTPSession(void)
{
}

/** */
void RTPSession::Init()
{
	_bActivaBitMarker= true;	//ESC. 04.01.2017
	_secuencia = 0;
	_timestamp = rand();		//note that the RTP timestamp should start at a random number and not at zero
	_SSRC = rand();				//ESC. Synchronization Source.  This identifier SHOULD be chosen randomly
}

void RTPSession::SetBitMarker()
{
	_bActivaBitMarker= true;
}

void RTPSession::HeaderInsert(void *donde)
{
	unsigned char * phdr = (unsigned char *) donde;
	
	phdr[0] = 0x80;				//Bits'0-1' (Network Order), indican versión RTP (versión 2)
								//Bit'3' (Network Order), Indicates presence of an Extension header (sin extensión de cabecera).

	phdr[1] = (unsigned char) PAYLOAD_TYPE_PCMA;
	if(_bActivaBitMarker)
	{
		//ESC. 09.01.2017. En el primer paquete RTP de la sesión, Activa bit Marker (Bit'8' Network Order)
		phdr[1] = (unsigned char) PAYLOAD_TYPE_PCMA | 0x80;
		_bActivaBitMarker= false;
	}

	u_short sequence = htons(_secuencia ++);
    u_long timestamp = htonl(getCurrentTimestamp());
	memcpy(&(phdr[2]), &sequence, sizeof(u_short) );
	memcpy(&(phdr[4]), &timestamp, sizeof(u_long) );
	memcpy(&(phdr[8]), &_SSRC, sizeof(u_long) );			//ESC. 20.07.2016. Este campo debería ser _SSRC. Antes '0x00'
}

/** */
unsigned long RTPSession::getCurrentTimestamp() 
{
#define INC_TSTAMP_20MS_SAMPLE_8KHZ	160
	// NOTA: the PCMA codec has a fixed sample clock of 8kHz. That results in a timetick of 125 µs, for that given codec
	
	_timestamp += INC_TSTAMP_20MS_SAMPLE_8KHZ;
	return _timestamp;
//	return ( _timestamp + (unsigned long) _secuencia );		//ESC. 07.04.2016, comentado código de Arturo
}
