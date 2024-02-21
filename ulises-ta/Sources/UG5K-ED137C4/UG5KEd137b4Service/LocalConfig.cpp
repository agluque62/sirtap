#include "stdafx.h"															//ESC. 01.12.2015. Añadido
#include "LocalConfig.h"
#include "NLOG.h"															//ESC. 09.12.2015. Añadido


/**  
*/
static const char *strFile = "ug5krec-config.ini";

static const char *strSectionGeneral = "GENERAL";
static const char *strSectionGeneralMaxSessions = "MAX_SESSIONS";
static const char *strSectionGeneralDualRecord = "DUAL_RECORDER";
static const char *strSectionGeneralModoVOTER = "MODO_VOTER";

static const char *strSectionServicio = "SERVICIO";
static const char *strSectionServicioIPDef = "IP_SERVICIO";
static const char *strSectionServicioUdpPort = "PORT_IN_SERVICIO";
// ESC. El módulo de grabación responde a la IP@port origen
// static const char *strSectionServicioUdpResIP = "UDP_IP_RESPUESTA";
// static const char *strSectionServicioUdpResPort = "UDP_PORT_IN_VOIP";

static const char *strSectionRTSP = "RTSP";
static const char *strSectionRTSPServIpRecA = "IP_REC_A";
static const char *strSectionRTSPServIpRecB = "IP_REC_B";
static const char *strSectionRTSPServPortA = "PORT_RTSP";
static const char* strSectionRTSPServPortB = "PORT_RTSP2";
static const char *EnableGrabacionED137 = "EnableGrabacionED137";
//static const char *strSectionRTSPServUri = "URI";


/**
*/
LocalConfig::LocalConfig(void)
{
	ini = LoadIni(ON_WORKING_DIR(strFile));
}

/**
*/
LocalConfig::~LocalConfig(void)
{
}


/**
*/
string LocalConfig::ServiceIpDefault()
{
	string _sVal= GetIniSetting(ini, strSectionGeneral, strSectionServicioIPDef, "10.0.2.61");
	if(_sVal.empty())
	{
		_sVal= "10.0.2.61";
		NLOG_ERROR("UG5K ED-137, entrada [SERVICIO]IP_DEF. Asigna valor defecto '%s'", _sVal.c_str());
	}
	return(_sVal);
}

int LocalConfig::ServiceUdpPort()
{
	string _sVal = GetIniSetting(ini, strSectionServicio, strSectionServicioUdpPort, "65003");
	int iVal= atoi(_sVal.c_str());
	if(iVal==0)
	{
		iVal= 65003;
		NLOG_ERROR("UG5K ED-137, entrada [SERVICIO]UDP_PORT_IN_GRABACION. Asigna valor defecto '%d'", iVal);
	}
	return iVal;
}

/*
 // ESC. El módulo de grabación responde a la IP@port origen
int LocalConfig::ServiceUdpResultPort()
{
	string _sVal = GetIniSetting(ini, strSectionServicio, strSectionServicioUdpResPort, "65001");
	int iVal= atoi(_sVal.c_str());
	if(iVal==0)
	{
		iVal= 65001;
		NLOG_ERROR("UG5K ED-137, entrada [SERVICIO]UDP_PORT_IN_VOIP. Asigna valor defecto '%d'", iVal);
	}
	return iVal;
}

string LocalConfig::ServiceUdpResultIp()
{
	string _sVal= GetIniSetting(ini, strSectionServicio, strSectionServicioUdpResIP, "127.0.0.1");
	if(_sVal.empty())
	{
		_sVal= "127.0.0.1";
		NLOG_ERROR("UG5K ED-137, entrada [SERVICIO]UDP_IP_RESPUESTA. Asigna valor defecto '%s'", _sVal.c_str());
	}
	return(_sVal);
}
*/


/**
*/
int LocalConfig::RTSPPortA()
{
	string _sVal = GetIniSetting(ini, strSectionRTSP, strSectionRTSPServPortA, "554");
	int iVal= atoi(_sVal.c_str());
	if(iVal==0)
	{
		iVal= 554;
		NLOG_ERROR("UG5K ED-137, entrada [RTSP]PORT A. Asigna valor defecto '%d'", iVal);
	}
	return iVal;
}

/**
*/
int LocalConfig::RTSPPortB()
{
	string _sVal = GetIniSetting(ini, strSectionRTSP, strSectionRTSPServPortB, "554");
	int iVal = atoi(_sVal.c_str());
	if (iVal == 0)
	{
		iVal = 554;
		NLOG_ERROR("UG5K ED-137, entrada [RTSP]PORT B. Asigna valor defecto '%d'", iVal);
	}
	return iVal;
}

/**
*/
string LocalConfig::RTSPIp_RecA()
{
	string _sVal= GetIniSetting(ini, strSectionRTSP, strSectionRTSPServIpRecA, "");
	if(_sVal.empty())
	{
		_sVal= "10.249.1.210";
		NLOG_ERROR("UG5K ED-137, entrada [RTSP]%s. Asigna valor defecto '%s'", strSectionRTSPServIpRecA, _sVal.c_str());
	}
	return(_sVal);
}

bool LocalConfig::GetEnableGrabacionED137()
{
	string _sVal = GetIniSetting(ini, strSectionRTSP, EnableGrabacionED137, "");
	if (_sVal.empty())
	{
		_sVal = "False";
		NLOG_ERROR("UG5K ED-137, entrada [RTSP]%s. Asigna valor defecto '%s'", EnableGrabacionED137, _sVal.c_str());
	}

	for (unsigned int i = 0; i < _sVal.size(); i++)
	{
		_sVal[i] = std::tolower(_sVal[i]);
	}

	NLOG_INFO("LocalConfig::GetEnableGrabacionED137 %s", _sVal.c_str());

	if (_sVal == "true")
	{		
		return true;
	}
	else
	{
		return false;
	}
}

/**
*/
string LocalConfig::RTSPUri_frmt()
{
//ESC. 23.12.2015. El formateador de la URI no se carga del fichero de inicialización

	/* 
	//ESC. Formato URI empleado en Demo unit recorder de julio.2015 con Oivind
	string _sVal= "rtsp://%s:554/%s/";				//IP_cgw:554/sampler/
	*/

	//ESC. 08.01.2016. Formato URI indicado por Algirdas Mockus
#ifdef RTSPURL_ALGIRDAS
	string _sVal= "rtsp://%s:%d/iprecorder/%s";	//IP:554/iprecorder/sampler
#else
	string _sVal= "rtsp://%s@%s:%d/iprecorder";	//sampler@IP:554/iprecorder	//Formato capturado por Antonio de TX/RX Jotron
#endif

/*
	string _sVal= GetIniSetting(ini, strSectionRTSP, strSectionRTSPServUri, "rtsp://%s@%s:554/iprecorder0/");
	if(_sVal.empty())
	{
		_sVal= "rtsp://%s@%s:554/iprecorder0/";
		NLOG_ERROR("UG5K ED-137, entrada [RTSP]URI. Asigna valor defecto '%s'", _sVal.c_str());
	}
*/
	return(_sVal);
}

int LocalConfig::MaxSessions()
{
	string _sVal = GetIniSetting(ini, strSectionGeneral, strSectionGeneralMaxSessions, "16");
	int iVal= atoi(_sVal.c_str());
	if((iVal < 0)|| (iVal > 16))
	{
		iVal= 16;
		NLOG_ERROR("UG5K ED-137, entrada [GENERAL]%s. Asigna valor defecto '%d'", strSectionGeneralMaxSessions, iVal);
	}
	return iVal;
}

//ESC. 27.01.2016. Añadido
bool LocalConfig::GrabadorDual()
{
	bool bVal= false;					//ESC. Por defecto NO considera dualidad de grabadores.
	string _sVal = GetIniSetting(ini, strSectionGeneral, strSectionGeneralDualRecord, "-1");
	int iVal= atoi(_sVal.c_str());
	
	if(iVal == 1)
		bVal= true;
	else if (iVal == 0)
		bVal= false;
	else
	{
		iVal= 0;
		NLOG_ERROR("UG5K ED-137, entrada [GENERAL]%s. Asigna valor defecto '%d'", strSectionGeneralDualRecord, iVal);
	}
	return bVal;
}


//ESC. 18.01.2017. Añadido
bool LocalConfig::ModoVOTER()
{
	bool bVal= false;
	string _sVal = GetIniSetting(ini, strSectionGeneral, strSectionGeneralModoVOTER, "-1");
	int iVal= atoi(_sVal.c_str());
	
	if(iVal == 1)
		bVal= true;
	else
		bVal= false;

	return bVal;
}


string LocalConfig::RTSPIp_RecB()
{
	string _sVal= GetIniSetting(ini, strSectionRTSP, strSectionRTSPServIpRecB, "");
	if(_sVal.empty())
	{
		_sVal= "10.249.2.210";
		NLOG_ERROR("UG5K ED-137, entrada [RTSP]%s. Asigna valor defecto '%s'", strSectionRTSPServIpRecB, _sVal.c_str());
	}
	return(_sVal);
}
//--ESC
