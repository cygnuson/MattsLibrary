#pragma once

#include "../net/SocketRW.hpp"
#include "SecureHelpers.hpp"

namespace cg {
/**Send a public key through a socket.
\param socket The socket to send through.
\param key The key to send.*/
inline void SendPublicKey(cg::net::Socket& socket,
	const CryptoPP::RSA::PublicKey& key)
{
	cg::net::SocketRW socketWriter(&socket);
	CryptoPP::ByteQueue keyBytes;
	key.Save(keyBytes);
	char data[1024];
	CryptoPP::ArraySink as((byte*)data, 1024);
	uint64_t size = (uint64_t)keyBytes.CopyTo(as);
	socketWriter.Write(data, size, -1);
	cg::Logger::LogNote(3, "Sent RSA public key (", size, " bytes) to socket ",
		socket.Id(), ". ");
}
/**Wait for a public key from a socket.
\param socket The socket to wait on.
\param dest The place to put the key.
\return Ture if the key was recevied and validated, false if the key in
invalid.*/
inline bool GetPublicKey(cg::net::Socket& socket,
	CryptoPP::RSA::PublicKey& dest)
{
	cg::net::SocketRW reader(&socket);
	auto av = reader.Read();
	CryptoPP::ArraySource as((byte*)av.data(), av.size(), true);
	dest.Load(as);
	cg::Logger::LogNote(3, "Got the RSA public key (", av.size(), 
		" bytes) from socket ", socket.Id(), ".");
	bool validated = dest.Validate(CryptoPP::RandomNumberGenerator(), 3);
	if (validated)
	{
		cg::Logger::LogNote(3, "The RSA Key was validated.");
		return true;
	}

	cg::Logger::LogError("The RSA Key was ~NOT~ validated.");
	return false;

}

}