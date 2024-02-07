#ifndef _NLOG_
#define _NLOG_

/** Define el LOGGER */
#define _EASYLOGGER			0
#define _EASYLOGGER_CV11	0
#define _PLOGGER			1

#include <string>
#include <iostream>

#include <pthread.h>
#include <semaphore.h>

#ifndef _WIN32
    #include <sys/types.h>
    #include <fcntl.h>
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
	static CInitNlog _init;

public:
	static void Log(eLevel level, const char *fmt, ... );
	static void LogIf(bool trueCondition, eLevel level, const char *fmt, ... );
};

#endif

