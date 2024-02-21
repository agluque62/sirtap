#ifndef _Logger_h_
#define _Logger_h_

#include <vector>
#include <queue>

//#include <plog/Appenders/IAppender.h>
//#include <plog/Util.h>
#include "Appenders/IAppender.h"
#include "Util.h"

#define _PLOG_MT_

#ifdef _PLOG_MT_
 #include <pthread.h>
#endif // _PLOG_MT_


namespace plog
{
	template<int instance>
	class Logger : public util::Singleton<Logger<instance> >, public IAppender
	{
#ifdef _PLOG_MT_
	private:
		static void *loggingTh(void *arg)
		{
			Logger *pLog = (Logger *)arg;
			pLog->_thLog();
			return NULL;
		}
	protected:
		void _thLog()
		{
			while (bstarted)
			{
				if (!m_queue.empty())
				{
					util::MutexLock lock(m_mutex);
					
					Record data = m_queue.front();
					m_queue.pop();
					for (std::vector<IAppender*>::iterator it = m_appenders.begin(); it != m_appenders.end(); ++it)
					{
						(*it)->write(data);
					}
				}
				Sleep(10);
			}
		}


#endif // _PLOG_MT_

	public:
		/** */
		Logger(Severity maxSeverity = none) : m_maxSeverity(maxSeverity)
		{
#ifdef _PLOG_MT_
			bstarted=true;
			pthread_create(&m_hThread, NULL, Logger::loggingTh, this);
#endif // _PLOG_MT_
		}

#ifdef _PLOG_MT_
		~Logger()
		{
			bstarted = false;
			Sleep(20);
		}
#endif // _PLOG_MT_


		/** */
		Logger& addAppender(IAppender* appender)
		{
			assert(appender != this);
			m_appenders.push_back(appender);
			return *this;
		}

		/** */
		Severity getMaxSeverity() const
		{
			return m_maxSeverity;
		}

		/** */
		void setMaxSeverity(Severity severity)
		{
			m_maxSeverity = severity;
		}

		/** */
		bool checkSeverity(Severity severity) const
		{
			return severity <= m_maxSeverity;
		}

		/** */
		virtual void write(const Record& record)
		{
			if (checkSeverity(record.getSeverity()))
			{
				*this += record;
			}
		}

		/** */
		void operator+=(const Record& record)
		{
#ifdef _PLOG_MT_
			util::MutexLock lock(m_mutex);
			m_queue.push(record);
#else
			for (std::vector<IAppender*>::iterator it = m_appenders.begin(); it != m_appenders.end(); ++it)
			{
				(*it)->write(record);
			}
#endif // _PLOG_MT_
		}

	private:
		Severity m_maxSeverity;
		std::vector<IAppender*> m_appenders;
#ifdef _PLOG_MT_
		bool bstarted;
		pthread_t m_hThread;
		std::queue<Record >   m_queue;
		util::Mutex     m_mutex;
#endif // _PLOG_MT_

	};

	template<int instance>
	inline Logger<instance>* get()
	{
		return Logger<instance>::getInstance();
	}

	inline Logger<0>* get()
	{
		return Logger<0>::getInstance();
	}
}
#endif
