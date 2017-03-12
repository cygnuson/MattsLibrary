#pragma once

#include <list>
#include <map>
#include <sstream>

#include "NetLoggerMessage.hpp"
#include "../net/SocketRW.hpp"
#include "../net/IServer.hpp"
#include "../Timer.hpp"
#include "../Colors.hpp"

namespace cg {


/**If the message "STOPDEBUG" is received, then the server will shutdown.*/
class NetLogServer : public cg::net::IServer
{
public:
	/**Create a net logget with no initial values.*/
	NetLogServer() {};
	/**Print a msg.
	\param av The data to print as NetLoggerMessage. Will be deserialized to a
	NetLoggerMessage.*/
	void PrintMsg(const NetLoggerMessage& av);
private:
	/**Do somthing with a ready socket.
	\param sock The ready socket.
	\return True if the socket should stay in the list. False if it should be
	closed and removed.*/
	virtual bool HandleData(ClientList::iterator sock)override;
	/**Will be called on each socket as soon as it is accepted.
	\param sock A iterator to the socket that was accepted.*/
	virtual void HandleAccept(ClientList::iterator sock)override;
	/**This function will be called right before a socket is removed from the
	client list.
	\param grace True if the socket closed properly.
	\param sock A iterator to the socket that is about to close.*/
	virtual void SocketRemoved(bool grace, ClientList::iterator sock)override;

};

}


