#include "SocketRW.hpp"

namespace cg {
namespace net {

SocketRW::SocketRW(Socket* sock,
	Filter * writeFilter,
	Filter * readFilter)
	:m_socket(sock), m_readFilter(readFilter), m_writeFilter(writeFilter)
{
	EnableLogs(true, "SocketRW");
}
SocketRW::SocketRW(SocketRW && other)
	: m_socket(other.m_socket),
	m_writeFilter(other.m_writeFilter),
	m_readFilter(other.m_readFilter)
{
	other.m_readFilter = nullptr;
	other.m_writeFilter = nullptr;
}
void SocketRW::operator=(SocketRW && other)
{
	m_socket = other.m_socket;
	m_readFilter = other.m_readFilter;
	m_writeFilter = other.m_writeFilter;
	other.m_readFilter = nullptr;
	other.m_writeFilter = nullptr;
	other.m_socket = nullptr;
}
SocketRW::SocketRW(Socket * sock)
	: m_socket(sock),
	m_readFilter(nullptr),
	m_writeFilter(nullptr)
{
	EnableLogs(true, "SocketRW");
}
std::ptrdiff_t SocketRW::Write(const char * data,
	std::size_t size,
	std::ptrdiff_t timeout)
{
	CheckAndReport();
	if (!WriteReady(timeout))
		return 0;

	auto sLock = m_socket->ScopeLock();
	auto sSize = uint64_t(size);
	/*prepend the size of the data.*/
	m_socket->Send((char*)&sSize, sizeof(uint64_t), true);
	if (!m_writeFilter)
		return m_socket->Send(data, size, true);
	else
	{
		auto tData =
			m_writeFilter->Transform(data, size);

		return m_socket->Send(tData.data(), tData.size(), true);
	}
}

std::ptrdiff_t SocketRW::Read(char * dest,
	std::size_t sizep,
	std::ptrdiff_t timeout)
{
	CheckAndReport();
	if (!ReadReady(timeout))
		return 0;

	auto sLock = m_socket->ScopeLock(); 
	std::uint64_t size = 0;
	m_socket->Recv((char*)&size, sizeof(size), true);
	if (!m_readFilter)
		return m_socket->Recv(dest, size, true);
	else
	{
		auto ret = m_socket->Recv(dest, size, true);
		auto tData =
			m_readFilter->Transform(dest, size);
		std::memcpy(dest, tData.data(), size);
		return ret;
	}
}

bool SocketRW::ReadReady(std::ptrdiff_t timeout) const
{
	CheckAndReport();
	return m_socket->ReadReady(timeout);
}

bool SocketRW::WriteReady(std::ptrdiff_t timeout) const
{
	CheckAndReport();
	return m_socket->WriteReady(timeout);
}

void SocketRW::CheckAndReport() const
{
	if (!((const Socket*) m_socket))
	{
		LogError("There is not set socket. This object was moved most ",
			"likely.");
		throw NetworkException(Error::BadSocket);
	}
}


}
}
