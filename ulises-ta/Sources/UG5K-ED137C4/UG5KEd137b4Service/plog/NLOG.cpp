#include "stdafx.h"
#include <stdio.h>
#include <stdarg.h>

#include "NLOG.h"

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

 #define _UDPAPPENDER_
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
	cfg.Configure(LOGCONF);

	//plog::init(plog::debug, cfg.FileName().c_str(), cfg.FileSize(), cfg.NumberOfFile());
	//if (cfg.ToStdout())
	//{
	//	static plog::ConsoleAppender<plog::ConsoleFormatter> consoleAppender;
	//	plog::init<1>(plog::info, &consoleAppender);
	//}
		
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

#ifndef _WIN32
	isSem=false;
	isParent=false;
#endif
}

/**
 */
CInitNlog::~CInitNlog()
{
#ifndef _WIN32
	close_sem();
#endif
}

#ifndef _WIN32
void CInitNlog::init_sem(bool parent)
{
	if (cfg.IpcShare())
	{
		isParent=parent;
		if (isParent)
		{
			sem_id=sem_open("nlog_sem", O_CREAT, 0600, 1);
			if(sem_id == SEM_FAILED) {
				perror("parent sem_open");
			}
			int sem_val;
			sem_getvalue(sem_id, &sem_val);
			if (sem_val==0)
				sem_post(sem_id);
			}
		else 
		{
			sem_id=sem_open("nlog_sem", O_CREAT);
			if(sem_id == SEM_FAILED) {
				perror("child sem_open");
			}
		}
		isSem=true;
	}
}

/** */
void CInitNlog::close_sem()
{
	if (cfg.IpcShare() && isSem)
	{
		sem_close(sem_id);
		if (isParent)
			sem_unlink("nlog_sem");
	}
}

/** */
void CInitNlog::wait_sem()
{
	if (cfg.IpcShare() && isSem)
	{
		if(sem_wait(sem_id) < 0) 
		{
			perror("sem_wait");
		}    
	}
}

/** */
void CInitNlog::post_sem()
{
	if (cfg.IpcShare() && isSem)
	{
		if(sem_post(sem_id) < 0) 
		{
			perror("sem_post");
		}
	}
}
#endif

/**
*/
CInitNlog NLOG::_init;

/**
*/
NLOG::NLOG(void)
{
}
/**
*/
NLOG::~NLOG(void)
{
}

/**
*/
void NLOG::Log(eLevel level, const char *fmt, ... )
{
	char textString[1024] = {'\0'};
    memset(textString, '\0', sizeof(textString));

	try
	{
		va_list args;
		va_start ( args, fmt );
#ifdef _WIN32
		vsnprintf_s ( textString, sizeof(textString), fmt, args );
#else
		vsnprintf ( textString, sizeof(textString), fmt, args );
#endif
	    va_end ( args );
	}
	catch(...)
	{
		return;
	}
#ifndef _WIN32
	NLOG::_init.wait_sem();
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
#if _PLOGGER
	switch (level)
	{
	case NLOG::Info:
#if _PLOGNAME
		PLOG_INFO << textString;
		//if (NLOG::_init.IsConEnabled())
		//	PLOG_INFO_(1) << textString;
#else
		LOG(plog::info) << textString;
#endif
		break;
	case NLOG::Error:
#if _PLOGNAME
		PLOG(plog::error) << textString;
#else
		LOG(plog::error) << textString;
#endif
		break;
	case NLOG::Debug:
#if _PLOGNAME
		PLOG(plog::debug) << textString;
#else
		LOG(plog::debug) << textString;
#endif
		break;
	default:
		break;
	}
#endif

#ifndef _WIN32
	NLOG::_init.post_sem();
#endif
}

/**
*/
void NLOG::LogIf(bool trueCondition, eLevel level, const char *fmt, ... )
{
	if (!trueCondition) return;

	char textString[1024] = {'\0'};
    memset(textString, '\0', sizeof(textString));

	try
	{
		va_list args;
		va_start ( args, fmt );
#ifdef _WIN32
		vsnprintf_s ( textString, sizeof(textString), fmt, args );
#else
		vsnprintf ( textString, sizeof(textString), fmt, args );
#endif
	    va_end ( args );
	}
	catch(...)
	{
		return;
	}
#ifndef _WIN32
	NLOG::_init.wait_sem();
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
#if _PLOGGER
	switch (level)
	{
	case NLOG::Info:
#if _PLOGNAME
		PLOG(plog::info) << textString;
#else
		LOG(plog::info) << textString;
#endif
		break;
	case NLOG::Error:
#if _PLOGNAME
		PLOG(plog::error) << textString;
#else
		LOG(plog::error) << textString;
#endif
		break;
	case NLOG::Debug:
#if _PLOGNAME
		PLOG(plog::debug) << textString;
#else
		LOG(plog::debug) << textString;
#endif
		break;
	default:
		break;
	}
#endif

#ifndef _WIN32
	NLOG::_init.post_sem();
#endif
}

