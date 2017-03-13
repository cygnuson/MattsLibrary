#pragma once

#include "SocketRW.hpp"
#include "../Timer.hpp"
#include "../LockBox.hpp"
#include "../Memory.hpp"

namespace cg {
namespace net {

class IServerMT : public NetworkObject
{
public:
	/**The type of list for the clients.*/
	using ClientList = std::list<cg::net::Socket*>;
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
	/**Change the speed of the acceptor loop (default 100 FPS).
	\param fps The FPS of the loop.*/
	void ChangeAcceptorSpeed(double fps);
	/**Change the speed of the ready-check loop (default 100 FPS).
	\param fps The FPS of the loop.*/
	void ChangeScannerSpeed(double fps);
	/**Loop through the active sockets and process data.*/
	void DataLoop();
	/**Close all open threads with grace.*/
	void CloseAll();
	/**Wait for all the threads to finish before returning.*/
	void WaitForThreads();
	/**Scan the client list and look for clients that are ready to be read 
	from.*/
	void ScanForReady();
	/**Continuiously try to accept a new socket.*/
	void AutoAccept();
	/**Do somthing on a socket right after it gets accepted.
	\param sock The socket that was just accepted.
	\return True if the socket should stay in the list. False if it should be
	closed and removed.*/
	virtual bool SocketAccepted(cg::net::Socket& sock) = 0;
	/**Process a ready socket.
	\param sock The socket that was reported ready
	\return True if the socket should stay active, false if it should be
	closed and removed.*/
	virtual bool ProcessSocket(cg::net::Socket& sock) = 0;
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

	/**The speed limit of the acceptor loop. 100 is usually good unless it 
	needs to be faster (in FPS).*/
	cg::SpeedLimit m_acceptorLimit;
	/**The speed of the ready scanning loop. 100 is usually good unless it must
	be faster (in FPS).*/
	cg::SpeedLimit m_readyLimit;
	/**The locked access box for the clients list.*/
	cg::LockBox<ClientList> m_clientBox;
	/**The locked access box for the ready list.*/
	cg::LockBox<ClientList> m_readyBox;
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