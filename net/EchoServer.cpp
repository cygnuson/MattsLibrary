#include "EchoServer.hpp"

void cg::EchoServer::Print(cg::ArrayView & data, cg::net::Socket& sock)
{
	auto addr = sock.GetAddress();
	std::cout << "[" << addr.first << " ~ " << addr.second << "]: "
		<< data.data();
}

void cg::EchoServer::Start(uint16_t port)
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
				cg::Logger::LogNote(1, __FUNCSTR__, "The socket ", it->Id(),
					" will be closed.");
				m_clients.erase(it);
				break;
			}
			if (sock.ReadReady())
			{
				cg::net::SocketRW reader(&sock);
				auto received = reader.Read();
				/**if sie == 1, its just a null term*/
				if (received.size() > 1)
					Print(received, sock);
			}
		}
	}

	auto it = m_clients.begin();
	auto end = m_clients.end();
	for (; it != end; ++it)
	{
		if (it->IsOpen())
			it->Close();
	}
	m_serverSocket.Close();
}

void cg::EchoServer::AsyncStart(uint16_t port)
{
	m_asyncThread = new std::thread(&cg::EchoServer::Start, this, port);
}

cg::EchoServer::~EchoServer()
{
	Stop();
	if (m_asyncThread)
		if (m_asyncThread->joinable())
			m_asyncThread->join();
}

void cg::EchoServer::Stop()
{
	m_run = false;
}

void cg::EchoServer::Wait()
{
	while (m_run)
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

cg::EchoClient::EchoClient(double fps)
	: m_fps(fps)
{
	cg::Logger::AddLog("STDCOUTLOG", std::cout.rdbuf());
}

void cg::EchoClient::Start(uint16_t port, const std::string & address)
{
	m_run = true;
	try
	{
		m_socket.Connect(address, port);
	}
	catch (const cg::net::NetworkException& e)
	{
		auto oldLog = cg::Logger::ActiveLogName();
		cg::Logger::ChangeLog("STDCOUTLOG");
		cg::Logger::LogError(__FUNCSTR__, "The socket could not connect.",
			" Exception:", e.What());
		cg::Logger::ChangeLog(oldLog);
		std::cout << m_stream.rdbuf();
		return;
	}
	cg::net::SocketRW writer(&m_socket);
	cg::SpeedLimit sLimit(m_fps);
	while (m_run)
	{
		sLimit();
		if (!m_socket.IsOpen())
		{
			auto oldLog = cg::Logger::ActiveLogName();
			cg::Logger::ChangeLog("STDCOUTLOG");
			cg::Logger::LogError(__FUNCSTR__, "The socket is not open.");
			std::cout << m_stream.rdbuf();
			break;
		}
		if (!m_run)
			break;
		if (m_stream.good())
		{
			if (writer.WriteReady())
			{
				auto str = m_stream.str();
				m_stream.str("");
				writer.Write(str.data(), str.size() + 1);
			}
			else
			{
				auto oldLog = cg::Logger::ActiveLogName();
				cg::Logger::ChangeLog("STDCOUTLOG");
				cg::Logger::LogError(__FUNCSTR__, "The socket is not open.");
				std::cout << m_stream.rdbuf();
				cg::Logger::ChangeLog(oldLog);
			}
		}

	}
}

void cg::EchoClient::AsyncStart(uint16_t port, const std::string& address)
{
	m_asyncThread =
		new std::thread(&cg::EchoClient::Start, this, port, address);
}

void cg::EchoClient::Stop()
{
	m_run = false;
}

std::ostream & cg::EchoClient::Stream()
{
	return m_stream;
}

cg::EchoClient::~EchoClient()
{
	Stop();
	if (m_asyncThread)
		if (m_asyncThread->joinable())
			m_asyncThread->join();
}

void cg::EchoClient::Wait()
{
	while (m_run)
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	if (m_asyncThread)
		if (m_asyncThread->joinable())
			m_asyncThread->join();
}
