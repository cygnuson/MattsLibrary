

#include "SecureHelpers.hpp"
#include "../Filter.hpp"
#include "../Logger.hpp"

namespace cg {

class RSADecryptFilter : public cg::Filter
{
public:
	/**Create the filter.

	\param key The private key.*/
	RSADecryptFilter(CryptoPP::RSA::PrivateKey key)
		:m_key(key) {};
	/**Create the filter.

	\param keys The pair of keys.*/
	RSADecryptFilter(cg::KeyPair keys)
		:m_key(keys.m_private) {};
	/**Transform some data.
	
	\param dest The place to put the transformed data.
	\param src The place to read data.
	\param dSize The size of the destination.
	\param sSize The size of the source data.*/
	inline virtual void Transform(char * dest,
		const char * src,
		std::size_t dSize,
		std::size_t sSize) override
	{
		auto ret = cg::SecureHelpers::RSADecrypt(src, sSize, m_key);
		if (dSize < ret.size())
			cg::Logger::LogError("The desination size is too small.");

		std::memcpy(dest, ret.data(), ret.size());
	}
private:
	/**A pointer to the key. might be public, might be a private key.*/
	CryptoPP::RSA::PrivateKey m_key;

};

class RSAEncryptFilter : public cg::Filter
{
public:
	/**Create the filter.

	\param key The public key.*/
	RSAEncryptFilter(CryptoPP::RSA::PublicKey key)
		:m_key(key) {};
	/**Create the filter.

	\param keys The pair of keys.*/
	RSAEncryptFilter(cg::KeyPair keys)
		:m_key(keys.m_public) {};
	/**Transform some data.

	\param dest The place to put the transformed data.
	\param src The place to read data.
	\param dSize The size of the destination.
	\param sSize The size of the source data.*/
	inline virtual void Transform(char * dest,
		const char * src,
		std::size_t dSize,
		std::size_t sSize) override
	{
		auto ret = cg::SecureHelpers::RSAEncrypt(src, sSize, m_key);
		if (dSize < ret.size())
			cg::Logger::LogError("The desination size is too small.");

		std::memcpy(dest, ret.data(), ret.size());
	}
private:
	/**A pointer to the key. might be public, might be a private key.*/
	CryptoPP::RSA::PublicKey m_key;
};

}