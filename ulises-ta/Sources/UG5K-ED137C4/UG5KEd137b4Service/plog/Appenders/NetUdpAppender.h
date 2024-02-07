#ifndef _NetUdpAppender_
#define _NetUdpAppender_

#ifdef _WIN32
#	include <winsock.h>
#else
#	include <sys/socket.h>
#       include <arpa/inet.h>
#       include <errno.h>
#endif

#include <algorithm>
#include "../Util.h"
#include "../Converters/UTF8Converter.h"

namespace plog
{
	template<class Formatter, class Converter = UTF8Converter>
	class NetUdpAppender : public IAppender
	{
	public:
		NetUdpAppender(const char* ipTo, int udpPort, const char* ipFrom) 
		{
			m_socket=-1;
			m_firstWrite=true;

			m_addr.sin_family = AF_INET;
			m_addr.sin_port = htons(udpPort);
			m_addr.sin_addr.s_addr = inet_addr(ipTo);

			m_ipFrom =std::string(ipFrom);
		}

		virtual void write(const Record& record)
		{
			util::MutexLock lock(m_mutex);

			if (m_firstWrite)
			{
				m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
				if (m_socket == -1)
				{
#ifdef _WIN32
					wprintf(L"socket failed with error: %d\n", WSAGetLastError());
#else
					wprintf(L"socket failed with error: %d\n", errno);
#endif
				}
				m_firstWrite = false;
			}

			if (m_socket != -1)
			{
				((Record *)&record)->ipFrom = m_ipFrom;

				string data = Converter::convert(Formatter::format(record));
				// string data = Formatter::format(record);
#ifdef _WIN32
				int iResult = sendto(m_socket, data.c_str(), data.length(), 0, (SOCKADDR *) & m_addr, sizeof (m_addr));
				if (iResult == SOCKET_ERROR) 
				{
					wprintf(L"sendto failed with error: %d\n", WSAGetLastError());
				}    			
#else
				int iResult = sendto(m_socket, data.c_str(), data.length(), 0, (const sockaddr *)&m_addr, sizeof (m_addr));
				if (iResult == -1) 
				{
					wprintf(L"sendto failed with error: %d\n", errno);
				}    			
#endif
			}

		}

	private:
		util::Mutex			m_mutex;
		bool				m_firstWrite;
		int					m_socket;
#ifdef _WIN32
		sockaddr_in			m_addr;
#else
		struct sockaddr_in  m_addr;
#endif
		std::string			m_ipFrom;
	};
}
#endif
