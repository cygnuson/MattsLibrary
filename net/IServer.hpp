#pragma once

#include <list>

#include "Socket.hpp"
#include "../Timer.hpp"

namespace cg {
namespace net {

class IServer
{
public:
	/**The virt decon*/
	virtual ~IServer();
	/**Start the server.
	\param port The port to connect to.
	\param updateSpeed The amount of times the loop will be run per sec.*/
	virtual void Start(uint16_t port, double updateSpeed = 30);
	/**Stop the server.*/
	virtual void Stop();
	/***/
protected:
	/**The type of list for holding clients.*/
	using ClientList = std::list<cg::net::Socket>;
	/**Wait for the server to finish what its doing.*/
	virtual void Wait() const;
	/**Accept any sockets that are available to accept.*/
	virtual void AcceptAll();
	/**Basic server loop.*/
	virtual void ServerLoop();
	/**Do somthing with a ready socket.
	\param sock The ready socket.
	\return True if the socket should stay in the list. False if it should be
	closed and removed.*/
	virtual bool HandleData(ClientList::iterator sock) = 0;
	/**Will be called on each socket as soon as it is accepted.
	\param sock A iterator to the socket that was accepted.*/
	virtual void HandleAccept(ClientList::iterator sock) = 0;
	/**This function will be called right before a socket is removed from the
	client list.
	\param grace True if the socket closed properly.
	\param sock A iterator to the socket that is about to close.*/
	virtual void SocketRemoved(bool grace, ClientList::iterator sock) = 0;
	/**Iterate all sockets and call HandleData on each one that is ready.*/
	virtual void CheckClients();
	/**The server socket.*/
	cg::net::Socket m_serverSocket;
	/**The port to connect to.*/
	uint16_t m_port;
	/**True to run the server components*/
	bool m_run = false;
	/**The client list.*/
	ClientList m_clients;
	/**The update speed of the server.*/
	cg::SpeedLimit m_updateSpeed;
};

}
}


