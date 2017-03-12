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
	/*disable cout temproarily so that it will be exclusive to this message.*/
	auto coutBuf = std::cout.rdbuf();
	std::stringstream ss;
	std::ostream& out = ss;
	std::cout.rdbuf(ss.rdbuf());
	out.rdbuf(coutBuf);
	auto color = m_colorMap[msg.m_level];
	ColorStart(color.first, color.second);
	out << msg.m_text << std::endl;
	ColorEnd();
	/*put cout back.*/
	std::cout.rdbuf(coutBuf);
	/*write anything that was intented for cout while it was disabled.*/
	std::cout << ss.str();
}

void NetLogServer::RegisterColor(cg::Logger::Level level, 
	unsigned int fore, 
	unsigned int back)
{
	m_colorMap[level] = std::make_pair(fore, back);
}

void NetLogServer::ColorStart(unsigned int fore, unsigned int back)
{
#ifdef _WIN32
	unsigned int col = fore | back;
	if (!SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), col))
	{
		/*could not set*/
	}
#else

#endif
}

void NetLogServer::ColorEnd()
{
#ifdef _WIN32
	unsigned int col = (unsigned int) colors::BackgroundColor::Black |
		(unsigned int) colors::ForegroundColor::White;
	if (!SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), col))
	{
		int stop = 0;
		/*could not set*/
	}
#else

#endif
}

}



