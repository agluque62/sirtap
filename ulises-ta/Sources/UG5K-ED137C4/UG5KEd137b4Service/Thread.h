// Thread.h: interface for the CThread class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_THREAD_H__E04790E1_2ADA_11D3_84D1_00201856631F__INCLUDED_)
#define AFX_THREAD_H__E04790E1_2ADA_11D3_84D1_00201856631F__INCLUDED_

#ifdef _WIN32
	#include <Windows.h>
#else
	#include <stdlib.h>
	#include <pthread.h>
	#include <stdint.h>
	#include <assert.h>
	#include <unistd.h>

	#define Sleep(m)	usleep(1000*m)		//Arturo. 04.12.2015. Añadido
	#define _NO_WORKING_THREAD_				//Arturo. 04.12.2015. Añadido

	#define THREAD_PRIORITY_NORMAL 1
#endif


/**
*/
class CCritSec  
{
private:
#ifdef _WIN32
    CRITICAL_SECTION m_critSec;
#else
	pthread_mutex_t m_critSec;
#endif

public:
	CCritSec()
    {
#ifdef _WIN32
	    InitializeCriticalSection(&m_critSec);
#else
	    assert(pthread_mutex_init(&m_critSec, 0) == 0);
#endif
    }

	~CCritSec()
    {
#ifdef _WIN32
		DeleteCriticalSection(&m_critSec);
#else
		assert(pthread_mutex_destroy(&m_critSec) == 0);
#endif
    }

#ifdef _WIN32
    operator LPCRITICAL_SECTION()
#else
	operator pthread_mutex_t *()
#endif
    {
		return &m_critSec;
    }

private:

};

/**
*/
class CCSLock
{
public:
    CCSLock(CCritSec &cs) : m_cs(cs)
    {
#ifdef _WIN32
		EnterCriticalSection(m_cs);
#else
		pthread_mutex_lock((pthread_mutex_t *)m_cs);
#endif
    }

    ~CCSLock()
    {
#ifdef _WIN32
		LeaveCriticalSection(m_cs);
#else
		pthread_mutex_unlock((pthread_mutex_t *)m_cs);
#endif
    }

private:
    CCritSec& m_cs;
};

/**
*/
class CDead
{
protected:
	bool m_bRunning;

#ifdef _WIN32
	HANDLE m_hDead;
#else
	struct _event_t_
	{
		pthread_cond_t CVariable;
		pthread_mutex_t Mutex;
	} m_hDead;
#endif

public:
	CDead();
	virtual ~CDead();

	void Start();
	void Stop();
	void Signal();
	bool IsRunning(){ return m_bRunning;};
};

/**
*/
class CThread  
{
private:
	long m_id;

public:
	CThread();
	virtual ~CThread();

	virtual void Start(int nPriority = THREAD_PRIORITY_NORMAL);
	virtual void Stop();
	bool IsLive();
public:
	void SetId(long id) { m_id=id; }
	long GetId(void)  { return m_id; }

protected:
	virtual void Run() = 0;
public:
	bool IsRunning()
	{
		return m_Dead.IsRunning();
	}
protected:
	CDead m_Dead;
	bool m_bLive;

/*	void _sleep(int msec)
	{
#ifdef _WIN32
		Sleep(msec);
#else
#endif
	}
*/
#ifdef _WIN32
	HANDLE m_hThread;
	static DWORD WINAPI sRun(LPVOID pParam);
#else
	pthread_t m_hThread;
	static void *sRun(void *arg);
#endif

public:
	static void sleep(int msec);

};


#endif // !defined(AFX_THREAD_H__E04790E1_2ADA_11D3_84D1_00201856631F__INCLUDED_)
