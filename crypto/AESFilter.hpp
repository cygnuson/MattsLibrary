#pragma once
#include "SecureHelpers.hpp"
#include "../Filter.hpp"

namespace cg {

class AESEncryptFilter : public cg::Filter
{
public:
	AESEncryptFilter(const CryptoPP::SecByteBlock& key,
		const CryptoPP::SecByteBlock& iv)
		:m_key(key), m_iv(iv) {}
	/**Transform data in place (no copies).
	\param data The data place.
	\param size The data size.*/
	virtual void Transform(char* data, std::size_t size)
	{
		cg::SecureHelpers::AESEncrypt(data, data, size, size, m_key, m_iv);
	}
private:
	/**The key used.*/
	const CryptoPP::SecByteBlock& m_key;
	/**The IV used.*/
	const CryptoPP::SecByteBlock& m_iv;
};

class AESDecryptFilter : public cg::Filter
{
public:
	AESDecryptFilter(const CryptoPP::SecByteBlock& key,
		const CryptoPP::SecByteBlock& iv)
		:m_key(key), m_iv(iv) {}
	/**Transform data in place (no copies).
	\param data The data place.
	\param size The data size.*/
	virtual void Transform(char* data, std::size_t size)
	{
		cg::SecureHelpers::AESDecrypt(data, data, size, size, m_key, m_iv);
	}
private:
	/**The key used.*/
	const CryptoPP::SecByteBlock& m_key;
	/**The IV used.*/
	const CryptoPP::SecByteBlock& m_iv;

};


}