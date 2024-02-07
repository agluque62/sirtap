// stdafx.h: archivo de inclusión de los archivos de inclusión estándar del sistema
// o archivos de inclusión específicos de un proyecto utilizados frecuentemente,
// pero rara vez modificados
//
#if !defined(STDAFX_H__INCLUDED_)
#define STDAFX_H__INCLUDED_

//#pragma once

#ifdef _WIN32

	#include "targetver.h"
	#include <stdio.h>
	#include <tchar.h>

	#define WORKING_DIR		"./"

#else
	#include <stdio.h>
	#include <unistd.h>

	#ifdef _PPC82xx_
		#define WORKING_DIR		"/home/rec/"	//Permite acceso a ficheros cuando arranca desde "carper"
	#else
		#define WORKING_DIR		"./"
	#endif


#endif

#include <string>
#include <iostream>
#include <map>


#define ON_WORKING_DIR(m)	(((std::string)WORKING_DIR)+((std::string)m)).c_str()

#define RTSPURL_ALGIRDAS			1		//ESC. 15.01.2016
//#define TEARDOWN_NO_CONECTA_TCP	1		//ESC. 15.11.2016

#define ACTIVA_KEEPALIVE_TCP		1		//ESC. 10.04.2017
//#undef ACTIVA_KEEPALIVE_TCP

#define CHECK_TCP_SOCKET_WHEN_NO_DATA	1	//ESC. 25.09.2018. Activa define

// TODO: mencionar aquí los encabezados adicionales que el programa necesita


#endif
