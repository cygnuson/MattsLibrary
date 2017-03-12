#include "NetLogServer.hpp"

namespace cg {




void NetLogServer::Start(uint16_t port)
{
	m_serverSocket.Bind(true, port);
	m_serverSocket.Listen();
	m_run = true;
	cg::SpeedLimit limit(30);
	while (m_run)
	{
		limit();
		while (m_serverSocket.ReadReady())
		{
			m_clients.emplace_back(false);
			auto& sock = m_clients.back();
			bool accepted = m_serverSocket.Accept(sock);
			if (!accepted)
			{
				m_clients.pop_back();
				cg::Logger::LogNote(3, "Socket ", sock.Id(),
					" has disconnected.");
			}
		}
		if (m_clients.size() > 0)
		{
			auto it = m_clients.begin();
			auto end = m_clients.end();
			for (; it != end; ++it)
			{
				/*if its close, erase it and skip it.*/
				if (!it->IsOpen())
				{
					m_clients.erase(it);
					if (m_clients.size() == 0)
						break;
					continue;
				}
				while (it->ReadReady())
				{
					cg::net::SocketRW reader(std::addressof(*it));
					auto av = reader.Read();
					PrintMsg(av);
				}
			}
		}
	}
}

void NetLogServer::PrintMsg(const cg::ArrayView & av)
{
	NetLoggerMessage msg(av);
	if (msg.m_text == "STOPDEBUG")
	{
		m_run = false;
		return;
	}
	std::stringstream ss;
	ss << "[Thread:" << std::setw(6) << std::setfill(' ')
		<< msg.m_threadId << "]" << std::flush;
	cg::Logger::SupressNextThreadOutput();
	cg::Logger::SupressNextTimeOutput();
	cg::Logger::SupressNextLocalOutput();
	cg::Logger::Log(msg.m_level, "[", msg.m_name, "]", ss.str(),
		msg.m_time,msg.m_text);
}

}



