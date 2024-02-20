// Thread.cpp: implementation of the CThread class.
//
//////////////////////////////////////////////////////////////////////
#include "Thread.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//
CDead::CDead()
{
#ifdef _WIN32
	m_hDead = CreateEvent(0, false, 0, 0);
#else
		assert(pthread_cond_init(&m_hDead.CVariable, 0) == 0);
		assert(pthread_mutex_init(&m_hDead.Mutex, 0) == 0);
#endif
	m_bRunning = false;
};

//////////////////////////////////////////////////////////////////////
//
CDead::~CDead()
{
#ifdef _WIN32
	CloseHandle(m_hDead);
#else
	try 
	{
		pthread_cond_destroy(&m_hDead.CVariable);
	}
	catch(...)
	{
	}
	try 
	{
		pthread_mutex_destroy(&m_hDead.Mutex);
	}
	catch(...)
	{
	}
#endif
};

//////////////////////////////////////////////////////////////////////
//
void CDead::Start()
{
	m_bRunning = true;
};

//////////////////////////////////////////////////////////////////////
//
void CDead::Stop()
{
	if(m_bRunning)
	{
		m_bRunning = false;
#ifdef _WIN32
		WaitForSingleObject(m_hDead, 5000/*INFINITE*/);
#else
		struct timespec timeout;
		timeout.tv_sec = 5;
		timeout.tv_nsec = 0;
		pthread_cond_timedwait(&m_hDead.CVariable, &m_hDead.Mutex, &timeout);
#endif
	}
};

//////////////////////////////////////////////////////////////////////
//
void CDead::Signal()
{
#ifdef _WIN32
	Sleep(100);
	SetEvent(m_hDead);
#else
	assert(pthread_cond_signal(&m_hDead.CVariable) == 0);
#endif
};


//////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////
//
CThread::CThread()
{
	m_hThread = 0;
	m_bLive = false;
	m_id = 0;
}

//////////////////////////////////////////////////////////////////////
//
CThread::~CThread()
{
	m_hThread = 0;
}

//////////////////////////////////////////////////////////////////////
//
void CThread::Start(int nPriority)
{
#ifdef _WIN32
	DWORD dwThreadId;
	m_hThread = CreateThread(NULL, 0, sRun, (LPVOID)this, CREATE_SUSPENDED, &dwThreadId);
	if(m_hThread)
	{
		SetThreadPriority(m_hThread, nPriority);
		ResumeThread(m_hThread);
		Sleep(100);
	}
#else
	assert(pthread_create(&m_hThread, NULL, sRun, this) == 0);
#endif
}

//////////////////////////////////////////////////////////////////////
//
void CThread::Stop()
{
	m_Dead.Stop();
}

//////////////////////////////////////////////////////////////////////
//
#ifdef _WIN32
DWORD WINAPI CThread::sRun(LPVOID pParam)
#else
void *CThread::sRun(void *pParam)
#endif
{	
	((CThread *)pParam)->m_Dead.Start();
	((CThread *)pParam)->m_bLive = true;
	try
	{
		((CThread *)pParam)->Run();
	}
	catch(...)
	{
#ifdef _WIN32

#else
		assert(0);
#endif
	}

	((CThread *)pParam)->m_bLive = false;
	((CThread *)pParam)->m_Dead.Signal();

	return 0;
}

//////////////////////////////////////////////////////////////////////
//
bool CThread::IsLive()
{
	return m_bLive;
}


void CThread::sleep(int msec)
{
#ifdef _WIN32
	Sleep(msec);
#else
	usleep(msec*1000);
#endif

}
