#if !defined(LOCALCONFIG_H__INCLUDED_)
#define LOCALCONFIG_H__INCLUDED_

#include <iostream>
#include <string>
#include <list>
#include <map>
#include <vector>
#include <cstdlib>

#include "stlini.h"

using namespace std;

class LocalConfig
{
private:
	int LogLevel(const char *tipo);
	INIFile ini;

public:
	LocalConfig(void);
	~LocalConfig(void);

public:
	string ServiceIpDefault();
	int ServiceUdpPort();
//	string ServiceUdpResultIp();			//ESC. Comentado
//	int ServiceUdpResultPort();				//ESC. Comentado

	string RTSPIp_RecA();
	int RTSPPortA();
	string RTSPUri_frmt();

	bool GetEnableGrabacionED137();

	int MaxSessions();
	bool GrabadorDual();					//ESC. 27.01.2016. Añadido
	string RTSPIp_RecB();					//ESC. 02.02.2016. Añadido
	int RTSPPortB();
	bool ModoVOTER();						//ESC. 18.01.2017. Añadido
};

#endif
