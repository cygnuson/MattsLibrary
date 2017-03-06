#include "NetworkObject.hpp"

namespace cg {
namespace net {


std::size_t NetworkObject::ms_socketsActive = 0;

NetworkObject::NetworkObject()
{
	if (ms_socketsActive++ == 0)
	{
		StartSockets();
	}
}

NetworkObject::~NetworkObject()
{
	if (--ms_socketsActive == 0)
	{
		StopSockets();
	}
}

void NetworkObject::StartSockets()
{
	cg::Logger::LogNote(3, "Network: Starting Sockets.");
	bool success = OSStart();
	cg::Logger::LogError(!success,__FUNCSTR__, "Sockets not started.");
}

void NetworkObject::StopSockets()
{
	cg::Logger::LogNote(3, "Network: Stopping Sockets.");
	OSStop();
}


bool NetworkObject::OSStart()
{
#ifdef _WIN32
	WSAData data = {};
	int error = WSAStartup(MAKEWORD(2, 2), &data);
	if (error != 0)
	{
		cg::Logger::LogError(__FUNCSTR__, "The WSA system could not start.",
			" Probably a windows issue with WSStartup.");
		return false;
	}
	else
		return true;
#endif

}

void NetworkObject::OSStop()
{
#ifdef _WIN32
	WSACleanup();
#endif
}

}
}