#ifndef _NLOG_
#define _NLOG_

#ifndef _WIN32

#include "include/base/code-base.h"

#else

/** Define el LOGGER */
#define _EASYLOGGER			0
#define _EASYLOGGER_CV11	0
#define _PLOGGER			1

/** Arranque Statico o desde main */
//#define _NLOG_MAIN_

#include <sys/stat.h>
#include <string>
#include <iostream>

#include <pthread.h>
#include <semaphore.h>

#ifndef _WIN32
    #include <sys/types.h>
    #include <fcntl.h>
	#define Sleep(m)	usleep(1000*m)
#endif

using namespace std;

#define NLOG_INFO(format, ...)	NLOG::Log(NLOG::Info, format, ##__VA_ARGS__)
#define NLOG_ERROR(format, ...)	NLOG::Log(NLOG::Error, format, ##__VA_ARGS__)
#define NLOG_DEBUG(format, ...)	NLOG::Log(NLOG::Debug, format, ##__VA_ARGS__)

/**
*/
class CInitNlog
{
public:
	CInitNlog();
	~CInitNlog();
#ifndef _WIN32
public:
	void init_sem(bool parent=false);
	void close_sem();
	void wait_sem();
	void post_sem();
protected:
	bool isParent;
	bool isSem;
public:
	 sem_t *sem_id;
#endif
#ifdef _PLOGGER
public:
	bool IsConEnabled(){return true;}
#endif
};

/**
*/
class NLOG
{
public:
	enum eLevel {Info, Error, Debug};

public:
	NLOG(void);
	~NLOG(void);

public:
#ifdef _NLOG_MAIN_
	CInitNlog	*pInitLog;
#else
	static CInitNlog _init;
#endif

private:
	static void _Log(eLevel level, const char *fmt, va_list args);

public:
	static void Log(eLevel level, const char *fmt, ... );
	static void LogIf(bool trueCondition, eLevel level, const char *fmt, ... );
};

#endif // !_WIN32

#endif

