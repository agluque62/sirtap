#ifndef _IAppender_
#define _IAppender_

namespace plog
{
    class IAppender
    {
    public:
        virtual ~IAppender()
        {
        }

        virtual void write(const Record& record) = 0;
    };
}
#endif
