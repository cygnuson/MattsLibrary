#include "Socket.hpp"

namespace cg {
namespace net {

std::size_t Socket::ms_idCounter = 0;

Socket::Socket()
{
	m_id = ++ms_idCounter;
	EnableLogs(true, "Socket");
}

Socket::~Socket()
{
	if (m_id == InvalidId)
		LogWarn(__FUNCSTR__, "The socket will be closed forcfully.");
}

Socket::Socket(bool locked)
{
	m_id = ++ms_idCounter;
	EnableLogs(true, "Socket");
	if (locked)
		m_lock.Lock();
}

Socket::Socket(Socket && other)
{
	EnableLogs(true, "Socket");
	other.Lock();
	Lock();
	m_id = other.m_id;
	m_socket = other.m_socket;
	other.m_id = Socket::InvalidId;
	other.m_socket = Socket::InvalidSocket;
}

std::size_t Socket::Id() const
{
	return m_id;
}

bool Socket::operator==(const OSSocket & other)const
{
	return m_socket == other;
}

bool Socket::operator==(const Socket & other)const
{
	return (this == &other);
}

void Socket::Lock() const
{
	if (m_lock.TryLock())
	{
		LogNote(2, __FUNCSTR__, "The socket with id = ", Id(),
			" is now locked.");
		return;
	}
	else
		LogWarn(__FUNCSTR__, "The mutex is about to be accessed by a thread",
			" that probably cant lock the mutex (try lock fail). Socket id ",
			"= ", Id(), " .");

	LogNote(2, __FUNCSTR__, "Block to lock socket with id = ", Id(),
		" is now locked.");
	m_lock.Lock();
}

void Socket::UnLock() const
{
	m_lock.UnLock();
	LogNote(2, __FUNCSTR__, "The socket with id = ", Id(),
		" is now unlocked.");
}
void Socket::GetPeerName(sockaddr_storage & addr) const
{
	socklen_t addSize = sizeof(addr);
	Lock();
	if (getpeername(m_socket, (sockaddr*)&addr, &addSize) == -1)
	{
		NetworkException e; //auto numbering
		LogError(__FUNCSTR__, "Could not get peer name. Exception:", e.What());
		throw e;
	}
}

Socket::Address Socket::GetAddress()
{
	sockaddr_storage addr;
	try
	{
		Lock();
		GetPeerName(addr);
	}
	catch (const Exception& err)
	{
		LogError(__FUNCSTR__, "Exception: ", err.What());
		throw;
	}
	return GetAddress(addr);
}

Socket::Address Socket::GetAddress(sockaddr_storage info)
{
	Address returnMe;
	const char* ret = nullptr;
	if (info.ss_family == AF_INET)
	{
		/*ip4 version*/
		sockaddr_in *s = (sockaddr_in*)&info;
		returnMe.second = ntohs(s->sin_port);
		char ipstr[INET_ADDRSTRLEN];
		ret = inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof(ipstr));
		returnMe.first = ipstr;
	}
	else
	{
		/*ip6 version*/
		sockaddr_in6 *s = (sockaddr_in6*)&info;
		returnMe.second = ntohs(s->sin6_port);
		char ipstr[INET6_ADDRSTRLEN];
		ret = inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof(ipstr));
		returnMe.first = ipstr;
	}
	if (ret == nullptr)
	{
		NetworkException e; //auto numbering
		LogError(__FUNCSTR__, "Could not convert address. Exception:",
			e.What());
		throw e;
	}
	return returnMe;
}
bool Socket::Ready(std::ptrdiff_t timeout) const
{
	timeval to = timeval();
	int code;
	fd_set set;
	FD_ZERO(&set);
	Lock();
	FD_SET(m_socket, &set);
	if (timeout < 0)
		code = select(((int)m_socket) + 1, &set, 0, 0, nullptr);
	else /*timeout must be > 0 here. OK to cast*/
	{
		/*Get amount of seconds.*/
		to.tv_sec = ((long)timeout) / 1000000;
		/*Get the left over micro secs*/
		to.tv_usec = timeout % 1000000;
		code = select(((int)m_socket) + 1, &set, 0, 0, &to);
	}

	if (code == 0)
		return false;
	else if (code == -1)
	{
		NetworkException e; //auto numbering
		LogError(__FUNCSTR__, "Could not check ready status. Exception:",
			e.What());
		throw e;
	}
	else
		return true;
}

void Socket::operator=(const OSSocket & other)
{
	if (m_lock.TryLock())
		m_lock.UnLock();
	else
		LogWarn(__FUNCSTR__, "The mutex is about to be accessed by a thread",
			" that probably cant lock the mutex (try lock fail). Socket id ",
			"= ", Id(), " .");
	/*will block if the socket is locked from a different thread.*/
	Lock();
	m_socket = other;
}

bool Socket::Ready()
{
	return false;
}

std::size_t Socket::Write(const char * data, std::size_t size, std::size_t timeout)
{
	return std::size_t();
}

std::size_t Socket::Read(char * dest, std::size_t size, std::size_t timeout)
{
	return std::size_t();
}

Socket::operator OSSocket()const
{
	if (m_lock.TryLock())
		m_lock.UnLock();
	else
		LogWarn(__FUNCSTR__, "The lock failed. Will block here. This is ",
			"almost certianly not intended.");
	/*will block if the socket is locked from a different thread.*/
	Lock();
	return m_socket;
}
/******************************************************************************************/
ServerSocket::ServerSocket()
	:Socket()
{

}

ServerSocket::ServerSocket(bool locked)
	: Socket(locked)
{
}

ServerSocket::ServerSocket(ServerSocket && other)
	: Socket(std::forward<ServerSocket>(other))
{

}

ServerSocket::~ServerSocket()
{
	if (m_id == InvalidId)
		LogWarn(__FUNCSTR__, "The socket will be closed forcfully.");
}


}
}