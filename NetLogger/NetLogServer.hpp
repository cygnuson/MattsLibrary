#pragma once

#include <list>
#include <map>
#include <sstream>

#include "NetLoggerMessage.hpp"
#include "../net/SocketRW.hpp"
#include "../Timer.hpp"
#include "../Colors.hpp"

namespace cg {


/**If the message "STOPDEBUG" is received, then the server will shutdown.*/
class NetLogServer
{
public:
	/**Create a net logget with no initial values.*/
	NetLogServer() {};
	/**Start listening for net logger messages.
	\param port The port to listen on.*/
	void Start(uint16_t port);
	/**Print a msg.
	\param av The data to print as array view. Will be deserialized to a
	NetLoggerMessage.*/
	void PrintMsg(const cg::ArrayView& av);
private:
	/**The socket to listen for connections on.*/
	cg::net::Socket m_serverSocket;
	/**True to keep running.*/
	bool m_run = false;
	/**A list of clients.*/
	std::list<cg::net::Socket> m_clients;
};

}


