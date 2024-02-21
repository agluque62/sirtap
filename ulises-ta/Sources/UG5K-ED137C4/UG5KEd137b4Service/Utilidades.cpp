#include "Utilidades.h"
#ifdef _WINSERVICE
#	include "stdafx.h"
#	include <windows.h>
#endif


/** */
void Utilidades::split(std::vector<std::string> &tokens, const std::string &text, char sep)
{
	unsigned int start = 0, end = 0;
 
	while ((end = text.find(sep, start)) != std::string::npos) 
	{
		tokens.push_back(text.substr(start, end - start));
		start = end + 1;
	}
  
	tokens.push_back(text.substr(start));
}

/** 
bool Utilidades::StrContains(std::string texto, std::string valor)
{
	if (texto.find(valor) != std::string::npos)
		return true;
	return false;
}
*/

/** */
std::string Utilidades::StrFormat ( const char *fmt, ... )
{
	char textString[1024] = {'\0'};		// todo. Revisar este limite...

    // -- empty the buffer properly to ensure no leaks.
    memset(textString, '\0', sizeof(textString));

    va_list args;
    va_start ( args, fmt );
    vsnprintf ( textString, 1024, fmt, args );		// todo. Tambien esta aqui el limite...
    va_end ( args );
    std::string retStr = textString;
    return retStr;
}

/*
static inline unsigned char char2byte(char c)
{
  if(c>= '0' && c <= '9')
    return c - '0';
  if(c >= 'A' && c <= 'F')
    return c - 'A' + 10;
  if(c >= 'a' && c <= 'f')
    return c - 'a' + 10;
  return 0;
}

static inline char byte2char (unsigned char b)
{
  if(b <= 9)
    return b + '0';
  if(b <= 15)
    return b - 10 + 'A';
  return '0';
}

unsigned char * Utilidades::HexStrToBin ( std::string valor )
{
	int nbytes = valor.length()/2;
	const char * hex = valor.c_str();
	unsigned char *bin = (unsigned char *)malloc(nbytes);
	for ( int i = 0; i < nbytes; i = i + 1 )
	{
		bin[i] = char2byte(hex[2*i])*16 + char2byte(hex[2*i+1]);
	}
	return ( bin );
}

std::string Utilidades::BinToHexStr ( unsigned char * bin, int nbytes )
{
	std::ostringstream text;

	for ( int i = 0; i < nbytes; i = i + 1 )
	{
		text << byte2char(bin[i]/16) << byte2char(bin[i]%16);
	}
	return ( text.str() );
}
*/

char * Utilidades::parseNextField ( char * data, int * nbytes, std::string & value )
{
	int i = 0;
	if ( data != NULL )
	{
		while ( data[i] != Utilidades::SEPARADOR_PARAMETROS && i < *nbytes )
		{
			i = i + 1;
		}
		if ( data[i] == Utilidades::SEPARADOR_PARAMETROS ) 
		{
			data [i] = 0;
			char * text = &(data [0]);
			value = text;
			*nbytes = *nbytes - i -1;
			return &(data[i+1]);
		}
		else
		{
			char * text = &(data [0]);
			value = text;
			*nbytes = 0;
			return NULL;
		}
	}
	else
	{
		return NULL;
	}
}

#ifdef _WINSERVICE
char * Utilidades::ExeDir ()
{
	HMODULE hModule = GetModuleHandleW(NULL);
	WCHAR path[MAX_PATH];
	GetModuleFileNameW(hModule, path, MAX_PATH);
	char * ruta = (char *) malloc (MAX_PATH); 
	wcstombs ( ruta, path, MAX_PATH );
	int i = strlen(ruta)-1;
	while ( ruta[i] != '\\' )
	{
		ruta[i] = 0x00;
		i = i - 1;
	}
	return ( ruta );
}
#endif

int Utilidades::mystoi(std::string s)
{
	int ret = 0;
	char* sret = (char*)malloc(s.length() + 1);
	if (sret == NULL) return 0;

	size_t i = 0;
	for (i = 0; i < s.length(); i++)
	{
		sret[i] = (char)s[i];
	}
	sret[i] = '\0';
	ret = (int)atoi(sret);
	free(sret);
	return ret;
}




