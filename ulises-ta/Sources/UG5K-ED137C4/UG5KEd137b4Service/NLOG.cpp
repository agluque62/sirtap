#include "stdafx.h"
#include <stdio.h>
#include <stdarg.h>

#include "NLOG.h"
//#include "local-config.h"
//#include "Utilidades.h"
//#include "sistema.h"

/** Preferencias de Logger */
#if _EASYLOGGER
 #undef _EASYLOGGER_CV11
 #undef _PLOGGER
 #include "easylogging++891.h"
#elif _EASYLOGGER_CV11
 #undef _EASYLOGGER
 #undef _PLOGGER
 #include "easylogging++.h"
#else
 #undef _EASYLOGGER
 #undef _EASYLOGGER_CV11

//	#define _PLOGNAME			1
//	#define _UDPAPPENDER_
	#define _PLOG_MULTIPLE_
 #include "plog/Log.h"
#endif

#if _EASYLOGGER
	_INITIALIZE_EASYLOGGINGPP;
	#define LOG _LOG
	#define LOGCONF	ON_WORKING_DIR("easylog891.conf")
#elif _EASYLOGGER_CV11
	INITIALIZE_EASYLOGGINGPP;
	#define LOGCONF	ON_WORKING_DIR("easylog.conf")
#else
	static plog::LogConfig cfg;
	#define LOGCONF	ON_WORKING_DIR("plog.conf")
#endif

#ifdef _PLOG_MULTIPLE_
	// Tipos de Logs.
	enum emPlogs
	{
		plogFile = 0,
		plogConsole= 1,
		plogNetwork= 2
	};
#endif

/**
*/
CInitNlog::CInitNlog()
{
#if _EASYLOGGER
    // Load configuration from file
    easyloggingpp::Configurations conf(LOGCONF);
    // Reconfigure single logger
    easyloggingpp::Loggers::reconfigureLogger("default", conf);
    // Actually reconfigure all loggers instead
    easyloggingpp::Loggers::reconfigureAllLoggers(conf);
    // Now all the loggers will use configuration from file
#elif _EASYLOGGER_CV11
	 el::Configurations conf(LOGCONF);
	 el::Loggers::reconfigureLogger("default", conf);
	 el::Loggers::reconfigureAllLoggers(conf);
#else

#ifdef _WIN32
	cfg.Configure("./wplog.conf");
#else
	cfg.Configure(LOGCONF);
#endif
	string id=cfg.UdpId();

#ifdef _PLOG_MULTIPLE_
	static plog::ConsoleAppender<plog::ConsoleFormatter> consoleAppender;
	static plog::NetUdpAppender<plog::Log4viewFormatter> Log4viewAppender(cfg.IpUpd().c_str(), cfg.PortUdp(), id.c_str());

	plog::init<plogFile>(plog::verbose, cfg.FileName().c_str(), cfg.FileSize(), cfg.NumberOfFile());
	plog::init<plogConsole>(plog::verbose, &consoleAppender);
	plog::init<plogNetwork>(plog::verbose, &Log4viewAppender);

#else		
	static plog::RollingFileAppender<plog::TxtFormatter> txtfileAppender(cfg.FileName().c_str(), cfg.FileSize(), cfg.NumberOfFile());
	static plog::RollingFileAppender<plog::CsvFormatter> csvfileAppender(cfg.FileName().c_str(), cfg.FileSize(), cfg.NumberOfFile());
#ifdef _UDPAPPENDER_
	static plog::NetUdpAppender<plog::TxtFormatter> netAppender("192.168.1.31", 877);
#endif
	
	if (cfg.ToStdout())
	{
		static plog::ConsoleAppender<plog::ConsoleFormatter> consoleAppender;
		if (cfg.FileFormat()=="csv")
		{
#ifdef _UDPAPPENDER_
			plog::init(plog::debug, &csvfileAppender).addAppender(&consoleAppender).addAppender(&netAppender);
#else
			plog::init(plog::debug, &csvfileAppender).addAppender(&consoleAppender)/*.addAppender(&netAppender)*/;
#endif
		}
		else
		{
#ifdef _UDPAPPENDER_
			plog::init(plog::debug, &txtfileAppender).addAppender(&consoleAppender).addAppender(&netAppender);
#else
			plog::init(plog::debug, &txtfileAppender).addAppender(&consoleAppender)/*.addAppender(&netAppender)*/;
#endif
		}
	}
	else
	{
		if (cfg.FileFormat()=="csv")
			plog::init(plog::debug, &csvfileAppender); 
		else
			plog::init(plog::debug, &txtfileAppender); 
	}

#endif
#endif

//#ifndef _WIN32
//	isSem=false;
//	isParent=false;
//#endif
}

/**
 */
CInitNlog::~CInitNlog()
{
//#ifndef _WIN32
//	close_sem();
//#endif
}

//#ifndef _WIN32
//void CInitNlog::init_sem(bool parent)
//{
//	if (cfg.IpcShare())
//	{
//		isParent=parent;
//		if (isParent)
//		{
//			sem_id=sem_open("nlog_sem", O_CREAT, 0600, 1);
//			if(sem_id == SEM_FAILED) {
//				perror("parent sem_open");
//			}
//			int sem_val;
//			sem_getvalue(sem_id, &sem_val);
//			if (sem_val==0)
//				sem_post(sem_id);
//			}
//		else 
//		{
//			sem_id=sem_open("nlog_sem", O_CREAT);
//			if(sem_id == SEM_FAILED) {
//				perror("child sem_open");
//			}
//		}
//		isSem=true;
//	}
//}
//
///** */
//void CInitNlog::close_sem()
//{
//	if (cfg.IpcShare() && isSem)
//	{
//		sem_close(sem_id);
//		if (isParent)
//			sem_unlink("nlog_sem");
//	}
//}
//
///** */
//void CInitNlog::wait_sem()
//{
//	if (cfg.IpcShare() && isSem)
//	{
//		if(sem_wait(sem_id) < 0) 
//		{
//			perror("sem_wait");
//		}    
//	}
//}
//
///** */
//void CInitNlog::post_sem()
//{
//	if (cfg.IpcShare() && isSem)
//	{
//		if(sem_post(sem_id) < 0) 
//		{
//			perror("sem_post");
//		}
//	}
//}
//#endif

/**
*/
#ifdef _NLOG_MAIN_
#else
 CInitNlog NLOG::_init;
#endif

/**
*/
NLOG::NLOG(void)
{
#ifdef _NLOG_MAIN_
	pInitLog = new CInitNlog();
#endif
}
/**
*/
NLOG::~NLOG(void)
{
#ifdef _NLOG_MAIN_
	delete pInitLog;
#endif
}

/** 
*/
void NLOG::_Log(eLevel level, const char *fmt, va_list args)
{
	char textString[1024] = {'\0'};
    memset(textString, '\0', sizeof(textString));
//#ifndef _WIN32
//	NLOG::_init.wait_sem();
//#endif
#ifdef _WIN32
		vsnprintf_s ( textString, sizeof(textString), fmt, args );
#else
		vsnprintf ( textString, sizeof(textString), fmt, args );
#endif

#if _EASYLOGGER || _EASYLOGGER_CV11
	switch (level)
	{
	case NLOG::Info:
		LOG(INFO) << textString;
		break;
	case NLOG::Error:
		LOG(ERROR) << textString;
		break;
	case NLOG::Debug:
		LOG(DEBUG) << textString;
		break;
	default:
		break;
	}
#endif

#ifdef _PLOGGER
	plog::Severity plogSev = level == NLOG::Info ? plog::info : level==NLOG::Error ? plog::error : level==NLOG::Debug ? plog::debug : plog::verbose;
	cfg.TestCfgChange();
#ifdef _PLOG_MULTIPLE_
#if _PLOGNAME
	if (plog::pLogProfiles[(int)plogSev].toFile)
			PLOG_(plogFile, plogSev) << textString;
	if (plog::pLogProfiles[(int)plogSev].toConsole=
			PLOG_(plogConsole, plogSev) << textString;
	if (plog::pLogProfiles[(int)plogSev].toNetwork)
		PLOG_(plogNetwork, plogSev) << textString;
#else
	if (plog::pLogProfiles[(int)plogSev].toFile)
		LOG_(plogFile, plogSev) << textString;
	if (plog::pLogProfiles[(int)plogSev].toConsole)
		LOG_(plogConsole, plogSev) << textString;
	if (plog::pLogProfiles[(int)plogSev].toNetwork)
		LOG_(plogNetwork, plogSev) << textString;
#endif
#else
#if _PLOGNAME
	PLOG_(plogSeveriy) << textString;
#else
	LOG_(plogSeveriy) << textString;
#endif
#endif
#endif

//#ifndef _WIN32
//	NLOG::_init.post_sem();
//#endif
}

/**
*/
void NLOG::Log(eLevel level, const char *fmt, ... )
{
	try
	{
		va_list args;
		va_start ( args, fmt );
		_Log(level, fmt, args);
		va_end(args);
	}
	catch(...)
	{
		return;
	}
}

/**
*/
void NLOG::LogIf(bool trueCondition, eLevel level, const char *fmt, ... )
{
	if (!trueCondition) return;

	try
	{
		va_list args;
		va_start ( args, fmt );
		_Log(level, fmt, args);
		va_end(args);
	}
	catch(...)
	{
		return;
	}

}

