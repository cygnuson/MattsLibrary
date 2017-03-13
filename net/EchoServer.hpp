#pragma once

#include <list>
#include <string>
#include <thread>
#include <sstream>

#include "SocketRW.hpp"
#include "../Timer.hpp"
#include "../Memory.hpp"

namespace cg {
/**Data sent to this server must have 8 bytes sent before each message which 
will be the size of the following message. Using cg::net::SocketRW to write
to the server will do this automatically.*/
class EchoServer
{
public:
	/**Write the incomming data to somewhere.  The implimentor is responsible
	managing the the output (stream/locks/etc...)
	\param data The string of data to print.
	\param sock The socket that sent the data.*/
	virtual void Print(cg::ArrayView& data, cg::net::Socket& sock);
	/**Start reading and echoing data.
	\param port The port to listen on.*/
	virtual void Start(uint16_t port);
	/**Start reading and echoing data.
	\param port The port to listen on.*/
	virtual void AsyncStart(uint16_t port);
	/**Virtual dtor*/
	virtual ~EchoServer();
	/**Stop the server*/
	virtual void Stop();
	/**wait for the server to stop.*/
	virtual void Wait();
private:
	/**The thread for running in async mode.*/
	std::thread* m_asyncThread;
	/**The socket writer to listen for connections.*/
	cg::net::Socket m_serverSocket;
	/**A list of clients.*/
	std::list<cg::net::Socket> m_clients;
	/**True to keep running.*/
	std::atomic_bool m_run = false;
};

class EchoClient
{
public:
	/**Create the echo client.
	\param speed The speed of the syncing in fps.*/
	EchoClient(double fps = 30);
	/**Start the syncing the stream to the echo server.
	\param port The port to connect to.
	\param address The address to connect to.*/
	void Start(uint16_t port, const std::string& address);
	/**Start in async mode.	
	\param port The port to connect to.
	\param address The address to connect to.*/
	void AsyncStart(uint16_t port, const std::string& address);
	/**Stop the server.*/
	void Stop();
	/**Get the stream from the echo client.
	\return The internal stream that whill sync.*/
	std::ostream& Stream();
	/**~DTOR*/
	virtual ~EchoClient();
	/**wait for the server to stop.*/
	virtual void Wait();
private:
	/**The thread for running in async mode.*/
	std::thread* m_asyncThread;
	/**The speed of the syncing in fps.*/
	double m_fps;
	/**The stream to watch*/
	std::stringstream m_stream;
	/**The socket that connects.*/
	cg::net::Socket m_socket;
	/**the run flag*/
	bool m_run = false;
};

}