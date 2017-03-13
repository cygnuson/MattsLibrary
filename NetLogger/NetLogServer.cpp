#include "NetLogServer.hpp"

namespace cg {

void NetLogServer::PrintMsg(const NetLoggerMessage& msg)
{
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
		msg.m_time, msg.m_text);
}

bool NetLogServer::HandleAccept(ClientList::iterator sock)
{
	cg::Logger::LogNote(3, "Accepted a client.");
	return true;
}

bool NetLogServer::HandleData(ClientList::iterator sock)
{
	cg::net::SocketRW reader(std::addressof(*sock));
	auto av = reader.Read();
	NetLoggerMessage msg(av);
	if (msg.m_text == "STOPDEBUG")
		return false;
	PrintMsg(msg);
	return true;
}

void NetLogServer::SocketRemoved(bool grace,
	ClientList::iterator sock)
{
	cg::Logger::LogNote(3, "Closed a client. Gracefully? ",
		grace ? "Yes" : "No");
}

}



