
#include "IServer.hpp"

namespace cg {
namespace net {



IServer::~IServer()
{
	Wait();
}

void IServer::Start(uint16_t port, double updateSpeed)
{
	if (m_run)
		Stop();
	m_updateSpeed = updateSpeed;
	m_port = port;
	m_serverSocket.Bind(true, m_port);
	m_serverSocket.Listen();
	m_run = true;
	ServerLoop();
}

void IServer::Stop()
{
	m_run = false;
	Wait();
}

void IServer::Wait() const
{
	while (m_run)
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void IServer::AcceptAll()
{
	while (m_serverSocket.ReadReady())
	{
		m_updateSpeed();
		auto it = m_clients.emplace(m_clients.end(), false);
		auto& sock = m_clients.back();
		bool accepted = m_serverSocket.Accept(sock);
		if (!accepted)
			m_clients.pop_back();
		else
			HandleAccept(it);
	}
}

void IServer::ServerLoop()
{
	while (m_run)
	{
		m_updateSpeed();
		AcceptAll();
		CheckClients();
	}
}

void IServer::CheckClients()
{
	if (m_clients.size() == 0)
		return;
	auto it = m_clients.begin();
	auto end = m_clients.end();
	for (; it != end; ++it)
	{
		m_updateSpeed();
		/*if its close, erase it and skip it.*/
		if (!it->IsOpen())
		{
			SocketRemoved(false, it);
			m_clients.erase(it);
			if (m_clients.size() == 0)
				break;
			continue;
		}
		while (it->ReadReady())
		{
			if (!HandleData(it))
			{
				SocketRemoved(true, it);
				m_clients.erase(it);
				break;
			}
		}
		/*if the last one was removed, stop looping. The for loop will cause an
		exception otherwise.*/
		if (m_clients.size() == 0)
			break;
	}
}

}
}