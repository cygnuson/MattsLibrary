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
	/**Set a logger level to have acertian color.
	\param fore The foreground color.
	\param back The background color.*/
	void RegisterColor(cg::Logger::Level level,
		unsigned int fore,
		unsigned int back);
private:
	/**Set the output color (linux or windows)
	\param fore The foreground color.
	\param back The background color.*/
	void ColorStart(unsigned int fore, unsigned  int back);
	/**Set the color back to normal.*/
	void ColorEnd();
	/**The socket to listen for connections on.*/
	cg::net::Socket m_serverSocket;
	/**True to keep running.*/
	bool m_run = false;
	/**A list of clients.*/
	std::list<cg::net::Socket> m_clients;
	/**Color map.  The pair is foreground(first) and background(second)*/
	std::map<cg::Logger::Level, std::pair<int,int>> m_colorMap;
};

}


