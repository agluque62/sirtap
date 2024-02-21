#ifndef _MTIMER_H
#define _MTIMER_H

#include "Thread.h"

class Mtimer : public CThread
{
	bool running;
	int _timeout_sec;
	int count;
	void (*_timeout_cb)(void *);
	void *_param;

public:
	Mtimer(int timeout_sec, void *param, void (*timeout_cb)(void *));
	~Mtimer();
	void Run();
	void EndThread();
};

#endif




