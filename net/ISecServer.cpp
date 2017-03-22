#include "ISecServer.hpp"

cg::net::SocketRW cg::net::ISecServerMT::GetSocketRW(cg::net::Socket & sock)
{
	auto& cd = m_secClients[sock.Id()];
	cg::net::SocketRW rw(&sock,
		new cg::AESEncryptFilter(cd.m_aesKey, cd.m_aesIv),
		new cg::AESDecryptFilter(cd.m_aesKey, cd.m_aesIv));
	return rw;
}

bool cg::net::ISecServerMT::SocketAccepted(cg::net::Socket & sock)
{
	cg::net::SocketRW rw(&sock);
	cg::net::ClientDescriptor& cd = m_secClients[sock.Id()];
	cg::GetPublicKey(sock, cd.m_rsaPub);
	rw.SetWriterFilter(new RSAEncryptFilter(cd.m_rsaPub));
	cd.m_aesKey = cg::SecureHelpers::MakeAESKey();
	cd.m_aesIv = cg::SecureHelpers::MakeAESIv();
	auto& aesKey = cd.m_aesKey;
	auto& aesIv = cd.m_aesIv;
	rw.Write((char*)aesKey.data(), aesKey.size());
	rw.Write((char*)aesIv.data(), aesIv.size());
	return SecSocketAccepted(sock);
}

bool cg::net::ISecServerMT::ProcessSocket(cg::net::Socket & sock)
{
	return SecProcessSocket(sock);
}

void cg::net::ISecServerMT::SocketClosed(cg::net::Socket & sock,
	bool grace)
{
	return SecSocketClosed(sock, grace);
}

const cg::net::ClientDescriptor & 
cg::net::ISecServerMT::GetClientDescriptor(const cg::net::Socket& sock)
{
	return m_secClients[sock.Id()];
}
