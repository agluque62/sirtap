#if !defined(UTILIDADES_H__INCLUDED_)
#define UTILIDADES_H__INCLUDED_

// #pragma once

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <malloc.h>

#include <string>
#include <vector>
#include <sstream>

#define SIG_CONF_CHANGE	40

class Utilidades
{
public:
	static const char SEPARADOR_PARAMETROS = ',';
	static void split(std::vector<std::string> &tokens, const std::string &text, char sep);
	//static bool StrContains(std::string text, std::string valor);
	static std::string StrFormat ( const char *fmt, ... );
	//static unsigned char * HexStrToBin ( std::string valor );
	//static std::string BinToHexStr ( unsigned char * bin, int nbytes );
	static char * parseNextField ( char * data, int * nbytes, std::string & value );

	static int mystoi(std::string s);

#  ifdef _WINSERVICE
	static char * ExeDir ();
#  endif
};

#endif

