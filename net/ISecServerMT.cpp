#include "ISecServerMT.hpp"

namespace cg {
namespace net {
ISecServerMT::~ISecServerMT()
{

}
SocketRW ISecServerMT::GetSocketRW(Socket & sock)
{
	auto& cd = m_secClients.Writer()->at(sock.Id());
	SocketRW rw(&sock,
		cg::New<AESEncryptFilter>(__FUNCSTR__, cd.m_aesKey, cd.m_aesIv),
		cg::New<AESDecryptFilter>(__FUNCSTR__, cd.m_aesKey, cd.m_aesIv));
	return rw;
}

SocketPair
ISecServerMT::GetConnected(const std::string & address, uint16_t port)
{
	SocketPair sp;
	sp.m_socket = cg::New<Socket>(__FUNCSTR__);
	auto& sock = *sp.m_socket;
	sock.Connect("::1", 55899);
	sp.m_rw = cg::New<SocketRW>(__FUNCSTR__, &sock);
	auto& rw = *sp.m_rw;
	auto keys = cg::SecureHelpers::MakeRSAKeys();
	cg::SendPublicKey(sock, keys.m_public);
	rw.SetReaderFilter(cg::New<RSADecryptFilter>(__FUNCSTR__, keys.m_private));
	auto key = rw.Read();
	cg::Logger::LogWarn(3, __FUNCSTR__, "Got the aes key.");
	CryptoPP::SecByteBlock keyBlock((const byte*)key.data(), key.size());
	auto iv = rw.Read();
	cg::Logger::LogWarn(3, __FUNCSTR__, "Got the aes iv.");
	CryptoPP::SecByteBlock ivBlock((const byte*)iv.data(), iv.size());
	rw.SetReaderFilter(cg::New<AESDecryptFilter>(__FUNCSTR__, keyBlock,
		ivBlock));
	rw.SetWriterFilter(cg::New<AESEncryptFilter>(__FUNCSTR__, keyBlock,
		ivBlock));
#if _DEBUGISECSERVERMT
	cg::Serial serial;
	serial << std::string("Test!");
	auto X = serial.GetArrayView();
	rw.Write(X.data(), X.size());
#endif
	return sp;
}

bool ISecServerMT::SocketAccepted(Socket & sock)
{
	SocketRW rw(&sock);
	auto cdl = m_secClients.Writer();
	ClientDescriptor& cd = cdl->operator[](sock.Id());
	cg::GetPublicKey(sock, cd.m_rsaPub);
	cg::Logger::LogNote(3, __FUNCSTR__, "Got the public key.");
	rw.SetWriterFilter(cg::New<RSAEncryptFilter>(__FUNCSTR__, cd.m_rsaPub));
	cd.m_aesKey = cg::SecureHelpers::MakeAESKey();
	cd.m_aesIv = cg::SecureHelpers::MakeAESIv();
	auto& aesKey = cd.m_aesKey;
	auto& aesIv = cd.m_aesIv;
	rw.Write((char*)aesKey.data(), aesKey.size());
	cg::Logger::LogNote(3, __FUNCSTR__, "sent the aes key.");
	rw.Write((char*)aesIv.data(), aesIv.size());
	cg::Logger::LogNote(3, __FUNCSTR__, "sent the aes iv.");
	return SecSocketAccepted(sock);
}

bool ISecServerMT::ProcessSocket(Socket & sock)
{
	return SecProcessSocket(sock);
}

void ISecServerMT::SocketClosed(Socket & sock,
	bool grace)
{
	m_secClients.Writer()->erase(sock.Id());
	return SecSocketClosed(sock, grace);
}

const ClientDescriptor &
ISecServerMT::GetClientDescriptor(const Socket& sock)
{
	return m_secClients.Writer()->at(sock.Id());
}

}
}