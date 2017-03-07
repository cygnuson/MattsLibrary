
#include "SecureHelpers.hpp"

namespace cg {
namespace sec {

class RSADecryptFilter
{
public:

private:
	/**A pointer to the key. might be public, might be a private key.*/
	CryptoPP::RSA::PrivateKey m_key;
};

class RSAEncryptFilter
{
public:

private:
	/**A pointer to the key. might be public, might be a private key.*/
	CryptoPP::RSA::PublicKey m_key;
};

}
}