#include "SocketRW.hpp"

namespace cg {
namespace net {

SocketRW::SocketRW(const Socket* sock,
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
SocketRW::SocketRW(const Socket * sock)
	: m_socket(sock),
	m_readFilter(nullptr),
	m_writeFilter(nullptr)
{
	EnableLogs(true, "SocketRW");
}
std::ptrdiff_t SocketRW::Write(const char * data,
	int64_t size,
	std::ptrdiff_t timeout)
{
	CheckAndReport();
	if (!WriteReady(timeout))
		return 0;

	auto sLock = m_socket->ScopeLock();
	if (!m_writeFilter)
	{
		auto sSize = uint64_t(size);
		/*prepend the size of the data.*/
		m_socket->Send((char*)&sSize, sizeof(uint64_t), true);
		if (!m_writeFilter)
			return m_socket->Send(data, size, true);
		else
		{
			auto tData =
				m_writeFilter->TransformCopy(data, size);

			return m_socket->Send(tData.data(), tData.size(), true);
		}
	}
	else
	{
		auto tData =
			m_writeFilter->TransformCopy(data, size);
		int64_t sSize = tData.size();
		/*prepend the size of the data.*/
		m_socket->Send((char*)&sSize, sizeof(uint64_t), true);
		return m_socket->Send(tData.data(), tData.size(), true);
	}
}
cg::ArrayView SocketRW::Read(int64_t expectedSize,
	std::ptrdiff_t timeout)
{
	CheckAndReport();
	if (!ReadReady(timeout))
		return ArrayView();

	auto sLock = m_socket->ScopeLock();
	std::int64_t size = 0;
	auto got = m_socket->Recv((char*)&size, sizeof(size), true);
	if (got == -1)
	{
		/*socket closed normally.*/
		LogNote(3, __FUNCSTR__, "Socket closed normally.");
		throw NetworkException(Error::NotConnected);
	}
	cg::ArrayView av(size);
	m_socket->Recv(av.data(), size, true);
	if (m_readFilter)
	{
		if (m_readFilter->SizeChanges())
			av = m_readFilter->TransformCopy(av.data(), size);
		else
			m_readFilter->Transform(av.data(), size);

	}
	return av;
}

void SocketRW::SetReaderFilter(cg::Filter * newFilter)
{
	if (m_readFilter)
		cg::Delete(__FUNCSTR__,m_readFilter);
	m_readFilter = newFilter;
}

void SocketRW::SetWriterFilter(cg::Filter * newFilter)
{
	if (m_writeFilter)
		cg::Delete(__FUNCSTR__,m_writeFilter);
	m_writeFilter = newFilter;
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
	if (!((const Socket*)m_socket))
	{
		LogError("There is not set socket. This object was moved most ",
			"likely.");
		throw NetworkException(Error::BadSocket);
	}
}


}
}
