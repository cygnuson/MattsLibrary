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
		/*LogNote(1, __FUNCSTR__, "The socket with id = ", Id(),
			" is now locked.");*/
		return;
	}
	/*else
		LogWarn(__FUNCSTR__, "The mutex is about to be accessed by a thread",
			" that probably cant lock the mutex (try lock fail). Socket id ",
			"= ", Id(), " .");*/

	/*LogNote(1, __FUNCSTR__, "Block to lock socket with id = ", Id(),
		" is now locked.");*/
	m_lock.Lock();
}

void Socket::Unlock() const
{
	m_lock.Unlock();
	/*LogNote(1, __FUNCSTR__, "The socket with id = ", Id(),
		" is now unlocked.");*/
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
	Unlock();
}

Socket::Address Socket::GetAddress() const
{
	sockaddr_storage addr;
	try
	{
		Lock();
		GetPeerName(addr);
		Unlock();
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
bool Socket::ReadReady(std::ptrdiff_t timeout) const
{
	timeval to = timeval();
	int code;
	fd_set set;
	FD_ZERO(&set);
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

bool Socket::WriteReady(std::ptrdiff_t timeout) const
{
	timeval to = timeval();
	int code;
	fd_set set;
	FD_ZERO(&set);
	FD_SET(m_socket, &set);
	if (timeout < 0)
		code = select(((int)m_socket) + 1,  0,&set, 0, nullptr);
	else /*timeout must be > 0 here. OK to cast*/
	{
		/*Get amount of seconds.*/
		to.tv_sec = ((long)timeout) / 1000000;
		/*Get the left over micro secs*/
		to.tv_usec = timeout % 1000000;
		code = select(((int)m_socket) + 1,  0,&set, 0, &to);
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
		m_lock.Unlock();
	else
		LogWarn(__FUNCSTR__, "The mutex is about to be accessed by a thread",
			" that probably cant lock the mutex (try lock fail). Socket id ",
			"= ", Id(), " .");
	/*will block if the socket is locked from a different thread.*/
	Lock();
	m_socket = other;
	Unlock();
}

Socket::operator OSSocket()const
{
	if (!m_lock.TryLock())
	{
		LogWarn(__FUNCSTR__, "The lock failed. Will block here. This is ",
			"almost certianly not intended.");
		/*will block if the socket is locked from a different thread.*/
		Lock();
	}
	Unlock();
	return m_socket;
}

void Socket::Shutdown(cg::net::Shutdown how)
{
	Lock();
	auto sd = shutdown(m_socket, (socklen_t)how);
	Unlock();
	if (sd == -1)
	{
		NetworkException e; //number auto generated
		LogError(__FUNCSTR__, "The socket could not shut down. Exception:",
			e.What());
		throw e;
	}
	LogNote(2, __FUNCSTR__, "Shutdown socket ", Id());
}

void Socket::Close()
{
	int error;
	Lock();
#ifdef _WIN32
	error = closesocket(m_socket);
#else
	error = close(m_socket);
#endif // _WIN32
	Unlock();
	if (error != 0)
	{
		NetworkException e; //number auto generated
		LogError(__FUNCSTR__, "The socket could not close. Exception:",
			e.What());
		throw e;
	}
	LogNote(2, __FUNCSTR__, "Closed socket ", Id(), ".");
	m_id = Socket::InvalidId;
}

void Socket::Bind(bool ip6, Port port)
{
	Create(ip6, true);
	auto res = MakeAddress("", port, true);
	auto b = bind(m_socket, res->ai_addr, (int)res->ai_addrlen);
	Unlock();
	if (b == -1)
	{
		NetworkException e; //number auto generated
		LogError(__FUNCSTR__, "The socket could not bind. Exception:",
			e.What());
		throw e;
	}
	freeaddrinfo(res);
	LogNote(2, __FUNCSTR__, "Bound socket ", Id(), " to port ", port, ".");
}

bool Socket::Connect(const std::string & address, Port port)
{
	Create(address.find_first_of(':') != std::string::npos, true);
	auto res = MakeAddress(address.c_str(), port);
	int ret = connect(m_socket, res->ai_addr, (int)res->ai_addrlen);
	Unlock();
	if (ret == -1)
	{
		freeaddrinfo(res);
		NetworkException e;
		LogWarn(__FUNCSTR__, "The socket was not connected: ",
			e.What());
		throw e;
	}
	freeaddrinfo(res);
	LogNote(2, __FUNCSTR__, "Connected Socket ", Id(), " to ", address,
		" on port ", port, ".");
	SetBlockMode(false);
	return true;
}

void Socket::Listen(socklen_t backlog)
{
	Lock();
	auto l = listen(m_socket, backlog);
	Unlock();
	if (l == -1)
	{
		NetworkException e; //number auto generated
		LogError(__FUNCSTR__, "The socket could not listen. Exception:",
			e.What());
		throw e;
	}
	LogNote(2, __FUNCSTR__, "Socket ", Id(), " is listening.");
}
bool Socket::Accept(Socket & sock, bool block) const
{
	if (block)
		ReadReady(-1);
	else
		if (!ReadReady())
		{
			return false;
		}

	sockaddr_storage clientAddy = { 0 };
	socklen_t len = sizeof(sockaddr_storage);
	sock.Lock();
	Lock();
	sock = accept(m_socket, (sockaddr*)&clientAddy, &len);
	sock.SetBlockMode(false);
	Unlock();
	sock.Unlock();
	if (sock.m_socket == Socket::InvalidSocket)
	{
		NetworkException e; //number auto generated
		LogError(__FUNCSTR__, "Accept failed. Exception:",
			e.What());
		throw e;
	}
	auto addr = GetAddress(clientAddy);
	LogNote(2, __FUNCSTR__, "Socket ", sock.Id(), " accepted.\n\tAddress=",
		addr.first, "\n\t   Port=", addr.second);
	return true;
}
int Socket::IsOpen() const
{
	if (m_id == InvalidId)
		return false;
	char byte = 0;
	Lock();
	socklen_t got = recv(m_socket, &byte, 1, MSG_PEEK);
	Unlock();
	if (got == 0)
	{
		return 0;
	}
	else if (got == -1)
	{
		auto code = NetworkException::GetErrno();
		if (code == Error::WouldBlock)
			return 1;
		else
			return -1;
	}
	else
	{
		return 1;
	}
}
addrinfo* Socket::MakeAddress(std::string && address,
	Port port,
	bool portOnly) const
{
	addrinfo hints = { 0 };
	addrinfo* resPtr;
	hints.ai_family = m_useIp6 ? AF_INET6 : AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	auto portStr = cg::ToString(port);
	int result;
	if (!portOnly)
	{
		result
			= getaddrinfo(address.c_str(), portStr.c_str(), &hints, &resPtr);
	}
	else
	{
		result
			= getaddrinfo(NULL, portStr.c_str(), &hints, &resPtr);
	}
	if (result != 0)
	{
		NetworkException e; //number auto generated
		LogError(__FUNCSTR__, "Get Address Info failed. Exception:",
			e.What());
		throw e;
	}
	LogNote(2, __FUNCSTR__, "Addrinfo data allocated, make sure to clean it",
		" up.");
	return resPtr;
}

std::ptrdiff_t Socket::Recv(char* data,
	std::size_t size,
	bool block,
	socklen_t flags) const
{
	Lock();
	/*If block, wait untill we know the recv has data.*/
	if (block)
		ReadReady(-1);
	if (size == 0)
	{
		LogError(__FUNCSTR__, "The size is zero, will recv nothing.");
	}

	socklen_t got =
		recv(m_socket, data, (socklen_t)size, flags);

	Unlock();
	if (got == -1)
	{
		auto code = NetworkException::GetErrno();
		if (code == Error::WouldBlock)
		{
			LogNote(3, __FUNCSTR__, "The socket is not ready (would block).");
			return 0;
		}
		NetworkException e; //auto numbering
		LogError(__FUNCSTR__, "Could not recv data. Exception:", e.What());
		throw e;
	}
	if (got == 0)
	{
		/*Normal close happened*/
		return -1;
	}
	return got;
}
std::ptrdiff_t Socket::Send(const char* data,
	std::size_t size,
	bool block,
	socklen_t flags) const
{
	Lock();
	/*If block, wait untill we know the send has data.*/
	if (block)
		WriteReady(-1);
	if (size == 0)
	{
		LogError(__FUNCSTR__, "The size is zero, will send nothing.");
	}

	socklen_t sent
		= send(m_socket, data, (socklen_t)size, flags);
	Unlock();
	if (sent == -1)
	{
		auto code = NetworkException::GetErrno();
		if (code == Error::WouldBlock)
		{
			LogNote(3, __FUNCSTR__, "The socket is not ready (would block).");
			return 0;
		}
		NetworkException e; //auto numbering
		LogError(__FUNCSTR__, "Could not send data. Exception:", e.What());
		throw e;
	}
	if (sent == 0)
		return -1;
	return sent;
}

void Socket::Create(bool useIp6, bool stayLocked)
{
	m_useIp6 = useIp6;
	Lock();
	m_socket = ::socket((useIp6 ? AF_INET6 : AF_INET), SOCK_STREAM, 0);
	if (m_socket == Socket::InvalidSocket)
	{
		NetworkException e; //number auto generated
		LogError(__FUNCSTR__, "The socket could not be created. Exception:",
			e.What());
		throw e;
	}
	LogNote(2, __FUNCSTR__, "Socket ", Id(),
		" registered with the os. Sockets may be created in a locked state.");
	if (!stayLocked)
		Unlock();
}

void Socket::SetBlockMode(bool block)
{
	Lock();
	LogNote(1, __FUNCSTR__, "Setting the block mode to block = ",
		(block ? "True" : "False"));
#ifdef _WIN32
	/*imode 0=block 1=no block*/
	u_long iMode = block ? 0 : 1;
	socklen_t code = ioctlsocket(m_socket, FIONBIO, &iMode);
	Unlock();
	if (code != 0)
	{
		NetworkException e; //auto numbering
		LogError(__FUNCSTR__, "Could not set block mode. Exception:",
			e.What());
		throw e;
	}
	
#else
	socklen_t opts = fcntl(m_socket, F_GETFL);
	Unlock();
	if (opts < 0)
	{
		NetworkException e; //auto numbering
		LogError(__FUNCSTR__, "Could not set block mode. Exception:",
			e.What());
		throw e;
	}

	if (opts & O_NONBLOCK)
	{
		/*blocking already*/
		if (block)
			return;
	}
	else
	{
		/*non blocking already*/
		if (!block)
			return;
}


	opts = block ? opts | O_NONBLOCK : opts & (~O_NONBLOCK);
	if (fcntl(socket, F_SETFL, opts) < 0)
	{
		NetworkException e; //auto numbering
		LogError(__FUNCSTR__, "Could not set block mode. Exception:", e.What());
		throw e;
	}
#endif
}




}
}






