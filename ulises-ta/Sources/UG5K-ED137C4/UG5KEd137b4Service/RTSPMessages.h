#if !defined(RTSPMESSAGES_H__INCLUDED_)
#define RTSPMESSAGES_H__INCLUDED_

// #pragma once

#include <string>
#include <list>

typedef std::pair< std::string, std::string> rtsp_prop;
typedef std::list< rtsp_prop > rtsp_props;

class RTSPMessages
{
public:
	static std::string MsgANNOUNCE(int seq, std::string uri /*, std::string agent*/, std::string sdpDescription);
	static std::string MsgSETUP(int seq, std::string uri, int channel, char Ed137version);
	static std::string MsgTEARDOWN(int seq, std::string uri, std::string sesion, char Ed137version);
	static std::string MsgRECORD(int seq, std::string uri, std::string sesion, std::string crd, char Ed137version);
	static std::string MsgPAUSE(int seq, std::string uri, std::string sesion, std::string crd, char Ed137version);
	static std::string MsgPARAMS(int seq, std::string uri, std::string sesion, std::string crd, char Ed137version);
	static std::string MsgGetPARAMS(int seq, std::string uri, std::string sesion, char Ed137version);

public:
	static std::string CRD(std::string connref, rtsp_props &prop, rtsp_props &ope);

public:
	static bool RspParse(std::string &rsp, std::string &res, std::list<std::pair<std::string, std::string> > &headers);

public:
	static std::string NormalizeHeaderValue(std::string header);
	static std::string UTCTimestamp();
	static std::string GenerateConnref(std::string id, std::string ip);
	static bool RTSPMessages::StringsInsensEqual(const std::string& a, const std::string& b);	//ESC. 18.01.2017. Añadido
};

#endif
