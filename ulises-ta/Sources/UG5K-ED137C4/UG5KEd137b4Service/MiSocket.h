#if !defined(MISOCKET_H__INCLUDED_)
#define MISOCKET_H__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif 

#ifdef _WIN32
 #include <qos2.h>
 #include <winsock2.h>
 #pragma comment(lib, "ws2_32.lib")
#else
	#include <sys/types.h>     /*  Solaris 2.5.1 fix: u_short, required by sys/socket.h */
	#include <sys/socket.h>    /*  sockets */
	#include <sys/select.h>
	#include <sys/time.h>      /*  timeval */
	#include <sys/ioctl.h>     /*  ioctl  */
	#include <string.h>        /*  bzero, for FD_SET */
	#include <strings.h>       /*  bzero, for FD_ZERO (AIX) */
	#include <netinet/in.h>    /*  INADDR_*, in_addr, sockaddr_in, htonl etc. */
	#include <netdb.h>         /*  getservbyname */
	#include <arpa/inet.h>     /*  inet_addr */
	#include <errno.h>         /*  socket error codes */
	#include <netinet/tcp.h>   /*  TCP_KEEPCNT,  */

	#define SD_RECEIVE			SHUT_RD
	#define SD_SEND				SHUT_WR
	#define SD_BOTH				SHUT_RDWR

	#define INVALID_SOCKET		-1
	#define SOCKET_ERROR		-1
	#define WSAENOTSOCK			1
	#define WSAECONNRESET		2
	#define WSAETIMEDOUT		3
#endif
#include "win_error.h"


/** */
class CIPAddress;
class CUDPSocket;

/** */
class CInitSocket
{
private:
    static long s_count;
    static CInitSocket s_is;

public:
	static char ipLocal[32];

public:
    CInitSocket();
    CInitSocket(const CInitSocket &is);
    ~CInitSocket();
};

//////////////////////////////
class socket_error : public win_error
{
protected:
    virtual const char *Prefix() const
    {
#ifdef _WIN32
        return "WSAE";
#else
        return "LSE";
#endif
    }

public:
    socket_error(const char *s="")
#ifdef _WIN32
        : win_error(WSAGetLastError(), s)
#else
		: win_error(errno, s)	// NOTA. Aquí no hay manera de ver que el error haya sido de sockets. Habría que conocer los sockets abiertos
								// y llamar a getsockopt(socket, SOL_SOCKET, SO_ERROR, ...) para ver si hay error en algún socket y
								// luego obtener el errno. todo. obtener error.
#endif
    {
    }

    socket_error(int lastError, const char *s="")
        :win_error(lastError, s)
    {
    }

    bool WouldBlock() const
    {
#ifdef _WIN32
        return GetCode()==WSAEWOULDBLOCK;
#else
		return false;		// NOTA: El equivalente se ha de hacer con getsockopt(socket, SOL_SOCKET, SO_ERROR, ...) para cada socket
							// todo. obtener este valor en linux
#endif
    }
};

/** */
inline void TestSocket(int res, const char *txt)
{
#ifdef _WIN32
    if (res==SOCKET_ERROR)
        throw socket_error(txt);
#else
	if (res < 0)
	{
		throw socket_error(errno, txt);
	}
#endif
}

//////////////////////////////
class CIPAddress
{
public:
    static const char BROADCAST[], ANY[], LOOPBACK[], NONE[];

    CIPAddress(const sockaddr_in *sockaddr);
    CIPAddress(const char *name=NONE, int port=0);
    CIPAddress(const std::string &name, int port=0);
    //CIPAddress(const char *name, const char *serv, const char *proto=NULL);
    //CIPAddress(unsigned long dwIp, int port=0);

    operator const sockaddr*() const;
    int size() const
    { return sizeof(m_addr); }
    sockaddr_in& Get_m_addr()
    {
        return m_addr;
    }

    //bool IsAssigned() const;
    const std::string &GetHostName() const;
    //const std::string &GetRealHostName() const;
    //const in_addr &GetAddress() const;
    const std::string GetStringAddress() const;
    unsigned short GetPort() const;
	//bool operator == (CIPAddress &in);	// {return TRUE;}

private:
    CInitSocket m_is;

    void Init(const char *name, int portNBO);
    sockaddr_in m_addr;

    mutable bool m_realHostName;  // mutable por GetRealHostName
    mutable std::string m_hostname; // mutable por GetRealHostName

};

template<class E, class T>
    std::basic_ostream<E, T>& operator<<(std::basic_ostream<E, T> &os, const CIPAddress &addr)
{
    return os << addr.GetHostName() << E(':') << addr.GetPort();
}

/////////////////////////////////////////////////////////////////////////////
// class CSocket

class CSocket
{
public:
    void NewSocket();
#ifdef _WIN32
    int SetDSCPQoS(unsigned long DSCPValue, CIPAddress& dest_add);
#endif
    void Bind(int port);
    void Bind(const CIPAddress &addr);
	virtual ~CSocket();

    bool Connect(const CIPAddress &addr);

    void SendTo(const void *buf, int sz, const CIPAddress &addr);
    int RecvFrom(void *buf, int sz, CIPAddress *addr);	// si addr es NULL no devuelve la direcci�n

    int Send(const void *buf, int sz);
    int Recv(void *buf, int sz);
	int TCPcnxAlive();									//ESC. 26.09.2018. Método añadido


    //const CIPAddress &GetLocalAddress() const;
    //const CIPAddress GetRemoteAddress() const;
    void Close();
    void Shutdown(int how=SD_SEND);  // SD_SEND SD_RECEIVE SD_BOTH

    void SetSockOpt(int level, int optname, const void *optval, int optlen);
    int GetSockOpt(int level, int optname, void *optval, int *optlen);	//ESC. 25.09.2018, cambia tipo a 'int'
    void GetSockOpt(int level, int optname, void *optval, int optlen);	// sin puntero en optlen

    // Funciones de 'select'. timeout en milisegundos: si es 0 -> no bloqueante, si es -1 -> espera infinita
    int Select(fd_set *fr, fd_set *fw, fd_set *fe, int timeout=0) const;
    bool IsReadable(int timeout=0) const; 
    bool IsWritable(int timeout=0) const;
    bool IsException(int timeout=0) const;

    int GetHandle()
    { return m_socket; }

	/*
    bool IsValid() const
    { 
		return m_socket!=INVALID_SOCKET; 
	}
	*/
	CIPAddress GetHostAddress(int nPort = 0);

protected:
    CSocket();
    virtual void Init() =0;
	int m_socket;

#ifdef _WIN32
    HANDLE QoSHandle;
#else

#endif

private:
    CInitSocket m_is;
    mutable CIPAddress m_localAddr, m_remoteAddr;  // mutable por GetLocalName, GetRemoteName
    CSocket(const CSocket &sck); // los sockets no se copian
    CSocket &operator=(const CSocket &sck); // los sockets no se asignan
};

/*template<class E, class T>
    std::basic_ostream<E, T>& operator<<(std::basic_ostream<E, T> &os, const CSocket &sck)
{
    os << sck.GetLocalAddress() << E('-') << E('>') << sck.GetRemoteAddress();
    return os;
}
*/

/////////////////////////////////////////////////////////////////////////////
// class CUDPSocket

class CUDPSocket : public CSocket
{
public:
	explicit CUDPSocket(bool broadcast=false);      // no hace bind
	explicit CUDPSocket(int localport, bool broadcast=false);  // localport=0 es un puerto cualquiera
	explicit CUDPSocket(const CIPAddress &local, bool broadcast=false);
public:
	void SetBroadcast(void );

protected:
    virtual void Init();
private:
    bool m_broadcast;
};

/////////////////////////////////////////////////////////////////////////////
// class CTCPSocket
class CTCPSocket : public CSocket
{
public:
	CTCPSocket();
	explicit CTCPSocket(const CIPAddress &remote);
	CTCPSocket(const CIPAddress &local, const CIPAddress &remote);

    void Write(const void *buf, int sz);  // escribe exactamente sz bytes (lo mismo que Send)
    void Read(void *buf, int sz);         // lee exactamente sz bytes: si hay menos, espera; si se cierra la conexi�n lanza el error WSAECONNRESET)
protected:
    virtual void Init();
private:
    friend class CTCPServerSocket;
	CTCPSocket(int sck);  // para los sockets del accept
};

/*
/////////////////////////////////////////////////////////////////////////////
// class CTCPServerSocket
class CTCPServerSocket : public CTCPSocket
{
public:
    CTCPServerSocket();
	explicit CTCPServerSocket(int localport);
	explicit CTCPServerSocket(const CIPAddress &local);

    void Listen(int backlog=SOMAXCONN);
    CTCPSocket *Accept(int backlog=SOMAXCONN);
    void Accept(CTCPSocket &sck, int backlog=SOMAXCONN);
protected:
    virtual void Init();
private:
    int m_listenBacklog;

    int AcceptPriv(int backlog);
};
*/


// }  //namespace sck

#endif // !defined(AFX_MISOCKET_H__7062F444_6603_487E_993C_747B875FBDE0__INCLUDED_)
