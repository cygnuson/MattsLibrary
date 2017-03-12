
#include "NetLog.hpp"

namespace cg {

uint16_t NetLog::ms_port;
std::string NetLog::ms_name;
bool NetLog::ms_isInit = false;
cg::net::Socket* NetLog::ms_client = nullptr;

void NetLog::Stop()
{
	if (!ms_client)
	{
		cg::Logger::LogError("The NetLogger is not setup.");
		return;
	}

	LogWarn("STOPDEBUG");
}

void NetLog::Init(const std::string & name,
	uint16_t port)
{
	ms_port = port;
	ms_name = name;
	ms_client = new cg::net::Socket();
	bool connected = false;
	try {
		connected = ms_client->Connect("::1", ms_port);
	}
	catch (const cg::Exception& e)
	{
		cg::Logger::LogError(__FUNCSTR__, "Could not connect to the debug",
			" server. Exception: ", e.What());
	}
	ms_isInit = true;
}

void NetLog::WriteMessage(const cg::NetLoggerMessage & msg)
{
	if (!ms_client)
	{
		cg::Logger::LogError("The NetLogger is not setup.");
		return;
	}
	for (int i = 0; i < 5; ++i)
	{
		try {
			if (ms_client->IsOpen())
				break;
			else
				ms_client->Connect("::1", ms_port);
		}
		catch (const cg::Exception& e)
		{
			cg::Logger::LogError("The NetLog could not connect. Exception:",
				e.What());
		}
	}
	if (!ms_client->IsOpen())
	{
		cg::Logger::LogError("The NetLog could not connect.");
		return;
	}
	auto serial = msg.Serialize();
	cg::net::SocketRW writer(ms_client);
	auto sent = writer.Write(serial.data(), serial.size());
	if (sent != serial.size())
		cg::Logger::LogError("The NetLog could not send all the data.");
}


}