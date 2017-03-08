#include "SocketRW.hpp"

namespace cg {
namespace net {
SocketRW::SocketRW(Socket & sock,
	Filter * writeFilter,
	Filter * readFilter)
	:m_socket(sock), m_readFilter(readFilter), m_writeFilter(writeFilter)
{

}
SocketRW::SocketRW(Socket & sock)
	: m_socket(sock), m_readFilter(nullptr), m_writeFilter(nullptr)
{

}
std::ptrdiff_t SocketRW::Write(const char * data,
	std::size_t size,
	std::ptrdiff_t timeout)
{
	if (!WriteReady(timeout))
		return 0;
	if (!m_writeFilter)
		return m_socket.Send(data, size, true);
	else 
	{
		auto tData = 
			m_writeFilter->Transform(data,size);

		return m_socket.Send(tData.data(), tData.size(), true);
	}
}

std::ptrdiff_t SocketRW::Read(char * dest,
	std::size_t size,
	std::ptrdiff_t timeout)
{

	if (!ReadReady(timeout))
		return 0;
	if (!m_readFilter)
		return m_socket.Recv(dest, size, true);
	else
	{
		auto tData =
			m_readFilter->Transform(dest, size);

		return m_socket.Recv(tData.data(), tData.size(), true);
	}
}

bool SocketRW::ReadReady(std::ptrdiff_t timeout) const
{
	return m_socket.ReadReady();
}

bool SocketRW::WriteReady(std::ptrdiff_t timeout) const
{
	return m_socket.WriteReady();
}


}
}
