#pragma once

#include <map>

#include "IServerMT.hpp"
#include "../Serial.hpp"
#include "../crypto/AESFilter.hpp"
#include "../crypto/RSAExchange.hpp"
#include "../crypto/RSAFilter.hpp"

#define _DEBUGISECSERVERMT _DEBUG && 1

namespace cg {
namespace net {

/**A pair of socket pointer and SocketRW.  Both pointers need to be delted by
the developer.*/
struct SocketPair
{
	/**A pointer to a socket that was created.*/
	cg::net::Socket* m_socket = nullptr;
	/**A pointer to a SocketRW that is istup properly to send data to a
	ISecSocketMT server.*/
	cg::net::SocketRW* m_rw = nullptr;
	/**Qucik delete this pair.*/
	void Delete() {
		if (m_socket)
			cg::Delete(__FUNCSTR__,m_socket);
		if (m_rw)
			cg::Delete(__FUNCSTR__,m_rw);
	}
};

class ClientDescriptor
{
public:
	/**The socket for which this client is connected.*/
	cg::net::Socket* socket;
	/**The client aes key.*/
	CryptoPP::SecByteBlock m_aesKey;
	/**The client aes IV*/
	CryptoPP::SecByteBlock m_aesIv;
	/**The RSA public key from the client.*/
	CryptoPP::RSA::PublicKey m_rsaPub;
};
/**Create a secure server. Clients are expected to immediatly send an RSA
public key and then an AES key.*/
class ISecServerMT : public IServerMT
{
public:
	/**Destroy*/
	virtual ~ISecServerMT();
	/**Do something after a socket is excepted.  Will run after the security
	protocols have run on top of the IServerMT.
	\param sock The socket that was excepted.*/
	virtual bool SecSocketAccepted(cg::net::Socket& sock) = 0;
	/**Do stuff on a socket that has data ready. Data should be accessed via a
	SocketRW from .GetSocketRW(sock).
	\param sock The socket that has data ready.*/
	virtual bool SecProcessSocket(cg::net::Socket& sock) = 0;
	/**Do stuff when a socket is about to close.  Will run after the security
	protocols are done.
	\param sock The socket that is about to close.
	\param grace will be true if the socket is about to close gracefully.*/
	virtual void SecSocketClosed(cg::net::Socket& sock,
		bool grace) = 0;
	/**Get a socket RW that is setup with the proper keys for the client in
	order to receive data from it.
	\return A SocketRW that will be able to receive encrypted data from the
	client.*/
	cg::net::SocketRW GetSocketRW(cg::net::Socket& sock);
	/**Get a connected socket and writer.
	\return A socket pointer and SocketRW setup to send and receive data from
	this server.*/
	static SocketPair GetConnected(const std::string& address, uint16_t port);
private:
	/**The type of map used for the client descriptor.*/
	using CDMap = std::map<std::size_t, ClientDescriptor>;
	/**Do somthing on a socket right after it gets accepted.
	\param sock The socket that was just accepted.
	\return True if the socket should stay in the list. False if it should be
	closed and removed.*/
	bool SocketAccepted(cg::net::Socket& sock);
	/**Process a ready socket.
	\param sock The socket that was reported ready
	\return True if the socket should stay active, false if it should be
	closed and removed.*/
	bool ProcessSocket(cg::net::Socket& sock);
	/**Do stuff that should happen when the socket is just about to be closed.
	\param sock The socket that is to be closed off.
	\param grace True if the socket was closed properly.*/
	void SocketClosed(cg::net::Socket& sock,
		bool grace);
	/**Get a socket from the client decriptor list.
	\param sock The socket for which the id will be retreived.
	\return The client descriptor matching the socket.*/
	const cg::net::ClientDescriptor&
		GetClientDescriptor(const cg::net::Socket& sock);

	/**A list of client descriptors.*/
	cg::LockBox<CDMap> m_secClients;
};

}
}