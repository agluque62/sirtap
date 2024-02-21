#include "Mtimer.h"

Mtimer::Mtimer(int timeout_sec, void *param, void (*timeout_cb)(void * param))
{
	running = true;
	_timeout_sec = timeout_sec;
	count = timeout_sec;
	_timeout_cb = timeout_cb;
	_param = param;
}

Mtimer::~Mtimer()
{
	running = false;
	_timeout_cb = NULL;
}

void Mtimer::Run()
{
	while (running)
	{
		for (int i = 0; i < 5; i++)
		{
			if (!running) break;
			sleep(200);
			if (!running) break;
		}
		if (count > 0)
		{
			count--;
			if (count == 0 && running)
			{
				if (_timeout_cb != NULL)
				{
					(*_timeout_cb)(_param);
				}
				count = _timeout_sec;
			}
		}
	}	
}

void Mtimer::EndThread()
{
	_timeout_cb = NULL;
	running = false;	
	sleep(300);
}






