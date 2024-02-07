//
// UG5KEd137b4Service.cpp: define el punto de entrada de la aplicaci�n de consola.
//
#ifdef _WIN32
	#include "stdafx.h"
#else
	#undef _WINSERVICE 
#endif

#include "UG5KRecordService.h"
#include "Utilidades.h"
#include <stdio.h>

#define VRS_ID_SOFTWARE			"UG5K ED-137b4 SERVICE"
#define VRS_VERSION_MAYOR		1
#define VRS_VERSION_MENOR_A		0
#define VRS_VERSION_MENOR_B		2

#ifdef _WIN32
	static char acBuildString[] = "Version WIN";
#else
	#include <signal.h>
	extern char acBuildString[];					//ESC. 02.12.2915. Se emplea en PPC para que el ejecutable incluya la fecha de generaci�n.
#endif


char *acStrVersion()
{
	static char buffer[128];
	sprintf(buffer, " %s   V%d.%d.%d (%s)",
                  VRS_ID_SOFTWARE,
                  VRS_VERSION_MAYOR, VRS_VERSION_MENOR_A, VRS_VERSION_MENOR_B,
                  acBuildString );
	return buffer;
}


static void sleep (int msec);
//static void RunServer ();


// ===---
#ifdef _WIN32
	#include <conio.h>
#else
	#include <termios.h>
	#include <unistd.h>
	#include <fcntl.h>
 
	int _kbhit(void)
	{
	  struct termios oldt, newt;
	  int ch;
	  int oldf;
 
	  tcgetattr(STDIN_FILENO, &oldt);
	  newt = oldt;
	  newt.c_lflag &= ~(ICANON | ECHO);
	  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
	  ch = getchar();
 
	  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	  fcntl(STDIN_FILENO, F_SETFL, oldf);
 
	  if(ch != EOF)
	  {
		ungetc(ch, stdin);
		return 1;
	  }
 
	  return 0;
	}


	static struct termios oldT, newT;

	// Initialize new terminal i/o settings
	void initTermios(int echo) 
	{
		tcgetattr(0, &oldT);					// grab old terminal i/o settings
		newT = oldT;							// make new settings same as old settings
		newT.c_lflag &= ~ICANON;				// disable buffered i/o
		newT.c_lflag &= echo ? ECHO : ~ECHO;	// set echo mode
		tcsetattr(0, TCSANOW, &newT);			//use these new terminal i/o settings now
	}

	// Restore old terminal i/o settings
	void resetTermios(void) 
	{
		tcsetattr(0, TCSANOW, &oldT);
	}

	char getch_linux(int echo) 
	{
		char ch;
		initTermios(echo);
		ch = getchar();
		resetTermios();
		return ch;
	}

	// Read 1 character without echo */
	char _getch(void) 
	{
		return getch_linux(0);
	}
#endif
// ===---



#ifdef _WIN32
int _tmain(int argc, _TCHAR* argv[] )
#else
int main(int argc, char* argv[] )
#endif
{
	

#ifdef _WIN32
	NLOG_INFO("===---===---===---===---===---===---===---===");
	const char *buildString = "'__DATE__, __TIME__'";
	NLOG_INFO("UG5K ED-137b4-Service. FECHA: %s", buildString);
	//NLOG_INFO("UG5K ED-137b4-Service. PULSE 'x' para SALIR.");
#else

	static int iDescPerro=-1;			// Descriptor de fichero para refrescar al perro.
	static unsigned char ucPerro=0;		// Envio al Perro...		

	if (argc == 2)
	{
		if (!strncmp(argv[1], "/V", 3))
		{
			printf( " %s   V%d.%d.%d (%s)\n",
						  VRS_ID_SOFTWARE,
						  VRS_VERSION_MAYOR, VRS_VERSION_MENOR_A, VRS_VERSION_MENOR_B,
						  acBuildString );

			//NLOG_INFO("PETICION de VERSION: (%s).", acBuildString);
		
			Sleep(200);					//ESC. 20.02.2017. OJO! Sirve para que NLog almacena registro 

			return -1;
		}
		else 
		{    // Si se ejecuta desde CARPER, nos pasan como primer parametro el descriptor de pipe para refrescar al perro (a CARPER).
			iDescPerro = atoi(argv[1]);
		}
	}
    CodeBase::plogInit();

	NLOG_INFO("===---===---===---===---===---===---===---===");
	NLOG_INFO("UG5K ED-137b4-Service: (%s).", acBuildString);

	if (iDescPerro != -1)					//ESC. 10.05.2016. Impide que el "carper" reinice el agente, en el caso de que el arranque sea lento
		write( iDescPerro, &ucPerro, 1 );
#endif

	try
	{
		UG5KRecordService _service;

		_service.Start();

		sleep(500);

		while ( _service.IsRunning() )
		{
			int iHit;

			iHit= _kbhit();
			if(iHit)
			{
				char cTecla;

				cTecla= _getch();

				if((cTecla == 'x')|| (cTecla == 'X'))
					break;
			}

#define PERIODO_WRITE_CARPER	500						//milisegundos

#ifndef _WIN32 
			
			if (iDescPerro != -1)
				write( iDescPerro, &ucPerro, 1 );		//Keep-Alive para el carper


			//ESC. 22.05.2017. En caso de que CPU=>Reserva, y tenga recursos, cierra m�dulo Grabaci�n.
			int iUmbralVerifCPUmode= 30 * (1000 / PERIODO_WRITE_CARPER);//ESC. 22.05.2017. Cada 30" ejecuta este procedimiento.
			static int iCountForCheckCPUmode= iUmbralVerifCPUmode;

			if(++iCountForCheckCPUmode >= iUmbralVerifCPUmode)		
			{
				iCountForCheckCPUmode= 0;

				if (! _service.CPU_Pasarela_Es_Principal())
				{
					if (_service.GetNumItfsConfigForRecording() > 0)
					{
						NLOG_INFO("UG5K ED-137b4. main(). GW=>'Reserva', recording '%d' recursos", _service.GetNumItfsConfigForRecording());

						break;		//### ESC. 22.05.2017. Sale del bucle y cierra m�dulo Grabaci�n.
					}
				}
			}
			//--ESC. 22.05.2017
#endif

			sleep(PERIODO_WRITE_CARPER);	
		}


		NLOG_INFO("UG5K ED-137b4. main(). _service.Dispose()...");

		_service.Dispose();
	}
	catch (...)
	{
		NLOG_ERROR("UG5K ED-137b4. main(). Se ha producido una excepcion incontralada");
	}


	NLOG_INFO("UG5K ED-137b4-Service. FIN");
	return 0;
}


static void sleep(int msec)
{
#ifdef _WIN32
	Sleep(msec);
#else
	usleep(msec*1000);
#endif
}




