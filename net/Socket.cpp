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
		LogError(__FUNCSTR__, "Exception:", e.What());
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