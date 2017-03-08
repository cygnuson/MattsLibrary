

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
	/**Covnert data from one place and store to another.  Dest and source may
	be the same, but may not overlap if they are different pointers.
	\param src The place to read the data from.
	\param size The size of the data destination.
	\return An array view with the converted data.*/
	virtual ArrayView Transform(const char* src, std::size_t size) override
	{
		auto ret = cg::SecureHelpers::RSADecrypt(src, size, m_key);
		ArrayView av(ret.size());
		std::memcpy(av.data(), ret.data(), ret.size());
		return av;
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
	/**Covnert data from one place and store to another.  Dest and source may
	be the same, but may not overlap if they are different pointers.
	\param src The place to read the data from.
	\param size The size of the data destination.
	\return An array view with the converted data.*/
	virtual ArrayView Transform(const char* src, std::size_t size) override
	{
		auto ret = cg::SecureHelpers::RSAEncrypt(src, size, m_key);
		ArrayView av(ret.size());
		std::memcpy(av.data(), ret.data(), ret.size());
		return av;
	}
private:
	/**A pointer to the key. might be public, might be a private key.*/
	CryptoPP::RSA::PublicKey m_key;
};

}