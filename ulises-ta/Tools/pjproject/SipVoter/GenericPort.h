/**
 * @file GenericPort.cpp
 * @brief Gestion de un 'puerto' de media generico en CORESIP.dll
 * Tiene dos puertos _UpStreamPort. Con PutInGenericPort se escribe en un buffer jitter, el cual luego es leido en la función get_frame. Con esto
 *									implementamos un puerto de subida con un buffer jitter. Puede servir para mandar audio a una llamada call_id
 *					_DownStreamPort. Con GetFromGenericPort, se lee de un buffer jitter, el cual es escrito en put_frame. Con esto implementamos un 
 *									puerto de subida, por ejemplo para recibir audio de una llamada call_id
 * 
 *	Implementa la clase 'GenericPort'.
 *
 *	@addtogroup CORESIP
 */
/*@{*/

#ifndef __CORESIP_GENERICPORT_H__
#define __CORESIP_GENERICPORT_H__

#include "Global.h"

class GenericPort
{
public:
	static const int MAX_GENERIC_PORTS = CORESIP_MAX_SOUND_RX_PORTS;
	static GenericPort* _Generic_Ports[MAX_GENERIC_PORTS];
	
	pjsua_conf_port_id _UpStreamSlot;		//su Slot de la conferencia pjmedia
	pjsua_conf_port_id _DownStreamSlot;		//su Slot de la conferencia pjmedia
public:
	static int CreateGenericPort(int jitter_buff_size);
	static void DestroyGenericPort(int gen_port_id);
	static void PutInGenericPort(int gen_port_id, short* buffer, int buffer_length_in_bytes, pj_bool_t blocking);
	static void GetFromGenericPort(int gen_port_id, short* buffer, int buffer_length_in_bytes, pj_bool_t blocking);
	static void GetJitterStatusGenericPort(int gen_port_id, unsigned int* size);	
	
	GenericPort();
	~GenericPort();	
	int Init(int jitter_buff_size);

	
private:
	static const unsigned int MAX_COUNT_IN_JBUF = 100;

	static pj_status_t UpStreamGetFrame(pjmedia_port* port, pjmedia_frame* frame);
	static pj_status_t UpStreamReset(pjmedia_port* this_port);
	static pj_status_t DownStreamPutFrame(pjmedia_port* port, const pjmedia_frame* frame);
	static pj_status_t DownStreamReset(pjmedia_port* this_port);
		
	pj_pool_t * _Pool;

	pjmedia_port _UpStreamPort;				//Puerto de subida.	
	pj_lock_t * _UpStreamLock;				
	pjmedia_jbuf *_UpStreamJbuf;			//Buffer jitter
	pjmedia_plc *_UpStreamPlc;				//PLC para generar muestras
	pj_uint16_t UpStreamSeq;				//Numero de secuencia
	char Prev_UpStream_frame_type;			//Valor previo del tipo de frame leido del buffer

	pjmedia_port _DownStreamPort;			//Puerto de bajada	
	pj_lock_t* _DownStreamLock;				
	pjmedia_jbuf* _DownStreamJbuf;			//Buffer jitter
	pjmedia_plc* _DownStreamPlc;			//PLC para generar muestras
	pj_uint16_t DownStreamSeq;				//Numero de secuencia
	char Prev_DownStream_frame_type;		//Valor previo del tipo de frame leido del buffer

	int _Jitter_buff_size;					

	pj_sem_t* semGet;
	pj_sem_t* semPut;

	pj_bool_t destroying;

private:

	int Put(short* buffer, int buffer_length_in_bytes, pj_bool_t blocking);
	int Get(short* buffer, int buffer_length_in_bytes, pj_bool_t blocking);
	void Dispose();
	
};


#endif

/*@}*/

