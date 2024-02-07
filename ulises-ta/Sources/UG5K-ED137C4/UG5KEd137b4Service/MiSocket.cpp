// MiSocket.cpp : implementation file
//
//#include "stdafx.h"
#include "MiSocket.h"

#include <string>
#include <sstream>
#include <memory>
#include "NLOG.h"


#ifndef _WIN32
# include <unistd.h>
//#include <sys/epoll.h>
//#include <errno.h>
#include <poll.h>
#endif

long CInitSocket::s_count = -1;
CInitSocket CInitSocket::s_is; // para que la dll est� cargada todo el tiempo que dure la funci�n WinMain
char CInitSocket::ipLocal[32];

////////////////////////////////////////////////////////////////////////////////
// CInitSocket
CInitSocket::CInitSocket()
{
#ifdef _WIN32
	if (InterlockedIncrement(&s_count)==0)
	{
		WSADATA wsaData;
		int res = WSAStartup(MAKEWORD(2,0), &wsaData);
		// AGL
		char name[255];
		PHOSTENT hostinfo;
		if( gethostname ( name, sizeof(name)) == 0)
		{
			if((hostinfo = gethostbyname(name)) != NULL)
			{
				strncpy_s(ipLocal,inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list),sizeof(ipLocal));
			}
		}
		_ASSERT(res==0);
	}
#else
	// todo. Inicializacioin del sistema de sockets..
/*
	char name[255];
	PHOSTENT hostinfo;
	if( gethostname ( name, sizeof(name)) == 0)
	{
		if((hostinfo = gethostbyname(name)) != NULL)
		{
			strncpy(ipLocal,inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list),sizeof(ipLocal));
		}
	}
	*/
#endif
}

////////////////////////////////////////////////////////////////////////////////
//
CInitSocket::CInitSocket(const CInitSocket &is)
{
#ifdef _WIN32
	InterlockedIncrement(&s_count); // no puede ser el primero, porque ya existe is
#else
	// todo
#endif
}

////////////////////////////////////////////////////////////////////////////////
//
CInitSocket::~CInitSocket()
{
#ifdef _WIN32
	if (InterlockedDecrement(&s_count)<0)
	{                      
		int res = WSACleanup();
		_ASSERT(res==0);
	}
#else
	// todo
#endif
}

////////////////////////////////////////////////////////////////////////////////
// CIPAddress
////////////////////////////////////////////////////////////////////////////////
//
const char CIPAddress::BROADCAST[]  = {"<broadcast>"};
const char CIPAddress::ANY[]        = {"<any>"};
const char CIPAddress::LOOPBACK[]   = {"<localhost>"};
const char CIPAddress::NONE[]       = {"<none>"};

////////////////////////////////////////////////////////////////////////////////
CIPAddress::CIPAddress(const sockaddr_in *sockaddr)
{
#ifdef _WIN32
	_ASSERT(sockaddr->sin_family==AF_INET);
#else
	// todo
#endif

	m_addr = *sockaddr;
	m_realHostName = false;

		// broadcast y none tienen la misma direcci�n!!!
#ifdef _WIN32
	if (m_addr.sin_addr.S_un.S_addr==INADDR_BROADCAST)
		m_hostname = BROADCAST;
	else if (m_addr.sin_addr.S_un.S_addr==INADDR_ANY)
		m_hostname = ANY;
	else if (m_addr.sin_addr.S_un.S_addr==INADDR_LOOPBACK)
		m_hostname = LOOPBACK;
	else if (m_addr.sin_addr.S_un.S_addr==INADDR_NONE)
		m_hostname = NONE;
	else
		m_hostname = GetStringAddress();
#else
	if (m_addr.sin_addr.s_addr==INADDR_BROADCAST)
		m_hostname = BROADCAST;
	else if (m_addr.sin_addr.s_addr==INADDR_ANY)
		m_hostname = ANY;
	else if (m_addr.sin_addr.s_addr==INADDR_LOOPBACK)
		m_hostname = LOOPBACK;
	else if (m_addr.sin_addr.s_addr==INADDR_NONE)
		m_hostname = NONE;
	else
		m_hostname = GetStringAddress();

#endif
}


////////////////////////////////////////////////////////////////////////////////
//
CIPAddress::CIPAddress(const char *name/*=NONE*/, int port/*=0*/)
{
	Init(name, htons(port));
}

////////////////////////////////////////////////////////////////////////////////
//
CIPAddress::CIPAddress(const std::string &name, int port/*=0*/)
{
	Init(name.c_str(), htons(port));
}

////////////////////////////////////////////////////////////////////////////////
/*
CIPAddress::CIPAddress(const char *name, const char *serv, const char *proto)
{
	servent *se = getservbyname(serv, proto);
	if (!se)
	   throw socket_error("getservbyname");

	Init(name, se->s_port);
}
*/
////////////////////////////////////////////////////////////////////////////////
/*
CIPAddress::CIPAddress(unsigned long dwIp, int port)
{
	memset(&m_addr, 0, sizeof(m_addr));

	m_addr.sin_family = AF_INET;
	m_addr.sin_port   = htons(port);
#ifdef _WIN32
	m_addr.sin_addr.S_un.S_addr = htonl(dwIp);
#else
	m_addr.sin_addr.s_addr = htonl(dwIp);
#endif

	m_realHostName = false;
	m_hostname = GetStringAddress();
}
*/
////////////////////////////////////////////////////////////////////////////////
//
void CIPAddress::Init(const char *name, int portNBO)
{
	memset(&m_addr, 0, sizeof(m_addr));
	m_addr.sin_family = AF_INET;

	if (portNBO)
		m_addr.sin_port = portNBO;

	if (!name)
		name = NONE;

	m_hostname = name;
	m_realHostName = false; //todav�a no se sabe el nombre real

#ifdef _WIN32
	if (m_hostname==BROADCAST)
		m_addr.sin_addr.S_un.S_addr = INADDR_BROADCAST;
	else if (m_hostname==ANY)
		m_addr.sin_addr.S_un.S_addr = INADDR_ANY;
	else if (m_hostname==LOOPBACK)
		m_addr.sin_addr.S_un.S_addr = INADDR_LOOPBACK;
	else if (m_hostname==NONE)
		m_addr.sin_addr.S_un.S_addr = INADDR_NONE;
	else
	{
		m_addr.sin_addr.S_un.S_addr = inet_addr(name);
		if (m_addr.sin_addr.S_un.S_addr!=INADDR_NONE)
			;  // es una direccion a.b.c.d, no se conoce el nombre, y no se busca hasta que se pida
		else
		{    // es un nombre, as� que hay que resolver la direcci�n
			hostent *he = gethostbyname(name);
			if (!he)
				throw socket_error("gethostbyname");
			memcpy(&m_addr.sin_addr, he->h_addr_list[0], he->h_length);
			m_hostname = he->h_name;
			m_realHostName = true;
		}
	}
#else
	if (m_hostname==BROADCAST)
		m_addr.sin_addr.s_addr = INADDR_BROADCAST;
	else if (m_hostname==ANY)
		m_addr.sin_addr.s_addr = INADDR_ANY;
	else if (m_hostname==LOOPBACK)
		m_addr.sin_addr.s_addr = INADDR_LOOPBACK;
	else if (m_hostname==NONE)
		m_addr.sin_addr.s_addr = INADDR_NONE;
	else
	{
		m_addr.sin_addr.s_addr = inet_addr(name);
		if (m_addr.sin_addr.s_addr!=INADDR_NONE)
			;  // es una direccion a.b.c.d, no se conoce el nombre, y no se busca hasta que se pida
		else
		{    // es un nombre, as� que hay que resolver la direcci�n
			hostent *he = gethostbyname(name);
			if (!he)
				throw socket_error("gethostbyname");
			memcpy(&m_addr.sin_addr, he->h_addr_list[0], he->h_length);
			m_hostname = he->h_name;
			m_realHostName = true;
		}
	}
#endif
}

////////////////////////////////////////////////////////////////////////////////
//
CIPAddress::operator const sockaddr *() const
{
	return reinterpret_cast<const sockaddr*>(&m_addr);
}

////////////////////////////////////////////////////////////////////////////////
/*
bool CIPAddress::operator == (CIPAddress &o)
{
	if (GetPort() != o.GetPort())
		return false;

	return m_hostname == o.GetHostName();
}
*/
////////////////////////////////////////////////////////////////////////////////
/*
const std::string &CIPAddress::GetRealHostName() const
{
	if (!m_realHostName)
	{
		hostent *he = gethostbyaddr(reinterpret_cast<const char*>(&m_addr.sin_addr), sizeof(m_addr.sin_addr), AF_INET);
		if (!he)
			throw socket_error("gethostbyaddr");
		m_hostname = he->h_name;
		m_realHostName = true;
	}

	return m_hostname;
};
*/

////////////////////////////////////////////////////////////////////////////////
/*
bool CIPAddress::IsAssigned() const
{
#ifdef _WIN32
	return m_addr.sin_addr.S_un.S_addr!=INADDR_NONE;
#else
	return m_addr.sin_addr.s_addr!=INADDR_NONE;
#endif
}
*/
////////////////////////////////////////////////////////////////////////////////
//
const std::string &CIPAddress::GetHostName() const
{
	return m_hostname;
}

////////////////////////////////////////////////////////////////////////////////
/*
const in_addr &CIPAddress::GetAddress() const
{
	return m_addr.sin_addr;
}
*/
////////////////////////////////////////////////////////////////////////////////
//
const std::string CIPAddress::GetStringAddress() const
{
	std::ostringstream os;
	unsigned char *byte = (unsigned char *)(&m_addr.sin_addr);
#ifdef _WIN32
	os << (int)(unsigned char)m_addr.sin_addr.S_un.S_un_b.s_b1 << '.' << (int)(unsigned char)m_addr.sin_addr.S_un.S_un_b.s_b2 << '.'
	   << (int)(unsigned char)m_addr.sin_addr.S_un.S_un_b.s_b3 << '.' << (int)(unsigned char)m_addr.sin_addr.S_un.S_un_b.s_b4;
#else
	os << (int)byte[0] << '.' << (int)byte[1] << '.' << (int)byte[2] << '.' << (int)byte[3];

#endif
	return os.str();
}

////////////////////////////////////////////////////////////////////////////////
//
unsigned short CIPAddress::GetPort() const
{
	return ntohs(m_addr.sin_port);
}

/////////////////////////////////////////////////////////////////////////////
// CSocket
////////////////////////////////////////////////////////////////////////////////
//
CSocket::CSocket()
	: m_socket(-1)
{
#ifdef _WIN32
	QoSHandle = INVALID_HANDLE_VALUE;
#else

#endif
}

////////////////////////////////////////////////////////////////////////////////
//
void CSocket::NewSocket()
{
	Close();
	Init();
}

////////////////////////////////////////////////////////////////////////////////
//
#ifdef _WIN32
int CSocket::SetDSCPQoS(unsigned long DSCPValue, CIPAddress &dest_add)
{
	int ret = 0;

	if (m_socket < 0) return -1;

	if (QoSHandle != INVALID_HANDLE_VALUE)
	{
		QOSCloseHandle(QoSHandle);
		QoSHandle = INVALID_HANDLE_VALUE;
	}

	BOOL QoSResult;
	QOS_VERSION Version;

	// Initialize the QoS version parameter.
	Version.MajorVersion = 1;
	Version.MinorVersion = 0;

	// Get a handle to the QoS subsystem.
	QoSResult = QOSCreateHandle(&Version, &QoSHandle);
	if (QoSResult != TRUE)
	{
		QoSHandle = INVALID_HANDLE_VALUE;
		ret = -1;
	}
	else
	{
		QOS_FLOWID QoSFlowId = 0;
		QoSResult = QOSAddSocketToFlow(QoSHandle, m_socket, (PSOCKADDR) &dest_add.Get_m_addr(), QOSTrafficTypeVoice, QOS_NON_ADAPTIVE_FLOW, &QoSFlowId);
		if (QoSResult != TRUE)
		{
			ret = -1;
		}
		else
		{
			DWORD DSCPVal = (DWORD) DSCPValue;
			QoSResult = QOSSetFlow(QoSHandle, QoSFlowId, QOSSetOutgoingDSCPValue, sizeof(DSCPVal), &DSCPVal, 0, NULL);
			if (QoSResult != TRUE)
			{
				ret = -1;
			}
		}
	}

	return ret;
}
#endif

////////////////////////////////////////////////////////////////////////////////
//
void CSocket::Bind(int port)
{
	// ANY
	CIPAddress addr(CIPAddress::ANY, port);
	Bind(addr);
}

////////////////////////////////////////////////////////////////////////////////
//
void CSocket::Bind(const CIPAddress &addr)
{
	int res = bind(m_socket, addr, addr.size());
	TestSocket(res, "bind");
}

////////////////////////////////////////////////////////////////////////////////
//
CSocket::~CSocket()
{
	if (m_socket != INVALID_SOCKET)
	{
#ifdef _WIN32
		TestSocket(closesocket(m_socket), "close");
#else
		TestSocket(close(m_socket), "close");
#endif
	}
}

////////////////////////////////////////////////////////////////////////////////
//
void CSocket::SendTo(const void *buf, int sz, const CIPAddress &addr)
{
	if (m_socket==INVALID_SOCKET)
		throw socket_error(WSAENOTSOCK, "SendTo");
	int num = sendto(m_socket, static_cast<const char*>(buf), sz, 0, addr, addr.size());
	TestSocket(num, "sendto");
}

////////////////////////////////////////////////////////////////////////////////
int CSocket::RecvFrom(void *buf, int sz, CIPAddress *addr)
{
	if (m_socket==INVALID_SOCKET)
		throw socket_error(WSAENOTSOCK, "RecvFrom");

	sockaddr_in fromaddr;
	int len = sizeof(fromaddr);
	int num;
	char *pBuf = static_cast<char*>(buf);
	// int err = 0;
#ifdef _WIN32
	num = recvfrom(m_socket, pBuf, sz, 0, reinterpret_cast<sockaddr*>(&fromaddr), &len);
#else
	num = recvfrom(m_socket, pBuf, sz, 0, reinterpret_cast<sockaddr*>(&fromaddr), (socklen_t *)&len);
#endif
	// TestSocket(num, "recvfrom");		//ESC. Comentado; al configurar time-out da un error al finalizar la temporización
	
	if (addr)
		*addr = CIPAddress(&fromaddr);

	return num;
}

////////////////////////////////////////////////////////////////////////////////
//
bool CSocket::Connect(const CIPAddress &addr)
{
	if (m_socket==INVALID_SOCKET)
		NLOG_INFO("Connect m_socket < 0");
	if (m_socket==INVALID_SOCKET)
		throw socket_error(WSAENOTSOCK, "Connect");

	int res = connect(m_socket, addr, addr.size());
	if (res!=0)
		NLOG_INFO("connect errno %i", errno);
	else
	{		
	}
	
	// TestSocket(res, "connect");
	return res==0 ? true : false;
}

////////////////////////////////////////////////////////////////////////////////
//
int CSocket::Send(const void *buf, int sz)
{
	if (m_socket==INVALID_SOCKET)
		throw socket_error(WSAENOTSOCK, "Send");

	int num = send(m_socket, static_cast<const char*>(buf), sz, 0);
	// TestSocket(num, "send");
	return num;
}

////////////////////////////////////////////////////////////////////////////////
//
int CSocket::Recv(void *buf, int sz)
{ // Ojo: puede dar excepciones en UDP de vez en cuando, WSAECONNRESET
	if (m_socket==INVALID_SOCKET)
		throw socket_error(WSAENOTSOCK, "Recv");

	int num;
	char *pBuf = static_cast<char*>(buf);
	    
	num = recv(m_socket, pBuf, sz, 0);

/*
ESC. 16.03.2016. Comentado. No veo la conveniencia de generar una excepción cuando no se han recibido datos

	if (num < 0 )
	{
#ifdef _WIN32
		if ( WSAGetLastError() == WSAETIMEDOUT )
#else
		if ( errno == EAGAIN ||  errno == EWOULDBLOCK )
#endif		
		{
			throw socket_error(WSAETIMEDOUT, "Recv");
		}
	}
*/
	return num;
}

//ESC. 26.09.2018. Método añadido. Pretende verificar el estado de un socket TCP previamente conectado
int CSocket::TCPcnxAlive()
{ // Ojo: puede dar excepciones en UDP de vez en cuando, WSAECONNRESET
	if (m_socket==INVALID_SOCKET)
		throw socket_error(WSAENOTSOCK, "Recv");

	char data;
	int iRx;
	bool bRet= false;

#ifdef _WIN32
	u_long uModeNonBlock= 1;
	u_long uModeBlock= 0;


	ioctlsocket(m_socket, FIONBIO, &uModeNonBlock);		//Pasa Sck a modo Non-Block

	iRx = recv(m_socket, &data, 1, MSG_PEEK);

	if(iRx > 0)
		bRet= true;
	else if(iRx == 0)
		bRet= false;
	else   //iRx < 0
	{
		int iError= WSAGetLastError();		//WSAGetLastError no es aplicable en "receipt of an asynchronous message"
		if((iError != WSAEWOULDBLOCK)&& (iError != WSAETIMEDOUT))
			bRet= false;
		else
			bRet= true;
	}


	ioctlsocket(m_socket, FIONBIO, &uModeBlock);	//Pasa Sck a modo Block

	return bRet;

#else
	//26.09.2018. Eduardo, tienes que probar/verificar TODAS estas SENTENCIAS en la pasarela

	iRx= recv(m_socket, &data, 1, 0);

	if(iRx > 0)
	{
		while (iRx > 0)
		{
			iRx= recv(m_socket, &data, 1, 0);
		}
		bRet= true;
	}
	else if(iRx == 0)
		bRet= true;
	else   //iRx < 0
	{
		if((errno != EAGAIN)&& (errno != EWOULDBLOCK))
		{
			NLOG_INFO("TCPcnxAliverecv devuelve errno %i", errno);
			bRet= false;
		}
		else
			bRet= true;
	}

	return bRet;

#endif
}

////////////////////////////////////////////////////////////////////////////////
/*
const CIPAddress &CSocket::GetLocalAddress() const
{
	if (!m_localAddr.IsAssigned())
	{
		sockaddr_in name;
		int len = sizeof(name);
#ifdef _WIN32
		if (getsockname(m_socket, reinterpret_cast<sockaddr*>(&name), &len)!=SOCKET_ERROR)
			m_localAddr = CIPAddress(&name);
#else
		if (getsockname(m_socket, reinterpret_cast<sockaddr*>(&name), (socklen_t *)&len)!=SOCKET_ERROR)
			m_localAddr = CIPAddress(&name);
#endif

	}
	return m_localAddr;
}
*/

////////////////////////////////////////////////////////////////////////////////
/*
const CIPAddress CSocket::GetRemoteAddress() const
{
	if (!m_remoteAddr.IsAssigned())
	{
		sockaddr_in name;
		int len = sizeof(name);
#ifdef _WIN32
		if (getpeername(m_socket, reinterpret_cast<sockaddr*>(&name), &len)!=SOCKET_ERROR)
			m_remoteAddr = CIPAddress(&name);
#else
		if (getpeername(m_socket, reinterpret_cast<sockaddr*>(&name), (socklen_t *)&len)!=SOCKET_ERROR)
			m_remoteAddr = CIPAddress(&name);
#endif
	}
	return m_remoteAddr;
}
*/
////////////////////////////////////////////////////////////////////////////////
//
void CSocket::Close()
{
#ifdef _WIN32
	if (QoSHandle != INVALID_HANDLE_VALUE)
	{
		QOSCloseHandle(QoSHandle);
		QoSHandle = INVALID_HANDLE_VALUE;
	}
#endif // _WIN32

	if (m_socket==INVALID_SOCKET)
		return;

#ifdef _WIN32
	TestSocket(closesocket(m_socket),"close");
#else
	TestSocket(close(m_socket),"close");
#endif

	m_localAddr = m_remoteAddr = CIPAddress();
	m_socket = INVALID_SOCKET;
}

////////////////////////////////////////////////////////////////////////////////
//
void CSocket::Shutdown(int how/*=SD_SEND*/)
{
	if (m_socket==INVALID_SOCKET)
		return;
	TestSocket(shutdown(m_socket, how),"shutdown");
}

////////////////////////////////////////////////////////////////////////////////
//
void CSocket::SetSockOpt(int level, int optname, const void *optval, int optlen)
{
	if (m_socket==INVALID_SOCKET)
		throw socket_error(WSAENOTSOCK, "SetSockOpt");

	int res = setsockopt(m_socket, level, optname, static_cast<const char*>(optval), optlen);
	if (res < 0)
		NLOG_ERROR("SetSockOpt res %i level %i optname %i", res, level, optname);
	TestSocket(res, "setsockopt");
}

////////////////////////////////////////////////////////////////////////////////
//
int CSocket::GetSockOpt(int level, int optname, void *optval, int *optlen)
{
	if (m_socket==INVALID_SOCKET)
		throw socket_error(WSAENOTSOCK, "GetSockOpt");
#ifdef _WIN32
	int res = getsockopt(m_socket, level, optname, static_cast<char*>(optval), optlen);
#else
	int res = getsockopt(m_socket, level, optname, static_cast<char*>(optval), (socklen_t *)optlen);
#endif

	return res;		//ESC. 25.09.2018. Añadido, antes retornaba 'void'

	TestSocket(res, "getsockopt");
}

////////////////////////////////////////////////////////////////////////////////
//
int CSocket::Select(fd_set *fr, fd_set *fw, fd_set *fe, int timeout/*=0*/) const
{
	if (m_socket==INVALID_SOCKET)
		throw socket_error(WSAENOTSOCK, "Select");

	timeval *pt=NULL, t;
	if (timeout>=0)
	{
		t.tv_sec  = timeout / 1000;
		t.tv_usec = (timeout % 1000) * 1000;
		pt = &t;
	}

#ifdef _WIN32
	int res = select(0, fr, fw, fe, pt);
#else
	//int res = (select(1+1, fr, fw, fe, pt) > 0)? 1: 0;	//NO es CORRECTO; file descriptor (1+1) no coindide con el del socket.
	int res = select(1+1, fr, fw, fe, pt);

		//NLOG_INFO("Select res %i",res);
/*
//ESC. 16.03.2016. Intento de implementación correcta de [IsWritable|IsReadable]+Select en Linux
	int res= 0;

	if(select(m_socket +1, fr, fw, fe, pt) < 0)		//ESC. 10.03.2016. OJO!!!, Sólo se permite comprobar un "file descriptor"
		return res;

	if(fr != NULL)
	{
		printf("fr not NULL\n");					//BORRAR
		if(FD_ISSET(m_socket, fr))
		{
			//FD_CLR(m_socket, fr);
			printf("IsReadable\n");					//BORRAR
			res= 1;
		}
	}
	else if(fw != NULL)
	{
		printf("fw not NULL\n");					//BORRAR
		if(FD_ISSET(m_socket, fw))
		{
			//FD_CLR(m_socket, fw);
			printf("IsWritable\n");					//BORRAR
			res= 1;
		}
	}
	else if(fe != NULL)
	{
		printf("fe not NULL\n");					//BORRAR
		if(FD_ISSET(m_socket, fe))
		{
			//FD_CLR(m_socket, fe);
			printf("IsException\n");				//BORRAR
			res= 1;
		}
	}
*/
#endif

//	TestSocket(res, "select");							//ESC. 22.02.2016. Comentado.
	return res;
}

////////////////////////////////////////////////////////////////////////////////
//
bool CSocket::IsReadable(int timeout/*=0*/) const
{
//	printf("Funcion IsReadable\n");					//BORRAR

	fd_set fdr;
	FD_ZERO(&fdr);
	FD_SET(m_socket, &fdr);

	return Select(&fdr, NULL, NULL, timeout)!=0;	//ESC. 20.07.2017: Debería retorna 'true' cuando ">0"
}

////////////////////////////////////////////////////////////////////////////////
//
bool CSocket::IsWritable(int timeout/*=0*/) const
{
//	printf("Funcion IsWritable\n");					//BORRAR

//#ifdef _WIN32
	fd_set fdw;
	FD_ZERO(&fdw);
	FD_SET(m_socket, &fdw);

	return Select(NULL, &fdw, NULL, timeout)!=0;	//ESC. 20.07.2017: Debería retorna 'true' cuando ">0"
													//ESC. 22.02.2016: "fd_set" como segundo parámetro.
//#else

/*
ESC. 19.07.2016. Alternativas "IsWritable" para LINUX... TAMPOCO funcionan correctamente

// LINUX_2.6. "epoll()"

	struct epoll_event newPeerConnectionEvent;
	int epollFD = -1;
	struct epoll_event processableEvents;
	int numEvents = -1;

	if ((epollFD = epoll_create(1)) == -1)
	{
		printf("Could not create the epoll FD list. Aborting!\n");
		return false;
	}

	newPeerConnectionEvent.data.fd = m_socket;
	newPeerConnectionEvent.events = EPOLLOUT | EPOLLIN | EPOLLERR;

	if (epoll_ctl(epollFD, EPOLL_CTL_ADD, m_socket, &newPeerConnectionEvent) == -1)
	{
		printf("Could not add the socket FD to the epoll FD list. Aborting!\n");
		return false;
	}

	numEvents = epoll_wait(epollFD, &processableEvents, 1, timeout);

	if (numEvents < 0)
	{
		printf("Serious error in epoll setup: epoll_wait () returned < 0 status!");
		return false;
	}

	int retVal=0;
	socklen_t retValLen= sizeof(retVal);
    if ((getsockopt(m_socket, SOL_SOCKET, SO_ERROR, &retVal, &retValLen) < 0)||
		(retVal != 0))
	{
		printf("getsockopt ERROR\n");
		return false;
	}

	return true;
*/

/*
//	LINUX_2.4 "poll()"

	struct pollfd fds[1];
	int retval, nfds=1;

	memset(fds, 0 , sizeof(fds));

	fds[0].fd = m_socket;
	fds[0].events = POLLOUT;

	retval = poll(fds, nfds, timeout);
	if (retval < 0)
	{
		printf("poll() failed.\n");
		return false;
	}
	if(retval == 0)
	{
		printf("poll() timeout.\n");
		return false;
	}

	if(fds[0].revents & POLLOUT)
		return true;
	else
		return false;
*/
//#endif
}

////////////////////////////////////////////////////////////////////////////////
//
bool CSocket::IsException(int timeout/*=0*/) const
{
	fd_set fs;
	FD_ZERO(&fs);
	FD_SET(m_socket, &fs);

	return Select(NULL, NULL, &fs, timeout)!=0;
}

////////////////////////////////////////////////////////////////////////////////
//
void CSocket::GetSockOpt(int level, int optname, void *optval, int optlen) // sin puntero en optlen
{
	GetSockOpt(level, optname, optval, &optlen);
}

////////////////////////////////////////////////////////////////////////////////
//
CIPAddress CSocket::GetHostAddress(int nPort)
{
	CIPAddress ip(m_is.ipLocal, nPort);
	return ip;
}

/////////////////////////////////////////////
// class CUDPSocket
////////////////////////////////////////////////////////////////////////////////
//
CUDPSocket::CUDPSocket(bool broadcast/*=false*/)
		: m_broadcast(broadcast)
{
	Init();
}

////////////////////////////////////////////////////////////////////////////////
//
CUDPSocket::CUDPSocket(int localport, bool broadcast/*=false*/)
		: m_broadcast(broadcast)
{
	Init();

	Bind(localport);
}

////////////////////////////////////////////////////////////////////////////////
//
CUDPSocket::CUDPSocket(const CIPAddress &local, bool broadcast/*=false*/)
		: m_broadcast(broadcast)
{
	Init();

	Bind(local);
}

////////////////////////////////////////////////////////////////////////////////
//
void CUDPSocket::Init()
{
	m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (m_socket==INVALID_SOCKET)
		throw socket_error("socket");

	if (m_broadcast)
	{
		bool bBC = true;
		SetSockOpt(SOL_SOCKET, SO_BROADCAST, &bBC, sizeof(bBC));
	}
}

////////////////////////////////////////////////////////////////////////////////
//
void CUDPSocket::SetBroadcast(void )
{
	bool bBC = true;
	m_broadcast=true;
	SetSockOpt(SOL_SOCKET, SO_BROADCAST, &bBC, sizeof(bBC));
}

////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
// class CTCPSocket

CTCPSocket::CTCPSocket()
{
	Init();
}

////////////////////////////////////////////////////////////////////////////////
//
CTCPSocket::CTCPSocket(const CIPAddress &remote)
{
	Init();
	Connect(remote);
}

////////////////////////////////////////////////////////////////////////////////
//
CTCPSocket::CTCPSocket(const CIPAddress &local, const CIPAddress &remote)
{
	Init();
	Bind(local);
	Connect(remote);
}

////////////////////////////////////////////////////////////////////////////////
//
CTCPSocket::CTCPSocket(int sck)
{
	m_socket = sck;
}

////////////////////////////////////////////////////////////////////////////////
//
void CTCPSocket::Init()
{
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_socket==INVALID_SOCKET)
		throw socket_error("socket");
}

////////////////////////////////////////////////////////////////////////////////
//
void CTCPSocket::Write(const void *buf, int sz)
{
	Send(buf, sz);
}

////////////////////////////////////////////////////////////////////////////////
//
void CTCPSocket::Read(void *buf, int sz)
{
	char *cbuf = reinterpret_cast<char*>(buf);

	while (sz>0)
	{
		int r = Recv(cbuf, sz);
		if (r==0)
			throw socket_error(WSAECONNRESET, "Read");
		cbuf += r;
		sz  -= r;
	}
}

/*
////////////////////////////////////////////////////////////////////////////////
//
/////////////////////////////////////////////////////////////////////////////
// class CTCPServerSocket
CTCPServerSocket::CTCPServerSocket()
		: m_listenBacklog(0)
{
}

////////////////////////////////////////////////////////////////////////////////
//
CTCPServerSocket::CTCPServerSocket(int localport)
		: m_listenBacklog(0)
{
	Bind(localport);
}

////////////////////////////////////////////////////////////////////////////////
//
CTCPServerSocket::CTCPServerSocket(const CIPAddress &local)
		: m_listenBacklog(0)
{
	Bind(local);
}

////////////////////////////////////////////////////////////////////////////////
//
void CTCPServerSocket::Init()
{
	m_listenBacklog = 0;
	CTCPSocket::Init();
}

////////////////////////////////////////////////////////////////////////////////
//
void CTCPServerSocket::Listen(int backlog)
{
	if (m_listenBacklog!=backlog)
	{
		m_listenBacklog = backlog;
		int res = listen(m_socket, m_listenBacklog);
		TestSocket(res, "listen");
	}
}

////////////////////////////////////////////////////////////////////////////////
//
CTCPSocket *CTCPServerSocket::Accept(int backlog)
{
	int socket = AcceptPriv(backlog);

	CTCPSocket * pSck = new CTCPSocket(socket);

	return pSck;
}

////////////////////////////////////////////////////////////////////////////////
//
void CTCPServerSocket::Accept(CTCPSocket &sck, int backlog)
{
	sck.Close();
	sck.m_socket = AcceptPriv(backlog);
}

////////////////////////////////////////////////////////////////////////////////
//
int CTCPServerSocket::AcceptPriv(int backlog)
{
	Listen(backlog);

	int socket = accept(m_socket, NULL, NULL);
	if (socket==INVALID_SOCKET)
		throw socket_error("accept");

	return socket;
}
*/

// }  //namespace sck
