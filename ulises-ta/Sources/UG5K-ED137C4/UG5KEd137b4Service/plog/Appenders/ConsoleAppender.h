#ifndef _ConsoleAppender_
#define _ConsoleAppender_
#include <iostream>

#include "../Util.h"

namespace plog
{
    template<class Formatter>
    class ConsoleAppender : public IAppender
    {
    public:
        ConsoleAppender()
        {
#ifdef _WIN32
            ::setlocale(LC_ALL, "");
#endif
        }

        virtual void write(const Record& record)
        {
            util::MutexLock lock(m_mutex);
#ifdef _WIN32
            std::wcout << Formatter::format(record);
#else
            std::cout << Formatter::format(record);
#endif
        }
    private:
        util::Mutex     m_mutex;
    };
}
#endif
