#pragma once

#include "SocketRW.hpp"
#include "../Timer.hpp"
#include "../LockBox.hpp"

namespace cg {
namespace net {

class IServerMT
{
public:
	/**The type of list for the clients.*/
	using ClientList = std::list<cg::net::Socket>;
	/**The type of list for the clients.*/
	using ReadyList = std::list<cg::net::Socket*>;
	/**Default construct
	\param dataThreads The amount of threads that will receive and process
	data from the clients.*/
	IServerMT(int dataThreads = 5);
	/**Virt decon*/
	~IServerMT();
	/**Start the server.
	\param port The port to start on.*/
	void Start(uint16_t port);
	/**Stop the server and wait for all the threads to stop.*/
	void Stop();
	/**Wait untill the server stops on its own.*/
	void Wait();
protected:
	/**Loop through the active sockets and process data.*/
	void DataLoop();
	/**Process a ready socket.
	\param sock The socket that was reported ready*/
	virtual void ProcessSocket(cg::net::Socket& sock) = 0;
	/**Close all open threads with grace.*/
	void CloseAll();
	/**Wait for all the threads to finish before returning.*/
	void WaitForThreads();
	/**The locked access box for the clients list.*/
	cg::LockBox<std::list<cg::net::Socket>> m_clientBox;
	/**The locked access box for the ready list.*/
	cg::LockBox<std::list<cg::net::Socket*>> m_readyBox;
	/**Scan the client list and look for clients that are ready to be read 
	from.*/
	void ScanForReady();
	/**Continuiously try to accept a new socket.*/
	void AutoAccept();
	/**Do somthing on a socket right after it gets accepted.
	\param sock The socket that was just accepted.*/
	virtual void SocketAccepted(cg::net::Socket& sock) = 0;
	/**Do stuff that should happen when the socket is just about to be closed.
	\param sock The socket that is to be closed off.
	\param grace True if the socket was closed properly.*/
	virtual void SocketClosed(cg::net::Socket& sock,
		bool grace) = 0;
private:
	/**The loop that will run to scan for clients.*/
	void ScanForReadyLoop();
	/**The loop for checking for ready sockets.*/
	void AcceptLoop();

	/**The initial server socket.*/
	cg::net::Socket m_serverSocket;
	/**The master loop controller.*/
	std::atomic_bool m_run;
	/**The port for which to start on.*/
	uint16_t m_port;
	/**The thread that will scan for clients that are ready.*/
	std::thread* mt_readyScanner = nullptr;
	/**The thread that will auto accept connections.*/
	std::thread* mt_acceptScanner = nullptr;
	/**True if the accept thread is notrunning.*/
	bool m_acceptScannerStopped = true;
	/**True if the ready thread has stopped.*/
	bool m_readyScannerStopped = true;
	/**The amount of threads for processing data.*/
	int m_dataThreadCount;
	/**The threads for data processing.*/
	std::thread** m_dataThreads;
	/**The amount of active data threads*/
	int m_activeDataThreads = 0;
};

}
}