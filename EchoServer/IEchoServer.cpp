#include "IEchoServer.hpp"

void cg::IEchoServer::Start(uint16_t port)
{
	m_serverSocket.Bind(true, port);
	m_serverSocket.Listen(1000);
	m_run = true;
	while (m_run)
	{
		if (m_serverSocket.ReadReady())
		{
			m_clients.emplace_back(true);
			if (!m_serverSocket.Accept(m_clients.back(), true))
			{
				/*the client was not eccepted for some reason. get rid
				of it*/
				m_clients.back().Unlock();
				m_clients.pop_back();
			}
			else
				/*unlock so it can be used.*/
				m_clients.back().Unlock();
		}
		auto it = m_clients.begin();
		auto end = m_clients.end();
		for (; it != end; ++it)
		{
			auto& sock = *it;
			if (!sock.IsOpen())
			{
				m_clients.erase(it);
				break;
			}
			if (sock.ReadReady())
			{
				cg::net::SocketRW reader(&sock);
				auto received = reader.Read();
				Print(received);
			}
		}
	}

	auto it = m_clients.begin();
	auto end = m_clients.end();
	for (; it != end; ++it)
	{
		it->Close();
	}
	m_serverSocket.Close();
}

cg::IEchoServer::~IEchoServer()
{
	if (m_runThread)
		if (m_runThread->joinable())
			m_runThread->join();
}
