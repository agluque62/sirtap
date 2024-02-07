#ifndef _Log4viewFormatter_
#define _Log4viewFormatter_

#include <iomanip>
#include "../Util.h"

namespace plog
{
	/** Formato a Generar por Cada mensaje para verlo en Log4View...
		<log4j:event logger="TestNLog.Program" level="INFO" timestamp="1448022393457" thread="8">
			<log4j:message>Esto es una prueba...</log4j:message>
			<log4j:properties>
				<log4j:data name="log4japp" value="TestNLog.vshost.exe(5688)" />
				<log4j:data name="log4jmachinename" value="DF1501" />
			</log4j:properties>
		</log4j:event>
	*/
    class Log4viewFormatter
    {
    public:
        static util::nstring header()
        {
            return util::nstring();
        }

        static util::nstring format(const Record& record)
        {
            //tm t;
            //util::localtime_s(&t, &record.getTime().time);
            util::nstringstream ss;

			ss << "<log4j:event logger=\"" << record.ipFrom.c_str() << "\" level=\"" << getSeverityName(record.getSeverity());
			ss << "\" timestamp=\"" << std::setfill(PLOG_NSTR('0')) << std::setw(10) << record.getTime().time << std::setfill(PLOG_NSTR('0')) << std::setw(3) << record.getTime().millitm;
			ss << "\" thread=\"" << record.getTid() << "\">";
			ss << "<log4j:message>" << record.getMessage().c_str() << "</log4j:message>";
			ss << "<log4j:properties>";
			ss << "<log4j:data name=\"log4japp\" value=\"" << "PLOG" << "\" />";
			ss << "<log4j:data name=\"log4jmachinename\" value=\"" << "PLOG-M" << "\" />";
			ss << "</log4j:properties>";
			ss << "</log4j:event>";

            return ss.str();
        }
    };
}
#endif
