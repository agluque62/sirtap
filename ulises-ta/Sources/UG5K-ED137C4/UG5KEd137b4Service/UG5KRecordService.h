#if !defined(UG5KRECORDSERVICE_H__INCLUDED_)
#define UG5KRECORDSERVICE_H__INCLUDED_

// #pragma once
#include "LocalConfig.h"
#include "MiSocket.h"
#include "Thread.h"
#include "NLOG.h"


/** */
#include <map>
#include <vector>

//ESC. 14.01.2016
#define TX_CLIENT_NO_ERROR			0
#define TX_CLIENT_NO_CFG_IPCGW		1
#define TX_CLIENT_NO_EXISTE_SESION	2
#define TX_CLIENT_NUM_MAX_SESIONES	3
#define TX_CLIENT_CMD_NOTFOUND		4
#define TX_CLIENT_YA_EXISTE_SESION	5
#define TX_CLIENT_ERR_CREATE_SESION	6
#define CODIGO_MAX_ERROR			7
#define TX_CLIENT_RX_MEDIA_NO_RECORDING	7

#define ID_NO_FRQ	"NoFrequencyID"
//--ESC

class UG5KRecordService :
	public CThread
{
protected:
	std::map<std::string, void *> _sesiones;
	CUDPSocket _sckRxUDP;
//	CUDPSocket _sckTxUDP;							//ESC. 17.04.2017. Comentado, empleamos un �nico socket UDP

private:
	int _portRx;
//	CIPAddress _IPrespuesta;						//ESC. Comentado
	int _maxcanales;
	int _ThreadsRecursos;
	std::string _ipPasarela;
	bool _finalizarServ;
	CIPAddress _CIP_RecA, _CIP_RecB;
	std::string _uri;
	bool _bDualRecSrv;								//ESC. 27.01.2016. A�adido
	bool _bModoVOTERSrv;							//ESC. 24.01.2017. A�adido
	bool _EnableGrabacionED137;

public:
	UG5KRecordService(void);
	~UG5KRecordService(void);

	void Dispose();
	bool GetDualRecordCfg();						//ESC. 27.01.2016. A�adido
	bool GetModoVOTER();										//ESC. 25.01.2017
//	bool GetModoVOTER_VCStelephonyUA(RTSPSession &pRtspSes);	//ESC. 26.01.2017
	bool CPU_Pasarela_Es_Principal();							//ESC. 22.05.2017
	int  GetNumItfsConfigForRecording();									//ESC. 22.05.2017


protected:
	void Init();
	int Interprete(int Recibidos, void *cmd, int &estSesRecA, int &estSesRecB);

private:
	void EnviarRespuestaCliente(int valor, CIPAddress ip, int estSesRecA, int estSesRecB);
	void StopThreads_sesiones();
	void AvisarFinSesiones();

	void Run();
	bool ValidarConfiguracion();
};

#endif
