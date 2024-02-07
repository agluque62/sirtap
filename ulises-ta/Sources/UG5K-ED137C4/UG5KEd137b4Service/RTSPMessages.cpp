#include "RTSPMessages.h"

#include "Utilidades.h"

#include <sstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <time.h>

/** 
*/
std::string RTSPMessages::MsgANNOUNCE(int seq, std::string uri/*, std::string agent*/, std::string sdpDescription)
{
	const char* format = "ANNOUNCE %s RTSP/1.0\r\n"
		"WG67-Version: recorder.02,recorder.01\r\n"
		"CSeq: %d \r\n"
		"Content-Type: application/sdp\r\n"
		"Content-Length: %d\r\n\r\n"
		"%s";									//ESC. 14.03.2016. DEBE Incluir \r\n, terminación de línea.
		//"\r\n";								//ESC. 14.03.2016. Terminación del mensaje

	return Utilidades::StrFormat(format, uri.c_str(), seq, sdpDescription.size(), sdpDescription.c_str());
}

/** 
*/
std::string RTSPMessages::MsgSETUP(int seq, std::string uri, int channel, char Ed137version)
{
	const char* format = "SETUP %s RTSP/1.0\r\n"
		"WG67-Version: %s\r\n"
		"CSeq: %d \r\n"
		"Transport: RTP/AVP/TCP;interleaved=%d\r\n"
		"\r\n";								//ESC. 14.03.2016. Terminación del mensaje

	std::string WG67VersionValue = Ed137version == 'C' ? "recorder.02" : "recorder.01";
	return Utilidades::StrFormat(format, uri.c_str(), WG67VersionValue.c_str(), seq, channel);
}

/** */
std::string RTSPMessages::MsgTEARDOWN(int seq, std::string uri, std::string sesion, char Ed137version)
{
	const char *format = "TEARDOWN %s RTSP/1.0\r\n"
				   "WG67-Version: %s\r\n"
				   "CSeq: %d \r\n"
				   "Session: %s\r\n"
				   "\r\n";								//ESC. 14.03.2016. Terminación del mensaje

	std::string WG67VersionValue = Ed137version == 'C' ? "recorder.02" : "recorder.01";
	return Utilidades::StrFormat(format, uri.c_str(), WG67VersionValue.c_str(), seq, sesion.c_str());
}

/** */
std::string RTSPMessages::MsgRECORD(int seq, std::string uri, std::string sesion, std::string crd, char Ed137version)
{
	const char *format = "RECORD %s RTSP/1.0\r\n"
				   "WG67-Version: %s\r\n"
				   "CSeq: %d \r\n"
				   "Session: %s\r\n"
				   "Content-Type: application/x-crd+xml\r\n"
				   "Content-Length: %d\r\n\r\n"
				   "%s"									//ESC. 14.03.2016. DEBE incluir \r\n, terminación de línea.
				   "\r\n";								//ESC. 14.03.2016. Terminación del mensaje

	std::string WG67VersionValue = Ed137version == 'C' ? "recorder.02" : "recorder.01";
	return Utilidades::StrFormat(format, uri.c_str(), WG67VersionValue.c_str(), seq, sesion.c_str(), crd.size(), crd.c_str());
}

/** */
std::string RTSPMessages::MsgPAUSE(int seq, std::string uri, std::string sesion, std::string crd, char Ed137version)
{
	const char *format = "PAUSE %s RTSP/1.0\r\n"
				   "WG67-Version: %s\r\n"
				   "CSeq: %d \r\n"
				   "Session: %s\r\n"
				   "Content-Type: application/x-crd+xml\r\n"
				   "Content-Length: %d\r\n\r\n"
				   "%s"									//ESC. 14.03.2016. DEBE Incluir \r\n, terminación de línea.
				   "\r\n";								//ESC. 14.03.2016. Terminación del mensaje

	std::string WG67VersionValue = Ed137version == 'C' ? "recorder.02" : "recorder.01";
	return Utilidades::StrFormat(format, uri.c_str(), WG67VersionValue.c_str(), seq, sesion.c_str(), crd.size(), crd.c_str());
}

/** */
std::string RTSPMessages::MsgPARAMS(int seq, std::string uri, std::string sesion, std::string crd, char Ed137version)
{
	const char *format = "SET_PARAMETER %s RTSP/1.0\r\n"
				   "WG67-Version: %s\r\n"
				   "CSeq: %d \r\n"
				   "Session: %s\r\n"
				   "Content-Type: application/x-crd+xml\r\n"
				   "Content-Length: %d\r\n\r\n"
				   "%s"									//ESC. 14.03.2016. DEBE Incluir \r\n, terminación de línea.
				   "\r\n";								//ESC. 14.03.2016. Terminación del mensaje

	std::string WG67VersionValue = Ed137version == 'C' ? "recorder.02" : "recorder.01";
	return Utilidades::StrFormat(format, uri.c_str(), WG67VersionValue.c_str(), seq, sesion.c_str(), crd.size(), crd.c_str());
}

/** */
std::string RTSPMessages::MsgGetPARAMS(int seq, std::string uri, std::string sesion, char Ed137version)
{
	const char* format = "GET_PARAMETER %s RTSP/1.0\r\n"
		"WG67-Version: %s\r\n"
		"CSeq: %d \r\n"
		"Session: %s\r\n\r\n";
		//"Content-Type: application/x-crd+xml\r\n"
		//"Content-Length: 0\r\n\r\n";

	std::string WG67VersionValue = Ed137version == 'C' ? "recorder.02" : "recorder.01";
	return Utilidades::StrFormat(format, uri.c_str(), WG67VersionValue.c_str(), seq, sesion.c_str());
}



/** */
std::string RTSPMessages::CRD(std::string connref, rtsp_props &props, rtsp_props &opes)
{
	std::string crd;
	//crd.append("<?xml version='1.0' encoding='UTF-8'?>\r\n");
	crd.append(Utilidades::StrFormat("<call-record-data connref=\"%s\">\r\n", connref.c_str()));

	if (props.size() > 0)
	{
		crd.append("<properties>\r\n");

		rtsp_props::iterator prop;
		for (prop = props.begin(); prop != props.end(); prop++)
		{
			crd.append(Utilidades::StrFormat("<property name=\"%s\">%s</property>\r\n", prop->first.c_str(), prop->second.c_str()));
		}

		crd.append("</properties>\r\n");
	}


	if (opes.size() > 0)
	{
		crd.append("<operations>\r\n");

		rtsp_props::iterator ope;
		for (ope = opes.begin(); ope != opes.end(); ope++)
		{
			crd.append(Utilidades::StrFormat("<operation name=\"%s\" time=\"%s\">%s</operation>\r\n", 
				ope->first.c_str(), UTCTimestamp().c_str(), ope->second.c_str()));
		}

		crd.append("</operations>\r\n");
	}

	crd.append("</call-record-data>\r\n");
	return crd;
}

/** */
bool RTSPMessages::RspParse(std::string &rsp, std::string &res, std::list<std::pair<std::string, std::string> > &headers)
{	
	std::istringstream f(rsp.c_str());
	std::string line;    
    
	if (std::getline(f,line))					// La primera Linea es la RESPUESTA
	{
		std::vector<std::string> tokens;
		Utilidades::split(tokens, line, ' ');

		if (tokens.size() < 3)					// Primera Linea FORMATO RTSP/1.0 200 OK...
			return false;

		res = tokens.at(1);

		while (std::getline(f, line))			// Las siguiente son cabeceras con el formato HEADER: valor
		{
			if (line.size() > 0)
			{
				std::vector<std::string> tokens;
				Utilidades::split(tokens, line, ':');
				if (tokens.size() >= 2)
				{
					headers.push_back(std::pair<std::string,std::string>(tokens.at(0),tokens.at(1)));
				}
			}
		}
	}
	
	return true;
}

/** Quita blancos iniciales y \r o \n finales */
std::string RTSPMessages::NormalizeHeaderValue(std::string header)
{
   std::string result="";

   int len = header.length();
   for (int i=0; i<len; i++) 
   {
	   if (i==0 && header[i]==' ')
		   continue;
	   if (i==(len-1) && (header[i]=='\r' || header[i]=='\n') )
		   continue;

	   result += header[i];      
   }
   return result;
}

/** */
//static unsigned long cnt_connref=0;
std::string RTSPMessages::GenerateConnref(std::string id, std::string ip)
{
	return Utilidades::StrFormat("%s@%s", id.c_str(), ip.c_str());
}

/** */
std::string RTSPMessages::UTCTimestamp()
{
    time_t seconds_since_the_epoch;
    struct tm tm_struct;

    std::ostringstream buf;

    // Return the time as seconds elapsed since midnight, January 1, 1970,
    // or -1 in the case of an error.
    time(&seconds_since_the_epoch);

    if (seconds_since_the_epoch == -1) {
        return "";
    }

    // Convert the time in seconds to the time structure.
#ifdef _WIN32
    errno_t err;
    err = gmtime_s(&tm_struct, &seconds_since_the_epoch);
    if (err) {
        return "";
    }
#else
    gmtime_r(&seconds_since_the_epoch, &tm_struct);
#endif


    // Format the structure YYYY-MM-DD_HH:MM:SS.XXX+0000 
	// todo. obtener los mili-segundos..
    buf
        <<        std::setw(4) << std::setfill('0') << tm_struct.tm_year + 1900
        << "-" << std::setw(2) << std::setfill('0') << tm_struct.tm_mon + 1
        << "-" << std::setw(2) << std::setfill('0') << tm_struct.tm_mday
        << "_" << std::setw(2) << std::setfill('0') << tm_struct.tm_hour
        << ":" << std::setw(2) << std::setfill('0') << tm_struct.tm_min
        << ":" << std::setw(2) << std::setfill('0') << tm_struct.tm_sec
        << ".000+0000";

    return buf.str();
}

bool RTSPMessages::StringsInsensEqual(const std::string& a, const std::string& b)
{
    unsigned int sz = a.size();
    if (b.size() != sz)
        return false;
    
	for (unsigned int i = 0; i < sz; ++i)
        if (tolower(a[i]) != tolower(b[i]))
            return false;
    
	return true;
}

