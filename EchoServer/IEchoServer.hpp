#pragma once

#include <list>
#include <string>
#include <thread>

#include "../net/SocketRW.hpp"

namespace cg {
/**Data sent to this server must have 8 bytes sent before each message which 
will be the size of the following message. Using cg::net::SocketRW to write
to the server will do this automatically.*/
class IEchoServer
{
public:
	/**Write the incomming data to somewhere.  The implimentor is responsible
	managing the the output (stream/locks/etc...)
	\param data The string of data to print.*/
	virtual void Print(cg::ArrayView& data) = 0;
	/**Start reading and echoing data.
	\param port The port to listen on.*/
	virtual void Start(uint16_t port);
	/**Virtual dtor*/
	virtual ~IEchoServer();
private:
	/**The socket writer to listen for connections.*/
	cg::net::Socket m_serverSocket;
	/**A list of clients.*/
	std::list<cg::net::Socket> m_clients;
	/**The thread that will print out data.*/
	std::thread* m_runThread = nullptr;
	/**True to keep running.*/
	std::atomic_bool m_run = false;
};

}